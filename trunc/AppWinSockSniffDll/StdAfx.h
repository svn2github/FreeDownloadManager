/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_STDAFX_H__E337E97A_1D0F_4F8B_A2A7_FE9D6D5F1447__INCLUDED_)
#define AFX_STDAFX_H__E337E97A_1D0F_4F8B_A2A7_FE9D6D5F1447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define WIN32_LEAN_AND_MEAN		

#define _CRT_SECURE_NO_WARNINGS

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#include <windows.h>
#include <winsock2.h>
#include <wininet.h>
#include <tchar.h>
#include <shellapi.h>
#include <dbghelp.h>
#include <Psapi.h>

#include <atlbase.h>
#include <atlwin.h>

#include <string>
#include <vector>
#include <process.h>
using namespace std;
#include <assert.h>
#include <memory>
#include <set>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <ctime>

#include "common.h"

#ifdef _DEBUG

#endif

#include "../common/vms_sifdm_cl/base.h"
#include "../common/vms_sifdm_cl/thread/vmsCreatesThreads2.h"
#include "../common/vms_sifdm_cl/inet/http/vmsHttpRequestParser.h"
#include "../common/vms_sifdm_cl/inet/http/vmsHttpResponseParser.h"

#include "../Include.Add/vmsSourceCodeLogger.h"
#include "vmsLogHelper.h"
#include <atlbase.h>

//{{AFX_INSERT_LOCATION}}

#endif 
