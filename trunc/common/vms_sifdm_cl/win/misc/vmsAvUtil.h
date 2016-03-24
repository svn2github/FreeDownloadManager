/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsAvUtil  
{
public:
	static HANDLE CreateRemoteThread (HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
	{
		HMODULE hDll = LoadLibrary (_T ("kernel32.dll"));
		typedef HANDLE (WINAPI *FNCreateRemoteThread)(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
		FNCreateRemoteThread pfn = (FNCreateRemoteThread) GetProcAddress (hDll, "CreateRemoteThread");
		if (!pfn)
			return NULL;
		return pfn (hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	}

	static HRESULT URLDownloadToFile (LPUNKNOWN p1,LPCTSTR p2,LPCTSTR p3,DWORD p4,LPBINDSTATUSCALLBACK p5)
	{
		std::wstring str = _T ("url");
		HMODULE hDll = LoadLibrary ((str + _T ("mon.dll")).c_str ());

		typedef HRESULT (WINAPI *FNURLDTF)(LPUNKNOWN,LPCTSTR,LPCTSTR,DWORD,LPBINDSTATUSCALLBACK);

		std::string str2 = "URL";

#if defined (_UNICODE) || defined (UNICODE)
		FNURLDTF pfn = (FNURLDTF)GetProcAddress (hDll, (str2 + "DownloadToFile" + "W").c_str ());
#else
		FNURLDTF pfn = (FNURLDTF)GetProcAddress (hDll, (str2 + "DownloadToFile" + "A").c_str ());
#endif

		return pfn (p1, p2, p3, p4, p5);
	}

	static DWORD ExpandEnvironmentStrings(LPCTSTR p1,LPTSTR p2,DWORD p3)
	{
		HMODULE hDll = LoadLibrary (_T ("kernel32.dll"));
		typedef DWORD (WINAPI *FNExpandEnvironmentStrings)(LPCTSTR,LPTSTR,DWORD);
		std::string str = "Expand";
		str += "EnvironmentStrings";
#if defined (UNICODE) || defined (_UNICODE)
		str += "W";
		FNExpandEnvironmentStrings pfnExpandEnvironmentStrings = (FNExpandEnvironmentStrings)GetProcAddress (hDll, str.c_str ());
#else
		str += "A";
		FNExpandEnvironmentStrings pfnExpandEnvironmentStrings = (FNExpandEnvironmentStrings)GetProcAddress (hDll, str.c_str ());
#endif

		return pfnExpandEnvironmentStrings (p1, p2, p3);
	}

	static BOOL CreateProcess (LPCTSTR p1,LPTSTR p2,LPSECURITY_ATTRIBUTES p3,LPSECURITY_ATTRIBUTES p4,BOOL p5,DWORD p6,LPVOID p7,LPCTSTR p8,LPSTARTUPINFO p9,LPPROCESS_INFORMATION p10)
	{
		HMODULE hDll = LoadLibrary (_T ("kernel32.dll"));

		typedef BOOL (WINAPI *FNCreateProcess)(LPCTSTR,LPTSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,
			BOOL,DWORD,LPVOID,LPCTSTR,LPSTARTUPINFO,LPPROCESS_INFORMATION);

		std::string str = "Create";
		str += "Process";

#if defined (UNICODE) || defined (_UNICODE)
		str += "W";
		FNCreateProcess pfnCreateProcess = (FNCreateProcess) GetProcAddress (hDll, str.c_str ());
#else
		str += "A";
		FNCreateProcess pfnCreateProcess = (FNCreateProcess) GetProcAddress (hDll, str.c_str ());
#endif

		return pfnCreateProcess (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
};

