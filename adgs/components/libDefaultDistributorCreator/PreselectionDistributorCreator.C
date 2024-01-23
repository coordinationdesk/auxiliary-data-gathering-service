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

	$Prod: A.C.S. libDefaultDistributorCreator Library $

	$Id$

	$Author$

	$Log$
	Revision 2.14  2017/07/26 12:50:56  marfav
	S2PDGS-1826
	Preselection made in blocks of 50 ordered by insertion date
	SQLString used instead of direct SQL dialact statements
	
	Revision 2.13  2016/07/13 15:12:46  marfav
	CSGACQ-86 oracle sql code reviewed and fixed
	
	Revision 2.12  2016/03/26 21:36:55  marpas
	using new db I/F
	
	Revision 2.11  2014/07/29 16:02:18  marpas
	qa & gcc warnings fixed
	
	Revision 2.10  2014/03/26 15:39:12  marpas
	query fixed
	
	Revision 2.9  2014/03/26 14:56:45  marpas
	allowing runtime switching between t_distruleoriginator and t_distpoloriginator tables
	test fixed
	
	Revision 2.8  2014/02/05 15:39:14  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.7  2013/10/22 16:51:25  marpas
	query fixed
	
	Revision 2.6  2013/10/22 16:33:35  marpas
	t_distpoloriginator table moved in t_distruleoriginator
	coding best practices applied
	
	Revision 2.5  2013/05/14 18:50:49  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.4  2013/04/12 12:19:58  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.3  2013/02/26 18:30:47  marpas
	new dbSet I/F adopted
	
	Revision 2.2  2013/02/11 09:46:05  marpas
	using  dbConnection::localMaster method
	
	Revision 2.1  2013/02/07 17:52:27  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.10  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.9  2010/12/20 10:26:59  crivig
	 limit to 50 preselected records
	
	Revision 1.8  2010/04/14 13:01:29  enrcar
	EC:: Oracle11 compatibility improved
	
	Revision 1.7  2010/04/08 09:58:24  crivig
	Oracle db handled
	
	Revision 1.6  2008/09/17 08:29:02  crivig
	bug fixed in porting
	
	Revision 1.5  2008/09/16 10:28:31  crivig
	porting to postgres8
	
	Revision 1.4  2008/04/22 13:12:56  crivig
	Update transaction shortened
	
	Revision 1.3  2007/03/21 16:53:35  crivig
	testing...
	
	Revision 1.2  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/


#include <PreselectionDistributorCreator.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryFactoryChain.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>
#include <Transaction.h>
#include <Filterables.h>

#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(PreselectionDistributorCreator) 




PreselectionDistributorCreator::PreselectionDistributorCreator(const string &algo) :
	DistributorCreator(algo) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)  ;
}

PreselectionDistributorCreator::~PreselectionDistributorCreator() throw() 
{
}

void PreselectionDistributorCreator::updateTempDist(const vector<dbPersistent::IdType> &id, dbConnection &theConnection) const // PRQA S 4020, 4214
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)  ;

	if (not id.empty())
	{
		db::Transaction transaction(theConnection) ;
		try
		{
            const string &dbKey = theConnection.getKey() ;

			// construct the SQL command to update
			SQLString sqlString ( dbKey, SQLString::UPDATE ) ;

			sqlString.addTableList("t_temporaryDistribution") ;
			sqlString.addSetList( "preselection", db::toSQL(dbKey, true) );

			// Where condition
			dbQueryParameters pp(dbKey);
			pp.addParameter( dbParam(dbKey, "t_temporaryDistribution", "id", dbParam::IN, id) ); 
			sqlString.addANDWhereParameter(pp);

			dbQuery UpdateQuery(theConnection,sqlString,"update t_temporaryDistribution") ;
			transaction.begin_work("") ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"executing \"" << sqlString.getSQL() << "\"") ;


			UpdateQuery.execForStatus() ;
			transaction.commit() ; 
		}
		catch (exception&) {
			transaction.roll_back() ;
			throw ;
		}
	}
}


void PreselectionDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred) const // PRQA S 4020
{

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &dbKey = theConnection.getKey() ;
    bool polvsrule = dbSchemaMetaDataProvider::existTable("t_distpoloriginator",theConnection) ; // PRQA S 4412

	SQLString queryString ( dbKey, SQLString::SELECT) ; 

	// limit to 50 items
	queryString.setLimit(50); // PRQA S 4400

	// Add table
	queryString.addTableList ("t_temporarydistribution", "t");

	// Start selection list
	queryString.addSelectList ("t.id");
	queryString.addSelectList ("t.filename");
	queryString.addSelectList ("t.fileversion");
	queryString.addSelectList ("t.policy");

	// subquery 1
	// select count(d.dataset) from t_distpoldataset d where d.policy = t.policy
	{
		SQLString subq (dbKey, SQLString::SELECT) ; 
		// subq1 composition
		// Table
		subq.addTableList ("t_distpoldataset", "d");

		// Selection list
    	SQLString count(dbKey, SQLString::FUNCTION ) ;
	    count.setFunctionName("COUNT"); 
	    count.addFunctionParameter("d.dataset");
	    subq.addSelectList(count);	

		// Where condition
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "d", "policy", dbParam::EQUAL_TO, "t.policy") ); 
		subq.addANDWhereParameter(pp);

		// Add the subquery in the main query selection list
		queryString.addSelectList (subq);
	}

	if (not polvsrule)
	{
		// subquery 2
        // select count(o.originator_id) from t_distruleoriginator o where o.distributionrule_id = t.policy

		SQLString subq (dbKey, SQLString::SELECT) ; 
		// subq1 composition
		// Table
		subq.addTableList ("t_distruleoriginator", "o");

		// Selection list
    	SQLString count(dbKey, SQLString::FUNCTION ) ;
	    count.setFunctionName("COUNT"); 
	    count.addFunctionParameter("o.originator_id");
	    subq.addSelectList(count);	

		// Where condition
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "o", "distributionrule_id", dbParam::EQUAL_TO, "t.policy") ); 
		subq.addANDWhereParameter(pp);

		// Add the subquery in the main query selection list
		queryString.addSelectList (subq);

	}
	else
	{
		// alternative subquery 2
        // select count(o.originator) from t_distpoloriginator o where o.policy = t.policy

		SQLString subq (dbKey, SQLString::SELECT) ;
		// subq1 composition
		// Table
		subq.addTableList ("t_distpoloriginator", "o");

		// Selection list
    	SQLString count(dbKey, SQLString::FUNCTION ) ;
	    count.setFunctionName("COUNT"); 
	    count.addFunctionParameter("o.originator");
	    subq.addSelectList(count);	

		// Where condition
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "o", "policy", dbParam::EQUAL_TO, "t.policy") ); 
		subq.addANDWhereParameter(pp);

		// Add the subquery in the main query selection list
		queryString.addSelectList (subq);
	}

	// Where condition : t.preselection = " + db::toSQL(dbKey, false)
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter( dbParam(dbKey, "t", "preselection", dbParam::EQUAL_TO, db::toSQL(dbKey, false))); 
		queryString.addANDWhereParameter(pp);
	}

	// Order by condition : order by creationdate asc - (older first)
	queryString.addOrderByList("t.creationdate", SQLString::ASC) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"executing \"" << queryString.getSQL() << "\"") ;

	dbQuery query(theConnection, queryString, "SelPreselection") ;
	dbSet set ;
	query.execForSet(set) ;
	vector <int> id;
	vector <string> filename;
	vector <string> fileversion;
	vector <string> policy;
	vector <int> countmsd;
	vector <int> countor;
    size_t sizeSet = set.size() ;
	for (size_t i=0; i < sizeSet; ++i)
	{
        // PRQA S 4400 L1
		dbRow row(theConnection.getKey()) ;
		set.getNextRow(row) ;
		int rowId = 0 ;
		row.column(0,rowId) ;
		string rowfilename;
		row.column(1,rowfilename) ;
		string rowfileversion;
		row.column(2,rowfileversion) ;
		string rowpolicy;
		row.column(3,rowpolicy) ;
		int rowcountmsd = 0 ;
		row.column(4,rowcountmsd) ;
		int rowcountor = 0 ;
		row.column(5,rowcountor) ;
        // PRQA L:L1
		id.push_back(rowId);
		filename.push_back(rowfilename);
		fileversion.push_back(rowfileversion);
		policy.push_back(rowpolicy);
		countmsd.push_back(rowcountmsd);
		countor.push_back(rowcountor);
	}

	vector<dbPersistent::IdType> deleteid;
	vector<dbPersistent::IdType> updateid;
    size_t idSize = id.size() ;
	for (size_t i = 0; i < idSize; ++i)
	{
        if (stop_pred()) { return ; }
		bool msdFlag = false;
		bool orFlag = false;
		// if the measurement data set configured >0 then check if the
		// real data set measurements match at least 1 of the configured 
		if (0 != countmsd[i])
		{
		
			// faccio select secca con oggetto dbInventory....
			SQLString sqlString1(dbKey, SQLString::SELECT) ;

			SQLString count(dbKey, SQLString::FUNCTION ) ;
			count.setFunctionName("COUNT"); 
			count.addFunctionParameter("id.measdatasetid");
			sqlString1.addSelectList(count);	

			// table list
			sqlString1.addTableList ("t_inventorydataset","id");
			sqlString1.addTableList ("t_distpoldataset", "d");

			// where condition
			dbQueryParameters pp(dbKey);
			pp.addParameter( dbParam(dbKey, "id", "filename", dbParam::EQUAL_TO, db::toSQL(dbKey,filename[i]) )); 
			pp.addParameter( dbParam(dbKey, "id", "fileversion", dbParam::EQUAL_TO, db::toSQL(dbKey,fileversion[i]) )); 
			pp.addParameter( dbParam(dbKey, "d", "policy", dbParam::EQUAL_TO, policy[i] )); 
			pp.addParameter( dbParam(dbKey, "id", "measdatasetid", dbParam::EQUAL_TO, "d.dataset" )); 
			pp.addParameter( dbParam(dbKey, "id", "count", dbParam::MAJOR_OR_EQUAL_OF, "d.num" )); 
			pp.setParamOperator (dbQueryParameters::AND );
			sqlString1.addANDWhereParameter(pp);


			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executing \"" << sqlString1.getSQL() << "\"") ;

			dbQuery querymsd(theConnection, sqlString1, "SelCountMeasDataSet") ;
            dbSet msdset ;
		    querymsd.execForSet(msdset) ;
            ACS_COND_THROW(msdset.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081
			dbRow msdrow(dbKey) ;
            msdset.getNextRow(msdrow) ; // first and only 
			int countRecord = 0;
            msdrow.column(0,countRecord) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "countDataset passed:" <<countRecord) ;
			if (0 != countRecord) {
                msdFlag = true;
		    }
        }
		else {
            msdFlag = true;
        }
		// if the originator configured >0 then check if the
		// if real data originator match at least 1 of the configured 
		if (0 != countor[i])
		{


			SQLString sqlString1(dbKey, SQLString::SELECT) ;

			SQLString count(dbKey, SQLString::FUNCTION ) ;
			count.setFunctionName("COUNT"); 
			count.addFunctionParameter("id.filename");
			sqlString1.addSelectList(count);	

			// table list
			sqlString1.addTableList ("t_inventory","id");
			sqlString1.addTableList ("t_distruleoriginator", "d");

			// where condition
			dbQueryParameters pp(dbKey);
			pp.addParameter( dbParam(dbKey, "id", "filename", dbParam::EQUAL_TO, db::toSQL(dbKey,filename[i]) )); 
			pp.addParameter( dbParam(dbKey, "id", "fileversion", dbParam::EQUAL_TO, db::toSQL(dbKey,fileversion[i]) )); 
			pp.addParameter( dbParam(dbKey, "d", "policy", dbParam::EQUAL_TO, db::toSQL(dbKey,policy[i]) )); 
			pp.addParameter( dbParam(dbKey, "id", "originator", dbParam::EQUAL_TO, "d.originator_id" )); 
			pp.setParamOperator (dbQueryParameters::AND );
			sqlString1.addANDWhereParameter(pp);

			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executing \"" << sqlString1.getSQL() << "\"") ;

			dbQuery queryor(theConnection, sqlString1, "originator match" ) ;


            dbSet orset ;
		    queryor.execForSet(orset) ;
            ACS_COND_THROW(orset.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081
			dbRow orrow(dbKey) ;
            orset.getNextRow(orrow) ; // first and only 

			int countRecord = 0 ;
			orrow.column(0,countRecord) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "countOriginator passed:" <<countRecord) ;
			if (0 != countRecord) {
                orFlag = true;
            }
		}
		else {
            orFlag = true;
        }

		
		if (orFlag &&  msdFlag)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "updating policy: " << policy[i]<<  ", id: " << id[i])  ;
 			// update the preselection flag in t_temporary distribution to true
			updateid.push_back(id[i]); // PRQA S 3000
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "removing policy: " << policy[i]<<  ", id: " << id[i]) ;
			//remove record from t_temporarydistribution	
			deleteid.push_back(id[i]);  // PRQA S 3000
		}

	}
	deleteFromTempDist(deleteid, theConnection);
	updateTempDist(updateid, theConnection);
	
}


_ACS_END_NAMESPACE
