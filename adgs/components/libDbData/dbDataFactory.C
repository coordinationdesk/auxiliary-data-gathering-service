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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.5  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.4  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.1  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.4  2003/10/21 10:34:38  paoscu
	staticClassName() used instead of string constant.
	
	Revision 1.3  2003/10/15 15:22:32  paoscu
	dbApplicationStatus::staticClassName() used instead string constant.
	
	Revision 1.2  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.1  2003/03/05 10:03:35  marpas
	data factory added
	
	Revision 1.7  2003/01/27 14:52:57  paoscu
	dbDistributedItem, dbDistributedItemQuery, dbProcessedOrder, dbProcessedOrderQuery eliminated
	
	Revision 1.6  2002/12/17 17:43:49  paoscu
	dbInventoryLatestValidityClosestQuery added.
	
	Revision 1.5  2002/12/16 14:06:05  danalt
	added dbOrderStateTransition table
	
	Revision 1.4  2002/12/13 18:41:06  paoscu
	Special Queries
	
	Revision 1.3  2002/11/27 11:26:04  paoscu
	More debug prints.
	
	Revision 1.2  2002/11/06 12:12:31  danalt
	aligned with libDbSpace
	
	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
	Import libDbCryosat
		

*/

#include <dbDataFactory.h>

#include <dbApplicationStatus.h>
#include <dbApplicationStatusQuery.h>

#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDataFactory);



dbDataFactory::dbDataFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START.") ;

	registerDbPersistentFactoryMethod(dbApplicationStatus::staticClassName() , &buildDbApplicationStatus );
	registerDbQueryFactoryMethod(dbApplicationStatusQuery::staticClassName() , &buildDbApplicationStatusQuery );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END.") ;

}


dbDataFactory::~dbDataFactory()
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START.") ;
    
    try {
	    unregisterDbPersistentFactoryMethod(dbApplicationStatus::staticClassName(), &buildDbApplicationStatus );
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

    try {
	    unregisterDbQueryFactoryMethod(dbApplicationStatusQuery::staticClassName() , &buildDbApplicationStatusQuery );
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END.") ;
    // PRQA L:L1

}

// *************** dbApplicationStatus ***************************

dbPersistent* dbDataFactory::buildDbApplicationStatus(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbApplicationStatus;
}


dbGeneralQuery* dbDataFactory::buildDbApplicationStatusQuery(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;	
	return new dbApplicationStatusQuery(dbKey) ;
}



_ACS_END_NAMESPACE
