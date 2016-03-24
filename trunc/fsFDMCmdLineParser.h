/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSFDMCMDLINEPARSER_H__3DD882CC_AADC_457D_91DE_228723758761__INCLUDED_)
#define AFX_FSFDMCMDLINEPARSER_H__3DD882CC_AADC_457D_91DE_228723758761__INCLUDED_

#include "fsCommandLineParser.h"	
#if _MSC_VER > 1000
#pragma once
#endif 

class fsFDMCmdLineParser  
{
public:
	bool isNeedExit () const {return m_bNeedExit;}
	BOOL is_ForceSilentSpecified();
	
	enum PerformTasksOfType {Elevated, Normal};
	void Parse(PerformTasksOfType enPTT);
	fsFDMCmdLineParser();
	virtual ~fsFDMCmdLineParser();

protected:
	void AddTorrent (LPCTSTR pszTorrent);
	BOOL m_bAnotherFDMStarted;	
	fsCommandLineParser m_parser;	
	BOOL m_bForceSilent;		
	bool m_bNeedExit;
	bool m_bNeedRegisterServer, m_bNeedUnregisterServer, m_bNeedRegisterServerUserOnly;
	bool m_bRunAsElevatedTasksProcessor;
	bool m_bInstallIeIntegration;
	bool m_bInstallChromeIntegration;
	std::vector <vmsKnownBrowsers::Browser> m_vBrowsersToInstallIntegration, m_vBrowsersToDeinstallIntegration;
public:
	bool isNeedRegisterServer(void);
	bool isNeedUnregisterServer(void);
	bool isNeedRegisterServerUserOnly(void);
	bool isRunAsElevatedTasksProcessor () const {return m_bRunAsElevatedTasksProcessor;}
	bool isNeedInstallIeIntegration () const {return m_bInstallIeIntegration;}
	bool isNeedInstallChromeIntegration() const { return m_bInstallChromeIntegration; }
	const std::vector <vmsKnownBrowsers::Browser>& getBrowsersToInstallIntegration (bool bInstall) const {return bInstall ? m_vBrowsersToInstallIntegration : m_vBrowsersToDeinstallIntegration;}

protected:
	static void ReadIntVector(LPCTSTR ptsz, std::vector <int>& v);
};

#endif 
