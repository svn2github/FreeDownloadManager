/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#include <libtorrent/kademlia/find_data.hpp>
#include <libtorrent/kademlia/routing_table.hpp>
#include <libtorrent/kademlia/rpc_manager.hpp>
#include <libtorrent/io.hpp>

namespace libtorrent { namespace dht
{

typedef boost::shared_ptr<observer> observer_ptr;

class find_data_observer : public observer
{
public:
	find_data_observer(
		boost::intrusive_ptr<find_data> const& algorithm
		, node_id self
		, node_id target)
		: m_algorithm(algorithm)
		, m_target(target) 
		, m_self(self)
	{}
	~find_data_observer();

	void send(msg& m)
	{
		m.reply = false;
		m.message_id = messages::get_peers;
		m.info_hash = m_target;
	}

	void timeout();
	void reply(msg const&);
	void abort() { m_algorithm = 0; }

private:
	boost::intrusive_ptr<find_data> m_algorithm;
	node_id const m_target;
	node_id const m_self;
};

find_data_observer::~find_data_observer()
{
	if (m_algorithm) m_algorithm->failed(m_self);
}

void find_data_observer::reply(msg const& m)
{
	if (!m_algorithm)
	{
		assert(false);
		return;
	}

	if (!m.peers.empty())
	{
		m_algorithm->got_data(&m);
	}
	else
	{
		for (msg::nodes_t::const_iterator i = m.nodes.begin()
			, end(m.nodes.end()); i != end; ++i)
		{
			m_algorithm->traverse(i->id, i->addr);	
		}
	}
	m_algorithm->finished(m_self);
	m_algorithm = 0;
}

void find_data_observer::timeout()
{
	if (!m_algorithm) return;
	m_algorithm->failed(m_self);
	m_algorithm = 0;
} 

find_data::find_data(
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
	, m_done(false)
{
	boost::intrusive_ptr<find_data> self(this);
	add_requests();
}

void find_data::invoke(node_id const& id, asio::ip::udp::endpoint addr)
{
	if (m_done)
	{
		m_invoke_count = -1;
		return;
	}

	observer_ptr p(new find_data_observer(this, id, m_target));
	m_rpc.invoke(messages::get_peers, addr, p);
}

void find_data::got_data(msg const* m)
{
	m_done = true;
	m_done_callback(m);
}

void find_data::done()
{
	if (m_invoke_count != 0) return;
	if (!m_done) m_done_callback(0);
}

void find_data::initiate(
	node_id target
	, int branch_factor
	, int max_results
	, routing_table& table
	, rpc_manager& rpc
	, done_callback const& callback
)
{
	std::cerr << "find_data::initiate, key: " << target << "\n";
	new find_data(target, branch_factor, max_results, table, rpc, callback);
}

} } 

