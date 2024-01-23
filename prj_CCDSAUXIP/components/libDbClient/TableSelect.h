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
	Revision 5.1  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/06/03 12:39:09  marpas
	coding besta practices partially applied
	
	Revision 4.4  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.3  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.1  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:12  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.2  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.1  2006/03/02 13:07:21  marpas
	TableSelect has parameters optionally
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/02/21 11:46:34  marpas
	columns method added
	
	Revision 1.2  2006/02/21 11:37:59  marpas
	ctor I/f changed
	
	Revision 1.1  2006/02/17 18:00:21  marpas
	new class added
	
	
*/

#ifndef _TableSelect_H_
#define _TableSelect_H_ 

#include <dbException.h>
#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


class dbConnection ;
class dbQueryParameters ;
/*! class TableSelect \deprecated
*
* This is the base class that retrieves all values of a specified field.
* It provide the basic mechanism to load all values of a specified field
* of a specified table.
*
*/

class TableSelect // PRQA S 2109
{

public:
	TableSelect(const std::string &key, 
                const std::string &tbl,
                const std::vector<std::string> &cols, 
                dbQueryParameters * = 0, 
                bool uniqueIfSingle = false, 
                unsigned int orderByCol = 0);
                
	TableSelect(dbConnection &, 
				const std::string &tbl,
				const std::vector<std::string> &cols, 
				dbQueryParameters * = 0, 
				bool uniqueIfSingle = false, 
				unsigned int orderByCol = 0);
	
	TableSelect &operator=(const TableSelect &) = default ;
	TableSelect(const TableSelect &) = default ;

	~TableSelect() = default ;

	// return the element number
	size_t size() const noexcept ; 

	// return the element number
	bool empty() const noexcept ; 
    
	// return the column number
	size_t columns() const ; 

	// return the table name 
	const std::string &tableName() const ; 
	
	// return the column name 
	const std::string & colName(size_t which) const ;
	
	// return the i-th value
	const std::string &value(size_t row, unsigned int col) const ;

	void load(dbConnection &conn, dbQueryParameters *pars= 0, bool uniqueIfSingle = false) ; 
	
	friend std::ostream &operator<<(std::ostream &, const TableSelect&) ;  // PRQA S 2107
	friend exostream &operator<<(exostream &, const TableSelect&) ;    // PRQA S 2072, 2107

private:
	TableSelect() ; // declared but not implemented
		
	
// data section
private:
	std::string _tableName ; 
	std::vector<std::string> _cols ;
	unsigned int _orderByCol ;
	std::vector< std::vector<std::string> > _values;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TableSelect) ;
} ; 

std::ostream &operator<<(std::ostream &, const TableSelect&) ;  
exostream &operator<<(exostream &, const TableSelect&) ;  

_ACS_END_NAMESPACE


#endif // _TableSelect_H_
