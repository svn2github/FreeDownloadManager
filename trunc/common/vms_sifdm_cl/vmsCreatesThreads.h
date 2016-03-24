/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <map>
#include "vmsThread.h"

class vmsCreatesThreads
{
public:
	vmsCreatesThreads()
	{
		m_hevShuttingDown = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	virtual ~vmsCreatesThreads()
	{
		
		assert(m_vThreads.empty());
		assert(m_vThreadsObsolete.empty());
		CloseHandle(m_hevShuttingDown);
	}

	virtual void Shutdown(bool bWaitThreadsForShutdown = false)
	{
		SetEvent(m_hevShuttingDown);
		if (bWaitThreadsForShutdown)
		{
			WaitThreadsForShutdown();
			WaitThreadsForShutdownObsolete();
		}
	}

	void WaitThreadsForShutdown()
	{
		for (;;)
		{
			DWORD threadId = 0;
			vmsThread *thread = nullptr;
			vmsWinHandle::tSP spHandle;

			vmsAUTOLOCKSECTION(m_csThreads);
			if (m_vThreads.empty())
				return;

			auto itBegin = m_vThreads.begin();
			threadId = itBegin->first;
			thread = itBegin->second;
			assert(thread != nullptr);
			if (thread == nullptr)
			{
				m_vThreads.erase(itBegin);
				continue;
			}

			spHandle = thread->handle();
			assert(spHandle.get());
			if (!spHandle.get())
			{
				m_vThreads.erase(itBegin);
				delete thread;
				continue;
			}
			vmsAUTOLOCKSECTION_UNLOCK(m_csThreads);

			WaitForSingleObject(spHandle->getHandle (), INFINITE);
			
			{
				
				vmsAUTOLOCKSECTION(m_csThreads);
				if (!m_vThreads.empty () && m_vThreads.begin ()->first == threadId)
				{
					delete thread;
					m_vThreads.erase (m_vThreads.begin ());
				}
			}
		}
	}

	void WaitThreadsForShutdownObsolete()
	{
		for (;;)
		{
			HANDLE hThread = NULL;
			EnterCriticalSection(m_csThreads);
			if (!m_vThreadsObsolete.empty())
			{
				hThread = m_vThreadsObsolete[0]->m_handle;
				assert(hThread != NULL);
			}
			LeaveCriticalSection(m_csThreads);
			if (!hThread)
				return;
			WaitForSingleObject(hThread, INFINITE);
			EnterCriticalSection(m_csThreads);
			if (!m_vThreadsObsolete.empty() && hThread == m_vThreadsObsolete[0]->m_handle)
				m_vThreadsObsolete.erase(m_vThreadsObsolete.begin());
			LeaveCriticalSection(m_csThreads);
		}
	}

	size_t get_thread_count () const
	{
		vmsAUTOLOCKSECTION (m_csThreads);
		return m_vThreads.size () + m_vThreadsObsolete.size ();
	}

public:
	void onThreadCreated(vmsThread *thread)
	{
		DWORD threadId = GetCurrentThreadId();

		vmsAUTOLOCKSECTION(m_csThreads);
		auto res = m_vThreads.insert(std::make_pair(threadId, thread));
		assert(res.second);
	}
	void onThreadTerminating()
	{
		DWORD threadId = GetCurrentThreadId();
		vmsAUTOLOCKSECTION(m_csThreads);
		auto it = m_vThreads.find(threadId);
		assert(it != m_vThreads.end());
		if (it != m_vThreads.end())
		{
			vmsThread *thread = it->second;
			if (m_vThreads.erase(threadId))
			{
				delete thread;
				thread = nullptr;
			}
		}
	}

protected:
	
	void onThreadCreated(vmsWinHandle::tSP sphThread) 
	{
		assert(sphThread != NULL && sphThread->m_handle != NULL);
		if (!sphThread || !sphThread->m_handle)
			return;
		vmsAUTOLOCKSECTION(m_csThreads);
		m_vThreadsObsolete.push_back(sphThread);
	}

protected:
	
	vmsWinHandle::tSP CreateThread(unsigned(__stdcall *start_address)(vmsCreatesThreads*), char *threadName = nullptr)
	{
		vmsThread *thread = new vmsThread(this, (unsigned(__stdcall *)(void*))start_address, this, threadName);
		return thread->handle();
	}
	vmsWinHandle::tSP CreateThread(unsigned(__stdcall *start_address)(void*), void* pvParam, char *threadName = nullptr)
	{
		vmsThread *thread = new vmsThread(this, start_address, pvParam, threadName);
		return thread->handle();
	}

	
	bool WaitForShutdownEvent(unsigned uMilliseconds)
	{
		return WaitForSingleObject(m_hevShuttingDown, uMilliseconds) == WAIT_OBJECT_0;
	}

protected:
	std::map <DWORD, vmsThread*> m_vThreads;
	std::vector <vmsWinHandle::tSP> m_vThreadsObsolete;
	mutable vmsCriticalSection m_csThreads;
	HANDLE m_hevShuttingDown;
};

static unsigned _stdcall ThreadEntryPoint::threadMain(void *pv)
{
	std::unique_ptr<ThreadEntryPoint::ThreadParams> params(reinterpret_cast <ThreadEntryPoint::ThreadParams*>(pv));

	params->threadOwner->onThreadCreated(params->thread);
	vmsThread::setThreadName(-1, params->name);
	(*params->routine)(params->routineParams);
	params->threadOwner->onThreadTerminating();

	return 0;
}