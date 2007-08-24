/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

  

 

#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <numeric>
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "libtorrent/kademlia/routing_table.hpp"
#include "libtorrent/kademlia/node_id.hpp"
#include "libtorrent/session_settings.hpp"

using boost::bind;
using boost::uint8_t;

using boost::posix_time::second_clock;
using boost::posix_time::minutes;
using boost::posix_time::seconds;
using boost::posix_time::hours;

namespace pt = boost::posix_time;

namespace libtorrent { namespace dht
{

using asio::ip::udp;
typedef asio::ip::address_v4 address;

routing_table::routing_table(node_id const& id, int bucket_size
	, dht_settings const& settings)
	: m_bucket_size(bucket_size)
	, m_settings(settings)
	, m_id(id)
	, m_lowest_active_bucket(160)
{
	
	
	for (int i = 0; i < 160; ++i)
		m_bucket_activity[i] = second_clock::universal_time() - seconds(15*60 - i*5);
}

boost::tuple<int, int> routing_table::size() const
{
	int nodes = 0;
	int replacements = 0;
	for (table_t::const_iterator i = m_buckets.begin()
		, end(m_buckets.end()); i != end; ++i)
	{
		nodes += i->first.size();
		replacements += i->second.size();
	}
	return boost::make_tuple(nodes, replacements);
}

void routing_table::print_state(std::ostream& os) const
{
	os << "kademlia routing table state\n"
		<< "bucket_size: " << m_bucket_size << "\n"
		<< "node_id: " << m_id << "\n\n";

	os << "number of nodes per bucket:\n"
		"live\n";
	for (int k = 0; k < 8; ++k)
	{
		for (table_t::const_iterator i = m_buckets.begin(), end(m_buckets.end());
			i != end; ++i)
		{
			os << (int(i->first.size()) > (7 - k) ? "|" : " ");
		}
		os << "\n";
	}
	for (table_t::const_iterator i = m_buckets.begin(), end(m_buckets.end());
		i != end; ++i)
	{
		os << "+";
	}
	os << "\n";
	for (int k = 0; k < 8; ++k)
	{
		for (table_t::const_iterator i = m_buckets.begin(), end(m_buckets.end());
			i != end; ++i)
		{
			os << (int(i->second.size()) > k ? "|" : " ");
		}
		os << "\n";
	}
	os << "cached\n-----------\n";

	os << "nodes:\n";
	for (table_t::const_iterator i = m_buckets.begin(), end(m_buckets.end());
		i != end; ++i)
	{
		int bucket_index = int(i - m_buckets.begin());
		os << "bucket " << bucket_index << " "
			<< to_simple_string(m_bucket_activity[bucket_index])
			<< " " << (bucket_index >= m_lowest_active_bucket?"active":"inactive")
			<< "\n";
		for (bucket_t::const_iterator j = i->first.begin()
			, end(i->first.end()); j != end; ++j)
		{
			os << "ip: " << j->addr << " 	fails: " << j->fail_count
				<< " 	id: " << j->id << "\n";
		}
	}
}

void routing_table::touch_bucket(int bucket)
{
	m_bucket_activity[bucket] = second_clock::universal_time();
}

boost::posix_time::ptime routing_table::next_refresh(int bucket)
{
	assert(bucket < 160);
	assert(bucket >= 0);
	
	
	if (bucket <= m_lowest_active_bucket && bucket > 0)
		return second_clock::universal_time() + minutes(15);
	return m_bucket_activity[bucket] + minutes(15);
}

void routing_table::replacement_cache(bucket_t& nodes) const
{
	for (table_t::const_iterator i = m_buckets.begin()
		, end(m_buckets.end()); i != end; ++i)
	{
		std::copy(i->second.begin(), i->second.end()
			, std::back_inserter(nodes));
	}
}

bool routing_table::need_node(node_id const& id)
{
	int bucket_index = distance_exp(m_id, id);
	assert(bucket_index < (int)m_buckets.size());
	assert(bucket_index >= 0);
	bucket_t& b = m_buckets[bucket_index].first;
	bucket_t& rb = m_buckets[bucket_index].second;

	
	
	
	if ((int)rb.size() >= m_bucket_size) return false;
	
	
	if (std::find_if(b.begin(), b.end(), bind(std::equal_to<node_id>()
		, bind(&node_entry::id, _1), id)) != b.end()) return false;

	if (std::find_if(rb.begin(), rb.end(), bind(std::equal_to<node_id>()
		, bind(&node_entry::id, _1), id)) != rb.end()) return false;

	return true;
}

void routing_table::node_failed(node_id const& id)
{
	int bucket_index = distance_exp(m_id, id);
	assert(bucket_index < (int)m_buckets.size());
	assert(bucket_index >= 0);
	bucket_t& b = m_buckets[bucket_index].first;
	bucket_t& rb = m_buckets[bucket_index].second;

	bucket_t::iterator i = std::find_if(b.begin(), b.end()
		, bind(std::equal_to<node_id>()
		, bind(&node_entry::id, _1), id));

	if (i == b.end()) return;
	
	
	if (bucket_index == 0) return;

	if (rb.empty())
	{
		++i->fail_count;
		if (i->fail_count >= m_settings.max_fail_count)
		{
			b.erase(i);
			assert(m_lowest_active_bucket <= bucket_index);
			while (m_buckets[m_lowest_active_bucket].first.empty()
				&& m_lowest_active_bucket < 160)
			{
				++m_lowest_active_bucket;
			}
		}
		return;
	}

	b.erase(i);
	b.push_back(rb.back());
	rb.erase(rb.end() - 1);
}

void routing_table::add_router_node(udp::endpoint router)
{
	m_router_nodes.insert(router);
}       

bool routing_table::node_seen(node_id const& id, udp::endpoint addr)
{
	if (m_router_nodes.find(addr) != m_router_nodes.end()) return false;
	int bucket_index = distance_exp(m_id, id);
	assert(bucket_index < (int)m_buckets.size());
	assert(bucket_index >= 0);
	bucket_t& b = m_buckets[bucket_index].first;

	bucket_t::iterator i = std::find_if(b.begin(), b.end()
		, bind(std::equal_to<node_id>()
		, bind(&node_entry::id, _1), id));

	bool ret = need_bootstrap();

	m_bucket_activity[bucket_index] = second_clock::universal_time();

	if (i != b.end())
	{
		
		 

		
		
		
		
		b.erase(i);
		b.push_back(node_entry(id, addr));

		return ret;
	}

	
	
	
	
	if ((int)b.size() < m_bucket_size)
	{
		b.push_back(node_entry(id, addr));
		
		
		if (bucket_index < m_lowest_active_bucket
			&& bucket_index > 0)
			m_lowest_active_bucket = bucket_index;

		return ret;
	}

	
	
	
	
	
	
	

	i = std::max_element(b.begin(), b.end()
		, bind(std::less<int>()
			, bind(&node_entry::fail_count, _1)
			, bind(&node_entry::fail_count, _2)));

	if (i != b.end() && i->fail_count > 0)
	{
		
		
		b.erase(i);
		b.push_back(node_entry(id, addr));

		return ret;
	}

	
	
	
	

	bucket_t& rb = m_buckets[bucket_index].second;

	i = std::find_if(rb.begin(), rb.end()
		, bind(std::equal_to<node_id>()
		, bind(&node_entry::id, _1), id));

	
	
	if (i != rb.end()) return ret;
	
	if ((int)rb.size() > m_bucket_size) rb.erase(rb.begin());
	rb.push_back(node_entry(id, addr));

	return ret;
}

bool routing_table::need_bootstrap() const
{
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->fail_count == 0) return false;
	}
	return true;
}  

void routing_table::find_node(node_id const& target
	, std::vector<node_entry>& l, bool include_self, int count)
{
	l.clear();
	if (count == 0) count = m_bucket_size;
	l.reserve(count);

	int bucket_index = distance_exp(m_id, target);
	bucket_t& b = m_buckets[bucket_index].first;

	
	
	std::remove_copy_if(b.begin(), b.end(), std::back_inserter(l)
		, bind(&node_entry::fail_count, _1));
	assert((int)l.size() <= count);

	if ((int)l.size() == count)
	{
		assert(std::count_if(l.begin(), l.end()
			, boost::bind(std::not_equal_to<int>()
				, boost::bind(&node_entry::fail_count, _1), 0)) == 0);
		return;
	}

	
	
	
	
	
	bucket_t tmpb;
	for (int i = include_self?0:1; i < count; ++i)
	{
		bucket_t& b = m_buckets[i].first;
		std::remove_copy_if(b.begin(), b.end(), std::back_inserter(tmpb)
			, bind(&node_entry::fail_count, _1));
	}

	std::random_shuffle(tmpb.begin(), tmpb.end());
	size_t to_copy = (std::min)(m_bucket_size - l.size()
		, tmpb.size());
	std::copy(tmpb.begin(), tmpb.begin() + to_copy
		, std::back_inserter(l));
		
	assert((int)l.size() <= m_bucket_size);

	
	
	
	if ((int)l.size() == count
		|| bucket_index == (int)m_buckets.size() - 1)
	{
		assert(std::count_if(l.begin(), l.end()
			, boost::bind(std::not_equal_to<int>()
				, boost::bind(&node_entry::fail_count, _1), 0)) == 0);
		return;
	}

	for (size_t i = bucket_index + 1; i < m_buckets.size(); ++i)
	{
		bucket_t& b = m_buckets[i].first;
	
		std::remove_copy_if(b.begin(), b.end(), std::back_inserter(l)
			, bind(&node_entry::fail_count, _1));
		if ((int)l.size() >= count)
		{
			l.erase(l.begin() + count, l.end());
			assert(std::count_if(l.begin(), l.end()
				, boost::bind(std::not_equal_to<int>()
					, boost::bind(&node_entry::fail_count, _1), 0)) == 0);
			return;
		}
	}
	assert((int)l.size() == count
		|| std::distance(l.begin(), l.end()) < m_bucket_size);
	assert((int)l.size() <= count);

	assert(std::count_if(l.begin(), l.end()
		, boost::bind(std::not_equal_to<int>()
			, boost::bind(&node_entry::fail_count, _1), 0)) == 0);
}

routing_table::iterator routing_table::begin() const
{
	return iterator(m_buckets.begin(), m_buckets.end());
}

routing_table::iterator routing_table::end() const
{
	return iterator(m_buckets.end(), m_buckets.end());
}

} } 

