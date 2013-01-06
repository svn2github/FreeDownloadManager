/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsOpNetIntegrationMgr.h"
#include "FolderBrowser.h"
#include "vmsCommandLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsOpNetIntegrationMgr::fsOpNetIntegrationMgr()
{

}

fsOpNetIntegrationMgr::~fsOpNetIntegrationMgr()
{

}

void fsOpNetIntegrationMgr::Initialize()
{
	CRegKey key;
	
	CString strOpKey = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Opera.exe";
	
	CString strNetKey1 = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscp.exe";
	CString strNetKey2 = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscp6.exe";
	CString strNetKey3 = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscape.exe";
	CString strNetKey4 = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\navigator.exe";
	
	CString strFfKey = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Firefox.exe";
	
	CString strMozSKey1 = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\SeaMonkey.exe";
	CString strMozSKey2 = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\mozilla.exe";

	if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, "Software\\Opera Software", KEY_READ))
	{
		char szPath [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szPath);
		if (ERROR_SUCCESS == key.QueryValue (szPath, "Plugin Path", &dw))
		{
			if (*szPath)
			{
				m_strOpPath = szPath;
				if (m_strOpPath [m_strOpPath.GetLength () - 1] != '\\' || m_strOpPath [m_strOpPath.GetLength () - 1] != '/')
					m_strOpPath += '\\';
			}
		}
	}

	if (m_strOpPath == "" && ERROR_SUCCESS == key.Open (HKEY_CLASSES_ROOT, 
			"Applications\\Opera.exe\\shell\\open\\command", KEY_READ))
	{
		char sz [MY_MAX_PATH] = "";
		DWORD dw = sizeof (sz);
		key.QueryValue (sz, NULL, &dw);
		vmsCommandLine cl;
		cl.fromString (sz, true);
		if (cl.getExe () != NULL && *cl.getExe () != 0)
		{
			*sz = 0;
			fsGetPath (cl.getExe (), sz);
			if (*sz)
			{
				m_strOpPath = sz;
				if (m_strOpPath [m_strOpPath.GetLength () - 1] != '\\' || m_strOpPath [m_strOpPath.GetLength () - 1] != '/')
					m_strOpPath += '\\';
				if (GetFileAttributes (m_strOpPath + "program\\plugins") != DWORD (-1))
					m_strOpPath += "program\\plugins\\";
				else
					m_strOpPath += "Plugins\\";
			}
		}
	}

	if (m_strOpPath == "" && ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strOpKey, KEY_READ))
	{
		

		char szPath [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		if (*szPath)
		{
			m_strOpPath = szPath;
			if (m_strOpPath [m_strOpPath.GetLength () - 1] != '\\' || m_strOpPath [m_strOpPath.GetLength () - 1] != '/')
				m_strOpPath += '\\';
			if (GetFileAttributes (m_strOpPath + "program\\plugins") != DWORD (-1))
				m_strOpPath += "program\\plugins\\";	
			else
				m_strOpPath += "Plugins\\";
		}
		key.Close ();
	}
	
	if (m_strOpPath == "")
	{
		
		
		
		m_strOpPath = _App.Monitor_OperaPDInstalledTo ();
	}

	
	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey1, KEY_READ) ||
		 ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey2, KEY_READ) || 
		 ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey3, KEY_READ) ||
		 ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strNetKey4, KEY_READ))
	{
		char szPath [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		if (*szPath)
		{
			m_strNetPath = szPath;
			if (m_strNetPath [m_strNetPath.GetLength () - 1] != '\\' || m_strNetPath [m_strNetPath.GetLength () - 1] != '/')
				m_strNetPath += '\\';
			m_strNetPath += "Plugins\\";
		}
		key.Close ();

		if (m_strNetPath == m_strOpPath)
			m_strNetPath = _App.Monitor_NetscapePDInstalledTo ();
	}
	else
	{
		m_strNetPath = _App.Monitor_NetscapePDInstalledTo ();
	}

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strFfKey, KEY_READ))
	{
		

		char szPath [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		if (*szPath)
		{
			m_strFfPath = szPath;
			if (m_strFfPath [m_strFfPath.GetLength () - 1] != '\\' || m_strFfPath [m_strFfPath.GetLength () - 1] != '/')
				m_strFfPath += '\\';
			m_strFfPath += "Plugins\\";
		}
		key.Close ();
	}
	else
	{
		m_strFfPath = _App.Monitor_FirefoxPDInstalledTo ();
	}

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strMozSKey1, KEY_READ) ||
			ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strMozSKey2, KEY_READ))
	{
		

		char szPath [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		if (*szPath)
		{
			m_strMozSPath = szPath;
			if (m_strMozSPath [m_strMozSPath.GetLength () - 1] != '\\' || m_strMozSPath [m_strMozSPath.GetLength () - 1] != '/')
				m_strMozSPath += '\\';
			m_strMozSPath += "Plugins\\";
		}
		key.Close ();
	}
	else
	{
		m_strMozSPath = _App.Monitor_MozillaSuitePDInstalledTo ();
	}

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, "Software\\Clients\\StartMenuInternet\\Safari.exe\\shell\\open\\command", KEY_READ))
	{
		char szCmd [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szCmd);
		key.QueryValue (szCmd, NULL, &dw);
		if (*szCmd)
		{
			LPSTR psz = szCmd;
			if (*psz == '"')
			{
				LPSTR psz2 = strchr (psz+1, '"');
				if (psz2)
					*psz2 = 0;
				psz++;
			}
			else
			{
				LPSTR psz2 = strchr (psz, ' ');
				if (psz2)
					*psz2 = 0;
			}
			LPSTR psz3 = strrchr (psz, '\\');
			if (psz3)
				psz3 [1] = 0;
			strcat (psz, "Plugins\\");
			m_strSafariPath = psz;
		}
	}
	else
	{
		m_strSafariPath = _App.Monitor_SafariPDInstalledTo ();
	}

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, "Software\\Clients\\StartMenuInternet\\chrome.exe\\shell\\open\\command", KEY_READ) || 
		ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, "Software\\Clients\\StartMenuInternet\\Google Chrome\\shell\\open\\command", KEY_READ))
	{
		char szCmd [MY_MAX_PATH] = "";
		DWORD dw = sizeof (szCmd);
		key.QueryValue (szCmd, NULL, &dw);
		if (*szCmd)
		{
			LPSTR psz = szCmd;
			if (*psz == '"')
			{
				LPSTR psz2 = strchr (psz+1, '"');
				if (psz2)
					*psz2 = 0;
				psz++;
			}
			else
			{
				LPSTR psz2 = strchr (psz, ' ');
				if (psz2)
					*psz2 = 0;
			}
			LPSTR psz3 = strrchr (psz, '\\');
			if (psz3)
				psz3 [1] = 0;
			strcat (psz, "Plugins\\");
			m_strChromePath = psz;
		}
	}
	else
	{
		LPITEMIDLIST pidl = NULL;
		if (SUCCEEDED (SHGetSpecialFolderLocation (NULL, CSIDL_LOCAL_APPDATA, &pidl)))
		{
			TCHAR tszPath [MY_MAX_PATH] = _T ("");
			SHGetPathFromIDList (pidl, tszPath);
			if (tszPath [_tcslen (tszPath)-1] != '\\')
				_tcscat (tszPath, _T ("\\"));
			_tcscat (tszPath, _T ("Google\\Chrome\\Application"));
			if (GetFileAttributes (tszPath) != DWORD (-1))
			{
				_tcscat (tszPath, _T ("\\Plugins\\"));
				m_strChromePath = tszPath;
			}

			IMallocPtr spMalloc;
			SHGetMalloc (&spMalloc);
			spMalloc->Free (pidl);
		}

		if (m_strChromePath.IsEmpty ())		
			m_strChromePath = _App.Monitor_ChromePDInstalledTo ();
	}
}

BOOL fsOpNetIntegrationMgr::IsOperaPluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strOpPath == "")
	{
		Initialize ();
		if (m_strOpPath.GetLength ())
			return IsOperaPluginInstalled (bQueryPluginDirIfUnknown);

		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;

		if (MessageBox (NULL, LS (L_CANTFINDOPERADIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;

		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;

		m_strOpPath = fb->GetPath ();

		if (m_strOpPath [m_strOpPath.GetLength () - 1] != '\\' || m_strOpPath [m_strOpPath.GetLength () - 1] != '/')
			m_strOpPath += '\\';

		
		
		if (strnicmp (LPCSTR (m_strOpPath) + m_strOpPath.GetLength () - 8, "Plugins\\", 8))
		{
			if (GetFileAttributes (m_strOpPath + "program\\plugins") != DWORD (-1))
				m_strOpPath += "program\\plugins\\";	
			else
				m_strOpPath += "Plugins\\";
		}
	}

	CString str = m_strOpPath;
	str += "npfdm.dll";
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallOperaPlugin()
{
	if (m_strOpPath == "")
		return FALSE;

	CString str = m_strOpPath;
	str += "npfdm.dll";
	if (!fsBuildPathToFile (str))
		return FALSE;

	
	_App.Monitor_OperaPDInstalledTo (m_strOpPath);

	return CopyFile ("npfdm.dll", str, FALSE);
}

BOOL fsOpNetIntegrationMgr::DeinstallOperaPlugin()
{
	if (m_strOpPath == "")
		return FALSE;

	_App.Monitor_OperaPDInstalledTo ("");

	CString str = m_strOpPath;
	str += "npfdm.dll";

	m_strOpPath = "";

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsNetscapePluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strNetPath == "")
	{
		Initialize ();
		if (m_strNetPath.GetLength ())
			return IsNetscapePluginInstalled (bQueryPluginDirIfUnknown);

		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;

		if (MessageBox (NULL, LS (L_CANTFINDNETSCAPEDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;

		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;

		m_strNetPath = fb->GetPath ();
		if (m_strNetPath [m_strNetPath.GetLength () - 1] != '\\' || m_strNetPath [m_strNetPath.GetLength () - 1] != '/')
			m_strNetPath += '\\';
		if (strnicmp (LPCSTR (m_strNetPath)  + m_strNetPath.GetLength () - 8, "Plugins\\", 8))
			m_strNetPath += "Plugins\\";
	}

	CString str = m_strNetPath;
	str += "npfdm.dll";
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallNetscapePlugin()
{
	if (m_strNetPath == "")
		return FALSE;

	CString str = m_strNetPath;
	str += "npfdm.dll";
	if (!fsBuildPathToFile (str))
		return FALSE;

	_App.Monitor_NetscapePDInstalledTo (m_strNetPath);

	return CopyFile ("npfdm.dll", str, FALSE);
}

BOOL fsOpNetIntegrationMgr::DeinstallNetscapePlugin()
{
	if (m_strNetPath == "")
		return FALSE;

	_App.Monitor_NetscapePDInstalledTo ("");

	CString str = m_strNetPath;
	str += "npfdm.dll";

	m_strNetPath = "";

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsFirefoxPluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strFfPath == "")
	{
		Initialize ();
		if (m_strFfPath.GetLength ())
			return IsFirefoxPluginInstalled (bQueryPluginDirIfUnknown);

		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;

		if (MessageBox (NULL, LS (L_CANTFINDFIREFOXDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;

		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;

		m_strFfPath = fb->GetPath ();

		if (m_strFfPath [m_strFfPath.GetLength () - 1] != '\\' || m_strFfPath [m_strFfPath.GetLength () - 1] != '/')
			m_strFfPath += '\\';

		
		
		if (strnicmp (LPCSTR (m_strFfPath) + m_strFfPath.GetLength () - 8, "Plugins\\", 8))
			m_strFfPath += "Plugins\\";
	}

	CString str = m_strFfPath;
	str += "npfdm.dll";
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallFirefoxPlugin()
{
	if (m_strFfPath == "")
		return FALSE;

	CString str = m_strFfPath;
	str += "npfdm.dll";
	if (!fsBuildPathToFile (str))
		return FALSE;
	if (CopyFile ("npfdm.dll", str, FALSE))
	{
		
		_App.Monitor_FirefoxPDInstalledTo (m_strFfPath);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL fsOpNetIntegrationMgr::DeinstallFirefoxPlugin()
{
	if (m_strFfPath == "")
		return FALSE;

	_App.Monitor_FirefoxPDInstalledTo ("");

	CString str = m_strFfPath;
	str += "npfdm.dll";

	m_strFfPath = "";

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsMozillaSuitePluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strMozSPath == "")
	{
		Initialize ();
		if (m_strMozSPath.GetLength ())
			return IsMozillaSuitePluginInstalled (bQueryPluginDirIfUnknown);
		return FALSE;
	}

	CString str = m_strMozSPath;
	str += "npfdm.dll";
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallMozillaSuitePlugin()
{
	if (m_strMozSPath == "")
		return FALSE;

	CString str = m_strMozSPath;
	str += "npfdm.dll";
	if (!fsBuildPathToFile (str))
		return FALSE;

	
	_App.Monitor_MozillaSuitePDInstalledTo (m_strMozSPath);

	return CopyFile ("npfdm.dll", str, FALSE);
}

BOOL fsOpNetIntegrationMgr::DeinstallMozillaSuitePlugin()
{
	if (m_strMozSPath == "")
		return FALSE;

	_App.Monitor_MozillaSuitePDInstalledTo ("");

	CString str = m_strMozSPath;
	str += "npfdm.dll";

	m_strMozSPath = "";

	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsSafariPluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strSafariPath == "")
	{
		Initialize ();
		if (m_strSafariPath.GetLength ())
			return IsSafariPluginInstalled (bQueryPluginDirIfUnknown);
		
		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;
		
		if (MessageBox (NULL, LS (L_CANTFINDSAFARIDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;
		
		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;
		
		m_strSafariPath = fb->GetPath ();
		
		if (m_strSafariPath [m_strSafariPath.GetLength () - 1] != '\\' || m_strSafariPath [m_strSafariPath.GetLength () - 1] != '/')
			m_strSafariPath += '\\';
		
		
		
		if (strnicmp (LPCSTR (m_strSafariPath) + m_strSafariPath.GetLength () - 8, "Plugins\\", 8))
				m_strSafariPath += "Plugins\\";
	}
	
	CString str = m_strSafariPath;
	str += "npfdm.dll";
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallSafariPlugin()
{
	if (m_strSafariPath == "")
		return FALSE;
	
	CString str = m_strSafariPath;
	str += "npfdm.dll";
	if (!fsBuildPathToFile (str))
		return FALSE;

	_App.Monitor_SafariPDInstalledTo (m_strSafariPath);

	return CopyFile ("npfdm.dll", str, FALSE);
}

BOOL fsOpNetIntegrationMgr::DeinstallSafariPlugin()
{
	if (m_strSafariPath == "")
		return FALSE;
	
	_App.Monitor_SafariPDInstalledTo ("");
	
	CString str = m_strSafariPath;
	str += "npfdm.dll";
	
	m_strSafariPath = "";
	
	return DeleteFile (str);
}

BOOL fsOpNetIntegrationMgr::IsChromePluginInstalled(BOOL bQueryPluginDirIfUnknown)
{
	if (m_strChromePath == "")
	{
		Initialize ();
		if (m_strChromePath.GetLength ())
			return IsChromePluginInstalled (bQueryPluginDirIfUnknown);
		
		if (bQueryPluginDirIfUnknown == FALSE)
			return FALSE;
		
		if (MessageBox (NULL, LS (L_CANTFINDCHROMEDIR), vmsFdmAppMgr::getAppName (), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return FALSE;
		
		CFolderBrowser *fb = CFolderBrowser::Create (LS (L_CHOOSEOUTFOLDER), NULL, NULL, NULL);
		if (fb == NULL)
			return FALSE;
		
		m_strChromePath = fb->GetPath ();
		
		if (m_strChromePath [m_strChromePath.GetLength () - 1] != '\\' || m_strChromePath [m_strChromePath.GetLength () - 1] != '/')
			m_strChromePath += '\\';
		
		
		
		if (strnicmp (LPCSTR (m_strChromePath) + m_strChromePath.GetLength () - 8, "Plugins\\", 8))
			m_strChromePath += "Plugins\\";
	}
	
	CString str = m_strChromePath;
	str += "npfdm.dll";
	if (GetFileAttributes (str) == DWORD (-1))
		return FALSE;
	else
		return TRUE;
}

BOOL fsOpNetIntegrationMgr::InstallChromePlugin()
{
	if (m_strChromePath == "")
		return FALSE;
	
	CString str = m_strChromePath;
	str += "npfdm.dll";
	if (!fsBuildPathToFile (str))
		return FALSE;

	_App.Monitor_ChromePDInstalledTo (m_strChromePath);

	return CopyFile ("npfdm.dll", str, FALSE);
}

BOOL fsOpNetIntegrationMgr::DeinstallChromePlugin()
{
	if (m_strChromePath == "")
		return FALSE;
	
	_App.Monitor_ChromePDInstalledTo ("");
	
	CString str = m_strChromePath;
	str += "npfdm.dll";
	
	m_strChromePath = "";
	
	return DeleteFile (str);
}

LPCSTR fsOpNetIntegrationMgr::getOperaPluginPath() const
{
	return m_strOpPath;
}

LPCSTR fsOpNetIntegrationMgr::getNetscapePluginPath() const
{
	return m_strNetPath;
}

LPCSTR fsOpNetIntegrationMgr::getMozillaSuitePluginPath() const
{
	return m_strMozSPath;
}

LPCSTR fsOpNetIntegrationMgr::getSafariPluginPath() const
{
	return m_strSafariPath;
}

LPCSTR fsOpNetIntegrationMgr::getChromePluginPath() const
{
	return m_strChromePath;
}

