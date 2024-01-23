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
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.4  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.3  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.2  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.1  2010/09/11 18:04:48  marpas
	libDbExpress 3.2 interface adopted
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.2  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.1  2008/11/10 16:21:59  marpas
	dbCursor implemented
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/04/27 10:09:17  enrcar
	work in progress
	
	Revision 1.4  2005/04/22 20:29:31  marpas
	work in progress
	
	Revision 1.3  2005/04/22 10:43:18  marpas
	work in progress
	
	Revision 1.2  2005/03/15 13:34:43  enrcar
	*** empty log message ***
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.7  2003/07/11 13:08:02  marpas
	exException::clone implemented
	
	Revision 1.6  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
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

#ifndef _dbCursor_H_
#define _dbCursor_H_

#include <SQLString.h>
#include <dbException.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

	
class dbRow ;
class dbConnection ;
class dbQuery ;

//////////////////////////////////////////////
//              							// 
// class dbCursor							// 
//              							//
//////////////////////////////////////////////

class dbCursor // PRQA S 2109
{
public:

	/*! class NullRowException declaration */
	exDECLARE_EXCEPTION(LockNullRowException,dbException) ; // PRQA S 2131, 2153, 2502

	/*! class ExecuteException declaration */
	exDECLARE_EXCEPTION(ExecuteException,dbException) ; // PRQA S 2131, 2153, 2502

	explicit dbCursor(dbQuery &query) ;
	dbCursor(dbConnection &conn,const SQLString &q,const std::string &queryStatLabel = "") ;
	virtual	~dbCursor() throw() ;

	void exec() ; // PRQA S 2502 3
	void open() ;
	void close() ;


	void getNextRow(dbRow &) ;

	//! \return The SQL std::string
	const SQLString &sqlString() const ;
	//! Set the SQL String
	void sqlString(const SQLString &,const std::string &queryStatLabel = "") ;



private:
	// defined but not declared
	dbCursor() ; 			
	dbCursor(const dbCursor &) ; 			
	dbCursor &operator=(const dbCursor &) ; 

private:

	dbConnection &_connection ;
	SQLString	_sqlString ;
	std::string _queryStatLabel ;
    dbSQLCursor _cursor ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbCursor) ;
} ;

_ACS_END_NAMESPACE

#endif // _dbCursor_H_
