/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBrowserHelperApp.h"
#include "vmsSimpleIpcTaskManager.h"
#include "vmsFdmBhIpcTaskProcessorManager.h"
#include "vmsFdmSettingsManager.h"
#include "vmsParentBrowserProcessManager.h"
#include "vmsFdmBhSniffDllProxyManager.h"
#include "../common/vmsBrowserActivityMonitor.h"

vmsFdmBrowserHelperApp::vmsFdmBrowserHelperApp ()
{
}

vmsFdmBrowserHelperApp::~vmsFdmBrowserHelperApp ()
{
}

int vmsFdmBrowserHelperApp::run ()
{
	vmsComInitialize comInit;

	auto parentProcessManager = std::make_shared <vmsParentBrowserProcessManager> ();
	verify (parentProcessManager->injectSniffDllProxy ());

	auto settingsManager = std::make_shared <vmsFdmSettingsManager> ();

	auto sniffDllProxyManager = std::make_shared <vmsFdmBhSniffDllProxyManager> ();
	verify (sniffDllProxyManager->initialize ());

	auto browserActivityMonitor = std::make_shared <vmsBrowserActivityMonitor> ();

	vmsSimpleIpcTaskManager <vmsFdmBhIpcTask> ipcmgr;
	vmsFdmBhIpcTaskProcessorManager task_processor (
		ipcmgr.get_sender (),
		ipcmgr.get_receiver (),
		settingsManager,
		sniffDllProxyManager,
		browserActivityMonitor,
		parentProcessManager);

	auto process_callback = [&ipcmgr] (
		std::shared_ptr <vmsFdmBhIpcTask> task, std::shared_ptr <vmsFdmBhIpcTaskResponse> response)
	{
		vmsSimpleIpcTaskItem <vmsFdmBhIpcTaskResponse> item (response);
		verify (ipcmgr.send (&item));
	};

	std::unique_ptr <vmsSimpleIpcTaskItem<vmsFdmBhIpcTask>> ipcitem;
	while ((ipcitem = ipcmgr.receive_task ()))
	{
		if (ipcitem->task ()->get_type () == fdm_ipc_task_type::shutdown)
			break;
		task_processor.process (ipcitem->task (), process_callback);
	}

	task_processor.wait_tasks_processed ();

	return 0;
}
