/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcTaskProcessorManager.h"
#include "vmsFdmBhIpcHandshakeTaskProcessor.h"
#include "vmsFdmBhIpcUiStringsTaskProcessor.h"
#include "vmsFdmBhIpcCreateDownloadsTaskProcessor.h"
#include "vmsFdmBhIpcQuerySettingsTaskProcessor.h"
#include "vmsFdmBhIpcBrowserProxyTaskProcessor.h"
#include "vmsFdmBhIpcWindowTaskProcessor.h"
#include "vmsFdmBhIpcVideoSnifferTaskProcessor.h"
#include "vmsFdmBhIpcNetworkRequestNotificationTaskProcessor.h"
#include "vmsFdmBhIpcDownloadStateReportTaskProcessor.h"

vmsFdmBhIpcTaskProcessorManager::vmsFdmBhIpcTaskProcessorManager (
	vmsSimpleIpcItemSender *ipc_sender,
	vmsSimpleIpcItemReceiver *ipc_rcvr,
	std::weak_ptr <vmsFdmSettingsManager> settingsManager,
	std::shared_ptr <vmsFdmBhSniffDllProxyManager> sniffDllProxyManager,
	std::shared_ptr <vmsBrowserActivityMonitor> browserActivityMonitor,
	std::weak_ptr <vmsParentBrowserProcessManager> parentBrowserProcessManager)
{
	m_processors [fdm_ipc_task_type::handshake] = 
		std::make_unique <vmsFdmBhIpcHandshakeTaskProcessor> (
		ipc_sender, ipc_rcvr, settingsManager, parentBrowserProcessManager);

	m_processors [fdm_ipc_task_type::ui_strings] = 
		std::make_unique <vmsFdmBhIpcUiStringsTaskProcessor> ();

	m_processors [fdm_ipc_task_type::create_downloads] = 
		std::make_unique <vmsFdmBhIpcCreateDownloadsTaskProcessor> (
		settingsManager);

	m_processors [fdm_ipc_task_type::query_settings] =
		std::make_unique <vmsFdmBhIpcQuerySettingsTaskProcessor> (settingsManager);

	m_processors [fdm_ipc_task_type::browser_proxy] =
		std::make_unique <vmsFdmBhIpcBrowserProxyTaskProcessor> ();

	m_processors [fdm_ipc_task_type::window] = 
		std::make_unique <vmsFdmBhIpcWindowTaskProcessor> ();

	m_processors [fdm_ipc_task_type::video_sniffer] =
		std::make_unique <vmsFdmBhIpcVideoSnifferTaskProcessor> (sniffDllProxyManager);

	m_processors [fdm_ipc_task_type::network_request_notification] = 
		std::make_unique <vmsFdmBhIpcNetworkRequestNotificationTaskProcessor> (
		sniffDllProxyManager, browserActivityMonitor);

	m_processors [fdm_ipc_task_type::browser_download_state_report] =
		std::make_unique <vmsFdmBhIpcDownloadStateReportTaskProcessor> (browserActivityMonitor);
}

vmsFdmBhIpcTaskProcessorManager::~vmsFdmBhIpcTaskProcessorManager ()
{
}

void vmsFdmBhIpcTaskProcessorManager::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	auto it = m_processors.find (task->get_type ());
	assert (it != m_processors.end ());
	if (it == m_processors.end ())
	{
		callback (task, construct_response (task, L"unknown_task_type"));
		return;
	}
	it->second->process (task, callback);
}

void vmsFdmBhIpcTaskProcessorManager::wait_tasks_processed ()
{
}
