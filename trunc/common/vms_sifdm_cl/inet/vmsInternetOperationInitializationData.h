/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsInternetProxyInfo.h"
#include "vmsInternetUserAgentInfo.h"
#include "vmsHttpRequestInitData.h"
#include "../progress/vmsOperationSpeedLimitManager.h"

class vmsInternetOperationInitializationData : public vmsThreadSafe
{
public:
	typedef std::shared_ptr <vmsInternetOperationInitializationData> tSP;
	typedef std::shared_ptr <const vmsInternetOperationInitializationData> tcSP;

	virtual void setProxyInfo (vmsInternetProxyInfo::tSP spProxyInfo)
	{
		vmsTHREAD_SAFE_SCOPE;
		m_spProxyInfo = spProxyInfo;
	}

	virtual vmsInternetProxyInfo::tcSP getProxyInfo () const
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_spProxyInfo;
	}

	virtual vmsInternetProxyInfo::tSP getProxyInfo ()
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_spProxyInfo;
	}

	virtual void setUserAgentInfo (vmsInternetUserAgentInfoBase::tSP spUserAgentInfo)
	{
		vmsTHREAD_SAFE_SCOPE;
		m_spUserAgentInfo = spUserAgentInfo;
	}

	virtual vmsInternetUserAgentInfoBase::tcSP getUserAgentInfo () const
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_spUserAgentInfo;
	}

	virtual vmsInternetUserAgentInfoBase::tSP getUserAgentInfo ()
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_spUserAgentInfo;
	}

	virtual void setHttpData (const std::shared_ptr <vmsHttpRequestInitData> &data)
	{
		vmsTHREAD_SAFE_SCOPE;
		m_httpData = data;
	}

	std::shared_ptr <vmsHttpRequestInitData> getHttpData () const
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_httpData;
	}

	virtual void setDownloadSpeedLimitManager (
		const std::shared_ptr <vmsOperationSpeedLimitManager> &manager)
	{
		vmsTHREAD_SAFE_SCOPE;
		m_downloadSpeedLimit = manager;
	}

	std::shared_ptr <vmsOperationSpeedLimitManager> getDownloadSpeedLimitManager () const
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_downloadSpeedLimit;
	}

	virtual void setUploadSpeedLimitManager (
		const std::shared_ptr <vmsOperationSpeedLimitManager> &manager)
	{
		vmsTHREAD_SAFE_SCOPE;
		m_uploadSpeedLimit = manager;
	}

	std::shared_ptr <vmsOperationSpeedLimitManager> getUploadSpeedLimitManager () const
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_uploadSpeedLimit;
	}

	void readDataFrom (vmsInternetOperationInitializationData *p)
	{
		setProxyInfo (p->getProxyInfo ());
		setUserAgentInfo (p->getUserAgentInfo ());
		setHttpData (p->getHttpData ());
		setDownloadSpeedLimitManager (p->getDownloadSpeedLimitManager ());
		setUploadSpeedLimitManager (p->getUploadSpeedLimitManager ());
	}

	vmsInternetOperationInitializationData () {}
	virtual ~vmsInternetOperationInitializationData () {}

protected:
	vmsInternetProxyInfo::tSP m_spProxyInfo;
	vmsInternetUserAgentInfoBase::tSP m_spUserAgentInfo;
	std::shared_ptr <vmsHttpRequestInitData> m_httpData;
	std::shared_ptr <vmsOperationSpeedLimitManager> m_downloadSpeedLimit;
	std::shared_ptr <vmsOperationSpeedLimitManager> m_uploadSpeedLimit;
};

class vmsInternetOperationInitializationDataReceiver
{
public:
	vmsInternetOperationInitializationDataReceiver () {}

	virtual ~vmsInternetOperationInitializationDataReceiver () {}

	virtual void setInternetOperationInitializationData (vmsInternetOperationInitializationData::tcSP spData)
	{
		m_spInetOpInitData = spData;
	}

	virtual vmsInternetOperationInitializationData::tcSP getInternetOperationInitializationData () const
	{
		return m_spInetOpInitData;
	}

	void readDataFrom (const vmsInternetOperationInitializationDataReceiver *p)
	{
		setInternetOperationInitializationData (p->getInternetOperationInitializationData ());
	}

protected:
	vmsInternetOperationInitializationData::tcSP m_spInetOpInitData;
};