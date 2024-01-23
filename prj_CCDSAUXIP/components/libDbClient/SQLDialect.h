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
	Revision 5.13  2016/09/20 16:12:11  enrcar
	CSGACQ-96: EC:: dialect-specific method getTableNameForEmptyFROM added in order to handle SELECT without FROM
	
	Revision 5.12  2016/07/28 15:54:16  enrcar
	Method translateSQLCast made virtual (dialect-specific)
	
	Revision 5.11  2016/07/26 15:09:33  enrcar
	CSGACQ-87 EC:: Added support and test for CASE WHEN ELSE and TIME INTERVAL
	
	Revision 5.10  2016/07/21 14:27:17  enrcar
	CSGACQ-82  EC:: intervalToSeconds interface added to DB
	
	Revision 5.9  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.8  2016/03/15 09:11:47  marpas
	moving into dialect (getLimitedQueryWrapped) the responsibility to provide its own wrapped sql string
	
	Revision 5.7  2016/03/14 15:39:14  marpas
	introducing offset in queries
	
	Revision 5.6  2015/09/30 09:00:03  enrcar
	EC:: extended to handle UNION queries
	
	Revision 5.5  2015/03/19 14:29:16  marpas
	getTimeStampMaxDecimalDigits virtual method added. Its purpose is to know the maximum dialect limit for timestamp/datetime decimal digits
	(e.g Informix has 5, Postgres and Oracle 6).
	
	Revision 5.4  2015/02/17 14:16:24  enrcar
	EC:: Added support (and public method setValuesList) to handle the Token NESTED_VALUES_LIST, that is a nested SELECT to provide the columns value of an INSERT
	
	Revision 5.3  2014/12/02 12:09:02  lucio.pulvirenti
	2PDGS-908: getStringDelimiter() method removed: pure virtual delimit() method declared to be implemented by specific dialect to delimit strings properly
	and possibily escape delimiter inside strings.
	
	Revision 5.2  2013/11/05 16:41:57  enrcar
	EC:: Operator NOT_IN added
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:55  marpas
	adopting libException 5.x standards
	
	Revision 4.14  2013/05/15 11:18:26  marpas
	propTypeBlob and propTypeClob methods added along their virtual counterparts
	
	Revision 4.13  2013/05/14 18:05:30  marpas
	implementing PostGRES/PostGIS support: in case of PostGIS geography type, the column is translated into ST_AsText(column) in order to get properly parsed by dbCrow/dbColumn classes
	
	Revision 4.12  2013/03/11 14:53:51  marpas
	FDGuard is now a template specialization
	
	Revision 4.11  2013/02/27 09:59:15  marpas
	ql string limit vaues are now size_t
	
	Revision 4.10  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.9  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.8  2012/11/19 13:30:19  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/04/02 11:12:26  marpas
	attempt to repeat queries - execForStatus - if some conditions are met into the db
	
	Revision 4.1  2012/02/17 13:55:12  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.14  2011/10/14 07:49:45  marpas
	FORREADONLY token added
	
	Revision 3.13  2011/05/25 08:35:06  lucio.pulvirenti
	getIsGoodDummyQuery() pure virtual method added
	
	Revision 3.12  2011/03/31 15:47:31  lucio.pulvirenti
	method rowLimitInsideWhere turned into limitWrapsQuery
	
	Revision 3.11  2011/03/02 13:34:25  marpas
	serial8 support added
	
	Revision 3.10  2011/03/01 16:12:32  marpas
	GCC 4.4.x support
	
	Revision 3.9  2010/09/11 18:12:25  marpas
	libDbExpress 3.2 interface adopted
	
	Revision 3.8  2010/08/19 09:42:43  marpas
	new method to know if a database has a timed wait for lock.
	Warning: the method is pure virtual, requires correct dialect version
	
	Revision 3.7  2010/04/29 10:44:31  marpas
	deal with special columntype (proprietary type e.g. Postgres TEXT)
	some specific type can be treated as blob/clob
	
	Revision 3.6  2010/03/31 16:38:32  lucio.pulvirenti
	EC:: method setColumnNameTraslationMap added
	
	Revision 3.5  2010/03/19 11:27:35  marpas
	class SQLDialectAppSpecific added to manage specific dialect issues for applications
	
	Revision 3.4  2010/03/17 14:02:35  enrcar
	ENRCAR:: executeSpecificCommand / executeSpecificCommandVirtual methods added
	MARPAS:: Finalize methods added
	
	Revision 3.3  2010/02/10 17:19:13  enrcar
	EC:: LIMIT inside the WHERE clause (e.g. Oracle dialect): now handled
	
	Revision 3.2  2010/02/02 18:27:03  enrcar
	EC:: getStatementTerminator method added
	
	Revision 3.1  2009/10/29 13:07:50  enrcar
	EC:: enum SQLCastType, class SQLCast added. Method translateSQLCast() added.
	
	Revision 3.0  2009/10/05 14:47:47  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.23  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.22  2009/06/19 17:47:57  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.21  2009/06/17 14:40:27  marpas
	SQLJoinToken enumeration with number
	
	Revision 2.20  2009/06/16 13:09:21  marpas
	SQL join syntax implemented
	
	Revision 2.19  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.18  2008/12/09 16:11:23  marpas
	dbConv::Null implemented to allow setting Null values using SQLString
	
	Revision 2.17  2008/11/20 10:46:09  marpas
	getAllowConcurrency added
	
	Revision 2.16  2008/11/17 15:23:52  marpas
	currentDateTime added
	
	Revision 2.15  2008/11/04 13:43:37  marpas
	preliminary introduction of bitwise operators
	
	Revision 2.14  2008/05/22 14:37:54  ivafam
	EC:: Modified after importing into Rational Rose
	
	Revision 2.13  2008/05/19 14:27:44  enrcar
	EC:: Support for custom procedures added
	
	Revision 2.12  2008/05/08 16:36:20  enrcar
	EC:: ColumnTraslationMap method handled
	
	Revision 2.11  2008/05/07 17:07:21  enrcar
	EC:: createEscapeSequence method added
	
	Revision 2.10  2008/04/16 13:51:18  enrcar
	EC:: template for custom functions added
	
	Revision 2.9  2008/04/09 13:19:55  enrcar
	EC:: SQLSelectList, SQLOrdGrpByList delimiters handled
	
	Revision 2.8  2007/12/17 10:44:32  paoscu
	Added support to retrieve information on dependant (child) tables.
	
	Revision 2.7  2007/12/11 13:12:51  enrcar
	EC:: returnExecForSet, returExecForStatus methods made pure-virtual since must be implemented into specific dialect
	
	Revision 2.6  2007/12/07 18:40:56  enrcar
	ENRCAR: Methods returnExecForSet, returnExecForStatus added
	
	Revision 2.5  2007/12/03 13:23:10  crivig
	modified in order to handle short int as bool for MySQL
	
	Revision 2.4  2006/12/13 18:13:10  paoscu
	Added methods to understand if a LO field is NULL.
	
	Revision 2.3  2006/06/28 16:56:32  marpas
	getTimeStampResolution abstract method added
	
	Revision 2.2  2006/04/04 08:46:10  enrcar
	Nested function handled into dialect
	
	Revision 2.1  2006/04/03 16:37:06  enrcar
	Function template handled
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.32  2006/02/27 11:22:12  giucas
	setEnv & setFastConnect are now abstract const
	
	Revision 1.31  2006/01/09 16:22:10  enrcar
	getLastSerial virtual method added
	
	Revision 1.30  2006/01/05 17:11:03  enrcar
	virtual functions function_ISNULL function_ISNOTNULL added
	
	Revision 1.29  2005/12/22 18:47:00  marpas
	added FileDescriptorGuard class
	new concept about delimiters for strings implemented
	
	Revision 1.28  2005/12/20 16:51:19  enrcar
	SQLDialect interface for LOBJ improved for better compatibility between DBs
	
	Revision 1.27  2005/12/01 17:09:46  paoscu
	SQLString passed as const reference in blob functions.
	
	Revision 1.26  2005/11/30 15:44:17  enrcar
	downloadBlobClob, uploadBlobClob pure virtual methods added
	
	Revision 1.25  2005/11/25 16:27:53  paoscu
	Methods:
		static std::string convertCase(std::string const &)
		virtual std::string convertCaseFilter(std::string const &) const
	added.
	
	Revision 1.24  2005/10/27 15:33:12  enrcar
	blob/clob function now return bool value: true if specific dialect must perform operation, otherwise dbQuery will be used.
	
	Revision 1.23  2005/10/26 12:54:11  enrcar
	Clob handling added (required by Informix)
	
	Revision 1.22  2005/10/21 09:07:59  enrcar
	dbConnection* passed to blob functions
	
	Revision 1.21  2005/10/19 08:35:03  enrcar
	installConnectionCallback uninstallConnectionCallback virtual methods added
	
	Revision 1.20  2005/10/10 18:02:19  paoscu
	ISNOTNULL added.
	
	Revision 1.19  2005/10/07 13:41:07  enrcar
	zeroBlob function added into dialect
	
	Revision 1.18  2005/09/29 16:35:14  enrcar
	Interface changed for Blob ops
	
	Revision 1.17  2005/09/28 09:18:12  enrcar
	Dialect IF changed to handle blobs
	
	Revision 1.16  2005/09/27 14:53:40  enrcar
	Exception added
	IF changed to handle blobs (temporary - still under dvl)
	
	Revision 1.15  2005/08/10 07:58:43  enrcar
	Interface changed: SQLFilterReturn() made pure-virtual.
	MUST BE IMPLEMENTED into specific-db dialect
	
	Revision 1.14  2005/07/20 15:24:31  enrcar
	DELETE template added
	
	Revision 1.13  2005/07/20 13:25:17  enrcar
	added separator
	
	Revision 1.12  2005/07/19 13:15:22  enrcar
	INSERT added
	Boolean operators handled
	
	Revision 1.11  2005/07/19 12:07:23  enrcar
	DROP Method handled
	
	Revision 1.10  2005/07/19 07:47:14  enrcar
	Added debug level
	Method translate renamed
	
	Revision 1.9  2005/07/18 08:20:53  enrcar
	UPDATE template added
	
	Revision 1.8  2005/07/14 13:15:44  enrcar
	Added OrdeyBy DESC/ASC handling
	
	Revision 1.7  2005/07/06 14:47:30  enrcar
	Added token & command handling
	
	Revision 1.6  2005/05/05 14:59:09  enrcar
	Added filter for SQL driver returned values
	
	Revision 1.5  2005/04/22 20:29:31  marpas
	work in progress
	
	Revision 1.4  2005/04/21 17:11:15  marpas
	work in progress
	
	Revision 1.3  2005/04/19 08:16:10  marpas
	work in progress
	
	Revision 1.2  2005/04/18 19:42:27  marpas
	work in progress
	
	Revision 1.1  2005/04/18 16:37:33  marpas
	just added
	
	
	

*/

#ifndef _SQLDialect_
#define _SQLDialect_

#include <acs_c++config.hpp>
#include <FileGuard.h>
#include <DbExpress.h>
#include <dbParam.h>
#include <dbException.h>

#include <string>
#include <map>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class dbConnection ;
class SQLString ;
class dbQuery ;
class dbSet ;

/**
 * \brief Provides a way to get different strings format for various SQL
dialects
Every dialect must inheritate from this class and implement all the abstract,
specific methods
 */


class SQLDialect // PRQA S 2109
{
public:	

    
	typedef void(dbQuery::*ptrExecSQL)(dbSet&, bool&, const std::vector<dbSpecificColumnType::Type>&, bool);


    
    enum SQLToken
    {
        // Tokens should be explicitly numerated in order to ensure faster debugs...
        /* TEMPLATE_INVALID: When the SQLString is used as container, and NOT TO BUILD a real SQL sentence, 
		    the template must be INVALID in order to raise an exception if someone wants to execute a "standard query" */
        TEMPLATE_INVALID=0,    
        SELECT=1,
        UPDATE=2,
        SET=3,
        DROP=4,
        INSERT=5,
        DELETE=6,
        VALUES=7,
        VALUES_LIST=8,
        NESTED_VALUES_LIST=9,
        COLUMN_LIST=10,
        LIMIT=11,
        DISTINCT=12,
        SELECT_LIST=13,
        SET_LIST=14,
        FROM=15,
        TABLE_LIST=16,
		JOIN=17,
        WHERE=18,
        GROUPBY=19,
        HAVING=20,
        ORDERBY=21,
        FILENAME=22,
        FUNCTIONARG=23,
        CUSTFUNCTIONARG=24,
        CUSTPROCEDUREARG=25,
        FUNCTIONNAME=26,
        CUSTFUNCTIONNAME=27,
        NESTEDFUNCTIONNAME=28,
        CUSTNESTEDFUNCTIONNAME=29,
        CUSTPROCEDURENAME=30,
        CUSTNESTEDPROCEDURENAME=31,
        COLUMNNAME=32,
        FORREADONLY=33,
		UNION=34,		
		UNION_ALL=35,
		UNION_LIST=36,
        OFFSET=37,		
        ON_CONFLICT_DO=38,
        ON_CONFLICT_CNSR_DO=39
    };


	enum SQLJoinToken {
		CROSS = 0,
		INNER = 1, 
		LEFT  = 2,
		RIGHT = 3,
		FULL  = 4
	} ;
	
	//WARNING!! THERE IS CURRENTLY EXACTLY THE SAME ENUM IN dbParam (NAMED "Operation")!!!
	//THAT ONE SHOULD BE REMOVED. BY THAT TIME THIS TWO ENUMS MUST BE IDENTICAL

	
    enum SQLOperator {
		NONE=dbParam::NONE, 
		EQUAL_TO=dbParam::EQUAL_TO, 		// ==
		LIKE=dbParam::LIKE,					// LIKE (only for characters)
		MINOR_OF=dbParam::MINOR_OF, 		// <	
		MAJOR_OF=dbParam::MAJOR_OF, 		// >	
		MAJOR_OR_EQUAL_OF=dbParam::MAJOR_OR_EQUAL_OF, 	// >=    
		MINOR_OR_EQUAL_OF=dbParam::MINOR_OR_EQUAL_OF, 	// <=    
		DIFFERENT_FROM=dbParam::DIFFERENT_FROM, 		// !=
		BETWEEN=dbParam::BETWEEN,				// BETWEEN
		NOT_BETWEEN=dbParam::NOT_BETWEEN,		// !BETWEEN
				    // Geodetic data blade module functions
		INSIDE=dbParam::INSIDE, // The INSIDE functions compare two spatio-temporal objects
								// and return true if all of the following are true:
								//	-	All point, line segments, and the interior area
								//		(if any) of the first object are spatially
								//		etirely inside the boundary and the interior area of 
								//		the second object
								//	-	The altitude range of the first object intersect the 
								//		range of the second object
								//	-	The time ranges of the two objects intersect.
	    INTERSECT=dbParam::INTERSECT, 	// 	The INTERSECT function returns TRUE if both of the following
								        //	are true:
								        //	-	Any point or partial line segment of the object in the
								        //		first argument lies partially inside of the object in
								        //		the second argument
								        //	-	The time ranges for the object intersect
		OUTSIDE=dbParam::OUTSIDE, 		// The OUTSIDE functions takes two spatio-temporal objects
								        // and return true if both of the following are true:     
								        //	-	All points and line segment and the entire interior
								        //		are (if any) of the first object lie spatially outside
								        //		the second object                                 
								        //	-	The time ranges of the two objects intersect      
		ISNULL=dbParam::ISNULL,			//  -   IS NULL
        ISNOTNULL=dbParam::ISNOTNULL,	//  -   IS NOT NULL
		IN=dbParam::IN,					// The IN function is to produce IN (,,,) where clause
		NOT_IN=dbParam::NOT_IN					// The NOT_IN function is to produce NOT IN (,,,) where clause
	} ; 
	
	
	/* Enum SQLCastType, add all possible casts here: */
	//
	enum SQLCastType
	{
		SQL_TYPEUNDEF=0,/* Do not use */
		
		/* Standard SQL Types: */
		SQL_BOOLEAN=1,	/* Implemented in different ways, usually a short Integer */
		SQL_SMALLINT=2,	/* Integer, 2 bytes */
		SQL_INTEGER=3,	/* Integer, 4 bytes */
		SQL_BIGINT=4,	/* Integer, 8 bytes */
		SQL_SERIAL=5,	/* Integer, auto-incremental, 4 bytes */
		SQL_BIGSERIAL=6,/* Integer, auto-incremental, 8 bytes */
		SQL_FLOAT=7,	/* FP, 4 bytes */
		SQL_DOUBLE=8,	/* FP, 8 bytes */
		SQL_DECIMAL=9,	/* FP, with selectable precision */
		SQL_CHAR=16,	/* Fixed-length character string */
		SQL_VARCHAR=17	/* Variable-length character string */

		/* Non standard and database-specific types */
		// (...) 
	} ;
	
	
	/* Descriptor/Container for a type cast */
	class SQLCast
	{
	public:
		explicit SQLCast (SQLCastType=SQL_TYPEUNDEF, const std::string &arg1="", const std::string &arg2="") ;
		~SQLCast() = default ;
		SQLCast (const SQLCast &) = default ;
		SQLCast& operator= (const SQLCast &) = default ;
		SQLCast (SQLCast &&) = default ;
		SQLCast& operator= (SQLCast &&) = default ;

		SQLCastType getSQLCastType() const noexcept { return _castType ; }
		const std::string &getArg1() const noexcept { return _arg1 ; }
		const std::string &getArg2() const noexcept { return _arg2 ; }

	private:
		SQLCastType _castType ;
		std::string _arg1 ;
        std::string _arg2 ;
	} ;
	

    /*! class CastTypeNotFoundException declaration */
	//
    exDECLARE_EXCEPTION(CastTypeNotFoundException,dbException) ; // PRQA S 2131, 2153, 2502

    /*! class TokenNotFoundException declaration */
	
    exDECLARE_EXCEPTION(TokenNotFoundException,dbException) ; // PRQA S 2131, 2153, 2502

    /*! class OperatorNotFoundException declaration */
	
    exDECLARE_EXCEPTION(OperatorNotFoundException,dbException) ; // PRQA S 2131, 2153, 2502

    /*! class InvalidOperation declaration */
	
    exDECLARE_EXCEPTION(InvalidOperation,dbException) ; // PRQA S 2131, 2153, 2502

    /*! class InternalErrorException declaration */
	
    exDECLARE_EXCEPTION(InternalErrorException,dbException) ; // PRQA S 2131, 2153, 2502

protected:    
    class GuardFileDesc // PRQA S 2109
    {
    // Close descriptor && erase file
    public:

        GuardFileDesc() = delete ;
        GuardFileDesc(const GuardFileDesc &) = delete ;
        GuardFileDesc & operator= (const GuardFileDesc &) = delete ;
        GuardFileDesc(GuardFileDesc &&) = delete ;
        GuardFileDesc & operator= (GuardFileDesc &&) = delete ;
        explicit GuardFileDesc(int fd, const std::string &fname="") : fg_(fname), fdg_(fd) {}
        ~GuardFileDesc() = default ; 
        void resetFileName() { fg_.detach() ; } // PRQA S 4211
    private:
        FileGuard fg_ ;
        pattern::FDGuard fdg_ ;
    } ;


public:	
    
    explicit SQLDialect(const std::string &) ;
    SQLDialect() = delete ;
    SQLDialect(const SQLDialect &) = delete ;
    SQLDialect &operator=(const SQLDialect &) = delete ;
    SQLDialect(SQLDialect &&) = delete ;
    SQLDialect &operator=(SQLDialect &&) = delete ;


    virtual ~SQLDialect() noexcept ; 


    const std::string &dialectName() const noexcept ;

    virtual void setEnv(rsResourceSet &) const = 0 ;

    // returns the default behaviour for DB concurrency
    virtual bool getAllowConcurrency() const = 0 ;

    // wait = 0: wait for lock forever 
    // wait < 0 : no wait 
    // wait > 0 wait for specified secs

    virtual std::string getLockModeWait(int waitTimeSec) const = 0 ;
    
    virtual std::string getIsolationCommittedRead() const = 0 ;

    virtual std::string currentDateTime() const = 0 ;

    virtual std::string getIsGoodDummyQuery() const = 0 ;

	virtual std::string caseWhenElse(const std::vector< std::pair<std::string, std::string > > &, const std::string &) const = 0 ;

	
    virtual std::string beginTransaction(const std::string& txname="") const = 0 ;
    virtual void beginFinalize(dbSQLConnection &) const ;


    virtual std::string commitTransaction() const = 0 ;
    virtual void commitFinalize(dbSQLConnection &) const ;

    
    virtual std::string rollbackTransaction() const = 0 ;
    virtual void rollbackFinalize(dbSQLConnection &) const ;

    virtual std::string metadataQuery(std::string const & tableName) const = 0 ;

    virtual std::string childTablesQuery(std::string const & tableName) const = 0 ;
    
	// Shall return a set of two columns: indexName, columnName 
	// Describing all the indexes of a given table
	virtual std::string getTableIndexesQuery (std::string const & tableName ) const = 0;

    virtual std::string getSeparator() const = 0 ;
    virtual std::string getStatementTerminator() const = 0 ;
    
    virtual std::string orderByASC() const = 0 ;

    virtual std::string orderByDESC() const = 0 ;

    virtual std::string boolTrue() const = 0 ;

    virtual std::string boolFalse() const = 0 ;

    virtual bool checkBooleanValue(const std::string &) const = 0 ;

    virtual std::string bitAnd(const std::string &, const std::string &) = 0 ;
    virtual std::string bitOr(const std::string &, const std::string &) = 0 ;
    virtual std::string bitXor(const std::string &, const std::string &) = 0 ;
    virtual std::string bitNot(const std::string &) = 0 ;

    virtual std::string nullValue() const ;

    virtual bool hasTimedWaitOnLock() const = 0 ;
    
    virtual std::string delimit(const std::string &) const = 0 ;
        
    virtual std::string getSQLSelectListDelimiter_L() const = 0 ;

    virtual std::string getSQLSelectListDelimiter_R() const = 0 ;
	
    virtual std::string getSQLOrdGrpByListDelimiter_L() const = 0 ;
    
    virtual std::string getSQLOrdGrpByListDelimiter_R() const = 0 ;
  
    virtual std::string getTableNameForEmptyFROM() const = 0 ;

    // default up to usecs
    virtual unsigned short getTimeStampMaxDecimalDigits() const { return 6 ; } // PRQA S 2131, 4400

    virtual std::vector<SQLToken> const & getBlobIsNullTemplate() const ;

    virtual std::vector<SQLToken> const & getDeleteTemplate() const ;

    virtual std::vector<SQLToken> const & getDropTemplate() const ;

    virtual std::vector<SQLToken> const & getInsertTemplate() const ;

    virtual std::vector<SQLToken> const & getSelectTemplate() const ;

    virtual std::vector<SQLToken> const & getUnionTemplate() const ;

    virtual std::vector<SQLToken> const & getUpdateTemplate() const ;

    virtual std::vector<SQLToken> const & getBlobUploadTemplate() const ;

    virtual std::vector<SQLToken> const & getBlobDownloadTemplate() const ;

    virtual std::vector<SQLToken> const & getCustFunctionTemplate() const ;

    virtual std::vector<SQLToken> const & getFunctionTemplate() const ;

    virtual std::vector<SQLToken> const & getCustProcedureTemplate() const ;
    
    
    virtual std::map<std::string, std::string> const & getColumnNameTraslationMap() const ;
    virtual std::map<std::string, std::string> & getColumnNameTraslationMap() ;
    virtual void setColumnNameTraslationMap( std::map<std::string, std::string> & ) ;
  
    std::string translateToken(SQLToken, bool raiseException=true) const ;


    std::string translateJoinToken(SQLJoinToken, bool raiseException=true) const ;
    
    std::string translateOperator(SQLOperator, bool raiseException=true) const ;

    std::string translateColumnName(const std::string &) const ;
    
    virtual bool limitWrapsQuery() const ;
    
    virtual std::string getLimitedQueryWrapped(const std::string &, size_t limit, size_t offset=0) const ;

    virtual std::string translateSQLCast(const std::string& item, const SQLCast &, bool b=true) const = 0 ;

    virtual std::string intervalToSeconds(const std::string &intv) const = 0; 

    virtual std::string intervalToSeconds(const std::string &intv_beg, const std::string &intv_end) const = 0; 

    virtual ptrExecSQL returnExecForSet(SQLString const&, ptrExecSQL pExecForSet, ptrExecSQL pExecForStatus) const = 0;
    
    virtual ptrExecSQL returnExecForStatus(SQLString const&, ptrExecSQL pExecForStatus, ptrExecSQL pExecForSet) const = 0;
    
    virtual long SQLFilterReturn(long) const =0;

    /*
     * This method returns true if the command failed for conditions dependent from other, not
     * nominal activity on the db, hence its repetition should have success.
     */
    virtual bool shouldRepeatCommand(const dbSQLConnection &, const dbSQLCommand &comm) const ;

    
    static long SQLReturn(const std::string &key, long) ;

    /*
     * This method returns true if the command failed for conditions dependent from other, not
     * nominal activity on the db, hence its repetition should have success.
     */
    static bool shouldRepeatCommand(const std::string &key, const dbSQLConnection &, const dbSQLCommand &comm) ;
    
    static std::string createEscapeSequence(const std::string&) ;
    
    static std::string createEscapeSequence(const char*, size_t) ;

	/*
     * Used to make a specific dialect to run a command (e.g. return a parameter, translate a buffer and so on).
     * EACH SPECIFIC DIALECT SHALL IMPLEMENT ITS OWN COMMAND AND INTERFACES.
     * THE CALLER MUST IDENTIFY THE SPECIFIC DIALECT BEFORE RUNNING THE COMMAND
     */

    static int executeSpecificCommand(const std::string &key, void**, unsigned char, signed short int) ;

    virtual int executeSpecificCommandVirtual(void**, unsigned char, signed short int) const ;

    /**
     * Convert the case of the string.
     * Every dialect will implement its own convertion policy
     * It would be usefull that the policy could be decided at runtime.
     * E.g. for a case-sensitive database (like MySql) it could be needed 
     * to  convert all the queries to the the correct case (if possible) 
     */
    static std::string convertCase(const std::string &key, std::string const &) ;

    
    virtual std::string convertCaseFilter(std::string const &) const =0;

    static std::string convertColumnToString(const std::string &key, const std::string &, dbSpecificColumnType::Type const &) ;

    virtual std::string convertColumnToStringFilter(std::string const &, dbSpecificColumnType::Type const &) const =0;

    static dbSpecificColumnType::Type guessType (const std::string&, dbSpecificColumnType::Type const & type, std::string const & dbType);



    virtual dbSpecificColumnType::Type guessTypeFilter (dbSpecificColumnType::Type const & type, std::string const & dbType) const = 0 ;

    // return true if the proprietary type can be treated as BLOB or CLOB
    static bool propTypeBlobClob (const std::string &key, std::string const & proprietarytype) ;
    virtual bool propTypeBlobClobFilter (std::string const & proprietarytype) const ;


    // return true if the proprietary type can be treated as BLOB 
    static bool propTypeBlob (const std::string &key, std::string const & proprietarytype) ;
    virtual bool propTypeBlobFilter (std::string const & proprietarytype) const ;


    // return true if the proprietary type can be treated as CLOB
    static bool propTypeClob (const std::string &key, std::string const & proprietarytype) ;
    virtual bool propTypeClobFilter (std::string const & proprietarytype) const ;
 
    // return true if the proprietary type can be treated as JSONB
    static bool propTypeJsonb (const std::string &key, std::string const & proprietarytype) ;
    virtual bool propTypeJsonbFilter (std::string const & proprietarytype) const ;

    /**
	 * SQL Functions
	 */
	
    virtual std::string function_ISNULL(std::string const &) const =0;
    
    virtual std::string function_ISNOTNULL(std::string const &) const =0;

    /**
     * SQL Methods
     */
	
    virtual std::string getLastSerial(std::string const&, dbConnection &) const =0;
    
    /** default implementation call getLastSerial
    */
    virtual std::string getLastSerial4(std::string const&, dbConnection &) const ;
    virtual std::string getLastSerial8(std::string const&, dbConnection &) const ;

    
    
    virtual void installConnectionCallback(dbConnection *) = 0 ;

    virtual void uninstallConnectionCallback() = 0 ;


    virtual unsigned short getTimeStampResolution() const = 0 ;

    
    virtual bool blobIsNull(dbConnection*, 
			    const std::string &tableName, 
			    const std::string& columnName, 
			    const std::string& whereCondition, 
			    bool& ) =0;
    
    virtual bool clobIsNull(dbConnection*, 
			    const std::string &tableName, 
			    const std::string& columnName, 
			    const std::string& whereCondition, 
			    bool& ) =0;
    
    virtual void downloadBlob(dbConnection*, 
			      const SQLString &sqlString, 
			      const std::string& completePath="" ) =0;
    
    virtual void downloadBlob(dbConnection*, 
			      const std::string &tableName, 
			      const std::string& columnName, 
			      const std::string&, 
			      const std::string& completePath ) =0;
    
    virtual void downloadClob(dbConnection*, 
			      const SQLString &sqlString, 
			      const std::string& completePath="" ) =0;
    
    virtual void downloadClob(dbConnection*, 
			      const std::string &tableName, 
			      const std::string& columnName, 
			      const std::string&, 
			      const std::string& completePath ) =0;
    
    /*! returns number of affected records */
    virtual size_t uploadBlob(dbConnection*, 
			    const SQLString &sqlString, 
			    const std::string& completePath="" ) =0;
    
    /*! returns number of affected records */
    virtual size_t uploadBlob(dbConnection*, 
			    const std::string &tableName, 
			    const std::string& columnName, 
			    const std::string&, 
			    const std::string& completePath ) =0;

    /*! returns number of affected records */
    virtual size_t uploadClob(dbConnection*, 
			    const SQLString &sqlString, 
			    const std::string& completePath="" ) =0;
    
    /*! returns number of affected records */
    virtual size_t uploadClob(dbConnection*, 
			    const std::string &tableName, 
			    const std::string& columnName, 
			    const std::string&, 
			    const std::string& completePath ) =0;
    
    /*! returns number of affected records */
    virtual size_t zeroBlob(dbConnection*, 
			  const std::string &tableName, 
			  const std::string& columnName, 
			  const std::string& whereCondition) =0;
    
    /*! returns number of affected records */
    virtual size_t zeroClob(dbConnection*, 
			  const std::string &tableName, 
			  const std::string& columnName, 
			  const std::string& whereCondition) =0;
			  

    /**
	 * Listener operations:
     */

	virtual void startListener (dbConnection *conn, const std::string& channel) const  ;

	virtual bool listenToMessages (dbConnection *conn, std::vector<std::string>& messages_V, const uint64_t& timeout_usec) const ;


    /**
	 * GIS operations:
     */
    virtual std::string gisVersionQuery() const ;
    virtual void adjustGisOperatorMap() const ;

protected:
    // PRQA S 2101 L1
    std::map<SQLToken, std::string const> _sqlTokenMap ;
    std::map<SQLJoinToken, std::string const> _sqlJoinTokenMap ;
    mutable std::map<SQLOperator, std::string const> _sqlOperatorMap ;
    std::map<SQLCastType, std::pair<std::string const, std::string const> > _sqlCastTypeMap ;
    std::map<std::string, std::string> _columnTranslationMap ;
    std::vector<SQLToken> _blobIsNullTemplate ;
    std::vector<SQLToken> _deleteTemplate ;
    std::vector<SQLToken> _downloadBlobTemplate ;
    std::vector<SQLToken> _dropTemplate ;
    std::vector<SQLToken> _functionTemplate ;
    std::vector<SQLToken> _custFunctionTemplate ;
    std::vector<SQLToken> _custProcedureTemplate ;
    std::vector<SQLToken> _insertTemplate ;
    std::vector<SQLToken> _selectTemplate ;
    std::vector<SQLToken> _unionTemplate ;
    std::vector<SQLToken> _updateTemplate ;
    std::vector<SQLToken> _uploadBlobTemplate ;
    // PRQA L:L1

private:
    
    std::string _dialectName ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(SQLDialect) ;
} ;

std::string toString(SQLDialect::SQLCastType) ;

std::ostream & operator << (std::ostream &, SQLDialect::SQLCastType) ;
exostream & operator << (exostream &, SQLDialect::SQLCastType) ; // PRQA S 2072

std::string toString(SQLDialect::SQLToken) ;

std::ostream & operator << (std::ostream &, SQLDialect::SQLToken) ;
exostream & operator << (exostream &, SQLDialect::SQLToken) ; // PRQA S 2072

std::string toString(SQLDialect::SQLJoinToken) ;

std::ostream & operator << (std::ostream &, SQLDialect::SQLJoinToken) ;
exostream & operator << (exostream &, SQLDialect::SQLJoinToken) ; // PRQA S 2072

std::string toString(SQLDialect::SQLOperator) ;

std::ostream & operator << (std::ostream &, SQLDialect::SQLOperator) ;
exostream & operator << (exostream &, SQLDialect::SQLOperator) ; // PRQA S 2072



_ACS_END_NAMESPACE

#endif
