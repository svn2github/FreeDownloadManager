/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsInternetUserAgentInfoBase
{
public:
	typedef std::shared_ptr <vmsInternetUserAgentInfoBase> tSP;
	typedef std::shared_ptr <const vmsInternetUserAgentInfoBase> tcSP;

public:
	virtual std::wstring getUserAgentString () const = 0;
	virtual void setUserAgentString (const std::wstring &wstrUserAgentString) = 0;
	virtual ~vmsInternetUserAgentInfoBase () {}
};

class vmsInternetUserAgentInfo : 
	public vmsInternetUserAgentInfoBase,
	public vmsThreadSafe
{
public:
	typedef std::shared_ptr <vmsInternetUserAgentInfo> tSP;
	typedef std::shared_ptr <const vmsInternetUserAgentInfo> tcSP;

	vmsInternetUserAgentInfo () {}
	virtual ~vmsInternetUserAgentInfo () {}

	virtual std::wstring getUserAgentString () const override
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_wstrUserAgentString;
	}

	virtual void setUserAgentString (const std::wstring &wstrUserAgentString) override
	{
		vmsTHREAD_SAFE_SCOPE;
		m_wstrUserAgentString = wstrUserAgentString;
	}

protected:
	std::wstring m_wstrUserAgentString;
};