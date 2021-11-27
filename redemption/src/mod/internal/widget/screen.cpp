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

#include "mod/internal/widget/screen.hpp"
#include "mod/internal/widget/tooltip.hpp"
#include "gdi/graphic_api.hpp"
#include "core/RDP/rdp_pointer.hpp"

#include <cassert>


WidgetScreen::WidgetScreen(
    gdi::GraphicApi & drawable, uint16_t width, uint16_t height,
    Font const & font, NotifyApi * notifier, Theme theme
)
    : WidgetParent(drawable, *this, notifier)
    , theme(std::move(theme))
    , tooltip(nullptr)
    , current_over(nullptr)
    , font(font)
{
    this->set_wh(width, height);
    this->impl = &composite_array;

    this->tab_flag = IGNORE_TAB;

    this->set_xy(0, 0);
}

WidgetScreen::~WidgetScreen() = default;

void WidgetScreen::show_tooltip(
    Widget * widget, const char * text, int x, int y,
    Rect const preferred_display_rect)
{
    if (text == nullptr) {
        if (this->tooltip) {
            this->remove_widget(this->tooltip.get());
            this->rdp_input_invalidate(this->tooltip->get_rect());
            this->tooltip.reset();
        }
    }
    else if (this->tooltip == nullptr) {
        Rect display_rect = this->get_rect();
        if (!preferred_display_rect.isempty()) {
            display_rect = display_rect.intersect(preferred_display_rect);
        }

        this->tooltip = std::make_unique<WidgetTooltip>(
            this->drawable,
            *this, widget,
            "",
            this->theme.tooltip.fgcolor,
            this->theme.tooltip.bgcolor,
            this->theme.tooltip.border_color,
            this->font);
        this->tooltip->set_text(text, this->cx());
        Dimension dim = this->tooltip->get_optimal_dim();
        this->tooltip->set_wh(dim);

        int w = this->tooltip->cx();
        int h = this->tooltip->cy();
        int sw = display_rect.x + display_rect.cx;
        int sh = display_rect.y + display_rect.cy;
        int posx = ((x + w) > sw) ? (sw - w) : x;
        int posy = (y - h >= display_rect.y) ? (y - h) : (y + h > sh) ? (sh - h) : y;
        this->tooltip->set_xy(posx, posy);

        this->add_widget(this->tooltip.get());
        this->rdp_input_invalidate(this->tooltip->get_rect());
    }
}

bool WidgetScreen::next_focus()
{
    if (this->current_focus) {
        if (this->current_focus->next_focus()) {
            return true;
        }

        Widget * future_focus_w = this->get_next_focus(this->current_focus);
        if (!future_focus_w) {
            future_focus_w = this->get_next_focus(nullptr);
        }
        assert(this->current_focus);
        this->set_widget_focus(future_focus_w, focus_reason_tabkey);

        return true;
    }

    return false;
}

bool WidgetScreen::previous_focus()
{
    if (this->current_focus) {
        if (this->current_focus->previous_focus()) {
            return true;
        }

        Widget * future_focus_w = this->get_previous_focus(this->current_focus);
        if (!future_focus_w) {
            future_focus_w = this->get_previous_focus(nullptr);
        }
        assert(this->current_focus);
        this->set_widget_focus(future_focus_w, focus_reason_backtabkey);

        return true;
    }

    return false;
}

void WidgetScreen::rdp_input_mouse(int device_flags, int x, int y)
{
    this->redo_mouse_pointer_change(x, y);

    if (this->tooltip) {
        if (device_flags & MOUSE_FLAG_MOVE) {
            if (this->last_widget_at_pos(x, y) != this->tooltip->notifier) {
                this->hide_tooltip();
            }
        }
        if (device_flags & (MOUSE_FLAG_BUTTON1)) {
            this->hide_tooltip();
        }
    }

    WidgetParent::rdp_input_mouse(device_flags, x, y);
}

void WidgetScreen::rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap)
{
    if (this->tooltip) {
        this->hide_tooltip();
    }
    WidgetParent::rdp_input_scancode(flags, scancode, event_time, keymap);
}

void WidgetScreen::rdp_input_unicode(KbdFlags flag, uint16_t unicode)
{
    if (this->tooltip) {
        this->hide_tooltip();
    }
    WidgetParent::rdp_input_unicode(flag, unicode);
}

void WidgetScreen::allow_mouse_pointer_change(bool allow)
{
    this->allow_mouse_pointer_change_ = allow;
}

void WidgetScreen::redo_mouse_pointer_change(int x, int y)
{
    Widget * w = this->last_widget_at_pos(x, y);
    if (this->current_over != w){
        if (this->allow_mouse_pointer_change_) {
            switch (w ? w->pointer_flag : PointerType::Normal) {
            case PointerType::Edit:
                this->drawable.cached_pointer(PredefinedPointer::Edit);
            break;
            case PointerType::Custom: {
                if (auto const* cache_index = w->get_cache_pointer_index()) {
                    this->drawable.cached_pointer(*cache_index);
                    break;
                }
                [[fallthrough]];
            }
            case PointerType::Normal:
                this->drawable.cached_pointer(PredefinedPointer::Normal);
            }
        }
        this->current_over = w;
    }
}
