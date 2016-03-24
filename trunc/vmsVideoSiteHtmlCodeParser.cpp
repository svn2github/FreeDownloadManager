/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsVideoSiteHtmlCodeParser.h"
#include "inetutil.h"

_COM_SMARTPTR_TYPEDEF (IXMLDOMNamedNodeMap, __uuidof (IXMLDOMNamedNodeMap));

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define is_valid_char(c) (c >= 32 && c < 127)

vmsVideoSiteHtmlCodeParser::vmsVideoSiteHtmlCodeParser()
{
	CoInitialize (NULL);
}

vmsVideoSiteHtmlCodeParser::~vmsVideoSiteHtmlCodeParser()
{
	CoUninitialize ();
}

BOOL vmsVideoSiteHtmlCodeParser::Parse(LPCSTR pszSite, LPCSTR pszHtml)
{
	USES_CONVERSION;
	tstring sSite = CA2T(pszSite);
	switch (GetSupportedSiteIndex (sSite.c_str()))
	{
	case 0:
		return Parse_Youtube (pszHtml);

	case 1:
		return Parse_GoogleVideo (pszHtml);

	case 2:
		return Parse_LiveDigital (pszHtml);

	case 3:
		return Parse_MySpace (pszHtml);

	case 4:
		return Parse_Sharkle (pszHtml);

	case 5:
		return Parse_Blennus (pszHtml);

	case 6:
		return Parse_DailyMotion (pszHtml);

	case 7:
		return Parse_Grouper (pszHtml);

	default:
		return FALSE;
	}
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Youtube(LPCSTR pszHtml)
{
	std::string strTitle, strUrl; 

	
	
	
	
	LPSTR psz = const_cast<LPSTR>(strstr (pszHtml, "<meta name=\"title\""));

	if (psz)
	{
		psz = strstr (psz, "content=");
		if (psz)
		{
			psz += strlen ("content=");
			if (*psz == '"')
			{
				psz++;
				while (*psz && *psz != '"')
				{
					if (is_valid_char (*psz))
						strTitle += *psz++;
					else
						psz++;				
				}
			}
		}
	}

	
	
	
	psz = const_cast<LPSTR>(strstr (pszHtml, "swfArgs ="));
	if (psz == NULL)
		return FALSE;
	psz = strchr (psz, '{');
	if (psz == NULL)
		return FALSE;
	psz++;

	std::string strBase = "http://youtube.com/"; 
	std::string strParams;
	
	while (*psz != '}')
	{
		while (*psz == ' ' || *psz == ',')
			psz++;

		std::string str;
	
		while (*psz && *psz != ':')
		{
			if (*psz == ' ')
			{
				str = "";
				break;
			}
			str += *psz++;
		}

		if (str.empty ())
			break;

		if (*psz == ':')
			psz++;
		while (*psz == ' ')
			psz++;

		if (str.length () > 2 && str [0] == '"' && str [str.length () - 1] == '"')
		{
			
			str = str.substr(1);
			str [str.length () - 1] = 0;
		}

		if (strcmpi (str.c_str(), "BASE_YT_URL") == 0)
		{
			strBase = ExtractValue (psz);
		}
		else
		{
			if (strParams.empty () == FALSE)
				strParams += '&';

			strParams += str; strParams += "="; strParams += ExtractValue (psz);
		}
	}

	strUrl = strBase;
	strUrl += "get_video?";
	strUrl += strParams;

	

	fsDecodeHtmlText (strTitle);

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "flv";
	m_bDirectLink   = TRUE;

	return TRUE;
}

LPCSTR vmsVideoSiteHtmlCodeParser::get_VideoTitle()
{
	return m_strVideoTitle.c_str();
}

LPCSTR vmsVideoSiteHtmlCodeParser::get_VideoUrl()
{
	return m_strVideoUrl.c_str();
}

LPCSTR vmsVideoSiteHtmlCodeParser::get_VideoType()
{
	return m_strVideoType.c_str();
}

BOOL vmsVideoSiteHtmlCodeParser::IsSiteSupported(LPCTSTR pszHost)
{
	return GetSupportedSiteIndex (pszHost) != -1;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_GoogleVideo(LPCSTR pszHtml)
{
	LPCSTR psz = strstr (pszHtml, "/googleplayer.swf?");
	if (psz == NULL)
		return FALSE;
	psz = strstr (psz, "videoUrl");
	if (psz == NULL)
		return FALSE;
	psz = strstr (psz, "http");
	if (psz == NULL)
		return FALSE;

	std::string strUrl;
	while (*psz && *psz != '"')
		strUrl += *psz++;
	if (strUrl [strUrl.length () - 1] == '\\')
		strUrl [strUrl.length () - 1] = 0;
	fsDecodeHtmlUrl (strUrl);

	std::string strTitle;
	psz = strstr (pszHtml, "pvprogtitle");
	if (psz)
	{
		while (*psz && *psz != '>')
			psz++;
		if (*psz)
		{
			psz++;
			while (*psz == ' ')
				psz++;
			while (*psz && *psz != '<')
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else 
					psz++;
			}
			while (strTitle.length () && strTitle [strTitle.length () - 1] == ' ')
				strTitle [strTitle.length () - 1] = 0;
			fsDecodeHtmlText (strTitle);
		}
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "flv";
	m_bDirectLink   = TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Youtube_RootPage(LPCSTR pszHtml)
{
	std::string strUrl; 

	LPCSTR psz = strstr (pszHtml, "/admp.swf");
	if (psz == NULL)
		return FALSE;

	while (*psz && *psz != '=')
		psz++;
	if (*psz == 0)
		return FALSE;
	psz++;

	std::string strId;

	while (*psz && *psz != '&')
		strId += *psz++;

	strUrl = "http://www.youtube.com/watch?v=";
	strUrl += strId;

	m_strVideoTitle = "";
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "";
	m_bDirectLink = FALSE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::get_IsVideoUrlDirectLink()
{
	return m_bDirectLink;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_LiveDigital(LPCSTR pszHtml)
{
	LPCSTR psz = strstr (pszHtml, "flashvars");
	if (psz == NULL)
		psz = strstr (pszHtml, "flashVars");
	
	std::string strC, strH;

	if (psz)
	{
		LPCSTR pszC = strstr (psz, "c="), 
			pszH = strstr (psz, "h=");

		if (pszC == NULL || pszH == NULL)
			return FALSE;

		pszC += 2;
		pszH += 2;

		while (*pszC && *pszC != '&' && *pszC != '"')
			strC += *pszC++;

		while (*pszH && *pszH != '&' && *pszH != '"')
			strH += *pszH++;
	}
	else
	{
		
		psz = strstr (pszHtml, "content_id/");
		if (psz)
		{
			psz += strlen ("content_id/");
			while (isdigit (*psz))
				strC += *psz++;
		}

		if (strC.empty ())
		{
			psz = strstr (pszHtml, "/content/");
			if (psz)
			{
				psz += strlen ("/content/");
				while (isdigit (*psz))
					strC += *psz++;
				if (strC.empty ())
					return FALSE;
			}
		}

		strH = "livedigital.com";
	}

	std::string strUrl = "http://";
	strUrl += strH;
	strUrl += "/content/flash_load_content/";
	strUrl += strC;

	m_strVideoTitle = "";
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "";
	m_bDirectLink = FALSE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Further(LPCSTR pszSite, LPCSTR pszHtml)
{
	tstring sSite = CA2CT(pszSite);
	switch (GetSupportedSiteIndex (sSite.c_str()))
	{
	case 2:
		return Parse_Further_LiveDigital (pszHtml);

	case 3:
		return Parse_Further_MySpace (pszHtml);

	default:
		return Parse (pszSite, pszHtml);
	}
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Further_LiveDigital(LPCSTR pszTxt)
{
	LPCSTR psz = strstr (pszTxt, "content_url=");
	if (psz == NULL)
		return FALSE;

	std::string strUrl;
	psz += strlen ("content_url=");
	while (*psz && *psz != '&')
		strUrl += *psz++;

	fsDecodeHtmlUrl (strUrl);

	std::string strTitle;

	psz = strstr (pszTxt, "title=");
	if (psz)
	{
		psz += strlen ("title=");
		while (*psz && *psz != '&')
		{
			if (is_valid_char (*psz))
				strTitle += *psz++;
			else
				psz++;
		}
		fsDecodeHtmlText (strTitle);
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = (LPCSTR)strUrl.c_str() + strUrl.length () - 3;
	m_bDirectLink   = TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_MySpace(LPCSTR pszHtml)
{
	CStringA str; 

	LPCSTR psz = strstr (pszHtml, "flashvars=");
	if (psz == NULL)
		psz = strstr (pszHtml, "flashVars=");
	if (psz != NULL)
	{
		psz += 10;

		if (*psz == '"')
			psz++;
		else if (strnicmp (psz, "&quot;", 6) == 0)
			psz += 6;
		else
			return FALSE;

		while (*psz && *psz != '"' && strnicmp (psz, "&quot;", 6))
			str += *psz++;
		if (str.IsEmpty ())
			return FALSE;
		str.Replace ("m=", "mediaID=");
	}
	else
	{
		psz = strstr (pszHtml, "videoID =");
		if (!psz)
		{
			psz = strstr (pszHtml, "videoid=");
			if (!psz)
				return FALSE;
			else 
				psz += 8;
		}
		else
		{
			psz += 9;
		}
		while (*psz == ' ')
			psz++;
		str = "videoID=";
		while (isdigit (*psz))
			str += *psz++;
	}

	std::string strUrl;
	strUrl = "http://"; strUrl += "mediaservices.myspace.com/services/rss.ashx?";
	strUrl += (LPCSTR)str;

	m_strVideoTitle = "";
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "";
	m_bDirectLink = FALSE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Further_MySpace(LPCSTR pszHtml)
{
	USES_CONVERSION;
	IXMLDOMDocumentPtr spXML;
	IXMLDOMNodePtr spNode, spNode2;

	while (*pszHtml && *pszHtml != '<')
		pszHtml++;

	spXML.CreateInstance (__uuidof (DOMDocument));

	if (spXML == NULL)
		return FALSE;

	spXML->put_async (FALSE);

	VARIANT_BOOL bRes;
	spXML->loadXML (A2W (pszHtml), &bRes);
	if (bRes == FALSE)
		return FALSE;

	spXML->selectSingleNode (L"rss", &spNode);
	if (spNode == NULL)
		return FALSE;

	spNode->selectSingleNode (L"channel", &spNode2);
	if (spNode2 == NULL)
		return FALSE;

	spNode = NULL;
	spNode2->selectSingleNode (L"item", &spNode);
	if (spNode == NULL)
		return FALSE;

	spNode2 = NULL;
	spNode->selectSingleNode (L"title", &spNode2);
	if (spNode2 == NULL)
		return FALSE;

	CComBSTR bstrTitle;
	spNode2->get_text (&bstrTitle);

	spNode2 = NULL;
	spNode->selectSingleNode (L"media:content", &spNode2);
	if (spNode2 == NULL)
		return FALSE;
	IXMLDOMNamedNodeMapPtr spAttrs;
	spNode2->get_attributes (&spAttrs);
	if (spAttrs == NULL)
		return FALSE;
	IXMLDOMNodePtr spUrlValue;
	spAttrs->getNamedItem (L"url", &spUrlValue);
	if (spUrlValue == NULL)
		return FALSE;
	COleVariant vtUrl;
	spUrlValue->get_nodeValue (&vtUrl);
	ASSERT (vtUrl.vt == VT_BSTR);
	if (vtUrl.vt != VT_BSTR)
		return FALSE;

	m_strVideoTitle = W2A (bstrTitle);
	fsDecodeHtmlText (m_strVideoTitle);
	m_strVideoUrl   = W2A (vtUrl.bstrVal);
	m_strVideoType  = m_strVideoUrl.c_str() + m_strVideoUrl.length () - 3;
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Sharkle(LPCSTR pszHtml)
{
	LPCSTR psz = strstr (pszHtml, "splayer.swf?");
	if (psz == NULL)
		return FALSE;

	psz = strstr (psz, "rnd=");
	if (psz == NULL)
		return FALSE;

	std::string strRnd;

	psz += 4;
	while (*psz && *psz != '&')
		strRnd += *psz++;
	if (strRnd.empty ())
		return FALSE;

	std::string strUrl;
	strUrl = "http://sharkle.com/inc/misc/about.php?rnd=";
	strUrl += strRnd;
	strUrl += "&ssd=ZeleninGalaburda";

	std::string strTitle;

	psz = strstr (pszHtml, "blog_header");
	if (psz != NULL)
	{
		while (*psz && *psz != '>')
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == ' ' || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != '<')
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			while (strTitle.length () && strTitle [strTitle.length () - 1] == ' ')
				strTitle [strTitle.length () - 1] = 0;
		}
	}

	fsDecodeHtmlText (strTitle);

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "flv";
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Blennus(LPCSTR pszHtml)
{
	LPCSTR psz = strstr (pszHtml, "embed");
	if (psz == NULL)
		psz = strstr (pszHtml, "EMBED");
	if (psz == NULL)
		return FALSE;

	psz = strstr (psz, "src=");
	if (psz == NULL)
		return FALSE;
	psz += 4;
	if (*psz++ != '"')
		return FALSE;
	std::string strUrl;
	while (*psz && *psz != '"')
		strUrl += *psz++;
	if (strUrl.empty ())
		return FALSE;

	std::string strTitle;
	psz = strstr (pszHtml, "contentheading");
	if (psz != NULL)
	{
		while (*psz && *psz != '>')
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == ' ' || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != '<')
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			fsDecodeHtmlText (strTitle);
			while (strTitle.length () && strTitle [strTitle.length () - 1] == ' ')
				strTitle [strTitle.length () - 1] = 0;
		}
	}

	fsDecodeHtmlText (strTitle);

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "wmv";
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_DailyMotion(LPCSTR pszHtml)
{
	LPCSTR psz = strstr (pszHtml, "flashvars=");
	if (psz == NULL)
		psz = strstr (pszHtml, "flashVars=");
	if (psz == NULL)
		return FALSE;
	
_lSearchUrl:
	psz = strstr (psz, "url=");
	if (psz == NULL)
		return FALSE;
	psz += 4;
	if (strncmp (psz, "rev=", 4) == 0)
		goto _lSearchUrl;

	std::string strUrl;
	while (*psz && *psz != '&')
		strUrl += *psz++;
	fsDecodeHtmlUrl (strUrl);

	std::string strTitle;
	psz = strstr (pszHtml, "<h1");
	if (psz == NULL)
		psz = strstr (pszHtml, "<H1");
	if (psz != NULL)
	{
		while (*psz && *psz != '>')
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == ' ' || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != '<')
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			fsDecodeHtmlText (strTitle);
			while (strTitle.length () && strTitle [strTitle.length () - 1] == ' ')
				strTitle [strTitle.length () - 1] = 0;
		}
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "flv";
	m_bDirectLink	= TRUE;

	return TRUE;
}

BOOL vmsVideoSiteHtmlCodeParser::Parse_Grouper(LPCSTR pszHtml)
{
	LPCSTR psz = strstr (pszHtml, "flvURL=");
	if (psz == NULL)
		return FALSE;
	psz += 7;

	std::string strUrl;
	while (*psz && *psz != '&')
		strUrl += *psz++;

	std::string strTitle;
	psz = strstr (pszHtml, "<h1");
	if (psz == NULL)
		psz = strstr (pszHtml, "<H1");
	if (psz != NULL)
	{
		while (*psz && *psz != '>')
			psz++;
		if (*psz != 0)
		{
			psz++;
			while (*psz == ' ' || is_valid_char (*psz) == FALSE)
				psz++;
			while (*psz && *psz != '<')
			{
				if (is_valid_char (*psz))
					strTitle += *psz++;
				else
					psz++;
			}
			fsDecodeHtmlText (strTitle);
			while (strTitle.length () && strTitle [strTitle.length () - 1] == ' ')
				strTitle [strTitle.length () - 1] = 0;
		}
	}

	m_strVideoTitle = strTitle;
	m_strVideoUrl   = strUrl;
	m_strVideoType  = "flv";
	m_bDirectLink	= TRUE;	

	return TRUE;
}

std::string vmsVideoSiteHtmlCodeParser::ExtractValue(LPSTR &psz)
{
	char c;
	while (*psz == ' ')
		psz++;
	if (*psz == '"' || *psz == '\'')
		c = *psz++;
	else 
		c = ',';
	std::string strRes;
	while (*psz && *psz != c)
	{
		if (*psz == '}' && c == ',')
			break;
		strRes += *psz++;
	}
	if (*psz != '}')
		psz++;
	return strRes;
}
