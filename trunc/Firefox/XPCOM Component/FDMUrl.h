/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __FDMURL_H_
#define __FDMURL_H_

#include "ivmsfdmff.h"

#define FDMURL_CONTRACTID "@freedownloadmanager.org/FDMUrl;1"
#define FDMURL_CLASSNAME "Free Download Manager Url component for Firefox"

#define FDMURL_CID { 0xde19d41e, 0xecd8, 0x4e0b, { 0x99, 0x0, 0xa2, 0x3b, 0x65, 0xe0, 0x12, 0x5e } }

#include <windows.h>
#include <fsString.h>
#include <comutil.h>
#include <string>

class CFDMUrl : public IFDMUrl
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IFDMURL

  CFDMUrl();

private:
  ~CFDMUrl();

protected:
	_bstr_t m_strUrl;
	_bstr_t m_strReferer;
	_bstr_t m_strComment;
	_bstr_t m_strCookies;
	_bstr_t m_strPostData;
	std::wstring m_wstrUserAgent, m_wstrOriginalUrl;
};

#endif 