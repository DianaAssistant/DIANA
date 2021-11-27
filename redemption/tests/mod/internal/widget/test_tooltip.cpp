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

#include "test_only/test_framework/redemption_unit_tests.hpp"
#include "test_only/test_framework/check_img.hpp"

#include "mod/internal/widget/tooltip.hpp"
#include "mod/internal/widget/screen.hpp"
#include "mod/internal/widget/label.hpp"

#include "test_only/gdi/test_graphic.hpp"
#include "test_only/core/font.hpp"


#define IMG_TEST_PATH FIXTURES_PATH "/img_ref/mod/internal/widget/tooltip/"

RED_AUTO_TEST_CASE(TraceWidgetTooltip)
{
    TestGraphic drawable(800, 600);

    // WidgetTooltip is a tooltip widget at position 0,0 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_lato_light_16(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    BGRColor border_color = BLACK;
    int16_t x = 10;
    int16_t y = 10;
    const char * tooltiptext = "testémq";

    WidgetTooltip wtooltip(drawable, parent, notifier, tooltiptext, fg_color, bg_color, border_color, global_font_lato_light_16());
    Dimension dim = wtooltip.get_optimal_dim();
    wtooltip.set_wh(dim);
    wtooltip.set_xy(x, y);

    // ask to widget to redraw
    wtooltip.rdp_input_invalidate(Rect(0, 0, 100, 100));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "tooltip_1.png");
}

RED_AUTO_TEST_CASE(TraceWidgetTooltipScreen)
{
    TestGraphic drawable(800, 600);
    int x = 50;
    int y = 20;

    // WidgetTooltip is a tooltip widget at position 0,0 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_lato_light_16(), nullptr, Theme{});

    WidgetLabel label(drawable, parent, &parent, "TOOLTIPTEST",
                      0, BLACK, WHITE, global_font_lato_light_16());
    Dimension dim = label.get_optimal_dim();
    label.set_wh(dim);
    label.set_xy(x, y);

    WidgetLabel label2(drawable, parent, &parent, "TOOLTIPTESTMULTI",
                      0, BLACK, WHITE, global_font_lato_light_16());
    dim = label2.get_optimal_dim();
    label2.set_wh(dim);
    label2.set_xy(x + 50, y + 90);

    parent.add_widget(&label);
    parent.add_widget(&label2);

    auto rdp_input_mouse = [&](WidgetLabel const& label, const char * text) {
        auto x = label.x() + label.cx() / 2;
        auto y = label.y() + label.cy() / 2;
        parent.rdp_input_mouse(MOUSE_FLAG_MOVE, x, y);
        Widget * wid = parent.widget_at_pos(x, y);
        RED_CHECK(wid == &label);
        parent.show_tooltip(wid, text, x, y, Rect(0, 0, 500, 41));
        parent.rdp_input_invalidate(parent.get_rect());
    };

    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "tooltip_2.png");

    rdp_input_mouse(label, "Test tooltip description");
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "tooltip_3.png");

    rdp_input_mouse(label2,
                    "Test tooltip\n"
                    "description in\n"
                    "multilines !");
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "tooltip_4.png");

    parent.tooltip->set_text("Test tooltip\n"
                             "Text modification\n"
                             "text has been changed !", parent.cx());
    parent.rdp_input_invalidate(parent.get_rect());

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "tooltip_5.png");

    parent.clear();
}
