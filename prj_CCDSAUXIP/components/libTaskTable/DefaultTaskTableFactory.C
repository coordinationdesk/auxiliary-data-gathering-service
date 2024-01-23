#if 0

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TL Default TaskTable Factory $
	
	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.2  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.1  2010/08/03 14:28:06  nicvac
	Task Table plugins management
	
	
	
#endif


#include <DefaultTaskTableFactory.h>
#include <TaskTableFactoryChain.h>
#include <TaskTable.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultTaskTableFactory);

DefaultTaskTableFactory::DefaultTaskTableFactory () 
{
	// CTOR of the factory
	// calls the register method
	// to allow default task table instantiation
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	registerFactory();
}

DefaultTaskTableFactory::~DefaultTaskTableFactory ()  throw()
{
    // PRQA S 4631 L1
	// DTOR of the factory
	// calls the unregister method to clean up
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    try {
    
	    unregisterFactory();
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}

void
DefaultTaskTableFactory::registerFactory ()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Registering default builder") ;
	
	// Generate a key composed of wildcards
	// to use this builder in any case but the overriden ones
	StringKeyElement wildCard = StringKeyElement::getWildCard();
	StringKey defaultKey (wildCard, wildCard, wildCard, wildCard, wildCard);
	
	// Register with the default key
	TaskTableFactoryChain::instance() -> registerFactory (defaultKey, &DefaultTaskTableFactory::buildDefaultTaskTable);
}

void
DefaultTaskTableFactory::unregisterFactory ()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "un-registering default builder") ;
    
	// Generate again the default key
	StringKeyElement wildCard = StringKeyElement::getWildCard();
	StringKey defaultKey (wildCard, wildCard, wildCard, wildCard, wildCard);
	
	// Unregister the bulder from the factory chain
	TaskTableFactoryChain::instance() -> unregisterFactory (defaultKey, &DefaultTaskTableFactory::buildDefaultTaskTable);
}

TaskTable*
DefaultTaskTableFactory::buildDefaultTaskTable ()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	return new TaskTable;
}


_ACS_END_NAMESPACE
