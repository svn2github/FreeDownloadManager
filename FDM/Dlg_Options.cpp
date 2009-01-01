/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "Dlg_Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif        

CDlg_Options::CDlg_Options(CWnd* pParent , LPCSTR pszStartPage)
	: CDialog(CDlg_Options::IDD, pParent)
{
	m_fntCaption.CreateFont (15, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, 0, _T ("Tahoma"));

	

	if (pszStartPage)
		m_strStartPage = pszStartPage;

	m_ptCurPage = NULL;
}  

void CDlg_Options::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Options)
	DDX_Control(pDX, IDC_PAGELIST, m_wndPagesList);
	//}}AFX_DATA_MAP
}  

BEGIN_MESSAGE_MAP(CDlg_Options, CDialog)
	//{{AFX_MSG_MAP(CDlg_Options)
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, IDC_PAGELIST, OnSelchangedPagelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDlg_Options::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitializeData ();
	
	

	FillPagesList ();
	
	GetDlgItem (IDC__DLGCAPTION)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC__DLGFRAME)->ShowWindow (SW_HIDE);

	SetDlgItemText (IDCANCEL, LS (L_CANCEL));
	
	return TRUE;
}

void CDlg_Options::set_CurPage(OPTIONS_PAGES_TREE* ptPage)
{
	if (ptPage->GetData ().page == NULL)
	{
		if (ptPage->GetLeafCount ())
		{
			HTREEITEM hItem = m_wndPagesList.GetSelectedItem ();
			hItem = m_wndPagesList.GetChildItem (hItem);
			ASSERT (hItem != NULL);
			m_wndPagesList.SelectItem (hItem);
			set_CurPage (ptPage->GetLeaf (0));
		}
		else
		{
			m_ptCurPage = ptPage;
		}
		return;
	}

	if (m_ptCurPage == ptPage)
		return;

	if (m_ptCurPage && m_ptCurPage->GetData ().page)
		m_ptCurPage->GetData ().page->ShowWindow (SW_HIDE);

	if (IsWindow (ptPage->GetData ().page->m_hWnd) == FALSE)
		CreatePageDialog (ptPage->GetData ().page);

	ptPage->GetData ().page->ShowWindow (SW_SHOW);
	ptPage->GetData ().page->SetFocus ();
	m_ptCurPage = ptPage;

	CRect rc; GetDlgItem (IDC__DLGCAPTION)->GetWindowRect (&rc);
	ScreenToClient (&rc);
	InvalidateRect (&rc, FALSE);
}

void CDlg_Options::CreatePageDialog(CDlg_Options_Page *page)
{
	page->Create (page->get_Template (), this);

	CRect rc; GetDlgItem (IDC__DLGFRAME)->GetWindowRect (&rc);
	ScreenToClient (&rc);

	page->MoveWindow (&rc);
}

void CDlg_Options::OnPaint() 
{
	CPaintDC dc(this); 
	
	CRect rc; 
	GetDlgItem (IDC__DLGCAPTION)->GetWindowRect (&rc);
	ScreenToClient (&rc);
	
	CBrush br (RGB (170, 170, 170));
	CBrush* brOld = dc.SelectObject (&br);

	CPen pen (PS_SOLID, 1, RGB (170, 170, 170));
	CPen* penOld = dc.SelectObject (&pen);

	dc.Rectangle (&rc);

	CFont* fntOld = dc.SelectObject (&m_fntCaption);

	rc.left += 5;
	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor (RGB (255,255,255));
	CString str = m_ptCurPage->GetData ().page ? m_ptCurPage->GetData ().page->get_PageTitle () : 
		m_ptCurPage->GetData ().strJustCaption;
	dc.DrawText (str, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject (fntOld);
	dc.SelectObject (penOld);
	dc.SelectObject (brOld);
}

void CDlg_Options::OnOK() 
{
	if (FALSE == OptionsTree_Apply ())
		return;
	CDialog::OnOK();
}

void CDlg_Options::FillPagesList(OPTIONS_PAGES_TREE* ptRoot, HTREEITEM htRoot)
{
	if (!ptRoot)
		ptRoot = &m_tPages;

	for (int i = 0; i < ptRoot->GetLeafCount (); i++)
	{
		OPTIONS_PAGES_TREE *ptPage = ptRoot->GetLeaf (i);
		CString str = ptPage->GetData ().page ? ptPage->GetData ().page->get_PageShortTitle () : 
			ptPage->GetData ().strJustCaption;
		HTREEITEM hPage = m_wndPagesList.InsertItem (TVIF_TEXT | TVIF_PARAM | TVIF_STATE, 
				str, 0, 0, TVIS_EXPANDED, TVIS_EXPANDED, 
				(DWORD_PTR)ptPage, htRoot, TVI_LAST);
		if (!m_strStartPage.IsEmpty () && m_strStartPage == str)
			m_wndPagesList.Select (hPage, TVGN_CARET);
		if (ptPage->GetLeafCount ())
			FillPagesList (ptPage, hPage);
	}

	if (ptRoot == &m_tPages)
	{
		set_CurPage (ptRoot->GetLeaf (0));
		m_wndPagesList.Select (m_wndPagesList.GetChildItem (TVI_ROOT), TVGN_CARET);
		ASSERT (m_strStartPage.IsEmpty ()); 
	}
}

BOOL CDlg_Options::OptionsTree_Apply(OPTIONS_PAGES_TREE *ptRoot)
{
	if (!ptRoot)
		ptRoot = &m_tPages;

	for (int i = 0; i < ptRoot->GetLeafCount (); i++)
	{
		OPTIONS_PAGES_TREE *ptPage = ptRoot->GetLeaf (i);
		CDlg_Options_Page *page = ptPage->GetData ().page;
		
		if (page && IsWindow (page->m_hWnd))
		{
			if (FALSE == page->Apply ())
			{
				set_CurPage (ptPage);
				return FALSE;
			}
		}
		
		if (ptPage->GetLeafCount ())
		{
			if (FALSE == OptionsTree_Apply (ptPage))
				return FALSE;
		}
	}

	return TRUE;
}

void CDlg_Options::OnSelchangedPagelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	

	HTREEITEM hPage = m_wndPagesList.GetSelectedItem ();
	OPTIONS_PAGES_TREE *ptPage = (OPTIONS_PAGES_TREE*) m_wndPagesList.GetItemData (hPage);

	if (ptPage)
	{
		set_CurPage (ptPage);
		m_wndPagesList.SetFocus ();
	}
	
	*pResult = 0;
}