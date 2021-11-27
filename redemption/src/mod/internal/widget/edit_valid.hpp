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
 *   Copyright (C) Wallix 1010-2013
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen,
 *              Meng Tan
 */

#pragma once

#include "mod/internal/widget/widget.hpp"
#include "mod/internal/widget/button.hpp"
#include "utils/colors.hpp"

class WidgetEdit;
class WidgetLabel;

class WidgetEditValid : public Widget
{
public:
    WidgetEditValid(gdi::GraphicApi & drawable,
                    Widget & parent, NotifyApi* notifier, const char * text,
                    int group_id, Color fgcolor, Color bgcolor,
                    Color focus_color, Color border_none_color, Font const & font,
                    const char * title, bool use_title, std::size_t edit_position = -1, /*NOLINT*/
                    // TODO re-enable
                    int /*xtext*/ = 0, int /*ytext*/ = 0, bool pass = false); /*NOLINT*/

    Dimension get_optimal_dim() const override;

    ~WidgetEditValid() override;

    void use_title(bool use);

    virtual void set_text(const char * text/*, int position = 0*/);

    [[nodiscard]] const char * get_text() const;

    void set_xy(int16_t x, int16_t y) override;

    void set_wh(uint16_t w, uint16_t h) override;

    using Widget::set_wh;

    void rdp_input_invalidate(Rect clip) override;

    void draw_border(const Rect clip, Color color);

    void focus(int reason) override;

    void blur() override;

    Widget * widget_at_pos(int16_t x, int16_t y) override;

    void rdp_input_mouse(int device_flags, int x, int y) override;

    void rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap) override;

    void rdp_input_unicode(KbdFlags flag, uint16_t unicode) override;

    void notify(Widget& widget, NotifyApi::notify_event_t event) override;

    [[nodiscard]] static unsigned get_border_height()
    {
        return 1;
    }

private:
    WidgetButton button;
    WidgetEdit * editbox;
    WidgetLabel * label;

    bool use_label_;

    Color border_none_color;
};
