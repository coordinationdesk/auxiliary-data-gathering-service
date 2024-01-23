// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 5.1  2017/09/25 15:36:15  lucio.pulvirenti
	S2PDGS-1843: Passed optional boolean parameter to be passed to ResourceSpace CTOR.
	
	Revision 5.0  2013/06/18 10:14:19  marpas
	adoption of libException 5.x standards
	
	Revision 2.6  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.5  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.4  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.3  2012/03/01 16:00:39  marpas
	using acs::base for default static paths
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.2  2003/04/30 10:20:24  marpas
	using namespace std was removed from includes
	
	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
	libConfiguration import
		

*/


#include <ResourceSpaceFactory.h>
#include <ResourceSpace.h>
#include <Filterables.h>
#include <File.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

string ResourceSpaceFactory::_configurationRepository = base::defaultConfPath()  ;
string ResourceSpaceFactory::_optConfigurationRepository = base::defaultOptConfPath()  ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ResourceSpaceFactory)
ResourceSpaceFactory *ResourceSpaceFactory::_instance = 0 ;
vector<ResourceSpaceFactory*> ResourceSpaceFactory::_factories ;
// constructor
ResourceSpaceFactory::ResourceSpaceFactory() 
{
	_factories.insert(_factories.begin(), this) ;
}


ResourceSpaceFactory::~ResourceSpaceFactory() throw() 
{
    // PRQA S 4631 L1
    try {
	    vector<ResourceSpaceFactory *>::iterator tbr = find(_factories.begin(), _factories.end(), this) ;
	    if (tbr !=_factories.end()) {
		    _factories.erase(tbr) ; 
        }
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}


ResourceSpace *ResourceSpaceFactory::createItem(const string &space, const string &version, bool absPath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	return new ResourceSpace(space,version, absPath) ;
}

ResourceSpace *ResourceSpaceFactory::newItem(const string &space, const string &version, bool absPath) // PRQA S 4020
{
	// W8115_OFF
	if (not _factories.empty()) { // uses the last factory (the last created)
		return _factories.back()->createItem(space,version, absPath) ;
	}
	// if here no factory has been created until now. 
	// create one (default)
	_instance = new ResourceSpaceFactory() ;
	
	// recursive call
	return newItem(space, version, absPath) ;
}

string ResourceSpaceFactory::optionalConfigurationPath()  // PRQA S 4020
{
	// W8115_OFF
	if (not _factories.empty()) { // uses the last factory (the last created)
		return _factories.back()->optConfigurationRepository() ;
	}
	// if here no factory has been created until now. 
	// create one (default)
	_instance = new ResourceSpaceFactory() ;
	
	// recursive call
	return optionalConfigurationPath() ;
}


string ResourceSpaceFactory::commonConfigurationPath()  // PRQA S 4020
{
	// W8115_OFF
	if (not _factories.empty()) { // uses the last factory (the last created)
		return _factories.back()->configurationRepository() ;
	}
	// if here no factory has been created until now. 
	// create one (default)
	_instance = new ResourceSpaceFactory() ;
	
	// recursive call
	return commonConfigurationPath() ;
}


string ResourceSpaceFactory::configurationRepository() const
{
	return _configurationRepository ;
}

string ResourceSpaceFactory::optConfigurationRepository() const
{
	return _optConfigurationRepository ;
}



void ResourceSpaceFactory::setConfigurationRepository(const string &cr) 
{
	_configurationRepository = cr ;
}

const string &ResourceSpaceFactory::getConfigurationRepository() throw() 
{
	return _configurationRepository ;
}


void ResourceSpaceFactory::setOptConfigurationRepository(const string &cr) 
{
	_optConfigurationRepository = cr ;
}

const string &ResourceSpaceFactory::getOptConfigurationRepository() throw() 
{
	return _optConfigurationRepository ;
}


bool ResourceSpaceFactory::hasOptionalConf() const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "configurationRepository(): " << configurationRepository()) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "optConfigurationRepository(): " << optConfigurationRepository()) ;

    // multiple returns to keep is easy to read
    if (configurationRepository() != optConfigurationRepository()) {
        // check existence
        try {
            return File::isDir(optConfigurationRepository()) ;
        }
        catch(exFileOpenException &)
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, optConfigurationRepository() << " was not found or is not readable") ;
            // not exists or cannot stat 
            return false ;
        }
        catch(std::exception &e) {
            ACS_LOG_NOTIFY_EX(e) ;
            return false ;
        }
    }
    
    return false ;
}



bool ResourceSpaceFactory::hasOptionalConfDir() throw()  // PRQA S 4020
{
	// W8115_OFF
	if (not _factories.empty()) { // uses the last factory (the last created)
		return _factories.back()->hasOptionalConf() ;
	}
	// if here no factory has been created until now. 
	// create one (default)
	_instance = new ResourceSpaceFactory() ;
	
	// recursive call
	return hasOptionalConfDir() ;
}
    



_ACS_END_NAMESPACE
