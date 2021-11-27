/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2013
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen,
 *              Meng Tan
 */

#pragma once

#include "mod/internal/widget/edit.hpp"

class WidgetNumberEdit : public WidgetEdit
{
public:
    WidgetNumberEdit(gdi::GraphicApi & drawable, Widget & parent,
                     NotifyApi* notifier, const char* text, int group_id,
                     Color fgcolor, Color bgcolor, Color focus_color,
                     Font const & font, size_t edit_position = -1, /*NOLINT*/
                     int xtext = 0, int ytext = 0); /*NOLINT*/

    void set_text(const char * text) override;

    void insert_text(const char* text) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

private:
    int initial_x_text;
};

