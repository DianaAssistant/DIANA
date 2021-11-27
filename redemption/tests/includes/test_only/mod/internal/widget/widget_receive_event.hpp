/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2021
Author(s): Proxies Team
*/

#pragma once

#include "mod/internal/widget/widget.hpp"

struct WidgetReceiveEvent : public Widget
{
    Widget* last_widget = nullptr;
    NotifyApi::notify_event_t last_event = 0;

    WidgetReceiveEvent(gdi::GraphicApi& gd)
    : Widget(gd, *this, nullptr)
    {}

    WidgetReceiveEvent(gdi::GraphicApi& gd, Widget& parent)
    : Widget(gd, parent, nullptr)
    {}

    void rdp_input_invalidate(Rect /*r*/) override
    {}

    void notify(Widget& widget, NotifyApi::notify_event_t event) override
    {
        this->last_widget = &widget;
        this->last_event = event;
    }
};
