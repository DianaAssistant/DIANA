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
 *   Copyright (C) Wallix 2010-2012
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen,
 *              Meng Tan
 */

#pragma once

#include "mod/internal/widget/widget.hpp"


class Font;
namespace gdi
{
    class ColorCtx;
}

class WidgetButton : public Widget
{
private:
    static const size_t buffer_size = 256;

    char buffer[buffer_size];

    bool auto_resize_;

    int x_text;
    int y_text;

    unsigned border_width;

public:
    enum class State : bool
    {
        Normal,
        Pressed,
    };

    State state;

public:
    const Color fg_color;
    const Color bg_color;
    const Color focus_color;

private:
    const bool logo;

    Font const & font;

    Rect label_rect;

public:
    WidgetButton(gdi::GraphicApi & drawable, Widget& parent,
                     NotifyApi* notifier, const char * text,
                     int group_id, Color fg_color, Color bg_color, Color focus_color,
                     unsigned border_width, Font const & font,
                     int xtext = 0, int ytext = 0, bool logo = false); /*NOLINT*/

    ~WidgetButton();

    void set_xy(int16_t x, int16_t y) override;

    void set_wh(uint16_t w, uint16_t h) override;

    using Widget::set_wh;

    void set_text(char const* text);

    void rdp_input_invalidate(Rect clip) override;

    static void draw(Rect const clip, Rect const rect, gdi::GraphicApi& drawable,
                     bool logo, bool has_focus, char const* text,
                     Color fg_color, Color bg_color, Color focuscolor, gdi::ColorCtx color_ctx,
                     Rect label_rect, State state, unsigned border_width, Font const& font, int xtext, int ytext);

    void rdp_input_mouse(int device_flags, int x, int y) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

    void rdp_input_unicode(KbdFlags flag, uint16_t unicode) override;

    Dimension get_optimal_dim() const override;

    static Dimension get_optimal_dim(unsigned border_width, Font const& font, char const* text, int xtext = 0, int ytext = 0);
};
