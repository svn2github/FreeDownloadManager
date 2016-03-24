/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "../common/vmsBrowserActivityMonitor.h"
class vmsFdmBhIpcDownloadStateReportTaskProcessor :
	public vmsFdmBhIpcTaskProcessor
{
public:
	vmsFdmBhIpcDownloadStateReportTaskProcessor (
		std::shared_ptr <vmsBrowserActivityMonitor> browserActivityMonitor);
	~vmsFdmBhIpcDownloadStateReportTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	std::shared_ptr <vmsBrowserActivityMonitor> m_browserActivityMonitor;
	std::map <std::wstring, uint32_t> m_downloadsInProgress;
};

