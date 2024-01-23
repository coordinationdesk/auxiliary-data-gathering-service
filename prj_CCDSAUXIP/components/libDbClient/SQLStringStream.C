// PRQA S 1050 EOF
/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SQLStringStream $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2013/11/20 09:29:10  marpas
	fixing debug
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 1.11  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 1.10  2012/07/26 13:37:44  enrcar
	EC:: dbparam renamed as param. Classed column and table moved to dbParamStream. param can be used w/ closing brackets to lower nesting level (e.g. param('}'))
	
	Revision 1.9  2012/07/24 13:59:27  enrcar
	EC:: class param modifed in order to create a further nested level ( arguments "(" "{" )
	
	Revision 1.8  2012/07/20 15:58:32  enrcar
	EC:: Type dbParamStream handled. Type dbGeoObject handled
	
	Revision 1.7  2012/07/19 10:01:11  enrcar
	EC:: class "op" moved inside dbParamStream
	
	Revision 1.6  2012/07/18 09:28:13  enrcar
	EC:: added operator<< for easily writing into ostream and excerr
	
	Revision 1.5  2012/07/17 16:08:55  enrcar
	EC:: refactoring. Now the class name is not required for accessing the subclasses (use: <<limit instead of:  <<SQLStringStream::limit)
	
	Revision 1.4  2012/07/16 09:47:42  enrcar
	EC:: added logical brakets { }
	
	Revision 1.3  2012/07/13 14:31:04  enrcar
	EC:: Several modifications; the operator ")" now also allow Aliasing. Expression stack improved in order to handle arbitary expressions enclosed in "( )" (thus allowing the Aliasing)
	
	Revision 1.2  2012/07/12 13:26:02  enrcar
	EC:: key handled. Logical operators are translated by the Dialects. Unary "NOT" handled.
	
	Revision 1.1  2012/07/11 12:29:02  enrcar
	EC:: Imported source file
	

*/


#include <SQLDialectFactory.h>
#include <SQLStringStream.h>
#include <dbGeoObject.h>
#include <Filterables.h>

using namespace acs;
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SQLStringStream);


/*********************************************************************************************
**                                                                                          **
*                                Outside class declarations                                  *
**                                                                                          **
*********************************************************************************************/

exostream& acs::operator<<(exostream &o, const SQLStringStream &s)	/* to excerr */
{
	const bool nested = false ;
	o << s.getSQL(nested) ;
	return o ;
}

ostream& acs::operator<<(ostream &o, const SQLStringStream &s)	/* to ostrem */
{
	const bool nested = false ;
	o << s.getSQL(nested) ;
	return o ;
}


/*********************************************************************************************
**                                                                                          **
*                              SUBCLASS  SQLStringStream::Status                             *
**                                                                                          **
*********************************************************************************************/

//
//  SQLStringStream::Status C'TOR
//
SQLStringStream::Status::Status(const string& key, const SQLString::QueryType &q) : 
	_currentToken(SQLDialect::TOKEN_UNDEFINED), 
	_logicalOp(LOP_UNDEF), 
	_name(), 
	_alias(),
	_key(key),
	_joinToken(SQLDialect::INNER), /* Any value */
	_sortDirection(SQLString::SORT_DEFAULT),
	_sqlString(key, q),
	_paramStream(key)
{
}


//
//  SQLStringStream::Status VIRTUAL D'TOR
//
SQLStringStream::Status::~Status() noexcept 
{
}


//
//  SQLStringStream::Status ASSIGNMENT C'TOR
//
SQLStringStream::Status::Status(const Status &rhs) :
	_currentToken(rhs._currentToken), 
	_logicalOp(rhs._logicalOp), 
	_name(rhs._name), 
	_alias(rhs._alias),
	_key(rhs._key),
	_joinToken(rhs._joinToken),	/* Any value */
	_sortDirection(rhs._sortDirection),
	_sqlString(rhs._sqlString),
	_paramStream(rhs._paramStream)
{
	*this = rhs ;	// Call assignment ctor
}


//
//  SQLStringStream::Status COPY OPERATOR
//
SQLStringStream::Status& SQLStringStream::Status::operator=(const Status &rhs) 
{
	if (this != &rhs) {
		
		// copy parameters
		_currentToken = rhs._currentToken ; 
		_logicalOp = rhs._logicalOp ; 
		_name = rhs._name ; 
		_alias = rhs._alias ;
		_key = rhs._key ;
		_joinToken = rhs._joinToken ;
		_sortDirection = rhs._sortDirection ;
		_sqlString = rhs._sqlString ;
		_paramStream = rhs._paramStream ;
	}
	
	return *this ;
}



/*********************************************************************************************
**                                                                                          **
*                          SUBCLASS  SQLStringStream::SubExpression                          *
**                                                                                          **
*********************************************************************************************/

//
//  SQLStringStream::SubExpression C'TOR
//
SQLStringStream::SubExpression::SubExpression(const string& key) : 
	_status(key, SQLString::QUERY_UNDEFINED), _expression() 
{
}


//
//  SQLStringStream::SubExpression C'TOR
//
SQLStringStream::SubExpression::SubExpression(const Status& s) : 
	_status(s), _expression() 
{
}


//
//  SQLStringStream::SubExpression VIRTUAL D'TOR
//
SQLStringStream::SubExpression::~SubExpression() noexcept 
{
}


//
//  SQLStringStream::SubExpression ASSIGNMENT C'TOR
//
SQLStringStream::SubExpression::SubExpression(const SubExpression &rhs) :
	_status(rhs._status),
	_expression(rhs._expression)
{
	*this = rhs ;	// Call assignment ctor
}


//
//  SQLStringStream::SubExpression COPY OPERATOR
//
SQLStringStream::SubExpression& SQLStringStream::SubExpression::operator=(const SubExpression &rhs) 
{
	if (this != &rhs) {
		
		// copy parameters
		_status = rhs._status ; 
		_expression = rhs._expression ; 
	}
	
	return *this ;
}


//
//  SQLStringStream::addParam
//
void SQLStringStream::SubExpression::addParam(const string& q) 
{ 
	const LogicalOp o = _status.getLOp() ;
	if ((o != LOP_UNDEF) && (o != LOP_NOT)) {
		_expression += " " + translateLogicalOp(getKey(), o) + " " ;
	}
	if ( o & LOP_NOT ) {
		_expression += " " + SQLDialectFactory::sqlDialect( getKey() ).logicalNot() + " " ; 
	}

	_expression += q ;
}


//
//  SQLStringStream::getSQL
//
string SQLStringStream::SubExpression::getSQL() const 
{ 
	
	if (_status.getToken() == SQLDialect::TOKEN_UNDEFINED)
	{
		return _expression ;
	}
	else if (_status.getToken() == SQLDialect::DBPARAM)
	{
		return _status.getDbParamStream().getSQL() ;
	}
	else 
	{
		const bool nested = true ; 
		return _status.getSQLString().getSQL(nested) ;
	}
}


/*********************************************************************************************
**                                                                                          **
*                                    CLASS  SQLStringStream                                  *
**                                                                                          **
*********************************************************************************************/


//
//  SQLStringStream C'TOR
//
SQLStringStream::SQLStringStream(const string &key, SQLString::QueryType t) :
	_stackExpressions(),
	_key(key) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Method called") ;

	Status emptystatus(key, t) ;
	stack_push(emptystatus)	; /* Allocate the stack bottom */
	
	switch(t) {
		case SQLString::INSERT :
			setCurrentToken(SQLDialect::INSERT) ; break ;
		case SQLString::DELETE :
			setCurrentToken(SQLDialect::DELETE) ; break ; 
		case SQLString::DROP :
			setCurrentToken(SQLDialect::DROP) ; break ; 
		case SQLString::SELECT :
			setCurrentToken(SQLDialect::SELECT) ; break ;
        case SQLString::UPDATE :
 			setCurrentToken(SQLDialect::UPDATE) ; break ; 
		case SQLString::FUNCTION :
        case SQLString::CUSTFUNCTION :
 			setCurrentToken(SQLDialect::CUSTFUNCTIONNAME) ; break ; 
        case SQLString::CUSTPROCEDURE :
 			setCurrentToken(SQLDialect::CUSTPROCEDURENAME) ; break ; 
		default:
		{
			ostringstream errorMessage;
			errorMessage << "SQLStringStream& SQLStringStream::processStackTopExpression()  Unsupported qyerytype " << static_cast<int>(t) ;
			ACS_THROW(Exception(errorMessage.str() )) ;
		}
	
	}	/* END switch */
}


//
//  SQLStringStream D'TOR
//
SQLStringStream::~SQLStringStream() noexcept 
{
}


//
//  SQLStringStream Copy
//
SQLStringStream::SQLStringStream(const SQLStringStream &e) :
	_stackExpressions( e._stackExpressions ),
	_key( e._key )
 
{
    // empty
}


//
//  SQLStringStream Assignment
//
SQLStringStream& SQLStringStream::operator=(const SQLStringStream &e)
{
    if (this != &e) // avoid auto assignation
    {
		_stackExpressions = e._stackExpressions ;
		_key = e._key ;
    }
	// return the object
    return *this;
}


//
//  SQLStringStream::stackDumpDebug
//
void SQLStringStream::stackDumpDebug(const stack <SubExpression>& s)
{
	ACS_LOG_DEBUG("  **  STACK DUMP  ********************************************************************" ;
	stack<SubExpression> stk (s) ;
	const size_t sz = stk.size() ;
	
	while (! stk.empty())
	{
		ACS_LOG_DEBUG("  Dumping level: " << (1+sz-stk.size()) << " / " << sz) ;
	
		SubExpression se(stk.top()) ;
		stk.pop() ;
		expressionDumpDebug(se) ;
		ACS_LOG_DEBUG("") ;
	}
	
	ACS_LOG_DEBUG("  **  END OF DUMP  *******************************************************************") ;
}


//
//  SQLStringStream::expressionDumpDebug
//
void SQLStringStream::expressionDumpDebug(const SubExpression& se)
{
	Status s(se.getStatus()) ;

	statusDumpDebug(se.getKey(), s) ;
	ACS_LOG_DEBUG("   expression: \"" << se.getExpression() << "\"") ;
}


//
//  SQLStringStream::statusDumpDebug
//
void SQLStringStream::statusDumpDebug(const string& key, const Status& s)
{
	ACS_LOG_DEBUG(" Status:  operator: " << translateLogicalOp(key, s.getLOp()) << "  token: " << SQLDialect::debugSQLToken(s.getToken()))  ;
	ACS_LOG_DEBUG(" Status:  name: " << s.getName()) ;
}

			
//
//  SQLStringStream::incr_nesting_level
//
SQLStringStream& SQLStringStream::incr_nesting_level(const string& braket)
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called with braket: \"" << braket << "\"") ;

	stack_push_empty() ;	/* Add a new, empty SubExpression and push it to the stack */

	setCurrentLOp(LOP_UNDEF) ; 		

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished") ;

	return (*this) ; 
}

			
//
//  SQLStringStream::decr_nesting_level
//
SQLStringStream& SQLStringStream::decr_nesting_level(const string& braket, const string& alias)
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called with braket: \"" << braket << "\"  alias: \"" << alias << "\"" );

	if (stack_size() <= 1) {
		ostringstream errorMessage;
		errorMessage << "SQLStringStream::decr_nesting_level  Mismatching operator : \"" << braket << "\"" ;
		ACS_THROW(Exception(errorMessage.str() )) ;
	} 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing operator \"" << braket << "\" ..." ) ;

	SubExpression se = stack_pop() ;

	const string a = ( alias.empty() ? "" : (" AS " + alias) ) ;	/* EC:: keyword "AS" should be obtained by the Dialect */

	if (braket == ")") {
		stack_addtotop( " ( " + se.getSQL() + " )" + a + " " ) ;	/* Explicit brakets: Expression will be sourronded by parenthes */
	} else {
		stack_addtotop( " " + se.getSQL() + a + " " ) ;				/* Implicit (logic) brakets: Expression won't be sourronded by parenthes */
	}

	setCurrentLOp(LOP_UNDEF) ; 	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		expressionDumpDebug(se) ;
	ACS_CLASS_END_DEBUG

	se = stack_top_and_reset() ;	/* Get the subexpression from the top of stack and reset it, without pop */

	operator<<( se.getExpression() )  ;	/* Process the expression, as string */

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished") ;

	return (*this) ; 
}
		
			
/*********************************************************************************************
**                                                                                          **
*                                       Stream operators                                     *
**                                                                                          **
*********************************************************************************************/


//
//  SQLStringStream::operator<<(const op&)
//
SQLStringStream& SQLStringStream::operator<<(const op& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called with argument: \"" << c.get_s() << "\"  currentLop: " << translateLogicalOp(_key, getCurrentLOp()));

	/* Check if the payload of op is a string */
	ACS_COND_THROW( (c.get_optype() != op::OP_STRING), \
							  Exception("dbParamStream::operator<<  Unsupported type of operation") ) ; 


	const string s = StringUtils::lowercase(StringUtils::trim(c.get_s())) ;
	string a = StringUtils::trim(c.get_alias()) ;
	
	ACS_COND_THROW( ((!a.empty()) && ((s!=")") && (s!="}"))), Exception("SQLStringStream::operator<<(const op&)  alias not allowed") ) ;

	if (s == "and") {
		ACS_COND_THROW((stack_size()<2), Exception("SQLStringStream::operator<<(const op&)  Logical operator AND requires a nested level!")) ;
		setCurrentLOp(LOP_AND) ; 		
	} else if (s == "or") {
		ACS_COND_THROW((stack_size()<2), Exception("SQLStringStream::operator<<(const op&)  Logical operator OR requires a nested level!")) ;
		setCurrentLOp(LOP_OR) ; 		
	} else if (s == "xor") {
		ACS_COND_THROW((stack_size()<2), Exception("SQLStringStream::operator<<(const op&)  Logical operator XOR requires a nested level!")) ;
		setCurrentLOp(LOP_XOR) ; 		
	} else if (s == "not") {
		/* Being a unary operator, a nested level is not required */
		setCurrentLOp(LOP_NOT) ; 		
	} 
	else if ((s == "(") || (s == "{"))
	{
		incr_nesting_level(s) ;
	} 
	else if ((s == ")") || (s == "}"))
	{
		if (stack_size() <= 1) {
			ostringstream errorMessage;
			errorMessage << "SQLStringStream& SQLStringStream::operator<<(const op&)  Mismatching operator : \"" << c.get_s() ;
			ACS_THROW(Exception(errorMessage.str() )) ;
		} 
		
		decr_nesting_level(s, a) ;
	}
	else 
	{
		ostringstream errorMessage;
		errorMessage << "SQLStringStream& SQLStringStream::operator<<(const op&)  Invalid SQLStringStream::operator: \"" << c.get_s() << "\"";
		ACS_THROW(Exception(errorMessage.str() )) ;
	}
	
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		stackDumpDebug(_stackExpressions) ;
    ACS_CLASS_END_DEBUG

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ;

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const op&)
//
SQLStringStream& SQLStringStream::operator<<(const dbParam::Operation& o)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;


	/* Check if the current token is DBPARAM */
	ACS_COND_THROW( (getCurrentToken()!= SQLDialect::DBPARAM), \
							  Exception("dbParamStream::operator<<(const dbParam::Operation&)  Unexpected token") ) ; 

	getCurrentDbParamStream().operator<<(o) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ;

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const dbGeoObject&)
//
SQLStringStream& SQLStringStream::operator<<(const dbGeoObject& o)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;


	/* Check if the current token is DBPARAM */
	ACS_COND_THROW( (getCurrentToken()!= SQLDialect::DBPARAM), \
							  Exception("dbParamStream::operator<<(const dbParam::Operation&)  Unexpected token") ) ; 

	getCurrentDbParamStream().operator<<(o) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ;

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const subquery&)
//
SQLStringStream& SQLStringStream::operator<<(const subquery& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	if (getCurrentQueryType() != SQLString::QUERY_UNDEFINED)
	{
			ACS_THROW( Exception("SQLStringStream::operator<<(const subquery& c)  Unable to create a new subquery (stack level already set)") ) ;
	}

	SQLString::QueryType t = c.get_querytype() ;

	setCurrentQueryType(t) ;
	
	switch(t) {
		case SQLString::INSERT :
			setCurrentToken(SQLDialect::INSERT) ; break ;
		case SQLString::DELETE :
			setCurrentToken(SQLDialect::DELETE) ; break ; 
		case SQLString::DROP :
			setCurrentToken(SQLDialect::DROP) ; break ; 
		case SQLString::SELECT :
			setCurrentToken(SQLDialect::SELECT) ; break ;
        case SQLString::UPDATE :
 			setCurrentToken(SQLDialect::UPDATE) ; break ; 
		case SQLString::FUNCTION :
        case SQLString::CUSTFUNCTION :
 			setCurrentToken(SQLDialect::CUSTFUNCTIONNAME) ; break ; 
        case SQLString::CUSTPROCEDURE :
 			setCurrentToken(SQLDialect::CUSTPROCEDURENAME) ; break ; 
		default:
		{
			ostringstream errorMessage;
			errorMessage << "SQLStringStream::operator<<(const subquery&)  Unsupported qyerytype " << static_cast<int>(t) ;
			ACS_THROW(Exception(errorMessage.str() )) ;
		}
	
	}	/* END switch */

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const values&)
//
SQLStringStream& SQLStringStream::operator<<(const values& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( (getCurrentToken() != SQLDialect::DBPARAM), \
		Exception("SQLStringStream::operator<<(const values&)  Identifier \"values\" only allowed as param" ) ) ;

	/* IF HERE: Current token is a DBPARAM */

	getCurrentDbParamStream().operator<<(c) ;

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const param&)
//
SQLStringStream& SQLStringStream::operator<<(const param& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	const string brakets = StringUtils::lowercase(StringUtils::trim(c.get_brakets())) ;
	
	if ( (brakets==")") || (brakets=="}") ) 
	{
			decr_nesting_level( brakets, StringUtils::trim(c.get_alias()) ) ;	/* Decrease nesting level. Analogue to:  op("()")  */
			return (*this) ;	/* Return immediately */
	}

	if ( (brakets=="(") || (brakets=="{") ) 
	{ 
		incr_nesting_level(brakets) ;	/* Increase nesting level. Analogue to:  op("(") << param()  */
	}	
	else if ( (getCurrentQueryType() != SQLString::QUERY_UNDEFINED) )
	{
		ACS_THROW( Exception("SQLStringStream::operator<<(const param&)  Unable to create a new param (stack level already set)") ) ;
	}

	setCurrentToken(SQLDialect::DBPARAM) ; 

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const fpname&)
//
SQLStringStream& SQLStringStream::operator<<(const fpname& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;


	switch( getCurrentQueryType() )
	{
		case SQLString::FUNCTION :
		case SQLString::CUSTFUNCTION :
			getCurrentSQLString().setFunctionName(c.get_v()) ;
			setCurrentToken(SQLDialect::CUSTFUNCTIONARG) ;
			break ;
	
		case SQLString::CUSTPROCEDURE :   
			getCurrentSQLString().setProcedureName(c.get_v()) ;
			setCurrentToken(SQLDialect::CUSTPROCEDUREARG) ;
			break ; 

		default:
			ACS_THROW( Exception("SQLStringStream::operator<<(const fpname& c)  Unsupported query type: " + getCurrentQueryTypeAsString()) ) ;
	}

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const addfunction&)
//
SQLStringStream& SQLStringStream::operator<<(const addfunction& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Function name: \"" << c.get_n() << "\"") ;

	SQLString tmp(_key, SQLString::FUNCTION) ;
	tmp.setFunctionName(c.get_n()) ;
	
	const vector <string> v = c.get_argv() ;
	for (vector<string>::const_iterator i=v.begin() ; i!=v.end(); i++) {
		tmp.addFunctionParameter( *i ) ; 
	}
	
	const bool nested = true ;
	operator<<( tmp.getSQL(nested) ) ;	/* as string */
	
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const column&)
//
SQLStringStream& SQLStringStream::operator<<(const column& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Query type: " + getCurrentQueryTypeAsString()) ;

	if (getCurrentToken() == SQLDialect::DBPARAM) {
		getCurrentDbParamStream().operator<<(c) ;
	}
	else
	{
		ACS_COND_THROW( (getCurrentQueryType() != SQLString::SELECT), \
			Exception("SQLStringStream::operator<<(const column&)  Token \"column\" only allowed in SELECT queries" ) ) ;

		setCurrentToken(SQLDialect::SELECT_LIST) ;

		if (! c.get_selString().empty()) {
			getCurrentSQLString().addSelectList( c.get_selString(), c.get_sqlCast(), c.get_lbl() ) ;
		}
	}

	return (*this) ; 
}


//
//  SQLStringStream::operator<<(const set&)
//
SQLStringStream& SQLStringStream::operator<<(const set& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Query type: " + getCurrentQueryTypeAsString()) ;

	switch( getCurrentQueryType() )
	{
		case SQLString::INSERT :
			getCurrentSQLString().addValuesList( c.get_k(), c.get_v() ) ;
			break ;
		case SQLString::UPDATE :
			getCurrentSQLString().addSetList( c.get_k(), c.get_v() ) ;
			break ;
		default:
			ACS_THROW( Exception("SQLStringStream::operator<<(const set&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;
	}

	return (*this) ; 
}


//
//  SQLStringStream::operator<<(const table&)
//
SQLStringStream& SQLStringStream::operator<<(const table& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Query type: " + getCurrentQueryTypeAsString()) ;
	
	if (getCurrentToken() == SQLDialect::DBPARAM) {
		getCurrentDbParamStream().operator<<(c) ;
	}
	else
	{
		ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
				&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const table&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

		setCurrentToken(SQLDialect::FROM) ;
		const string tableName = c.get_v() ;
		if (! tableName.empty() )
		getCurrentSQLString().addTableList( tableName, c.get_lbl() ) ;
	}

	return (*this) ; 
}


//
//  SQLStringStream::operator<<(const join&)
//
SQLStringStream& SQLStringStream::operator<<(const join& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( (getCurrentQueryType() != SQLString::SELECT), \
		Exception("SQLStringStream::operator<<(const join&)  Token \"join\" only allowed in SELECT queries" ) ) ;
	
	/* Store in the status the join-specific attributes */
	setCurrentJoinToken(c.get_token()) ; setCurrentName(c.get_table()) ; 

	setCurrentToken(SQLDialect::JOIN) ;
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const where&)
//
SQLStringStream& SQLStringStream::operator<<(const where& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const where&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	setCurrentToken(SQLDialect::WHERE) ;
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const orderby&)
//
SQLStringStream& SQLStringStream::operator<<(const orderby& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const orderby&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	setCurrentToken(SQLDialect::ORDERBY) ;
	setCurrentSortDirection(c.get_o()) ;
	const string columnName = c.get_s() ;
	if (! columnName.empty() ) { getCurrentSQLString().addOrderByList(columnName, c.get_o()) ; }

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const orderby_string&)
//
SQLStringStream& SQLStringStream::operator<<(const orderby_string& c)
{
	/**
		A pre-formatted string is passed as ORDER BY
	*/
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const orderby_string&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	getCurrentSQLString().setOrderByString( c.get_s() ) ; 

	return (*this) ; 
}


//
//  SQLStringStream::operator<<(const groupby&)
//
SQLStringStream& SQLStringStream::operator<<(const groupby& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const groupby&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	setCurrentToken(SQLDialect::GROUPBY) ;
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const groupby_string&)
//
SQLStringStream& SQLStringStream::operator<<(const groupby_string& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const groupby_string&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	getCurrentSQLString().setGroupByString( c.get_s() ) ; 	
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const having&)
//
SQLStringStream& SQLStringStream::operator<<(const having& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const having&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	setCurrentToken(SQLDialect::HAVING) ;
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const limit&)
//
SQLStringStream& SQLStringStream::operator<<(const limit& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	ACS_COND_THROW( ((getCurrentQueryType() != SQLString::DELETE) && (getCurrentQueryType() != SQLString::DROP) \
			&& (getCurrentQueryType() != SQLString::INSERT) && (getCurrentQueryType() != SQLString::SELECT) && (getCurrentQueryType() != SQLString::UPDATE)), \
			Exception("SQLStringStream::operator<<(const limit&)  Operator unexpected for query type: " + getCurrentQueryTypeAsString() ) ) ;

	getCurrentSQLString().setLimit(c.get_v()) ; 
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const readonly&)
//
SQLStringStream& SQLStringStream::operator<<(const readonly& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	ACS_COND_THROW( (getCurrentQueryType() != SQLString::SELECT), \
		Exception("SQLStringStream::operator<<(const readonly&)  Token \"readonly\" only allowed in SELECT queries" ) ) ;

	getCurrentSQLString().setReadOnlySelect(c.get_v()) ; 
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const distinct&)
//
SQLStringStream& SQLStringStream::operator<<(const distinct& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	ACS_COND_THROW( (getCurrentQueryType() != SQLString::SELECT), \
		Exception("SQLStringStream::operator<<(const distinct&)  Token \"distinct\" only allowed in SELECT queries" ) ) ;

	getCurrentSQLString().setDistinct(c.get_v()) ; 
	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const vector<string> &)
//
SQLStringStream& SQLStringStream::operator<<(const vector<string> &c) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	switch (getCurrentToken())
    {
		case SQLDialect::DBPARAM :
		{	
			getCurrentDbParamStream().operator<<(c) ;
		}
		break ;
		
		default:
			ACS_THROW( Exception("SQLStringStream::operator<<(const vector<T>&)  Vector entry not supported by this token" ) ) ;
	}
  
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ;

	return (*this) ;	
}


//
//  SQLStringStream::operator<<(const string&)
//
SQLStringStream& SQLStringStream::operator<<(const string& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for string: \"" << c << "\"") ;
	
	switch (getCurrentToken())
    {
	
		case SQLDialect::TOKEN_UNDEFINED :
		{
			stack_addtotop(c) ;
			setCurrentLOp(LOP_UNDEF) ;	/* To prevent the piping of expressions without logical operators */
			break ; 
		}
	
		case SQLDialect::DBPARAM :
		{
			getCurrentDbParamStream().operator<<(c) ;
			break ; 
		}

		case SQLDialect::FUNCTIONARG :
		case SQLDialect::CUSTFUNCTIONARG :
		{
			getCurrentSQLString().addFunctionParameter(c) ;
			break ;
		}
	
		case SQLDialect::CUSTPROCEDUREARG :
		{     
			getCurrentSQLString().addProcedureParameter(c) ;
			break ;
		}
	
		case SQLDialect::SELECT :
		case SQLDialect::SELECT_LIST :
		{
			getCurrentSQLString().addSelectList( c ) ;
			break ;
		}			
	
		case SQLDialect::FROM :
		case SQLDialect::UPDATE :
		case SQLDialect::DROP   :
		case SQLDialect::INSERT :
		case SQLDialect::DELETE :
		{
			getCurrentSQLString().addTableList( c ) ;
			break ;
		}
			
		case SQLDialect::GROUPBY  :
		{
			getCurrentSQLString().addGroupByList( c ) ;
			break ; 
		}

		case SQLDialect::ORDERBY :
    	{	
			getCurrentSQLString().addOrderByList( c, getCurrentSortDirection() ) ;
			break ; 
		}
	
	
		case SQLDialect::WHERE :
		{	
			getCurrentSQLString().addANDWhereParameter( c ) ;
			break ; 
		}
   
   
   		case SQLDialect::JOIN :
		{	
			getCurrentSQLString().addJoinList( getCurrentJoinToken(), getCurrentName(), c ) ;
			break ; 
		}
   
   
   		case SQLDialect::HAVING :
		{	
			getCurrentSQLString().addHavingParameter(c) ;
			break ; 
		}
 
   
		default:
		{
			ostringstream errorMessage;
			errorMessage << "SQLStringStream& SQLStringStream::operator<<(const string& c)  Unsupported token : \"" 
				<< SQLDialect::debugSQLToken( getCurrentToken() ) << "\"" ;
			ACS_THROW(Exception(errorMessage.str() )) ;
		}	
	
	
    }	/* switch */
	
	
	/*
	    TEMPLATE_INVALID=0,    
        SET=3,
        VALUES=7,
        VALUES_LIST=8,
        COLUMN_LIST=9,
        LIMIT=10,
        DISTINCT=11,
        SELECT_LIST=12,
        SET_LIST=13,
        TABLE_LIST=15,
        FILENAME=21,
        CUSTFUNCTIONNAME=26,
        NESTEDFUNCTIONNAME=27,
        CUSTNESTEDFUNCTIONNAME=28,
        CUSTNESTEDPROCEDURENAME=30,
        COLUMNNAME=31,
        FORREADONLY=32,
   */

	return (*this) ; 
} 


//
//  SQLStringStream::operator<<(const SQLString&)
//
SQLStringStream& SQLStringStream::operator<<(const SQLString& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	const bool nested = true ; 
	return operator<<( string ( " ( " + (c.getSQL(nested)) + " ) " ) ) ; 
}


//
//  SQLStringStream::operator<<(const SQLStringStream&)
//
SQLStringStream& SQLStringStream::operator<<(const SQLStringStream& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	const bool nested = true ; 
	return operator<<( string ( " ( " + (c.getSQL(nested)) + " ) " ) ) ; 
}


//
//  SQLStringStream::operator<<(const dbQueryParameters&)
//
SQLStringStream& SQLStringStream::operator<<(const dbQueryParameters& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	SQLStringStream::operator<<( c.getSqlString() ) ; 

	return (*this) ; 
}


//
//  SQLStringStream::operator<<(const dbParam&)
//
SQLStringStream& SQLStringStream::operator<<(const dbParam& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	dbQueryParameters t(_key) ; t.addParameter(c) ; 
	SQLStringStream::operator<<( t.getSqlString() ) ;

	return (*this) ; 
}


//
//  SQLStringStream::operator<<(const dbParam&)
//
SQLStringStream& SQLStringStream::operator<<(const dbParamStream& c)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	return operator<<( string ( " ( " + (c.getSQL()) + " ) " ) ) ; 
}


//
//  SQLStringStream::translateLogicalOp
//
string SQLStringStream::translateLogicalOp(const string& key, SQLStringStream::LogicalOp o) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
		
	string s ;
	switch(o & (~ LOP_NOT) )	/* Remove the NOT, if present */
	{
		case LOP_AND:
			s = SQLDialectFactory::sqlDialect( key ).logicalAnd() ;
			break ;
		case LOP_OR:
			s = SQLDialectFactory::sqlDialect( key ).logicalOr() ;
			break ;
		case LOP_XOR:
			s = SQLDialectFactory::sqlDialect( key ).logicalXor() ;
			break ;
		default:
			s = "LOP_UNDEFINED" ; 
	}	/* switch */

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning: \"" << s << "\"") ;
	return s ;
}



/*********************************************************************************************
**                                                                                          **
*                            SQLStringStream  STACK HANDLERS                                 *
**                                                                                          **
*********************************************************************************************/


//
//  SQLStringStream::stack_push
//
void SQLStringStream::stack_push(const SQLStringStream::SubExpression& e) 
{ 
/**
	Create a new level, adding a complete SubExpression
*/

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;
	
	_stackExpressions.push(e) ; 
}


//
//  SQLStringStream::stack_push
//
void SQLStringStream::stack_push(const SQLStringStream::Status& status) 
{ 
/**
	Create a new level, adding an empty SubExpression (with only Status full) 
*/
   
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	SubExpression empty_se(status) ;	/* Create an empty subexpression, but w/ a proper status */
	stack_push(empty_se) ;
}


//
//  SQLStringStream::stack_push_empty
//
void SQLStringStream::stack_push_empty() 
{ 
/**
	Create a new level, adding an empty SubExpression (with only Status full) 
*/
   
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	SubExpression empty_se(_key) ;	/* Create an empty subexpression */
	stack_push(empty_se) ;
}


void SQLStringStream::stack_updatetop(const SQLStringStream::Status& status) 
{ 
/**
	Update the level at the top of the stack, setting a new Status 
*/

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	SubExpression se = stack_pop() ;	/* Exception will be thrown if empty stack */
	se.setStatus(status) ; 
	stack_push(se) ;
}


//
//  SQLStringStream::stack_addtotop
//
void SQLStringStream::stack_addtotop(const string& q)
{ 
/**
	Update the level at the top of the stack, adding to the subexpression the QueryParameter   
*/

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	SubExpression se = stack_pop() ;	/* Exception will be thrown if empty stack */

	se.addParam(q) ; 
	stack_push(se) ;
}



//
//  SQLStringStream::stack_pop
//
SQLStringStream::SubExpression SQLStringStream::stack_pop() 
{
/**
	Returns and removes the level at the top of the stack
*/
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	ACS_COND_THROW( (_stackExpressions.size()<1), Exception("SQLStringStream::stack_pop() Cannot pop an empty stack!") ) ;

	SubExpression se(_stackExpressions.top()) ; 
	_stackExpressions.pop();

	return se ;
}


//
//  SQLStringStream::stack_top
//
SQLStringStream::SubExpression SQLStringStream::stack_top() const
{
/**
	Returns and removes the level at the top of the stack
*/
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	ACS_COND_THROW( (_stackExpressions.size()<1), Exception("SQLStringStream::stack_pop() Cannot pop an empty stack!") ) ;

	SubExpression se(_stackExpressions.top()) ; 

	return se ;
}
	

//
//  SQLStringStream::stack_top
//
SQLStringStream::SubExpression SQLStringStream::stack_top_and_reset() 
{
/**
	Returns and removes the level at the top of the stack
*/
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called") ;

	ACS_COND_THROW( (_stackExpressions.size()<1), Exception("SQLStringStream::stack_pop() Cannot pop an empty stack!") ) ;

	SubExpression se(_stackExpressions.top()) ; 

	_stackExpressions.top().resetExpression() ;
	
	return se ;
}


//
//  SQLStringStream::stack_size
//
size_t SQLStringStream::stack_size() 
{    
/**
	Returns the stack size. It should be always >0 (even without nested levels, it should contain exactly one subexpression)
*/
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called. Returning: " << _stackExpressions.size()) ;
 
	return _stackExpressions.size(); 
} 
