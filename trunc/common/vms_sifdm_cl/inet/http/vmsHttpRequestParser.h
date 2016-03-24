/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsHttpParser.h"

class vmsHttpRequestParser : public vmsHttpParser  
{
public:
	enum RequestType
	{
		GET, 
		POST,
		UNKNOWN,
	};

public:
	RequestType getRequestType () const
	{
		return m_enRT;
	}

	LPCSTR getPath () const
	{
		return m_strPath.c_str ();
	}

	void ParseHeader (LPCSTR pszHdr)
	{
		vmsHttpParser::ParseHeader (pszHdr);

		if (strnicmp (pszHdr, "GET ", 4) == 0)
			m_enRT = GET;
		else if (strnicmp (pszHdr, "POST ", 5) == 0)
			m_enRT = POST;
		else
			m_enRT = UNKNOWN;

		m_strPath = m_strHttpVersion = "";

		while (*pszHdr && *pszHdr != ' ')
			pszHdr++;

		if (*pszHdr == 0)
			return;

		while (*pszHdr == ' ')
			pszHdr++;

		while (*pszHdr != ' ' && *pszHdr != 0)
			m_strPath += *pszHdr++;

		if (*pszHdr == 0)
			return;

		while (*pszHdr == ' ')
			pszHdr++;

		while (*pszHdr != 0 && *pszHdr != '\r' && *pszHdr != '\n' && *pszHdr != ' ')
			m_strHttpVersion += *pszHdr++;
	}

	vmsHttpRequestParser()
	{
	}

	virtual ~vmsHttpRequestParser()
	{
	}

protected:
	RequestType m_enRT;
	std::string m_strPath; 
	std::string m_strHttpVersion;
};
