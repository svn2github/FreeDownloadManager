/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsYouTubeParserErrorSenderPOST.h"

void vmsYouTubeParserErrorSenderPOST::sendErrorMessage( std::wstring wstrVersion, YouTubeParser::ErrorInfo errorInfo ){
	vmsPostRequest req;
		
	std::string version;
	UniToAnsi( wstrVersion, version );
	if ( version.empty() )
		version = "?";
	std::string page_url, video_url, error;
	UniToAnsi( errorInfo.url, page_url );
	UniToAnsi( errorInfo.video_url, video_url );
	UniToAnsi( errorInfo.error, error );
	req.AddPart ("version", NULL, version.c_str(), version.size() );
	req.AddPart ("page_url", NULL, page_url.c_str(), page_url.size() );
	req.AddPart ("video_url", NULL, video_url.c_str(), video_url.size() );
	req.AddPart ("error", NULL, error.c_str(), error.size() );
	
	req.Send (serverAddress.c_str(), scriptAddress.c_str(), NULL);
}