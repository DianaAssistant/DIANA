/*
    This program is free software; you can redistribute it and/or modify it
     under the terms of the GNU General Public License as published by the
     Free Software Foundation; either version 2 of the License, or (at your
     option) any later version.

    This program is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
     Public License for more details.

    You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     675 Mass Ave, Cambridge, MA 02139, USA.

    Product name: redemption, a FLOSS RDP proxy
    Copyright (C) Wallix 2013
    Author(s): Christophe Grosjean, Meng Tan, Jonathan Poelen, Raphael Zhou
*/

#include "mod/internal/rail_module_host_mod.hpp"

#include "utils/timebase.hpp"
#include "mod/null/null.hpp"
#include "RAIL/client_execute.hpp"
#include "keyboard/keymap.hpp"


using namespace std::chrono_literals;

void RailModuleHostMod::rdp_input_invalidate(Rect r)
{
    this->screen.rdp_input_invalidate(r);

    if (this->rail_enabled) {
        this->rail_client_execute.input_invalidate(r);
    }
}

void RailModuleHostMod::rdp_input_scancode(
    KbdFlags flags, Scancode scancode, uint32_t event_time, Keymap const& keymap)
{
    this->screen.rdp_input_scancode(flags, scancode, event_time, keymap);

    if (this->rail_enabled
     && keymap.last_kevent() == Keymap::KEvent::F4
     && keymap.is_alt_pressed()
    ) {
        LOG(LOG_INFO, "RailModuleHostMod::rdp_input_scancode: Close by user (Alt+F4)");
        throw Error(ERR_WIDGET);
    }
}

RailModuleHostMod::RailModuleHostMod(
    EventContainer& events,
    gdi::GraphicApi & drawable,
    FrontAPI& front, uint16_t width, uint16_t height,
    Rect const widget_rect, ClientExecute& rail_client_execute,
    Font const& font, Theme const& theme,
    const GCC::UserData::CSMonitor& cs_monitor, bool can_resize_hosted_desktop)
    : front(front)
    , screen(drawable, width, height, font, nullptr, theme)
    , rail_client_execute(rail_client_execute)
    , dvc_manager(false)
    , disconnection_reconnection_timer(events)
    , rail_enabled(rail_client_execute.is_rail_enabled())
    , managed_mod(std::make_unique<null_mod>())
    , module_host(drawable, this->screen, /*notifier=*/nullptr,
                  *this->managed_mod, font, cs_monitor,
                  widget_rect, width, height)
    , can_resize_hosted_desktop(can_resize_hosted_desktop)
{
    this->screen.set_xy(widget_rect.x, widget_rect.y);
    this->screen.set_wh(width, height);
    this->screen.add_widget(&this->module_host);
    this->screen.set_widget_focus(&this->module_host, Widget::focus_reason_tabkey);
}

void RailModuleHostMod::set_mod(std::unique_ptr<mod_api>&& managed_mod) noexcept
{
    this->managed_mod = std::move(managed_mod);
    this->module_host.set_mod(*this->managed_mod);
}

RailModuleHostMod::~RailModuleHostMod()
{
    this->rail_client_execute.reset(true);
    this->screen.clear();
}

void RailModuleHostMod::init()
{
    if (this->rail_enabled && !this->rail_client_execute.is_ready()) {
        this->rail_client_execute.ready(
            *this, this->font(), this->is_resizing_hosted_desktop_allowed());
        this->dvc_manager.ready(this->front);
    }
}

// RdpInput
void RailModuleHostMod::rdp_gdi_up_and_running()
{
    mod_api& mod = this->module_host.get_managed_mod();
    mod.rdp_gdi_up_and_running();
}

void RailModuleHostMod::rdp_input_mouse(int device_flags, int x, int y)
{
    Rect client_execute_auxiliary_window_rect = this->rail_client_execute.get_auxiliary_window_rect();

    if (!client_execute_auxiliary_window_rect.isempty()
     && client_execute_auxiliary_window_rect.contains_pt(x, y)
    ) {
        mod_api& mod = this->module_host.get_managed_mod();

        mod.rdp_input_mouse(device_flags, x, y);
    }
    else {
        if (device_flags & (MOUSE_FLAG_WHEEL | MOUSE_FLAG_HWHEEL)) {
            x = this->old_mouse_x;
            y = this->old_mouse_y;
        }
        else {
            this->old_mouse_x = x;
            this->old_mouse_y = y;
        }

        if (!this->rail_enabled) {
            this->screen.rdp_input_mouse(device_flags, x, y);
            return;
        }

        bool mouse_is_captured
          = this->rail_client_execute.input_mouse(device_flags, x, y);

        if (mouse_is_captured) {
            this->screen.allow_mouse_pointer_change(false);
            this->current_mouse_owner = MouseOwner::ClientExecute;
        }
        else {
            if (MouseOwner::WidgetModule != this->current_mouse_owner) {
                this->screen.redo_mouse_pointer_change(x, y);
            }

            this->current_mouse_owner = MouseOwner::WidgetModule;
        }

        this->screen.rdp_input_mouse(device_flags, x, y);

        if (mouse_is_captured) {
            this->screen.allow_mouse_pointer_change(true);
        }
    }
}

// Callback

void RailModuleHostMod::send_to_mod_channel(
    CHANNELS::ChannelNameId front_channel_name,
    InStream& chunk, size_t length, uint32_t flags)
{
    if (front_channel_name == CHANNELS::channel_names::rail){
        if (this->rail_enabled
         && this->rail_client_execute.is_ready()
        ) {
            this->rail_client_execute.send_to_mod_rail_channel(length, chunk, flags);
        }
        return;
    }

    if (this->rail_enabled
     && this->rail_client_execute.is_ready()
     && front_channel_name == CHANNELS::channel_names::drdynvc
    ) {
        this->dvc_manager.send_to_mod_drdynvc_channel(length, chunk, flags);
    }

    mod_api& mod = this->module_host.get_managed_mod();
    mod.send_to_mod_channel(front_channel_name, chunk, length, flags);
}

// mod_api

bool RailModuleHostMod::is_up_and_running() const
{
    return this->module_host.get_managed_mod().is_up_and_running();
}

bool RailModuleHostMod::is_auto_reconnectable() const
{
    return this->module_host.get_managed_mod().is_auto_reconnectable();
}

bool RailModuleHostMod::server_error_encountered() const
{
    return this->module_host.get_managed_mod().server_error_encountered();
}

void RailModuleHostMod::move_size_widget(int16_t left, int16_t top, uint16_t width,
                        uint16_t height)
{
    Dimension dim = this->get_dim();

    this->module_host.set_xy(left, top);
    this->module_host.set_wh(width, height);

    if (dim.w && dim.h && ((dim.w != width) || (dim.h != height)) &&
        this->rail_client_execute.is_resizing_hosted_desktop_enabled()
    ) {
        this->disconnection_reconnection_timer.reset_timeout_or_create_event(
            1s, "RAIL Module Host Disconnection Reconnection Timeout",
            [this](Event&)
            {
                // TODO there is a channel to resize the session, we should use it
                if (this->module_host.get_managed_mod().is_auto_reconnectable()){
                    throw Error(ERR_RAIL_RESIZING_REQUIRED);
                }
            }
        );
    }
}

Dimension RailModuleHostMod::get_dim() const
{
    const mod_api& mod = this->module_host.get_managed_mod();

    return mod.get_dim();
}

bool RailModuleHostMod::is_resizing_hosted_desktop_allowed() const
{
    return this->can_resize_hosted_desktop;
}

void RailModuleHostMod::acl_update(AclFieldMask const& acl_fields)
{
    return this->module_host.get_managed_mod().acl_update(acl_fields);
}
