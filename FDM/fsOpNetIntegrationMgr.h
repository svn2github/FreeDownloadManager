/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSOPNETINTEGRATIONMGR_H__FAFB09B6_9FCC_4D7F_BE4C_95E6CD22AAA0__INCLUDED_)
#define AFX_FSOPNETINTEGRATIONMGR_H__FAFB09B6_9FCC_4D7F_BE4C_95E6CD22AAA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class fsOpNetIntegrationMgr  
{
public:
	BOOL DeinstallMozillaSuitePlugin();
	BOOL InstallMozillaSuitePlugin();
	BOOL IsMozillaSuitePluginInstalled(BOOL bQueryPluginDirIfUnknown = FALSE);
	
	void Initialize();

	
	BOOL InstallOperaPlugin();
	
	
	
	BOOL IsOperaPluginInstalled(BOOL bQueryPluginDirIfUnknown = FALSE);
	
	BOOL DeinstallOperaPlugin();

	BOOL DeinstallNetscapePlugin();
	BOOL InstallNetscapePlugin();
	BOOL IsNetscapePluginInstalled(BOOL bQueryPluginDirIfUnknown = FALSE);

	BOOL DeinstallFirefoxPlugin();
	BOOL InstallFirefoxPlugin();
	BOOL IsFirefoxPluginInstalled(BOOL bQueryPluginDirIfUnknown = FALSE);
	
	fsOpNetIntegrationMgr();
	virtual ~fsOpNetIntegrationMgr();

protected:
	CString m_strOpPath;	
	CString m_strNetPath;	
	CString m_strFfPath;	
	CString m_strMozSPath;	
};

#endif 
