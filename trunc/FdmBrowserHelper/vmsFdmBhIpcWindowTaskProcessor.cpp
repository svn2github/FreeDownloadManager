/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcWindowTaskProcessor.h"

vmsFdmBhIpcWindowTaskProcessor::vmsFdmBhIpcWindowTaskProcessor ()
{
}

vmsFdmBhIpcWindowTaskProcessor::~vmsFdmBhIpcWindowTaskProcessor ()
{
}

void vmsFdmBhIpcWindowTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	auto fn_process = [] (
		std::shared_ptr <WindowsTasksManager> wtman,
		std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback)
	{
		wtman->perform_task (task->m_window.m_task);
		auto resp = vmsFdmBhIpcTaskProcessor::construct_response (task);
		callback (task, resp);
	};

	std::async (std::launch::async, fn_process,
		m_wtman, task, callback);
}
