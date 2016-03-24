/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsHttpCookie.h"
class vmsHttpRequestInitData
{
public:
	
	void request_headers (const std::string &headers)
	{
		m_requestHeaders = headers;
	}

	std::string request_headers () const
	{
		return m_requestHeaders;
	}

	void cookies (const std::vector <vmsHttpCookie> &v)
	{
		m_cookies = v;
	}

	std::vector <vmsHttpCookie> cookies () const
	{
		return m_cookies;
	}

protected:
	std::string m_requestHeaders;
	std::vector <vmsHttpCookie> m_cookies;
};