/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "fdm.h"
#include "vmsNotEverywhereSupportedFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsNotEverywhereSupportedFunctions::vmsNotEverywhereSupportedFunctions()
{
	
}

vmsNotEverywhereSupportedFunctions::~vmsNotEverywhereSupportedFunctions()
{

}

LONG vmsNotEverywhereSupportedFunctions::RegOverridePredefKey(HKEY hkey1, HKEY hkey2)
{
	HMODULE h = LoadLibrary (_T("advapi32.dll"));
	if (!h)
		return ERROR_BAD_ENVIRONMENT;
	typedef LONG (WINAPI *FNROPK)(HKEY,HKEY);
	FNROPK pfn = (FNROPK)GetProcAddress (h, "RegOverridePredefKey");
	if (!pfn)
	{
		FreeLibrary (h);
		return ERROR_BAD_ENVIRONMENT;
	}
	LONG lRes = pfn (hkey1, hkey2);
	FreeLibrary (h);
	return lRes;
}

BOOL vmsNotEverywhereSupportedFunctions::OverrideHKCRForUser(){
	HKEY hKey;
	LONG lRes;
	lRes = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hKey);
	if (lRes == ERROR_SUCCESS){
		lRes = RegOverridePredefKey(HKEY_CLASSES_ROOT, hKey);
		RegCloseKey(hKey);
	}
	return ( lRes == ERROR_SUCCESS );
}

BOOL vmsNotEverywhereSupportedFunctions::ResetHKCR(){
	return ( RegOverridePredefKey(HKEY_CLASSES_ROOT, NULL) == ERROR_SUCCESS );
}