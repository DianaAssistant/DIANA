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
    Author(s): Christophe Grosjean, Raphael Zhou
*/

#include "test_only/test_framework/redemption_unit_tests.hpp"


#include "core/RDP/mppc/mppc_50.hpp"

RED_AUTO_TEST_CASE(TestRDP50BlukCompression2)
{
    #include "fixtures/test_mppc_2.hpp"

    rdp_mppc_50_enc mppc_enc;

    static_assert(sizeof(historyBuffer) == RDP_50_HIST_BUF_LEN);
    static_assert(sizeof(outputBufferPlus) == RDP_50_HIST_BUF_LEN + 64);
    static_assert(sizeof(hash_table) == rdp_mppc_50_enc::hash_table_manager::get_table_size());
    static_assert(sizeof(uncompressed_data) == 4037);
    static_assert(sizeof(compressed_data) == 3015);

    memcpy(mppc_enc.historyBuffer,           historyBuffer,    RDP_50_HIST_BUF_LEN);
    memcpy(mppc_enc.outputBufferPlus,        outputBufferPlus, RDP_50_HIST_BUF_LEN + 64);
    mppc_enc.historyOffset = 61499;
    mppc_enc.bytes_in_opb  = 2834;
    mppc_enc.flags         = 33;
    mppc_enc.flagsHold     = 0;
    mppc_enc.first_pkt     = false;
    mppc_enc.hash_tab_mgr.initialize_hash_table(hash_table);

    uint8_t  compressionFlags;
    uint16_t datalen;

    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    RED_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));
    RED_CHECK_EQUAL(3015,  datalen);
    RED_CHECK(make_array_view(compressed_data) ==
        array_view(mppc_enc.outputBuffer, mppc_enc.bytes_in_opb));
}

RED_AUTO_TEST_CASE(TestRDP50BlukCompression3)
{
    #include "fixtures/test_mppc_3.hpp"

    rdp_mppc_50_enc mppc_enc;

    static_assert(sizeof(historyBuffer) == RDP_50_HIST_BUF_LEN);
    static_assert(sizeof(outputBufferPlus) == RDP_50_HIST_BUF_LEN + 64);
    static_assert(sizeof(hash_table) == rdp_mppc_50_enc::hash_table_manager::get_table_size());
    static_assert(sizeof(uncompressed_data) == 12851);
    static_assert(sizeof(compressed_data) == 8893);


    memcpy(mppc_enc.historyBuffer,           historyBuffer,    RDP_50_HIST_BUF_LEN);
    memcpy(mppc_enc.outputBufferPlus,        outputBufferPlus, RDP_50_HIST_BUF_LEN + 64);
    mppc_enc.historyOffset = 0;
    mppc_enc.bytes_in_opb  = 0;
    mppc_enc.flags         = 0;
    mppc_enc.flagsHold     = 0;
    mppc_enc.first_pkt     = true;
    mppc_enc.hash_tab_mgr.initialize_hash_table(hash_table);

    uint8_t  compressionFlags;
    uint16_t datalen;

    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    RED_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));
    RED_CHECK_EQUAL(8893,  datalen);
    RED_CHECK(make_array_view(compressed_data) ==
        array_view(mppc_enc.outputBuffer, mppc_enc.bytes_in_opb));
}

RED_AUTO_TEST_CASE(TestRDP50BlukDecompression5)
{
    #include "fixtures/test_mppc_5.hpp"

    rdp_mppc_50_dec mppc_dec;

    static_assert(sizeof(compressed_data) == 2053);
    static_assert(sizeof(uncompressed_data) == 3790);


    memcpy(mppc_dec.history_buf, historyBuffer, RDP_50_HIST_BUF_LEN);
    mppc_dec.history_buf_end = mppc_dec.history_buf + 65535;
    mppc_dec.history_ptr     = mppc_dec.history_buf + 54626;

    uint8_t  compressionFlags = 0x21;

    RED_CHECK(make_array_view(uncompressed_data) ==
        mppc_dec.decompress(make_array_view(compressed_data), compressionFlags));
}
