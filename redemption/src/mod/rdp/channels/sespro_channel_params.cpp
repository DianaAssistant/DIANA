/*
    This program is free software; you can redistribute it and/or modify it
     under the terms of the GNU General Public License as published by the
     Free Software Foundation; either version 2 of the License, or (at your
     option) any later version.

    This program is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
     Public License for more details.

    You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     675 Mass Ave, Cambridge, MA 02139, USA.

    Product name: redemption, a FLOSS RDP proxy
    Copyright (C) Wallix 2015
    Author(s): Christophe Grosjean, Raphael Zhou
*/

#include "mod/rdp/channels/sespro_channel_params.hpp"
#include "utils/sugar/zstring_view.hpp"
#include "utils/strutils.hpp"

#include <iterator>

#include <cassert>


ExtraSystemProcesses::ExtraSystemProcesses(const char * comme_separated_processes)
{
    if (comme_separated_processes) {
        const char * process = comme_separated_processes;

        while (*process) {
            if ((*process == ',') || (*process == '\t') || (*process == ' ')) {
                process++;
                continue;
            }

            char const * process_begin = process;

            const char * process_separator = strchr(process, ',');

            this->processes.emplace_back(
                process_begin,
                (process_separator ? process_separator - process_begin : ::strlen(process_begin))
            );

            if (!process_separator) {
                break;
            }

            process = process_separator + 1;
        }
    }
}

// TODO {std:string const&, bool} ?
bool ExtraSystemProcesses::get(
    size_t index,
    std::string & out_name
) const {
    if (this->processes.size() <= index) {
        out_name.clear();

        return false;
    }

    out_name = this->processes[index];

    return true;
}

std::string ExtraSystemProcesses::to_string() const
{
    std::string r;
    for (auto& x : this->processes) {
        str_append(r, x, ", ");
    }
    return r;
}


OutboundConnectionMonitorRules::OutboundConnectionMonitorRules(
    const char * comme_separated_monitoring_rules
) {
    if (comme_separated_monitoring_rules) {
        const char * rule = comme_separated_monitoring_rules;

        auto RULE_PREFIX_ALLOW  = "$allow:"_zv;
        auto RULE_PREFIX_NOTIFY = "$notify:"_zv;
        auto RULE_PREFIX_DENY   = "$deny:"_zv;

        while (*rule) {
            if ((*rule == ',') || (*rule == '\t') || (*rule == ' ')) {
                rule++;
                continue;
            }

            char const * rule_begin = rule;

            Type uType = Type::Deny;
            if (strcasestr(rule, RULE_PREFIX_ALLOW.c_str()) == rule)
            {
                uType  = Type::Allow;
                rule  += RULE_PREFIX_ALLOW.size();
            }
            else if (strcasestr(rule, RULE_PREFIX_NOTIFY.c_str()) == rule)
            {
                uType  = Type::Notify;
                rule  += RULE_PREFIX_NOTIFY.size();
            }
            else if (strcasestr(rule, RULE_PREFIX_DENY.c_str()) == rule)
            {
                uType  = Type::Deny;
                rule  += RULE_PREFIX_DENY.size();
            }

            const char * rule_separator = strchr(rule, ',');

            std::string rule_string(rule, (rule_separator ? rule_separator - rule : ::strlen(rule)));

            const char * rule_c_str = rule_string.c_str();

            const char * info_separator = strrchr(rule_c_str, ':');

            if (info_separator)
            {
                std::string description_string(rule_begin,
                    (rule_separator ? rule_separator - rule_begin : ::strlen(rule_begin)));

                std::string host_address_or_subnet(rule_c_str, info_separator - rule_c_str);

                this->rules.push_back({
                    uType,
                    std::move(host_address_or_subnet),
                    std::string(info_separator + 1),
                    std::move(description_string)
                });
            }

            if (!rule_separator) {
                break;
            }

            rule = rule_separator + 1;
        }
    }
}

// TODO {std:string const&, bool} ?
bool OutboundConnectionMonitorRules::get(
    size_t index,
    unsigned int & out_type,
    std::string & out_host_address_or_subnet,
    std::string & out_port_range,
    std::string & out_description
) const {
    if (this->rules.size() <= index) {
        out_type = 0;
        out_host_address_or_subnet.clear();
        out_port_range.clear();
        out_description.clear();

        return false;
    }

    out_type                   = unsigned(this->rules[index].type);
    out_host_address_or_subnet = this->rules[index].address;
    out_port_range             = this->rules[index].port_range;
    out_description            = this->rules[index].description;

    return true;
}

std::string OutboundConnectionMonitorRules::to_string() const
{
    std::string r;
    constexpr chars_view type_s[]{
        "Notify"_av,
        "Deny"_av,
        "Allow"_av,
    };
    for (auto& x : this->rules) {
        assert(unsigned(x.type) < std::size(type_s));
        str_append(r, '{',
            type_s[unsigned(x.type)], ", ",
            x.address, ", ",
            x.port_range, ", ",
            x.description, "}, "
        );
    }
    return r;
}


ProcessMonitorRules::ProcessMonitorRules(const char * comme_separated_rules)
{
    if (comme_separated_rules) {
        const char * rule = comme_separated_rules;

        auto RULE_PREFIX_NOTIFY = "$notify:"_zv;
        auto RULE_PREFIX_DENY   = "$deny:"_zv;

        while (*rule) {
            if ((*rule == ',') || (*rule == '\t') || (*rule == ' ')) {
                rule++;
                continue;
            }

            char const * rule_begin = rule;

            Type uType = Type::Deny;
            if (strcasestr(rule, RULE_PREFIX_NOTIFY.c_str()) == rule)
            {
                uType  = Type::Notify;
                rule  += RULE_PREFIX_NOTIFY.size();
            }
            else if (strcasestr(rule, RULE_PREFIX_DENY.c_str()) == rule)
            {
                uType  = Type::Deny;
                rule  += RULE_PREFIX_DENY.size();
            }

            const char * rule_separator = strchr(rule, ',');

            std::string description_string(rule_begin, (rule_separator ? rule_separator - rule_begin : ::strlen(rule_begin)));

            std::string pattern(rule, (rule_separator ? rule_separator - rule : ::strlen(rule)));

            this->rules.push_back({
                uType, std::move(pattern), std::move(description_string)
            });

            if (!rule_separator) {
                break;
            }

            rule = rule_separator + 1;
        }
    }
}

// TODO {std:string const&, bool} ?
bool ProcessMonitorRules::get(
    size_t index,
    unsigned int & out_type,
    std::string & out_pattern,
    std::string & out_description
) const {
    if (this->rules.size() <= index) {
        out_type = 0;
        out_pattern.clear();
        out_description.clear();

        return false;
    }

    out_type                   = unsigned(this->rules[index].type);
    out_pattern                = this->rules[index].pattern;
    out_description            = this->rules[index].description;

    return true;
}

std::string ProcessMonitorRules::to_string() const
{
    std::string r;
    constexpr chars_view type_s[]{
        "Notify"_av,
        "Deny"_av,
    };
    for (auto& x : this->rules) {
        assert(unsigned(x.type) < std::size(type_s));
        str_append(r, '{',
            type_s[unsigned(x.type)], ", ",
            x.pattern, ", ",
            x.description, "}, "
        );
    }
    return r;
}
