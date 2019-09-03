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
  Copyright (C) Wallix 2013
  Author(s): Christophe Grosjean, Raphael Zhou, Meng Tan
*/

#include "test_only/test_framework/redemption_unit_tests.hpp"


#include "core/RDP/nla/credssp.hpp"

#include "test_only/check_sig.hpp"


RED_AUTO_TEST_CASE(TestBERIntegerCase1)
{
    auto r = BER::mkInteger(114178754);
    RED_CHECK_EQUAL(r.size(), 6);

    InStream in_s(r);
    uint32_t value = BER::read_integer(in_s, "Integer", ERR_CREDSSP_TS_REQUEST);
    RED_CHECK_EQUAL(value, 114178754);
}

RED_AUTO_TEST_CASE(TestBERIntegerCase2)
{
    auto r = BER::mkInteger(1);
    RED_CHECK_EQUAL(r.size(), 3);

    InStream in_s = InStream(r);
    uint32_t value = BER::read_integer(in_s, "Integer", ERR_CREDSSP_TS_REQUEST);

    RED_CHECK_EQUAL(value, 1);
}

RED_AUTO_TEST_CASE(TestBERIntegerCase3)
{
    auto r = BER::mkInteger(52165);
    RED_CHECK_EQUAL(r.size(), 5);

    InStream in_s = InStream(r);
    uint32_t value = BER::read_integer(in_s, "Integer", ERR_CREDSSP_TS_REQUEST);

    RED_CHECK_EQUAL(value, 52165);
}

RED_AUTO_TEST_CASE(TestBERIntegerCase4)
{
    auto r = BER::mkInteger(0x0FFF);
    RED_CHECK_EQUAL(r.size(), 4);

    InStream in_s = InStream(r);
    uint32_t value = BER::read_integer(in_s, "Integer", ERR_CREDSSP_TS_REQUEST);

    RED_CHECK_EQUAL(value, 0x0FFF);
}

RED_AUTO_TEST_CASE(TestBEROctetString)
{
    StaticOutStream<2048> s;
    const uint8_t oct_str[] = {1, 2, 3, 4, 5, 6, 7};

    auto r = BER::mkOctetStringHeader(sizeof(oct_str));
    r.insert(r.end(), oct_str, oct_str+sizeof(oct_str));

    InStream in_s(r);
    int length = BER::read_tag_length(in_s, BER::CLASS_UNIV|BER::PC_PRIMITIVE|BER::TAG_OCTET_STRING, "TestOctetString", ERR_CREDSSP_TS_REQUEST);
    RED_CHECK_EQUAL(length, 7);
}


RED_AUTO_TEST_CASE(TestBERContextual)
{
    StaticOutStream<2048> s;
    uint8_t tag = 0x06;

    const uint8_t data[] = {0, 1, 2};

    auto v = BER::mkContextualFieldHeader(sizeof(data), tag);
    v.insert(v.end(), data, data+sizeof(data));

    InStream in_s(v);
    RED_CHECK_EQUAL(true, BER::check_ber_ctxt_tag(in_s, tag));
    in_s.in_skip_bytes(1);
    int length = BER::read_length(in_s, "TS Request [1] negoTokens", ERR_CREDSSP_TS_REQUEST);
    RED_CHECK_EQUAL(length, sizeof(data));
}

RED_AUTO_TEST_CASE(TestTSRequestNTLMSSP_NEGOTIATE)
{
    // ===== NTLMSSP_NEGOTIATE =====
    constexpr static uint8_t packet[] = {
        0x30, 0x37, 0xa0, 0x03, 0x02, 0x01, 0x03, 0xa1,
        0x30, 0x30, 0x2e, 0x30, 0x2c, 0xa0, 0x2a, 0x04,
        0x28, 0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50,
        0x00, 0x01, 0x00, 0x00, 0x00, 0xb7, 0x82, 0x08,
        0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x05, 0x01, 0x28, 0x0a, 0x00, 0x00, 0x00,
        0x0f
    };

    TSRequest ts_req = recvTSRequest(make_array_view(packet), 3);

    RED_CHECK_EQUAL(ts_req.version, 3);
    RED_CHECK_EQUAL(ts_req.negoTokens.size(), 0x28);
    RED_CHECK_EQUAL(ts_req.authInfo.size(), 0);
    RED_CHECK_EQUAL(ts_req.error_code, 0);
    RED_CHECK_EQUAL(ts_req.pubKeyAuth.size(), 0);

    auto v = emitTSRequest(ts_req.version,
                           ts_req.negoTokens,
                           ts_req.authInfo,
                           ts_req.pubKeyAuth,
                           ts_req.error_code,
                           ts_req.clientNonce.clientNonce,
                           ts_req.clientNonce.initialized);
    RED_CHECK_EQUAL(v.size(), 0x37 + 2);
    RED_CHECK_SIG_FROM(v, packet);
}

RED_AUTO_TEST_CASE(TestTSRequestNTLMSSP_CHALLENGE)
{
// ===== NTLMSSP_CHALLENGE =====
    constexpr static uint8_t packet[] = {
        0x30, 0x81, 0x94, 0xa0, 0x03, 0x02, 0x01, 0x03,
        0xa1, 0x81, 0x8c, 0x30, 0x81, 0x89, 0x30, 0x81,
        0x86, 0xa0, 0x81, 0x83, 0x04, 0x81, 0x80, 0x4e,
        0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x38,
        0x00, 0x00, 0x00, 0x35, 0x82, 0x8a, 0xe2, 0x26,
        0x6e, 0xcd, 0x75, 0xaa, 0x41, 0xe7, 0x6f, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
        0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x06,
        0x01, 0xb0, 0x1d, 0x00, 0x00, 0x00, 0x0f, 0x57,
        0x00, 0x49, 0x00, 0x4e, 0x00, 0x37, 0x00, 0x02,
        0x00, 0x08, 0x00, 0x57, 0x00, 0x49, 0x00, 0x4e,
        0x00, 0x37, 0x00, 0x01, 0x00, 0x08, 0x00, 0x57,
        0x00, 0x49, 0x00, 0x4e, 0x00, 0x37, 0x00, 0x04,
        0x00, 0x08, 0x00, 0x77, 0x00, 0x69, 0x00, 0x6e,
        0x00, 0x37, 0x00, 0x03, 0x00, 0x08, 0x00, 0x77,
        0x00, 0x69, 0x00, 0x6e, 0x00, 0x37, 0x00, 0x07,
        0x00, 0x08, 0x00, 0xa9, 0x8d, 0x9b, 0x1a, 0x6c,
        0xb0, 0xcb, 0x01, 0x00, 0x00, 0x00, 0x00
    };

    TSRequest ts_req = recvTSRequest(make_array_view(packet), 3);

    RED_CHECK_EQUAL(ts_req.version, 3);
    RED_CHECK_EQUAL(ts_req.negoTokens.size(), 0x80);
    RED_CHECK_EQUAL(ts_req.authInfo.size(), 0);
    RED_CHECK_EQUAL(ts_req.pubKeyAuth.size(), 0);

    auto v = emitTSRequest(ts_req.version,
                           ts_req.negoTokens,
                           ts_req.authInfo,
                           ts_req.pubKeyAuth,
                           ts_req.error_code,
                           ts_req.clientNonce.clientNonce,
                           ts_req.clientNonce.initialized);
    RED_CHECK_EQUAL(v.size(), 0x94 + 3);
    RED_CHECK_SIG_FROM(v, packet);
}


RED_AUTO_TEST_CASE(TestTSRequestNTLMSSP_AUTH)
{
    // ===== NTLMSSP_AUTH =====
    constexpr static uint8_t packet[] = {
        0x30, 0x82, 0x02, 0x41, 0xa0, 0x03, 0x02, 0x01,
        0x03, 0xa1, 0x82, 0x01, 0x12, 0x30, 0x82, 0x01,
        0x0e, 0x30, 0x82, 0x01, 0x0a, 0xa0, 0x82, 0x01,
        0x06, 0x04, 0x82, 0x01, 0x02, 0x4e, 0x54, 0x4c,
        0x4d, 0x53, 0x53, 0x50, 0x00, 0x03, 0x00, 0x00,
        0x00, 0x18, 0x00, 0x18, 0x00, 0x6a, 0x00, 0x00,
        0x00, 0x70, 0x00, 0x70, 0x00, 0x82, 0x00, 0x00,
        0x00, 0x08, 0x00, 0x08, 0x00, 0x48, 0x00, 0x00,
        0x00, 0x10, 0x00, 0x10, 0x00, 0x50, 0x00, 0x00,
        0x00, 0x0a, 0x00, 0x0a, 0x00, 0x60, 0x00, 0x00,
        0x00, 0x10, 0x00, 0x10, 0x00, 0xf2, 0x00, 0x00,
        0x00, 0x35, 0x82, 0x88, 0xe2, 0x05, 0x01, 0x28,
        0x0a, 0x00, 0x00, 0x00, 0x0f, 0x77, 0x00, 0x69,
        0x00, 0x6e, 0x00, 0x37, 0x00, 0x75, 0x00, 0x73,
        0x00, 0x65, 0x00, 0x72, 0x00, 0x6e, 0x00, 0x61,
        0x00, 0x6d, 0x00, 0x65, 0x00, 0x57, 0x00, 0x49,
        0x00, 0x4e, 0x00, 0x58, 0x00, 0x50, 0x00, 0xa0,
        0x98, 0x01, 0x10, 0x19, 0xbb, 0x5d, 0x00, 0xf6,
        0xbe, 0x00, 0x33, 0x90, 0x20, 0x34, 0xb3, 0x47,
        0xa2, 0xe5, 0xcf, 0x27, 0xf7, 0x3c, 0x43, 0x01,
        0x4a, 0xd0, 0x8c, 0x24, 0xb4, 0x90, 0x74, 0x39,
        0x68, 0xe8, 0xbd, 0x0d, 0x2b, 0x70, 0x10, 0x01,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3,
        0x83, 0xa2, 0x1c, 0x6c, 0xb0, 0xcb, 0x01, 0x47,
        0xa2, 0xe5, 0xcf, 0x27, 0xf7, 0x3c, 0x43, 0x00,
        0x00, 0x00, 0x00, 0x02, 0x00, 0x08, 0x00, 0x57,
        0x00, 0x49, 0x00, 0x4e, 0x00, 0x37, 0x00, 0x01,
        0x00, 0x08, 0x00, 0x57, 0x00, 0x49, 0x00, 0x4e,
        0x00, 0x37, 0x00, 0x04, 0x00, 0x08, 0x00, 0x77,
        0x00, 0x69, 0x00, 0x6e, 0x00, 0x37, 0x00, 0x03,
        0x00, 0x08, 0x00, 0x77, 0x00, 0x69, 0x00, 0x6e,
        0x00, 0x37, 0x00, 0x07, 0x00, 0x08, 0x00, 0xa9,
        0x8d, 0x9b, 0x1a, 0x6c, 0xb0, 0xcb, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb1,
        0xd2, 0x45, 0x42, 0x0f, 0x37, 0x9a, 0x0e, 0xe0,
        0xce, 0x77, 0x40, 0x10, 0x8a, 0xda, 0xba, 0xa3,
        0x82, 0x01, 0x22, 0x04, 0x82, 0x01, 0x1e, 0x01,
        0x00, 0x00, 0x00, 0x91, 0x5e, 0xb0, 0x6e, 0x72,
        0x82, 0x53, 0xae, 0x00, 0x00, 0x00, 0x00, 0x27,
        0x29, 0x73, 0xa9, 0xfa, 0x46, 0x17, 0x3c, 0x74,
        0x14, 0x45, 0x2a, 0xd1, 0xe2, 0x92, 0xa1, 0xc6,
        0x0a, 0x30, 0xd4, 0xcc, 0xe0, 0x92, 0xf6, 0xb3,
        0x20, 0xb3, 0xa0, 0xf1, 0x38, 0xb1, 0xf4, 0xe5,
        0x96, 0xdf, 0xa1, 0x65, 0x5b, 0xd6, 0x0c, 0x2a,
        0x86, 0x99, 0xcc, 0x72, 0x80, 0xbd, 0xe9, 0x19,
        0x1f, 0x42, 0x53, 0xf6, 0x84, 0xa3, 0xda, 0x0e,
        0xec, 0x10, 0x29, 0x15, 0x52, 0x5c, 0x77, 0x40,
        0xc8, 0x3d, 0x44, 0x01, 0x34, 0xb6, 0x0a, 0x75,
        0x33, 0xc0, 0x25, 0x71, 0xd3, 0x25, 0x38, 0x3b,
        0xfc, 0x3b, 0xa8, 0xcf, 0xba, 0x2b, 0xf6, 0x99,
        0x0e, 0x5f, 0x4e, 0xa9, 0x16, 0x2b, 0x52, 0x9f,
        0xbb, 0x76, 0xf8, 0x03, 0xfc, 0x11, 0x5e, 0x36,
        0x83, 0xd8, 0x4c, 0x9a, 0xdc, 0x9d, 0x35, 0xe2,
        0xc8, 0x63, 0xa9, 0x3d, 0x07, 0x97, 0x52, 0x64,
        0x54, 0x72, 0x9e, 0x9a, 0x8c, 0x56, 0x79, 0x4a,
        0x78, 0x91, 0x0a, 0x4c, 0x52, 0x84, 0x5a, 0x4a,
        0xb8, 0x28, 0x0b, 0x2f, 0xe6, 0x89, 0x7d, 0x07,
        0x3b, 0x7b, 0x6e, 0x22, 0xcc, 0x4c, 0xff, 0xf4,
        0x10, 0x96, 0xf2, 0x27, 0x29, 0xa0, 0x76, 0x0d,
        0x4c, 0x7e, 0x7a, 0x42, 0xe4, 0x1e, 0x6a, 0x95,
        0x7d, 0x4c, 0xaf, 0xdb, 0x86, 0x49, 0x5c, 0xbf,
        0xc2, 0x65, 0xb6, 0xf2, 0xed, 0xae, 0x8d, 0x57,
        0xed, 0xf0, 0xd4, 0xcb, 0x7a, 0xbb, 0x23, 0xde,
        0xe3, 0x43, 0xea, 0xb1, 0x02, 0xe3, 0xb4, 0x96,
        0xe9, 0xe7, 0x48, 0x69, 0xb0, 0xaa, 0xec, 0x89,
        0x38, 0x8b, 0xc2, 0xbd, 0xdd, 0xf7, 0xdf, 0xa1,
        0x37, 0xe7, 0x34, 0x72, 0x7f, 0x91, 0x10, 0x14,
        0x73, 0xfe, 0x32, 0xdc, 0xfe, 0x68, 0x2b, 0xc0,
        0x08, 0xdf, 0x05, 0xf7, 0xbd, 0x46, 0x33, 0xfb,
        0xc9, 0xfc, 0x89, 0xaa, 0x5d, 0x25, 0x49, 0xc8,
        0x6e, 0x86, 0xee, 0xc2, 0xce, 0xc4, 0x8e, 0x85,
        0x9f, 0xe8, 0x30, 0xb3, 0x86, 0x11, 0xd5, 0xb8,
        0x34, 0x4a, 0xe0, 0x03, 0xe5
    };

    TSRequest ts_req = recvTSRequest(make_array_view(packet), 3);

    RED_CHECK_EQUAL(ts_req.version, 3);
    RED_CHECK_EQUAL(ts_req.negoTokens.size(), 0x102);
    RED_CHECK_EQUAL(ts_req.authInfo.size(), 0);
    RED_CHECK_EQUAL(ts_req.pubKeyAuth.size(), 0x11e);

    auto v = emitTSRequest(ts_req.version,
                           ts_req.negoTokens,
                           ts_req.authInfo,
                           ts_req.pubKeyAuth,
                           ts_req.error_code,
                           ts_req.clientNonce.clientNonce,
                           ts_req.clientNonce.initialized);
    RED_CHECK_EQUAL(v.size(), 0x241 + 4);
    RED_CHECK_SIG_FROM(v, packet);
}

RED_AUTO_TEST_CASE(TestTSRequestPUBKEYAUTH)
{
// ===== PUBKEYAUTH =====
    constexpr static uint8_t packet[] = {
        0x30, 0x82, 0x01, 0x2b, 0xa0, 0x03, 0x02, 0x01,
        0x03, 0xa3, 0x82, 0x01, 0x22, 0x04, 0x82, 0x01,
        0x1e, 0x01, 0x00, 0x00, 0x00, 0xc9, 0x88, 0xfc,
        0xf1, 0x11, 0x68, 0x2c, 0x72, 0x00, 0x00, 0x00,
        0x00, 0xc7, 0x51, 0xf4, 0x71, 0xd3, 0x9f, 0xb6,
        0x50, 0xbe, 0xa8, 0xf6, 0x20, 0x77, 0xa1, 0xfc,
        0xdd, 0x8e, 0x02, 0xf0, 0xa4, 0x6b, 0xba, 0x3f,
        0x9d, 0x65, 0x9d, 0xab, 0x4a, 0x95, 0xc9, 0xb4,
        0x38, 0x03, 0x87, 0x04, 0xb1, 0xfe, 0x42, 0xec,
        0xfa, 0xfc, 0xaa, 0x85, 0xf1, 0x31, 0x2d, 0x26,
        0xcf, 0x63, 0xfd, 0x62, 0x36, 0xcf, 0x56, 0xc3,
        0xfb, 0xf6, 0x36, 0x9b, 0xe5, 0xb2, 0xe7, 0xce,
        0xcb, 0xe1, 0x82, 0xb2, 0x89, 0xff, 0xdd, 0x87,
        0x5e, 0xd3, 0xd8, 0xff, 0x2e, 0x16, 0x35, 0xad,
        0xdb, 0xda, 0xc9, 0xc5, 0x81, 0xad, 0x48, 0xf1,
        0x8b, 0x76, 0x3d, 0x74, 0x34, 0xdf, 0x80, 0x6b,
        0xf3, 0x68, 0x6d, 0xf6, 0xec, 0x5f, 0xbe, 0xea,
        0xb7, 0x6c, 0xea, 0xe4, 0xeb, 0xe9, 0x17, 0xf9,
        0x4e, 0x0d, 0x79, 0xd5, 0x82, 0xdd, 0xb7, 0xdc,
        0xcd, 0xfc, 0xbb, 0xf1, 0x0b, 0x9b, 0xe9, 0x18,
        0xe7, 0xb3, 0xb3, 0x8b, 0x40, 0x82, 0xa0, 0x9d,
        0x58, 0x73, 0xda, 0x54, 0xa2, 0x2b, 0xd2, 0xb6,
        0x41, 0x60, 0x8a, 0x64, 0xf2, 0xa2, 0x59, 0x64,
        0xcf, 0x27, 0x1a, 0xe6, 0xb5, 0x1a, 0x0e, 0x0e,
        0xe1, 0x14, 0xef, 0x26, 0x68, 0xeb, 0xc8, 0x49,
        0xe2, 0x66, 0xbb, 0x11, 0x71, 0x49, 0xad, 0x7e,
        0xae, 0xde, 0xa8, 0x78, 0xfd, 0x64, 0x51, 0xd8,
        0x18, 0x01, 0x11, 0xc0, 0x8d, 0x3b, 0xec, 0x40,
        0x2b, 0x1f, 0xc5, 0xa4, 0x45, 0x1e, 0x07, 0xae,
        0x5a, 0xd8, 0x1c, 0xab, 0xdf, 0x89, 0x96, 0xdc,
        0xdc, 0x29, 0xd8, 0x30, 0xdb, 0xbf, 0x48, 0x2a,
        0x42, 0x27, 0xc2, 0x50, 0xac, 0xf9, 0x02, 0xd1,
        0x20, 0x12, 0xdd, 0x50, 0x22, 0x09, 0x44, 0xac,
        0xe0, 0x22, 0x1f, 0x66, 0x64, 0xec, 0xfa, 0x2b,
        0xb8, 0xcd, 0x43, 0x3a, 0xce, 0x40, 0x74, 0xe1,
        0x34, 0x81, 0xe3, 0x94, 0x47, 0x6f, 0x49, 0x01,
        0xf8, 0xb5, 0xfc, 0xd0, 0x75, 0x80, 0xc6, 0x35,
        0xac, 0xc0, 0xfd, 0x1b, 0xb5, 0xa2, 0xd3
    };

    TSRequest ts_req = recvTSRequest(make_array_view(packet), 3);

    RED_CHECK_EQUAL(ts_req.version, 3);
    RED_CHECK_EQUAL(ts_req.negoTokens.size(), 0);
    RED_CHECK_EQUAL(ts_req.authInfo.size(), 0);
    RED_CHECK_EQUAL(ts_req.pubKeyAuth.size(), 0x11e);

    auto v = emitTSRequest(ts_req.version,
                           ts_req.negoTokens,
                           ts_req.authInfo,
                           ts_req.pubKeyAuth,
                           ts_req.error_code,
                           ts_req.clientNonce.clientNonce,
                           ts_req.clientNonce.initialized);
    RED_CHECK_EQUAL(v.size(), 0x12b + 4);
    RED_CHECK_SIG_FROM(v, packet);
}

RED_AUTO_TEST_CASE(TestTSRequestAUTHINFO)
{
    // ===== AUTHINFO =====
    constexpr static uint8_t packet[] = {
        0x30, 0x5a, 0xa0, 0x03, 0x02, 0x01, 0x03, 0xa2,
        0x53, 0x04, 0x51, 0x01, 0x00, 0x00, 0x00, 0xb3,
        0x2c, 0x3b, 0xa1, 0x36, 0xf6, 0x55, 0x71, 0x01,
        0x00, 0x00, 0x00, 0xa8, 0x85, 0x7d, 0x11, 0xef,
        0x92, 0xa0, 0xd6, 0xff, 0xee, 0xa1, 0xae, 0x6d,
        0xc5, 0x2e, 0x4e, 0x65, 0x50, 0x28, 0x93, 0x75,
        0x30, 0xe1, 0xc3, 0x37, 0xeb, 0xac, 0x1f, 0xdd,
        0xf3, 0xe0, 0x92, 0xf6, 0x21, 0xbc, 0x8f, 0xa8,
        0xd4, 0xe0, 0x5a, 0xa6, 0xff, 0xda, 0x09, 0x50,
        0x24, 0x0d, 0x8f, 0x8f, 0xf4, 0x92, 0xfe, 0x49,
        0x2a, 0x13, 0x52, 0xa6, 0x52, 0x75, 0x50, 0x8d,
        0x3e, 0xe9, 0x6b, 0x57
    };
    TSRequest ts_req = recvTSRequest(make_array_view(packet), 3);

    RED_CHECK_EQUAL(ts_req.version, 3);

    RED_CHECK_EQUAL(ts_req.negoTokens.size(), 0);
    RED_CHECK_EQUAL(ts_req.authInfo.size(), 0x51);
    RED_CHECK_EQUAL(ts_req.pubKeyAuth.size(), 0);

    auto v = emitTSRequest(ts_req.version,
                           ts_req.negoTokens,
                           ts_req.authInfo,
                           ts_req.pubKeyAuth,
                           ts_req.error_code,
                           ts_req.clientNonce.clientNonce,
                           ts_req.clientNonce.initialized);

    RED_CHECK_EQUAL(v.size(), 0x5c);
    RED_CHECK_SIG_FROM(v, packet);
}

RED_AUTO_TEST_CASE(TestTSCredentialsPassword)
{

    std::vector<uint8_t> domain = {'f', 'l', 'a', 't', 'l', 'a', 'n', 'd', 0};
    std::vector<uint8_t> user = { 's', 'q', 'u', 'a', 'r', 'e', 0};
    std::vector<uint8_t> pass = {'h', 'y', 'p', 'e', 'r', 'c', 'u', 'b', 'e', 0};

    auto r = emitTSCredentialsPassword(domain, user, pass);
    RED_CHECK_EQUAL(r.size(), r[1]+2);

    

    TSCredentials ts_cred_received = recvTSCredentials(r);

    RED_CHECK_EQUAL(ts_cred_received.credType, 1);
    RED_CHECK_EQUAL(ts_cred_received.passCreds.domainName, domain);
    RED_CHECK_EQUAL(ts_cred_received.passCreds.userName, user);
    RED_CHECK_EQUAL(ts_cred_received.passCreds.password, pass);


    std::vector<uint8_t> domain2 = {'s','o','m','e','w','h','e','r','e', 0};
    std::vector<uint8_t> user2 = {'s', 'o', 'm', 'e', 'o', 'n', 'e', 0};
    std::vector<uint8_t> pass2 = {'s', 'o', 'm', 'e', 'p', 'a', 's', 's', 0};
}

RED_AUTO_TEST_CASE(TestTSCredentialsSmartCard)
{

    std::vector<uint8_t> pin = {'3', '6', '1', '5', 0};
    std::vector<uint8_t> userHint = {'a', 'k', 'a', 0};
    std::vector<uint8_t> domainHint = {'g', 'r', 'a', 'n', 'd', 'p', 'a', 'r', 'c', 0};

    std::vector<uint8_t> cardName = {'p', 'a', 's', 's', 'e', 'p', 'a', 'r', 't', 'o', 'u', 't', 0};
    std::vector<uint8_t> readerName = {'a', 'c', 'r', 'o', 'b', 'a', 't', 0};
    std::vector<uint8_t> containerName = {'d', 'o', 'c', 'k', 'e', 'r', 0};
    std::vector<uint8_t> cspName = {'w', 'h', 'a', 't', 0};
    uint32_t keySpec = 32;

    auto r = emitTSCredentialsSmartCard(pin, userHint, domainHint, keySpec, cardName, readerName, containerName, cspName);
    RED_CHECK_EQUAL(r.size(), r[1]+2);

    TSCredentials ts_cred = recvTSCredentials(r);

    RED_CHECK_EQUAL(ts_cred.credType, 2);
    RED_CHECK_EQUAL(ts_cred.smartcardCreds.pin, pin);
    RED_CHECK_EQUAL(ts_cred.smartcardCreds.userHint, userHint);
    RED_CHECK_EQUAL(ts_cred.smartcardCreds.domainHint, domainHint);
    RED_CHECK_EQUAL(ts_cred.smartcardCreds.cspData.keySpec, keySpec);
    
    RED_CHECK_SMEM(ts_cred.smartcardCreds.cspData.cardName, cardName);
    RED_CHECK_SMEM(ts_cred.smartcardCreds.cspData.readerName, readerName);
    RED_CHECK_SMEM(ts_cred.smartcardCreds.cspData.containerName, containerName);
    RED_CHECK_SMEM(ts_cred.smartcardCreds.cspData.cspName, cspName);
}

//rdpCredssp - Client Authentication : Sending Authentication Token
//TSRequest hexdump ---------------------------------
//TSRequest version 6 ------------------------------
//TSRequest negoTokens ------------------------------
///* 0000 */ 0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00, 0x01, 0x00, 0x00, 0x00, 0xb7, 0x82, 0x08, 0xe2,  // NTLMSSP.........
///* 0010 */ 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,  // ....(.......(...
///* 0020 */ 0x06, 0x01, 0xb1, 0x1d, 0x00, 0x00, 0x00, 0x0f,                                                  // ........
//TSRequest authInfo --------------------------------
//TSRequest pubkeyAuth ------------------------------
//TSRequest error_code 0 ---------------------------
//TSRequest clientNonce -----------------------------
///* 0000 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ................
///* 0010 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ................
//TSRequest ts_request_header -----------------------
//TSRequest version_field ---------------------------
///* 0000 */ 0xa0, 0x03, 0x02, 0x01, 0x06,                                                                    // .....
//TSRequest nego_tokens_header ----------------------
///* 0000 */ 0xa1, 0x30, 0x30, 0x2e, 0x30, 0x2c, 0xa0, 0x2a, 0x04, 0x28,                                      // .00.0,.*.(
//TSRequest auth_info_header ------------------------
//TSRequest pub_key_auth_header ---------------------
//TSRequest error_code field ------------------------
//TSRequest nonce -----------------------------------
//emit TSRequest full dump--------------------------------
///* 0000 */ 0x30, 0x37, 0xa0, 0x03, 0x02, 0x01, 0x06, 0xa1, 0x30, 0x30, 0x2e, 0x30, 0x2c, 0xa0, 0x2a, 0x04,  // 07......00.0,.*.
///* 0010 */ 0x28, 0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00, 0x01, 0x00, 0x00, 0x00, 0xb7, 0x82, 0x08,  // (NTLMSSP........
///* 0020 */ 0xe2, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00,  // .....(.......(..
///* 0030 */ 0x00, 0x06, 0x01, 0xb1, 0x1d, 0x00, 0x00, 0x00, 0x0f,                                            // .........
//emit TSRequest hexdump -DONE----------------------------



RED_AUTO_TEST_CASE(TestCredssp_scenarized_nla_ntlm)
{
    //activating CREDSSP
    //Credssp: NTLM Authentication
    //NTLM Send Negotiate
    //rdpCredssp - Client Authentication : Sending Authentication Token

    uint32_t version = 6;
    std::vector<uint8_t> negoTokens = {
        0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00, 0x01, 0x00, 0x00, 0x00, 0xb7, 0x82, 0x08, 0xe2,  // NTLMSSP.........
        0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,  // ....(.......(...
        0x06, 0x01, 0xb1, 0x1d, 0x00, 0x00, 0x00, 0x0f,
    };
    std::vector<uint8_t> authInfo = {};
    std::vector<uint8_t> pubKeyAuth = {};
    uint32_t error_code = 0;
    std::vector<uint8_t> clientNonce = {};

    auto result = emitTSRequest(version, negoTokens, authInfo, pubKeyAuth, error_code, clientNonce, false);

    std::vector<uint8_t> expectedResult = {
        // TSRequest ber sequence header
        0x30, 0x37, 
        // version field
        0xa0, 0x03, 0x02, 0x01, 0x06, 
        // negoTokens ber header
        0xa1, 0x30, 0x30, 0x2e, 0x30, 0x2c, 0xa0, 0x2a, 0x04, 0x28, // 07......00.0,.*.(
        // negoTokens
        0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00, 0x01, 0x00, 0x00, 0x00, 0xb7, 0x82, 0x08, 0xe2, // NTLMSSP........
        0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, // .....(.......(..
        0x06, 0x01, 0xb1, 0x1d, 0x00, 0x00, 0x00, 0x0f,  
    };

    RED_CHECK_EQUAL(expectedResult, result);

}

//rdpproxy: [rdpproxy] psid="156751302831637" user="u" type="TARGET_CONNECTION" target="Christophe" host="10.10.44.101" port="3389"
//connecting to 10.10.44.101:3389
//connection to 10.10.44.101:3389 (10.10.44.101) succeeded : socket 7
//SocketTransport: recv_timeout=1000
//Remote RDP Server domain="" login="Christophe" host="cgrthc"
//RdpNego: TLS=Enabled NLA=Enabled adminMode=Disabled
//Server key layout is 409
//ModuleManager::Creation of new mod 'RDP' suceeded
//RdpNego::NEGO_STATE_INITIAL
//RdpNego::send_x224_connection_request_pdu
//Send cookie:
///* 0000 */ "\x43\x6f\x6f\x6b\x69\x65\x3a\x20\x6d\x73\x74\x73\x68\x61\x73\x68" // Cookie: mstshash
///* 0010 */ "\x3d\x43\x68\x72\x69\x73\x74\x6f\x70\x68\x65\x0d\x0a"             // =Christophe..
//RdpNego::send_x224_connection_request_pdu done
//RdpNego::recv_next_data::Negociate
//nego->state=RdpNego::NEGO_STATE_NEGOCIATE
//RdpNego::NEGO_STATE_NLA
//RdpNego::recv_connection_confirm
//NEG_RSP_TYPE=2 NEG_RSP_FLAGS=9 NEG_RSP_LENGTH=8 NEG_RSP_SELECTED_PROTOCOL=2
//CC Recv: PROTOCOL HYBRID
//activating TLS (HYBRID)
//RdpNego::activate_ssl_hybrid
//Client TLS start
//SSL_connect()
//RdpNego::recv_next_data::SslHybrid
//RdpNego::activate_ssl_hybrid
//RdpNego::recv_next_data::SslHybrid
//RdpNego::activate_ssl_hybrid
//SSL_get_peer_certificate()
//certificate directory is: '/etc/rdpproxy/cert/0000'
//certificate file is: '/etc/rdpproxy/cert/0000/rdp,10.10.44.101,3389,X509.pem'
//nb1=692 nb2=692
//TLS::X509 existing::issuer=CN = localhost
//TLS::X509 existing::subject=CN = localhost
//TLS::X509 existing::fingerprint=13:0c:f2:13:13:4f:08:60:67:e5:6b:78:80:c1:bb:ea:59:8d:e0:35
//rdpproxy: [RDP Session] session_id="SESSIONID-0000" client_ip="10.10.43.12" target_ip="10.10.44.101" user="u" device="win2008rzh" service="rdp" account="Christophe" type="SERVER_CERTIFICATE_MATCH_SUCCESS" description="X.509 server certificate match"
//rdpproxy: [RDP Session] session_id="SESSIONID-0000" client_ip="10.10.43.12" target_ip="10.10.44.101" user="u" device="win2008rzh" service="rdp" account="Christophe" type="CERTIFICATE_CHECK_SUCCESS" description="Connexion to server allowed"
//TLS::X509::issuer=CN = localhost
//TLS::X509::subject=CN = localhost
//TLS::X509::fingerprint=13:0c:f2:13:13:4f:08:60:67:e5:6b:78:80:c1:bb:ea:59:8d:e0:35
//TLSContext::X509_get_pubkey()
//TLSContext::i2d_PublicKey()
//Connected to target using TLS version TLSv1.2
//SocketTransport::enable_client_tls() done




//RdpNego::recv_next_data::Credssp
//RdpNego::recv_credssp
//recv TSRequest full dump--------------------------------
///* 0000 */ "\x30\x82\x01\x02\xa0\x03\x02\x01\x05\xa1\x81\xfa\x30\x81\xf7\x30" // 0...........0..0
///* 0010 */ "\x81\xf4\xa0\x81\xf1\x04\x81\xee\x4e\x54\x4c\x4d\x53\x53\x50\x00" // ........NTLMSSP.
///* 0020 */ "\x02\x00\x00\x00\x1e\x00\x1e\x00\x38\x00\x00\x00\x35\x82\x8a\xe2" // ........8...5...
///* 0030 */ "\x3e\xc6\x6f\xb0\x06\x1e\x8b\xb2\x00\x00\x00\x00\x00\x00\x00\x00" // >.o.............
///* 0040 */ "\x98\x00\x98\x00\x56\x00\x00\x00\x06\x01\xb1\x1d\x00\x00\x00\x0f" // ....V...........
///* 0050 */ "\x52\x00\x44\x00\x50\x00\x2d\x00\x57\x00\x49\x00\x4e\x00\x44\x00" // R.D.P.-.W.I.N.D.
///* 0060 */ "\x4f\x00\x57\x00\x53\x00\x2d\x00\x44\x00\x45\x00\x56\x00\x02\x00" // O.W.S.-.D.E.V...
///* 0070 */ "\x1e\x00\x52\x00\x44\x00\x50\x00\x2d\x00\x57\x00\x49\x00\x4e\x00" // ..R.D.P.-.W.I.N.
///* 0080 */ "\x44\x00\x4f\x00\x57\x00\x53\x00\x2d\x00\x44\x00\x45\x00\x56\x00" // D.O.W.S.-.D.E.V.
///* 0090 */ "\x01\x00\x1e\x00\x52\x00\x44\x00\x50\x00\x2d\x00\x57\x00\x49\x00" // ....R.D.P.-.W.I.
///* 00a0 */ "\x4e\x00\x44\x00\x4f\x00\x57\x00\x53\x00\x2d\x00\x44\x00\x45\x00" // N.D.O.W.S.-.D.E.
///* 00b0 */ "\x56\x00\x04\x00\x1e\x00\x72\x00\x64\x00\x70\x00\x2d\x00\x77\x00" // V.....r.d.p.-.w.
///* 00c0 */ "\x69\x00\x6e\x00\x64\x00\x6f\x00\x77\x00\x73\x00\x2d\x00\x64\x00" // i.n.d.o.w.s.-.d.
///* 00d0 */ "\x65\x00\x76\x00\x03\x00\x1e\x00\x72\x00\x64\x00\x70\x00\x2d\x00" // e.v.....r.d.p.-.
///* 00e0 */ "\x77\x00\x69\x00\x6e\x00\x64\x00\x6f\x00\x77\x00\x73\x00\x2d\x00" // w.i.n.d.o.w.s.-.
///* 00f0 */ "\x64\x00\x65\x00\x76\x00\x07\x00\x08\x00\x9b\xe0\x43\x90\x51\x62" // d.e.v.......C.Qb
///* 0100 */ "\xd5\x01\x00\x00\x00\x00"                                         // ......
//recv TSRequest hexdump - START PARSING DATA-------------
//Credssp recvTSCredentials() Remote Version 5, Negotiated version 5
//rdpCredssp - Client Authentication : Receiving Authentication Token
//NTLMContextClient Read Challenge
//NTLMContextClient Compute response from challenge
//NTLMContextClient TimeStamp
//NTLMContextClient Generate Client Challenge nonce(8)
//using /dev/urandom as random source
//NTLMContextClient Compute response: NtProofStr
//NTLMContextClient Compute response: NtChallengeResponse
//Compute response: NtChallengeResponse Ready
//NTLMContextClient Compute response: LmChallengeResponse
//NTLMContextClient Compute response: SessionBaseKey
//NTLMContextClient Encrypt RandomSessionKey
//NTLMContextClient Generate Exported Session Key
//using /dev/urandom as random source
//rdpproxy: DEBUG (31637/31637) -- Field LmChallengeResponse, len: 24, maxlen: 24, offset: 0
///* 0000 */ 0x6c, 0xb2, 0x8c, 0xd0, 0x98, 0xad, 0x94, 0xb7, 0xf2, 0x0c, 0xa1, 0x70, 0x51, 0x62, 0xb6, 0x78,  // l..........pQb.x
///* 0010 */ 0x5f, 0x61, 0x16, 0x68, 0xa3, 0x93, 0xe7, 0x44,                                                  // _a.h...D
//rdpproxy: DEBUG (31637/31637) -- Field NtChallengeResponse, len: 200, maxlen: 200, offset: 0
///* 0000 */ 0xf3, 0x6c, 0x31, 0x08, 0x86, 0x6d, 0xb8, 0x73, 0x2c, 0x3b, 0x00, 0x27, 0x3d, 0x4b, 0x6d, 0x30,  // .l1..m.s,;.'=Km0
///* 0010 */ 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x94, 0x0a, 0x00, 0xde, 0x59, 0x6e, 0x5d,  // ........"....Yn]
///* 0020 */ 0x5f, 0x61, 0x16, 0x68, 0xa3, 0x93, 0xe7, 0x44, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x1e, 0x00,  // _a.h...D........
///* 0030 */ 0x52, 0x00, 0x44, 0x00, 0x50, 0x00, 0x2d, 0x00, 0x57, 0x00, 0x49, 0x00, 0x4e, 0x00, 0x44, 0x00,  // R.D.P.-.W.I.N.D.
///* 0040 */ 0x4f, 0x00, 0x57, 0x00, 0x53, 0x00, 0x2d, 0x00, 0x44, 0x00, 0x45, 0x00, 0x56, 0x00, 0x01, 0x00,  // O.W.S.-.D.E.V...
///* 0050 */ 0x1e, 0x00, 0x52, 0x00, 0x44, 0x00, 0x50, 0x00, 0x2d, 0x00, 0x57, 0x00, 0x49, 0x00, 0x4e, 0x00,  // ..R.D.P.-.W.I.N.
///* 0060 */ 0x44, 0x00, 0x4f, 0x00, 0x57, 0x00, 0x53, 0x00, 0x2d, 0x00, 0x44, 0x00, 0x45, 0x00, 0x56, 0x00,  // D.O.W.S.-.D.E.V.
///* 0070 */ 0x04, 0x00, 0x1e, 0x00, 0x72, 0x00, 0x64, 0x00, 0x70, 0x00, 0x2d, 0x00, 0x77, 0x00, 0x69, 0x00,  // ....r.d.p.-.w.i.
///* 0080 */ 0x6e, 0x00, 0x64, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00, 0x2d, 0x00, 0x64, 0x00, 0x65, 0x00,  // n.d.o.w.s.-.d.e.
///* 0090 */ 0x76, 0x00, 0x03, 0x00, 0x1e, 0x00, 0x72, 0x00, 0x64, 0x00, 0x70, 0x00, 0x2d, 0x00, 0x77, 0x00,  // v.....r.d.p.-.w.
///* 00a0 */ 0x69, 0x00, 0x6e, 0x00, 0x64, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00, 0x2d, 0x00, 0x64, 0x00,  // i.n.d.o.w.s.-.d.
///* 00b0 */ 0x65, 0x00, 0x76, 0x00, 0x07, 0x00, 0x08, 0x00, 0x9b, 0xe0, 0x43, 0x90, 0x51, 0x62, 0xd5, 0x01,  // e.v.......C.Qb..
///* 00c0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                  // ........
//rdpproxy: DEBUG (31637/31637) -- Field DomainName, len: 0, maxlen: 0, offset: 0
//rdpproxy: DEBUG (31637/31637) -- Field UserName, len: 20, maxlen: 20, offset: 0
///* 0000 */ 0x43, 0x00, 0x68, 0x00, 0x72, 0x00, 0x69, 0x00, 0x73, 0x00, 0x74, 0x00, 0x6f, 0x00, 0x70, 0x00,  // C.h.r.i.s.t.o.p.
///* 0010 */ 0x68, 0x00, 0x65, 0x00,                                                                          // h.e.
//rdpproxy: DEBUG (31637/31637) -- Field Workstation, len: 12, maxlen: 12, offset: 0
///* 0000 */ 0x63, 0x00, 0x67, 0x00, 0x72, 0x00, 0x74, 0x00, 0x68, 0x00, 0x63, 0x00,                          // c.g.r.t.h.c.
//rdpproxy: DEBUG (31637/31637) -- Field EncryptedRandomSessionKey, len: 16, maxlen: 16, offset: 0
///* 0000 */ 0x20, 0x06, 0x82, 0x94, 0x77, 0x96, 0x0c, 0x77, 0x0c, 0x1d, 0x9c, 0xf5, 0x51, 0x47, 0x3f, 0x84,  //  ...w..w....QG?.
//negotiateFlags "0xE288A235"{
//	NTLMSSP_NEGOTIATE_56 (31),
//	NTLMSSP_NEGOTIATE_KEY_EXCH (30),
//	NTLMSSP_NEGOTIATE_128 (29),
//	NTLMSSP_NEGOTIATE_VERSION (25),
//	NTLMSSP_NEGOTIATE_TARGET_INFO (23),
//	NTLMSSP_NEGOTIATE_EXTENDED_SESSION_SECURITY (19),
//	NTLMSSP_NEGOTIATE_ALWAYS_SIGN (15),
//	NTLMSSP_NEGOTIATE_WORKSTATION_SUPPLIED (13),
//	NTLMSSP_NEGOTIATE_NTLM (9),
//	NTLMSSP_NEGOTIATE_SEAL (5),
//	NTLMSSP_NEGOTIATE_SIGN (4),
//	NTLMSSP_REQUEST_TARGET (2),
//	NTLMSSP_NEGOTIATE_UNICODE (0),
//}
//VERSION = {
//	ProductMajorVersion: 6
//	ProductMinorVersion: 1
//	ProductBuild: 7601
//	NTLMRevisionCurrent: 0x0F
//}
//rdpproxy: DEBUG (31637/31637) -- MIC
///* 0000 */ 0x0d, 0x05, 0x25, 0xab, 0x57, 0x6d, 0xe3, 0x81, 0x14, 0xc1, 0x5c, 0x52, 0xb7, 0x7a, 0x53, 0xaf,  // ..%.Wm.....R.zS.
//rdpCredsspClientNTLM::encrypt_public_key_echo
//rdpCredsspClientNTLM::credssp generate client nonce
//using /dev/urandom as random source
//rdpCredsspClientNTLM::generate credssp public key hash (client->server)
//rdpCredssp - Client Authentication : Sending Authentication Token
//rdpCredsspClientNTLM::send
//TSRequest hexdump ---------------------------------
//TSRequest ts_request_header -----------------------
//TSRequest version_field ---------------------------
///* 0000 */ "\xa0\x03\x02\x01\x06"                                             // .....
//TSRequest nego_tokens_header ----------------------
///* 0000 */ "\xa1\x82\x01\x78\x30\x82\x01\x74\x30\x82\x01\x70\xa0\x82\x01\x6c" // ...x0..t0..p...l
///* 0010 */ "\x04\x82\x01\x68"                                                 // ...h
//TSRequest auth_info_header ------------------------
//TSRequest pub_key_auth_header ---------------------
///* 0000 */ "\xa3\x32\x04\x30"                                                 // .2.0
//TSRequest error_code field ------------------------
//TSRequest nonce -----------------------------------
///* 0000 */ "\xa5\x22\x04\x20"                                                 // .". 
//emit TSRequest full dump--------------------------------
///* 0000 */ "\x30\x82\x01\xd9\xa0\x03\x02\x01\x06\xa1\x82\x01\x78\x30\x82\x01" // 0...........x0..
///* 0010 */ "\x74\x30\x82\x01\x70\xa0\x82\x01\x6c\x04\x82\x01\x68\x4e\x54\x4c" // t0..p...l...hNTL
///* 0020 */ "\x4d\x53\x53\x50\x00\x03\x00\x00\x00\x18\x00\x18\x00\x58\x00\x00" // MSSP.........X..
///* 0030 */ "\x00\xc8\x00\xc8\x00\x70\x00\x00\x00\x00\x00\x00\x00\x38\x01\x00" // .....p.......8..
///* 0040 */ "\x00\x14\x00\x14\x00\x38\x01\x00\x00\x0c\x00\x0c\x00\x4c\x01\x00" // .....8.......L..
///* 0050 */ "\x00\x10\x00\x10\x00\x58\x01\x00\x00\x35\xa2\x88\xe2\x06\x01\xb1" // .....X...5......
///* 0060 */ "\x1d\x00\x00\x00\x0f\x0d\x05\x25\xab\x57\x6d\xe3\x81\x14\xc1\x5c" // .......%.Wm.....
///* 0070 */ "\x52\xb7\x7a\x53\xaf\x6c\xb2\x8c\xd0\x98\xad\x94\xb7\xf2\x0c\xa1" // R.zS.l..........
///* 0080 */ "\x70\x51\x62\xb6\x78\x5f\x61\x16\x68\xa3\x93\xe7\x44\xf3\x6c\x31" // pQb.x_a.h...D.l1
///* 0090 */ "\x08\x86\x6d\xb8\x73\x2c\x3b\x00\x27\x3d\x4b\x6d\x30\x01\x01\x00" // ..m.s,;.'=Km0...
///* 00a0 */ "\x00\x00\x00\x00\x00\x22\x94\x0a\x00\xde\x59\x6e\x5d\x5f\x61\x16" // ....."....Yn]_a.
///* 00b0 */ "\x68\xa3\x93\xe7\x44\x00\x00\x00\x00\x02\x00\x1e\x00\x52\x00\x44" // h...D........R.D
///* 00c0 */ "\x00\x50\x00\x2d\x00\x57\x00\x49\x00\x4e\x00\x44\x00\x4f\x00\x57" // .P.-.W.I.N.D.O.W
///* 00d0 */ "\x00\x53\x00\x2d\x00\x44\x00\x45\x00\x56\x00\x01\x00\x1e\x00\x52" // .S.-.D.E.V.....R
///* 00e0 */ "\x00\x44\x00\x50\x00\x2d\x00\x57\x00\x49\x00\x4e\x00\x44\x00\x4f" // .D.P.-.W.I.N.D.O
///* 00f0 */ "\x00\x57\x00\x53\x00\x2d\x00\x44\x00\x45\x00\x56\x00\x04\x00\x1e" // .W.S.-.D.E.V....
///* 0100 */ "\x00\x72\x00\x64\x00\x70\x00\x2d\x00\x77\x00\x69\x00\x6e\x00\x64" // .r.d.p.-.w.i.n.d
///* 0110 */ "\x00\x6f\x00\x77\x00\x73\x00\x2d\x00\x64\x00\x65\x00\x76\x00\x03" // .o.w.s.-.d.e.v..
///* 0120 */ "\x00\x1e\x00\x72\x00\x64\x00\x70\x00\x2d\x00\x77\x00\x69\x00\x6e" // ...r.d.p.-.w.i.n
///* 0130 */ "\x00\x64\x00\x6f\x00\x77\x00\x73\x00\x2d\x00\x64\x00\x65\x00\x76" // .d.o.w.s.-.d.e.v
///* 0140 */ "\x00\x07\x00\x08\x00\x9b\xe0\x43\x90\x51\x62\xd5\x01\x00\x00\x00" // .......C.Qb.....
///* 0150 */ "\x00\x00\x00\x00\x00\x43\x00\x68\x00\x72\x00\x69\x00\x73\x00\x74" // .....C.h.r.i.s.t
///* 0160 */ "\x00\x6f\x00\x70\x00\x68\x00\x65\x00\x63\x00\x67\x00\x72\x00\x74" // .o.p.h.e.c.g.r.t
///* 0170 */ "\x00\x68\x00\x63\x00\x20\x06\x82\x94\x77\x96\x0c\x77\x0c\x1d\x9c" // .h.c. ...w..w...
///* 0180 */ "\xf5\x51\x47\x3f\x84\xa3\x32\x04\x30\x01\x00\x00\x00\x40\xb2\xca" // .QG?..2.0....@..
///* 0190 */ "\x75\xd3\xb8\x27\x64\x00\x00\x00\x00\x26\x36\x74\x9f\xea\x8a\xe1" // u..'d....&6t....
///* 01a0 */ "\x10\xd0\x18\xb2\x85\xf5\x78\xf4\xe1\x65\x87\x57\x83\xe6\x26\x5b" // ......x..e.W..&[
///* 01b0 */ "\x1b\xdd\xa0\x31\x0a\x16\x55\x8a\x5c\xa5\x22\x04\x20\x06\x10\xff" // ...1..U...". ...
///* 01c0 */ "\xc4\x8f\xfb\x1c\x08\x76\x47\xf6\x5d\x62\xca\x60\x68\x63\x72\x55" // .....vG.]b.`hcrU
///* 01d0 */ "\x17\x1a\xf7\x68\xae\x24\x6a\x42\x01\x9f\x0e\xab\x0e"             // ...h.$jB.....
//emit TSRequest hexdump -DONE----------------------------
//RdpNego::recv_next_data::Credssp
//RdpNego::recv_credssp
//rdpCredssp - Client Authentication : Receiving Encrypted PubKey + 1
//recv TSRequest full dump--------------------------------
///* 0000 */ "\x30\x39\xa0\x03\x02\x01\x05\xa3\x32\x04\x30\x01\x00\x00\x00\xe1" // 09......2.0.....
///* 0010 */ "\x01\xea\xac\xa0\xc2\x9d\x6c\x00\x00\x00\x00\x96\xd0\x35\x03\x05" // ......l......5..
///* 0020 */ "\x0c\xb8\x70\xc9\xad\x21\x39\x01\x1f\xaf\xd9\x21\xb6\xeb\xf2\xc7" // ..p..!9....!....
///* 0030 */ "\xb1\x70\xed\x5e\xb5\x5c\xb1\xe5\xe8\x4a\x8c"                     // .p.^.....J.
//recv TSRequest hexdump - START PARSING DATA-------------
//Credssp recvTSCredentials() Remote Version 5, Negotiated version 5
//rdpCredsspClientNTLM::encrypt_ts_credentials
//TSRequest hexdump ---------------------------------
//TSRequest ts_request_header -----------------------
//TSRequest version_field ---------------------------
///* 0000 */ "\xa0\x03\x02\x01\x06"                                             // .....
//TSRequest nego_tokens_header ----------------------
//TSRequest auth_info_header ------------------------
///* 0000 */ "\xa2\x5b\x04\x59"                                                 // .[.Y
//TSRequest pub_key_auth_header ---------------------
//TSRequest error_code field ------------------------
//TSRequest nonce -----------------------------------
///* 0000 */ "\xa5\x22\x04\x20"                                                 // .". 
//emit TSRequest full dump--------------------------------
///* 0000 */ "\x30\x81\x86\xa0\x03\x02\x01\x06\xa2\x5b\x04\x59\x01\x00\x00\x00" // 0........[.Y....
///* 0010 */ "\xa6\x0b\x7c\x3b\xe5\xc7\xc0\x87\x01\x00\x00\x00\xd1\xa5\xed\x58" // ..|;...........X
///* 0020 */ "\x9f\x49\x90\xf9\x07\x2e\xec\x8d\xf2\x9c\xac\x51\x4d\x29\x35\x49" // .I.........QM)5I
///* 0030 */ "\xc9\x1c\x71\xef\xe1\xdf\x02\x2e\x23\x30\x0b\x99\x23\x5c\xe3\x96" // ..q.....#0..#...
///* 0040 */ "\xea\x29\xd5\x71\xd2\x60\x57\xfb\x29\xa3\x52\xfd\xef\xe4\xb8\x2e" // .).q.`W.).R.....
///* 0050 */ "\xe3\x85\xa1\x37\xff\x2a\x41\x88\x9c\x26\x93\xb8\x0e\x47\xfe\xa8" // ...7.*A..&...G..
///* 0060 */ "\x35\xf3\xdb\xc3\xf4\xa5\x22\x04\x20\x06\x10\xff\xc4\x8f\xfb\x1c" // 5.....". .......
///* 0070 */ "\x08\x76\x47\xf6\x5d\x62\xca\x60\x68\x63\x72\x55\x17\x1a\xf7\x68" // .vG.]b.`hcrU...h
///* 0080 */ "\xae\x24\x6a\x42\x01\x9f\x0e\xab\x0e"                             // .$jB.....
//emit TSRequest hexdump -DONE----------------------------
//CS_Cluster: Server Redirection Supported
//GCC::UserData tag=0c01 length=12
//GCC::UserData tag=0c03 length=16
//GCC::UserData tag=0c02 length=12
//=================== SC_SECURITY =============
//No encryption
//send extended login info (RDP5-style) 107fb :Christophe
//recv_tls WANT READ
//Rdp::Get license: username="Christophe"
//RdpNegociation::get_license LIC::RecvFactory::flic.tag=1
//Rdp::Get license: username="Christophe"
//RdpNegociation::get_license LIC::RecvFactory::flic.tag=2
//Rdp::Get license: username="Christophe"
//RdpNegociation::get_license LIC::RecvFactory::flic.tag=255
///* 0000 */ 0xff, 0x03, 0x10, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,  // ................
//use rdp5
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Rdp::receiving the server-to-client Monitor Layout PDU MonitorLayoutPDU monitorCount=1 ((left=0, top=0, right=1023, bottom=767, primary=yes(0x1)))
//Resizing to 1024x768x16
//rdpproxy: [RDP Session] session_id="SESSIONID-0000" client_ip="10.10.43.12" target_ip="10.10.44.101" user="u" device="win2008rzh" service="rdp" account="Christophe" type="SESSION_ESTABLISHED_SUCCESSFULLY"
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Front::can_be_start_capture: Capture is not necessary
//Front::incoming: (Fast-Path) Synchronize Event toggleFlags=0x0
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Front::incoming: (Fast-Path) Synchronize Event toggleFlags=0x0
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Front::incoming: (Fast-Path) Synchronize Event toggleFlags=0x0
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//process error info pdu: code=0x00000000 error=?
//Deactivate All PDU
//use rdp5
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Rdp::receiving the server-to-client Monitor Layout PDU MonitorLayoutPDU monitorCount=1 ((left=0, top=0, right=1023, bottom=767, primary=yes(0x1)))
//Resizing to 1024x768x16
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Front::can_be_start_capture: Capture is not necessary
//Front::incoming: (Fast-Path) Synchronize Event toggleFlags=0x0
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//Front::incoming: (Fast-Path) Synchronize Event toggleFlags=0x0
//mod_rdp::send_input_fastpath: Synchronize Event toggleFlags=0x0
//process save session info : Logon long
//Logon Info Version 2 (data): Domain="RDP-WINDOWS-DEV" UserName="Christophe" SessionId=6
//process save session info : Logon extended info
//process save session info : Auto-reconnect cookie
//ServerAutoReconnectPacket: Version=1 LogonId=6
//0000 de 55 e4 17 97 e7 e3 8b 89 4c 09 a3 6d 8d d8 8f  .U.......L..m...
//got SIGPIPE(13) : ignoring
//Failure in SSL library, error=6, Broken pipe [32]
//partial_send_tls error:00000006:lib(0):func(0):EVP lib
//rdpproxy: WARNING (31637/31637) -- SocketTransport::Send failed on RDP Client (5) errno=32 [Broken pipe]
//mod_rdp::draw_event() state switch raised exception
//===========> MODULE_NEXT
//----------> ACL next_module <--------
//===========> MODULE_CLOSE
//----------> ACL invoke_close_box <--------
//rdpproxy: [RDP Session] session_id="SESSIONID-0000" client_ip="10.10.43.12" target_ip="10.10.44.101" user="u" device="win2008rzh" service="rdp" account="Christophe" type="SESSION_DISCONNECTION" duration="0:00:09"
//rdpproxy: [rdpproxy] psid="156751302831637" user="u" type="TARGET_DISCONNECTION" reason="End of connection"
//RDP Target (-1): total_received=38346, total_sent=3943
//----------> ACL new_mod <--------
//target_module=MODULE_INTERNAL_CLOSE(5)
//ModuleManager::Creation of new mod 'INTERNAL::Close'
//Failure in SSL library, error=1, Success [0]
//partial_send_tls error:00000001:lib(0):func(0):reason(1)
//partial_send_tls error:1409E10F:SSL routines:ssl3_write_bytes:bad length
//rdpproxy: WARNING (31637/31637) -- SocketTransport::Send failed on RDP Client (5) errno=0 [Success]
//rdpproxy: ERR (31637/31637) -- Session::Session exception (2) = Exception ERR_TRANSPORT_WRITE_FAILED no: 1502
//----------> ACL invoke_close_box <--------
//----------> ACL new_mod <--------
//target_module=MODULE_INTERNAL_CLOSE(5)
//ModuleManager::Creation of new mod 'INTERNAL::Close'
//Failure in SSL library, error=1, Success [0]
//partial_send_tls error:00000001:lib(0):func(0):reason(1)
//partial_send_tls error:1409E10F:SSL routines:ssl3_write_bytes:bad length
//rdpproxy: WARNING (31637/31637) -- SocketTransport::Send failed on RDP Client (5) errno=0 [Success]
//Session::Session Init exception = Exception ERR_TRANSPORT_WRITE_FAILED no: 1502!
//Session::Client Session Disconnected
//rdpproxy: [rdpproxy] psid="156751302831637" type="DISCONNECT" reason="Exception ERR_TRANSPORT_WRITE_FAILED no: 1502"


