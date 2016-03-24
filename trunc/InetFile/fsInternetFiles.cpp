/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include <windows.h>
#include "common.h"
#include "fsInternetFiles.h"

fsInternetFiles::fsInternetFiles()
{
	m_strPath = _T("");
}

fsInternetFiles::~fsInternetFiles()
{

}

UINT fsInternetFiles::GetFileCount()
{
	return m_vFiles.size ();
}

fsFileInfo* fsInternetFiles::GetFileInfo(UINT uIndex)
{
	return &m_vFiles [uIndex];
}

LPCTSTR fsInternetFiles::GetCurrentPath()
{
	return m_strPath;
}
