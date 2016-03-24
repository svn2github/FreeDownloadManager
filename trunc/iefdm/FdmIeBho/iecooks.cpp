/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "iecooks.h"

#include "iecooks_i.c"
#include "FDMIECookiesBHO.h"
#include "FDMIEStat.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_FDMIEBHO, CFDMIEBHO)
OBJECT_ENTRY(CLSID_FDMIEStat, CFDMIEStat)
END_OBJECT_MAP()

BOOL _bIsWin9x = FALSE;

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		_bIsWin9x = GetVersion () & 0x80000000;

        _Module.Init(ObjectMap, hInstance, &LIBID_IEFDMBHOLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    
}

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

bool RegisterBho (HKEY hkRoot, bool register_)
{
	LPCTSTR key_path = _T ("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\{CC59E0F9-7E43-44FA-9FAA-8377850BF205}");
	
	if (!register_)
		return SHDeleteKey (hkRoot, key_path) == ERROR_SUCCESS;

	CRegKey key;
	auto result = key.Create (hkRoot, key_path, nullptr, 0, KEY_WRITE);
	if (result != ERROR_SUCCESS)
		return result == ERROR_ALREADY_EXISTS;

	key.SetDWORDValue (_T ("NoExplorer"), 1);

	return true;
}

bool RegisterBho (bool register_)
{
	bool per_user = false;
	AtlGetPerUserRegistration (&per_user);
	return RegisterBho (per_user ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE, 
		register_);
}

STDAPI DllRegisterServer(void)
{
    
    HRESULT hr = _Module.RegisterServer(TRUE);
	if (FAILED (hr))
		return hr;
	return RegisterBho (true) ? S_OK : E_FAIL;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _Module.UnregisterServer(TRUE);
	if (FAILED (hr))
		return hr;
	return RegisterBho (false) ? S_OK : E_FAIL;
}

BSTR WINAPI fdmbho_getTabUrl (DWORD dwThreadId)
{
	vmsCriticalSectionAutoLock csal (&CFDMIEBHO::_csInstances);
	int nIndex = CFDMIEBHO::findInstanceIndexByThreadId (dwThreadId);
	if (nIndex == -1)
		return NULL;
	CFDMIEBHO *bho = CFDMIEBHO::_vInstances [nIndex].pBHO;
	vmsCriticalSectionAutoLock csal2 (&bho->m_cswstrPageUrl);
	return SysAllocString (bho->m_wstrPageUrl.c_str ());
}

STDAPI vmsInstaller (bool bInstall, bool bCurrentUserOnly)
{
	if (bCurrentUserOnly)
		AtlSetPerUserRegistration (true);

	HRESULT hrRes = bInstall ? DllRegisterServer () : DllUnregisterServer ();

	if (bCurrentUserOnly)
		AtlSetPerUserRegistration (false);

	return hrRes;
}