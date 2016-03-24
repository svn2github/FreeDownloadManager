/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSHASH_H__A22980FB_579D_4BD9_825C_699E3BED340F__INCLUDED_)
#define AFX_VMSHASH_H__A22980FB_579D_4BD9_825C_699E3BED340F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <windows.h>
#pragma warning (push, 3)
#include "vmsHashEvents.h"
#include <string>
#pragma warning (pop)

enum vmsHashAlgorithm
{
	HA_CRC32,
	HA_MD5,
	HA_SHA1,
	HA_SHA2,
};

enum vmsHash_SHA2Strength {
	HSHA2S_256,
	HSHA2S_384,
	HSHA2S_512,
};

class vmsHash  
{
public:
	void set_SHA2Strength (vmsHash_SHA2Strength en);
	void set_EventsHandler (vmsHashEvents* pEvents);
	tstring Hash (LPCTSTR pszFile, vmsHashAlgorithm enHA = HA_MD5);

	vmsHash();
	virtual ~vmsHash();

protected:
	tstring Hash_SHA2 (LPCTSTR pszFile);
	tstring Hash_SHA1 (LPCTSTR pszFile);
	tstring Hash_CRC32 (LPCTSTR pszFile);
	tstring Hash_ResultToStr (unsigned char* pHR, int nSize);
	tstring Hash_MD5 (LPCTSTR pszFile);
	UINT64 Hash_MD5_File (LPCTSTR fn, unsigned long seed, void *mdContext);
	vmsHashEvents* m_pEvents;
	int m_nSHA2Strength;
};

#endif 
