/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <shellapi.h>

inline void vmsGetPath (LPCTSTR pszFile, LPTSTR pszPath)
{
	_tcscpy (pszPath, pszFile);

	int len = (int)_tcslen (pszPath) - 1;

	while (len >= 0 && pszPath [len] != '\\' && pszPath [len] != '/')
		len--;

	pszPath [len+1] = 0;
}

template <typename T>
T vmsFileNameFromPath (const T& path)
{
	auto pos = path.rfind ('\\');
	if (pos == path.npos)
		pos = path.rfind ('/');
	if (pos == path.npos)
		return path;
	return T (path.begin () + pos + 1, path.end ());
}

inline BOOL vmsBuildPath (LPCTSTR pszPath)
{
	int len = (int)_tcslen (pszPath);
	int start = 0;

	
	if (pszPath [0] == '\\' && pszPath [1] == '\\')
	{
		
		LPCTSTR psz = _tcschr (pszPath + 2, '\\');
		
		if (psz)
			psz = _tcschr (psz+1, '\\');
		if (psz)
			psz++;
		if (psz == NULL)
		{
			
			SetLastError (ERROR_PATH_NOT_FOUND);
			return FALSE;
		}
		
		
		start = (int)(psz - pszPath);
	}
	else
	{
		if (pszPath [1] == ':')
			start = 3;
	}

	for (int i = start; i < len; i++)
	{
		if (pszPath [i] == '\\' || pszPath [i] == '/')
		{
			
			
			
			
			
			

			TCHAR szPath2 [MAX_PATH];

			CopyMemory (szPath2, pszPath, i * sizeof (TCHAR));
			szPath2 [i] = 0;

			if (FALSE == CreateDirectory (szPath2, NULL))	
			{
				if (GetLastError () != ERROR_ALREADY_EXISTS)
					return FALSE;
			}
		}
	}

	if (pszPath [len-1] != '\\' && pszPath [len-1] != '/')
	{
		if (FALSE == CreateDirectory (pszPath, NULL))
		{
			if (GetLastError () != ERROR_ALREADY_EXISTS)
				return FALSE;
		}
	}

	return TRUE;
}

inline BOOL vmsBuildPathToFile (LPCTSTR pszFileName)
{
	TCHAR szPath [MAX_PATH];
	vmsGetPath (pszFileName, szPath);
	return vmsBuildPath (szPath);
}

inline void vmsTrimPath( std::wstring& wstrPath)
{
	while (!wstrPath.empty() && (wstrPath.back() == L'\\' || wstrPath.back() == L'/'))
		wstrPath.erase(wstrPath.end() - 1);
}

inline std::wstring vmsAppendPath(const std::wstring& wstrPathFirst, const std::wstring& wstrPathSecond)
{
	std::wstring ret(wstrPathFirst);
	if (!wstrPathSecond.empty())
	{
		vmsTrimPath(ret);
		if (wstrPathSecond.front() != L'\\' && wstrPathSecond.front() != L'/')
			ret += L'\\';
		ret += wstrPathSecond;
	}
	return ret;
}

inline bool vmsFileExists(const std::wstring& wstrFile)
{
	DWORD dwAttr = GetFileAttributes(wstrFile.c_str());
	return (dwAttr != INVALID_FILE_ATTRIBUTES);
}

inline bool vmsIsFolder(const std::wstring& wstrFolder)
{
	DWORD dwAttr = GetFileAttributes(wstrFolder.c_str());
	return (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY));
}

inline bool vmsRemoveFile (const std::wstring& wstrFile)
{
	return !_wremove (wstrFile.c_str ());
}

inline bool vmsCheckDirectoryWriteAccess (const std::wstring& path)
{
	const auto testName = L"559B4D33-E48B-43FD-A679-721A0E68C87F.test";
	std::wstring file = path + L"\\" + testName;
	HANDLE hFile = CreateFile (file.c_str (), GENERIC_WRITE, 
		0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	CloseHandle (hFile);
	DeleteFile (file.c_str ());
	return true;
}

inline BOOL vmsRemoveDirectoryWithContents(const std::wstring &path, bool silently = true)
{
	SHFILEOPSTRUCT sh;
	ZeroMemory(&sh, sizeof(sh));
	sh.wFunc = FO_DELETE;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;
	if (silently)
		sh.fFlags |= FOF_SILENT;

	std::vector <wchar_t> vtchFrom;
	vtchFrom.resize(path.length() + 1);
	CopyMemory(&vtchFrom[0], path.c_str(), (path.length() + 1) * sizeof(wchar_t));
	vtchFrom.push_back(0);
	sh.pFrom = &vtchFrom[0];

	return (SHFileOperation(&sh) == 0);
}

inline BOOL vmsMoveDirectoryWithContents(const std::wstring &pathFrom, const std::wstring &pathTo, bool silently = true)
{
	SHFILEOPSTRUCT sh;
	ZeroMemory(&sh, sizeof(sh));
	sh.wFunc = FO_MOVE;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI;
	if (silently)
		sh.fFlags |= FOF_SILENT;

	std::vector <wchar_t> vtchFrom;
	vtchFrom.resize(pathFrom.length() + 1);
	CopyMemory(&vtchFrom[0], pathFrom.c_str(), (pathFrom.length() + 1) * sizeof(wchar_t));
	vtchFrom.push_back(0);
	sh.pFrom = &vtchFrom[0];

	std::vector <wchar_t> vtchTo;
	vtchTo.resize(pathTo.length() + 1);
	CopyMemory(&vtchTo[0], pathTo.c_str(), (pathTo.length() + 1) * sizeof(wchar_t));
	vtchTo.push_back(0);
	sh.pTo = &vtchTo[0];

	return (SHFileOperation(&sh) == 0);
}

inline std::wstring vmsGenerateUniqueFileName(
	const std::wstring &wstrFileName, 
	const std::wstring &wstrAddExtension = L"")
{
	std::wstring wstr(wstrFileName);

	std::wstring wstrExt(wstrAddExtension);
	if (!wstrExt.empty() && wstrExt.front() != L'.')
		wstrExt = L'.' + wstrExt;

	for (size_t i = 1;; i++)
	{
		if (!vmsFileExists(wstr + wstrExt))
			break;

		std::wstringstream wss;
		wss << L" (" << i << L")";

		size_t nPos = wstrFileName.rfind('.');
		if (nPos == std::wstring::npos)
			nPos = wstrFileName.length();

		wstr = wstrFileName;
		wstr.insert(nPos, wss.str());
	}

	return (wstr + wstrExt);
}

inline tstring vmsGetTempPath ()
{
	TCHAR path [MAX_PATH] = _T ("");
	GetTempPath (MAX_PATH, path);
	assert (*path);
	return path;
}

inline tstring vmsGetFileSystemName (const tstring& rootPathName)
{
	TCHAR fs [MAX_PATH+1] = _T("");
	if (!::GetVolumeInformation (rootPathName.c_str (), nullptr, 0, 
			nullptr, nullptr, nullptr, fs, _countof (fs)))
	{
		return tstring ();
	}
	return fs;
}

inline uint64_t vmsGetMaximumFileSize (const tstring &fileSystemName)
{
	if (fileSystemName == _T("FAT12"))
		return 8*1024*1024;

	if (fileSystemName == _T("FAT16"))
		return UINT32_MAX;

	if (fileSystemName == _T("FAT32"))
		return UINT32_MAX;

	if (fileSystemName == _T("NTFS"))
		return (256ULL*1024*1024*1024*1024 - 64*1024);

	return 0; 
}

inline uint64_t vmsGetMaximumFileSizeForFolder(const tstring &path)
{
	tstring rootPath = path;
	assert(rootPath.length() >= 3);
	if (rootPath.length() < 3)
		return -1;

	rootPath.erase(rootPath.begin() + 3, rootPath.end());

	auto fsname = vmsGetFileSystemName(rootPath);
	assert(!fsname.empty());
	if (fsname.empty())
		return -1;

	auto maxsize = vmsGetMaximumFileSize(fsname);
	assert(maxsize);
	if (!maxsize)
		return -1;

	return maxsize;
}

inline bool vmsCheckMaximumFileSize(const tstring &path, uint64_t filesize)
{
	return (filesize <= vmsGetMaximumFileSizeForFolder(path));
}