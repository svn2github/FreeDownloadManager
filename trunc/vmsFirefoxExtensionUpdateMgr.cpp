/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFirefoxExtensionUpdateMgr.h"
#include "vmsAppFileUpdateInfo.h"
#include "common/vmsFirefoxExtensionInstaller.h"
#include "vmsSecurity.h"
#include "FdmApp.h"

vmsFirefoxExtensionUpdateMgr::vmsFirefoxExtensionUpdateMgr(bool currentBuildFirstRun) :
	m_bExtensionPathChanged (false)
{
	LoadState (currentBuildFirstRun);
}

vmsFirefoxExtensionUpdateMgr::~vmsFirefoxExtensionUpdateMgr(void)
{
}

LPCTSTR vmsFirefoxExtensionUpdateMgr::getExtensionPath(void)
{
	return m_tstrExtensionPath.c_str ();
}

void vmsFirefoxExtensionUpdateMgr::LoadState(bool currentBuildFirstRun)
{
	vmsAppSettingsStore *pstgs = _App.get_SettingsStore ();

	tstring extensionPath = pstgs->GetProfileString (_T ("Settings\\Update\\FirefoxExt"), _T ("Path"), _T (""));
	vmsAppVersion extensionVersion;
	extensionVersion.FromString (pstgs->GetProfileString (_T ("Settings\\Update\\FirefoxExt"), _T ("Version"), _T ("")));

	if (!currentBuildFirstRun)
	{
		m_tstrExtensionPath = extensionPath;
		m_extensionVersion = extensionVersion;

		if (IS_PORTABLE_MODE && !m_tstrExtensionPath.empty ())
		{
			TCHAR tsz [MAX_PATH] = _T ("");
			GetModuleFileName (NULL, tsz, _countof (tsz));
			m_tstrExtensionPath [0] = tsz [0]; 
		}
	}
	

	if (m_tstrExtensionPath.empty () || m_extensionVersion.empty () || GetFileAttributes (m_tstrExtensionPath.c_str ()) == DWORD (-1))
	{
		LocateFirefoxExtension ();

		if (!m_tstrExtensionPath.empty ())
		{
			pstgs->WriteProfileString (_T ("Settings\\Update\\FirefoxExt"), _T ("Path"), m_tstrExtensionPath.c_str ());
			pstgs->WriteProfileString (_T ("Settings\\Update\\FirefoxExt"), _T ("Version"), m_extensionVersion.ToString ().c_str ());

			if (extensionPath != m_tstrExtensionPath)
				m_bExtensionPathChanged = true;
		}
	}
}

void vmsFirefoxExtensionUpdateMgr::LocateFirefoxExtension(void)
{
	vmsAppVersion verMax;
	tstring tstrExtensionPath;
	tstring tstrPath, tstr;

	m_tstrExtensionPath = _T ("");
	m_extensionVersion.clear ();

	

	TCHAR tszExeDir [MY_MAX_PATH], tszExeFile [MY_MAX_PATH];
	GetModuleFileName (NULL, tszExeFile, sizeof (tszExeFile));
	fsGetPath (tszExeFile, tszExeDir);
	tstrPath = tszExeDir;
	tstrPath += _T ("Firefox\\Extension");
	tstr = tstrPath + _T ("\\install.rdf");
	tstring tstrVer;
	if (vmsFirefoxExtensionInstaller::ExtractExtensionInfo (tstr.c_str (), tstrVer))
	{
		verMax.FromString (tstrVer.c_str ());
		tstrExtensionPath = tstrPath;
	}

	if (tstrExtensionPath.empty ())
	{
		vmsAppSettingsStore *pstgs = _App.get_SettingsStore ();
		tstrPath = pstgs->GetProfileString (_T (""), _T("Path"), _T (""));
		if (!tstrPath.empty ())
		{
			tstrPath += _T ("\\Firefox\\Extension");
			tstr = tstrPath + _T ("\\install.rdf");
			if (vmsFirefoxExtensionInstaller::ExtractExtensionInfo (tstr.c_str (), tstrVer))
			{
				verMax.FromString (tstrVer.c_str ());
				tstrExtensionPath = tstrPath;
			}
		}
	}

	

	GetExtensionsInstallationPath (tstrPath);

	tstr = tstrPath + _T ("\\*.*");

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile (tstr.c_str (), &wfd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile (hFind, &wfd))
		{
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			if (!_tcscmp (wfd.cFileName, _T ("..")) || _tcscmp (wfd.cFileName, _T (".")))
				continue;
			vmsAppVersion ver;
			ver.FromString (wfd.cFileName);
			if (ver.empty ())
				continue;
			if (verMax.CompareVersions (ver, vmsAppVersion::CT_FLOATING) < 0)
			{
				verMax = ver;
				tstrExtensionPath = tstrPath + _T ("\\") + wfd.cFileName;
			}
		}

		FindClose (hFind);
	}

	if (!tstrExtensionPath.empty ())
	{
		m_tstrExtensionPath = tstrExtensionPath;
		m_extensionVersion = verMax;
	}
}

void vmsFirefoxExtensionUpdateMgr::GetExtensionsInstallationPath(tstring& tstrResult)
{
	TCHAR tszPath [MAX_PATH] = _T ("");

	if (IS_PORTABLE_MODE)
	{
		GetModuleFileName (NULL, tszPath, _countof (tszPath));
		assert (_tcsrchr (tszPath, '\\') != NULL);
		_tcscpy (_tcsrchr (tszPath, '\\'), _T (""));
	}
	else
	{
		SHGetFolderPath (NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, tszPath);
		if (!*tszPath)
			SHGetFolderPath (NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, tszPath);
		_tcscat (tszPath, _T ("\\Free Download Manager"));
	}

	assert (*tszPath);

	tstrResult = tszPath;
	tstrResult += _T ("\\Firefox\\Extensions");
}

bool vmsFirefoxExtensionUpdateMgr::CheckForUpdates(void)
{
	LOGFN ("vmsFirefoxExtensionUpdateMgr::CheckForUpdates");
	m_tstrExtensionNewVerURL = _T ("");
	vmsAppFileUpdateInfo fui (_T ("http://up.freedownloadmanager.org/update/ffext.xml"), _AppMgr.getBuildNumber ());
	if (!fui.Retrieve ())
		return false;
	if (fui.getResult ()->tstrFileURL.empty ())
		return true;
	vmsAppVersion ver; 
	ver.FromString (fui.getResult ()->tstrVersion.c_str ());
	LOG ("new version: %s", stringFromTstring (fui.getResult ()->tstrVersion).c_str ());
	if (ver.CompareVersions (m_extensionVersion, vmsAppVersion::CT_FLOATING) > 0)
	{
		m_tstrExtensionNewVerURL = fui.getResult ()->tstrFileURL;
		m_extensionNewVer = ver;
	}
	return true;
}

bool vmsFirefoxExtensionUpdateMgr::PerformUpdate(void)
{
	if (m_tstrExtensionNewVerURL.empty ())
		return false;

	vmsTmpFileName tmpFile;
	HRESULT hr = URLDownloadToFile (NULL, m_tstrExtensionNewVerURL.c_str (), tmpFile, 0, NULL);
	if (FAILED (hr))
		return false;

	TCHAR tszExeDir [MY_MAX_PATH], tszExeFile [MY_MAX_PATH];
	GetModuleFileName (NULL, tszExeFile, sizeof (tszExeFile));
	fsGetPath (tszExeFile, tszExeDir);
	tstring tstrKeyFile = tszExeDir;
	tstrKeyFile += _T ("sigkey.dat");
	if (!vmsSecurity::VerifySign (tmpFile, tstrKeyFile.c_str ()))
	{
		m_tstrExtensionNewVerURL = _T ("");
		return false;
	}

	tstring tstr;
	GetExtensionsInstallationPath (tstr);

	tstr += '\\';
	tstr += m_extensionNewVer.ToString ();

	fsBuildPathToFile ((tstr + _T ("\\1")).c_str ());

	vmsUnZip unzip;
	vmsError err;
	if (FALSE == unzip.Unpack (tmpFile, tstr.c_str (), err))
		return false;

	if (!AdjustInstallConfig(tstr))
		return false;

	m_tstrExtensionPath = tstr;
	m_extensionVersion = m_extensionNewVer;

	vmsAppSettingsStore *pstgs = _App.get_SettingsStore ();
	pstgs->WriteProfileString (_T ("Settings\\Update\\FirefoxExt"), _T ("Path"), m_tstrExtensionPath.c_str ());
	pstgs->WriteProfileString (_T ("Settings\\Update\\FirefoxExt"), _T ("Version"), m_extensionVersion.ToString ().c_str ());

	m_tstrExtensionNewVerURL = _T ("");
	m_extensionNewVer.clear ();	

	return true;
}

bool vmsFirefoxExtensionUpdateMgr::AdjustInstallConfig(const std::wstring& installationPath)
{
	std::wstring rdfPath = installationPath;
	rdfPath += _T("\\install.rdf");
	if (GetFileAttributes(rdfPath.c_str()) == DWORD(-1))
		return false;

	
	HANDLE hFile = CreateFile(rdfPath.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	SYSTEMTIME st;
	GetSystemTime(&st);
	
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	
	BOOL res = SetFileTime(hFile,
		(LPFILETIME)NULL, 
		&ft,	
		&ft);	

	return res == TRUE;
}