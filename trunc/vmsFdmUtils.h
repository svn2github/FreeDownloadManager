/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsMagnetExtension.h"
#include <TlHelp32.h>
class vmsFdmUtils
{
public:
	static bool AssociateFdmWithMagnetLinks (bool bAssociate)
	{
		if (!bAssociate && vmsMagnetExtension::IsAssociatedWithUs ())
		{
			vmsMagnetExtension::AssociationParameters ap;
			ap.tstrShellOpenCmdLine = _App.Bittorrent_OldMagnetAssociation ();
			ap.tstrUrlProtocol = _App.Bittorrent_OldMagnetUrlProtocol ();
			ap.bUrlProtocolSpecified = ap.tstrUrlProtocol != _T ("-");
			return FALSE != vmsMagnetExtension::AssociateWith (ap);
		}
		else if (bAssociate && !vmsMagnetExtension::IsAssociatedWithUs ())
		{
			vmsMagnetExtension::AssociationParameters ap = vmsMagnetExtension::GetCurrentAssociation ();
			if (!_tcsicmp (ap.tstrShellOpenCmdLine.c_str (), vmsMagnetExtension::get_ShellOpenCommandLine ()))
			{
				_App.Bittorrent_OldMagnetAssociation (ap.tstrShellOpenCmdLine.c_str ());
				_App.Bittorrent_OldMagnetUrlProtocol (ap.bUrlProtocolSpecified ? ap.tstrUrlProtocol.c_str () : _T ("-"));
			}
			else
			{
				_App.Bittorrent_OldMagnetAssociation (_T (""));
				_App.Bittorrent_OldMagnetUrlProtocol (_T ("-"));
			}
			return FALSE != vmsMagnetExtension::Associate ();
		}

		return true;
	}

	static std::vector<DWORD> GetProcessIds(LPCTSTR szProcName)	{
		std::vector<DWORD> result;
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE)
			return result;

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (_tcsicmp(entry.szExeFile, szProcName) == 0)
					result.push_back(entry.th32ProcessID);
			}
		}
		CloseHandle(snapshot);
		std::sort(result.begin(), result.end());  
		return result;
	}

	static bool IsProcessRunning(LPCTSTR szProcName)	{

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE)
			return false;

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (_tcsicmp(entry.szExeFile, szProcName) == 0)
				{
					CloseHandle(snapshot);
					return true;
				}
			}
		}
		CloseHandle(snapshot);
		return false;
	}

	static bool KillTask(tstring ExeFileName)
	{
		bool Co;
		HANDLE FS;
		HANDLE hProcess = NULL;
		DWORD dwError;
		int result;
		FS = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 ProcEntry;
		ProcEntry.dwSize = sizeof(ProcEntry);
		Co = Process32First(FS, &ProcEntry);

		while (Co)
		{
			if (ProcEntry.szExeFile == ExeFileName)
			{
				hProcess = OpenProcess(PROCESS_TERMINATE, false, ProcEntry.th32ProcessID);

				if (hProcess == NULL)
				{
					if (GetLastError() != ERROR_ACCESS_DENIED)
						return false;

					OSVERSIONINFO osvi;

					
					osvi.dwOSVersionInfoSize = sizeof(osvi);
					GetVersionEx(&osvi);

					
					if (osvi.dwPlatformId != VER_PLATFORM_WIN32_NT)
					{
						SetLastError(ERROR_ACCESS_DENIED);
						return false;
					}

					
					TOKEN_PRIVILEGES Priv, PrivOld;
					DWORD cbPriv = sizeof(PrivOld);
					HANDLE hToken;

					
					if (!OpenThreadToken(GetCurrentThread(),
						TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
						FALSE, &hToken))
					{
						if (GetLastError() != ERROR_NO_TOKEN)
							return false;

						
						if (!OpenProcessToken(GetCurrentProcess(),
							TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
							&hToken))
							return false;
					}

					_ASSERTE(ANYSIZE_ARRAY > 0);

					Priv.PrivilegeCount = 1;
					Priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Priv.Privileges[0].Luid);

					
					if (!AdjustTokenPrivileges(hToken, FALSE, &Priv, sizeof(Priv),
						&PrivOld, &cbPriv))
					{
						dwError = GetLastError();
						CloseHandle(hToken);
						SetLastError(dwError);
						return false;
					}

					if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
					{
						
						CloseHandle(hToken);
						SetLastError(ERROR_ACCESS_DENIED);
						return false;
					}

					
					hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcEntry.th32ProcessID);
					dwError = GetLastError();

					
					AdjustTokenPrivileges(hToken, FALSE, &PrivOld, sizeof(PrivOld),
						NULL, NULL);
					CloseHandle(hToken);

					if (hProcess == NULL)
					{
						SetLastError(FALSE);
						return false;
					}
				}
				
				if (!TerminateProcess(hProcess, (UINT)-1))
				{
					dwError = GetLastError();
					CloseHandle(hProcess);
					SetLastError(dwError);
					return false;
				}
			}
			Co = Process32Next(FS, &ProcEntry);
		}

		CloseHandle(FS);
		CloseHandle(hProcess);

		return true;
	}

	static tstring GetChromeUserDataPath()	
	{
		LPITEMIDLIST pidl = NULL;
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl)))
		{
			TCHAR tszPath[MY_MAX_PATH] = _T("");
			SHGetPathFromIDList(pidl, tszPath);
			if (tszPath[_tcslen(tszPath) - 1] != '\\')
				_tcscat(tszPath, _T("\\"));
			_tcscat(tszPath, _T("Google\\Chrome\\User Data\\"));

			IMallocPtr spMalloc;
			SHGetMalloc(&spMalloc);
			spMalloc->Free(pidl);
			return tszPath;
		}
		return _T("");
	}

	static tstring GetChromeScriptPath()
	{
		
		TCHAR tszExeFile[MAX_PATH];
		TCHAR tszWorkingDir[MAX_PATH];
		tstring sChromeScriptPath;
		if (GetModuleFileName(NULL, tszExeFile, MAX_PATH) != 0)
		{
			fsGetPath(tszExeFile, tszWorkingDir);
#ifdef _DEBUG
			sChromeScriptPath = tszWorkingDir;
			sChromeScriptPath += _T("\\..\\..\\To Distributive\\chrome.js");
#else
			sChromeScriptPath = tszWorkingDir;
			sChromeScriptPath += _T("\\Chrome\\chrome.js");
#endif
			if (GetFileAttributes(sChromeScriptPath.c_str()) == (DWORD) - 1)
			{
#ifndef _DEBUG
				sChromeScriptPath = tszWorkingDir;
				sChromeScriptPath += _T("\\..\\..\\To Distributive\\chrome.js");
#endif
				if (GetFileAttributes(sChromeScriptPath.c_str()) == (DWORD) - 1)
					return _T("");
			}
		}
		return sChromeScriptPath;
	}
};

