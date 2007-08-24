/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <libtorrent/kademlia/closest_nodes.hpp>
#include <libtorrent/kademlia/routing_table.hpp>
#include <libtorrent/kademlia/rpc_manager.hpp>

namespace libtorrent { namespace dht
{

using asio::ip::udp;

typedef boost::shared_ptr<observer> observer_ptr;

class closest_nodes_observer : public observer
{
public:
	closest_nodes_observer(
		boost::intrusive_ptr<traversal_algorithm> const& algorithm
		, node_id self
		, node_id target)
		: m_algorithm(algorithm)
		, m_target(target) 
		, m_self(self)
	{}
	~closest_nodes_observer();

	void send(msg& p)
	{
		p.info_hash = m_target;
	}

	void timeout();
	void reply(msg const&);
	void abort() { m_algorithm = 0; }

private:
	boost::intrusive_ptr<traversal_algorithm> m_algorithm;
	node_id const m_target;
	node_id const m_self;
};

closest_nodes_observer::~closest_nodes_observer()
{
	if (m_algorithm) m_algorithm->failed(m_self, true);
}

void closest_nodes_observer::reply(msg const& in)
{
	if (!m_algorithm)
	{
		assert(false);
		return;
	}

	if (!in.nodes.empty())
	{
		for (msg::nodes_t::const_iterator i = in.nodes.begin()
			, end(in.nodes.end()); i != end; ++i)
		{
			m_algorithm->traverse(i->id, i->addr);
		}
	}
	m_algorithm->finished(m_self);
	m_algorithm = 0;
}

void closest_nodes_observer::timeout()
{
	if (!m_algorithm) return;
	m_algorithm->failed(m_self);
	m_algorithm = 0;
} 

closest_nodes::closest_nodes(
	node_id target
	, int branch_factor
	, int max_results
	, routing_table& table
	, rpc_manager& rpc
	, done_callback const& callback
)
	: traversal_algorithm(
		target
		, branch_factor
		, max_results
		, table
		, rpc
		, table.begin()
		, table.end()
	)
	, m_done_callback(callback)
{
	boost::intrusive_ptr<closest_nodes> self(this);
	add_requests();
}

void closest_nodes::invoke(node_id const& id, udp::endpoint addr)
{
	observer_ptr p(new closest_nodes_observer(this, id, m_target));
	m_rpc.invoke(messages::find_node, addr, p);
}

void closest_nodes::done()
{
	std::vector<node_entry> results;
	int result_size = m_table.bucket_size();
	if (result_size > (int)m_results.size()) result_size = (int)m_results.size();
	for (std::vector<result>::iterator i = m_results.begin()
		, end(m_results.begin() + result_size); i != end; ++i)
	{
		results.push_back(node_entry(i->id, i->addr));
	}
	m_done_callback(results);
}

void closest_nodes::initiate(
	node_id target
	, int branch_factor
	, int max_results
	, routing_table& table
	, rpc_manager& rpc
	, done_callback const& callback
)
{
	new closest_nodes(target, branch_factor, max_results, table, rpc, callback);
}

} } 

