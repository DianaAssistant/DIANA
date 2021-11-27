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
 *              Meng Tan, Raphael Zhou
 */

#include "mod/internal/widget/composite.hpp"
#include "keyboard/keymap.hpp"
#include "utils/region.hpp"
#include "core/RDP/orders/RDPOrdersPrimaryOpaqueRect.hpp"
#include "gdi/graphic_api.hpp"

void fill_region(gdi::GraphicApi & drawable, const SubRegion & region, Widget::Color bg_color)
{
    for (Rect const & rect : region.rects) {
        drawable.draw(RDPOpaqueRect(rect, bg_color), rect, gdi::ColorCtx::depth24());
    }
}


CompositeArray::CompositeArray() = default;

int CompositeArray::add(Widget * w)
{
    assert(w);
    assert(this->children_count < MAX_CHILDREN_COUNT);
    this->child_table[this->children_count] = w;
    return this->children_count++;
}

void CompositeArray::remove(const Widget * w)
{
    assert(w);
    assert(this->children_count);
    auto last = this->child_table + this->children_count;
    auto it = std::find(&this->child_table[0], last, w);
    assert(it != last);
    if (it != last) {
        auto new_last = std::copy(it+1, last, it);
        *new_last = nullptr;
        this->children_count--;
    }
}

Widget * CompositeArray::get(int index) const
{
    return this->child_table[index];
}

int CompositeArray::get_first()
{
    if (!this->children_count) {
        return -1;
    }

    return 0;
}

int CompositeArray::get_last()
{
    if (!this->children_count) {
        return -1;
    }

    return this->children_count - 1;
}

int CompositeArray::get_previous(int index)
{
    if (index == this->get_first()) {
        return -1;
    }

    return index - 1;
}

int CompositeArray::get_next(int index)
{
    if (index == this->get_last()) {
        return -1;
    }

    return index + 1;
}

int CompositeArray::find(const Widget * w)
{
    for (size_t i = 0; i < this->children_count; i++) {
        if (this->child_table[i] == w) {
            return i;
        }
    }

    return -1;
}

void CompositeArray::clear()
{
    this->children_count = 0;
}


WidgetParent::WidgetParent(
    gdi::GraphicApi & drawable, Widget & parent, NotifyApi * notifier, int group_id
)
    : Widget(drawable, parent, notifier, group_id)
    , pressed(nullptr)
    , bg_color(BLACK)
    , impl(nullptr)
    , current_focus(nullptr)
{}

WidgetParent::~WidgetParent() = default;

void WidgetParent::set_widget_focus(Widget * new_focused, int reason)
{
    assert(new_focused);
    if (new_focused != this->current_focus) {
        if (this->current_focus) {
            this->current_focus->blur();
        }
        this->current_focus = new_focused;
    }

    this->current_focus->focus(reason);
}

void WidgetParent::focus(int reason)
{
    const bool tmp_has_focus = this->has_focus;
    if (!this->has_focus) {
        this->has_focus = true;
        this->send_notify(NOTIFY_FOCUS_BEGIN);

        if (reason == focus_reason_tabkey) {
            this->current_focus = this->get_next_focus(nullptr);
        }
        else if (reason == focus_reason_backtabkey) {
            this->current_focus = this->get_previous_focus(nullptr);
        }
    }
    if (this->current_focus) {
        this->current_focus->focus(reason);
    }
    if (!tmp_has_focus) {
        this->rdp_input_invalidate(this->get_rect());
    }
}

void WidgetParent::blur()
{
    if (this->has_focus) {
        this->has_focus = false;
        this->send_notify(NOTIFY_FOCUS_END);
    }
    if (this->current_focus) {
        this->current_focus->blur();
    }
    this->rdp_input_invalidate(this->get_rect());
}

Widget * WidgetParent::get_next_focus(Widget * w)
{
    int index;
    if (!w) {
        if ((index = this->impl->get_first()) == -1) {
            return nullptr;
        }

        w = this->impl->get(index);
        if ((w->tab_flag != Widget::IGNORE_TAB) && (w->focus_flag != Widget::IGNORE_FOCUS)) {
            return w;
        }
    }
    else {
        index = this->impl->find(w);
        assert(index != -1);
    }

    int future_focus_index;
    while ((future_focus_index = this->impl->get_next(index)) != -1) {
        Widget * future_focus_w = this->impl->get(future_focus_index);
        if ((future_focus_w->tab_flag != Widget::IGNORE_TAB) && (future_focus_w->focus_flag != Widget::IGNORE_FOCUS)) {
            return future_focus_w;
        }

        if (future_focus_w == w) {
            break;
        }

        index = future_focus_index;
    }

    return nullptr;
}

Widget * WidgetParent::get_previous_focus(Widget * w)
{
    int index;
    if (!w) {
        if ((index = this->impl->get_last()) == -1) {
            return nullptr;
        }

        w = this->impl->get(index);
        if ((w->tab_flag != Widget::IGNORE_TAB) && (w->focus_flag != Widget::IGNORE_FOCUS)) {
            return w;
        }
    }
    else {
        index = this->impl->find(w);
        assert(index != -1);
    }

    int future_focus_index;
    while ((future_focus_index = this->impl->get_previous(index)) != -1) {
        Widget * future_focus_w = this->impl->get(future_focus_index);
        if ((future_focus_w->tab_flag != Widget::IGNORE_TAB) && (future_focus_w->focus_flag != Widget::IGNORE_FOCUS)) {
            return future_focus_w;
        }

        if (future_focus_w == w) {
            break;
        }

        index = future_focus_index;
    }

    return nullptr;
}

void WidgetParent::add_widget(Widget * w)
{
    this->impl->add(w);

    if (!this->current_focus &&
        (w->tab_flag != Widget::IGNORE_TAB) && (w->focus_flag != Widget::IGNORE_FOCUS)) {
        this->current_focus = w;
    }
}

void WidgetParent::remove_widget(Widget * w)
{
    if (this->current_focus == w) {
        Widget * future_focus_w = this->get_next_focus(w);
        if (not future_focus_w) {
            future_focus_w = this->get_previous_focus(w);
        }
        this->current_focus = future_focus_w;
    }

    this->impl->remove(w);
}

int  WidgetParent::find_widget(Widget * w)
{
    return this->impl->find(w);
}

void WidgetParent::clear()
{
    this->impl->clear();

    this->current_focus = nullptr;
}

void WidgetParent::invalidate_children(Rect clip)
{
    int index_w_current = this->impl->get_first();
    while (index_w_current != -1) {
        Widget * w = this->impl->get(index_w_current);
        assert(w);

        Rect newr = clip.intersect(w->get_rect());

        if (!newr.isempty()) {
            w->rdp_input_invalidate(newr);
        }

        index_w_current = this->impl->get_next(index_w_current);
    }
}

void WidgetParent::draw_inner_free(Rect clip, Color bg_color)
{
    SubRegion region;
    region.add_rect(clip.intersect(this->get_rect()));

    int index_w_current = this->impl->get_first();
    while (index_w_current != -1) {
        Widget * w = this->impl->get(index_w_current);
        assert(w);

        Rect rect_widget = clip.intersect(w->get_rect());
        if (!rect_widget.isempty()) {
            region.subtract_rect(rect_widget);
        }

        index_w_current = this->impl->get_next(index_w_current);
    }

    ::fill_region(this->drawable, region, bg_color);
}

void WidgetParent::move_xy(int16_t x, int16_t y)
{
    this->set_xy(this->x() + x, this->y() + y);

    this->move_children_xy(x, y);
}

void WidgetParent::move_children_xy(int16_t x, int16_t y)
{
    int index_w_first = this->impl->get_first();
    if (index_w_first != -1) {
        int index_w_current = index_w_first;
        do {
            Widget * w = this->impl->get(index_w_current);
            assert(w);
            w->move_xy(x, y);

            index_w_current = this->impl->get_next(index_w_current);
        }
        while ((index_w_current != index_w_first) &&
                (index_w_current != -1));
    }
}

bool WidgetParent::next_focus()
{
    if (this->current_focus) {
        if (this->current_focus->next_focus()) {
            return true;
        }

        Widget * future_focus_w = this->get_next_focus(this->current_focus);

        if (future_focus_w) {
            this->set_widget_focus(future_focus_w, focus_reason_tabkey);

            return true;
        }

        this->current_focus->blur();
        this->current_focus = this->get_next_focus(nullptr);
        assert(this->current_focus);
    }

    return false;
}

bool WidgetParent::previous_focus()
{
    if (this->current_focus) {
        if (this->current_focus->previous_focus()) {
            return true;
        }

        Widget * future_focus_w = this->get_previous_focus(this->current_focus);

        if (future_focus_w) {
            this->set_widget_focus(future_focus_w, focus_reason_backtabkey);

            return true;
        }

        this->current_focus->blur();
        this->current_focus = this->get_previous_focus(nullptr);
        assert(this->current_focus);
    }

    return false;
}

Widget * WidgetParent::widget_at_pos(int16_t x, int16_t y)
{
    if (!this->get_rect().contains_pt(x, y)) {
        return nullptr;
    }
    if (this->current_focus) {
        if (this->current_focus->get_rect().contains_pt(x, y)) {
            return this->current_focus;
        }
    }
    // Foreground widget is the last in the list.
    int index_w_current = this->impl->get_last();
    while (index_w_current != -1) {
        Widget * w = this->impl->get(index_w_current);
        assert(w);
        if (w->get_rect().contains_pt(x, y)) {
            return w;
        }

        index_w_current = this->impl->get_previous(index_w_current);
    }

    return nullptr;
}

void WidgetParent::rdp_input_invalidate(Rect clip)
{
    Rect rect_intersect = clip.intersect(this->get_rect());

    if (!rect_intersect.isempty()) {
        this->drawable.begin_update();

        this->draw_inner_free(rect_intersect, this->get_bg_color());
        this->invalidate_children(rect_intersect);

        this->drawable.end_update();
    }
}

void WidgetParent::rdp_input_scancode(KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap)
{
    REDEMPTION_DIAGNOSTIC_PUSH()
    REDEMPTION_DIAGNOSTIC_GCC_IGNORE("-Wswitch-enum")
    switch (keymap.last_kevent()){
        case Keymap::KEvent::Tab:
            this->next_focus();
            break;

        case Keymap::KEvent::BackTab:
            this->previous_focus();
            break;

        default:
            if (this->current_focus) {
                this->current_focus->rdp_input_scancode(flags, scancode, event_time, keymap);
            }
            break;
    }
    REDEMPTION_DIAGNOSTIC_POP()
}

void WidgetParent::rdp_input_unicode(KbdFlags flag, uint16_t unicode)
{
    if (this->current_focus) {
        this->current_focus->rdp_input_unicode(flag, unicode);
    }
}

void WidgetParent::rdp_input_mouse(int device_flags, int x, int y)
{
    if (device_flags & (MOUSE_FLAG_WHEEL | MOUSE_FLAG_HWHEEL)) {
        x = this->old_mouse_x;
        y = this->old_mouse_y;
    }
    else {
        this->old_mouse_x = x;
        this->old_mouse_y = y;
    }

    Widget * w = this->widget_at_pos(x, y);

    // Mouse clic release
    // w could be null if mouse is located at an empty space
    if (device_flags == MOUSE_FLAG_BUTTON1) {
        if (this->pressed
            && (w != this->pressed)) {
            this->pressed->rdp_input_mouse(device_flags, x, y);
        }
        this->pressed = nullptr;
    }
    if (w) {
        // get focus when mouse clic
        if (device_flags == (MOUSE_FLAG_BUTTON1 | MOUSE_FLAG_DOWN)) {
            this->pressed = w;
            if (/*(*/w->focus_flag != IGNORE_FOCUS/*) && (w != this->current_focus)*/) {
                this->set_widget_focus(w, focus_reason_mousebutton1);
            }
        }
        w->rdp_input_mouse(device_flags, x, y);
    }
    else {
        Widget::rdp_input_mouse(device_flags, x, y);
    }
    if (device_flags == MOUSE_FLAG_MOVE && this->pressed) {
        this->pressed->rdp_input_mouse(device_flags, x, y);
    }
}
