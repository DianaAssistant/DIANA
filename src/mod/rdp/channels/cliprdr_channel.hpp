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
    Copyright (C) Wallix 2015
    Author(s): Christophe Grosjean, Raphael Zhou, Clément Moroldo
*/


#pragma once

#include "core/error.hpp"
#include "core/front_api.hpp"
#include "core/session_reactor.hpp"
#include "mod/icap_files_service.hpp"
#include "mod/rdp/channels/base_channel.hpp"
#include "mod/rdp/channels/clipboard_virtual_channels_params.hpp"
#include "mod/rdp/channels/cliprdr_channel_send_and_receive.hpp"
#include "mod/rdp/channels/sespro_launcher.hpp"
#include "utils/arcsight.hpp"
#include "utils/difftimeval.hpp"
#include "utils/key_qvalue_pairs.hpp"
#include "utils/log.hpp"
#include "utils/stream.hpp"
#include "utils/sugar/algostring.hpp"
#include "utils/sugar/cast.hpp"


#include <vector>
#include <optional>
#include <algorithm>
#include <string_view>


class ClipboardVirtualChannel final : public BaseVirtualChannel
{
    struct ICapValidator
    {
        enum class Direction : bool
        {
            FileFromServer,
            FileFromClient,
        };

        struct ICapFileItem
        {
            std::string filename;
            uint64_t filesize;
            ICAPFileId id;
            Direction direction;
        };

        struct FileInfo
        {
            ICapFileItem file;
            std::string_view result_content;
            LocalICAPProtocol::ValidationType validation_type;
        };

        ICapValidator(ICAPService * icap_service) noexcept
        : icap_service(icap_service)
        {}

        explicit operator bool () const noexcept
        {
            return this->icap_service;
        }

        void new_file(std::string filename, uint64_t filesize, const Direction direction, std::string_view target_name)
        {
            assert(this->icap_service);

            if (this->icap_file_id != invalid_icap_file_id) {
                this->icap_service->send_eof(this->icap_file_id);
            }

            this->icap_file_id = this->icap_service->open_file(filename, target_name);
            this->icap_files.items.push_back({
                std::move(filename), filesize, this->icap_file_id, direction});
        }

        void set_data(const_bytes_view data)
        {
            assert(this->icap_service);
            this->icap_service->send_data(this->icap_file_id, data);
        }

        void set_end_of_file()
        {
            assert(this->icap_service);
            this->icap_service->send_eof(this->icap_file_id);
            this->icap_file_id = invalid_icap_file_id;
        }

        std::optional<FileInfo> receive_response()
        {
            assert(this->icap_service);

            for (;;){
                switch (this->icap_service->receive_response()) {
                    case ICAPService::ResponseType::WaitingData:
                        return {};
                    case ICAPService::ResponseType::HasContent:
                        this->icap_file_id = invalid_icap_file_id;
                        return {FileInfo{
                            this->icap_files.pop_id(this->icap_service->last_file_id()),
                            this->icap_service->get_content(),
                            this->icap_service->last_result_flag()
                        }};
                    case ICAPService::ResponseType::HasPacket:
                    case ICAPService::ResponseType::Error:
                        ;
                }
            }
        }

        bool has_valid_file_id() const noexcept
        {
            return this->icap_file_id != invalid_icap_file_id;
        }

    private:
        struct ICapFiles
        {
            unsigned quick_pos = 0;
            std::vector<ICapFileItem> items;

            ICapFileItem pop_id(ICAPFileId id)
            {
                if (this->items.size() <= this->quick_pos
                    || this->items[this->quick_pos].id != id
                ) {
                    auto it = std::find_if(this->items.begin(), this->items.end(), [id](auto& f){
                        return f.id == id;
                    });

                    if (it == this->items.end()) {
                        LOG(LOG_ERR, "ICapValidator::receive_response: invalid id %u",
                            underlying_cast(id));
                        throw Error(ERR_ICAP_LOCAl_PROTOCOL);
                    }

                    this->quick_pos = std::distance(this->items.begin(), it);
                }

                auto r = std::move(this->items[this->quick_pos]);
                if (this->quick_pos + 1u < this->items.size()) {
                    this->items[this->quick_pos] = std::move(this->items.back());
                    ++this->quick_pos;
                }
                this->items.pop_back();
                return r;
            }
        };

        ICAPService * icap_service;
        const std::string up_target_name;
        const std::string down_target_name;
        ICapFiles icap_files;
        ICAPFileId icap_file_id = invalid_icap_file_id;
    };


    ClipboardData clip_data;

    std::vector<RDPECLIP::FileDescriptor> file_descr_list;

    Cliprdr::FormatNameInventory format_name_inventory;


    const ClipboardVirtualChannelParams params;

    FrontAPI& front;

    SessionProbeLauncher* clipboard_monitor_ready_notifier = nullptr;
    SessionProbeLauncher* clipboard_initialize_notifier    = nullptr;
    SessionProbeLauncher* format_list_notifier             = nullptr;
    SessionProbeLauncher* format_list_response_notifier    = nullptr;
    SessionProbeLauncher* format_data_request_notifier     = nullptr;

    const bool proxy_managed;   // Has not client.

    SessionReactor& session_reactor;

    ICapValidator icap;

    static constexpr uint32_t last_lindex_unknown = ~uint32_t{};
    uint32_t last_lindex = last_lindex_unknown;
    uint64_t last_lindex_total_send = 0;
    uint64_t last_lindex_packet_remaining = 0;

    using Direction = ICapValidator::Direction;

public:
    ClipboardVirtualChannel(
        VirtualChannelDataSender* to_client_sender_,
        VirtualChannelDataSender* to_server_sender_,
        FrontAPI& front,
        SessionReactor& session_reactor,
        const BaseVirtualChannel::Params & base_params,
        const ClipboardVirtualChannelParams & params,
        ICAPService * icap_service)
    : BaseVirtualChannel(to_client_sender_,
                         to_server_sender_,
                         base_params)
    , params([&]{
        auto p = params;
        if (!icap_service) {
            p.validator_params.up_target_name.clear();
            p.validator_params.down_target_name.clear();
        }
        return p;
    }())
    , front(front)
    , proxy_managed(to_client_sender_ == nullptr)
    , session_reactor(session_reactor)
    , icap(icap_service)
    {}

    void empty_client_clipboard() {
        LOG_IF(bool(this->verbose & RDPVerbose::cliprdr), LOG_INFO,
            "ClipboardVirtualChannel::empty_client_clipboard");

        RDPECLIP::CliprdrHeader clipboard_header(RDPECLIP::CB_FORMAT_LIST,
            RDPECLIP::CB_RESPONSE__NONE_, 0);

        StaticOutStream<256> out_s;

        clipboard_header.emit(out_s);

        const size_t totalLength = out_s.get_offset();

        this->send_message_to_server(
            totalLength,
            CHANNELS::CHANNEL_FLAG_FIRST | CHANNELS::CHANNEL_FLAG_LAST,
            out_s.get_bytes());
    }

    bool use_long_format_names() const {
        return (this->clip_data.client_data.use_long_format_names &&
            this->clip_data.server_data.use_long_format_names);
    }

public:
    void process_client_message(uint32_t total_length,
        uint32_t flags, const_bytes_view chunk_data) override
    {
        LOG_IF(bool(this->verbose & RDPVerbose::cliprdr), LOG_INFO,
            "ClipboardVirtualChannel::process_client_message: "
                "total_length=%u flags=0x%08X chunk_data_length=%zu",
            total_length, flags, chunk_data.size());

        if (bool(this->verbose & RDPVerbose::cliprdr_dump)) {
            const bool send              = false;
            const bool from_or_to_client = true;
            ::msgdump_c(send, from_or_to_client, total_length, flags, chunk_data);
        }

        InStream chunk(chunk_data);
        RDPECLIP::CliprdrHeader header;

        if (flags & CHANNELS::CHANNEL_FLAG_FIRST) {
            /* msgType(2) + msgFlags(2) + dataLen(4) */
            ::check_throw(chunk, 8, "ClipboardVirtualChannel::process_client_message", ERR_RDP_DATA_TRUNCATED);
            header.recv(chunk);
            this->clip_data.client_data.message_type = header.msgType();
        }

        if (bool(this->verbose & RDPVerbose::cliprdr)) {
            log_client_message_type(
                "process_client_message",
                this->clip_data.client_data.message_type, flags);
        }

        bool send_message_to_server = true;

        switch (this->clip_data.client_data.message_type)
        {
            case RDPECLIP::CB_FORMAT_LIST:
                send_message_to_server = this->process_format_list_pdu(
                    flags, chunk, header,
                    this->to_client_sender_ptr(),
                    this->clip_data.client_data,
                    this->params.clipboard_down_authorized || this->params.clipboard_up_authorized || this->format_list_response_notifier);
            break;

            case RDPECLIP::CB_FORMAT_DATA_REQUEST: {
                FormatDataRequestReceive receiver(this->clip_data, this->verbose, chunk);
                if (!this->params.clipboard_down_authorized) {
                    LOG_IF(bool(this->verbose & RDPVerbose::cliprdr), LOG_INFO,
                        "ClipboardVirtualChannel::process_client_format_data_request_pdu: "
                            "Server to client Clipboard operation is not allowed.");

                    FormatDataRequestSendBack sender(this->to_client_sender_ptr());
                }
                send_message_to_server = this->params.clipboard_down_authorized;
            }
            break;

            case RDPECLIP::CB_FORMAT_DATA_RESPONSE: {
                const bool is_from_remote_session = false;
                send_message_to_server = this->process_format_data_response_pdu(
                    flags, chunk, header, is_from_remote_session);
            }
            break;

            case RDPECLIP::CB_CLIP_CAPS:
            {
                ClipboardCapabilitiesReceive receiver(this->clip_data.client_data, chunk, this->verbose);
                send_message_to_server = true;
            }
            break;

            case RDPECLIP::CB_FILECONTENTS_REQUEST:
                send_message_to_server = this->process_filecontents_request_pdu(
                    chunk, header, this->to_client_sender_ptr(),
                    this->clip_data.client_data, Direction::FileFromServer);
            break;

            case RDPECLIP::CB_FILECONTENTS_RESPONSE: {
                const bool from_remote_session = false;
                send_message_to_server = this->process_filecontents_response_pdu(
                    flags, chunk, header, from_remote_session);
            }
            break;

            case RDPECLIP::CB_LOCK_CLIPDATA: {
                LockClipDataReceive receiver(this->clip_data.server_data, this->clip_data.client_data, chunk, this->verbose, header);
            }
            break;

            case RDPECLIP::CB_UNLOCK_CLIPDATA: {
                UnlockClipDataReceive receive(this->clip_data.server_data, this->clip_data.client_data, chunk, this->verbose, header);
            }
            break;
        }   // switch (this->client_message_type)

        if (send_message_to_server) {
            this->send_message_to_server(total_length, flags, chunk_data);
        }
    }   // process_client_message

    bool process_server_format_data_request_pdu(uint32_t total_length,
        uint32_t flags, InStream& chunk, const RDPECLIP::CliprdrHeader & /*in_header*/)
    {
        (void)total_length;
        (void)flags;
        FormatDataRequestReceive receiver(this->clip_data, this->verbose, chunk);

        if (this->format_data_request_notifier &&
            (this->clip_data.requestedFormatId  == RDPECLIP::CF_TEXT)) {
            if (!this->format_data_request_notifier->on_server_format_data_request()) {
                this->format_data_request_notifier = nullptr;
            }

            return false;
        }

        if (!this->params.clipboard_up_authorized) {
            LOG_IF(bool(this->verbose & RDPVerbose::cliprdr), LOG_INFO,
                "ClipboardVirtualChannel::process_server_format_data_request_pdu: "
                    "Client to server Clipboard operation is not allowed.");

            FormatDataRequestSendBack sender(this->to_server_sender_ptr());

            return false;
        }

        return true;
    }   // process_server_format_data_request_pdu

    void process_server_message(uint32_t total_length,
        uint32_t flags, const_bytes_view chunk_data,
        std::unique_ptr<AsynchronousTask> & out_asynchronous_task) override
    {
        (void)out_asynchronous_task;

        LOG_IF(bool(this->verbose & RDPVerbose::cliprdr), LOG_INFO,
            "ClipboardVirtualChannel::process_server_message: "
                "total_length=%u flags=0x%08X chunk_data_length=%zu",
            total_length, flags, chunk_data.size());

        if (bool(this->verbose & RDPVerbose::cliprdr_dump)) {
            const bool send              = false;
            const bool from_or_to_client = false;
            ::msgdump_c(send, from_or_to_client, total_length, flags, chunk_data);
        }

        InStream chunk(chunk_data);
        RDPECLIP::CliprdrHeader header;

        if (flags & CHANNELS::CHANNEL_FLAG_FIRST) {
            /* msgType(2) + msgFlags(2) + dataLen(4) */
            ::check_throw(chunk, 8, "ClipboardVirtualChannel::process_server_message", ERR_RDP_DATA_TRUNCATED);
            header.recv(chunk);
            this->clip_data.server_data.message_type = header.msgType();
        }

        if (bool(this->verbose & RDPVerbose::cliprdr)) {
            log_client_message_type(
                "process_server_message",
                this->clip_data.server_data.message_type, flags);
        }

        bool send_message_to_client = true;

        switch (this->clip_data.server_data.message_type)
        {
            case RDPECLIP::CB_MONITOR_READY: {
                if (this->proxy_managed) {
                    this->clip_data.client_data.use_long_format_names = true;
                    ServerMonitorReadySendBack sender(this->verbose, this->use_long_format_names(), this->to_server_sender_ptr());
                }

                if (this->clipboard_monitor_ready_notifier) {
                    if (!this->clipboard_monitor_ready_notifier->on_clipboard_monitor_ready()) {
                        this->clipboard_monitor_ready_notifier = nullptr;
                    }
                }

                send_message_to_client = !this->proxy_managed;
            }
            break;

            case RDPECLIP::CB_FORMAT_LIST:
                send_message_to_client = this->process_format_list_pdu(
                    flags, chunk, header,
                    this->to_server_sender_ptr(),
                    this->clip_data.server_data,
                    this->params.clipboard_down_authorized || this->params.clipboard_up_authorized);

                if (this->format_list_notifier) {
                    if (!this->format_list_notifier->on_server_format_list()) {
                        this->format_list_notifier = nullptr;
                    }
                }
            break;

            case RDPECLIP::CB_FORMAT_LIST_RESPONSE:
                if (this->clipboard_initialize_notifier) {
                    if (!this->clipboard_initialize_notifier->on_clipboard_initialize()) {
                        this->clipboard_initialize_notifier = nullptr;
                    }
                }
                else if (this->format_list_response_notifier) {
                    if (!this->format_list_response_notifier->on_server_format_list_response()) {
                        this->format_list_response_notifier = nullptr;
                    }
                }
            break;

            case RDPECLIP::CB_FORMAT_DATA_REQUEST:
                send_message_to_client =
                    this->process_server_format_data_request_pdu(
                        total_length, flags, chunk, header);
            break;

            case RDPECLIP::CB_FORMAT_DATA_RESPONSE: {
                const bool is_from_remote_session = true;
                send_message_to_client = this->process_format_data_response_pdu(
                    flags, chunk, header, is_from_remote_session);
            }
            break;

            case RDPECLIP::CB_CLIP_CAPS:
            {
                ClipboardCapabilitiesReceive receiver(this->clip_data.server_data, chunk, this->verbose);
                send_message_to_client = !this->proxy_managed;
            }
            break;

            case RDPECLIP::CB_FILECONTENTS_REQUEST:
                send_message_to_client = this->process_filecontents_request_pdu(
                    chunk, header, this->to_server_sender_ptr(),
                    this->clip_data.server_data, Direction::FileFromClient);
            break;

            case RDPECLIP::CB_FILECONTENTS_RESPONSE: {
                const bool from_remote_session = true;
                send_message_to_client = this->process_filecontents_response_pdu(
                    flags, chunk, header, from_remote_session);
            }
            break;

            case RDPECLIP::CB_LOCK_CLIPDATA: {
                LockClipDataReceive receiver(this->clip_data.client_data, this->clip_data.server_data, chunk, this->verbose, header);
            }
            break;

            case RDPECLIP::CB_UNLOCK_CLIPDATA: {
                UnlockClipDataReceive receive(this->clip_data.client_data, this->clip_data.server_data, chunk, this->verbose, header);
            }
            break;
        }   // switch (this->server_message_type)

        if (send_message_to_client) {
            this->send_message_to_client(total_length, flags, chunk_data);
        }   // switch (this->server_message_type)
    }   // process_server_message

    void set_session_probe_launcher(SessionProbeLauncher* launcher) {
        this->clipboard_monitor_ready_notifier = launcher;
        this->clipboard_initialize_notifier    = launcher;
        this->format_list_notifier             = launcher;
        this->format_list_response_notifier    = launcher;
        this->format_data_request_notifier     = launcher;
    }

    void log_client_message_type(char const* funcname, uint16_t message_type, uint32_t flags)
    {
        const auto first_last = CHANNELS::CHANNEL_FLAG_FIRST | CHANNELS::CHANNEL_FLAG_LAST;
        LOG(LOG_INFO, "ClipboardVirtualChannel::%s: %s (%u)%s)",
            funcname, RDPECLIP::get_msgType_name(message_type), message_type,
            ((flags & first_last) == first_last) ? " FIRST|LAST"
            : (flags & CHANNELS::CHANNEL_FLAG_FIRST) ? "FIRST"
            : (flags & CHANNELS::CHANNEL_FLAG_LAST) ? "LAST"
            : "");
    }

    void DLP_antivirus_check_channels_files()
    {
        if (!this->icap) {
            return ;
        }

        while (auto response = this->icap.receive_response()) {
            switch (response->validation_type) {
                case LocalICAPProtocol::ValidationType::Wait:
                    return;
                case LocalICAPProtocol::ValidationType::IsAccepted:
                    if (!this->params.validator_params.log_if_accepted) {
                        continue;
                    }
                    [[fallthrough]];
                case LocalICAPProtocol::ValidationType::IsRejected:
                case LocalICAPProtocol::ValidationType::Error:
                    ;
            }

            auto direction = (response->file.direction == Direction::FileFromClient)
                ? "UP"_av : "DOWN"_av;

            auto const info = key_qvalue_pairs({
                {"type", "FILE_VERIFICATION" },
                {"direction", direction},
                {"filename", response->file.filename},
                {"status", response->result_content}
            });

            ArcsightLogInfo arc_info;
            arc_info.name = "FILE_SCAN_RESULT";
            arc_info.signatureID = ArcsightLogInfo::ID::FILE_SCAN_RESULT;
            arc_info.ApplicationProtocol = "rdp";
            arc_info.fileName = response->file.filename;
            arc_info.fileSize = response->file.filesize;
            arc_info.direction_flag = (response->file.direction == Direction::FileFromServer)
                ? ArcsightLogInfo::Direction::SERVER_SRC
                : ArcsightLogInfo::Direction::SERVER_DST;
            arc_info.message = response->result_content;

            this->report_message.log6(info, arc_info, session_reactor.get_current_time());
            std::string message = str_concat("FILE_VERIFICATION=",
                response->file.filename, '\x01', direction, '\x01', response->result_content);
            this->front.session_update(message);
        }
    }

private:
    bool process_filecontents_response_pdu(
        uint32_t flags, InStream& chunk, const RDPECLIP::CliprdrHeader & header,
        const bool from_remote_session)
    {
        auto& from_server = from_remote_session
            ? this->clip_data.server_data : this->clip_data.client_data;
        auto& from_client = from_remote_session
            ? this->clip_data.client_data : this->clip_data.server_data;
        bool enable_icap = from_remote_session
            ? !this->params.validator_params.down_target_name.empty()
            : !this->params.validator_params.up_target_name.empty();

        FileContentsResponseReceive receive(from_server, header, flags, chunk);

        if (receive.must_log_file_info_type) {
            this->log_file_info(receive.file_info, from_remote_session);
        }

        if (icap.has_valid_file_id()
         && enable_icap
         && from_client.last_dwFlags == RDPECLIP::FILECONTENTS_RANGE
        ) {
            auto data = chunk.remaining_bytes();
            auto data_len = std::min<size_t>(data.size(), this->last_lindex_packet_remaining);
            if (flags & CHANNELS::CHANNEL_FLAG_LAST) {
                this->check_descr_index(this->last_lindex);
                auto file_size = this->file_descr_list[this->last_lindex].file_size();
                data_len = std::min<size_t>(data_len, file_size - this->last_lindex_total_send);
            }
            data = data.first(data_len);
            this->icap.set_data(data);
            this->last_lindex_packet_remaining -= data_len;
            this->last_lindex_total_send += data_len;

            if (flags & CHANNELS::CHANNEL_FLAG_LAST) {
                auto file_size = this->file_descr_list[this->last_lindex].file_size();
                if (this->last_lindex_total_send == file_size) {
                    this->icap.set_end_of_file();
                }
            }
            // return !this->enable_interrupting;
        }

        return true;
    }

    bool process_filecontents_request_pdu(
        InStream& chunk, const RDPECLIP::CliprdrHeader & header,
        VirtualChannelDataSender* sender, ClipboardSideData& side_data,
        Direction direction)
    {
        FilecontentsRequestReceive receiver(side_data, chunk, this->verbose, header.dataLen());

        if (!this->params.clipboard_file_authorized) {
            FilecontentsRequestSendBack pdu(receiver.dwFlags, receiver.streamID, sender);
            return false;
        }

        std::string const& target_name = (direction == Direction::FileFromServer)
            ? this->params.validator_params.down_target_name
            : this->params.validator_params.up_target_name;

        if (!target_name.empty() && receiver.dwFlags == RDPECLIP::FILECONTENTS_RANGE) {
            this->check_descr_index(receiver.lindex);
            const RDPECLIP::FileDescriptor & desc = this->file_descr_list[receiver.lindex];

            this->last_lindex_packet_remaining = receiver.requested;

            if (this->last_lindex != receiver.lindex) {
                this->last_lindex = receiver.lindex;
                this->icap.new_file(desc.file_name, desc.file_size(), direction, target_name);
            }
        }

        return true;
    }

    void check_descr_index(std::size_t i)
    {
        if (i >= this->file_descr_list.size()) {
            LOG(LOG_ERR, "ClipboardVirtualChannel::check_descr_index(%zu): out of bounds index", i);
            throw Error(ERR_RDP_PROTOCOL);
        }
    }

    bool process_format_data_response_pdu(uint32_t flags, InStream& chunk, const RDPECLIP::CliprdrHeader & in_header, bool is_from_remote_session)
    {
        auto& side_data = is_from_remote_session
            ? this->clip_data.server_data
            : this->clip_data.client_data;
        auto requested_format_id = this->clip_data.requestedFormatId;
        auto file_list_format_id = side_data.file_list_format_id;

        if (file_list_format_id && requested_format_id == file_list_format_id) {
            FormatDataResponseReceiveFileList receiver(
                chunk,
                in_header,
                this->params.dont_log_data_into_syslog,
                side_data.file_list_format_id,
                flags,
                side_data.file_descriptor_stream,
                this->verbose,
                is_from_remote_session ? "client" : "server"
            );

            this->file_descr_list.insert(
                this->file_descr_list.end(),
                receiver.files_descriptors.begin(),
                receiver.files_descriptors.end());

            if (flags & CHANNELS::CHANNEL_FLAG_LAST) {
                this->clip_data.requestedFormatId = 0;
            }

            for (RDPECLIP::FileDescriptor const& fd : receiver.files_descriptors) {
                side_data.update_file_contents_request_inventory(fd);
            }

            this->log_siem_info(flags, in_header, this->clip_data.requestedFormatId, std::string{}, is_from_remote_session);
        }
        else {
            FormatDataResponseReceive receiver(requested_format_id, chunk, flags);

            this->log_siem_info(flags, in_header, this->clip_data.requestedFormatId, receiver.data_to_dump, is_from_remote_session);
        }

        return true;
    }

    bool process_format_list_pdu(
        uint32_t flags, InStream& chunk, const RDPECLIP::CliprdrHeader & in_header,
        VirtualChannelDataSender* sender, ClipboardSideData& side_data, bool clip_enabled)
    {
        if (!(flags & CHANNELS::CHANNEL_FLAG_LAST)) {
            LOG(LOG_ERR, "Format List PDU is not yet supported!");
            FormatListSendBack pdu(sender);
            return false;
        }

        this->file_descr_list.clear();
        this->last_lindex = last_lindex_unknown;
        this->last_lindex_total_send = 0;
        this->last_lindex_packet_remaining = 0;

        if (!clip_enabled) {
            LOG(LOG_WARNING, "Clipboard is fully disabled.");
            FormatListSendBack pdu(sender);
            return false;
        }

        this->format_name_inventory.clear();

        FormatListReceive receiver(
            this->use_long_format_names(),
            in_header,
            chunk,
            this->format_name_inventory,
            this->verbose);

        if (receiver.file_list_format_id) {
            side_data.file_list_format_id = receiver.file_list_format_id;
        }
        return true;
    }

    void log_file_info(ClipboardSideData::file_info_type & file_info, bool from_remote_session)
    {
        const char* type = (
                  from_remote_session
                ? "CB_COPYING_PASTING_FILE_FROM_REMOTE_SESSION"
                : "CB_COPYING_PASTING_FILE_TO_REMOTE_SESSION"
            );

        uint8_t digest[SslSha256::DIGEST_LENGTH] = { 0 };

        file_info.sha256.final(digest);

        char digest_s[128];
        snprintf(digest_s, sizeof(digest_s),
            "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
            "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            digest[ 0], digest[ 1], digest[ 2], digest[ 3], digest[ 4], digest[ 5], digest[ 6], digest[ 7],
            digest[ 8], digest[ 9], digest[10], digest[11], digest[12], digest[13], digest[14], digest[15],
            digest[16], digest[17], digest[18], digest[19], digest[20], digest[21], digest[22], digest[23],
            digest[24], digest[25], digest[26], digest[27], digest[28], digest[29], digest[30], digest[31]);

        auto const file_size_str = std::to_string(file_info.size);

        auto const info = key_qvalue_pairs({
                { "type", type },
                { "file_name", file_info.file_name },
                { "size", file_size_str },
                { "sha256", digest_s }
            });

        ArcsightLogInfo arc_info;
        arc_info.name = type;
        arc_info.ApplicationProtocol = "rdp";
        arc_info.fileName = file_info.file_name;
        arc_info.fileSize = file_info.size;
        arc_info.direction_flag = from_remote_session ? ArcsightLogInfo::Direction::SERVER_SRC : ArcsightLogInfo::Direction::SERVER_DST;

        this->report_message.log6(info, arc_info, this->session_reactor.get_current_time());

        if (!this->params.dont_log_data_into_syslog) {
            LOG(LOG_INFO, "%s", info);
        }

        if (!this->params.dont_log_data_into_wrm) {
            std::string message = str_concat(
                type, '=', file_info.file_name, '\x01', file_size_str, '\x01', digest_s);
            this->front.session_update(message);
        }
    }

    void log_siem_info(uint32_t flags, const RDPECLIP::CliprdrHeader & in_header, const uint32_t requestedFormatId, const std::string & data_to_dump, const bool is_from_remote_session) {
        if (flags & CHANNELS::CHANNEL_FLAG_FIRST) {

            if (in_header.msgFlags() & RDPECLIP::CB_RESPONSE_OK) {

                const auto type = (is_from_remote_session)
                    ? (data_to_dump.empty()
                        ? "CB_COPYING_PASTING_DATA_FROM_REMOTE_SESSION"_av
                        : "CB_COPYING_PASTING_DATA_FROM_REMOTE_SESSION_EX"_av)
                    : (data_to_dump.empty()
                        ? "CB_COPYING_PASTING_DATA_TO_REMOTE_SESSION"_av
                        : "CB_COPYING_PASTING_DATA_TO_REMOTE_SESSION_EX"_av);

                auto* format_name = this->format_name_inventory.find(requestedFormatId);
                auto utf8_format = (format_name && !format_name->utf8_name().empty())
                    ? format_name->utf8_name()
                    : RDPECLIP::get_FormatId_name_av(requestedFormatId);

                bool const log_current_activity = (
                    !this->params.log_only_relevant_clipboard_activities
                 || !format_name
                 || (
                     !format_name->utf8_name_equal(Cliprdr::preferred_drop_effect_utf8)
                  && !format_name->utf8_name_equal(Cliprdr::file_group_descriptor_w_utf8)
                ));

                auto format = str_concat(utf8_format.as_chars(),
                    '(', std::to_string(requestedFormatId), ')');

                auto const size_str = std::to_string(in_header.dataLen());

                std::string info;
                ::key_qvalue_pairs(
                        info,
                        {
                            { "type", type },
                            { "format", format },
                            { "size", size_str }
                        }
                    );
                if (!data_to_dump.empty()) {
                    ::key_qvalue_pairs(
                        info,
                        {
                            { "partial_data", data_to_dump }
                        }
                    );
                }

                ArcsightLogInfo arc_info;
                arc_info.name = data_to_dump.empty() ? "CB_COPYING_PASTING_DATA" : "CB_COPYING_PASTING_DATA_EX";
                arc_info.signatureID = data_to_dump.empty() ? ArcsightLogInfo::ID::CB_COPYING_PASTING_DATA : ArcsightLogInfo::ID::CB_COPYING_PASTING_DATA_EX;
                arc_info.ApplicationProtocol = "rdp";
                arc_info.message = info;
                arc_info.direction_flag = is_from_remote_session ? ArcsightLogInfo::Direction::SERVER_SRC : ArcsightLogInfo::Direction::SERVER_DST;

                if (log_current_activity) {
                    this->report_message.log6(info, arc_info, this->session_reactor.get_current_time());
                }

                if (!this->params.dont_log_data_into_syslog) {
                    LOG(LOG_INFO, "%s", info);
                }

                if (!this->params.dont_log_data_into_wrm) {
                    str_assign(info, type, '=', format, '\x01', size_str);
                    if (!data_to_dump.empty()) {
                        str_append(info, '\x01', data_to_dump);
                    }

                    this->front.session_update(info);
                }
            }
        }
    }
};  // class ClipboardVirtualChannel
