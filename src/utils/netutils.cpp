/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2013
   Author(s): Christophe Grosjean

   Network related utility functions

*/

#include "utils/netutils.hpp"

#include "utils/log.hpp"
#include "utils/select.hpp"
#include "utils/sugar/int_to_chars.hpp"

#include <charconv>

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/un.h>
#include <netinet/in.h>

static_assert(sizeof(IpAddress::ip_addr) >= INET6_ADDRSTRLEN);

void AddrInfoDeleter::operator()(addrinfo *addr_info) noexcept
{
    freeaddrinfo(addr_info);
}

bool try_again(int errnum)
{
    int res = false;
    // TODO Check wich signals are actually necessary depending on what we are doing
    // looks like EINPROGRESS or EALREADY only occurs when calling connect()
    // EAGAIN is when blocking IO would block (other name for EWOULDBLOCK)
    // EINTR when an interruption stopped system call (and we could do it again)
    switch (errnum){
        case EAGAIN:
        /* case EWOULDBLOCK: */ // same as EAGAIN on Linux
        case EINPROGRESS:
        case EALREADY:
        case EBUSY:
        case EINTR:
            res = true;
            break;
        default:
            ;
    }
    return res;
}

namespace
{
    bool set_snd_buffer(int sck, int buffer_size)
    {
        /* set snd buffer to at least 32 Kbytes */
        int snd_buffer_size = buffer_size;
        socklen_t option_len = static_cast<socklen_t>(sizeof(snd_buffer_size));
        if (0 == getsockopt(sck, SOL_SOCKET, SO_SNDBUF, &snd_buffer_size, &option_len)) {
            if (snd_buffer_size < buffer_size) {
                snd_buffer_size = buffer_size;
                if (-1 == setsockopt(sck,
                        SOL_SOCKET,
                        SO_SNDBUF,
                        &snd_buffer_size, sizeof(snd_buffer_size))){
                    LOG(LOG_WARNING, "setsockopt failed with errno = %d (%s)", errno, strerror(errno));
                    return false;
                }
            }
        }
        else {
            LOG(LOG_WARNING, "getsockopt failed with errno = %d (%s)", errno, strerror(errno));
            return false;
        }

        return true;
    }

    unique_fd connect_sck(int sck, int nbretry, int retry_delai_ms, sockaddr & addr,
                          socklen_t addr_len, const char * target, bool no_log, char const** error_result = nullptr)
    {
        fcntl(sck, F_SETFL, fcntl(sck, F_GETFL) | O_NONBLOCK);

        int trial = 0;
        for (; trial < nbretry ; trial++){
            int const res = ::connect(sck, &addr, addr_len);
            if (-1 != res){
                // connection suceeded
                break;
            }

            int const err =  errno;
            if (trial > 0){
                char const* errmes = strerror(err);
                if (error_result) {
                    *error_result = errmes;
                }
                LOG(LOG_INFO, "Connection to %s failed with errno = %d (%s)", target, err, errmes);
            }

            if ((err == EINPROGRESS) || (err == EALREADY)){
                // try again
                fd_set fds;
                io_fd_zero(fds);
                io_fd_set(sck, fds);
                struct timeval timeout = {
                    retry_delai_ms / 1000,
                    1000 * (retry_delai_ms % 1000)
                };
                // exit select on timeout or connect or error
                // connect will catch the actual error if any,
                // no need to care of select result
                select(sck+1, nullptr, &fds, nullptr, &timeout);
            }
            else {
                // real failure
                trial = nbretry;
                break;
            }
        }

        if (trial >= nbretry){
            if (error_result) {
                *error_result = "All trials done";
            }
            LOG(LOG_ERR, "All trials done connecting to %s", target);
            return unique_fd{-1};
        }

        LOG_IF(!no_log, LOG_INFO, "connection to %s succeeded : socket %d", target, sck);
        return unique_fd{sck};
    }
} // namespace

char const* resolve_ipv4_address(const char* ip, in_addr & s4_sin_addr)
{
    if (!inet_aton(ip, &s4_sin_addr)) {
        struct addrinfo * addr_info = nullptr;
        int               result    = getaddrinfo(ip, nullptr, nullptr, &addr_info);
        if (result) {
            char const* error = (result == EAI_SYSTEM) ? strerror(errno) : gai_strerror(result);
            LOG(LOG_ERR, "DNS resolution failed for %s with errno = %d (%s)",
                ip, (result == EAI_SYSTEM) ? errno : result, error);
            return error;
        }
        s4_sin_addr.s_addr = (reinterpret_cast<sockaddr_in *>(addr_info->ai_addr))->sin_addr.s_addr; /*NOLINT*/
        freeaddrinfo(addr_info);
    }
    return nullptr;
}

unique_fd ip_connect(const char* ip, int port, char const** error_result)
{
    LOG(LOG_INFO, "connecting to %s:%d", ip, port);

    // we will try connection several time
    // the trial process include "socket opening, hostname resolution, etc
    // because some problems can come from the local endpoint,
    // not necessarily from the remote endpoint.
    int sck = socket(PF_INET, SOCK_STREAM, 0);

    union
    {
      sockaddr s;
      sockaddr_storage ss;
      sockaddr_in s4;
      sockaddr_in6 s6;
    } u;

    memset(&u, 0, sizeof(u));
    u.s4.sin_family = AF_INET;
    REDEMPTION_DIAGNOSTIC_PUSH()
    REDEMPTION_DIAGNOSTIC_GCC_IGNORE("-Wold-style-cast") // only to release
    u.s4.sin_port = htons(port);
    REDEMPTION_DIAGNOSTIC_POP()
    if (auto error = resolve_ipv4_address(ip, u.s4.sin_addr)){
        if (error_result) {
            *error_result = error;
        }
        LOG(LOG_ERR, "Connecting to %s:%d failed", ip, port);
        close(sck);
        return unique_fd{-1};
    }

    /* set snd buffer to at least 32 Kbytes */
    if (!set_snd_buffer(sck, 32768)) {
        if (error_result) {
            *error_result = "Cannot set socket buffer size";
        }
        LOG(LOG_ERR, "Connecting to %s:%d failed : cannot set socket buffer size", ip, port);
        close(sck);
        return unique_fd{-1};
    }

    char text_target[256];
    snprintf(text_target, sizeof(text_target), "%s:%d (%s)", ip, port, inet_ntoa(u.s4.sin_addr));

    int nbretry = 3;
    int retry_delai_ms = 1000;
    bool const no_log = false;

    return connect_sck(sck, nbretry, retry_delai_ms, u.s, sizeof(u), text_target, no_log, error_result);
}

AddrInfoPtrWithDel_t
resolve_both_ipv4_and_ipv6_address(const char *ip,
                                   int port,
                                   const char **error_result) noexcept
{
    addrinfo *addr_info = nullptr;
    addrinfo hints { };

    hints.ai_flags |= AI_V4MAPPED;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    auto port_str = int_to_decimal_zchars(port);
    if (int res = ::getaddrinfo(ip, port_str.c_str(), &hints, &addr_info))
    {
        const char *error = (res == EAI_SYSTEM) ?
            strerror(errno) : gai_strerror(res);

        if (error_result)
        {
            *error_result = error;
        }
        LOG(LOG_ERR,
            "DNS resolution failed for %s with errno = %d (%s)",
            ip,
            (res == EAI_SYSTEM) ? errno : res, error);
    }
    return AddrInfoPtrWithDel_t(addr_info);
}

unique_fd ip_connect_both_ipv4_and_ipv6(const char* ip,
                                        int port,
                                        const char **error_result) noexcept
{
    AddrInfoPtrWithDel_t addr_info_ptr =
        resolve_both_ipv4_and_ipv6_address(ip, port, error_result);

    if (!addr_info_ptr)
    {
        return unique_fd { -1 };
    }

    LOG(LOG_INFO, "connecting to %s:%d", ip, port);

    // we will try connection several time
    // the trial process include "socket opening, hostname resolution, etc
    // because some problems can come from the local endpoint,
    // not necessarily from the remote endpoint.

    int sck = socket(addr_info_ptr->ai_family,
                     addr_info_ptr->ai_socktype,
                     addr_info_ptr->ai_protocol);

    if (sck == -1)
    {
        if (error_result)
        {
            *error_result = "Cannot create socket";
        }
        LOG(LOG_ERR, "socket failed : %s", ::strerror(errno));
        close(sck);
        return unique_fd { -1 };
    }

    /* set snd buffer to at least 32 Kbytes */
    if (!set_snd_buffer(sck, 32768))
    {
        if (error_result)
        {
            *error_result = "Cannot set socket buffer size";
        }
        LOG(LOG_ERR,
            "Connecting to %s:%d failed : cannot set socket buffer size",
            ip,
            port);
        close(sck);
        return unique_fd{-1};
    }

    char resolved_ip_addr[NI_MAXHOST] { };

    if (int res = ::getnameinfo(addr_info_ptr->ai_addr,
                                addr_info_ptr->ai_addrlen,
                                resolved_ip_addr,
                                sizeof(resolved_ip_addr),
                                nullptr,
                                0,
                                NI_NUMERICHOST))
    {
        if (error_result)
        {
            *error_result = "Cannot get ip address";
        }
        LOG(LOG_ERR,
            "getnameinfo failed : %s",
            (res == EAI_SYSTEM) ?
            ::strerror(errno) : ::gai_strerror(res));
        close(sck);
        return unique_fd { -1 };
    }

    char text_target[2048] { };

    snprintf(text_target,
             sizeof(text_target),
             "%s:%d (%s)",
             ip,
             port,
             resolved_ip_addr);

    int nbretry = 3;
    int retry_delai_ms = 1000;
    const bool no_log = false;

    return connect_sck(sck,
                       nbretry,
                       retry_delai_ms,
                       *addr_info_ptr->ai_addr,
                       addr_info_ptr->ai_addrlen,
                       text_target,
                       no_log,
                       error_result);
}

unique_fd local_connect(const char* sck_name, bool no_log)
{
    char target[1024] = {};
    snprintf(target, sizeof(target), "%s", sck_name);

    LOG_IF(!no_log, LOG_INFO, "connecting to %s", sck_name);
    // we will try connection several time
    // the trial process include "ocket opening, hostname resolution, etc
    // because some problems can come from the local endpoint,
    // not necessarily from the remote endpoint.
    int sck = socket(AF_UNIX, SOCK_STREAM, 0);

    /* set snd buffer to at least 32 Kbytes */
    if (!set_snd_buffer(sck, 32768)) {
        return unique_fd{-1};
    }

    union
    {
      sockaddr_un s;
      sockaddr addr;
    } u;

    auto len = strnlen(sck_name, sizeof(u.s.sun_path)-1u);
    memcpy(u.s.sun_path, sck_name, len);
    u.s.sun_path[len] = 0;
    u.s.sun_family = AF_UNIX;

    int nbretry = 1;
    int retry_delai_ms = 1000;
    return connect_sck(sck, nbretry, retry_delai_ms, u.addr, static_cast<int>(offsetof(sockaddr_un, sun_path) + strlen(u.s.sun_path) + 1u), target, no_log);
}


unique_fd addr_connect(zstring_view addr, bool no_log_for_unix_socket)
{
    const char* pos = strchr(addr, ':');
    if (!pos) {
        return local_connect(addr, no_log_for_unix_socket);
    }

    int port;
    auto r = std::from_chars(pos + 1, addr.end(), port);
    if (r.ec == std::errc()) {
        std::string ip(addr.data(), pos);
        return ip_connect(ip.c_str(), port);
    }

    LOG(LOG_ERR, "Connecting to %s failed: invalid port", pos + 1);
    return unique_fd{-1};
}


unique_fd addr_connect_blocking(zstring_view addr, bool no_log_for_unix_socket)
{
    auto fd = addr_connect(addr, no_log_for_unix_socket);
    if (fd) {
        const auto sck = fd.fd();
        fcntl(sck, F_SETFL, fcntl(sck, F_GETFL) & ~O_NONBLOCK);
    }
    return fd;
}


struct LineBuffer
{
    char buffer[20480];
    int end_buffer;

    int fd;
    int begin_line;
    int eol;

    explicit LineBuffer(int fd)
    : end_buffer(0)
    , fd(fd)
    , begin_line(0)
    , eol(0)
    {
    }

    int readline()
    {
        for (int i = this->begin_line; i < this->end_buffer; i++){
            if (this->buffer[i] == '\n'){
                this->eol = i+1;
                return 1;
            }
        }
        size_t trailing_room = sizeof(this->buffer) - this->end_buffer;
        // reframe buffer if no trailing room left
        if (trailing_room == 0){
            size_t used_len = this->end_buffer - this->begin_line;
            memmove(this->buffer, &(this->buffer[this->begin_line]), used_len);
            this->end_buffer = used_len;
            this->begin_line = 0;
        }

        ssize_t res = read(this->fd, &(this->buffer[this->end_buffer]), sizeof(this->buffer) - this->end_buffer);
        if (res < 0){
            return res;
        }
        this->end_buffer += res;
        if (this->begin_line == this->end_buffer) {
            return 0;
        }
        for (int i = this->begin_line; i < this->end_buffer; i++){
            if (this->buffer[i] == '\n'){
                this->eol = i+1;
                return 1;
            }
        }
        this->eol = this->end_buffer;
        return 1;
    }
};

namespace minipeg
{
namespace
{
    struct Rng
    {
        char const* s;
        char const* e;

        std::size_t size() const noexcept { return std::size_t(e-s); }
    };

    auto to_parser(char c)
    {
        return [c](Rng& r){
            if (*r.s == c) {
                ++r.s;
                return true;
            }
            return false;
        };
    }

    auto to_parser(std::string_view s)
    {
        return [s](Rng& r){
            if (r.size() >= s.size()) {
                if (std::string_view(r.s, s.size()) == s) {
                    r.s += s.size();
                    return true;
                }
            }
            return false;
        };
    }

    auto to_parser(char const* s)
    {
        return to_parser(std::string_view(s));
    }

    template<class P>
    P to_parser(P p)
    {
        return p;
    }

    template<class... Ps>
    auto group(Ps... ps)
    {
        if constexpr (sizeof...(Ps) == 1) {
            return [](auto p) { return p; }(to_parser(ps)...);
        }
        else {
            return [](auto... p) {
                return [=](Rng& r){
                    auto s = r.s;
                    if ((... && p(r))) {
                        return true;
                    }
                    r.s = s;
                    return false;
                };
            }(to_parser(ps)...);
        }
    }

    template<class... Ps>
    auto zero_or_one(Ps... ps)
    {
        return [p = group(ps...)](Rng& r){
            p(r);
            return true;
        };
    }

    template<class... Ps>
    auto one_or_more(Ps... ps)
    {
        return [p = group(ps...)](Rng& r){
            if (p(r)) {
                while (p(r)) {
                }
                return true;
            }
            return false;
        };
    }

    template<std::size_t N, std::size_t M = N, class... Ps>
    auto repeat(Ps... ps)
    {
        return [p = group(ps...)](Rng& r){
            auto s = r.s;
            for (std::size_t i = 0; i < N; ++i) {
                if (!p(r)) {
                    r.s = s;
                    return false;
                }
            }

            for (std::size_t i = N; i < M && p(r); ++i) {
            }

            return true;
        };
    }

    template<class... Ps>
    auto after(Ps... ps)
    {
        return [p = group(ps...)](Rng& r){
            auto s = r.s;
            while (!p(r)) {
                ++r.s;
                if (r.s == r.e) {
                    r.s = s;
                    return false;
                }
            }
            return true;
        };
    }

    template<class F>
    auto is(F f)
    {
        return [f](Rng& r){
            if (f(*r.s)) {
                ++r.s;
                return true;
            }
            return false;
        };
    }

    struct Capture
    {
        char const* start;
        char const* end;
    };

    template<class... Ps>
    auto capture(Capture& cap, Ps... ps)
    {
        return [&cap, p = group(ps...)](Rng& r){
            auto s = r.s;
            if (p(r)) {
                cap.start = s;
                cap.end = r.s;
                return true;
            }
            return false;
        };
    }
} // namespace minpeg
} // anonymous namespace

zstring_view parse_ip_conntrack(
    int fd, const char * source, const char * dest, int sport, int dport,
    writable_bytes_view transparent_dest, uint32_t verbose)
{
    LineBuffer line(fd);
    //"tcp      6 299 ESTABLISHED src=10.10.43.13 dst=10.10.47.93 sport=36699 dport=22 packets=5256 bytes=437137 src=10.10.47.93 dst=10.10.43.13 sport=22 dport=36699 packets=3523 bytes=572101 [ASSURED] mark=0 secmark=0 use=2\n"

    using namespace minipeg;

    Capture ip_dest_cap;
    auto sport_s = int_to_decimal_chars(sport);
    auto dport_s = int_to_decimal_chars(dport);

    auto sv = [](chars_view av) { return std::string_view(av.data(), av.size()); };
    auto is_digit = [](char c) { return c <= '9' && c >= '0'; };

    auto digit = is(is_digit);
    auto ws = one_or_more(' ');
    auto int_ = one_or_more(digit);
    auto int3 = repeat<1, 3>(digit);
    auto ip = group(int3, '.', int3, '.', int3, '.', int3);

    auto parser = group(
        zero_or_one("ipv4", ws, '2', ws),
        "tcp", ws, '6', ws, int_, ws, "ESTABLISHED", ws,
        "src=", ip, ws,
        "dst=", capture(ip_dest_cap, ip), ws,
        "sport=", int_, ws,
        "dport=", int_, ' ',
        after("src="), source, ws,
        "dst=", dest, ws,
        "sport=", sv(sport_s), ws,
        "dport=", sv(dport_s), ' ',
        after("[ASSURED]")
    );

    int status = line.readline();
    for (; status == 1 ; (line.begin_line = line.eol), (status = line.readline())) {
        if (verbose) {
            fprintf(stderr, "Line: %.*s", line.eol - line.begin_line, &line.buffer[line.begin_line]);
        }

        if (line.eol - line.begin_line < 100) {
            continue;
        }

        const char * s = line.buffer + line.begin_line;

        const bool contains_endl = line.buffer[line.eol-1] == '\n';
        if (contains_endl) {
            line.buffer[line.eol-1] = 0;
        }

        Rng rng{s, line.buffer + (line.eol - 1)};
        if (parser(rng)) {
            const size_t match_size = std::size_t(ip_dest_cap.end - ip_dest_cap.start);
            if (match_size >= transparent_dest.size()){
                LOG(LOG_WARNING, "No enough space to store transparent ip target address");
                return zstring_view{};
            }

            memcpy(transparent_dest.data(), ip_dest_cap.start, match_size);
            transparent_dest[match_size] = 0;

            auto ip = zstring_view::from_null_terminated(
                transparent_dest.as_chars().data(), match_size);

            LOG_IF(verbose, LOG_INFO, "Match found: %s", ip);

            return ip;
        }

        if (contains_endl) {
            line.buffer[line.eol-1] = '\n';
        }
    }
    // transparent ip route not found in ip_conntrack
    return zstring_view{};
}

FILE* popen_conntrack(const char* source_ip, int source_port, int target_port)
{
    char cmd[256];
    sprintf(cmd, "/usr/sbin/conntrack -L -p tcp --src %s --sport %d --dport %d",
            source_ip, source_port, target_port);
    return popen(cmd, "r");
}

bool get_local_ip_address(IpAddress& client_address, int fd) noexcept
{
    union
    {
        sockaddr s;
        sockaddr_in s4;
        sockaddr_in6 s6;
        sockaddr_storage ss;
    } u;
    socklen_t namelen = sizeof(u);

    std::memset(&u, 0, namelen);
    if (::getsockname(fd, &u.s, &namelen) == -1)
    {
        LOG(LOG_ERR, "getsockname failed with errno = %d (%s)",
            errno, strerror(errno));
        return false;
    }

    if (int res = ::getnameinfo(&u.s,
                                sizeof(u.ss),
                                client_address.ip_addr,
                                sizeof(client_address.ip_addr),
                                nullptr,
                                0,
                                NI_NUMERICHOST))
    {
        LOG(LOG_ERR, "getnameinfo failed : %s",
            (res == EAI_SYSTEM) ? ::strerror(errno) : ::gai_strerror(res));
        return false;
    }

    return true;
}
