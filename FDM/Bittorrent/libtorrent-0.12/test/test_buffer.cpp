/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <cassert>
#include <boost/timer.hpp>
#include <iostream>
#include <vector>
#include <utility>

#include "libtorrent/buffer.hpp"

#include "test.hpp"

using libtorrent::buffer; 

int test_main()
{
	char data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	buffer b;

	TEST_CHECK(b.size() == 0);
	TEST_CHECK(b.capacity() == 0);
	TEST_CHECK(b.empty());
	
	buffer::interval i = b.allocate(1);
	memcpy(i.begin, data, 1);

	TEST_CHECK(b.size() == 1);
	TEST_CHECK(b.capacity() >= 1);
	
	i = b.allocate(4);
	memcpy(i.begin, data + 1, 4);
	TEST_CHECK(b.size() == 5);
	TEST_CHECK(b.capacity() >= 5);

	i = b.allocate(4);
	memcpy(i.begin, data + 5, 4);
	TEST_CHECK(b.size() == 9);
	TEST_CHECK(b.capacity() >= 9);

	TEST_CHECK(!b.empty());

	buffer::interval_type read_data = b.data();
	TEST_CHECK(std::equal(read_data.first.begin, read_data.first.end, data));

	b.erase(5);

	TEST_CHECK(b.space_left() == 5);

	i = b.allocate(3);
	memcpy(i.begin, data, 3);
	TEST_CHECK(b.space_left() == 2);
	TEST_CHECK(b.size() == 7);

	read_data = b.data();
	TEST_CHECK(std::equal(read_data.first.begin, read_data.first.end, data + 5));
	TEST_CHECK(std::equal(read_data.second.begin, read_data.second.end, data));

	b.erase(7);
	
	TEST_CHECK(b.empty());
	return 0;
}

