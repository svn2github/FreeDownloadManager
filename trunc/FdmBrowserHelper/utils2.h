/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmSettings.h"

inline LPCTSTR strcmp_m (LPCTSTR pszWhere, LPCTSTR pszWhat)
{
	if (*pszWhere == 0)
		return *pszWhat == 0 ? pszWhere : NULL;

	if (*pszWhat == 0)
		return NULL;

	
	if (*pszWhat == _T ('*'))
	{
		
		if (pszWhat [1] == 0)
			return pszWhere;

		
		
		LPCTSTR psz = strcmp_m (pszWhere, pszWhat + 1);
		if (psz)
			return psz;

		
		
		
		return strcmp_m (pszWhere + 1, pszWhat);
	}

	

	if (*pszWhat != _T ('?'))
	{
		
		if (*pszWhere != *pszWhat)
			return NULL;
	}

	return strcmp_m (pszWhere + 1, pszWhat + 1) ? pszWhere : NULL;
}

inline LPCTSTR strcmpi_m (LPCTSTR pszWhere, LPCTSTR pszWhat)
{
	TCHAR *psz1 = new TCHAR [lstrlen (pszWhere) + 1];
	TCHAR *psz2 = new TCHAR [lstrlen (pszWhat) + 1];

	lstrcpy (psz1, pszWhere);
	lstrcpy (psz2, pszWhat);

	CharLower (psz1);
	CharLower (psz2);

	LPCTSTR psz = strcmp_m (psz1, psz2);
	LPCTSTR pszRet = NULL;
	
	if (psz)
		pszRet = pszWhere + (psz - psz1);

	delete [] psz1;
	delete [] psz2;

	return pszRet;
}

inline BOOL IsExtStrEq (LPCTSTR pszMasked, LPCTSTR psz2)
{
	return strcmpi_m (psz2, pszMasked) != NULL;
}

inline BOOL IsExtInExtsStr (LPCTSTR pszExts, LPCTSTR pszExt)
{
	if (pszExt == NULL)
		return FALSE;

	int len = lstrlen (pszExts);
	int i = 0;
	TCHAR tszExt [1000]; 

	do
	{
		int j = 0;

		

		while (i < len && pszExts [i] != _T (' '))
			tszExt [j++] = pszExts [i++];

		tszExt [j] = 0;
		i++;

		if (IsExtStrEq (tszExt, pszExt))
			return TRUE;

	} while (i < len);

	return FALSE;
}

inline BOOL IsServerInServersStr (LPCTSTR pszServers, LPCTSTR pszServer)
{
	if (pszServer == NULL || !*pszServer)
		return FALSE;

	int len = _tcslen (pszServers);
	int i = 0;
	TCHAR szServer [10000];

	do
	{
		int j = 0;

		while (i < len && pszServers [i] != ' ')
			szServer [j++] = pszServers [i++];

		szServer [j] = 0;
		i++;

		if (IsExtStrEq (szServer, pszServer))
			return TRUE;

		tstring str;
		str = _T("*."); str += szServer;
		if (IsExtStrEq (str.c_str (), pszServer))
			return TRUE;

	} while (i < len);

	return FALSE;
}

inline bool vmsIsCatchedDownloadShoudBeIgnored (
	const vmsFdmBhIpcDownloadInfo *dlinfo,
	const vmsFdmSettings &settings)
{
	if (!settings.m_browser.m_monitor.m_enable)
		return true;

	if (settings.m_browser.m_monitor.m_need_alt &&
		!(GetKeyState (VK_MENU) & 0x8000))
	{
		return true;
	}
	
	if (!dlinfo->m_suggestedName.empty ())
	{
		auto n = dlinfo->m_suggestedName.rfind ('.');
		if (n != std::wstring::npos)
		{
			std::wstring ext (dlinfo->m_suggestedName.begin () + n + 1,
				dlinfo->m_suggestedName.end ());
			if (IsExtInExtsStr (settings.m_browser.m_monitor.m_skipExtensions.c_str (), 
					ext.c_str ()))
			{
				return true;
			}
		}
	}

	if (!settings.m_browser.m_monitor.m_skipServers.empty ())
	{
		if (IsServerInServersStr (settings.m_browser.m_monitor.m_skipServers.c_str (),
				domainFromHttpUrl (dlinfo->m_url).c_str ()))
		{
			return true;
		}
	}

	return false;
}