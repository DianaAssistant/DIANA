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
*1324
*   Product name: redemption, a FLOSS RDP proxy
*   Copyright (C) Wallix 2018
*   Author(s): Clément Moroldo
*/

#include "test_only/test_framework/redemption_unit_tests.hpp"
#include "test_only/test_framework/working_directory.hpp"
#include "test_only/test_framework/file.hpp"

#include "core/RDP/clipboard/format_list_serialize.hpp"
#include "mod/rdp/rdp_metrics.hpp"
#include "mod/metrics_hmac.hpp"


using namespace std::literals::chrono_literals;

RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCycle1, wd)
{
    MonotonicTimePoint epoch(1533211671s);
    RealTimePoint real_time{epoch.time_since_epoch() + 10s};

    Metrics m(wd.dirname()
          , "164d89c1a56957b752540093e178"
          , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
          , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
          , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
          , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
          , epoch
          , real_time
          , 24h
          , 5s
          );
    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);
    RED_CHECK_FILE_CONTENTS(logindex1, "2018-08-02 12:08:01 connection 164d89c1a56957b752540093e178 user=51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58 account=1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31 target_service_device=EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48 client_info=B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7\n"_av);

    metrics.right_click_pressed();
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

    m.log(epoch+1s, real_time+1s);
    metrics.right_click_pressed();
    RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

    m.log(epoch+3s, real_time+3s);
    RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

    m.log(epoch+5s, real_time+5s);
    RED_CHECK_FILE_CONTENTS(logmetrics1, "2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"_av);

    m.log(epoch+7s, real_time+7s);
    RED_CHECK_FILE_CONTENTS(logmetrics1, "2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"_av);

    m.log(epoch+10s, real_time+10s);
    RED_CHECK_FILE_CONTENTS(logmetrics1, "2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"_av);
}

RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCycle2, wd)
{
    {
        const MonotonicTimePoint epoch(1533211681s);
        RealTimePoint real_time{epoch.time_since_epoch()};

        Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 3s
                );
        RDPMetrics metrics(m);

        auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
        auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
        RED_CHECK_WORKSPACE(wd);

        RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

        metrics.right_click_pressed();
        m.log(epoch+0s, real_time+0s);
        RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

        m.log(epoch+1s, real_time+1s);
        RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

        m.log(epoch+2s, real_time+2s);
        RED_CHECK_FILE_CONTENTS(logmetrics1, ""_av);

        m.log(epoch+3s, real_time+3s);
        RED_CHECK_FILE_CONTENTS(logmetrics1, "2018-08-02 12:08:04 164d89c1a56957b752540093e178 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"_av);
    }
}

RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogBasicIncrement, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
             , "164d89c1a56957b752540093e178"
             , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
             , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
             , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
             , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
             , epoch
             , real_time
             , 24h
             , 5s
             );
    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics("2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");

    epoch += 5s;
    real_time += 5s;
    metrics.right_click_pressed();
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    metrics.right_click_pressed();
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 3 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    metrics.client_main_channel_data(3);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:21 164d89c1a56957b752540093e178 3 2 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    metrics.left_click_pressed();
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:26 164d89c1a56957b752540093e178 3 2 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    metrics.key_pressed();
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:31 164d89c1a56957b752540093e178 3 2 1 1 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    metrics.mouse_move(0, 0);
    metrics.mouse_move(2, 2);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:36 164d89c1a56957b752540093e178 3 2 1 1 4 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    metrics.server_main_channel_data(3);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:41 164d89c1a56957b752540093e178 3 2 1 1 4 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 0 0 0\n";
    metrics.client_rail_channel_data(3);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:46 164d89c1a56957b752540093e178 3 2 1 1 4 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 0 0\n";
    metrics.server_rail_channel_data(3);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:51 164d89c1a56957b752540093e178 3 2 1 1 4 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 0\n";
    metrics.client_other_channel_data(3);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);

    epoch += 5s;
    real_time += 5s;
    expected_log_metrics += "2018-08-02 12:08:56 164d89c1a56957b752540093e178 3 2 1 1 4 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 3\n";
    metrics.server_other_channel_data(3);
    m.log(epoch, real_time);
    RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
}

RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCLIPRDRIServerImageCopy_PasteOnClient, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 5s
                );
    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics;

    {  // FORMAT LIST TEXT COPY ON SERVER
        expected_log_metrics += "2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 14 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{RDPECLIP::CF_METAFILEPICT, {}}});

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT TEXT DATA REQUEST PASTE ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 14 0 0 0 0 0 1 0 12 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 4);
        RDPECLIP::FormatDataRequestPDU format(RDPECLIP::CF_METAFILEPICT);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT TEXT DATA RESPONSE FROM SERVER
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 64 0 0 0 0 0 1 0 12 0 1 0 42 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        size_t size = 42;
        const uint8_t data[42] = { 0 };
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_RESPONSE, RDPECLIP::CB_RESPONSE_OK, size);
        RDPECLIP::FormatDataResponsePDU format_data_response_pdu;
        header.emit(out_stream);
        format_data_response_pdu.emit(out_stream, data, size);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}

RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCLIPRDRIServerFileCopy_PasteOnClient, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 5s
                );

    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics;

    {  // FORMAT LIST FILE COPY ON SERVER
        expected_log_metrics += "2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 54 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{49562,
                Cliprdr::formats::file_group_descriptor_w.ascii_name}});

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT FILE DATA REQUEST PASTE ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 54 0 0 0 0 0 0 1 12 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 4);
        RDPECLIP::FormatDataRequestPDU format(49562);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FILE CONTENT REQUEST SIZE FROM CLIENT
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 54 0 0 0 0 0 0 1 48 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;

        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FILECONTENTS_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 36);
        RDPECLIP::FileContentsRequestPDU fcrq_size(0, 0, RDPECLIP::FILECONTENTS_SIZE, 0, 42, 0, 42, true);
        header.emit(out_stream);
        fcrq_size.emit(out_stream);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FILE CONTENT RESPONSE SIZE FROM CLIENT
        expected_log_metrics += "2018-08-02 12:08:21 164d89c1a56957b752540093e178 0 0 0 0 0 0 74 0 0 0 0 0 0 1 48 0 0 1 42 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FILECONTENTS_RESPONSE, RDPECLIP::CB_RESPONSE_OK, 16);
        RDPECLIP::FileContentsResponseSize fcrp_size(0, 42);
        header.emit(out_stream);
        fcrp_size.emit(out_stream);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}


RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCLIPRDRIServerTextCopy_PasteOnClient, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 5s
                );

    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics;

    {  // FORMAT LIST TEXT COPY ON SERVER
        expected_log_metrics += "2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 14 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{RDPECLIP::CF_TEXT, {}}});

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT TEXT DATA REQUEST PASTE ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 14 0 0 0 0 1 0 0 12 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 4);
        RDPECLIP::FormatDataRequestPDU format(RDPECLIP::CF_TEXT);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT TEXT DATA RESPONSE FROM SERVER
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 64 0 0 0 0 1 0 0 12 1 0 0 42 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        size_t size = 42;
        const uint8_t data[42] = { 0 };
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_RESPONSE, RDPECLIP::CB_RESPONSE_OK, size);
        RDPECLIP::FormatDataResponsePDU format_data_response_pdu;
        header.emit(out_stream);
        format_data_response_pdu.emit(out_stream, data, size);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}


RED_AUTO_TEST_CASE_WD(TestRDPMetricsRDPDRReadChunk, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};
    Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 5s
                );

    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics("2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 12 0 0 0 0 0 0 0 0 0 0 0\n");

    { // CLIENT PDU
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        rdpdr::SharedHeader sharedHeader( rdpdr::Component::RDPDR_CTYP_CORE
                                        , rdpdr::PacketId::PAKID_CORE_CLIENTID_CONFIRM);
        sharedHeader.emit(out_stream);

        rdpdr::ClientAnnounceReply clientAnnounceReply( 1, 1, 1);
        clientAnnounceReply.emit(out_stream);

        metrics.set_client_rdpdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // READ FILE
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 12 56 1 0 0 0 42 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        rdpdr::SharedHeader header(rdpdr::RDPDR_CTYP_CORE, rdpdr::PAKID_CORE_DEVICE_IOREQUEST);
        header.emit(out_stream);
        rdpdr::DeviceIORequest dior(0, 0, 0, rdpdr::IRP_MJ_READ, 0);
        dior.emit(out_stream);

        rdpdr::DeviceReadRequest drr(42, 0);
        drr.emit(out_stream);

        metrics.set_server_rdpdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // WRITE FILE
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 12 154 1 0 1 0 42 42 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        rdpdr::SharedHeader header(rdpdr::RDPDR_CTYP_CORE, rdpdr::PAKID_CORE_DEVICE_IOREQUEST);
        header.emit(out_stream);
        rdpdr::DeviceIORequest dior(0, 0, 0, rdpdr::IRP_MJ_WRITE, 0);
        dior.emit(out_stream);
        const uint8_t data[42] = { 0 };
        rdpdr::DeviceWriteRequest dwr(42, 0, data);
        dwr.emit(out_stream);

        metrics.set_server_rdpdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // RENAME FILE
        expected_log_metrics += "2018-08-02 12:08:21 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 12 210 1 0 1 1 42 42 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        rdpdr::SharedHeader header(rdpdr::RDPDR_CTYP_CORE, rdpdr::PAKID_CORE_DEVICE_IOREQUEST);
        header.emit(out_stream);
        rdpdr::DeviceIORequest dior(0, 0, 0, rdpdr::IRP_MJ_SET_INFORMATION, 0);
        dior.emit(out_stream);

        rdpdr::ServerDriveSetInformationRequest sdsir(rdpdr::FileRenameInformation, 0);
        sdsir.emit(out_stream);

        metrics.set_server_rdpdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // DELETE FILE
        expected_log_metrics += "2018-08-02 12:08:26 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 12 266 1 1 1 1 42 42 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        rdpdr::SharedHeader header(rdpdr::RDPDR_CTYP_CORE, rdpdr::PAKID_CORE_DEVICE_IOREQUEST);
        header.emit(out_stream);
        rdpdr::DeviceIORequest dior(0, 0, 0, rdpdr::IRP_MJ_SET_INFORMATION, 0);
        dior.emit(out_stream);

        rdpdr::ServerDriveSetInformationRequest sdsir(rdpdr::FileDispositionInformation, 0);
        sdsir.emit(out_stream);

        metrics.set_server_rdpdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}


RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCLIPRDRIClientImageCopy_PasteOnServer, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 5s
                );

    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics("2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 54 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");

    { // FORMAT LIST INITIALISATION
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{49562,
                Cliprdr::formats::file_group_descriptor_w.ascii_name}});

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT LIST IMAGE COPY ON SERVER
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 68 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{RDPECLIP::CF_METAFILEPICT, {}}});

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT IMAGE DATA REQUEST PASTE ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 12 0 1 0 0 0 0 0 68 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 4);
        RDPECLIP::FormatDataRequestPDU format(RDPECLIP::CF_METAFILEPICT);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT IMAGE DATA RESPONSE FROM SERVER
        expected_log_metrics += "2018-08-02 12:08:21 164d89c1a56957b752540093e178 0 0 0 0 0 0 12 0 1 0 42 0 0 0 118 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        size_t size = 42;
        const uint8_t data[42] = { 0 };
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_RESPONSE, RDPECLIP::CB_RESPONSE_OK, size);
        RDPECLIP::FormatDataResponsePDU format_data_response_pdu;
        header.emit(out_stream);
        format_data_response_pdu.emit(out_stream, data, size);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}


RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCLIPRDRIClientFileCopy_PasteOnServer, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
            , "164d89c1a56957b752540093e178"
            , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
            , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
            , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
            , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
            , epoch
            , real_time
            , 24h
            , 5s
            );

    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics("2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 54 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");

    {  // FORMAT LIST INITIALISATION
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{49562,
                Cliprdr::formats::file_group_descriptor_w.ascii_name}});

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT LIST FILE COPY ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 108 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{49562,
                Cliprdr::formats::file_group_descriptor_w.ascii_name}});

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT FILE DATA REQUEST PASTE ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 12 0 0 1 0 0 0 0 108 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 4);
        RDPECLIP::FormatDataRequestPDU format(49562);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FILE CONTENT REQUEST FROM CLIENT
        expected_log_metrics += "2018-08-02 12:08:21 164d89c1a56957b752540093e178 0 0 0 0 0 0 48 0 0 1 0 0 0 0 108 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FILECONTENTS_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 36);
        RDPECLIP::FileContentsRequestPDU format(0, 0, RDPECLIP::FILECONTENTS_SIZE, 0, 42, 0, 42, true);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FILE CONTENT RESPONSE SIZE FROM CLIENT
        expected_log_metrics += "2018-08-02 12:08:26 164d89c1a56957b752540093e178 0 0 0 0 0 0 48 0 0 1 42 0 0 0 128 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FILECONTENTS_RESPONSE, RDPECLIP::CB_RESPONSE_OK, 16);
        RDPECLIP::FileContentsResponseSize fcrp_size(0, 42);
        header.emit(out_stream);
        fcrp_size.emit(out_stream);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}


RED_AUTO_TEST_CASE_WD(TestRDPMetricsLogCLIPRDRIClientTextCopy_PasteOnServer, wd)
{
    MonotonicTimePoint epoch(1533211681s);
    RealTimePoint real_time{epoch.time_since_epoch()};

    Metrics m(wd.dirname()
                , "164d89c1a56957b752540093e178"
                , "51614130003BD5522C94E637866E4D749DDA13706AC2610C6F77BBFE111F3A58"_av
                , "1C57BA616EEDA5C9D8FF2E0202BB087D0B5D865AC830F336CDB9804331095B31"_av
                , "EAF28B142E03FFC03A35676722BB99DBC21908F3CEA96A8DA6E3C2321056AC48"_av
                , "B079C9845904075BAC3DBE0A26CB7364CE0CC0A5F47DC082F44D221EBC6722B7"_av
                , epoch
                , real_time
                , 24h
                , 5s
                );

    RDPMetrics metrics(m);

    auto const logmetrics1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logmetrics");
    auto const logindex1 = wd.add_file("rdp_metrics-v1.0-2018-08-02.logindex");
    RED_CHECK_WORKSPACE(wd);

    std::string expected_log_metrics("2018-08-02 12:08:06 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 54 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");

    {  // FORMAT LIST INITIALISATION
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{49562,
                Cliprdr::formats::file_group_descriptor_w.ascii_name}});

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT LIST TEXT COPY ON SERVER
        expected_log_metrics += "2018-08-02 12:08:11 164d89c1a56957b752540093e178 0 0 0 0 0 0 0 0 0 0 0 0 0 0 68 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;

        StaticOutStream<1600> out_stream;
        Cliprdr::format_list_serialize_with_header(
            out_stream, Cliprdr::IsLongFormat(true),
            std::array{Cliprdr::FormatNameRef{RDPECLIP::CF_TEXT, {}}});

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT TEXT DATA REQUEST PASTE ON CLIENT
        expected_log_metrics += "2018-08-02 12:08:16 164d89c1a56957b752540093e178 0 0 0 0 0 0 12 1 0 0 0 0 0 0 68 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_REQUEST, RDPECLIP::CB_RESPONSE_NONE, 4);
        RDPECLIP::FormatDataRequestPDU format(RDPECLIP::CF_TEXT);
        header.emit(out_stream);
        format.emit(out_stream);

        metrics.set_server_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }

    {  // FORMAT TEXT DATA RESPONSE FROM SERVER
        expected_log_metrics += "2018-08-02 12:08:21 164d89c1a56957b752540093e178 0 0 0 0 0 0 12 1 0 0 42 0 0 0 118 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
        epoch += 5s;
        real_time += 5s;
        StaticOutStream<1600> out_stream;
        size_t size = 42;
        const uint8_t data[42] = { 0 };
        RDPECLIP::CliprdrHeader header(RDPECLIP::CB_FORMAT_DATA_RESPONSE, RDPECLIP::CB_RESPONSE_OK, size);
        RDPECLIP::FormatDataResponsePDU format_data_response_pdu;
        header.emit(out_stream);
        format_data_response_pdu.emit(out_stream, data, size);

        metrics.set_client_cliprdr_metrics(
            InStream(out_stream.get_produced_bytes()),
            out_stream.get_offset(), CHANNELS::CHANNEL_FLAG_FIRST);

        m.log(epoch, real_time);

        RED_CHECK_FILE_CONTENTS(logmetrics1, expected_log_metrics);
    }
}
