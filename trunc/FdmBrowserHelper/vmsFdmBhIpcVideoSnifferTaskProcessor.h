/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "vmsFdmBhSniffDllProxyManager.h"
class vmsFdmBhIpcVideoSnifferTaskProcessor :
	public vmsFdmBhIpcTaskProcessor,
	public vmsCreatesThreads2
{
public:
	vmsFdmBhIpcVideoSnifferTaskProcessor (
		std::shared_ptr <vmsFdmBhSniffDllProxyManager> sniffDllProxyManager);
	~vmsFdmBhIpcVideoSnifferTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	std::shared_ptr <vmsFdmBhSniffDllProxyManager> m_sniffDllProxyManager;

protected:
	void thread_process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback);

	bool process_CreateVideoDownloadFromUrl 
		(std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback);
};

