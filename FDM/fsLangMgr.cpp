/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsLangMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsLangMgr::fsLangMgr()
{
	m_iCurLng = -1;
}

fsLangMgr::~fsLangMgr()
{

}  

#define LNG_COMMENT_CHAR	';'

BOOL fsLangMgr::Initialize()
{
	LOG ("fsLM::I: start" << nl);

	LoadBuiltInLngStrings ();

	WIN32_FIND_DATA wfd;

	m_strLngFolder = ((CFdmApp*)AfxGetApp ())->m_strAppPath;
	m_strLngFolder += "Language\\";

	CString strMask = m_strLngFolder;
	strMask += "*.lng";

	LOG ("fsLM::I: searching for " << strMask << nl);

	HANDLE hFind = FindFirstFile (strMask, &wfd);
	BOOL bFind = hFind != INVALID_HANDLE_VALUE;

	while (bFind)
	{
		try {
		fsLngFileInfo info;
		info.strFileName = wfd.cFileName;

		LOG ("fsLM::I: found " << wfd.cFileName << nl);

		CStdioFile file (m_strLngFolder + info.strFileName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone);

		LOG ("fsLM::I: was opened ok." << nl);

		
		while (file.ReadString (info.strLngName))
		{
			if (info.strLngName.GetLength () && info.strLngName [0] != LNG_COMMENT_CHAR)
			{
				LOG ("fsLM::I: it's a valid lng file. add it." << nl);
				AddLngFileInfo (info);
				break;
			}
		}
		}
		catch (...){
			LOG ("fsLM::I: some file error occurred." << nl);
		}

		bFind = FindNextFile (hFind, &wfd);
	}

	if (hFind != INVALID_HANDLE_VALUE)
		FindClose (hFind);

	LOG ("fsLM::I: there are " << m_vLngFiles.size () << " languages found" << nl);

	return m_vLngFiles.size () != 0;
}

int fsLangMgr::GetLngCount()
{
	return m_vLngFiles.size ();
}

LPCSTR fsLangMgr::GetLngName(int iIndex)
{
	static char szBuiltIn [] = "Built In";
	if (iIndex < 0 || iIndex >= m_vLngFiles.size ())
		return szBuiltIn;
	return m_vLngFiles [iIndex].strLngName;
}

BOOL fsLangMgr::LoadLng(int iIndex)
{
	try {
	
	CStdioFile file (m_strLngFolder + m_vLngFiles [iIndex].strFileName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone);
	CString str;

	
	do
	{
		if (FALSE == file.ReadString (str))
			return FALSE;

		if (str.GetLength () && str [0] != LNG_COMMENT_CHAR)
			break;
	}
	while (TRUE);

	m_vStrings.clear ();

	
	while (file.ReadString (str))
	{
		if (str.GetLength () == 0 || str [0] == LNG_COMMENT_CHAR)
			continue; 

		PreprocessLanguageString (str);

		m_vStrings.add (str);
	}

	m_iCurLng = iIndex;

	return m_vStrings.size () != 0;

	}
	catch (...)
	{
		return FALSE;
	}
}

int fsLangMgr::FindLngByName(LPCSTR pszName)
{
	LOG ("fsLM::FLbN: find for " << pszName << nl);

	for (int i = 0; i < m_vLngFiles.size (); i++)
	{
		if (m_vLngFiles [i].strLngName == pszName || 
				lstrcmpi (m_vLngFiles [i].strFileName, pszName) == 0)
		{
			LOG ("fsLM::FLbN: was found." << nl);
			return i;
		}
	}

	LOG ("fsLM::FLbN: was not found." << nl);
	return -1;
}

int fsLangMgr::GetStringCount()
{
	return m_vStrings.size ();
}

LPCSTR fsLangMgr::GetString(int iIndex)
{
	static char szNull [1] = {0};

	try{

	if (iIndex < m_vStrings.size ()) 
		return m_vStrings [iIndex];
	else if (iIndex < m_vBuiltInStrings.size ()) 
		return m_vBuiltInStrings [iIndex];
	else
		return szNull;

	}
	catch (...) {ASSERT (FALSE); return szNull;}
}

int fsLangMgr::GetCurLng()
{
	return m_iCurLng;
}

CString fsLangMgr::GetStringNP(int iIndex)
{
	CString str = GetString (iIndex);
	str.Remove ('&');
	return str;
}

void fsLangMgr::AddLngFileInfo(fsLngFileInfo &info)
{
	for (int i = 0; i < m_vLngFiles.size (); i++)
	{
		if (stricmp (m_vLngFiles [i].strLngName, info.strLngName) > 0)
		{
			m_vLngFiles.insert (i, info);
			return;
		}
	}

	m_vLngFiles.add (info);
}

void fsLangMgr::LoadBuiltInLngStrings()
{
	HGLOBAL hResource;
	HRSRC hRes;
	LPBYTE pbRes;

	hRes = FindResource (NULL, MAKEINTRESOURCE (IDR_ENG_LNGSTRINGS), "RT_LNGSTRINGS");
	if (hRes == NULL)
		return;

	hResource = LoadResource (NULL, hRes);
	pbRes = (LPBYTE) LockResource (hResource);
	LPBYTE pbResEnd = pbRes + SizeofResource (NULL, hRes);

	LPCSTR psz = (LPCSTR) pbRes;
	LPCSTR pszE = (LPCSTR) pbResEnd;
	bool bLngNamePassed = false;

	while (psz < pszE) 
	{
		CString str;
		while (psz < pszE && *psz != '\n' && *psz != '\r')
			str += *psz++;
		while (psz < pszE && (*psz == '\n' || *psz == '\r'))
			psz++;

		if (str == "" || str [0] == LNG_COMMENT_CHAR)
			continue;

		if (bLngNamePassed == false) {
			bLngNamePassed = true;
			continue;
		}

		PreprocessLanguageString (str);
		m_vBuiltInStrings.add (str);
	}
}

void fsLangMgr::PreprocessLanguageString(CString &str)
{
	str.Replace ("\\n", "\n");
}
