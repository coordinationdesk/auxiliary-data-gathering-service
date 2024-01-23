// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 5.14  2016/10/20 10:20:00  marpas
	coding best practice applied
	fixing qa warnings
	
	Revision 5.13  2016/10/19 09:59:24  enrcar
	EC:: SQLString I/F extended in order to allow SQL CASE and SQL CAST as argument of both addSelectList and addWhereParamater
	
	Revision 5.12  2016/07/26 15:09:33  enrcar
	CSGACQ-87 EC:: Added support and test for CASE WHEN ELSE and TIME INTERVAL
	
	Revision 5.11  2016/07/21 14:25:47  enrcar
	CSGACQ-82  EC:: intervalToSeconds interface added
	
	Revision 5.10  2016/07/14 09:48:48  marfav
	Adding table alias support also in join clauses
	
	Revision 5.9  2016/06/21 08:34:39  damdec
	clearOrderBy method added.
	
	Revision 5.8  2016/05/04 10:08:32  lucio.pulvirenti
	addFunctionParameter and addProcedureParameter bug fixed: db::toSQL introduces unwanted quotes.
	
	Revision 5.7  2016/04/29 12:16:19  marpas
	fixed addFunctionParameter and addProcedureParameter bug when used with a string (or const char *)
	
	Revision 5.6  2016/03/14 15:39:14  marpas
	introducing offset in queries
	
	Revision 5.5  2015/09/30 09:09:19  enrcar
	EC:: New type of query: UNION handled
	
	Revision 5.4  2015/08/28 13:16:39  marfav
	Added support for update using a nested SQLString as value
	
	Revision 5.3  2015/02/17 14:16:24  enrcar
	EC:: Added support (and public method setValuesList) to handle the Token NESTED_VALUES_LIST, that is a nested SELECT to provide the columns value of an INSERT
	
	Revision 5.2  2013/07/22 16:21:05  marpas
	deprecating ctors without the dbkey
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.13  2013/04/11 09:54:36  marpas
	addValuesList accept an SQLString as value. In this case encloses it in ellipsis.
	
	Revision 4.12  2013/02/27 09:59:15  marpas
	ql string limit vaues are now size_t
	
	Revision 4.11  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.10  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.9  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/02/22 10:35:08  marpas
	refactoring
	
	Revision 4.2  2012/02/21 10:40:44  marpas
	refactoring
	
	Revision 4.1  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.4  2011/10/14 07:49:45  marpas
	FORREADONLY token added
	
	Revision 3.3  2010/09/27 13:04:55  lucio.pulvirenti
	EC:: optional alias on table list supported. Multiple JOINs supported.
	
	Revision 3.2  2010/03/15 15:05:08  marpas
	transaction name can be passed trough beginTransaction method
	
	Revision 3.1  2009/10/29 13:11:33  enrcar
	EC:: addSelectList now handles the SQLDialect::SQLCast optional parameter
	
	Revision 3.0  2009/10/05 14:47:48  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.17  2009/06/16 13:09:21  marpas
	SQL join syntax implemented
	
	Revision 2.16  2008/11/04 15:52:36  marpas
	bitwise operators finalized
	
	Revision 2.15  2008/11/04 13:43:06  marpas
	template functions out of class definition
	
	Revision 2.14  2008/10/08 15:33:30  crivig
	porting to postgres8
	
	Revision 2.13  2008/09/02 14:33:09  enrcar
	EC:: AddSelectList. Optional parameter "label" handled
	
	Revision 2.12  2008/05/22 14:37:54  ivafam
	EC:: Modified after importing into Rational Rose
	
	Revision 2.11  2008/05/19 14:27:44  enrcar
	EC:: Support for custom procedures added
	
	Revision 2.10  2008/05/09 13:37:38  lucio.pulvirenti
	EC:: extra-qualifier removed
	
	Revision 2.9  2008/05/08 16:36:20  enrcar
	EC:: ColumnTraslationMap method handled
	
	Revision 2.8  2008/04/18 13:35:57  clanas
	addSelectedList overloaded in case of DBEXPRSSS_COMPAT for a const char *
	
	Revision 2.7  2008/04/16 13:54:48  enrcar
	EC:: Support for Custom-Functions added. Selects accept a SQLString as column name
	
	Revision 2.6  2007/12/17 10:44:32  paoscu
	Added support to retrieve information on dependant (child) tables.
	
	Revision 2.5  2007/12/11 13:10:04  enrcar
	EC:: getTypeAsString method added
	
	Revision 2.4  2006/12/13 18:13:10  paoscu
	Added methods to understand if a LO field is NULL.
	
	Revision 2.3  2006/09/21 18:02:20  danalt
	Fix for GCC 4.1.0
	
	Revision 2.2  2006/04/05 14:03:39  enrcar
	operator << added
	
	Revision 2.1  2006/04/03 16:36:19  enrcar
	FUNCTION added into QueryType
	addFunctionParameter, addFunctionParameterString, setFunctionName: methods added
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.20  2005/12/19 09:36:11  enrcar
	buildSQLQuery handles nested queries
	
	Revision 1.19  2005/11/30 19:50:00  enrcar
	LOBJs handling: interface changed to avoid memory loss
	
	Revision 1.18  2005/11/30 15:40:59  enrcar
	getCommandAndFileName(), setCommandAndFileName()
	Methods added. Allow upload/download LOBJs using an explicit SQLString
	
	Revision 1.17  2005/11/24 09:47:00  enrcar
	SQLString::QueryType structure: Large object upload/download operations renamed as 
	LOBJ_UPLOAD, LOBJ_DOWNLOAD to make code more readable (was: BLOB_UPLOAD, BLOB_DOWNLOAD)
	
	Revision 1.16  2005/10/25 07:54:02  marpas
	temporary added ctor for const char *
	
	Revision 1.15  2005/09/29 16:36:04  enrcar
	get methods added for blob handling
	
	Revision 1.14  2005/09/28 09:17:51  enrcar
	IF changed to handle blobs
	
	Revision 1.13  2005/09/08 12:05:24  paoscu
	New methods:addANDWhereParameter(std::string const &), addORWhereParameter(std::string const &) and metadataQuery
	
	Revision 1.12  2005/07/22 11:57:20  enrcar
	getOrderBy, getWhereParameter methods added
	
	Revision 1.11  2005/07/21 09:38:51  enrcar
	addWhereParameter method removed
	addORWhereParameter and addANDWhereParameter methods added
	
	Revision 1.10  2005/07/20 15:24:02  enrcar
	DELETE command handled
	
	Revision 1.9  2005/07/20 13:24:54  enrcar
	OrderBy interface changed. Now handles ASC/DESC or DEFAULT for every sort-item
	
	Revision 1.8  2005/07/19 13:16:23  enrcar
	INSERT (and relative tokens) now handled
	
	Revision 1.7  2005/07/19 12:07:23  enrcar
	DROP Method handled
	
	Revision 1.6  2005/07/19 07:55:23  enrcar
	Added debug level
	Added usedtoken vector
	
	Revision 1.5  2005/07/18 08:21:51  enrcar
	UPDATE method handled
	
	Revision 1.4  2005/07/14 13:14:03  enrcar
	Added OrderBy ASC/DESC string.
	Added SQLString with (explicit) string ctor
	
	Revision 1.3  2005/07/11 13:31:31  enrcar
	reset,setType  methods added
	
	Revision 1.2  2005/07/08 16:37:52  enrcar
	*** empty log message ***
	
	Revision 1.1  2005/07/06 14:51:18  enrcar
	SQLString added
	
	

*/

#ifndef _SQLString_H_
#define _SQLString_H_

#include <acs_c++config.hpp>
#include <dbQueryParameters.h>
#include <ThreadSafe.h>
#include <SQLDialect.h>

#include <exException.h>



_ACS_BEGIN_NAMESPACE(acs)

// Builds the SQL Commands for each database. Specific issues are fetched from
// the implementation of dialect classes and used to create valid statements.

class SQLString : public ThreadSafe // PRQA S 2109, 2153
{

public:	

	class SQLCase // PRQA S 2109
	{
	public:
		SQLCase() : _paramsV(), _else() {}
		~SQLCase() throw() {} 
		void addWhen(const std::string &p, const std::string&s) { _paramsV.push_back( std::make_pair (p,s) ) ; }
		void addWhen(const dbQueryParameters  &p, const std::string&s) { addWhen (p.getSqlString(),s) ; }
		void setElse(const std::string &s) { _else = s ; }
		std::vector < std::pair< std::string, std::string > >	get_when() const { return _paramsV ; }
		std::string 											get_else() const { return _else ; }
		std::string toString(const std::string& key) const ; // PRQA S 2502
	private:
        SQLCase(const SQLCase &) ; // not implemented
        SQLCase& operator=(const SQLCase &) ; // not implemented

    private:
		std::vector < std::pair< std::string, std::string > >	_paramsV ;
		std::string												_else ; 
	} ;
	
	
    enum QueryType
	{
        QUERY_UNDEFINED,    /* "void" -> Type MUST BE RE-DEFINED */
        COMMAND,            /* SQL String to be executed directly. NOT PORTABLE! */
        DELETE,             /* SQL DELETE */
        DROP,               /* SQL DROP TABLE */
        INSERT,             /* SQL INSERT INTO */
        SELECT,             /* SQL SELECT */
        UNION,             	/* SQL UNION */
        UPDATE,             /* SQL UPDATE */
        LOBJ_UPLOAD,        /* Large object (Blob, Clob...) Upload operation... Not a REAL Query! */
        LOBJ_DOWNLOAD,      /* Large object (Blob, Clob...) Download operation... Not a REAL Query! */
        LOBJ_ISNULL,      	/* Large object (Blob, Clob...) IsNull operation... Not a REAL Query! */
	    FUNCTION,			/* SQL Function (e.g. COUNT(*)) */
	    CUSTFUNCTION,        /* SQL Custom Function (i.e. UDF) */
	    CUSTPROCEDURE       /* SQL Custom Procedure (i.e. UDP) */
    } ;


    
    enum OrderBy
	{
        SORT_DEFAULT,   /* means: Database default */
        ASC,
        DESC
	} ;

	/*! class Exception declaration */
	
    exDECLARE_EXCEPTION(Exception,dbException) ; // PRQA S 2131, 2153, 2502


	
    explicit SQLString(const std::string& key, std::string const &) ;
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
    /*- brief ctor \deprecated*/
    explicit __attribute__((deprecated)) SQLString(std::string const &) ;
#endif		
	
	
    explicit SQLString(const std::string& key, const char *) ;
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
    /*- brief ctor \deprecated*/
    explicit __attribute__((deprecated)) SQLString(const char *) ;
#endif		
	

	
    explicit SQLString(const std::string& key, std::string &s) ;
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
    /*- brief ctor \deprecated*/
    explicit __attribute__((deprecated)) SQLString(std::string &s) ;
#endif		

#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
    /*- brief ctor \deprecated*/
    explicit __attribute__((deprecated)) SQLString(QueryType t = QUERY_UNDEFINED, bool readOnly = false) ; // readOnly for select
#endif	
    SQLString(const std::string& key, QueryType t, bool readOnly = false) ; // readOnly for select

    
    virtual ~SQLString() throw() ; 

    const std::string &getKey() const throw() { return _key ; }
    
    void forceSQLString( std::string const &) ;
    
    bool sqlForcedStringEmpty() const ;
 
	void setReadOnlySelect(bool) ;
 
	void addSelectList(std::string const &, SQLDialect::SQLCast const &sqlCast, std::string const &lbl = "") ;
    void addSelectList(std::string const &, std::string const &lbl = "") ;

    void addSelectList(SQLString const &, SQLDialect::SQLCast const &sqlCast, std::string const &lbl = "") ;
    void addSelectList(SQLString const &, std::string const &lbl = "") ;
    
    void addSelectList(SQLString::SQLCase const &s, std::string const &lbl = "") { addSelectList(s.toString(_key), lbl ) ; } 
    void addSelectList(SQLString::SQLCase const &s, SQLDialect::SQLCast const &sqlCast, std::string const &lbl = "") { addSelectList(s.toString(_key), sqlCast, lbl ) ; } 

    void addUnionList(std::string const &, const bool union_all=false) ;

    void addUnionList(SQLString const &, const bool union_all=false) ;
    
    void addTableList(std::string const &, std::string const &lbl = "") ;
    
    void addANDWhereParameter(dbQueryParameters const &) ;
    
    void addORWhereParameter(dbQueryParameters const &) ;
    
    void addANDWhereParameter(std::string const &) ;
    void addANDWhereParameter(std::string const &, SQLDialect::SQLCast const &) ;
    void addANDWhereParameter(SQLString::SQLCase const &s) { addANDWhereParameter( s.toString(_key) )  ; }
    void addANDWhereParameter(SQLString::SQLCase const &s, SQLDialect::SQLCast const &sqlCast) { addANDWhereParameter( s.toString(_key), sqlCast)  ; }

    void addORWhereParameter(std::string const &) ;
    void addORWhereParameter(std::string const &, SQLDialect::SQLCast const &) ;
	void addORWhereParameter(SQLString::SQLCase const &s) { addORWhereParameter( s.toString(_key) )  ; }
	void addORWhereParameter(SQLString::SQLCase const &s, SQLDialect::SQLCast const &sqlCast) { addORWhereParameter( s.toString(_key), sqlCast )  ; }

	void addJoinList(SQLDialect::SQLJoinToken, std::string const &table, dbQueryParameters const &) ;

	void addJoinList(SQLDialect::SQLJoinToken, std::string const &table, std::string const &alias, dbQueryParameters const &) ;
	    
    template <typename T> void addFunctionParameter(const T& v) 
	{ std::ostringstream os ; os << v ; addFunctionParameterString(os.str()); }  		
    
    template <typename T> void addProcedureParameter(const T& v) 
	{ std::ostringstream os ; os << v ; addProcedureParameterString(os.str()); } 

    void addGroupByList(std::string const &) ;
    
    void addHavingParameter(dbQueryParameters const &) ;
    
    void addOrderByList(std::string const &, OrderBy const & o = SORT_DEFAULT) ;
    
    void setOrderByString(const std::string& s) ; /* To set a "complete" orderby string */
	
	void copyJoinParameters(SQLString&);
	
	void clearOrderBy();
    
    void addSetList(std::string const &, std::string const &) ;

    void addSetList(std::string const &, SQLString const &) ;
    
    void addValuesList(std::string const &, std::string const &) ;
       
	void addValuesList(std::string const &, SQLString const &) ;

	void setValuesList(std::vector<std::string> const &colv_in, SQLString const &v) ;

    void setColumnAndFileName(std::string const &, std::string const &) ;
    
    void setColumnName(std::string const &) ;
    
    void setCommandAndFileName(SQLString const &, std::string const& = "") ;
    
    void setFunctionName(const std::string&) ;
    
    void setProcedureName(const std::string&) ;
    
    void setDistinct(bool d= true) ;
    
    void setLimit(size_t) ;
    void setOffset(size_t) ;
    
    void setType(QueryType t, bool readOnly = false) ; // readOnly for select
    
    void reset() ;	
    
	std::string getSQL(bool nestedStatement=false) const ;
    
	const std::string &getWhereParameters() const ;
    
    const std::pair <std::string, std::string> &getColumnAndFileName() const ;
    
    std::pair <SQLString, std::string> getCommandAndFileName() const ;
    
    const std::vector< std::pair<std::string,std::string> > &getTableList() const ;
    
	std::string getOrderBy() const ;

    const std::vector<std::string>& getJoinParameters() const;
    
    QueryType getType() const ;
    
    std::string getTypeAsString() const ;

	static std::string intervalToSeconds(const std::string& key, const std::string &intv_in) ;

	static std::string intervalToSeconds(const std::string& key, const std::string &beg_intv_in, const std::string &end_intv_in) ;
	
    static SQLString const beginTransaction(const std::string& key, const std::string& txname) ;
	
   
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
    static SQLString const commitTransaction(const std::string& key="") ;
#else
    static SQLString const commitTransaction(const std::string& key) ;
#endif 

    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	static SQLString const getIsolationCommittedRead(const std::string& key="") ;
#else
	static SQLString const getIsolationCommittedRead(const std::string& key) ;
#endif 

    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	static SQLString const getLockModeWait(int waitTimeSec) ; 
#endif 
	static SQLString const getLockModeWait(const std::string& key, int waitTimeSec) ; 

    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
   static std::string const getOrderByString(const std::string&, const OrderBy& o= SQLString::SORT_DEFAULT) ;
#endif 
   static std::string const getOrderByString(const std::string& key, const std::string&, const OrderBy& o= SQLString::SORT_DEFAULT) ;
 
    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	static SQLString const rollbackTransaction(const std::string& key="") ;
#else
	static SQLString const rollbackTransaction(const std::string& key) ;
#endif
 
    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
    static SQLString const metadataQuery(std::string const & tableName) ;
#endif
    static SQLString const metadataQuery(const std::string& key, std::string const & tableName) ;
 
    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	static SQLString const dependantTablesQuery(std::string const & tableName) ;
#endif
	static SQLString const dependantTablesQuery(const std::string& key, std::string const & tableName) ;
 
    
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	static std::string const getSeparator(const std::string& key="") ;
#else
	static std::string const getSeparator(const std::string& key) ;
#endif

	static SQLString const bitAnd(const std::string& key, const std::string &, const std::string &) ;
	static SQLString const bitOr(const std::string& key, const std::string &, const std::string &) ;
	static SQLString const bitXor(const std::string& key, const std::string &, const std::string &) ;
	static SQLString const bitNot(const std::string& key, const std::string &) ;

	// Indexes query
	static SQLString const tableIndexesQuery (const std::string& key, std::string const& tableName);
    
    SQLString(const SQLString &) ;
    
    SQLString &operator=(const SQLString &) ;


private:
	class SelectListContainer // PRQA S 2109
	{
	public:
        SelectListContainer() ;
		SelectListContainer (const std::string& item, SQLDialect::SQLCast const &sqlCast, const std::string& lbl) ;
		~SelectListContainer() throw() ;
        SelectListContainer(const SelectListContainer &) ;
        SelectListContainer& operator= (const SelectListContainer &) ;

		const std::string &getAlias() const throw() { return _lbl ; } 
		const std::string &getItem() const throw() { return _item ; }
		const SQLDialect::SQLCast &getSQLCast() const throw() { return _sqlCast ; }
	private:
		SQLDialect::SQLCast _sqlCast ;
		std::string _item;
        std::string _lbl ;
	} ;

private:
    
    std::string buildSQLQuery(bool nestedStatement) const ; 
    
    void addToUsedTokenList(SQLDialect::SQLToken const& ) ;
    
    void checkSQLTokenMap(std::vector<SQLDialect::SQLToken> const &) const ; /* Exception is raised if token not supported */
    
    void addFunctionParameterString(const std::string &v) ;
    
    void addProcedureParameterString(const std::string &v) ;
    
	std::vector <std::string> splitPattern(std::string s, const std::string& p) ;
    
    std::string replacePattern(std::string s, const std::string& p, const std::string& r) ;

private:
    /*
        If some variable added below: 
        - MUST BE ADDED also to copy constructor.
        - MUST BE ADDED to method reset()
    */
	
    QueryType _queryType ;
    
	std::string _forcedString ;
	
	size_t _sqlLimit ;
    
	size_t _sqlOffset ;
    
	bool _sqlDistinct ;
    
	std::string _sqlFunctionName ;
    
	std::string _sqlProcedureName ;
    
	std::vector< SelectListContainer > _sqlSelectList ;
    	
	std::vector< std::pair<std::string, bool> > _sqlUnionList ;

	std::vector< std::pair<std::string, std::string> > _sqlTableList ;
    
	std::vector<std::string> _sqlFunctionParamsList ;
    
	std::vector<std::string> _sqlProcedureParamsList ;
 
	std::vector<std::string>  _sqlJoinParameters ;

	
	std::string _sqlWhereParameters ;
    
	std::vector<std::string> _sqlGroupByList ;
    
	std::string _sqlHavingParameters ;
    
    std::vector < std::pair <std::string, OrderBy> > _sqlOrderByList;
    
    std::vector < std::pair <std::string, std::string> > _sqlSetList;
    
    std::vector < std::pair <std::string, std::string> > _sqlValuesList;

	std::pair <std::vector<std::string>, std::string> _sqlNestedValuesList;
    
    std::pair <std::string, std::string> _sqlColumnAndFileName;
    
    std::pair <SQLString, std::string> *_sqlCommandAndFileName;
    
    std::vector <SQLDialect::SQLToken> _usedTokens ;
	bool _isReadOnlySelect ;

	std::string _key;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(SQLString) ;

} ;

template <> inline 
void SQLString::addFunctionParameter(const SQLString & v) // PRQA S 2133, 2134
{ addFunctionParameterString(v.getSQL(true)); } 

template <> inline 
void SQLString::addProcedureParameter(const SQLString& v) // PRQA S 2133, 2134 
{ addProcedureParameterString(v.getSQL(true)); } 


std::ostream &operator<<(std::ostream &, const acs::SQLString&) ;  

std::string toString(SQLString::QueryType) ;

std::ostream & operator << (std::ostream &, SQLString::QueryType) ;
exostream & operator << (exostream &, SQLString::QueryType) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif
