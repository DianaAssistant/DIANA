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

#pragma once

#include "system/basic_hmac.hpp"

#include <cstdint>
#include <cstring>

#include <openssl/md5.h>


class SslMd5
{
    MD5_CTX md5;

public:
    enum : unsigned { DIGEST_LENGTH = MD5_DIGEST_LENGTH };

    SslMd5()
    {
        MD5_Init(&this->md5);
    }

    void update(bytes_view data)
    {
        MD5_Update(&this->md5, data.as_u8p(), data.size());
    }

    void final(sized_writable_u8_array_view<DIGEST_LENGTH> out_data)
    {
        MD5_Final(out_data.data(), &this->md5);
    }
};


using SslHMAC_Md5 = detail_::basic_HMAC<&EVP_md5, SslMd5::DIGEST_LENGTH>;
