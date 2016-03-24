/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsYouTubeParserDllMgr.h"

std::shared_ptr<vmsYouTubeParserDllMgr> vmsYouTubeParserDllMgr::s_youTubeParserDllMgr;

vmsYouTubeParserDllMgr::vmsYouTubeParserDllMgr(std::shared_ptr <vmsAppDataFolder> appDataFolder,
		bool currentBuildFirstRun) :
	m_bPerformingUpdate (false),
	m_timeLastUpdateAttempt (0)
{
	m_updateConfig = std::make_shared <vmsFdmSingleFileUpdaterFacilityConfig> (
		appDataFolder, L"ytparser.dll");

	std::wstring file = m_updateConfig->componentCurrentPath ();
	if (file != m_updateConfig->componentOriginalPath ())
	{
		if (!vmsFdmVerifySign (file))
		{
			DeleteFile (file.c_str ());
			file = m_updateConfig->componentOriginalPath ();
		}
	}

	m_ytpObjsHolder = std::make_shared <vmsYouTubeParserObjectsHolder> (file);

	m_updateMgr.reset (new vmsFdmDllUpdater (m_updateConfig, m_ytpObjsHolder, 
		L"http://up.freedownloadmanager.org/update/ytparser.xml", currentBuildFirstRun));
}

vmsYouTubeParserDllMgr::~vmsYouTubeParserDllMgr(void)
{
	Shutdown (true);
}
