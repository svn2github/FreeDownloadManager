/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "FdmApp.h"
#include "Downloads_Bittorrent_Files.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

CDownloads_Bittorrent_Files::CDownloads_Bittorrent_Files()
{
}

CDownloads_Bittorrent_Files::~CDownloads_Bittorrent_Files()
{
}  

BEGIN_MESSAGE_MAP(CDownloads_Bittorrent_Files, CListCtrl)
	//{{AFX_MSG_MAP(CDownloads_Bittorrent_Files)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDownloads_Bittorrent_Files::Create(CWnd *pwndParent)
{
	CRect rc (0, 0, 50, 50);

	if (FALSE == CListCtrl::Create (WS_CHILD | LVS_REPORT | LVS_SINGLESEL | 
			LVS_NOSORTHEADER | LVS_OWNERDATA,
			rc, pwndParent, 0x3ea1))
		return FALSE;

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	InsertColumn (0, "Name");
	InsertColumn (1, "Size");
	InsertColumn (2, "Progress", LVCFMT_LEFT, GetStringWidth ("100%") + 20);

	return TRUE;
}

void CDownloads_Bittorrent_Files::ApplyLanguage()
{
	HDITEM item;
	item.mask = HDI_TEXT;

	item.pszText = (LPSTR) LS (L_NAME);
	GetHeaderCtrl ()->SetItem (0, &item);

	item.pszText = (LPSTR) LS (L_SIZE);
	GetHeaderCtrl ()->SetItem (1, &item);

	item.pszText = (LPSTR) LS (L_PROGRESS);
	GetHeaderCtrl ()->SetItem (2, &item);
}

void CDownloads_Bittorrent_Files::set_ActiveDownload(vmsDownloadSmartPtr dld)
{
	m_nFileNameOffset = 0;
	SetItemCount (0);
	m_dld = dld;
	if (dld == NULL)
		return;

	vmsBtDownloadManager *mgr = dld->pMgr->GetBtDownloadMgr ();

	m_nFileNameOffset = mgr->get_RootFolderName ().Length ();
	if (m_nFileNameOffset)
		m_nFileNameOffset++;

	int w = 0;
	for (int i = 0; i < mgr->get_FileCount (); i++)
		w = max (w, GetStringWidth (mgr->get_FileName (i) + m_nFileNameOffset));
	SetColumnWidth (0, w+30);

	w = 0;
	for (i = 0; i < mgr->get_FileCount (); i++)
		w = max (w, GetStringWidth (BytesToString (mgr->get_FileSize (i))));
	SetColumnWidth (1, w+30);  

	SetItemCount (m_dld->pMgr->GetBtDownloadMgr ()->get_FileCount ());
}

void CDownloads_Bittorrent_Files::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;

	try{

	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
		case 0:
			lstrcpyn (pItem->pszText, 
				m_dld->pMgr->GetBtDownloadMgr ()->get_FileName (pItem->iItem) + m_nFileNameOffset,
				pItem->cchTextMax);
			break;

		case 1:
			lstrcpy (pItem->pszText, BytesToString (m_dld->pMgr->GetBtDownloadMgr ()->get_FileSize (pItem->iItem)));
			break;
			
		case 2:
			CString str;
			str.Format ("%d%%", m_dld->pMgr->GetBtDownloadMgr ()->get_FilePercentDone (pItem->iItem));
			lstrcpy (pItem->pszText, str);
			break;
		}
	}

	}catch (...) {}

	*pResult = 0;
}

void CDownloads_Bittorrent_Files::UpdateProgress()
{
	if (IsWindowVisible () == FALSE)
		return;

	CRect rc;
	GetHeaderCtrl ()->GetItemRect (2, &rc);

	CRect rc2; GetClientRect (&rc2);

	rc.top = 0; 
	rc.bottom = rc2.bottom;
	rc.left += 3; rc.right -= 3;

	InvalidateRect (&rc, FALSE);
}
