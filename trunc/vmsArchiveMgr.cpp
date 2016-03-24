/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsArchiveMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsArchiveMgr::vmsArchiveMgr()
{
	m_pUsedArchive = NULL;
}

vmsArchiveMgr::~vmsArchiveMgr()
{

}

bool vmsArchiveMgr::Extract(LPCTSTR pszArchive, LPCTSTR pszOutFolder)
{
	m_pUsedArchive = &m_7zip;

	LPCTSTR pszExt;
	pszExt = _tcsrchr (pszArchive, _T('.'));
	if (pszExt)
	{
		pszExt++;
		if (lstrcmpi (pszExt, _T("rar")) == 0)
			m_pUsedArchive = &m_rar;
	}

	while (false == m_pUsedArchive->Extract (pszArchive, pszOutFolder))
	{
		if (m_pUsedArchive->get_LastExtractError () == AEE_ABORTED_BY_USER || 
				m_pUsedArchive == &m_7zip)
			return false;
		m_pUsedArchive = &m_7zip;
	}

	return true;
}

void vmsArchiveMgr::set_ArchiveCallback(vmsArchiveCallback *pAC)
{
	m_rar.set_ArchiveCallback (pAC);
	m_7zip.set_ArchiveCallback (pAC);
}

vmsArchiveExtractError vmsArchiveMgr::get_LastExtractError()
{
	return m_pUsedArchive ? m_pUsedArchive->get_LastExtractError () : AEE_NO_ERROR;
}
