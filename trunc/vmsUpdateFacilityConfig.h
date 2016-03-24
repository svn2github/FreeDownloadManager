/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "common\vms_sifdm_cl\app\vmsAppDataFolder.h"
class vmsUpdateFacilityConfig
{
public:
	virtual std::wstring componentOriginalPath () const = 0;
	virtual std::wstring componentCurrentPath () const = 0;
	virtual std::wstring componentAfterUpdatePath (bool bCommon) const = 0;
	virtual ~vmsUpdateFacilityConfig(void) {}
};

class vmsFdmSingleFileUpdaterFacilityConfig :
	public vmsUpdateFacilityConfig
{
public:
	vmsFdmSingleFileUpdaterFacilityConfig (std::shared_ptr <vmsAppDataFolder> appDataFolder,
			const std::wstring& fileName) :
		m_appDataFolder (appDataFolder),
		m_fileName (fileName)
	{
		assert (m_appDataFolder);
		assert (!m_fileName.empty ());
	}

	virtual std::wstring componentOriginalPath () const override
	{
		return vmsGetModulePath () + L"\\" + m_fileName;
	}

	virtual std::wstring componentCurrentPath () const override
	{
		#ifdef _DEBUG
			return componentOriginalPath ();
		#endif
		std::wstring ret = componentAfterUpdatePath (false);
		if (GetFileAttributes (ret.c_str ()) != DWORD (-1))
			return ret;
		return componentOriginalPath ();
	}

	virtual std::wstring componentAfterUpdatePath (bool bCommon) const override
	{
		return m_appDataFolder->filePath (m_fileName, bCommon);
	}

protected:
	std::shared_ptr <vmsAppDataFolder> m_appDataFolder;
	std::wstring m_fileName;
};

