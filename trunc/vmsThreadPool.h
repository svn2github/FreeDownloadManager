/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSTHREADPOOLH
#define VMSTHREADPOOLH
#include "vmsThreadPoolTask.h"
class vmsThreadPool{
public:
	virtual void addTask( std::shared_ptr<vmsThreadPoolTask> task ) = 0;
	virtual void runTask( std::shared_ptr<vmsThreadPoolTask> task ) = 0;
	virtual void stop() = 0;
	virtual ~vmsThreadPool(){};
};
#endif