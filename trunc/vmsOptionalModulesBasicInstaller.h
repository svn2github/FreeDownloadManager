/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOptionalModulesInstaller.h"
#include "vmsOptionalModulesList.h"
class vmsOptionalModulesBasicInstaller :
	public vmsOptionalModulesInstaller
{
public:
	vmsOptionalModulesBasicInstaller(std::shared_ptr <vmsOptionalModulesList> modules,
		const std::wstring &installation_path) :
		m_modules (modules),
		m_installation_path (installation_path)
	{
		assert (m_modules);
		assert (!m_installation_path.empty ());
	}

	~vmsOptionalModulesBasicInstaller(void)
	{
	}

	virtual void build_modules_installation_info (const std::vector <std::wstring> &modules,
		std::map <std::wstring, module_installation_info> &result) override
	{
		result.clear ();

		std::map <std::wstring, std::shared_ptr <distributive_info>> distributives; 

		auto&& modules_list = m_modules->modules ();

		for (auto it = modules.begin (); it != modules.end (); ++it)
		{
			auto itmodule = modules_list.find (*it);
			assert (itmodule != modules_list.end ());
			if (itmodule == modules_list.end ())
				continue;

			auto itdistr = distributives.find (itmodule->second->download_url);
			if (itdistr == distributives.end ())
			{
				auto distr = std::make_shared <distributive_info> ();
				distr->url = itmodule->second->download_url;
				itdistr = distributives.insert (std::make_pair (distr->url, distr)).first;
			}

			module_installation_info mii;
			mii.distributive = itdistr->second;
			mii.module_info = itmodule->second;
			result [*it] = mii;
		}
	}

	virtual bool module_exists (const std::wstring& name) override
	{
		return vmsFileExists (m_installation_path + L"\\" + name);
	}

	virtual bool install_module (const std::wstring &name, 
		const module_installation_info &mii, vmsError &err) override
	{
		if (mii.distributive->state != vmsOptionalModulesInstaller::distributive_info::installed)
		{
			if (mii.distributive->state != vmsOptionalModulesInstaller::distributive_info::downloaded)
			{
				err.error = common_error::unknown_error; 
				return false;
			}

			if (!install_distributive (name, mii.distributive, err))
			{
				mii.distributive->state = vmsOptionalModulesInstaller::distributive_info::failure;
				return false;
			}
		}

		return true; 
	}

protected:
	std::shared_ptr <vmsOptionalModulesList> m_modules;
	std::wstring m_installation_path;

protected:
	bool install_distributive (const std::wstring &name, 
		const std::shared_ptr <distributive_info> &distributive, vmsError &err)
	{
		vmsCommandLine cl; 
		cl.setExe (distributive->file.c_str ());
		std::wstringstream wss;
		wss << L"/verysilent /norestart /dir \"" << m_installation_path << L"\"";
		cl.setArgs (wss.str ().c_str ());
		DWORD flags = vmsCommandLine::WaitForCompletion | 
			vmsCommandLine::RunElevatedIfRequired;
		if (!vmsCheckDirectoryWriteAccess (m_installation_path))
			flags |= vmsCommandLine::RunElevated;
		return cl.Execute (flags, nullptr, nullptr, &err);
	}
};

