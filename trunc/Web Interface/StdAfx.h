/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_STDAFX_H__0CA4122A_5758_4A99_B804_CB72B44676C6__INCLUDED_)
#define AFX_STDAFX_H__0CA4122A_5758_4A99_B804_CB72B44676C6__INCLUDED_

#define _WIN32_WINNT 0x0501

#if _MSC_VER > 1000
#pragma once
#endif 

#define _WIN32_WINNT 0x0501

#include <string>

#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#define VC_EXTRALEAN		

#include <afxwin.h>         
#include <afxext.h>         
#include <afxdisp.h>        
#include <afxdtctl.h>		
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			
#endif 

//{{AFX_INSERT_LOCATION}}

#include "fsTrayIconMgr.h"
extern fsTrayIconMgr _TrayMgr;

#endif 
