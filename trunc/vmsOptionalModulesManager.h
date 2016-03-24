/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOptionalModulesList.h"
#include "vmsOptionalModulesDownloader.h"
#include "vmsOptionalModulesInstaller.h"
class vmsOptionalModulesManager
{
public:
	vmsOptionalModulesManager(std::shared_ptr <vmsOptionalModulesList> modules,
		std::shared_ptr <vmsOptionalModulesDownloader> downloader,
		std::shared_ptr <vmsOptionalModulesInstaller> installer) :
		m_modules (modules),
		m_downloader (downloader),
		m_installer (installer)
	{
		assert (m_modules);
		assert (m_downloader);
		assert (m_installer);
	}

	virtual ~vmsOptionalModulesManager(void)
	{
	}

	std::shared_ptr <const vmsOptionalModulesList> modules_list () const
	{
		return m_modules;
	}

	bool module_exists (const std::wstring& name)
	{
		return m_installer->module_exists (name);
	}

	bool modules_exists (const vmsOptionalModulesList::modules_names_t& modules)
	{
		return m_installer->modules_exists (modules);
	}

	void prepare_modules_installation (
		const vmsOptionalModulesInstaller::modules_to_install_t &modules,
		std::map <std::wstring, vmsOptionalModulesInstaller::module_installation_info> &result)
	{
		assert (m_installer);
		m_installer->build_modules_installation_info (modules, result);
	}

	bool install_module (const std::wstring &name, 
		const vmsOptionalModulesInstaller::module_installation_info &mii,
		vmsOperationProgressCallback *callback, vmsError &err)
	{
		assert (m_installer && m_downloader);

		if (mii.distributive->state == vmsOptionalModulesInstaller::distributive_info::none)
		{
			if (!m_downloader->download_module (mii.distributive->url, 
				mii.distributive->file, callback, err))
			{
				mii.distributive->state = vmsOptionalModulesInstaller::distributive_info::failure;
				return false;
			}

			mii.distributive->state = vmsOptionalModulesInstaller::distributive_info::downloaded;
		}

		return m_installer->install_module (name, mii, err);
	}

protected:
	std::shared_ptr <vmsOptionalModulesList> m_modules;
	std::shared_ptr <vmsOptionalModulesDownloader> m_downloader;
	std::shared_ptr <vmsOptionalModulesInstaller> m_installer;
};

