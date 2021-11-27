/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2019
 *   Author(s): Meng Tan
 *
 *   Show a waiting message in modules transitions,
 *   also allow to quit with escape key
 */


#pragma once

#include "mod/internal/widget/tooltip.hpp"
#include "mod/internal/rail_mod_base.hpp"

class TransitionMod : public RailModBase
{
public:
    TransitionMod(
        char const * message,
        gdi::GraphicApi & drawable,
        FrontAPI & front, uint16_t width, uint16_t height,
        Rect const widget_rect, ClientExecute & rail_client_execute, Font const& font,
        Theme const& theme
    );

    ~TransitionMod() override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

    void acl_update(AclFieldMask const&/* acl_fields*/) override {}

private:
    WidgetTooltip ttmessage;
};
