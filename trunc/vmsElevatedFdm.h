/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#pragma once
class vmsElevatedFdm
{
public:
	vmsElevatedFdm(void);
	~vmsElevatedFdm(void);
	bool Run(LPCTSTR ptszAdditionalArgs);
	static vmsElevatedFdm& o(void);
protected:
	
	bool m_bRunning;
public:
	bool InstallIeIntegration(bool bShowPreUacDlg);
};

