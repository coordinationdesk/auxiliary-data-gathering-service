// PRQA S 1050 EOF
/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
	Revision 2.1  2013/02/13 18:26:27  marpas
	coding best practices applied
	qa rules enforced
	new namespace model
	
	Revision 2.0  2013/01/21 17:52:32  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.3  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.2  2012/03/08 11:18:27  marpas
	refactoring
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
*/


#include <DistributorCreatorFactory.h>
#include <DistributorCreatorFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(DistributorCreatorFactory) ;

DistributorCreatorFactory::DistributorCreatorFactory() 
{
	registerToChain() ;
}

DistributorCreatorFactory::~DistributorCreatorFactory()
{
	unregisterFromChain() ; // PRQA S 4631
}

void DistributorCreatorFactory::registerToChain() 
{
	DistributorCreatorFactoryChain::instance()->registerFactory(this) ;
}

void DistributorCreatorFactory::unregisterFromChain() 
{
	DistributorCreatorFactoryChain::instance()->unregisterFactory(this) ;
}


_ACS_END_NAMESPACE
