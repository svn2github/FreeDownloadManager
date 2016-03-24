/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsThread;
class vmsCreatesThreads;

namespace ThreadEntryPoint
{
	struct ThreadParams
	{
		vmsThread *thread;
		vmsCreatesThreads *threadOwner;
		unsigned(__stdcall *routine)(void*);
		void *routineParams;
		char *name;
	};

	static unsigned _stdcall threadMain(void *pv);
}

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;		
	LPCSTR szName;		
	DWORD dwThreadID;	
	DWORD dwFlags;		
} THREADNAME_INFO;
#pragma pack(pop)

class vmsThread
{
public:
	vmsThread(vmsCreatesThreads *threadOwner, 
			  unsigned(__stdcall *start_address)(void*),
			  void *pvParam,
			  char* threadName = nullptr)
	{
		auto *params = new ThreadEntryPoint::ThreadParams;
		params->thread = this;
		params->threadOwner = threadOwner;
		params->routine = start_address;
		params->routineParams = pvParam;
		params->name = threadName;
		m_spHandle = std::make_shared<vmsWinHandle>((HANDLE)_beginthreadex(NULL, 0, ThreadEntryPoint::threadMain, params, 0, NULL));
	}

	virtual ~vmsThread() {}

	vmsWinHandle::tSP handle() const
	{
		return m_spHandle;
	}

	static void setThreadName(DWORD dwThreadID, char* threadName)
	{
		if (!threadName)
			return;

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

private:
	vmsWinHandle::tSP m_spHandle;
};