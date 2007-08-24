/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

  

 

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <vector>
#include <utility>
#include <numeric>

#include "libtorrent/peer_connection.hpp"
#include "libtorrent/bt_peer_connection.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/extensions/metadata_transfer.hpp"

using boost::posix_time::second_clock;

namespace libtorrent { namespace
{
	int div_round_up(int numerator, int denominator)
	{
		return (numerator + denominator - 1) / denominator;
	}

	std::pair<int, int> req_to_offset(std::pair<int, int> req, int total_size)
	{
		assert(req.first >= 0);
		assert(req.second > 0);
		assert(req.second <= 256);
		assert(req.first + req.second <= 256);

		int start = div_round_up(req.first * total_size, 256);
		int size = div_round_up((req.first + req.second) * total_size, 256) - start;
		return std::make_pair(start, size);
	}

	std::pair<int, int> offset_to_req(std::pair<int, int> offset, int total_size)
	{
		int start = offset.first * 256 / total_size;
		int size = (offset.first + offset.second) * 256 / total_size - start;

		std::pair<int, int> ret(start, size);
	
		assert(start >= 0);
		assert(size > 0);
		assert(start <= 256);
		assert(start + size <= 256);

		
#ifndef NDEBUG
		std::pair<int, int> identity = req_to_offset(ret, total_size);
		assert(offset == identity);
#endif
		return ret;
	} 

	struct metadata_plugin : torrent_plugin
	{
		metadata_plugin(torrent& t)
			: m_torrent(t)
			, m_metadata_progress(0)
			, m_metadata_size(0)
		{
			m_requested_metadata.resize(256, 0);
		}
	
		virtual boost::shared_ptr<peer_plugin> new_connection(
			peer_connection* pc);
		
		std::vector<char> const& metadata() const
		{
			if (m_metadata.empty())
			{
				bencode(std::back_inserter(m_metadata)
					, m_torrent.torrent_file().create_info_metadata());

				assert(hasher(&m_metadata[0], m_metadata.size()).final()
					== m_torrent.torrent_file().info_hash());
			}
			assert(!m_metadata.empty());
			return m_metadata;
		}

		bool received_metadata(char const* buf, int size, int offset, int total_size)
		{
			if (m_torrent.valid_metadata()) return false;

			if ((int)m_metadata.size() < total_size)
				m_metadata.resize(total_size);

			std::copy(
				buf
				, buf + size
				, &m_metadata[offset]);

			if (m_have_metadata.empty())
				m_have_metadata.resize(256, false);

			std::pair<int, int> req = offset_to_req(std::make_pair(offset, size)
				, total_size);

			assert(req.first + req.second <= (int)m_have_metadata.size());

			std::fill(
				m_have_metadata.begin() + req.first
				, m_have_metadata.begin() + req.first + req.second
				, true);
		
			bool have_all = std::count(
				m_have_metadata.begin()
				, m_have_metadata.end()
				, true) == 256;

			if (!have_all) return false;

			hasher h;
			h.update(&m_metadata[0], (int)m_metadata.size());
			sha1_hash info_hash = h.final();

			if (info_hash != m_torrent.torrent_file().info_hash())
			{
				std::fill(
					m_have_metadata.begin()
					, m_have_metadata.begin() + req.first + req.second
					, false);
				m_metadata_progress = 0;
				m_metadata_size = 0;
				

				return false;
			}

			entry metadata = bdecode(m_metadata.begin(), m_metadata.end());
			m_torrent.set_metadata(metadata);

			
			std::vector<bool>().swap(m_have_metadata);
			std::vector<int>().swap(m_requested_metadata);

			return true;
		}

		
		
		std::pair<int, int> metadata_request();

		void cancel_metadata_request(std::pair<int, int> req)
		{
			for (int i = req.first; i < req.first + req.second; ++i)
			{
				assert(m_requested_metadata[i] > 0);
				if (m_requested_metadata[i] > 0)
					--m_requested_metadata[i];
			}
		}

		
		
		void metadata_progress(int total_size, int received)
		{
			m_metadata_progress += received;
			m_metadata_size = total_size;
		}

	private:
		torrent& m_torrent;

		
		
		
		
		mutable std::vector<char> m_metadata;

		int m_metadata_progress;
		int m_metadata_size;

		
		
		
		
		
		std::vector<bool> m_have_metadata;
		
		
		std::vector<int> m_requested_metadata;
	}; 

	struct metadata_peer_plugin : peer_plugin
	{
		metadata_peer_plugin(torrent& t, peer_connection& pc
			, metadata_plugin& tp)
			: m_waiting_metadata_request(false)
			, m_message_index(0)
			, m_metadata_progress(0)
			, m_no_metadata(
				boost::gregorian::date(1970, boost::date_time::Jan, 1)
				, boost::posix_time::seconds(0))
			, m_metadata_request(
				boost::gregorian::date(1970, boost::date_time::Jan, 1)
				, boost::posix_time::seconds(0))
			, m_torrent(t)
			, m_pc(pc)
			, m_tp(tp)
		{}

		
		virtual void add_handshake(entry& h)
		{
			entry& messages = h["m"];
			messages["LT_metadata"] = 14;
		}

		
		virtual bool on_extension_handshake(entry const& h)
		{
			entry const& messages = h["m"];
			if (entry const* index = messages.find_key("LT_metadata"))
			{
				m_message_index = index->integer();
				return true;
			}
			else
			{
				m_message_index = 0;
				return false;
			}
		}

		void write_metadata_request(std::pair<int, int> req)
		{
			assert(req.first >= 0);
			assert(req.second > 0);
			assert(req.first + req.second <= 256);
			assert(!m_pc.associated_torrent().expired());
			assert(!m_pc.associated_torrent().lock()->valid_metadata());

			int start = req.first;
			int size = req.second;

			
			if (m_message_index == 0) return;

			buffer::interval i = m_pc.allocate_send_buffer(9);

			detail::write_uint32(1 + 1 + 3, i.begin);
			detail::write_uint8(bt_peer_connection::msg_extended, i.begin);
			detail::write_uint8(m_message_index, i.begin);
			
			detail::write_uint8(0, i.begin);
			detail::write_uint8(start, i.begin);
			detail::write_uint8(size - 1, i.begin);
			assert(i.begin == i.end);
			m_pc.setup_send();
		}

		void write_metadata(std::pair<int, int> req)
		{
			assert(req.first >= 0);
			assert(req.second > 0);
			assert(req.second <= 256);
			assert(req.first + req.second <= 256);
			assert(!m_pc.associated_torrent().expired());

			
			if (m_message_index == 0) return;

			
			if (m_torrent.valid_metadata() && !m_torrent.torrent_file().priv())
			{
				std::pair<int, int> offset
					= req_to_offset(req, (int)m_tp.metadata().size());

				buffer::interval i = m_pc.allocate_send_buffer(15 + offset.second);

				
				detail::write_uint32(11 + offset.second, i.begin);
				detail::write_uint8(bt_peer_connection::msg_extended, i.begin);
				detail::write_uint8(m_message_index, i.begin);
				
				detail::write_uint8(1, i.begin);
				detail::write_uint32((int)m_tp.metadata().size(), i.begin);
				detail::write_uint32(offset.first, i.begin);
				std::vector<char> const& metadata = m_tp.metadata();
				std::copy(metadata.begin() + offset.first
					, metadata.begin() + offset.first + offset.second, i.begin);
				i.begin += offset.second;
				assert(i.begin == i.end);
			}
			else
			{
				buffer::interval i = m_pc.allocate_send_buffer(4 + 3);
				
				
				detail::write_uint32(1 + 2, i.begin);
				detail::write_uint8(bt_peer_connection::msg_extended, i.begin);
				detail::write_uint8(m_message_index, i.begin);
				
				detail::write_uint8(2, i.begin);
				assert(i.begin == i.end);
			}
			m_pc.setup_send();
		}

		virtual bool on_extended(int length
			, int msg, buffer::const_interval body)
		{
			if (msg != 14) return false;
			if (m_message_index == 0) return false;

			if (length > 500 * 1024)
				throw protocol_error("LT_metadata message larger than 500 kB");

			if (body.left() < 1) return true;
			int type = detail::read_uint8(body.begin);

			switch (type)
			{
			case 0: 
				{
					if (body.left() < 2) return true;
					int start = detail::read_uint8(body.begin);
					int size = detail::read_uint8(body.begin) + 1;

					if (length != 3)
					{
						
						throw protocol_error("invalid metadata request");
					}

					write_metadata(std::make_pair(start, size));
				}
				break;
			case 1: 
				{
					if (body.left() < 8) return true;

					int total_size = detail::read_int32(body.begin);
					int offset = detail::read_int32(body.begin);
					int data_size = length - 9;

					if (total_size > 500 * 1024)
						throw protocol_error("metadata size larger than 500 kB");
					if (total_size <= 0)
						throw protocol_error("invalid metadata size");
					if (offset > total_size || offset < 0)
						throw protocol_error("invalid metadata offset");
					if (offset + data_size > total_size)
						throw protocol_error("invalid metadata message");

					m_tp.metadata_progress(total_size
						, body.left() - m_metadata_progress);
					m_metadata_progress = body.left();

					if (body.left() < data_size) return true;

					m_waiting_metadata_request = false;
					m_tp.received_metadata(body.begin, data_size
						, offset, total_size);
					m_metadata_progress = 0;
				}
				break;
			case 2: 
				m_no_metadata = second_clock::universal_time();
				if (m_waiting_metadata_request)
					m_tp.cancel_metadata_request(m_last_metadata_request);
				m_waiting_metadata_request = false;
				break;
			default:
				throw protocol_error("unknown metadata extension message: "
					+ boost::lexical_cast<std::string>(type));
			}
			return true;
		}

		virtual void tick()
		{
			
			
			
			
			if (!m_torrent.valid_metadata()
				&& m_message_index != 0
				&& !m_waiting_metadata_request
				&& has_metadata())
			{
				m_last_metadata_request = m_tp.metadata_request();
				write_metadata_request(m_last_metadata_request);
				m_waiting_metadata_request = true;
				m_metadata_request = second_clock::universal_time();
			}
		}

		bool has_metadata() const
		{
			using namespace boost::posix_time;
			return second_clock::universal_time() - m_no_metadata > minutes(5);
		}

	private:

		
		
		
		bool m_waiting_metadata_request;
		
		
		
		int m_message_index;

		
		
		
		
		
		int m_metadata_progress;

		
		
		boost::posix_time::ptime m_no_metadata;

		
		
		boost::posix_time::ptime m_metadata_request;

		
		
		std::pair<int, int> m_last_metadata_request;

		torrent& m_torrent;
		peer_connection& m_pc;
		metadata_plugin& m_tp;
	};

	boost::shared_ptr<peer_plugin> metadata_plugin::new_connection(
		peer_connection* pc)
	{
		return boost::shared_ptr<peer_plugin>(new metadata_peer_plugin(m_torrent, *pc, *this));
	}

	std::pair<int, int> metadata_plugin::metadata_request()
	{
		
		
		int peers = 0;
#ifndef TORRENT_DISABLE_EXTENSIONS
		typedef std::map<tcp::endpoint, peer_connection*> conn_map;
		for (conn_map::iterator i = m_torrent.begin()
			, end(m_torrent.end()); i != end; ++i)
		{
			bt_peer_connection* c = dynamic_cast<bt_peer_connection*>(i->second);
			if (c == 0) continue;
			metadata_peer_plugin* p
				= c->supports_extension<metadata_peer_plugin>();
			if (p == 0) continue;
			if (!p->has_metadata()) continue;
			++peers;
		}
#endif

		
		int num_blocks = 256 / (peers + 1);
		if (num_blocks < 1) num_blocks = 1;
		assert(num_blocks <= 128);

		int min_element = std::numeric_limits<int>::max();
		int best_index = 0;
		for (int i = 0; i < 256 - num_blocks + 1; ++i)
		{
			int min = *std::min_element(m_requested_metadata.begin() + i
				, m_requested_metadata.begin() + i + num_blocks);
			min += std::accumulate(m_requested_metadata.begin() + i
				, m_requested_metadata.begin() + i + num_blocks, (int)0);

			if (min_element > min)
			{
				best_index = i;
				min_element = min;
			}
		}

		std::pair<int, int> ret(best_index, num_blocks);
		for (int i = ret.first; i < ret.first + ret.second; ++i)
			m_requested_metadata[i]++;

		assert(ret.first >= 0);
		assert(ret.second > 0);
		assert(ret.second <= 256);
		assert(ret.first + ret.second <= 256);

		return ret;
	}

} }

namespace libtorrent
{

	boost::shared_ptr<torrent_plugin> create_metadata_plugin(torrent* t)
	{
		return boost::shared_ptr<torrent_plugin>(new metadata_plugin(*t));
	}

} 

