/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <libtorrent/kademlia/traversal_algorithm.hpp>
#include <libtorrent/kademlia/routing_table.hpp>
#include <libtorrent/kademlia/rpc_manager.hpp>

#include <boost/bind.hpp>

using boost::bind;
using asio::ip::udp;

namespace libtorrent { namespace dht
{
#ifdef TORRENT_DHT_VERBOSE_LOGGING
TORRENT_DEFINE_LOG(traversal)
#endif

void traversal_algorithm::add_entry(node_id const& id, udp::endpoint addr, unsigned char flags)
{
	if (m_failed.find(addr) != m_failed.end()) return;

	result const entry(id, addr, flags);

	std::vector<result>::iterator i = std::lower_bound(
		m_results.begin()
		, m_results.end()
		, entry
		, bind(
			compare_ref
			, bind(&result::id, _1)
			, bind(&result::id, _2)
			, m_target
		)
	);

	if (i == m_results.end() || i->id != id)
	{
		assert(std::find_if(m_results.begin(), m_results.end()
			, bind(std::equal_to<node_id>()
				, bind(&result::id, _1), id)) == m_results.end());
#ifdef TORRENT_DHT_VERBOSE_LOGGING
		TORRENT_LOG(traversal) << "adding result: " << id << " " << addr;
#endif
		m_results.insert(i, entry);
	}
}

void traversal_algorithm::traverse(node_id const& id, udp::endpoint addr)
{
	add_entry(id, addr, 0);
}

void traversal_algorithm::finished(node_id const& id)
{
	--m_invoke_count;
	add_requests();
	if (m_invoke_count == 0) done();
}   

void traversal_algorithm::failed(node_id const& id, bool prevent_request)
{
	m_invoke_count--;

	std::vector<result>::iterator i = std::find_if(
		m_results.begin()
		, m_results.end()
		, bind(
			std::equal_to<node_id>()
			, bind(&result::id, _1)
			, id
		)
	);

	assert(i != m_results.end());

	if (i != m_results.end())
	{
		assert(i->flags & result::queried);
		m_failed.insert(i->addr);
#ifdef TORRENT_DHT_VERBOSE_LOGGING
		TORRENT_LOG(traversal) << "failed: " << i->id << " " << i->addr;
#endif
		m_results.erase(i);
	}
	if (prevent_request)
	{
		--m_branch_factor;
		if (m_branch_factor <= 0) m_branch_factor = 1;
	}
	else
	{
		m_table.node_failed(id);
	}
	add_requests();
	if (m_invoke_count == 0) done();
}

namespace
{
	bool bitwise_nand(unsigned char lhs, unsigned char rhs)
	{
		return (lhs & rhs) == 0;
	}
}

void traversal_algorithm::add_requests()
{
	while (m_invoke_count < m_branch_factor)
	{
		
		
		std::vector<result>::iterator i = std::find_if(
			m_results.begin()
			, last_iterator()
			, bind(
				&bitwise_nand
				, bind(&result::flags, _1)
				, (unsigned char)result::queried
			)
		);
#ifdef TORRENT_DHT_VERBOSE_LOGGING
		TORRENT_LOG(traversal) << "nodes left (" << this << "): " << (last_iterator() - i);
#endif

		if (i == last_iterator()) break;

		try
		{
			invoke(i->id, i->addr);
			++m_invoke_count;
			i->flags |= result::queried;
		}
		catch (std::exception& e) {}
	}
}

std::vector<traversal_algorithm::result>::iterator traversal_algorithm::last_iterator()
{
	return (int)m_results.size() >= m_max_results ?
		m_results.begin() + m_max_results
		: m_results.end();
}

} } 

