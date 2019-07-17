//
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN
//

R"xxxyyyzzz(#!/usr/bin/python -O
# -*- coding: utf-8 -*-

# DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

cp_spec = {
  'session': {
    u'inactivity_timeout': ('inactivity_timeout', 0),
  },
  'session_log': {
    u'keyboard_input_masking_level': ('keyboard_input_masking_level', 2),
  },
  'rdp': {
    u'enable_nla': ('enable_nla', True),
    u'enable_kerberos': ('enable_kerberos', False),
    u'server_redirection': ('server_redirection', False),
    u'load_balance_info': ('load_balance_info', u''),
    u'use_client_provided_alternate_shell': ('use_client_provided_alternate_shell', False),
    u'use_client_provided_remoteapp': ('use_client_provided_remoteapp', False),
    u'use_native_remoteapp_capability': ('use_native_remoteapp_capability', True),
    u'wabam_uses_translated_remoteapp': ('wabam_uses_translated_remoteapp', False),
    u'rail_disconnect_message_delay': ('remote_programs_disconnect_message_delay', 3000),
    u'use_session_probe_to_launch_remote_program': ('use_session_probe_to_launch_remote_program', True),
  },
  'session_probe': {
    u'session_probe': ('enable_session_probe', False),
    u'session_probe_use_smart_launcher': ('use_smart_launcher', True),
    u'session_probe_enable_launch_mask': ('enable_launch_mask', True),
    u'enable_session_probe_launch_mask': ('enable_launch_mask', None), # Deprecated, for compatibility only.
    u'session_probe_on_launch_failure': ('on_launch_failure', 2),
    u'session_probe_launch_timeout': ('launch_timeout', 40000),
    u'session_probe_launch_fallback_timeout': ('launch_fallback_timeout', 10000),
    u'session_probe_start_launch_timeout_timer_only_after_logon': ('start_launch_timeout_timer_only_after_logon', True),
    u'session_probe_keepalive_timeout': ('keepalive_timeout', 5000),
    u'session_probe_on_keepalive_timeout': ('on_keepalive_timeout', 1),
    u'session_probe_end_disconnected_session': ('end_disconnected_session', False),
    u'session_probe_enable_log': ('enable_log', False),
    u'session_probe_enable_log_rotation': ('enable_log_rotation', True),
    u'session_probe_disconnected_application_limit': ('disconnected_application_limit', 0),
    u'session_probe_disconnected_session_limit': ('disconnected_session_limit', 0),
    u'session_probe_idle_session_limit': ('idle_session_limit', 0),
    u'session_probe_smart_launcher_clipboard_initialization_delay': ('smart_launcher_clipboard_initialization_delay', 2000),
    u'session_probe_smart_launcher_start_delay': ('smart_launcher_start_delay', 0),
    u'session_probe_smart_launcher_long_delay': ('smart_launcher_long_delay', 500),
    u'session_probe_smart_launcher_short_delay': ('smart_launcher_short_delay', 50),
    u'session_probe_launcher_abort_delay': ('launcher_abort_delay', 2000),
    u'session_probe_enable_crash_dump': ('enable_crash_dump', False),
    u'session_probe_handle_usage_limit': ('handle_usage_limit', 0),
    u'session_probe_memory_usage_limit': ('memory_usage_limit', 0),
    u'session_probe_ignore_ui_less_processes_during_end_of_session_check': ('ignore_ui_less_processes_during_end_of_session_check', True),
    u'session_probe_childless_window_as_unidentified_input_field': ('childless_window_as_unidentified_input_field', True),
    u'session_probe_disabled_features': ('disabled_features', 0),
    u'session_probe_public_session': ('public_session', False),
    u'session_probe_outbound_connection_monitoring_rules': ('outbound_connection_monitoring_rules', u''),
    u'session_probe_process_monitoring_rules': ('process_monitoring_rules', u''),
    u'session_probe_extra_system_processes': ('extra_system_processes', u''),
    u'session_probe_windows_of_these_applications_as_unidentified_input_field': ('windows_of_these_applications_as_unidentified_input_field', u''),
  },
  'server_cert': {
    u'server_cert_store': ('server_cert_store', True),
    u'server_cert_check': ('server_cert_check', 1),
    u'server_access_allowed_message': ('server_access_allowed_message', 1),
    u'server_cert_create_message': ('server_cert_create_message', 1),
    u'server_cert_success_message': ('server_cert_success_message', 1),
    u'server_cert_failure_message': ('server_cert_failure_message', 1),
  },
  'video': {
    u'disable_keyboard_log': ('disable_keyboard_log', 1),
  },
  'file_verification': {
    u'enable_up': ('enable_up', False),
    u'enable_down': ('enable_down', False),
    u'log_if_accepted': ('log_if_accepted', False),
  },
  'vnc': {
    u'server_is_apple': ('server_is_apple', False),
    u'server_unix_alt': ('server_unix_alt', False),
  },
}
)xxxyyyzzz";
