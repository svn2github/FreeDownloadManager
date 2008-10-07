/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "fdm.h"
#include "Dlg_Options_Notifications.h"
#include "DlgSounds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif        

CDlg_Options_Notifications::CDlg_Options_Notifications(CWnd* pParent )
	: CDlg_Options_Page(CDlg_Options_Notifications::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Options_Notifications)
		
	//}}AFX_DATA_INIT
}  

void CDlg_Options_Notifications::DoDataExchange(CDataExchange* pDX)
{
	CDlg_Options_Page::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Options_Notifications)
	DDX_Control(pDX, IDC_TIMEOUTSPIN, m_wndTSpin);
	//}}AFX_DATA_MAP
}  

BEGIN_MESSAGE_MAP(CDlg_Options_Notifications, CDlg_Options_Page)
	//{{AFX_MSG_MAP(CDlg_Options_Notifications)
	ON_BN_CLICKED(IDC_USEBALLOON, OnUseballoon)
	ON_BN_CLICKED(IDC_USESOUNDS, OnUsesounds)
	ON_BN_CLICKED(IDC_CUSTOMIZE, OnCustomize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDlg_Options_Notifications::OnInitDialog() 
{
	CDlg_Options_Page::OnInitDialog();
	
	m_wndTSpin.SetRange (1, UD_MAXVAL);
	
	CheckDlgButton (IDC_USEBALLOON, _App.Notif_UseBalloon () ? BST_CHECKED : BST_UNCHECKED);
	SetDlgItemInt (IDC_TIMEOUT, _App.Notif_BalloonTimeout ());
	
	CheckDlgButton (IDC_USESOUNDS, _App.Snd_Use () ? BST_CHECKED : BST_UNCHECKED);
	
	CheckDlgButton (IDC_DISABLEFORBATCHDLDS, _App.Notif_DisableForBatchDownloads () ? BST_CHECKED : BST_UNCHECKED);
	
	ApplyLanguage ();
	UpdateEnabled ();
	
	return TRUE;  
	              
}

CString CDlg_Options_Notifications::get_PageTitle()
{
	return LS (L_NOTIFICATIONS);
}

CString CDlg_Options_Notifications::get_PageShortTitle()
{
	return LS (L_NOTIFICATIONS);
}

BOOL CDlg_Options_Notifications::Apply()
{
	_App.Notif_UseBalloon (IsDlgButtonChecked (IDC_USEBALLOON) == BST_CHECKED);
	_App.Notif_BalloonTimeout (GetDlgItemInt (IDC_TIMEOUT));
	
	_App.Snd_Use (IsDlgButtonChecked (IDC_USESOUNDS) == BST_CHECKED);
	_Snds.ReadSettings ();
	
	_App.Notif_DisableForBatchDownloads (IsDlgButtonChecked (IDC_DISABLEFORBATCHDLDS) == BST_CHECKED);

	return TRUE;
}

void CDlg_Options_Notifications::ApplyLanguage()
{
	fsDlgLngInfo lnginfo [] =  {
		fsDlgLngInfo (IDC_USEBALLOON, L_USEBALLOONS),
		fsDlgLngInfo (IDC__TIMEOUT, L_TIMEOUT),
		fsDlgLngInfo (IDC__SEC, L_SECS),
		fsDlgLngInfo (IDC_USESOUNDS, L_USESOUNDS),
		fsDlgLngInfo (IDC_CUSTOMIZE, L_CUSTOMIZE),
		fsDlgLngInfo (IDC_DISABLEFORBATCHDLDS, L_DISABLENOTIFICATIONSFORBATCHDLDS),
	};
	
	_LngMgr.ApplyLanguage (	this, lnginfo, sizeof (lnginfo) / sizeof (fsDlgLngInfo), 0);
}

void CDlg_Options_Notifications::OnUseballoon() 
{
	if (GetShell32Version () < 5)
	{
		MessageBox (LS (L_AVAILON2000), LS (L_ERR), MB_ICONEXCLAMATION);
		CheckDlgButton (IDC_USEBALLOON, BST_UNCHECKED);
		return;
	}
	
	UpdateEnabled ();	
}

void CDlg_Options_Notifications::UpdateEnabled()
{
	BOOL b = IsDlgButtonChecked (IDC_USEBALLOON) == BST_CHECKED;
	
	GetDlgItem (IDC__TIMEOUT)->EnableWindow (b);
	GetDlgItem (IDC_TIMEOUT)->EnableWindow (b);
	GetDlgItem (IDC_TIMEOUTSPIN)->EnableWindow (b);
	GetDlgItem (IDC__SEC)->EnableWindow (b);
	
	GetDlgItem (IDC_CUSTOMIZE)->EnableWindow (
		IsDlgButtonChecked (IDC_USESOUNDS) == BST_CHECKED);
}

void CDlg_Options_Notifications::OnUsesounds() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Notifications::OnCustomize() 
{
	CDlgSounds dlg;
	_DlgMgr.DoModal (&dlg);	
}
