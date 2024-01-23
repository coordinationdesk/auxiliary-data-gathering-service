// PRQA S 1050 EOF
/*

	Copyright 2013-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libOrderCreator $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2014/02/12 14:32:49  francesco.avanzi
	better code practices applied
	
	Revision 5.2  2013/07/22 16:32:07  marpas
	compilation warnngs fixed
	
	Revision 5.1  2013/07/22 13:40:18  marpas
	ADC-193 implemented
	coding besta practices enforced
	compilation and qa warnings fixed
	
	Revision 5.0  2013/06/19 13:54:02  marpas
	satelliteid is now unsigned int - to be coherent with its NameIdAssociator
	OrderCreator has 2 static methods to deal with null satellite.
	libException 5.x standards adopted
	qa rules
	
	Revision 2.4  2013/05/14 18:49:51  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.3  2013/04/12 12:17:44  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.2  2013/02/26 18:12:39  marpas
	new dbSet i/F adopted
	
	Revision 2.1  2013/02/13 18:24:26  marpas
	new class introduced to execute order (application level)
	coding best practices applied
	
	
*/

#include <OrderExecutor.h>
#include <OrderCreator.h>
#include <dbGenQueryScopeGuard.h>
#include <OGDBSimulationSingleton.h>
#include <OrderCreatorFactoryChain.h>
#include <OrderCreatorFactory.h>
#include <Filterables.h>
#include <dbQueryFactoryChain.h>
#include <dbConnectionPool.h>
#include <dbOrderTypeQuery.h>
#include <dbOrderType.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SatelliteName.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderExecutor) 


OrderExecutor::OrderExecutor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
}

OrderExecutor::~OrderExecutor() throw() 
{
}

void OrderExecutor::process( // PRQA S 4020
    dbConnectionPool & pool,
    ConfigurationSingleton::StopPredicate &stop_pred )
{
	vector<string> uniqueOrderTypes ;
    dbSet set ;
    const string &dbKey = pool.getKey() ;
    {
        dbConnectionWrapper pw(pool) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
	    dbOrderTypeQuery *orderTypes = 0 ;
        const string &dbKey = theConnection.getKey() ;
	    orderTypes=dynamic_cast<dbOrderTypeQuery*>(dbQueryFactoryChain::instance()->newObject("dbOrderTypeQuery", theConnection.getKey())); // PRQA S 3081
	    dbGenQueryScopeGuard scopeG(orderTypes) ;
	    if (!orderTypes) {
		    ACS_THROW(exCriticalException("Cannot create objects of type dbOrderTypeQuery")) ; // PRQA S 3081
	    }


	    orderTypes->doQuery(theConnection) ;

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, orderTypes->size()  << " order types found") ;

	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
            ostringstream dbg ;
		    size_t osize = orderTypes->size() ;
            for (size_t i=0; i < osize; ++i) {
                if ( i > 0 ) { dbg << "\n" ; }
			    dbg<<"\t[" << i << "] " << (*orderTypes)[i]->getOrderType();
            }
            ACS_LOG_DEBUG(SimpleDebugSignature << dbg.str()) ;
	    ACS_CLASS_END_DEBUG

	    // now we have all the possible order types
	    // Let's check if in the temporary orders table there are some of them
	    string queryString ;
	    const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	    if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		    queryString = "SELECT DISTINCT OrderType FROM t_temporaryorders" ;
	    } else {
		    ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	    }
	    SQLString sqlString(dbKey, SQLString::COMMAND) ;
	    sqlString.forceSQLString(queryString) ;

	    dbQuery query(theConnection, sqlString, "TmpOrderTypes") ;
	    query.execForSet(set) ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unique order types in temporary table are " << set.size()) ;
    
    size_t rcount = set.size() ;
	for (size_t i=0; i < rcount; ++i) {
		dbRow row(dbKey) ;
		set.getNextRow(row) ;
		string oType ;
		row.column(0,oType) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t[" << i << "] " << oType) ;
 		uniqueOrderTypes.push_back(oType) ;
	}

    size_t uesize = uniqueOrderTypes.size() ;
    
	for (size_t i=0; i< uesize; ++i) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Processing order type " << uniqueOrderTypes[i]) ;
        if (stop_pred()) { return ; }
		process(uniqueOrderTypes[i], pool, stop_pred) ;
	}

	OrderCreator::emitSummary() ;
}


void OrderExecutor::process(const string &otype, dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred ) // PRQA S 4020
{
	string queryString ;
    const string &dbKey = pool.getKey() ;
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "SELECT DISTINCT OrderType, Algorithm, OrderSat FROM t_temporaryorders where OrderType = " ;
		queryString += db::toSQL(dbKey, otype) ; // PRQA S 3050
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    dbQuery query(theConnection, sqlString, "TmpOrderByType") ;
	dbSet set ;
	query.execForSet(set) ;
    
    size_t rcount = set.size() ;
	for (size_t i=0; i < rcount; ++i) {
        if (stop_pred()) { return ; }
			
		dbRow row(dbKey) ;
		set.getNextRow(row) ;
		string algo ;
		row.column(1,algo) ;
		unsigned int satId = 0 ;
		bool satIdNull = false ;
		row.column(2,satId, &satIdNull) ; // PRQA S 4400
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
			ostringstream os ;
			if (!satIdNull) {
				os << satId ;
            }
			else {
				os << "<null>" ;
            }
			ACS_LOG_DEBUG(SimpleDebugSignature << "Processing algo " << algo 
				   << " for sat Id " << os.str()) ;
		ACS_CLASS_END_DEBUG
        
		if (satIdNull) {
			satId = OrderCreator::getNullSatId() ;
        }
        
		try {
			// here fill orderType and orderAlgo with the current 
			// temporary order columns

			OrderCreator *theCreator = OrderCreatorFactoryChain::instance()->newCreator(otype,algo,satId,theConnection) ;
            ACS_SMARTPTR<OrderCreator> scopeMgr(theCreator) ; 
			if (!theCreator) {
				ostringstream os ;
				os << "Cannot find a Creator for OrderType \"" << otype << "\" "
				   << "with the \"" << algo << "\" algo "
				   << "for satId: " << satId ;
				ACS_THROW(OrderCreatorFactory::NotFoundException(os.str())); // PRQA S 3081
			}
			
			theCreator->createOrders(pool, stop_pred) ;
		}
		catch(exception &ex) {
			ACS_LOG_NOTIFY_EX(ex) ;
		}
	}
}


_ACS_END_NAMESPACE
