/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcDownloadStateReportTaskProcessor.h"

vmsFdmBhIpcDownloadStateReportTaskProcessor::vmsFdmBhIpcDownloadStateReportTaskProcessor (
	std::shared_ptr <vmsBrowserActivityMonitor> browserActivityMonitor) :
	m_browserActivityMonitor (browserActivityMonitor)
{
}

vmsFdmBhIpcDownloadStateReportTaskProcessor::~vmsFdmBhIpcDownloadStateReportTaskProcessor ()
{
}

void vmsFdmBhIpcDownloadStateReportTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	auto it = m_downloadsInProgress.find (task->m_downloadState.m_url);

	if (task->m_downloadState.m_state == L"created")
	{
		if (it == m_downloadsInProgress.end ())
			m_downloadsInProgress.insert (std::make_pair (task->m_downloadState.m_url, 1));
		else
			it->second++;

		m_browserActivityMonitor->onDownloadBegin (
			task->m_downloadState.m_url.c_str ());
	}
	else
	{
		if (it != m_downloadsInProgress.end ())
		{
			it->second--;
			if (!it->second)
				m_downloadsInProgress.erase (it);
			m_browserActivityMonitor->onDownloadEnd (
				task->m_downloadState.m_url.c_str ());
		}
	}

	callback (task, construct_response (task));
}