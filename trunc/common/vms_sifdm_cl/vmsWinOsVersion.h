/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "os\vmsOsVersion.h"
#include "win\version.h"
class vmsWinOsVersion : 
	public vmsOsVersionBase
{
public:
	vmsWinOsVersion ()
	{
		ZeroMemory (&m_ver, sizeof (m_ver));
		m_ver.dwOSVersionInfoSize = sizeof (m_ver);
		if (!vmsGetVersion ((LPOSVERSIONINFO)&m_ver))
			m_ver.dwMajorVersion = 0; 
		assert (m_ver.dwMajorVersion != 0);
	}

	const OSVERSIONINFOEX* getRawVersion () const {return &m_ver;}

	bool isValid () const {return m_ver.dwMajorVersion != 0;}

	bool isVistaOrHigher () const {return m_ver.dwMajorVersion >= 6;}
	bool isWin7OrHigher () const {return m_ver.dwMajorVersion >= 7 || (m_ver.dwMajorVersion == 6 && m_ver.dwMinorVersion >= 1);}
	bool isWin8OrHigher () const {return m_ver.dwMajorVersion >= 7 || (m_ver.dwMajorVersion == 6 && m_ver.dwMinorVersion >= 2);}

	std::string os_name () const override {return "Windows NT";}
	virtual unsigned major_version () const override {return m_ver.dwMajorVersion;} 
	virtual unsigned minor_version () const override {return m_ver.dwMinorVersion;}
	virtual unsigned build_number () const override {return m_ver.dwBuildNumber;}
	
protected:
	OSVERSIONINFOEX m_ver;
};

