// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.


	$Prod: A.C.S. Packager Optimiser Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2014/06/25 16:24:12  marpas
	coding best practices applied
	
	Revision 2.3  2014/03/10 15:00:33  francesco.avanzi
	better code practices applied
	
	Revision 2.2  2013/07/22 18:27:03  marpas
	aligning to libDbData new interface
	
	Revision 2.1  2009/03/24 14:32:09  enrcar
	EC:: c++rules
	
	Revision 2.0  2006/02/28 10:05:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/06/20 15:23:39  enrcar
	Added handling for namespace std
	
	Revision 1.3  2004/09/15 11:02:46  marpas
	the factory will not respond any more to dbDistributionItemQuery
	
	Revision 1.2  2004/09/14 16:42:22  marpas
	query i/f changed now its direct base is dbGeneralQuery
	
	Revision 1.1  2004/09/14 16:28:37  marpas
	object extended to limit query number
	


*/

#include <dbPackagerOptimiserFactory.h>

#include <dbDistributionItemExtended.h>
#include <dbDistributionItemExtendedQuery.h>

#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(dbPackagerOptimiserFactory);



dbPackagerOptimiserFactory::dbPackagerOptimiserFactory()
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);		


	// Register available factories
	registerDbPersistentFactoryMethod(dbDistributionItem::staticClassName() , &buildDbDistributionItemExtended );
	registerDbPersistentFactoryMethod(dbDistributionItemExtended::staticClassName() , &buildDbDistributionItemExtended );
	registerDbQueryFactoryMethod(dbDistributionItemExtendedQuery::staticClassName() , &buildDbDistributionItemExtendedQuery );

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);		

}


dbPackagerOptimiserFactory::~dbPackagerOptimiserFactory() throw() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	// PRQA S 4631 L1	


	// Unregister available factories
	unregisterDbPersistentFactoryMethod(dbDistributionItem::staticClassName(), &buildDbDistributionItemExtended );
	unregisterDbPersistentFactoryMethod(dbDistributionItemExtended::staticClassName(), &buildDbDistributionItemExtended );
	unregisterDbQueryFactoryMethod(dbDistributionItemExtendedQuery::staticClassName(), &buildDbDistributionItemExtendedQuery );

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);		

    // PRQA L:L1
}


// *************** dbDistributionItem ***************************

dbPersistent *dbPackagerOptimiserFactory::buildDbDistributionItemExtended(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);			
	return new dbDistributionItemExtended;
}

dbGeneralQuery *dbPackagerOptimiserFactory::buildDbDistributionItemExtendedQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);			
	return new dbDistributionItemExtendedQuery(dbKey);
}

namespace {

dbPackagerOptimiserFactory EmbeddedPackagerOptimiserFactory;

}


_ACS_END_NAMESPACE
