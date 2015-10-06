enum authid_t {
    AUTHID_UNKNOWN = 0,
    AUTHID_CAPTURE_CHUNK,
    AUTHID_AUTH_USER,
    AUTHID_HOST,
    AUTHID_TARGET,
    AUTHID_TARGET_DEVICE,
    AUTHID_TARGET_USER,
    AUTHID_TARGET_APPLICATION,
    AUTHID_TARGET_APPLICATION_ACCOUNT,
    AUTHID_TARGET_APPLICATION_PASSWORD,
    AUTHID_OPT_FILE_ENCRYPTION,
    AUTHID_ALTERNATE_SHELL,
    AUTHID_SHELL_WORKING_DIRECTORY,
    AUTHID_OPT_MOVIE,
    AUTHID_OPT_MOVIE_PATH,
    AUTHID_OPT_WABAGENT,
    AUTHID_ENABLE_WAB_AGENT_LOADING_MASK,
    AUTHID_OPT_WABAGENT_LAUNCH_TIMEOUT,
    AUTHID_OPT_WABAGENT_ON_LAUNCH_FAILURE,
    AUTHID_OPT_WABAGENT_KEEPALIVE_TIMEOUT,
    AUTHID_KEYBOARD_LAYOUT,
    AUTHID_DISABLE_TSK_SWITCH_SHORTCUTS,
    AUTHID_RDP_BOGUS_SC_NET_SIZE,
    AUTHID_OPT_CLIENT_DEVICE_ANNOUNCE_TIMEOUT,
    AUTHID_OPT_PROXY_MANAGED_DRIVES,
    AUTHID_VNC_CLIPBOARD_UP,
    AUTHID_VNC_CLIPBOARD_DOWN,
    AUTHID_VNC_SERVER_CLIPBOARD_ENCODING_TYPE,
    AUTHID_VNC_BOGUS_CLIPBOARD_INFINITE_LOOP,
    AUTHID_DISABLE_KEYBOARD_LOG,
    AUTHID_DISABLE_CLIPBOARD_LOG,
    AUTHID_DISABLE_FILE_SYSTEM_LOG,
    AUTHID_RT_DISPLAY,
    AUTHID_LANGUAGE,
    AUTHID_PASSWORD_EN,
    AUTHID_PASSWORD_FR,
    AUTHID_OPT_BITRATE,
    AUTHID_OPT_FRAMERATE,
    AUTHID_OPT_QSCALE,
    AUTHID_OPT_BPP,
    AUTHID_OPT_HEIGHT,
    AUTHID_OPT_WIDTH,
    AUTHID_AUTH_ERROR_MESSAGE,
    AUTHID_SELECTOR,
    AUTHID_SELECTOR_CURRENT_PAGE,
    AUTHID_SELECTOR_DEVICE_FILTER,
    AUTHID_SELECTOR_GROUP_FILTER,
    AUTHID_SELECTOR_PROTO_FILTER,
    AUTHID_SELECTOR_LINES_PER_PAGE,
    AUTHID_SELECTOR_NUMBER_OF_PAGES,
    AUTHID_TARGET_PASSWORD,
    AUTHID_TARGET_HOST,
    AUTHID_TARGET_SERVICE,
    AUTHID_TARGET_PORT,
    AUTHID_TARGET_PROTOCOL,
    AUTHID_PASSWORD,
    AUTHID_REPORTING,
    AUTHID_AUTH_CHANNEL_ANSWER,
    AUTHID_AUTH_CHANNEL_RESULT,
    AUTHID_AUTH_CHANNEL_TARGET,
    AUTHID_MESSAGE,
    AUTHID_ACCEPT_MESSAGE,
    AUTHID_DISPLAY_MESSAGE,
    AUTHID_REJECTED,
    AUTHID_AUTHENTICATED,
    AUTHID_KEEPALIVE,
    AUTHID_SESSION_ID,
    AUTHID_END_DATE_CNX,
    AUTHID_END_TIME,
    AUTHID_MODE_CONSOLE,
    AUTHID_TIMEZONE,
    AUTHID_REAL_TARGET_DEVICE,
    AUTHID_AUTHENTICATION_CHALLENGE,
    AUTHID_TICKET,
    AUTHID_COMMENT,
    AUTHID_DURATION,
    AUTHID_WAITINFORETURN,
    AUTHID_SHOWFORM,
    AUTHID_FORMFLAG,
    AUTHID_MODULE,
    AUTHID_FORCEMODULE,
    AUTHID_PROXY_OPT,
    MAX_AUTHID
};
constexpr char const * const authstr[] = {
    "capture_chunk",
    "login",
    "ip_client",
    "ip_target",
    "target_device",
    "target_login",
    "target_application",
    "target_application_account",
    "target_application_password",
    "file_encryption",
    "alternate_shell",
    "shell_working_directory",
    "is_rec",
    "rec_path",
    "wab_agent",
    "enable_wab_agent_loading_mask",
    "wab_agent_launch_timeout",
    "wab_agent_on_launch_failure",
    "wab_agent_keepalive_timeout",
    "keyboard_layout",
    "disable_tsk_switch_shortcuts",
    "rdp_bogus_sc_net_size",
    "client_device_announce_timeout",
    "proxy_managed_drives",
    "clipboard_up",
    "clipboard_down",
    "vnc_server_clipboard_encoding_type",
    "vnc_bogus_clipboard_infinite_loop",
    "disable_keyboard_log",
    "disable_clipboard_log",
    "disable_file_system_log",
    "rt_display",
    "language",
    "password_en",
    "password_fr",
    "bitrate",
    "framerate",
    "qscale",
    "bpp",
    "height",
    "width",
    "auth_error_message",
    "selector",
    "selector_current_page",
    "selector_device_filter",
    "selector_group_filter",
    "selector_proto_filter",
    "selector_lines_per_page",
    "selector_number_of_pages",
    "target_password",
    "target_host",
    "target_service",
    "target_port",
    "proto_dest",
    "password",
    "reporting",
    "auth_channel_answer",
    "auth_channel_result",
    "auth_channel_target",
    "message",
    "accept_message",
    "display_message",
    "rejected",
    "authenticated",
    "keepalive",
    "session_id",
    "timeclose",
    "end_time",
    "mode_console",
    "timezone",
    "real_target_device",
    "authentication_challenge",
    "ticket",
    "comment",
    "duration",
    "waitinforeturn",
    "showform",
    "formflag",
    "module",
    "forcemodule",
    "proxy_opt",
};

