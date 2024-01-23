#ifndef PYSTANDALONEAPP_HH
#define PYSTANDALONEAPP_HH

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

#include <string>

#include <rsResourceSet.h>
#include <XMLIstream.h>
#include <exException.h>
#include <exFMessage.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <ConfigurationSingleton.h>

#include <MergeConfiguration.h>

#include <ACSTimeConvSingleton.h>
#include <TimeConverterSingleton.h>
#include <TimeConverter.h>
#include <SMPluginLoader.h>

#include "AuxFileRetrieve.h"
#include "AuxFileInsert.h"
#include <StandaloneResourceSpaceFactory.h>

#include <boost/python.hpp>
#include <boost/python/list.hpp>

class pyStandaloneApp {
private:
  void initialize(std::vector<std::string> configuration_file_names);
public:
  int a;
  acs::ACSTimeConvSingleton * atcs;
  pyStandaloneApp(std::vector<std::string> configuration_file_names);
  pyStandaloneApp(boost::python::list configuration_file_names);
  virtual ~pyStandaloneApp();
};

#endif // PYSTANDALONEAPP_HH
