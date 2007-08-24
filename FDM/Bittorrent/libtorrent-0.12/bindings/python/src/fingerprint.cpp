/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
   

#include <libtorrent/fingerprint.hpp>
#include <boost/python.hpp>

void bind_fingerprint()
{
    using namespace boost::python;
    using namespace libtorrent;

    class_<fingerprint>("fingerprint", no_init)
        .def(
            init<char const*,int,int,int,int>(
                (arg("id"), "major", "minor", "revision", "tag")
            )
        )
        .def("__str__", &fingerprint::to_string)
        .def_readonly("name", &fingerprint::name)
        .def_readonly("major_version", &fingerprint::major_version)
        .def_readonly("minor_version", &fingerprint::minor_version)
        .def_readonly("revision_version", &fingerprint::revision_version)
        .def_readonly("tag_version", &fingerprint::tag_version)
        ;
}

