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

	$Prod: A.C.S. dbParamStream Header $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 1.2  2012/07/26 13:40:18  enrcar
	EC:: Classes column() and table() can be used to arbitrarily sort the tokens.
	
	Revision 1.1  2012/07/24 09:20:53  enrcar
	EC:: Imported source file
	


*/

#ifndef _dbParamStream_H_
#define _dbParamStream_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <vector>

#include <dbParam.h>
#include <SQLDialect.h>


namespace acs {

	class SQLString ;
	class SQLStringStream ;
	class dbGeoObject ;

	/*
	*
   	*     Manipulator functions and stream operators (coupled) :
	*
	*/


	class op	
	{
		public:
			enum OpType { OP_UNDEFINED=0, OP_STRING=1, OP_DBPARAMOPER=2 } ;
	
		public:
			op(dbParam::Operation o) : _o(o), _s(), _alias(), _optype(OP_DBPARAMOPER) {}
    		op(const std::string& s, const std::string& a="") :  _o(dbParam::NONE), _s(s), _alias(a), _optype(OP_STRING) {}
			virtual ~op() noexcept {}
			dbParam::Operation get_o () const { return _o ; } 
			std::string get_s () const { return _s ; } 
			std::string get_alias () const { return _alias ; } 
			OpType get_optype () const { return _optype ; } 
		private:
			dbParam::Operation _o ;
			std::string _s, _alias ;
			OpType _optype ; 
	} ;
			
	
	class column
	{
		public:
			column() ;
			column(std::string const &sel, SQLDialect::SQLCast const &sqlCast, std::string const &lbl = "") ;
			column(std::string const &sel, std::string const &lbl = "") ;
			column(SQLString const &sel, SQLDialect::SQLCast const &sqlCast, std::string const &lbl = "") ;
			column(SQLString const &sel, std::string const &lbl = "") ;
			column(SQLStringStream const &sel, SQLDialect::SQLCast const &sqlCast, std::string const &lbl = "") ;
			column(SQLStringStream const &sel, std::string const &lbl = "") ;
			virtual ~column() noexcept ;
			std::string get_selString() const { return _selString ; }
			SQLDialect::SQLCast get_sqlCast() const { return _sqlCast ; }
			std::string get_lbl() const { return _lbl ; }

		private:
			std::string _selString ;
			SQLDialect::SQLCast _sqlCast ;
			std::string _lbl ;
	} ;	
			
	
	class table
	{
		public:
    		table(std::string const &v, std::string const &lbl = "") : _v(v), _lbl(lbl) {}
    		table() : _v(), _lbl() {}
			virtual ~table() noexcept {}
			std::string get_v() const { return _v ; }
			std::string get_lbl() const { return _lbl ; }
		private:
    		std::string _v, _lbl ; 
	} ; 
		
		
	class values
	{
		public:
			values() : _v() {}
			values(const std::string& c) : _v() { _v.push_back(c) ; }
			template <typename T>
	    		values(const std::vector<T> c) : _v() { for (size_t i=0; i<c.size(); i++) { 
					std::ostringstream o ; o << c[i] ; _v.push_back( o.str() ) ; } }
			values(const char* c) : _v()  { _v.push_back( std::string(c) ) ; }
			values(const signed long long int& c) : _v() { std::ostringstream o ; o << c ; _v.push_back( o.str() ) ; }
			values(const int& c) : _v() { std::ostringstream o ; o << c ; _v.push_back( o.str() ) ; }
			values(const double& c) : _v() { std::ostringstream o ; o << c ; _v.push_back( o.str() ) ; }
			virtual ~values() noexcept { }
			const std::vector<std::string>& get_v() const { return _v ; }
		private:
    		std::vector<std::string> _v ; 
	} ; 
	
	
	class dbParamStream : std::ostream /*, public ThreadSafe*/
	{

	public:

		/*! class Exception declaration */

		exDECLARE_EXCEPTION(Exception, dbParam::InvalidParameter) ;	

		dbParamStream(const std::string& key) ;
		virtual ~dbParamStream() noexcept ;
		dbParamStream(const dbParamStream &) ;
		dbParamStream &operator=(const dbParamStream &) ;


		std::string getSQL() const ;
		dbParam getDbParam() const ;


		/*
		*
   		*     Manipulator functions and stream operators (coupled) :
		*
		*/

#if(0)
		dbParamStream& operator<<(const op&) ;	/* Implemented, but never used */
#endif
		dbParamStream& operator<<(const table&) ; 
		dbParamStream& operator<<(const column&) ; 
		dbParamStream& operator<<(const values&) ; 
		dbParamStream& operator<<(const dbParam::Operation&) ; 
		dbParamStream& operator<<(const SQLString &s) ;
		dbParamStream& operator<<(const SQLStringStream &s) ;
		dbParamStream& operator<<(const dbGeoObject &s) ;


		/*
		*
   		*     Other stream manipulators :
		*
		*/

		dbParamStream& operator<<(const std::string& c) ;
		dbParamStream& operator<<(const char* c) { return operator<<( std::string(c) ) ; }
		dbParamStream& operator<<(const signed long long int& c) { std::ostringstream o ; o << c ;  return operator<<( o.str() ) ; }
		dbParamStream& operator<<(const int& c) { std::ostringstream o ; o << c ;  return operator<<( o.str() ) ; }
		dbParamStream& operator<<(const double& c) { std::ostringstream o ; o << c ;  return operator<<( o.str() ) ; }
		template <typename T>
	    	dbParamStream& operator<<(const std::pair<T, T> c) { std::ostringstream f, s ; f << c.first ; s << c.second ; 
				operator<<( f.str() ) ; operator<<( s.str() ) ; return *this ; }
		template <typename T>
	    	dbParamStream& operator<<(const std::vector<T> c) { for (size_t i=0; i<c.size(); i++) { 
				std::ostringstream o ; o << c[i] ; operator<<( o.str() ) ; } return *this ; }

		/*
		*
   		*     excerr and ostream manipulators :
		*
		*/

		friend exostream& operator<<(exostream &, const dbParamStream&) ;		/* to excerr */
		friend std::ostream& operator<<(std::ostream& , const dbParamStream&);	/* to ostream */


	private:	
		std::string _key ;						/* Connection key */
		std::string _table, _column ;			/* Table and Column (lvalues) */
		std::vector <std::string> _right_v;		/* Vector of rvalues */
		dbParam::Operation _oper ;				/* Operation */
		SQLDialect::SQLToken _currentStatus ;	/* To implement a FSM (Finite-State-Machine). Possible states are only: TABLE_LIST, COLUMN_LIST and VALUES_LIST */

    	exCLASS_DECLARE_DEBUG_LEVEL

	} ;


}//namespace acs

#endif
