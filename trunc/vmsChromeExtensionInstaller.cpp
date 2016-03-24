/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsRegisteredApp.h"

#include <fstream>
#include <boost/scoped_array.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "picojson.h"

#include "vmsChromeExtensionInstaller.h"
#include "vmsFdmUtils.h"

using namespace std;
using namespace picojson;

#define EXT_ID "ahmpjcflkgiildlgicmcieglgoilbfdp" 
#define CHROME_BINARY _T("chrome.exe")
#define REMOVE_URL _T("http://files2.freedownloadmanager.org/uninstall_fdm_chrome_extension.htm")
#define INSTALL_FROM_STORE_URL L"https://chrome.google.com/webstore/detail/free-download-manager-chr/ahmpjcflkgiildlgicmcieglgoilbfdp"

#define BUFFER_SIZE MAX_PATH*2
static TCHAR buffer[BUFFER_SIZE];

BOOL vmsChromeExtensionInstaller::IsChromeInstalled()
{
	return !vmsRegisteredApp::GetFullPath2 (CHROME_BINARY).IsEmpty ();
}

bool vmsChromeExtensionInstaller::IsInstalled()
{
	
	if(!IsChromeInstalled())
		return false;

	

	memset(buffer, 0, BUFFER_SIZE*sizeof(TCHAR));
	if(S_OK != SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, buffer))
		return false;

	_tcscat(buffer, _T("\\Google\\Chrome\\User Data\\Default\\Extensions\\"));
	_tcscat(buffer, _T(EXT_ID));

	DWORD dwAttrs = GetFileAttributes(buffer);
	if(dwAttrs == INVALID_FILE_ATTRIBUTES)
		return false;
	if(dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

int vmsChromeExtensionInstaller::GetChromeVersion()
{
	HKEY hClients;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Google\\Update\\Clients"), 0, KEY_READ, &hClients) != ERROR_SUCCESS)
		return -1;

	DWORD nSubkeys;
	DWORD nMaxSubkeyLength;
	if(RegQueryInfoKey(hClients, NULL, NULL, NULL, &nSubkeys, &nMaxSubkeyLength, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		RegCloseKey(hClients);
		return -1;
	}

	boost::scoped_array<TCHAR> szSubkey(new TCHAR[nMaxSubkeyLength+1]);
	const size_t nNameLen = 1024;
	boost::scoped_array<TCHAR> szName(new TCHAR[nNameLen]);
	const size_t nVerLen = 1024;
	boost::scoped_array<TCHAR> szVersion(new TCHAR[nVerLen]);
	szVersion.get()[0] = 0;
	for(DWORD i = 0; i < nSubkeys; i++)
	{
		DWORD nSubkeyLength = nMaxSubkeyLength+1;
		if(RegEnumKeyEx(hClients, i, szSubkey.get(), &nSubkeyLength, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			HKEY hClient;
			if(RegOpenKeyEx(hClients, szSubkey.get(), 0, KEY_READ, &hClient) == ERROR_SUCCESS)
			{
				DWORD dwNameLen = nNameLen * sizeof(TCHAR);
				if(RegQueryValueEx(hClient,	_T("name"), NULL, NULL, (LPBYTE)szName.get(), &dwNameLen) == ERROR_SUCCESS)
				{
					if(_tcsicmp(szName.get(), _T("Google Chrome")) == 0)
					{
						DWORD dwVerLen = nVerLen * sizeof(TCHAR);
						if(RegQueryValueEx(hClient, _T("pv"), NULL, NULL, (LPBYTE)szVersion.get(), &dwVerLen) == ERROR_SUCCESS)
						{
							RegCloseKey(hClient);
							break;
						}
					}
				}
				RegCloseKey(hClient);
			}
		}
	}
	RegCloseKey(hClients);
	if(szVersion.get()[0] == 0)
		return -1;

	std::vector<tstring> v;
	TCHAR* s = szVersion.get();
	boost::split(v, s, boost::is_any_of("."));
	if(v.size() != 4)  
		return -1;

	int res;
	try
	{
		res = stoi(v[0]);
	}
	catch(std::exception&)
	{
		return -1;
	}
	return res;
}

static bool DOT(value& json, const string& field)
{
	if(!json.is<object>())
		return false;
	object obj = json.get<object>();
	if(obj.count(field) == 0)
		return false;
	json = obj[field];

	return true;
}

void vmsChromeExtensionInstaller::RemoveAddonFromBlacklisted()
{
	assert (!"expected");

	

	
}

bool vmsChromeExtensionInstaller::Enabled(bool& isEnabled)
{
	

	tstring userDefaultpath = vmsFdmUtils::GetChromeUserDataPath();
	userDefaultpath += _T("\\Default\\Preferences");
	tstring chromePatchScriptPath = vmsFdmUtils::GetChromeScriptPath();
	DWORD isEnabledExt = 0;

	tstring parametersScript = _T("//E:jscript \"");
	parametersScript += chromePatchScriptPath;
	parametersScript += _T("\" check \"");
	parametersScript += userDefaultpath;
	parametersScript += _T("\" \"ahmpjcflkgiildlgicmcieglgoilbfdp\"");
	
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	
	sei.lpFile = _T("cscript.exe");
	sei.lpParameters = parametersScript.c_str();
	sei.nShow = SW_HIDE;

	BOOL bOK = ShellExecuteEx(&sei) != FALSE;

	if (bOK)
	{
		WaitForSingleObject(sei.hProcess, INFINITE);

		GetExitCodeProcess(sei.hProcess, &isEnabledExt);

		CloseHandle(sei.hProcess);
	}
	else
	{
		return false;
	}

	isEnabled = (isEnabledExt == 1);

	return true;
}

BOOL vmsChromeExtensionInstaller::Install()
{
	MessageBox (AfxGetMainWnd () ? AfxGetMainWnd ()->m_hWnd : nullptr,
		L"Click OK to continue Chrome extension installation from Chrome Web Store", L"Free Download Manager",
		MB_OK | MB_SETFOREGROUND);
	return RunChrome (INSTALL_FROM_STORE_URL, wait_process::dont_wait);

	
}

BOOL vmsChromeExtensionInstaller::AdminRightsRequired ()
{
	return FALSE;
}

BOOL vmsChromeExtensionInstaller::Uninstall()
{
	int ver = GetChromeVersion();
	if(ver != -1 && ver < 36)  
	{
		
		return RunChrome(_T("--uninstall-extension=") _T(EXT_ID));
	}
	else
	{
		
		bool bEnabled = true;
		Enabled(bEnabled);  
		if(!bEnabled)
			
			return FALSE;
		return RunChrome(REMOVE_URL, wait_process::wait_idle);
	}
}

BOOL vmsChromeExtensionInstaller::RunChrome(const TCHAR* parameters, wait_process wp, bool doWindowHack)
{
	auto path = vmsRegisteredApp::GetFullPath2 (CHROME_BINARY);
	if (path.IsEmpty ())
		return FALSE;

	WCHAR wsz [MAX_PATH] = L"";
	swprintf (wsz, L"\"%s\" %s", (LPCWSTR)path, parameters);

	HANDLE hProcess = nullptr;
	BOOL ret = FALSE;

	{
		STARTUPINFO si = {0};
		PROCESS_INFORMATION pi = {0};

		ret = vmsWinSecurity::RunAsDesktopUser (path, wsz, nullptr, si, pi);
		if (ret)
		{
			CloseHandle (pi.hThread);
			hProcess = pi.hProcess;
		}
	}

	if (!ret)
	{
		SHELLEXECUTEINFO ShExecInfo;
		ShExecInfo.cbSize = sizeof (SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = path;
		ShExecInfo.lpParameters = parameters;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_NORMAL;
		ShExecInfo.hInstApp = NULL;
		ret = ShellExecuteEx (&ShExecInfo);
		if (ret)
			hProcess = ShExecInfo.hProcess;
	}

	if (ret)
	{
		if (ret)
		{
			switch (wp)
			{
			case wait_process::wait:
				WaitForSingleObject (hProcess, INFINITE);
				break;

			case wait_process::wait_idle:
				WaitForSingleObject (hProcess, 5*1000);
				break;
			}
		}			

		CloseHandle (hProcess);
	}

	return ret;
}
