/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "FloatingWndDropTarget.h"
#include "DownloadsWnd.h"
#include "SpiderWnd.h"
#include "SpiderAddPageDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CDownloadsWnd* _pwndDownloads;
extern CSpiderWnd* _pwndSpider;

CFloatingWndDropTarget::CFloatingWndDropTarget()
{

}

CFloatingWndDropTarget::~CFloatingWndDropTarget()
{

}

DROPEFFECT CFloatingWndDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pData, DWORD dwKeyState, CPoint pt)
{
	return OnDragOver (pWnd, pData, dwKeyState, pt);
}

DROPEFFECT CFloatingWndDropTarget::OnDragOver(CWnd*, COleDataObject* pData, DWORD , CPoint )
{
	CLIPFORMAT cfmt;
#ifdef UNICODE
	cfmt = CF_UNICODETEXT;
#else
	cfmt = CF_TEXT;
#endif

	if (pData->IsDataAvailable (cfmt))
	{
		HGLOBAL hMem = pData->GetGlobalData (cfmt);
		if (hMem)
		{
			LPCTSTR psz = (LPCTSTR) GlobalLock (hMem);
			fsURL url;
			BOOL bOk = url.Crack (psz) == IR_SUCCESS;
			GlobalUnlock (hMem);
			GlobalFree (hMem);
			if (bOk)
				return DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK;
		}
	}
	else if (pData->IsDataAvailable (CF_HDROP))
	{
		return DROPEFFECT_COPY;
	}

	return DROPEFFECT_NONE;
}

BOOL CFloatingWndDropTarget::OnDrop(CWnd*, COleDataObject *pData, DROPEFFECT, CPoint)
{
	CLIPFORMAT cfmtText;
#ifdef UNICODE
	cfmtText = CF_UNICODETEXT;
#else
	cfmtText = CF_TEXT;
#endif

	CLIPFORMAT cf;
	if (pData->IsDataAvailable (cfmtText))
		cf = cfmtText;
	else if (pData->IsDataAvailable (CF_HDROP))
		cf = CF_HDROP;
	else
		return FALSE;

	HGLOBAL hMem = pData->GetGlobalData (cf);
	if (hMem)
	{
		std::vector <fsString> vUrls;
		LPVOID pvLock = GlobalLock (hMem);

		if (cf == cfmtText)
		{
			LPCTSTR pszUrl = (LPCTSTR) pvLock;
			fsURL url;
			if (url.Crack (pszUrl) == IR_SUCCESS)
				vUrls.push_back (pszUrl);
		}
		else
		{
			HDROP hDrop = (HDROP) pvLock;
			int cFiles = DragQueryFile (hDrop, 0xFFFFFFFF, NULL, 0);
			for (int i = 0; i < cFiles; i++)
			{
				TCHAR szFile [MY_MAX_PATH];
				DragQueryFile (hDrop, i, szFile, sizeof (szFile));
				fsString strUrl = _T("file://");
				strUrl += szFile;
				vUrls.push_back (strUrl);
			}
		}
		
		if (!vUrls.empty ())
		{
			BOOL bSilent = _App.Monitor_Silent ();

			auto p = new std::vector <std::pair <vmsNewDownloadInfo, bool>> ();

			for (size_t i = 0; i < vUrls.size (); i++)
			{
				vmsNewDownloadInfo dl;
				dl.strUrl = vUrls [i];
				dl.bAddSilent = bSilent;
				dl.useSpecificCreationMethod = vmsNewDownloadInfo::scm_use;
				p->push_back (std::make_pair (dl, bSilent != FALSE));
			}

			_pwndDownloads->PostMessage (WM_DW_CREATE_DOWNLOADS, TRUE, (LPARAM)p);
		}

		GlobalUnlock (hMem);
		GlobalFree (hMem);

		if (vUrls.size ())
			return TRUE;
	}

	return FALSE;
}
