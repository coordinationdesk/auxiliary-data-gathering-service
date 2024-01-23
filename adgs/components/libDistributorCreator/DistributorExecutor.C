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
	Revision 2.8  2016/05/10 21:37:13  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.7  2014/02/06 16:10:18  francesco.avanzi
	better code practices applied
	
	Revision 2.6  2014/02/05 15:51:51  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after being removed by mistake
	
	Revision 2.5  2013/05/14 18:50:31  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.4  2013/04/12 12:19:50  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.3  2013/02/26 18:26:27  marpas
	dbSet new I/F adopted
	
	Revision 2.2  2013/02/11 09:41:18  marpas
	using localMaster method of dbConnection
	
	Revision 2.1  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	
*/

#include <DistributorExecutor.h>
#include <DistributorCreatorFactoryChain.h>
#include <DistributorCreatorFactory.h>
#include <DistributorCreator.h>
#include <DGDBSimulationSingleton.h>
#include <Filterables.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DistributorExecutor);
DistributorExecutor::DistributorExecutor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
	
}

DistributorExecutor::~DistributorExecutor() throw() 
{
}

void DistributorExecutor::distribute( // PRQA S 4020
    dbConnectionPool & pool,
    ConfigurationSingleton::StopPredicate &stop_pred )
{
	// First perform a preselection algo
	try 
	{
		DistributorCreator *theCreator = DistributorCreatorFactoryChain::instance()->newCreator("preselection") ; // PRQA S 4412
        ACS_SMARTPTR<DistributorCreator> scopeMgr(theCreator) ; 

		if (!theCreator) {
			
			ACS_THROW(DistributorCreatorFactory::NotFoundException("Cannot find a DistributorCreator for preselection algo\"")) ;
		}

		theCreator->createDistribution(pool, stop_pred) ;
	}
	catch(exception &ex) {
		ACS_LOG_NOTIFY_EX(ex) ;
	}


	// now select all the distribution algorithm configured and apply them
	string sqlCommand ;
	const string dialectName = SQLDialectFactory::dialectName(pool.getKey());
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	string theKey = theConnection.getKey();
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand = "SELECT DISTINCT a.name FROM t_temporarydistribution t, t_distributionalgo a WHERE a.id = t.algo AND t.preselection = ";
		sqlCommand += db::toSQL(theKey, true) ; // PRQA S 3050, 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand << '\"' );
	} 
	else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}

	SQLString sqlString(theKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(sqlCommand) ;
	dbQuery query(theConnection, sqlString, "t_temporarydistribution distinct algo") ;
	dbSet set ;
	query.execForSet(set) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"got " << set.size() << " records" );

    size_t srcs = set.size() ;
	for (size_t i=0; (i < srcs) && (not stop_pred()); ++i) { // PRQA S 4238, 4244
    
		dbRow row(theKey) ;
		set.getNextRow(row) ;
		string algo ;
		row.column(0,algo) ;
		algo = StringUtils::lowercase (algo);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Processing algo " << algo );
        
		try {
			// here fill orderType and orderAlgo with the current 
			// temporary order columns
			DistributorCreator *theCreator = DistributorCreatorFactoryChain::instance()->newCreator(algo) ;
            ACS_COND_THROW( // PRQA S 3081
                0 == theCreator,
    			DistributorCreatorFactory::NotFoundException("Cannot find a DistributorCreator for algo \"" +algo + "\"")) ;
			ACS_SMARTPTR<DistributorCreator> scopeMgr(theCreator) ; 
			theCreator->createDistribution(pool, stop_pred) ;
		}
		catch(exception &ex) {
			ACS_LOG_NOTIFY_EX( ex) ;
		}
	}

	DistributorCreator::emitSummary() ;
}


_ACS_END_NAMESPACE
