/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_STDAFX_H__26AF3072_0185_4726_A622_751AE316A367__INCLUDED_)
#define AFX_STDAFX_H__26AF3072_0185_4726_A622_751AE316A367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define STRICT
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define _ATL_APARTMENT_THREADED

#define ISOLATION_AWARE_ENABLED 1

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_INSECURE_NO_DEPRECATE

#define MY_MAX_PATH		10000

#include <windows.h>
#include <commctrl.h>
#include <comdef.h>
#include <shlobj.h>
#include <shlguid.h>
#include <wininet.h>

#include <atlbase.h>

extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include <DbgHelp.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>
using namespace std;

#define LOGFN(a)

#include "../../common/vms_sifdm_cl/base.h"

//{{AFX_INSERT_LOCATION}}

#endif 
