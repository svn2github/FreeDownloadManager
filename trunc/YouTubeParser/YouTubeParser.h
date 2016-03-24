/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef YOUTUBEPARSERH
#define YOUTUBEPARSERH
#include "vmsURLDownloader.h"
class YouTubeParser{
public:
	enum MediaType{AUDIO, VIDEO, AUDIOVIDEO};
	struct vmsFile {
		std::wstring strName;
		std::wstring fileExtension;
		UINT64 nFileSize;
		int nIndex; 
		int iPriority;
	};
	struct fileNode	{
		vmsFile fFile;
		int nParent;
		std::wstring url;
		std::wstring complementaryURL;
		MediaType mediaType;
		UINT nResolution;
		bool is3D;
	};
	struct ErrorInfo{
		std::wstring url;
		std::wstring video_url;
		std::wstring error;
	};
	virtual std::vector<YouTubeParser::fileNode> GetVariantsInfo() = 0;
	virtual bool IsYouTubeVideoPage(const std::wstring& url) = 0;
	virtual bool Extract() = 0;	
	virtual std::wstring GetPageTitle() = 0;
	virtual void setDownloader( std::shared_ptr <vmsURLDownloader> urlDownloader ) = 0;
	virtual YouTubeParser::ErrorInfo getErrorInfo() = 0;
	virtual std::vector<std::wstring> getPreviewURLs() = 0;
	virtual std::wstring getYouTubeID() = 0;
	virtual std::wstring reloadURL(const std::wstring &oldURL) = 0;
	virtual bool extractIDFromImageURL( const std::wstring& url, std::wstring& id ) = 0;
	virtual std::wstring createDownloadURLFromID( const std::wstring& id ) = 0;
	virtual ~YouTubeParser(){};
};

#endif