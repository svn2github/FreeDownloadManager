/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "data stretcher.h"
#include "DownloaderProperties_NetworkPage.h"
#include "vmsDialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

IMPLEMENT_DYNCREATE(CDownloaderProperties_NetworkPage, CPropertyPage)

CDownloaderProperties_NetworkPage::CDownloaderProperties_NetworkPage() : CPropertyPage(CDownloaderProperties_NetworkPage::IDD)
{
	m_psp.dwFlags |= PSP_USETITLE;
	m_psp.pszTitle = LS (L_NETWORK);
	m_bIModifing = FALSE;
}

CDownloaderProperties_NetworkPage::~CDownloaderProperties_NetworkPage()
{
}

void CDownloaderProperties_NetworkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloaderProperties_NetworkPage)
	DDX_Control(pDX, IDC_TRAFFICLIMITDIMENSION, m_wndTLD);
	DDX_Control(pDX, IDC_MODE, m_wndMode);
	//}}AFX_DATA_MAP
}  

BEGIN_MESSAGE_MAP(CDownloaderProperties_NetworkPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDownloaderProperties_NetworkPage)
	ON_BN_CLICKED(IDC_USEMAXCONNS, OnUsemaxconns)
	ON_EN_CHANGE(IDC_MAXCONNS, OnChangeMaxconns)
	ON_BN_CLICKED(IDC_USEMAXCONNSPERSERVER, OnUsemaxconnsperserver)
	ON_EN_CHANGE(IDC_MAXCONNSPERSERVER, OnChangeMaxconnsperserver)
	ON_EN_CHANGE(IDC_TRAFFICLIMITVAL, OnChangeTrafficlimitval)
	ON_CBN_SELCHANGE(IDC_TRAFFICLIMITDIMENSION, OnSelchangeTrafficlimitdimension)
	ON_CBN_SELCHANGE(IDC_MODE, OnSelchangeMode)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WHATISTHIS, OnWhatisthis)
	ON_BN_CLICKED(IDC_DETECTACTIVITY, OnDetectactivity)
	ON_BN_CLICKED(IDC_DECBYLEVEL, OnDecbylevel)
	ON_BN_CLICKED(IDC_SETTOLIGHT, OnSettolight)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_MAXDLDSVAL, OnChangeMaxdldsval)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDownloaderProperties_NetworkPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_wndMode.AddString (LS (L_LIGHT));
	m_wndMode.AddString (LS (L_MEDIUM));
	m_wndMode.AddString (LS (L_HEAVY));
	m_wndMode.SetCurSel (2);
	m_iCurMode = 2;

	CString str; 
	
	str.Format ("%s/%s", LS (L_B), LS (L_S));	m_wndTLD.AddString (str); 
	str.Format ("%s/%s", LS (L_KB), LS (L_S)); m_wndTLD.AddString (str); 
	str.Format ("%s/%s", LS (L_MB), LS (L_S)); m_wndTLD.AddString (str);

	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl*) GetDlgItem (IDC_MAXCONNSSPIN);
	pSpin->SetRange (1, UD_MAXVAL);
	pSpin = (CSpinButtonCtrl*) GetDlgItem (IDC_MAXCONNSPERSERVERSPIN);
	pSpin->SetRange (1, UD_MAXVAL);
	pSpin = (CSpinButtonCtrl*) GetDlgItem (IDC_MAXDLDSSPIN);
	pSpin->SetRange (1, UD_MAXVAL);

	CopyMemory (m_aTUM, _DldsMgr.GetTUMs (), sizeof (m_aTUM));

	WriteTUS ();

	if (_DldsMgr.GetTUMManage () == TM_OFF)
		CheckRadioButton (IDC_DECBYLEVEL, IDC_SETTOLIGHT, IDC_DECBYLEVEL);
	else
	{
		CheckDlgButton (IDC_DETECTACTIVITY, BST_CHECKED);
		if (_DldsMgr.GetTUMManage () == TM_DECREASEBYONELEVEL)
			CheckRadioButton (IDC_DECBYLEVEL, IDC_SETTOLIGHT, IDC_DECBYLEVEL);
		else
			CheckRadioButton (IDC_DECBYLEVEL, IDC_SETTOLIGHT, IDC_SETTOLIGHT);
	}

	UpdateEnabled ();

	ApplyLanguage ();
	
	return TRUE;  
	              
}

BOOL CDownloaderProperties_NetworkPage::ReadTUS()
{
	fsTrafficUsageMode tum;
	UINT nID;
	bool bMaxPSErr = false;

	if (IsDlgButtonChecked (IDC_USEMAXCONNS) == BST_CHECKED)
	{
		tum.uMaxConns = GetDlgItemInt (IDC_MAXCONNS, NULL, FALSE);
		if (tum.uMaxConns == 0)
		{
			nID = IDC_MAXCONNS;
			goto _lErr;
		}
	}
	else
		tum.uMaxConns = UINT_MAX;

	if (IsDlgButtonChecked (IDC_USEMAXCONNSPERSERVER) == BST_CHECKED)
	{
		tum.uMaxConnsPS = GetDlgItemInt (IDC_MAXCONNSPERSERVER, NULL, FALSE);
		if (tum.uMaxConnsPS == 0)
		{
			nID = IDC_MAXCONNSPERSERVER;
			goto _lErr;
		}
		if (tum.uMaxConnsPS > MAX_NOT_COMPLETED_SECTION_COUNT)
		{
			bMaxPSErr = true;
			nID = IDC_MAXCONNSPERSERVER;
			goto _lErr;
		}
	}
	else
		tum.uMaxConnsPS = UINT_MAX;

	if (IsDlgButtonChecked (IDC_USEMAXDLDS) == BST_CHECKED)
	{
		tum.uMaxDlds = GetDlgItemInt (IDC_MAXDLDSVAL, NULL, FALSE);
		if (tum.uMaxDlds == 0)
		{
			nID = IDC_MAXDLDSVAL;
			goto _lErr;
		}
	}
	else
		tum.uMaxDlds = UINT_MAX;

	if (m_iCurMode != TUM_HEAVY)
	{
		UINT64 u;
		vmsDialogHelper::GetDlgBytesGroup (this, IDC_TRAFFICLIMITVAL, IDC_TRAFFICLIMITDIMENSION, &u);
		tum.uTrafficLimit = (UINT)u;
		if (tum.uTrafficLimit == 0)
		{
			nID = IDC_TRAFFICLIMITVAL;
			goto _lErr;
		}
	}

	m_aTUM [m_iCurMode] = tum;

	return TRUE;

_lErr:
	CString str;
	if (bMaxPSErr)
		str.Format (LS (L_ENTERLESS), MAX_NOT_COMPLETED_SECTION_COUNT);
	else
		str.Format (LS (L_ENTERGREATER), 0);
	MessageBox (str, LS (L_INPERR), MB_ICONEXCLAMATION);
	CEdit *pE = (CEdit*) GetDlgItem (nID);
	pE->SetSel (0, -1);
	pE->SetFocus ();
	return FALSE;
}

void CDownloaderProperties_NetworkPage::WriteTUS()
{
	m_bIModifing = TRUE;

	fsTrafficUsageMode tum = m_aTUM [m_iCurMode];

	if (tum.uMaxConns != UINT_MAX)
		SetDlgItemInt (IDC_MAXCONNS, tum.uMaxConns);
	
	CheckDlgButton (IDC_USEMAXCONNS, tum.uMaxConns == UINT_MAX ? BST_UNCHECKED : BST_CHECKED);

	if (tum.uMaxConnsPS != UINT_MAX)
		SetDlgItemInt (IDC_MAXCONNSPERSERVER, tum.uMaxConnsPS);

	CheckDlgButton (IDC_USEMAXCONNSPERSERVER, tum.uMaxConnsPS == UINT_MAX ? BST_UNCHECKED : BST_CHECKED);

	if (tum.uMaxDlds != UINT_MAX)
		SetDlgItemInt (IDC_MAXDLDSVAL, tum.uMaxDlds);
	
	CheckDlgButton (IDC_USEMAXDLDS, tum.uMaxDlds == UINT_MAX ? BST_UNCHECKED : BST_CHECKED);

	if (m_iCurMode != TUM_HEAVY)
		vmsDialogHelper::SetDlgBytesGroup (this, tum.uTrafficLimit, IDC_TRAFFICLIMITVAL, IDC_TRAFFICLIMITDIMENSION);
	else
	{
		SetDlgItemText (IDC_TRAFFICLIMITVAL, LS (L_UNLIMITED));
	}

	UpdateEnabled ();

	m_bIModifing = FALSE;
}

void CDownloaderProperties_NetworkPage::UpdateEnabled()
{
	BOOL b;

	b = IsDlgButtonChecked (IDC_USEMAXCONNS) == BST_CHECKED;
	GetDlgItem (IDC_MAXCONNS)->EnableWindow (b);
	GetDlgItem (IDC_MAXCONNSSPIN)->EnableWindow (b);

	b = IsDlgButtonChecked (IDC_USEMAXCONNSPERSERVER) == BST_CHECKED;
	GetDlgItem (IDC_MAXCONNSPERSERVER)->EnableWindow (b);
	GetDlgItem (IDC_MAXCONNSPERSERVERSPIN)->EnableWindow (b);

	b = m_iCurMode != TUM_HEAVY;
	GetDlgItem (IDC__TUR)->EnableWindow (b);
	GetDlgItem (IDC_TRAFFICLIMITVAL)->EnableWindow (b);
	GetDlgItem (IDC_TRAFFICLIMITDIMENSION)->EnableWindow (b);

	b = IsDlgButtonChecked (IDC_DETECTACTIVITY) == BST_CHECKED;
	GetDlgItem (IDC_DECBYLEVEL)->EnableWindow (b);
	GetDlgItem (IDC_SETTOLIGHT)->EnableWindow (b);
}

void CDownloaderProperties_NetworkPage::OnUsemaxconns() 
{
	UpdateEnabled ();
	if (m_bIModifing == FALSE) SetModified ();	
}

void CDownloaderProperties_NetworkPage::OnChangeMaxconns() 
{
	if (m_bIModifing == FALSE) SetModified ();
}

void CDownloaderProperties_NetworkPage::OnUsemaxconnsperserver() 
{
	UpdateEnabled ();
	if (m_bIModifing == FALSE) SetModified ();
}

void CDownloaderProperties_NetworkPage::OnChangeMaxconnsperserver() 
{
	if (m_bIModifing == FALSE) SetModified ();
}

void CDownloaderProperties_NetworkPage::OnChangeTrafficlimitval() 
{
	if (m_bIModifing == FALSE) SetModified ();
}

void CDownloaderProperties_NetworkPage::OnSelchangeTrafficlimitdimension() 
{
	if (m_bIModifing == FALSE) SetModified ();	
}

BOOL CDownloaderProperties_NetworkPage::OnApply() 
{
	if (FALSE == ReadTUS ())
		return FALSE;

	CopyMemory (_DldsMgr.GetTUMs (), m_aTUM, sizeof (m_aTUM));

	
	if (IsDlgButtonChecked (IDC_DETECTACTIVITY) == BST_CHECKED)
	{
		if (IsDlgButtonChecked (IDC_DECBYLEVEL) == BST_CHECKED)
			_DldsMgr.SetTUMManage (TM_DECREASEBYONELEVEL);
		else
			_DldsMgr.SetTUMManage (TM_SETTOLIGHT);
	}
	else
		_DldsMgr.SetTUMManage (TM_OFF);
	
	return CPropertyPage::OnApply();
}

void CDownloaderProperties_NetworkPage::OnSelchangeMode() 
{
	if (FALSE == ReadTUS ())
	{
		m_wndMode.SetCurSel (m_iCurMode);
	}
	else
	{
		m_iCurMode = m_wndMode.GetCurSel ();
		WriteTUS ();
	}
}

void CDownloaderProperties_NetworkPage::OnContextMenu(CWnd* , CPoint point) 
{
	PrepareCHMgr (point);
	_CHMgr.ShowMenu ();
}

void CDownloaderProperties_NetworkPage::OnWhatisthis() 
{
	_CHMgr.OnWT ();	
}

void CDownloaderProperties_NetworkPage::ApplyLanguage()
{
	fsDlgLngInfo lnginfo [] =  {
		fsDlgLngInfo (IDC__TUMS, L_TUMS),
		fsDlgLngInfo (IDC__MODEADJUSTMENT, L_MODETOADJUST),
		fsDlgLngInfo (IDC_USEMAXCONNS, L_MAXCOUNTOFCONNS),
		fsDlgLngInfo (IDC_USEMAXCONNSPERSERVER, L_MAXCOUNTOFCONNSPERSERV),
		fsDlgLngInfo (IDC_USEMAXDLDS, L_MAXDLDS),
		fsDlgLngInfo (IDC__TUR, L_TUR),
		fsDlgLngInfo (IDC_DETECTACTIVITY, L_DETECTACTIVITY),
		fsDlgLngInfo (IDC_DECBYLEVEL, L_DECBYLEVEL),
		fsDlgLngInfo (IDC_SETTOLIGHT, L_SETTOLIGHT)
	};

	_LngMgr.ApplyLanguage (this, lnginfo, sizeof (lnginfo) / sizeof (fsDlgLngInfo), 0);
}

void CDownloaderProperties_NetworkPage::OnDetectactivity() 
{
	UpdateEnabled ();
	SetModified ();
}

void CDownloaderProperties_NetworkPage::OnDecbylevel() 
{
	SetModified ();	
}

void CDownloaderProperties_NetworkPage::OnSettolight() 
{
	SetModified ();	
}

void CDownloaderProperties_NetworkPage::PrepareCHMgr(CPoint point)
{
	fsCtrlContextHelp aCH1 [] = {
		fsCtrlContextHelp (IDC__MODEADJUSTMENT, LS (L_TUMHERE)),
		fsCtrlContextHelp (IDC_MODE, LS (L_TUMHERE)),
		fsCtrlContextHelp (IDC_USEMAXCONNS, LS (L_MAXCONNSHERE)),
		fsCtrlContextHelp (IDC_USEMAXCONNSPERSERVER, LS (L_MAXCONNSPERSERVHERE)),
		fsCtrlContextHelp (IDC_USEMAXDLDS, LS (L_MAXDLDSHERE)),
		fsCtrlContextHelp (IDC__TUR, LS (L_TURHERE)),
		fsCtrlContextHelp (IDC_TRAFFICLIMITVAL, LS (L_TURHERE)),
		fsCtrlContextHelp (IDC_TRAFFICLIMITDIMENSION, LS (L_DIMHERE)),
		fsCtrlContextHelp (IDC_DETECTACTIVITY, LS (L_DETECTACTIVITYHERE)),
		fsCtrlContextHelp (IDC_DECBYLEVEL, LS (L_DECBYLEVELHERE)),
		fsCtrlContextHelp (IDC_SETTOLIGHT, LS (L_SETTOLIGHTHERE)),
	};

	static fsCtrlContextHelp aCH [sizeof (aCH1) / sizeof (fsCtrlContextHelp)];
	CopyMemory (aCH, aCH1, sizeof (aCH));

	_CHMgr.PrepareContextMenu (this, point, aCH, sizeof (aCH) / sizeof (fsCtrlContextHelp));	
}

BOOL CDownloaderProperties_NetworkPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	RECT rc;
	CWnd *pItem = GetDlgItem (pHelpInfo->iCtrlId);
	if (pItem == NULL)
		return TRUE;
	pItem->GetWindowRect (&rc);
	if (PtInRect (&rc, pHelpInfo->MousePos) == FALSE)
	{
		pHelpInfo->MousePos.x = rc.left + (rc.right - rc.left) / 2;
		pHelpInfo->MousePos.y = rc.top + (rc.bottom - rc.top) / 2;
	}
	PrepareCHMgr (pHelpInfo->MousePos);
	_CHMgr.OnWT ();
	return TRUE;
}

void CDownloaderProperties_NetworkPage::OnChangeMaxdldsval() 
{
	if (m_bIModifing == FALSE)
		SetModified ();	
}
