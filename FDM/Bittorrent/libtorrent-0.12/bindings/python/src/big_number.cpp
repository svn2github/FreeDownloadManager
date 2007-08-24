/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

  

   

#include <libtorrent/peer_id.hpp>
#include <boost/python.hpp>

void bind_big_number()
{
    using namespace boost::python;
    using namespace libtorrent;

    class_<big_number>("big_number")
        .def(self == self)
        .def(self != self)
        .def(self < self)
        .def(self_ns::str(self))
        ;
}

