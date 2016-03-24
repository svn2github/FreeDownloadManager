/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFirefoxUtil.h"
#include "vmsFileUtil.h"

typedef std::vector <fsString> FU_STRINGLIST;

class vmsFirefoxUtil  
{
public:
	
	static bool GetDefaultProfilePath (LPTSTR pszPath)
	{
		FU_STRINGLIST v; 
		int nDefaultProfile;

		if (false == GetProfilesPathes (v, nDefaultProfile) || v.size () == 0)
			return false;

		if (nDefaultProfile == -1 || nDefaultProfile < 0 || nDefaultProfile >= v.size ())
			nDefaultProfile = 0;

		_tcscpy (pszPath, v [nDefaultProfile]);

		return  true;
	}

	
	
	static bool GetProfilesPathes (FU_STRINGLIST& v, int& nDefaultProfile)
	{
		try 
		{
			TCHAR szPath [MAX_PATH];
			GetDataPath (szPath);

			

			TCHAR szProfilesIni [MAX_PATH];
			lstrcpy (szProfilesIni, szPath);
			lstrcat (szProfilesIni, _T("profiles.ini"));

			if (GetFileAttributes (szProfilesIni) != DWORD (-1))
			{
				nDefaultProfile = -1;

				for (int i = 0;; i++)
				{
					TCHAR sz [100], szPP [MAX_PATH] = L""; 
					_stprintf (sz, _T ("Profile%d"), i);
					GetPrivateProfileString (sz, _T ("Path"), _T(""), szPP, sizeof (szPP), szProfilesIni);
					if (*szPP == 0)
						break; 
					LPTSTR psz = szPP;
					while (*psz) {
						if (*psz == '/')
							*psz = '\\';
						psz++;
					}
					if (szPP [1] == ':') 
					{
						v.push_back (szPP);
					}
					else
					{
						
						TCHAR sz [MAX_PATH];
						lstrcpy (sz, szPath);
						lstrcat (sz, szPP);
						v.push_back (sz);
					}

					if (nDefaultProfile == -1 &&
						GetPrivateProfileInt (sz, _T("Default"), 0, szProfilesIni))
						nDefaultProfile = i; 
				}
			}

			if (v.size () == 0)
			{
				

				

				TCHAR szProfiles [MAX_PATH];
				vmsFirefoxUtil::GetProfilesPath (szProfiles);
				lstrcat (szProfiles, _T("*"));

				WIN32_FIND_DATA wfd;
				HANDLE hFind = FindFirstFile (szProfiles, &wfd);
				if (hFind == INVALID_HANDLE_VALUE)
					return false;

				do
				{
					if (0 == lstrcmp (wfd.cFileName, _T(".")) || 0 == lstrcmp (wfd.cFileName, _T("..")))
						continue;

					TCHAR sz [MAX_PATH];
					lstrcpy (sz, szPath);
					lstrcat (sz, wfd.cFileName);

					DWORD dw = GetFileAttributes (sz);
					if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY)) {
						{
							TCHAR sz2 [MAX_PATH];
							lstrcpy (sz2, sz);
							lstrcat (sz2, _T("\\extensions\\"));
							
							DWORD dw = GetFileAttributes (sz2);
							if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY))
								v.push_back (sz); 
						}
					}
				}
				while (FindNextFile (hFind, &wfd));

				FindClose (hFind);
			}

			return true;
		}
		catch (...)
		{
			assert (FALSE);
			return false;
		}
	}

	
	static void GetDataPath (LPTSTR pszPath)
	{
		vmsFDM::vmsFileUtil::GetAppDataPath (_T("Mozilla"), pszPath);
		vmsFDM::vmsFileUtil::MakePathOK (pszPath, true);
		lstrcat (pszPath, _T("Firefox\\"));
	}

	
	static void GetProfilesPath (LPTSTR pszPath)
	{
		vmsFirefoxUtil::GetDataPath (pszPath);
		lstrcat (pszPath, _T("Profiles\\"));
	}

	vmsFirefoxUtil() {}
	virtual ~vmsFirefoxUtil() {}
};

