/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2013
   Author(s): Christophe Grosjean, Meng Tan

*/

#include "test_only/test_framework/redemption_unit_tests.hpp"

#include "configs/config.hpp"
#include "core/RDP/capabilities/window.hpp"
#include "RAIL/client_execute.hpp"
#include "mod/internal/login_mod.hpp"
#include "gdi/graphic_api.hpp"
#include "keyboard/keymap.hpp"
#include "keyboard/keylayouts.hpp"
#include "test_only/front/fake_front.hpp"
#include "test_only/core/font.hpp"
#include "core/events.hpp"

RED_AUTO_TEST_CASE(TestLoginMod)
{
    ScreenInfo screen_info{800, 600, BitsPerPixel{24}};
    FakeFront front(screen_info);
    WindowListCaps window_list_caps;
    EventContainer events;
    ClientExecute client_execute(events.get_time_base(), front.gd(), front, window_list_caps, false);

    Inifile ini;
    Theme theme;

    Keymap keymap(*find_layout_by_id(KeyLayout::KbdId(0x040C)));

    RED_CHECK_NE(ini.get<cfg::globals::auth_user>(), "user");
    RED_CHECK_NE(ini.get<cfg::context::password>(), "pass");

    LoginMod d(ini, events, "user", "pass", front.gd(), front, screen_info.width, screen_info.height,
               Rect(0, 0, 799, 599), client_execute, global_font(), theme);
    d.init();

    keymap.event(Keymap::KbdFlags(), Keymap::Scancode(0x1c)); // enter
    d.rdp_input_scancode(Keymap::KbdFlags(), Keymap::Scancode(0x1c), 0, keymap);

    RED_CHECK_EQUAL(ini.get<cfg::globals::auth_user>(), "user");
    RED_CHECK_EQUAL(ini.get<cfg::context::password>(), "pass");
}

RED_AUTO_TEST_CASE(TestLoginMod2)
{
    ScreenInfo screen_info{2048, 1536, BitsPerPixel{24}};
    FakeFront front(screen_info);
    WindowListCaps window_list_caps;
    EventManager event_manager;
    auto& events = event_manager.get_events();
    ClientExecute client_execute(events.get_time_base(), front.gd(), front, window_list_caps, false);

    Inifile ini;
    Theme theme;

    ini.set<cfg::globals::authentication_timeout>(std::chrono::seconds(1));

    LoginMod d(ini, events, "user", "pass", front.gd(), front, screen_info.width, screen_info.height,
               Rect(1024, 768, 1023, 767), client_execute, global_font(), theme);
    d.init();

    using namespace std::literals::chrono_literals;

    event_manager.execute_events([](int){return false;}, false);
    RED_CHECK_EQUAL(BACK_EVENT_NONE, d.get_mod_signal());

    event_manager.get_writable_time_base().monotonic_time = MonotonicTimePoint{2s + 1us};
    event_manager.execute_events([](int){return false;}, false);
    RED_CHECK_EQUAL(BACK_EVENT_STOP, d.get_mod_signal());
}
