/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"

void InitializeFdmBtSuppDll ()
{
	
}

int WINAPI vmsBt_getMinimumFdmBuildRequired ()
{
	
	return 1601;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		InitializeFdmBtSuppDll ();

    return TRUE;
}

