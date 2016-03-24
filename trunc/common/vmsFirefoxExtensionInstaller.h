/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFirefoxUtil.h"
#include "vmsFile.h"

class vmsFirefoxExtensionInstaller  
{
public:
	
	
	
	static bool IsInstalledInProfile (LPCTSTR pszCID, bool bInDefaultProfileOnly, const tstring& firefoxPortablePath)
	{
		FU_STRINGLIST v; int nDefaultProfile;
		vmsFirefoxUtil::GetProfilesPathes (v, nDefaultProfile);

		if (!firefoxPortablePath.empty ())
		{
			fsString str2 = (firefoxPortablePath + _T("\\data\\profile")).c_str ();
			TCHAR sz [MAX_PATH];
			GetModuleFileName (NULL, sz, MAX_PATH);
			str2 [0] = sz [0];
			v.push_back (str2);
		}

		if (v.size () == 0)	
			return false;

		if (nDefaultProfile == -1 || nDefaultProfile >= v.size ())
			bInDefaultProfileOnly = false;

		for (int i = 0; i < v.size (); i++)
		{
			if (bInDefaultProfileOnly && i != nDefaultProfile)
				continue;

			TCHAR sz [MAX_PATH];
			lstrcpy (sz, v [i]);
			lstrcat (sz, _T("\\extensions\\"));
			lstrcat (sz, pszCID);

			if (GetFileAttributes (sz) != DWORD (-1))
				return true;
		}

		return false;
	}
	
	static bool IsInstalledUsingRegistry (LPCTSTR pszCID)
	{
		CRegKey key;
		if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, _T ("SOFTWARE\\Wow6432Node\\Mozilla\\Firefox\\Extensions"), KEY_READ) ||
			ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, _T ("SOFTWARE\\Mozilla\\Firefox\\Extensions"), KEY_READ)) {
				TCHAR sz [1000];
				DWORD dw = sizeof (sz);
				if (ERROR_SUCCESS == key.QueryValue (sz, pszCID, &dw) && *sz)
					return true;
		}
		if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, _T ("SOFTWARE\\Mozilla\\Firefox\\Extensions"), KEY_READ)) {
			TCHAR sz [1000];
			DWORD dw = sizeof (sz);
			if (ERROR_SUCCESS == key.QueryValue (sz, pszCID, &dw) && *sz)
				return true;
		}
		return false;
	}

	
	
	static bool Do (LPCTSTR pszCID, LPCTSTR pszExtPath, const tstring& firefoxPortablePath, bool bInstall = true)
	{
		USES_CONVERSION;
		FU_STRINGLIST v; int nDefProf;
		vmsFirefoxUtil::GetProfilesPathes (v, nDefProf);

		if (!firefoxPortablePath.empty ())
		{
			fsString str2 = (firefoxPortablePath + _T("\\data\\profile")).c_str ();
			TCHAR sz [MAX_PATH];
			GetModuleFileName (NULL, sz, MAX_PATH);
			str2 [0] = sz [0];
			v.push_back (str2);
		}

		if (v.size () == 0)
			return false;

		for (int i = 0; i < v.size (); i++)
		{
			TCHAR sz [MAX_PATH];
			lstrcpy (sz, v [i]);
			lstrcat (sz, _T("\\extensions\\"));
			lstrcat (sz, pszCID);

			if (bInstall)
			{
				vmsBuildPathToFile (sz);

				try
				{
					vmsFDM::vmsFile file;
					file.Create (sz, GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
					file.Write (pszExtPath, lstrlen (pszExtPath));
					file.Close ();
				}
				catch (...)
				{
					assert (FALSE);
					return false;
				}
			}
			else
			{
				DeleteFile (sz);
			}
		}

		return true;
	}

	static bool DoUsingRegistry (LPCTSTR pszCID, LPCTSTR pszExtPath, bool bInstall = true)
	{
		bool result = false;

		struct reg_key_info
		{
			HKEY root;
			LPCTSTR path;
			bool create;
		};

		static const reg_key_info keys [] = {
			{HKEY_CURRENT_USER, _T("SOFTWARE\\Mozilla"), true},
			{HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Mozilla"), false},
			{HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Mozilla"), false}
		};

		for (const auto &key_desc : keys)
		{
			CRegKey key;

			bool opened = ERROR_SUCCESS == key.Open (key_desc.root, key_desc.path, KEY_WRITE);
			if (!opened && bInstall && key_desc.create)
				opened = ERROR_SUCCESS == key.Create (key_desc.root, key_desc.path, nullptr, 0, KEY_WRITE);

			if (opened)
			{
				CRegKey extKey;
				if (bInstall)
				{
					if (ERROR_SUCCESS == extKey.Create(key, _T("Firefox\\Extensions")))
					{
						if (ERROR_SUCCESS == extKey.SetValue(pszExtPath, pszCID))
							result = true;
					}
				}
				else if (ERROR_SUCCESS == extKey.Open(key, _T("Firefox\\Extensions")))
				{
					if (ERROR_SUCCESS == extKey.DeleteValue(pszCID))
						result = true;
				}
			}

			if (bInstall)
				break; 
		}
		
		return result;
	}

	vmsFirefoxExtensionInstaller()
	{

	}

	virtual ~vmsFirefoxExtensionInstaller()
	{

	}

	static bool ExtractExtensionInfo(LPCTSTR ptszRdf, tstring& tstrVersion)
	{
		USES_CONVERSION;
		IXMLDOMDocumentPtr spXML;
		spXML.CreateInstance (__uuidof (DOMDocument));
		if (!spXML)
			return false;
		spXML->put_async (FALSE);
		VARIANT_BOOL bRes = FALSE;
		spXML->load (CComVariant (ptszRdf), &bRes);
		if (!bRes)
			return false;
		IXMLDOMNodePtr spRDF;
		spXML->selectSingleNode (L"RDF", &spRDF);
		if (!spRDF)
			return false;
		IXMLDOMNodePtr spDesc;
		spRDF->selectSingleNode (L"Description", &spDesc);
		if (!spDesc)
			return false;
		IXMLDOMNodePtr spVer;
		spDesc->selectSingleNode (L"em:version", &spVer);
		if (!spVer)
			return false;
		CComBSTR bstr;
		spVer->get_text (&bstr);
		tstrVersion = W2CT (bstr);
		return true;
	}
};

