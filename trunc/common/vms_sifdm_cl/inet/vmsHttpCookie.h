/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsHttpCookie :
	public vmsSerializable
{
public:
	std::wstring m_domain;
	bool m_validForSubdomains = false;
	std::wstring m_path;
	bool m_secure = false;
	time_t m_expires = 0;
	std::wstring m_name;
	std::wstring m_value;

public:
	std::string toNetscapeFmtString () const
	{
		std::stringstream result;
		char SEP = '\t';
		result << utf8FromWide (m_domain) << 
			SEP <<	(m_validForSubdomains ? "TRUE" : "FALSE") <<
			SEP << utf8FromWide (m_path) <<
			SEP << (m_secure ? "TRUE" : "FALSE") <<
			SEP << m_expires << 
			SEP << utf8FromWide (m_name) <<
			SEP << utf8FromWide (m_value);
		return result.str ();
	}

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		int subdomains = m_validForSubdomains, secure = m_secure;
		auto result = pStm->SerializeValue (L"domain", m_domain) &&
			pStm->SerializeValueS (L"valid_for_subdomains", subdomains) &&
			pStm->SerializeValue (L"path", m_path) &&
			pStm->SerializeValueS (L"secure", secure) &&
			pStm->SerializeValueS (L"expires", m_expires) &&
			pStm->SerializeValue (L"name", m_name) &&
			pStm->SerializeValue (L"value", m_value);
		m_validForSubdomains = subdomains != 0;
		m_secure = secure != 0;
		return true;
	}
};