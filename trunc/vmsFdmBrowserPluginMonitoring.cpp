/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsFdmBrowserPluginMonitoring.h"
#include "vmsFirefoxMonitoring.h"
#include "vmsChromeExtensionInstaller.h"
#include "vmsElevatedFdm.h"
#include "vmsRegisteredApp.h"

vmsFdmBrowserPluginMonitoring::vmsFdmBrowserPluginMonitoring(){
}

vmsFdmBrowserPluginMonitoring::~vmsFdmBrowserPluginMonitoring(){
}

bool vmsFdmBrowserPluginMonitoring::MonitorFF( BOOL bMonitor, BOOL &bNeedRestartBrowser ){
	BOOL bFF = bMonitor;
	if (bFF && vmsFirefoxMonitoring::IsInstalled () == false)
	{
		if (vmsFirefoxMonitoring::Install (true) == false)
		{
			MessageBox (NULL, LS (L_CANTINITFFMONITOR), LS (L_ERR), MB_ICONERROR);			
			bFF = FALSE;
		}
	}
	if (bFF && vmsFirefoxMonitoring::IsFlashGotInstalled ())
	{
		MessageBox (NULL, LS (L_FLASHGOTDETECTED), vmsFdmAppMgr::getAppName (), MB_ICONWARNING);		
		bFF = FALSE;
	}
	if (bFF)
	{
		DWORD dwResult = 0;
		CheckFirefoxExtension (&dwResult);
		if (dwResult)
		{
			if (!(dwResult & (1<<1)))
			{				
				bFF = FALSE;
			}
			else
			{
				
				_App.get_SettingsStore ()->WriteProfileInt (_T ("State"), _T ("FfExtChecked"), FALSE);
			}
		}
	}
	
	_App.Monitor_Firefox (bFF);
	if (bFF)
		vmsFirefoxMonitoring::Install (true); 

	return bFF;
}

bool vmsFdmBrowserPluginMonitoring::MonitorChrome(bool* value, bool *needRestartBrowser)
{
	if (needRestartBrowser)
		*needRestartBrowser = false;

	if (value == nullptr) 
	{
		bool bEnabled = true;
		vmsChromeExtensionInstaller::Enabled(bEnabled);
		return _App.Monitor_Chrome() && vmsChromeExtensionInstaller::IsInstalled() && bEnabled;
	}
	else 
	{

		_App.Monitor_Chrome (*value);

		bool bOk = false;

		if (*value)
		{
			bool bEnabled = true;
			vmsChromeExtensionInstaller::Enabled(bEnabled);

			auto needInstall = !vmsChromeExtensionInstaller::IsInstalled() || !bEnabled;

			if (needInstall)
			{
				if (vmsChromeExtensionInstaller::AdminRightsRequired ())
				{
					bool needElevated = IsUserAnAdmin () ? false : true;

					bOk = vmsElevatedFdm::o ().InstallChromeIntegration (needElevated);
					if (bOk)
					{
						if (!_App.MonitorChrome_Cancel_HKLM ()) 
						{
							vmsChromeExtensionInstaller::RemoveAddonFromBlacklisted ();
							runChrome (); 
						}
					}
				}
				else
				{
					bOk = vmsChromeExtensionInstaller::Install ();
				}

				if (!bOk)
					MessageBox (NULL, LS (L_CANTINITCHROMEMONITOR), LS (L_ERR), MB_ICONERROR);
			}
			else
			{
				if (needRestartBrowser)
					*needRestartBrowser = !_App.Monitor_Chrome ();
			}
		}
		else
		{
			if (needRestartBrowser)
				*needRestartBrowser = true;
		}

		return *value;
	}
}

void vmsFdmBrowserPluginMonitoring::CheckFirefoxExtension(LPDWORD pdwResult)
{
	if (pdwResult)
		*pdwResult = 0;

	bool bEnabled;
	if (!vmsFirefoxMonitoring::IsEnabledInFirefox (bEnabled) || bEnabled)
		return;

	if (pdwResult)
		*pdwResult |= 1;

	if (IDYES != MessageBox (NULL, _T ("Free Download Manager extension for Firefox is disabled. You need to enable it in order to use the integration with Firefox. Would you like to enable it?"), 
			_T ("Free Download Manager"), MB_YESNO))
		return;

	if (pdwResult)
		*pdwResult |= (1 << 1);

	MessageBox (NULL, _T ("Firefox addons page will be opened. You need to enable the extension there."), _T ("Free Download Manager"), MB_OK);

	ShellExecute (NULL, _T ("open"), _T ("firefox.exe"), _T ("about:addons"), NULL, SW_SHOWNORMAL);
}

void vmsFdmBrowserPluginMonitoring::SuggestBrowserExtensions()
{
	BOOL bOldFFInstalled     = _App.Monitor_FirefoxInstalled();
	

	_App.Monitor_ChromeInstalled(vmsChromeExtensionInstaller::IsChromeInstalled());
	BOOL bFFInstalled = vmsFirefoxMonitoring::IsFFInstalled() && vmsFirefoxMonitoring::ProfileExists();
	if (bFFInstalled && bOldFFInstalled != bFFInstalled)
		_App.Monitor_FirefoxInstalled( bFFInstalled );

	bool bMonitoringFF = _App.Monitor_Firefox() && vmsFirefoxMonitoring::IsInstalled () &&
		( vmsFirefoxMonitoring::IsFlashGotInstalled () == false );
	if (!bOldFFInstalled && !_App.Monitor_FirefoxExtensionSuggested() && bFFInstalled && !bMonitoringFF )
	{
		_App.Monitor_FirefoxExtensionSuggested(TRUE);
		if(MessageBox(
				NULL, _T("Free Download Manager is able to catch downloads from Mozilla Firefox. Do you want to enable this feature?"),
				_T("Free Download Manager"),
				MB_ICONINFORMATION | MB_YESNO) == IDYES)
		{
			BOOL doMonitor = TRUE;
			BOOL needRestartBrowser = FALSE;
			MonitorFF(doMonitor, needRestartBrowser);
			if(  needRestartBrowser )
				MessageBox(NULL, _T("Changes will take effect after you restart browser."), _T ("Free Download Manager"), MB_OK);					
		}
	}
	else
	{
		if (!_App.get_SettingsStore ()->GetProfileInt (_T ("State"), _T ("FfExtChecked"), FALSE))
		{
			DWORD dwResult;
			vmsFdmBrowserPluginMonitoring::CheckFirefoxExtension (&dwResult);
			if (!(dwResult & (1 << 1)))
				_App.get_SettingsStore ()->WriteProfileInt (_T ("State"), _T ("FfExtChecked"), TRUE); 
		}
	}

	
}

void vmsFdmBrowserPluginMonitoring::runChrome(void)
{
	CString fullPathChrome = vmsRegisteredApp::GetFullPath(_T("chrome.exe"));
	BOOL bResult = BOOL(fullPathChrome.GetLength() != 0);

	if (!bResult)
	{
		fullPathChrome = vmsRegisteredApp::GetFullPath(_T("chrome.exe"), true);
		bResult = BOOL(fullPathChrome.GetLength() != 0);
	}
	if (bResult)
	{
		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = _T("open");
		sei.hwnd = NULL;
		sei.lpFile = fullPathChrome;
		sei.hInstApp = NULL;
		sei.lpParameters = _T("\"-restore-last-session\"");
		sei.nShow = SW_SHOWNORMAL;

		ShellExecuteEx(&sei);
	}
}
