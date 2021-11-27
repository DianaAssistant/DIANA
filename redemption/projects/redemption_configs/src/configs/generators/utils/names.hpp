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
Copyright (C) Wallix 2010-2019
Author(s): Jonathan Poelen
*/

#pragma once

#include "configs/generators/utils/spec_writer.hpp"

namespace cfg_generators
{
    template<class Pack>
    std::string sesman_network_name(Pack const& infos, Names const& section_names)
    {
        Names const& names = infos;
        if constexpr (is_convertible_v<Pack, connection_policy_t>) {
            return str_concat(section_names.all, ':', names.all);
        }
        else {
            return names.sesman_name();
        }
    }
}
