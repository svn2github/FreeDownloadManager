/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsVideoMonitoringModulesManager.h"
#include "WaitDlg2.h"
class vmsVideoMonitoringModulesUiManager
{
public:
	vmsVideoMonitoringModulesUiManager (
		std::shared_ptr <vmsVideoMonitoringModulesManager> vmm);
	~vmsVideoMonitoringModulesUiManager ();

protected:
	std::shared_ptr <vmsVideoMonitoringModulesManager> m_vmm;
public:
	bool MakeSureModulesInstalled (CWnd* parentWindow, 
		bool ask_confirmation = true, bool check_for_updates = true);
	bool modules_installed ()  {return m_vmm->modules_installed ();}

protected:
	void install_monitoring_modules (
		CWaitDlg2::sleep_function_t sleepfn,
		vmsOperationProgressCallback *callback);
public:
	bool InstallModules (CWnd* parentWindow);
	bool CheckForUpdates (CWnd* parentWindow, bool ask_confirmation = true);
};

