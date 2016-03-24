/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "fsInternetURLFiles.h"

fsInternetURLFiles::fsInternetURLFiles()
{
	m_pFiles = NULL;
}

fsInternetURLFiles::~fsInternetURLFiles()
{

}

fsInternetResult fsInternetURLFiles::GetList(fsInternetURLServer *pServer, LPCTSTR pszPath)
{
	
	if (pServer->IsHttpServer () || pServer->IsHttpsServer ())
	{
		
		m_pFiles = &m_httpFiles;
		m_httpFiles.SetServer (pServer->GetHttpServer ());
		m_httpFiles.UseSecure (pServer->IsHttpsServer ());
	}
	else if (pServer->IsFtpServer ())
	{
		
		m_pFiles = &m_ftpFiles;
		m_ftpFiles.SetServer (pServer->GetFtpServer ());
	}
	else
		return IR_BADURL;

	return m_pFiles->GetList (pszPath);
}

UINT fsInternetURLFiles::GetFileCount()
{
	return m_pFiles->GetFileCount ();
}

fsFileInfo* fsInternetURLFiles::GetFileInfo(UINT uIndex)
{
	return m_pFiles->GetFileInfo (uIndex);
}

void fsInternetURLFiles::Abort()
{
	if (m_pFiles)
		m_pFiles->Abort ();
}

LPCTSTR fsInternetURLFiles::GetLastError()
{
	return m_pFiles->GetLastError ();
}

LPCTSTR fsInternetURLFiles::GetCurrentPath()
{
	return m_pFiles ? m_pFiles->GetCurrentPath () : NULL;
}

void fsInternetURLFiles::SetHttpEventFunc(fntHttpFilesEventFunc pfn, LPVOID lpParam)
{
	m_httpFiles.SetEventFunc (pfn, lpParam);
}

void fsInternetURLFiles::RetreiveInfoWhileGettingList(BOOL b)
{
	m_httpFiles.RetreiveInfoWhileGettingList (b);
}
