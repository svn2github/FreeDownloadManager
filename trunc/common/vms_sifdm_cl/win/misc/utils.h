/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../../../vms_sifdm_cl/win/vmsWinRect.h"
#include "../vmsProcessList.h"

inline bool vmsIsBrowserRunning (bool *pbIsForeground)
{
	*pbIsForeground = false;

	LPCTSTR pptszBrowserWndClasses [] = {
		_T ("IEFrame"), 
		_T ("MozillaUIWindowClass"), 
		_T ("OpWindow"), 
		_T ("Chrome_WidgetWin_0"),  
	};

	bool bRunning = false;

	HWND hwndActive = GetForegroundWindow ();

	for (int i = 0; i < sizeof (pptszBrowserWndClasses) / sizeof (LPCTSTR); i++)
	{
		HWND hwnd = NULL;
		while (hwnd = FindWindowEx (NULL, hwnd, pptszBrowserWndClasses [i], NULL))
		{
			bRunning = true; 
			if (hwndActive == hwnd)
			{
				*pbIsForeground = true;
				return true;
			}
		}
	}

	if (bRunning)
		return true;

	LPCTSTR pptszBrowserProcesses [] = {
		_T ("iexplore.exe"), 
		_T ("firefox.exe"), 
		_T ("opera.exe"), 
		_T ("chrome.exe"),
	};

	vmsProcessList pl;
	pl.GenerateList ();

	DWORD dwActiveWndPid = 0;
	GetWindowThreadProcessId (hwndActive, &dwActiveWndPid);

	for (int i = 0; i < sizeof (pptszBrowserProcesses) / sizeof (LPCTSTR) + 1; i++)
	{
		LPCTSTR ptszPName = NULL;
		vmsCommandLine cl;

		if (i < sizeof (pptszBrowserProcesses) / sizeof (LPCTSTR))
		{
			ptszPName = pptszBrowserProcesses [i];
		}
		else
		{
			
			CRegKey key; 
			if (ERROR_SUCCESS == key.Open (HKEY_CLASSES_ROOT, _T ("http\\shell\\open\\command"), KEY_READ))
			{
				TCHAR tsz [MAX_PATH] = _T (""); 
				DWORD dw = _countof (tsz);
				key.QueryStringValue (NULL, tsz, &dw);
				if (*tsz)
				{
					cl.fromString (tsz);
					ptszPName = cl.getExeName ();
				}
			}
		}

		if (!ptszPName || !*ptszPName)
			break;

		int nProcess = pl.findProcessIndex (ptszPName);
		if (nProcess == -1)
			continue;

		bRunning = true;

		if (pl.getProcess (nProcess)->dwProcessId == dwActiveWndPid)
		{
			*pbIsForeground = true;
			return true;
		}
	}

	if (bRunning)		
		return true;

	return false;
}

inline bool vmsIsInFullScreenMode ()
{
	HWND hwnd = GetForegroundWindow ();
	if (hwnd != NULL && hwnd != GetDesktopWindow ())
	{
		vmsWinRect rc; GetWindowRect (hwnd, &rc);
		if (rc.Width () == GetSystemMetrics (SM_CXSCREEN) &&
			rc.Height () == GetSystemMetrics (SM_CYSCREEN))
		{
			TCHAR tszClass [1000] = _T ("");
			GetClassName (hwnd, tszClass, 1000);
			return _tcsicmp (tszClass, _T ("progman")) != 0;
		}	
	}
	return false;
}

inline void vmsShowMainWindow (HWND hwnd)
{
	if (IsIconic (hwnd))
	{
		ShowWindow (hwnd, SW_RESTORE);
	}
	else if (!IsWindowVisible (hwnd))
	{
		WINDOWPLACEMENT wp; 
		wp.length = sizeof (wp);
		if (GetWindowPlacement (hwnd, &wp))
		{
			assert (wp.showCmd != SW_HIDE);
			SetWindowPlacement (hwnd, &wp);
		}
		else
		{
			
			ShowWindow (hwnd, SW_SHOW);
		}
	}
	SetForegroundWindow (hwnd);
}

inline void vmsShowHideMainWindow (HWND hwnd)
{
	if (IsIconic (hwnd) || !IsWindowVisible (hwnd))
		vmsShowMainWindow (hwnd);
	else
		ShowWindow (hwnd, SW_HIDE);
}

inline void vmsShowHideMainWindow2 (HWND hwnd)
{
	

	RECT rcWnd;
	GetWindowRect (hwnd, &rcWnd);

	const bool target_topmost = (GetWindowLong (hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;

	auto wndPrev = hwnd;
	while ((wndPrev = GetWindow (wndPrev, GW_HWNDPREV)))
	{
		if (!IsWindowVisible (wndPrev))
			continue;

		if (!target_topmost && (GetWindowLong (wndPrev, GWL_EXSTYLE) & WS_EX_TOPMOST))
			break;

		RECT rcWndPrev;
		GetWindowRect (wndPrev, &rcWndPrev);

		RECT rc;
		if (IntersectRect (&rc, &rcWnd, &rcWndPrev))
		{
			
			vmsShowMainWindow (hwnd);
			return;
		}
	}

	vmsShowHideMainWindow (hwnd);
}

inline void vmsOpenInWindowsExplorer (const std::wstring& wstrPath)
{
	assert (!wstrPath.empty ());
	if (wstrPath.empty ())
		return;

	DWORD dwAttributes = GetFileAttributes (wstrPath.c_str ());

	if (dwAttributes == DWORD (-1))
	{
		WCHAR wszPath [MAX_PATH] = L"";
		vmsGetPath (wstrPath.c_str (), wszPath);
		assert (*wszPath);
		ShellExecute (NULL, L"explore", wszPath, NULL, NULL, SW_SHOW);
	}
	else if (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ShellExecute (NULL, L"explore", wstrPath.c_str (), NULL, NULL, SW_SHOW);
	}
	else
	{
		std::wstringstream wss;
		wss << L"/select,\"" << wstrPath << L"\"";
		ShellExecute (NULL, L"open", L"explorer.exe", wss.str ().c_str (), NULL, SW_SHOW);
	}
}