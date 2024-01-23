// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/03/14 15:11:24  marpas
	removing deprecated class (dbConnectionGuard)
	
	Revision 5.2  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.9  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.8  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	
	Revision 4.7  2013/04/04 15:35:07  marpas
	statistics improved
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/10/18 17:23:46  marpas
	statistics improved
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.3  2011/05/24 13:23:18  lucio.pulvirenti
	exec: isOpen called with false not to test the connection status (isGood())
	
	Revision 3.2  2010/09/11 18:05:00  marpas
	libDbExpress 3.2 interface adopted
	
	Revision 3.1  2009/10/08 10:50:13  marpas
	dbStat class removed and substituted by new libException.Statistics
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.3  2009/01/27 13:16:19  marpas
	db statistics improved, some other minor changes with no regression
	
	Revision 2.2  2008/12/05 17:06:24  marpas
	iunused variable removed
	
	Revision 2.1  2008/11/10 16:21:59  marpas
	dbCursor implemented
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/06/22 15:27:59  enrcar
	destructor uniplemented. Fixed.
	
	Revision 1.8  2005/05/05 13:40:21  enrcar
	sql driver returned values are now filtered
	
	Revision 1.7  2005/04/27 10:09:17  enrcar
	work in progress
	
	Revision 1.6  2005/04/22 20:29:31  marpas
	work in progress
	
	Revision 1.5  2005/04/22 10:43:18  marpas
	work in progress
	
	Revision 1.4  2005/04/15 10:05:45  enrcar
	*** empty log message ***
	
	Revision 1.3  2005/03/21 14:32:35  enrcar
	*** empty log message ***
	
	Revision 1.2  2005/03/15 13:34:43  enrcar
	*** empty log message ***
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.11  2004/05/31 13:17:46  marpas
	dbLockGuard revived
	
	Revision 1.8  2003/07/11 13:08:02  marpas
	exException::clone implemented
	
	Revision 1.7  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.6  2003/04/17 14:53:53  paoscu
	ITQuery::Data() return value controlled.
	
	Revision 1.5  2003/03/10 18:38:33  paoscu
	work in progress
	
	Revision 1.4  2003/03/07 10:16:05  achval
	working in progress
	
	Revision 1.3  2003/03/06 18:30:50  paoscu
	work in progress
	
	Revision 1.2  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/


#include <dbCursor.h>
#include <dbRow.h>
#include <dbQuery.h>
#include <dbConnection.h>
#include <Statistics.h>
#include <SQLDialectFactory.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using acs::math::StatNode ;

//////////////////////
//              	// 
// class dbCursor	// 
//              	//
//////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbCursor)

dbCursor::dbCursor(dbQuery &query) :
	_connection(query.connection()),
	_sqlString(_connection.getKey(), query.sqlString()),
	_queryStatLabel(query.queryStatLabel()),
    _cursor()
{
}

dbCursor::dbCursor(dbConnection &conn,const SQLString &q, const string &queryStatLabel) :
	_connection(conn), // PRQA S 2528
	_sqlString(q),
	_queryStatLabel(queryStatLabel),
    _cursor()
{
}


dbCursor::~dbCursor() throw() {}


void dbCursor::exec()
{
	StatNode &stat = dbQuery::statN(_connection.getKey())["Cursor #"]+=1 ; // PRQA S 4412

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "the connection is: " << (!_connection.isOpen(false) ? "not " : "") << "open") ; // PRQA S 3380
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "the SQL command is : \"" << _sqlString.getSQL() << "\"") ;
	if (!_connection.isOpen(false)) { // the connection MUST be open

		// exception
		ACS_THROW(ExecuteException("dbCursor: connection closed ")) ; // PRQA S 3081
	}

	dbQuery query(_connection, _sqlString, _queryStatLabel) ;
	query.updateStat(stat,"invoked") ;
	// execute the query
	dbQuery::ErrorType eType = query.dbxExecForSet(_sqlString.getSQL(), _cursor);
	bool acceptEmptySet = true ;
	bool success = false ;
	if ( eType == dbQuery::NoError ) {
        success = true ;
    }
    else if ( (eType == dbQuery::EmptyCursor) && (acceptEmptySet) ) {
        success = true ;
    }
	else {
        ; // success = false ;
    }

    string errorString = !success ? _connection.getLastErrorText() : string("") ; // PRQA S 3081, 3384, 3385

	if (!success) { // error 

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ThreadId: " << pthread_self() 
				   << " query.dbxExecForSet: \"" << errorString << "\"") ;

		if (eType == dbQuery::PrepareError) {
			query.updateStat2(stat,"error","db error") ;
			ACS_THROW(ExecuteException(string("dbQuery: ") + " " + errorString + " Query is : \"" + _sqlString.getSQL() + "\"" )) ; // PRQA S 3081
		}
		else { // should be an ExecuteError or EmptyCursor
			query.updateStat2(stat,"error","lock") ;
			ACS_THROW(LockNullRowException(string("dbQuery: ") + " Lock Error" + errorString + " Query is : " + _sqlString.getSQL() )) ; // PRQA S 3081
		}
	}

	return ;
}

void dbCursor::open()
{

}


void dbCursor::close()
{
    _cursor.reset() ;
}


const SQLString &dbCursor::sqlString() const  // PRQA S 4120
{ 
	return _sqlString ; 
}

void dbCursor::sqlString(const SQLString &s, const string &queryStatLabel) 
{ 
	_sqlString = s ;
	_queryStatLabel = queryStatLabel ;
}


void dbCursor::getNextRow(dbRow &row) 
{
	bool locked = false ;
    int commandStatus = SQLDialect::SQLReturn(_connection.getKey(), _cursor->next()) ; // PRQA S 3010
    if (commandStatus == SQL_SUCCESS )
    {
        row.row(_cursor);    /* row is downloaded */
    }
    else if (commandStatus == DBXERR_EOF )
    {
        // EOF Found - This is not an error, but there are no more data
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Scan finished. Got status: " << commandStatus 
			   << " (" << _connection.errorText(_cursor) << ")") ;
		ACS_THROW(LockNullRowException("Scan finished." )) ; // PRQA S 3081
    }
    else
    {
        /*
            HERE IF: (commandStatus != SQL_SUCCESS) AND (commandStatus != DBXERR_EOF).

            When a table is locked, (generic) error SQL_ERROR is raised.
            So, flag locked is set TRUE below.

            (TODO: check other databases for lock-error...
            A switch can be introduced:
            SQL_ERROR -> locked
            OTHER -> not locked, so the error should be emitted).
            */
         ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Scan finished. Got status: " << commandStatus 
			   << " (" << _connection.errorText(_cursor) << ")") ;
        locked = true ;
    }
	if (locked) { // LOCK ERROR
		 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ThreadId: " << pthread_self() 
				   << " Error: " << _connection.getLastErrorText() ) ;
		// exception
		LockNullRowException e(string("dbCursor: ") + " Lock Error" +  // PRQA S 3081
                                _connection.getLastErrorText() + " " + 
								"Query is : "+ _sqlString.getSQL() ) ; 
		// updateStat(stat,"error","lock") ;
		ACS_THROW(e) ;
	}
}

_ACS_END_NAMESPACE

