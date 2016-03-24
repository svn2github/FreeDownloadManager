/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmURLDownloader.h"
#include "vmsFdmSimpleDownloader.h"
#include "Utils.h"

bool vmsFdmURLDownloader::getPageContent( const std::wstring& url, std::wstring& result ){	
	vmsFdmSimpleDownloader dldr;
	std::stringstream content;
	fsInternetResult res = dldr.Download (url, content);
	if(res == IR_SUCCESS) {
		result = toUnicode(content.str(), CP_UTF8);
		return true;
	}

	return false;
}

UINT64 vmsFdmURLDownloader::extractFileSize( const std::wstring& url ){
	fsDownloadMgr mgr (NULL);
	if(mgr.CreateByUrl (url.c_str(), TRUE) != IR_SUCCESS)
		return 0;
	mgr.QuerySize ();
	UINT64 uSize = mgr.GetDownloader ()->GetSSFileSize ();

	return uSize;
}
