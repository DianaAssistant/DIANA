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
#include "mod/internal/widget/widget_rect.hpp"
#include "mod/internal/widget/label.hpp"
#include "mod/internal/widget/multiline.hpp"
#include "mod/internal/widget/image.hpp"
#include "mod/internal/widget/button.hpp"
#include "utils/translation.hpp"

#include <string>
#include <chrono>

class Theme;

class WidgetWabClose : public WidgetParent
{
    CompositeArray composite_array;

    WidgetLabel        connection_closed_label;
    WidgetRect         separator;

    WidgetLabel        username_label;
    WidgetLabel        username_value;
    WidgetLabel        target_label;
    WidgetLabel        target_value;
    WidgetLabel        diagnostic_label;
    WidgetMultiLine    diagnostic_value;
    WidgetLabel        timeleft_label;
    WidgetLabel        timeleft_value;

public:
    WidgetButton   cancel;
private:
    WidgetButton * back;

    WidgetImage        img;

    Color bg_color;

    long prev_time;

    Language lang;

    bool showtimer;

    Font const & font;

    std::string  diagnostic_text;
    bool         fixed_format_diagnostic_text;

public:
    WidgetWabClose(gdi::GraphicApi & drawable,
                 int16_t left, int16_t top, int16_t width, int16_t height, Widget& parent,
                 NotifyApi* notifier, const char * diagnostic_text,
                 const char * username, const char * target,
                 bool showtimer, const char * extra_message, Font const & font, Theme const & theme,
                 Language lang, bool back_selector = false); /*NOLINT*/

    ~WidgetWabClose();

    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height);

    [[nodiscard]] Color get_bg_color() const override;

    std::chrono::seconds refresh_timeleft(std::chrono::seconds remaining);

    void notify(Widget& widget, NotifyApi::notify_event_t event) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;
};
