/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "WindowsTasksManager.h"
class vmsFdmBhIpcWindowTaskProcessor :
	public vmsFdmBhIpcTaskProcessor
{
public:
	vmsFdmBhIpcWindowTaskProcessor ();
	~vmsFdmBhIpcWindowTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	std::shared_ptr <WindowsTasksManager> m_wtman = std::make_shared <WindowsTasksManager> ();
};

