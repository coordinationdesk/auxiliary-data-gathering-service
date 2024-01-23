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
	Revision 5.0  2013/06/18 10:14:18  marpas
	adoption of libException 5.x standards
	
	Revision 2.6  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.5  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.4  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.3  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.2  2008/11/14 13:44:35  marfav
	Using always the ConfigurationSingleton element to load plugin config spaces
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.1  2004/06/04 08:22:11  marfav
	Importing ConfigurablePluginLoader
	

*/


#include <ConfigurablePluginLoader.h>
#include <ConfigMerger.h>
#include <ResourceSpace.h>
#include <File.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigurablePluginLoader)

// constructor
ConfigurablePluginLoader::ConfigurablePluginLoader(const string& loadingPath) :
    PluginLoader (loadingPath)
{
}

// destructor
ConfigurablePluginLoader::~ConfigurablePluginLoader() throw() {}

// Pre-Load plugin
bool
ConfigurablePluginLoader::preLoadPlugin (const Plugin& plugin)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START [" << File::getFileName (plugin.file()) << " - " << plugin.description() << "]") ;

	bool retval = true;

	try
	{
		// Add the configuration space to the ConfigurationSingleton too 
		// forcing the immediate load
		// This will ensure persistence of the plugin configuration spaces 
		// after the initialization
		// Note that the plugin configuration spaces are not mandatory
		ConfigurationSingleton::instance()->addSpace 
			(ConfigurationSingleton::ConfSpace (File::getFileName (plugin.file()), "", false), ConfigurationSingleton::LoadImmediate);
	}
	catch (ResourceSpace::SpaceLoadException& e)
	{
		// The configuration space does not exists... it's all ok
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exception caught: " << e) ;
	}
	catch (exception &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exception caught: " << e) ;
		ACS_COND_THROW(!plugin.optional(),PreLoadError(e, "Unable to execute preload configuration for plugin [" + plugin.file() + "]")) ; // PRQA S 3081
		retval = false;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END - Retval = " << boolalpha << retval) ;

	return retval;
}


// Post-load Plugin
bool
ConfigurablePluginLoader::postLoadPlugin (const Plugin& plugin)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning TRUE") ;

	// do nothing
	return true;
}



_ACS_END_NAMESPACE


