/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmBhIpcTaskHandshake :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTaskHandshake ()
	{
	}

	~vmsFdmBhIpcTaskHandshake ()
	{
	}

public:
	uint32_t m_api_version = 0;
	bool m_response_add_crlf = false;
	bool m_response_send_data_len_as_string = false;
	bool m_uri_encoding_for_rcv = false;
	bool m_uri_encoding_for_send = false;
	bool m_base64_encoding_for_rcv = false;
	bool m_base64_encoding_for_send = false;	
	std::wstring m_browser = L"Firefox";

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags) override
	{
		pStm->SerializeValueS (L"response_add_crlf", m_response_add_crlf);
		pStm->SerializeValueS (L"response_send_data_len_as_string", m_response_send_data_len_as_string);
		pStm->SerializeValueS (L"uri_encoding_for_rcv", m_uri_encoding_for_rcv);
		pStm->SerializeValueS (L"uri_encoding_for_send", m_uri_encoding_for_send);
		pStm->SerializeValueS (L"base64_encoding_for_rcv", m_base64_encoding_for_rcv);
		pStm->SerializeValueS (L"base64_encoding_for_send", m_base64_encoding_for_send);
		pStm->SerializeValue (L"browser", m_browser);
		return pStm->SerializeValueS (L"api_version", m_api_version);
	}
};

