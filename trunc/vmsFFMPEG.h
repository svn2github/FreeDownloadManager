/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include <boost/thread.hpp>

#include "vmsDLL.h"

class vmsFFMPEG : private boost::noncopyable
{
	vmsFFMPEG();
	virtual ~vmsFFMPEG();

public:
	static vmsFFMPEG* instance();
	virtual bool Init();
	bool Initialized() const;
	void LockDLL();
	void UnlockDLL();

	
	

	
	int __cdecl launchFFMPEG(BOOL* bStop, ...);

protected:
	vmsDLL m_dll;
	volatile bool m_isInitialized;
	boost::recursive_mutex m_mtxDLL;

	void* m_launchFFMPEG;
	

};

#define FFMPEG (vmsFFMPEG::instance())
