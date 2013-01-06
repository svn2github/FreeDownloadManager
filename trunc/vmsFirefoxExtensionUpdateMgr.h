/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#pragma once
class vmsFirefoxExtensionUpdateMgr
{
public:
	vmsFirefoxExtensionUpdateMgr(void);
	virtual ~vmsFirefoxExtensionUpdateMgr(void);
	LPCTSTR getExtensionPath(void);

protected:
	tstring m_tstrExtensionPath;
	vmsAppVersion m_extensionVersion;
	void LoadState(void);
	void LocateFirefoxExtension(void);
	static void GetExtensionsInstallationPath(tstring& tstrResult);
	tstring m_tstrExtensionNewVerURL;
	vmsAppVersion m_extensionNewVer;
public:
	bool CheckForUpdates(void);
	bool isNewVersionAvailable () const {return !m_tstrExtensionNewVerURL.empty ();}
	bool PerformUpdate(void);
};

