/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhSniffDllProxyApp.h"

DWORD WINAPI _threadMain (LPVOID)
{
	vmsFdmBhSniffDllProxyApp app;
	return (DWORD)app.Run ();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle (
			CreateThread (nullptr, 0, _threadMain, nullptr, 0, nullptr));
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

