/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFlvSniffCallProcTask :
	public vmsSerializable
{
public:
	vmsFlvSniffCallProcTask ()
	{
	}

	~vmsFlvSniffCallProcTask ()
	{
	}

public:
	std::wstring m_name;

	
	std::wstring m_webPageUrl, m_frameUrl, m_swfUrl,
		m_flashVars, m_otherSwfUrls, m_otherFlashVars;

	
	std::wstring m_url, m_srcTabUrl;

	std::wstring m_requestHeaders, m_responseHeaders;
	std::wstring m_requestId;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		pStm->SerializeValue (L"webPageUrl", m_webPageUrl);
		pStm->SerializeValue (L"frameUrl", m_frameUrl);
		pStm->SerializeValue (L"swfUrl", m_swfUrl);
		pStm->SerializeValue (L"flashVars", m_flashVars);
		pStm->SerializeValue (L"otherSwfUrls", m_otherSwfUrls);
		pStm->SerializeValue (L"otherFlashVars", m_otherFlashVars);
		pStm->SerializeValue (L"url", m_url);
		pStm->SerializeValue (L"srcTabUrl", m_srcTabUrl);
		pStm->SerializeValue (L"requestHeaders", m_requestHeaders);
		pStm->SerializeValue (L"responseHeaders", m_responseHeaders);
		pStm->SerializeValue (L"requestId", m_requestId);
		return pStm->SerializeValue (L"name", m_name);
	}
};

