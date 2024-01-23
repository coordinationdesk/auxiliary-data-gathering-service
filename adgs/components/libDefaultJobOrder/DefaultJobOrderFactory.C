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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:34:07  marpas
	adopting libException 5.x standards and coding best practices
	qa and compilation warnings removed
	
	Revision 1.1.1.1  2012/10/17 13:11:09  chicas
	imported sources
	
	
*/	


#include <DefaultJobOrderFactory.h>
#include <JobOrderFactoryChain.h>
#include <DefaultJobOrder.h>
#include <Filterables.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>

#include <defaultjoV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    defaultjoV version;

    const string _defaultJobOrderConfigKey = "DefaultJobOrder.PluginsRegistrationSettings.DefaultJobOrderFactory";
    // *************************************************************
    // ***************** EMBEDDED FACTORY **************************
    // *************************************************************

    DefaultJobOrderFactory EmbeddedDefaultJobOrderFactory;

    // *************************************************************
    // ***************** EMBEDDED FACTORY **************************
    // *************************************************************

}
///usr/acs/plugins/DataStoragePlugins


// **********************************************************************
// ***************** DSDefault JOB ORDER FACTORY **************************
// **********************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultJobOrderFactory);

DefaultJobOrderFactory::DefaultJobOrderFactory () :
		StringKeyFactory <JobOrder> (_defaultJobOrderConfigKey, &DefaultJobOrderFactory::buildDefaultJobOrder)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	if (!rs.hasKey (_defaultJobOrderConfigKey))
	{
		// No configuration found. Add the default key.
		ACS_LOG_PRIVINFO ("The DefaultJobOrder has been registered as default plugin for any job order key");
		StringKey newKey( StringKeyElement::getWildCard(), StringKeyElement::getWildCard(), StringKeyElement::getWildCard(), 
		                  StringKeyElement::getWildCard(), StringKeyElement::getWildCard());
		SingletonType::instance() -> registerFactory(newKey, &buildDefaultJobOrder);
	}
}

JobOrder*
DefaultJobOrderFactory::buildDefaultJobOrder ()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new DefaultJobOrder;
}


_ACS_END_NAMESPACE
