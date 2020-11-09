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
  Copyright (C) Wallix 2019
  Author(s): Christophe Grosjean

    ModFactory : Factory class used to instanciate BackEnd modules

*/

#pragma once

#include "utils/sugar/not_null_ptr.hpp"

class mod_api;
class rdp_api;
class windowing_api;
class RailModuleHostMod;
class SocketTransport;

struct ModPack
{
    not_null_ptr<mod_api> mod;
    rdp_api* rdpapi;
    windowing_api* winapi;
    RailModuleHostMod* rail_module_host_ptr;
    bool enable_osd;
    bool connected;
    SocketTransport* psocket_transport;
};
