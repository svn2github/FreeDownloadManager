/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsFile.h"

namespace vmsFDM {

class vmsFileUtil  
{
public:
	
	
	static void BuildPath (LPCTSTR pszPath)
	{
		TCHAR szPath [MAX_PATH];
		lstrcpy (szPath, pszPath);
		MakePathOK (szPath, true);
		LPTSTR psz = szPath;
		if (psz [1] == _T(':'))
			psz += 3;

		while (*psz)
		{
			TCHAR szPathNow [MAX_PATH];
			psz = _tcschr (psz, _T('\\')) + 1;
			lstrcpyn (szPathNow, szPath, psz - szPath);
			if (FALSE == CreateDirectory (szPathNow, NULL) && 
				ERROR_ALREADY_EXISTS != GetLastError ())
				throw GetLastError ();
		}
	}

	
	static fsString GetPathFromPathName (LPCTSTR pszPathName)
	{
		TCHAR szPath [MAX_PATH];
		lstrcpy (szPath, pszPathName);
		MakePathOK (szPath, false);
		LPTSTR psz = _tcsrchr (szPath, _T('\\'));
		if (psz == NULL)
			return _T("");
		*psz = 0;
		return szPath;
	}

	
	static void BuildPathToFile (LPCTSTR pszPathName)
	{
		BuildPath (GetPathFromPathName (pszPathName));
	}

	
	
	static void GetAppDataPath (LPCTSTR pszAppName, LPTSTR szPath)
	{
		LPITEMIDLIST pidl = NULL;
		SHGetSpecialFolderLocation (NULL, CSIDL_APPDATA, &pidl);
		SHGetPathFromIDList (pidl, szPath);

		vmsFileUtil::MakePathOK (szPath);
		lstrcat (szPath, pszAppName);
		lstrcat (szPath, _T("\\"));
	}

	
	static void MakePathOK (LPTSTR szPath, bool bNeedBackslashAtEnd = true)
	{
		LPTSTR psz = szPath;
		while (*psz)
		{
			if (*psz == _T('/'))
				*psz = _T('\\');
			psz++;
		}

		if (bNeedBackslashAtEnd && psz [-1] != _T('\\'))
		{
			psz [0] = _T('\\');
			psz [1] = 0;
		}
	}

	
	
	static void ReadHeader (vmsFile& file, fsString& strDesc, WORD& wVersion)
	{
		ReadString (file, strDesc);
		file.Read (&wVersion, sizeof (WORD));
	}

	
	static void WriteHeader (vmsFile& file, LPCTSTR pszDesc, WORD wVersion)
	{
		WriteString (file, pszDesc);
		file.Write (&wVersion, sizeof (WORD));
	}

	
	static void ReadString (vmsFile& file, fsString &str)
	{
		int l = 0;
		file.Read (&l, sizeof (l));

		if (l > 100 * 1024 * 1024)	
			throw ERROR_BAD_FORMAT;

		TCHAR* psz = new TCHAR [l+1];
		try 
		{
			file.Read (psz, l * sizeof(TCHAR));
			psz [l] = 0;
		} 
		catch (DWORD dw) 
		{
			delete [] psz;
			throw dw;
		}

		str = psz;
		delete [] psz;
	}

	static void WriteString (vmsFile& file, LPCTSTR psz)
	{
		int l = lstrlen (psz);
		file.Write (&l, sizeof (l));
		file.Write (psz, l * sizeof(TCHAR));
	}

	static BOOL DirectoryExists(const TCHAR* dirName)
	{
		DWORD attribs = ::GetFileAttributes(dirName);
		if (attribs == INVALID_FILE_ATTRIBUTES)
			return false;

		return (attribs & FILE_ATTRIBUTE_DIRECTORY);
	}
};

inline void vmsFILE_SAVE(vmsFile& file, LPCTSTR psz) {vmsFileUtil::WriteString (file, psz);}
inline void vmsFILE_SAVE(vmsFile& file, int iValue){file.Write (&iValue, sizeof (int));}
inline void vmsFILE_SAVE(vmsFile& file, UINT uValue){file.Write (&uValue, sizeof (UINT));}
inline void vmsFILE_SAVE(vmsFile& file, DWORD uValue){file.Write (&uValue, sizeof (DWORD));}
inline void vmsFILE_SAVE(vmsFile& file, double fValue){file.Write (&fValue, sizeof (double));}
inline void vmsFILE_SAVE(vmsFile& file, __int64 iValue){file.Write (&iValue, sizeof (__int64));}

inline void vmsFILE_READ(vmsFile& file, fsString& str) {vmsFileUtil::ReadString (file, str);}
inline void vmsFILE_READ(vmsFile& file, int& iValue){file.Read (&iValue, sizeof (int));}
inline void vmsFILE_READ(vmsFile& file, UINT& uValue){file.Read (&uValue, sizeof (UINT));}
inline void vmsFILE_READ(vmsFile& file, DWORD& uValue){file.Read (&uValue, sizeof (DWORD));}
inline void vmsFILE_READ(vmsFile& file, double& fValue){file.Read (&fValue, sizeof (double));}
inline void vmsFILE_READ(vmsFile& file, __int64& iValue){file.Read (&iValue, sizeof (__int64));}

} 

