// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$

	$Log$
	Revision 2.9  2014/07/29 16:02:18  marpas
	qa & gcc warnings fixed
	
	Revision 2.8  2014/02/05 15:39:14  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.7  2013/05/14 18:50:49  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.6  2013/04/12 12:19:57  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.5  2013/04/09 08:22:03  marpas
	NameIdAssociator refactoring
	
	Revision 2.4  2013/02/26 18:30:47  marpas
	new dbSet I/F adopted
	
	Revision 2.3  2013/02/11 09:46:05  marpas
	using  dbConnection::localMaster method
	
	Revision 2.2  2013/02/07 17:52:26  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.1  2013/02/04 15:41:21  marpas
	disseminationpackingalgo_id transferred from temporarydistribution to distributionitems
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.17  2013/01/08 16:25:57  marfav
	Cast to string fixed in queries for postgres literals
	
	Revision 1.16  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.15  2010/04/14 13:01:29  enrcar
	EC:: Oracle11 compatibility improved
	
	Revision 1.14  2010/04/08 09:58:24  crivig
	Oracle db handled
	
	Revision 1.13  2009/06/25 09:19:08  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.12  2009/06/09 08:39:47  crivig
	exception handling modified
	
	Revision 1.11  2009/03/24 09:57:46  crivig
	modified in order to handle new fields in t_distributionitems for carts
	
	Revision 1.10  2009/02/03 10:09:47  crivig
	timestamp for informix, tstamp for postgres
	
	Revision 1.9  2008/11/17 11:24:00  crivig
	timestamp switched to tstamp
	
	Revision 1.8  2008/09/17 08:29:02  crivig
	bug fixed in porting
	
	Revision 1.7  2008/09/16 10:28:31  crivig
	porting to postgres8
	
	Revision 1.6  2007/03/29 09:40:46  crivig
	bug fixed
	
	Revision 1.5  2007/03/29 08:18:20  crivig
	clear vector after each call
	
	Revision 1.4  2007/03/21 16:53:35  crivig
	testing...
	
	Revision 1.3  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/


#include <AlwaysDistributorCreator.h>
#include <DistributionAlgo.h>
#include <dbConnectionPool.h>
#include <dbRow.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryFactoryChain.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>

#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AlwaysDistributorCreator);

AlwaysDistributorCreator::AlwaysDistributorCreator(const string &algo) :
	DistributorCreator(algo)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
}

AlwaysDistributorCreator::~AlwaysDistributorCreator() throw()
{
}


void AlwaysDistributorCreator::createDistribution(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const // PRQA S 4020
{

    
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &dbKey = theConnection.getKey();	
	// get the algoId from the t_distributionalgo
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	string sqlCommand;
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand = "SELECT id FROM t_distributionalgo WHERE name =";
		sqlCommand += db::toSQL(dbKey, _algo);    // PRQA S 3050, 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand << '\"' );
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(sqlCommand) ;
	dbQuery query(theConnection, sqlString, "SEL t_distributionalgo") ;
	dbSet set ;
	query.execForSet(set) ;
    int algoid = DistributionAlgo::instance(dbKey)->id(_algo); // PRQA S 3000
	
	// now select all the records from t_temporarydistribution with allqualified = false or orderid = NULL
	string sqlCommand1;
	if( dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand1 = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
				(CASE 		\
					WHEN t.cartid IS NULL THEN -1 \
					ELSE t.cartid 		\
				END), 		\
				(CASE 		\
					WHEN t.subreq IS NULL THEN -1 \
					ELSE t.subreq 		\
				END) " ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand1 += ", CAST (' ' as varchar(5) ) , t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand1 += " FROM t_temporarydistribution  t, t_users u WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand1 += " AND t.userid = u.userid AND (t.orderid is  NULL OR u.allqualified = ";
		sqlCommand1 += db::toSQL(dbKey, false);    // PRQA S 3050, 3081
		sqlCommand1 += " ) AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);  // PRQA S 3050, 3081
	}
	else if( dialectName == "Informix9" ) {
		sqlCommand1 = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.timestamp, \
				(CASE 		\
					WHEN t.cartid IS NULL THEN -1 \
					ELSE t.cartid 		\
				END), 		\
				(CASE 		\
					WHEN t.subreq IS NULL THEN -1 \
					ELSE t.subreq 		\
				END) " ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand1 += ", \" \", t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand1 += " 				FROM t_temporarydistribution  t, t_users u WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand1 += " AND t.userid = u.userid AND (t.orderid is  NULL OR u.allqualified = ";
		sqlCommand1 += db::toSQL(dbKey, false);    // PRQA S 3050, 3081
		sqlCommand1 += " ) AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	vector <TemporaryInfo> TmpInfo; 
	queryFromTempDist(sqlCommand1, "t_temporarydistribution Always", TmpInfo, theConnection);
	// these temporary orders can be sent in distribution...
	// the files are not grouped so send them one at the time
    size_t size = TmpInfo.size() ;
	for (unsigned int i = 0; i < size ; ++i ) 
	{
        if (stop_pred()) { return ; }

		try
		{
			vector <TemporaryInfo> newTmpInfo; 
			newTmpInfo.push_back(TmpInfo[i]);
			sendInDistribution(newTmpInfo, theConnection);
		}
		catch(exException &ex) 
		{
            ACS_LOG_WARNING("AlwaysDistributorCreator::createDistribution Exception received for t_temporarydistribution id = " << TmpInfo[i].id );
			ACS_LOG_NOTIFY_EX(ex);
		}

	}


	
	string sqlCommand2;
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand2 = "SELECT DISTINCT  t.orderid, t.userid, t.mediatype FROM t_temporarydistribution  t, t_users u WHERE t.preselection = ";
		sqlCommand2 += db::toSQL(dbKey, true);    // PRQA S 3050, 3081
		sqlCommand2 += " AND t.userid = u.userid AND t.orderid is not NULL AND u.allqualified = ";
		sqlCommand2 += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand2 += " AND t.algo = ";
		sqlCommand2 += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand2 << '\"');
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString2(dbKey, SQLString::COMMAND) ;
	sqlString2.forceSQLString(sqlCommand2) ;
	dbQuery queryDist(theConnection, sqlString2, "SEL t_temporarydistribution") ;
	dbSet setDist ;
	queryDist.execForSet(setDist) ;

	vector <int> orderid;
	vector <string> userid;
	vector <string> mediatype;
    size_t count = setDist.size() ;
	for (size_t i=0; i < count; ++i)
	{
			
		dbRow row(dbKey) ;
		setDist.getNextRow(row) ;
		int rowOrderId = 0 ;
		string rowUserId;
		string rowMediaType;
        // PRQA S 4400 4
		row.column(0,rowOrderId) ;
		row.column(1,rowUserId) ;
		row.column(2,rowMediaType) ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"OrderId with orderId NOT NULL and allqualified TRUE " << rowOrderId << 
						'\n' << "UserId with orderId NOT NULL and allqualified TRUE " << rowUserId << 
						'\n' << "MediaType with orderId NOT NULL and allqualified TRUE " << rowMediaType );
		orderid.push_back(rowOrderId);
		userid.push_back(rowUserId);
		mediatype.push_back(rowMediaType);
	}

    size_t sizeO = orderid.size() ;
	for (unsigned int i = 0; i < sizeO ; ++i)
	{
        if (stop_pred()) { return ; }
		// check if there is at least another record 
		// with algo != _algo
		string sqlCommand3;
		if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
			sqlCommand3 = "SELECT count(t.id) FROM t_temporarydistribution  t WHERE t.orderid=";
			sqlCommand3 += db::toSQL(dbKey, orderid[i]);    // PRQA S 3050, 3081
			sqlCommand3 += " AND t.userid = ";
			sqlCommand3 += db::toSQL(dbKey, userid[i]);    // PRQA S 3050, 3081
			sqlCommand3 += " AND t.mediatype =";
			sqlCommand3 += db::toSQL(dbKey, mediatype[i]);    // PRQA S 3050, 3081
			sqlCommand3 += " AND t.algo != ";
			sqlCommand3 += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand3 << '\"' );
		} else {
			ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
		}
		SQLString sqlString3(dbKey, SQLString::COMMAND) ;
		sqlString3.forceSQLString(sqlCommand3) ;
		dbQuery queryCount(theConnection, sqlString3, "SEL t_temporarydistribution") ;
		dbSet setCount ;
		queryCount.execForSet(setCount) ;

		int countNotA = 0;
        size_t countSet = setCount.size() ;		
		for (size_t j=0; j < countSet; ++j)
		{

			dbRow row(dbKey) ;
			setCount.getNextRow(row) ;
			row.column(0,countNotA) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "number of temporary distributions with algo != always " << countNotA );
		}
		// if there is at least another record of the group (same orderid, userid, mediatype)
		// with different algo, block the distribution
		if (0 == countNotA)
		{
			string sqlCommand4;
			if(dialectName == "Postgres8" || dialectName == "Oracle11" ) {
				sqlCommand4 = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate,t.tstamp, \
					(CASE 		\
						WHEN t.cartid IS NULL THEN -1 \
						ELSE t.cartid 		\
					END), 		\
					(CASE 		\
						WHEN t.subreq IS NULL THEN -1 \
						ELSE t.subreq 		\
					END) " ;
#ifndef PDS_OLD_SCHEMA
                sqlCommand4 += ", CAST (' ' as varchar (5) ) , t.policy, t.disseminationpackingalgo_id" ; 
#endif
		        sqlCommand4 += " FROM t_temporarydistribution  t WHERE t.orderid=";
				sqlCommand4 += db::toSQL(dbKey, orderid[i]);    // PRQA S 3050, 3081
				sqlCommand4 += " AND t.userid = ";
				sqlCommand4 += db::toSQL(dbKey, userid[i]);    // PRQA S 3050, 3081
				sqlCommand4 += " AND t.mediatype =";
				sqlCommand4 += db::toSQL(dbKey, mediatype[i]);    // PRQA S 3050, 3081
			}
			else if( dialectName == "Informix9" ) {
				sqlCommand4 = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate,t.timestamp, \
					(CASE 		\
						WHEN t.cartid IS NULL THEN -1 \
						ELSE t.cartid 		\
					END), 		\
					(CASE 		\
						WHEN t.subreq IS NULL THEN -1 \
						ELSE t.subreq 		\
					END) " ;
#ifndef PDS_OLD_SCHEMA
                sqlCommand4 += ", \" \", t.policy, t.disseminationpackingalgo_id" ; 
#endif
				sqlCommand4 += " FROM t_temporarydistribution  t WHERE t.orderid=";
				sqlCommand4 += db::toSQL(dbKey, orderid[i]);    // PRQA S 3050, 3081
				sqlCommand4 += " AND t.userid = ";
				sqlCommand4 += db::toSQL(dbKey, userid[i]);    // PRQA S 3050, 3081
				sqlCommand4 += " AND t.mediatype =";
				sqlCommand4 += db::toSQL(dbKey, mediatype[i]);    // PRQA S 3050, 3081

			} else {
				ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
			}
            
			TmpInfo.clear();
			queryFromTempDist(sqlCommand4, "t_temporarydistribution orderid Always", TmpInfo, theConnection);
			// these temporary orders can be sent in distribution...
			// the files are not grouped so send in distribution one at the time
            size_t sizeT = TmpInfo.size() ;
			for (unsigned int j = 0; j < sizeT; ++j ) 
			{
                if (stop_pred()) { return ; }
				try
				{
					vector <TemporaryInfo> newTmpInfo; 
					newTmpInfo.push_back(TmpInfo[j]);
					sendInDistribution(newTmpInfo, theConnection);
				}
				catch(exException &ex) 
				{
					ACS_LOG_WARNING("AlwaysDistributorCreator::createDistribution Exception received for t_temporarydistribution id = " << TmpInfo[j].id );
					ACS_LOG_NOTIFY_EX(ex);
				}
			}
		}
	} 
}


_ACS_END_NAMESPACE
