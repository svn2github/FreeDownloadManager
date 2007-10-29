/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

   

#include <libtorrent/torrent.hpp>
#include <boost/python.hpp>

using namespace boost::python;
using namespace libtorrent;

void bind_torrent()
{
    class_<torrent, boost::noncopyable>("torrent", no_init);
}

