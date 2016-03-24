/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsInternetProxyInfo : public vmsSerializableTs
{
public:
	typedef std::shared_ptr <vmsInternetProxyInfo> tSP;
	typedef std::shared_ptr <const vmsInternetProxyInfo> tcSP;

	vmsInternetProxyInfo () : m_uPort (0) {}
	virtual ~vmsInternetProxyInfo () {}

	std::wstring getAddress () const 
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_wstrAddr;
	}

	void setAddress (const std::wstring &wstrAddr)
	{
		vmsTHREAD_SAFE_SCOPE;
		if (m_wstrAddr == wstrAddr)
			return;
		m_wstrAddr = wstrAddr;
		setDirty ();
	}

	unsigned __int16 getPort () const 
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_uPort;
	}

	void setPort (unsigned __int16 uPort)
	{
		vmsTHREAD_SAFE_SCOPE;
		if (m_uPort == uPort)
			return;
		m_uPort = uPort;
		setDirty ();
	}

	bool hasLoginInfoSpecified () const 
	{
		vmsTHREAD_SAFE_SCOPE;
		return !m_wstrUserName.empty ();
	}

	std::wstring getUserName () const 
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_wstrUserName;
	}

	void setUserName (const std::wstring &wstrUserName)
	{
		vmsTHREAD_SAFE_SCOPE;
		if (m_wstrUserName == wstrUserName)
			return;
		m_wstrUserName = wstrUserName;
		setDirty ();
	}

	std::wstring getPassword () const 
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_wstrPassword;
	}

	void setPassword (const std::wstring &wstrPassword)
	{
		vmsTHREAD_SAFE_SCOPE;
		if (m_wstrPassword == wstrPassword)
			return;
		m_wstrPassword = wstrPassword;
		setDirty ();
	}

protected:
	std::wstring m_wstrAddr;
	unsigned __int16 m_uPort;
	std::wstring m_wstrUserName, m_wstrPassword;

public: 
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValue (L"addr", m_wstrAddr) &&
				pStm->SerializeValueS (L"port", m_uPort) &&
				pStm->SerializeValue (L"user", m_wstrUserName) &&
				pStm->SerializeValue (L"password", m_wstrPassword);
	}
};