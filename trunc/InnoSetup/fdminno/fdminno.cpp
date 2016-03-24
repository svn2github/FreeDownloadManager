/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include <tchar.h>
#include <stdio.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DWORD _stdcall GetSystemDefaultLanguageId ()
{
	TCHAR sz [10] = _T("");
	GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_IDEFAULTLANGUAGE, sz, sizeof (sz));
	DWORD dw = 0;
	_stscanf_s (sz, _T("%x"), &dw);
	return dw;
}

