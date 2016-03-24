/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsOptionalModulesDownloader
{
public:
	
	virtual bool download_module (const std::wstring &url, 
		std::wstring &out_path, 
		vmsOperationProgressCallback *callback, vmsError &err) = 0;
	virtual ~vmsOptionalModulesDownloader(void) {}
};

