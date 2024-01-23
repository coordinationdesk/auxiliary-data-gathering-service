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

#include "pyStandaloneApp.hh"
#include <ACSTimeConvSingleton.h>
 
static std::string __revision__("$Id$");

void pyStandaloneApp::initialize(std::vector<std::string> configuration_file_names) {
  acs::SMPluginLoader::init();
  acs::StandaloneResourceSpaceFactory resourcesFactory;
  acs::rsResourceSet confset;  
  acs::MergeConfiguration m;
  for (std::vector<std::string>::const_iterator configuration_file_name = configuration_file_names.begin();
       configuration_file_name != configuration_file_names.end();
       configuration_file_name++)
    m.addSpace(*configuration_file_name) ;
  m.getResources(confset);
  if (m.getReplacements().size()) {
    std::ostringstream os ;
    os << "The following configuration spaces have been replaced:"  << std::endl ;
    for (unsigned long i =0; i < m.getReplacements().size(); i++) {
      os << "\"" << m.getReplacements()[i].first << "\" with \"" << m.getReplacements()[i].second << std::endl ;
      acs::excerr << os.str() <<std::endl;
    }
    //Exception rx(string(os.rdbuf()->str())) ;
  }
  acs::ConfigurationSingleton::instance()->set(confset);
}

pyStandaloneApp::pyStandaloneApp(std::vector<std::string> configuration_file_names) : a(0) {
  atcs = new acs::ACSTimeConvSingleton();
  initialize(configuration_file_names);
}
 
pyStandaloneApp::pyStandaloneApp(boost::python::list configuration_file_names) {
  atcs = new acs::ACSTimeConvSingleton();
  std::vector<std::string> config_fn_vec;
  for (int i=0;i!=boost::python::extract<size_t>(configuration_file_names.attr("__len__")());i++)
    config_fn_vec.push_back(std::string(boost::python::extract<char*>(configuration_file_names[i])));
  initialize(config_fn_vec);
}

pyStandaloneApp::~pyStandaloneApp() {
  delete atcs;
}

#ifdef THIS_IS_MAIN
int main(){
  std::vector<std::string> c;
  c.push_back("ACSTimeConverter") ;
  c.push_back("AuxFileRetrieve") ;
  pyStandaloneApp p(c);
  std::cout << p.a << std::endl;
  return 0;
}
#else
BOOST_PYTHON_MODULE(pyStandaloneApp) {
  boost::python::class_<pyStandaloneApp>("StandaloneApp", boost::python::init<boost::python::list>()) ;
}
#endif
