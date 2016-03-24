/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

inline bool vmsGetVersion (LPOSVERSIONINFOW version)
{
	typedef LONG (WINAPI *FNRtlGetVersion) (PRTL_OSVERSIONINFOW lpVersionInformation);
	FNRtlGetVersion pfn = (FNRtlGetVersion) GetProcAddress (GetModuleHandle (L"ntdll"), "RtlGetVersion");
	assert (pfn);
	if (pfn)
		return pfn (version) == 0;
#pragma warning (push)
#pragma warning (disable: 4996)
	return GetVersionExW (version) != FALSE;
#pragma warning (pop)
}