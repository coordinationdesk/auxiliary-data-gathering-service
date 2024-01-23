/*

	Copyright 1995-2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  A.C.S. WS Config Merger $

	$Id$

	$Author$

	$Log$
	Revision 2.11  2016/07/08 13:30:47  marfav
	CSGACQ-84
	New project/application based log rotation management strategy enforced
	
	Revision 2.10  2016/05/11 13:50:40  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.9  2015/11/19 16:43:58  marpas
	coding best practices application in progress
	
	Revision 2.8  2013/06/20 10:53:18  marfav
	Aligned to libException v5
	
	Revision 2.7  2012/02/13 11:39:53  marpas
	refactoring
	
	Revision 2.6  2010/06/15 11:35:27  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 2.5  2008/11/26 11:18:11  danalt
	Using ConfigurationSingleton instead of MergeConfiguration to load confspaces
	
	Revision 2.4  2007/04/20 10:38:35  ivafam
	Patch to remove libProc dependence of libGfeSatStore
	
	Revision 2.3  2007/04/19 08:53:55  giucas
	Removed dependencies from StationApp for ENVISAT
	
	Revision 2.2  2007/03/22 18:11:54  fracar
	fixed bug in pushNameSpace handling while loading WSConfig file
	
	Revision 2.1  2006/04/21 13:36:50  clanas
	implemented StandAlone application model
	
	Revision 2.0  2006/02/28 17:57:53  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/09/02 15:59:02  marfav
	Legacy mode read from deltaWsConfig if local wsconfig is empty
	
	Revision 1.4  2005/09/02 15:33:54  marfav
	Configuration space for OrderType management loaded
	
	Revision 1.3  2005/09/02 13:24:06  danalt
	added Cosmo project
	
	Revision 1.2  2005/02/11 08:00:18  marfav
	Makefile fixed for multimission support
	
	Revision 1.1.1.1  2004/03/12 09:16:50  marfav
	Import WS Config Merger
	
	


*/


#include <wscmV.h>

#include <WSConfigMerger.h>

#include <WSConfig.h>

#include <ConfigurationSingleton.h>
#include <exException.h>
#include <rsPushNSpace.h>
#include <XMLIstream.h>
#include <SpawnProc.h>
#include <Filterables.h>
#include <exMacros.h>
#include <Statistics.h>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef WSCONFIGMERGER_SUBSYSTEM
#define WSCONFIGMERGER_SUBSYSTEM "IPF"
#endif

#ifndef WSCONFIGMERGER_APPNAME
#define WSCONFIGMERGER_APPNAME "WSConfigMerger"
#endif 

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    wscmV version;
    Application::GenericBuilder<WSConfigMerger> theApplicationBuilder (WSCONFIGMERGER_APPNAME, WSCONFIGMERGER_SUBSYSTEM);

    const string _tldRootKey = "TlDaemon";
    const string _cfgPathKey = "TLD_WS_Config_Path";
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(WSConfigMerger)


int WSConfigMerger::usage(const string &exename) const
{
    excerr << "WS Config Merger: usage:\n";
    excerr << "\t" << exename << " -a <delta WS Config filename>\n";
    excerr << "\tor\n";
    excerr << "\t" << exename << " -d <delta WS Config filename>\n";
    return -1;
}



int WSConfigMerger::main(int argc, char const * const * argv, char const * const * env)
{
    if ( (argc != 3) || (argv[1][0]!='-') || ((argv[1][1]!='a')&&(argv[1][1]!='d')) )
    {
        // Disable statistics
        acs::math::Statistics::instance()->enableStatistics(false) ;
        return usage(argv[0]);
    }

	//rotateLog() ; // open the first log
	
	/////////////////////////////////////////
	SpawnProc spawnproc("") ; // just to force library use
	/////////////////////////////////////////
	
	_deltaWSConfigPath = argv[2];

	if (argv[1][1] == 'a')
	{
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Running in append mode");
	    _mergeMode = MM_APPEND;
    }
	else // must be 'd': we're in the else branch of an if, in the end... ;-)
	{
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Running in delete mode");
	    _mergeMode = MM_DELETE;
    }

    try
    {
      run ();
    }
    catch(exException &e)
    {
        ACS_LOG_NOTIFY_EX(e);
        ACS_LOG_INFO ("WS Config Merger: exiting return code -1");
        return -1;
    }
    catch(...)
    {
        ACS_LOG_INFO ("WS Config Merger: unknown exception caught");
        ACS_LOG_INFO ("WS Config Merger: exiting return code -1");
        return -1;
    }

    ACS_LOG_INFO ("WS Config Merger: exiting return code 0");
    return 0;
}


WSConfigMerger::WSConfigMerger (const string &appName, const string &subSystem) :
#if defined (HAS_MISSION_INCLUDES)
            PROJECT_APP(appName, subSystem),
#elif STANDALONE
            StandaloneApp(appName, subSystem),
#elif COSMO
		    CosmoApp(appName, subSystem),
#else 
		    CryosatApp(appName, subSystem, true),
#endif 
		    _WSConfigPath(),
            _deltaWSConfigPath(),
            _wsConfiguration(NULL),
		    _wsDeltaConfiguration(NULL),
            _mergeMode()
{
}

WSConfigMerger::~WSConfigMerger() throw() 
{
  delete _wsConfiguration;
  _wsConfiguration = 0;
}



static const char* namespaces[] =
{
  "tlDaemon",
  "OrderTypeManager"
};



void
WSConfigMerger::loadTLConfiguration()
{
  //first: load general configuration spaces

  rsResourceSet rs;

  for (size_t i=0; i<sizeof(namespaces)/sizeof(namespaces[0]); i++)
  {
    ACS_LOG_INFO ("WS Config Merger: requesting configuration space " << namespaces[i]);
    addSpace(ConfigurationSingleton::ConfSpace(namespaces[i])) ;
  }

  // do the actual load
  ACS_LOG_INFO ("WS Config Merger: loading configuration spaces");
  loadConfiguration();
	rotateLog() ;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "configuration spaces loaded");

}

void
WSConfigMerger::loadWSConfiguration()
{
    // retrieve WS cfg file pathname from the configuration and create the WSConfig object
    {
        rsResourceSet& rs( ConfigurationSingleton::instance() -> get() );
        rsPushNSpace nspace_tld (rs, _tldRootKey);

        rs.getValue(_cfgPathKey, _WSConfigPath);
        ACS_LOG_INFO ("WS Config Merger: loading ws configuration file " << _WSConfigPath);
    }

    // avoid creating a new WSConfig object during the reload of the configuration spaces
    if (_wsConfiguration)
        _wsConfiguration->load(_WSConfigPath);
    else
        _wsConfiguration = new WSConfig(_WSConfigPath);
}


void
WSConfigMerger::loadDeltaWSConfiguration()
{
    ACS_LOG_INFO ("WS Config Merger: loading delta ws configuration file " << _deltaWSConfigPath);
    _wsDeltaConfiguration = new WSConfig(_deltaWSConfigPath);

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "WS Config Merger:: delta ws configuration file contains " << _wsDeltaConfiguration->size() << " elements");
}


void
WSConfigMerger::run()
{
  // initialise
    {
      // first, set up configuration stuff

      ACS_LOG_INFO ("WS Config Merger: loading TL Configuration");

      try
      {
        loadTLConfiguration();
      }                
      catch (const exException& toCatch) 
      {
        WSConfigMergerException e(toCatch, "Cannot load Thin Layer configuration settings: initialisation failed ") ;
        ACS_THROW (e) ;
      }
    }


      // load WS Configuration File of this node
      ACS_LOG_INFO ("WS Config Merger: loading WS Configuration");

      try
      {
        loadWSConfiguration();
      }                
      catch (const exException& toCatch) 
      {
        WSConfigMergerException e(toCatch, "Cannot load WS Configuration file: initialisation failed ") ;
        ACS_THROW (e) ;
      }

      // load delta WS Configuration File coming from command line
      try
      {
        loadDeltaWSConfiguration();
      }                
      catch (const exException& toCatch) 
      {
        WSConfigMergerException e(toCatch, "Cannot load delta WS Configuration file: initialisation failed ") ;
        ACS_THROW (e) ;
      }

      ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "WS Config Merger::run: executed initialisazion");

    // select the correct legacy mode
    bool legacyMode;
    if (_wsConfiguration->size() == 0)
    {
      ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Local ws config has zero size. Legacy mode read from deltaWsconfig file");
	  legacyMode = _wsDeltaConfiguration->isLegacy();
    }	  
    else
    {
      ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "legacy mode read from local ws config file");
	  legacyMode = _wsConfiguration->isLegacy();
    }

    // do actual merge
    if (_mergeMode == MM_APPEND)
    {
      ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "appending " << _deltaWSConfigPath << " to " << _WSConfigPath);
      mergeConfigurations(*_wsConfiguration, *_wsDeltaConfiguration); 
    }
    else // _mergeMode == MM_DELETE
    {
      ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "subtracting " << _deltaWSConfigPath << " from " << _WSConfigPath);
      subtractConfiguration(*_wsConfiguration, *_wsDeltaConfiguration);
    }

    // write file
    
      ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "WS Config Merger::run: destination legacy mode set to " << (legacyMode?"ON":"OFF"));

    _wsConfiguration->setLegacy(legacyMode);
    ofstream of(_WSConfigPath.c_str());

    of << *_wsConfiguration  ;
}



string WSConfigMerger::getNodeName()
{
  return hostname();
}

static
bool compareNameVers(const WSConfigElement &e1, const WSConfigElement &e2)
{
    return (e1.name == e2.name) && (e1.version == e2.version);
}

void WSConfigMerger::mergeConfigurations(WSConfig &_wsConfiguration, const WSConfig &_wsDeltaConfiguration)
{
    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB,"Entered. Target config has " << _wsConfiguration.size() << " processors")

    // first: check if overwriting some of the existing elements; in this case, error!
    vector<WSConfigElement>::iterator p = find_first_of(_wsConfiguration.begin(), _wsConfiguration.end(),
							_wsDeltaConfiguration.begin(), _wsDeltaConfiguration.end(),
							compareNameVers);

    if (p != _wsConfiguration.end())
    {
        WSConfigMergerException e("trying to replace esisting processor " + p->name + " " + p->version) ;
        ACS_THROW (e) ;
    }
    
    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB,"copying " << _wsDeltaConfiguration.size() << " processors")

	std::vector<WSConfigElement> deltaElements = _wsDeltaConfiguration.getWSConfigElements ();
	for (size_t t = 0; t < deltaElements.size(); ++t)
	{
		_wsConfiguration.push_back(deltaElements[t]);
	}

    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB,"copy done. target config has " << _wsConfiguration.size() << " processors")

}

class proc_finder
{
	const WSConfig& _m;

    public:
	explicit proc_finder(const WSConfig& mm)
		: _m(mm)
	{
	}

	bool operator() (const WSConfigElement& e)
	{
	    WSConfigElement temp;
	    bool discardIt = _m.getProcessor(e.name, e.version, temp);

       ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB,"processor " << e.name << ' ' <<  e.version << " is " << (discardIt? "" : "not") << " being discarded")

	    return discardIt;
	}

    private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL (proc_finder)
};
ACS_CLASS_DEFINE_DEBUG_LEVEL(proc_finder)


void WSConfigMerger::subtractConfiguration(WSConfig &_wsConfiguration, const WSConfig &_wsDeltaConfiguration)
{
    // remove processors in _wsConfiguration that have a match in _wsDeltaConfiguration
    vector<WSConfigElement>::iterator p = remove_if(_wsConfiguration.begin(), _wsConfiguration.end(), proc_finder(_wsDeltaConfiguration));
    _wsConfiguration.erase(p, _wsConfiguration.end());
}

_ACS_END_NAMESPACE
