/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "common\vms_sifdm_cl\vmsResourceObjectsHolder.h"
#include "YouTubeParser\vmsYouTubeParserDll.h"
class vmsYouTubeParserObjectsHolder : 
	public vmsResourceObjectsHolder <YouTubeParser>
{
public:
	vmsYouTubeParserObjectsHolder(const std::wstring& dllPath) :
		m_dllYtpName (dllPath)
	{
		assert (!m_dllYtpName.empty ());
	}

	~vmsYouTubeParserObjectsHolder(void)
	{
	}

	std::unique_ptr <vmsResourceObjectHolder<YouTubeParser>> createResourceObject (const std::wstring& url)
	{
		boost::mutex::scoped_lock lock (m_mutex);
		return registerRawResourceObject (
			createRawResourceObject (url));
	}

	virtual void releaseResourceReference () override
	{
		assert (m_objects.empty ());
		m_dllYtp = nullptr;
	}

	virtual void loadResource (const std::wstring& resourceLocation) override
	{
		m_dllYtp.reset (new vmsYouTubeParserDll (m_dllYtpName = resourceLocation));
		m_mutex.unlock ();
	}

protected:
	std::wstring m_dllYtpName;
	std::unique_ptr <vmsYouTubeParserDll> m_dllYtp;

protected:
	YouTubeParser* createRawResourceObject (const std::wstring& url)
	{
		if (!m_dllYtp)
			m_dllYtp.reset (new vmsYouTubeParserDll (m_dllYtpName));
		return m_dllYtp->createYouTubeParser (url).release ();
	}
};

