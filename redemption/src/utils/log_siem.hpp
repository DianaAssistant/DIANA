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
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni, Jonathan Poelen
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   log file including syslog
*/

#pragma once

#include "utils/log.hpp"
#include <string_view>


#ifdef IN_IDE_PARSER
# define LOG_SIEM(...) ::compiler_aux_::unused_variables("" __VA_ARGS__)
# define LOG_PROXY_SIEM(type, ...) ::compiler_aux_::unused_variables("" type, "" __VA_ARGS__)

#else
# define LOG_SIEM(...) do {                                          \
    using ::log_value;                                               \
    ::detail::LOGCHECK_REDEMPTION_INTERNAL((                         \
        LOG_REDEMPTION_FORMAT_CHECK(__VA_ARGS__),                    \
        ::detail::LOG_SIEM_REDEMPTION_INTERNAL(""                    \
            LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(__VA_ARGS__)), \
        1                                                            \
    ));                                                              \
 } while (0)

# define LOG_PROXY_SIEM(type, ...) do {                              \
    using ::log_value;                                               \
    ::detail::LOGCHECK_REDEMPTION_INTERNAL((                         \
        LOG_REDEMPTION_FORMAT_CHECK(__VA_ARGS__),                    \
        ::detail::LOG_PROXY_SIEM_REDEMPTION_INTERNAL(                \
            "[rdpproxy] psid=\"%s\" user=\"%s\" type=\"" type "\" "  \
            LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(__VA_ARGS__)), \
        1                                                            \
    ));                                                              \
 } while (0)
#endif

namespace log_proxy
{
    void set_psid(std::string_view psid) noexcept;
    void incoming_connection(std::string_view source_ip, int source_port) noexcept;
    void set_user(std::string_view username) noexcept;
    void target_disconnection(char const* reason, char const* session_id) noexcept;
    void disconnection(char const* reason) noexcept;

    char const* get_psid() noexcept;
    char const* get_user() noexcept;
} // namespace log_proxy

namespace detail
{
    template<class... Ts>
    void LOG_SIEM_REDEMPTION_INTERNAL(char const * format, Ts const & ... args)
    {
        using ::log_value;
        LOG_REDEMPTION_INTERNAL_IMPL(
            LOG_INFO,
            format,
            REDEMPTION_LOG_VALUE(args)...
        );
    }

    template<class... Ts>
    void LOG_PROXY_SIEM_REDEMPTION_INTERNAL(char const * format, Ts const & ... args)
    {
        using ::log_value;
        LOG_REDEMPTION_INTERNAL_IMPL(
            LOG_INFO,
            format,
            log_proxy::get_psid(),
            log_proxy::get_user(),
            REDEMPTION_LOG_VALUE(args)...
        );
    }
} // namespace detail
