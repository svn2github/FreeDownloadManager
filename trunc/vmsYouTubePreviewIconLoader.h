/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSYOUTUBEPREVIEWICONLOADER
#define VMSYOUTUBEPREVIEWICONLOADER
class vmsYouTubePreviewIconLoader{
public:
	virtual ~vmsYouTubePreviewIconLoader(){};
	virtual bool loadPreviewIcon( std::wstring url, std::wstring destPath );
};
#endif