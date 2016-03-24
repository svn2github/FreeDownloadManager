/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFlvSniffTask.h"
#include "vmsFlvSniffTaskResponse.h"
class vmsFlvSniffTaskProcessor
{
public:
	vmsFlvSniffTaskProcessor ();
	virtual ~vmsFlvSniffTaskProcessor ();
	virtual bool process_task (const vmsFlvSniffTask& task, vmsFlvSniffTaskResponse &result);

protected:
	std::map <std::wstring, UINT> m_requestResponseIdMap;
};

