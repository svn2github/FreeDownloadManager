/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

  

 

#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <set>

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/next_prior.hpp>
#include <boost/bind.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "libtorrent/torrent_info.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/entry.hpp"

using namespace libtorrent;
using namespace boost::filesystem;

namespace
{
	void convert_to_utf8(std::string& str, unsigned char chr)
	{
		str += 0xc0 | ((chr & 0xff) >> 6);
		str += 0x80 | (chr & 0x3f);
	}

	void verify_encoding(file_entry& target)
	{
		std::string tmp_path;
		std::string file_path = target.path.string();
		bool valid_encoding = true;
		for (std::string::iterator i = file_path.begin()
			, end(file_path.end()); i != end; ++i)
		{
			
			if ((*i & 0x80) == 0)
			{
				tmp_path += *i;
				continue;
			}
			
			if (std::distance(i, end) < 2)
			{
				convert_to_utf8(tmp_path, *i);
				valid_encoding = false;
				continue;
			}
			
			
			if ((i[0] & 0xe0) == 0xc0
				&& (i[1] & 0xc0) == 0x80)
			{
				tmp_path += i[0];
				tmp_path += i[1];
				i += 1;
				continue;
			}

			if (std::distance(i, end) < 3)
			{
				convert_to_utf8(tmp_path, *i);
				valid_encoding = false;
				continue;
			}

			
			if ((i[0] & 0xf0) == 0xe0
				&& (i[1] & 0xc0) == 0x80
				&& (i[2] & 0xc0) == 0x80)
			{
				tmp_path += i[0];
				tmp_path += i[1];
				tmp_path += i[2];
				i += 2;
				continue;
			}

			if (std::distance(i, end) < 4)
			{
				convert_to_utf8(tmp_path, *i);
				valid_encoding = false;
				continue;
			}

			
			if ((i[0] & 0xf0) == 0xe0
				&& (i[1] & 0xc0) == 0x80
				&& (i[2] & 0xc0) == 0x80
				&& (i[3] & 0xc0) == 0x80)
			{
				tmp_path += i[0];
				tmp_path += i[1];
				tmp_path += i[2];
				tmp_path += i[3];
				i += 3;
				continue;
			}

			convert_to_utf8(tmp_path, *i);
			valid_encoding = false;
		}
		
		
		
		
		if (!valid_encoding)
		{
			target.orig_path.reset(new path(target.path));
			target.path = tmp_path;
		}
	}

	void extract_single_file(const entry& dict, file_entry& target
		, std::string const& root_dir)
	{
		target.size = dict["length"].integer();
		target.path = root_dir; 

		
		
		

		const entry::list_type* list = 0;
		if (entry const* p = dict.find_key("path.utf-8"))
		{
			list = &p->list();
		}
		else
		{
			list = &dict["path"].list();
		}

		for (entry::list_type::const_iterator i = list->begin();
			i != list->end(); ++i)
		{
			if (i->string() != "..")
				target.path /= i->string();
		}
		verify_encoding(target);
		if (target.path.is_complete()) throw std::runtime_error("torrent contains "
			"a file with an absolute path: '"
			+ target.path.native_file_string() + "'");
	}

	void extract_files(const entry::list_type& list, std::vector<file_entry>& target
		, std::string const& root_dir)
	{
		size_type offset = 0;
		for (entry::list_type::const_iterator i = list.begin(); i != list.end(); ++i)
		{
			target.push_back(file_entry());
			extract_single_file(*i, target.back(), root_dir);
			target.back().offset = offset;
			offset += target.back().size;
		}
	}

	void remove_dir(path& p)
	{
		assert(p.begin() != p.end());
		path tmp;
		for (path::iterator i = boost::next(p.begin()); i != p.end(); ++i)
			tmp /= *i;
		p = tmp;
	}
}

namespace libtorrent
{

	using namespace boost::gregorian;
	using namespace boost::posix_time;

	
	torrent_info::torrent_info(const entry& torrent_file)
		: m_creation_date(date(not_a_date_time))
		, m_multifile(false)
		, m_private(false)
		, m_extra_info(entry::dictionary_t)
	{
		try
		{
			read_torrent_info(torrent_file);
		}
		catch(type_error&)
		{
			throw invalid_torrent_file();
		}
	}

	
	
	
	
	torrent_info::torrent_info(sha1_hash const& info_hash)
		: m_piece_length(0)
		, m_total_size(0)
		, m_info_hash(info_hash)
		, m_name()
		, m_creation_date(second_clock::universal_time())
		, m_multifile(false)
		, m_private(false)
		, m_extra_info(entry::dictionary_t)
	{
	}

	torrent_info::torrent_info()
		: m_piece_length(0)
		, m_total_size(0)
		, m_info_hash(0)
		, m_name()
		, m_creation_date(second_clock::universal_time())
		, m_multifile(false)
		, m_private(false)
		, m_extra_info(entry::dictionary_t)
	{
	}

	torrent_info::~torrent_info()
	{}

	void torrent_info::set_piece_size(int size)
	{
		
#ifndef NDEBUG
		for (int i = 0; i < 32; ++i)
		{
			if (size & (1 << i))
			{
				assert((size & ~(1 << i)) == 0);
				break;
			}
		}
#endif
		m_piece_length = size; 

		int num_pieces = static_cast<int>(
			(m_total_size + m_piece_length - 1) / m_piece_length);
		int old_num_pieces = static_cast<int>(m_piece_hash.size());

		m_piece_hash.resize(num_pieces);
		for (int i = old_num_pieces; i < num_pieces; ++i)
		{
			m_piece_hash[i].clear();
		}
	}

	void torrent_info::parse_info_section(entry const& info)
	{
		
		std::vector<char> buf;
		bencode(std::back_inserter(buf), info);
		hasher h;
		h.update(&buf[0], (int)buf.size());
		m_info_hash = h.final();

		
		m_piece_length = (int)info["piece length"].integer();
		if (m_piece_length <= 0) throw std::runtime_error("invalid torrent. piece length <= 0");

		
		if (entry const* e = info.find_key("name.utf-8"))
		{ m_name = e->string(); }
		else
		{ m_name = info["name"].string(); }
		
		path tmp = m_name;
		if (tmp.is_complete()) throw std::runtime_error("torrent contains "
			"a file with an absolute path: '" + m_name + "'");
		if (tmp.has_branch_path()) throw std::runtime_error(
			"torrent contains name with directories: '" + m_name + "'");
	
		
		entry const* i = info.find_key("files");
		if (i == 0)
		{
			
			
			file_entry e;
			e.path = m_name;
			e.offset = 0;
			e.size = info["length"].integer();
			m_files.push_back(e);
		}
		else
		{
			extract_files(i->list(), m_files, m_name);
			m_multifile = true;
		}

		
		m_total_size = 0;
		for (std::vector<file_entry>::iterator i = m_files.begin(); i != m_files.end(); ++i)
			m_total_size += i->size;

		
		
		

		int num_pieces = static_cast<int>((m_total_size + m_piece_length - 1) / m_piece_length);
		m_piece_hash.resize(num_pieces);
		const std::string& hash_string = info["pieces"].string();

		if ((int)hash_string.length() != num_pieces * 20)
			throw invalid_torrent_file();

		for (int i = 0; i < num_pieces; ++i)
			std::copy(
				hash_string.begin() + i*20
				, hash_string.begin() + (i+1)*20
				, m_piece_hash[i].begin());

		for (entry::dictionary_type::const_iterator i = info.dict().begin()
			, end(info.dict().end()); i != end; ++i)
		{
			if (i->first == "pieces"
				|| i->first == "piece length"
				|| i->first == "length")
				continue;
			m_extra_info[i->first] = i->second;
		}

		if (entry const* priv = info.find_key("private"))
		{
			if (priv->type() != entry::int_t
				|| priv->integer() != 0)
			{
				
				
				m_private = true;
			}
		}

#ifndef NDEBUG
		std::vector<char> info_section_buf;
		entry gen_info_section = create_info_metadata();
		bencode(std::back_inserter(info_section_buf), gen_info_section);
		assert(hasher(&info_section_buf[0], info_section_buf.size()).final()
			== m_info_hash);
#endif
	}

	
	
	void torrent_info::read_torrent_info(const entry& torrent_file)
	{
		
		if (entry const* i = torrent_file.find_key("announce-list"))
		{
			const entry::list_type& l = i->list();
			for (entry::list_type::const_iterator j = l.begin(); j != l.end(); ++j)
			{
				const entry::list_type& ll = j->list();
				for (entry::list_type::const_iterator k = ll.begin(); k != ll.end(); ++k)
				{
					announce_entry e(k->string());
					e.tier = (int)std::distance(l.begin(), j);
					m_urls.push_back(e);
				}
			}

			if (m_urls.size() == 0)
			{
				
				
				m_urls.push_back(announce_entry(
					torrent_file["announce"].string()));
			}
			
			std::vector<announce_entry>::iterator start = m_urls.begin();
			std::vector<announce_entry>::iterator stop;
			int current_tier = m_urls.front().tier;
			for (stop = m_urls.begin(); stop != m_urls.end(); ++stop)
			{
				if (stop->tier != current_tier)
				{
					std::random_shuffle(start, stop);
					start = stop;
					current_tier = stop->tier;
				}
			}
			std::random_shuffle(start, stop);
		}
		else if (entry const* i = torrent_file.find_key("announce"))
		{
			m_urls.push_back(announce_entry(i->string()));
		}

		if (entry const* i = torrent_file.find_key("nodes"))
		{
			entry::list_type const& list = i->list();
			for (entry::list_type::const_iterator i(list.begin())
				, end(list.end()); i != end; ++i)
			{
				if (i->type() != entry::list_t) continue;
				entry::list_type const& l = i->list();
				entry::list_type::const_iterator iter = l.begin();
				if (l.size() < 1) continue;
				std::string const& hostname = iter->string();
				++iter;
				int port = 6881;
				if (l.end() != iter) port = iter->integer();
				m_nodes.push_back(std::make_pair(hostname, port));
			}
		}

		
		try
		{
			m_creation_date = ptime(date(1970, Jan, 1))
				+ seconds(long(torrent_file["creation date"].integer()));
		}
		catch (type_error) {}

		
		try
		{
			entry const& url_seeds = torrent_file["url-list"];
			if (url_seeds.type() == entry::string_t)
			{
				m_url_seeds.push_back(url_seeds.string());
			}
			else if (url_seeds.type() == entry::list_t)
			{
				entry::list_type const& l = url_seeds.list();
				for (entry::list_type::const_iterator i = l.begin();
					i != l.end(); ++i)
				{
					m_url_seeds.push_back(i->string());
				}
			}
		}
		catch (type_error&) {}

		
		if (entry const* e = torrent_file.find_key("comment.utf-8"))
		{ m_comment = e->string(); }
		else if (entry const* e = torrent_file.find_key("comment"))
		{ m_comment = e->string(); }
	
		if (entry const* e = torrent_file.find_key("created by.utf-8"))
		{ m_created_by = e->string(); }
		else if (entry const* e = torrent_file.find_key("created by"))
		{ m_created_by = e->string(); }

		parse_info_section(torrent_file["info"]);
	}

	boost::optional<ptime>
	torrent_info::creation_date() const
	{
		if (m_creation_date != ptime(date(not_a_date_time)))
		{
			return boost::optional<ptime>(m_creation_date);
		}
		return boost::optional<ptime>();
	}

	void torrent_info::add_tracker(std::string const& url, int tier)
	{
		announce_entry e(url);
		e.tier = tier;
		m_urls.push_back(e);

		using boost::bind;
		std::sort(m_urls.begin(), m_urls.end(), boost::bind<bool>(std::less<int>()
			, bind(&announce_entry::tier, _1), bind(&announce_entry::tier, _2)));
	}

	void torrent_info::add_file(boost::filesystem::path file, size_type size)
	{
		assert(file.begin() != file.end());

		if (!file.has_branch_path())
		{
			
			
			
			
			assert(m_files.empty());
			assert(!m_multifile);
			m_name = file.string();
		}
		else
		{
#ifndef NDEBUG
			if (!m_files.empty())
				assert(m_name == *file.begin());
#endif
			m_multifile = true;
			m_name = *file.begin();
		}

		file_entry e;
		e.path = file;
		e.size = size;
		e.offset = m_files.empty() ? 0 : m_files.back().offset
			+ m_files.back().size;
		m_files.push_back(e);

		m_total_size += size;
		
		if (m_piece_length == 0)
			m_piece_length = 256 * 1024;

		int num_pieces = static_cast<int>(
			(m_total_size + m_piece_length - 1) / m_piece_length);
		int old_num_pieces = static_cast<int>(m_piece_hash.size());

		m_piece_hash.resize(num_pieces);
		if (num_pieces > old_num_pieces)
			std::for_each(m_piece_hash.begin() + old_num_pieces
				, m_piece_hash.end(), boost::bind(&sha1_hash::clear, _1));
	}

	void torrent_info::add_url_seed(std::string const& url)
	{
		m_url_seeds.push_back(url);
	}

	void torrent_info::set_comment(char const* str)
	{
		m_comment = str;
	}

	void torrent_info::set_creator(char const* str)
	{
		m_created_by = str;
	}

	entry torrent_info::create_info_metadata() const
	{
		namespace fs = boost::filesystem;

		
		assert(!m_files.empty());
	
		entry info(m_extra_info);

		if (!info.find_key("name"))
			info["name"] = m_name;

		if (!m_multifile)
		{
			info["length"] = m_files.front().size;
		}
		else
		{
			if (!info.find_key("files"))
			{
				entry& files = info["files"];

				for (std::vector<file_entry>::const_iterator i = m_files.begin();
					i != m_files.end(); ++i)
				{
					files.list().push_back(entry());
					entry& file_e = files.list().back();
					file_e["length"] = i->size;
					entry& path_e = file_e["path"];

					fs::path const* file_path;
					if (i->orig_path) file_path = &(*i->orig_path);
					else file_path = &i->path;
					assert(file_path->has_branch_path());
					assert(*file_path->begin() == m_name);

					for (fs::path::iterator j = boost::next(file_path->begin());
						j != file_path->end(); ++j)
					{
						path_e.list().push_back(entry(*j));
					}
				}
			}
		}

		info["piece length"] = piece_length();
		entry& pieces = info["pieces"];

		std::string& p = pieces.string();

		for (std::vector<sha1_hash>::const_iterator i = m_piece_hash.begin();
			i != m_piece_hash.end(); ++i)
		{
			p.append((char*)i->begin(), (char*)i->end());
		}

		return info;
	}

	entry torrent_info::create_torrent() const
	{
		assert(m_piece_length > 0);

		using namespace boost::gregorian;
		using namespace boost::posix_time;

		namespace fs = boost::filesystem;

		if ((m_urls.empty() && m_nodes.empty()) || m_files.empty())
		{
			
			
			return entry();
		}

		entry dict;

		if (m_private) dict["private"] = 1;

		if (!m_urls.empty())
			dict["announce"] = m_urls.front().url;
		
		if (!m_nodes.empty())
		{
			entry& nodes = dict["nodes"];
			entry::list_type& nodes_list = nodes.list();
			for (nodes_t::const_iterator i = m_nodes.begin()
				, end(m_nodes.end()); i != end; ++i)
			{
				entry::list_type node;
				node.push_back(entry(i->first));
				node.push_back(entry(i->second));
				nodes_list.push_back(entry(node));
			}
		}

		if (m_urls.size() > 1)
		{
			entry trackers(entry::list_t);
			entry tier(entry::list_t);
			int current_tier = m_urls.front().tier;
			for (std::vector<announce_entry>::const_iterator i = m_urls.begin();
				i != m_urls.end(); ++i)
			{
				if (i->tier != current_tier)
				{
					current_tier = i->tier;
					trackers.list().push_back(tier);
					tier.list().clear();
				}
				tier.list().push_back(entry(i->url));
			}
			trackers.list().push_back(tier);
			dict["announce-list"] = trackers;
		}

		if (!m_comment.empty())
			dict["comment"] = m_comment;

		dict["creation date"] =
			(m_creation_date - ptime(date(1970, Jan, 1))).total_seconds();

		if (!m_created_by.empty())
			dict["created by"] = m_created_by;
			
		if (!m_url_seeds.empty())
		{
			if (m_url_seeds.size() == 1)
			{
				dict["url-list"] = m_url_seeds.front();
			}
			else
			{
				entry& list = dict["url-list"];
				for (std::vector<std::string>::const_iterator i
					= m_url_seeds.begin(); i != m_url_seeds.end(); ++i)
				{
					list.list().push_back(entry(*i));
				}
			}
		}

		dict["info"] = create_info_metadata();

		entry const& info_section = dict["info"];
		std::vector<char> buf;
		bencode(std::back_inserter(buf), info_section);
		m_info_hash = hasher(&buf[0], buf.size()).final();

		return dict;
	}

	void torrent_info::set_hash(int index, const sha1_hash& h)
	{
		assert(index >= 0);
		assert(index < (int)m_piece_hash.size());
		m_piece_hash[index] = h;
	}

	void torrent_info::convert_file_names()
	{
		assert(false);
	}

	void torrent_info::print(std::ostream& os) const
	{
		os << "trackers:\n";
		for (std::vector<announce_entry>::const_iterator i = trackers().begin();
			i != trackers().end(); ++i)
		{
			os << i->tier << ": " << i->url << "\n";
		}
		if (!m_comment.empty())
			os << "comment: " << m_comment << "\n";
		if (m_creation_date != ptime(date(not_a_date_time)))
			os << "creation date: " << to_simple_string(m_creation_date) << "\n";
		os << "private: " << (m_private?"yes":"no") << "\n";
		os << "number of pieces: " << num_pieces() << "\n";
		os << "piece length: " << piece_length() << "\n";
		os << "files:\n";
		for (file_iterator i = begin_files(); i != end_files(); ++i)
			os << "  " << std::setw(11) << i->size << "  " << i->path.string() << "\n";
	}

	size_type torrent_info::piece_size(int index) const
	{
		assert(index >= 0 && index < num_pieces());
		if (index == num_pieces()-1)
		{
			size_type size = total_size()
				- (num_pieces() - 1) * piece_length();
			assert(size > 0);
			assert(size <= piece_length());
			return size;
		}
		else
			return piece_length();
	}

	void torrent_info::add_node(std::pair<std::string, int> const& node)
	{
		m_nodes.push_back(node);
	}

	std::vector<file_slice> torrent_info::map_block(int piece, size_type offset
		, int size) const
	{
		assert(num_files() > 0);
		std::vector<file_slice> ret;

		size_type start = piece * (size_type)m_piece_length + offset;
		assert(start + size <= m_total_size);

		
		
		size_type file_offset = start;
		std::vector<file_entry>::const_iterator file_iter;

		int counter = 0;
		for (file_iter = begin_files();; ++counter, ++file_iter)
		{
			assert(file_iter != end_files());
			if (file_offset < file_iter->size)
			{
				file_slice f;
				f.file_index = counter;
				f.offset = file_offset;
				f.size = (std::min)(file_iter->size - file_offset, (size_type)size);
				size -= f.size;
				file_offset += f.size;
				ret.push_back(f);
			}
			
			assert(size >= 0);
			if (size <= 0) break;

			file_offset -= file_iter->size;
		}
		return ret;
	}
	
	peer_request torrent_info::map_file(int file_index, size_type file_offset
		, int size) const
	{
		assert(file_index < (int)m_files.size());
		assert(file_index >= 0);
		size_type offset = file_offset + m_files[file_index].offset;

		peer_request ret;
		ret.piece = offset / piece_length();
		ret.start = offset - ret.piece * piece_length();
		ret.length = size;
		return ret;
	}

}
