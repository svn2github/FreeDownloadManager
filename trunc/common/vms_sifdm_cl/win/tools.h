/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsCommandLine.h"
#include "../stringh.h"
#include "security/vmsWinSecurity.h"

inline std::wstring vmsGetModuleFileName (HMODULE hMod = NULL)
{
	wchar_t wszPath [MAX_PATH] = L"";
	GetModuleFileName (hMod, wszPath, _countof (wszPath));
	return wszPath;
}

inline std::wstring vmsGetModulePath (HMODULE hMod = NULL)
{
	wchar_t wszPath [MAX_PATH] = L"";
	GetModuleFileName (hMod, wszPath, _countof (wszPath));

	wchar_t *pwsz = wcsrchr (wszPath, '\\');
	assert (pwsz);
	if (pwsz)
		*pwsz = 0;

	return wszPath;
}

inline void vmsOpenUrlInBrowser (LPCTSTR pszUrl)
{
	vmsCommandLine cl;
	tstring tstrArgs;
	TCHAR szReg[MAX_PATH] = _T("");
	TCHAR tszCmdLine[MAX_PATH] = _T("");
	DWORD dwLen;
	HKEY hReg;

	TCHAR protocol[10];
	if (_tcsnicmp(pszUrl, _T("http://"), 7) == 0)
		_tcscpy(protocol, _T("http"));
	else if (_tcsnicmp(pszUrl, _T("https://"), 8) == 0)
		_tcscpy(protocol, _T("https"));
	else if (_tcsnicmp(pszUrl, _T("ftp://"), 6) == 0)
		_tcscpy(protocol, _T("ftp"));
	else
		return;
	
	

	
	
	vmsWinOsVersion osver;
	if (osver.isVistaOrHigher())
	{
		_tcscat(szReg, _T("Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\"));
		_tcscat(szReg, protocol);
		_tcscat(szReg, _T("\\UserChoice"));
		
		TCHAR tszProgId[MAX_PATH] = _T("");

		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, szReg, 0, KEY_QUERY_VALUE, &hReg))
			goto _lErr;

		dwLen = _countof(tszProgId) - 1;
		if (ERROR_SUCCESS != RegQueryValueEx(hReg, _T("ProgId"), NULL, NULL, (LPBYTE)tszProgId, &dwLen))
		{
			RegCloseKey(hReg);
			goto _lErr;
		}

		tszProgId[dwLen] = 0;
		RegCloseKey(hReg);

		if (*tszProgId == 0)
			goto _lErr;

		_tcscpy(szReg, tszProgId);
	}
	else
	{
		_tcscpy(szReg, protocol);
	}
	
	_tcscat(szReg, _T("\\shell\\open\\command"));

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT, szReg, 0, KEY_QUERY_VALUE, &hReg))
		goto _lErr;

	dwLen = _countof(tszCmdLine) - 1;
	if (ERROR_SUCCESS != RegQueryValueEx(hReg, NULL, NULL, NULL, (LPBYTE)tszCmdLine, &dwLen))
	{
		RegCloseKey(hReg);
		goto _lErr;
	}

	tszCmdLine [dwLen] = 0;

	RegCloseKey (hReg);

	if (*tszCmdLine == 0)
		goto _lErr;

	cl.fromString (tszCmdLine);

	
	_tcslwr (tszCmdLine);
	if (!_tcsstr (tszCmdLine, _T (".exe")))
		goto _lErr;

	tstrArgs = cl.getArgs ();
	if (tstrArgs.find (_T ("%1")) != tstring::npos)
	{
		tstringReplace (tstrArgs, _T ("%1"), pszUrl);
	}
	else
	{
		tstrArgs += _T (" \"");
		tstrArgs += pszUrl;
		tstrArgs += _T ("\"");
	}

	cl.setArgs (tstrArgs.c_str ());

	if (vmsWinSecurity::os_supports_elevation () &&
			vmsWinSecurity::IsProcessElevated ())
	{
		STARTUPINFO si = {0}; PROCESS_INFORMATION pi = {0};
		std::wstring cmdline = cl.toString ();
		cmdline.push_back (0);
		if (vmsWinSecurity::RunAsDesktopUser (nullptr, &cmdline.front (), nullptr, si, pi))
			return;
	}

	if (32 >= (int)ShellExecute (HWND_DESKTOP, _T ("open"), cl.getExe (), tstrArgs.c_str (), NULL, SW_SHOW))
		goto _lErr;

	return;

_lErr:
	ShellExecute (HWND_DESKTOP, _T ("open"), pszUrl, NULL, NULL, SW_SHOW);
}

inline HMONITOR vmsGetPrimaryMonitorHandle ()
{
	const POINT ptZero = {0, 0};
	return MonitorFromPoint (ptZero, MONITOR_DEFAULTTOPRIMARY);
}