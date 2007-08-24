/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "fsLocalFile.h"
#include "fsURL.h"        

fsLocalFile::fsLocalFile()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_enRST = RST_PRESENT;
}

fsLocalFile::~fsLocalFile()
{
	CloseHandle ();
	m_hFile = NULL;
}

fsInternetResult fsLocalFile::Open(LPCSTR pszFilePath, UINT64 uStartPos)
{
	CloseHandle ();

	fsString strPath;
	if (m_strServer != "")
	{
		
		strPath = "\\\\";
		strPath += m_strServer;
	}
	strPath += pszFilePath;

	m_hFile = CreateFile (strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		0, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
		return fsWinErrorToIR ();

	DWORD dw1, dw2;
	dw1 = ::GetFileSize (m_hFile, &dw2);
	m_uFileSize = dw1 | UINT64 (dw2) << 32;

	if (uStartPos)
	{
		LONG lOffsetHi = 0;
		UINT64 uOffset = uStartPos;
		DWORD dwMethod = FILE_BEGIN;
		
		while (uOffset > DWORD (-1))
		{
			SetFilePointer (m_hFile, -1, &lOffsetHi, dwMethod);
			if (GetLastError ())
				return IR_RANGESNOTAVAIL;

			uOffset -= DWORD (-1);
			dwMethod = FILE_CURRENT;
			lOffsetHi = 0;
		}
		if (uOffset != SetFilePointer (m_hFile, DWORD (uOffset), &lOffsetHi, dwMethod))
			return IR_RANGESNOTAVAIL;
	}
	
	return IR_SUCCESS;
}

fsInternetResult fsLocalFile::Read(LPBYTE pBuffer, DWORD dwSize, DWORD *pdwRead)
{
	if (FALSE == ReadFile (m_hFile, pBuffer, dwSize, pdwRead, NULL))
		return fsWinErrorToIR ();

	return IR_SUCCESS;
}

void fsLocalFile::Initialize(LPCSTR pszServer)
{
	m_strServer = pszServer;
}

void fsLocalFile::CloseHandle()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle (m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

fsInternetResult fsLocalFile::QuerySize(LPCSTR pszFilePath)
{
	HANDLE hFile = CreateFile (pszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return fsWinErrorToIR ();

	DWORD dw1, dw2;
	dw1 = ::GetFileSize (hFile, &dw2);
	m_uFileSize = dw1 | UINT64 (dw2) << 32;

	::CloseHandle (hFile);

	return IR_SUCCESS;
}

fsInternetResult fsLocalFile::OpenEx(LPCSTR pszFilePath, UINT64 uStartPos, UINT64 , UINT64 uUploadTotalSize)
{
	if (uUploadTotalSize != _UI64_MAX)
		return IR_ERROR;
	return Open (pszFilePath, uStartPos);
}

fsInternetResult fsLocalFile::Write(LPBYTE , DWORD , DWORD* )
{
	return IR_ERROR;
}
