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
  Copyright (C) Wallix 2010
  Author(s): Christophe Grosjean, Javier Caverni, Dominique Lafages,
             Raphael Zhou, Meng Tan, Clément Moroldo
  Based on xrdp Copyright (C) Jay Sorg 2004-2010

  rdp module main
*/

#include <cstring>
#include "utils/log.hpp"
#include "mod/rdp/rdp_negociation_data.hpp"

RdpLogonInfo::RdpLogonInfo(bounded_chars_view<0, HOST_NAME_MAX> hostname, bool hide_client_name,
                           char const* target_user, bool split_domain) noexcept
{
    if (hide_client_name) {
        this->_hostname.delayed_build([](auto& array) {
            ::gethostname(array.data(), array.size());
            array.back() = '\0';
            char* separator = strchr(array.data(), '.');
            if (!separator) {
                separator = &array.back();
            }
            return checked_int(separator - array.data());
        });
    }
    else{
        this->_hostname = hostname;
    }

    const char * domain_pos   = nullptr;
    size_t       domain_len   = 0;
    const char * username_pos = nullptr;
    size_t       username_len = 0;
    const char * separator = strchr(target_user, '\\');
    const char * separator_a = strchr(target_user, '@');

    username_pos = target_user;
    username_len = strlen(username_pos);

    if (separator && !separator_a)
    {
        // Legacy username
        // Split only if there's no @, otherwise not a legacy username
        domain_pos   = target_user;
        domain_len   = separator - target_user;
        username_pos = ++separator;
        username_len = strlen(username_pos);
    }
    else if (split_domain)
    {
        // Old behavior
        if (separator)
        {
            domain_pos   = target_user;
            domain_len   = separator - target_user;
            username_pos = ++separator;
            username_len = strlen(username_pos);
        }
        else if (separator_a)
        {
            domain_pos   = separator_a + 1;
            domain_len   = strlen(domain_pos);
            username_pos = target_user;
            username_len = separator_a - target_user;
            LOG(LOG_INFO, "mod_rdp: username_len=%zu", username_len);
        }
    }

    this->_username.assign(username_pos, username_pos+username_len);
    this->_domain.assign(domain_pos, domain_pos+domain_len);

    LOG(LOG_INFO, "Remote RDP Server domain=\"%s\" login=\"%s\" host=\"%s\"",
        this->_domain, this->_username, this->_hostname);
}

