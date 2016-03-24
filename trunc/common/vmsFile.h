/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "misc.h"

namespace vmsFDM
{

class vmsFile  
{
public:
	
	void ReadDouble (double &f)
	{
		Read (&f, sizeof (f));
	}

	void ReadInt64(__int64 &i)
	{
		Read (&i, sizeof (i));
	}

	void ReadInt (int &i)
	{
		Read (&i, sizeof (i));
	}

	void WriteInt (int i)
	{
		Write (&i, sizeof (i));
	}

	void WriteInt64 (__int64 i)
	{
		Write (&i, sizeof (i));
	}

	void WriteDouble (double f)
	{
		Write (&f, sizeof (f));
	}

	void ReadString(fsString& str)
	{
		LPTSTR psz;
		if (FALSE == fsReadStrFromFile (&psz, m_hFile))
			throw GetLastError ();
		str  = psz;
		delete [] psz;
	}

	void ReadString(std::string& str)
	{
		LPSTR psz;
		if (FALSE == fsReadStrFromFileA (&psz, m_hFile))
			throw GetLastError ();
		str  = psz;
		delete [] psz;
	}
	
	void WriteString (LPCWSTR psz)
	{
		if (FALSE == fsSaveStrToFileW (psz, m_hFile))
			throw GetLastError ();
	}

	void WriteString (LPCSTR psz)
	{
		if (FALSE == fsSaveStrToFileA (psz, m_hFile))
			throw GetLastError ();
	}

	
	void Seek (DWORD dwDistance, DWORD dwMethod = FILE_BEGIN)
	{
		SetFilePointer (m_hFile, dwDistance, NULL, dwMethod);
	}

	
	DWORD GetSize()
	{
		if (m_hFile == INVALID_HANDLE_VALUE)
			throw ERROR_INVALID_HANDLE;

		return GetFileSize (m_hFile, NULL);
	}

	
	void Create (LPCTSTR pszFileName, DWORD dwAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwCreationDisposition = OPEN_EXISTING, DWORD dwFlags = FILE_ATTRIBUTE_NORMAL)
	{
		Close ();

		m_hFile = CreateFile (pszFileName, dwAccess, dwShareMode, NULL, dwCreationDisposition,
			dwFlags, NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
			throw GetLastError ();
	}

	
	void Close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			if (FALSE == CloseHandle (Detach ()))
				throw GetLastError ();
		}
	}

	
	
	void Attach (HANDLE hFile, bool bAutoClose = true)
	{
		Close ();
		m_hFile = hFile;
		m_bAutoClose = bAutoClose;
	}

	
	HANDLE Detach()
	{
		HANDLE h = m_hFile;
		m_hFile = INVALID_HANDLE_VALUE;
		return h;
	}

	
	void Read (LPVOID pvBuffer, DWORD dwToRead, DWORD* pdwRead = NULL)
	{
		if (m_hFile == INVALID_HANDLE_VALUE)
			throw ERROR_INVALID_HANDLE;

		DWORD dwRead = 0;

		if (FALSE == ReadFile (m_hFile, pvBuffer, dwToRead, &dwRead, NULL))
			throw GetLastError ();

		if (pdwRead)
			*pdwRead = dwRead;

		if (m_bThrowOnPartialRead && dwRead != dwToRead)
			throw ERROR_BAD_FORMAT;
	}

	void Write (LPCVOID pvBuffer, DWORD dwToWrite)
	{
		if (m_hFile == INVALID_HANDLE_VALUE)
			throw ERROR_INVALID_HANDLE;

		DWORD dwWritten = 0;

		if (FALSE == WriteFile (m_hFile, pvBuffer, dwToWrite, &dwWritten, NULL))
			throw GetLastError ();

		if (dwWritten != dwToWrite)
			throw ERROR_BAD_LENGTH;
	}

	
	operator HANDLE() const {return m_hFile;}
	
	vmsFile(HANDLE hFile = INVALID_HANDLE_VALUE, bool bAutoClose = true)
	{
		m_hFile = hFile;
		m_bAutoClose = bAutoClose;
		m_bThrowOnPartialRead = true;
	}

	virtual ~vmsFile()
	{
		if (m_bAutoClose)
			Close ();
	}

protected:
	
	bool m_bThrowOnPartialRead;
	bool m_bAutoClose;
	HANDLE m_hFile;	
};

};

