/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "FdmApp.h"
#include "Downloads_Bittorrent_Peers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

CDownloads_Bittorrent_Peers::CDownloads_Bittorrent_Peers()
{
	m_bDldChanged = false;
}

CDownloads_Bittorrent_Peers::~CDownloads_Bittorrent_Peers()
{
}  

BEGIN_MESSAGE_MAP(CDownloads_Bittorrent_Peers, CListCtrl)
	//{{AFX_MSG_MAP(CDownloads_Bittorrent_Peers)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_UPDATE, OnCmdUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDownloads_Bittorrent_Peers::Create(CWnd *pwndParent)
{
	CRect rc (0, 0, 50, 50);

	if (FALSE == CListCtrl::Create (WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER,
			rc, pwndParent, 0x1fa1))
		return FALSE;

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	InsertColumn (0, "", LVCFMT_LEFT, 50);
	InsertColumn (1, "", LVCFMT_LEFT, 50);
	InsertColumn (2, "", LVCFMT_LEFT, 150);
	InsertColumn (3, "", LVCFMT_LEFT, 150);

	return TRUE;
}

void CDownloads_Bittorrent_Peers::set_ActiveDownload(vmsDownloadSmartPtr dld)
{
	m_dld = dld;
	if (dld == NULL)
		return;
	m_bDldChanged = true;
	UpdateStat ();
}

void CDownloads_Bittorrent_Peers::UpdateStat()
{
	if (IsWindowVisible () == FALSE)
		return;

	vmsBtDownloadPeerInfoList *p;
	p = m_dld->pMgr->GetBtDownloadMgr ()->get_PeerInfoList ();
	if (p == NULL)
	{
		DeleteAllItems ();
		return;
	}

	if (p->UpdateList () || m_bDldChanged)
	{
		m_bDldChanged = false;
		DeleteAllItems ();

		for (int i = 0; i < p->get_PeerCount (); i++)
		{
			vmsBtDownloadPeerInfo *peer = p->get_Peer (i);

			char sz [100];
			peer->get_Ip (sz);
			InsertItem (i, sz);

			peer->get_Client (sz);
			SetItemText (i, 1, sz);
		}

		if (p->get_PeerCount ())
		{
			SetColumnWidth (0, LVSCW_AUTOSIZE);
			SetColumnWidth (1, LVSCW_AUTOSIZE);
			for (int i = 0; i < 2; i++)
			{
				if (GetColumnWidth (i) < 50)
					SetColumnWidth (i, 50);
			}
		}
	}

	UpdatePeersDetails ();
}

void CDownloads_Bittorrent_Peers::UpdatePeersDetails()
{
	vmsBtDownloadPeerInfoList *p;
	p = m_dld->pMgr->GetBtDownloadMgr ()->get_PeerInfoList ();
	if (p == NULL)
		return;

	if (GetItemCount () != p->get_PeerCount ())
		return;

	for (int i = 0; i < p->get_PeerCount (); i++)
	{
		vmsBtDownloadPeerInfo *peer = p->get_Peer (i);

		CString str;

		str.Format ("%s, %s/%s", BytesToString (peer->get_BytesDownloaded ()),
			BytesToString (peer->get_DownloadSpeed ()), LS (L_S));
		SetItemText (i, 2, str);

		str.Format ("%s, %s/%s", BytesToString (peer->get_BytesUploaded ()),
			BytesToString (peer->get_UploadSpeed ()), LS (L_S));
		SetItemText (i, 3, str);

		
	}	
}

void CDownloads_Bittorrent_Peers::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CListCtrl::OnShowWindow(bShow, nStatus);
	PostMessage (WM_COMMAND, ID_UPDATE);
}

void CDownloads_Bittorrent_Peers::OnCmdUpdate() 
{
	UpdateStat ();
}

void CDownloads_Bittorrent_Peers::ApplyLanguage()
{
	HDITEM item;
	item.mask = HDI_TEXT;

	item.pszText = (LPSTR) "IP";
	GetHeaderCtrl ()->SetItem (0, &item);

	item.pszText = (LPSTR) LS (L_CLIENT);
	GetHeaderCtrl ()->SetItem (1, &item);

	item.pszText = (LPSTR) LS (L_DOWNLOADED);
	GetHeaderCtrl ()->SetItem (2, &item);

	item.pszText = (LPSTR) LS (L_UPLOADED);
	GetHeaderCtrl ()->SetItem (3, &item);
}
