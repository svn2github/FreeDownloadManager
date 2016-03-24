/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSURLDOWNLOADMGRH
#define VMSURLDOWNLOADMGRH
#include "vmsURLDownloader.h"
class vmsURLDownloadMgr{
public:
	virtual std::shared_ptr <vmsURLDownloader> createDownloader () = 0;
	virtual ~vmsURLDownloadMgr(){};
};
#endif