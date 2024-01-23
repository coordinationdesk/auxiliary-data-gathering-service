// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libPostgres8Dialect $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2016/09/20 16:12:11  enrcar
	CSGACQ-96: EC:: dialect-specific method getTableNameForEmptyFROM added in order to handle SELECT without FROM
	
	Revision 5.5  2016/07/28 15:54:16  enrcar
	Method translateSQLCast made virtual (dialect-specific)
	
	Revision 5.4  2016/07/26 15:20:03  enrcar
	CSGACQ-87  EC:: Added support for SQL CASE WHEN/ELSE
	
	Revision 5.3  2016/07/21 14:30:20  enrcar
	CSGACQ-82 EC:: Interface intervalToSeconds added
	
	Revision 5.2  2016/03/26 21:46:51  marpas
	enforcing use of connection reference getting last serial
	
	Revision 5.1  2014/12/02 12:01:25  lucio.pulvirenti
	S2PDGS-908: getStringDelimiter() method removed: delimit() method implemented instead to delimit strings properly and escape delimiter inside strings.
	
	Revision 5.0  2013/06/18 09:49:24  marpas
	adoption of libException 5.x standards
	
	Revision 1.29  2013/05/15 11:51:04  marpas
	propTypeClobFilter and propTypeBlobFilter spcific methods added
	
	Revision 1.28  2013/05/14 18:42:53  marpas
	PostGRES/PostGIS support
	
	Revision 1.27  2013/02/27 12:38:47  marpas
	coding best practices applied
	qa rules
	interface rationalization
	
	Revision 1.26  2012/11/21 10:10:23  marpas
	HEAD realignement
	
	Revision 1.21  2012/02/17 14:09:56  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 1.20  2012/02/13 15:27:48  marpas
	refactoring
	
	Revision 1.19  2011/05/25 08:36:50  lucio.pulvirenti
	getIsGoodDummyQuery() method added
	
	Revision 1.18  2010/08/19 09:45:40  marpas
	new method hasTimedWaitOnLock. To be used from dbQuery to lock the main methods
	
	Revision 1.17  2010/04/29 10:45:15  marpas
	some proprietary types can be treated as blob/clob - new method added
	
	Revision 1.16  2010/04/19 10:36:51  marpas
	executeSpecificCommandVirtual implemented
	
	Revision 1.15  2010/03/15 13:56:33  marpas
	beginTransaction method has transaction name as defaulted parameter
	
	Revision 1.14  2010/02/02 17:57:40  enrcar
	EC:: Added method: getStatementTerminator() { return ";" }
	
	Revision 1.13  2009/06/22 12:43:49  marpas
	SQLDialect interface improved
	
	Revision 1.12  2009/06/19 18:00:02  marpas
	getLasSerial method slightly changed
	
	Revision 1.11  2008/11/20 10:46:44  marpas
	getAllowConcurrency added
	
	Revision 1.10  2008/11/17 15:27:02  marpas
	currentDateTime added
	
	Revision 1.9  2008/11/04 12:03:23  marpas
	bitwise operation introduced
	
	Revision 1.8  2008/05/22 16:32:26  lucio.pulvirenti
	EC:: Re-commited since DOS EOLs were used
	
	Revision 1.7  2008/05/22 14:07:19  ivafam
	EC:: Modified after importing into Rational Rose
	
	Revision 1.6  2008/05/07 16:05:15  enrcar
	EC:: PGresultGuard added
	
	Revision 1.5  2008/05/07 14:19:40  enrcar
	EC:: Blob & Clob handling: work in progress
	
	Revision 1.4  2008/04/14 12:28:14  enrcar
	EC:: work in progress
	
	Revision 1.3  2007/12/11 13:48:17  enrcar
	EC:: Methods returnExecForSet, returnExecForStatus implemented
	
	Revision 1.2  2007/12/10 09:47:55  enrcar
	EC:: Local Dialect is VIRTUALLY derived from base dialect to allow inheritance of virtual methods
	
	Revision 1.1.1.2  2007/12/04 10:44:23  crivig
	Imported source
	
*/

#ifndef _Postgres8SQLDialect_
#define _Postgres8SQLDialect_

#include <acs_c++config.hpp>
#include <SQLDialect.h>
#include <exMacros.h>
#include <libpq-fe.h>


_ACS_BEGIN_NAMESPACE(acs)

class SQLString ;
/**
 * \brief Provides a way to get strings formatted for Postgres SQL Database
Provides the implementation of all the Postgres-specific methods.
Offers all the abstract methods listed into the base classes of the dialect.
 */


class Postgres8SQLDialect : public SQLDialect // PRQA S 2109
{
public:


	class TransactionHandler { // PRQA S 2109
    	public:

            explicit TransactionHandler(PGconn*) ;

            ~TransactionHandler() throw() ;

        	void commit() ;

        	void rollback() ;

        private:
            TransactionHandler() ;
            TransactionHandler(const TransactionHandler &) ;     
            TransactionHandler & operator= (const TransactionHandler &) ;     
        private:

            PGconn* _conn ;
    } ;




	class PGresultGuard { // PRQA S 2109
        public:

            explicit PGresultGuard(PGresult* r) throw() : _res(r) {} 

            ~PGresultGuard() throw() 
            { // PRQA S 4631 L1
                if (0 != _res) { 
                    PQclear(_res); _res = 0; 
                } 
              // PRQA L:L1
            } ;
        	
        private:
            PGresultGuard() ;
            PGresultGuard(const PGresultGuard &) ;     
            PGresultGuard & operator= (const PGresultGuard &) ;     
        private:

	        PGresult* _res ;
    } ;



	explicit Postgres8SQLDialect(const std::string &key) ;

	virtual ~Postgres8SQLDialect() throw() ; 
	

	virtual void setFastConnect() const ;

	virtual void setEnv(rsResourceSet &) const ;
	// returns the default behaviour for DB concurrency it is always true for Postgres
	// can be changed with db.conf parameter (AllowConcurrency)
	virtual bool getAllowConcurrency() const ;


	virtual std::string getLockModeWait(int waitTimeSec) const ;

	virtual std::string getIsolationCommittedRead() const ;

    virtual std::string currentDateTime() const ;
	
	virtual std::string getIsGoodDummyQuery() const ;

	virtual std::string caseWhenElse(const std::vector< std::pair<std::string, std::string > > &, const std::string &) const ;
	
	virtual std::string beginTransaction(const std::string& txname="") const ;

	virtual std::string commitTransaction() const ;

    virtual std::string rollbackTransaction() const ;

	virtual std::string metadataQuery(std::string const & tableName) const ;

	// Shall return a set of two columns: indexName and columnName 
	// Describing all the indexes of a given table
	virtual std::string getTableIndexesQuery (std::string const & tableName ) const;

    virtual std::string childTablesQuery(std::string const & tableName) const ;

    virtual std::string getSeparator() const ;
    virtual std::string getStatementTerminator() const ;

    virtual std::string orderByASC() const ;

    virtual std::string orderByDESC() const ;

    virtual std::string boolTrue() const ;

    virtual std::string boolFalse() const ;

	virtual bool checkBooleanValue(const std::string &) const ;	

	virtual std::string bitAnd(const std::string &, const std::string &) ;
	virtual std::string bitOr(const std::string &, const std::string &) ;
	virtual std::string bitXor(const std::string &, const std::string &) ;
	virtual std::string bitNot(const std::string &) ;
    
	virtual bool hasTimedWaitOnLock() const ; // returns false

    virtual std::string delimit(const std::string &) const ;

	virtual std::string getSQLSelectListDelimiter_L() const ;

	virtual std::string getSQLSelectListDelimiter_R() const ;

    virtual std::string getSQLOrdGrpByListDelimiter_L() const ;

	virtual std::string getSQLOrdGrpByListDelimiter_R() const ;
 
    virtual std::string getTableNameForEmptyFROM() const ;

    virtual std::string translateSQLCast(const std::string&, const SQLDialect::SQLCast &, bool raiseException=true) const ;

	virtual long SQLFilterReturn(long) const ;
	virtual std::string convertCaseFilter(std::string const &) const ;
	virtual std::string convertColumnToStringFilter(std::string const &, dbSpecificColumnType::Type const &) const ;
	virtual dbSpecificColumnType::Type guessTypeFilter (dbSpecificColumnType::Type const & type, std::string const & dbType) const;

    virtual std::string intervalToSeconds(const std::string &intv) const; 
    virtual std::string intervalToSeconds(const std::string &intv_beg, const std::string &intv_end) const; 

	virtual bool propTypeBlobClobFilter (std::string const & proprietarytype) const ;

	virtual bool propTypeClobFilter (std::string const & proprietarytype) const ;

	virtual bool propTypeJsonbFilter (std::string const & proprietarytype) const ;

	virtual bool propTypeBlobFilter (std::string const & proprietarytype) const ;
    

	virtual void installConnectionCallback(dbConnection *) ;

	virtual void uninstallConnectionCallback() ;
		
    virtual int executeSpecificCommandVirtual(void**, unsigned char, signed short int) const ;

    // SQL Functions

    virtual std::string function_ISNULL(std::string const &) const ;

    virtual std::string function_ISNOTNULL(std::string const &) const ;

    // SQL Methods

    virtual std::string getLastSerial(std::string const&, dbConnection &) const ;


	virtual unsigned short getTimeStampResolution() const ;

	// File operations 

    virtual bool blobIsNull( dbConnection *, const std::string &tableName, const std::string& columnName, const std::string& whereCondition, bool& retValue ) ;

    virtual bool clobIsNull( dbConnection *, const std::string &tableName, const std::string& columnName, const std::string& whereCondition, bool& retValue ) ;

    virtual void downloadBlob(dbConnection *, const SQLString& sqlString, const std::string& IGNORED_completePath="" ) ;

    virtual void downloadBlob(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string&, const std::string& completePath ) ;

    virtual void downloadClob(dbConnection *, const SQLString& sqlString, const std::string& completePath ) ;

    virtual void downloadClob(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string&, const std::string& completePath ) ;

    virtual size_t uploadBlob(dbConnection *, const SQLString& sqlString, const std::string& IGNORED_completePath="" ) ;

    virtual size_t uploadBlob(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string&, const std::string& completePath ) ;

    virtual size_t uploadClob(dbConnection *, const SQLString& sqlString, const std::string& IGNORED_completePath="" ) ;

    virtual size_t uploadClob(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string&, const std::string& completePath ) ;

    virtual size_t zeroBlob(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string& whereCondition ) ;

    virtual size_t zeroClob(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string& whereCondition ) ;
      
    virtual ptrExecSQL returnExecForStatus(SQLString const&, ptrExecSQL pExecForStatus, ptrExecSQL pExecForSet) const ;

    virtual ptrExecSQL returnExecForSet(SQLString const&, ptrExecSQL pExecForSet, ptrExecSQL pExecForStatus) const ;

	// Listener operations:

	virtual void  startListener (dbConnection *conn, const std::string& channel) const  ;
	virtual bool listenToMessages (dbConnection *conn, std::vector<std::string>& messages_V, const uint64_t& timeout_usec) const ;

    virtual std::string gisVersionQuery() const ;

    virtual void adjustGisOperatorMap() const ;

private:

	static std::string dump(const std::vector<std::string>&) ; 

   // Internal File operations 

    Oid getOid(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string&) ;

    int setNullOid(dbConnection *, const std::string &tableName, const std::string& columnName, const std::string&) ;

    int setOid(dbConnection *, Oid, const std::string &tableName, const std::string& columnName, const std::string&) ;

    int setOidInternal(dbConnection *, const std::string& , const std::string &tableName, const std::string& columnName, const std::string&) ;

    size_t uploadBlobClob(dbConnection *, const SQLString& sqlString) ;


	PGconn* getConnectionPointer(dbConnection *) const ;

	// declared but not implemented

 	Postgres8SQLDialect() ;

    Postgres8SQLDialect(const Postgres8SQLDialect&) ;

	Postgres8SQLDialect& operator=(const Postgres8SQLDialect&) ;

private:

	static const char *_minimumConTime_ ;

	static char _conTimeout_[32] ; // PRQA S 4403

	static char _dateStyle_[64] ; // PRQA S 4403

	static uint64_t _notify_counter ; 

	mutable unsigned short _timeStampResolution ;
	mutable bool _gisOperatorAdjusted ;
	std::string _key ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Postgres8SQLDialect) ;
	
} ;

_ACS_END_NAMESPACE

#endif
