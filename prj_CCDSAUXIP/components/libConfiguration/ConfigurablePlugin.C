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
	Revision 2.5  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.4  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.3  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.1  2004/05/25 10:25:02  marpas
	ConfigurablePlugin class added
	documentation created
	
	
*/

#include <ConfigurablePlugin.h>
#include <ResourceSpaceFactory.h>
#include <PluginLoader.h>
#include <PluginSet.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigurablePlugin)

// constructor
ConfigurablePlugin::ConfigurablePlugin(const vector<pair<string, string> > &spaces) : ConfigMerger(spaces)
{
}

ConfigurablePlugin::ConfigurablePlugin() : ConfigMerger()
{
}


ConfigurablePlugin::~ConfigurablePlugin() throw() {}

void ConfigurablePlugin::fillLoader(PluginLoader &loader, const string &plugNSpace, rsResourceSet *set) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "plugNSpace: \"" << plugNSpace << "\""// PRQA S 3380
            << " resourceSet: " << (set ? "" : "not ") << "passed") ; 
	rsResourceSet _rs;
	
	if (!set) {
		set = &_rs ;
	}
    
	rsResourceSet &rset = *set ;
	
	getResources(rset);
    rsPushNSpace nspace(rset, plugNSpace);
	PluginSet pset;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding plugins to the PluginLoader") ;
	pset.load(rset,loader) ;
}

void ConfigurablePlugin::load(PluginLoader &loader, const string &plugNSpace, rsResourceSet *set) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "calling fillLoader") ;
    
	fillLoader(loader,plugNSpace,set) ;
	
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading plugins") ;
    
	loader.load() ;
}


_ACS_END_NAMESPACE
