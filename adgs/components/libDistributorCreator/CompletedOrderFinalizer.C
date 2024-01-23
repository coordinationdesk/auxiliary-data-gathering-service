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

	$Prod: libDistributorCreator $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2016/05/10 21:37:13  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.5  2014/02/06 16:10:18  francesco.avanzi
	better code practices applied
	
	Revision 2.4  2014/02/05 15:51:50  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after being removed by mistake
	
	Revision 2.3  2013/04/12 12:19:50  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.2  2013/02/26 18:26:27  marpas
	dbSet new I/F adopted
	
	Revision 2.1  2013/02/07 12:02:04  marpas
	introducing CompletedOrderFinalizer class
	qa rules enforced
	
	
*/

#include <CompletedOrderFinalizer.h>
#include <DGDBSimulationSingleton.h>
#include <Filterables.h>
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <dbQuery.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(CompletedOrderFinalizer);

CompletedOrderFinalizer::CompletedOrderFinalizer()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
}

CompletedOrderFinalizer::~CompletedOrderFinalizer() throw() 
{
}

void CompletedOrderFinalizer::distributeCompletedOrders( // PRQA S 4020
    dbConnectionPool & pool,
    ConfigurationSingleton::StopPredicate &stop_pred, 
    unsigned int ordersmax)
{
    if (ordersmax < 1) { return ; }
    
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    dbSet set ;
    selectCompletedOrders(set, theConnection, ordersmax) ;
	const string &theKey = theConnection.getKey();
	// Loop on all completed orders
    size_t rcs = set.size() ;
	for (size_t i=0; (i < rcs) && not stop_pred(); ++i) 
	{
		dbRow row(theConnection.getKey()) ;
		set.getNextRow(row) ;
		int OrderId = 0 ;
		row.column(0,OrderId) ;

		db::Transaction transaction(theConnection) ;
		transaction.begin_work("") ;

		try {
			SQLString sqlCommand(theKey,  SQLString::CUSTPROCEDURE ) ; // PRQA S 2502

			// Add Table list
			sqlCommand.setProcedureName("p_completed_order_distribution");
			sqlCommand.addProcedureParameter(OrderId);

			dbQuery CompQuery(theConnection, sqlCommand, "p_completed_order_distribution") ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand.getSQL() << '\"' );
			if (!DGDBSimulationSingleton::instance()->simulation()) {
				// remove the temporay distributions
				CompQuery.execForStatus() ;
			}
			else {
                ACS_LOG_INFO("SIMUL MODE no call to p_completed_order_distribution" );
			}


			SQLString queryString(theKey,  SQLString::DELETE );
			queryString.addTableList(theKey, "t_completed_orders") ;

			dbQueryParameters pp(theKey) ;
        	pp.addParameter( dbParam(theKey,"T_completed_orders","order_id", dbParam::EQUAL_TO , db::toSQL(theKey, OrderId))) ; // PRQA S 3050, 3081
        	queryString.addANDWhereParameter(pp) ;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << queryString.getSQL() << '\"' );

			dbQuery deleteQuery(theConnection,queryString,"t_completed_orders delete") ;

			if (!DGDBSimulationSingleton::instance()->simulation()) {
				// remove the temporay distributions
				deleteQuery.execForStatus() ;
			}
			else {
				ACS_LOG_INFO("SIMUL MODE no processed order removed" );
			}

			transaction.commit() ;
		}
		catch(exception &x)  {
			transaction.roll_back() ;
			throw;
		}
	}

}


void CompletedOrderFinalizer::selectCompletedOrders(dbSet &set, dbConnection &theConnection, unsigned int ordersmax) 
{
	// Select all completed orders from t_completed_orders
	const string &theKey = theConnection.getKey();

	SQLString sqlString (theKey, SQLString::SELECT );
	sqlString.addTableList ("t_completed_orders");
    sqlString.addSelectList( "order_id" );
	sqlString.setLimit (ordersmax);

	dbQuery query(theConnection, sqlString, "t_completed_orders limited select") ;

	query.execForSet(set) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got " << set.size() << " records" );
}


_ACS_END_NAMESPACE
