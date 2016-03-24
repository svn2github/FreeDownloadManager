#pragma once
#include "zip.h"
class vmsZip  
{
public:	
	BOOL CreateNewZipFile (const std::wstring& file)
	{
		m_zip = zipOpen (utf8FromWide (file).c_str (), 0);
		return m_zip != NULL;
	}

	void CloseZipFile ()
	{
		if (m_zip)
		{
			zipClose (m_zip, NULL);
			m_zip = NULL;
		}
	}

	BOOL AddBuffer (LPCVOID pData, DWORD dwDataSize, const std::wstring& name)
	{
		zip_fileinfo zfileinfo = { 0 };
		struct _stat file_stat = { 0 };

		if (ZIP_OK != zipOpenNewFileInZip (m_zip, utf8FromWide (name).c_str (), &zfileinfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 9/*BEST*/))
			return FALSE;

		if (ZIP_OK != zipWriteInFileInZip (m_zip, pData, dwDataSize))
			return FALSE;

		if (Z_OK != zipCloseFileInZip (m_zip))
			return FALSE;

		return TRUE;
	}

	BOOL AddFile (LPCTSTR ptszFileName, const std::wstring& name)
	{
		HANDLE hFile = CreateFile (ptszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return FALSE;
		DWORD dwDataSize = GetFileSize (hFile, NULL);
		LPVOID pData = new BYTE[dwDataSize];
		ReadFile (hFile, pData, dwDataSize, &dwDataSize, NULL);
		CloseHandle (hFile);
		return AddBuffer (pData, dwDataSize, name);
	}

	vmsZip() : 
		m_zip (nullptr) {}

	~vmsZip()
	{
		if (m_zip)
			zipClose (m_zip, NULL);
	}

protected:
	zipFile m_zip;
};
