/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1 of the License, or
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
 *   Copyright (C) Wallix 2010-2014
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen,
 *              Meng Tan
 */

#pragma once

#include "mod/internal/widget/widget.hpp"
#include "mod/internal/widget/label.hpp"

class WidgetEdit : public Widget
{
public:
    WidgetLabel label;
    size_t buffer_size;
    size_t num_chars;
    size_t edit_buffer_pos;
    size_t edit_pos;
private:
    size_t cursor_px_pos;
    int w_text;
    int h_text;
    Color cursor_color;
public:
    Color focus_color;
private:
    bool drawall;
public:
    bool draw_border_focus;
private:
    Font const & font;

public:
    WidgetEdit(gdi::GraphicApi & drawable,
               Widget & parent, NotifyApi* notifier, const char * text,
               int group_id, Color fgcolor, Color bgcolor, Color focus_color,
               Font const & font, std::size_t edit_position = -1, int xtext = 0, int ytext = 0); /*NOLINT*/

    Dimension get_optimal_dim() const override;

    virtual void set_text(const char * text/*, int position = 0*/);

    virtual void insert_text(const char * text/*, int position = 0*/);

    [[nodiscard]] const char * get_text() const;

    void set_xy(int16_t x, int16_t y) override;

    void set_wh(uint16_t w, uint16_t h) override;

    using Widget::set_wh;

    void rdp_input_invalidate(Rect clip) override;

    void draw_border(Rect clip, Color color);

    [[nodiscard]] virtual Rect get_cursor_rect() const;

    void draw_current_cursor();
    void draw_cursor(const Rect clip);

    void increment_edit_pos();

    size_t utf8len_current_char();

    void decrement_edit_pos();

    virtual void update_draw_cursor(Rect old_cursor);

    void move_to_last_character();

    void move_to_first_character();

    void rdp_input_mouse(int device_flags, int x, int y) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

    void rdp_input_unicode(KbdFlags flag, uint16_t unicode) override;

    void clipboard_paste(CopyPaste& copy_paste) override;
    void clipboard_copy(CopyPaste& copy_paste) override;
    void clipboard_cut(CopyPaste& copy_paste) override;
    void clipboard_insert_utf8(zstring_view text) override;

private:
    void insert_unicode_char(uint16_t unicode_char);
};
