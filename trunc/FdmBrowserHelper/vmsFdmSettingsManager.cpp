/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmSettingsManager.h"

vmsFdmSettingsManager::vmsFdmSettingsManager ()
{
	verify (ERROR_SUCCESS == m_fdmKey.Open (HKEY_CURRENT_USER,
		_T ("Software\\FreeDownloadManager.ORG\\Free Download Manager"), 
		KEY_READ));

	verify (ERROR_SUCCESS == m_fdmBrMonitorKey.Open (m_fdmKey, 
		_T ("Settings\\Monitor"), KEY_READ));

	verify (ERROR_SUCCESS == m_fdmBrMenuKey.Open (m_fdmKey, 
		_T("Settings\\Monitor\\IEMenu"), KEY_READ));
}

vmsFdmSettingsManager::~vmsFdmSettingsManager ()
{
}

void vmsFdmSettingsManager::ReadSettings ()
{
	vmsThreadSafe4Scope;
	ReadBrowserMenuSettings_ ();
	ReadBrowserMonitorSettings_ ();
}

void vmsFdmSettingsManager::ReadBrowserMenuSettings_ ()
{
	struct menu_item
	{
		std::wstring name;
		std::wstring mutex_name;
		bool *setting;
	};

	const menu_item items [] = {
		{L"DLThis", L"mx::fdm: dllink_disable", &m_settings.m_browser.m_menu.m_dllink},
		{L"DLAll", L"mx::fdm: dlall_disable", &m_settings.m_browser.m_menu.m_dlall},
		{L"DLSelected", L"mx::fdm: dlselected_disable", &m_settings.m_browser.m_menu.m_dlselected},
		{L"DLFlashVideo", L"mx::fdm: dlvideo_disable", &m_settings.m_browser.m_menu.m_dlvideo}
	};

	DWORD dwShow = TRUE;

	HANDLE hmx = CreateMutex (NULL, FALSE, _T ("mx::fdm: brmenu_disable"));
	assert (hmx);
	if (GetLastError () == ERROR_ALREADY_EXISTS)
		dwShow = FALSE;
	else
		m_fdmBrMenuKey.QueryDWORDValue (_T ("Enable"), dwShow);
	CloseHandle (hmx);

	for (const auto &item : items)
	{
		if (!dwShow)
		{
			*item.setting = false;
			continue;
		}

		DWORD dwShowItem;
		HANDLE hmx = CreateMutex (NULL, FALSE, item.mutex_name.c_str ());
		assert (hmx);
		if (GetLastError () == ERROR_ALREADY_EXISTS)
			dwShowItem = FALSE;
		else
			m_fdmBrMenuKey.QueryDWORDValue (item.name.c_str (), dwShowItem);
		CloseHandle (hmx);

		*item.setting = dwShowItem != FALSE;
	}
}

void vmsFdmSettingsManager::ReadBrowserMonitorSettings_ ()
{
	DWORD dw = TRUE;
	m_fdmBrMonitorKey.QueryDWORDValue (m_browser.c_str (), dw);
	m_settings.m_browser.m_monitor.m_enable = dw != FALSE;

	dw = FALSE;
	m_fdmBrMonitorKey.QueryDWORDValue (_T ("ALTShouldPressed"), dw);
	m_settings.m_browser.m_monitor.m_need_alt = dw != FALSE;

	dw = TRUE;
	m_fdmBrMonitorKey.QueryDWORDValue (_T ("AllowDownload"), dw);
	m_settings.m_browser.m_monitor.m_allowDownload = dw != FALSE;

	TCHAR sz [10000] = _T("pls m3u"); dw = _countof (sz);
	m_fdmBrMonitorKey.QueryStringValue (_T ("SkipExtensions"), sz, &dw);
	m_settings.m_browser.m_monitor.m_skipExtensions = sz;

	*sz = 0; dw = _countof (sz);
	m_fdmBrMonitorKey.QueryStringValue (_T ("SkipServers"), sz, &dw);
	m_settings.m_browser.m_monitor.m_skipServers = sz;

	ReadBrowserMonitorVideoSettings_ ();
}

void vmsFdmSettingsManager::ReadBrowserMonitorVideoSettings_ ()
{
	DWORD dwEnable = TRUE;
	DWORD dwProcessList = 0xffffffff;
	DWORD dwShowButton = TRUE;

	CRegKey key;

	if (ERROR_SUCCESS == key.Open (m_fdmKey, _T ("Settings\\FlvMonitoring"), KEY_READ))
	{
		key.QueryDWORDValue (_T ("Enable"), dwEnable);
		key.QueryDWORDValue (_T("ProcessList"), dwProcessList);
		key.QueryDWORDValue (_T ("ShowDownloadItBtn"), dwShowButton);
	}

	m_settings.m_browser.m_monitor.m_video.m_enable = dwEnable != FALSE;
	m_settings.m_browser.m_monitor.m_video.m_enableSnifferDll = false;
	m_settings.m_browser.m_monitor.m_video.m_showButton = dwShowButton != FALSE;

	if (!m_settings.m_browser.m_monitor.m_video.m_enable)
		return;

	static const auto browsersFlags = {
		std::make_pair (L"IE", 1),
		std::make_pair (L"Firefox", 1<<1),
		std::make_pair (L"Opera", 1<<2),
		std::make_pair (L"Netscape", 1<<3),
		std::make_pair (L"SeaMonkey", 1<<4),
		std::make_pair (L"Safari", 1<<5),
		std::make_pair (L"Chrome", 1<<6)
	};

	for (const auto &bf : browsersFlags)
	{
		if (bf.first == m_browser)
		{
			m_settings.m_browser.m_monitor.m_video.m_enableSnifferDll = 
				(dwProcessList & bf.second) != 0;
			break;
		}
	}
}
