/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "UrlWnd.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUrlWnd::CUrlWnd()
{
}

CUrlWnd::~CUrlWnd()
{
}

BEGIN_MESSAGE_MAP(CUrlWnd, CStatic)
	//{{AFX_MSG_MAP(CUrlWnd)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CUrlWnd::Init()
{
	SetClassLong (m_hWnd, GCL_HCURSOR, (LONG)LoadCursor (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDC_HAND_)));
	GetWindowText (m_strUrl);
}

void CUrlWnd::OnLButtonDown(UINT , CPoint )
{
	if (_tcsncicmp (m_strUrl, _T("mailto:"), 7))
		fsOpenUrlInBrowser (m_strUrl);
	else
		ShellExecute (::GetDesktopWindow (), _T("open"), m_strUrl, NULL, NULL, SW_SHOW);
}

void CUrlWnd::SetUrl(LPCSTR pszUrl)
{
	m_strUrl = pszUrl;
}
