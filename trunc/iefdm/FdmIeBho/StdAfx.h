/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_STDAFX_H__47BAA06B_F3FC_415D_9BFF_4633F8789428__INCLUDED_)
#define AFX_STDAFX_H__47BAA06B_F3FC_415D_9BFF_4633F8789428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define STRICT
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define _ATL_APARTMENT_THREADED

#define ISOLATION_AWARE_ENABLED			1
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <DbgHelp.h>

#include <atlbase.h>

extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include <comdef.h>

#include <string>
#include <vector>
using namespace std;
#include <assert.h>
#include <regex>
#include <map>
#include <memory>
#include <functional>

#include <Mshtml.h>
#include <exdisp.h>

#include "../common/vms_sifdm_cl/base.h"

//{{AFX_INSERT_LOCATION}}

#endif 
