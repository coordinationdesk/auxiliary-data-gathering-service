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
	Revision 2.13  2017/02/08 11:35:44  marpas
	getConf (and the underlying rsResourceSet) would not raise not found exception if searching for an array. Instead it will return an empty array.
	So the code in the ctor would not emit any warning if List_Of_Qualified.Qualified array would be missing fron the conf. The same applies for missing List_Of_Waiting.Waiting but, because this is likely to happen, the warning will be emitted not before 30' from last one.
	
	Revision 2.12  2016/07/13 15:12:46  marfav
	CSGACQ-86 oracle sql code reviewed and fixed
	
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
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.5  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.4  2012/04/19 12:14:11  chicas
	Removed warning when reading configuration values
	
	Revision 1.3  2012/03/02 09:06:06  chicas
	Minor changes
	
	Revision 1.2  2012/03/01 15:42:50  chicas
	Removed allqualified flag management
	
	Revision 1.1  2012/02/29 15:21:12  chicas
	Added; to be tested
	

*/

#include <QCCReportDistributorCreator.h>
#include <DistributionAlgo.h>
#include <dbRow.h>
#include <dbConnectionPool.h>
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
ACS_CLASS_DEFINE_DEBUG_LEVEL(QCCReportDistributorCreator);


QCCReportDistributorCreator::QCCReportDistributorCreator(const string &algo) :
	DistributorCreator(algo),
    _agelimit(3.0), // PRQA S 4400
    _sendReport(false),
    _qualifiedValues(),
    _waitingsValues()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// get from configuration the ageLimit after which files have to be deleted
	string key ;

	try { getConf("AgeLimit", key, _agelimit, "") ; } 
	catch (rsResourceSet::NotFoundException &e) {
        ACS_LOG_WARNING( "no AgeLimit found in configuration, default set to: \""  << _agelimit << " days\"");
	}

	try { getConf("Report", key, _sendReport, "") ; } 
	catch (rsResourceSet::NotFoundException &e) {
        ACS_LOG_WARNING("no Report found in configuration, default set to: \""  << boolalpha << _sendReport );
	}
    //vector
    getConf("List_Of_Qualified.Qualified", key, _qualifiedValues, "") ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"_qualifiedValues contains " << _qualifiedValues.size() << " elements " );
    if (_qualifiedValues.empty()) {
        
        _qualifiedValues.push_back("QUALIFIED_PRODUCT");
        _qualifiedValues.push_back("VALID_PRODUCT");
	    
		exFWarning war ;
        war << "List_Of_Qualified.Qualified Configured defaulted to:";
        size_t qvSize = _qualifiedValues.size() ;
        for (size_t i = 0; i < qvSize; ++i) {
            if (i >0) {war << ',' ; }
            war << " \"" << _qualifiedValues[i] << '\"';
        }
        ACS_LOG_WARNING(war);
	}
    
	try { getConf("List_Of_Waiting.Waiting", key, _waitingsValues, "") ; } 
	catch (rsResourceSet::NotFoundException &e) {
        // ignore: empty vector will trace the problem
    }
    
    if (_waitingsValues.empty()) {
        static time_t w_emitted = 0 ;
        if (time(0) - w_emitted > (60*30)) { // PRQA S 4400
            ACS_LOG_WARNING("List_Of_Waiting.Waiting missing in configuration - this message will not appear before 30'" );
            w_emitted = time(0) ;
        }
    }
}

QCCReportDistributorCreator::~QCCReportDistributorCreator() throw()
{
}

string QCCReportDistributorCreator::getAlgoKeyString() const
{
	return "QCCDistrConditioned" ;
}


void QCCReportDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred ) const // PRQA S 4020 
{

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &dbKey = theConnection.getKey();
    if (0 == _qualifiedValues.size())  { //never
        return; 
    }

	// get the algoId from t_distributionalgo table

	const string dialectName = SQLDialectFactory::dialectName(dbKey);
    int algoid = DistributionAlgo::instance(dbKey)->id(_algo); // PRQA S 3000
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"algoid =  " << algoid << " for algo = " << _algo );


	// now select all the records from t_temporarydistribution 
    // correctly qualified (Default:VALID_PRODUCT,QUALIFIED_PRODUCT)
		string sqlCommand;
	if(dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END), \
            (CASE 		\
				WHEN i.qualifierfile IS NULL THEN CAST (' ' as varchar(5) ) \
				ELSE i.qualifierfile 		\
			END) " ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand += " FROM t_temporarydistribution  t, t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand += " AND t.algo = " ; 
		sqlCommand += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand += " AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id ";
        sqlCommand +="AND q.name IN (";
        sqlCommand += db::toSQL(dbKey, _qualifiedValues[0]);   // PRQA S 3050, 3081
        for (size_t i = 1; i < _qualifiedValues.size(); i++)
        {
            sqlCommand += " , ";
            sqlCommand += db::toSQL(dbKey, _qualifiedValues[i]);   // PRQA S 3050, 3081
        }
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
			END), \
            (CASE 		\
				WHEN i.qualifierfile IS NULL THEN \" \" \
				ELSE i.qualifierfile 		\
			END) " ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand += " FROM t_temporarydistribution  t,  t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand += " AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand += " AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id ";
        
        sqlCommand +="AND q.name IN (";
        sqlCommand += db::toSQL(dbKey, _qualifiedValues[0]);   // PRQA S 3050, 3081
        for (size_t i = 1; i < _qualifiedValues.size(); i++)
        {
            sqlCommand += " , ";
            sqlCommand += db::toSQL(dbKey, _qualifiedValues[i]);   // PRQA S 3050, 3081
        }
        sqlCommand += ")";
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand << '\"' );


	vector <TemporaryInfo> TmpInfo; 
	queryFromTempDist(sqlCommand, "t_temporarydistribution QCC", TmpInfo, theConnection);
    size_t tmpsize = TmpInfo.size();
    for (size_t i = 0; i< tmpsize; ++i) 
	{
        if (stop_pred()) { return ; }

		vector <TemporaryInfo> newTmpInfo; 
		newTmpInfo.push_back(TmpInfo[i]);
        try
        {

            if (_sendReport && !((TmpInfo[i].qualifierFile).empty()) )
            {
	        // find the report fileversion from the t_inventory
                TemporaryInfo reportInfo(TmpInfo[i]);
                string sqlC;
                if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
                    sqlC = "SELECT MAX(fileversion) FROM t_inventory WHERE ";
                    sqlC += "filename = ";
                    sqlC += db::toSQL(dbKey, reportInfo.qualifierFile);   // PRQA S 3050, 3081

                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand << '\"');
                } else {
                    ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
                }
                SQLString sqlString(dbKey, SQLString::COMMAND) ;
                sqlString.forceSQLString(sqlC) ;
                dbQuery query(theConnection, sqlString, "t_inventory max(fileversion)" ) ;
                dbSet set ;
                query.execForSet(set) ;
                dbRow row(dbKey) ;
                set.getNextRow(row) ;
                row.column(0,reportInfo.fileversion) ;

                reportInfo.filename = TmpInfo[i].qualifierFile ;
                newTmpInfo.push_back(reportInfo);
            }

            sendInDistribution(newTmpInfo, theConnection);
        }
        catch(exException &ex) 
        {
            ACS_LOG_NOTIFY_EX(ex);
            appendSummary(TemporaryDistributionIdHasProblem,TmpInfo[i].id) ;
            exFWarning msg;
            msg << "Error creating distribution items for file " << TmpInfo[i].filename; 
            if (_sendReport) {
                msg << " and file report " << TmpInfo[i].qualifierFile ;
            }
            msg << " File skipped!" ;
            ACS_LOG_WARNING( msg ) ;
        }
    }
    TmpInfo.clear();

    //delete and waiting

	// now remove all id files from t_temporarydistribution
    // not qualified properly
    // or  in waiting status but too old: (currentDate - tstamp >= _agelimit)

   	DateTime curtime ;
	DateTime::JD50 oldestDateJD50(curtime.jd50() - _agelimit); // PRQA S 3050, 3081
	DateTime oldestDate(oldestDateJD50);
	string sqlCommand1;

	if( dialectName == "Postgres8" || dialectName == "Oracle11" ||  dialectName == "Informix9" ) {
		sqlCommand1 = "SELECT t.id FROM t_temporarydistribution  t, t_inventory i , t_qualifiedflag q WHERE t.preselection = ";
		sqlCommand1 += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
		sqlCommand1 += " AND t.algo = ";
		sqlCommand1 += db::toSQL(dbKey, algoid);   // PRQA S 3050, 3081
		sqlCommand1 += " AND t.filename = i.filename AND t.fileversion=i.fileversion AND i.qualified = q.id AND (";
        if (not _waitingsValues.empty())
        {
            sqlCommand1 += " ( q.name IN ( ";
            sqlCommand1 += db::toSQL(dbKey, _waitingsValues[0]);   // PRQA S 3050, 3081
            for (size_t i = 1; i < _waitingsValues.size(); i++)
            {
                sqlCommand1 += " , ";
                sqlCommand1 += db::toSQL(dbKey, _waitingsValues[i]);   // PRQA S 3050, 3081
            }
            sqlCommand1 += ")";
            if (dialectName == "Postgres8" || dialectName == "Oracle11" ) {
                sqlCommand1 += " AND t.tstamp <= ";
            }
            else if( dialectName == "Informix9" ) { 
                sqlCommand1 += " AND t.timestamp <= ";
            }
            else {
                ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
            }
            
            sqlCommand1 += db::toSQL(dbKey, oldestDate.getSqlString());    // PRQA S 3050, 3081
            sqlCommand1 += " ) OR "; 
        }
		sqlCommand1 += " q.name NOT IN ( ";
        sqlCommand1 += db::toSQL(dbKey, _qualifiedValues[0]);   // PRQA S 3050, 3081
        for (size_t i = 1; i < _qualifiedValues.size(); i++)
        {
            sqlCommand1 += " , ";
            sqlCommand1 += db::toSQL(dbKey, _qualifiedValues[i]);   // PRQA S 3050, 3081
        }
        if (not _waitingsValues.empty())
        {
            size_t waitingSize = _waitingsValues.size() ;
            for (size_t i = 0; i < waitingSize; ++i)
            {
                sqlCommand1 += " , ";
                sqlCommand1 += db::toSQL(dbKey, _waitingsValues[i]);   // PRQA S 3050, 3081
            }
        }
        sqlCommand1 += ") )";
	}
	else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}

	 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand1 << '\"' );

	SQLString sqlString1(dbKey, SQLString::COMMAND) ;	
	sqlString1.forceSQLString(sqlCommand1) ;
	dbQuery queryDel(theConnection, sqlString1, "t_temporarydistribution QCC" ) ;
	dbSet setDel ;
	queryDel.execForSet(setDel) ;
	vector <dbPersistent::IdType> id;
    size_t delSize = setDel.size() ;
	for (size_t i=0; i < delSize; ++i)
	{
		dbRow row(theConnection.getKey()) ;
		setDel.getNextRow(row) ;
		dbPersistent::IdType rowId = 0;
		row.column(0,rowId) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id to delete " << rowId );
		id.push_back(rowId);
	}
	deleteFromTempDist(id, theConnection);

}

_ACS_END_NAMESPACE
