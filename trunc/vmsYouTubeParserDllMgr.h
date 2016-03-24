/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmDllUpdater.h"
#include "vmsYouTubeParserObjectsHolder.h"

class vmsYouTubeParserDllMgr : public vmsCreatesThreads
{
public:
	vmsYouTubeParserDllMgr(std::shared_ptr <vmsAppDataFolder> appDataFolder, 
		bool currentBuildFirstRun);
	~vmsYouTubeParserDllMgr(void);

	static void reset(std::shared_ptr<vmsYouTubeParserDllMgr> youTubeParserDllMgr)
	{
		s_youTubeParserDllMgr = youTubeParserDllMgr;
	}

	static std::shared_ptr<vmsYouTubeParserDllMgr> get()
	{
		return s_youTubeParserDllMgr;
	}

	vmsYouTubeParserObjectsHolder* objectsHolder () 
	{
		return m_ytpObjsHolder.get ();
	}

	bool PerformUpdate ()
	{
		{
			boost::lock_guard <boost::mutex> lock (m_mutex);
			if (m_bPerformingUpdate)
				return false;
			m_bPerformingUpdate = true;
		}

		return PerformUpdate2 ();
	}

	void onYouTubeParserDllParseError ()
	{
		boost::lock_guard <boost::mutex> lock (m_mutex);
		if (m_bPerformingUpdate)
			return;
		if (std::difftime (time (nullptr), m_timeLastUpdateAttempt) < 3600)
			return; 
		m_bPerformingUpdate = true;
		CreateThread (_threadPerformUpdate);
	}

	std::wstring getYouTubeParserVersion()
	{
		boost::lock_guard <boost::mutex> lock (m_mutex);
		std::wstring str;
		vmsBinaryFileVersionInfo bfvi;
		std::wstring path = m_updateConfig->componentCurrentPath();				
		if ( bfvi.Retrieve ( path.c_str() ) )
			str = bfvi.m_tstrFileVersion;

		return str;
	}

protected:
	std::shared_ptr <vmsUpdateFacilityConfig> m_updateConfig;
	std::unique_ptr <vmsFdmDllUpdater> m_updateMgr;
	std::shared_ptr <vmsYouTubeParserObjectsHolder> m_ytpObjsHolder;
	bool m_bPerformingUpdate;
	boost::mutex m_mutex;
	time_t m_timeLastUpdateAttempt;

	static std::shared_ptr<vmsYouTubeParserDllMgr> s_youTubeParserDllMgr;

protected:
	bool PerformUpdate2 ()
	{
		assert (m_bPerformingUpdate);

		bool updatePerformed;
		bool ret = m_updateMgr->PerformUpdate (updatePerformed);

		{
			boost::lock_guard <boost::mutex> lock (m_mutex);
			time (&m_timeLastUpdateAttempt);
			m_bPerformingUpdate = false;
		}

		return ret;
	}

	static unsigned __stdcall _threadPerformUpdate (vmsCreatesThreads *pthis2)
	{
		vmsYouTubeParserDllMgr *pthis = static_cast <vmsYouTubeParserDllMgr*> (pthis2);
		assert (pthis);
		pthis->PerformUpdate2 ();
		return 0;
	}
};
