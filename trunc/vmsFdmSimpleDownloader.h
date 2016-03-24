/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "DownloadProperties.h"
class vmsFdmSimpleDownloader
{
public:
	vmsFdmSimpleDownloader(void);
	~vmsFdmSimpleDownloader(void);

	fsInternetResult Download (const tstring& url, std::ostream& os);

protected:
	static fsInternetResult OpenFile (const fsDownload_Properties& dp, 
		fsDownload_NetworkProperties& dnp, UINT64 uStartPos, 
		std::unique_ptr <fsInternetURLFile> &file);

	static fsInternetResult InitializeWithURL (const tstring& url,
		fsDownload_NetworkProperties& dnp);

protected:
	fsDownload_Properties m_dp;
	fsDownload_NetworkProperties m_dnp;
};

