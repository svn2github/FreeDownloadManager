/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "FDMCustIEBtn.h"

#include "FDMCustIEBtn_i.c"
#include "FDMCustButton.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_FDMCustButton, CFDMCustButton)
END_OBJECT_MAP()

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_FDMCUSTIEBTNLib);
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

STDAPI DllRegisterServer(void)
{
   	HKEY key;
	TCHAR szIco [MAX_PATH];
	TCHAR szCustomizer [1000];
	DWORD dwSize = MAX_PATH;
	DWORD dwBuffSize = dwSize * sizeof(TCHAR);

	if (ERROR_SUCCESS != RegOpenKey (HKEY_CURRENT_USER, _T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), &key))
		return E_FAIL;
	
	
	if (ERROR_SUCCESS != RegQueryValueEx (key, _T("Path"), NULL, NULL, (LPBYTE) szIco, &dwBuffSize))
		return E_FAIL;
	dwSize = dwBuffSize / sizeof(TCHAR);
	szIco [dwSize] = 0;
	if (szIco [dwSize-1] != _T('\\'))
		_tcscat_s (szIco, MAX_PATH, _T("\\"));
	_tcscat_s (szIco, MAX_PATH, _T("fdmcsiebtn.ico"));

	dwSize = 1000;
	if (ERROR_SUCCESS != RegQueryValueEx (key, _T("Customizer"), NULL, NULL, (LPBYTE) szCustomizer, &dwBuffSize))
		return E_FAIL;
	dwSize = dwBuffSize / sizeof(TCHAR);
	szCustomizer [dwSize] = 0;

	RegCloseKey (key);

	
    HRESULT hr = _Module.RegisterServer(TRUE);
	if (FAILED (hr))
		return hr;

	RegOpenKey (HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Extensions\\{DAC360AF-9FD0-432D-B2F2-ED3220F4CAD9}"), &key);
	RegSetValueEx (key, _T("Icon"), 0, REG_SZ, (LPBYTE)szIco, _tcslen (szIco));
	RegSetValueEx (key, _T("HotIcon"), 0, REG_SZ, (LPBYTE)szIco, _tcslen (szIco));
	RegSetValueEx (key, _T("ButtonText"), 0, REG_SZ, (LPBYTE)szCustomizer, _tcslen (szCustomizer));
	RegCloseKey (key);
	
	return hr;
}

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}

