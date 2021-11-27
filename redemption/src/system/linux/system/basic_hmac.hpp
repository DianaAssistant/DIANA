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
*   Copyright (C) Wallix 2010-2016
*   Author(s): Jonathan Poelen
*/

#pragma once

#include "core/error.hpp"
#include "utils/sugar/bytes_view.hpp"
#include "utils/sugar/bounded_array_view.hpp"

#include <cstdint>
#include <cstring>

#include <openssl/hmac.h>


namespace detail_
{

struct HMACWrap
{
# if OPENSSL_VERSION_NUMBER < 0x10100000L
    void init() { HMAC_CTX_init(&this->hmac); }
    void deinit() { HMAC_CTX_cleanup(&this->hmac); }
    HMAC_CTX * operator->() noexcept { return &this->hmac; }
    operator HMAC_CTX * () noexcept { return &this->hmac; }

private:
    HMAC_CTX hmac;
# else
    void init() { this->hmac = HMAC_CTX_new(); }
    void deinit() { HMAC_CTX_free(this->hmac); }
    HMAC_CTX * operator->() noexcept { return this->hmac; }
    operator HMAC_CTX * () noexcept { return this->hmac; }

private:
    HMAC_CTX * hmac = nullptr;
# endif
};

template<const EVP_MD * (* evp)(), std::size_t DigestLength>
class basic_HMAC
{
    HMACWrap hmac;

public:
    basic_HMAC(bytes_view key)
    {
        this->hmac.init();
        int res = HMAC_Init_ex(this->hmac, key.as_u8p(), key.size(), evp(), nullptr);
        if (res == 0) {
            throw Error(ERR_SSL_CALL_HMAC_INIT_FAILED);
        }
    }

    ~basic_HMAC()
    {
        this->hmac.deinit();
    }

    void update(bytes_view data)
    {
        int res = HMAC_Update(this->hmac, data.as_u8p(), data.size());
        if (res == 0) {
            throw Error(ERR_SSL_CALL_HMAC_UPDATE_FAILED);
        }
    }

    void final(sized_writable_u8_array_view<DigestLength> out_data)
    {
        unsigned int len = 0;
        int res = HMAC_Final(this->hmac, out_data.data(), &len);
        if (res == 0) {
            throw Error(ERR_SSL_CALL_HMAC_FINAL_FAILED);
        }
        assert(len == DigestLength);
    }
};

template<const EVP_MD * (* evp)(), std::size_t DigestLength>
class DelayedHMAC
{
    bool initialized = false;
    HMACWrap hmac;

public:
    DelayedHMAC() = default;

    void init(bytes_view key)
    {
        if (this->initialized){
            throw Error(ERR_SSL_CALL_HMAC_INIT_FAILED);
        }
        this->hmac.init();
        int res = HMAC_Init_ex(this->hmac, key.as_u8p(), key.size(), evp(), nullptr);
        if (res == 0) {
            throw Error(ERR_SSL_CALL_HMAC_INIT_FAILED);
        }
        this->initialized = true;
    }

    ~DelayedHMAC()
    {
        if (this->initialized){
            this->hmac.deinit();
        }
    }

    void update(bytes_view data)
    {
        if (!this->initialized){
            throw Error(ERR_SSL_CALL_HMAC_UPDATE_FAILED);
        }
        int res = HMAC_Update(this->hmac, data.as_u8p(), data.size());
        if (res == 0) {
            throw Error(ERR_SSL_CALL_HMAC_UPDATE_FAILED);
        }
    }

    void final(sized_writable_u8_array_view<DigestLength> out_data)
    {
        if (!this->initialized){
            throw Error(ERR_SSL_CALL_HMAC_FINAL_FAILED);
        }
        unsigned int len = 0;
        int res = HMAC_Final(this->hmac, out_data.data(), &len);
        if (res == 0) {
            throw Error(ERR_SSL_CALL_HMAC_FINAL_FAILED);
        }
        assert(len == DigestLength);
        this->hmac.deinit();
        this->initialized = false;
    }
};

}  // namespace detail_
