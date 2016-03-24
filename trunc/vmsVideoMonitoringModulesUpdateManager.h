/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsAppFileUpdateInfo.h"
class vmsVideoMonitoringModulesUpdateManager
{
public:
	vmsVideoMonitoringModulesUpdateManager ()
	{
		read_last_check_time ();
		read_new_ver_to_skip ();
	}

	bool check_for_updates ()
	{
		m_newVersionExists = false;

		update_last_check_time ();

		vmsBinaryFileVersionInfo bfviSniffer;
		if (!bfviSniffer.Retrieve ((vmsGetModulePath () + L"\\flvsniff.dll").c_str ()))
			return false;

		vmsAppFileUpdateInfo fui (
			_T ("http://up.freedownloadmanager.org/update/videomon.xml"), 
			_AppMgr.getBuildNumber ());

		if (!fui.Retrieve ())
			return false;

		m_newVersion = fui.getResult ()->tstrVersion;

		if (!m_newVersion.empty () &&
			m_newVersion != m_newVersionToSkip)
		{
			vmsAppVersion verNew;
			verNew.FromString (m_newVersion.c_str ());

			m_newVersionExists = verNew.CompareVersions (
				bfviSniffer.m_fileVersion, vmsAppVersion::CT_FLOATING) > 0;
		}

		return true;
	}

	bool new_version_exists () const
	{
		return m_newVersionExists;
	}

	bool checked_for_updates_recently () const
	{
		return m_lastCheckTime &&
			std::difftime (time (nullptr), m_lastCheckTime) < 24*3600;
	}

	void skip_this_update ()
	{
		assert (m_newVersionExists);
		assert (!m_newVersion.empty ());

		m_newVersionToSkip = m_newVersion;

		_App.get_SettingsStore ()->WriteProfileString (
			_T ("Settings\\Update"), _T ("VideomonSkipVer"), m_newVersion.c_str ());
	}

protected:
	bool m_newVersionExists = false;
	tstring m_newVersion;
	time_t m_lastCheckTime = 0;
	tstring m_newVersionToSkip;

protected:
	void read_last_check_time ()
	{
		m_lastCheckTime = _App.get_SettingsStore ()->GetProfileUint64 (
			_T ("Settings\\Update"), _T ("LastVideomonCheck"), 0);
	}

	void update_last_check_time ()
	{
		time (&m_lastCheckTime);
		_App.get_SettingsStore ()->WriteProfileUint64 (
			_T ("Settings\\Update"), _T ("LastVideomonCheck"), m_lastCheckTime);
	}

	void read_new_ver_to_skip ()
	{
		m_newVersionToSkip = _App.get_SettingsStore ()->GetProfileString (
			_T ("Settings\\Update"), _T ("VideomonSkipVer"), L"");
	}
};

