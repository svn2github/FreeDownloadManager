/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsChromeExtensionInstaller
{
public:
	static BOOL IsChromeInstalled();
	static int GetChromeVersion();
	static bool IsInstalled();
	static void RemoveAddonFromBlacklisted();
	static bool Enabled(bool& isEnabled);
	static BOOL Install();
	static BOOL Uninstall();
	
	static BOOL AdminRightsRequired ();

private:
	vmsChromeExtensionInstaller();
	vmsChromeExtensionInstaller(const vmsChromeExtensionInstaller&);
	~vmsChromeExtensionInstaller();

	enum class wait_process {wait, wait_idle, dont_wait};

	static BOOL RunChrome(const TCHAR* parameters, wait_process wp = wait_process::wait, bool doWindowHack = false);
	static BOOL ChromeWindowHack();

};
