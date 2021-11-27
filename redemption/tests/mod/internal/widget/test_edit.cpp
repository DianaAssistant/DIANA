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

#include "mod/internal/widget/edit.hpp"
#include "mod/internal/widget/screen.hpp"
#include "keyboard/keymap.hpp"
#include "keyboard/keylayouts.hpp"
#include "test_only/gdi/test_graphic.hpp"
#include "test_only/core/font.hpp"
#include "test_only/mod/internal/widget/notify_trace.hpp"


#define IMG_TEST_PATH FIXTURES_PATH "/img_ref/mod/internal/widget/edit/"


RED_AUTO_TEST_CASE(TraceWidgetEdit)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget at position 0,0 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = 0;
    int16_t y = 0;
    uint16_t cx = 50;
    int xtext = 4;
    int ytext = 1;
    size_t edit_pos = 2;

    WidgetEdit wedit(drawable, parent, notifier, "test1", id,
                     fg_color, bg_color, bg_color, global_font_deja_vu_14(), edit_pos, xtext, ytext);
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at it's current position
    wedit.rdp_input_invalidate(Rect(wedit.x(),
                                    wedit.x(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_1.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEdit2)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position 10,100 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = 10;
    int16_t y = 100;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test2", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at it's current position
    wedit.rdp_input_invalidate(Rect(wedit.x(),
                                    wedit.y(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_2.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEdit3)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position -10,500 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = -10;
    int16_t y = 500;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test3", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at it's current position
    wedit.rdp_input_invalidate(Rect(wedit.x(),
                                    wedit.y(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_3.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEdit4)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position 770,500 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = 770;
    int16_t y = 500;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test4", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at it's current position
    wedit.rdp_input_invalidate(Rect(wedit.x(),
                                    wedit.y(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_5.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEdit5)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position -20,-7 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = -20;
    int16_t y = -7;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test5", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at it's current position
    wedit.rdp_input_invalidate(Rect(wedit.x(),
                                    wedit.y(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_6.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEdit6)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position 760,-7 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = 760;
    int16_t y = -7;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test6", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at it's current position
    wedit.rdp_input_invalidate(Rect(wedit.x(),
                                    wedit.y(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_7.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEditClip)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position 760,-7 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = 760;
    int16_t y = -7;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test6", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at position 780,-7 and of size 120x20. After clip the size is of 20x13
    wedit.rdp_input_invalidate(Rect(20 + wedit.x(),
                                    wedit.y(),
                                    wedit.cx(),
                                    wedit.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_8.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEditClip2)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 100x20 at position 10,7 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;
    BGRColor fg_color = RED;
    BGRColor bg_color = YELLOW;
    int id = 0;
    int16_t x = 0;
    int16_t y = 0;
    uint16_t cx = 50;

    WidgetEdit wedit(drawable, parent, notifier, "test6", id, fg_color, bg_color, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    // ask to widget to redraw at position 30,12 and of size 30x10.
    wedit.rdp_input_invalidate(Rect(20 + wedit.x(),
                                    5 + wedit.y(),
                                    30,
                                    10));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_9.png");
}

RED_AUTO_TEST_CASE(EventWidgetEdit)
{
    TestGraphic drawable(800, 600);

    NotifyTrace notifier;

    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    int16_t x = 0;
    int16_t y = 0;
    uint16_t cx = 100;

    WidgetEdit wedit(drawable, parent, &notifier, "abcdef", 0, GREEN, RED, RED, global_font_deja_vu_14());
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    wedit.focus(Widget::focus_reason_tabkey);

    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_10.png");

    Keymap keymap(*find_layout_by_id(KeyLayout::KbdId(0x040C)));

    auto rdp_input_scancode = [&](uint16_t scancode_and_flags){
        using KFlags = Keymap::KbdFlags;
        using Scancode = Keymap::Scancode;
        auto scancode = Scancode(scancode_and_flags);
        auto flags = KFlags(scancode_and_flags & 0xff00u);
        keymap.event(flags, scancode);
        wedit.rdp_input_scancode(flags, scancode, 0, keymap);
        keymap.event(flags | KFlags::Release, scancode);
        wedit.rdp_input_scancode(flags | KFlags::Release, scancode, 0, keymap);
    };

    rdp_input_scancode(0x10); // 'a'
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_11.png");
    RED_CHECK(notifier.last_widget == &wedit);
    RED_CHECK(notifier.last_event == NOTIFY_TEXT_CHANGED);
    notifier.last_widget = nullptr;
    notifier.last_event = 0;

    rdp_input_scancode(0x11); // 'z'
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_12.png");
    RED_CHECK(notifier.last_widget == &wedit);
    RED_CHECK(notifier.last_event == NOTIFY_TEXT_CHANGED);
    notifier.last_widget = nullptr;
    notifier.last_event = 0;

    rdp_input_scancode(0x148); // up
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_13.png");
    RED_CHECK(notifier.last_widget == nullptr);
    RED_CHECK(notifier.last_event == 0);

    rdp_input_scancode(0x14d); // right
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_12.png");

    rdp_input_scancode(0x0e); // backspace
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_11.png");

    rdp_input_scancode(0x14b); // left
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_16.png");

    rdp_input_scancode(0x14b); // left
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_17.png");

    rdp_input_scancode(0x153); // delete
    RED_CHECK(notifier.last_widget == nullptr);
    RED_CHECK(notifier.last_event == 0);
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_18.png");

    rdp_input_scancode(0x14f); // end
    RED_CHECK(notifier.last_widget == nullptr);
    RED_CHECK(notifier.last_event == 0);
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_19.png");

    rdp_input_scancode(0x147); // home
    RED_CHECK(notifier.last_widget == nullptr);
    RED_CHECK(notifier.last_event == 0);
    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_20.png");

    RED_CHECK(notifier.last_widget == nullptr);
    RED_CHECK(notifier.last_event == 0);
    rdp_input_scancode(0x1c); // enter
    RED_CHECK(notifier.last_widget == &wedit);
    RED_CHECK(notifier.last_event == NOTIFY_SUBMIT);
    notifier.last_widget = nullptr;
    notifier.last_event = 0;

    wedit.rdp_input_mouse(MOUSE_FLAG_BUTTON1|MOUSE_FLAG_DOWN, x+10, y+3);
    RED_CHECK(notifier.last_widget == nullptr);
    RED_CHECK(notifier.last_event == 0);

    wedit.rdp_input_invalidate(Rect(0, 0, wedit.cx(), wedit.cx()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_21.png");
}

RED_AUTO_TEST_CASE(TraceWidgetEditAndComposite)
{
    TestGraphic drawable(800, 600);


    // WidgetEdit is a edit widget of size 256x125 at position 0,0 in it's parent context
    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    NotifyApi * notifier = nullptr;

    WidgetComposite wcomposite(drawable, parent, notifier);
    wcomposite.set_wh(800, 600);
    wcomposite.set_xy(0, 0);

    WidgetEdit wedit1(drawable, wcomposite, notifier,
                      "abababab", 4, YELLOW, BLACK, BLACK, global_font_deja_vu_14());
    Dimension dim = wedit1.get_optimal_dim();
    wedit1.set_wh(50, dim.h);
    wedit1.set_xy(0, 0);

    WidgetEdit wedit2(drawable, wcomposite, notifier,
                      "ggghdgh", 2, WHITE, RED, RED, global_font_deja_vu_14());
    dim = wedit2.get_optimal_dim();
    wedit2.set_wh(50, dim.h);
    wedit2.set_xy(0, 100);

    WidgetEdit wedit3(drawable, wcomposite, notifier,
                      "lldlslql", 1, BLUE, RED, RED, global_font_deja_vu_14());
    dim = wedit3.get_optimal_dim();
    wedit3.set_wh(50, dim.h);
    wedit3.set_xy(100, 100);

    WidgetEdit wedit4(drawable, wcomposite, notifier,
                      "LLLLMLLM", 20, PINK, DARK_GREEN, DARK_GREEN, global_font_deja_vu_14());
    dim = wedit4.get_optimal_dim();
    wedit4.set_wh(50, dim.h);
    wedit4.set_xy(300, 300);

    WidgetEdit wedit5(drawable, wcomposite, notifier,
                      "dsdsdjdjs", 0, LIGHT_GREEN, DARK_BLUE, DARK_BLUE, global_font_deja_vu_14());
    dim = wedit5.get_optimal_dim();
    wedit5.set_wh(50, dim.h);
    wedit5.set_xy(700, -10);

    WidgetEdit wedit6(drawable, wcomposite, notifier,
                      "xxwwp", 2, ANTHRACITE, PALE_GREEN, PALE_GREEN, global_font_deja_vu_14());
    dim = wedit6.get_optimal_dim();
    wedit6.set_wh(50, dim.h);
    wedit6.set_xy(-10, 550);

    wcomposite.add_widget(&wedit1);
    wcomposite.add_widget(&wedit2);
    wcomposite.add_widget(&wedit3);
    wcomposite.add_widget(&wedit4);
    wcomposite.add_widget(&wedit5);
    wcomposite.add_widget(&wedit6);

    // ask to widget to redraw at position 100,25 and of size 100x100.
    wcomposite.rdp_input_invalidate(Rect(100, 25, 100, 100));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_22.png");

    // ask to widget to redraw at it's current position
    wcomposite.rdp_input_invalidate(Rect(0, 0, wcomposite.cx(), wcomposite.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_23.png");

    wcomposite.clear();
}

RED_AUTO_TEST_CASE(TraceWidgetEditScrolling)
{
    TestGraphic drawable(800, 600);


    WidgetScreen parent(drawable, 800, 600, global_font_deja_vu_14(), nullptr, Theme{});

    int16_t x = 0;
    int16_t y = 0;
    uint16_t cx = 100;

    WidgetEdit wedit(drawable, parent, &parent, "abcde", 0, BLACK, WHITE, WHITE, global_font_deja_vu_14(),
                     -1u, 1, 1);
    Dimension dim = wedit.get_optimal_dim();
    wedit.set_wh(cx, dim.h);
    wedit.set_xy(x, y);

    wedit.focus(Widget::focus_reason_tabkey);
    parent.add_widget(&wedit);
    parent.current_focus = &wedit;

    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));

    Keymap keymap(*find_layout_by_id(KeyLayout::KbdId(0x040C)));

    auto rdp_input_scancode = [&](uint16_t scancode_and_flags){
        using KFlags = Keymap::KbdFlags;
        using Scancode = Keymap::Scancode;
        auto scancode = Scancode(scancode_and_flags);
        auto flags = KFlags(scancode_and_flags & 0xff00u);
        keymap.event(flags, scancode);
        wedit.rdp_input_scancode(flags, scancode, 0, keymap);
        keymap.event(flags | KFlags::Release, scancode);
        wedit.rdp_input_scancode(flags | KFlags::Release, scancode, 0, keymap);
    };

    rdp_input_scancode(0x10); // 'a'

    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_24.png");

    rdp_input_scancode(0x11); // 'z'
    rdp_input_scancode(0x12); // 'e'
    rdp_input_scancode(0x10); // 'a'
    rdp_input_scancode(0x10); // 'a'
    rdp_input_scancode(0x10); // 'a'
    rdp_input_scancode(0x10); // 'a'
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_25.png");

    rdp_input_scancode(0x10); // 'a'
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_26.png");

    rdp_input_scancode(0x10); // 'a'
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_27.png");

    rdp_input_scancode(0x19); // 'p'
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_28.png");

    rdp_input_scancode(0x147); // home
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_29.png");

    rdp_input_scancode(0x14f); // end
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_28.png");

    rdp_input_scancode(0x0e); // backspace
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));
    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_31.png");

    for (int i = 0; i < 10; i++) {
        rdp_input_scancode(0x14b); // left
    }
    parent.rdp_input_invalidate(Rect(0, 0, parent.cx(), parent.cy()));

    RED_CHECK_IMG(drawable, IMG_TEST_PATH "edit_32.png");

    parent.clear();
}
