/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent_info.hpp" 

int main(int argc, char* argv[])
{
	using namespace libtorrent;

	if (argc != 2)
	{
		std::cerr << "usage: dump_torrent torrent-file\n";
		return 1;
	}

	boost::filesystem::path::default_name_check(boost::filesystem::no_check);

	try
	{
		std::ifstream in(argv[1], std::ios_base::binary);
		in.unsetf(std::ios_base::skipws);
		entry e = bdecode(std::istream_iterator<char>(in), std::istream_iterator<char>());

		std::cout << "\n\n----- raw info -----\n\n";
		e.print(std::cout);

		torrent_info t(e);
	
		
		std::cout << "\n\n----- torrent file info -----\n\n";
		std::cout << "nodes:\n";
		typedef std::vector<std::pair<std::string, int> > node_vec;
		node_vec const& nodes = t.nodes();
		for (node_vec::const_iterator i = nodes.begin(), end(nodes.end());
			i != end; ++i)
		{
			std::cout << i->first << ":" << i->second << "\n";
		}
		std::cout << "trackers:\n";
		for (std::vector<announce_entry>::const_iterator i = t.trackers().begin();
			i != t.trackers().end(); ++i)
		{
			std::cout << i->tier << ": " << i->url << "\n";
		}

		std::cout << "number of pieces: " << t.num_pieces() << "\n";
		std::cout << "piece length: " << t.piece_length() << "\n";
		std::cout << "info hash: " << t.info_hash() << "\n";
		std::cout << "comment: " << t.comment() << "\n";
		std::cout << "created by: " << t.creator() << "\n";
		std::cout << "files:\n";
		for (torrent_info::file_iterator i = t.begin_files();
			i != t.end_files(); ++i)
		{
			std::cout << "  " << std::setw(11) << i->size
			<< " " << i->path.string() << "\n";
		}
		
	}
	catch (std::exception& e)
	{
  		std::cout << e.what() << "\n";
	}

	return 0;
}

