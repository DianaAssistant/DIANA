/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2010-2018
Author(s): Jonathan Poelen
*/

#include "log_siem.hpp"
#include "utils/strutils.hpp"

#include <cstring>


namespace
{
    char g_psid[32] = "42"; /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */
    char g_username[256] = ""; /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */
}

namespace log_proxy
{
    void set_psid(std::string_view psid) noexcept
    {
        utils::strlcpy(g_psid, psid);
    }

    void incoming_connection(std::string_view source_ip, int source_port) noexcept
    {
        LOG_REDEMPTION_INTERNAL_IMPL(
            LOG_INFO,
            R"([rdpproxy] psid="%s" type="INCOMING_CONNECTION" src_ip="%.*s" src_port="%d")",
            g_psid, int(source_ip.size()), source_ip.data(), source_port
        );
    }

    void set_user(std::string_view username) noexcept
    {
        utils::strlcpy(g_username, username);
    }

    void target_disconnection(char const* reason, char const* session_id) noexcept
    {
        if (reason && *reason) {
            LOG_PROXY_SIEM("TARGET_DISCONNECTION",
                           R"(session_id="%s" reason="%s")",
                           session_id,
                           reason);
        }
        else {
            LOG_PROXY_SIEM("TARGET_DISCONNECTION",
                           R"(session_id="%s")",
                           session_id);
        }
    }

    void disconnection(char const* reason) noexcept
    {
        if (reason && *reason) {
            if (g_username[0]) {
                LOG_REDEMPTION_INTERNAL_IMPL(
                    LOG_INFO,
                    R"([rdpproxy] psid="%s" user="%s" type="DISCONNECT" reason="%s")",
                    g_psid, g_username, reason
                );
            }
            else {
                LOG_REDEMPTION_INTERNAL_IMPL(
                    LOG_INFO,
                    R"([rdpproxy] psid="%s" type="DISCONNECT" reason="%s")",
                    g_psid, reason
                );
            }
        }
        else {
            if (g_username[0]) {
                LOG_REDEMPTION_INTERNAL_IMPL(
                    LOG_INFO,
                    R"([rdpproxy] psid="%s" user="%s" type="DISCONNECT")",
                    g_psid, g_username
                );
            }
            else {
                LOG_REDEMPTION_INTERNAL_IMPL(
                    LOG_INFO,
                    R"([rdpproxy] psid="%s" type="DISCONNECT")",
                    g_psid
                );
            }
        }
    }

    char const* get_psid() noexcept
    {
        return g_psid;
    }

    char const* get_user() noexcept
    {
        return g_username;
    }
} // namespace log_proxy
