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

*/

#include "test_only/test_framework/redemption_unit_tests.hpp"
#include "test_only/test_framework/working_directory.hpp"

#include "utils/netutils.hpp"

#include <cstdlib>
#include <unistd.h>
#include <string_view>

using namespace std::string_view_literals;


RED_AUTO_TEST_CASE(ParseIpConntrack)
{
    chars_view conntrack1
      = "unknown  2 580 src=10.10.43.13 dst=224.0.0.251 packets=2 bytes=64 [UNREPLIED] src=224.0.0.251 dst=10.10.43.13 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "udp      17 28 src=10.10.43.30 dst=255.255.255.255 sport=17500 dport=17500 packets=1102 bytes=154280 [UNREPLIED] src=255.255.255.255 dst=10.10.43.30 sport=17500 dport=17500 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "unknown  2 205 src=10.10.47.124 dst=224.0.0.251 packets=2 bytes=64 [UNREPLIED] src=224.0.0.251 dst=10.10.47.124 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "unknown  2 209 src=10.10.41.13 dst=224.0.0.251 packets=5 bytes=160 [UNREPLIED] src=224.0.0.251 dst=10.10.41.13 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "udp      17 19 src=10.10.41.14 dst=10.10.47.255 sport=17500 dport=17500 packets=5937 bytes=831180 [UNREPLIED] src=10.10.47.255 dst=10.10.41.14 sport=17500 dport=17500 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "udp      17 19 src=10.10.41.14 dst=255.255.255.255 sport=17500 dport=17500 packets=5937 bytes=831180 [UNREPLIED] src=255.255.255.255 dst=10.10.41.14 sport=17500 dport=17500 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "udp      17 29 src=10.10.43.33 dst=10.10.47.255 sport=17500 dport=17500 packets=971 bytes=135940 [UNREPLIED] src=10.10.47.255 // dst=10.10.43.33 sport=17500 dport=17500 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "udp      17 29 src=10.10.43.33 dst=255.255.255.255 sport=17500 dport=17500 packets=971 bytes=135940 [UNREPLIED] src=255.255.255.255 dst=10.10.43.33 sport=17500 dport=17500 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "unknown  2 455 src=10.10.47.170 dst=224.0.0.251 packets=1 bytes=32 [UNREPLIED] src=224.0.0.251 dst=10.10.47.170 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "unknown  2 580 src=0.0.0.0 dst=224.0.0.1 packets=1422 bytes=45504 [UNREPLIED] src=224.0.0.1 dst=0.0.0.0 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "tcp      6 431995 ESTABLISHED src=127.0.0.1 dst=127.0.0.1 sport=51040 dport=3450 packets=5 bytes=588 src=127.0.0.1 dst=127.0.0.1 sport=3450 dport=51040 packets=4 bytes=573 [ASSURED] mark=0 secmark=0 use=2\n"
        "udp      17 28 src=10.10.43.30 dst=10.10.47.255 sport=17500 dport=17500 packets=1102 bytes=154280 [UNREPLIED] src=10.10.47.255 dst=10.10.43.30 sport=17500 dport=17500 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "unknown  2 79 src=10.10.45.11 dst=224.0.0.251 packets=1 bytes=32 [UNREPLIED] src=224.0.0.251 dst=10.10.45.11 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "tcp      6 431979 ESTABLISHED src=10.10.43.13 dst=10.10.46.78 sport=41971 dport=3389 packets=96 bytes=10739 src=10.10.47.93 dst=10.10.43.13 sport=3389 dport=41971 packets=96 bytes=39071 [ASSURED] mark=0 secmark=0 use=2\n"
        "udp      17 0 src=10.10.41.14 dst=10.10.47.255 sport=631 dport=631 packets=1 bytes=274 [UNREPLIED] src=10.10.47.255 dst=10.10.41.14 sport=631 dport=631 packets=0 bytes=0 mark=0 secmark=0 use=2\n"
        "tcp      6 299 ESTABLISHED src=10.10.43.13 dst=10.10.47.93 sport=36699 dport=22 packets=5256 bytes=437137 src=10.10.47.93 dst=10.10.43.13 sport=22 dport=36699 packets=3523 bytes=572101 [ASSURED] mark=0 secmark=0 use=2\n"
        "tcp      6 431997 ESTABLISHED src=10.10.43.13 dst=10.10.47.93 sport=46392 dport=3389 packets=90 bytes=10061 src=10.10.47.93 dst=10.10.43.13 sport=3389 dport=46392 packets=89 bytes=38707 [ASSURED] mark=0 secmark=0 use=2\n"
        "udp      17 0 src=10.10.43.31 dst=10.10.47.255 sport=57621 dport=57621 packets=1139 bytes=82008 [UNREPLIED] src=10.10.47.255 dst=10.10.43.31 sport=57621 dport=57621 packets=0 bytes=0 mark=0 secmark=0 use=2\n"_av;

    chars_view conntrack2
      =  "unknown  2 580 src=10.10.43.13 dst=224.0.0.251 [UNREPLIED] src=224.0.0.251 dst=10.10.43.13 mark=0 secmark=0 use=2\n"
        "udp      17 28 src=10.10.43.30 dst=255.255.255.255 sport=17500 dport=17500 [UNREPLIED] src=255.255.255.255 dst=10.10.43.30 sport=17500 dport=17500 mark=0 secmark=0 use=2\n"
        "unknown  2 205 src=10.10.47.124 dst=224.0.0.251 [UNREPLIED] src=224.0.0.251 dst=10.10.47.124 mark=0 secmark=0 use=2\n"
        "unknown  2 209 src=10.10.41.13 dst=224.0.0.251 [UNREPLIED] src=224.0.0.251 dst=10.10.41.13 mark=0 secmark=0 use=2\n"
        "udp      17 19 src=10.10.41.14 dst=10.10.47.255 sport=17500 dport=17500 [UNREPLIED] src=10.10.47.255 dst=10.10.41.14 sport=17500 dport=17500 mark=0 secmark=0 use=2\n"
        "udp      17 19 src=10.10.41.14 dst=255.255.255.255 sport=17500 dport=17500 [UNREPLIED] src=255.255.255.255 dst=10.10.41.14 sport=17500 dport=17500 mark=0 secmark=0 use=2\n"
        "udp      17 29 src=10.10.43.33 dst=10.10.47.255 sport=17500 dport=17500 [UNREPLIED] src=10.10.47.255 // dst=10.10.43.33 sport=17500 dport=17500 mark=0 secmark=0 use=2\n"
        "udp      17 29 src=10.10.43.33 dst=255.255.255.255 sport=17500 dport=17500 [UNREPLIED] src=255.255.255.255 dst=10.10.43.33 sport=17500 dport=17500 mark=0 secmark=0 use=2\n"
        "unknown  2 455 src=10.10.47.170 dst=224.0.0.251 [UNREPLIED] src=224.0.0.251 dst=10.10.47.170 mark=0 secmark=0 use=2\n"
        "unknown  2 580 src=0.0.0.0 dst=224.0.0.1 [UNREPLIED] src=224.0.0.1 dst=0.0.0.0 mark=0 secmark=0 use=2\n"
        "tcp      6 431995 ESTABLISHED src=127.0.0.1 dst=127.0.0.1 sport=51040 dport=3450 src=127.0.0.1 dst=127.0.0.1 sport=3450 dport=51040 [ASSURED] mark=0 secmark=0 use=2\n"
        "udp      17 28 src=10.10.43.30 dst=10.10.47.255 sport=17500 dport=17500 [UNREPLIED] src=10.10.47.255 dst=10.10.43.30 sport=17500 dport=17500 mark=0 secmark=0 use=2\n"
        "unknown  2 79 src=10.10.45.11 dst=224.0.0.251 [UNREPLIED] src=224.0.0.251 dst=10.10.45.11 mark=0 secmark=0 use=2\n"
        "tcp      6 431979 ESTABLISHED src=10.10.43.13 dst=10.10.46.78 sport=41971 dport=3389 src=10.10.47.93 dst=10.10.43.13 sport=3389 dport=41971 [ASSURED] mark=0 secmark=0 use=2\n"
        "udp      17 0 src=10.10.41.14 dst=10.10.47.255 sport=631 dport=631 [UNREPLIED] src=10.10.47.255 dst=10.10.41.14 sport=631 dport=631 mark=0 secmark=0 use=2\n"
        "tcp      6 299 ESTABLISHED src=10.10.43.13 dst=10.10.47.93 sport=36699 dport=22 src=10.10.47.93 dst=10.10.43.13 sport=22 dport=36699 [ASSURED] mark=0 secmark=0 use=2\n"
        "tcp      6 431997 ESTABLISHED src=10.10.43.13 dst=10.10.47.93 sport=46392 dport=3389 src=10.10.47.93 dst=10.10.43.13 sport=3389 dport=46392 [ASSURED] mark=0 secmark=0 use=2\n"
        "udp      17 0 src=10.10.43.31 dst=10.10.47.255 sport=57621 dport=57621 [UNREPLIED] src=10.10.47.255 dst=10.10.43.31 sport=57621 dport=57621 mark=0 secmark=0 use=2\n"_av;

    std::string prefix =  "ipv4     2 ";
    std::string conntrack3 = prefix;
    for (char c: conntrack1) {
        conntrack3 += c;
        if (c == '\n') {
            conntrack3 += prefix;
        }
    }
    conntrack3.resize(conntrack3.size() - prefix.size());

    std::string conntrack4 = prefix;
    for (char c: conntrack2) {
        conntrack4 += c;
        if (c == '\n') {
            conntrack4 += prefix;
        }
    }
    conntrack4.resize(conntrack4.size() - prefix.size());

    struct D {
        char const* name;
        chars_view data;
    };
    RED_TEST_CONTEXT_DATA(D const& d, d.name, {
        // "tcp      6 431979 ESTABLISHED src=10.10.43.13 dst=10.10.46.78 sport=41971 dport=3389 packets=96 bytes=10739 src=10.10.47.93 dst=10.10.43.13 sport=3389 dport=41971 packets=96 bytes=39071 [ASSURED] mark=0 secmark=0 use=2\n"
        D{"test1", conntrack1},
        // "tcp      6 431979 ESTABLISHED src=10.10.43.13 dst=10.10.46.78 sport=41971 dport=3389 src=10.10.47.93 dst=10.10.43.13 sport=3389 dport=41971 [ASSURED] mark=0 secmark=0 use=2\n"
        D{"test2", conntrack2},
        D{"test3", conntrack3},
        D{"test4", conntrack4}
    })
    {
        WorkingFile wf(d.name);
        int fd = ::open(wf.c_str(), O_RDWR|O_CREAT, 0777);
        RED_CHECK(ssize_t(d.data.size()) == write(fd, d.data.data(), d.data.size()));
        RED_CHECK(0 == lseek(fd, 0, SEEK_SET));
        char transparent_target[256];

        RED_CHECK("10.10.46.78"sv == parse_ip_conntrack(
            fd, "10.10.47.93", "10.10.43.13", 3389, 41971,
            make_writable_array_view(transparent_target), 0).to_sv());

        RED_CHECK(0 == lseek(fd, 0, SEEK_SET));
        transparent_target[0] = 0;
        RED_CHECK(""sv == parse_ip_conntrack(
            fd, "10.10.47.21", "10.10.43.13", 3389, 46392,
            make_writable_array_view(transparent_target), 0).to_sv());

        close(fd);
    }
}
