/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

  

 

#include <iostream>

#include "libtorrent/peer_connection.hpp"

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "libtorrent/web_peer_connection.hpp"
#include "libtorrent/policy.hpp"
#include "libtorrent/torrent.hpp"
#include "libtorrent/socket.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/invariant_check.hpp"
#include "libtorrent/aux_/session_impl.hpp"

namespace libtorrent
{
	class peer_connection;
}

using namespace boost::posix_time;
using boost::bind;

namespace
{
	using namespace libtorrent;

	size_type collect_free_download(
		torrent::peer_iterator start
		, torrent::peer_iterator end)
	{
		size_type accumulator = 0;
		for (torrent::peer_iterator i = start; i != end; ++i)
		{
			
			
			
			
			size_type diff = i->second->share_diff();
			assert(diff < std::numeric_limits<size_type>::max());
			if (i->second->is_peer_interested() || diff <= 0)
				continue;

			assert(diff > 0);
			i->second->add_free_upload(-diff);
			accumulator += diff;
			assert(accumulator > 0);
		}
		assert(accumulator >= 0);
		return accumulator;
	} 

	
	
	size_type distribute_free_upload(
		torrent::peer_iterator start
		, torrent::peer_iterator end
		, size_type free_upload)
	{
		if (free_upload <= 0) return free_upload;
		int num_peers = 0;
		size_type total_diff = 0;
		for (torrent::peer_iterator i = start; i != end; ++i)
		{
			size_type d = i->second->share_diff();
			assert(d < std::numeric_limits<size_type>::max());
			total_diff += d;
			if (!i->second->is_peer_interested() || i->second->share_diff() >= 0) continue;
			++num_peers;
		}

		if (num_peers == 0) return free_upload;
		size_type upload_share;
		if (total_diff >= 0)
		{
			upload_share = std::min(free_upload, total_diff) / num_peers;
		}
		else
		{
			upload_share = (free_upload + total_diff) / num_peers;
		}
		if (upload_share < 0) return free_upload;

		for (torrent::peer_iterator i = start; i != end; ++i)
		{
			peer_connection* p = i->second;
			if (!p->is_peer_interested() || p->share_diff() >= 0) continue;
			p->add_free_upload(upload_share);
			free_upload -= upload_share;
		}
		return free_upload;
	}

	struct match_peer_ip
	{
		match_peer_ip(tcp::endpoint const& ip)
			: m_ip(ip)
		{}

		bool operator()(policy::peer const& p) const
		{ return p.ip.address() == m_ip.address(); }

		tcp::endpoint m_ip;
	};

	struct match_peer_connection
	{
		match_peer_connection(peer_connection const& c)
			: m_conn(c)
		{}

		bool operator()(policy::peer const& p) const
		{ return p.connection == &m_conn; }

		const peer_connection& m_conn;
	}; 

}

namespace libtorrent
{
	
	
	
	
	void request_a_block(
		torrent& t
		, peer_connection& c
		, std::vector<peer_connection*> ignore)
	{
		assert(!t.is_seed());
		assert(!c.has_peer_choked());
		int num_requests = c.desired_queue_size()
			- (int)c.download_queue().size()
			- (int)c.request_queue().size();

		assert(c.desired_queue_size() > 0);
		
		
		if (num_requests <= 0) return;

		piece_picker& p = t.picker();
		std::vector<piece_block> interesting_pieces;
		interesting_pieces.reserve(100);

		bool prefer_whole_pieces = c.prefer_whole_pieces();
		if (!prefer_whole_pieces)
		{
			prefer_whole_pieces = c.statistics().download_payload_rate()
				* t.settings().whole_pieces_threshold
				> t.torrent_file().piece_length();
		}
	
		
		
		
		
		assert((c.proxy() == tcp::endpoint() && c.remote() == c.get_socket()->remote_endpoint())
			|| c.proxy() == c.get_socket()->remote_endpoint());

		
		
		
		
		
		
		
		
		p.pick_pieces(c.get_bitfield(), interesting_pieces
			, num_requests, prefer_whole_pieces, c.remote());

		
		
		
		
		
		std::vector<piece_block> busy_pieces;
		busy_pieces.reserve(10);

		for (std::vector<piece_block>::iterator i = interesting_pieces.begin();
			i != interesting_pieces.end(); ++i)
		{
			if (p.is_downloading(*i))
			{
				busy_pieces.push_back(*i);
				continue;
			}

			
			
			
			c.add_request(*i);
			num_requests--;
		}

		c.send_block_requests();

		
		
		
		

		if (busy_pieces.empty()) return;

		
		
		
		

		while (num_requests > 0)
		{
			peer_connection* peer = 0;

			const int initial_queue_size = (int)c.download_queue().size()
				+ (int)c.request_queue().size();

			
			
			float min_weight = initial_queue_size == 0
				? std::numeric_limits<float>::max()
				: c.statistics().download_payload_rate() / initial_queue_size;

			
			
			
			for (torrent::peer_iterator i = t.begin();
				i != t.end(); ++i)
			{
				
				if (i->second == &c)
					continue;

				
				if (std::find(ignore.begin(), ignore.end(), i->second) != ignore.end())
					continue;

				const std::deque<piece_block>& download_queue = i->second->download_queue();
				const std::deque<piece_block>& request_queue = i->second->request_queue();
				const int queue_size = (int)i->second->download_queue().size()
					+ (int)i->second->request_queue().size();

				bool in_request_queue = std::find_first_of(
						busy_pieces.begin()
						, busy_pieces.end()
						, request_queue.begin()
						, request_queue.end()) != busy_pieces.end();
						
				bool in_download_queue = std::find_first_of(
						busy_pieces.begin()
						, busy_pieces.end()
						, download_queue.begin()
						, download_queue.end()) != busy_pieces.end();

				
				
				
				
				
				
				const float weight = (queue_size == 0)
					? std::numeric_limits<float>::max()
					: i->second->statistics().download_payload_rate() / queue_size
						* in_request_queue ? .1f : 1.f;

				
				
				
				
				if (weight < min_weight && (in_request_queue || in_download_queue))
				{
					peer = i->second;
					min_weight = weight;
				}
			}

			if (peer == 0)
			{
				
				
				break;
			}

			

			std::deque<piece_block>::const_reverse_iterator common_block =
				std::find_first_of(
					peer->request_queue().rbegin()
					, peer->request_queue().rend()
					, busy_pieces.begin()
					, busy_pieces.end());

			if (common_block == peer->request_queue().rend())
			{
				common_block = std::find_first_of(
					peer->download_queue().rbegin()
					, peer->download_queue().rend()
					, busy_pieces.begin()
					, busy_pieces.end());
				assert(common_block != peer->download_queue().rend());
			}

			piece_block block = *common_block;

			
			
			
			
			peer->cancel_request(block);
			c.add_request(block);
			ignore.push_back(&c);
			if (!peer->has_peer_choked() && !t.is_seed())
			{
				request_a_block(t, *peer, ignore);
				peer->send_block_requests();
			}

			num_requests--;

			const int queue_size = (int)c.download_queue().size()
				+ (int)c.request_queue().size();
			const float weight = queue_size == 0
				? std::numeric_limits<float>::max()
				: c.statistics().download_payload_rate() / queue_size;

			
			
			if (weight <= min_weight) break;
		}
		c.send_block_requests();
	}

	policy::policy(torrent* t)
		: m_torrent(t)
		, m_num_unchoked(0)
		, m_available_free_upload(0)
		, m_last_optimistic_disconnect(boost::gregorian::date(1970,boost::gregorian::Jan,1))
	{ assert(t); }
	
	
	
	policy::peer* policy::find_choke_candidate()
	{
		INVARIANT_CHECK;

		peer* worst_peer = 0;
		size_type min_weight = std::numeric_limits<int>::min();

#ifndef NDEBUG
		int unchoked_counter = m_num_unchoked;
#endif
		
		

		for (std::vector<peer>::iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			peer_connection* c = i->connection;

			if (c == 0) continue;
			if (c->is_choked()) continue;
#ifndef NDEBUG
			unchoked_counter--;
#endif
			if (c->is_disconnecting()) continue;
			
			if (!c->is_peer_interested())
				return &(*i);

			size_type diff = i->total_download()
				- i->total_upload();

			size_type weight = static_cast<int>(c->statistics().download_rate() * 10.f)
				+ diff
				+ ((c->is_interesting() && c->has_peer_choked())?-10:10)*1024;

			if (weight >= min_weight && worst_peer) continue;

			min_weight = weight;
			worst_peer = &(*i);
			continue;
		}
		assert(unchoked_counter == 0);
		return worst_peer;
	}

	policy::peer* policy::find_unchoke_candidate()
	{
		INVARIANT_CHECK;

		
		
		if (m_num_unchoked == m_torrent->num_peers())
			return 0;

		using namespace boost::posix_time;
		using namespace boost::gregorian;

		peer* unchoke_peer = 0;
		ptime min_time(date(9999,Jan,1));
		float max_down_speed = 0.f;

		

		for (std::vector<peer>::iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			peer_connection* c = i->connection;
			if (c == 0) continue;
			if (c->is_disconnecting()) continue;
			if (!c->is_choked()) continue;
			if (!c->is_peer_interested()) continue;
			if (c->share_diff() < -free_upload_amount
				&& m_torrent->ratio() != 0) continue;
			if (c->statistics().download_rate() < max_down_speed) continue;

			min_time = i->last_optimistically_unchoked;
			max_down_speed = c->statistics().download_rate();
			unchoke_peer = &(*i);
		}
		return unchoke_peer;
	}

	policy::peer* policy::find_disconnect_candidate()
	{
		peer *disconnect_peer = 0;
		double slowest_transfer_rate = std::numeric_limits<double>::max();

		boost::posix_time::ptime local_time
			= second_clock::universal_time();

		for (std::vector<peer>::iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			peer_connection* c = i->connection;
			if(c == 0)
				continue;
			if(c->is_disconnecting())
				continue;

			double transferred_amount
				= (double)c->statistics().total_payload_download();

			boost::posix_time::time_duration connected_time
				= local_time - i->connected;

			double connected_time_in_seconds
				= connected_time.seconds()
				+ connected_time.minutes()*60.0
				+ connected_time.hours()*60.0*60.0;

			double transfer_rate
				= transferred_amount / (connected_time_in_seconds+1);

			if (transfer_rate <= slowest_transfer_rate)
			{
				slowest_transfer_rate = transfer_rate;
				disconnect_peer = &(*i);
			}
		}
		return disconnect_peer;
	}

	policy::peer *policy::find_connect_candidate()
	{
		boost::posix_time::ptime local_time=second_clock::universal_time();
		boost::posix_time::ptime ptime(local_time);
		policy::peer* candidate  =0;

		for (std::vector<peer>::iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			if(i->connection) continue;
			if(i->banned) continue;
			if(i->type == peer::not_connectable) continue;

			assert(i->connected <= local_time);

			boost::posix_time::ptime next_connect = i->connected;

			if (next_connect <= ptime)
			{
				ptime = next_connect;
				candidate = &(*i);
			}
		}
		
		assert(ptime <= local_time);

		return candidate;
	}

	policy::peer* policy::find_seed_choke_candidate()
	{
		INVARIANT_CHECK;

		assert(m_num_unchoked > 0);
		
		
		
		using namespace boost::posix_time;
		using namespace boost::gregorian;

		peer* candidate = 0;

		
		ptime last_unchoke = ptime(date(1970, Jan, 1));

		
		
		
		peer* second_candidate = 0;
		size_type lowest_share_diff = 0; 

		for (std::vector<peer>::iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			peer_connection* c = i->connection;
            
            
			if (c == 0) continue;

			if (c->is_choked()) continue;
			if (c->is_disconnecting()) continue;

			size_type share_diff = c->share_diff();

			
			
			if (!second_candidate || share_diff <= lowest_share_diff)
			{
				lowest_share_diff = share_diff;
				second_candidate = &(*i);
			}
			
			
			
			if (share_diff > 0) continue;
			if (!candidate || last_unchoke > i->last_optimistically_unchoked)
			{
				last_unchoke = i->last_optimistically_unchoked;
				candidate = &(*i);
			}
		}
		if (candidate) return candidate;
		if (second_candidate) return second_candidate;
		assert(false);
		return 0;
	}

	policy::peer* policy::find_seed_unchoke_candidate()
	{
		INVARIANT_CHECK;

		peer* candidate = 0;
		boost::posix_time::ptime last_unchoke
			= second_clock::universal_time();

		for (std::vector<peer>::iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			peer_connection* c = i->connection;
			if (c == 0) continue;
			if (!c->is_choked()) continue;
			if (!c->is_peer_interested()) continue;
			if (c->is_disconnecting()) continue;
			if (last_unchoke < i->last_optimistically_unchoked) continue;
			last_unchoke = i->last_optimistically_unchoked;
			candidate = &(*i);
		}
		return candidate;
	}

	bool policy::seed_unchoke_one_peer()
	{
		INVARIANT_CHECK;

		peer* p = find_seed_unchoke_candidate();
		if (p != 0)
		{
			assert(p->connection->is_choked());
			p->connection->send_unchoke();
			p->last_optimistically_unchoked
				= second_clock::universal_time();
			++m_num_unchoked;
		}
		return p != 0;
	}

	void policy::seed_choke_one_peer()
	{
		INVARIANT_CHECK;

		peer* p = find_seed_choke_candidate();
		if (p != 0)
		{
			assert(!p->connection->is_choked());
			p->connection->send_choke();
			--m_num_unchoked;
		}
	}

	void policy::pulse()
	{
		INVARIANT_CHECK;

		if (m_torrent->is_paused()) return;

		using namespace boost::posix_time;

		
		m_peers.erase(
			std::remove_if(m_peers.begin()
				, m_peers.end()
				, old_disconnected_peer())
			, m_peers.end());

		
		
		

		
		
		int num_connected_peers = 0;

		for (std::vector<peer>::iterator i = m_peers.begin();
					i != m_peers.end(); ++i)
		{
			if (i->connection && !i->connection->is_disconnecting())
				++num_connected_peers;
		}

		if (m_torrent->m_connections_quota.given != std::numeric_limits<int>::max())
		{

			int max_connections = m_torrent->m_connections_quota.given;

			if (num_connected_peers >= max_connections)
			{
				

				boost::posix_time::ptime local_time = second_clock::universal_time();
				if (m_last_optimistic_disconnect + boost::posix_time::seconds(120) <= local_time)
				{
					m_last_optimistic_disconnect = local_time;
					--max_connections; 
				}
			}
			else
			{
				
				m_last_optimistic_disconnect = second_clock::universal_time();
			}

			while (num_connected_peers > max_connections)
			{
				bool ret = disconnect_one_peer();
				(void)ret;
				assert(ret);
				--num_connected_peers;
			}
		}

		while (m_torrent->num_peers() < m_torrent->m_connections_quota.given)
		{
			if (!connect_one_peer())
				break;
		} 

		
		
		

		
		
		
		
		
		
		

		
		
		
		if (m_torrent->ratio() != 0.f)
		{
			
			
			m_available_free_upload
				+= collect_free_download(
					m_torrent->begin()
					, m_torrent->end());

			
			m_available_free_upload = distribute_free_upload(
				m_torrent->begin()
				, m_torrent->end()
				, m_available_free_upload);
		}

		
		
		
		if (m_torrent->is_seed())
		{
			if (m_num_unchoked > m_torrent->m_uploads_quota.given)
			{
				do
				{
					peer* p = find_seed_choke_candidate();
					--m_num_unchoked;
					assert(p != 0);
					if (p == 0) break;

					assert(!p->connection->is_choked());
					p->connection->send_choke();
				} while (m_num_unchoked > m_torrent->m_uploads_quota.given);
			}
			else if (m_num_unchoked > 0)
			{
				
				
				
				
				assert(m_num_unchoked <= m_torrent->num_peers());
				peer* p = find_seed_unchoke_candidate();
				if (p)
				{
					assert(p->connection->is_choked());
					seed_choke_one_peer();
					p->connection->send_unchoke();
					++m_num_unchoked;
				}
			
			}

			
			
			while (m_num_unchoked < m_torrent->m_uploads_quota.given)
			{
				if (!seed_unchoke_one_peer()) break;
			}
#ifndef NDEBUG
			check_invariant();
#endif
		}

		
		
		
		else
		{
			if (m_torrent->ratio() != 0)
			{
				
				for (std::vector<peer>::iterator i = m_peers.begin();
					i != m_peers.end(); ++i)
				{
					peer_connection* c = i->connection;
					if (c == 0) continue;

					size_type diff = i->connection->share_diff();
					if (diff < -free_upload_amount
						&& !c->is_choked())
					{
						
						
						c->send_choke();
						--m_num_unchoked;
					}
				}
			}
			
			if (m_torrent->m_uploads_quota.given < m_torrent->num_peers())
			{
				assert(m_torrent->m_uploads_quota.given >= 0);

				
				
				if (m_num_unchoked > m_torrent->m_uploads_quota.given)
				{
					do
					{
						peer* p = find_choke_candidate();
						if (!p) break;
						assert(p);
						assert(!p->connection->is_choked());
						p->connection->send_choke();
						--m_num_unchoked;
					} while (m_num_unchoked > m_torrent->m_uploads_quota.given);
				}
				else
				{
					
					
					
					
					assert(m_num_unchoked <= m_torrent->num_peers());
					peer* p = find_unchoke_candidate();
					if (p)
					{
						assert(p->connection->is_choked());
						choke_one_peer();
						p->connection->send_unchoke();
						++m_num_unchoked;
					}
				}
			}

			
			
			while (m_num_unchoked < m_torrent->m_uploads_quota.given
				&& unchoke_one_peer());
		}
	}

	void policy::ban_peer(const peer_connection& c)
	{
		INVARIANT_CHECK;

		std::vector<peer>::iterator i = std::find_if(
			m_peers.begin()
			, m_peers.end()
			, match_peer_connection(c));

		if (i == m_peers.end())
		{
			
			if (web_peer_connection const* p = dynamic_cast<web_peer_connection const*>(&c))
			{
				m_torrent->remove_url_seed(p->url());
			}
			return;
		}

		i->type = peer::not_connectable;
		i->ip.port(0);
		i->banned = true;
	}

	void policy::new_connection(peer_connection& c)
	{
		assert(!c.is_local());

		INVARIANT_CHECK;

		
		
		

		
		
		assert((c.proxy() == tcp::endpoint() && c.remote() == c.get_socket()->remote_endpoint())
			|| c.proxy() == c.get_socket()->remote_endpoint());

		if (m_torrent->num_peers() >= m_torrent->m_connections_quota.given
			&& c.remote().address() != m_torrent->current_tracker().address())
		{
			throw protocol_error("too many connections, refusing incoming connection"); 
		}

#if defined(TORRENT_VERBOSE_LOGGING) || defined(TORRENT_LOGGING)
		if (c.remote().address() == m_torrent->current_tracker().address())
		{
			m_torrent->debug_log("overriding connection limit for tracker NAT-check");
		}
#endif

		std::vector<peer>::iterator i;

		if (m_torrent->settings().allow_multiple_connections_per_ip)
		{
			i = m_peers.end();
		}
		else
		{
			i = std::find_if(
				m_peers.begin()
				, m_peers.end()
				, match_peer_ip(c.remote()));
		}

		if (i != m_peers.end())
		{
			if (i->banned)
				throw protocol_error("ip address banned, closing");

			if (i->connection != 0)
			{
				assert(i->connection != &c);
				
				
				if (!i->connection->is_connecting() || c.is_local())
				{
					throw protocol_error("duplicate connection, closing");
				}
				else
				{
#if defined(TORRENT_VERBOSE_LOGGING) || defined(TORRENT_LOGGING)
					m_torrent->debug_log("duplicate connection. existing connection"
					" is connecting and this connection is incoming. closing existing "
					"connection in favour of this one");
#endif
					i->connection->disconnect();
					i->connection = 0;
				}
			}
		}
		else
		{
			using namespace boost::posix_time;
			using namespace boost::gregorian;

			
			
			assert((c.proxy() == tcp::endpoint() && c.remote() == c.get_socket()->remote_endpoint())
				|| c.proxy() == c.get_socket()->remote_endpoint());

			peer p(c.remote(), peer::not_connectable);
			m_peers.push_back(p);
			i = m_peers.end()-1;
		}
		
		assert(i->connection == 0);
		c.add_stat(i->prev_amount_download, i->prev_amount_upload);
		i->prev_amount_download = 0;
		i->prev_amount_upload = 0;
		i->connection = &c;
		assert(i->connection);
		i->connected = second_clock::universal_time();
		m_last_optimistic_disconnect = second_clock::universal_time();
	}

	void policy::peer_from_tracker(const tcp::endpoint& remote, const peer_id& pid)
	{
		INVARIANT_CHECK;

		
		if(remote.address() == address() || remote.port() == 0)
			return;

		try
		{
			std::vector<peer>::iterator i;
			
			if (m_torrent->settings().allow_multiple_connections_per_ip)
			{
				i = m_peers.end();
			}
			else
			{
				i = std::find_if(
					m_peers.begin()
					, m_peers.end()
					, match_peer_ip(remote));
			}
			
			bool just_added = false;
			
			if (i == m_peers.end())
			{
				using namespace boost::posix_time;
				using namespace boost::gregorian;

				
				
				peer p(remote, peer::connectable);
				m_peers.push_back(p);
				
				
				i = m_peers.end() - 1;
				just_added = true;
			}
			else
			{
				i->type = peer::connectable;

				
				
				
				i->ip = remote;

				if (i->connection)
				{
					
					
					

#if defined(TORRENT_VERBOSE_LOGGING) || defined(TORRENT_LOGGING)
					m_torrent->debug_log("already connected to peer: " + remote.address().to_string() + ":"
						+ boost::lexical_cast<std::string>(remote.port()) + " "
						+ boost::lexical_cast<std::string>(i->connection->pid()));
#endif

					assert(i->connection->associated_torrent().lock().get() == m_torrent);
					return;
				}
			}

			if (i->banned) return;

			if (m_torrent->num_peers() < m_torrent->m_connections_quota.given
				&& !m_torrent->is_paused())
			{
				if (!connect_peer(&*i) && just_added)
				{
					
					
					
					assert(i == m_peers.end() - 1);
					m_peers.erase(i);
				}
			}
			return;
		}
		catch(std::exception& e)
		{
			if (m_torrent->alerts().should_post(alert::debug))
			{
				m_torrent->alerts().post_alert(
					peer_error_alert(remote, pid, e.what()));
			}
		}
	}

	
	
	
	void policy::choked(peer_connection&)
	{
	}

	void policy::piece_finished(int index, bool successfully_verified)
	{
		INVARIANT_CHECK;

		assert(index >= 0 && index < m_torrent->torrent_file().num_pieces());

		if (successfully_verified)
		{
			
			for (std::vector<peer>::iterator i = m_peers.begin();
				i != m_peers.end(); ++i)
			{
				if (i->connection == 0) continue;
				
				if (!i->connection->is_interesting()) continue;
				if (!i->connection->has_piece(index)) continue;

				bool interested = false;
				const std::vector<bool>& peer_has = i->connection->get_bitfield();
				const std::vector<bool>& we_have = m_torrent->pieces();
				assert(we_have.size() == peer_has.size());
				for (int j = 0; j != (int)we_have.size(); ++j)
				{
					if (!we_have[j] && peer_has[j])
					{
						interested = true;
						break;
					}
				}
				if (!interested)
					i->connection->send_not_interested();
				assert(i->connection->is_interesting() == interested);
			}
		}
	}

	
	
	
	void policy::block_finished(peer_connection& c, piece_block)
	{
		INVARIANT_CHECK;

		
		if (!c.has_peer_choked() && !m_torrent->is_seed())
			request_a_block(*m_torrent, c);
	}

	
	
	
	void policy::unchoked(peer_connection& c)
	{
		INVARIANT_CHECK;
		if (c.is_interesting())
		{
			request_a_block(*m_torrent, c);
		}
	}

	
	void policy::interested(peer_connection& c)
	{
		INVARIANT_CHECK;

		assert(std::find_if(m_peers.begin(), m_peers.end()
			, boost::bind<bool>(std::equal_to<peer_connection*>(), bind(&peer::connection, _1)
			, &c)) != m_peers.end());
		
		
		
		
		
		
		
		
		
		
		
		if (c.is_choked()
			&& m_num_unchoked < m_torrent->m_uploads_quota.given
			&& (m_torrent->ratio() == 0
				|| c.share_diff() >= -free_upload_amount
				|| m_torrent->is_seed()))
		{
			c.send_unchoke();
			++m_num_unchoked;
		}
	}

	
	void policy::not_interested(peer_connection& c)
	{
		INVARIANT_CHECK;

		if (m_torrent->ratio() != 0.f)
		{
			assert(c.share_diff() < std::numeric_limits<size_type>::max());
			size_type diff = c.share_diff();
			if (diff > 0 && c.is_seed())
			{
				
				
				
				m_available_free_upload += diff;
				c.add_free_upload(-diff);
			}
		}
		if (!c.is_choked())
		{
			c.send_choke();
			--m_num_unchoked;

			if (m_torrent->is_seed()) seed_unchoke_one_peer();
			else unchoke_one_peer();
		}
	}

	bool policy::unchoke_one_peer()
	{
		peer* p = find_unchoke_candidate();
		if (p == 0) return false;
		assert(p->connection);
		assert(!p->connection->is_disconnecting());

		assert(p->connection->is_choked());
		p->connection->send_unchoke();
		p->last_optimistically_unchoked = second_clock::universal_time();
		++m_num_unchoked;
		return true;
	}

	void policy::choke_one_peer()
	{
		peer* p = find_choke_candidate();
		if (p == 0) return;
		assert(p->connection);
		assert(!p->connection->is_disconnecting());
		assert(!p->connection->is_choked());
		p->connection->send_choke();
		--m_num_unchoked;
	}

	bool policy::connect_one_peer()
	{
		if(m_torrent->num_peers() >= m_torrent->m_connections_quota.given)
			return false;
		peer* p = find_connect_candidate();
		if (p == 0) return false;
		assert(!p->banned);
		assert(!p->connection);
		assert(p->type == peer::connectable);

		return connect_peer(p);
	}

	bool policy::connect_peer(peer *p)
	{
		INVARIANT_CHECK;
		try
		{
			assert(!p->connection);
			p->connection = &m_torrent->connect_to_peer(p->ip);
			assert(p->connection);
			p->connection->add_stat(p->prev_amount_download, p->prev_amount_upload);
			p->prev_amount_download = 0;
			p->prev_amount_upload = 0;
			p->connected =
				m_last_optimistic_disconnect = 
					second_clock::universal_time();
			return true;
		}
		catch (std::exception& e)
		{}
		return false;
	}

	bool policy::disconnect_one_peer()
	{
		peer *p = find_disconnect_candidate();
		if(!p)
			return false;
#if defined(TORRENT_VERBOSE_LOGGING)
		(*p->connection->m_logger) << "*** CLOSING CONNECTION 'too many connections'\n";
#endif

		p->connection->disconnect();
		return true;
	}

	
	void policy::connection_closed(const peer_connection& c) try
	{
		INVARIANT_CHECK; 

		bool unchoked = false;

		std::vector<peer>::iterator i = std::find_if(
			m_peers.begin()
			, m_peers.end()
			, match_peer_connection(c));

		
		if (i == m_peers.end()) return;
		assert(i->connection == &c);

		i->connected = second_clock::universal_time();
		if (!i->connection->is_choked() && !m_torrent->is_aborted())
		{
			unchoked = true;
		}

		if (c.failed())
		{
			i->type = peer::not_connectable;
			i->ip.port(0);
		}

		
		
		
		if (m_torrent->ratio() != 0.f)
		{
			assert(i->connection->associated_torrent().lock().get() == m_torrent);
			assert(i->connection->share_diff() < std::numeric_limits<size_type>::max());
			m_available_free_upload += i->connection->share_diff();
		}
		i->prev_amount_download += c.statistics().total_payload_download();
		i->prev_amount_upload += c.statistics().total_payload_upload();
		i->connection = 0;

		if (unchoked)
		{
			
			
			
			--m_num_unchoked;
			if (m_torrent->is_seed()) seed_unchoke_one_peer();
			else unchoke_one_peer();
		}
	}
	catch (std::exception& e)
	{
#ifndef NDEBUG
		std::string err = e.what();
#endif
		assert(false);
	}

	void policy::peer_is_interesting(peer_connection& c)
	{
		INVARIANT_CHECK;

		c.send_interested();
		if (c.has_peer_choked()) return;
		request_a_block(*m_torrent, c);
	}

#ifndef NDEBUG
	bool policy::has_connection(const peer_connection* c)
	{
		assert(c);
		assert((c->proxy() == tcp::endpoint() && c->remote() == c->get_socket()->remote_endpoint())
			|| c->proxy() == c->get_socket()->remote_endpoint());

		return std::find_if(
			m_peers.begin()
			, m_peers.end()
			, match_peer_connection(*c)) != m_peers.end();
	}

	void policy::check_invariant() const
	{
		if (m_torrent->is_aborted()) return;
		int actual_unchoked = 0;
		int connected_peers = 0;

		int total_connections = 0;
		int nonempty_connections = 0;
		
		
		for (std::vector<peer>::const_iterator i = m_peers.begin();
			i != m_peers.end(); ++i)
		{
			++total_connections;
			if (!i->connection) continue;
			++nonempty_connections;
			if (!i->connection->is_disconnecting())
				++connected_peers;
			if (!i->connection->is_choked()) ++actual_unchoked;
		}

		assert(actual_unchoked == m_num_unchoked);

		int num_torrent_peers = 0;
		for (torrent::const_peer_iterator i = m_torrent->begin();
			i != m_torrent->end(); ++i)
		{
			if (i->second->is_disconnecting()) continue;
			
			
			if (dynamic_cast<web_peer_connection*>(i->second)) continue;
			++num_torrent_peers;
		}

		
		
		
		
		
		
		
		

		assert(connected_peers == num_torrent_peers
			|| (connected_peers == num_torrent_peers + 1
				&& connected_peers > 0)
			|| (connected_peers + 1 == num_torrent_peers
				&& num_torrent_peers > 0));
		
		
		
	}
#endif

	policy::peer::peer(const tcp::endpoint& ip_, peer::connection_type t)
		: ip(ip_)
		, type(t)
		, last_optimistically_unchoked(
			boost::gregorian::date(1970,boost::gregorian::Jan,1))
		, connected(boost::gregorian::date(1970,boost::gregorian::Jan,1))
		, prev_amount_upload(0)
		, prev_amount_download(0)
		, banned(false)
		, connection(0)
	{
		assert(connected < second_clock::universal_time());
	}

	size_type policy::peer::total_download() const
	{
		if (connection != 0)
		{
			assert(prev_amount_download == 0);
			return connection->statistics().total_payload_download();
		}
		else
		{
			return prev_amount_download;
		}
	}

	size_type policy::peer::total_upload() const
	{
		if (connection != 0)
		{
			assert(prev_amount_upload == 0);
			return connection->statistics().total_payload_upload();
		}
		else
		{
			return prev_amount_upload;
		}
	}
}

