/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "Web Interface.h"
#include "Web InterfaceDlg.h"
#include <winsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CWebInterfaceApp, CWinApp)
	//{{AFX_MSG_MAP(CWebInterfaceApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CWebInterfaceApp::CWebInterfaceApp() : 
	m_si (_T("FDM - remote control server"))
{
}

CWebInterfaceApp theApp;

BOOL CWebInterfaceApp::InitInstance()
{
	if (m_si.is_1stInstance () == FALSE)
		return FALSE;

	TCHAR sz [MAX_PATH];
	GetModuleFileName (NULL, sz, MAX_PATH);
	*_tcsrchr (sz, _T('\\')) = 0;
	SetCurrentDirectory (sz);

	AfxEnableControlContainer();

	

#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif

	SetRegistryKey (_T("FreeDownloadManager.ORG"));

	CoInitialize (NULL);

	WORD w = MAKEWORD (1,1);
	WSADATA data;
	::WSAStartup (w, &data);

	CWebInterfaceDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	
	
	return FALSE;
}

int CWebInterfaceApp::ExitInstance() 
{
	WSACleanup ();

	CoUninitialize ();
	
	return CWinApp::ExitInstance();
}
