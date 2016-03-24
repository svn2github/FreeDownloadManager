/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmBhIpcDownloadInfo :
	public vmsSerializable
{
public:
	vmsFdmBhIpcDownloadInfo ()
	{
	}

	~vmsFdmBhIpcDownloadInfo ()
	{
	}

public:
	std::wstring m_url;
	std::wstring m_originalUrl;
	std::wstring m_suggestedName;
	std::wstring m_httpReferer;
	std::wstring m_userAgent;
	std::wstring m_httpCookies;
	std::wstring m_httpPostData;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags) override
	{
		pStm->SerializeValue (L"suggestedName", m_suggestedName);
		pStm->SerializeValue (L"httpReferer", m_httpReferer);
		pStm->SerializeValue (L"userAgent", m_userAgent);
		pStm->SerializeValue (L"httpCookies", m_httpCookies);
		pStm->SerializeValue (L"httpPostData", m_httpPostData);
		pStm->SerializeValue (L"originalUrl", m_originalUrl);
		return pStm->SerializeValue (L"url", m_url);
	}
};

