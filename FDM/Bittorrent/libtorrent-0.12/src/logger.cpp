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

#include "libtorrent/extensions/logger.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/entry.hpp"
#include "libtorrent/peer_request.hpp"
#include "libtorrent/peer_connection.hpp"

namespace libtorrent { namespace
{

	struct logger_peer_plugin : peer_plugin
	{
		logger_peer_plugin(std::string const& filename)
		{
			using namespace boost::filesystem;
			path dir(complete("libtorrent_ext_logs"));
			if (!exists(dir)) create_directories(dir);
			m_file.open(dir / filename, std::ios_base::out | std::ios_base::out);
			m_file << "\n\n\n";
			log_timestamp();
			m_file << "*** starting log ***\n";
		}

		void log_timestamp()
		{
			using namespace boost::posix_time;
			std::string now(to_simple_string(second_clock::universal_time()));
			m_file << now << ": ";
		}

		
		virtual void add_handshake(entry&) {}
		
		
		virtual bool on_extension_handshake(entry const& h)
		{
			log_timestamp();
			m_file << "<== EXTENSION_HANDSHAKE\n";
			h.print(m_file);
			return true;
		}

		
		
		
		
		

		virtual bool on_choke()
		{
			log_timestamp();
			m_file << "<== CHOKE\n";
			m_file.flush();
			return false;
		}

		virtual bool on_unchoke()
		{
			log_timestamp();
			m_file << "<== UNCHOKE\n";
			m_file.flush();
			return false;
		}

		virtual bool on_interested()
		{
			log_timestamp();
			m_file << "<== INTERESTED\n";
			m_file.flush();
			return false;
		}

		virtual bool on_not_interested()
		{
			log_timestamp();
			m_file << "<== NOT_INTERESTED\n";
			m_file.flush();
			return false;
		}

		virtual bool on_have(int index)
		{
			log_timestamp();
			m_file << "<== HAVE [" << index << "]\n";
			m_file.flush();
			return false;
		}

		virtual bool on_bitfield(std::vector<bool> const& bitfield)
		{
			log_timestamp();
			m_file << "<== BITFIELD\n";
			m_file.flush();
			return false;
		}

		virtual bool on_request(peer_request const& r)
		{
			log_timestamp();
			m_file << "<== REQUEST [ piece: " << r.piece << " | s: " << r.start
				<< " | l: " << r.length << " ]\n";
			m_file.flush();
			return false;
		}

		virtual bool on_piece(peer_request const& r, char const*)
		{
			log_timestamp();
			m_file << "<== PIECE [ piece: " << r.piece << " | s: " << r.start
				<< " | l: " << r.length << " ]\n";
			m_file.flush();
			return false;
		}

		virtual bool on_cancel(peer_request const& r)
		{
			log_timestamp();
			m_file << "<== CANCEL [ piece: " << r.piece << " | s: " << r.start
				<< " | l: " << r.length << " ]\n";
			m_file.flush();
			return false;
		}
	
		
		
		
		
		virtual bool on_extended(int length
			, int msg, buffer::const_interval body)
		{ return false; }

		virtual bool on_unknown_message(int length, int msg
			, buffer::const_interval body)
		{
			if (body.left() < length) return false;
			log_timestamp();
			m_file << "<== UNKNOWN [ msg: " << msg
				<< " | l: " << length << " ]\n";
			m_file.flush();
			return false;
		}

		virtual void on_piece_pass(int index)
		{
			log_timestamp();
			m_file << "*** HASH PASSED *** [ piece: " << index << " ]\n";
			m_file.flush();
		}

		virtual void on_piece_failed(int index)
		{
			log_timestamp();
			m_file << "*** HASH FAILED *** [ piece: " << index << " ]\n";
			m_file.flush();
		}

	private:
		boost::filesystem::ofstream m_file;
	};

	struct logger_plugin : torrent_plugin
	{
		virtual boost::shared_ptr<peer_plugin> new_connection(
			peer_connection* pc)
		{
			return boost::shared_ptr<peer_plugin>(new logger_peer_plugin(
				pc->remote().address().to_string() + "_"
				+ boost::lexical_cast<std::string>(pc->remote().port()) + ".log"));
		}
	};

} }

namespace libtorrent
{

	boost::shared_ptr<torrent_plugin> create_logger_plugin(torrent*)
	{
		return boost::shared_ptr<torrent_plugin>(new logger_plugin());
	}

} 

