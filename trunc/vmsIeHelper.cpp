/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "fdm.h"
#include "vmsIeHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsIeHelper::vmsIeHelper()
{

}

vmsIeHelper::~vmsIeHelper()
{

}

bool vmsIeHelper::RegisterExeAsSafeToRun(LPCTSTR ptszAppGuid)
{
	CString strKey = _T ("Software\\Microsoft\\Internet Explorer\\Low Rights\\ElevationPolicy\\");
	strKey += ptszAppGuid;
	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_LOCAL_MACHINE, strKey, KEY_ALL_ACCESS))
	{
		if (ERROR_SUCCESS != key.Create (HKEY_LOCAL_MACHINE, strKey, NULL, 0, KEY_ALL_ACCESS, NULL, NULL))
			return false;
	}

	TCHAR tsz [MY_MAX_PATH] = _T ("");
	GetModuleFileName (NULL, tsz, MY_MAX_PATH);

	LPTSTR ptsz = strrchr (tsz, '\\');
	if (!ptsz)
		return false;

	CString strName = ptsz+1;
	*ptsz = 0;

	bool bOk = ERROR_SUCCESS == key.SetValue (strName, _T ("AppName"));
	bOk = ERROR_SUCCESS == key.SetValue (tsz, _T ("AppPath")) && bOk;
	bOk = ERROR_SUCCESS == key.SetValue (3, _T ("Policy")) && bOk;

	return bOk;
}

bool vmsIeHelper::RegisterExeAsSafeToDragDrop(LPCTSTR ptszAppGuid)
{
	CString strKey = _T ("Software\\Microsoft\\Internet Explorer\\Low Rights\\DragDrop\\");
	strKey += ptszAppGuid;
	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_LOCAL_MACHINE, strKey, KEY_ALL_ACCESS))
	{
		if (ERROR_SUCCESS != key.Create (HKEY_LOCAL_MACHINE, strKey, NULL, 0, KEY_ALL_ACCESS, NULL, NULL))
			return false;
	}
	
	TCHAR tsz [MY_MAX_PATH] = _T ("");
	GetModuleFileName (NULL, tsz, MY_MAX_PATH);
	
	LPTSTR ptsz = strrchr (tsz, '\\');
	if (!ptsz)
		return false;
	
	CString strName = ptsz+1;
	*ptsz = 0;
	
	bool bOk = ERROR_SUCCESS == key.SetValue (strName, _T ("AppName"));
	bOk = ERROR_SUCCESS == key.SetValue (tsz, _T ("AppPath")) && bOk;
	bOk = ERROR_SUCCESS == key.SetValue (3, _T ("Policy")) && bOk;
	
	return bOk;
}
