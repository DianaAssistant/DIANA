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
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni, Meng Tan, Raphael Zhou
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   Unit test for bitmap class (mostly tests of compression/decompression)
*/

#include "test_only/test_framework/redemption_unit_tests.hpp"

#include "utils/bitmap_private_data.hpp" // aux_::bitmap_data_allocator

#include "utils/rle.cpp" // Yolo :O
#include "utils/stream.hpp"


RED_AUTO_TEST_CASE(TestRleCompressHardenned)
{
    const unsigned white = 0xFF;

    // test COLOR COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        RED_CHECK_EQUAL(16u, decompressor.get_color_count(pmax, pmin, 0x05));
        RED_CHECK_EQUAL(0u, decompressor.get_color_count(pmax, pmin, 0xFE));
    }

    // test COLOR COUNT 2
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x01, 0x02, 0x02, 0x04,
            0x05, 0x06, 0x07, 0x08,
            0x09, 0x0A, 0x0B, 0x0C,
            0x0D, 0x0E, 0x0F, 0x10};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        RED_CHECK_EQUAL(1u, decompressor.get_color_count(pmax, pmin, 0x01));
        RED_CHECK_EQUAL(2u, decompressor.get_color_count(pmax, pmin + 1, 0x02));
    }

    // test BICOLOR COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x01, 0x05, 0x01, 0x05,
            0x01, 0x05, 0x01, 0x05,
            0x01, 0x05, 0x01, 0x05,
            0x01, 0x05, 0x01, 0x05};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        RED_CHECK_EQUAL(16u, decompressor.get_bicolor_count(pmax, pmin, 0x01, 0x05));
        RED_CHECK_EQUAL(14u, decompressor.get_bicolor_count(pmax, pmin+1, 0x05, 0x01));
        RED_CHECK_EQUAL(0u , decompressor.get_bicolor_count(pmax, pmin, 0x05, 0x01));
    }

    // test BICOLOR COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x01, 0x05, 0x01, 0x05,
            0x01, 0x05, 0x01, 0x05,
            0x01, 0x05, 0x01, 0x01,
            0x01, 0x05, 0x01, 0x05};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        RED_CHECK_EQUAL(10u, decompressor.get_bicolor_count(pmax, pmin, 0x01, 0x05));
        RED_CHECK_EQUAL(10u, decompressor.get_bicolor_count(pmax, pmin + 1, 0x05, 0x01));
    }

    // test FILL COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x00, 0x00, 0x00, 0x00,
            0x01, 0x05, 0x01, 0x05,
            0x01, 0x05, 0x01, 0x01,
            0x01, 0x05, 0x01, 0x05};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        const size_t line_size = 4;

        // Line above first line is black
        RED_CHECK_EQUAL(4u, decompressor.get_fill_count(line_size, pmin, pmax, data));
        RED_CHECK_EQUAL(3u, decompressor.get_fill_count(line_size, pmin, pmax, data+1));

        // 3rd line, compared to 2nd line
        RED_CHECK_EQUAL(3u, decompressor.get_fill_count(line_size, pmin, pmax, data+8));
    }

    // test FILL COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x01, 0x00, 0x01, 0x00,
            0x01, 0x05, 0x01, 0x01,
            0x01, 0x05, 0x01, 0x01,
            0x01, 0x05, 0x01, 0x01};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        const size_t line_size = 4;

        // Line above first line is black
        RED_CHECK_EQUAL(0u, decompressor.get_fill_count(line_size, pmin, pmax, data));
        RED_CHECK_EQUAL(1u, decompressor.get_fill_count(line_size, pmin, pmax, data+1));
        RED_CHECK_EQUAL(0u, decompressor.get_fill_count(line_size, pmin, pmax, data+2));
        RED_CHECK_EQUAL(2u, decompressor.get_fill_count(line_size, pmin, pmax, data+3));

        // until the end
        RED_CHECK_EQUAL(8u, decompressor.get_fill_count(line_size, pmin, pmax, data+8));

    }

    // test MIX COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0xFF, 0xFF, 0xFF, 0xFF,
            0x01, 0x05, 0x01, 0x05,
            0xFE, 0xFA, 0xFE, 0x01,
            0x01, 0x05, 0x01, 0x05};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        const size_t line_size = 4;

        // Line above first line is black
        RED_CHECK_EQUAL(4u, decompressor.get_mix_count(line_size, pmin, pmax, data, white));
        RED_CHECK_EQUAL(3u, decompressor.get_mix_count(line_size, pmin, pmax, data+1, white));

        // 3rd line, compared to 2nd line
        RED_CHECK_EQUAL(3u, decompressor.get_mix_count(line_size, pmin, pmax, data+8, white));
    }

    // test MIX COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x01, 0xFF, 0x01, 0xFF,
            0xFE, 0x05, 0x01, 0x01,
            0x01, 0xFA, 0xFE, 0xFE,
            0xFE, 0x05, 0x01, 0x01};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        const size_t line_size = 4;

        // Line above first line is black
        RED_CHECK_EQUAL(0u, decompressor.get_mix_count(line_size, pmin, pmax, data, white));
        RED_CHECK_EQUAL(1u, decompressor.get_mix_count(line_size, pmin, pmax, data+1, white));
        RED_CHECK_EQUAL(0u, decompressor.get_mix_count(line_size, pmin, pmax, data+2, white));
        RED_CHECK_EQUAL(2u, decompressor.get_mix_count(line_size, pmin, pmax, data+3, white));

        // until the end
        RED_CHECK_EQUAL(8u, decompressor.get_mix_count(line_size, pmin, pmax, data+8, white));
    }

    // test FILL OR MIX COUNT
    {
        RLEDecompressorImpl<8> decompressor;
        uint8_t data[4*4] = {
            0x02, 0x03, 0x04, 0x05,
            0xFD, 0x03, 0xFB, 0x05,
            0xFD, 0xFC, 0xFB, 0xFA,
            0x02, 0x03, 0x04, 0xFA};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        const size_t line_size = 4;

        // Line above first line is black
        uint8_t masks[512];
        unsigned flags = 0;
        unsigned color = white;
        RED_CHECK_EQUAL(1u, decompressor.get_fom_count_set(line_size, pmin, pmax, data+15, color, flags));
        RED_CHECK_EQUAL(white, color);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FILL), flags);
        RED_CHECK_EQUAL(2u, decompressor.get_fom_count_set(line_size, pmin, pmax, data+14, color, flags));
        RED_CHECK_EQUAL(white, color);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FOM), flags);
        decompressor.get_fom_masks(line_size, pmin, pmin+14, masks, 2);
        RED_CHECK_EQUAL(0x01, masks[0]);


        RED_CHECK_EQUAL(4u, decompressor.get_fom_count_set(line_size, pmin, pmax, data+12, color, flags));
        RED_CHECK_EQUAL(white, color);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FOM), flags);
        decompressor.get_fom_masks(line_size, pmin, pmin+12, masks, 4);
        RED_CHECK_EQUAL(0x07, masks[0]);

        RED_CHECK_EQUAL(5u, decompressor.get_fom_count_set(line_size, pmin, pmax, data+11, color, flags));
        RED_CHECK_EQUAL(white, color);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FOM), flags);

        RED_CHECK_EQUAL(6u, decompressor.get_fom_count_set(line_size, pmin, pmax, data+10, color, flags));
        RED_CHECK_EQUAL(white, color);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FOM), flags);
        RED_CHECK_EQUAL(12u, decompressor.get_fom_count_set(line_size, pmin, pmax, data+4, color, flags));
        RED_CHECK_EQUAL(white, color);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FOM), flags);
        decompressor.get_fom_masks(line_size, pmin, pmin+4, masks, 12);
        RED_CHECK_EQUAL(0xA5, masks[0]);
        RED_CHECK_EQUAL(0x07, masks[1]);
    }

    {
        const int bpp = 8;
        RLEDecompressorImpl<bpp> decompressor;
        uint8_t data[4*4] = {
            0x01, 0x02, 0x03, 0x04,
            0x05, 0x06, 0x07, 0x08,
            0x09, 0x0A, 0x0B, 0x0C,
            0x0D, 0x0E, 0x0F, 0x10};

        const uint8_t * pmin = data;
        const uint8_t * pmax = pmin + sizeof(data);
        const size_t line_size = 4;

        unsigned flags = 0;
        const uint8_t * p = data+3;
        unsigned foreground = decompressor.get_pixel_above(line_size, pmin, p) ^ decompressor.get_pixel(p);
        RED_CHECK_EQUAL(0x04u, foreground);
        RED_CHECK_EQUAL(3u, decompressor.get_mix_count(line_size, pmin, pmax, p+nbbytes(bpp), foreground));
        foreground = white;
        RED_CHECK_EQUAL(4u, decompressor.get_fom_count_set(line_size, pmin, pmax, p, foreground, flags));
        RED_CHECK_EQUAL(0x04u, foreground);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_MIX), flags);
    }


    {
        const int bpp = 8;
        RLEDecompressorImpl<bpp> decompressor;
        uint8_t multicolor[4*4] = {
            0x01, 0x02, 0x03, 0x04,
            0x01, 0x01, 0x01, 0x01,
            0x05, 0x06, 0x07, 0x08,
            0x01, 0x01, 0x01, 0x01};

        const uint8_t * pmin = multicolor;
        const uint8_t * pmax = pmin + sizeof(multicolor);
        const uint8_t * p = multicolor+3;
        const size_t line_size = 4;

        unsigned foreground = decompressor.get_pixel_above(line_size, pmin, p) ^ decompressor.get_pixel(p);
        RED_CHECK_EQUAL(4u, foreground);
        RED_CHECK_EQUAL(0u, decompressor.get_mix_count(line_size, pmin, pmax, p+nbbytes(bpp), foreground));
        foreground = white;
        unsigned flags = 0;
        RED_CHECK_EQUAL(2u, decompressor.get_fom_count_set(line_size, pmin, pmax, p, foreground, flags));
        RED_CHECK_EQUAL(4u, foreground);
        RED_CHECK_EQUAL(static_cast<unsigned>(RLEDecompressorImpl<0>::FLAG_FOM), flags); // MIX then FILL
    }
}

RED_AUTO_TEST_CASE(TestRDP60BitmapGetRun) {
    uint32_t run_length;
    uint32_t raw_bytes;

    get_run(byte_ptr_cast("AAAABBCCCCCD"), 12, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(3u,  run_length);
    RED_CHECK_EQUAL(1u,  raw_bytes);

    get_run(byte_ptr_cast("BBCCCCCD"), 8, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(4u,  run_length);
    RED_CHECK_EQUAL(3u,  raw_bytes);

    get_run(byte_ptr_cast("D"), 1, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(0u,  run_length);
    RED_CHECK_EQUAL(1u,  raw_bytes);

    get_run(byte_ptr_cast("ABCDEFGHIJKL"), 12, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(0u,  run_length);
    RED_CHECK_EQUAL(12u, raw_bytes);

    get_run(byte_ptr_cast("ABCDEFGHIJKLMNOP"), 16, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(0u,  run_length);
    RED_CHECK_EQUAL(16u, raw_bytes);

    get_run(byte_ptr_cast("ABCDEFGHIJKLMNOOOOO"), 19, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(4u,  run_length);
    RED_CHECK_EQUAL(15u, raw_bytes);

    get_run(byte_ptr_cast("\0\0\0\0\0\0\0\0"), 8, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(8u,  run_length);
    RED_CHECK_EQUAL(0u,  raw_bytes);

    get_run(byte_ptr_cast("AAABB"), 5, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(0u,  run_length);
    RED_CHECK_EQUAL(5u,  raw_bytes);

    get_run(byte_ptr_cast("\0\0\0\0\0\0\0\0"), 8, 0, run_length, raw_bytes);
    RED_CHECK_EQUAL(8u,  run_length);
    RED_CHECK_EQUAL(0u,  raw_bytes);

    get_run(byte_ptr_cast("\0\0\0\0\0\0\0\0"), 8, 0x64, run_length, raw_bytes);
    RED_CHECK_EQUAL(7u,  run_length);
    RED_CHECK_EQUAL(1u,  raw_bytes);
}

RED_AUTO_TEST_CASE(TestRDP60BitmapCompressColorPlane) {
    uint8_t data[] = {
        "AAAAAAAAAA" "AAAAAAAAAA" "AAAAAAAAAA" "AAAAAAAAAA" "AAAAAAAAAA"
        "AAAAAAAAAA" "AAAAAAAAAA" "AAAAAAAAAA" "AAAAAAAAAA" "AAAAAAAAAA"
    };

    StaticOutStream<1024> outbuffer;

    compress_color_plane(100, 1, outbuffer, data);

    RED_CHECK(outbuffer.get_produced_bytes() == "\x1F\x41\xF2\x52"_av);
}

RED_AUTO_TEST_CASE(TestRDP60BitmapCompressColorPlane1) {
    uint8_t data[] = {
        255, 255, 255, 255, 254, 253,
        254, 192, 132,  96,  75,  25,
        253, 140,  62,  14, 135, 193
    };

    StaticOutStream<1024> outbuffer;

    compress_color_plane(6, 3, outbuffer, data);

    RED_CHECK(outbuffer.get_produced_bytes() ==
        "\x13\xFF\x20\xFE\xFD\x60\x01\x7D\xF5\xC2\x9A\x38\x60\x01\x67\x8B\xA3\x78\xAF"_av);
}

RED_AUTO_TEST_CASE(TestRDP60BitmapCompressColorPlane2) {
    uint8_t data[] = {
/* 0000 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff,  // ..........`.....
/* 0010 */ 0xff, 0x30, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x28, 0xff, 0xff, 0xff, 0xff,  // .0.........(....
/* 0020 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff,  // ................
/* 0030 */ 0xe7, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xbf, 0xff, 0xff, 0xff,  // ................
/* 0040 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff,  // ................
/* 0050 */ 0x9f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x58, 0xff, 0xff, 0xff,  // ............X...
/* 0060 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // .........@......
/* 0070 */ 0x60, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xef, 0xff, 0xff,  // `...............
/* 0080 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // ................
/* 0090 */ 0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x8f, 0xff, 0xff,  // ................
/* 00a0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf,  // ................
/* 00b0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x30, 0xff, 0xff,  // .............0..
/* 00c0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x18, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,  // ................
/* 00d0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xc7, 0xff,  // ................
/* 00e0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,  // ........`......@
/* 00f0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x68, 0xff,  // ..............h.
/* 0100 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x08,  // ................
/* 0110 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xf7,  // ................
/* 0120 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x08,  // ................
/* 0130 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x97,  // ................
/* 0140 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x60, 0x08,  // .......8......`.
/* 0150 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38,  // ...............8
/* 0160 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x08,  // .............. .
/* 0170 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0180 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd7, 0x08, 0x08,  // ................
/* 0190 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 01a0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x08, 0x08,  // ................
/* 01b0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 01c0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x58, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40, 0x08, 0x08,  // ......X......@..
/* 01d0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 01e0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x08, 0x08, 0x08,  // ................
/* 01f0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0200 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x08, 0x08, 0x08,  // ................
/* 0210 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0220 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x30, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x70, 0x08, 0x08, 0x08,  // .....0......p...
/* 0230 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0240 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x28, 0x08, 0x08, 0x08,  // .....x......(...
/* 0250 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0260 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0270 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0280 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x97, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0290 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 02a0 */ 0x08, 0x08, 0x08, 0x08, 0x50, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x50, 0x08, 0x08, 0x08, 0x08,  // ....P......P....
/* 02b0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 02c0 */ 0x08, 0x08, 0x08, 0x08, 0x97, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x09, 0x08, 0x08, 0x08, 0x08,  // ................
/* 02d0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 02e0 */ 0x08, 0x08, 0x08, 0x08, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 02f0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0300 */ 0x08, 0x08, 0x08, 0x20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x08, 0x08, 0x08, 0x08, 0x08,  // ... ............
/* 0310 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0320 */ 0x08, 0x08, 0x08, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x30, 0x08, 0x08, 0x08, 0x08, 0x08,  // ...p......0.....
/* 0330 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0340 */ 0x08, 0x08, 0x08, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0350 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0360 */ 0x08, 0x08, 0x08, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0370 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 0380 */ 0x08, 0x08, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x60, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ..@......`......
/* 0390 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 03a0 */ 0x08, 0x08, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 03b0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 03c0 */ 0x08, 0x08, 0xd7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 03d0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 03e0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
/* 03f0 */ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  // ................
    };

    StaticOutStream<1024> outbuffer;

    compress_color_plane(32, 32, outbuffer, data);

    uint8_t result[] = {
/* 0000 */ 0x19, 0x08, 0x25, 0x60, 0xff, 0x28, 0x30, 0x08, 0x23, 0x28, 0xff, 0x0a, 0x24, 0x9e, 0x00, 0x38,  // ..%`.(0.#(..$..8
/* 0010 */ 0x2f, 0x4f, 0x00, 0x50, 0x3f, 0x7f, 0x00, 0x00, 0x00, 0x0a, 0x24, 0x80, 0x00, 0x2a, 0x8f, 0x00,  // /O.P?.....$..*..
/* 0020 */ 0x40, 0xcd, 0x00, 0x00, 0x00, 0x09, 0x34, 0x70, 0x20, 0x00, 0x2a, 0x7d, 0x00, 0x40, 0x9f, 0x1f,  // @.....4p .*}.@..
/* 0030 */ 0x00, 0x00, 0x09, 0x25, 0x80, 0x00, 0x2b, 0xad, 0x00, 0x30, 0xbf, 0x00, 0x00, 0x09, 0x24, 0x9e,  // ...%..+..0....$.
/* 0040 */ 0x00, 0x3b, 0x5f, 0x01, 0x00, 0x30, 0xbd, 0x00, 0x00, 0x08, 0x34, 0x20, 0x60, 0x00, 0x2c, 0x9d,  // .;_..0....4 `.,.
/* 0050 */ 0x00, 0x30, 0x4f, 0x6f, 0x00, 0x08, 0x25, 0x90, 0x00, 0x2d, 0x7f, 0x00, 0x20, 0xbd, 0x00, 0x08,  // .0Oo..%..-.. ...
/* 0060 */ 0x24, 0x7e, 0x00, 0x3d, 0x1f, 0x6f, 0x00, 0x20, 0xbf, 0x0f, 0x08, 0x24, 0xa0, 0x00, 0x2f, 0x7f,  // $~.=.o. ...$../.
/* 0070 */ 0x00, 0x10, 0xbf, 0x07, 0x34, 0x60, 0x20, 0x00, 0x2f, 0x9d, 0x00, 0x10, 0xbd, 0x07, 0x25, 0x90,  // ....4` ./.....%.
/* 0080 */ 0x00, 0x2f, 0x7f, 0x00, 0x10, 0x5f, 0x07, 0x24, 0x7e, 0x00, 0x3f, 0x4f, 0x2f, 0x00, 0x10, 0x00,  // ./..._.$~.?O/...
/* 0090 */ 0x06, 0x34, 0x02, 0x80, 0x00, 0x2f, 0x8f, 0x00, 0x20, 0x00, 0x00, 0x06, 0x25, 0x9e, 0x00, 0x2f,  // .4.../.. ...%../
/* 00a0 */ 0x9d, 0x00, 0x20, 0x00, 0x00, 0x06, 0x24, 0x8e, 0x00, 0x3f, 0x0f, 0x6f, 0x00, 0x20, 0x00, 0x00,  // .. ...$..?.o. ..
/* 00b0 */ 0x06, 0x24, 0x80, 0x00, 0x2f, 0x7f, 0x00, 0x03, 0x05, 0x34, 0x50, 0x40, 0x00, 0x2f, 0x8d, 0x00,  // .$../....4P@./..
/* 00c0 */ 0x03, 0x05, 0x25, 0x90, 0x00, 0x2f, 0x8f, 0x00, 0x03, 0x05, 0x24, 0x8e, 0x00, 0x3f, 0x3f, 0x3f,  // ..%../....$..???
/* 00d0 */ 0x00, 0x03, 0x05, 0x24, 0x70, 0x00, 0x2f, 0x8f, 0x00, 0x04, 0x04, 0x34, 0x90, 0x10, 0x00, 0x2f,  // ...$p./....4.../
/* 00e0 */ 0x8d, 0x00, 0x04, 0x04, 0x24, 0x8e, 0x00, 0x3f, 0x0f, 0x8d, 0x00, 0x04, 0x04, 0x24, 0x90, 0x00,  // ....$..?.....$..
/* 00f0 */ 0x3f, 0x6f, 0x01, 0x00, 0x04, 0x64, 0x00, 0x00, 0x00, 0x30, 0x40, 0x00, 0x2f, 0x7d, 0x00, 0x05,  // ?o...d...0@./}..
/* 0100 */ 0x55, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x2f, 0x9f, 0x00, 0x05, 0x54, 0x00, 0x00, 0x00, 0x8e, 0x00,  // U...../...T.....
/* 0110 */ 0x3f, 0x2f, 0x4f, 0x00, 0x05, 0x54, 0x00, 0x00, 0x00, 0x80, 0x00, 0x2f, 0x8f, 0x00, 0x06, 0x54,  // ?/O..T...../...T
/* 0120 */ 0x00, 0x00, 0x70, 0x10, 0x00, 0x2f, 0x7d, 0x00, 0x06, 0x45, 0x00, 0x00, 0x9e, 0x00, 0x2f, 0xad,  // ..p../}..E..../.
/* 0130 */ 0x00, 0x06, 0x44, 0x00, 0x00, 0x90, 0x00, 0x3f, 0x6f, 0x01, 0x00, 0x06, 0x44, 0x00, 0x00, 0x62,  // ..D....?o...D..b
/* 0140 */ 0x12, 0x2f, 0x82, 0x00, 0x07,                                   // ./...
    };

    RED_CHECK(outbuffer.get_produced_bytes() == make_array_view(result));
}

RED_AUTO_TEST_CASE(TestRDP60BitmapDecompressColorPlane) {
    uint8_t data[] = {
        0x13, 0xFF, 0x20, 0xFE, 0xFD,
        0x60, 0x01, 0x7D, 0xF5, 0xC2, 0x9A, 0x38,
        0x60, 0x01, 0x67, 0x8B, 0xA3, 0x78, 0xAF
    };

    const uint8_t * compressed_data      = data;
    size_t          compressed_data_size = sizeof(data);

    uint8_t color_plane[6 * 3];

    ::decompress_color_plane(6, 3, compressed_data, compressed_data_size, 6, color_plane);

    uint8_t result[] = {
        255, 255, 255, 255, 254, 253,
        254, 192, 132, 96,  75,  25,
        253, 140, 62,  14,  135, 193
    };

    RED_CHECK(make_array_view(color_plane) == make_array_view(result));
}
