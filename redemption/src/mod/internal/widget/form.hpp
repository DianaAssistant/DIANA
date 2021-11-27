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

#include "mod/internal/widget/composite.hpp"
#include "mod/internal/widget/button.hpp"
#include "mod/internal/widget/edit.hpp"
#include "utils/translation.hpp"

#include <chrono>


class Theme;

class WidgetForm : public WidgetParent
{
    CompositeArray composite_array;

    WidgetLabel      warning_msg;
    WidgetLabel      duration_label;
public:
    WidgetEdit       duration_edit;
private:
    WidgetLabel      duration_format;
    WidgetLabel      ticket_label;
public:
    WidgetEdit       ticket_edit;
private:
    WidgetLabel      comment_label;
public:
    WidgetEdit       comment_edit;
private:
    WidgetLabel      notes;
    WidgetButton confirm;

    Translator tr;

    unsigned flags;
    std::chrono::minutes duration_max;

    char warning_buffer[512];

public:
    // TODO enum class
    enum {
        NONE               = 0x00,
        COMMENT_DISPLAY    = 0x01,
        COMMENT_MANDATORY  = 0x02,
        TICKET_DISPLAY     = 0x04,
        TICKET_MANDATORY   = 0x08,
        DURATION_DISPLAY   = 0x10,
        DURATION_MANDATORY = 0x20,
    };

    WidgetForm(gdi::GraphicApi& drawable, int16_t left, int16_t top, int16_t width, int16_t height,
             Widget & parent, NotifyApi* notifier, int group_id,
             Font const & font, Theme const & theme, Language lang,
             unsigned flags = NONE,
             std::chrono::minutes duration_max = std::chrono::minutes::zero()); /*NOLINT*/

    WidgetForm(gdi::GraphicApi& drawable,
             Widget & parent, NotifyApi* notifier, int group_id,
             Font const & font, Theme const & theme, Language lang,
             unsigned flags = NONE,
             std::chrono::minutes duration_max = std::chrono::minutes::zero()); /*NOLINT*/

    ~WidgetForm() override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height);

    void notify(Widget& widget, NotifyApi::notify_event_t event) override;

private:
    template<class T, class... Ts>
    void set_warning_buffer(trkeys::TrKeyFmt<T> k, Ts const&... xs);

    void check_confirmation();
};
