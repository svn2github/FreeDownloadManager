/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSYOUTUBEPARSERERRORSENDER
#define VMSYOUTUBEPARSERERRORSENDER
#include "YouTubeParser\YouTubeParser.h"
class vmsYouTubeParserErrorSender{
public:
	vmsYouTubeParserErrorSender(){};
	virtual ~vmsYouTubeParserErrorSender(){};
	virtual void sendErrorMessage( std::wstring wstrVersion, YouTubeParser::ErrorInfo errorInfo ) = 0;
};
#endif