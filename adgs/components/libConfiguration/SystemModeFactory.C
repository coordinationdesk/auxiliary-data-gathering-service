// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.6  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.5  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.4  2012/03/01 16:00:39  marpas
	using acs::base for default static paths
	
	Revision 2.3  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.2  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.1  2008/12/15 18:43:52  marpas
	SystemMode implementation in progress
	

*/


#include <SystemModeFactory.h>
#include <SystemMode.h>
#include <Filterables.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

string SystemModeFactory::_configurationRepository = base::defaultConfPath() ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SystemModeFactory)
SystemModeFactory *SystemModeFactory::_instance = 0 ;
vector<SystemModeFactory*> SystemModeFactory::_factories ;
// constructor
SystemModeFactory::SystemModeFactory() 
{
	_factories.insert(_factories.begin(), this) ;
}


SystemModeFactory::~SystemModeFactory() throw() 
{
    // PRQA S 4631 L1
    try {
	    vector<SystemModeFactory *>::iterator tbr = find(_factories.begin(), _factories.end(), this) ;
	    if (tbr !=_factories.end()) {
		    _factories.erase(tbr) ; 
        }
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}


SystemMode *SystemModeFactory::createItem(const string &app, const string &sub)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	return new SystemMode(app,sub) ;
}

SystemMode *SystemModeFactory::newItem(const string &app, const string &sub)   // PRQA S 4020
{
	//W8115_OFF
	if (not _factories.empty()) { // uses the last factory (the last created)
		return _factories.back()->createItem(app,sub) ;
    }
	
	// if here no factory has been created until now. 
	// create one (default)
	_instance = new SystemModeFactory() ;
	
	// recursive call
	return newItem(app,sub) ;
}

string SystemModeFactory::commonConfigurationPath()     // PRQA S 4020
{
	//W8115_OFF
	if (not _factories.empty()) { // uses the last factory (the last created)
		return _factories.back()->configurationRepository() ;
	}
	// if here no factory has been created until now. 
	// create one (default)
	_instance = new SystemModeFactory() ;
	
	// recursive call
	return commonConfigurationPath() ;
}

string SystemModeFactory::configurationRepository() 
{
	return _configurationRepository ;
}

void SystemModeFactory::setConfigurationRepository(const string &cr)  // PRQA S 4121
{
	_configurationRepository = cr ;
}

const string &SystemModeFactory::getConfigurationRepository() throw() // PRQA S 4120
{
	return _configurationRepository ;
}


_ACS_END_NAMESPACE


