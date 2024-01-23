// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
	Revision 1.19  2013/05/30 11:49:40  marpas
	debug message issued even if no debug: removed
	
	Revision 1.18  2013/05/14 18:01:27  marpas
	implementing PostGRES/PostGIS support
	
	Revision 1.17  2013/04/30 10:40:13  marpas
	Attempt to check if a symbol is present in the client libpq.so in order to infere if the
	application_name connection info parameter is supported or not.
	
	Revision 1.16  2013/04/22 15:27:01  marpas
	role name passed to connection info
	
	Revision 1.15  2012/12/10 17:02:29  marpas
	qa rules
	
	Revision 1.14  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.13  2011/03/08 15:13:29  marpas
	partial compliant to gcc 4.3 and greater
	
	Revision 1.12  2010/10/22 15:54:38  enrcar
	EC:: Method getServerVersionInt added
	
	Revision 1.11  2008/12/16 14:59:27  lucio.pulvirenti
	XTREMELOGCERR define commented
	
	Revision 1.10  2008/12/11 11:23:18  lucio.pulvirenti
	cstr definition moved to component libDbExpress
	XTREMELOG improved
	
	Revision 1.9  2008/12/10 16:33:17  lucio.pulvirenti
	some fixing for debug purposes
	
	Revision 1.8  2008/12/05 16:54:13  lucio.pulvirenti
	xtreme logging fixed again
	
	Revision 1.7  2008/12/05 16:52:51  lucio.pulvirenti
	extreme logging fixed
	
	Revision 1.6  2008/12/05 16:45:02  lucio.pulvirenti
	cleanup and DEBUG (tmp)
	
	Revision 1.5  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.4  2008/11/04 12:00:55  marpas
	eConnServerPort fixed
	
	Revision 1.3  2008/09/17 11:54:26  marpas
	64 bit support improved
	
	Revision 1.2  2008/07/21 17:03:51  paoscu
	work in progress
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.10  2008/07/08 10:18:41  enrcar
	EC:: unable to compile. Fixed.
	
	Revision 1.9  2008/07/07 14:20:30  enrcar
	EC:: improved 64bit compatibility for char* parameters
	
	Revision 1.8  2008/07/04 14:37:49  enrcar
	EC:: setOption and getOption now handles 64bit lvalues
	
	Revision 1.7  2008/07/02 08:56:25  enrcar
	EC:: option eConnCustomInfo removed since redundant. eConnNativeHandle should be used instead.
	
	Revision 1.6  2008/06/13 09:49:51  lucio.pulvirenti
	EC:: serverVersion was used uninitialized -- FIXED
	
	Revision 1.5  2008/04/18 10:04:39  enrcar
	EC:: debug message added
	
	Revision 1.4  2008/04/09 15:11:58  enrcar
	EC:: parameter eSQLConnectOption:eConnCustomInfo used to retrieve a pointer to the internal connection
	
	Revision 1.3  2008/03/20 17:41:35  enrcar
	EC:: first test -- small bugs fixed
	
	Revision 1.2  2007/12/17 18:38:30  marpas
	work in progress
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/

#include "pglconnection.hpp"

#include "license.hpp"

#include "pglcursor.hpp"
#include "pglerror.hpp"
#include "pglcommand.hpp"

//#define XTREMELOG 1
#ifdef XTREMELOG
#include <stdio.h>
#endif

//#define XTREMELOGCERR 1

#include <iostream>
#include <iomanip>

#include <dlfcn.h>

using namespace std ;
/////////////////////////////////////////////////////////////////////////////
// CPGLConnection

map<string, bool> CPGLConnection::_roleSupported ;

CPGLConnection::CPGLConnection (CPGLDriver *driver): 
	_pgconn(0), 
    _driver(driver), // here to avoid the dtor call Release when _driver is not empty 
	_typesMapping(),
    _server(""), 
    _hasActivity(false),
    _isAnsiDatabase(false),
    _isInTransaction(false),
	_serverVersionInt(-1),			/* Undefined */
    _autoCommit(false),
    _preTxnAutoCommit(false),
    _asyncSupport(false),
    _blobSize(driver->getBlobSize()),
    _waitOnLocks(false),
    _commitRetain(1),
    _txnIsoLevel(xilREADCOMMITTED),
    _handle(),
    _callback(),
	_serverPort(defaultServerPort),
	_serverVersion("01234567890"),	/* Integer as string, len worst case*/
	_dbName(""),
    _supportsTransaction(true),
    _isOnLine(false),
	_trimChar(false),
    _qualifiedName(),
    _catalogName(0),
    _schemaName(0),
    _objectName(0),
    _quotedObjectName(),
    _catalogNameSeparator(),
    _connTimeout(),
    _roleName(""),
    _GeographyOid()
{
    #ifdef ACDBEXPPGL_DEBUG
	    printf("CPGLConnection::CPGLConnection\n");
    #endif
//	_driver = driver ; // here to avoid the dtor call Release when _driver is not empty 
#ifdef XTREMELOG
	FILE *elog = fopen("/tmp/pglconn.log","a") ;
	if (elog) fprintf(elog,"%p, prima di AddRef su driver\n", this) ;
#endif

#ifdef XTREMELOGCERR
	int driverRefCount=
#endif
    _driver->AddRef() ;
#ifdef XTREMELOGCERR
	cerr << getpid() << " " << pthread_self() << " _driver->AddRef() :" << driverRefCount << endl ;
#endif

//	_hasActivity = false;
//	_autoCommit = false;
//	_asyncSupport = false;
#ifdef XTREMELOG
	if (elog) fprintf(elog,"%p prima di getBlobSize su driver\n", this) ;
#endif
//	_blobSize = driver->getBlobSize();
//	_waitOnLocks = false;
//	_commitRetain = 1;
//	_txnIsoLevel = xilREADCOMMITTED;
//	_trimChar = false;
#ifdef XTREMELOG
	if (elog) fprintf(elog,"%p fine CPGLconnection ctor\n",this) ;
	if (elog) fclose(elog) ;
#endif
}

CPGLConnection::~CPGLConnection () throw()
{
    #ifdef ACDBEXPPGL_DEBUG
	    printf("CPGLConnection::~CPGLConnection\n");
    #endif
	try {
		disconnect();
	}
	catch(exception &)
	{
		// no exception from here
	}
	if (_driver != 0) {
		// avoiding multiple destructions
		CPGLDriver *tmpdr = _driver ;
		_driver = 0 ;
#ifdef XTREMELOGCERR
		int driverRefCount=
#endif
        tmpdr->Release();
#ifdef XTREMELOGCERR
		cerr << getpid() << " " << pthread_self() << " _driver->Release() :" << driverRefCount << endl ;
#endif

	}
	else { // should never happen .. 
		throw CPGLDriverError("multiple dtor on CPGLConnection called");
	}
}



void CPGLConnection::setAutoCommit (bool value)
{
	_autoCommit = value;
	if (isConnected()) {
		if (_isInTransaction)
			throw CPGLDriverError("Cannot change autocommit during transaction");
		applyAutoCommit();
	}
}


INT32 CPGLConnection::getBlobSize ()
{
	return _blobSize;
}

INT32 CPGLConnection::getServerVersionInt ()
{
	return _serverVersionInt;
}

void CPGLConnection::setBlobSize (INT32 value)
{
	_blobSize = value;
}

CCallback * CPGLConnection::getCallback ()
{
	if (_callback.getHandler() == 0)
		return 0;
	return &_callback;
}


INT32 CPGLConnection::getCommitRetain ()
{
	return _commitRetain;
}

void CPGLConnection::setCommitRetain (INT32 value)
{
	_commitRetain = value ? 0x1000 : 0;
}


void CPGLConnection::setCurrent ()
{
	if (!isConnected())
		throw CPGLDriverError("Only established connection can be set current");
}


void CPGLConnection::setDormant ()
{
	if (!isConnected())
		throw CPGLDriverError("Only established connection can be set dormant");
}

void CPGLConnection::setIsolationLevel (eXILType value)
{
	_txnIsoLevel = value;
	if (isConnected() /* && isOnLine() */) {
//		CPGLActivityPtr activity = activate(false/*, true*/);
		applyTxnIsoLevel(/*false*/);
	}
}

void CPGLConnection::setTimeout (INT32 value)
{
	_connTimeout = value;
}

INT32 CPGLConnection::getTimeout ()
{
	return _connTimeout ;
}

bool CPGLConnection::isOnLine ()
{
	return _isOnLine;
}

void CPGLConnection::setServer (char *value)
{
	_server = value;
}

bool CPGLConnection::getTrimChar ()
{
	return _trimChar;
}

void CPGLConnection::setTrimChar(bool value)
{
	_trimChar = value;
}

CPGLTypesMapping * CPGLConnection::getTypesMapping ()
{
	return &_typesMapping;
}


void CPGLConnection::setWaitOnLocks(bool value)
{
}

// Implementation of SQLConnection interface

SQL_METHOD_IMP CPGLConnection::connect (
	pCHAR pszServerName, pCHAR pszUserName, pCHAR pszPassword) SAFE_SQL_METHOD
{
	CLicenseManager::checkLicense();

	TRACE_MESSAGE(getCallback())(traceCONNECT, "connect to %s as %s", pszServerName, pszUserName);

	if (isConnected())
		throw CPGLDriverError("Already connected");

	internalConnect(pszServerName, pszUserName, pszPassword);
	finalizeConnect();


	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLConnection::disconnect () SAFE_SQL_METHOD
{
	if (!isConnected())
		return SQL_SUCCESS;

	TRACE_MESSAGE(getCallback())(traceCONNECT, "disconnect");

	setCurrent();

	PQfinish(_pgconn);
	_pgconn = 0 ;
	
	RETURN_SQL_RESULT;
}


SQL_METHOD_IMP CPGLConnection::getSQLCommand (ppSQLCommand ppComm) SAFE_SQL_METHOD
{
	*ppComm = 0 ;
    *ppComm = new CPGLCommand(this);

	RETURN_SQL_RESULT;
}


#ifdef PGL_SHOW_WARNING
#warning MP CPGLConnection::getSQLMetaData body disabled
#endif

SQL_METHOD_IMP CPGLConnection::getSQLMetaData (ppSQLMetaData ppMeta) SAFE_SQL_METHOD
{
	// *ppMeta = new CInformixMetaData(this);
	RETURN_SQL_RESULT;
}


SQL_METHOD_IMP CPGLConnection::setOption (eSQLConnectOption eCOption, INT32 lValue) SAFE_SQL_METHOD
{
    #ifdef ACDBEXPPGL_DEBUG
	    printf("CPGLConnection::setOption32\n");
    #endif
        
	switch (eCOption)
	{
	case eConnAutoCommit:
		setAutoCommit(lValue != 0); break;

	case eConnBlobSize:
		_blobSize = lValue; break;

	case eConnWaitOnLocks:
		setWaitOnLocks(lValue != 0); break;

	case eConnCommitRetain:
		_commitRetain = lValue ? 0x1000 : 0; break;

#ifdef PGL_SHOW_WARNING
#warning eConnRollbackRetain collapsed into eConnCommitRetain
#endif
	case eConnRollbackRetain:
		_commitRetain = lValue ? 0x1000 : 0; break;

	case eConnTxnIsoLevel:
		setIsolationLevel(static_cast<eXILType>(lValue)); break;

#ifndef __x86_64__ 
	case eConnCallBack:
        _callback.setHandler(reinterpret_cast<pfCallBack>(lValue)); break;

	case eConnHostName:
        _server = reinterpret_cast<char *>(lValue) ; break;

	case eConnDatabaseName:
        _dbName = reinterpret_cast<char *>(lValue) ; break;

	case eConnRoleName:
        _roleName = reinterpret_cast<char *>(lValue) ; break;

	case eConnCallBackInfo:
        _callback.setInfo(lValue); break;

	case eConnQualifiedName:
        setQualifiedName(reinterpret_cast<char *>(lValue)); break;
#endif

	case eConnServerPort:
	      _serverPort = atoi(reinterpret_cast<char*>(lValue)) ; break ;

/*
	case eConnServerCharSet:
		_clientLocale = reinterpret_cast<char *>(lValue); break;
*/
	case eConnOnLine:
		_isOnLine = lValue != 0; break;

	case eConnTrimChar:
		_trimChar = lValue != 0; break;

	case eConnTimeOut:
		setTimeout(lValue); break;

	default:
		return DBXERR_NOTSUPPORTED;
	}

	RETURN_SQL_RESULT;
}


#ifdef __x86_64__
SQL_METHOD_IMP CPGLConnection::setOption (eSQLConnectOption eCOption, INT64 lValue) SAFE_SQL_METHOD
{
    #ifdef ACDBEXPPGL_DEBUG
	    printf("CPGLConnection::setOption64\n");
    #endif
    
	switch (eCOption)
	{

		case eConnCallBack:
			_callback.setHandler(reinterpret_cast<pfCallBack>(lValue)); break;

		case eConnHostName:
			_server = reinterpret_cast<char *>(lValue) ; break;

		case eConnDatabaseName:
			_dbName = reinterpret_cast<char *>(lValue) ; break;

		case eConnRoleName:
			_roleName = reinterpret_cast<char *>(lValue) ; break;

		case eConnCallBackInfo:
			_callback.setInfo(lValue); break;

		case eConnQualifiedName:
			setQualifiedName(reinterpret_cast<char *>(lValue)); break;

		case eConnServerPort:
	    	  _serverPort = atoi(reinterpret_cast<char*>(lValue)) ; break ;

		default:
			return DBXERR_NOTSUPPORTED;
	}

	RETURN_SQL_RESULT;
}
#endif


SQL_METHOD_IMP CPGLConnection::getOption (eSQLConnectOption eCOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength)
{
    #ifdef ACDBEXPPGL_DEBUG
	    printf("CPGLConnection::getOption32\n");
    #endif
    
	*piLength = 0 ;
	try {
		switch (eCOption)
		{
		case eConnAutoCommit:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _autoCommit; break;

		case eConnBlockingMode:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _asyncSupport; break;

		case eConnBlobSize:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _blobSize; break;

		case eConnWaitOnLocks:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _waitOnLocks; break;

		case eConnCommitRetain:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _commitRetain; break;

		case eConnRollbackRetain:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _commitRetain; break;

		case eConnTxnIsoLevel:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _txnIsoLevel; break;
	
			
#ifndef __x86_64__   
        case eConnServerVersion:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_serverVersion), plValue, iMaxLength, piLength)), _serverVersion); break;

		case eConnHostName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_server), plValue, iMaxLength, piLength)), _server); break;
   
        case eConnNativeHandle:
			*getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = reinterpret_cast<ACS_PTRASINT_T>(_pgconn); break;

		case eConnCallBack:
			*getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = reinterpret_cast<ACS_PTRASINT_T>(_callback.getHandler()); break;
		
        case eConnCallBackInfo:
			*getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = _callback.getInfo(); break;

		case eConnDatabaseName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_dbName), plValue, iMaxLength, piLength)), _dbName); break;

		case eConnRoleName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_roleName), plValue, iMaxLength, piLength)), _roleName); break;
#endif

		case eConnMaxActiveComm:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = -1; break;

		case eConnSupportsTransaction:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _supportsTransaction; break;

		case eConnMultipleTransaction:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = 0; break;

		case eConnOnLine:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = isOnLine(); break;

		case eConnTrimChar:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _trimChar; break;

		case eConnCatalogName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_catalogName), plValue, iMaxLength, piLength)), _catalogName); break;

		case eConnSchemaName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_schemaName), plValue, iMaxLength, piLength)), _schemaName); break;

		case eConnObjectName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_objectName), plValue, iMaxLength, piLength)), _objectName); break;

		case eConnQuotedObjectName:
			strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_quotedObjectName), plValue, iMaxLength, piLength)), _quotedObjectName); break;
		
		case eConnTimeOut:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = getTimeout(); break;
		
		case eConnServerPort:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _serverPort; break;

		default:
			return DBXERR_NOTSUPPORTED;
       }	/* switch */
	} catch (SQLResult sqlResult) {
		return sqlResult;
	}

	return SQL_SUCCESS;
}


#ifdef __x86_64__
SQL_METHOD_IMP CPGLConnection::getOption (eSQLConnectOption eCOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength)
{
    #ifdef ACDBEXPPGL_DEBUG
	    printf("CPGLConnection::getOption64\n");
    #endif

	*piLength = 0 ;
	try {
		switch (eCOption)
		{
		
            case eConnServerVersion:
				strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_serverVersion), plValue, iMaxLength, piLength)), _serverVersion); break;

			case eConnHostName:
				strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_server), plValue, iMaxLength, piLength)), _server); break;

            case eConnNativeHandle:
            	*getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = reinterpret_cast<ACS_PTRASINT_T>(_pgconn); break;

            case eConnCallBack:
            	*getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = reinterpret_cast<ACS_PTRASINT_T>(_callback.getHandler()); break;

            case eConnCallBackInfo:
            	*getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = _callback.getInfo(); break;

            case eConnDatabaseName:
            	strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_dbName), plValue, iMaxLength, piLength)), _dbName); break;

            case eConnRoleName:
            	strcpy(reinterpret_cast<char *>(getVerifiedValueDestination(strlen(_roleName), plValue, iMaxLength, piLength)), _roleName); break;
			default:
				return DBXERR_NOTSUPPORTED;
    
        }	/* switch */
	} catch (SQLResult sqlResult) {
		return sqlResult;
	}

	return SQL_SUCCESS;
}
#endif


SQL_METHOD_IMP CPGLConnection::beginTransaction (UINT32 ulTransDesc) SAFE_SQL_METHOD
{
	if (_isInTransaction)
		throw CPGLDriverError("Already in transaction");
	if (!isConnected())
		throw CPGLDriverError("Not Connected: cannot begin transactions");
	
	TRACE_MESSAGE(getCallback())(traceTRANSACT, "BEGIN TRANSACTION");

	applyTxnIsoLevel();

	if (_autoCommit) {

		#ifdef ACDBEXPPGL_DEBUG
			printf("CPGLConnection::beginTransaction: autocommit is %d\n",(int)_autoCommit) ;
		#endif
		_preTxnAutoCommit = _autoCommit ;
		#ifdef ACDBEXPPGL_DEBUG
			printf("CPGLConnection::beginTransaction: calling set autocommit false\n") ;
		#endif
		setAutoCommit(false) ;
		_isInTransaction = true;
	}

	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLConnection::commit (UINT32 ulTransDesc) SAFE_SQL_METHOD
{
	TRACE_MESSAGE(getCallback())(traceTRANSACT, "COMMIT");

	ACSNOTIMPLEMENTED("CPGLConnection::commit: not implemented") ;

	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLConnection::rollback (UINT32 ulTransDesc) SAFE_SQL_METHOD
{
	TRACE_MESSAGE(getCallback())(traceTRANSACT, "ROLLBACK");

	ACSNOTIMPLEMENTED("CPGLConnection::rollback: not implemented") ;

	RETURN_SQL_RESULT;
}


COM_METHOD_IMP(void) CPGLConnection::getLastInsertedIdentity (long long &identity) 
{
	try {

		ACSNOTIMPLEMENTED("CPGLConnection::getLastInsertedIdentity: not implemented") ;
		return ;
	}
	catch (exception& e) {
		TRACE_MESSAGE(getCallback())(traceERROR, e.what()) ;
	} 
	catch(...) {
		
		TRACE_MESSAGE(getCallback())(traceERROR, "Unknown exception getting last inserted identity") ;
		
	}
}


PGconn * CPGLConnection::getPGconn() 
{
	return _pgconn ;
}

// Private methods

void CPGLConnection::finalizeConnect ()
{
	try
	{
//		applyWaitOnLocks(); NO longer needed
//		applyAutoCommit(); NO longer needed
		detectServerFeatures();
	}
	catch (...)
	{
		disconnect();
		throw;
	}
	setDormant();
	_isInTransaction = false;
}

void CPGLConnection::applyTxnIsoLevel (/* bool retainUpdateLocks */)
{
	const char *isolationLevelText;
//	char *retainUpdateLocksText;

	switch (_txnIsoLevel)
	{
	case xilREADCOMMITTED:
//		isolationLevelText = "COMMITTED READ";break;
		isolationLevelText = "READ COMMITTED";break;

	case xilREPEATABLEREAD:
		isolationLevelText = "REPEATABLE READ"; break;

	case xilDIRTYREAD:
		isolationLevelText = "DIRTY READ"; break;

	case xilCUSTOM:
 		ACSNOTIMPLEMENTED("CPGLConnection::applyTxnIsoLevel: xilCUSTOM transaction level not implemented") ;
		isolationLevelText = "CURSOR STABILITY"; break;

	default:
//		isolationLevelText = "COMMITTED READ";
		isolationLevelText = "READ COMMITTED";
	}

	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLConnection::applyTxnIsoLevel: %d -> \"%s\"\n", (int)_txnIsoLevel,isolationLevelText) ;
	#endif

	// note this can be accomplished using SQLSetConnectAttr (SQL_ATTR_TXN_ISOLATION) 
	char sql[64]; 
//	sprintf(sql, "SET ISOLATION TO %s%s", isolationLevelText, retainUpdateLocksText); MP
	sprintf(sql, "SET ISOLATION %s", isolationLevelText);

	ACSNOTIMPLEMENTED("CPGLConnection::applyTxnIsoLevel: not implemented") ;

}



void CPGLConnection::applyAutoCommit (/* bool retainUpdateLocks */)
{
// Marco: Postgres is always in autocommit mode ... 
}


void CPGLConnection::applyWaitOnLocks ()
{
	ACSNOTIMPLEMENTED("CPGLConnection::applyWaitOnLocks: not implemented") ;
}

int CPGLConnection::getGeographyOid() const { return _GeographyOid ; } 


void CPGLConnection::detectServerFeatures ()
{
	_supportsTransaction = true;
	_isOnLine = false;
	_isAnsiDatabase = true;  // MP --- to be confirmed if anybody cares

	_dbName = PQdb(_pgconn) ;
	checkPGLResult(_pgconn, static_cast<char *>(_dbName)) ;
	
	int serverVersion = PQserverVersion(_pgconn);
	checkPGLResult(_pgconn,static_cast<char *>(_dbName)) ;
    _serverVersion = "123456789012345" ; // to initialize internal array
	sprintf(static_cast<char *>(_serverVersion),"%06d",serverVersion) ;
	_serverVersionInt = serverVersion ;

	_server = PQhost(_pgconn);
	checkPGLResult(_pgconn,static_cast<char *>(_dbName)) ;

	_cstr_t userName = _cstr_t(PQuser(_pgconn)) ;
	checkPGLResult(_pgconn,static_cast<char *>(_dbName)) ;
    _GeographyOid = getGeoOID() ;
    CPGLColumnFactory::setGeographicOID(_GeographyOid) ;
	#ifdef ACDBEXPPGL_DEBUG
		cout << "CPGLConnection::detectServerFeatures: PQuser \"" << (char *)userName << "\"\n" ; 
		cout << "\tServer: " <<  (char*)_server << "\n" ;
		cout << "\tServer Port: " <<  _serverPort << "\n" ;
		cout << "\tDb Name: " << static_cast<char *>(_dbName) << "\n" ;
		cout << "\tUser Name: " << (char*)userName << "\n" ;
		cout << "\tRole Name: " << (char*)_roleName << "\n" ;
		cout << "\tServer Version: " << (char*)_serverVersion << "\n" ;
		cout << "\tServer Version (as Int): " << _serverVersionInt << "\n" ;
		cout << "\tsupportsTransaction: " << boolalpha << _supportsTransaction << "\n" ;
		cout << "\tisOnLine: " << boolalpha << _isOnLine << "\n" ;
		cout << "\tisAnsiDatabase: " << boolalpha << _isAnsiDatabase << "\n" ;
		cout << "\tgetGeographyOID: " << _GeographyOid << "\n" ;
	#endif
    
	char *sport = PQport(_pgconn) ;
	checkPGLResult(_pgconn,static_cast<char *>(_dbName)) ;
	if (sport) _serverPort = atoi(sport) ;

}


void CPGLConnection::internalConnect (char *database, char *userName, char *password)
{
	PGconn *conn = 0 ;
	try {
		char conninfo[1024] ;
        string srv(static_cast<const char *>(_server)) ;
        if (_roleSupported.find(srv) == _roleSupported.end()) {
            void *handle = dlopen(0,RTLD_NOLOAD | RTLD_GLOBAL | RTLD_LAZY) ;
            if (handle) {
                void *conninfoParseFunction = dlsym(handle, "PQconninfoParse");
                _roleSupported[srv] = (conninfoParseFunction != 0) ;
                dlclose(handle) ;
            }
            else { _roleSupported[srv] = false ; }
        }
        
        if (strcmp(_roleName, "") && _roleSupported[srv] ) {
    	    sprintf(conninfo,"host='%s' dbname='%s' user='%s' password='%s' port='%d' application_name='%s'",static_cast<char *>(_server), database, userName, password, _serverPort, static_cast<char *>(_roleName)) ;
        }
        else {
    	    sprintf(conninfo,"host='%s' dbname='%s' user='%s' password='%s' port='%d'",static_cast<char *>(_server), database, userName, password, _serverPort) ;
		}      
#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLConnection::internalConnect: !%s!\n", conninfo) ;
#endif
		conn = PQconnectdb(conninfo);
		checkPGLResult(conn, database) ;
	}
	catch(...)  {
		if (conn) {
			PQfinish(conn) ;
		}
		throw ;
	}

	_pgconn = conn;
    
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLConnection::internalConnect: _pgconn: %lld (0x%08llx)\n", (long long int)_pgconn,(long long int)_pgconn) ;
#endif
}

bool CPGLConnection::isConnected ()
{
	return _pgconn != 0 ;
}

void CPGLConnection::setQualifiedName (char *value)
{
	_qualifiedName = value;

	// parse qualified name for catalog name, schema name and object name
	//
	char *pos = _qualifiedName;
	while (*pos != 0 && *pos != '.' && *pos != ':') pos++;
	if (*pos == ':') {
		*pos = 0;
		_catalogName = _qualifiedName;
		_schemaName = ++pos;
	} else {
		_catalogName = "";
		_schemaName = _qualifiedName;
	}
	while (*pos != 0 && *pos != '.') pos++;
	if (*pos == '.') {
		*pos = 0;
		_objectName = ++pos;
	}
	else {
		_objectName = _schemaName;
		_schemaName = "";
	}

	// create valid quoted name
	//
	_quotedObjectName.attach(new char [strlen(_catalogName) + strlen(_schemaName) + strlen(_objectName) + sizeof(":\"\".")]);
	if (*_catalogName != 0)
		strcat(strcpy(_quotedObjectName, _catalogName), ":");
	else
		*_quotedObjectName = 0;
	if (*_schemaName != 0)
		strcat(strcat(strcat(_quotedObjectName, "\""), _schemaName), "\".");
	strcat(_quotedObjectName, _objectName);
}

int
CPGLConnection::getGeoOID()
{
    if (!_pgconn) {
		cout << "getGeoOID: No connection !\n" ;
        return 0 ;
    }
	PGresult *res1;
	char *temp_int;
	int OID;

	res1=PQexec(_pgconn, "select OID from pg_type where typname = 'geography'");
	if ( ! res1 || PQresultStatus(res1) != PGRES_TUPLES_OK )
	{
		cout << "getGeoOID: OIDQuery: " << PQerrorMessage(_pgconn) ;
		return -1;
	}

	if (PQntuples(res1) <= 0 )
	{
		cout << "getGeoOID: Geometry type unknown (have you enabled postgis?)\n" ;
		return 0;
	}

	temp_int = PQgetvalue(res1, 0, 0);
	OID = atoi(temp_int);
	PQclear(res1);
	return OID;
}

/////////////////////////////////////////////////////////////////////////////
// CPGLConnectionStringParser

CPGLConnectionStringParser CPGLConnectionStringParser::_instance;

CPGLConnectionStringParser& CPGLConnectionStringParser::getInstance ()
{
	return _instance;
}

#ifdef PGL_SHOW_WARNING
#warning MP CPGLConnectionStringParser::parse body disabled 
#endif
void CPGLConnectionStringParser::parse (char *connectionString, CPGLConnection *connectionContext)
{
/*
	char *name, *_name, *value, *_value, separator;

	while (*connectionString != 0)
	{
		// extract name
		while (isspace(*connectionString) != 0) connectionString++;
		name = _name = connectionString;
		while (*connectionString != '=' && *connectionString != ';' && *connectionString != 0)
			if (isspace(*connectionString++) == 0)
				_name = connectionString;
		if ((separator = *connectionString) != 0)
			connectionString++;
		*_name = 0;

		// value without a key is interpreted as server name
		if (separator != '=') {
			connectionContext->setServer(name);
			continue;
		}
		
		// extract value
		while (isspace(*connectionString) != 0) connectionString++;
		value = _value = connectionString;
		while (*connectionString != ';' && *connectionString != 0)
			if (isspace(*connectionString++) == 0)
				_value = connectionString;
		if (*connectionString != 0)
			connectionString++;
		*_value = 0;

		// dispatch connection string parameter
		OptionFuncTable::_item_t *item = _optionFuncTable.get(_cstr_t::upperCase(name));
		if (item == 0)
			ifx_putenv(name, _name - name, value, _value - value);
		else
			item->value(connectionContext, value);
	}
*/
}

CPGLConnectionStringParser::CPGLConnectionStringParser (): _optionFuncTable(23)
{
	_optionFuncTable.add("AUTOCOMMIT")->value = setAutoCommit;
	_optionFuncTable.add("BLOBSIZE")->value = setBlobSize;
#ifdef PGL_SHOW_WARNING
#warning MP CPGLConnection::setClientLocale temporary disabled
#endif
//	_optionFuncTable.add("CLIENT_LOCALE")->value = setClientLocale;
	_optionFuncTable.add("COMMITRETAIN")->value = setCommitRetain;
	_optionFuncTable.add("DECIMAL")->value = setDecimal;
	_optionFuncTable.add("INFORMIXSERVER")->value = setServer;
	_optionFuncTable.add("INT8")->value = setInt8;
	_optionFuncTable.add("ISOLATIONLEVEL")->value = setIsolationLevel;
	_optionFuncTable.add("MONEY")->value = setMoney;
	_optionFuncTable.add("LVARCHAR")->value = setLVarChar;
	_optionFuncTable.add("TRIM CHAR")->value = setTrimChar;
	_optionFuncTable.add("WAITONLOCKS")->value = setWaitOnLocks;
}

void CPGLConnectionStringParser::setAutoCommit (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setAutoCommit(strcmp(_cstr_t::upperCase(value), "TRUE") == 0);
}

void CPGLConnectionStringParser::setBlobSize (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setBlobSize(atoi(value));
}
#ifdef PGL_SHOW_WARNING
#warning MP CPGLConnection::setClientLocale temporary disabled
#endif
/*
void CPGLConnectionStringParser::setClientLocale (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setClientLocale(value);
}
*/
void CPGLConnectionStringParser::setCommitRetain (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setCommitRetain(strcmp(_cstr_t::upperCase(value), "TRUE") == 0);
}

void CPGLConnectionStringParser::getNumericMapping (char *value, DataType& dataType)
{
	if (strcmp(_cstr_t::upperCase(value), "BCD") == 0)
		dataType = DataType(fldBCD, 0);
	else if (strcmp(value, "FMTBCD") == 0)
		dataType = DataType(fldFMTBCD, 0);
	else if (strcmp(value, "FLOAT") == 0)
		dataType = DataType(fldFLOAT, 0);
	else if (strcmp(value, "CURRENCY") == 0)
		dataType = DataType(fldFLOAT, fldstMONEY);
}

void CPGLConnectionStringParser::setDecimal (CPGLConnection *connectionContext, char *value)
{
	getNumericMapping(value, connectionContext->getTypesMapping()->decimal);
}

void CPGLConnectionStringParser::setInt8 (CPGLConnection *connectionContext, char *value)
{
	getNumericMapping(value, connectionContext->getTypesMapping()->int8);
}

void CPGLConnectionStringParser::setIsolationLevel (CPGLConnection *connectionContext, char *value)
{
	eXILType level;
	if (strcmp(_cstr_t::upperCase(value), "DIRTYREAD") == 0)
		level = xilDIRTYREAD;
	else if (strcmp(value, "READCOMMITTED") == 0)
		level = xilREADCOMMITTED;
	else if (strcmp(value, "REPEATABLEREAD") == 0)
		level = xilREPEATABLEREAD;
	else
		level = xilCUSTOM;
	connectionContext->setIsolationLevel(level);
}

void CPGLConnectionStringParser::setMoney (CPGLConnection *connectionContext, char *value)
{
	getNumericMapping(value, connectionContext->getTypesMapping()->money);
}

void CPGLConnectionStringParser::setLVarChar (CPGLConnection *connectionContext, char *value)
{
	if (strcmp(_cstr_t::upperCase(value), "STRING") == 0)
		connectionContext->getTypesMapping()->lvarchar = DataType(fldZSTRING, 0);
	else if (strcmp(value, "BLOB") == 0)
		connectionContext->getTypesMapping()->lvarchar = DataType(fldBLOB, fldstFMTMEMO);
}

void CPGLConnectionStringParser::setServer (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setServer(value);
}

void CPGLConnectionStringParser::setTrimChar (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setTrimChar(strcmp(_cstr_t::upperCase(value), "TRUE") == 0);
}

void CPGLConnectionStringParser::setWaitOnLocks (CPGLConnection *connectionContext, char *value)
{
	connectionContext->setWaitOnLocks(strcmp(_cstr_t::upperCase(value), "TRUE") == 0);
}


