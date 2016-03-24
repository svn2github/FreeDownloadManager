/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsFirefoxMonitoring.h"
#include "common/vmsFirefoxExtensionInstaller.h"
#include "vmsAppGlobalObjects.h"
#include "common/vmsFirefoxUtil.h"
#include "vmsFirefoxExtensionInfo.h"
#include "vmsTmpFileName.h"
#include "vmsRegisteredApp.h"

#define FLASHGOT_CID	_T("{19503e42-ca3c-4c27-b1e2-9cdb2170ee34}")
#define FDM_CID			_T("fdm_ffext@freedownloadmanager.org")

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
	return vmsFirefoxExtensionInstaller::IsInstalledUsingRegistry(FDM_CID) ||
		vmsFirefoxExtensionInstaller::IsInstalledInProfile (FDM_CID, true, (LPCTSTR)_App.Firefox_PortableVersionPath ());
}

bool vmsFirefoxMonitoring::IsFlashGotInstalled()
{
	return vmsFirefoxExtensionInstaller::IsInstalledUsingRegistry (FLASHGOT_CID) ||
		vmsFirefoxExtensionInstaller::IsInstalledInProfile (FLASHGOT_CID, true, (LPCTSTR)_App.Firefox_PortableVersionPath ());
}

bool vmsFirefoxMonitoring::Install(bool bInstall)
{
	assert (_spFfExtUpdateMgr != NULL);
	CString strPath;
	if (!_spFfExtUpdateMgr)
	{
		strPath = ((CFdmApp*)AfxGetApp ())->m_strAppPath;
		strPath += "Firefox\\Extension";
	}
	else
	{
		strPath = _spFfExtUpdateMgr->getExtensionPath ();
	}
	
	if ( bInstall )
		return vmsFirefoxExtensionInstaller::DoUsingRegistry (FDM_CID, strPath, bInstall);
	else{
		bool bResult1 = vmsFirefoxExtensionInstaller::DoUsingRegistry (FDM_CID, strPath, bInstall) ;
		bool bResult2 = vmsFirefoxExtensionInstaller::Do (FDM_CID, strPath, 
			(LPCTSTR)_App.Firefox_PortableVersionPath (), bInstall) ;
		return bResult1 && bResult2;
	}
}

bool vmsFirefoxMonitoring::IsEnabledInFirefox(bool &bEnabled)
{
	TCHAR tszPath [MAX_PATH] = _T ("");
	if (!vmsFirefoxUtil::GetDefaultProfilePath (tszPath))
		return false;
	if (!*tszPath)
		return false;

	vmsFirefox26ExtensionInfo fei26;
	if(CheckEnabled(fei26, tszPath, _T ("\\extensions.json"), bEnabled))
		return true;

	vmsFirefoxExtensionInfo fei;
	if(CheckEnabled(fei, tszPath, _T ("\\extensions.sqlite"), bEnabled))
		return true;

	return false;
}

bool vmsFirefoxMonitoring::CheckEnabled(vmsFirefoxExtensionInfo& fei,
		LPCTSTR profile, LPCTSTR file, bool& bEnabled)
{
	bEnabled = false;
	TCHAR tszPath[MAX_PATH];
	_tcsncpy(tszPath, profile, MAX_PATH);
	_tcsncat(tszPath, file, MAX_PATH);

	if (!fei.Read (tszPath, FDM_CID))
	{
		vmsTmpFileName tmpFile;
		if (!CopyFile (tszPath, tmpFile, FALSE))
			return false;
		if (!fei.Read (tmpFile, FDM_CID))
			return false;
	}

	bEnabled = !fei.m_iAppDisabled && !fei.m_iSoftDisabled && !fei.m_iUserDisabled;

	return true;
}

bool vmsFirefoxMonitoring::IsFFInstalled ()
{
	CString str = _App.Firefox_PortableVersionPath ();
	if (!str.IsEmpty ())
	{
		str += _T("\\firefox.exe");
		return GetFileAttributes (str) != DWORD (-1);
	}
	else
	{
		if(vmsRegisteredApp::GetFullPath(_T("firefox.exe")).GetLength() == 0)
			return false;
		return true;
	}
}

bool vmsFirefoxMonitoring::ProfileExists(){
	CString str = _App.Firefox_PortableVersionPath ();
	if (!str.IsEmpty ())
	{
		return true;
	}
	FU_STRINGLIST v; int nDefProf;
	vmsFirefoxUtil::GetProfilesPathes (v, nDefProf);
	return ( v.size() > 0 );
}