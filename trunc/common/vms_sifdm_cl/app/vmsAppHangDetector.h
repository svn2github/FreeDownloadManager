/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsAppHangDetector :
	public vmsCreatesThreads2,
	public vmsThreadSafe3,
	protected vmsConsumesVaEvents
{
public:
	static const int hang_timeout = 60; 

public:
	vmsAppHangDetector (std::shared_ptr <vmsRunTimer> run_timer) :
		m_run_timer (run_timer)
	{
		assert (m_run_timer);
		create_thread (&vmsAppHangDetector::check_threads, this);
	}

	virtual ~vmsAppHangDetector () 
	{
		shutdown ();
	}

public:
	class thread_state :
		public vmsRaisesVaEvents
	{
	public:
		thread_state (
			std::shared_ptr <vmsRunTimer> run_timer) :
			m_run_timer (run_timer),
			m_dont_check_for_hangs (false)
		{
			assert (m_run_timer);
			update ();
		}

		~thread_state ()
		{
			RaiseEvent ((unsigned)0);
		}

		void update ()
		{
			m_timerLastValue = m_run_timer->seconds_elapsed ();
		}

		int timer_last_value () const
		{
			return m_timerLastValue;
		}

		void dont_check_for_hangs (bool b)
		{
			m_dont_check_for_hangs = b;
		}

		bool dont_check_for_hangs ()
		{
			return m_dont_check_for_hangs;
		}

	protected:
		std::shared_ptr <vmsRunTimer> m_run_timer;
		int m_timerLastValue;
		bool m_dont_check_for_hangs;
	};

public:
	std::shared_ptr <thread_state> this_thread_state ()
	{
		vmsThreadSafe3Scope;
		auto id = std::this_thread::get_id ();
		auto it = m_threads_states.find (id);
		if (it == m_threads_states.end ())
		{
			auto ts = std::make_shared <thread_state> (m_run_timer);
			m_threads_states [id] = ts;
			return ts;
			
		}
		return it->second.lock ();
	}

protected:
	std::map <std::thread::id, std::weak_ptr <thread_state>> m_threads_states;
	std::shared_ptr <vmsRunTimer> m_run_timer;

protected:
	virtual void onVaEvent (class vmsRaisesVaEvents *pObject, 
		unsigned nEvent, va_list args) override
	{
		assert (dynamic_cast <thread_state*> (pObject));
		auto id = std::this_thread::get_id ();
		{
			vmsThreadSafe3Scope;
			auto it = m_threads_states.find (id);
			assert (it != m_threads_states.end ());
			m_threads_states.erase (it);
		}
	}

	void check_threads ()
	{
		while (!wait_for_shutdown (1000))
		{
			auto cur_timer = m_run_timer->seconds_elapsed ();

			vmsThreadSafe3Scope;

			for (auto& item : m_threads_states)
			{
				auto ts = item.second.lock ();
				if (!ts)
					continue;
				if (ts->dont_check_for_hangs ())
					continue;
				auto last_timer = ts->timer_last_value ();
				if (cur_timer > last_timer + hang_timeout)
				{
					
					on_thread_hang (item.first);
				}
			}
		}
	}

	void on_thread_hang (std::thread::id thread_id)
	{
		char *p = nullptr;
		++p;
		*p = 0; 
	}
};