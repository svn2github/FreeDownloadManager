/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSFDMURLDOWNLOADMGRH
#define VMSFDMURLDOWNLOADMGRH
#include "vmsFdmURLDownloader.h"
#include "vmsURLDownloadMgr.h"
class vmsFdmURLDownloadMgr : public vmsURLDownloadMgr{
public:
	std::shared_ptr <vmsURLDownloader> createDownloader (){
		return std::make_shared <vmsFdmURLDownloader>();
	}
	~vmsFdmURLDownloadMgr(){};
};
#endif