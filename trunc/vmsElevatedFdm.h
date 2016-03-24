/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsElevatedFdm
{
public:
	vmsElevatedFdm(void);
	~vmsElevatedFdm(void);
	static bool Run(LPCTSTR ptszAdditionalArgs, bool bWaitForComplete = false, bool bShowPreUacDlg = true);
	static vmsElevatedFdm& o(void);
protected:
	
	bool m_bInstallIeIntegrationPerformed;
public:
	bool InstallIeIntegration(bool bShowPreUacDlg);
	bool InstallChromeIntegration(bool bShowPreUacDlg);
	
	
	
	
	
	static bool InstallIntegration (const std::vector <vmsKnownBrowsers::Browser> &vBrowsers, bool bInstall, bool bShowPreUacDlg);
};

