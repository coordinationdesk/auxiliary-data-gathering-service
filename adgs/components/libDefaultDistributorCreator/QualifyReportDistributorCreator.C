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
	Revision 2.10  2014/07/29 16:02:19  marpas
	qa & gcc warnings fixed
	
	Revision 2.9  2014/04/08 16:50:04  marpas
	adopting libDateTime 5.13
	
	Revision 2.8  2014/02/05 15:39:15  francesco.avanzi
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
	
	Revision 1.13  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.12  2010/04/14 13:01:29  enrcar
	EC:: Oracle11 compatibility improved
	
	Revision 1.11  2010/04/08 09:58:24  crivig
	Oracle db handled
	
	Revision 1.10  2009/09/29 14:36:06  marpas
	missing include added
	
	Revision 1.9  2009/06/25 09:19:08  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.8  2009/03/24 09:57:46  crivig
	modified in order to handle new fields in t_distributionitems for carts
	
	Revision 1.7  2009/02/03 10:09:47  crivig
	timestamp for informix, tstamp for postgres
	
	Revision 1.6  2008/11/17 11:24:00  crivig
	timestamp switched to tstamp
	
	Revision 1.5  2008/09/17 08:29:02  crivig
	bug fixed in porting
	
	Revision 1.4  2008/09/16 10:28:31  crivig
	porting to postgres8
	
	Revision 1.3  2008/04/10 14:11:04  crivig
	if problem with an item, skip it and go to the next
	
	Revision 1.2  2008/02/06 14:18:48  crivig
	bug fixed in removing old temporary distribution id
	
	Revision 1.1  2008/02/01 15:01:15  crivig
	imported new source for algo QualifyReport
	
*/


#include <QualifyReportDistributorCreator.h>
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
ACS_CLASS_DEFINE_DEBUG_LEVEL(QualifyReportDistributorCreator);
QualifyReportDistributorCreator::QualifyReportDistributorCreator(const string &algo) :
	DistributorCreator(algo),
    _agelimit(3.0) // PRQA S 4400
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// get from configuration the ageLimit after which files have to be deleted
	string key ;
	try 
	{
		getConf("AgeLimit", key, _agelimit, "") ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("Cannot find configuration value for " << key << " setting default: " <<  _agelimit);
	}
}

QualifyReportDistributorCreator::~QualifyReportDistributorCreator() throw() 
{
}



void QualifyReportDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred ) const // PRQA S 4020
{

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &dbKey = theConnection.getKey();
    int algoid = DistributionAlgo::instance(dbKey)->id(_algo); // PRQA S 3000
	
	// now select all the records from t_temporarydistribution with qualifierfile NOT NULL and algo = REPORT
	string sqlCommand;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END), \
			i.qualifierfile" ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", t.policy, t.disseminationpackingalgo_id" ; 
#endif
        sqlCommand += " FROM t_temporarydistribution  t, t_inventory i  WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand += " AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
		sqlCommand += "AND t.filename = i.filename AND t.fileversion = i.fileversion AND ";
		sqlCommand += " i.qualifierfile IS NOT NULL";
	} else if ( dialectName == "Informix9") {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.timestamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END), \
			i.qualifierfile";
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", t.policy, t.disseminationpackingalgo_id" ; 
#endif
        sqlCommand += " FROM t_temporarydistribution  t, t_inventory i  WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand += " AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
		sqlCommand += "AND t.filename = i.filename AND t.fileversion = i.fileversion AND ";
		sqlCommand += " i.qualifierfile IS NOT NULL";
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));  // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand << '\"' );
	vector <TemporaryInfo> TmpInfo; 
	queryFromTempDist(sqlCommand, "t_temporarydistribution pres w/qualifier", TmpInfo, theConnection);
	// these temporary orders together with the qualifierFile (report) can be sent in distribution...
    size_t tmpSize = TmpInfo.size() ;
	for (size_t i = 0; i<tmpSize; ++i ) 
	{
        if (stop_pred()) { return ; }

		vector <TemporaryInfo> newTmpInfo; 
		newTmpInfo.push_back(TmpInfo[i]);

		// find the report fileversion from the t_inventory
		 TemporaryInfo reportInfo(TmpInfo[i]);
		try
		{
			string sqlCommand1;
			if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
				sqlCommand1 = "SELECT MAX(fileversion) FROM t_inventory WHERE ";
				sqlCommand1 += "filename = ";
				sqlCommand1 += db::toSQL(dbKey, reportInfo.qualifierFile);    // PRQA S 3050, 3081

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand1 << '\"' );
			} else {
				ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
			}
			SQLString sqlString(dbKey, SQLString::COMMAND) ;
			sqlString.forceSQLString(sqlCommand1) ;
			dbQuery query(theConnection, sqlString, "SelLastReportVersion") ;
			dbSet set ;
			query.execForSet(set) ;
			dbRow row(dbKey) ;
			set.getNextRow(row) ;
			row.column(0,reportInfo.fileversion) ;

			reportInfo.filename = TmpInfo[i].qualifierFile ;
			newTmpInfo.push_back(reportInfo);

			// distribute both the product and the report
			sendInDistribution(newTmpInfo, theConnection);
		} 
		catch(exException &ex) 
		{
			ACS_LOG_NOTIFY_EX( ex );
			ACS_LOG_WARNING( "QualifyReportDistributorCreator::createDistribution Error creating distribution items for file "
						 << TmpInfo[i].filename   << " and file report " 
						 << TmpInfo[i].qualifierFile << " File skipped!" );
		}
	}

	// now remove all id files from t_temporarydistribution
	// that have not received a report yet but are too old: (currentDate - tstamp >= _agelimit)
    DateTime curtime ;
	DateTime::JD50 oldestDateJD50(curtime.jd50() - _agelimit);
	DateTime oldestDate(oldestDateJD50);
	string sqlCommand1;
	if(dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand1 = "SELECT t.id, t.filename FROM t_temporarydistribution  t, t_inventory i WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand1 += " AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
		sqlCommand1 += " AND t.filename = i.filename AND t.fileversion=i.fileversion AND ";
		sqlCommand1 += "  i.qualifierfile IS NULL";
		sqlCommand1 += " AND t.tstamp <= ";
		sqlCommand1 += db::toSQL(dbKey, oldestDate.getSqlString());     // PRQA S 3050, 3081
	}
	else if( dialectName == "Informix9") {
		sqlCommand1 = "SELECT t.id, t.filename FROM t_temporarydistribution  t, t_inventory i WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand1 += " AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
		sqlCommand1 += " AND t.filename = i.filename AND t.fileversion=i.fileversion AND ";
		sqlCommand1 += "  i.qualifierfile IS NULL";
		sqlCommand1 += " AND t.timestamp <= ";
		sqlCommand1 += db::toSQL(dbKey, oldestDate.getSqlString());     // PRQA S 3050, 3081
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand1 <<'\"' );
	
	SQLString sqlString2(dbKey, SQLString::COMMAND) ;
	sqlString2.forceSQLString(sqlCommand1) ;
	dbQuery queryDel(theConnection, sqlString2, "SEL t_temporarydistribution") ;
	dbSet setDel ;
	queryDel.execForSet(setDel) ;
	vector <dbPersistent::IdType> ids;
	exFWarning msg;
    size_t setDelSize = setDel.size() ;
	for (size_t i=0; i < setDelSize; ++i)
	{
		dbRow row(dbKey) ;
		setDel.getNextRow(row) ;
		dbPersistent::IdType rowId = 0 ;
		string filename;
		row.column(0,rowId) ;
		row.column(1,filename) ;
		msg << "QualifyReportDistributorCreator::createDistribution File " << filename << " not distributed! Qualifier report not received in configured timeout!\n";
		ids.push_back(rowId);
	}
	ACS_LOG_WARNING( msg);
	deleteFromTempDist(ids, theConnection);
}

_ACS_END_NAMESPACE
