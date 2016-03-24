/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsFdmBtObject.h"

class vmsUTorrentDownloadsDb : public vmsFdmBtObject
{
public:
	struct Download
	{
		std::string strTorrentFileName;
		std::string strOutputPath;
	};
public:	
	virtual BOOL ImportUtorrentDownloads (LPCTSTR pszDownloadsDbFile) = NULL;
	virtual int getDownloadCount () = NULL;
	virtual const Download* getDownload (int nIndex) = NULL;
};