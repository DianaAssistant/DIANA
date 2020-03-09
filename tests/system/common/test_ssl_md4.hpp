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


RED_AUTO_TEST_CASE(TestSslMd4)
{
    uint8_t sig[SslMd4::DIGEST_LENGTH];
    uint8_t data[512] = {
        /* 0000 */ 0xba, 0x96, 0x63, 0xe9, 0x11, 0x42, 0xf1, 0xec,  // ..c..B..
        /* 0008 */ 0x31, 0xed, 0xd5, 0xb4, 0xcd, 0x2e, 0xee, 0x64,  // 1......d
        /* 0010 */ 0x77, 0x2c, 0x53, 0x6d, 0xef, 0x3b, 0xb7, 0xf9,  // w,Sm.;..
        /* 0018 */ 0x1d, 0x2c, 0x3c, 0x90, 0xcb, 0x60, 0x5a, 0x27,  // .,<..`Z'
        /* 0020 */ 0xeb, 0x82, 0xf0, 0x1d, 0x87, 0x01, 0x9e, 0xf7,  // ........
        /* 0028 */ 0x29, 0x4a, 0xcb, 0x7e, 0x4d, 0x14, 0xf6, 0x9c,  // )J.~M...
        /* 0030 */ 0xd6, 0x17, 0x9d, 0xd4, 0x92, 0x6d, 0x3d, 0xda,  // .....m=.
        /* 0038 */ 0x6f, 0x23, 0xeb, 0xea, 0x43, 0x1b, 0x01, 0x31,  // o#..C..1
        /* 0040 */ 0xca, 0x93, 0x7a, 0x4d, 0xa3, 0xf2, 0x0b, 0x30,  // ..zM...0
        /* 0048 */ 0xeb, 0xdb, 0x2b, 0xf3, 0xb9, 0xfc, 0xcb, 0xbc,  // ..+.....
        /* 0050 */ 0xd5, 0x0f, 0x22, 0x19, 0x92, 0x47, 0x36, 0x0f,  // .."..G6.
        /* 0058 */ 0xf3, 0x79, 0xe3, 0x4b, 0x08, 0x64, 0x29, 0xf6,  // .y.K.d).
        /* 0060 */ 0x64, 0x8a, 0xdc, 0x57, 0xc9, 0x2b, 0xb3, 0xbe,  // d..W.+..
        /* 0068 */ 0x2a, 0xff, 0x24, 0xbb, 0xa2, 0xd0, 0x1b, 0x0f,  // *.$.....
        /* 0070 */ 0xea, 0x65, 0xaa, 0x11, 0x6c, 0x0e, 0x45, 0x56,  // .e..l.EV
        /* 0078 */ 0x9e, 0xf0, 0x54, 0x00, 0x55, 0x5a, 0x12, 0x4b,  // ..T.UZ.K
        /* 0080 */ 0x94, 0x71, 0xf5, 0x03, 0x52, 0x56, 0xb9, 0x13,  // .q..RV..
        /* 0088 */ 0xfc, 0x25, 0x09, 0x80, 0x51, 0x23, 0x10, 0x19,  // .%..Q#..
        /* 0090 */ 0x9a, 0x68, 0x2c, 0xf5, 0x7e, 0xd6, 0xb0, 0xbf,  // .h,.~...
        /* 0098 */ 0x07, 0xf0, 0xf2, 0x69, 0x75, 0xa6, 0x4f, 0x40,  // ...iu.O@
        /* 00a0 */ 0x0d, 0x8f, 0xaf, 0x2f, 0x61, 0x62, 0xd7, 0xcb,  // .../ab..
        /* 00a8 */ 0x51, 0x1a, 0x1e, 0x03, 0x7b, 0xa8, 0x6e, 0x41,  // Q...{.nA
        /* 00b0 */ 0xec, 0x3a, 0x08, 0x45, 0xf3, 0xa1, 0xbf, 0x9c,  // .:.E....
        /* 00b8 */ 0xcc, 0xd7, 0xcd, 0x59, 0x77, 0x49, 0x40, 0xd0,  // ...YwI@.
        /* 00c0 */ 0xb0, 0x33, 0x97, 0x5a, 0xbf, 0xba, 0x5e, 0x3a,  // .3.Z..^:
        /* 00c8 */ 0x77, 0x81, 0x55, 0xa5, 0x7f, 0xdc, 0x71, 0x43,  // w.U...qC
        /* 00d0 */ 0x4e, 0x6b, 0xbc, 0x0f, 0xc9, 0xaf, 0xfc, 0xcf,  // Nk......
        /* 00d8 */ 0xc4, 0x2b, 0x02, 0x65, 0x4f, 0x09, 0x92, 0x64,  // .+.eO..d
        /* 00e0 */ 0x2d, 0x29, 0x23, 0xea, 0xf5, 0x02, 0xe3, 0xd2,  // -)#.....
        /* 00e8 */ 0x05, 0x21, 0x06, 0xde, 0x6a, 0x0d, 0xd2, 0xe7,  // .!..j...
        /* 00f0 */ 0xf8, 0x2b, 0xbd, 0xea, 0x07, 0xc6, 0xc2, 0x79,  // .+.....y
        /* 00f8 */ 0x1c, 0xa5, 0x10, 0xf4, 0xb5, 0x51, 0x4f, 0x03,  // .....QO.
        /* 0100 */ 0x5e, 0x23, 0x2a, 0x67, 0xd6, 0x65, 0x5b, 0x94,  // ^#*g.e[.
        /* 0108 */ 0xe2, 0x1c, 0xda, 0x78, 0xf6, 0x7b, 0xd8, 0x97,  // ...x.{..
        /* 0110 */ 0x71, 0xb4, 0x1a, 0x05, 0xc1, 0x62, 0xfb, 0x38,  // q....b.8
        /* 0118 */ 0x92, 0xba, 0x2a, 0x21, 0x65, 0xdb, 0x0e, 0xb2,  // ..*!e...
        /* 0120 */ 0xcb, 0xa1, 0x78, 0x29, 0x9b, 0xed, 0x3d, 0xbe,  // ..x)..=.
        /* 0128 */ 0x84, 0x8b, 0xe9, 0xc9, 0x82, 0xfe, 0x5f, 0x66,  // ......_f
        /* 0130 */ 0x24, 0x16, 0x51, 0xdf, 0x6c, 0xd6, 0xc8, 0x7f,  // $.Q.l...
        /* 0138 */ 0x99, 0xe1, 0x2a, 0xe4, 0x39, 0x1c, 0x49, 0xc4,  // ..*.9.I.
        /* 0140 */ 0xbc, 0xd2, 0x76, 0xd3, 0x2a, 0x58, 0x48, 0x5b,  // ..v.*XH[
        /* 0148 */ 0x13, 0xc4, 0x9c, 0x81, 0xc1, 0x4a, 0xec, 0x5b,  // .....J.[
        /* 0150 */ 0xab, 0x7f, 0xfa, 0x96, 0x02, 0xc6, 0x19, 0x1e,  // ........
        /* 0158 */ 0xbd, 0x83, 0x35, 0x1b, 0xfd, 0x26, 0x68, 0xba,  // ..5..&h.
        /* 0160 */ 0x68, 0x70, 0x47, 0x5a, 0xfc, 0xb0, 0xc8, 0x15,  // hpGZ....
        /* 0168 */ 0x50, 0xe0, 0x4d, 0x81, 0x64, 0x79, 0x31, 0x80,  // P.M.dy1.
        /* 0170 */ 0x5b, 0x5c, 0x17, 0xa2, 0x56, 0x3f, 0x15, 0xd6,  // [...V?..
        /* 0178 */ 0x74, 0x22, 0x3e, 0x79, 0x19, 0x69, 0x42, 0x5c,  // t">y.iB.
        /* 0180 */ 0x8c, 0x7d, 0x73, 0xbe, 0x71, 0x11, 0x39, 0xca,  // .}s.q.9.
        /* 0188 */ 0x80, 0xcb, 0xc1, 0xb6, 0x76, 0xb8, 0x14, 0xfb,  // ....v...
        /* 0190 */ 0x03, 0x35, 0x3d, 0xdf, 0x00, 0x08, 0x2c, 0x3a,  // .5=...,:
        /* 0198 */ 0x76, 0xff, 0x3b, 0xca, 0xca, 0x47, 0x0c, 0x58,  // v.;..G.X
        /* 01a0 */ 0x56, 0x21, 0xcb, 0x75, 0xa4, 0x88, 0x75, 0xef,  // V!.u..u.
        /* 01a8 */ 0xd9, 0x8c, 0x87, 0x69, 0xed, 0x4a, 0x72, 0xb6,  // ...i.Jr.
        /* 01b0 */ 0x9e, 0x61, 0x03, 0x0b, 0xe5, 0xac, 0x90, 0x0d,  // .a......
        /* 01b8 */ 0x5c, 0x03, 0x9a, 0x2f, 0x51, 0x53, 0xc6, 0x5e,  // .../QS.^
        /* 01c0 */ 0xbd, 0x58, 0xdc, 0x52, 0xc9, 0xb1, 0x85, 0x4e,  // .X.R...N
        /* 01c8 */ 0x1b, 0x06, 0x20, 0xb3, 0x0f, 0x02, 0x96, 0x3c,  // .. ....<
        /* 01d0 */ 0xa0, 0x86, 0xff, 0xdf, 0xca, 0xf2, 0xdb, 0x96,  // ........
        /* 01d8 */ 0xc2, 0xc8, 0xce, 0xa8, 0x6d, 0xae, 0x56, 0x9f,  // ....m.V.
        /* 01e0 */ 0xfd, 0x6d, 0xf5, 0x51, 0x13, 0x71, 0xe9, 0x27,  // .m.Q.q.'
        /* 01e8 */ 0x15, 0x1d, 0x4c, 0x4e, 0x88, 0xf3, 0xc8, 0x0f,  // ..LN....
        /* 01f0 */ 0x09, 0xca, 0xcc, 0x22, 0x28, 0x66, 0xae, 0x8e,  // ..."(f..
        /* 01f8 */ 0xd4, 0x96, 0xd4, 0x5a, 0x38, 0xf6, 0x33, 0xfd,  // ...Z8.3.
    };

    {
        SslMd4 md;

        md.update(make_array_view(data));
        md.final(sig);
        // hexdump96_c(sig, sizeof(sig));

        RED_CHECK(make_array_view(sig) ==
            "\x33\xce\xe5\xdd\x0b\x6f\x3a\xf7\xd9\xa4\xa1\x9a\xbc\x1b\xc6\x58"_av);
    }

    {
        SslMd4 md;

        md.update({data, 128});
        md.update({data + 128, 128});
        md.update({data + 256, 128});
        md.update({data + 384, 128});
        md.final(sig);
        // hexdump96_c(sig, sizeof(sig));

        RED_CHECK(make_array_view(sig) ==
            "\x33\xce\xe5\xdd\x0b\x6f\x3a\xf7\xd9\xa4\xa1\x9a\xbc\x1b\xc6\x58"_av);
    }
}

RED_AUTO_TEST_CASE(TestSslHmacMd4)
{
    SslHMAC_Md4 hmac(cstr_array_view("key"));

    hmac.update(cstr_array_view("The quick brown fox jumps over the lazy dog"));

    uint8_t sig[SslMd4::DIGEST_LENGTH];
    hmac.final(sig);
    // hexdump96_c(sig, sizeof(sig));
    RED_CHECK(make_array_view(sig) ==
        "\x8d\x33\x66\xc4\x40\xa9\xc6\x51\x24\xab\x0b\x5f\x4c\xa2\x73\x38"_av);
}
