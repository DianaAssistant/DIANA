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

#include "core/RDP/orders/RDPOrdersPrimaryOpaqueRect.hpp"
#include "core/font.hpp"
#include "mod/internal/widget/delegated_copy.hpp"
#include "gdi/graphic_api.hpp"
#include "gdi/text_metrics.hpp"
#include "utils/sugar/cast.hpp"
#include "utils/utf.hpp"

WidgetDelegatedCopy::WidgetDelegatedCopy(
    gdi::GraphicApi & drawable, Widget & parent, NotifyApi& notifier,
    int group_id, Color fgcolor, Color bgcolor, Color activecolor,
    Font const & font, int xicon, int yicon, MouseButton copy_buttons
)
    : Widget(drawable, parent, &notifier, group_id)
    , bg_color(bgcolor)
    , fg_color(fgcolor)
    , active_color(activecolor)
    , optimal_glyph_dim(get_optimal_dim(font, xicon, yicon))
    , x_icon(xicon)
    , y_icon(yicon)
    , copy_buttons(copy_buttons)
{
    this->tab_flag = IGNORE_TAB;
    this->focus_flag = IGNORE_FOCUS;
}

void WidgetDelegatedCopy::rdp_input_invalidate(Rect clip)
{
    Rect rect_intersect = clip.intersect(this->get_rect());

    if (!rect_intersect.isempty()) {
        this->draw(
            clip, this->get_rect(), this->drawable,
            this->fg_color, this->bg_color, this->x_icon, this->y_icon
        );
    }
}

void WidgetDelegatedCopy::draw(
    Rect clip, Rect rect, gdi::GraphicApi & drawable,
    Color fg, Color bg, int xicon, int yicon)
{
    drawable.begin_update();

    const auto color_ctx = gdi::ColorCtx::depth24();

    drawable.draw(RDPOpaqueRect(rect, bg), clip, color_ctx);

    auto drawRect = [&](int16_t x, int16_t y, uint16_t w, uint16_t h){
        drawable.draw(RDPOpaqueRect(Rect(x, y, w, h), fg), clip, color_ctx);
    };

    rect.x += xicon;
    rect.y += yicon;
    rect.cx -= xicon * 2;
    rect.cy -= yicon * 2;

    // left
    drawRect(rect.x, rect.y + 1, 1, rect.cy - 1);
    // right
    drawRect(rect.eright() - 1, rect.y + 1, 1, rect.cy - 1);
    // top
    drawRect(rect.x, rect.y + 1, rect.cx, 1);
    // bottom
    drawRect(rect.x, rect.ebottom() - 1, rect.cx, 1);

    // clip
    const int16_t d = ((rect.cx - 2) / 4) + /* border=*/1;
    drawRect(rect.x + d, rect.y, rect.cx - d * 2, 3);
    drawRect(rect.x + 2, rect.y + (rect.cy - 6) / 3 + 3, rect.cx - 4, 1);
    drawRect(rect.x + 2, rect.y + (rect.cy - 6) / 3 * 2 + 4, rect.cx - 4, 1);

    drawable.end_update();
}

Dimension WidgetDelegatedCopy::get_optimal_dim() const
{
    return this->optimal_glyph_dim;
}

Dimension WidgetDelegatedCopy::get_optimal_dim(Font const & font, int xicon, int yicon)
{
    auto& glyph = font.glyph_or_unknown('E');
    return Dimension{
        checked_int{glyph.width + 4 + xicon * 2},
        checked_int{glyph.height + 3 + yicon * 2},
    };
}

void WidgetDelegatedCopy::set_color(Color bg_color, Color fg_color)
{
    this->bg_color = bg_color;
    this->fg_color = fg_color;
}

static bool operator&(WidgetDelegatedCopy::MouseButton a,
                      WidgetDelegatedCopy::MouseButton b)
{
    return underlying_cast(a) & underlying_cast(b);
}

void WidgetDelegatedCopy::rdp_input_mouse(int device_flags, int /*x*/, int /*y*/)
{
    uint32_t mouse_match = (
      ((this->copy_buttons & MouseButton::Left)  ? MOUSE_FLAG_BUTTON1 : 0)
    | ((this->copy_buttons & MouseButton::Right) ? MOUSE_FLAG_BUTTON2 : 0)
    );
    if ((device_flags & mouse_match)
     && (device_flags & MOUSE_FLAG_DOWN)
     && !this->is_active
    ) {
        this->draw(
            this->get_rect(), this->get_rect(), this->drawable,
            this->active_color, this->bg_color, this->x_icon, this->y_icon
        );
        this->is_active = true;
        this->send_notify(*this, NOTIFY_COPY);
    }
    else if ((device_flags & mouse_match) && this->is_active) {
        this->draw(
            this->get_rect(), this->get_rect(), this->drawable,
            this->fg_color, this->bg_color, this->x_icon, this->y_icon
        );
        this->is_active = false;
    }
}
