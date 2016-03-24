/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSAFTERMERGEACTION
#define VMSAFTERMERGEACTION
#include "vmsAVMergingTask.h"

class vmsAfterMergeAction{
public:
	virtual ~vmsAfterMergeAction(){};
	virtual void doAfterMerge( std::shared_ptr<vmsAVMergingTask> mergingTask, int bMergeResult ) = 0;
};
#endif