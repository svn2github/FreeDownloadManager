/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#include "libtorrent/invariant_check.hpp"
#include "libtorrent/bandwidth_manager.hpp"
#include "libtorrent/peer_connection.hpp"
#if defined TORRENT_LOGGING || defined TORRENT_VERBOSE_LOGGING
#include "libtorrent/aux_/session_impl.hpp"
#endif

namespace libtorrent
{
	namespace
	{
		const pt::time_duration window_size = pt::seconds(1);
	}

	history_entry::history_entry(intrusive_ptr<peer_connection> p
		, weak_ptr<torrent> t, int a, pt::ptime exp)
		: expires_at(exp), amount(a), peer(p), tor(t)
	{}
	
	bw_queue_entry::bw_queue_entry(intrusive_ptr<peer_connection> const& pe
		, bool no_prio)
		: peer(pe), non_prioritized(no_prio)
	{}

	bandwidth_manager::bandwidth_manager(io_service& ios, int channel)
		: m_ios(ios)
		, m_history_timer(m_ios)
		, m_limit(bandwidth_limit::inf)
		, m_current_quota(0)
		, m_channel(channel)
	{}

	void bandwidth_manager::request_bandwidth(intrusive_ptr<peer_connection> peer
		, bool non_prioritized)
	{
		INVARIANT_CHECK;

		
		
#ifndef NDEBUG
		for (std::deque<bw_queue_entry>::iterator i = m_queue.begin()
			, end(m_queue.end()); i != end; ++i)
		{
			assert(i->peer < peer || peer < i->peer);
		}
#endif

		assert(peer->max_assignable_bandwidth(m_channel) > 0);
		
		
		
		
		
		if (m_queue.empty() || non_prioritized)
		{
			m_queue.push_back(bw_queue_entry(peer, non_prioritized));
		}
		else
		{
			
			
			std::deque<bw_queue_entry>::reverse_iterator i = m_queue.rbegin();
			while (i != m_queue.rend() && i->non_prioritized) ++i;
			m_queue.insert(i.base(), bw_queue_entry(peer, non_prioritized));
		}
		if (m_queue.size() == 1) hand_out_bandwidth();
	} 

#ifndef NDEBUG
	void bandwidth_manager::check_invariant() const
	{
		int current_quota = 0;
		for (std::deque<history_entry>::const_iterator i
			= m_history.begin(), end(m_history.end()); i != end; ++i)
		{
			current_quota += i->amount;
		}

		assert(current_quota == m_current_quota);
	}
#endif

	void bandwidth_manager::add_history_entry(history_entry const& e) try
	{
		INVARIANT_CHECK;
#if defined TORRENT_LOGGING || defined TORRENT_VERBOSE_LOGGING

#endif

		m_history.push_front(e);
		m_current_quota += e.amount;
		
		
		if (m_history.size() > 1) return;

		m_history_timer.expires_at(e.expires_at);
		m_history_timer.async_wait(bind(&bandwidth_manager::on_history_expire, this, _1));
	}
	catch (std::exception&) { assert(false); }

	void bandwidth_manager::on_history_expire(asio::error_code const& e) try
	{
		INVARIANT_CHECK;

		if (e) return;

#if defined TORRENT_LOGGING || defined TORRENT_VERBOSE_LOGGING

#endif

		assert(!m_history.empty());

		pt::ptime now(pt::microsec_clock::universal_time());
		while (!m_history.empty() && m_history.back().expires_at <= now)
		{
			history_entry e = m_history.back();
			m_history.pop_back();
			m_current_quota -= e.amount;
			assert(m_current_quota >= 0);
			intrusive_ptr<peer_connection> c = e.peer;
			shared_ptr<torrent> t = e.tor.lock();
			if (!c->is_disconnecting()) c->expire_bandwidth(m_channel, e.amount);
			if (t) t->expire_bandwidth(m_channel, e.amount);
		}
		
		
		if (!m_history.empty())
		{
			m_history_timer.expires_at(m_history.back().expires_at);
			m_history_timer.async_wait(bind(&bandwidth_manager::on_history_expire, this, _1));
		}

		
		
		
		if (!m_queue.empty()) hand_out_bandwidth();
	}
	catch (std::exception&)
	{
		assert(false);
	};

	void bandwidth_manager::hand_out_bandwidth() try
	{
		INVARIANT_CHECK;
#if defined TORRENT_LOGGING || defined TORRENT_VERBOSE_LOGGING

#endif

		pt::ptime now(pt::microsec_clock::universal_time());

		mutex_t::scoped_lock l(m_mutex);
		int limit = m_limit;
		l.unlock();

		
		int amount = limit - m_current_quota;

		int bandwidth_block_size_limit = max_bandwidth_block_size;
		if (m_queue.size() > 3 && bandwidth_block_size_limit > limit / int(m_queue.size()))
			bandwidth_block_size_limit = std::max(max_bandwidth_block_size / int(m_queue.size() - 3)
				, min_bandwidth_block_size);

		while (!m_queue.empty() && amount > 0)
		{
			assert(amount == limit - m_current_quota);
			bw_queue_entry qe = m_queue.front();
			m_queue.pop_front();

			shared_ptr<torrent> t = qe.peer->associated_torrent().lock();
			if (!t) continue;
			if (qe.peer->is_disconnecting())
			{
				t->expire_bandwidth(m_channel, -1);
				continue;
			}

			
			
			
			
			int max_assignable = qe.peer->max_assignable_bandwidth(m_channel);
			if (max_assignable == 0)
			{
				t->expire_bandwidth(m_channel, -1);
				continue;
			}
			
			
			if (max_assignable > t->bandwidth_throttle(m_channel))
				max_assignable = t->bandwidth_throttle(m_channel);

			
			
			
			int single_amount = std::min(amount
				, std::min(bandwidth_block_size_limit
					, max_assignable));
			assert(single_amount > 0);
			amount -= single_amount;
			qe.peer->assign_bandwidth(m_channel, single_amount);
			t->assign_bandwidth(m_channel, single_amount);
			add_history_entry(history_entry(qe.peer, t, single_amount, now + window_size));
		}
	}
	catch (std::exception& e)
	{ assert(false); };

}
