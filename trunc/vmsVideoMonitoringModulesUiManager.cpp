/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsVideoMonitoringModulesUiManager.h"
#include "WaitDlg2.h"
#include "vmsVideoMonitoringModulesUpdateManager.h"
#include "MyMessageBox.h"

vmsVideoMonitoringModulesUiManager::vmsVideoMonitoringModulesUiManager (
	std::shared_ptr <vmsVideoMonitoringModulesManager> vmm) :
	m_vmm (vmm)
{
	assert (m_vmm);
}

vmsVideoMonitoringModulesUiManager::~vmsVideoMonitoringModulesUiManager ()
{
}

bool vmsVideoMonitoringModulesUiManager::MakeSureModulesInstalled (
	CWnd* parentWindow, 
	bool ask_confirmation, 
	bool check_for_updates)
{
	assert (parentWindow);

	if (!m_vmm->modules_installed ())
	{
		if (ask_confirmation &&
			IDYES != parentWindow->MessageBox (LS (L_ADD_COMP_REQ_WYL_TO_DL),
			nullptr, MB_ICONQUESTION | MB_YESNO))
		{
			return false;
		}

		return InstallModules (parentWindow);
	}
	else if (check_for_updates)
	{
		CheckForUpdates (parentWindow);
	}

	return true;
}

void vmsVideoMonitoringModulesUiManager::install_monitoring_modules (
	CWaitDlg2::sleep_function_t sleepfn,
	vmsOperationProgressCallback *callback)
{
	vmsOptionalModulesList::modules_names_t modules;
	m_vmm->modules_list ()->modules_names (modules);

	std::map <std::wstring, vmsOptionalModulesInstaller::module_installation_info> mmii;
	m_vmm->prepare_modules_installation (modules, mmii);

	for (auto it = mmii.begin (); it != mmii.end (); ++it)
	{
		vmsError err;
		m_vmm->install_module (it->first, it->second, callback, err);
	}
}

bool vmsVideoMonitoringModulesUiManager::InstallModules (CWnd* parentWindow)
{
	assert (parentWindow);

	CWaitDlg2::job_function_t jobfn = std::bind (
		&vmsVideoMonitoringModulesUiManager::install_monitoring_modules,
		this, std::placeholders::_1, std::placeholders::_2);

	CWaitDlg2 dlg (parentWindow, LS (L_DOWNLOADING_INSTALLING_COMP),
		jobfn, nullptr, true);

	_DlgMgr.DoModal (&dlg);

	if (dlg.cancelledByUser ())
		return false;

	bool result = m_vmm->modules_installed ();

	if (!result)
		parentWindow->MessageBox (LS (L_ERROR_INSTALLING_ADD_COMP), nullptr, MB_ICONERROR);

	return result;
}

bool vmsVideoMonitoringModulesUiManager::CheckForUpdates (
	CWnd* parentWindow,
	bool ask_confirmation)
{
	vmsVideoMonitoringModulesUpdateManager um;

	if (um.checked_for_updates_recently () ||
		!um.check_for_updates () ||
		!um.new_version_exists ())
	{
		return false;
	}

	if (ask_confirmation)
	{
		auto ret = MyMessageBox2 (parentWindow, 
			LS (L_VIDEOMON_NEWVER_AVAIL),
			L"Free Download Manager",
			LS (L_SKIP_THIS_UPDATE),
			IDI_QUESTION,
			L"OK",
			LS (L_CANCEL));
		
		if (ret.first == IDC_BTN2)
		{
			if (ret.second)
				um.skip_this_update ();
			return false;
		}
	}

	return InstallModules (parentWindow);
}
