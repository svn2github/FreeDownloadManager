/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "vmsSimpleIpcItemSender.h"
#include "vmsSimpleIpcItemReceiver.h"
#include "vmsFdmSettingsManager.h"
#include "vmsFdmBhSniffDllProxyManager.h"
#include "../common/vmsBrowserActivityMonitor.h"
#include "vmsParentBrowserProcessManager.h"
class vmsFdmBhIpcTaskProcessorManager :
	public vmsFdmBhIpcTaskProcessor
{
public:
	vmsFdmBhIpcTaskProcessorManager (vmsSimpleIpcItemSender *ipc_sender,
		vmsSimpleIpcItemReceiver *ipc_rcvr,
		std::weak_ptr <vmsFdmSettingsManager> settingsManager,
		std::shared_ptr <vmsFdmBhSniffDllProxyManager> sniffDllProxyManager,
		std::shared_ptr <vmsBrowserActivityMonitor> browserActivityMonitor,
		std::weak_ptr <vmsParentBrowserProcessManager> parentBrowserProcessManager);
	~vmsFdmBhIpcTaskProcessorManager ();

	void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback);
	void wait_tasks_processed ();

protected:
	std::map <fdm_ipc_task_type, std::unique_ptr <vmsFdmBhIpcTaskProcessor>> m_processors;
};

