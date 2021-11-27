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
 *   Author(s): Christophe Grosjean, Meng Tan, Jennifer Inthavong
 */

#pragma once

#include "mod/internal/widget/composite.hpp"
#include "mod/internal/widget/label.hpp"
#include "mod/internal/widget/edit_valid.hpp"
#include "mod/internal/widget/image.hpp"
#include "mod/internal/widget/vertical_scroll_text.hpp"
#include "utils/translation.hpp"
#include "mod/internal/widget/multiline.hpp"

class Theme;

class WidgetLogin : public WidgetParent
{
    CompositeArray composite_array;

    WidgetLabel        error_message_label;
    WidgetLabel        login_label;
public:
    WidgetEditValid    login_edit;
private:
    WidgetLabel        password_label;
public:
    WidgetEditValid    password_edit;
private:
    WidgetLabel        target_label;
public:
    WidgetEditValid    target_edit;
private:
    WidgetVerticalScrollText message_label;
    WidgetImage        img;
    WidgetLabel        version_label;
public:
    WidgetButton   helpicon;
private:
    WidgetButton * extra_button;

    Translator tr;

    bool labels_added = false;
    bool show_target = false;

    Color bg_color;

public:
    WidgetLogin(gdi::GraphicApi & drawable,
              int16_t left, int16_t top, uint16_t width, uint16_t height, Widget & parent,
              NotifyApi* notifier, const char* caption,
              const char * login, const char * password, const char * target,
              const char * label_text_login,
              const char * label_text_password,
              const char * label_text_target,
              const char * label_error_message,
              const char * login_message,
              WidgetButton * extra_button,
              bool enable_target_field,
              Font const & font, Translator tr, Theme const & theme);

    ~WidgetLogin() override;

    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height);

    [[nodiscard]] Color get_bg_color() const override;

    void notify(Widget& widget, NotifyApi::notify_event_t event) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

    void rdp_input_mouse(int device_flags, int x, int y) override;
};
