// PRQA S 1050 EOF
/*

	Copyright 1995-2007, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DBXpress for Postgres $

	$Id$

	$Author$

	$Log$
	Revision 1.9  2013/05/14 18:01:27  marpas
	implementing PostGRES/PostGIS support
	
	Revision 1.8  2013/04/30 10:40:13  marpas
	Attempt to check if a symbol is present in the client libpq.so in order to infere if the
	application_name connection info parameter is supported or not.
	
	Revision 1.7  2013/04/22 15:27:01  marpas
	role name passed to connection info
	
	Revision 1.6  2012/12/10 17:02:29  marpas
	qa rules
	
	Revision 1.5  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.4  2011/03/08 15:13:29  marpas
	partial compliant to gcc 4.3 and greater
	
	Revision 1.3  2010/10/22 15:54:38  enrcar
	EC:: Method getServerVersionInt added
	
	Revision 1.2  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.4  2008/07/07 14:19:09  enrcar
	EC:: CR style set to unix
	
	Revision 1.3  2008/07/04 14:37:49  enrcar
	EC:: setOption and getOption now handles 64bit lvalues
	
	Revision 1.2  2008/05/16 10:07:56  enrcar
	virtual dtor added
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include "pgldriver.hpp"

#include "cstr.hpp"
#include "hash.hpp"
// MARCO #include "db2defs.hpp"
#include "libpq-fe.h"
#include <map>
#include <string>


/////////////////////////////////////////////////////////////////////////////
// CPGLTypesMapping

struct CPGLTypesMapping
{
	CPGLTypesMapping () 
		: lvarchar(fldZSTRING, 0), decimal(fldBCD, 0),
		money(fldFLOAT, fldstMONEY), int8(fldFMTBCD, 0) 
	{} ;
	DataType lvarchar;
	DataType decimal;
	DataType money;
	DataType int8;
};


/////////////////////////////////////////////////////////////////////////////
// CPGLConnection - SQLConnection implementation for DB2


struct IPGLConnection : public SQLConnection
{
	virtual ~IPGLConnection () throw() {} 
	// COM_METHOD(bool) interruptSQL () = 0; will be moved soon on SQLCommand 
	COM_METHOD(void) getLastInsertedIdentity (long long &value) = 0;
} ; // com_interface;

class CPGLConnection : public CErrorHandlingImpl< CUnknownImpl<IPGLConnection> >
{
public:
	CPGLConnection (CPGLDriver *driver);
	virtual ~CPGLConnection () throw() ;

//	CPGLActivityPtr activate (bool allowAutoCommit/*, bool saveConnectionArea */); // no save connection area
	void setAutoCommit (bool value);
	INT32 getBlobSize ();
	void setBlobSize (INT32 value);
	CCallback * getCallback ();

	INT32 getCommitRetain ();
	void setCommitRetain (INT32 value);
	void setCurrent ();
	void setDormant ();
	void setIsolationLevel (eXILType value);
	bool isOnLine ();
	void setServer (char *value);
	bool getTrimChar ();
	void setTrimChar(bool value);
	CPGLTypesMapping * getTypesMapping ();
	void setWaitOnLocks(bool value);
	void setTimeout (INT32 value);
	INT32 getTimeout () ;
	INT32 getServerVersionInt () ;

// SQLConnection
	SQL_METHOD connect (pCHAR pszServerName, pCHAR pszUserName, pCHAR pszPassword);
	SQL_METHOD disconnect ();
	SQL_METHOD getSQLCommand (ppSQLCommand ppComm);
	SQL_METHOD getSQLMetaData (ppSQLMetaData ppMeta);
	SQL_METHOD setOption (eSQLConnectOption eCOption, INT32 lValue);
	SQL_METHOD getOption (eSQLConnectOption eCOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength);
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLConnectOption eCOption, INT64 lValue);
	SQL_METHOD getOption (eSQLConnectOption eCOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength);
#endif
	SQL_METHOD beginTransaction (UINT32 ulTransDesc);
	SQL_METHOD commit (UINT32 ulTransDesc);
	SQL_METHOD rollback (UINT32 ulTransDesc);
// IDB2Connection
	// COM_METHOD(bool) interruptSQL (); to be moved on SQLCOmmand
	COM_METHOD(void) getLastInsertedIdentity (long long &value);


	PGconn  *getPGconn() ;
	int getGeographyOid() const ;
    
private:
	void applyAutoCommit () ;
	void applyTxnIsoLevel(/* bool retainUpdateLocks */);
	void applyWaitOnLocks ();
	void detectServerFeatures ();
	void finalizeConnect ();
	void internalConnect (char *database, char *userName, char *password);
	bool isConnected ();
	void setQualifiedName (char *value);
    int getGeoOID() ;
    
    CPGLConnection(const CPGLConnection &) ;
    CPGLConnection & operator= (const CPGLConnection &) ;
private:

	PGconn  *_pgconn ;

	CPGLDriver *_driver;
	CPGLTypesMapping _typesMapping;
	_cstr_t _server;
	bool _hasActivity;
	bool _isAnsiDatabase; 
	bool _isInTransaction;
	INT32 _serverVersionInt ; 		

	                            // dbExpress connection options
	                            // ____________________________
	                            //
	bool _autoCommit;           // eConnAutoCommit
	bool _preTxnAutoCommit ;
	bool _asyncSupport;         // eConnBlockingMode
	INT32 _blobSize; 	    // eConnBlobSize
	bool _waitOnLocks;          // eConnWaitOnLocks
	INT32 _commitRetain;		// eConnCommitRetain + eConnRollbackRetain
	eXILType _txnIsoLevel;      // eConnTxnIsoLevel
	_cstr_t _handle;            // eConnNativeHandle
	CCallback _callback;        // eConnCallBack
	                            // eConnCallBackInfo
//	_cstr_t _clientLocale;      	   // eConnServerCharSet
	static const int defaultServerPort = 5432 ;
	INT32 _serverPort ; 		// eConnServerPort
	_cstr_t _serverVersion;     // eConnServerVersion
	_cstr_t _dbName;     		// eConnDatabaseName
	bool _supportsTransaction;  // eConnSupportsTransaction
	bool _isOnLine;             // eConnOnLine
	bool _trimChar;             // eConnTrimChar
	_cstr_t _qualifiedName;     // eConnQualifiedName
	const char *_catalogName;         // eConnCatalogName
	const char *_schemaName;          // eConnSchemaName
	const char *_objectName;          // eConnObjectName
	_cstr_t _quotedObjectName;  // eConnQuotedObjectName
	char _catalogNameSeparator ;
	INT32 _connTimeout ;		// eConnTimeout
    _cstr_t _roleName;          // eConnRoleName
    int _GeographyOid ;

/*	
	class CAutoDormantActivity : public CPGLActivity
	{
	public:
		CAutoDormantActivity (CPGLConnection *connection);
		virtual ~CAutoDormantActivity ();
		CPGLConnection *getConnection() ;
	private:
		CPGLConnection *_connection;
	};

	class CAutoCommitDecorator : public CPGLActivity
	{
	public:
		CAutoCommitDecorator (CPGLActivityPtr& activity);
		virtual ~CAutoCommitDecorator ();
	private:
		CPGLActivityPtr _activity;
	};
*/
    static std::map<std::string, bool> _roleSupported ;
};


class CPGLConnectionStringParser
{
public:
	static CPGLConnectionStringParser& getInstance ();
	void parse (char *connectionString, CPGLConnection *connectionContext);

private:
	static CPGLConnectionStringParser _instance;

	CPGLConnectionStringParser ();

	static void getNumericMapping (char *value, DataType& dataType);

	static void setAutoCommit (CPGLConnection *connectionContext, char *value);
	static void setBlobSize (CPGLConnection *connectionContext, char *value);
	static void setCommitRetain (CPGLConnection *connectionContext, char *value);
	static void setDecimal (CPGLConnection *connectionContext, char *value);
	static void setInt8 (CPGLConnection *connectionContext, char *value);
	static void setIsolationLevel (CPGLConnection *connectionContext, char *value);
	static void setMoney (CPGLConnection *connectionContext, char *value);
	static void setLVarChar (CPGLConnection *connectionContext, char *value);
	static void setServer (CPGLConnection *connectionContext, char *value);
	static void setTrimChar (CPGLConnection *connectionContext, char *value);
	static void setWaitOnLocks (CPGLConnection *connectionContext, char *value);

	typedef void (*OptionFunc) (CPGLConnection *connectionContext, char *value);
	typedef _hash_t<const char *, OptionFunc, compare_cstr> OptionFuncTable;
	OptionFuncTable _optionFuncTable;
};
