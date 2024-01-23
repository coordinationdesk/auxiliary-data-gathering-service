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

#include "pyasV.h"
using namespace acs;
static pyasV Version;

#include<string>
static std::string __revision__("$Id$");

#include <ACSStationsWrapper.h>

class pyACSStationsWrapper {
public:
  acs::ACSStationsWrapper wrapper;
  std::string getName(int id) { return wrapper.getName(id); }
};

# include <boost/python.hpp>
using namespace boost::python;
		
BOOST_PYTHON_MODULE(pyACSStations) {
class_<pyACSStationsWrapper>("pyACSStationsWrapper")
  .def("get_name",          &pyACSStationsWrapper::getName)
  ;
}
