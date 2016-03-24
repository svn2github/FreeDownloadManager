/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsRegisteredApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsRegisteredApp::vmsRegisteredApp()
{

}

vmsRegisteredApp::~vmsRegisteredApp()
{

}

CString vmsRegisteredApp::GetFullPath(LPCTSTR pszExeName, bool bUser)
{
	CRegKey key;
	CString str = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\");
	HKEY hkey = HKEY_LOCAL_MACHINE;
	if ( bUser )
		hkey = HKEY_CURRENT_USER;
	str += pszExeName;
	if (ERROR_SUCCESS == key.Open (hkey, str, KEY_READ))
	{
		TCHAR szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath) * sizeof(TCHAR);

		if (ERROR_SUCCESS == key.QueryValue (szPath, _T("Path"), &dw))
		{
			str = szPath;
			if (str [str.GetLength () - 1] != _T('\\') && str [str.GetLength () - 1] != _T('/'))
				str += _T('\\');
			str += pszExeName;
			return str;
		}

		if (ERROR_SUCCESS == key.QueryValue (szPath, NULL, &dw))
		{
			str = szPath;
			if (str != _T("") && str [0] == _T('"'))
			{
				str.Delete (0);
				str.Delete (str.GetLength () - 1);
			}
			return str;
		}
	}

	return _T("");
}

CString vmsRegisteredApp::GetFullPath2(LPCTSTR pszExeName)
{
	auto result = GetFullPath (pszExeName, true);
	if (result.IsEmpty ())
		result = GetFullPath (pszExeName, false);
	return result;
}