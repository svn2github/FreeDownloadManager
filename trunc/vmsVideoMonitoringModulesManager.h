/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmOptionalModulesBasicManager.h"
class vmsVideoMonitoringModulesManager :
	public vmsFdmOptionalModulesBasicManager
{
public:
	vmsVideoMonitoringModulesManager(void) :
	  vmsFdmOptionalModulesBasicManager (std::make_shared <vmsOptionalModulesList> ())
	{
		auto mi = std::make_shared <vmsOptionalModulesList::module_info> ();
		mi->download_url = 
			L"http://files2.freedownloadmanager.org/fdm_components/fdm_videomon_inst2.exe";
		m_modules->register_module (L"flvsniff.dll", mi);
	}

	virtual ~vmsVideoMonitoringModulesManager(void)
	{
	}

	bool modules_installed ()
	{
		vmsOptionalModulesList::modules_names_t modules;
		modules_list ()->modules_names (modules);
		return modules_exists (modules);
	}
};

