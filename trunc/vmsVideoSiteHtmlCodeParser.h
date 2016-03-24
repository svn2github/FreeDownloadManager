/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSVIDEOSITEHTMLCODEPARSER_H__36077242_C9A7_4188_8F8D_CFE99AF921C2__INCLUDED_)
#define AFX_VMSVIDEOSITEHTMLCODEPARSER_H__36077242_C9A7_4188_8F8D_CFE99AF921C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsVideoSiteHtmlCodeParser  
{
public:
	BOOL Parse_Further (LPCSTR pszSite, LPCSTR pszHtml);
	BOOL get_IsVideoUrlDirectLink();
	BOOL Parse_GoogleVideo (LPCSTR pszHtml);
	static BOOL IsSiteSupported (LPCTSTR pszHost);
	LPCSTR get_VideoType();
	LPCSTR get_VideoUrl();
	LPCSTR get_VideoTitle();
	BOOL Parse (LPCSTR pszSite, LPCSTR pszHtml);

	static int GetSupportedSiteIndex (LPCTSTR pszSite)
	{
		
		#define register_site_with_subdomains(s,n)\
			if (_tcsicmp (pszSite, ##s) == 0)\
				return n;\
			if (lstrlen (pszSite) > lstrlen (_T(".")##s) && \
					_tcsnicmp (pszSite + lstrlen (pszSite) - lstrlen (_T(".")##s), _T(".")##s, lstrlen (_T(".")##s)) == 0)\
				return n;
		

		if (_tcsnicmp (pszSite, _T("www."), 4) == 0)
			pszSite += 4;	

		register_site_with_subdomains (_T("youtube.com"), 0);
		

		if (_tcsnicmp (pszSite, _T("video.google."), 13) == 0)
			return 1;

		register_site_with_subdomains (_T("livedigital.com"), 2);

		register_site_with_subdomains (_T("myspace.com"), 3);

		if (_tcsicmp (pszSite, _T("sharkle.com")) == 0)
			return 4;

		if (_tcsicmp (pszSite, _T("blennus.com")) == 0)
			return 5;

		if (_tcsicmp (pszSite, _T("dailymotion.com")) == 0)
			return 6;

		if (_tcsicmp (pszSite, _T("grouper.com")) == 0)
			return 7;

		return -1;
	}

	vmsVideoSiteHtmlCodeParser();
	virtual ~vmsVideoSiteHtmlCodeParser();

protected:
	static std::string ExtractValue (LPSTR &psz);
	BOOL Parse_Grouper (LPCSTR pszHtml);
	BOOL Parse_DailyMotion (LPCSTR pszHtml);
	BOOL Parse_Blennus (LPCSTR pszHtml);
	BOOL Parse_Sharkle (LPCSTR pszHtml);
	BOOL Parse_Further_MySpace (LPCSTR pszHtml);
	BOOL Parse_MySpace (LPCSTR pszHtml);
	BOOL Parse_Further_LiveDigital (LPCSTR pszTxt);
	BOOL Parse_LiveDigital (LPCSTR pszHtml);
	BOOL m_bDirectLink;
	BOOL Parse_Youtube_RootPage (LPCSTR pszHtml);
	std::string m_strVideoTitle, m_strVideoUrl, m_strVideoType;
	BOOL Parse_Youtube (LPCSTR pszHtml);
};

#endif 
