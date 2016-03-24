/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include <winternl.h>

class vmsWinUtil
{
public:

	static tstring GetRegisteredExePath (LPCTSTR ptszExeName)
	{
		CRegKey key;
		tstring tstr = _T ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\");
		tstr += ptszExeName;
		if (ERROR_SUCCESS != key.Open (HKEY_LOCAL_MACHINE, tstr.c_str (), KEY_READ))
		{
			if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, tstr.c_str (), KEY_READ))
				return _T ("");
		}
		TCHAR tsz [MAX_PATH] = _T ("");
		DWORD dw = _countof (tsz);
		key.QueryStringValue (_T ("Path"), tsz, &dw);
		return tsz;
	}

	static bool GetProcessCommandLine (DWORD dwPID, LPWSTR pwszCmdLine, UINT *puLen)
	{
		if (!pwszCmdLine)
			return false;

		*pwszCmdLine = 0;

		typedef NTSTATUS (NTAPI *FNNtQueryInformationProcess) (HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation,
			ULONG ProcessInformationLength, PULONG ReturnLength);

		FNNtQueryInformationProcess pfnQIP = (FNNtQueryInformationProcess) GetProcAddress (
			GetModuleHandle (_T ("ntdll.dll")), "NtQueryInformationProcess");
		if (!pfnQIP)
			return false;

		HANDLE hProcess = OpenProcess (PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
		if (hProcess == NULL)
			return false;

		PROCESS_BASIC_INFORMATION pbi;
		PEB peb;
		if (pfnQIP (hProcess, ProcessBasicInformation, &pbi, sizeof (pbi), NULL))
		{
			CloseHandle (hProcess);
			return false;
		}

		SIZE_T nSize = 0;

		if (!ReadProcessMemory (hProcess, pbi.PebBaseAddress, &peb, sizeof (peb), &nSize))
		{
			CloseHandle (hProcess);
			return false;
		}

		RTL_USER_PROCESS_PARAMETERS upp;
		if (!ReadProcessMemory (hProcess, peb.ProcessParameters, &upp, sizeof (upp), &nSize))
		{
			CloseHandle (hProcess);
			return false;
		}

		if (*puLen * 2 <= upp.CommandLine.Length)
		{
			*puLen = upp.CommandLine.Length / 2 + 1;
			CloseHandle (hProcess);
			return false;
		}

		if (!ReadProcessMemory (hProcess, upp.CommandLine.Buffer, pwszCmdLine, upp.CommandLine.Length, &nSize))
		{
			CloseHandle (hProcess);
			return false;
		}

		pwszCmdLine [nSize / 2] = 0;

		CloseHandle (hProcess);

		return *pwszCmdLine != 0;
	}

	static DWORD GetModuleFileNameEx (HANDLE hProcess, HMODULE hModule, LPTSTR ptszFileName, DWORD nSize)
	{
		typedef DWORD (WINAPI *FNGMFNEX)(HANDLE, HMODULE, LPTSTR, DWORD);
		HMODULE hDll = LoadLibrary (_T ("Psapi.dll"));
		if (hDll == NULL)
			return 0;

#if defined (UNICODE) || defined (_UNICODE)
		FNGMFNEX pfn = (FNGMFNEX) GetProcAddress (hDll, "GetModuleFileNameExW");
#else
		FNGMFNEX pfn = (FNGMFNEX) GetProcAddress (hDll, "GetModuleFileNameExA");
#endif

		DWORD dwRet = 0;

		if (pfn)
			dwRet = pfn (hProcess, hModule, ptszFileName, nSize);

		FreeLibrary (hDll);

		return dwRet;
	}

	
	static bool ExtractIconAndId(tstring& tstrIcon, int* piIconIndex)
	{
		if (tstrIcon.empty ())
			return false;

		if (!tstrIcon.empty () && tstrIcon [0] == '"')
		{
			tstrIcon.erase (tstrIcon.begin ());
			size_t n = tstrIcon.find ('"');
			if (n < tstrIcon.length ())
				tstrIcon.erase (tstrIcon.begin () + n);
		}

		LPCTSTR ptsz = _tcsrchr (tstrIcon.c_str (), ',');
		if (ptsz)
		{
			LPCTSTR ptsz2 = ptsz + 1;
			if (*ptsz2 == '-')
				ptsz2++;
			while (*ptsz2 && _istdigit (*ptsz2))
				ptsz2++;
			if (*ptsz2 == 0)
			{
				if (piIconIndex)
					*piIconIndex = _ttoi (ptsz + 1);
				tstrIcon.erase (tstrIcon.end () - (ptsz2 - ptsz), tstrIcon.end ());
				return true; 
			}
		}

		return false; 
	}

	static BOOL GetFileTime (LPCTSTR ptszFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
	{
		if (lpCreationTime)
			ZeroMemory (lpCreationTime, sizeof (FILETIME));
		if (lpLastAccessTime)
			ZeroMemory (lpLastAccessTime, sizeof (FILETIME));
		if (lpLastWriteTime)
			ZeroMemory (lpLastWriteTime, sizeof (FILETIME));
		HANDLE hFile = CreateFile (ptszFile, GENERIC_READ, 
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return FALSE;
		BOOL bOK = ::GetFileTime (hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
		CloseHandle (hFile);
		return bOK;
	}

	static BOOL IsWow64()
	{
#ifdef _WIN64
		return FALSE;
#else
		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE hProcess,PBOOL Wow64Process);
		LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)
			GetProcAddress (GetModuleHandle (_T ("kernel32")), "IsWow64Process");
		BOOL bIsWow64 = FALSE;

		if (NULL != fnIsWow64Process)
		{
			if (!fnIsWow64Process (GetCurrentProcess (), &bIsWow64))
			{
				
			}
		}

		return bIsWow64;
#endif
	}
};