/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <vector>
#include <iostream>
#include <cctype>
#include <iomanip>
#include <sstream>

#include "zlib.h"

#include <boost/bind.hpp>

#include "libtorrent/tracker_manager.hpp"
#include "libtorrent/http_tracker_connection.hpp"
#include "libtorrent/udp_tracker_connection.hpp"
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent.hpp"
#include "libtorrent/peer_connection.hpp"

using namespace libtorrent;
using boost::tuples::make_tuple;
using boost::tuples::tuple;
using boost::bind;

namespace
{
	enum
	{
		minimum_tracker_response_length = 3,
		http_buffer_size = 2048
	}; 

	enum
	{
		FTEXT = 0x01,
		FHCRC = 0x02,
		FEXTRA = 0x04,
		FNAME = 0x08,
		FCOMMENT = 0x10,
		FRESERVED = 0xe0,

		GZIP_MAGIC0 = 0x1f,
		GZIP_MAGIC1 = 0x8b
	};

}

namespace libtorrent
{
	using boost::posix_time::second_clock;
	using boost::posix_time::seconds;
	using boost::posix_time::ptime;
	using boost::posix_time::time_duration;

	
	int gzip_header(const char* buf, int size)
	{
		assert(buf != 0);
		assert(size > 0);

		const unsigned char* buffer = reinterpret_cast<const unsigned char*>(buf);
		const int total_size = size;

		
		if (size < 10) return -1;

		
		if ((buffer[0] != GZIP_MAGIC0) || (buffer[1] != GZIP_MAGIC1)) return -1;

		int method = buffer[2];
		int flags = buffer[3];

		
		if (method != Z_DEFLATED || (flags & FRESERVED) != 0) return -1;

		
		size -= 10;
		buffer += 10;

		if (flags & FEXTRA)
		{
			int extra_len;

			if (size < 2) return -1;

			extra_len = (buffer[1] << 8) | buffer[0];

			if (size < (extra_len+2)) return -1;
			size -= (extra_len + 2);
			buffer += (extra_len + 2);
		}

		if (flags & FNAME)
		{
			while (size && *buffer)
			{
				--size;
				++buffer;
			}
			if (!size || *buffer) return -1;

			--size;
			++buffer;
		}

		if (flags & FCOMMENT)
		{
			while (size && *buffer)
			{
				--size;
				++buffer;
			}
			if (!size || *buffer) return -1;

			--size;
			++buffer;
		}

		if (flags & FHCRC)
		{
			if (size < 2) return -1;

			size -= 2;
			buffer += 2;
		}

		return total_size - size;
	}

	bool inflate_gzip(
		std::vector<char>& buffer
		, tracker_request const& req
		, request_callback* requester
		, int maximum_tracker_response_length)
	{
		assert(maximum_tracker_response_length > 0);

		int header_len = gzip_header(&buffer[0], (int)buffer.size());
		if (header_len < 0)
		{
			requester->tracker_request_error(req, 200, "invalid gzip header in tracker response");
			return true;
		}

		
		
		std::vector<char> inflate_buffer(1024);

		
		z_stream str;

		
		
		str.avail_in = (int)buffer.size() - header_len - 8;
		str.next_in = reinterpret_cast<Bytef*>(&buffer[header_len]);
		str.next_out = reinterpret_cast<Bytef*>(&inflate_buffer[0]);
		str.avail_out = (int)inflate_buffer.size();
		str.zalloc = Z_NULL;
		str.zfree = Z_NULL;
		str.opaque = 0;
		
		
		if (inflateInit2(&str, -15) != Z_OK)
		{
			requester->tracker_request_error(req, 200, "gzip out of memory");
			return true;
		}

		
		int ret = inflate(&str, Z_SYNC_FLUSH);
		while (ret == Z_OK)
		{
			if (str.avail_out == 0)
			{
				if (inflate_buffer.size() >= (unsigned)maximum_tracker_response_length)
				{
					inflateEnd(&str);
					requester->tracker_request_error(req, 200
						, "tracker response too large");
					return true;
				}
				int new_size = (int)inflate_buffer.size() * 2;
				if (new_size > maximum_tracker_response_length) new_size = maximum_tracker_response_length;
				int old_size = (int)inflate_buffer.size();

				inflate_buffer.resize(new_size);
				str.next_out = reinterpret_cast<Bytef*>(&inflate_buffer[old_size]);
				str.avail_out = new_size - old_size;
			}

			ret = inflate(&str, Z_SYNC_FLUSH);
		}

		inflate_buffer.resize(inflate_buffer.size() - str.avail_out);
		inflateEnd(&str);

		if (ret != Z_STREAM_END)
		{
			requester->tracker_request_error(req, 200, "gzip error");
			return true;
		}

		
		std::swap(buffer, inflate_buffer);
		return false;
	}

	std::string base64encode(const std::string& s)
	{
		static const char base64_table[] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};

		unsigned char inbuf[3];
		unsigned char outbuf[4];
	
		std::string ret;
		for (std::string::const_iterator i = s.begin(); i != s.end();)
		{
			
			
			int available_input = std::min(3, (int)std::distance(i, s.end()));

			
			std::fill(inbuf, inbuf+3, 0);

			
			for (int j = 0; j < available_input; ++j)
			{
				inbuf[j] = *i;
				++i;
			}

			
			outbuf[0] = (inbuf[0] & 0xfc) >> 2;
			outbuf[1] = ((inbuf[0] & 0x03) << 4) | ((inbuf [1] & 0xf0) >> 4);
			outbuf[2] = ((inbuf[1] & 0x0f) << 2) | ((inbuf [2] & 0xc0) >> 6);
			outbuf[3] = inbuf[2] & 0x3f;

			
			for (int j = 0; j < available_input+1; ++j)
			{
				ret += base64_table[outbuf[j]];
			}

			
			for (int j = 0; j < 3 - available_input; ++j)
			{
				ret += '=';
			}
		}
		return ret;
	}

	void intrusive_ptr_add_ref(timeout_handler const* c)
	{
		assert(c != 0);
		assert(c->m_refs >= 0);
		timeout_handler::mutex_t::scoped_lock l(c->m_mutex);
		++c->m_refs;
	}

	void intrusive_ptr_release(timeout_handler const* c)
	{
		assert(c != 0);
		assert(c->m_refs > 0);
		timeout_handler::mutex_t::scoped_lock l(c->m_mutex);
		--c->m_refs;
		if (c->m_refs == 0)
		{
			l.unlock();
			delete c;
		}
	} 

	timeout_handler::timeout_handler(asio::strand& str)
		: m_strand(str)
		, m_start_time(second_clock::universal_time())
		, m_read_time(second_clock::universal_time())
		, m_timeout(str.io_service())
		, m_completion_timeout(0)
		, m_read_timeout(0)
		, m_refs(0)
	{}

	void timeout_handler::set_timeout(int completion_timeout, int read_timeout)
	{
		m_completion_timeout = completion_timeout;
		m_read_timeout = read_timeout;
		m_start_time = second_clock::universal_time();
		m_read_time = second_clock::universal_time();

		m_timeout.expires_at(std::min(
			m_read_time + seconds(m_read_timeout)
			, m_start_time + seconds(m_completion_timeout)));
		m_timeout.async_wait(m_strand.wrap(bind(
			&timeout_handler::timeout_callback, self(), _1)));
	}

	void timeout_handler::restart_read_timeout()
	{
		m_read_time = second_clock::universal_time();
	}

	void timeout_handler::cancel()
	{
		m_completion_timeout = 0;
		m_timeout.cancel();
	}

	void timeout_handler::timeout_callback(asio::error_code const& error) try
	{
		if (error) return;
		if (m_completion_timeout == 0) return;
		
		ptime now(second_clock::universal_time());
		time_duration receive_timeout = now - m_read_time;
		time_duration completion_timeout = now - m_start_time;
		
		if (m_read_timeout
			< receive_timeout.total_seconds()
			|| m_completion_timeout
			< completion_timeout.total_seconds())
		{
			on_timeout();
			return;
		}

		m_timeout.expires_at(std::min(
			m_read_time + seconds(m_read_timeout)
			, m_start_time + seconds(m_completion_timeout)));
		m_timeout.async_wait(m_strand.wrap(
			bind(&timeout_handler::timeout_callback, self(), _1)));
	}
	catch (std::exception& e)
	{
		assert(false);
	}

	tracker_connection::tracker_connection(
		tracker_manager& man
		, tracker_request req
		, asio::strand& str
		, address bind_interface_
		, boost::weak_ptr<request_callback> r)
		: timeout_handler(str)
		, m_requester(r)
		, m_bind_interface(bind_interface_)
		, m_man(man)
		, m_req(req)
	{}

	request_callback& tracker_connection::requester()
	{
		boost::shared_ptr<request_callback> r = m_requester.lock();
		assert(r);
		return *r;
	}

	void tracker_connection::fail(int code, char const* msg)
	{
		if (has_requester()) requester().tracker_request_error(
			m_req, code, msg);
		close();
	}

	void tracker_connection::fail_timeout()
	{
		if (has_requester()) requester().tracker_request_timed_out(m_req);
		close();
	}
	
	void tracker_connection::close()
	{
		cancel();
		m_man.remove_request(this);
	}

	void tracker_manager::remove_request(tracker_connection const* c)
	{
		mutex_t::scoped_lock l(m_mutex);

		tracker_connections_t::iterator i = std::find(m_connections.begin()
			, m_connections.end(), boost::intrusive_ptr<const tracker_connection>(c));
		if (i == m_connections.end()) return;

		m_connections.erase(i);
	}
	
	tuple<std::string, std::string, int, std::string>
		parse_url_components(std::string url)
	{
		std::string hostname; 
		std::string protocol; 
		int port = 80;

		
		std::string::iterator start = url.begin();
		
		while (start != url.end() && (*start == ' ' || *start == '\t'))
			++start;
		std::string::iterator end
			= std::find(url.begin(), url.end(), ':');
		protocol = std::string(start, end);

		if (end == url.end()) throw std::runtime_error("invalid url");
		++end;
		if (end == url.end()) throw std::runtime_error("invalid url");
		if (*end != '/') throw std::runtime_error("invalid url");
		++end;
		if (end == url.end()) throw std::runtime_error("invalid url");
		if (*end != '/') throw std::runtime_error("invalid url");
		++end;
		start = end;

		end = std::find(start, url.end(), '/');
		std::string::iterator port_pos
			= std::find(start, url.end(), ':');

		if (port_pos < end)
		{
			hostname.assign(start, port_pos);
			++port_pos;
			try
			{
				port = boost::lexical_cast<int>(std::string(port_pos, end));
			}
			catch(boost::bad_lexical_cast&)
			{
				throw std::runtime_error("invalid url: \"" + url
					+ "\", port number expected");
			}
		}
		else
		{
			hostname.assign(start, end);
		}

		start = end;
		return make_tuple(protocol, hostname, port
			, std::string(start, url.end()));
	}

	void tracker_manager::queue_request(
		asio::strand& str
		, tracker_request req
		, std::string const& auth
		, address bind_infc
		, boost::weak_ptr<request_callback> c)
	{
		mutex_t::scoped_lock l(m_mutex);
		assert(req.num_want >= 0);
		if (req.event == tracker_request::stopped)
			req.num_want = 0;

		try
		{
			std::string protocol;
			std::string hostname;
			int port;
			std::string request_string;

			boost::tie(protocol, hostname, port, request_string)
				= parse_url_components(req.url);

			boost::intrusive_ptr<tracker_connection> con;

			if (protocol == "http")
			{
				con = new http_tracker_connection(
					str
					, *this
					, req
					, hostname
					, port
					, request_string
					, bind_infc
					, c
					, m_settings
					, auth);
			}
			else if (protocol == "udp")
			{
				con = new udp_tracker_connection(
					str
					, *this
					, req
					, hostname
					, port
					, bind_infc
					, c
					, m_settings);
			}
			else
			{
				throw std::runtime_error("unkown protocol in tracker url");
			}

			m_connections.push_back(con);

			if (con->has_requester()) con->requester().m_manager = this;
		}
		catch (std::exception& e)
		{
			if (boost::shared_ptr<request_callback> r = c.lock())
				r->tracker_request_error(req, -1, e.what());
		}
	}

	void tracker_manager::abort_all_requests()
	{
		
		
		
		mutex_t::scoped_lock l(m_mutex);

		tracker_connections_t keep_connections;

		for (tracker_connections_t::const_iterator i =
			m_connections.begin(); i != m_connections.end(); ++i)
		{
			tracker_request const& req = (*i)->tracker_req();
			if (req.event == tracker_request::stopped)
				keep_connections.push_back(*i);
		}

		std::swap(m_connections, keep_connections);
	}
	
	bool tracker_manager::empty() const
	{
		mutex_t::scoped_lock l(m_mutex);
		return m_connections.empty();
	}

}
