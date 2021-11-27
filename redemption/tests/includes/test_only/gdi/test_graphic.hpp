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
 *   Copyright (C) Wallix 2010-2014
 *   Author(s): Christophe Grosjean, Raphael Zhou, Jonathan Poelen, Meng Tan
 */

#pragma once

#include "utils/image_view.hpp"

#include <memory>


namespace gdi
{
    class GraphicApi;
    class ImageFrameApi;
}

struct TestGraphic
{
    TestGraphic(uint16_t w, uint16_t h);
    ~TestGraphic();

    [[nodiscard]] uint16_t width() const;
    [[nodiscard]] uint16_t height() const;

    operator ImageView () const;
    operator gdi::GraphicApi& ();
    operator gdi::ImageFrameApi& ();
    gdi::GraphicApi* operator->();

    void resize(uint16_t w, uint16_t h);

    WritableImageView get_writable_image_view();

private:
    class D;
    std::unique_ptr<D> d;
};
