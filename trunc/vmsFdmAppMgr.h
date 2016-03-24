/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSFDMAPPMGR_H__BF49C5FB_A3C9_4C50_B272_F5DF493C409C__INCLUDED_)
#define AFX_VMSFDMAPPMGR_H__BF49C5FB_A3C9_4C50_B272_F5DF493C409C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsFdmAppMgr  
{
public:
	static LPCTSTR getAppBtAgentName ();
	static LPCTSTR getAppAgentNameUf();
	static BOOL MakeSureBtInstalled();
	static LPCTSTR getAppName();
	static LPCTSTR getAppAgentName();
	static LPCTSTR getAppVersionedName();
	static const vmsBinaryFileVersionInfo* getVersion();
	static LPCTSTR getBuildNumberAsString ();
	static int getBuildNumber ();
	static void ShowInstallBtMessage();
	static void ShowInstallMediaFeaturesMessage();
	static BOOL IsMediaFeaturesInstalled();
	static BOOL IsBtInstalled();

	vmsFdmAppMgr();
	virtual ~vmsFdmAppMgr();

};

#endif 
