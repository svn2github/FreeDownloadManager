/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "YouTubeVideoUrlExtractor.h"

#include "Utils.h"

#include <regex>
 
#include <fstream>
#include <sstream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <locale.h>

#include <cstdio>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <iterator>
#include <algorithm>

YouTubeVideoUrlExtractor::YouTubeVideoUrlExtractor(const std::wstring& url)
{
	fillFormatNamesWithDefaultValues();
	fillMimeTypes();
	if ( IsYouTubeShortLink( url ) )
		_url = ShortLinkToOrdinaryLink( url );
	else 
		_url = url;
	std::shared_ptr<SignatureDecipheringAlgorithmProvider> decipheringAlgorithmProvider = 
		std::make_shared<SignatureDecipheringAlgorithmProvider>();
	m_signatureDecipher = std::make_shared<SignatureDeciphering>( decipheringAlgorithmProvider );
	FormatInfoProvider formatInfoProvider;
	m_formatInfos = formatInfoProvider.getValues();
}

YouTubeVideoUrlExtractor::~YouTubeVideoUrlExtractor(void)
{
}

bool YouTubeVideoUrlExtractor::IsYouTubeVideoPage(const std::wstring& url)
{
	static std::wregex youtubePattern(_T("^(http[s]?):\\/\\/(www\\.)?youtube\\.com\\/watch\\?(([^v=]+)=([^&]+)&)*v=.+"));

	if(std::regex_match(url, youtubePattern))
	{
		return true;
	}

	return IsYouTubeShortLink( url );
}

bool YouTubeVideoUrlExtractor::IsYouTubeShortLink(const std::wstring& url)
{
	static std::wregex youtubeShortLinkPattern(_T("^(http[s]?):\\/\\/youtu\\.be\\/.*"));

	if(std::regex_match(url, youtubeShortLinkPattern))
	{
		return true;
	}

	return false;
}

std::wstring YouTubeVideoUrlExtractor::ShortLinkToOrdinaryLink(const std::wstring& url){
	std::wstring result = _T( "" );	
	auto pos = url.find_last_of(_T("/"));
	if ( ( pos != std::wstring::npos ) && ( pos + 1 < url.size() ) ){		
		std::wstring id = url.substr( pos + 1 );
		result = _T( "http://www.youtube.com/watch?v=" ) + id + _T( "&feature=youtu.be" );
	}

	return result;
}

int YouTubeVideoUrlExtractor::getVideoHeightByItag(std::wstring itag){
	if (itag.empty())
		return 0;
	for (auto it = m_formatInfos.begin(); it != m_formatInfos.end(); ++it){
		if (it->itag == itag)
			return it->height;
	}

	return 0;
}

std::wstring YouTubeVideoUrlExtractor::getResolution(const std::map<std::wstring, std::wstring> &variantConfig){
	std::wstring strFormat;
	std::wstring resolution;
	std::wstring itag;
	auto itagIt = variantConfig.find(_T("itag"));
	if (itagIt != variantConfig.end()){
		itag = itagIt->second;
		int height = getVideoHeightByItag(itag);
		if (height != 0){
			std::wstringstream tstrs;
			tstrs << height;
			resolution = tstrs.str();
		}
	}
	if (resolution.empty()){
		auto qualityIt = variantConfig.find(_T("quality"));
		if (qualityIt != variantConfig.end()){
			std::wstring quality = qualityIt->second; 
			std::transform(quality.begin(), quality.end(), quality.begin(), ::tolower); 
			strFormat = _qualityNames[quality];
			if (boost::ends_with(strFormat, _T("p")) && strFormat.length() > 1)
				resolution = strFormat.substr(0, strFormat.length() - 1);
		}
		else{
			auto sizeIt = variantConfig.find(_T("size"));
			if (sizeIt != variantConfig.end()){
				std::wstring strSize = sizeIt->second;
				std::vector<std::wstring> pair;
				boost::split(pair, strSize, boost::is_any_of(_T("x")));
				if (pair.size() == 2){
					resolution = pair[1];
				}
			}
		}
	}

	return resolution;
}

YouTubeParser::fileNode YouTubeVideoUrlExtractor::extractNode(const std::wstring& variantEncoded, bool bAudioAndVideoAreStoredSeparately){
	YouTubeParser::fileNode node;
	auto variantConfig = ExtractVariantConfig(variantEncoded);

	UINT64 nFileSizeFromVariantConfig = 0;
	std::wstring fileSize = variantConfig[_T("clen")];
	if (!fileSize.empty()){
		try{
			nFileSizeFromVariantConfig = std::stoi(fileSize);
		}
		catch (std::exception const&  ex)
		{
			OutputDebugString(L"\nCannot parse file size from variant config\n");
		}
	}

	std::wstring fullType = variantConfig[_T("type")]; 
	std::wstring mediaType;
	std::wstring fileExtension;
	if (!fullType.empty()){
		std::wstring type; 
		auto pos = fullType.find_first_of(';');
		if (pos != std::wstring::npos){
			type = fullType.substr(0, pos);
		}
		else
			type = fullType;
		std::vector<std::wstring> pair;
		boost::split(pair, type, boost::is_any_of(_T("/")));
		if (pair.size() == 2)
		{
			mediaType = pair[0];
			fileExtension = pair[1];
		}
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
		auto extensionIt = _mimeTypeToExtension.find(fileExtension);
		if (extensionIt != _mimeTypeToExtension.end())
			fileExtension = extensionIt->second;
	}
	std::wstring resolution = getResolution(variantConfig);
	
	std::wstring itag;
	if (variantConfig.find(_T("itag")) != variantConfig.end())
		itag = variantConfig[_T("itag")];

	node.fFile.iPriority = 0;
	node.fFile.nIndex = -1;
	node.nParent = -1;
	node.url = variantConfig[_T("url")];
	if (m_bUsingCipheredSignature && (variantConfig.find(_T("s")) != variantConfig.end()) && m_signatureDecipher){
		std::wstring signature = variantConfig[_T("s")];
		std::wstring decipheredSignature = m_signatureDecipher->decipherSignature(signature, m_player_id, m_player_info);
		
		if (!decipheredSignature.empty())
			node.url += _T("&signature=") + decipheredSignature;
	}
	if (nFileSizeFromVariantConfig > 0)
		node.fFile.nFileSize = nFileSizeFromVariantConfig;
	else
		node.fFile.nFileSize = ExtractFileSize(node.url);
	
	node.is3D = false;
	std::wstring stereo3D = variantConfig[_T("stereo3d")];
	if (stereo3D == _T("1"))
		node.is3D = true;
	std::wstring strFormat = resolution;
	if (!strFormat.empty())
		strFormat += _T("p");
	std::wstring name = BuildVariantName(mediaType, strFormat, node.is3D, fileExtension);

	node.fFile.fileExtension = fileExtension;
	if (mediaType == _T("video")){
		if (bAudioAndVideoAreStoredSeparately)
			node.mediaType = YouTubeParser::VIDEO;
		else
			node.mediaType = YouTubeParser::AUDIOVIDEO;
	}
	else{
		node.mediaType = YouTubeParser::AUDIO;		
	}
	node.fFile.strName = name;
	node.nResolution = 0;
	if (resolution.length() > 0){
		std::wstringstream tstrStream(resolution);
		tstrStream >> node.nResolution;
	}	

	return node;
}

bool YouTubeVideoUrlExtractor::ExtractFromFmtsArray( std::wstring &fmts, bool bAudioAndVideoAreStoredSeparately ){
	std::map<std::wstring, std::wstring> audioURLMap;
	std::vector<std::wstring> variants_encoded;
	boost::split(variants_encoded, fmts, boost::is_any_of(_T(",")));
	for (auto it_variant = variants_encoded.begin(); it_variant != variants_encoded.end(); ++it_variant)
	{
		std::wstring variant_encoded = *it_variant;
		YouTubeParser::fileNode node = extractNode(variant_encoded, bAudioAndVideoAreStoredSeparately);
		if (!node.url.empty() && !node.fFile.fileExtension.empty()){
			std::wstringstream wstrs;
			wstrs << node.nResolution;
			std::wstring strFormat = wstrs.str();
			if (!strFormat.empty())
				strFormat += _T("p");
			if (!foundFormats[strFormat] || !bAudioAndVideoAreStoredSeparately)
			{
				if (!bAudioAndVideoAreStoredSeparately)
					foundFormats[strFormat] = true;
				if (node.mediaType == YouTubeParser::AUDIO){
					audioURLMap[node.fFile.fileExtension] = node.url;
				}
				node.fFile.nIndex = m_nIndex++;
				_variantsInfo.push_back(node);				
			}
		}
	}

	for (auto it = _variantsInfo.begin(); it != _variantsInfo.end(); ++it) {
		if ( it->mediaType == YouTubeParser::VIDEO ){
			auto found = audioURLMap.find( it->fFile.fileExtension );
			if( found != audioURLMap.end() ){
				it->complementaryURL = found->second;
			}
		}
	}

	return true;
}

void YouTubeVideoUrlExtractor::TraverseDASHTree( const boost::property_tree::wptree &tree, YouTubeParser::fileNode &node ){
	BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, tree) {					
		if ( v.first == _T( "id" ) ){
			OutputDebugString( v.second.data().c_str() );
		}
		else if ( v.first == _T( "BaseURL" ) ){
			OutputDebugString( v.second.data().c_str() );
			OutputDebugString( _T("\n") );
			fileNode tempNode;
			node = tempNode;
		}
		TraverseDASHTree( v.second, node );
	}
}

bool YouTubeVideoUrlExtractor::ExtractFromDASHManifest( const std::wstring &dashURL ){
	boost::property_tree::wptree dashTree;
	_variantsInfoDASH.clear();

	std::wstring dashManifest = GetPageContent( dashURL );
	if ( !dashManifest.empty() ){		
		std::wistringstream manifestText( dashManifest );
		boost::property_tree::read_xml(manifestText, dashTree);
		print_tree( dashTree );		
		YouTubeParser::fileNode node;
		TraverseDASHTree( dashTree, node );		
		return true;
	}

	return false;
}

std::wstring YouTubeVideoUrlExtractor::GetVideoInfo(){
	std::wstring id = getYouTubeID();
	std::wstring videoInfo = _T("");
	if (id.empty())
		return videoInfo;
	std::wstring videoInfoURL = _T("http://youtube.com/get_video_info?video_id=");
	videoInfoURL += id;
	videoInfo = GetPageContent( videoInfoURL );		
	if ( !videoInfo.empty() && ( videoInfo.find(_T("status=fail")) != std::wstring::npos ) ){
		videoInfoURL += _T("&el=vevo&el=embedded");
		std::wstring vevoVideoInfo = GetPageContent( videoInfoURL );
		return vevoVideoInfo;
	}
	return videoInfo;
}

std::wstring YouTubeVideoUrlExtractor::GetPlayerIdFromURL( const std::wstring& playerURL ){
	std::wstring playerId;

	auto pos = playerURL.find_last_of(_T("//"));
	if ( pos != playerURL.npos && ( pos >= 9 ) ){
		playerId = playerURL.substr( pos - 9, 9 );
	}
	OutputDebugString( playerId.c_str() );

	return playerId;
}

std::wstring YouTubeVideoUrlExtractor::GetPlayerInfoFromURL( const std::wstring& playerURL ){
	std::wstring _playerURL = playerURL;
	if ( boost::starts_with( _playerURL, _T( "//" ) ) )
		_playerURL = _T( "http:" ) + _playerURL;
	std::wstring playerInfo = GetPageContent( _playerURL );	

	return playerInfo;
}

bool YouTubeVideoUrlExtractor::detectSignature(){	
	std::wstring videoInfo = GetVideoInfo();
	if (videoInfo.find(_T("use_cipher_signature=True")) != std::wstring::npos)
		return true;

	return false;
}

void YouTubeVideoUrlExtractor::parseVideoInfo(){
	_videoInfo = GetVideoInfo();	
	auto videoInfoConfig = ExtractVariantConfig(_videoInfo);

	if (videoInfoConfig.find(L"url_encoded_fmt_stream_map") != videoInfoConfig.end()){
		_urlEncodedFmts = videoInfoConfig[L"url_encoded_fmt_stream_map"];
	}
	if (videoInfoConfig.find(L"adaptive_fmts") != videoInfoConfig.end()){
		_adaptiveFmts = videoInfoConfig[L"adaptive_fmts"];
	}
	if (videoInfoConfig.find(L"use_cipher_signature") != videoInfoConfig.end()){
		std::wstring signatureUsageString = videoInfoConfig[L"use_cipher_signature"];
		std::transform(signatureUsageString.begin(), signatureUsageString.end(), signatureUsageString.begin(), ::tolower);
		m_bUsingCipheredSignature = (signatureUsageString == L"true");
	}
}

void YouTubeVideoUrlExtractor::loadPlayerInfo(const std::wstring &playerURL){
	m_player_id = GetPlayerIdFromURL(playerURL);
	m_player_info = GetPlayerInfoFromURL(playerURL);
}

bool YouTubeVideoUrlExtractor::Extract()
{
	_variantsInfo.clear();
	clearErrorInfo();
	m_player_id.clear();
	m_player_info.clear();
	_urlEncodedFmts.clear();
	_adaptiveFmts.clear();

	_pageContent = GetPageContent();
	auto ytplayerConfig = ExtractPlayerConfig();
	if (ytplayerConfig.empty()) {
		return false;
	}
	_pageTitle = ExtractPageTitle();
	_pageTitle = HTMLDecode( _pageTitle );
	_qualityNames = ExtractQualityNames();
	if ( _qualityNames.empty() ){
		fillQualityNamesWithDefaultValues();
	}

	foundFormats.clear();	
	for (std::map<std::wstring, std::wstring>::const_iterator iter = _qualityNames.begin(); iter != _qualityNames.end(); iter++){
		foundFormats[iter->second] = false;
	}

	m_nIndex = 0;
	

	parseVideoInfo();
	if (m_bUsingCipheredSignature){		
		try{
			std::wstring playerURL = ytplayerConfig.get<std::wstring>(_T("assets.js"));
			loadPlayerInfo( playerURL );
		}
		catch (std::exception const&  ex)
		{
			OutputDebugString(L"Cannot get Player URL");
		}
	}	
	std::wstring fmts;
	try{		
		if (m_bUsingCipheredSignature)
			fmts = ytplayerConfig.get<std::wstring>(_T("args.url_encoded_fmt_stream_map"));
		else
			fmts = _urlEncodedFmts;
		ExtractFromFmtsArray(fmts, false);		
	}
	catch(std::exception const&  ex)
	{
		OutputDebugString( L"Cannot get url_encoded_fmt_stream_map" );
	}

	try{	
		if (m_bUsingCipheredSignature)
			fmts = ytplayerConfig.get<std::wstring>(_T("args.adaptive_fmts"));
		else
			fmts = _adaptiveFmts;
		ExtractFromFmtsArray(fmts, true);		
	}
	catch(std::exception const&  ex)
	{
		OutputDebugString( L"Cannot get adaptive_fmts" );
	}
	errorInfo.url = _url;
	errorInfo.video_url = _url;
	errorInfo.error = _T("No video on page");
	
	SortVariants();	
	return true;
}

std::wstring YouTubeVideoUrlExtractor::GetPageContent()
{	
	return GetPageContent( _url );
}

std::wstring YouTubeVideoUrlExtractor::GetPageContent( const std::wstring &url )
{
	std::wstring result;
	if ( m_urlDownloader )
		m_urlDownloader->getPageContent( url, result );

	return result;
}

std::wstring YouTubeVideoUrlExtractor::ExtractPageTitle()
{
	std::wstring result;

	static std::wregex titlePattern(_T("<\\s*meta\\s*name\\s*=\\s*\"title\"\\s*content\\s*=\\s*\"(.*?)\"\\s*>"));
	std::wsmatch match;
	if(std::regex_search(_pageContent, match, titlePattern)) {
		if(match[1].matched) {
			result = match[1].str();
		}
	}

	return result;
}

boost::property_tree::wptree YouTubeVideoUrlExtractor::ExtractPlayerConfig()
{
	boost::property_tree::wptree result;

	static std::wregex ytplayerConfigPattern(_T("ytplayer\\.config\\s*=\\s*\\{(.*?)\\};"));
	std::wsmatch match;
	if(std::regex_search(_pageContent, match, ytplayerConfigPattern)) {
		if(match[1].matched) {
			
			std::wstring tmp = _T("{");
			tmp += match[1].str();
			tmp += _T("}");
			std::wistringstream configText(tmp);
			boost::property_tree::read_json(configText, result);
		}
	}

	return result;
}

std::map<std::wstring, std::wstring> YouTubeVideoUrlExtractor::ExtractVariantConfig(const std::wstring& rawVariant)
{
	std::map<std::wstring, std::wstring> result;
	if (rawVariant.empty())
		return result;

	std::vector<std::wstring> params_encoded;
	boost::split(params_encoded, rawVariant, boost::is_any_of(_T("&")));

	for (auto it_param = params_encoded.begin(); it_param != params_encoded.end(); ++it_param) {
		std::wstring token = *it_param;

		std::vector<std::wstring> pair;
		boost::split(pair, token, boost::is_any_of(_T("=")));
		if(pair.size() == 2) {
			std::wstring urlEncoded = pair[1];
			std::wstring urlDecoded;
			if (UrlDecode(urlEncoded, urlDecoded))
			{
				result[pair[0]] = urlDecoded;
			}
		}
	}

	return result;
}

bool YouTubeVideoUrlExtractor::UrlDecode(const std::wstring& in, std::wstring& out)
{
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] == _T('%'))
		{
			if (i + 3 <= in.size())
			{
				int value = 0;
				std::wistringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value)
				{
					out += static_cast<TCHAR>(value);
					i += 2;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (in[i] == _T('+'))
		{
			out += ' ';
		}
		else
		{
			out += in[i];
		}
	}
	return true;
}

std::vector<YouTubeParser::fileNode> YouTubeVideoUrlExtractor::GetVariantsInfo()
{
	return _variantsInfo;
}

std::wstring YouTubeVideoUrlExtractor::GetPageTitle()
{
	return _pageTitle;
}

UINT64 YouTubeVideoUrlExtractor::ExtractFileSize(const std::wstring& url)
{
	if ( m_urlDownloader )
		return m_urlDownloader->extractFileSize( url );

	return 0;
}

void YouTubeVideoUrlExtractor::SortVariants()
{
	struct {
		bool operator()(const fileNode& lhs, const fileNode& rhs)
		{	
			if (lhs.is3D && (!rhs.is3D))
				return false;
			if ((!lhs.is3D) && rhs.is3D)
				return true;
			if ( ( lhs.fFile.fileExtension == _T( "mp4" ) ) && ( rhs.fFile.fileExtension != _T( "mp4" ) ) )
				return true;
			if ( ( lhs.fFile.fileExtension != _T( "mp4" ) ) && ( rhs.fFile.fileExtension == _T( "mp4" ) ) )
				return false;

			if ( lhs.fFile.fileExtension != rhs.fFile.fileExtension )
				return lhs.fFile.fileExtension < rhs.fFile.fileExtension;
			
			if ( ( lhs.mediaType == YouTubeParser::AUDIO ) && ( rhs.mediaType != YouTubeParser::AUDIO ) )
				return false;
			if ( ( lhs.mediaType != YouTubeParser::AUDIO ) && ( rhs.mediaType == YouTubeParser::AUDIO ) )
				return true;

			if ( lhs.nResolution != rhs.nResolution )
				return lhs.nResolution > rhs.nResolution;
			
			return lhs.fFile.nFileSize < rhs.fFile.nFileSize;
		}
	} cmp;

	std::sort(_variantsInfo.begin(), _variantsInfo.end(), cmp);
	if ( !_variantsInfo.empty() ){
		_variantsInfo.at( 0 ).fFile.iPriority = 1;
	}
}

std::map<std::wstring, std::wstring> YouTubeVideoUrlExtractor::ExtractQualityNames()
{
	std::map<std::wstring, std::wstring> result;

	static std::wregex titlePattern(_T("<div\\s*id=\"YTP_QUALITY_(.*?)\"\\s*>(.*?)\\s*<"));
	std::wsmatch match;
	auto pageContentIt = _pageContent.cbegin();

	while(std::regex_search(pageContentIt, _pageContent.cend(), match, titlePattern))
	{
		
		if(match.size() < 2)
		{
			continue;
		}
		for (auto groupIt = std::next(match.begin()); groupIt != match.end(); ++groupIt )
		{
			auto key = groupIt->str();
			
			if(++groupIt != match.end() && groupIt->matched)
			{
				std::transform(key.begin(), key.end(), key.begin(), ::tolower); 
				result[key] = groupIt->str();
			}
		}
		pageContentIt = match[0].second; 
	}

	return result;
}

std::wstring YouTubeVideoUrlExtractor::BuildVariantName(const std::wstring& mediaType, const std::wstring& quality, bool bIsStereo3D, const std::wstring& fileExtension)
{
	std::wstring result = _T("[");
	if (bIsStereo3D)
		result += _T("3D ");
	if (!fileExtension.empty())
	{	
		std::wstring formatName = fileExtension;
		std::transform(formatName.begin(), formatName.end(), formatName.begin(), ::toupper); 
		std::wstring transformedFormatName = _formatNames[formatName];
		if ( !transformedFormatName.empty() )
			result += transformedFormatName;
		else
			result += formatName;
	}
	if (!quality.empty())
	{
		result += _T(" ");
		auto qualityNameIt = _qualityNames.find(quality);
		if (qualityNameIt != _qualityNames.end())
		{
			result += qualityNameIt->second;
		}
		else
		{
			result += quality;
		}
	}
	else if ( mediaType == _T("audio") ){
		result += _T(" ");
		result += _T( "Audio" );
	}

	result += _T("]");

	if (!_pageTitle.empty())
	{
		result += _T(" ");
		result += _pageTitle;
	}

	if (!fileExtension.empty())
	{
		result += _T(".");
		result += fileExtension;
	}
	
	RemoveIllegalChars(result);
	return result;
}

void YouTubeVideoUrlExtractor::RemoveIllegalChars(std::wstring& target)
{
	static std::wstring illegalChars = _T("\\/:?*\"<>|");
	for (auto it = target.begin() ; it < target.end() ; ++it){
		bool found = illegalChars.find(*it) != std::wstring::npos;
		if(found){
			*it = _T('_');
		}
	}
}

void YouTubeVideoUrlExtractor::fillQualityNamesWithDefaultValues(){
	_qualityNames[_T("highres")] = _T("2160p");
	_qualityNames[_T("hd1440")] = _T("1440p");
	_qualityNames[_T("hd1080")] = _T("1080p");
	_qualityNames[_T("hd720")] = _T("720p");
	_qualityNames[_T("large")] = _T("480p");
	_qualityNames[_T("medium")] = _T("360p");
	_qualityNames[_T("small")] = _T("240p");
	_qualityNames[_T("tiny")] = _T("144p");
}

void YouTubeVideoUrlExtractor::fillFormatNamesWithDefaultValues(){
	_formatNames[_T("3GPP")] = _T("3GP");
	_formatNames[_T("WEBM")] = _T("WebM");
}

YouTubeParser::ErrorInfo YouTubeVideoUrlExtractor::getErrorInfo(){
	return errorInfo;
}

std::vector<std::wstring> YouTubeVideoUrlExtractor::getPreviewURLs(){
	m_previewURLs.clear();
	std::wstring id = getYouTubeID();	
	if ( !id.empty() ){				
		m_previewURLs.push_back( _T("http://i.ytimg.com/vi/") + id + _T("/hqdefault.jpg") );
		m_previewURLs.push_back( _T("http://i1.ytimg.com/vi/") + id + _T("/hqdefault.jpg") );
		m_previewURLs.push_back( _T("http://i1.ytimg.com/vi/") + id + _T("/maxresdefault.jpg") );
	}
	return m_previewURLs;
}

std::wstring YouTubeVideoUrlExtractor::getYouTubeID(){
	std::wstring id = _T("");
	auto pos = _url.find_first_of(_T("?"));
	if ( ( pos != std::wstring::npos ) && ( pos + 1 < _url.size() ) ){		
		std::wstring strParams = _url.substr( pos + 1 );
		std::vector<std::wstring> params;
		boost::split(params, strParams, boost::is_any_of(_T("&")));
		for (auto it = params.begin(); it != params.end(); ++it)
		{
			std::wstring param = *it;			
			if ( boost::starts_with( param, _T("v=") ) && param.size() > 2 ){
				std::wstring paramId = param.substr( 2 );
				auto timePos = paramId.find_first_of(_T("#"));
				if ( timePos != std::wstring::npos )
					id = paramId.substr( 0, timePos );
				else
					id = paramId;
				break;
			}
		}
	}

	return id;
}

std::wstring YouTubeVideoUrlExtractor::extractItagFromURL( const std::wstring &url){
	static std::wregex pattern(_T(".+(itag=)([^&]+)"));
	std::wsmatch match;
	if (std::regex_search(url, match, pattern)) {
		if ((match.size() > 2) && match[2].matched) {
			return match[2].str();
		}
	}

	return _T( "" );
}

std::wstring YouTubeVideoUrlExtractor::findInFmtsArray(const std::wstring &fmts, const std::wstring &itag, bool bAudioAndVideoAreStoredSeparately){
	std::vector<std::wstring> variants_encoded;
	boost::split(variants_encoded, fmts, boost::is_any_of(_T(",")));
	for (auto it_variant = variants_encoded.begin(); it_variant != variants_encoded.end(); ++it_variant)
	{
		std::wstring variant_encoded = *it_variant;		
		auto variantConfig = ExtractVariantConfig(variant_encoded);
		YouTubeParser::fileNode node = extractNode(variant_encoded, bAudioAndVideoAreStoredSeparately);

		if (!node.url.empty()){
			std::wstring url = node.url;
			std::wstring foundItag = extractItagFromURL(url);
			if (foundItag == itag){
				if (m_bUsingCipheredSignature && (variantConfig.find(_T("s")) != variantConfig.end()) && m_signatureDecipher){
					std::wstring signature = variantConfig[_T("s")];
					std::wstring decipheredSignature = m_signatureDecipher->decipherSignature(signature, m_player_id, m_player_info);
					if (!decipheredSignature.empty())
						url += _T("&signature=") + decipheredSignature;
				}
				node.fFile.nIndex = 0;
				_variantsInfo.push_back( node );

				return url;
			}
				
		}
	}

	return _T("");
}

std::wstring YouTubeVideoUrlExtractor::reloadURL(const std::wstring &oldURL){
	_variantsInfo.clear();
	_pageContent = GetPageContent();
	auto ytplayerConfig = ExtractPlayerConfig();
	if (ytplayerConfig.empty()) {
		return _T("");
	}
	m_bUsingCipheredSignature = detectSignature();
	if (m_bUsingCipheredSignature){
		try{
			std::wstring playerURL = ytplayerConfig.get<std::wstring>(_T("assets.js"));
			loadPlayerInfo(playerURL);
		}
		catch (std::exception const&  ex)
		{
			OutputDebugString(L"Cannot get Player URL");
		}
	}
	std::wstring fmts;
	std::wstring itag = extractItagFromURL( oldURL );
	try{		
		fmts = ytplayerConfig.get<std::wstring>(_T("args.url_encoded_fmt_stream_map"));
		std::wstring result = findInFmtsArray( fmts, itag, false );
		if (!result.empty())
			return result;		
	}
	catch (std::exception const&  ex)
	{
		OutputDebugString(L"Cannot get url_encoded_fmt_stream_map");
	}

	try{
		fmts = ytplayerConfig.get<std::wstring>(_T("args.adaptive_fmts"));
		std::wstring result = findInFmtsArray(fmts, itag, true);
		if (!result.empty())
			return result;
	}
	catch (std::exception const&  ex)
	{
		OutputDebugString(L"Cannot get adaptive_fmts");
	}
	
	return _T("");
}

void YouTubeVideoUrlExtractor::fillMimeTypes(){
	_mimeTypeToExtension[L"x-flv"] = L"flv";
	_mimeTypeToExtension[L"3gpp"] = L"3gp";
}

bool YouTubeVideoUrlExtractor::extractIDFromImageURL(const std::wstring& url, std::wstring& id){	
	static std::wregex youtubeImagePattern(_T("^(http[s]?):\\/\\/([^.]+)\\.ytimg\\.com\\/vi\\/([^\\/]+)\\/[^.]+\\.jpg"));
	std::wsmatch match;
	if (std::regex_match(url, match, youtubeImagePattern) && (match.size() > 3))
	{		
		id = match[3].str();
		return true;
	}

	return false;
}

std::wstring YouTubeVideoUrlExtractor::createDownloadURLFromID(const std::wstring& id){
	std::wstring res = L"https://www.youtube.com/watch?v=";
	res += id;

	return res;
}

std::unique_ptr<YouTubeParser> createYouTubeParser(const std::wstring& url){
	std::unique_ptr<YouTubeParser> parser( new YouTubeVideoUrlExtractor( url ) );
	return parser;
}

