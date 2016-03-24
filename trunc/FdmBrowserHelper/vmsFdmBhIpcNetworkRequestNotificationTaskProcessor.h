/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "vmsFdmBhSniffDllProxyManager.h"
#include "../common/vmsBrowserActivityMonitor.h"
class vmsFdmBhIpcNetworkRequestNotificationTaskProcessor :
	public vmsFdmBhIpcTaskProcessor,
	public vmsCreatesThreads2
{
public:
	vmsFdmBhIpcNetworkRequestNotificationTaskProcessor (
		std::shared_ptr <vmsFdmBhSniffDllProxyManager> sniffDllProxyManager,
		std::shared_ptr <vmsBrowserActivityMonitor> browserActivityMonitor);
	~vmsFdmBhIpcNetworkRequestNotificationTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	std::shared_ptr <vmsFdmBhSniffDllProxyManager> m_sniffDllProxyManager;
	std::shared_ptr <vmsBrowserActivityMonitor> m_browserActivityMonitor;

protected:
	void thread_process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback);
};

