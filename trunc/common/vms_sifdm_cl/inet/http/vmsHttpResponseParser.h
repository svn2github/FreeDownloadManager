/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsHttpParser.h"

class vmsHttpResponseParser : public vmsHttpParser  
{
public:
	UINT getStatusCode () const
	{
		return m_nStatusCode;
	}

	LPCSTR getResponseResult () const
	{
		return m_strResponseResult.c_str ();
	}

	void ParseHeader (LPCSTR pszHdr)
	{
		vmsHttpParser::ParseHeader (pszHdr);

		m_strHttpVersion = m_strResponseResult = "";
		m_nStatusCode = 0;

		while (*pszHdr && *pszHdr != ' ')
			m_strHttpVersion += *pszHdr++;

		if (*pszHdr == 0)
			return;

		while (*pszHdr == ' ')
			pszHdr++;

		while (*pszHdr != '\r' && *pszHdr != '\n' && *pszHdr != 0)
			m_strResponseResult += *pszHdr++;

		m_nStatusCode = atoi (m_strResponseResult.c_str ());
	}

	vmsHttpResponseParser()
	{
		m_nStatusCode = 0;
	}

	virtual ~vmsHttpResponseParser()
	{
	}

protected:
	string m_strHttpVersion;
	string m_strResponseResult; 
	UINT m_nStatusCode;
};
