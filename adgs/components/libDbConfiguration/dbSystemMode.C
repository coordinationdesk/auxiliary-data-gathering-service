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

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2016/05/11 09:38:39  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.4  2016/03/26 21:42:59  marpas
	using new db I/F
	
	Revision 5.3  2013/07/22 18:01:03  marpas
	aligning to new libDbData interface
	
	Revision 5.2  2013/07/18 11:11:53  marpas
	different exception raised in case the space was not loaded or not found in the db.
	
	Revision 5.1  2013/06/11 14:27:21  marpas
	coding best practices applied
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.12  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.11  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.10  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.9  2012/03/09 13:55:05  marpas
	avoiding useless exception notification
	
	Revision 2.8  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.7  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 2.6  2009/08/03 16:13:18  marpas
	speeding up system mode retrieval (less connections)
	
	Revision 2.5  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.4  2009/01/30 15:17:25  marpas
	ispecific exception thrown
	
	Revision 2.3  2009/01/30 15:00:53  marpas
	no more excpetions in case of no data foud, exception message improved
	
	Revision 2.2  2008/12/16 16:57:03  marpas
	VEN-54
	dbSystemMode final implementation
	
	Revision 2.1  2008/12/16 14:51:37  marpas
	dbSystemMode implemented
	

*/


#include <dbSystemMode.h>
#include <dbSystemModeFactory.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SQLString.h>
#include <Filterables.h>

#include <dbSchemaMetaDataProvider.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSystemMode)

// constructor
dbSystemMode::dbSystemMode(dbConnectionPool &cp, const string &app, const string &sub) : 
    SystemMode(app,sub),
    connectionPool_(cp) // PRQA S 2528
{
}


dbSystemMode::dbSystemMode(const dbSystemMode &o) : 
    SystemMode(o),
    connectionPool_(o.connectionPool_)  // PRQA S 2528
{
}

dbSystemMode &dbSystemMode:: operator=(const dbSystemMode &o) 
{
	if (this != &o) {
		SystemMode::operator=(o) ;
        // no connection pool copy
	}
	return *this ;
}


dbSystemMode::~dbSystemMode() throw( ) {}


string dbSystemMode::load()
{
	static bool hasChecked = false;
	static bool hasTable = false;
	// Check only once and assume no multi threaded access to load will be performed
	if (! hasChecked )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Detecting t_pdsmodes table existance") ;

		// Connect to the database anche do the schema checking
		dbConnectionWrapper pw(connectionPool_) ;
		dbConnection &theConnection = pw ;  // PRQA S 3050 
		hasTable = dbSchemaMetaDataProvider::existTable ("t_pdsmodes", theConnection);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "t_pdsmodes table " << ( hasTable ? "exists" : "does not exist") ) ;


	if ( hasTable )
	{
		// here tries to load in the following order:
		// e.g. Application is App e SubSystem is Sub
		// 1: App, Sub
		// 2: App, ""
		// 3: System, Sub
		// 4: System, ""
		string mode ;
		
		// load the modes in the same sort order used to search them (see above)
		vector<pair<string, string > > paths ;
		paths.push_back(pair<string, string >(app(),sub())) ;
		paths.push_back(pair<string, string >(app(),"")) ;
		paths.push_back(pair<string, string >(systemGlobal(),sub())) ; 
		paths.push_back(pair<string, string >(systemGlobal(),"")) ;

		ostringstream osex ;
		size_t ps = paths.size() ;
		for (size_t i=0; i < ps; ++i) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying (\"" << paths[i].first << "\",\"" <<  paths[i].second << "\")") ;
			osex << ((i != 0) ? ", " : " ") << "(\"" << paths[i].first << "\",\"" <<  paths[i].second << "\")" ; // PRQA S 3380
			try {
				mode = dbSelectMode(paths[i].first, paths[i].second) ;
				return mode ;
			}
			catch(dbQuery::QueryNoDataFound &) {
				// just ignore
			}
			catch(dbQuery::QueryExecuteException &) {
				// just ignore
			}
			catch(exception &x) {
				ACS_LOG_WARNING("Ignored exception: " << x) ;
				continue ;
			}
		}
		// if here, no mode found
		// signals it with an exception (will be ignored by Application::start in any case)
		
		ACS_THROW(ModeException("Cannot find any of the following t_modes entries: " + osex.str())) ;	 // PRQA S 3081
	}

	// if here the pdsmode table does not exists at all
	// signals it with an exception (will be ignored by Application::start in any case)
	
	ACS_THROW(ModeException("Cannot find the application mode because the t_pdsmodes table does not exists.")) ;	 // PRQA S 3081

}

string dbSystemMode::dbSelectMode(const std::string &app, const std::string &sub) 
{
    const string &dbKey = connectionPool_.getKey() ;
   	SQLString sql(dbKey, SQLString::SELECT ) ;

    sql.setLimit(1) ;
    sql.addSelectList("mode"); 
    sql.addTableList("t_pdsmodes");
	dbQueryParameters pp(dbKey) ;
	pp.addParameter(dbParam(dbKey, "t_pdsmodes","application" ,  dbParam::EQUAL_TO , dbConv(dbKey, app) )) ; // PRQA S 3050, 3081
	pp.addParameter(dbParam(dbKey, "t_pdsmodes","subsystem" ,  dbParam::EQUAL_TO , dbConv(dbKey, sub) )) ; // PRQA S 3050, 3081
	sql.addANDWhereParameter(pp) ;

    dbConnectionWrapper pw(connectionPool_) ;
	dbConnection &theConnection = pw ;  // PRQA S 3050 
    dbQuery q(theConnection, sql, "PdsModeRetrieval") ;


    dbSet set ;
	q.execForSet(set) ;
    ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081
	dbRow row(dbKey) ;
    set.getNextRow(row) ; // first and only 
	string mode ;
	row.column(0,mode) ;
	
	return mode ;


}



_ACS_END_NAMESPACE
