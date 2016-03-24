/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../vmsAppDataFolder.h"
#include "../../fileio/util.h"
class vmsWinAppDataFolder : public vmsAppDataFolderBase
{
public:
	vmsWinAppDataFolder (const std::wstring& wstrCompanyName, const std::wstring& wstrAppName) :
		vmsAppDataFolderBase (wstrCompanyName, wstrAppName)
	{

	}

	virtual std::wstring path (bool bCommon = false) override
	{
		std::wstring& appData = bCommon ? m_wstrCommonPath : m_wstrPath;

		if (!appData.empty ())
			return appData;

		const int csidl = bCommon ? CSIDL_COMMON_APPDATA : CSIDL_APPDATA;

		TCHAR tszPath [MAX_PATH] = _T ("");
		SHGetFolderPath (NULL, csidl, NULL, SHGFP_TYPE_CURRENT, tszPath);

		appData = tszPath;
		appData += L"\\";
		if (!m_wstrCompanyName.empty ())
			appData += m_wstrCompanyName + L"\\";
		appData += m_wstrAppName;

		vmsBuildPathToFile ((appData + L"\\file.ext").c_str ());

		return appData;
	}

protected:
	std::wstring m_wstrPath;
	std::wstring m_wstrCommonPath;
};