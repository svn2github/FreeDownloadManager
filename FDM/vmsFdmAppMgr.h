/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSFDMAPPMGR_H__BF49C5FB_A3C9_4C50_B272_F5DF493C409C__INCLUDED_)
#define AFX_VMSFDMAPPMGR_H__BF49C5FB_A3C9_4C50_B272_F5DF493C409C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "vmsBinaryFileVersionInfo.h"

class vmsFdmAppMgr  
{
public:
	static LPCSTR getAppName();
	static LPCSTR getAppAgentName();
	static LPCSTR getAppVersionedName();
	static const vmsBinaryFileVersionInfo* getVersion();
	static LPCSTR getBuildNumber ();
	void ShowInstallBtMessage();
	void ShowInstallMediaFeaturesMessage();
	BOOL IsMediaFeaturesInstalled();
	BOOL IsBtInstalled();
	vmsFdmAppMgr();
	virtual ~vmsFdmAppMgr();

};

#endif 
