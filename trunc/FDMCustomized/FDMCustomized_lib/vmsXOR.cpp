/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsXOR.h"
#include <memory>
#include "Utils.h"

vmsXOR::vmsXOR()
{

}

vmsXOR::~vmsXOR()
{

}

void vmsXOR::set_Key(LPCSTR tszKey)
{
	m_strKey = tszKey;
}

void vmsXOR::Encrypt(LPBYTE pbIn, LPBYTE pbOut, DWORD dwSize)
{
	LPCSTR szKey = m_strKey.c_str ();

	while (dwSize--)
	{
		*pbOut++ = *pbIn++ ^ *szKey++;
		if (*szKey == 0)
			szKey = m_strKey.c_str ();
	}
}

void vmsXOR::Decrypt(LPBYTE pbIn, LPBYTE pbOut, DWORD dwSize)
{
	Encrypt (pbIn, pbOut, dwSize);
}
