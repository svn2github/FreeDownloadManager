/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#include <ctime>
#include <iterator>
#include <algorithm>
#include <set>
#include <functional>

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/ref.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/bind.hpp>
#include <boost/version.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "libtorrent/storage.hpp"
#include "libtorrent/torrent.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/peer_id.hpp"
#include "libtorrent/file.hpp"
#include "libtorrent/invariant_check.hpp"
#include "libtorrent/file_pool.hpp"
#include "libtorrent/aux_/session_impl.hpp"

#ifndef NDEBUG
#include <ios>
#include <iostream>
#include <iomanip>
#include <cstdio>
#endif

#if defined(_WIN32) && defined(UNICODE)

#include <windows.h>
#include <boost/filesystem/exception.hpp>
#include "libtorrent/utf8.hpp"

namespace libtorrent
{
	std::wstring safe_convert(std::string const& s)
	{
		try
		{
			return libtorrent::utf8_wchar(s);
		}
		catch (std::exception)
		{
			std::wstring ret;
			const char* end = &s[0] + s.size();
			for (const char* i = &s[0]; i < end;)
			{
				wchar_t c = '.';
				int result = std::mbtowc(&c, i, end - i);
				if (result > 0) i += result;
				else ++i;
				ret += c;
			}
			return ret;
		}
	}
}

namespace
{
	using libtorrent::safe_convert;
	using namespace boost::filesystem;
	
	
	bool create_directories_win(const path& ph)
	{
		if (ph.empty() || exists(ph))
		{
			if ( !ph.empty() && !is_directory(ph) )
				boost::throw_exception( filesystem_error(
					"boost::filesystem::create_directories",
					ph, "path exists and is not a directory",
					not_directory_error ) );
			return false;
		}

		
		create_directories_win(ph.branch_path());
		
		std::wstring wph(safe_convert(ph.native_directory_string()));
		CreateDirectory(wph.c_str(), 0);
		return true;
	}

	bool exists_win( const path & ph )
	{
		std::wstring wpath(safe_convert(ph.string()));
		if(::GetFileAttributes( wpath.c_str() ) == 0xFFFFFFFF)
		{
			UINT err = ::GetLastError();
			if((err == ERROR_FILE_NOT_FOUND)
				|| (err == ERROR_INVALID_PARAMETER)
				|| (err == ERROR_NOT_READY)
				|| (err == ERROR_PATH_NOT_FOUND)
				|| (err == ERROR_INVALID_NAME)
				|| (err == ERROR_BAD_NETPATH ))
			return false; 
			
			
			return true;
		}
		return true;
	}

	boost::intmax_t file_size_win( const path & ph )
	{
		std::wstring wpath(safe_convert(ph.string()));
		
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if ( !::GetFileAttributesExW( wpath.c_str(),
					::GetFileExInfoStandard, &fad ) )
			boost::throw_exception( filesystem_error(
				"boost::filesystem::file_size",
				ph, detail::system_error_code() ) );
		if ( (fad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0 )
			boost::throw_exception( filesystem_error(
				"boost::filesystem::file_size",
				ph, "invalid: is a directory",
				is_directory_error ) ); 
		return (static_cast<boost::intmax_t>(fad.nFileSizeHigh)
			<< (sizeof(fad.nFileSizeLow)*8))
			+ fad.nFileSizeLow;
	}
	
	std::time_t last_write_time_win( const path & ph )
	{
		struct _stat path_stat;
		std::wstring wph(safe_convert(ph.native_file_string()));
		if ( ::_wstat( wph.c_str(), &path_stat ) != 0 )
			boost::throw_exception( filesystem_error(
			"boost::filesystem::last_write_time",
			ph, detail::system_error_code() ) );
		return path_stat.st_mtime;
	}

	void rename_win( const path & old_path,
		const path & new_path )
	{
		std::wstring wold_path(safe_convert(old_path.string()));
		std::wstring wnew_path(safe_convert(new_path.string()));
		if ( !::MoveFile( wold_path.c_str(), wnew_path.c_str() ) )
		boost::throw_exception( filesystem_error(
			"boost::filesystem::rename",
			old_path, new_path, detail::system_error_code() ) );
	}

} 

#endif

#if BOOST_VERSION < 103200
bool operator<(boost::filesystem::path const& lhs
	, boost::filesystem::path const& rhs)
{
	return lhs.string() < rhs.string();
}
#endif

using namespace boost::filesystem;
namespace pt = boost::posix_time;
using boost::bind;
using namespace ::boost::multi_index;
using boost::multi_index::multi_index_container;

namespace
{
	using namespace libtorrent;

	void print_to_log(const std::string& s)
	{
		static std::ofstream log("log.txt");
		log << s;
		log.flush();
	}

}

namespace libtorrent
{

	std::vector<std::pair<size_type, std::time_t> > get_filesizes(
		torrent_info const& t, path p)
	{
		p = complete(p);
		std::vector<std::pair<size_type, std::time_t> > sizes;
		for (torrent_info::file_iterator i = t.begin_files();
			i != t.end_files(); ++i)
		{
			size_type size = 0;
			std::time_t time = 0;
			try
			{
				path f = p / i->path;
#if defined(_WIN32) && defined(UNICODE)
				size = file_size_win(f);
				time = last_write_time_win(f);
#else
				size = file_size(f);
				time = last_write_time(f);
#endif
			}
			catch (std::exception&) {}
			sizes.push_back(std::make_pair(size, time));
		}
		return sizes;
	}

	bool match_filesizes(
		torrent_info const& t
		, path p
		, std::vector<std::pair<size_type, std::time_t> > const& sizes
		, std::string* error)
	{
		if ((int)sizes.size() != t.num_files())
		{
			if (error) *error = "mismatching number of files";
			return false;
		}
		p = complete(p);

		std::vector<std::pair<size_type, std::time_t> >::const_iterator s
			= sizes.begin();
		for (torrent_info::file_iterator i = t.begin_files();
			i != t.end_files(); ++i, ++s)
		{
			size_type size = 0;
			std::time_t time = 0;
			try
			{
				path f = p / i->path;
#if defined(_WIN32) && defined(UNICODE)
				size = file_size_win(f);
				time = last_write_time_win(f);
#else
				size = file_size(f);
				time = last_write_time(f);
#endif
			}
			catch (std::exception&) {}
			if (size != s->first)
			{
				if (error) *error = "filesize mismatch for file '"
					+ i->path.native_file_string()
					+ "', expected to be " + boost::lexical_cast<std::string>(s->first)
					+ " bytes";
				return false;
			}
			if (time != s->second)
			{
				if (error) *error = "timestamp mismatch for file '"
					+ i->path.native_file_string()
					+ "', expected to have modification date "
					+ boost::lexical_cast<std::string>(s->second);
				return false;
			}
		}
		return true;
	}

	struct thread_safe_storage
	{
		thread_safe_storage(std::size_t n)
			: slots(n, false)
		{}

		boost::mutex mutex;
		boost::condition condition;
		std::vector<bool> slots;
	};

	struct slot_lock
	{
		slot_lock(thread_safe_storage& s, int slot_)
			: storage_(s)
			, slot(slot_)
		{
			assert(slot_>=0 && slot_ < (int)s.slots.size());
			boost::mutex::scoped_lock lock(storage_.mutex);

			while (storage_.slots[slot])
				storage_.condition.wait(lock);
			storage_.slots[slot] = true;
		}

		~slot_lock()
		{
			storage_.slots[slot] = false;
			storage_.condition.notify_all();
		}

		thread_safe_storage& storage_;
		int slot;
	};

	class storage::impl : public thread_safe_storage, boost::noncopyable
	{
	public:
		impl(torrent_info const& info, path const& path, file_pool& fp)
			: thread_safe_storage(info.num_pieces())
			, info(info)
			, files(fp)
		{
			save_path = complete(path);
			assert(save_path.is_complete());
		}

		impl(impl const& x)
			: thread_safe_storage(x.info.num_pieces())
			, info(x.info)
			, save_path(x.save_path)
			, files(x.files)
		{}

		~impl()
		{
			files.release(this);
		}

		torrent_info const& info;
		path save_path;
		
		
		
		file_pool& files;
	};

	storage::storage(torrent_info const& info, path const& path
		, file_pool& fp)
		: m_pimpl(new impl(info, path, fp))
	{
		assert(info.begin_files() != info.end_files());
	}

	void storage::release_files()
	{
		m_pimpl->files.release(m_pimpl.get());
	}

	void storage::swap(storage& other)
	{
		m_pimpl.swap(other.m_pimpl);
	}

	
	bool storage::move_storage(path save_path)
	{
		path old_path;
		path new_path;

		save_path = complete(save_path);

#if defined(_WIN32) && defined(UNICODE)
		std::wstring wsave_path(safe_convert(save_path.native_file_string()));
		if (!exists_win(save_path))
		{
			CreateDirectory(wsave_path.c_str(), 0);
		}
		else if ((GetFileAttributes(wsave_path.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			return false;
		}
#else
		if(!exists(save_path))
			create_directory(save_path);
		else if(!is_directory(save_path))
			return false;
#endif

		m_pimpl->files.release(m_pimpl.get());

		old_path = m_pimpl->save_path / m_pimpl->info.name();
		new_path = save_path / m_pimpl->info.name();

		try
		{
#if defined(_WIN32) && defined(UNICODE)
			rename_win(old_path, new_path);
#else
			rename(old_path, new_path);
#endif
			m_pimpl->save_path = save_path;
			return true;
		}
		catch (std::exception&) {}
		return false;
	}

#ifndef NDEBUG

	void storage::shuffle()
	{
		int num_pieces = m_pimpl->info.num_pieces();

		std::vector<int> pieces(num_pieces);
		for (std::vector<int>::iterator i = pieces.begin();
			i != pieces.end();
			++i)
		{
			*i = static_cast<int>(i - pieces.begin());
		}
		std::srand((unsigned int)std::time(0));
		std::vector<int> targets(pieces);
		std::random_shuffle(pieces.begin(), pieces.end());
		std::random_shuffle(targets.begin(), targets.end());

		for (int i = 0; i < (std::max)(num_pieces / 50, 1); ++i)
		{
			const int slot_index = targets[i];
			const int piece_index = pieces[i];
			const int slot_size =static_cast<int>(m_pimpl->info.piece_size(slot_index));
			std::vector<char> buf(slot_size);
			read(&buf[0], piece_index, 0, slot_size);
			write(&buf[0], slot_index, 0, slot_size);
		}
	}

#endif

	size_type storage::read(
		char* buf
		, int slot
		, int offset
		, int size)
	{
		assert(buf != 0);
		assert(slot >= 0 && slot < m_pimpl->info.num_pieces());
		assert(offset >= 0);
		assert(offset < m_pimpl->info.piece_size(slot));
		assert(size > 0);

		slot_lock lock(*m_pimpl, slot);

#ifndef NDEBUG
		std::vector<file_slice> slices
			= m_pimpl->info.map_block(slot, offset, size);
		assert(!slices.empty());
#endif

		size_type start = slot * (size_type)m_pimpl->info.piece_length() + offset;
		assert(start + size <= m_pimpl->info.total_size());

		
		size_type file_offset = start;
		std::vector<file_entry>::const_iterator file_iter;

		for (file_iter = m_pimpl->info.begin_files();;)
		{
			if (file_offset < file_iter->size)
				break;

			file_offset -= file_iter->size;
			++file_iter;
		}

		boost::shared_ptr<file> in(m_pimpl->files.open_file(
			m_pimpl.get()
			, m_pimpl->save_path / file_iter->path
			, file::in));

		assert(file_offset < file_iter->size);

		assert(slices[0].offset == file_offset);

		size_type new_pos = in->seek(file_offset);
		if (new_pos != file_offset)
		{
			
			throw file_error("slot has no storage");
		}

#ifndef NDEBUG
		size_type in_tell = in->tell();
		assert(in_tell == file_offset);
#endif

		int left_to_read = size;
		int slot_size = static_cast<int>(m_pimpl->info.piece_size(slot));

		if (offset + left_to_read > slot_size)
			left_to_read = slot_size - offset;

		assert(left_to_read >= 0);

		size_type result = left_to_read;
		int buf_pos = 0;

#ifndef NDEBUG
		int counter = 0;
#endif

		while (left_to_read > 0)
		{
			int read_bytes = left_to_read;
			if (file_offset + read_bytes > file_iter->size)
				read_bytes = static_cast<int>(file_iter->size - file_offset);

			if (read_bytes > 0)
			{
#ifndef NDEBUG
				assert(int(slices.size()) > counter);
				size_type slice_size = slices[counter].size;
				assert(slice_size == read_bytes);
				assert(m_pimpl->info.file_at(slices[counter].file_index).path
					== file_iter->path);
#endif

				size_type actual_read = in->read(buf + buf_pos, read_bytes);

				if (read_bytes != actual_read)
				{
					
					throw file_error("slot has no storage");
				}

				left_to_read -= read_bytes;
				buf_pos += read_bytes;
				assert(buf_pos >= 0);
				file_offset += read_bytes;
			}

			if (left_to_read > 0)
			{
				++file_iter;
#ifndef NDEBUG
				
				
				if (read_bytes > 0) ++counter;
#endif
				path path = m_pimpl->save_path / file_iter->path;

				file_offset = 0;
				in = m_pimpl->files.open_file(
					m_pimpl.get()
					, path, file::in);
				in->seek(0);
			}
		}

		return result;
	}

	
	void storage::write(
		const char* buf
		, int slot
		, int offset
		, int size)
	{
		assert(buf != 0);
		assert(slot >= 0);
		assert(slot < m_pimpl->info.num_pieces());
		assert(offset >= 0);
		assert(size > 0);

		slot_lock lock(*m_pimpl, slot);
		
#ifndef NDEBUG
		std::vector<file_slice> slices
			= m_pimpl->info.map_block(slot, offset, size);
		assert(!slices.empty());
#endif

		size_type start = slot * (size_type)m_pimpl->info.piece_length() + offset;

		
		size_type file_offset = start;
		std::vector<file_entry>::const_iterator file_iter;

		for (file_iter = m_pimpl->info.begin_files();;)
		{
			if (file_offset < file_iter->size)
				break;

			file_offset -= file_iter->size;
			++file_iter;
			assert(file_iter != m_pimpl->info.end_files());
		}

		path p(m_pimpl->save_path / file_iter->path);
		boost::shared_ptr<file> out = m_pimpl->files.open_file(
			m_pimpl.get()
			, p, file::out | file::in);

		assert(file_offset < file_iter->size);
		assert(slices[0].offset == file_offset);

		size_type pos = out->seek(file_offset);

		if (pos != file_offset)
		{
			std::stringstream s;
			s << "no storage for slot " << slot;
			throw file_error(s.str());
		}

		int left_to_write = size;
		int slot_size = static_cast<int>(m_pimpl->info.piece_size(slot));

		if (offset + left_to_write > slot_size)
			left_to_write = slot_size - offset;

		assert(left_to_write >= 0);

		int buf_pos = 0;
#ifndef NDEBUG
		int counter = 0;
#endif
		while (left_to_write > 0)
		{
			int write_bytes = left_to_write;
			if (file_offset + write_bytes > file_iter->size)
			{
				assert(file_iter->size >= file_offset);
				write_bytes = static_cast<int>(file_iter->size - file_offset);
			}

			if (write_bytes > 0)
			{
				assert(int(slices.size()) > counter);
				assert(slices[counter].size == write_bytes);
				assert(m_pimpl->info.file_at(slices[counter].file_index).path
					== file_iter->path);

				assert(buf_pos >= 0);
				assert(write_bytes >= 0);
				size_type written = out->write(buf + buf_pos, write_bytes);

				if (written != write_bytes)
				{
					std::stringstream s;
					s << "no storage for slot " << slot;
					throw file_error(s.str());
				}

				left_to_write -= write_bytes;
				buf_pos += write_bytes;
				assert(buf_pos >= 0);
				file_offset += write_bytes;
				assert(file_offset <= file_iter->size);
			}

			if (left_to_write > 0)
			{
			#ifndef NDEBUG
				if (write_bytes > 0) ++counter;
			#endif
				++file_iter;

				assert(file_iter != m_pimpl->info.end_files());
 				path p = m_pimpl->save_path / file_iter->path;
				file_offset = 0;
				out = m_pimpl->files.open_file(
					m_pimpl.get()
					, p, file::out | file::in);

				out->seek(0);
			}
		}
	}    

	

	class piece_manager::impl
	{
	friend class invariant_access;
	public:

		impl(
			torrent_info const& info
			, path const& path
			, file_pool& fp);

		bool check_fastresume(
			aux::piece_checker_data& d
			, std::vector<bool>& pieces
			, int& num_pieces
			, bool compact_mode);

		std::pair<bool, float> check_files(
			std::vector<bool>& pieces
			, int& num_pieces, boost::recursive_mutex& mutex);

		void release_files();

		void allocate_slots(int num_slots);
		void mark_failed(int index);
		unsigned long piece_crc(
			int slot_index
			, int block_size
			, const std::bitset<256>& bitmask);

		int slot_for_piece(int piece_index) const;

		size_type read(
			char* buf
			, int piece_index
			, int offset
			, int size);

		void write(
			const char* buf
			, int piece_index
			, int offset
			, int size);

		path const& save_path() const
		{ return m_save_path; }

		bool move_storage(path save_path)
		{
			if (m_storage.move_storage(save_path))
			{
				m_save_path = complete(save_path);
				return true;
			}
			return false;
		}

		void export_piece_map(std::vector<int>& p) const;
		
		
		
		
		int identify_data(
			const std::vector<char>& piece_data
			, int current_slot
			, std::vector<bool>& have_pieces
			, int& num_pieces
			, const std::multimap<sha1_hash, int>& hash_to_piece
			, boost::recursive_mutex& mutex);

		int allocate_slot_for_piece(int piece_index);
#ifndef NDEBUG
		void check_invariant() const;
#ifdef TORRENT_STORAGE_DEBUG
		void debug_log() const;
#endif
#endif
		storage m_storage;

		
		
		
		bool m_compact_mode;

		
		
		
		
		bool m_fill_mode;

		
		std::vector<bool> m_have_piece;

		torrent_info const& m_info;

		
		std::vector<int> m_unallocated_slots;
		
		std::vector<int> m_free_slots;

		enum
		{
			has_no_slot = -3 
		};

		
		
		std::vector<int> m_piece_to_slot;

		enum
		{
			unallocated = -1, 
			unassigned = -2   
		};

		
		
		std::vector<int> m_slot_to_piece;

		path m_save_path;

		mutable boost::recursive_mutex m_mutex;

		bool m_allocating;
		boost::mutex m_allocating_monitor;
		boost::condition m_allocating_condition;

		

		enum {
			
			state_none,
			
			state_finished,
			
			state_create_files,
			
			state_full_check,
			
			state_allocating
		} m_state;
		int m_current_slot;
		
		std::vector<char> m_piece_data;
		
		
		
		
		std::multimap<sha1_hash, int> m_hash_to_piece;
		
		
		
		
		
		std::vector<char> m_scratch_buffer;
	};

	piece_manager::impl::impl(
		torrent_info const& info
		, path const& save_path
		, file_pool& fp)
		: m_storage(info, save_path, fp)
		, m_compact_mode(false)
		, m_fill_mode(true)
		, m_info(info)
		, m_save_path(complete(save_path))
		, m_allocating(false)
	{
		assert(m_save_path.is_complete());
	}

	piece_manager::piece_manager(
		torrent_info const& info
		, path const& save_path
		, file_pool& fp)
		: m_pimpl(new impl(info, save_path, fp))
	{
	}

	piece_manager::~piece_manager()
	{
	}

	void piece_manager::release_files()
	{
		m_pimpl->release_files();
	}

	void piece_manager::impl::release_files()
	{
		m_storage.release_files();
	}

	void piece_manager::impl::export_piece_map(
			std::vector<int>& p) const
	{
		
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		

		INVARIANT_CHECK;

		p.clear();
		std::vector<int>::const_reverse_iterator last; 
		for (last = m_slot_to_piece.rbegin();
			last != m_slot_to_piece.rend(); ++last)
		{
			if (*last != unallocated) break;
		}

		for (std::vector<int>::const_iterator i =
			m_slot_to_piece.begin();
			i != last.base(); ++i)
		{
			p.push_back(*i);
		}
	}

	void piece_manager::export_piece_map(
			std::vector<int>& p) const
	{
		m_pimpl->export_piece_map(p);
	}

	void piece_manager::impl::mark_failed(int piece_index)
	{
		
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		

		INVARIANT_CHECK;

		assert(piece_index >= 0 && piece_index < (int)m_piece_to_slot.size());
		assert(m_piece_to_slot[piece_index] >= 0);

		int slot_index = m_piece_to_slot[piece_index];

		assert(slot_index >= 0);

		m_slot_to_piece[slot_index] = unassigned;
		m_piece_to_slot[piece_index] = has_no_slot;
		m_free_slots.push_back(slot_index);
	}

	void piece_manager::mark_failed(int index)
	{
		m_pimpl->mark_failed(index);
	}

	bool piece_manager::is_allocating() const
	{
		return m_pimpl->m_state
			== impl::state_allocating;
	}
	
	int piece_manager::slot_for_piece(int piece_index) const
	{
		return m_pimpl->slot_for_piece(piece_index);
	}

	int piece_manager::impl::slot_for_piece(int piece_index) const
	{
		assert(piece_index >= 0 && piece_index < m_info.num_pieces());
		return m_piece_to_slot[piece_index];
	}

	unsigned long piece_manager::piece_crc(
		int index
		, int block_size
		, const std::bitset<256>& bitmask)
	{
		return m_pimpl->piece_crc(index, block_size, bitmask);
	}

	unsigned long piece_manager::impl::piece_crc(
		int slot_index
		, int block_size
		, const std::bitset<256>& bitmask)
	{
		assert(slot_index >= 0);
		assert(slot_index < m_info.num_pieces());
		assert(block_size > 0);

		adler32_crc crc;
		std::vector<char> buf(block_size);
		int num_blocks = static_cast<int>(m_info.piece_size(slot_index)) / block_size;
		int last_block_size = static_cast<int>(m_info.piece_size(slot_index)) % block_size;
		if (last_block_size == 0) last_block_size = block_size;

		for (int i = 0; i < num_blocks-1; ++i)
		{
			if (!bitmask[i]) continue;
			m_storage.read(
				&buf[0]
				, slot_index
				, i * block_size
				, block_size);
			crc.update(&buf[0], block_size);
		}
		if (bitmask[num_blocks - 1])
		{
			m_storage.read(
				&buf[0]
				, slot_index
				, block_size * (num_blocks - 1)
				, last_block_size);
			crc.update(&buf[0], last_block_size);
		}
		return crc.final();
	}

	size_type piece_manager::impl::read(
		char* buf
	  , int piece_index
	  , int offset
	  , int size)
	{
		assert(buf);
		assert(offset >= 0);
		assert(size > 0);
		assert(piece_index >= 0 && piece_index < (int)m_piece_to_slot.size());
		assert(m_piece_to_slot[piece_index] >= 0 && m_piece_to_slot[piece_index] < (int)m_slot_to_piece.size());
		int slot = m_piece_to_slot[piece_index];
		assert(slot >= 0 && slot < (int)m_slot_to_piece.size());
		return m_storage.read(buf, slot, offset, size);
	}

	size_type piece_manager::read(
		char* buf
	  , int piece_index
	  , int offset
	  , int size)
	{
		return m_pimpl->read(buf, piece_index, offset, size);
	}

	void piece_manager::impl::write(
		const char* buf
	  , int piece_index
	  , int offset
	  , int size)
	{
		assert(buf);
		assert(offset >= 0);
		assert(size > 0);
		assert(piece_index >= 0 && piece_index < (int)m_piece_to_slot.size());
		int slot = allocate_slot_for_piece(piece_index);
		assert(slot >= 0 && slot < (int)m_slot_to_piece.size());
		m_storage.write(buf, slot, offset, size);
	}

	void piece_manager::write(
		const char* buf
	  , int piece_index
	  , int offset
	  , int size)
	{
		m_pimpl->write(buf, piece_index, offset, size);
	}

	int piece_manager::impl::identify_data(
		const std::vector<char>& piece_data
		, int current_slot
		, std::vector<bool>& have_pieces
		, int& num_pieces
		, const std::multimap<sha1_hash, int>& hash_to_piece
		, boost::recursive_mutex& mutex)
	{ 

		assert((int)have_pieces.size() == m_info.num_pieces());

		const int piece_size = static_cast<int>(m_info.piece_length());
		const int last_piece_size = static_cast<int>(m_info.piece_size(
			m_info.num_pieces() - 1));

		assert((int)piece_data.size() >= last_piece_size);

		
		
		
		hasher small_digest;
		small_digest.update(&piece_data[0], last_piece_size);
		hasher large_digest(small_digest);
		assert(piece_size - last_piece_size >= 0);
		if (piece_size - last_piece_size > 0)
		{
			large_digest.update(
				&piece_data[last_piece_size]
				, piece_size - last_piece_size);
		}
		sha1_hash large_hash = large_digest.final();
		sha1_hash small_hash = small_digest.final();

		typedef std::multimap<sha1_hash, int>::const_iterator map_iter;
		map_iter begin1;
		map_iter end1;
		map_iter begin2;
		map_iter end2;

		
		boost::tie(begin1, end1) = hash_to_piece.equal_range(small_hash);
		boost::tie(begin2, end2) = hash_to_piece.equal_range(large_hash);

		
		std::vector<int> matching_pieces;
		for (map_iter i = begin1; i != end1; ++i)
			matching_pieces.push_back(i->second);
		for (map_iter i = begin2; i != end2; ++i)
			matching_pieces.push_back(i->second);

		
		if (matching_pieces.empty())
			return unassigned;

		
		
		

		if (std::find(
			matching_pieces.begin()
			, matching_pieces.end()
			, current_slot) != matching_pieces.end())
		{
			const int piece_index = current_slot;

			
			boost::recursive_mutex::scoped_lock l(mutex);

			if (have_pieces[piece_index])
			{
				
				
				int other_slot = m_piece_to_slot[piece_index];
				assert(other_slot >= 0);

				
				
				int other_piece = -1;
				for (std::vector<int>::iterator i = matching_pieces.begin();
					i != matching_pieces.end(); ++i)
				{
					if (have_pieces[*i] || *i == piece_index) continue;
					other_piece = *i;
					break;
				}
				if (other_piece >= 0)
				{
					
					assert(have_pieces[other_piece] == false);
					have_pieces[other_piece] = true;
					m_slot_to_piece[other_slot] = other_piece;
					m_piece_to_slot[other_piece] = other_slot;
					++num_pieces;
				}
				else
				{
					
					
					
					
					
					m_slot_to_piece[other_slot] = unassigned;
					m_free_slots.push_back(other_slot);
				}
				assert(m_piece_to_slot[piece_index] != current_slot);
				assert(m_piece_to_slot[piece_index] >= 0);
				m_piece_to_slot[piece_index] = has_no_slot;
#ifndef NDEBUG
				
				have_pieces[piece_index] = false;
#endif
			}
			else
			{
				++num_pieces;
			}
			
			assert(have_pieces[piece_index] == false);
			assert(m_piece_to_slot[piece_index] == has_no_slot);
			have_pieces[piece_index] = true;

			return piece_index;
		}

		
		
		int free_piece = unassigned;
		for (std::vector<int>::iterator i = matching_pieces.begin();
			i != matching_pieces.end(); ++i)
		{
			if (have_pieces[*i]) continue;
			free_piece = *i;
			break;
		}

		if (free_piece >= 0)
		{
			
			boost::recursive_mutex::scoped_lock l(mutex);

			assert(have_pieces[free_piece] == false);
			assert(m_piece_to_slot[free_piece] == has_no_slot);
			have_pieces[free_piece] = true;
			++num_pieces;

			return free_piece;
		}
		else
		{
			assert(free_piece == unassigned);
			return unassigned;
		}
	}

	
	
	
	
	bool piece_manager::impl::check_fastresume(
		aux::piece_checker_data& data
		, std::vector<bool>& pieces
		, int& num_pieces, bool compact_mode)
	{
		assert(m_info.piece_length() > 0);
		
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		

		INVARIANT_CHECK;

		m_compact_mode = compact_mode;

		
		
		
		 

		m_piece_to_slot.resize(m_info.num_pieces(), has_no_slot);
		m_slot_to_piece.resize(m_info.num_pieces(), unallocated);
		m_free_slots.clear();
		m_unallocated_slots.clear();

		pieces.clear();
		pieces.resize(m_info.num_pieces(), false);
		num_pieces = 0;

		
		
		if (!data.piece_map.empty()
			&& data.piece_map.size() <= m_slot_to_piece.size())
		{
			for (int i = 0; i < (int)data.piece_map.size(); ++i)
			{
				m_slot_to_piece[i] = data.piece_map[i];
				if (data.piece_map[i] >= 0)
				{
					m_piece_to_slot[data.piece_map[i]] = i;
					int found_piece = data.piece_map[i];

					
					
					if (std::find_if(
						data.unfinished_pieces.begin()
						, data.unfinished_pieces.end()
						, piece_picker::has_index(found_piece))
						== data.unfinished_pieces.end())
					{
						++num_pieces;
						pieces[found_piece] = true;
					}
				}
				else if (data.piece_map[i] == unassigned)
				{
					m_free_slots.push_back(i);
				}
				else
				{
					assert(data.piece_map[i] == unallocated);
					m_unallocated_slots.push_back(i);
				}
			}

			m_unallocated_slots.reserve(int(pieces.size() - data.piece_map.size()));
			for (int i = (int)data.piece_map.size(); i < (int)pieces.size(); ++i)
			{
				m_unallocated_slots.push_back(i);
			}

			if (!m_compact_mode && !m_unallocated_slots.empty())
			{
				m_state = state_allocating;
				return false;
			}
			else
			{
				m_state = state_finished;
				return true;
			}
		}

		m_state = state_create_files;
		return false;
	}

	
	
	
	
	
	
	std::pair<bool, float> piece_manager::impl::check_files(
		std::vector<bool>& pieces, int& num_pieces, boost::recursive_mutex& mutex)
	{
		assert(num_pieces == std::count(pieces.begin(), pieces.end(), true));

		if (m_state == state_allocating)
		{
			if (m_compact_mode)
			{
				m_state = state_finished;
				return std::make_pair(true, 1.f);
			}
			
			if (m_unallocated_slots.empty())
			{
				m_state = state_finished;
				return std::make_pair(true, 1.f);
			}

			
			
			
			assert(!m_unallocated_slots.empty());
			allocate_slots(1);

			return std::make_pair(false, 1.f - (float)m_unallocated_slots.size()
				/ (float)m_slot_to_piece.size());
		}

		if (m_state == state_create_files)
		{
			
			path last_path;
			for (torrent_info::file_iterator file_iter = m_info.begin_files(),
				end_iter = m_info.end_files();  file_iter != end_iter; ++file_iter)
			{
				path dir = (m_save_path / file_iter->path).branch_path();

				
				
				if (dir == last_path
					&& file_iter->size == 0)
					file(m_save_path / file_iter->path, file::out);

				if (dir == last_path) continue;
				last_path = dir;

#if defined(_WIN32) && defined(UNICODE)
				if (!exists_win(last_path))
					create_directories_win(last_path);
#else
				if (!exists(last_path))
					create_directories(last_path);
#endif

				if (file_iter->size == 0)
					file(m_save_path / file_iter->path, file::out);
			}
			m_current_slot = 0;
			m_state = state_full_check;
			m_piece_data.resize(int(m_info.piece_length()));
			return std::make_pair(false, 0.f);
		}

		assert(m_state == state_full_check);

		
		
		

		try
		{

			m_storage.read(
				&m_piece_data[0]
				, m_current_slot
				, 0
				, int(m_info.piece_size(m_current_slot)));

			if (m_hash_to_piece.empty())
			{
				for (int i = 0; i < m_info.num_pieces(); ++i)
				{
					m_hash_to_piece.insert(std::make_pair(m_info.hash_for_piece(i), i));
				}
			}

			int piece_index = identify_data(
				m_piece_data
				, m_current_slot
				, pieces
				, num_pieces
				, m_hash_to_piece
				, mutex);

			assert(num_pieces == std::count(pieces.begin(), pieces.end(), true));
			assert(piece_index == unassigned || piece_index >= 0);

			const bool this_should_move = piece_index >= 0 && m_slot_to_piece[piece_index] != unallocated;
			const bool other_should_move = m_piece_to_slot[m_current_slot] != has_no_slot;

			
			
			
			

			
			
			
			
			
			
			

			
			
			
			

			
			
			
			
			

			

			
			if (this_should_move && !other_should_move)
			{
				assert(piece_index != m_current_slot);

				const int other_slot = piece_index;
				assert(other_slot >= 0);
				int other_piece = m_slot_to_piece[other_slot];

				m_slot_to_piece[other_slot] = piece_index;
				m_slot_to_piece[m_current_slot] = other_piece;
				m_piece_to_slot[piece_index] = piece_index;
				if (other_piece >= 0) m_piece_to_slot[other_piece] = m_current_slot;

				if (other_piece == unassigned)
				{
					std::vector<int>::iterator i =
						std::find(m_free_slots.begin(), m_free_slots.end(), other_slot);
					assert(i != m_free_slots.end());
					m_free_slots.erase(i);
					m_free_slots.push_back(m_current_slot);
				}

				const int slot1_size = static_cast<int>(m_info.piece_size(piece_index));
				const int slot2_size = other_piece >= 0 ? static_cast<int>(m_info.piece_size(other_piece)) : 0;
				std::vector<char> buf1(slot1_size);
				m_storage.read(&buf1[0], m_current_slot, 0, slot1_size);
				if (slot2_size > 0)
				{
					std::vector<char> buf2(slot2_size);
					m_storage.read(&buf2[0], piece_index, 0, slot2_size);
					m_storage.write(&buf2[0], m_current_slot, 0, slot2_size);
				}
				m_storage.write(&buf1[0], piece_index, 0, slot1_size);
				assert(m_slot_to_piece[m_current_slot] == unassigned
						|| m_piece_to_slot[m_slot_to_piece[m_current_slot]] == m_current_slot);
			}
			
			else if (!this_should_move && other_should_move)
			{
				assert(piece_index != m_current_slot);

				const int other_piece = m_current_slot;
				const int other_slot = m_piece_to_slot[other_piece];
				assert(other_slot >= 0);

				m_slot_to_piece[m_current_slot] = other_piece;
				m_slot_to_piece[other_slot] = piece_index;
				m_piece_to_slot[other_piece] = m_current_slot;
				if (piece_index >= 0) m_piece_to_slot[piece_index] = other_slot;

				if (piece_index == unassigned)
				{
					m_free_slots.push_back(other_slot);
				}

				const int slot1_size = static_cast<int>(m_info.piece_size(other_piece));
				const int slot2_size = piece_index >= 0 ? static_cast<int>(m_info.piece_size(piece_index)) : 0;
				std::vector<char> buf1(slot1_size);
				m_storage.read(&buf1[0], other_slot, 0, slot1_size);
				if (slot2_size > 0)
				{
					std::vector<char> buf2(slot2_size);
					m_storage.read(&buf2[0], m_current_slot, 0, slot2_size);
					m_storage.write(&buf2[0], other_slot, 0, slot2_size);
				}
				m_storage.write(&buf1[0], m_current_slot, 0, slot1_size);
				assert(m_slot_to_piece[m_current_slot] == unassigned
						|| m_piece_to_slot[m_slot_to_piece[m_current_slot]] == m_current_slot);
			}
			else if (this_should_move && other_should_move)
			{
				assert(piece_index != m_current_slot);
				assert(piece_index >= 0);

				const int piece1 = m_slot_to_piece[piece_index];
				const int piece2 = m_current_slot;
				const int slot1 = piece_index;
				const int slot2 = m_piece_to_slot[piece2];

				assert(slot1 >= 0);
				assert(slot2 >= 0);
				assert(piece2 >= 0);

				if (slot1 == slot2)
				{
					
					assert(piece1 >= 0);

					
					
					
					

					m_slot_to_piece[slot1] = piece_index;
					m_slot_to_piece[m_current_slot] = piece1;

					m_piece_to_slot[piece_index] = slot1;
					m_piece_to_slot[piece1] = m_current_slot;

					assert(piece1 == m_current_slot);
					assert(piece_index == slot1);

					const int slot1_size = static_cast<int>(m_info.piece_size(piece1));
					const int slot3_size = static_cast<int>(m_info.piece_size(piece_index));
					std::vector<char> buf1(static_cast<int>(slot1_size));
					std::vector<char> buf2(static_cast<int>(slot3_size));

					m_storage.read(&buf2[0], m_current_slot, 0, slot3_size);
					m_storage.read(&buf1[0], slot1, 0, slot1_size);
					m_storage.write(&buf1[0], m_current_slot, 0, slot1_size);
					m_storage.write(&buf2[0], slot1, 0, slot3_size);

					assert(m_slot_to_piece[m_current_slot] == unassigned
							|| m_piece_to_slot[m_slot_to_piece[m_current_slot]] == m_current_slot);
				}
				else
				{
					assert(slot1 != slot2);
					assert(piece1 != piece2);

					
					
					
					

					m_slot_to_piece[slot1] = piece_index;
					m_slot_to_piece[slot2] = piece1;
					m_slot_to_piece[m_current_slot] = piece2;

					m_piece_to_slot[piece_index] = slot1;
					m_piece_to_slot[m_current_slot] = piece2;
					if (piece1 >= 0) m_piece_to_slot[piece1] = slot2;

					if (piece1 == unassigned)
					{
						std::vector<int>::iterator i =
							std::find(m_free_slots.begin(), m_free_slots.end(), slot1);
						assert(i != m_free_slots.end());
						m_free_slots.erase(i);
						m_free_slots.push_back(slot2);
					}

					const int slot1_size = piece1 >= 0 ? static_cast<int>(m_info.piece_size(piece1)) : 0;
					const int slot2_size = static_cast<int>(m_info.piece_size(piece2));
					const int slot3_size = static_cast<int>(m_info.piece_size(piece_index));

					std::vector<char> buf1(static_cast<int>(m_info.piece_length()));
					std::vector<char> buf2(static_cast<int>(m_info.piece_length()));

					m_storage.read(&buf2[0], m_current_slot, 0, slot3_size);
					m_storage.read(&buf1[0], slot2, 0, slot2_size);
					m_storage.write(&buf1[0], m_current_slot, 0, slot2_size);
					if (slot1_size > 0)
					{
						m_storage.read(&buf1[0], slot1, 0, slot1_size);
						m_storage.write(&buf1[0], slot2, 0, slot1_size);
					}
					m_storage.write(&buf2[0], slot1, 0, slot3_size);
					assert(m_slot_to_piece[m_current_slot] == unassigned
						|| m_piece_to_slot[m_slot_to_piece[m_current_slot]] == m_current_slot);
				}
			}
			else
			{
				assert(m_piece_to_slot[m_current_slot] == has_no_slot || piece_index != m_current_slot);
				assert(m_slot_to_piece[m_current_slot] == unallocated);
				assert(piece_index == unassigned || m_piece_to_slot[piece_index] == has_no_slot);

				
				if (piece_index != unassigned)
					m_piece_to_slot[piece_index] = m_current_slot;
				else
					m_free_slots.push_back(m_current_slot);

				m_slot_to_piece[m_current_slot] = piece_index;

				assert(m_slot_to_piece[m_current_slot] == unassigned
						|| m_piece_to_slot[m_slot_to_piece[m_current_slot]] == m_current_slot);
			}
		}
		catch (file_error&)
		{
			
			size_type file_offset = 0;
			size_type current_offset = m_current_slot * m_info.piece_length();
			for (torrent_info::file_iterator i = m_info.begin_files();
					i != m_info.end_files(); ++i)
			{
				file_offset += i->size;
				if (file_offset > current_offset) break;
			}

			assert(file_offset > current_offset);
			int skip_blocks = static_cast<int>(
					(file_offset - current_offset + m_info.piece_length() - 1)
					/ m_info.piece_length());

			for (int i = m_current_slot; i < m_current_slot + skip_blocks; ++i)
			{
				assert(m_slot_to_piece[i] == unallocated);
				m_unallocated_slots.push_back(i);
			}

			
			m_current_slot += skip_blocks - 1;
		}
		++m_current_slot;

		if (m_current_slot >=  m_info.num_pieces())
		{
			assert(m_current_slot == m_info.num_pieces());

			
			std::vector<char>().swap(m_piece_data);
			std::multimap<sha1_hash, int>().swap(m_hash_to_piece);
			m_state = state_allocating;
			assert(num_pieces == std::count(pieces.begin(), pieces.end(), true));
			return std::make_pair(false, 1.f);
		}

		assert(num_pieces == std::count(pieces.begin(), pieces.end(), true));

		return std::make_pair(false, (float)m_current_slot / m_info.num_pieces());
	}

	bool piece_manager::check_fastresume(
		aux::piece_checker_data& d, std::vector<bool>& pieces
		, int& num_pieces, bool compact_mode)
	{
		return m_pimpl->check_fastresume(d, pieces, num_pieces, compact_mode);
	}

	std::pair<bool, float> piece_manager::check_files(
		std::vector<bool>& pieces
		, int& num_pieces
		, boost::recursive_mutex& mutex)
	{
		return m_pimpl->check_files(pieces, num_pieces, mutex);
	}

	int piece_manager::impl::allocate_slot_for_piece(int piece_index)
	{
		
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		  

		assert(piece_index >= 0);
		assert(piece_index < (int)m_piece_to_slot.size());
		assert(m_piece_to_slot.size() == m_slot_to_piece.size());

		int slot_index = m_piece_to_slot[piece_index];

		if (slot_index != has_no_slot)
		{
			assert(slot_index >= 0);
			assert(slot_index < (int)m_slot_to_piece.size());
			return slot_index;
		}

		if (m_free_slots.empty())
		{
			allocate_slots(1);
			assert(!m_free_slots.empty());
		}

		std::vector<int>::iterator iter(
			std::find(
				m_free_slots.begin()
				, m_free_slots.end()
				, piece_index));

		if (iter == m_free_slots.end())
		{
			assert(m_slot_to_piece[piece_index] != unassigned);
			assert(!m_free_slots.empty());
			iter = m_free_slots.end() - 1;

			
			
			if (*iter == m_info.num_pieces() - 1 && piece_index != *iter)
			{
				if (m_free_slots.size() == 1)
					allocate_slots(1);
				assert(m_free_slots.size() > 1);
				
				
				iter = m_free_slots.end() - 1;
			}
		}

		slot_index = *iter;
		m_free_slots.erase(iter);

		assert(m_slot_to_piece[slot_index] == unassigned);

		m_slot_to_piece[slot_index] = piece_index;
		m_piece_to_slot[piece_index] = slot_index;

		
		
		if (slot_index != piece_index
			&& m_slot_to_piece[piece_index] >= 0)
		{

#if !defined(NDEBUG) && defined(TORRENT_STORAGE_DEBUG)
			std::stringstream s;

			s << "there is another piece at our slot, swapping..";

			s << "\n   piece_index: ";
			s << piece_index;
			s << "\n   slot_index: ";
			s << slot_index;
			s << "\n   piece at our slot: ";
			s << m_slot_to_piece[piece_index];
			s << "\n";

			print_to_log(s.str());
			debug_log();
#endif

			int piece_at_our_slot = m_slot_to_piece[piece_index];
			assert(m_piece_to_slot[piece_at_our_slot] == piece_index);

			std::swap(
				m_slot_to_piece[piece_index]
				, m_slot_to_piece[slot_index]);

			std::swap(
				m_piece_to_slot[piece_index]
				, m_piece_to_slot[piece_at_our_slot]);

			const int slot_size = static_cast<int>(m_info.piece_size(slot_index));
			std::vector<char> buf(slot_size);
			m_storage.read(&buf[0], piece_index, 0, slot_size);
			m_storage.write(&buf[0], slot_index, 0, slot_size);

			assert(m_slot_to_piece[piece_index] == piece_index);
			assert(m_piece_to_slot[piece_index] == piece_index);

			slot_index = piece_index;

#if !defined(NDEBUG) && defined(TORRENT_STORAGE_DEBUG)
			debug_log();
#endif
		}

		assert(slot_index >= 0);
		assert(slot_index < (int)m_slot_to_piece.size());
		return slot_index;
	}

	namespace
	{
		
		
		struct allocation_syncronization
		{
			allocation_syncronization(
				bool& flag
				, boost::condition& cond
				, boost::mutex& monitor)
				: m_flag(flag)
				, m_cond(cond)
				, m_monitor(monitor)
			{
				boost::mutex::scoped_lock lock(m_monitor);

				while (m_flag)
					m_cond.wait(lock);

				m_flag = true;
			}

			~allocation_syncronization()
			{
				boost::mutex::scoped_lock lock(m_monitor);
				m_flag = false;
				m_cond.notify_one();
			}

			bool& m_flag;
			boost::condition& m_cond;
			boost::mutex& m_monitor;
		};

	}

	void piece_manager::impl::allocate_slots(int num_slots)
	{
		assert(num_slots > 0);

		
		
		allocation_syncronization sync_obj(
			m_allocating
			, m_allocating_condition
			, m_allocating_monitor);

		
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		  

		assert(!m_unallocated_slots.empty());

		const int piece_size = static_cast<int>(m_info.piece_length());

		std::vector<char>& buffer = m_scratch_buffer;
		buffer.resize(piece_size);

		for (int i = 0; i < num_slots && !m_unallocated_slots.empty(); ++i)
		{
			int pos = m_unallocated_slots.front();
			
			bool write_back = false;

			int new_free_slot = pos;
			if (m_piece_to_slot[pos] != has_no_slot)
			{
				assert(m_piece_to_slot[pos] >= 0);
				m_storage.read(&buffer[0], m_piece_to_slot[pos], 0, static_cast<int>(m_info.piece_size(pos)));
				new_free_slot = m_piece_to_slot[pos];
				m_slot_to_piece[pos] = pos;
				m_piece_to_slot[pos] = pos;
				write_back = true;
			}
			m_unallocated_slots.erase(m_unallocated_slots.begin());
			m_slot_to_piece[new_free_slot] = unassigned;
			m_free_slots.push_back(new_free_slot);

			if (write_back || m_fill_mode)
				m_storage.write(&buffer[0], pos, 0, static_cast<int>(m_info.piece_size(pos)));
		}

		assert(m_free_slots.size() > 0);
	}

	void piece_manager::allocate_slots(int num_slots)
	{
		m_pimpl->allocate_slots(num_slots);
	}

	path const& piece_manager::save_path() const
	{
		return m_pimpl->save_path();
	}

	bool piece_manager::move_storage(path const& save_path)
	{
		return m_pimpl->move_storage(save_path);
	}

#ifndef NDEBUG
	void piece_manager::impl::check_invariant() const
	{
		
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		
		if (m_piece_to_slot.empty()) return;

		assert((int)m_piece_to_slot.size() == m_info.num_pieces());
		assert((int)m_slot_to_piece.size() == m_info.num_pieces());

		for (std::vector<int>::const_iterator i = m_free_slots.begin();
			i != m_free_slots.end(); ++i)
		{
			assert(*i < (int)m_slot_to_piece.size());
			assert(*i >= 0);
			assert(m_slot_to_piece[*i] == unassigned);
			assert(std::find(i+1, m_free_slots.end(), *i)
				== m_free_slots.end());
		}

		for (std::vector<int>::const_iterator i = m_unallocated_slots.begin();
			i != m_unallocated_slots.end(); ++i)
		{
			assert(*i < (int)m_slot_to_piece.size());
			assert(*i >= 0);
			assert(m_slot_to_piece[*i] == unallocated);
			assert(std::find(i+1, m_unallocated_slots.end(), *i)
				== m_unallocated_slots.end());
		}

		for (int i = 0; i < m_info.num_pieces(); ++i)
		{
			
			if (m_piece_to_slot[i] != has_no_slot)
			{
				assert(m_piece_to_slot[i] >= 0);
				assert(m_piece_to_slot[i] < (int)m_slot_to_piece.size());
			}

			
			if (m_slot_to_piece[i] != unallocated
				&& m_slot_to_piece[i] != unassigned)
			{
				assert(m_slot_to_piece[i] >= 0);
				assert(m_slot_to_piece[i] < (int)m_piece_to_slot.size());
			}

			
			if (m_piece_to_slot[i] >= 0)
			{
				assert(m_slot_to_piece[m_piece_to_slot[i]] == i);
				if (m_piece_to_slot[i] != i)
				{
					assert(m_slot_to_piece[i] == unallocated);
				}
			}
			else
			{
				assert(m_piece_to_slot[i] == has_no_slot);
			}

			

			if (m_slot_to_piece[i] >= 0)
			{
				assert(m_slot_to_piece[i] < (int)m_piece_to_slot.size());
				assert(m_piece_to_slot[m_slot_to_piece[i]] == i);
#ifdef TORRENT_STORAGE_DEBUG
				assert(
					std::find(
						m_unallocated_slots.begin()
						, m_unallocated_slots.end()
						, i) == m_unallocated_slots.end()
				);
				assert(
					std::find(
						m_free_slots.begin()
						, m_free_slots.end()
						, i) == m_free_slots.end()
				);
#endif
			}
			else if (m_slot_to_piece[i] == unallocated)
			{
#ifdef TORRENT_STORAGE_DEBUG
				assert(m_unallocated_slots.empty()
					|| (std::find(
						m_unallocated_slots.begin()
						, m_unallocated_slots.end()
						, i) != m_unallocated_slots.end())
				);
#endif
			}
			else if (m_slot_to_piece[i] == unassigned)
			{
#ifdef TORRENT_STORAGE_DEBUG
				assert(
					std::find(
						m_free_slots.begin()
						, m_free_slots.end()
						, i) != m_free_slots.end()
				);
#endif
			}
			else
			{
				assert(false && "m_slot_to_piece[i] is invalid");
			}
		}
	}

#ifdef TORRENT_STORAGE_DEBUG
	void piece_manager::impl::debug_log() const
	{
		std::stringstream s;

		s << "index\tslot\tpiece\n";

		for (int i = 0; i < m_info.num_pieces(); ++i)
		{
			s << i << "\t" << m_slot_to_piece[i] << "\t";
			s << m_piece_to_slot[i] << "\n";
		}

		s << "---------------------------------\n";

		print_to_log(s.str());
	}
#endif
#endif
} 

