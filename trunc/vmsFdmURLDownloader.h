/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSFDMURLDOWNLOADER
#define VMSFDMURLDOWNLOADER
#include "vmsURLDownloader.h"
class vmsFdmURLDownloader : public vmsURLDownloader{
public:
	bool getPageContent( const std::wstring& url, std::wstring& result );
	UINT64 extractFileSize( const std::wstring& url );
	~vmsFdmURLDownloader(){};
};
#endif