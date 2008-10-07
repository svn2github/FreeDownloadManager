/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsOpNetIntegrationMgr.h"
#include "FolderBrowser.h"

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

	if (m_strOpPath == "" && ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strOpKey, KEY_READ))
	{
		

		char szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		m_strOpPath = szPath;
		if (m_strOpPath [m_strOpPath.GetLength () - 1] != '\\' || m_strOpPath [m_strOpPath.GetLength () - 1] != '/')
			m_strOpPath += '\\';
		if (GetFileAttributes (m_strOpPath + "program\\plugins") != DWORD (-1))
			m_strOpPath += "program\\plugins\\";	
		else
			m_strOpPath += "Plugins\\";
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
		char szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		m_strNetPath = szPath;
		if (m_strNetPath [m_strNetPath.GetLength () - 1] != '\\' || m_strNetPath [m_strNetPath.GetLength () - 1] != '/')
			m_strNetPath += '\\';
		m_strNetPath += "Plugins\\";
		key.Close ();

		if (m_strNetPath == m_strOpPath)
			m_strNetPath = _App.Monitor_NetscapePDInstalledTo ();
	}
	else
		m_strNetPath = _App.Monitor_NetscapePDInstalledTo ();

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strFfKey, KEY_READ))
	{
		

		char szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		m_strFfPath = szPath;
		if (m_strFfPath [m_strFfPath.GetLength () - 1] != '\\' || m_strFfPath [m_strFfPath.GetLength () - 1] != '/')
			m_strFfPath += '\\';
		m_strFfPath += "Plugins\\";
		key.Close ();
	}
	else
		m_strFfPath = _App.Monitor_FirefoxPDInstalledTo ();

	

	if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strMozSKey1, KEY_READ) ||
			ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, strMozSKey2, KEY_READ))
	{
		

		char szPath [MY_MAX_PATH];
		DWORD dw = sizeof (szPath);
		key.QueryValue (szPath, "Path", &dw);
		m_strMozSPath = szPath;
		if (m_strMozSPath [m_strMozSPath.GetLength () - 1] != '\\' || m_strMozSPath [m_strMozSPath.GetLength () - 1] != '/')
			m_strMozSPath += '\\';
		m_strMozSPath += "Plugins\\";
		key.Close ();
	}
	else
		m_strMozSPath = _App.Monitor_MozillaSuitePDInstalledTo ();
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
	if (CopyFile ("npfdm.dll", str, FALSE))
	{
		
		_App.Monitor_OperaPDInstalledTo (m_strOpPath);
		return TRUE;
	}
	else
		return FALSE;
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
	if (CopyFile ("npfdm.dll", str, FALSE))
	{
		_App.Monitor_NetscapePDInstalledTo (m_strNetPath);
		return TRUE;
	}

	return FALSE;
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
	if (CopyFile ("npfdm.dll", str, FALSE))
	{
		
		_App.Monitor_MozillaSuitePDInstalledTo (m_strMozSPath);
		return TRUE;
	}
	else
		return FALSE;
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
