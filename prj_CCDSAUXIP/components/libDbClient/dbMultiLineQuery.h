// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/11/21 14:53:49  marpas
	removing deprecated method
	
	Revision 5.2  2013/07/02 11:59:19  marpas
	minor changes: debug and statistics"
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/03/28 13:40:10  marpas
	enforcing dbConnection use
	
	Revision 4.4  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.3  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.1  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.1  2003/09/26 09:41:35  paoscu
	dbMultiLineQuery added.
	
*/


#ifndef _dbMultiLineQuery_H_
#define _dbMultiLineQuery_H_
#include <exException.h>

#include <vector>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbRow ;
class dbSet ;
/*!
    \class dbMultiLineQuery 
    this is the derived class to perform     
    queries  that contain more than one sql  
    statement                                
*/		 

class dbMultiLineQuery // PRQA S 2109
{
public:	


	/*!
		Constructor
		\param stat the name of the query for statistics
		\param conn a dbConnection to connect with the db
		\param stmtnum the number of SQL statements
		\param st an std::string containing the SQL command std::string defaulted to empty string
	 */
	dbMultiLineQuery(const std::string &stat, int stmtnum, dbConnection &conn,const std::string &st = "") ;

	/*!
		Constructor
		\param stat the name of the query for statistics
		\param conn a dbConnection to connect with the db
		\param stmtnum the number of SQL statements
		\param st : an std::stream containing the SQL command
	 */
	dbMultiLineQuery(const std::string &stat, int stmtnum, dbConnection &conn, std::istream &st) ;

	dbMultiLineQuery() = delete ; 

	//! Destructor
	virtual	~dbMultiLineQuery() = default ;

	//! Copy constructor
	dbMultiLineQuery(const dbMultiLineQuery &) ;

	dbMultiLineQuery &operator=(const dbMultiLineQuery &) ;

	void sqlString(std::istream &is) ;

	/*! \brief Perform a query with no result
		This function has to be used to perform query that have no result.
		Example : DELETE FROM table WHERE id = 1
	*/
	bool execForStatus() ;

	/*! \brief Perform a query with a result set
		This function has to be used to perform query that have a result set.
		Example : SELECT * FROM table
	*/
	void execForSet(dbSet &) ;
	static void removeComments(std::string&) ;
	static bool notEmpty(std::string const &) ;
    
    /*! \brief returns the sql string 
    */
    const std::string & sqlString() const noexcept { return _sqlString ; }

    /*! \brief to store another sql string
    */
    void sqlString(const std::string &s) { _sqlString = s ; }

protected:
	void parseSql() ;
    
private:
	dbConnection &_connection ;
	std::string _sqlString ;
	std::vector<std::string> _sqlVector;
    std::string statString_ ;
    int stmtnum_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbMultiLineQuery) ;
} ;
  
_ACS_END_NAMESPACE // acs


#endif // _dbMultiLineQuery_H_
