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
	Revision 5.3  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.2  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/06/12 17:07:03  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.7  2013/06/03 12:39:09  marpas
	coding besta practices partially applied
	
	Revision 4.6  2013/03/26 00:58:19  marpas
	dbRow columns are accessed throu a uint16_t
	other interfaces modified accordingly
	
	Revision 4.5  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.4  2012/12/14 18:38:53  marpas
	coding best practices
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
	
	Revision 2.6  2009/01/27 13:16:19  marpas
	db statistics improved, some other minor changes with no regression
	
	Revision 2.5  2008/10/08 15:33:30  crivig
	porting to postgres8
	
	Revision 2.4  2006/07/26 09:46:16  paoscu
	More debug added.
	
	Revision 2.3  2006/07/14 12:21:24  paoscu
	Table name prepened to  column name in select.
	
	Revision 2.2  2006/03/31 13:51:17  paoscu
	dbColumn::nullString() used instead of "<null>"
	
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

#include <TableSelect.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <SQLString.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TableSelect) 

TableSelect::TableSelect(
        const string &key, 
        const string &tbl, 
        const vector<string> &cols, 
        dbQueryParameters *params, 
        bool uniqueIfSingle, 
        unsigned int orderByCol
):
	_tableName(tbl),
	_cols(cols),
	_orderByCol(orderByCol),
    _values()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	load(dbConnectionWrapper(db::ConnPools::instance()->getPool(key)), params, uniqueIfSingle); // PRQA S 3050, 3081
}

TableSelect::TableSelect(
	    dbConnection &conn, 
	    const string &tbl, 
	    const vector<string> &cols, 
	    dbQueryParameters *params, 
	    bool uniqueIfSingle, 
	    unsigned int orderByCol
):
	_tableName(tbl),
	_cols(cols),
	_orderByCol(orderByCol),
    _values()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	load(conn, params, uniqueIfSingle);
}


TableSelect & TableSelect::operator=(const TableSelect &v)
{
	if (this != &v) { // avoid a = a
	
		// deep copy
		_tableName= v._tableName ; 
		_cols = v._cols; 
		_orderByCol = v._orderByCol; 
		_values= v._values; 
	}
	return *this ;
}

TableSelect::TableSelect(const TableSelect & v) :
	_tableName(v._tableName ), 
	_cols (v._cols), 
	_orderByCol (v._orderByCol), 
	_values(v._values)
{
}
	
TableSelect::~TableSelect() throw() { }

size_t TableSelect::size() const throw() 
{ 
	return _values.size(); 
}

bool TableSelect::empty() const throw()
{ 
	return _values.empty(); 
}


size_t TableSelect::columns() const 
{
	return _cols.size() ;
}


const string &TableSelect::tableName() const // PRQA S 4120
{ 
	return _tableName ; 
}

const string &TableSelect::colName(size_t which) const 
{
	if (which > _cols.size()-1) { // check boundary // PRQA S 3084
		ostringstream os ;
		os << "Cannot address colummn #" << which << " in a collection of " << _cols.size() << " columns " ;
		exIllegalValueException e(os.str()) ; 
		ACS_THROW(e) ;   
	} 
	return _cols[which] ; // return it
}


const string &TableSelect::value(size_t row, unsigned int col) const 
{
	if (row >= size()) { // row boundaries check
		ostringstream os ;
		os << "Cannot address element #" << row << " in a collection of " << size() << " elements " ;
		exIllegalValueException e(os.str()) ; 
		ACS_THROW(e) ;   
	}
	
	if (col > _cols.size()-1) { // col boundaries check // PRQA S 3084
		ostringstream os ;
		os << "Cannot address colummn # " << col << " in a collection of " << _cols.size() << " columns " ;
		exIllegalValueException e(os.str()) ; 
		ACS_THROW(e) ;   
	} 
	
	return _values[row][col] ; // return the value
	
}


void TableSelect::load(dbConnection &conn, dbQueryParameters *pars, bool uniqueIfSingle) 
{
	ACS_COND_THROW(_tableName.empty() || _cols.empty(), // PRQA S 3081
							  exIllegalValueException("TableSelect::load error: invalid mandatory field (table,col)")) ; 
	
    const string& dbKey = conn.getKey() ;	
	_values.clear() ; // clear previous result
	
	// prepare the SQL
    SQLString sqlString( dbKey, SQLString::SELECT ) ;
	if (_cols.size() == 1) {
		sqlString.setDistinct(uniqueIfSingle) ;
    }
	sqlString.addTableList(_tableName);
    size_t colsize = _cols.size() ;
	for (size_t col=0; col < colsize; ++col) {
    	sqlString.addSelectList(_tableName+"."+_cols[col]);
		if (col == _orderByCol) {
    		sqlString.addOrderByList(_tableName+"."+_cols[col]);
        }
	}
	// add parameters if any
	if (pars) {
		sqlString.addANDWhereParameter(*pars) ;
    }

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "query is \n" << sqlString.getSQL()) ;
	
	// create the query
	dbQuery query(conn,sqlString, "TableSelect"+_tableName) ;
	
	// execute it 
	dbSet set ; 
	query.execForSet(set) ;

	// parse the result
	dbRow row( dbKey ) ; 		
	while(set.getNextRow(row))
	{
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "loading row") ;
		vector<string> cols;
		size_t colssize = _cols.size() ;
		for (size_t col=0; col < colssize; ++col) {
	        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "loading col #" << col) ;
			string tmp ; 
			bool isNull = false ;
			row.column(col,tmp, &isNull) ; // PRQA S 3010
			cols.push_back(isNull ? dbColumn::nullString() : StringUtils::trim(tmp, StringUtils::is_not_space)) ; // PRQA S 3380, 3385
		}
		_values.push_back(cols) ;
	}
}


ostream &operator<<(ostream &os, const TableSelect &n) 	// output to stream
{
    size_t ns = n.size() ;
	for (size_t i=0; i< ns; ++i) {
		os << n.value(i,0) << "\n" ;
    }
	
	return os ;  // PRQA S 4028
}



exostream &operator<<(exostream &os, const TableSelect &n) 	// output to stream // PRQA S 4222
{
    size_t ns = n.size() ;
	for (size_t i=0; i< ns; ++i) {
		os << n.value(i,0) << "\n" ;
    }
		
	return os ;  // PRQA S 4028
}

_ACS_END_NAMESPACE
