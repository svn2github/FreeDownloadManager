/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsAVMergingTask.h"
class vmsAVMergingMgrCallback
{
public:
	virtual void task_created (std::shared_ptr<vmsAVMergingTask> task) {}
	virtual void task_started (std::shared_ptr<vmsAVMergingTask> task) {}
	virtual void task_finished (std::shared_ptr<vmsAVMergingTask> task, int result) {}
	virtual ~vmsAVMergingMgrCallback(void) {}
};

