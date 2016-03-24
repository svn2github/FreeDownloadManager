/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSYOUTUBEPARSERERRORSENDERPOST
#define VMSYOUTUBEPARSERERRORSENDERPOST
#include "vmsYouTubeParserErrorSender.h"
#include "YouTubeParser\YouTubeParser.h"
class vmsYouTubeParserErrorSenderPOST : public vmsYouTubeParserErrorSender{
public:
	vmsYouTubeParserErrorSenderPOST( tstring _serverAddress, tstring _scriptAddress ){
		serverAddress = _serverAddress;
		scriptAddress = _scriptAddress;
	}
	~vmsYouTubeParserErrorSenderPOST(){};
	void sendErrorMessage( std::wstring wstrVersion, YouTubeParser::ErrorInfo errorInfo );
private:
	tstring serverAddress, scriptAddress;
};
#endif