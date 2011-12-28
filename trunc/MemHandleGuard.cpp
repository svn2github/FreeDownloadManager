/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "MemHandleGuard.h"

CMemHandleGuard::CMemHandleGuard(LPVOID& pMemPtr, bool& bFailedToRelease)
	: m_pMemPtr(pMemPtr),
	  m_bFailedToRelease(bFailedToRelease)
{
}

CMemHandleGuard::~CMemHandleGuard()
{
	try {
		HLOCAL hMem = (HLOCAL)m_pMemPtr;
		if (hMem != 0) {
			if (LocalFree(hMem) != 0) {

				m_bFailedToRelease = true;
				
			}
			m_pMemPtr = 0;
		}
	} catch (...) {

		m_bFailedToRelease = true;

	}
}
