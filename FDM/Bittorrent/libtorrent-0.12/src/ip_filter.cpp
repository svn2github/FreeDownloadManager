/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#include "libtorrent/ip_filter.hpp"
#include <boost/utility.hpp>  

namespace libtorrent
{
	void ip_filter::add_rule(address first, address last, int flags)
	{
		if (first.is_v4())
		{
			assert(last.is_v4());
			m_filter4.add_rule(first.to_v4(), last.to_v4(), flags);
		}
		else if (first.is_v6())
		{
			assert(last.is_v6());
			m_filter6.add_rule(first.to_v6(), last.to_v6(), flags);
		}
		else
			assert(false);
	}

	int ip_filter::access(address const& addr) const
	{
		if (addr.is_v4())
			return m_filter4.access(addr.to_v4());
		assert(addr.is_v6());
		return m_filter6.access(addr.to_v6());
	}

	ip_filter::filter_tuple_t ip_filter::export_filter() const
	{
		return boost::make_tuple(m_filter4.export_filter()
			, m_filter6.export_filter());
	}
	

}

