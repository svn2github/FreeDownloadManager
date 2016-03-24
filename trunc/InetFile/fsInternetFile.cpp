/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "fsInternetFile.h"
#include "common.h"

fsInternetFile::fsInternetFile()
{
	m_hFile = NULL;
	m_pServer = NULL;
	m_pszLastError = NULL;
	m_enRST = RST_UNKNOWN;
	m_uFileSize = _UI64_MAX;

	m_bContentTypeValid = m_bDateValid = FALSE;
}

fsInternetFile::~fsInternetFile()
{
	CloseHandle ();

	SAFE_DELETE_ARRAY (m_pszLastError);
}

fsInternetResult fsInternetFile::Initialize(fsInternetServerConnection *pServer)
{
	m_pServer = pServer;
	return IR_SUCCESS;
}

LPCTSTR fsInternetFile::GetLastError()
{
	return m_pszLastError;
}

void fsInternetFile::CloseHandle()
{
	if (m_hFile)
	{
		InternetCloseHandle (m_hFile);
		m_hFile = NULL;
	}
}

BOOL fsInternetFile::GetLastModifiedDate(FILETIME *pTime)
{
	if (m_bDateValid)
		CopyMemory (pTime, &m_date, sizeof (m_date));
	return m_bDateValid;
}

BOOL fsInternetFile::GetContentType(LPTSTR pszType)
{
	*pszType = 0;

	if (m_bContentTypeValid)
		_tcscpy (pszType, m_strContentType);
		
	return m_bContentTypeValid;
}

fsResumeSupportType fsInternetFile::IsResumeSupported()
{
	return m_enRST;
}

LPCTSTR fsInternetFile::GetSuggestedFileName()
{
	return m_strSuggFileName;
}

fsInternetResult fsInternetFile::QuerySize(LPCTSTR)
{
	
	return IR_S_FALSE;
}

HINTERNET fsInternetFile::GetRawHandle()
{
	return m_hFile;
}

void fsInternetFile::SetSecurityCheckIgnoreFlags (DWORD flags)
{
	m_sctIgnoreFlags = flags;
}

fsSecurityCheckType fsInternetFile::get_lastSctFailure () const
{
	return m_lastSctFailure;
}