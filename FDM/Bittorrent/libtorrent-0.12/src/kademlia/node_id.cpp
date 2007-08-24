/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <algorithm>
#include <iomanip>
#include <cassert>
#include <boost/bind.hpp>

#include "libtorrent/kademlia/node_id.hpp"

using boost::bind;

namespace libtorrent { namespace dht
{  

node_id distance(node_id const& n1, node_id const& n2)
{
	node_id ret;
	node_id::iterator k = ret.begin();
	for (node_id::const_iterator i = n1.begin(), j = n2.begin()
		, end(n1.end()); i != end; ++i, ++j, ++k)
	{
		*k = *i ^ *j;
	}
	return ret;
} 

bool compare_ref(node_id const& n1, node_id const& n2, node_id const& ref)
{
	for (node_id::const_iterator i = n1.begin(), j = n2.begin()
		, k = ref.begin(), end(n1.end()); i != end; ++i, ++j, ++k)
	{
		boost::uint8_t lhs = (*i ^ *k);
		boost::uint8_t rhs = (*j ^ *k);
		if (lhs < rhs) return true;
		if (lhs > rhs) return false;
	}
	return false;
}  

int distance_exp(node_id const& n1, node_id const& n2)
{
	int byte = node_id::size - 1;
	for (node_id::const_iterator i = n1.begin(), j = n2.begin()
		, end(n1.end()); i != end; ++i, ++j, --byte)
	{
		assert(byte >= 0);
		boost::uint8_t t = *i ^ *j;
		if (t == 0) continue;
		
		
		
		int bit = byte * 8;
		for (int b = 7; b > 0; --b)
			if (t >= (1 << b)) return bit + b;
		return bit;
	}

	return 0;
}

} }  

