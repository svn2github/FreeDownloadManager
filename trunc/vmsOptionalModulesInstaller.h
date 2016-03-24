/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOptionalModulesList.h"
class vmsOptionalModulesInstaller
{
public:
	typedef vmsOptionalModulesList::modules_names_t modules_to_install_t;

	struct distributive_info
	{
		std::wstring url;
		std::wstring file;
		enum distributive_state {none, downloaded, installed, failure};
		distributive_state state;
		distributive_info () :
			state (none) {}
	};

	struct module_installation_info
	{
		std::shared_ptr <distributive_info> distributive;
		std::shared_ptr <vmsOptionalModulesList::module_info> module_info;
	};

public:
	vmsOptionalModulesInstaller(void)
	{
	}

	virtual ~vmsOptionalModulesInstaller(void)
	{
	}

	virtual bool module_exists (const std::wstring& name) = 0;

	virtual bool modules_exists (const modules_to_install_t& modules)
	{
		for (auto it = modules.begin (); it != modules.end (); ++it)
		{
			if (!module_exists (*it))
				return false;
		}

		return true;
	}

	virtual void build_modules_installation_info (const modules_to_install_t &modules,
		std::map <std::wstring, module_installation_info> &result) = 0;

	virtual bool install_module (const std::wstring &name, 
		const module_installation_info &mii, vmsError &err) = 0;
};

