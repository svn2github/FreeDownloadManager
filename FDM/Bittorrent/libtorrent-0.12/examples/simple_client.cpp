/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>

#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"

int main(int argc, char* argv[])
{
	using namespace libtorrent;

	namespace fs = boost::filesystem;
	fs::path::default_name_check(fs::no_check);

	if (argc != 2)
	{
		std::cerr << "usage: ./simple_client torrent-file\n"
			"to stop the client, press return.\n";
		return 1;
	}

	try
	{
		session s;
		s.listen_on(std::make_pair(6881, 6889));
	
		std::ifstream in(argv[1], std::ios_base::binary);
		in.unsetf(std::ios_base::skipws);
		entry e = bdecode(std::istream_iterator<char>(in), std::istream_iterator<char>());
		s.add_torrent(torrent_info(e), "./");

		
		char a;
		std::cin.unsetf(std::ios_base::skipws);
		std::cin >> a;
	}
	catch (std::exception& e)
	{
  		std::cout << e.what() << "\n";
	}
	return 0;
}
