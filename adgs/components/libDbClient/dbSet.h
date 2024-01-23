// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 5.2  2017/11/21 18:14:54  marpas
	rewind method added
	setEmptyRow  & getEmptyRow methods added to store an empty row (no value) in order to know columns and their types using dbQuery::execForSet wher no data is returned, so they cannot be get from the rows (0 returned rows).
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.2  2013/02/26 17:12:25  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.1  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.0  2011/10/18 12:31:15  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:53  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2009/06/22 12:36:36  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/08/11 07:54:48  marpas
	multiset patch removed due the fact the multiset is no longer supported by dbExpress approach
	
	Revision 1.4  2005/04/11 09:52:47  enrcar
	*** empty log message ***
	
	Revision 1.3  2005/04/11 08:46:46  enrcar
	*** empty log message ***
	
	Revision 1.8  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.7  2004/10/08 10:22:55  marpas
	pathced for multiset queries
	
	Revision 1.6  2004/10/07 01:24:17  marpas
	ITEssentialGuard I/F used
	
	Revision 1.5  2004/10/05 13:30:48  marpas
	multiset support added
	
	Revision 1.4  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.2  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#ifndef _dbSet_H_
#define _dbSet_H_

#include <acs_c++config.hpp>
#include <dbRow.h>
#include <dbException.h>

_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////////////////////////
//              							//
// class dbSet								//
//              							//
//////////////////////////////////////////////
/*!
	Class used to manage a set of data returned by a dbQuery
*/
class dbSet // PRQA S 2109
{
	friend class dbRow ;    // PRQA S 2107
public:

	/*! class SetExecuteException declaration */
	exDECLARE_EXCEPTION(SetExecuteException,dbException) ; // PRQA S 2131, 2153, 2502 

	dbSet() ;
	virtual	~dbSet() throw() ;

	/*!
		Used to get the next row in the set
		\param row : a reference to a dbRow to fill
		\return True if a row has been filled. False otherwise (for ex. if no row is available)
	*/
	bool getNextRow(dbRow &row) ;

	/*!
		Used to rewind the internal counter. So the next call to getNextRow will return the very first row if present.
	*/
	void rewind() throw() { _actualIndex = 0 ; } // PRQA S 2502

	/*!
		Used to get the a specified row in the set. Note index is 1-relative
		\param pos : The index of the row to get
		\param row : a reference to a dbRow to fill
		\return True if a row has been filled. False otherwise (for ex. if no row is available)
	   */
	bool getRow(size_t pos, dbRow &row) ;

	/*!
		Used to get a pointer to the Informix library ITSet class
		\internal
	  */
    const std::vector<dbRow> &set() const throw() ;

	/*!
		Used to assign a dbSet to a an Informix library ITSet
		\internal
	*/
    void set(const std::vector<dbRow>&) ;
    
    
	/*!
		For empty set this contains only column types, no values
		\internal
	*/
    void setEmptyRow(const dbRow &r) { _emptyRow = r ; }

    void getEmptyRow(dbRow &r) const { r = _emptyRow ; }
    
	/*!
		\return The number of row composing the set (deprecated use size() )
	  */
	size_t __attribute__((deprecated)) rowCount() const throw() ;

	/*!
		\return The number of row composing the set
	  */
	size_t size() const throw() ;

    bool empty() const throw() ;

protected :

	void reset() throw() ;

private:
	// defined but not declared
	dbSet(const dbSet &) ;
	dbSet &operator=(const dbSet &) ;

private:

	size_t	_actualIndex ;

    std::vector <dbRow> _set; 
    dbRow _emptyRow ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSet) ;
} ;

_ACS_END_NAMESPACE

#endif // _dbSet_H_
