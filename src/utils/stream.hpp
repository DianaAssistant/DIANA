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
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean, Javier Caverni
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   stream object, used for input / output communication between
   entities
*/

#ifndef _REDEMPTION_UTILS_STREAM_HPP_
#define _REDEMPTION_UTILS_STREAM_HPP_

#include "log.hpp"

#include <stdio.h>
#include <string.h> // for memcpy, memset

#include "error.hpp"
#include "bitfu.hpp"
#include "utf.hpp"

#include "parse.hpp"

#include "exchange.hpp"
#include "make_unique.hpp"

#include <new>
#include <memory>
#include <initializer_list>


// using a template for default size of stream would make sense instead of always using the large buffer below
enum {
    AUTOSIZE = 65536
};


class InStream
{
    uint8_t const * begin;
    uint8_t const * end;
    // relative coordinate between begin and end (at begin, p = 0)
    Parse p;

public:
    explicit InStream(uint8_t const * array, std::size_t len, std::size_t offset = 0)
    : begin(array)
    , end(array + len)
    , p(this->begin + offset)
    {
        assert(len >= offset);
    }

    explicit InStream(char const * array, std::size_t len, std::size_t offset = 0)
    : InStream(reinterpret_cast<uint8_t const *>(array), len, offset)
    {
    }

    template<std::size_t N>
    explicit InStream(char const (&array)[N])
    : InStream(reinterpret_cast<uint8_t const *>(array), N)
    {
    }

    template<std::size_t N>
    explicit InStream(uint8_t const (&array)[N])
    : InStream(array, N)
    {
    }

    template<class T>
    explicit InStream(T const & array)
    : InStream(array.data(), array.size())
    {
    }

    InStream() = delete;
    InStream(InStream &&) = default;
    InStream & operator=(InStream &&) = default;
    InStream & operator=(InStream const &) = delete;

private:
    InStream(InStream const &) = default;

public:
    InStream clone() const {
        return InStream(*this);
    }


    uint8_t const * get_data() const {
        return this->begin;
    }

    uint8_t const * get_current() const {
        return this->p.p;
    }

    size_t get_offset() const {
        return static_cast<size_t>(this->p.p - this->begin);
    }

    bool in_check_rem(const unsigned n) const {
    // returns true if there is enough data available to read n bytes
        return (n <= this->in_remain());
    }

    size_t in_remain() const {
        REDASSERT(this->p.p <= this->end);
        return this->end - this->p.p;
    }

    bool check_end(void) const {
        return this->p.p == this->end;
    }

    size_t get_capacity() const {
        return this->end - this->begin;
    }

    /// set current position to start buffer (\a p = \a begin)
    void rewind(std::size_t offset = 0) {
        REDASSERT(this->begin + offset <= this->end);
        this->p.p = this->begin + offset;
    }

    // =========================================================================
    // Generic binary Data access methods
    // =========================================================================

    int8_t in_sint8(void) {
        REDASSERT(this->in_check_rem(1));
        return this->p.in_sint8();
    }

    // ---------------------------------------------------------------------------

    uint8_t in_uint8(void) {
        REDASSERT(this->in_check_rem(1));
        return this->p.in_uint8();
    }

    /* Peek a byte from stream without move <p>. */
    uint8_t peek_uint8(void) {
        REDASSERT(this->in_check_rem(1));
        return *this->p.p;
    }

    int16_t in_sint16_be(void) {
        REDASSERT(this->in_check_rem(2));
        return this->p.in_sint16_be();
    }

    int16_t in_sint16_le(void) {
        REDASSERT(this->in_check_rem(2));
        return this->p.in_sint16_le();
    }

    uint16_t in_uint16_le(void) {
        REDASSERT(this->in_check_rem(2));
        return this->p.in_uint16_le();
    }

    uint16_t in_uint16_be(void) {
        REDASSERT(this->in_check_rem(2));
        return this->p.in_uint16_be();
    }

    unsigned int in_uint32_le(void) {
        REDASSERT(this->in_check_rem(4));
        return this->p.in_uint32_le();
    }

    unsigned int in_uint32_be(void) {
        REDASSERT(this->in_check_rem(4));
        return this->p.in_uint32_be();
    }

    int32_t in_sint32_le(void) {
        return this->p.in_sint32_le();
    }

    int32_t in_sint32_be(void) {
        return this->p.in_sint32_be();
    }

    // ---------------------------------------------------------------------------

    void in_timeval_from_uint64le_usec(timeval & tv)
    {
        const uint64_t movie_usec_lo = this->in_uint32_le();
        const uint64_t movie_usec_hi = this->in_uint32_le();
        const uint64_t movie_usec = (movie_usec_hi * 0x100000000LL + movie_usec_lo);
        tv.tv_usec = static_cast<uint32_t>(movie_usec % 1000000LL);
        tv.tv_sec = static_cast<uint32_t>(movie_usec / 1000000LL);
    }

    uint64_t in_uint64_le(void) {
        REDASSERT(this->in_check_rem(8));
        return this->p.in_uint64_le();
    }

    uint64_t in_uint64_be(void) {
        REDASSERT(this->in_check_rem(8));
        return this->p.in_uint64_be();
    }

    unsigned in_bytes_le(const uint8_t nb){
        REDASSERT(this->in_check_rem(nb));
        return this->p.in_bytes_le(nb);
    }

    unsigned in_bytes_be(const uint8_t nb){
        REDASSERT(this->in_check_rem(nb));
        return this->p.in_bytes_be(nb);
    }

    void in_copy_bytes(uint8_t * v, size_t n) {
        REDASSERT(this->in_check_rem(n));
        return this->p.in_copy_bytes(v, n);
    }

    void in_copy_bytes(char * v, size_t n) {
        REDASSERT(this->in_check_rem(n));
        return this->p.in_copy_bytes(v, n);
    }

    const uint8_t *in_uint8p(unsigned int n) {
        REDASSERT(this->in_check_rem(n));
        return this->p.in_uint8p(n);
    }

    void in_skip_bytes(unsigned int n) {
        REDASSERT(this->in_check_rem(n));
        return this->p.in_skip_bytes(n);
    }

    // MS-RDPEGDI : 2.2.2.2.1.2.1.2 Two-Byte Unsigned Encoding
    // =======================================================
    // (TWO_BYTE_UNSIGNED_ENCODING)

    // The TWO_BYTE_UNSIGNED_ENCODING structure is used to encode a value in
    // the range 0x0000 to 0x7FFF by using a variable number of bytes.
    // For example, 0x1A1B is encoded as { 0x9A, 0x1B }.
    // The most significant bit of the first byte encodes the number of bytes
    // in the structure.

    // c (1 bit): A 1-bit, unsigned integer field that contains an encoded
    // representation of the number of bytes in this structure. 0 implies val2 field
    // is not present, if 1 val2 is present.

    // val1 (7 bits): A 7-bit, unsigned integer field containing the most
    // significant 7 bits of the value represented by this structure.

    // val2 (1 byte): An 8-bit, unsigned integer containing the least significant
    // bits of the value represented by this structure.

    uint16_t in_2BUE()
    {
        return this->p.in_2BUE();
    }

// [MS-RDPEGDI] - 2.2.2.2.1.2.1.4 Four-Byte Unsigned Encoding
// (FOUR_BYTE_UNSIGNED_ENCODING)
// ==========================================================

// The FOUR_BYTE_UNSIGNED_ENCODING structure is used to encode a value in the
//  range 0x00000000 to 0x3FFFFFFF by using a variable number of bytes. For
//  example, 0x001A1B1C is encoded as { 0x9A, 0x1B, 0x1C }. The two most
//  significant bits of the first byte encode the number of bytes in the
//  structure.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | c |    val1   |val2 (optional)|val3 (optional)|val4 (optional)|
// +---+-----------+---------------+---------------+---------------+

// c (2 bits): A 2-bit, unsigned integer field containing an encoded
//  representation of the number of bytes in this structure.

// +----------------+----------------------------------------------------------+
// | Value          | Meaning                                                  |
// +----------------+----------------------------------------------------------+
// | ONE_BYTE_VAL   | Implies that the optional val2, val3, and val4 fields    |
// | 0              | are not present. Hence, the structure is 1 byte in size. |
// +----------------+----------------------------------------------------------+
// | TWO_BYTE_VAL   | Implies that the optional val2 field is present while    |
// | 1              | the optional val3 and val4 fields are not present.       |
// |                | Hence, the structure is 2 bytes in size.                 |
// +----------------+----------------------------------------------------------+
// | THREE_BYTE_VAL | Implies that the optional val2 and val3 fields are       |
// | 2              | present while the optional val4 fields are not present.  |
// |                | Hence, the structure is 3 bytes in size.                 |
// +----------------+----------------------------------------------------------+
// | FOUR_BYTE_VAL  | Implies that the optional val2, val3, and val4 fields    |
// | 3              | are all present. Hence, the structure is 4 bytes in      |
// |                | size.                                                    |
// +----------------+----------------------------------------------------------+

// val1 (6 bits): A 6-bit, unsigned integer field containing the most
//  significant 6 bits of the value represented by this structure.

// val2 (1 byte): An 8-bit, unsigned integer containing the second most
//  significant bits of the value represented by this structure.

// val3 (1 byte): An 8-bit, unsigned integer containing the third most
//  significant bits of the value represented by this structure.

// val4 (1 byte): An 8-bit, unsigned integer containing the least significant
//  bits of the value represented by this structure.

    uint32_t in_4BUE()
    {
        return this->p.in_4BUE();
    }

    // MS-RDPEGDI : 2.2.2.2.1.1.1.4 Delta-Encoded Points (DELTA_PTS_FIELD)
    // ===================================================================

    // ..., the delta value it  represents is encoded in a packet signed
    //  format:

    //  * If the high bit (0x80) is not set in the first encoding byte, the
    //    field is 1 byte long and is encoded as a signed delta in the lower
    //    7 bits of the byte.

    //  * If the high bit of the first encoding byte is set, the lower 7 bits
    //    of the first byte and the 8 bits of the next byte are concatenated
    //    (the first byte containing the high-order bits) to create a 15-bit
    //    signed delta value.
    int16_t in_DEP(void) {
        return this->p.in_DEP();
    }

    void in_utf16(uint16_t utf16[], size_t length)
    {
        return this->p.in_utf16(utf16, length);
    }

    // sz utf16 bytes are translated to ascci, 00 terminated
    void in_uni_to_ascii_str(uint8_t * text, size_t sz, size_t bufsz)
    {
        UTF16toUTF8(this->p.p, sz / 2, text, bufsz);
        this->p.p += sz;
    }
};


class OutStream
{
    uint8_t * begin = nullptr;
    uint8_t * end = nullptr;
    uint8_t * p = nullptr;

public:
    explicit OutStream(uint8_t * array, std::size_t len, std::size_t offset = 0)
    : begin(array)
    , end(array + len)
    , p(this->begin + offset)
    {
        assert(len >= offset);
    }

    explicit OutStream(char * array, std::size_t len, std::size_t offset = 0)
    : OutStream(reinterpret_cast<uint8_t*>(array), len, offset)
    {
    }

    template<std::size_t N>
    explicit OutStream(char (&array)[N])
    : OutStream(reinterpret_cast<uint8_t*>(array), N)
    {
    }

    template<std::size_t N>
    explicit OutStream(uint8_t (&array)[N])
    : OutStream(array, N)
    {
    }

    template<class T>
    explicit OutStream(T & array)
    : OutStream(&array[0], array.size())
    {
    }

    OutStream() = default;
    OutStream(OutStream &&) = default;
    OutStream(OutStream const &) = delete;
    OutStream & operator=(OutStream &&) = default;
    OutStream & operator=(OutStream const &) = delete;

    size_t tailroom() const {
        return  this->end - this->p;
    }

    bool has_room(size_t n) const {
        return  n <= this->tailroom();
    }

    uint8_t * get_data() const {
        return this->begin;
    }

    uint8_t * get_data_end() const {
        return this->end;
    }

    uint8_t * get_current() const {
        return this->p;
    }

    size_t get_offset() const {
        return static_cast<size_t>(this->p - this->begin);
    }

    // =========================================================================
    // Generic binary Data access methods
    // =========================================================================

    void out_timeval_to_uint64le_usec(const timeval & tv)
    {
        uint64_t usec = tv.tv_sec * 1000000ULL + tv.tv_usec;
        this->out_uint32_le(static_cast<uint32_t>(usec));
        this->out_uint32_le(static_cast<uint32_t>(usec >> 32));
    }

    // ---------------------------------------------------------------------------

    void out_uint64_le(uint64_t v) {
        REDASSERT(this->has_room(8));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p[4] = (v >> 32) & 0xFF;
        this->p[5] = (v >> 40) & 0xFF;
        this->p[6] = (v >> 48) & 0xFF;
        this->p[7] = (v >> 56) & 0xFF;
        this->p+=8;
    }

    void out_uint64_be(uint64_t v) {
        REDASSERT(this->has_room(8));
        this->p[0] = (v >> 56) & 0xFF;
        this->p[1] = (v >> 48) & 0xFF;
        this->p[2] = (v >> 40) & 0xFF;
        this->p[3] = (v >> 32) & 0xFF;
        this->p[4] = (v >> 24) & 0xFF;
        this->p[5] = (v >> 16) & 0xFF;
        this->p[6] = (v >> 8) & 0xFF;
        this->p[7] = v & 0xFF;
        this->p+=8;
    }

    void out_skip_bytes(unsigned int n) {
        REDASSERT(this->has_room(n));
        this->p+=n;
    }

    void out_uint8(uint8_t v) {
        REDASSERT(this->has_room(1));
        *(this->p++) = v;
    }

    void set_out_uint8(uint8_t v, size_t offset) {
        (this->get_data())[offset] = v;
    }

    // MS-RDPEGDI : 2.2.2.2.1.2.1.2 Two-Byte Unsigned Encoding
    // =======================================================
    // (TWO_BYTE_UNSIGNED_ENCODING)

    // The TWO_BYTE_UNSIGNED_ENCODING structure is used to encode a value in
    // the range 0x0000 to 0x7FFF by using a variable number of bytes.
    // For example, 0x1A1B is encoded as { 0x9A, 0x1B }.
    // The most significant bit of the first byte encodes the number of bytes
    // in the structure.

    // c (1 bit): A 1-bit, unsigned integer field that contains an encoded
    // representation of the number of bytes in this structure. 0 implies val2 field
    // is not present, if 1 val2 is present.

    // val1 (7 bits): A 7-bit, unsigned integer field containing the most
    // significant 7 bits of the value represented by this structure.

    // val2 (1 byte): An 8-bit, unsigned integer containing the least significant
    // bits of the value represented by this structure.

    void out_2BUE(uint16_t v){
        if (v <= 127){
            this->out_uint8(static_cast<uint8_t>(v));
        }
        else {
            this->out_uint16_be(v|0x8000);
        }
    }

// [MS-RDPEGDI] - 2.2.2.2.1.2.1.4 Four-Byte Unsigned Encoding
// (FOUR_BYTE_UNSIGNED_ENCODING)
// ==========================================================

// The FOUR_BYTE_UNSIGNED_ENCODING structure is used to encode a value in the
//  range 0x00000000 to 0x3FFFFFFF by using a variable number of bytes. For
//  example, 0x001A1B1C is encoded as { 0x9A, 0x1B, 0x1C }. The two most
//  significant bits of the first byte encode the number of bytes in the
//  structure.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | c |    val1   |val2 (optional)|val3 (optional)|val4 (optional)|
// +---+-----------+---------------+---------------+---------------+

// c (2 bits): A 2-bit, unsigned integer field containing an encoded
//  representation of the number of bytes in this structure.

// +----------------+----------------------------------------------------------+
// | Value          | Meaning                                                  |
// +----------------+----------------------------------------------------------+
// | ONE_BYTE_VAL   | Implies that the optional val2, val3, and val4 fields    |
// | 0              | are not present. Hence, the structure is 1 byte in size. |
// +----------------+----------------------------------------------------------+
// | TWO_BYTE_VAL   | Implies that the optional val2 field is present while    |
// | 1              | the optional val3 and val4 fields are not present.       |
// |                | Hence, the structure is 2 bytes in size.                 |
// +----------------+----------------------------------------------------------+
// | THREE_BYTE_VAL | Implies that the optional val2 and val3 fields are       |
// | 2              | present while the optional val4 fields are not present.  |
// |                | Hence, the structure is 3 bytes in size.                 |
// +----------------+----------------------------------------------------------+
// | FOUR_BYTE_VAL  | Implies that the optional val2, val3, and val4 fields    |
// | 3              | are all present. Hence, the structure is 4 bytes in      |
// |                | size.                                                    |
// +----------------+----------------------------------------------------------+

// val1 (6 bits): A 6-bit, unsigned integer field containing the most
//  significant 6 bits of the value represented by this structure.

// val2 (1 byte): An 8-bit, unsigned integer containing the second most
//  significant bits of the value represented by this structure.

// val3 (1 byte): An 8-bit, unsigned integer containing the third most
//  significant bits of the value represented by this structure.

// val4 (1 byte): An 8-bit, unsigned integer containing the least significant
//  bits of the value represented by this structure.


    void out_4BUE(uint32_t v){
        REDASSERT(!(v & 0xC0000000));
             if (v <= 0x3F      ) {
            this->out_uint8(static_cast<uint8_t>(v));
        }
        else if (v <= 0x3FFF    ) {
            this->out_uint8(0x40 | ((v >> 8 ) & 0x3F));
            this->out_uint8(         v        & 0xFF );
        }
        else if (v <= 0x3FFFFF  ) {
            this->out_uint8(0x80 | ((v >> 16) & 0x3F));
            this->out_uint8(        (v >> 8 ) & 0xFF );
            this->out_uint8(         v        & 0xFF );
        }
        else/* if (v <= 0x3FFFFFFF)*/ {
            this->out_uint8(0xC0 | ((v >> 24) & 0x3F));
            this->out_uint8(        (v >> 16) & 0xFF );
            this->out_uint8(        (v >> 8 ) & 0xFF );
            this->out_uint8(         v        & 0xFF );
        }
    }


    // MS-RDPEGDI : 2.2.2.2.1.1.1.4 Delta-Encoded Points (DELTA_PTS_FIELD)
    // ===================================================================

    // ..., the delta value it  represents is encoded in a packet signed
    //  format:

    //  * If the high bit (0x80) is not set in the first encoding byte, the
    //    field is 1 byte long and is encoded as a signed delta in the lower
    //    7 bits of the byte.

    //  * If the high bit of the first encoding byte is set, the lower 7 bits
    //    of the first byte and the 8 bits of the next byte are concatenated
    //    (the first byte containing the high-order bits) to create a 15-bit
    //    signed delta value.

    void out_DEP(int16_t point) {
        if (abs(point) > 0x3F) {
            uint16_t data;

            memcpy(&data, &point, sizeof(data));
            data |= 0x8000;

            if (point > 0) {
                data &= ~0x4000;
            }
            else {
                data |= 0x4000;
            }

            this->out_uint16_be(data);
        }
        else {
            int8_t  _yDelta;
            uint8_t data;

            _yDelta = point;

            memcpy(&data, &_yDelta, sizeof(data));
            data &= ~0x80;

            if (point > 0) {
                data &= ~0x40;
            }
            else {
                data |= 0x40;
            }

            this->out_uint8(data);
        }
    }

    void out_sint8(char v) {
        REDASSERT(this->has_room(1));
        *(this->p++) = v;
    }

    void out_uint16_le(unsigned int v) {
        REDASSERT(this->has_room(2));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p+=2;
    }

    void set_out_uint16_le(unsigned int v, size_t offset) {
        (this->get_data())[offset] = v & 0xFF;
        (this->get_data())[offset+1] = (v >> 8) & 0xFF;
    }

    void out_sint16_le(signed int v) {
        REDASSERT(this->has_room(2));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p+=2;
    }

    void out_uint16_be(unsigned int v) {
        REDASSERT(this->has_room(2));
        this->p[1] = v & 0xFF;
        this->p[0] = (v >> 8) & 0xFF;
        this->p+=2;
    }

    void set_out_uint16_be(unsigned int v, size_t offset) {
        (this->get_data())[offset+1] = v & 0xFF;
        (this->get_data())[offset] = (v >> 8) & 0xFF;
    }

    void out_uint32_le(unsigned int v) {
        REDASSERT(this->has_room(4));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p+=4;
    }

    void set_out_uint32_le(unsigned int v, size_t offset) {
        (this->get_data())[offset+0] = v & 0xFF;
        (this->get_data())[offset+1] = (v >> 8) & 0xFF;
        (this->get_data())[offset+2] = (v >> 16) & 0xFF;
        (this->get_data())[offset+3] = (v >> 24) & 0xFF;
    }

    void out_uint32_be(unsigned int v) {
        REDASSERT(this->has_room(4));
        this->p[0] = (v >> 24) & 0xFF;
        this->p[1] = (v >> 16) & 0xFF;
        this->p[2] = (v >> 8) & 0xFF;
        this->p[3] = v & 0xFF;
        this->p+=4;
    }

    void set_out_uint32_be(unsigned int v, size_t offset) {
        REDASSERT(this->has_room(4));
        (this->get_data())[offset+0] = (v >> 24) & 0xFF;
        (this->get_data())[offset+1] = (v >> 16) & 0xFF;
        (this->get_data())[offset+2] = (v >> 8) & 0xFF;
        (this->get_data())[offset+3] = v & 0xFF;
    }

    void out_sint32_le(int64_t v) {
        REDASSERT(this->has_room(4));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p+=4;
    }

    void out_unistr(const char* text)
    {
        const size_t len = UTF8toUTF16(reinterpret_cast<const uint8_t*>(text), this->p, this->tailroom());
        this->p += len;
        this->out_clear_bytes(2);
    }

    void out_date_name(const char* text, const size_t buflen)
    {
        size_t i = 0;
        for (; i < (buflen/2) ; i++) {
            if (!text[i]){
                break;
            }
            this->out_uint8(text[i]);
            this->out_uint8(0);
        }
        for (; i < (buflen/2) ; i++) {
            this->out_uint8(0);
            this->out_uint8(0);
        }
    }

    void out_utf16(const uint16_t utf16[], size_t length)
    {
        for (size_t i = 0; i < length ; i ++){
            this->out_uint16_le(utf16[i]);
        }
    }

    size_t get_capacity() const {
        return this->end - this->begin;
    }

    /// set current position to start buffer (\a p = \a begin)
    void rewind(std::size_t offset = 0) {
        REDASSERT(this->begin + offset <= this->end);
        this->p = this->begin + offset;
    }

    void out_copy_bytes(const uint8_t * v, size_t n) {
        REDASSERT(this->has_room(n));
        memcpy(this->p, v, n);
        this->p += n;
    }

    void set_out_copy_bytes(const uint8_t * v, size_t n, size_t offset) {
        memcpy(this->get_data()+offset, v, n);
    }

    void out_copy_bytes(const char * v, size_t n) {
        this->out_copy_bytes(reinterpret_cast<uint8_t const*>(v), n);
    }

    void set_out_copy_bytes(const char * v, size_t n, size_t offset) {
        this->set_out_copy_bytes(reinterpret_cast<uint8_t const*>(v), n, offset);
    }

    void out_clear_bytes(size_t n) {
        REDASSERT(this->has_room(n));
        memset(this->p, 0, n);
        this->p += n;
    }

    void out_bytes_le(const uint8_t nb, const unsigned value){
        REDASSERT(this->has_room(nb));
        ::out_bytes_le(this->p, nb, value);
        this->p += nb;
    }

    // =========================================================================
    // xER encoding rules support methods
    // =========================================================================

    enum {
        ER_CLASS_MASK = 0xC0,
        ER_PC_MASK    = 0x20,
        ER_TAG_MASK   = 0x1F
    };

    enum {
        // Tag - bits 5 to 1
        ER_TAG_BOOLEAN          = 0x01,
        ER_TAG_INTEGER          = 0x02,
        ER_TAG_BIT_STRING       = 0x03,
        ER_TAG_OCTET_STRING     = 0x04,
        ER_TAG_OBJECT_IDENFIER  = 0x06,
        ER_TAG_ENUMERATED       = 0x0A,
        ER_TAG_SEQUENCE         = 0x10,
        ER_TAG_SEQUENCE_OF      = 0x10,
        ER_TAG_GENERAL_STRING   = 0x1B,
        ER_TAG_GENERALIZED_TIME = 0x18,
        // P/C - bit 6
        ER_PRIMITIVE            = 0x00, // 0
        ER_CONSTRUCT            = 0x20, // 1
        // Class - bits 8 and 7
        ER_CLASS_UNIV           = 0x00, // 0 0
        ER_CLASS_APPL           = 0x40, // 0 1
        ER_CLASS_CTXT           = 0x80, // 1 0
        ER_CLASS_PRIV           = 0xC0  // 1 1
    };
};


template<std::size_t N, class StreamBase>
struct BasicStaticStream : StreamBase
{
    explicit BasicStaticStream(std::size_t offset = 0)
    : StreamBase(this->array_, N, offset)
    {}

    BasicStaticStream(BasicStaticStream const &) = delete;
    BasicStaticStream & operator = (BasicStaticStream const &) = delete;

    using array_type = uint8_t[N];

    constexpr std::size_t original_capacity() const {
        return N;
    }

private:
    uint8_t array_[N];
};

template<std::size_t N>
using StaticOutStream = BasicStaticStream<N, OutStream>;

template<std::size_t N>
using StaticInStream = BasicStaticStream<N, InStream>;

template<std::size_t LenMax>
struct StreamBufMaker
{
    StreamBufMaker() = default;

    uint8_t * reserve(std::size_t n) {
        uint8_t * p = this->buf_;
        if (n > sizeof(this->buf_)) {
            p = new uint8_t[n];
            this->dyn_buf_.reset(p);
        }
        return p;
    }

    OutStream reserve_out_stream(std::size_t n) {
        return OutStream(this->reserve(n), n);
    }

    InStream reserve_in_stream(std::size_t n) {
        return InStream(this->reserve(n), n);
    }

private:
    uint8_t buf_[LenMax];
    std::unique_ptr<uint8_t[]> dyn_buf_;
};

// class DynamicOutStream;
// class DynamicInStream;

inline InStream substream(const InStream & stream, std::size_t new_size, std::size_t offset = 0) {
    if ((offset + new_size) > stream.in_remain()){
        LOG(LOG_ERR, "substream definition outside underlying stream stream.remain=%u offset=%u new_size=%u",
            static_cast<unsigned>(stream.in_remain()),
            static_cast<unsigned>(offset),
            static_cast<unsigned>(new_size));
        throw Error(ERR_SUBSTREAM_OVERFLOW_IN_CONSTRUCTOR);
    }
    return InStream(stream.get_current() + offset, new_size);
}

namespace details_ {
    inline void copy_to_head_impl(char * & buf) {}

    template<class InStream, class... InStreams>
    void copy_to_head_impl(char * & buf, InStream const & in_stream, InStreams const & ... in_streams) {
        copy_to_head_impl(buf, in_streams...);
        buf -= in_stream.get_offset();
        memcpy(buf, in_stream.get_data(), in_stream.get_offset());
    }
}

template<class... InStream>
char * copy_to_head(char * buf, std::size_t headroom, InStream const & ... in_streams) {
#ifndef NDEBUG
    std::size_t len_streams = 0;
    (void)std::initializer_list<int>{(len_streams += in_streams.get_offset(), 1)...};
    REDASSERT(len_streams <= headroom);
#endif
    auto start = buf + headroom;
    details_::copy_to_head_impl(start, in_streams...);
    return start;
}

template<class... InStream>
uint8_t * copy_to_head(uint8_t * buf, std::size_t headroom, InStream const & ... in_streams) {
    return reinterpret_cast<uint8_t *>(copy_to_head(reinterpret_cast<char *>(buf), headroom, in_streams...));
}


struct OutReservedStreamHelper
{
    OutReservedStreamHelper(uint8_t * data, std::size_t reserved_leading_space, std::size_t buf_len)
    : buf(data + reserved_leading_space)
    , reserved_leading_space(reserved_leading_space)
    , stream(this->buf, buf_len - reserved_leading_space)
    {}

    struct Packet
    {
        uint8_t * data() const noexcept { return this->data_; }
        std::size_t size() const noexcept { return this->size_; }

        uint8_t * data_;
        std::size_t size_;
    };

    Packet get_packet() const noexcept {
        return Packet{this->buf, std::size_t(this->stream.get_data_end() - this->buf)};
    }

    std::size_t get_reserved_leading_space() const {
        return this->reserved_leading_space;
    }

    OutStream & get_data_stream() {
        return this->stream;
    }

    Packet copy_to_head(OutStream const & stream) {
        assert(stream.get_offset() <= this->reserved_leading_space);
        this->buf -= stream.get_offset();
        this->reserved_leading_space -= stream.get_offset();
        memcpy(this->buf, stream.get_data(), stream.get_offset());

        return get_packet();
    }

    Packet copy_to_head(OutStream const & stream1, OutStream const & stream2) {
        auto const total_stream_size = stream1.get_offset() + stream2.get_offset();
        assert(total_stream_size <= this->reserved_leading_space);
        this->reserved_leading_space -= total_stream_size;
        this->buf -= total_stream_size;

        auto start = this->buf;
        memcpy(start, stream1.get_data(), stream1.get_offset());
        start += stream1.get_offset();
        memcpy(start, stream2.get_data(), stream2.get_offset());

        return get_packet();
    }

    Packet copy_to_head(OutStream const & stream1, OutStream const & stream2, OutStream const & stream3) {
        auto const total_stream_size = stream1.get_offset() + stream2.get_offset() + stream3.get_offset();
        assert(total_stream_size <= this->reserved_leading_space);
        this->reserved_leading_space -= total_stream_size;
        this->buf -= total_stream_size;

        auto start = this->buf;
        memcpy(start, stream1.get_data(), stream1.get_offset());
        start += stream1.get_offset();
        memcpy(start, stream2.get_data(), stream2.get_offset());
        start += stream2.get_offset();
        memcpy(start, stream3.get_data(), stream3.get_offset());

        return get_packet();
    }

private:
    uint8_t * buf;
    std::size_t reserved_leading_space;
    OutStream stream;
};


template<std::size_t HeaderSz, std::size_t StreamSz>
struct StaticOutReservedStreamHelper : OutReservedStreamHelper
{
    StaticOutReservedStreamHelper()
    : OutReservedStreamHelper(this->data, HeaderSz, HeaderSz + StreamSz)
    {}

private:
    uint8_t data[HeaderSz + StreamSz];
};

/**
 * \addtogroup stream-utility
 * \addtogroup transport-utility
 * @{
 */
template<class Writer>
struct DynamicStreamWriter
{
    std::size_t packet_size() const noexcept {
        return this->stream_size_;
    }

    void operator()(std::size_t, OutStream & ostream) const {
        REDASSERT(ostream.get_capacity() == this->stream_size_);
        this->apply_writer1(ostream, this->writer_, 1);
    }

    void operator()(std::size_t, OutStream & ostream, uint8_t * buf, std::size_t used_buf_sz) const {
        REDASSERT(ostream.get_capacity() == this->stream_size_);
        this->apply_writer2(ostream, buf, used_buf_sz, this->writer_, 1);
    }

    Writer writer_;
    std::size_t stream_size_;

private:
    template<class W>
    auto apply_writer1(OutStream & ostream, W & writer, int) const
    -> decltype(writer(ostream))
    { writer(ostream); }

    template<class W>
    void apply_writer1(OutStream & ostream, W & writer, unsigned) const;

    template<class W>
    auto apply_writer2(OutStream & ostream, uint8_t *, std::size_t used_buf_sz, W & writer, int) const
    -> decltype(writer(ostream, used_buf_sz))
    { writer(ostream, used_buf_sz); }

    template<class W>
    void apply_writer2(OutStream & ostream, uint8_t * buf, std::size_t used_buf_sz, W & writer, unsigned) const
    { writer(ostream, buf, used_buf_sz); }
};

template<std::size_t N>
using StreamSize = std::integral_constant<std::size_t, N>;
/**
 * @}
 */

namespace details_ {
    /// Extract stream size of Writer
    /// @{
    template<class R, class C, class Sz, class ... Args>
    constexpr Sz packet_size_from_mfunc(R(C::*)(Sz, OutStream &, Args...)) {
        return Sz{};
    }

    template<class R, class C, class Sz, class ... Args>
    constexpr Sz packet_size_from_mfunc(R(C::*)(Sz, OutStream &, Args...) const) {
        return Sz{};
    }

    template<class F>
    constexpr auto packet_size_impl(F const &, std::false_type)
    -> decltype(packet_size_from_mfunc(&F::operator())){
        return packet_size_from_mfunc(&F::operator());
    }

    template<class Writer>
    constexpr auto packet_size_impl(Writer & writer, std::true_type)
    -> decltype(writer.packet_size()) {
        return writer.packet_size();
    }

    template<class F, class = void>
    struct has_packet_size : std::false_type
    {};

    template<class F>
    struct has_packet_size<F, decltype(void(std::declval<F>().packet_size()))> : std::true_type
    {};

    template<class F>
    constexpr auto packet_size(F const & f)
    -> decltype(packet_size_impl(f, has_packet_size<F>{})) {
        return packet_size_impl(f, has_packet_size<F>{});
    }

    template<class R, class Sz, class ... Args>
    constexpr Sz packet_size(R(*)(Sz, OutStream &, Args...)) {
        return Sz{};
    }
    /// @}


    /// Extract stream size of multiple Writers
    /// @{
    template<class T, class U>
    std::integral_constant<std::size_t, T::value + U::value>
    packet_size_add(T const & a, U const & b) {
        return {};
    }

    template<class T, class U>
    typename std::enable_if<
        std::is_same<T, std::size_t>::value
     || std::is_same<U, std::size_t>::value
    , std::size_t>::type
    packet_size_add(T const & a, U const & b) {
        return a + b;
    }

    template<class AccuSize>
    constexpr AccuSize packets_size_impl(AccuSize accu) {
        return accu;
    }

    template<class AccuSize, class... Fns>
    struct build_packets_size_type;

    template<class AccuSize, class Fn, class... Fns>
    struct build_packets_size_type<AccuSize, Fn, Fns...>
    {
        using type = typename build_packets_size_type<
            decltype(packet_size_add(AccuSize{}, packet_size(std::declval<Fn>()))),
            Fns...
        >::type;
    };

    template<class AccuSize>
    struct build_packets_size_type<AccuSize>
    { using type = AccuSize; };

    template<class AccuSize, class Fn, class... Fns>
    constexpr typename build_packets_size_type<AccuSize, Fn, Fns...>::type
    packets_size_impl(AccuSize accu, Fn const & f, Fns const & ... fns) {
        return packets_size_impl(packet_size_add(accu, packet_size(f)), fns...);
    }

    template<class... Fs>
    constexpr auto packets_size(Fs const & ... fns)
    -> decltype(packets_size_impl(StreamSize<0>{}, fns...)) {
        return packets_size_impl(StreamSize<0>{}, fns...);
    }
    /// @}


    template<class StreamSz, class Writer>
    auto apply_writer(StreamSz sz, OutStream & ostream, uint8_t *, std::size_t used_buf_sz, Writer & writer, int)
    -> decltype(writer(sz, ostream, used_buf_sz))
    { writer(sz, ostream, used_buf_sz); }

    template<class StreamSz, class Writer>
    void apply_writer(StreamSz sz, OutStream & ostream, uint8_t * buf, std::size_t used_buf_sz, Writer & writer, unsigned)
    { writer(sz, ostream, buf, used_buf_sz); }


    template<class StreamSz, class Writer>
    void write_packet(uint8_t * & full_buf, std::size_t & used_buf_sz, StreamSz sz, Writer & writer) {
        uint8_t data_buf[StreamSz::value];
        OutStream ostream(data_buf);
        apply_writer(sz, ostream, full_buf, used_buf_sz, writer, 1);
        used_buf_sz += ostream.get_offset();
        full_buf -= ostream.get_offset();
        memcpy(full_buf, ostream.get_data(), ostream.get_offset());
    }

    template<class DataBufSz, class HeaderBufSz, class Transport, class DataWriter, class... HeaderWriters>
    void write_packets_mpl(
        DataBufSz data_buf_sz, HeaderBufSz header_buf_sz, uint8_t * buf,
        Transport & trans, DataWriter & data_writer, HeaderWriters & ... header_writers)
    {
        OutStream data_stream(buf + header_buf_sz, data_buf_sz);
        data_writer(data_buf_sz, data_stream);
        auto * start = data_stream.get_data();
        std::size_t used_buf_sz = data_stream.get_offset();
        (void)std::initializer_list<int>{((
            details_::write_packet(start, used_buf_sz, details_::packet_size(header_writers), header_writers)
        ), 1)...};
        trans.send(start, used_buf_sz);
    }

    template<class DataBufSz, class HeaderBufSz, class Transport, class DataWriter, class... HeaderWriters>
    void write_packets(
        DataBufSz data_buf_sz, HeaderBufSz header_buf_sz, std::size_t,
        Transport & trans, DataWriter & data_writer, HeaderWriters & ... header_writers)
    {
        if (data_buf_sz + header_buf_sz < 65536) {
            uint8_t buf[65536];
            write_packets_mpl(data_buf_sz, header_buf_sz, buf, trans, data_writer, header_writers...);
        }
        else {
            auto u = std::make_unique<uint8_t[]>(data_buf_sz + header_buf_sz);
            write_packets_mpl(data_buf_sz, header_buf_sz, u.get(), trans, data_writer, header_writers...);
        }
    }

    template<
        class DataBufSz, class HeaderBufSz, class TotalSz,
        class Transport, class DataWriter, class... HeaderWriters
    >
    void write_packets(
        DataBufSz data_buf_sz, HeaderBufSz header_buf_sz, TotalSz,
        Transport & trans, DataWriter & data_writer, HeaderWriters & ... header_writers)
    {
        uint8_t buf[TotalSz::value];
        write_packets_mpl(data_buf_sz, header_buf_sz, buf, trans, data_writer, header_writers...);
    }
}


/**
 * \addtogroup stream-utility
 * \addtogroup transport-utility
 *
 * \param data_writer  function(IntegralConstant||std:size_t, OutStream &)
 * \param header_writers  function(IntegralConstant||std:size_t, OutStream &, [uint8_t * data,] std::size_t data_sz)
 *
 * write_packets(t, f1, f2, f3) write a packet [f3-f2-f1] in t
 */
template<class Transport, class DataWriter, class... HeaderWriters>
void write_packets(Transport & trans, DataWriter data_writer, HeaderWriters... header_writers)
{
    auto const data_buf_sz = details_::packet_size(data_writer);
    auto const header_buf_sz = details_::packets_size(header_writers...);
    details_::write_packets(
        data_buf_sz, header_buf_sz,
        details_::packet_size_add(header_buf_sz, data_buf_sz),
        trans, data_writer, header_writers...
    );
}

template<class Writer>
DynamicStreamWriter<Writer>
dynamic_packet(std::size_t stream_sz, Writer writer) {
    return {std::move(writer), stream_sz};
}


inline OutStream make_skip_out_stream(uint8_t * p, std::size_t sz)
{
    OutStream stream(p, sz);
    stream.out_skip_bytes(sz);
    return stream;
}


class Stream {
public:
    uint8_t* p;
    uint8_t* end;
protected:
    uint8_t* data;
public:
    size_t capacity;

public:
    virtual ~Stream() {}

    virtual void init(size_t capacity) = 0;

    virtual void reset(){
        this->end = this->p = this->data;
    }

    bool has_room(size_t n) const {
        return  n <= this->tailroom();
    }

    virtual size_t tailroom() const {
        return this->capacity - this->headroom() - this->get_offset();
    }

    virtual size_t endroom() const {
        return this->capacity - (this->end - this->data);
    }

    virtual size_t headroom() const {
        return 0;
    }

    virtual size_t get_capacity() const {
        return this->capacity - this->headroom();
    }

    virtual uint8_t * get_data() const {
        return this->data;
    }

    uint32_t get_offset() const {
        return this->p - this->get_data();
    }

    bool in_check_rem(const unsigned n) const {
    // returns true if there is enough data available to read n bytes
        return (n <= this->in_remain());
    }

    size_t in_remain() const {
        return this->end - this->p;
    }

    bool check_end(void) const {
        return this->p == this->end;
    }

    uint8_t * get_current() const {
        return this->p;
    }

    // =========================================================================
    // Generic binary Data access methods
    // =========================================================================

    int8_t in_sint8(void) {
        REDASSERT(this->in_check_rem(1));
        return *reinterpret_cast<int8_t*>(this->p++);
    }

    uint8_t in_uint8(void) {
        REDASSERT(this->in_check_rem(1));
        return *this->p++;
    }

    /* Peek a byte from stream without move <p>. */
    uint8_t peek_uint8(void) const {
        REDASSERT(this->in_check_rem(1));
        return *this->p;
    }

    int16_t in_sint16_be(void) {
        REDASSERT(this->in_check_rem(2));
        unsigned int v = this->in_uint16_be();
        return static_cast<int16_t>((v > 32767)?v - 65536:v);
    }


    int16_t in_sint16_le(void) {
        REDASSERT(this->in_check_rem(2));
        unsigned int v = this->in_uint16_le();
        return static_cast<int16_t>((v > 32767)?v - 65536:v);
    }

    uint16_t in_uint16_le(void) {
        REDASSERT(this->in_check_rem(2));
        this->p += 2;
        return static_cast<uint16_t>(this->p[-2] | (this->p[-1] << 8));
    }

    uint16_t in_uint16_be(void) {
        REDASSERT(this->in_check_rem(2));
        this->p += 2;
        return static_cast<uint16_t>(this->p[-1] | (this->p[-2] << 8));
    }

    unsigned int in_uint32_le(void) {
        REDASSERT(this->in_check_rem(4));
        this->p += 4;
        return  this->p[-4]
             | (this->p[-3] << 8)
             | (this->p[-2] << 16)
             | (this->p[-1] << 24)
             ;
    }

    unsigned int in_uint32_be(void) {
        REDASSERT(this->in_check_rem(4));
        this->p += 4;
        return  this->p[-1]
             | (this->p[-2] << 8)
             | (this->p[-3] << 16)
             | (this->p[-4] << 24)
             ;
    }

    int32_t in_sint32_le(void) {
        uint64_t v = this->in_uint32_le();
        return static_cast<int32_t>((v > 0x7FFFFFFF) ? v - 0x100000000LL : v);
    }

    int32_t in_sint32_be(void) {
        uint64_t v = this->in_uint32_be();
        return static_cast<int32_t>((v > 0x7FFFFFFF) ? v - 0x100000000LL : v);
    }

    // ---------------------------------------------------------------------------

    void in_timeval_from_uint64le_usec(timeval & tv)
    {
        const uint64_t movie_usec_lo = this->in_uint32_le();
        const uint64_t movie_usec_hi = this->in_uint32_le();
        const uint64_t movie_usec = (movie_usec_hi * 0x100000000LL + movie_usec_lo);
        tv.tv_usec = static_cast<uint32_t>(movie_usec % 1000000LL);
        tv.tv_sec = static_cast<uint32_t>(movie_usec / 1000000LL);
    }

    void out_timeval_to_uint64le_usec(const timeval & tv)
    {
        uint64_t usec = tv.tv_sec * 1000000ULL + tv.tv_usec;
        this->out_uint32_le(static_cast<uint32_t>(usec));
        this->out_uint32_le(static_cast<uint32_t>(usec >> 32));
    }

    // ---------------------------------------------------------------------------

    void out_uint64_le(uint64_t v) {
        REDASSERT(this->has_room(8));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p[4] = (v >> 32) & 0xFF;
        this->p[5] = (v >> 40) & 0xFF;
        this->p[6] = (v >> 48) & 0xFF;
        this->p[7] = (v >> 56) & 0xFF;
        this->p+=8;
    }

    uint64_t in_uint64_le(void) {
        REDASSERT(this->in_check_rem(8));
        uint64_t low = this->in_uint32_le();
        uint64_t high = this->in_uint32_le();
        return low + (high << 32);
    }

    void out_uint64_be(uint64_t v) {
        REDASSERT(this->has_room(8));
        this->p[0] = (v >> 56) & 0xFF;
        this->p[1] = (v >> 48) & 0xFF;
        this->p[2] = (v >> 40) & 0xFF;
        this->p[3] = (v >> 32) & 0xFF;
        this->p[4] = (v >> 24) & 0xFF;
        this->p[5] = (v >> 16) & 0xFF;
        this->p[6] = (v >> 8) & 0xFF;
        this->p[7] = v & 0xFF;
        this->p+=8;
    }

    void out_sint64_le(int64_t v) {
        REDASSERT(this->has_room(8));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p[4] = (v >> 32) & 0xFF;
        this->p[5] = (v >> 40) & 0xFF;
        this->p[6] = (v >> 48) & 0xFF;
        this->p[7] = (v >> 56) & 0xFF;
        this->p+=8;
    }

    int64_t in_sint64_le(void) {
        int64_t res;
        *(reinterpret_cast<uint64_t *>(&res)) = this->in_uint64_le();
        return res;
    }


    uint64_t in_uint64_be(void) {
        REDASSERT(this->in_check_rem(8));
        uint64_t high = this->in_uint32_be();
        uint64_t low = this->in_uint32_be();
        return low + (high << 32);
    }

    unsigned in_bytes_le(const uint8_t nb){
        REDASSERT(this->in_check_rem(nb));
        this->p += nb;
        return ::in_uint32_from_nb_bytes_le(nb, this->p - nb);
    }

    unsigned in_bytes_be(const uint8_t nb){
        REDASSERT(this->in_check_rem(nb));
        this->p += nb;
        return ::in_uint32_from_nb_bytes_be(nb, this->p - nb);
    }

    void in_copy_bytes(uint8_t * v, size_t n) {
        REDASSERT(this->in_check_rem(n));
        memcpy(v, this->p, n);
        this->p += n;
    }

    void in_copy_bytes(char * v, size_t n) {
        this->in_copy_bytes(reinterpret_cast<uint8_t*>(v), n);
    }

    const uint8_t *in_uint8p(unsigned int n) {
        REDASSERT(this->in_check_rem(n));
        this->p+=n;
        return this->p - n;
    }

    void in_skip_bytes(unsigned int n) {
        REDASSERT(this->in_check_rem(n));
        this->p+=n;
    }

    void out_skip_bytes(unsigned int n) {
        REDASSERT(this->has_room(n));
        this->p+=n;
    }

    void out_uint8(uint8_t v) {
        REDASSERT(this->has_room(1));
        *(this->p++) = v;
    }

    void set_out_uint8(uint8_t v, size_t offset) {
        (this->get_data())[offset] = v;
    }

    // MS-RDPEGDI : 2.2.2.2.1.2.1.2 Two-Byte Unsigned Encoding
    // =======================================================
    // (TWO_BYTE_UNSIGNED_ENCODING)

    // The TWO_BYTE_UNSIGNED_ENCODING structure is used to encode a value in
    // the range 0x0000 to 0x7FFF by using a variable number of bytes.
    // For example, 0x1A1B is encoded as { 0x9A, 0x1B }.
    // The most significant bit of the first byte encodes the number of bytes
    // in the structure.

    // c (1 bit): A 1-bit, unsigned integer field that contains an encoded
    // representation of the number of bytes in this structure. 0 implies val2 field
    // is not present, if 1 val2 is present.

    // val1 (7 bits): A 7-bit, unsigned integer field containing the most
    // significant 7 bits of the value represented by this structure.

    // val2 (1 byte): An 8-bit, unsigned integer containing the least significant
    // bits of the value represented by this structure.

    void out_2BUE(uint16_t v){
        if (v <= 127){
            this->out_uint8(static_cast<uint8_t>(v));
        }
        else {
            this->out_uint16_be(v|0x8000);
        }
    }

    void set_out_2BUE(uint16_t v, size_t offset){
        if (v <= 127){
            this->set_out_uint8(static_cast<uint8_t>(v), offset);
        }
        else {
            this->set_out_uint16_be(v|0x8000, offset);
        }
    }

    uint16_t in_2BUE()
    {
        uint16_t length = this->in_uint8();
        if (length & 0x80){
            length = ((length & 0x7F) << 8);
            length += this->in_uint8();
        }
        return length;
    }

// [MS-RDPEGDI] - 2.2.2.2.1.2.1.4 Four-Byte Unsigned Encoding
// (FOUR_BYTE_UNSIGNED_ENCODING)
// ==========================================================

// The FOUR_BYTE_UNSIGNED_ENCODING structure is used to encode a value in the
//  range 0x00000000 to 0x3FFFFFFF by using a variable number of bytes. For
//  example, 0x001A1B1C is encoded as { 0x9A, 0x1B, 0x1C }. The two most
//  significant bits of the first byte encode the number of bytes in the
//  structure.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | c |    val1   |val2 (optional)|val3 (optional)|val4 (optional)|
// +---+-----------+---------------+---------------+---------------+

// c (2 bits): A 2-bit, unsigned integer field containing an encoded
//  representation of the number of bytes in this structure.

// +----------------+----------------------------------------------------------+
// | Value          | Meaning                                                  |
// +----------------+----------------------------------------------------------+
// | ONE_BYTE_VAL   | Implies that the optional val2, val3, and val4 fields    |
// | 0              | are not present. Hence, the structure is 1 byte in size. |
// +----------------+----------------------------------------------------------+
// | TWO_BYTE_VAL   | Implies that the optional val2 field is present while    |
// | 1              | the optional val3 and val4 fields are not present.       |
// |                | Hence, the structure is 2 bytes in size.                 |
// +----------------+----------------------------------------------------------+
// | THREE_BYTE_VAL | Implies that the optional val2 and val3 fields are       |
// | 2              | present while the optional val4 fields are not present.  |
// |                | Hence, the structure is 3 bytes in size.                 |
// +----------------+----------------------------------------------------------+
// | FOUR_BYTE_VAL  | Implies that the optional val2, val3, and val4 fields    |
// | 3              | are all present. Hence, the structure is 4 bytes in      |
// |                | size.                                                    |
// +----------------+----------------------------------------------------------+

// val1 (6 bits): A 6-bit, unsigned integer field containing the most
//  significant 6 bits of the value represented by this structure.

// val2 (1 byte): An 8-bit, unsigned integer containing the second most
//  significant bits of the value represented by this structure.

// val3 (1 byte): An 8-bit, unsigned integer containing the third most
//  significant bits of the value represented by this structure.

// val4 (1 byte): An 8-bit, unsigned integer containing the least significant
//  bits of the value represented by this structure.

    uint32_t in_4BUE()
    {
        uint32_t length = this->in_uint8();
        switch (length & 0xC0)
        {
            case 0xC0:
                length =  ((length & 0x3F)  << 24);
                length += (this->in_uint8() << 16);
                length += (this->in_uint8() << 8 );
                length +=  this->in_uint8();
            break;
            case 0x80:
                length =  ((length & 0x3F)  << 16);
                length += (this->in_uint8() << 8 );
                length +=  this->in_uint8();
            break;
            case 0x40:
                length =  ((length & 0x3F)  << 8 );
                length +=  this->in_uint8();
            break;
        }

        return length;
    }

    void out_4BUE(uint32_t v){
        REDASSERT(!(v & 0xC0000000));
             if (v <= 0x3F      ) {
            this->out_uint8(static_cast<uint8_t>(v));
        }
        else if (v <= 0x3FFF    ) {
            this->out_uint8(0x40 | ((v >> 8 ) & 0x3F));
            this->out_uint8(         v        & 0xFF );
        }
        else if (v <= 0x3FFFFF  ) {
            this->out_uint8(0x80 | ((v >> 16) & 0x3F));
            this->out_uint8(        (v >> 8 ) & 0xFF );
            this->out_uint8(         v        & 0xFF );
        }
        else/* if (v <= 0x3FFFFFFF)*/ {
            this->out_uint8(0xC0 | ((v >> 24) & 0x3F));
            this->out_uint8(        (v >> 16) & 0xFF );
            this->out_uint8(        (v >> 8 ) & 0xFF );
            this->out_uint8(         v        & 0xFF );
        }
    }


    // MS-RDPEGDI : 2.2.2.2.1.1.1.4 Delta-Encoded Points (DELTA_PTS_FIELD)
    // ===================================================================

    // ..., the delta value it  represents is encoded in a packet signed
    //  format:

    //  * If the high bit (0x80) is not set in the first encoding byte, the
    //    field is 1 byte long and is encoded as a signed delta in the lower
    //    7 bits of the byte.

    //  * If the high bit of the first encoding byte is set, the lower 7 bits
    //    of the first byte and the 8 bits of the next byte are concatenated
    //    (the first byte containing the high-order bits) to create a 15-bit
    //    signed delta value.

    void out_DEP(int16_t point) {
        if (abs(point) > 0x3F) {
            uint16_t data;

            memcpy(&data, &point, sizeof(data));
            data |= 0x8000;

            if (point > 0) {
                data &= ~0x4000;
            }
            else {
                data |= 0x4000;
            }

            this->out_uint16_be(data);
        }
        else {
            int8_t  _yDelta;
            uint8_t data;

            _yDelta = point;

            memcpy(&data, &_yDelta, sizeof(data));
            data &= ~0x80;

            if (point > 0) {
                data &= ~0x40;
            }
            else {
                data |= 0x40;
            }

            this->out_uint8(data);
        }
    }

    int16_t in_DEP(void) {
        int16_t point = this->in_uint8();
        if (point & 0x80)
        {
            point = ((point & 0x7F) << 8) + this->in_uint8();
            if (point & 0x4000)
                point = - ((~(point - 1)) & 0x7FFF);
        }
        else
        {
            if (point & 0x40)
                point = - ((~(point - 1)) & 0x7F);
        }

        return point;
    }

    void out_sint8(char v) {
        REDASSERT(this->has_room(1));
        *(this->p++) = v;
    }

    void out_uint16_le(unsigned int v) {
        REDASSERT(this->has_room(2));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p+=2;
    }

    void set_out_uint16_le(unsigned int v, size_t offset) {
        (this->get_data())[offset] = v & 0xFF;
        (this->get_data())[offset+1] = (v >> 8) & 0xFF;
    }

    void out_sint16_le(signed int v) {
        REDASSERT(this->has_room(2));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p+=2;
    }

    void out_uint16_be(unsigned int v) {
        REDASSERT(this->has_room(2));
        this->p[1] = v & 0xFF;
        this->p[0] = (v >> 8) & 0xFF;
        this->p+=2;
    }

    void set_out_uint16_be(unsigned int v, size_t offset) {
        (this->get_data())[offset+1] = v & 0xFF;
        (this->get_data())[offset] = (v >> 8) & 0xFF;
    }

    void out_uint32_le(unsigned int v) {
        REDASSERT(this->has_room(4));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p+=4;
    }

    void set_out_uint32_le(unsigned int v, size_t offset) {
        (this->get_data())[offset+0] = v & 0xFF;
        (this->get_data())[offset+1] = (v >> 8) & 0xFF;
        (this->get_data())[offset+2] = (v >> 16) & 0xFF;
        (this->get_data())[offset+3] = (v >> 24) & 0xFF;
    }

    void out_uint32_be(unsigned int v) {
        REDASSERT(this->has_room(4));
        this->p[0] = (v >> 24) & 0xFF;
        this->p[1] = (v >> 16) & 0xFF;
        this->p[2] = (v >> 8) & 0xFF;
        this->p[3] = v & 0xFF;
        this->p+=4;
    }

    void set_out_uint32_be(unsigned int v, size_t offset) {
        REDASSERT(this->has_room(4));
        (this->get_data())[offset+0] = (v >> 24) & 0xFF;
        (this->get_data())[offset+1] = (v >> 16) & 0xFF;
        (this->get_data())[offset+2] = (v >> 8) & 0xFF;
        (this->get_data())[offset+3] = v & 0xFF;
    }

    void out_sint32_le(int64_t v) {
        REDASSERT(this->has_room(4));
        this->p[0] = v & 0xFF;
        this->p[1] = (v >> 8) & 0xFF;
        this->p[2] = (v >> 16) & 0xFF;
        this->p[3] = (v >> 24) & 0xFF;
        this->p+=4;
    }

    void out_unistr(const char* text)
    {
        const size_t len = UTF8toUTF16(reinterpret_cast<const uint8_t*>(text), this->p, this->tailroom());
        this->p += len;
        this->out_clear_bytes(2);
    }

    void out_date_name(const char* text, const size_t buflen)
    {
        size_t i = 0;
        for (; i < (buflen/2) ; i++) {
            if (!text[i]){
                break;
            }
            this->out_uint8(text[i]);
            this->out_uint8(0);
        }
        for (; i < (buflen/2) ; i++) {
            this->out_uint8(0);
            this->out_uint8(0);
        }
    }

    void out_utf16(const uint16_t utf16[], size_t length)
    {
        for (size_t i = 0; i < length ; i ++){
            this->out_uint16_le(utf16[i]);
        }
    }

    void in_utf16(uint16_t utf16[], size_t length)
    {
        for (size_t i = 0; i < length ; i ++){
            utf16[i] = this->in_uint16_le();
        }
    }

    // sz utf16 bytes are translated to ascci, 00 terminated
    void in_uni_to_ascii_str(uint8_t * text, size_t sz, size_t bufsz)
    {
        UTF16toUTF8(this->p, sz / 2, text, bufsz);
        this->p += sz;
    }

    void mark_end() {
        this->end = this->p;
    }

    virtual void rewind(){
        this->p = this->data;
    }

    size_t size() const {
        return this->end - this->get_data();
    }

    // Output zero terminated string, including trailing 0
    //void out_sz(const char * v) {
    //    size_t len = strlen(v);
    //    this->out_copy_bytes((uint8_t const*)v, len+1);
    //}

    // Output zero terminated string, non including trailing 0
    void out_string(const char * v) {
        size_t len = strlen(v);
        this->out_copy_bytes(reinterpret_cast<uint8_t const*>(v), len);
    }


    void out_copy_bytes(Stream & stream) {
        this->out_copy_bytes(stream.get_data(), stream.size());
    }

    void out_copy_bytes(const uint8_t * v, size_t n) {
        REDASSERT(this->has_room(n));
        memcpy(this->p, v, n);
        this->p += n;
    }

    void set_out_copy_bytes(const uint8_t * v, size_t n, size_t offset) {
        memcpy(this->get_data()+offset, v, n);
    }

    void out_copy_bytes(const char * v, size_t n) {
        this->out_copy_bytes(reinterpret_cast<uint8_t const*>(v), n);
    }

    void set_out_copy_bytes(const char * v, size_t n, size_t offset) {
        this->set_out_copy_bytes(reinterpret_cast<uint8_t const*>(v), n, offset);
    }

    void out_clear_bytes(size_t n) {
        REDASSERT(this->has_room(n));
        memset(this->p, 0, n);
        this->p += n;
    }

    void out_bytes_le(const uint8_t nb, const unsigned value){
        REDASSERT(this->has_room(nb));
        ::out_bytes_le(this->p, nb, value);
        this->p += nb;
    }

    void set_out_bytes_le(const uint8_t nb, const unsigned value, size_t offset){
        ::out_bytes_le(this->get_data()+offset, nb, value);
    }

    // =========================================================================
    // xER encoding rules support methods
    // =========================================================================

    enum {
        ER_CLASS_MASK = 0xC0,
        ER_PC_MASK    = 0x20,
        ER_TAG_MASK   = 0x1F
    };

    enum {
        // Tag - bits 5 to 1
        ER_TAG_BOOLEAN          = 0x01,
        ER_TAG_INTEGER          = 0x02,
        ER_TAG_BIT_STRING       = 0x03,
        ER_TAG_OCTET_STRING     = 0x04,
        ER_TAG_OBJECT_IDENFIER  = 0x06,
        ER_TAG_ENUMERATED       = 0x0A,
        ER_TAG_SEQUENCE         = 0x10,
        ER_TAG_SEQUENCE_OF      = 0x10,
        ER_TAG_GENERAL_STRING   = 0x1B,
        ER_TAG_GENERALIZED_TIME = 0x18,
        // P/C - bit 6
        ER_PRIMITIVE            = 0x00, // 0
        ER_CONSTRUCT            = 0x20, // 1
        // Class - bits 8 and 7
        ER_CLASS_UNIV           = 0x00, // 0 0
        ER_CLASS_APPL           = 0x40, // 0 1
        ER_CLASS_CTXT           = 0x80, // 1 0
        ER_CLASS_PRIV           = 0xC0  // 1 1
    };

    void print() {
        const char * data = reinterpret_cast<const char *>(this->get_data());
        size_t size = this->size();
        hexdump8_c(data, size);
    }

};


// BStream is for "buffering stream", as this stream allocate a work buffer.
class BStream : public Stream {
    uint8_t autobuffer[AUTOSIZE];

public:
    explicit BStream(size_t size = AUTOSIZE)
        : autobuffer()
    {
        this->p = nullptr;
        this->end = nullptr;
        this->data = this->autobuffer;
        this->capacity = AUTOSIZE;
        this->init(size);
    }

    ~BStream() override {
        // <this->data> is allocated dynamically.
        if (this->capacity > AUTOSIZE) {
            delete [] this->data;
        }
    }

    // a default buffer of 65536 bytes is allocated automatically, we will only allocate dynamic memory if we need more.
    void init(size_t v) override {
        if (v != this->capacity) {
            // <this->data> is allocated dynamically.
            if (this->capacity > AUTOSIZE){
                delete [] this->data;
            }

            this->capacity = v;
            if (v > AUTOSIZE){
                this->data = new(std::nothrow) uint8_t[v];
                if (!this->data) {
                    this->capacity = 0;
                    LOG(LOG_ERR, "failed to allocate buffer : size asked = %d\n", static_cast<int>(v));
                    throw Error(ERR_STREAM_MEMORY_ALLOCATION_ERROR);
                }
            }
            else {
                this->data = &(this->autobuffer[0]);
            }
        }
        this->p = this->data;
        this->end = this->data;
    }
};

class HStream : public BStream {
public:
    size_t    reserved_leading_space;
    uint8_t * data_start;

    explicit HStream(size_t reserved_leading_space, size_t total_size = AUTOSIZE)
            : BStream(total_size)
            , reserved_leading_space(reserved_leading_space) {
        if (reserved_leading_space > total_size) {
            LOG( LOG_ERR
               , "failed to allocate buffer : total_size=%u, reserved_leading_space=%u\n"
               , total_size, reserved_leading_space);
            throw Error(ERR_STREAM_VALUE_TOO_LARGE_FOR_RESERVED_LEADING_SPACE);
        }

        this->p          += this->reserved_leading_space;
        this->data_start  = this->p;
        this->end         = this->p;
    }

    void copy_to_head(const uint8_t * v, size_t n) {
        if (this->data_start - this->data >= static_cast<ssize_t>(n)) {
            ::memcpy(this->data_start - n , v, n);
            this->data_start = this->data_start - n;
        }
        else {
            LOG( LOG_ERR
               , "reserved leading space too small : size available = %d, size asked = %d\n"
               , this->data_start - this->data
               , static_cast<int>(n));
            throw Error(ERR_STREAM_RESERVED_LEADING_SPACE_TOO_SMALL);
        }
    }

    size_t headroom() const override {
        return this->data_start - this->data;
    }

    uint8_t * get_data() const override {
        return this->data_start;
    }

    void init(size_t body_size) override {
        BStream::init(this->reserved_leading_space + body_size);

        this->p          += this->reserved_leading_space;
        this->data_start  = this->p;
        this->end         = this->p;
    }

    void reset() override {
        BStream::reset();

        this->p          += this->reserved_leading_space;
        this->data_start  = this->p;
        this->end         = this->p;
    }

    void rewind() override {
        this->data_start = this->p = this->data + this->reserved_leading_space;
    }
};

// SubStream does not allocate any buffer
// (and the buffer pointed to should probably not be modifiable,
// but I'm not yet doing any distinction between stream that can or can't be modified
// many at some future time)
class SubStream : public Stream {
    public:
    SubStream(){}  // not yet initialized

    explicit SubStream(const Stream & stream, size_t offset = 0, size_t new_size = 0)
    {
        if ((offset + new_size) > stream.size()){
            LOG(LOG_ERR, "Substream definition outside underlying stream stream.size=%u offset=%u new_size=%u",
                static_cast<unsigned>(stream.size()),
                static_cast<unsigned>(offset),
                static_cast<unsigned>(new_size));
            throw Error(ERR_SUBSTREAM_OVERFLOW_IN_CONSTRUCTOR);
        }
        this->p = this->data = stream.get_data() + offset;
        this->capacity = (new_size == 0)?(stream.size() - offset):new_size;
        this->end = this->data + this->capacity;
    }

    explicit SubStream(const InStream & stream, size_t offset = 0, size_t new_size = 0)
    {
        if ((offset + new_size) > stream.get_capacity()){
            LOG(LOG_ERR, "Substream definition outside underlying stream stream.size=%u offset=%u new_size=%u",
                static_cast<unsigned>(stream.get_capacity()),
                static_cast<unsigned>(offset),
                static_cast<unsigned>(new_size));
            throw Error(ERR_SUBSTREAM_OVERFLOW_IN_CONSTRUCTOR);
        }
        TODO("IMPORTANT: This is bad cast")
        this->p = this->data = const_cast<uint8_t *>(stream.get_data()) + offset;
        this->capacity = (new_size == 0)?(stream.get_capacity() - offset):new_size;
        this->end = this->data + this->capacity;
    }

    // Not allowed on SubStreams
    void init(size_t) override {}
};

// FixedSizeStream does not allocate/reallocate any buffer
class FixedSizeStream : public Stream {
    public:
    FixedSizeStream(){
        // not yet initialized
        this->p = this->data = nullptr;
        this->capacity = 0;
        this->end = nullptr;
    }

    FixedSizeStream(uint8_t * data, size_t len){
        this->p = this->data = data;
        this->capacity = len;
        this->end = this->data + this->capacity;
    }

    // Not allowed on SubStreams
    void init(size_t) override {}
};

// StaticStream does not allocate/reallocate any buffer
// All write operations are forbidden
class StaticStream : public FixedSizeStream {
    public:
    StaticStream(){}  // not yet initialized

    StaticStream(const uint8_t * data, size_t len){
        this->p = this->data = const_cast<uint8_t *>(data);
        this->capacity = len;
        this->end = this->data + this->capacity;
    }

    StaticStream(const char * data, size_t len){
        this->p = this->data = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(data));
        this->capacity = len;
        this->end = this->data + this->capacity;
    }

    // Not allowed on SubStreams
    void init(size_t) override {}

    void resize(const uint8_t * data, size_t len) {
        this->p = this->data = const_cast<uint8_t *>(data);
        this->capacity = len;
        this->end = this->data + this->capacity;
    }
};

typedef StaticStream ReadOnlyStream;
typedef FixedSizeStream WriteOnlyStream;

#endif
