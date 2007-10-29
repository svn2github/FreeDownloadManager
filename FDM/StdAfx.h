/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/          

#if !defined(AFX_STDAFX_H__3DEDF9D7_1A31_481A_8FBD_C2D53CEFE688__INCLUDED_)
#define AFX_STDAFX_H__3DEDF9D7_1A31_481A_8FBD_C2D53CEFE688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define VC_EXTRALEAN		

#define WINVER 0x400
#define _WIN32_IE 0x501
#define _WIN32_WINNT 0x400  

const int PRG_BUILD_NUMBER = 724;

const char* const PRG_AGENT_NAME = "FDM 2.x";

const char* const PRG_NAME		= "Free Download Manager";
const char* const PRG_FULL_NAME = "Free Download Manager 2.5";

const char* const PRG_VERSION = "2.5";

const char* const IE_USERAGENT_ADDITION	= "FDM";    

#ifdef _USELOGGING
#pragma message ("============== WARNING: _USELOGGING is defined ==============")
#endif

#define _TB_SIZE_X	_SkinMgr.tbSizeX()
#define _TB_SIZE_Y	_SkinMgr.tbSizeY()
#define _TB_USE_ALPHA	_SkinMgr.tbUseAlpha()            

#include <afxwin.h>         
#include <afxext.h>         
#include <afxdisp.h>        
#include <afxdtctl.h>		
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			
#endif     

#pragma warning (disable : 4192)
#pragma warning (disable : 4146)
#import "mshtml.tlb"
#import "shdocvw.dll"
#pragma warning (default : 4192)
#pragma warning (default : 4146)

#include <msxml.h>
_COM_SMARTPTR_TYPEDEF(IXMLDOMDocument, __uuidof(IXMLDOMDocument));
_COM_SMARTPTR_TYPEDEF(IXMLDOMNode, __uuidof(IXMLDOMNode));
_COM_SMARTPTR_TYPEDEF(IXMLDOMElement, __uuidof(IXMLDOMElement));
_COM_SMARTPTR_TYPEDEF(IXMLDOMNodeList, __uuidof(IXMLDOMNodeList));

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>  

class CFdmModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CFdmModule _Module;
#include <atlcom.h>

#ifndef LONG_PTR
#define LONG_PTR		LONG
#define DWORD_PTR		DWORD
#endif

#define MY_MAX_PATH		10000

#pragma warning (push, 3)
#include <vector>
#include <list>
#include <string>
#include <xlocale>
#pragma warning (pop)

//{{AFX_INSERT_LOCATION}//}}

#include "Include.add/inetfile/inetfile.h"
#include "vmsMediaConvertMgr.h"
#include "vmsFdmAppMgr.h"
#include "fsAppSettings.h"
#include "common.h"
#include "system.h"
#include <limits.h>
#include "vmsBtSupport.h"
#include "fsTrayIconMgr.h"
#include "fsDialogsMgr.h"
#include "fsClipboardMgr.h"
#include "ToolBarEx.h"
#include "fsUpdateMgr.h"
#include "fsAutorunMgr.h"
#include "fsCmdHistoryMgr.h"
#include <exdispid.h>
#include <htmlhelp.h>
#include "fsContextHelpMgr.h"
#include "fsMFCLangMgr.h"
#include "fsSitesMgr.h"
#include "fsIEContextMenuMgr.h"
#include "fsOpNetIntegrationMgr.h"
#include "fsIECatchMgr.h"
#include "fsProgramLogMgr.h"
#include "fsSoundsMgr.h"
#include "fsHistoriesMgr.h"
#include "vmsSkinMgr.h"
#include "vmsDownloadsGroupsMgr.h"
#include "fsDownloadsMgr.h"
#include "vmsTheme.h"
#include "fsPluginMgr.h"

#define SBMP(id) _SkinMgr.bmp (id)
#define SICO(id) _SkinMgr.icon (id)
#define SICOEX(id,cx,cy) _SkinMgr.icon(id,cx,cy)

extern fsAppSettings _App;
extern fsTrayIconMgr _TrayMgr;
extern fsDialogsMgr _DlgMgr;
extern fsClipboardMgr _ClipbrdMgr;
extern CToolBarEx _TBMgr;
extern fsUpdateMgr _UpdateMgr;
extern fsAutorunMgr _AutorunMgr;
extern fsCmdHistoryMgr _LastUrlFiles;
extern fsCmdHistoryMgr _LastUrlPaths;
extern fsCmdHistoryMgr _LastFlashVideoUrls;
extern fsCmdHistoryMgr _LastBatchUrls;
extern fsCmdHistoryMgr _LastFolders;
extern fsContextHelpMgr _CHMgr;
extern fsMFCLangMgr _LngMgr;
extern fsSitesMgr _SitesMgr;
extern fsIEContextMenuMgr _IECMM;
extern fsOpNetIntegrationMgr _NOMgr;
extern fsIECatchMgr _IECatchMgr;
#ifdef _USELOGGING
extern fsProgramLogMgr _Log;
#define LOG(x) _Log << x
#else
#define LOG(x)
#endif
extern fsSoundsMgr _Snds;
extern fsHistoriesMgr _HsMgr;
extern fsCmdHistoryMgr _LastFind;
extern vmsSkinMgr _SkinMgr;
extern fsDownloadsMgr _DldsMgr;
extern vmsTheme _theme;
extern vmsDownloadsGroupsMgr _DldsGrps;
extern FILETIME _timeAppHasStarted;
extern vmsBtSupport _BT;
extern vmsMediaConvertMgr _MediaConvertMgr;
extern vmsFdmAppMgr _AppMgr;

#define APPSTATE_PORTABLE_MODE					1
#define APPSTATE_PORTABLE_MODE_NOREG			2
extern DWORD _dwAppState;	
#ifndef _DEBUG
#define IS_PORTABLE_MODE	((_dwAppState & APPSTATE_PORTABLE_MODE) != 0)
#else
#define IS_PORTABLE_MODE	((_dwAppState & APPSTATE_PORTABLE_MODE) != 0)

#endif

#include "lngphrases.h"  

#define TRAY_ICON_NORMAL		0
#define TRAY_ICON_DOWNLOADING	1
#define TRAY_ICON_ERRORS		2
#define TRAY_ICON_UNKNOWN		3

#ifdef _DEBUG
extern void dbglog ( char* szFormat ... );
extern void dbglog_eraselog ();
#endif

#endif 
