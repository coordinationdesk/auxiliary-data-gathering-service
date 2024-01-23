// PRQA S 1050 EOF
/*
TODO:
ADD A LOCK TO OPERATORS<<

CHECK WHAT HAPPENS IF THERE ARE MISSING PARENTHESIS E.g. where() << op("(") << ... ;
HANDLE MULTI-INSERT (?)

*/



/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SQLStringStream Header $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 1.11  2012/12/11 17:30:10  marpas
	qa rules
	
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

#ifndef _SQLStringStream_H_
#define _SQLStringStream_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <vector>

#include <dbQueryParameters.h>
#include <dbParamStream.h>
#include <ThreadSafe.h>
#include <SQLString.h>
#include <SQLDialect.h>
#include <StringUtils.h>
#include <exException.h>


namespace acs {


	/*
	*
   	*     Manipulator functions and stream operators (coupled) :
	*
	*/


	class SQLStringStream ; 	/* Forward declaration */
	class dbGeoObject ; 	/* Forward declaration */


	class param
	{
		public:
    		param() : _brakets(), _alias() {}
    		param(const std::string& brakets, const std::string& alias="") : _brakets(brakets), _alias(alias) {}
			virtual ~param() throw() {}
		 	std::string get_brakets () const { return _brakets ; } 
 		 	std::string get_alias() const { return _alias ; } 
		private:
   			std::string _brakets, _alias ;
	} ;


	class subquery
	{
		public:
    		subquery(const SQLString::QueryType &q) : _q(q) {} 
			virtual ~subquery() {} 
			SQLString::QueryType get_querytype() const { return _q ; } 
		private:
			SQLString::QueryType _q ;
	} ;

	
	class fpname
	{
		public:
    		fpname(const std::string& v) : _v(v) {}
			virtual ~fpname() throw() {}
		 	std::string get_v () const { return _v ; } 
		private:
    		std::string _v;
	};


	class addfunction
	{
		public:
   			
			addfunction(const std::string& n) : _n(n), _argv() {}  

			template <typename T> 
			addfunction(const std::string& n, const T& arg1) : _n(n), _argv() { std::ostringstream o ; o << arg1 ; _argv.push_back(o.str()); } 
	
			template <typename T1, typename T2>
			addfunction(const std::string& n, const T1& arg1, const T2& arg2) : _n(n), _argv() { 
				{ std::ostringstream o ; o << arg1 ; _argv.push_back(o.str()); } 
				{ std::ostringstream o ; o << arg2 ; _argv.push_back(o.str()); }  }

			template <typename T1, typename T2, typename T3>
			addfunction(const std::string& n, const T1& arg1, const T2& arg2, const T3& arg3) : _n(n), _argv() { 
				{ std::ostringstream o ; o << arg1 ; _argv.push_back(o.str()); } 
				{ std::ostringstream o ; o << arg2 ; _argv.push_back(o.str()); }  
				{ std::ostringstream o ; o << arg3 ; _argv.push_back(o.str()); }  }

			template <typename T>
	    	addfunction(const std::string& n, const std::vector<T>& argv) : _n(n), _argv() {
				for(size_t i=0; i<argv.size(); i++) { std::ostringstream o ; o << argv[i] ; _argv.push_back(o.str()) ; } 
			}
			virtual ~addfunction() throw() {}
		 	std::string get_n () const { return _n ; } 
		 	std::vector <std::string> get_argv () const { return _argv ; } 
		private:
    		std::string _n ;
			std::vector <std::string> _argv ;
	};
	
	
	class set
	{
		/**
			Used for both UPDATE and INSERT
		*/
		public:
    		set(const std::string& k, const std::string& v) : _k(k), _v(v) {}
			virtual ~set() throw() {}
		 	std::string get_k () const { return _k ; } 
		 	std::string get_v () const { return _v ; } 
		private:
    		std::string _k, _v ;
	} ;

	
	class join
	{
		public:
			join(SQLDialect::SQLJoinToken token, std::string const &table) : _token(token), _table(table) {} 
			virtual ~join() throw() {}
			SQLDialect::SQLJoinToken get_token() const { return _token ; }
			std::string get_table() const { return _table ; }
		private:
    		SQLDialect::SQLJoinToken _token ;
			std::string _table ;
	} ;

	
	class where
	{
		public:
		    where() {}
			virtual ~where() throw() {}
	} ;


	class orderby
	{
		public:
    		orderby(const std::string& s, const SQLString::OrderBy& o = SQLString::SORT_DEFAULT) : _s(s), _o(o) {}
    		orderby(const SQLString::OrderBy& o = SQLString::SORT_DEFAULT) : _s(""), _o(o) {}
			virtual ~orderby() throw() {}
			SQLString::OrderBy get_o() const { return _o ; } 
			std::string get_s() const { return _s ; } 
		private:
			std::string _s ;
			SQLString::OrderBy _o ;
	};

		
	class orderby_string
	{
		/**
			A pre-formatted string is passed as ORDER BY
		*/
		public:
    		orderby_string(const std::string& s) : _s(s) {}
			virtual ~orderby_string() throw() {}
			std::string get_s() const { return _s ; } 
		private:
			std::string _s ;
	};

	
	class groupby
	{
		public:
    		groupby() {}
			virtual ~groupby() throw() {}
	};


	class groupby_string
	{
		/**
			A pre-formatted string is passed as GROUP BY
		*/
		
		public:
    		groupby_string(const std::string& s) : _s(s) {}
			virtual ~groupby_string() throw() {}
			std::string get_s() const { return _s ; } 
		private:
			std::string _s ;
	};


	class having
	{
		public:
		    having() {}
			virtual ~having() throw() {}
	} ;
	
				
	class limit
	{
		public:
    		limit(const unsigned int& v) : _v(v) {}
			virtual ~limit() throw() {}
		 	unsigned get_v () const { return _v ; } 
		private:
    		unsigned int _v;
	};


	class readonly
	{
		public:
    		readonly(bool v=true) : _v(v) {}
			virtual ~readonly() throw() {}
		 	bool get_v () const { return _v ; } 
		private:
    		bool _v;
	};

	
	class distinct
	{
		public:
    		distinct(bool v=true) : _v(v) {}
			virtual ~distinct() throw() {}
		 	bool get_v () const { return _v ; } 
		private:
    		bool _v;
	};


class SQLStringStream : std::ostream /*, public ThreadSafe*/
{

public:

	/*! class Exception declaration */

	exDECLARE_EXCEPTION(Exception, SQLString::Exception) ;


	
	enum LogicalOp
	{
		LOP_UNDEF = 0x00,	/* Do not change sorting and values */
		LOP_NOT   = 0x01,	
		LOP_AND   = 0x02, 
		LOP_OR    = 0x04,
		LOP_XOR   = 0x08
	} ;
	
		
	class Status 
	{
		public :
			Status(const std::string& key, const SQLString::QueryType &) ;
			virtual ~Status() throw() ;
			Status(const Status &rhs) ;
			Status& operator=(const Status &rhs) ;
	
			void setToken(SQLDialect::SQLToken t) { _currentToken = t ; }
			SQLDialect::SQLToken getToken() const { return _currentToken ; }
			void setLOp(LogicalOp t) { if (t != LOP_NOT) _logicalOp = t ; else _logicalOp = static_cast<LogicalOp>( _logicalOp | LOP_NOT ) ;}
			LogicalOp getLOp() const { return _logicalOp ; }
			void setName(const std::string& t) { _name = t ; }
			std::string getName() const { return _name ; }
			void setAlias(const std::string& t) { _alias = t ; }
			std::string getAlias() const { return _alias ; }
			std::string getKey() const { return _key ; }
			void setJoinToken(SQLDialect::SQLJoinToken t) { _joinToken = t ; }
			SQLDialect::SQLJoinToken getJoinToken() const { return _joinToken ; }
			void setSortDirection(SQLString::OrderBy t) { _sortDirection = t ; }
			SQLString::OrderBy getSortDirection() const { return _sortDirection ; }
			SQLString::QueryType getQueryType() const { return _sqlString.getType() ; }
			void setQueryType(const SQLString::QueryType &q ) { return _sqlString.setType(q) ; }

			SQLString& getSQLString() { return _sqlString ; } 
			const SQLString& getSQLString() const { return _sqlString ; } 

			dbParamStream& getDbParamStream() { return _paramStream ; } 
			const dbParamStream& getDbParamStream() const { return _paramStream ; } 

		private:
			SQLDialect::SQLToken _currentToken ;
			LogicalOp _logicalOp ; 
			std::string _name ;						/* String payload. Used by JOIN to store the tablename, by FUNC/PROC to store the func/proc name */ 
			std::string _alias ;					/* Alias */
			std::string _key ;						/* Connection key */
			SQLDialect::SQLJoinToken _joinToken ;	/* JOIN -specific */
			SQLString::OrderBy _sortDirection ;		/* ORDER BY -specific */
			SQLString 		_sqlString ;
			dbParamStream	_paramStream ;
	} ;
	
	
	class SubExpression 
	{
		public:
			SubExpression(const std::string& key) ;
			SubExpression(const Status& s) ;
			virtual ~SubExpression() throw() ;
			SubExpression(const SubExpression &rhs) ;
			SubExpression& operator=(const SubExpression &rhs) ;

			void addParam(const std::string& query) ;
			const Status& getStatus() const { return _status ; }
			Status& getStatus() { return _status ; }
			void setStatus(const Status &status) { _status = status ; }
		
			const dbParamStream& getDbParamStream() const { return _status.getDbParamStream() ; }
			dbParamStream& getDbParamStream() { return _status.getDbParamStream() ; }
	
			const SQLString& getSQLString() const { return _status.getSQLString() ; }
			SQLString& getSQLString() { return _status.getSQLString() ; }
			std::string getKey() const { return _status.getKey() ; }
			std::string getExpression() const { return _expression ; }
			void resetExpression() { _expression = "" ; }
			std::string getSQL() const ;

		private:
			SQLStringStream::Status _status ;
			std::string _expression ;
	} ;
	
	
	SQLStringStream(const std::string& key, SQLString::QueryType t) ;
	virtual ~SQLStringStream() throw() ;
	SQLStringStream(const SQLStringStream &) ;
	SQLStringStream &operator=(const SQLStringStream &) ;

	SQLStringStream& decr_nesting_level(const std::string& braket, const std::string& a) ;
	SQLStringStream& incr_nesting_level(const std::string& braket) ;

	std::string getSQL(bool nested=false) const { return getCurrentSQLString().getSQL(nested) ; }


	/*
	*
   	*     Manipulator functions and stream operators (coupled) :
	*
	*/
	
	
	friend class op ;
	SQLStringStream& operator<<(const op&) ; 

	
	friend class subquery ;
	SQLStringStream& operator<<(const subquery&) ; 


	friend class param ;
	SQLStringStream& operator<<(const param&) ; 


	friend class values ;
	SQLStringStream& operator<<(const values&) ; 

	
	friend class fpname ;
	SQLStringStream& operator<<(const fpname&) ; 


	friend class function ;
	SQLStringStream& operator<<(const addfunction&) ; 

		
	friend class column ;
	SQLStringStream& operator<<(const column&) ; 

	
	friend class set ;
	SQLStringStream& operator<<(const set&) ; 

		
	friend class table ;
	SQLStringStream& operator<<(const table&) ; 

	
	friend class join ;
	SQLStringStream& operator<<(const join&) ; 

	
	friend class where ;
	SQLStringStream& operator<<(const where&) ; 
	

	friend class orderby ;
	SQLStringStream& operator<<(const orderby&) ; 

		
	friend class orderby_string ;
	SQLStringStream& operator<<(const orderby_string&) ; 

	
	friend class groupby ;
	SQLStringStream& operator<<(const groupby&) ; 


	friend class groupby_string ;
	SQLStringStream& operator<<(const groupby_string&) ; 


	friend class having ;
	SQLStringStream& operator<<(const having&) ; 

				
	friend class limit ;
	SQLStringStream& operator<<(const limit&) ; 

	friend class readonly ;
	SQLStringStream& operator<<(const readonly&) ; 

	
	friend class distinct ;
	SQLStringStream& operator<<(const distinct&) ; 


	/*
	*
   	*     Other stream manipulators :
	*
	*/
	
	template <typename T> 
		SQLStringStream& operator<<(const std::vector<T> &c) { std::vector <std::string> v ; 
			for (ssize_t i=0; i<v.size(); i++) { std::ostringstream o ; o << c[i] ; v.push_back(o.str()) ; }
			return operator<<(v) ; }
	SQLStringStream& operator<<(const dbParam::Operation&) ; 
	SQLStringStream& operator<<(const dbGeoObject&) ; 
	SQLStringStream& operator<<(const std::vector<std::string> &c) ;
	SQLStringStream& operator<<(const std::string& c) ;
	SQLStringStream& operator<<(const char* c) { return operator<<( std::string(c) ) ; }
	SQLStringStream& operator<<(const signed long long int& c) { std::ostringstream o ; o << c ;  return operator<<( o.str() ) ; }
	SQLStringStream& operator<<(const int& c) { std::ostringstream o ; o << c ;  return operator<<( o.str() ) ; }
	SQLStringStream& operator<<(const double& c) { std::ostringstream o ; o << c ;  return operator<<( o.str() ) ; }
	SQLStringStream& operator<<(const SQLString& c) ;
	SQLStringStream& operator<<(const SQLStringStream& c) ;
	SQLStringStream& operator<<(const dbQueryParameters& c) ;
	SQLStringStream& operator<<(const dbParam& c) ;
	SQLStringStream& operator<<(const dbParamStream& c) ;
	

	/*
	*
   	*     excerr and ostream manipulators :
	*
	*/
	
	friend exostream& operator<<(exostream &, const SQLStringStream&) ;			/* to excerr */
	friend std::ostream& operator<<(std::ostream& , const SQLStringStream&);	/* to ostream */


private:	

	/* Status handlers */
	Status getCurrentStatus() const { return (stack_top()).getStatus() ; } 
	void setCurrentToken(SQLDialect::SQLToken t = SQLDialect::TOKEN_UNDEFINED) { Status status = getCurrentStatus() ; \
					status.setToken(t) ; status.setLOp(LOP_UNDEF) ; stack_updatetop(status) ; }
	SQLDialect::SQLToken getCurrentToken() const { return (getCurrentStatus()).getToken() ; }
	bool matchingToken(SQLDialect::SQLToken t) { return ( (t == (getCurrentStatus()).getToken()) ) ;} 
	void setCurrentLOp(LogicalOp o) { Status status = getCurrentStatus() ; status.setLOp(o) ; stack_updatetop(status) ; }
	LogicalOp getCurrentLOp() const { return (getCurrentStatus()).getLOp() ; }
	bool matchingLOp(LogicalOp o) { return ( (o == (getCurrentStatus()).getLOp()) ) ;} 
	void setCurrentName(const std::string& t) { Status status = getCurrentStatus() ; status.setName(t) ;  stack_updatetop(status) ; }
	std::string getCurrentName() const { return (getCurrentStatus()).getName() ; }
	void setCurrentAlias(const std::string& t) { Status status = getCurrentStatus() ; status.setAlias(t) ;  stack_updatetop(status) ; }
	std::string getCurrentAlias() const { return (getCurrentStatus()).getAlias() ; }
	void setCurrentJoinToken(const SQLDialect::SQLJoinToken t) { Status status = getCurrentStatus() ; \
					status.setJoinToken(t) ; stack_updatetop(status) ; }
	SQLDialect::SQLJoinToken getCurrentJoinToken() const { return (getCurrentStatus()).getJoinToken() ; }
	void setCurrentSortDirection(const SQLString::OrderBy t)  { Status status = getCurrentStatus() ; \
					status.setSortDirection(t) ; stack_updatetop(status) ; }
	SQLString::OrderBy getCurrentSortDirection() const { return (getCurrentStatus()).getSortDirection() ; }

	/* Status' dbParamStream handlers */
	dbParamStream& getCurrentDbParamStream() { return _stackExpressions.top().getDbParamStream() ; }
	const dbParamStream& getCurrentDbParamStream() const { return _stackExpressions.top().getDbParamStream() ; }

	/* Status' SQLString handlers */
	SQLString& getCurrentSQLString() { return _stackExpressions.top().getSQLString() ; }
	const SQLString& getCurrentSQLString() const { return _stackExpressions.top().getSQLString() ; }
	
	void setCurrentQueryType(SQLString::QueryType q) { getCurrentSQLString().setType(q) ; }
	SQLString::QueryType getCurrentQueryType() const { return getCurrentSQLString().getType() ; }
	std::string getCurrentQueryTypeAsString() const { return getCurrentSQLString().getTypeAsString() ; }


	/* Stack handlers */
	void stack_push(const SubExpression& e) ;
	void stack_push(const Status& status) ;
	void stack_push_empty() ;
	void stack_addtotop(const std::string& q) ;
	void stack_updatetop(const Status& status) ;
	SubExpression stack_pop() ;
	SubExpression stack_top() const ;
	SubExpression stack_top_and_reset() ;
	size_t stack_size() ;



	/* Debug handlers */
	static void stackDumpDebug(const std::stack<SubExpression>& stk) ;
	static void expressionDumpDebug(const SubExpression& se) ;
	static void statusDumpDebug(const std::string& key, const Status& s) ;
	static std::string translateLogicalOp(const std::string& key, LogicalOp) ;



	/* Stack */
	std::stack <SubExpression> _stackExpressions ;

	/* Key */
	std::string _key ;


    ACS_CLASS_DECLARE_DEBUG_LEVEL(SQLStringStream) ;

} ;



}//namespace acs

#endif
