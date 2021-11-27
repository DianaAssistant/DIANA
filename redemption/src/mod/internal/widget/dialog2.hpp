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
 *              Meng Tan, Jennifer Inthavong
 */

#pragma once

#include "mod/internal/widget/composite.hpp"
#include "mod/internal/widget/button.hpp"
#include "mod/internal/widget/image.hpp"
#include "mod/internal/widget/label.hpp"
#include "mod/internal/widget/vertical_scroll_text.hpp"
#include "mod/internal/widget/widget_rect.hpp"
#include "mod/internal/widget/delegated_copy.hpp"
#include "mod/internal/copy_paste.hpp"

class Theme;

class WidgetDialog2 : public WidgetParent
{
    CompositeArray composite_array;

    WidgetLabel        title;
    WidgetRect         separator;
    WidgetVerticalScrollText dialog;

public:
    WidgetButton ok;
private:
    WidgetImage img;
    WidgetLabel link_label;
    std::string link_value;
    WidgetVerticalScrollText link_show;
    WidgetDelegatedCopy link_copy;
    CopyPaste & copy_paste;

    Color bg_color;

public:
    WidgetDialog2(gdi::GraphicApi & drawable,
                  Rect const widget_rect,
                  Widget & parent, NotifyApi & notifier,
                  const char* caption, const char * text,
                  const char * link_value, const char * link_label,
                  CopyPaste & copy_paste, Theme const & theme,
                  Font const & font, const char * ok_text = "Ok" /*NOLINT*/);

    ~WidgetDialog2() override;

    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height);

    [[nodiscard]] Color get_bg_color() const override;

    void notify(Widget& widget, NotifyApi::notify_event_t event) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;
};
