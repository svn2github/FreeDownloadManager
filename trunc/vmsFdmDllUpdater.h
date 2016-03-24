/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsUpdateFacilityConfig.h"
#include "common\vms_sifdm_cl\vmsResourceObjectsHolder.h"
#include "vmsFdmSimpleDownloader.h"
#include "vmsTmpFileName.h"
#include <fstream>
#include "vmsSecurity.h"
#include "vmsAppFileUpdateInfo.h"
#include "vmsURLDownloader.h"
class vmsFdmDllUpdater
{
public:
	vmsFdmDllUpdater(std::shared_ptr <vmsUpdateFacilityConfig> config, 
			std::shared_ptr <vmsResourceObjectsHolderController> rohController,
			const std::wstring &updateUrl, bool currentBuildFirstRun) :
		m_config (config),
		m_rohController (rohController),
		m_updateUrl (updateUrl)
	{
		assert (m_config);
		assert (m_rohController);
		assert (!m_updateUrl.empty ());
		if (currentBuildFirstRun)
			checkOriginalDllUpdated ();
	}

	~vmsFdmDllUpdater(void)
	{
	}

	bool PerformUpdate (bool& updatePerformed)
	{
		if (PerformUpdateViaCommonDll (updatePerformed) && updatePerformed)
			return true;

		if (PerformUpdateViaInternet (updatePerformed))
		{
			if (updatePerformed)
			{
				CopyFile (m_config->componentAfterUpdatePath (false).c_str (),
					m_config->componentAfterUpdatePath (true).c_str (), FALSE);
			}

			return true;
		}

		return false;
	}

	bool PerformUpdateViaCommonDll (bool& updatePerformed)
	{
		updatePerformed = false;
		const std::wstring commonDll = m_config->componentAfterUpdatePath (true);
		if (commonDll.empty () || !isNewerDll (commonDll))
			return true;
		if (!vmsFdmVerifySign (std::wstring (commonDll)))
			return false;
		if (!PerformDllHotSwap (commonDll))
			return false;
		return updatePerformed = true;
	}

	bool PerformUpdateViaInternet (bool& updatePerformed)
	{
		updatePerformed = false;

		std::wstring url;
		if (!queryUrlOfUpdatedDll (url))
			return false;
		if (url.empty ())
			return true;

		vmsFdmSimpleDownloader dldr;
		vmsTmpFileName tmpFileName;
		std::ofstream os (tmpFileName, std::ofstream::out | std::ofstream::binary); 
		if (IR_SUCCESS != dldr.Download (url, os))
			return false;
		os.close ();

		if (!vmsFdmVerifySign (std::wstring (tmpFileName)))
			return false;

		if (!PerformDllHotSwap (tmpFileName.path_name ()))
			return false;

		return updatePerformed = true;
	}

protected:
	std::shared_ptr <vmsUpdateFacilityConfig> m_config;
	std::shared_ptr <vmsResourceObjectsHolderController> m_rohController;
	std::wstring m_updateUrl;

protected:
	bool queryUrlOfUpdatedDll (std::wstring &url)
	{
		url.clear ();
		vmsAppFileUpdateInfo fui (m_updateUrl.c_str (), _AppMgr.getBuildNumber ());
		if (!fui.Retrieve ())
			return false;
		if (fui.getResult ()->tstrFileURL.empty ())
			return true;
		vmsAppVersion ver;
		ver.FromString (fui.getResult ()->tstrVersion.c_str ());
		vmsBinaryFileVersionInfo bfvi;
		bfvi.Retrieve (m_config->componentCurrentPath ().c_str ());
		if (ver.CompareVersions (bfvi.m_fileVersion, vmsAppVersion::CT_FLOATING) > 0)
			url = fui.getResult ()->tstrFileURL;
		return true;
	}

	bool PerformDllHotSwap (const std::wstring& file)
	{
		m_rohController->waitForResourceNotBusy ();
		m_rohController->releaseResourceReference ();
		bool bRet = FALSE != CopyFile (file.c_str (), 
			m_config->componentAfterUpdatePath (false).c_str (), FALSE);
		m_rohController->loadResource (m_config->componentCurrentPath ());
		return bRet;
	}

	bool isNewerDll (const std::wstring& file)
	{
		vmsBinaryFileVersionInfo bfvi;
		bfvi.Retrieve (m_config->componentCurrentPath ().c_str ());

		vmsBinaryFileVersionInfo bfvi2;
		bfvi2.Retrieve (file.c_str ());

		return bfvi2.m_fileVersion.CompareVersions (bfvi.m_fileVersion,
			vmsAppVersion::CT_FLOATING) > 0;
	}

	void checkOriginalDllUpdated ()
	{
		const std::wstring originalDll = m_config->componentOriginalPath ();
		if (isNewerDll (originalDll))
		{
			DeleteFile (m_config->componentAfterUpdatePath (true).c_str ());
			DeleteFile (m_config->componentAfterUpdatePath (false).c_str ());
			assert (m_config->componentCurrentPath () == m_config->componentOriginalPath ());
		}
	}
};

