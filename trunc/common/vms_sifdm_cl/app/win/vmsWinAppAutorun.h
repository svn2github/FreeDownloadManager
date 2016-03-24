/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../vmsAppAutorun.h"
class vmsWinAppAutorun :
	public vmsAppAutorunBase
{
public:
	vmsWinAppAutorun (const std::wstring &appname,
		const std::wstring &cmdline, bool current_user_only = true) :
		m_appname (appname),
		m_cmdline (cmdline),
		m_autorunKey (nullptr)
	{
		open_autorun_key (current_user_only);
	}

	~vmsWinAppAutorun ()
	{
		if (m_autorunKey)
			RegCloseKey (m_autorunKey);
	}

	virtual bool autorun_enabled () const override
	{
		DWORD dwType;
		return ERROR_SUCCESS == RegQueryValueEx (m_autorunKey, m_appname.c_str (), 
			nullptr, &dwType, nullptr, nullptr);
	}

	virtual bool enable_autorun (bool enable) override
	{
		if (enable)
		{
			return ERROR_SUCCESS == RegSetValueEx (m_autorunKey, m_appname.c_str (), 0, REG_SZ, 
				(CONST BYTE*) m_cmdline.c_str (),
				(DWORD)(m_cmdline.length () + 1) * sizeof (WCHAR));
		}
		else
		{
			return ERROR_SUCCESS == RegDeleteValue (m_autorunKey, m_appname.c_str ());
		}
	}

protected:
	std::wstring m_appname;
	std::wstring m_cmdline;
	HKEY m_autorunKey;

protected:
	bool open_autorun_key (bool current_user_only)
	{
		return ERROR_SUCCESS == RegOpenKeyEx (current_user_only ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
			_T ("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_READ | KEY_WRITE,
			&m_autorunKey);
	}
};
