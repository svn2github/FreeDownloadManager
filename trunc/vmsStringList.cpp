/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsStringList.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsStringList::vmsStringList()
{

}

vmsStringList::~vmsStringList()
{

}

void vmsStringList::Add(LPCTSTR psz)
{
	m_vList.add (psz);
}

int vmsStringList::get_Count() const
{
	return m_vList.size ();
}

void vmsStringList::Del(int nIndex)
{
	m_vList.del (nIndex);
}

LPCTSTR vmsStringList::get_String(int nIndex) const
{
	const fs::list <fsString>* pv = &m_vList;
	return ((fs::list <fsString>*)pv)->at (nIndex);
}

BOOL vmsStringList::Save(HANDLE hFile)
{
	int c = m_vList.size ();

	DWORD dw;

	if (FALSE == WriteFile (hFile, &c, sizeof (c), &dw, NULL))
		return FALSE;

	for (int i = 0; i < c; i++)
	{
		if (FALSE == fsSaveStrToFile (get_String (i), hFile))
			return FALSE;
	}

	return TRUE;
}

bool vmsStringList::LoadItems(HANDLE hFile, int nItemCount, WORD wVer)
{
	for (int i = 0; i < nItemCount; i++)
	{
		LPTSTR psz;

		if (FALSE == fsReadStrFromFile (&psz, hFile))
			return false;

		Add (psz);

		delete [] psz;
	}

	return true;

}

bool vmsStringList::LoadItems_old(HANDLE hFile, int nItemCount, WORD wVer)
{
	for (int i = 0; i < nItemCount; i++)
	{
		LPTSTR ptsz;
		LPSTR psz = 0;

		if (FALSE == fsReadStrFromFileA (&psz, hFile))
			return false;

		CopyString(&ptsz, psz);
		delete [] psz;
		psz = 0;

		Add (ptsz);

		delete [] ptsz;
	}

	return true;

}

BOOL vmsStringList::Load(HANDLE hFile, WORD wVer)
{
	Clear ();

	int c = 0;
	DWORD dw;

	if (FALSE == ReadFile (hFile, &c, sizeof (c), &dw, NULL))
		return FALSE;

	if (wVer > 1) {
		LoadItems(hFile, c, wVer);
	}

	if (wVer == 1) {
		
		LoadItems_old(hFile, c, wVer);
	}

	return TRUE;
}

void vmsStringList::Clear()
{
	m_vList.clear ();
}
