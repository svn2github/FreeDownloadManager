/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSAPPSETTINGSSTORE_H__232450C9_092A_49EF_9B4E_3D2DA2B9E736__INCLUDED_)
#define AFX_VMSAPPSETTINGSSTORE_H__232450C9_092A_49EF_9B4E_3D2DA2B9E736__INCLUDED_

#include "vmsDataFile.h"	
#include "vmsPersistObject.h"

#if _MSC_VER > 1000
#pragma once
#endif 

#define APPSETTINGS_FILE_CURRENT_VERSION	(1)

#define APPSETTINGS_FILE_SIG "FDM Application Settings"

struct vmsAppSettingsFileHdr
{
	char szSig [sizeof (APPSETTINGS_FILE_SIG) + 1];
	WORD wVer;

	vmsAppSettingsFileHdr ()
	{
		strcpy (szSig, APPSETTINGS_FILE_SIG);
		wVer = APPSETTINGS_FILE_CURRENT_VERSION;
	}
};

class vmsAppSettingsStore : public vmsPersistObject
{
public:
	void SaveSettingsToFile(LPCTSTR pszFile);
	void LoadSettingsFromFile (LPCTSTR pszFile);

	BOOL WriteProfileBinary (LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE pbData, UINT nBytes);
	BOOL GetProfileBinary (LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE* ppData, UINT* pBytes);

	BOOL WriteProfileString(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue);
	CString GetProfileString(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault);

	BOOL WriteProfileUint64(LPCTSTR pszSection, LPCTSTR pszEntry, UINT64 nValue);
	UINT64 GetProfileUint64(LPCTSTR pszSection, LPCTSTR pszEntry, UINT64 nDefault);

	BOOL WriteProfileInt (LPCTSTR pszSection, LPCTSTR pszEntry, int nValue);
	UINT GetProfileInt (LPCTSTR pszSection, LPCTSTR pszEntry, INT nDefault);

	void getObjectItselfStateBuffer(LPBYTE pb, LPDWORD pdwSize, bool bSaveToStorage);
	bool loadObjectItselfFromStateBuffer(LPBYTE pb, LPDWORD pdwSize, DWORD dwVer);

	vmsAppSettingsStore();
	virtual ~vmsAppSettingsStore();

protected:
	
	bool m_bUseRegistry;
	vmsDataFile m_file;
	CWinApp* m_app;
};

#endif 
