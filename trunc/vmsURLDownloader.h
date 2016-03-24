/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSURLDOWNLOADERH
#define VMSURLDOWNLOADERH
class vmsURLDownloader{
public:
	virtual bool getPageContent( const std::wstring& url, std::wstring& result ) = 0;
	virtual UINT64 extractFileSize( const std::wstring& url ) = 0;
	virtual ~vmsURLDownloader(){};
};
#endif