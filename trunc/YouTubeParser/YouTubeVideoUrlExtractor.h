/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <map>
#include "../vmsURLDownloader.h"
#include "YouTubeParser.h"
#include "SignatureDeciphering.h"
#include "SignatureDecipheringAlgorithmProvider.h"
#include "FormatInfoProvider.h"

class YouTubeVideoUrlExtractor : public YouTubeParser
{
public:	
	YouTubeVideoUrlExtractor(const std::wstring& url);
	virtual ~YouTubeVideoUrlExtractor(void);

	bool IsYouTubeVideoPage(const std::wstring& url);
	bool IsYouTubeShortLink(const std::wstring& url);
	std::wstring ShortLinkToOrdinaryLink(const std::wstring& url);
	bool Extract();
	std::vector<YouTubeParser::fileNode> GetVariantsInfo();	
	YouTubeParser::ErrorInfo getErrorInfo();
	std::wstring GetPageTitle();
	void setDownloader( std::shared_ptr <vmsURLDownloader> urlDownloader ){
		m_urlDownloader = urlDownloader;
	}
	std::vector<std::wstring> getPreviewURLs();
	std::wstring getYouTubeID();
	int getVideoHeightByItag( std::wstring itag );
	std::wstring reloadURL(const std::wstring &oldURL);
	std::wstring extractItagFromURL( const std::wstring &url );
	bool extractIDFromImageURL(const std::wstring& url, std::wstring& id);
	std::wstring createDownloadURLFromID(const std::wstring& id);

private:
	std::wstring GetPageContent();
	std::wstring GetPageContent( const std::wstring &url );
	bool UrlDecode(const std::wstring& in, std::wstring& out);
	std::wstring ExtractPageTitle();
	boost::property_tree::wptree ExtractPlayerConfig();
	std::map<std::wstring, std::wstring> ExtractVariantConfig(const std::wstring& rawVariant);
	UINT64 ExtractFileSize(const std::wstring& url);
	void SortVariants();	
	std::map<std::wstring, std::wstring> ExtractQualityNames();
	std::wstring BuildVariantName(const std::wstring& mediaType, const std::wstring& quality, bool bIsStereo3D, const std::wstring& fileExtension );
	void RemoveIllegalChars(std::wstring& target);	
	bool ExtractFromFmtsArray( std::wstring &fmts, bool bAudioAndVideoAreStoredSeparately );
	void TraverseDASHTree( const boost::property_tree::wptree &tree, YouTubeParser::fileNode &node );
	bool ExtractFromDASHManifest( const std::wstring &dashURL );
	void fillQualityNamesWithDefaultValues();	
	void fillFormatNamesWithDefaultValues();
	void fillMimeTypes();
	std::wstring GetVideoInfo();
	std::wstring GetPlayerIdFromURL( const std::wstring& playerURL );
	std::wstring GetPlayerInfoFromURL( const std::wstring& playerURL );
	std::wstring getResolution(const std::map<std::wstring, std::wstring> &variantConfig);
	std::wstring findInFmtsArray(const std::wstring &fmts, const std::wstring &itag, bool bAudioAndVideoAreStoredSeparately);
	bool detectSignature();
	void loadPlayerInfo( const std::wstring &playerURL );
	void parseVideoInfo();
	YouTubeParser::fileNode extractNode(const std::wstring& variantEncoded, bool bAudioAndVideoAreStoredSeparately);

private:
	std::wstring _url;
	std::wstring _pageContent;
	std::wstring _pageTitle;
	std::wstring _videoInfo;
	std::wstring _adaptiveFmts;
	std::wstring _urlEncodedFmts;
	std::vector<fileNode> _variantsInfo;
	std::vector<fileNode> _variantsInfoDASH;
	std::map<std::wstring, std::wstring> _qualityNames;
	std::map<std::wstring, std::wstring> _formatNames;
	std::map<std::wstring, std::wstring> _mimeTypeToExtension;
	std::shared_ptr <vmsURLDownloader> m_urlDownloader;
	std::map<std::wstring, bool> foundFormats;
	std::vector<std::wstring> m_previewURLs;
	YouTubeParser::ErrorInfo errorInfo;	
	int m_nIndex;
	std::shared_ptr<SignatureDeciphering> m_signatureDecipher;
	bool m_bUsingCipheredSignature;
	std::wstring m_player_id;
	std::wstring m_player_info;
	std::vector<FormatInfo> m_formatInfos;

protected:
	virtual void clearErrorInfo(){
		errorInfo.url = _T("");
		errorInfo.video_url = _T("");
		errorInfo.error = _T("");
	}
};
