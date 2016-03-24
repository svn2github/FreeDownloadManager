/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

inline bool vmsIsYouTubeShortLink (const std::wstring& url)
{
	static std::wregex youtubeShortLinkPattern(_T("^(http[s]?):\\/\\/youtu\\.be\\/.*"));

	if(std::regex_match(url, youtubeShortLinkPattern))
	{
		return true;
	}

	return false;
}

inline bool vmsIsYouTubeVideoPage (const std::wstring& url)
{
	static std::wregex youtubePattern(_T("^(http[s]?):\\/\\/(www\\.)?youtube\\.com\\/watch\\?(([^v=]+)=([^&]+)&)*v=.+"));

	if(std::regex_match(url, youtubePattern))
	{
		return true;
	}

	return vmsIsYouTubeShortLink( url );
}
