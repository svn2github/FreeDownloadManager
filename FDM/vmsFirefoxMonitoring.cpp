/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsFirefoxMonitoring.h"
#include "vmsFirefoxExtensionInstaller.h"

#define FLASHGOT_CID	"{19503e42-ca3c-4c27-b1e2-9cdb2170ee34}"
#define FDM_CID			"fdm_ffext@freedownloadmanager.org"  

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsFirefoxMonitoring::vmsFirefoxMonitoring()
{

}

vmsFirefoxMonitoring::~vmsFirefoxMonitoring()
{

}

bool vmsFirefoxMonitoring::IsInstalled()
{
	return vmsFirefoxExtensionInstaller::IsInstalled (FDM_CID, true);
}

bool vmsFirefoxMonitoring::IsFlashGotInstalled()
{
	return vmsFirefoxExtensionInstaller::IsInstalled (FLASHGOT_CID, true);
}

bool vmsFirefoxMonitoring::Install(bool bInstall)
{
	CString strPath = ((CFdmApp*)AfxGetApp ())->m_strAppPath;
	strPath += "Firefox\\Extension";
	return vmsFirefoxExtensionInstaller::Do (FDM_CID, strPath, bInstall);
}

