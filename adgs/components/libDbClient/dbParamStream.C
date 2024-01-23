/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. dbParamStream $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 1.3  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 1.2  2012/07/26 13:40:18  enrcar
	EC:: Classes column() and table() can be used to arbitrarily sort the tokens.
	
	Revision 1.1  2012/07/24 09:20:53  enrcar
	EC:: Imported source file
	

*/


#include <SQLDialectFactory.h>
#include <dbParamStream.h>
#include <SQLString.h>
#include <SQLStringStream.h>
#include <dbGeoObject.h>

using namespace acs;
using namespace std;

exCLASS_DEFINE_DEBUG_LEVEL(dbParamStream);


/*********************************************************************************************
**                                                                                          **
*                                Outside class declarations                                  *
**                                                                                          **
*********************************************************************************************/

column::column() :
	_selString(), _sqlCast(), _lbl() {}

column::column(std::string const &sel, SQLDialect::SQLCast const &sqlCast, std::string const &lbl) :
	_selString(sel), _sqlCast(sqlCast), _lbl(lbl) {}

column::column(std::string const &sel, std::string const &lbl) :
	_selString(sel), _sqlCast(), _lbl(lbl) {}

column::column(SQLString const &sel, SQLDialect::SQLCast const &sqlCast, std::string const &lbl) :
	_selString(sel.getSQL(true)), _sqlCast(sqlCast), _lbl(lbl) {}

column::column(SQLString const &sel, std::string const &lbl) :
	_selString(sel.getSQL(true)), _sqlCast(), _lbl(lbl) {}

column::column(SQLStringStream const &sel, SQLDialect::SQLCast const &sqlCast, std::string const &lbl) :
	_selString(sel.getSQL(true)), _sqlCast(sqlCast), _lbl(lbl) {}

column::column(SQLStringStream const &sel, std::string const &lbl) :
	_selString(sel.getSQL(true)), _sqlCast(), _lbl(lbl) {}

column::~column() throw() {}


exostream& acs::operator<<(exostream &o, const dbParamStream &s)	/* to excerr */
{
	o << s.getSQL() ;
	return o ;
}

ostream& acs::operator<<(ostream &o, const dbParamStream &s)	/* to ostrem */
{
	o << s.getSQL() ;
	return o ;
}


/*********************************************************************************************
**                                                                                          **
*                                     CLASS  dbParamStream                                   *
**                                                                                          **
*********************************************************************************************/

//
//  dbParamStream C'TOR
//
dbParamStream::dbParamStream(const string &key) :
 		_key(key),
		_table(), 
		_column(),
		_right_v(),
		_oper(dbParam::NONE),
		_currentStatus(SQLDialect::TABLE_LIST)	/* Set next token to wait for */
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called\n" ;
    exCLASS_END_DEBUG
}


//
//  dbParamStream D'TOR
//
dbParamStream::~dbParamStream() throw() 
{
}


//
//  dbParamStream Copy
//
dbParamStream::dbParamStream(const dbParamStream &e) :
	_key( e._key ),
	_table( e._table ), 
	_column( e._column ),
	_right_v( e._right_v ),
	_oper( e._oper ),
	_currentStatus( e._currentStatus )
{
    // empty
}


//
//  dbParamStream Assignment
//
dbParamStream& dbParamStream::operator=(const dbParamStream &e)
{
    if (this != &e) // avoid auto assignation
    {
		_key = e._key ;
		_table = e._table ; 
		_column = e._column ;
		_right_v = e._right_v ;
		_oper = e._oper ;
		_currentStatus = e._currentStatus ;
   }
	// return the object
    return *this;
}

			
/*********************************************************************************************
**                                                                                          **
*                                       Stream operators                                     *
**                                                                                          **
*********************************************************************************************/
#if(0)
/* ENRCAR: Implemented, but never used. */

//  dbParamStream::operator<<(const op&)
//
dbParamStream& dbParamStream::operator<<(const op& c)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called with operation: " << dbParam::getOperatorName(_key, c.get_o()) << endl ;
	exCLASS_END_DEBUG

	ACS_COND_THROW( (_oper != dbParam::NONE), \
							  Exception("dbParamStream::operator<<(const op& c)  Operation already set") ) ; 

	ACS_COND_THROW( (c.get_optype() != op::OP_DBPARAMOPER), \
							  Exception("dbParamStream::operator<<(const op& c)  Unsupported type of operation") ) ; 

	_oper = c.get_o() ;
	_currentStatus = SQLDialect::VALUES_LIST   /* Set next token to wait for */

    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method finished" << endl ;
    exCLASS_END_DEBUG

	return (*this) ; 

} 
#endif


//
//  dbParamStream::operator<<(const table&)
//
dbParamStream& dbParamStream::operator<<(const table& c)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called." << endl ;
    exCLASS_END_DEBUG
	
	_currentStatus = SQLDialect::TABLE_LIST ;	/* Set current status */
	
	const string tableName = c.get_v() ;
	if (! tableName.empty()) {
		_table = tableName ;
	}

	return (*this) ; 
}


//
//  dbParamStream::operator<<(const column&)
//
dbParamStream& dbParamStream::operator<<(const column& c)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called." << endl ;
    exCLASS_END_DEBUG

	_currentStatus = SQLDialect::COLUMN_LIST ;	/* Set current status */

	const string columnName = c.get_selString() ;
	if (! columnName.empty()) {
		_column = columnName ;
	}

	return (*this) ; 
}


//
//  dbParamStream::operator<<(const values&)
//
dbParamStream& dbParamStream::operator<<(const values& c)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called. Number of elements provided: " << (c.get_v()).size() << endl ;
    exCLASS_END_DEBUG

	_currentStatus = SQLDialect::VALUES_LIST ;	/* Set current status */
	
	for (size_t i=0; i<(c.get_v()).size(); i++) { _right_v.push_back( (c.get_v())[i] ) ; }

	return (*this) ; 
}


//
//  dbParamStream::operator<<(const dbParam::Operation&)
//
dbParamStream& dbParamStream::operator<<(const dbParam::Operation& o)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called with operation: " << dbParam::getOperatorName(_key, o) << endl ;
	exCLASS_END_DEBUG

	ACS_COND_THROW( (_oper != dbParam::NONE), \
							  Exception("dbParamStream::operator<<(const dbParam::Operation&)  Operation already set") ) ; 

	_oper = o ; 	
	_currentStatus = SQLDialect::VALUES_LIST ;  /* Set next token to wait for */

    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method finished" << endl ;
    exCLASS_END_DEBUG

	return (*this) ; 

} 


//
//  dbParamStream::operator<<(const string&)
//
dbParamStream& dbParamStream::operator<<(const string& c)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called with string value:  \"" << c << "\"" << endl ;
	exCLASS_END_DEBUG


	switch(_currentStatus)	/* Analyze the current state of the FSM and set the next one: */
	{
		case SQLDialect::TABLE_LIST :
		{
			_table = c ;
			_currentStatus = SQLDialect::COLUMN_LIST ;
		}	
		break ; 
	
		case SQLDialect::COLUMN_LIST :
		{
			_column = c ;
			_currentStatus = SQLDialect::VALUES_LIST ;
		}	
		break ; 
	
		case SQLDialect::VALUES_LIST :
		{
			_right_v.push_back(c) ; 
			_currentStatus = SQLDialect::VALUES_LIST ;	/* SIC, state unchanged */
		}	
		break ; 
		
		default:
		{
			ACS_THROW( Exception("dbParamStream::operator<<(string&)  Internal error, token unsupported") ) ; 
		}
		break ;
	}	/* END: switch(_currentStatus) */

	return *this ; 
}


//
//  dbParamStream::operator<<(const SQLString&)
//
dbParamStream& dbParamStream::operator<<(const SQLString& s)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called" << endl ;
	exCLASS_END_DEBUG
	
	const bool nested = true ;
	return operator<<(s.getSQL(nested)) ; 
} 


//
//  dbParamStream::operator<<(const SQLStringStream&)
//
dbParamStream& dbParamStream::operator<<(const SQLStringStream& s)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called" << endl ;
	exCLASS_END_DEBUG
	
	const bool nested = true ;
	return operator<<(s.getSQL(nested)) ; 
} 


//
//  dbParamStream::operator<<(const dbGeoObject&)
//
dbParamStream& dbParamStream::operator<<(const dbGeoObject& s)
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called" << endl ;
	exCLASS_END_DEBUG

	ostringstream os ;
	s.getSqlString(os,SQLDialectFactory::dialectName(_key)) ;

	return operator<<(os.str()) ; 
} 


/*********************************************************************************************
**                                                                                          **
*                                          getDbParam                                        *
**                                                                                          **
*********************************************************************************************/


dbParam dbParamStream::getDbParam() const
{

    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called" << endl ;
    exCLASS_END_DEBUG

	string ret ; 
	
	ACS_COND_THROW( (_oper == dbParam::NONE), \
								Exception("dbParamStream::operator<<  Operation not set") ) ; 


	/* Set the minimum and maximum number of rvalues, accordingly to the operation ( <0 = do not check ) */

	ssize_t	\
			rvalue_low = -1 ,	
			rvalue_high = -1 ;	

	switch(_oper)
	{
		case dbParam::ISNULL:
		case dbParam::ISNOTNULL:
			rvalue_low = rvalue_high = 0 ; 
		break ;

		case dbParam::BETWEEN:
		case dbParam::NOT_BETWEEN:
			rvalue_low = rvalue_high = 2 ; 
		break ;

		case dbParam::IN:
			rvalue_low = 1 ; rvalue_high = -1 ; 
		break ;

		default:
			rvalue_low = 1 ; rvalue_high = 1 ; 
		;
	} ;

	
	/* Match number of parameters with the operation */
	
	if ( ((rvalue_low > -1) && (static_cast<ssize_t>(_right_v.size()) < rvalue_low)) || ((rvalue_high > -1) && (static_cast<ssize_t>(_right_v.size()) > rvalue_high)) ) {
		ostringstream o ;
		o << "dbParamStream::getSQL()  Invalid number of rvalues for operation: " << dbParam::getOperatorName(_key, _oper) ;
		ACS_THROW( Exception(o.str()) ) ; 
	}


	/* If here: the number of parameters match the operation */

	/* Create a specific dbParam object */

	if (_table.empty() ^ _column.empty()) {	/* XOR */
	
		/* The XOR above means that exactly ONE parameter (over TWO) is provided as lvalue (0 XOR 1) == (1 XOR 0) == 1  */
		const string leftConditionPart = ( _table.empty() ? _column : _table ) ; 

		switch(_oper)
		{
			case dbParam::ISNULL:
			case dbParam::ISNOTNULL:
			{
				return( dbParam(_key, _oper, leftConditionPart) ) ; /* Exactly two parameters are passed as rvalue */
			}
			break ;
			
			case dbParam::IN :
			{
				return( dbParam(_key, _oper, leftConditionPart, _right_v) ) ;	/* A vector is passed as rvalue */
			}
			break ;
			
			default:
			{
				/* Always two parameters are passed as rvalue. If not enough arguments are provided as input, "" is used: */
				
				if (_right_v.size()==0) {
					return( dbParam(_key, _oper, leftConditionPart, "", "") ) ; 
				}
				else if (_right_v.size()==1) {
					return( dbParam(_key, _oper, leftConditionPart, _right_v[0], "") ) ; 
				} else if (_right_v.size()>1) {
					return( dbParam(_key, _oper, leftConditionPart, _right_v[0], _right_v[1]) ) ; 
				}
			}
		
		} /* Switch _oper */

	} else if ( (!_table.empty()) && (!_column.empty()) ) {
	
		/* Both tablename and _column as provided: */
	
		switch(_oper)
		{
			case dbParam::ISNULL:
			case dbParam::ISNOTNULL:
			{
				return( dbParam(_key, _table, _column, _oper) ) ; /* Exactly two parameters are passed as rvalue */
			}
			break ;
	
			case dbParam::IN :
			{
				return( dbParam(_key, _table, _column, _oper, _right_v) ) ;	/* A vector is passed as rvalue */
			}
			break ;
				
			default:
			{
				/* Always two parameters are passed as rvalue. If not enough arguments are provided as input, "" is used: */
				
				if (_right_v.size()==0) {
					return( dbParam(_key, _table, _column, _oper, "", "") ) ; 
				}
				else if (_right_v.size()==1) {
					return( dbParam(_key, _table, _column, _oper, _right_v[0], "") ) ; 
				} else if (_right_v.size()>1) {
					return( dbParam(_key, _table, _column, _oper, _right_v[0], _right_v[1]) ) ; 
				}
			}
		
		} /* Switch _oper */
	
	} else {

		/* Not supported, so far */
		ACS_THROW( Exception("dbParamStream::operator<<  Not enought lvalue parameters") ) ; 

	}

	
	/* IF HERE: INTERNAL ERROR. ALL THE POSSIBILE VALUES SHOULD BE HANDLED ABOVE */
		
	ACS_THROW( Exception("dbParamStream::operator<<  Internal Error. This position should be never reached") ) ; 

	return dbParam(_key) ; /* Fake output, just to prevent compilation warnings */ 
}


/*********************************************************************************************
**                                                                                          **
*                                             getSQL                                         *
**                                                                                          **
*********************************************************************************************/
string dbParamStream::getSQL() const
{
    exCLASS_BGN_DEBUG(10) 
        excerr << exDebugSignature << "Method called" << endl ;
    exCLASS_END_DEBUG

	return getDbParam().getSqlString() ;
}
