/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#include "libtorrent/ip_filter.hpp"
#include <boost/utility.hpp>

#include "test.hpp"  

using namespace libtorrent;

template <class Addr>
bool compare(ip_range<Addr> const& lhs
	, ip_range<Addr> const& rhs)
{
	return lhs.first == rhs.first
		&& lhs.last == rhs.last
		&& lhs.flags == rhs.flags;
}

void test_rules_invariant(std::vector<ip_range<address_v4> > const& r, ip_filter const& f)
{
	typedef std::vector<ip_range<address_v4> >::const_iterator iterator;
	TEST_CHECK(!r.empty());
	if (r.empty()) return;

	TEST_CHECK(r.front().first == address::from_string("0.0.0.0"));
	TEST_CHECK(r.back().last == address::from_string("255.255.255.255"));
	
	iterator i = r.begin();
	iterator j = boost::next(i);
	for (iterator i(r.begin()), j(boost::next(r.begin()))
		, end(r.end()); j != end; ++j, ++i)
	{
		TEST_CHECK(f.access(i->last) == i->flags);
		TEST_CHECK(f.access(j->first) == j->flags);
		TEST_CHECK(i->last.to_ulong() + 1 == j->first.to_ulong());
	}
}

int test_main()
{
	using namespace libtorrent;

	std::vector<ip_range<address_v4> > range;

	
	ip_range<address_v4> expected1[] =
	{
		{address_v4::from_string("0.0.0.0"), address_v4::from_string("0.255.255.255"), 0}
		, {address_v4::from_string("1.0.0.0"), address_v4::from_string("3.0.0.0"), ip_filter::blocked}
		, {address_v4::from_string("3.0.0.1"), address_v4::from_string("255.255.255.255"), 0}
	};
	
	{
		ip_filter f;
		f.add_rule(address::from_string("1.0.0.0"), address::from_string("2.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("2.0.0.1"), address::from_string("3.0.0.0"), ip_filter::blocked);

		range = boost::get<0>(f.export_filter());
		test_rules_invariant(range, f);

		TEST_CHECK(range.size() == 3);
		TEST_CHECK(std::equal(range.begin(), range.end(), expected1, &compare<address_v4>));

	}
	
	

	{
		ip_filter f;
		f.add_rule(address::from_string("2.0.0.1"), address::from_string("3.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("1.0.0.0"), address::from_string("2.0.0.0"), ip_filter::blocked);

		range = boost::get<0>(f.export_filter());
		test_rules_invariant(range, f);

		TEST_CHECK(range.size() == 3);
		TEST_CHECK(std::equal(range.begin(), range.end(), expected1, &compare<address_v4>));

	}	 

	

	{
		ip_filter f;
		f.add_rule(address::from_string("2.0.0.1"), address::from_string("3.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("1.0.0.0"), address::from_string("2.4.0.0"), ip_filter::blocked);

		range = boost::get<0>(f.export_filter());
		test_rules_invariant(range, f);

		TEST_CHECK(range.size() == 3);
		TEST_CHECK(std::equal(range.begin(), range.end(), expected1, &compare<address_v4>));

	}	 

	

	{
		ip_filter f;
		f.add_rule(address::from_string("1.0.0.0"), address::from_string("2.4.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("2.0.0.1"), address::from_string("3.0.0.0"), ip_filter::blocked);

		range = boost::get<0>(f.export_filter());
		test_rules_invariant(range, f);

		TEST_CHECK(range.size() == 3);
		TEST_CHECK(std::equal(range.begin(), range.end(), expected1, &compare<address_v4>));

	}	 

	

	{
		ip_filter f;
		f.add_rule(address::from_string("1.0.0.0"), address::from_string("2.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("3.0.0.0"), address::from_string("4.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("5.0.0.0"), address::from_string("6.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("7.0.0.0"), address::from_string("8.0.0.0"), ip_filter::blocked);

		f.add_rule(address::from_string("1.0.1.0"), address::from_string("9.0.0.0"), ip_filter::blocked);
		
		range = boost::get<0>(f.export_filter());
		test_rules_invariant(range, f);

		TEST_CHECK(range.size() == 3);
		ip_range<address_v4> expected[] =
		{
			{address_v4::from_string("0.0.0.0"), address_v4::from_string("0.255.255.255"), 0}
			, {address_v4::from_string("1.0.0.0"), address_v4::from_string("9.0.0.0"), ip_filter::blocked}
			, {address_v4::from_string("9.0.0.1"), address_v4::from_string("255.255.255.255"), 0}
		};
	
		TEST_CHECK(std::equal(range.begin(), range.end(), expected, &compare<address_v4>));

	}	

	

	{
		ip_filter f;
		f.add_rule(address::from_string("1.0.0.0"), address::from_string("2.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("3.0.0.0"), address::from_string("4.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("5.0.0.0"), address::from_string("6.0.0.0"), ip_filter::blocked);
		f.add_rule(address::from_string("7.0.0.0"), address::from_string("8.0.0.0"), ip_filter::blocked);

		f.add_rule(address::from_string("0.0.1.0"), address::from_string("7.0.4.0"), ip_filter::blocked);

		range = boost::get<0>(f.export_filter());
		test_rules_invariant(range, f);

		TEST_CHECK(range.size() == 3);
		ip_range<address_v4> expected[] =
		{
			{address_v4::from_string("0.0.0.0"), address_v4::from_string("0.0.0.255"), 0}
			, {address_v4::from_string("0.0.1.0"), address_v4::from_string("8.0.0.0"), ip_filter::blocked}
			, {address_v4::from_string("8.0.0.1"), address_v4::from_string("255.255.255.255"), 0}
		};
	
		TEST_CHECK(std::equal(range.begin(), range.end(), expected, &compare<address_v4>));

	}	

	return 0;
}

