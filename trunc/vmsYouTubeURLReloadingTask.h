/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSYOUTUBEURLRELOADINGTASKH
#define VMSYOUTUBEURLRELOADINGTASKH
#include "vmsThreadPoolTask.h"
class vmsYouTubeURLReloadingTask : public vmsThreadPoolTask{
public:
	vmsYouTubeURLReloadingTask(int nDownloadId);
	int doWork();
protected:
	int m_nDownloadId;
};
#endif