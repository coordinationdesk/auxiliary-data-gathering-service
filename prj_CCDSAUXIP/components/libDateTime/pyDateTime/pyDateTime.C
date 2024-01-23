/*
Copyright 1995-2006, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.it

All Rights Reserved.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, Inc.
*/

#include "pydtV.h"
using namespace acs;
static pydtV Version;

#include <string>
static std::string __revision__("$Id$");

# include "DateTime.h"
# include <exStream.h>
# include <boost/python.hpp>
using namespace std ;
using namespace acs ;
using namespace boost::python;

class pyJD50 : public DateTime::JD50 {
public: 
  pyJD50(double d=-1) {
    if (d>=0)
      DateTime::JD50::operator=(static_cast<long double>(d));
    else
      DateTime::JD50::operator=(DateTime().jd50());
  }
};

class pyUTCD : public DateTime::UTCD {
public:
  unsigned int microsec_void(void) const { return microsec(); }
};

class pyDateTime : public DateTime {
public:
  pyDateTime(const pyJD50&j) : DateTime(j) {}
  const char * getSqlString_void(void) const { return getSqlString().c_str(); }
  const char * getSqlString_int(int n) const { return getSqlString(n).c_str(); }
  void setDefaultFractDigits(int n) { return DateTime::setDefaultFractionDigits(static_cast<unsigned int>(n)); }
  pyUTCD utcd(void) const { pyUTCD z; toUTCD(z); return z; }
};

BOOST_PYTHON_MODULE(pyDateTime) {
  class_<pyJD50>("JD50", init<double>()) 
  ;
  class_<pyUTCD>("UTCD")
    .def_readonly("year",  &pyUTCD::year)
    .def_readonly("month", &pyUTCD::month)
    .def_readonly("day",   &pyUTCD::day)
    .def_readonly("hour",  &pyUTCD::hour)
    .def_readonly("min",   &pyUTCD::min)
    .def_readonly("sec",   &pyUTCD::sec)
    .def("microsec",       &pyUTCD::microsec_void)
  ;
  class_<pyDateTime>("DateTime", init<pyJD50&>())
    .def("setDefaultFractionDigits", &pyDateTime::setDefaultFractDigits)
    .def("getSqlString",             &pyDateTime::getSqlString_void)
    .def("getSqlString",             &pyDateTime::getSqlString_int)
    .def("jd50",                     &pyDateTime::jd50)
    .def("utcd",                     &pyDateTime::utcd)
  ;
}
