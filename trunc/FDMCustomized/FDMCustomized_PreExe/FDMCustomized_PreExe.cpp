/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include <stdio.h>
#include <objbase.h>
#include <shlobj.h>

vmsTmpDirName g_tmpDir (true, _T("{119D1570-8853-4FC9-9343-FA5C72E55D20}"));
tstring g_instName;
tstring g_postVersion;
DWORD _dwFlags;

HKEY _hFDMKey;
tstring g_custSite;

char LABEL [100];

void BuildLABEL ()
{
	strcpy (LABEL, "###");
	strcat (LABEL, "FDMDATA");
	strcat (LABEL, "###");
}

LPCSTR bufbuf (LPCSTR where, LPCSTR what, int maxpos)
{
	int len = strlen (what);
	while (memcmp (where, what, len))
	{
		where ++;
		if (--maxpos == 0)
			break;
	}

	return maxpos == 0 ? NULL : where;
}

BOOL CreateInstaller (LPCTSTR pszFile, LPCVOID pszStart, DWORD dwSize)
{
	if (FALSE == vmsBuildPathToFile (pszFile))
		return FALSE;

	HANDLE hFile = CreateFile (pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwWritten = 0;

	if (FALSE == WriteFile (hFile, pszStart, dwSize, &dwWritten, NULL) ||
		dwWritten != dwSize)
	{
		CloseHandle (hFile);
		return FALSE;
	}

	CloseHandle (hFile);

	return TRUE;
}

BOOL AddURLToDownload (LPCTSTR pszUrl, LPCTSTR pszReferer, LPCTSTR pszComment, BOOL bAutoStart)
{
	DWORD dwErr;
	HKEY key;
	static int _iKey = 0;

	TCHAR szKey [1000];
	_stprintf (szKey, _T("URLsToDownload\\FdmSetupInstaller_%d"), _iKey++);

	dwErr = RegCreateKey (_hFDMKey, szKey, &key);

	if (dwErr != ERROR_SUCCESS && dwErr)
		return FALSE;

	RegSetValueEx (key, _T("URL"), NULL, REG_SZ, LPBYTE (pszUrl), (lstrlen (pszUrl)+1)*sizeof (TCHAR));
	RegSetValueEx (key, _T("Referer"), NULL, REG_SZ, LPBYTE (pszReferer), (lstrlen (pszReferer)+1)*sizeof (TCHAR));
	RegSetValueEx (key, _T("Comment"), NULL, REG_SZ, LPBYTE (pszComment), (lstrlen (pszComment)+1)*sizeof (TCHAR));
	DWORD dw = TRUE;
	RegSetValueEx (key, _T("Silent"), NULL, REG_DWORD, (LPBYTE)&dw, sizeof (dw));
	RegSetValueEx (key, _T("zlr"), 0, REG_DWORD, (LPBYTE)&dw, sizeof (dw));
	
	if (bAutoStart)
		dw = TRUE;
	else
		dw = FALSE;

	RegSetValueEx (key, _T("AutoStart"), NULL, REG_DWORD, (LPBYTE)&dw, sizeof (dw));
	
	RegCloseKey (key);

	return TRUE;
}

LPCSTR DoJob_ExtractInstaller (LPCSTR psz)
{
	g_instName = (LPCTSTR)g_tmpDir;

	{
		wchar_t wszPath [MAX_PATH] = L"";
		GetModuleFileName (NULL, wszPath, _countof (wszPath));
		LPCWSTR pwsz = wcsrchr (wszPath, '\\');
		assert (pwsz);
		g_instName += pwsz ? pwsz : _T("\\fdminst.exe");
	}

	DWORD dwSize = *(LPDWORD(psz));
	psz += sizeof (DWORD);

	if (FALSE == CreateInstaller (g_instName.c_str (), psz, dwSize))
	{
		DeleteFile (g_instName.c_str ());
		return NULL;
	}

	return psz + dwSize;
}

#define FC_MODIFYHOMEPAGE		1L
#define FC_ADDLINKTOFAVOR		(1L << 1)
#define FC_ADDLINKTOSTARTMENU	(1L << 2)
#define FC_ADDBUTTONTOIE		(1L << 3)
#define FC_ADDDOWNLOADS			(1L << 4)
#define FC_ADDBANNERS			(1L << 5)

#define FC_MHP_CHECKEDBYDEF		(1L << 6)

#define FC_FAV_OPTIONAL			(1L << 7)
#define FC_FAV_CHECKEDBYDEF		(1L << 8)

#define FC_SM_OPTIONAL			(1L << 9)
#define FC_SM_CHECKEDBYDEF		(1L << 10)

#define FC_IEBTN_OPTIONAL		(1L << 11)
#define FC_IEBTN_CHECKEDBYDEF	(1L << 12)

LPCSTR DoJob_SetupInstallerRegistryParameters (LPCSTR psz)
{
	DWORD dwSize = *(LPDWORD(psz));
	psz += sizeof (DWORD);

	char sz [10000];
	CopyMemory (sz, psz, dwSize);
	sz [dwSize] = 0;
	psz += dwSize;
	g_postVersion = wideFromUtf8 (sz);

	DWORD dwErr;
	dwErr = RegCreateKey (HKEY_CURRENT_USER, 
				_T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), 
				&_hFDMKey);

	if (dwErr == ERROR_SUCCESS)
		RegSetValueEx (_hFDMKey, _T("PostVersion"), NULL, REG_SZ, LPBYTE (g_postVersion.c_str ()), (g_postVersion.length ()+1)*sizeof(TCHAR));

	tstring customizer;
	
	{
		char szCustomizer [1000];
		dwSize = *((LPDWORD) psz);
		psz += sizeof (DWORD);
		CopyMemory (szCustomizer, psz, dwSize);
		szCustomizer [dwSize] = 0;
		psz += dwSize;
		customizer = wideFromUtf8 (szCustomizer);
	}

	{
		char sz [10000];
		dwSize = *((LPDWORD) psz);
		psz += sizeof (DWORD);
		CopyMemory (sz, psz, dwSize);
		sz [dwSize] = 0;
		psz += dwSize;
		g_custSite = wideFromUtf8 (sz);
	}

	RegSetValueEx (_hFDMKey, _T("Customizer"), NULL, REG_SZ, LPBYTE (customizer.c_str ()), (customizer.length ()+1)*sizeof(TCHAR));
	RegSetValueEx (_hFDMKey, _T("CustSite"), NULL, REG_SZ, LPBYTE (g_custSite.c_str ()), (g_custSite.length ()+1)*sizeof(TCHAR));

	_dwFlags = *((LPDWORD) psz);
	psz += sizeof (DWORD);

	DWORD dw = 1;

	if (_dwFlags & (FC_ADDLINKTOFAVOR | FC_ADDLINKTOSTARTMENU))
	{
		if (_dwFlags & FC_ADDLINKTOFAVOR)
		{
			if (_dwFlags & FC_FAV_OPTIONAL)
			{
				dw = 2;
				if (_dwFlags & FC_FAV_CHECKEDBYDEF)
					dw = 3;
			}
			RegSetValueEx (_hFDMKey, _T("CreateLFM"), 0, REG_DWORD, (LPBYTE)&dw, 4);
			dw = 1;
		}

		if (_dwFlags & FC_ADDLINKTOSTARTMENU)
		{
			if (_dwFlags & FC_SM_OPTIONAL)
			{
				dw = 2;
				if (_dwFlags & FC_SM_CHECKEDBYDEF)
					dw = 3;
			}
			RegSetValueEx (_hFDMKey, _T("CreateLSM"), 0, REG_DWORD, (LPBYTE)&dw, 4);
			dw = 1;
		}
	}

	if (_dwFlags & FC_MODIFYHOMEPAGE)
	{
		dw = 2;
		if (_dwFlags & FC_MHP_CHECKEDBYDEF)
			dw = 3;
		RegSetValueEx (_hFDMKey, _T("UseHPage"), 0, REG_DWORD, (LPBYTE)&dw, sizeof (dw));
		RegSetValueEx (_hFDMKey, _T("HPageTo"), 0, REG_SZ, (LPBYTE)g_custSite.c_str (), (g_custSite.length ()+1)*sizeof(TCHAR));
		dw = 1;
	}
	
	if (_dwFlags & FC_ADDBUTTONTOIE)
	{
		if (_dwFlags & FC_IEBTN_OPTIONAL)
		{
			dw = 2;
			if (_dwFlags & FC_IEBTN_CHECKEDBYDEF)
				dw = 3;
		}
		RegSetValueEx (_hFDMKey, _T("IEBtn"), 0, REG_DWORD, (LPBYTE)&dw, sizeof (dw));
		dw = 1;
	}

	return psz;
}

LPCSTR ExtractFile (LPCSTR psz, LPCTSTR pszFile, BOOL bDontCreateIfSize0 = FALSE)
{
	DWORD dwSize = *((LPDWORD) psz);
	psz += sizeof (DWORD);
	if (dwSize == 0 && bDontCreateIfSize0) {
		return psz;
	}

	HANDLE hFile;
	do {
		hFile = CreateFile (pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			if (GetLastError () == ERROR_SHARING_VIOLATION) {
				Sleep (100);
				continue;
			}
			return psz;	
		}
	} while (hFile == INVALID_HANDLE_VALUE);

	WriteFile (hFile, psz, dwSize, &dwSize, NULL);

	psz += dwSize;
	CloseHandle (hFile);

	return psz;
}

LPCSTR ExtractFdmCsIcon (LPCSTR psz)
{
	TCHAR szIco [MAX_PATH];
	lstrcpy (szIco, g_tmpDir);
	lstrcat (szIco, _T ("\\fdmcs.ico"));
	psz = ExtractFile (psz, szIco, TRUE);
	return psz;
}

void DoJob_PerformPostInstallIeCustomization ()
{
	if (_dwFlags & (FC_ADDLINKTOFAVOR | FC_ADDLINKTOSTARTMENU))
	{
		
	}

	if (_dwFlags & FC_MODIFYHOMEPAGE)
	{
		DWORD dw = 0;
		DWORD dwSize = sizeof (dw);
		RegQueryValueEx (_hFDMKey, _T("HPageDo"), NULL, NULL, (LPBYTE)&dw, &dwSize);
		if (dw == 0)
			_dwFlags &= ~FC_MODIFYHOMEPAGE;
	}

	if (_dwFlags & FC_MODIFYHOMEPAGE)
	{
		HKEY hKey;
		RegOpenKey (HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), &hKey);

		TCHAR sz [10000] = _T("about:blank");
		DWORD dw = sizeof (sz);

		
		if (ERROR_SUCCESS != RegQueryValueEx (
				_hFDMKey, _T("CIEOP"), NULL, NULL, (LPBYTE)sz, &dw))
		{
			RegQueryValueEx (hKey, _T("Start Page"), NULL, NULL, (LPBYTE)sz, &dw);
			RegSetValueEx (_hFDMKey, _T("CIEOP"), 0, REG_SZ, (LPBYTE)sz, (lstrlen (sz)+1)*sizeof(TCHAR));
		}

		RegSetValueEx (hKey, _T("Start Page"), 0, REG_SZ, (LPBYTE)g_custSite.c_str (), (g_custSite.length ()+1)*sizeof(TCHAR));
		RegCloseKey (hKey);
	}
}

BOOL RegisterDLL (LPCTSTR pszDLL)
{
	typedef HRESULT (_stdcall *fntDllRegUnregServer)(void);
	HMODULE hLib = LoadLibrary (pszDLL);
	if (hLib == NULL)
		return FALSE;

	fntDllRegUnregServer pfnDll;
	pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllRegisterServer");

	if (pfnDll)
		pfnDll ();

	FreeLibrary (hLib);
	return TRUE;
}

LPCSTR ExtractFdmCsIeBtnIfRequired (LPCSTR psz)
{
	if (_dwFlags & FC_ADDBUTTONTOIE)
	{
		TCHAR szFile [MAX_PATH];

		

		lstrcpy (szFile, g_tmpDir);
		lstrcat (szFile, _T("\\fdmcsiebtn.ico"));
		psz = ExtractFile (psz, szFile);
	}

	return psz;
}

LPCSTR DoJob_AddDownloads (LPCSTR psz)
{
	if (_dwFlags & FC_ADDDOWNLOADS)
	{
		DWORD dwDLCount = *((LPDWORD) psz);
		psz += sizeof (DWORD);

		while (dwDLCount--)
		{
			char szURL [10000], szReferer [10000], szComment [10000];
			DWORD dwSize;

			dwSize = *((LPDWORD) psz);
			psz += sizeof (DWORD);
			CopyMemory (szURL, psz, dwSize);
			szURL [dwSize] = 0;
			psz += dwSize;

			dwSize = *((LPDWORD) psz);
			psz += sizeof (DWORD);
			CopyMemory (szReferer, psz, dwSize);
			szReferer [dwSize] = 0;
			psz += dwSize;

			dwSize = *((LPDWORD) psz);
			psz += sizeof (DWORD);
			CopyMemory (szComment, psz, dwSize);
			szComment [dwSize] = 0;
			psz += dwSize;

			BOOL bAutoStart = *((LPBOOL) psz);
			psz += sizeof (BOOL);

			AddURLToDownload (wideFromUtf8 (szURL).c_str (), wideFromUtf8 (szReferer).c_str (),
				wideFromUtf8 (szComment).c_str (), bAutoStart);
		}
	}

	return psz;
}

LPCSTR ExtractFdmCs (LPCSTR psz)
{
	TCHAR szFile [MAX_PATH];
	lstrcpy (szFile, g_tmpDir);
	lstrcat (szFile, _T("\\fdmcs.dat"));
	psz = ExtractFile (psz, szFile);
	return psz;
}

BOOL DoJob (LPCSTR psz, int iBufSize)
{
	LPCSTR pszStart;

	BuildLABEL ();

	SetLastError (ERROR_BAD_ENVIRONMENT);	

	pszStart = bufbuf (psz, LABEL, iBufSize);
	if (pszStart == NULL)
		return FALSE;
	pszStart += strlen (LABEL);

	pszStart = DoJob_ExtractInstaller (pszStart);
	if (NULL == pszStart)
		return FALSE;

	pszStart = DoJob_SetupInstallerRegistryParameters (pszStart);
	if (NULL == pszStart)
		return FALSE;

	pszStart = ExtractFdmCsIcon (pszStart);
	if (NULL == pszStart)
		return FALSE;

	pszStart = ExtractFdmCsIeBtnIfRequired (pszStart);
	if (NULL == pszStart)
		return FALSE;

	pszStart = DoJob_AddDownloads (pszStart);
	if (NULL == pszStart)
		return FALSE;

	pszStart = ExtractFdmCs (pszStart);
	if (NULL == pszStart)
		return FALSE;

	DWORD dwRet;
	vmsCommandLine cl (g_instName, _T(""));
	bool execOK = cl.Execute (
		vmsCommandLine::RunElevatedIfRequired | vmsCommandLine::WaitForCompletion,
		&dwRet);

	DeleteFile (g_instName.c_str ());
	RegDeleteValue (_hFDMKey, _T("PostVersion"));
	RegDeleteValue (_hFDMKey, _T("CreateLFM"));
	RegDeleteValue (_hFDMKey, _T("CreateLSM"));
	RegDeleteValue (_hFDMKey, _T("UseHPage"));
	RegDeleteValue (_hFDMKey, _T("HPageTo"));
	RegDeleteValue (_hFDMKey, _T("IEBtn"));

	if (!execOK)
		return FALSE;
	if (dwRet != 0)
		return TRUE;

	DoJob_PerformPostInstallIeCustomization ();

	return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	TCHAR szMyFileName [10000];
	HANDLE hFile = INVALID_HANDLE_VALUE;

	CoInitialize (NULL);

	if (0 == GetModuleFileName (GetModuleHandle (NULL), szMyFileName, sizeof (szMyFileName)))
		goto _lErr;

	hFile = CreateFile (szMyFileName, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		goto _lErr;

	char *psz;
	DWORD dwFileLen;

	dwFileLen = GetFileSize (hFile, NULL);
	psz = new char [dwFileLen+1];

	if (FALSE == ReadFile (hFile, psz, dwFileLen, &dwFileLen, NULL))
		goto _lErr;

	psz [dwFileLen] = 0; 

	CreateMutex (NULL, FALSE, _T("_mx_FDM_Lock_Start_"));

	if (FALSE == DoJob (psz, dwFileLen))
		goto _lErr;

	delete [] psz;
	CloseHandle (hFile);

	CoUninitialize ();
	return 0;

_lErr:
	CoUninitialize ();

	TCHAR szError [10000];

	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);

	_stprintf (szError, _T("Failed to start FDM installer!\nError code is %d"), 
		GetLastError ());

	MessageBox (NULL, szError, nullptr, MB_ICONERROR);

	return GetLastError ();
}

