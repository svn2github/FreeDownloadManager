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
#include "libtorrent/file.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/file_pool.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::filesystem;
using namespace libtorrent;

void add_files(
	torrent_info& t
	, path const& p
	, path const& l)
{
	if (l.leaf()[0] == '.') return;
	path f(p / l);
	if (is_directory(f))
	{
		for (directory_iterator i(f), end; i != end; ++i)
			add_files(t, p, l / i->leaf());
	}
	else
	{
		std::cerr << "adding \"" << l.string() << "\"\n";
		t.add_file(l, file_size(f));
	}
} 

int main(int argc, char* argv[])
{
	using namespace libtorrent;
	using namespace boost::filesystem;

	path::default_name_check(no_check);

	if (argc != 4 && argc != 5)
	{
		std::cerr << "usage: make_torrent <output torrent-file> "
			"<announce url> <file or directory to create torrent from> "
			"[url-seed]\n";
		return 1;
	}

	try
	{
		torrent_info t;
		path full_path = complete(path(argv[3]));
		ofstream out(complete(path(argv[1])), std::ios_base::binary);

		int piece_size = 256 * 1024;
		char const* creator_str = "libtorrent";

		add_files(t, full_path.branch_path(), full_path.leaf());
		t.set_piece_size(piece_size);

		file_pool fp;
		storage st(t, full_path.branch_path(), fp);
		t.add_tracker(argv[2]);

		
		int num = t.num_pieces();
		std::vector<char> buf(piece_size);
		for (int i = 0; i < num; ++i)
		{
			st.read(&buf[0], i, 0, t.piece_size(i));
			hasher h(&buf[0], t.piece_size(i));
			t.set_hash(i, h.final());
			std::cerr << (i+1) << "/" << num << "\r";
		}

		t.set_creator(creator_str);

		if (argc == 5)
			t.add_url_seed(argv[4]);

		
		entry e = t.create_torrent();
		libtorrent::bencode(std::ostream_iterator<char>(out), e);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	return 0;
}

