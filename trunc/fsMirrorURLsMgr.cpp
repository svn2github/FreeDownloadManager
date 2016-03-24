/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsInetUtils.h"
#include "fsMirrorURLsMgr.h"
#include "inetutil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsMirrorURLsMgr::fsMirrorURLsMgr()
{
	m_dldr.SetEventFunc (_DldrEvents, this);
}

fsMirrorURLsMgr::~fsMirrorURLsMgr()
{

}

void fsMirrorURLsMgr::Initialize(LPCTSTR pszFileName, UINT64 uSize, LPCTSTR pszBaseServer, fsInternetSession* pSession)
{
	m_strFile = pszFileName;
	m_uSize = uSize;
	m_strBaseServer = pszBaseServer;
	m_dldr.Initialize (pSession);
}

void fsMirrorURLsMgr::Set_SearchURL(LPCTSTR pszUrl)
{
	m_strSearchURL = pszUrl;
}

int fsMirrorURLsMgr::Get_MirrorURLCount()
{
	return m_vMirrorURLs.size ();
}

LPCTSTR fsMirrorURLsMgr::Get_MirrorURL(int iIndex)
{
	return m_vMirrorURLs [iIndex];
}

fsInternetResult fsMirrorURLsMgr::SearchForMirrors()
{
	fsInternetResult ir;

	CString strURL = m_strSearchURL;

	strURL.Replace (_T("%file%"), m_strFile);

	if (m_uSize != _UI64_MAX)
	{
		CString strSize;
		strSize.Format (_T("%I64u"), m_uSize);
		strURL.Replace (_T("%size%"), strSize);
	}
	else
		strURL.Replace (_T("%size%"), _T(""));

	m_bAbort = FALSE;

	LPCTSTR ptsz = _tcsrchr (strURL,  _T('/'));
	if (ptsz)
	{
		tstring tstrReferer;
		LPCTSTR ptsz2 = _tcsrchr (strURL,  _T('?'));
		if (ptsz2 < ptsz)
		{
			tstrReferer.assign (strURL, ptsz2 - (LPCTSTR)strURL);
			LPCTSTR ptsz = _tcsrchr (tstrReferer.c_str (),  '/');
			if (ptsz)
			{
				tstring tstr;
				tstr.assign (tstrReferer.c_str (), ptsz - tstrReferer.c_str () + 1);
				tstrReferer = tstr;
			}
		}
		else
		{
			tstrReferer.assign (strURL, ptsz - (LPCTSTR)strURL + 1);
		}
		m_dldr.Get_File ()->SetReferer (tstrReferer.c_str ());
	}

	ir = m_dldr.Download (strURL);
	if (ir != IR_SUCCESS)
		return ir;

	return OnSearchScriptResultsReceived ();
}

void fsMirrorURLsMgr::_DldrEvents(fsInternetURLFileDownloaderEvent ev, LPVOID lp)
{
	fsMirrorURLsMgr* pThis = (fsMirrorURLsMgr*) lp;

	switch (ev)
	{
		case UFDE_CONNECTING:
			pThis->Event (MUME_CONNECTINGSEARCHSERVER);
			break;

		case UFDE_DOWNLOADING:
			pThis->Event (MUME_RETREIVINGSEARCHRESULTS);
			break;
	}
}

void fsMirrorURLsMgr::Set_EventFunc(fntMirrorURLsMgrEvents pfn, LPVOID lp)
{
	m_pfnEvents = pfn;
	m_lpEvParam = lp;
}

void fsMirrorURLsMgr::Event(fsMirrorURLsMgrEvent ev)
{
	if (m_pfnEvents)
		m_pfnEvents (ev, m_lpEvParam);
}

fsInternetResult fsMirrorURLsMgr::OnSearchScriptResultsReceived()
{
	fsHTMLParser parser;

	parser.SetKillDupes (TRUE);
	parser.ParseHTML (LPSTR (m_dldr.Get_FileBuffer ()));

	m_vMirrorURLs.clear ();

#ifdef UNICODE
	WCHAR wszUrl[10000] = {0,};
	DWORD dwSize = 0;
#endif

	for (int i = 0; i < parser.GetUrlCount () && m_bAbort == FALSE; i++)
	{
		fsURL url;
		LPCTSTR pszUrl = 0;
#ifdef UNICODE
		dwSize = sizeof(wszUrl);
		vmsAnsiUrlToUnicode(parser.GetUrl (i), wszUrl, &dwSize);
		pszUrl = &wszUrl[0];
#else
		LPCTSTR pszUrl = parser.GetUrl (i);
#endif

		if (IR_SUCCESS != url.Crack (pszUrl))
			continue;

		if (fsIsServersEqual (url.GetHostName (), m_strBaseServer, TRUE) ||
			fsIsServersEqual (m_strBaseServer, url.GetHostName (), TRUE)    )
			continue;

		TCHAR szFileName [10000];
		fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
			TRUE, szFileName, sizeof (szFileName));

		if (_tcsicmp (szFileName, m_strFile) == 0)
		{
			

			if (IsMirrorURLGood (pszUrl) == FALSE)
				continue;

			m_vMirrorURLs.add (pszUrl);
		}
	}

	m_dldr.Free_FileBuffer ();

	if (m_bAbort)
		return IR_S_FALSE;

	Event (MUME_DONE);
	return IR_SUCCESS;
}

BOOL fsMirrorURLsMgr::IsMirrorURLGood(LPCTSTR )
{
	return TRUE;
}

void fsMirrorURLsMgr::Abort()
{
	m_bAbort = TRUE;
	m_dldr.Abort ();
}

fsMirrorURLsMgr_FileMirrorsDotCom::fsMirrorURLsMgr_FileMirrorsDotCom()
{
	Set_SearchURL (0);
}

fsMirrorURLsMgr_FileMirrorsDotCom::~fsMirrorURLsMgr_FileMirrorsDotCom()
{

}

void fsMirrorURLsMgr_FileMirrorsDotCom::Set_SearchURL(int nURL)
{
	static LPCTSTR _ppszURLs [] = {
		_T("http://www.filesearching.com/cgi-bin/s?q=%file%&w=a&t=f&e=on&m=20&o=n&s1=%size%&s2=%size%&d=&p=&p2=&x=28&y=14"),
		_T("http://findfiles.com/list.php?string=%file%&db=Mirrors&size=%size%"),
	};

	if (nURL < sizeof (_ppszURLs) / sizeof (LPCSTR))
		fsMirrorURLsMgr::Set_SearchURL (_ppszURLs [nURL]);
}
