/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsFirefoxExtensionInstaller.h"
#include "vmsFirefoxUtil.h"
#include "vmsFile.h"
using namespace vmsFDM;          

vmsFirefoxExtensionInstaller::vmsFirefoxExtensionInstaller()
{

}

vmsFirefoxExtensionInstaller::~vmsFirefoxExtensionInstaller()
{

}

bool vmsFirefoxExtensionInstaller::Do(LPCSTR pszCID, LPCSTR pszExtPath, bool bInstall)
{
	LOG ("vmsFEI::Do: in" << nl);

	FU_STRINGLIST v; int nDefProf;
	vmsFirefoxUtil::GetProfilesPathes (v, nDefProf);

	LOG ("vmsFEI::Do: " << v.size () << " profiles found" << nl);

	if (v.size () == 0)
		return false;

	for (int i = 0; i < v.size (); i++)
	{
		char sz [MY_MAX_PATH];
		lstrcpy (sz, v [i]);
		lstrcat (sz, "\\extensions\\");
		lstrcat (sz, pszCID);

		if (bInstall)
		{
			fsBuildPathToFile (sz);

			try{

			vmsFile file;
			LOG ("vmsFEI::Do: creating file: " << sz << nl);
			file.Create (sz, GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
			LOG ("vmsFEI::Do: writing to it" << nl);
			file.Write (pszExtPath, lstrlen (pszExtPath));
			LOG ("vmsFEI::Do: closing it" << nl);
			file.Close ();
			LOG ("vmsFEI::Do: done" << nl);

			}catch (...) {
				LOG ("vmsFEI::Do: failed write ext ptr file" << nl);
				return false;
			}
		}
		else
		{
			DeleteFile (sz);
		}
	}

	LOG ("vmsFEI::Do: ok" << nl);

	return true;
}

bool vmsFirefoxExtensionInstaller::IsInstalled(LPCSTR pszCID, bool bInDefaultProfileOnly)
{
	LOG ("vmsFEI::IsInst: in" << nl);

	FU_STRINGLIST v; int nDefaultProfile;
	vmsFirefoxUtil::GetProfilesPathes (v, nDefaultProfile);

	LOG ("vmsFEI::IsInst: " << v.size () << " profiles found" << nl);

	if (v.size () == 0)	
		return false;

	if (nDefaultProfile == -1 || nDefaultProfile >= v.size ())
		bInDefaultProfileOnly = false;

	for (int i = 0; i < v.size (); i++)
	{
		if (bInDefaultProfileOnly && i != nDefaultProfile)
			continue;

		char sz [MY_MAX_PATH];
		lstrcpy (sz, v [i]);
		lstrcat (sz, "\\extensions\\");
		lstrcat (sz, pszCID);

		LOG ("vmsFEI::IsInst: checking file presense: " << sz << nl);

		if (GetFileAttributes (sz) != DWORD (-1))
		{
			LOG ("vmsFEI::IsInst: ok (yes)" << nl);
			return true;
		}
	}

	LOG ("vmsFEI::IsInst: ok (no)" << nl);
	return false;
}
