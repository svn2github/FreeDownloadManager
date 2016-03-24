/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../win/vmsBinaryFileVersionInfo.h"
class vmsRunningAppVersionInfo
{
public:
	vmsRunningAppVersionInfo ()
	{
		TCHAR tsz [MAX_PATH];
		GetModuleFileName (NULL, tsz, MAX_PATH);
		m_bfvi.Retrieve (tsz);
	}

	unsigned getBuildNumber () const
	{
		return getVersion ()->m_appVersion.size () > 2 ? getVersion ()->m_appVersion [2].dwVal : 0;
	}

	const vmsBinaryFileVersionInfo* getVersion () const
	{
		return &m_bfvi;
	}

	std::wstring getBuildNumberAsString () const
	{
		return getVersion ()->m_appVersion.size () > 2 ? getVersion ()->m_appVersion [2].toString () : _T ("???");
	}

protected:
	vmsBinaryFileVersionInfo m_bfvi;
};