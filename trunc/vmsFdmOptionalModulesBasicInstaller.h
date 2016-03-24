/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOptionalModulesBasicInstaller.h"
#include "vmsSecurity.h"
class vmsFdmOptionalModulesBasicInstaller : 
	public vmsOptionalModulesBasicInstaller
{
public:
	vmsFdmOptionalModulesBasicInstaller (std::shared_ptr <vmsOptionalModulesList> modules,
		const std::wstring &installation_path) :
		vmsOptionalModulesBasicInstaller (modules, installation_path)
	{
	}

	virtual bool install_module (const std::wstring &name,
		const module_installation_info &mii, vmsError &err) override
	{
		if (!vmsFdmVerifySign (mii.distributive->file))
		{
			DeleteFile (mii.distributive->file.c_str ());
			return false;
		}

		return vmsOptionalModulesBasicInstaller::install_module (name, mii, err);
	}
};