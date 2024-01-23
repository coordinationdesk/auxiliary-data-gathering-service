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

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/02/11 12:50:19  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.3  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	
	Revision 2.2  2009/04/20 17:29:07  marpas
	debug improved
	
	Revision 2.1  2009/04/20 14:17:13  marpas
	debug improved, robustness improved
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:09:06  marfav
	Import libOrderCreator
	
		

*/


#include <OrderCreatorFactory.h>
#include <OrderCreatorFactoryChain.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace acs ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderCreatorFactory)


OrderCreatorFactory::OrderCreatorFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"about to register: " << this) ;
	registerToChain() ;
}

OrderCreatorFactory::~OrderCreatorFactory() throw()
{
    // PRQA S 4631 L1
    try {
	    unregisterFromChain() ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}

void OrderCreatorFactory::registerToChain() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"registering: " << this) ;
	OrderCreatorFactoryChain::instance()->registerFactory(this) ;
}

void OrderCreatorFactory::unregisterFromChain() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"unregistering: " << this) ;
	OrderCreatorFactoryChain::instance()->unregisterFactory(this) ;
}



_ACS_END_NAMESPACE
