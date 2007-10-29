/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

   

#include <libtorrent/version.hpp>
#include <boost/python.hpp>

using namespace boost::python;

void bind_version()
{
    scope().attr("version") = LIBTORRENT_VERSION;
    scope().attr("version_major") = LIBTORRENT_VERSION_MAJOR;
    scope().attr("version_minor") = LIBTORRENT_VERSION_MINOR;
}

