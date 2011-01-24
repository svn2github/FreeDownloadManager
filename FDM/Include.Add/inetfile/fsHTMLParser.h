/********************************************************************************

  Class fsHTMLParser.

  Simple HTML parser.
  It can:
	pull out A, IMG, LINK, AREA, META links from tag

********************************************************************************/

#if !defined(AFX_FSHTMLPARSER_H__AE1402E9_034A_41A0_8BD2_74430D5938AB__INCLUDED_)
#define AFX_FSHTMLPARSER_H__AE1402E9_034A_41A0_8BD2_74430D5938AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <fsString.h>
#include "../list.h"

// LINK tag type
enum fsLinkRelType
{
	LRT_STYLESHEET,	// Points to a CSS file
	LRT_UNKNOWN		// Other
};

// META HTTP-EQUIV type
enum fsMetaHttpEquivType
{
	MHET_REFRESH,	// pointed REFRESH
	MHET_UNKNOWN,	// other
};

class fsHTMLParser  
{
public:
	void ReplaceFrameUrl(int iIndex, LPCSTR pszNewUrl);
	LPCSTR GetFrameUrl  (int iIndex);
	int GetFrameUrlCount();
	void RemoveBaseTag();
	LPCSTR Get_BaseURL();
	// Parse HTML buffer
	void ParseHTML (LPSTR pszHTML);
	// Skip repeated links while parsing ?
	void SetKillDupes (BOOL bKill);
	// Replace URL in LINK tag on pszNewUrl
	// iIndex - URL order number
	void ReplaceLinkUrl(int iIndex, LPCSTR pszNewUrl);
	// Returns URL type from LINK tag
	fsLinkRelType GetLinkUrlRelType (int iIndex);
	// Returns URL from LINK tag
	LPCSTR GetLinkUrl (int iIndex);
	// Returns URL's number in LINK tags
	int GetLinkUrlCount();
	// Replace URL in IMG tag on pszNewImg
	void ReplaceImage (int iIndex, LPCSTR pszNewImg);
	// Replace URL in A tag on pszNewUrl
	void ReplaceUrl (int iIndex, LPCSTR pszNewUrl);
	// Returns URL index from À tag, an image is linked to
	int GetImageLinkTo (int iImage);
	// Returns URL from IMG tag
	LPCSTR GetImage (int iIndex);
	// Returns URL's number in IMG tags
	int GetImageCount();
	// Returns URL's number in A tags
	int GetUrlCount();
	// Returns URL from A tag
	LPCSTR GetUrl (int iIndex);
	// Returns HTML buffer lenght
	UINT GetHTMLLength ();

	fsHTMLParser();
	virtual ~fsHTMLParser();

protected:
	LPCSTR ParseTag_Frame_Src(LPCSTR pszTag, LPCSTR pszAddUrlEnds = NULL);
	static LPCSTR ParseTag_Frame (LPCSTR pszTag, fsHTMLParser* pThis);
	LPCSTR ParseTag_Base_Href(LPCSTR pszTag, LPCSTR pszAddUrlEnds = NULL);
	static LPCSTR ParseTag_Base (LPCSTR pszTag, fsHTMLParser *pThis);
	// Tag parsing
	// All functions receive a pointer to tag beginning
	// and returns a new position after a tag
	LPCSTR ParseTag (LPCSTR pszTag);
	LPCSTR ParseTag_Meta_Content (LPCSTR pszTag, fsMetaHttpEquivType mhet);
	LPCSTR ParseTag_Meta_HttpEquiv (LPCSTR pszTag, fsMetaHttpEquivType* mhet);
	static LPCSTR ParseTag_Meta (LPCSTR pszTag, fsHTMLParser *pThis);
	LPCSTR ParseTag_Link_Href (LPCSTR pszTag);
	LPCSTR ParseTag_Link_Rel (LPCSTR pszTag, fsLinkRelType *lrt);
	static LPCSTR ParseTag_Link (LPCSTR pszTag, fsHTMLParser* parser);
	LPCSTR ParseTag_Img_Src (LPCSTR pszTag);
	LPCSTR ParseTag_A_Href (LPCSTR pszTag, LPCSTR pszAddUrlEnds = NULL);
	LPCSTR ParseTag_Unknown (LPCSTR pszTag);
	static LPCSTR ParseTag_Img (LPCSTR pszTag, fsHTMLParser *pThis);
	static LPCSTR ParseTag_A (LPCSTR pszTag, fsHTMLParser *pThis);

	// Correct positions info about tags with URL, after taking
	// something operation on replacing one URL to another (ReplaceXXX methods).
	void CorrectRegions (int nStart, int nHole);
	
	// describe URL placement in HTML buffer
	struct fsTextRegion
	{
		int nStart;		// start position
		int nEnd;		// end position
	};

	// Replace string, located in rgn position, on new (pszNewVal)
	// Returns difference between old and new strings lenght
	int ReplaceString (LPCSTR pszNewVal, fsTextRegion &rgn);

	// start parsing position
	void ParseHTML ();
	// parse "=" operation
	// pszGiving - pointer to the beginning of a string with ( "=...")
	// ppszValue, [out] - r-value
	// pszAddEnds - string with additional symbols, r-value can be inclosed in.
	// basic symbols - " and '
	LPCSTR Parse_HTMLGiving (LPCSTR pszGiving, LPSTR* ppszValue, LPCSTR pszAddEnds = NULL);
	
	LPSTR m_pszHTML;	// buffer with HTML
	fs::list <fsString> m_vUrls, m_vImgs, m_vLinkUrls, m_vFrameUrls; // Links from A, IMG, LINK tags
	fs::list <fsTextRegion> m_vUrlsRgns, m_vImgsRgns, m_vLinkUrlsRgns, m_vFrameRgns;	// links placement
	fs::list <fsLinkRelType> m_vLRTs;	// link types from LINK tags (see above)
	fs::list <int> m_vImgsLinksTo;		// images points where?
	int m_htmlLen;					// HTML buffer length
	BOOL m_bKillDupes;				// skip repeated URLs ?
	fsString m_strBaseURL;
	fsTextRegion m_BaseURLPosition;
private:
	int m_iTagAOpened;				// is A tag opened ? Yes, == tag index, otherwise == -1.
									// used only while parsing
};

#endif // !defined(AFX_FSHTMLPARSER_H__AE1402E9_034A_41A0_8BD2_74430D5938AB__INCLUDED_)
