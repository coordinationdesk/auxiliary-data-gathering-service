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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.11  2015/12/10 10:54:25  matteo.airoldi
	Bug on ifdef PDS_OLD_SCHEMA for Postgres8 Dialect fixed
	
	Revision 2.10  2014/07/29 16:02:18  marpas
	qa & gcc warnings fixed
	
	Revision 2.9  2014/04/08 16:50:04  marpas
	adopting libDateTime 5.13
	
	Revision 2.8  2014/02/05 15:39:14  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.7  2013/05/14 18:50:49  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.6  2013/04/12 12:19:58  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.5  2013/04/09 08:22:03  marpas
	NameIdAssociator refactoring
	
	Revision 2.4  2013/02/26 18:30:47  marpas
	new dbSet I/F adopted
	
	Revision 2.3  2013/02/11 09:46:06  marpas
	using  dbConnection::localMaster method
	
	Revision 2.2  2013/02/07 17:52:27  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.1  2013/02/04 15:41:22  marpas
	disseminationpackingalgo_id transferred from temporarydistribution to distributionitems
	
	Revision 2.0  2013/01/21 18:02:23  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.18  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.17  2010/04/14 13:01:29  enrcar
	EC:: Oracle11 compatibility improved
	
	Revision 1.16  2010/04/08 09:58:24  crivig
	Oracle db handled
	
	Revision 1.15  2009/09/29 14:36:06  marpas
	missing include added
	
	Revision 1.14  2009/06/25 09:19:08  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.13  2009/06/09 08:39:47  crivig
	exception handling modified
	
	Revision 1.12  2009/05/05 14:47:36  marpas
	UNQUALIFIED_PRODUCT treated as NOT_AVAILABLE
	
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
	
	Revision 1.6  2008/02/01 15:09:56  crivig
	warning sent when a temporary distributionitem is deleted
	
	Revision 1.5  2007/03/22 16:00:58  crivig
	bug fixed in deleting records
	
	Revision 1.4  2007/03/21 16:53:35  crivig
	testing...
	
	Revision 1.3  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/


#include <QualifiedDistributorCreator.h>
#include <DistributionAlgo.h>
#include <dbConnectionPool.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <DateTime.h>
#include <dbQueryFactoryChain.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>

#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(QualifiedDistributorCreator);

QualifiedDistributorCreator::QualifiedDistributorCreator(const string &algo) :
	DistributorCreator(algo),
    _agelimit(3.0)  // PRQA S 4400
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// get from configuration the ageLimit after which files have to be deleted
	string key ;
	try 
	{
		getConf("AgeLimit", key, _agelimit, "") ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("Cannot find configuration value for " << key << " setting default to: " << _agelimit);
	}
}

QualifiedDistributorCreator::~QualifiedDistributorCreator() throw() 
{
}



void QualifiedDistributorCreator::createDistribution(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &dbKey = theConnection.getKey();
    int algoid = DistributionAlgo::instance(dbKey)->id(_algo); // PRQA S 3000
	
	// now select all the records from t_temporarydistribution with allqualified = false or orderid = NULL
	// but correctly qualified
	string sqlCommand;
	// get the algoId from t_distributionalgo table
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if(dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END) " ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", CAST (' ' as varchar(5) ), t.policy, t.disseminationpackingalgo_id" ; 
#endif
 		sqlCommand += " FROM t_temporarydistribution  t, t_users u, t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand += " AND t.userid = u.userid AND (t.orderid is NULL OR u.allqualified = ";
		sqlCommand += db::toSQL(dbKey, false);   // PRQA S 3050, 3081
		sqlCommand += " ) AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand += "AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id AND q.name NOT IN (";
		sqlCommand += db::toSQL(dbKey, "NOT_AVAILABLE");   // PRQA S 3050, 3081
		sqlCommand += " , ";
		sqlCommand += db::toSQL(dbKey, "PRODUCT_HAS_ERRORS");   // PRQA S 3050, 3081
		sqlCommand += " , ";
		sqlCommand += db::toSQL(dbKey, "UNQUALIFIED_PRODUCT");   // PRQA S 3050, 3081
		sqlCommand += ")";
	}
	else if( dialectName == "Informix9" ) {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.timestamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END) ";
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", \" \", t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand += " FROM t_temporarydistribution  t, t_users u, t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand += " AND t.userid = u.userid AND (t.orderid is NULL OR u.allqualified = ";
		sqlCommand += db::toSQL(dbKey, false);   // PRQA S 3050, 3081
		sqlCommand += " ) AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand += "AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id AND q.name NOT IN (";
		sqlCommand += db::toSQL(dbKey, "NOT_AVAILABLE");   // PRQA S 3050, 3081
		sqlCommand += " , ";
		sqlCommand += db::toSQL(dbKey, "PRODUCT_HAS_ERRORS");   // PRQA S 3050, 3081
		sqlCommand += " , ";
		sqlCommand += db::toSQL(dbKey, "UNQUALIFIED_PRODUCT");   // PRQA S 3050, 3081
		sqlCommand += ")";
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));  // PRQA S 3081
	}
	vector <TemporaryInfo> TmpInfo; 
	queryFromTempDist(sqlCommand,"t_temporarydistribution Qualified", TmpInfo, theConnection);
	// these temporary orders can be sent in distribution...
	// the files are not grouped so send them one at the time
    size_t tmpSize = TmpInfo.size() ;
	for (size_t i = 0; i<tmpSize; ++i ) 
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
            ACS_LOG_NOTIFY_EX( ex );
            appendSummary(TemporaryDistributionIdHasProblem, TmpInfo[i].id) ;
 		}
	}
	TmpInfo.clear();

	// now remove all id files from t_temporarydistribution
	// that are not grouped and not qualified properly but are too old: (currentDate - tstamp >= _agelimit)
    DateTime curtime ;
	DateTime::JD50 oldestDateJD50(curtime.jd50() - _agelimit);
	DateTime oldestDate(oldestDateJD50);
	string sqlCommand1;
	if( dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand1 = "SELECT t.id FROM t_temporarydistribution  t, t_users u, t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand1 += " AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand1 += " AND t.userid = u.userid AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id AND (((t.orderid is NULL OR u.allqualified = ";
		sqlCommand1 += db::toSQL(dbKey, false);   // PRQA S 3050, 3081
		sqlCommand1 += " )";
		sqlCommand1 += " AND q.name IN (";
		sqlCommand1 += db::toSQL(dbKey, "NOT_AVAILABLE");   // PRQA S 3050, 3081
		sqlCommand1 += " , ";
		sqlCommand1 += db::toSQL(dbKey, "UNQUALIFIED_PRODUCT");   // PRQA S 3050, 3081
		sqlCommand1 += ") AND t.tstamp <= ";
		sqlCommand1 += db::toSQL(dbKey, oldestDate.getSqlString());    // PRQA S 3050, 3081
		sqlCommand1 += " ) OR  q.name = ";
		sqlCommand1 += db::toSQL(dbKey, "PRODUCT_HAS_ERRORS");   // PRQA S 3050, 3081
		sqlCommand1 += ")";
	}
	else if( dialectName == "Informix9" ) {
		sqlCommand1 = "SELECT t.id FROM t_temporarydistribution  t, t_users u, t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand1 += " AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand1 += " AND t.userid = u.userid AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id AND (((t.orderid is NULL OR u.allqualified = ";
		sqlCommand1 += db::toSQL(dbKey, false);   // PRQA S 3050, 3081
		sqlCommand1 += " )";
		sqlCommand1 += " AND q.name IN (";
		sqlCommand1 += db::toSQL(dbKey, "NOT_AVAILABLE");   // PRQA S 3050, 3081
		sqlCommand1 += " , ";
		sqlCommand1 += db::toSQL(dbKey, "UNQUALIFIED_PRODUCT");   // PRQA S 3050, 3081
		sqlCommand1 += ") AND t.timestamp <= ";
		sqlCommand1 += db::toSQL(dbKey, oldestDate.getSqlString());    // PRQA S 3050, 3081
		sqlCommand1 += " ) OR  q.name = ";
		sqlCommand1 += db::toSQL(dbKey, "PRODUCT_HAS_ERRORS");   // PRQA S 3050, 3081
		sqlCommand1 += ")";
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand1 << "\"" );
	SQLString sqlString1(dbKey, SQLString::COMMAND) ;	
	sqlString1.forceSQLString(sqlCommand1) ;
	dbQuery queryDel(theConnection, sqlString1, "SelAllQualified") ;
	dbSet setDel ;
	queryDel.execForSet(setDel) ;
	vector <dbPersistent::IdType> ids;
    size_t setDelSize = setDel.size() ;
	for (size_t i=0; i < setDelSize; i++)
	{
		dbRow row(dbKey) ;
		setDel.getNextRow(row) ;
		int rowId = 0 ;
		row.column(0,rowId) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id with orderId NOT NULL and allqualified TRUE to delete " <<rowId);
		ids.push_back(rowId); // PRQA S 3000
	}
	deleteFromTempDist(ids, theConnection);

	
	// find all the groups of files (same order id, userid and mediatype)  with allQualifiedflag = true 
	string sqlCommand2;
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand2 = "SELECT DISTINCT  t.orderid, t.userid, t.mediatype FROM t_temporarydistribution  t, t_users u WHERE t.preselection = ";
		sqlCommand2 += db::toSQL(dbKey, true);   // PRQA S 3050, 3081
		sqlCommand2 += " AND t.userid = u.userid AND t.orderid is not  NULL AND u.allqualified = ";
		sqlCommand2 += db::toSQL(dbKey, true);    // PRQA S 3050, 3081
		sqlCommand2 += " AND t.algo = ";
		sqlCommand2 += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand2 << "\"" );
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString2(dbKey, SQLString::COMMAND) ;	
	sqlString2.forceSQLString(sqlCommand2) ;
	dbQuery queryDist(theConnection, sqlString2, "SelAllQualified") ;
	dbSet setDist ;
	queryDist.execForSet(setDist) ;

	vector <int> orderid;
	vector <string> userid;
	vector <string> mediatype;
	size_t setDistSize = setDist.size() ;
    for (size_t i=0; i < setDistSize; ++i)
	{
			
		dbRow row(dbKey) ;
		setDist.getNextRow(row) ;
		int rowOrderId = 0 ;
		string rowUserId;
		string rowMediaType;
		row.column(0,rowOrderId) ;
		row.column(1,rowUserId) ;
		row.column(2,rowMediaType) ; // PRQA S 4400
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"OrderId with orderId NOT NULL and allqualified TRUE " <<rowOrderId << '\n' <<
										   "UserId with orderId NOT NULL and allqualified TRUE " << rowUserId << '\n' <<
										   "MediaType with orderId NOT NULL and allqualified TRUE " << rowMediaType );
		orderid.push_back(rowOrderId);
		userid.push_back(rowUserId);
		mediatype.push_back(rowMediaType);
	}

	//loop on the number of groups
    size_t oidSize = orderid.size() ;
	for (size_t i = 0; i < oidSize; ++i)
	{
        if (stop_pred()) { return ; }

		// check if there is at least another record 
		// within the group not qualified
		string sqlCommand3;
		if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
			sqlCommand3 = "SELECT count(t.id) FROM t_temporarydistribution  t, t_inventory i, t_qualifiedflag q  WHERE t.orderid=";
			sqlCommand3 += db::toSQL(dbKey, orderid[i]);   // PRQA S 3050, 3081
			sqlCommand3 += " AND t.userid = ";
			sqlCommand3 += db::toSQL(dbKey, userid[i]);   // PRQA S 3050, 3081
			sqlCommand3 += " AND t.mediatype =";
			sqlCommand3 += db::toSQL(dbKey, mediatype[i]);   // PRQA S 3050, 3081
			sqlCommand3 += " AND t.algo == ";
			sqlCommand3 += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
			sqlCommand3 += " AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id AND q.name IN (";
			sqlCommand3 += db::toSQL(dbKey, "NOT_AVAILABLE");   // PRQA S 3050, 3081
			sqlCommand3 += " , ";
			sqlCommand3 += db::toSQL(dbKey, "UNQUALIFIED_PRODUCT");   // PRQA S 3050, 3081
			sqlCommand3 += " , ";
			sqlCommand3 += db::toSQL(dbKey, "PRODUCT_HAS_ERRORS");   // PRQA S 3050, 3081
			sqlCommand3 += ")";

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand3 << '\"' );
		} else {
			ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
		}
		SQLString sqlString(dbKey, SQLString::COMMAND) ;	
		sqlString.forceSQLString(sqlCommand3) ;
		dbQuery queryCount(theConnection, sqlString, "AtLeast1NotQual") ;
		dbSet setCount ;
		queryCount.execForSet(setCount) ;

		int count = 0;
        size_t setCountSize = setCount.size() ;		
		for (size_t j=0; j < setCountSize; ++j)
		{
			dbRow row(dbKey) ;
			setCount.getNextRow(row) ;
			row.column(0,count) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"number of temporary distribution not qualified " <<count  );
		}



		string sqlCommand4;
		if(dialectName == "Postgres8" || dialectName == "Oracle11" ) {
			sqlCommand4 = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
				(CASE 		\
					WHEN t.cartid IS NULL THEN -1 \
					ELSE t.cartid 		\
				END), 		\
				(CASE 		\
					WHEN t.subreq IS NULL THEN -1 \
					ELSE t.subreq 		\
				END) " ;
#ifndef PDS_OLD_SCHEMA
            sqlCommand4 += ", CAST (' ' as varchar (5) ), t.policy, t.disseminationpackingalgo_id" ; 
#endif
			sqlCommand4 += " FROM t_temporarydistribution  t WHERE t.orderid=";
			sqlCommand4 += db::toSQL(dbKey, orderid[i]);   // PRQA S 3050, 3081
			sqlCommand4 += " AND t.userid = ";
			sqlCommand4 += db::toSQL(dbKey, userid[i]);   // PRQA S 3050, 3081
			sqlCommand4 += " AND t.mediatype =";
			sqlCommand4 += db::toSQL(dbKey, mediatype[i]);   // PRQA S 3050, 3081
		}
		else if( dialectName == "Informix9" ) {
			sqlCommand4 = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.timestamp, \
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
			sqlCommand4 += db::toSQL(dbKey, orderid[i]);   // PRQA S 3050, 3081
			sqlCommand4 += " AND t.userid = ";
			sqlCommand4 += db::toSQL(dbKey, userid[i]);   // PRQA S 3050, 3081
			sqlCommand4 += " AND t.mediatype =";
			sqlCommand4 += db::toSQL(dbKey, mediatype[i]);   // PRQA S 3050, 3081
		} else {
			ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand4 << '\"' );
		TmpInfo.clear();
		queryFromTempDist(sqlCommand4, "t_temporarydistribution Qualified (orderid)", TmpInfo, theConnection);
		
		// if all records are qualified then send all the grouped records in distribution
		if (0 == count) { 
            sendInDistribution(TmpInfo, theConnection);
		}
        // otherwise check if files are to be deleted -> only if all group files are too old 
		else
		{
			exFWarning msg;
			vector<dbPersistent::IdType> deleteid;
            size_t tmpISize = TmpInfo.size() ;
			for (size_t j = 0; j < tmpISize; ++j) 
			{
				if(DateTime::JD50(TmpInfo[j].timestamp) <= oldestDateJD50) // PRQA S 3050, 3081
				{
					deleteid.push_back(TmpInfo[j].id);
					msg << "QualifiedDistributorCreator::createDistribution: File " << TmpInfo[j].filename << " not distributed! Qualifier report not received in configured timeout! \n";
				}
			}
			if (deleteid.size() == TmpInfo.size()) 
			{
				deleteFromTempDist(deleteid, theConnection);
				ACS_LOG_WARNING( msg );
			}
		}		
	} 
}



_ACS_END_NAMESPACE
