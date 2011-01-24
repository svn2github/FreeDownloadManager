/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "fdm.h"
#include "Dlg_Options_Downloads_Network.h"
#include "vmsDialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif        

CDlg_Options_Downloads_Network::CDlg_Options_Downloads_Network(CWnd* pParent )
	: CDlg_Options_Page(CDlg_Options_Downloads_Network::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Options_Downloads_Network)
		
	//}}AFX_DATA_INIT
	m_bIModifing = FALSE;
}  

void CDlg_Options_Downloads_Network::DoDataExchange(CDataExchange* pDX)
{
	CDlg_Options_Page::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Options_Downloads_Network)
	DDX_Control(pDX, IDC_TRAFFICLIMITDIMENSION, m_wndTLD);
	DDX_Control(pDX, IDC_MODE, m_wndMode);
	//}}AFX_DATA_MAP
}  

BEGIN_MESSAGE_MAP(CDlg_Options_Downloads_Network, CDlg_Options_Page)
	//{{AFX_MSG_MAP(CDlg_Options_Downloads_Network)
	ON_CBN_SELCHANGE(IDC_MODE, OnSelchangeMode)
	ON_BN_CLICKED(IDC_USEMAXCONNS, OnUsemaxconns)
	ON_BN_CLICKED(IDC_USEMAXCONNSPERSERVER, OnUsemaxconnsperserver)
	ON_BN_CLICKED(IDC_USEMAXDLDS, OnUsemaxdlds)
	ON_BN_CLICKED(IDC_DETECTACTIVITY, OnDetectactivity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDlg_Options_Downloads_Network::OnInitDialog() 
{
	CDlg_Options_Page::OnInitDialog();
	
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

CString CDlg_Options_Downloads_Network::get_PageTitle()
{
	return LS (L_NETWORK);
}

CString CDlg_Options_Downloads_Network::get_PageShortTitle()
{
	return LS (L_NETWORK);
}

BOOL CDlg_Options_Downloads_Network::Apply()
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

	return TRUE;
}

BOOL CDlg_Options_Downloads_Network::ReadTUS()
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
	else
	{
		tum.uTrafficLimit = UINT_MAX;
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

void CDlg_Options_Downloads_Network::WriteTUS()
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

void CDlg_Options_Downloads_Network::UpdateEnabled()
{
	BOOL b;
	
	b = IsDlgButtonChecked (IDC_USEMAXCONNS) == BST_CHECKED;
	GetDlgItem (IDC_MAXCONNS)->EnableWindow (b);
	GetDlgItem (IDC_MAXCONNSSPIN)->EnableWindow (b);
	
	b = IsDlgButtonChecked (IDC_USEMAXCONNSPERSERVER) == BST_CHECKED;
	GetDlgItem (IDC_MAXCONNSPERSERVER)->EnableWindow (b);
	GetDlgItem (IDC_MAXCONNSPERSERVERSPIN)->EnableWindow (b);
	
	b = IsDlgButtonChecked (IDC_USEMAXDLDS) == BST_CHECKED;
	GetDlgItem (IDC_MAXDLDSVAL)->EnableWindow (b);
	GetDlgItem (IDC_MAXDLDSSPIN)->EnableWindow (b);
	
	b = m_iCurMode != TUM_HEAVY;
	GetDlgItem (IDC__TUR)->EnableWindow (b);
	GetDlgItem (IDC_TRAFFICLIMITVAL)->EnableWindow (b);
	GetDlgItem (IDC_TRAFFICLIMITDIMENSION)->EnableWindow (b);
	
	b = IsDlgButtonChecked (IDC_DETECTACTIVITY) == BST_CHECKED;
	GetDlgItem (IDC_DECBYLEVEL)->EnableWindow (b);
	GetDlgItem (IDC_SETTOLIGHT)->EnableWindow (b);
}

void CDlg_Options_Downloads_Network::OnSelchangeMode() 
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

void CDlg_Options_Downloads_Network::ApplyLanguage()
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

void CDlg_Options_Downloads_Network::OnUsemaxconns() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Network::OnUsemaxconnsperserver() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Network::OnUsemaxdlds() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Network::OnDetectactivity() 
{
	UpdateEnabled ();	
}
