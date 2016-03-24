/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsAppVersion.h"
#include "../vmsStringHelper.h"
#include "../xml/vmsXmlUtil.h"

#pragma comment (lib, "version")

class vmsBinaryFileVersionInfo : public vmsSerializable 
{
public:
	BOOL Retrieve (LPCTSTR pszFile)
	{
		m_tstrProductName = _T ("");
		m_tstrProductVersion = _T ("");
		m_appVersion.clear ();
		m_fileVersion.clear ();

		DWORD dw;
		DWORD dwSize = GetFileVersionInfoSize (pszFile, &dw);
		if (dwSize == 0)
			return FALSE;
		LPVOID pvVer = _alloca (dwSize);
		ZeroMemory (pvVer, dwSize);
		if (FALSE == GetFileVersionInfo (pszFile, 0, dwSize, pvVer))
			return FALSE;
		return ExtractVersionInfo (pvVer, dwSize);
	}

	vmsBinaryFileVersionInfo(LPCTSTR ptszFile = NULL)
	{
		if (ptszFile)
			Retrieve (ptszFile);
	}

	virtual ~vmsBinaryFileVersionInfo()
	{

	}

	tstring m_tstrCompanyName;
	tstring m_tstrFileDescription;
	tstring m_tstrFileVersion;
	tstring m_tstrInternalName;
	tstring m_tstrLegalCopyright;
	tstring m_tstrOriginalFileName;
	tstring m_tstrProductName;
	tstring m_tstrProductVersion;
	tstring m_tstrSpecialBuildDescription;

	
	vmsAppVersion m_appVersion, m_fileVersion;

protected:
	BOOL ExtractVersionInfo (LPVOID pvVer, DWORD dwSize)
	{
		VS_FIXEDFILEINFO *pFileInfo = NULL;
		UINT u;
		BOOL bOK = FALSE;
		if (VerQueryValue (pvVer, _T ("\\"), (LPVOID*)&pFileInfo, &u))
		{
			m_appVersion.push_back (pFileInfo->dwProductVersionMS >> 16);
			m_appVersion.push_back (pFileInfo->dwProductVersionMS & 0xffff);
			m_appVersion.push_back (pFileInfo->dwProductVersionLS >> 16);
			m_appVersion.push_back (pFileInfo->dwProductVersionLS & 0xffff);
			while (m_appVersion.size () > 2 && m_appVersion [m_appVersion.size () - 1].dwVal == 0)
				m_appVersion.erase (m_appVersion.end () - 1);

			m_fileVersion.push_back (pFileInfo->dwFileVersionMS >> 16);
			m_fileVersion.push_back (pFileInfo->dwFileVersionMS & 0xffff);
			m_fileVersion.push_back (pFileInfo->dwFileVersionLS >> 16);
			m_fileVersion.push_back (pFileInfo->dwFileVersionLS & 0xffff);
			while (m_fileVersion.size () > 2 && m_fileVersion [m_fileVersion.size () - 1].dwVal == 0)
				m_fileVersion.erase (m_fileVersion.end () - 1);

			bOK = TRUE;
		}

		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		} *pTranslate;

		UINT uLen;

		tstring tstrLng;

		if (FALSE == VerQueryValue (pvVer, _T ("\\VarFileInfo\\Translation"), (LPVOID*) &pTranslate, &uLen))
		{
			LPBYTE pb;
			if (FALSE == VerQueryValue (pvVer, _T ("\\"), (LPVOID*)&pb, &uLen))
				return bOK;
			VS_FIXEDFILEINFO *p = (VS_FIXEDFILEINFO*)pb;
			pb += uLen;
			while (DWORD (pb - (LPBYTE)pvVer) < (dwSize - (DWORD)wcslen (L"StringFileInfo")) && wcsncmp ((LPWSTR)pb, L"StringFileInfo", wcslen (L"StringFileInfo")))
				pb++;
			if (DWORD (pb - (LPBYTE)pvVer) >= (dwSize - (DWORD)wcslen (L"StringFileInfo")))
				return bOK;
			pb += (wcslen (L"StringFileInfo") + 1) * 2;
			pb += 3*sizeof (WORD);
			do {
				while (static_cast <int> (pb - (LPBYTE)pvVer) < static_cast <int> (dwSize) - 8*2 && 
					(wcslen ((LPWSTR)pb) < 8) )
					pb++;
				LPWSTR pwsz = (LPWSTR)pb;
				while (*pwsz && (isdigit (*pwsz) || isalpha (*pwsz)))
					pwsz++;
				if (*pwsz == 0)
				{
					USES_CONVERSION;
					tstrLng = W2T ((LPWSTR)pb);
					break;
				}
			}
			while (static_cast <int> (pb - (LPBYTE)pvVer) < static_cast <int> (dwSize) - 8*2);
		}

                LPCTSTR atszValueName [] = {
			_T ("ProductVersion"),
			_T ("ProductName"),
			_T ("FileDescription"),
			_T ("CompanyName"),
			_T ("FileVersion"),
			_T ("InternalName"),
			_T ("LegalCopyright"),
			_T ("OriginalFilename"),
			_T ("SpecialBuild"),
		};

		m_tstrCompanyName = _T ("");
		m_tstrFileDescription = _T ("");
		m_tstrFileVersion = _T ("");
		m_tstrInternalName = _T ("");
		m_tstrLegalCopyright = _T ("");
		m_tstrOriginalFileName = _T ("");
		m_tstrProductName = _T ("");
		m_tstrProductVersion = _T ("");
		m_tstrSpecialBuildDescription = _T ("");

		tstring* atstrValues [] = {
			&m_tstrProductVersion, &m_tstrProductName, &m_tstrFileDescription, &m_tstrCompanyName,
			&m_tstrFileVersion, &m_tstrInternalName, &m_tstrLegalCopyright,
			&m_tstrOriginalFileName, &m_tstrSpecialBuildDescription,
		};

		int nLang = 0;

		if (tstrLng.empty ())
		{
			for (int i = 0; i < (int)uLen / (int)sizeof (LANGANDCODEPAGE) ; i++)
			{
				if ((pTranslate [i].wLanguage & 0x3FF) == 9)
				{
					
					nLang = i;
					break;
				}
			}
		}

                for (int i = 0; i < sizeof (atszValueName) / sizeof (LPCTSTR); i++)
		{
			TCHAR tsz [200];
			if (tstrLng.empty ())
			{
				swprintf (tsz, _countof (tsz), _T ("\\StringFileInfo\\%04x%04x\\%s"), pTranslate [nLang].wLanguage,
					pTranslate [nLang].wCodePage, atszValueName [i]);
			}
			else
			{
				swprintf (tsz, _countof (tsz), _T ("\\StringFileInfo\\%s\\%s"), tstrLng.c_str (), atszValueName [i]);
			}
			LPVOID pvValue;
			UINT uValLen;
			if (FALSE == VerQueryValue (pvVer, tsz, &pvValue, &uValLen))
			{
				if (tstrLng.empty () && pTranslate [nLang].wLanguage == 0)
				{
					pTranslate [nLang].wLanguage = 0x0409;
					i--;
				}
				continue;
			}

			atstrValues [i]->assign (
				vmsStringHelper::RemoveBadCharacters((LPCTSTR)pvValue).c_str ());
		}

		if (m_tstrProductVersion.empty ())
		{
			m_tstrProductVersion = m_appVersion.ToString ();
		}
		else
		{
			for (size_t i = 0; i < m_tstrProductVersion.length (); i++)
			{
				if (m_tstrProductVersion [i] == ',')
					m_tstrProductVersion [i] = '.';

				if (m_tstrProductVersion [i] == '.')
				{
					while (i+1 < m_tstrProductVersion.length () && m_tstrProductVersion [i+1] == ' ')
						m_tstrProductVersion.erase (m_tstrProductVersion.begin () + i + 1);
				}
			}
		}

		return TRUE;
	}

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		std::wstring wstrAppVersion, wstrFileVersion;

		if (pStm->isOutputStream ())
		{
			wstrAppVersion = m_appVersion.ToString ();
			wstrFileVersion = m_fileVersion.ToString ();
		}

		if (!(pStm->SerializeValue (L"ProductName", m_tstrProductName) &&
				pStm->SerializeValue (L"ProductVersion", m_tstrProductVersion) &&
				pStm->SerializeValue (L"ProductVersionDigit", wstrAppVersion) &&
				pStm->SerializeValue (L"FileVersion", m_tstrFileVersion) &&
				pStm->SerializeValue (L"FileVersionDigit", wstrFileVersion) &&
				pStm->SerializeValue (L"FileDescription", m_tstrFileDescription) &&
				pStm->SerializeValue (L"InternalName", m_tstrInternalName) &&
				pStm->SerializeValue (L"LegalCopyright", m_tstrLegalCopyright) &&
				pStm->SerializeValue (L"OriginalFilename", m_tstrOriginalFileName) &&
				pStm->SerializeValue (L"CompanyName", m_tstrCompanyName) &&
				pStm->SerializeValue (L"SpecialBuild", m_tstrSpecialBuildDescription)))
			return false;

		if (pStm->isInputStream ())
		{
			m_appVersion.FromString (wstrAppVersion.c_str ());
			m_fileVersion.FromString (wstrFileVersion.c_str ());
		}

		return true;
	}

	virtual bool Serialize (vmsSerializationOutputStream *pStm) 
	{
		pStm->WriteValue (L"ProductName", m_tstrProductName, false);
		pStm->WriteValue (L"ProductVersion", m_tstrProductVersion, false);
		pStm->WriteValue (L"ProductVersionDigit", m_appVersion.ToString (), false);
		pStm->WriteValue (L"FileVersion", m_tstrFileVersion, false);
		pStm->WriteValue (L"FileVersionDigit", m_fileVersion.ToString (), false);
		pStm->WriteValue (L"FileDescription", m_tstrFileDescription, false);
		pStm->WriteValue (L"InternalName", m_tstrInternalName, false);
		pStm->WriteValue (L"LegalCopyright", m_tstrLegalCopyright, false);
		pStm->WriteValue (L"OriginalFilename", m_tstrOriginalFileName, false);
		pStm->WriteValue (L"CompanyName", m_tstrCompanyName, false);
		pStm->WriteValue (L"SpecialBuild", m_tstrSpecialBuildDescription, false);
		return true;
	}

	virtual bool Serialize (vmsSerializationInputStream *pStm) 
	{
		std::wstring wstr;
		pStm->ReadValue (L"ProductName", m_tstrProductName, false);
		pStm->ReadValue (L"ProductVersion", m_tstrProductVersion, false);
		if (pStm->ReadValue (L"ProductVersionDigit", wstr, false))
			m_appVersion.FromString (wstr.c_str ());
		pStm->ReadValue (L"FileVersion", m_tstrFileVersion, false);
		if (pStm->ReadValue (L"FileVersionDigit", wstr, false))
			m_fileVersion.FromString (wstr.c_str ());
		pStm->ReadValue (L"FileDescription", m_tstrFileDescription, false);
		pStm->ReadValue (L"InternalName", m_tstrInternalName, false);
		pStm->ReadValue (L"LegalCopyright", m_tstrLegalCopyright, false);
		pStm->ReadValue (L"OriginalFilename", m_tstrOriginalFileName, false);
		pStm->ReadValue (L"CompanyName", m_tstrCompanyName, false);
		pStm->ReadValue (L"SpecialBuild", m_tstrSpecialBuildDescription, false);

		
		if (m_appVersion.empty ())
			m_appVersion.FromString (m_tstrProductVersion.c_str ());
		if (m_fileVersion.empty ())
			m_fileVersion.FromString (m_tstrFileVersion.c_str ());

		return true;
	}
};

