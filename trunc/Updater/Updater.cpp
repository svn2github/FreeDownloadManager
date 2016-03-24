/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include <tchar.h>
#include <string>

HANDLE StartProcess (LPCTSTR pszName, LPCTSTR pszArgs)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory (&si, sizeof (si));
	si.cb = sizeof (si);
	ZeroMemory (&pi, sizeof (pi));

	TCHAR sz [MAX_PATH] = _T("\"");
	lstrcat (sz, pszName);
	lstrcat (sz, _T("\""));
	
	if (pszArgs && *pszArgs)
	{
		lstrcat (sz, _T(" "));
		lstrcat (sz, pszArgs);
	}

	if (FALSE == CreateProcess (NULL, sz, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		return NULL;

	return pi.hProcess;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	HANDLE hMutex;
	DWORD dwLastError;

	

	if (*lpCmdLine++ != _T('"'))
		return 0;

	tstring strProcessExe, strAppMutex, strUpgradeExe, strUpgradeExeArgs,
		strDeleteDistribFlags;

	while (*lpCmdLine && *lpCmdLine != _T('"'))
		strProcessExe += *lpCmdLine++;
	if (*lpCmdLine == 0)
		return 0;
	lpCmdLine += 2;

	if (*lpCmdLine++ != _T('"'))
		return 0;
	while (*lpCmdLine && *lpCmdLine != _T('"'))
		strAppMutex += *lpCmdLine++;
	if (*lpCmdLine == 0)
		return 0;
	lpCmdLine += 2;

	if (*lpCmdLine++ != _T('"'))
		return 0;
	while (*lpCmdLine && *lpCmdLine != _T('"'))
		strUpgradeExe += *lpCmdLine++;
	if (*lpCmdLine == 0)
		return 0;
	lpCmdLine += 2;

	if (*lpCmdLine++ != _T('"'))
		return 0;
	while (*lpCmdLine && *lpCmdLine != _T('"'))
		strUpgradeExeArgs += *lpCmdLine++;
	if (*lpCmdLine == 0)
		return 0;
	lpCmdLine += 2;

	if (*lpCmdLine++ != _T('"'))
		return 0;
	while (*lpCmdLine && *lpCmdLine != _T('"'))
		strDeleteDistribFlags += *lpCmdLine++;
	
	if (strProcessExe == _T("") || strAppMutex == _T("") || strUpgradeExe == _T(""))
		return 0;

	
	do
	{
		hMutex = CreateMutex (NULL, TRUE, strAppMutex.c_str ());
	
		dwLastError = GetLastError ();
		CloseHandle (hMutex);

		Sleep (300);
	}
	while (dwLastError == ERROR_ALREADY_EXISTS);

	
	
	HANDLE hUpdate = StartProcess (strUpgradeExe.c_str (), strUpgradeExeArgs.c_str ());

	if (hUpdate == NULL)
		MessageBox (NULL, _T("Can't find upgrade distributive file."), _T("Update error"), MB_ICONERROR);
	else
		WaitForSingleObject (hUpdate, INFINITE);

	if (strDeleteDistribFlags == _T("1"))
		DeleteFile (strUpgradeExe.c_str ());

	
	StartProcess (strProcessExe.c_str (), NULL);

	return 0;
}

