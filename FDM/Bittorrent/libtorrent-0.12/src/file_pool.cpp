/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#include "libtorrent/file_pool.hpp"

#include <iostream>

namespace libtorrent
{
	using boost::multi_index::nth_index;
	using boost::multi_index::get;

	boost::shared_ptr<file> file_pool::open_file(void* st, fs::path const& p, file::open_mode m)
	{
		assert(st != 0);
		assert(p.is_complete());
		boost::mutex::scoped_lock l(m_mutex);
		typedef nth_index<file_set, 0>::type path_view;
		path_view& pt = get<0>(m_files);
		path_view::iterator i = pt.find(p);
		if (i != pt.end())
		{
			lru_file_entry e = *i;
			e.last_use = pt::second_clock::universal_time();

			
			
			assert(e.key == st);

			e.key = st;
			if ((e.mode & m) != m)
			{
				
				
				i->file_ptr.reset();
				assert(e.file_ptr.unique());
				e.file_ptr.reset();
				e.file_ptr.reset(new file(p, m));
				e.mode = m;
			}
			pt.replace(i, e);
			return e.file_ptr;
		}
		
		if ((int)m_files.size() >= m_size)
		{
			
			
			typedef nth_index<file_set, 1>::type lru_view;
			lru_view& lt = get<1>(m_files);
			lru_view::iterator i = lt.begin();
			
			assert(lt.size() == 1 || (i->last_use <= boost::next(i)->last_use));
			lt.erase(i);
		}
		lru_file_entry e(boost::shared_ptr<file>(new file(p, m)));
		e.mode = m;
		e.key = st;
		e.file_path = p;
		pt.insert(e);
		return e.file_ptr;
	}

	void file_pool::release(void* st)
	{
		boost::mutex::scoped_lock l(m_mutex);
		assert(st != 0);
		using boost::tie;

		typedef nth_index<file_set, 2>::type key_view;
		key_view& kt = get<2>(m_files);

		key_view::iterator start, end;
		tie(start, end) = kt.equal_range(st);
		kt.erase(start, end);
	}

	void file_pool::resize(int size)
	{
		assert(size > 0);
		if (size == m_size) return;
		boost::mutex::scoped_lock l(m_mutex);
		m_size = size;
		if (int(m_files.size()) <= m_size) return;

		
		typedef nth_index<file_set, 1>::type lru_view;
		lru_view& lt = get<1>(m_files);
		lru_view::iterator i = lt.begin();
		while (int(m_files.size()) > m_size)
		{
			
			assert(lt.size() == 1 || (i->last_use <= boost::next(i)->last_use));
			lt.erase(i++);
		}
	}

}
