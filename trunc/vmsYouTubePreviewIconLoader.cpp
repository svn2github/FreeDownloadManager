/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsYouTubePreviewIconLoader.h"
#include "vmsSimpleFileDownloader.h"
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

bool vmsYouTubePreviewIconLoader::loadPreviewIcon( std::wstring url, std::wstring destPath ){
	vmsSimpleFileDownloader dldr;	
	fsInternetResult res = dldr.Download (url.c_str(), destPath.c_str());
	while (dldr.IsRunning ())
		boost::this_thread::sleep(boost::posix_time::milliseconds(50));
	return ( ( res == IR_SUCCESS ) && ( boost::filesystem::exists( destPath ) ) );
}