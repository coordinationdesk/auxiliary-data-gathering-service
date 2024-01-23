// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libDistributorCreator $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2014/02/05 15:51:50  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after being removed by mistake
	
	Revision 2.1  2013/02/07 12:02:04  marpas
	introducing CompletedOrderFinalizer class
	qa rules enforced
	
*/



#ifndef _CompletedOrderFinalizer_H_
#define _CompletedOrderFinalizer_H_ 

#include <acs_c++config.hpp>
#include <ConfigurationSingleton.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class dbConnection ;
class dbSet ;

class CompletedOrderFinalizer // PRQA S 2109
{		
public:

	CompletedOrderFinalizer();
	virtual ~CompletedOrderFinalizer() throw() ;
    void distributeCompletedOrders(dbConnectionPool & pool, ConfigurationSingleton::StopPredicate &, unsigned int ordersmax = 50) ; // PRQA S 4400
#ifdef CLASS_UNDER_TEST
public:
#else
private:
#endif
    void selectCompletedOrders(dbSet &, dbConnection &, unsigned int ordersmax) ;

	// declared but not implemented 
	CompletedOrderFinalizer(const CompletedOrderFinalizer&);
	CompletedOrderFinalizer & operator=(const CompletedOrderFinalizer&);	

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CompletedOrderFinalizer);
};


_ACS_END_NAMESPACE

#endif /* _CompletedOrderFinalizer_H_ */
