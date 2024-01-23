// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.9  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.8  2014/03/13 11:42:21  nicvac
	S2PDGS-737: disabled log rotation from configuration.
	
	Revision 1.7  2013/10/01 13:16:23  chicas
	loadConfiguration notifyReplacement set to false
	
	Revision 1.6  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.5  2012/12/17 12:36:00  marpas
	compilation warning removed
	test compilation partially fixed
	
	Revision 1.4  2009/04/20 15:21:18  nicvac
	Propagator included.
	
	Revision 1.3  2009/01/28 17:06:26  nicvac
	Ignoring of extra Command line parameter.
	
	Revision 1.2  2008/11/13 13:20:54  marpas
	ConfigurationSingleton new approach adopted
	
	Revision 1.1  2008/10/13 14:40:44  nicvac
	Common app functions added.
	
        
*/ 

#include <PreInventoryCommonApp.h>

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectTimeConverterIncludes>
#include <ProjectPropagatorIncludes>
#endif


#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>
#include <Application.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PreInventoryCommonApp)


PreInventoryCommonApp::Constants PreInventoryCommonApp::c;


PreInventoryCommonApp::PreInventoryCommonApp(const std::string& applicationName):
   _applicationName(applicationName)
{
   static char Id[] = "@(#) ACS: $Id$";
   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;
}

PreInventoryCommonApp::~PreInventoryCommonApp() throw() 
{
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}

void PreInventoryCommonApp::getConfiguration() // PRQA S 4211
{
	//Disabled log rotation from configuration. Preinventory runs in tasktable. Log is caught by Scheduler.
	Application::instance()->setRotateLogParametersFromConf(false);

	Application::instance()->addSpace(ConfigurationSingleton::ConfSpace(_applicationName)) ; // PRQA S 3081
    // notifyReplacement set to false
	Application::instance()->loadConfiguration(false) ;
	
    vector<string> confFileToLoad(0);
    {
        rsPushNSpace confNs(ConfigurationSingleton::instance()->get(),_applicationName);

        //Main configuration contains reference to other resources to be loaded
        ConfigurationSingleton::instance()->get().getArray( "ConfigurationSpaceList.ConfigurationSpace", confFileToLoad);
    }

    //Main config file contains other resources to load
    for ( vector<string>::iterator elem= confFileToLoad.begin(); elem != confFileToLoad.end(); ++elem ) { // PRQA S 4238
        Application::instance()->addSpace(ConfigurationSingleton::ConfSpace( *elem));  // PRQA S 3081
    }

}


_ACS_END_NAMESPACE
