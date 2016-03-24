/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "../fsLangMgr.h"
#include "../lngphrases.h"

class vmsFdmTranslations  
{
public:
	static vmsFdmTranslations& o()
	{
		static vmsFdmTranslations o;
		return o;
	}

	LPCTSTR GetString (int nIndex)
	{
		return m_lm.GetString (nIndex);
	}

	vmsFdmTranslations()
	{
		CRegKey keyFDM;
		if (ERROR_SUCCESS == keyFDM.Open (HKEY_CURRENT_USER, 
			_T("Software\\FreeDownloadManager.ORG\\Free Download Manager"), KEY_READ))
		{
			TCHAR szPath [MAX_PATH] = _T(""); 
			DWORD dw = sizeof (szPath);
			keyFDM.QueryStringValue (_T("Path"), szPath, &dw);
			if (*szPath)
			{
				if (szPath [lstrlen (szPath) - 1] != _T('\\'))
					_tcscat_s (szPath, MAX_PATH, _T("\\"));

				_tcscat_s (szPath, MAX_PATH, _T("Language"));

				
				m_lm.Initialize (szPath);

				TCHAR szLang [100] = _T("English");	
				dw = _countof (szLang);
				CRegKey key;
				key.Open (keyFDM, _T("Settings\\View"), KEY_READ);
				
				key.QueryStringValue (_T("Language"), szLang, &dw);

				int nIndex = m_lm.FindLngByName (szLang);
				if (nIndex != -1)
					m_lm.LoadLng (nIndex);
			}
		}
	}

	virtual ~vmsFdmTranslations()
	{

	}

protected:
	fsLangMgr m_lm;
};

