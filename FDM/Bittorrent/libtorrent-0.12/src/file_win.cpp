/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include "libtorrent/file.hpp"
#include "libtorrent/utf8.hpp"

#ifdef UNICODE
#include "libtorrent/storage.hpp"
#endif

#include <sstream>
#include <windows.h>

namespace
{
	
	class auto_localfree
	{
	public:
		auto_localfree(HLOCAL memory)
			: m_memory(memory)
		{
		}
		~auto_localfree()
		{
			if (m_memory)
				LocalFree(m_memory);
		}
	private:
		HLOCAL m_memory;
	};

	std::string utf8_native(std::string const& s)
	{
		try
		{
			std::wstring ws;
			libtorrent::utf8_wchar(s, ws);
			std::size_t size = wcstombs(0, ws.c_str(), 0);
			if (size == std::size_t(-1)) return s;
			std::string ret;
			ret.resize(size);
			size = wcstombs(&ret[0], ws.c_str(), size + 1);
			if (size == wchar_t(-1)) return s;
			ret.resize(size);
			return ret;
		}
		catch(std::exception)
		{
			return s;
		}
	}
	
	void throw_exception(const char* thrower)
	{
		DWORD err = GetLastError();

#ifdef UNICODE
		wchar_t *wbuffer = 0;
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM
			|FORMAT_MESSAGE_ALLOCATE_BUFFER
			, 0, err, 0, (LPWSTR)&wbuffer, 0, 0);
		auto_localfree auto_free(wbuffer);
		std::string tmp_utf8;
		libtorrent::wchar_utf8(wbuffer, tmp_utf8);
		char const* buffer = tmp_utf8.c_str();
#else
		char* buffer = 0;
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM
			|FORMAT_MESSAGE_ALLOCATE_BUFFER
			, 0, err, 0, (LPSTR)&buffer, 0, 0);
		auto_localfree auto_free(buffer);
#endif

		std::stringstream s;
		s << (thrower ? thrower : "NULL") << ": " << (buffer ? buffer : "NULL");

		throw libtorrent::file_error(s.str());
	}
}

namespace libtorrent
{

	struct file::impl : boost::noncopyable
	{
		enum open_flags
		{
			read_flag = 1,
			write_flag = 2
		};

		enum seek_mode
		{
			seek_begin = FILE_BEGIN,
			seek_from_here = FILE_CURRENT,
			seek_end = FILE_END
		};

		impl()
		{
			m_file_handle = INVALID_HANDLE_VALUE;
		}

		void open(const char *file_name, open_flags flags)
		{
			assert(file_name);
			assert(flags & (read_flag | write_flag));

			DWORD access_mask = 0;
			if (flags & read_flag)
				access_mask |= GENERIC_READ;
			if (flags & write_flag)
				access_mask |= GENERIC_WRITE;

			assert(access_mask & (GENERIC_READ | GENERIC_WRITE));

		#ifdef UNICODE
			std::wstring wfile_name(safe_convert(file_name));
			HANDLE new_handle = CreateFile(
				wfile_name.c_str()
				, access_mask
				, FILE_SHARE_READ
				, 0
				, (flags & write_flag)?OPEN_ALWAYS:OPEN_EXISTING
				, FILE_ATTRIBUTE_NORMAL
				, 0);
		#else
			HANDLE new_handle = CreateFile(
				utf8_native(file_name).c_str()
				, access_mask
				, FILE_SHARE_READ
				, 0
				, (flags & write_flag)?OPEN_ALWAYS:OPEN_EXISTING
				, FILE_ATTRIBUTE_NORMAL
				, 0);
		#endif

			if (new_handle == INVALID_HANDLE_VALUE)
			{
				std::stringstream s;
				throw_exception(file_name);
			}
			
			close();
			m_file_handle = new_handle;
		}

		void close()
		{
			if (m_file_handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_file_handle);
				m_file_handle = INVALID_HANDLE_VALUE;
			}
		}

		~impl()
		{
			close();
		}

		size_type write(const char* buffer, size_type num_bytes)
		{
			assert(buffer);
			assert((DWORD)num_bytes == num_bytes);
			DWORD bytes_written = 0;
			if (num_bytes != 0)
			{
				if (FALSE == WriteFile(
					m_file_handle
					, buffer
					, (DWORD)num_bytes
					, &bytes_written
					, 0))
				{
					throw_exception("file::write");
				}
			}
			return bytes_written;
		}
		
		size_type read(char* buffer, size_type num_bytes)
		{
			assert(buffer);
			assert(num_bytes >= 0);
			assert((DWORD)num_bytes == num_bytes);

			DWORD bytes_read = 0;
			if (num_bytes != 0)
			{
				if (FALSE == ReadFile(
					m_file_handle
					, buffer
					, (DWORD)num_bytes
					, &bytes_read
					, 0))
				{
					throw_exception("file::read");
				}
			}
			return bytes_read;
		}

		size_type seek(size_type pos, seek_mode from_where)
		{
			assert(pos >= 0 || from_where != seek_begin);
			assert(pos <= 0 || from_where != seek_end);
			LARGE_INTEGER offs;
			offs.QuadPart = pos;
			if (FALSE == SetFilePointerEx(
				m_file_handle
				, offs
				, &offs
				, from_where))
			{
				throw_exception("file::seek");
			}
			return offs.QuadPart;
		}
		
		size_type tell()
		{
			LARGE_INTEGER offs;
			offs.QuadPart = 0;

			
			if (FALSE == SetFilePointerEx(
				m_file_handle
				, offs
				, &offs
				, FILE_CURRENT))
			{
				throw_exception("file::tell");
			}

			size_type pos = offs.QuadPart;
			assert(pos >= 0);
			return pos;
		}

	private:

		HANDLE m_file_handle;

	};
}

namespace libtorrent
{

	const file::seek_mode file::begin(file::impl::seek_begin);
	const file::seek_mode file::end(file::impl::seek_end);

	const file::open_mode file::in(file::impl::read_flag);
	const file::open_mode file::out(file::impl::write_flag);

	file::file()
		: m_impl(new libtorrent::file::impl())
	{
	}
	file::file(boost::filesystem::path const& p, open_mode m)
		: m_impl(new libtorrent::file::impl())
	{
		open(p,m);
	}

	file::~file()
	{
	}

	void file::open(boost::filesystem::path const& p, open_mode m)
	{
		assert(p.is_complete());
		m_impl->open(p.native_file_string().c_str(), impl::open_flags(m.m_mask));
	}

	void file::close()
	{
		m_impl->close();
	}

	size_type file::write(const char* buffer, size_type num_bytes)
	{
		return m_impl->write(buffer, num_bytes);
	}

	size_type file::read(char* buffer, size_type num_bytes)
	{
		return m_impl->read(buffer, num_bytes);
	}

	size_type file::seek(size_type pos, seek_mode m)
	{
		return m_impl->seek(pos,impl::seek_mode(m.m_val));
	}
	
	size_type file::tell()
	{
		return m_impl->tell();
	}
}
