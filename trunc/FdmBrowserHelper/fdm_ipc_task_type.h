/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

enum class fdm_ipc_task_type
{
	unknown,
	handshake,
	ui_strings,
	create_downloads,
	shutdown,
	query_settings,
	browser_proxy,
	window,
	video_sniffer,
	network_request_notification,
	browser_download_state_report
};

ENUM_STREAM_SUPPORT_BEGIN (fdm_ipc_task_type)
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::handshake, L"handshake")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::ui_strings, L"ui_strings")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::create_downloads, L"create_downloads")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::shutdown, L"shutdown")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::query_settings, L"query_settings")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::browser_proxy, L"browser_proxy")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::window, L"window")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::video_sniffer, L"video_sniffer")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::network_request_notification, L"network_request_notification")
	ENUM_STREAM_SUPPORT_ENTRY (fdm_ipc_task_type::browser_download_state_report, L"browser_download_state_report")
ENUM_STREAM_SUPPORT_END (fdm_ipc_task_type)