/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsClipboardMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsClipboardMgr::fsClipboardMgr()
{

}

fsClipboardMgr::~fsClipboardMgr()
{

}

void fsClipboardMgr::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
}

LPCTSTR fsClipboardMgr::Text()
{
	if (FALSE == OpenClipboard (m_hWnd))
		return NULL;

#ifdef UNICODE
	HANDLE hMem = GetClipboardData (CF_UNICODETEXT);
#else
	HANDLE hMem = GetClipboardData (CF_TEXT);
#endif

	if (hMem == NULL)
	{
		CloseClipboard ();
		return FALSE;
	}

	LPCTSTR pszText = (LPCTSTR) GlobalLock (hMem);
	m_strText = pszText;
	GlobalUnlock (hMem);

	CloseClipboard ();

	return m_strText;
}

BOOL fsClipboardMgr::Text(LPCTSTR psz)
{
	if (FALSE == OpenClipboard (m_hWnd))
		return FALSE;

	EmptyClipboard ();

	HANDLE hText = GlobalAlloc (GMEM_MOVEABLE, (_tcslen (psz)+1) * sizeof(TCHAR));
	if (hText == NULL)
	{
		CloseClipboard ();
		return FALSE;
	}

	LPTSTR pszText = (LPTSTR) GlobalLock (hText);
	_tcscpy (pszText, psz);
	GlobalUnlock (hText);

	CLIPFORMAT cfmtText;
#ifdef UNICODE
	cfmtText = CF_UNICODETEXT;
#else
	cfmtText = CF_TEXT;
#endif

	SetClipboardData (cfmtText, hText);

	CloseClipboard ();

	return TRUE;
}
