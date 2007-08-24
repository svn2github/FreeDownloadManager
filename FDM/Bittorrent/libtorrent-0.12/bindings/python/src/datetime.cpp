/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

  

   

#include <boost/python.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "optional.hpp"
#include <boost/version.hpp>

using namespace boost::python;

#if BOOST_VERSION < 103400 

object import(str name)
{
    
    char *n = extract<char *>(name);
    handle<> module(borrowed(PyImport_ImportModule(n)));
    return object(module);
}

#endif

object datetime_timedelta;
object datetime_datetime;

struct time_duration_to_python
{
    static PyObject* convert(boost::posix_time::time_duration const& d)
    {
        object result = datetime_timedelta(
            0 
          , 0 
          , d.total_microseconds()
        );

        return incref(result.ptr());
    }
};

struct ptime_to_python
{
    static PyObject* convert(boost::posix_time::ptime const& pt)
    {
        boost::gregorian::date date = pt.date();
        boost::posix_time::time_duration td = pt.time_of_day();

        object result = datetime_datetime(
            (int)date.year()
          , (int)date.month()
          , (int)date.day()
          , td.hours()
          , td.minutes()
          , td.seconds()
        );

        return incref(result.ptr());
    }
};

void bind_datetime()
{
    object datetime = import("datetime").attr("__dict__");

    datetime_timedelta = datetime["timedelta"];
    datetime_datetime = datetime["datetime"];

    to_python_converter<
        boost::posix_time::time_duration
      , time_duration_to_python
    >();

    to_python_converter<
        boost::posix_time::ptime
      , ptime_to_python
    >();

    optional_to_python<boost::posix_time::ptime>();
}

