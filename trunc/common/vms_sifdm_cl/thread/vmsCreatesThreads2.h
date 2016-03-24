/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsCreatesThreads2
{
public:
	vmsCreatesThreads2 () :
		m_shutting_down (false) {}

	virtual ~vmsCreatesThreads2 ()
	{
		
		assert (m_threads.empty ());
	}

	virtual void shutdown (bool bWaitThreadsForShutdown = true) 
	{
		m_shutting_down = true;
		m_shutting_down_cv.notify_all ();
		if (bWaitThreadsForShutdown)
			wait_threads_for_shutdown ();
	}

	void wait_threads_for_shutdown ()
	{
		for (;;)
		{
			std::shared_ptr <std::thread> thread;

			m_threads_mx.lock ();

			if (!m_threads.empty ())
			{
				thread = m_threads.front ();
				assert (thread);
			}

			m_threads_mx.unlock ();

			if (!thread)
				return;

			assert (thread->joinable ());
			thread->join ();

			std::lock_guard <std::mutex> lock (m_threads_mx);
			if (!m_threads.empty () && thread == m_threads.front ())
				m_threads.erase (m_threads.begin ());
		}
	}

protected:
	
 	void on_thread_created (std::shared_ptr <std::thread> thread)
	{
		assert (thread && thread->joinable ());
		if (!thread || !thread->joinable ())
			return;
		std::lock_guard <std::mutex> lock (m_threads_mx);
		m_threads.push_back (thread);
	}

	
	void on_thread_terminating ()
	{
		auto this_id = std::this_thread::get_id ();
		std::lock_guard <std::mutex> lock (m_threads_mx);
		for (auto it = m_threads.begin (); it != m_threads.end (); ++it)
		{
			if ((*it)->get_id () == this_id)
			{
				(*it)->detach ();
				m_threads.erase (it);
				return;
			}
		}
	}

	template <class _Fn, class... _Args>
	std::shared_ptr <std::thread> create_thread (_Fn&& fun, _Args&&... args)
	{
		auto thread = std::make_shared <std::thread> (fun, args...);
		on_thread_created (thread);
		return thread;
	}

	
	bool wait_for_shutdown (unsigned uMilliseconds)
	{
		std::unique_lock <std::mutex> lock (m_threads_mx);
		return m_shutting_down_cv.wait_for (
			lock, std::chrono::milliseconds (uMilliseconds), 
			[&]()->bool {return m_shutting_down;});
	}

protected:
	std::vector <std::shared_ptr <std::thread>> m_threads;
	std::mutex m_threads_mx;
	bool m_shutting_down;
	std::condition_variable m_shutting_down_cv;
};