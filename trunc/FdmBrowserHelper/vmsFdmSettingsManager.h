/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmSettings.h"
class vmsFdmSettingsManager :
	public vmsThreadSafe4
{
public:
	vmsFdmSettingsManager ();
	~vmsFdmSettingsManager ();

public:
	void ReadSettings ();

	vmsFdmSettings& settings (auto_lock_t&)
	{
		return m_settings;
	}

	void set_browser (const std::wstring &browser)
	{
		vmsThreadSafe4Scope;
		m_browser = browser;
	}

protected:
	std::wstring m_browser;
	vmsFdmSettings m_settings;
	CRegKey m_fdmKey, m_fdmBrMonitorKey, m_fdmBrMenuKey;

protected:
	void ReadBrowserMenuSettings_ ();
	void ReadBrowserMonitorSettings_ ();
	void ReadBrowserMonitorVideoSettings_ ();
};

