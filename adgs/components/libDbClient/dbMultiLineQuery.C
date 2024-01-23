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
	Revision 5.4  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.3  2013/07/02 11:59:19  marpas
	minor changes: debug and statistics"
	
	Revision 5.2  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.6  2013/03/28 13:40:10  marpas
	enforcing dbConnection use
	
	Revision 4.5  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.4  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/10/20 13:45:00  lucio.pulvirenti
	Statistics improved
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.1  2011/03/01 15:47:03  marpas
	GCC 4.4.x support
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2009/07/13 16:41:52  enrcar
	EC:: c++rules
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/07/08 16:18:11  enrcar
	SQLString added
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.2  2004/05/18 15:54:44  marpas
	dbLockGuard no longer exists in libDbBasic
	
	Revision 1.1  2003/09/26 09:41:35  paoscu
	dbMultiLineQuery added.
	
*/

#include <dbMultiLineQuery.h>
#include <dbRow.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <Filterables.h>
#include <exStream.h>


///////////////////////////////
//              	         //
// class dbMultiLineQuery	 //
//              	         //
///////////////////////////////



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbMultiLineQuery)

dbMultiLineQuery::dbMultiLineQuery(const string &stat, int stmtnum, dbConnection &conn, istream& st) :
	_connection(conn), // PRQA S 2528
    _sqlString(),
    _sqlVector(),
    statString_(stat),
    stmtnum_(stmtnum)
{
	sqlString(st) ;
}


dbMultiLineQuery::dbMultiLineQuery(const string &stat, int stmtnum, dbConnection &conn,const string &st) :
	_connection(conn), // PRQA S 2528
	_sqlString(st),
    _sqlVector(),
    statString_(stat),
    stmtnum_(stmtnum)
{
}



dbMultiLineQuery::~dbMultiLineQuery() throw() { }


dbMultiLineQuery::dbMultiLineQuery(const dbMultiLineQuery &dbq) :
	_connection(dbq._connection), // PRQA S 2528
	_sqlString(dbq._sqlString),
    _sqlVector(dbq._sqlVector),
    statString_(dbq.statString_),
    stmtnum_(dbq.stmtnum_)
{
}



dbMultiLineQuery &dbMultiLineQuery::operator=(const dbMultiLineQuery &dbq)
{
	if (this != &dbq) { // do not copy the connection
		_sqlString = dbq._sqlString ;
        _sqlVector= dbq._sqlVector ;
        statString_ = dbq.statString_ ;
        stmtnum_ = dbq.stmtnum_ ;
	}

	return *this ;
}


void dbMultiLineQuery:: sqlString(istream &is)
{
	_sqlString.clear() ;
	string aux ;
	while(getline(is,aux)) // PRQA S 3050
	{
		removeComments(aux) ;
		if(notEmpty(aux)) {
			_sqlString+=aux + " "	;	/* Add the value to _sqlString */
        }
	}

}


bool dbMultiLineQuery::execForStatus()
{
	parseSql();	/* fill the _sqlVector */
	
	bool res=false;
	size_t vsize = _sqlVector.size() ;
	for(size_t i=0; i<vsize; ++i)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "statement #" << stmtnum_ << " statString_ = \"" << statString_ << "\".") ;
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "query["<<i<<"] = \"" <<_sqlVector[i] << "\".") ;
  
        SQLString tmp(_connection.getKey(), SQLString::COMMAND);	// Execute the command(s)
        tmp.forceSQLString(_sqlVector[i]) ;
        ostringstream os ;
        os << "MQ[" << stmtnum_ << '.' << i << "] " << statString_  ;
        dbQuery query(_connection, tmp, os.str()) ;

		res=query.execForStatus() ;
	}
	return res;
}

void dbMultiLineQuery::execForSet(dbSet &set)
{
	parseSql();
	
	size_t vsize = _sqlVector.size() ;
	for(size_t i=0; i<vsize; ++i)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "statement #" << stmtnum_ << " statString_ = \"" << statString_ << "\".") ;
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "query["<<i<<"] = \"" <<_sqlVector[i] << "\".") ;
        SQLString tmp(_connection.getKey(), SQLString::COMMAND);	// Execute the command(s)
        tmp.forceSQLString(_sqlVector[i]) ;
        ostringstream os ;
        os << "MQ[" << stmtnum_ << '.' << i << "] " << statString_ ;
        dbQuery query(_connection, tmp, os.str()) ;

		if(i == (_sqlVector.size() -1 )) { // PRQA S 3084
			query.execForSet(set) ;
        }
		else {
			query.execForStatus() ;
        }
	}
}

void dbMultiLineQuery::parseSql() // PRQA S 4020
{
	_sqlVector.clear() ;
	if(_sqlString.empty()) {
		return ;	/* Nothing to do! */
    }

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "_sqlString = \"" << _sqlString << "\".") ;
	
	unsigned long count=0;
	size_t i=0;
	while(i != _sqlString.npos ) 
	{
		size_t k = _sqlString.find(';' , i) ;	/* Look for a command terminator */
		string singleQuery=string(_sqlString , i , k-i ) ; // PRQA S 3084
		
		if(notEmpty(singleQuery))
		{
			_sqlVector.push_back( singleQuery ) ;	/* Add the command to _sqlVector */
		
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "query[" << count << "] = \"" << singleQuery << "\""
					   <<" ; i= "<< i << " ; k= " << k) ;
		}

		if(k != _sqlString.npos) {
			if(k == (_sqlString.size() -1) ) { // PRQA S 3084
				k = _sqlString.npos ;	/* Quit the loop */
            }
			else {
				k++ ;
            }
		}
		i=k;
		count++;
	}
}



void dbMultiLineQuery::removeComments(std::string& s) // PRQA S 4020
{
	size_t i = s.find("--")  ; // PRQA S 4412
	if(i==s.npos) {
		return;
    }
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "removing comment from string = \"" << s << "\".") ;

	s=string(s , 0, i ) ;	// Strip comments
}


bool dbMultiLineQuery::notEmpty(std::string const &s)
{
	return ( s.find_first_not_of(" \t") != s.npos) ;
}
 
    
_ACS_END_NAMESPACE // acs
