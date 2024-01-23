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
	
	

*/

#include "pglcommand.hpp"

#include "pglcursor.hpp"
#include "pglerror.hpp"
#include "license.hpp"
#include "libpq-fe.h"


/////////////////////////////////////////////////////////////////////////////
// CPGLCommand - SQLCommand implementation for DB2

CPGLCommand::CPGLCommand (CPGLConnection *connection) : 
    _connection(0),
    _rowsAffected(0),
    _rowsetSize(1),
    _blobSize(connection->getBlobSize()),
	_blockRead(false),
	_prepared(false)
{
	(_connection = connection)->AddRef();
	memset(_curName,0,sizeof(_curName)) ;
#ifdef PGL_SHOW_WARNING
#warning MP CPGLCommand _parameters member temporary disabled
#endif
//	_parameters = 0;
//	_rowsetSize = 1;
//	_blobSize = connection->getBlobSize();
//	_blockRead = false;
//	_prepared = false ;
}

CPGLCommand::~CPGLCommand () throw() 
{
	_connection->Release();
}

CPGLConnection * CPGLCommand::getConnection ()
{
	return _connection;
}

INT32 CPGLCommand::getBlobSize ()
{
	return _blobSize;
}



#ifdef PGL_SHOW_WARNING
#warning MP CPGLCommand::getParameters temporary disabled
#endif
/*
CPGLParameters& CPGLCommand::getParameters ()
{
	return *_parameters;
}
*/


// SQLCommand interface implementation

SQL_METHOD_IMP CPGLCommand::setOption (eSQLCommandOption eSOption, INT32 lValue)
{
	switch (eSOption)
	{
	case eCommRowsetSize:
		_rowsetSize = lValue; break;

	case eCommBlobSize:
		_blobSize = lValue; break;

	case eCommBlockRead:
		_blockRead = lValue != 0; break;

	case eCommStoredProc:
		if (lValue != 0)
			return DBXERR_NOTSUPPORTED;
		else
			return SQL_SUCCESS;

	default:
		return DBXERR_NOTSUPPORTED;
	}

	return SQL_SUCCESS;
}

SQL_METHOD_IMP CPGLCommand::getOption (eSQLCommandOption eSOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLCommand::getOption: option: %d\n", (int)eSOption ) ;
#endif
	try {
		switch (eSOption)
		{
		case eCommRowsetSize:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _rowsetSize; 
			break;

		case eCommBlobSize:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _blobSize; 
			break;

		case eCommBlockRead:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _blockRead; 
			break;

		case eCommParamCount:
	//		*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _parameters->getCount(); break;
			throw CPGLDriverError("eCommParamCount command option still unsupported");

/* MARCO
		case eCommNativeHandle:
			*getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = static_cast<INT32>(_hstmt); 
			break;
*/
		default:
			throw CPGLDriverError("unsupported option");
		}
	}
	catch (std::exception& e) {
		setErrorMessage(e.what());
		return DBXERR_NOTSUPPORTED;
	}

	RETURN_SQL_RESULT ;
}

#ifdef __x86_64__

SQL_METHOD_IMP CPGLCommand::setOption (eSQLCommandOption eSOption, INT64 lValue)
{
	switch (eSOption)
	{
	default:
		return DBXERR_NOTSUPPORTED;
	}

	return SQL_SUCCESS;
}

SQL_METHOD_IMP CPGLCommand::getOption (eSQLCommandOption eSOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLCommand::getOption: option: %d\n", (int)eSOption ) ;
#endif
	try {
		switch (eSOption)
		{
		default:
			throw CPGLDriverError("unsupported option");
		}
	}
	catch (std::exception& e) {
		setErrorMessage(e.what());
		return DBXERR_NOTSUPPORTED;
	}

	RETURN_SQL_RESULT ;
}

#endif



#ifdef PGL_SHOW_WARNING
#warning MP CPGLCommand::setParameter body disabled
#endif
SQL_METHOD_IMP CPGLCommand::setParameter (UINT16 uParameterNumber, UINT16 uChildPos, STMTParamType ePType, UINT16 uLogType, UINT16 uSubType, INT32 lMaxPrecision, INT32 lMaxScale, UINT32 ulLength, pVOID pBuffer, BOOL bIsNull)
{
/*
	try
	{
		TRACE_MESSAGE(getConnection()->getCallback())(traceDATAIN, "set parameter number=%hu ptype=%d logtype=%hu subtype=%hu", uParameterNumber, ePType, uLogType, uSubType);

		if (!bIsNull)
		{
			CPGLParameterValueFactory::Args factoryArgs;
			factoryArgs.paramType = ePType;
			factoryArgs.dataType = DataType(uLogType, uSubType);
			factoryArgs.length = ulLength;
			factoryArgs.buffer = pBuffer;
			factoryArgs.connection = getConnection();
			getParameters()[uParameterNumber].setValue(CPGLParameterValueFactory::createValue(factoryArgs));
		}
		else
			getParameters()[uParameterNumber].setNull();

		return SQL_SUCCESS;
	} catch (std::bad_alloc&) {
		return DBXERR_NOMEMORY;
	} catch (std::exception& e) {
		getConnection()->setErrorMessage(e.what());
		return SQL_ERROR;
	} catch (SQLResult sqlResult) {
		return sqlResult;
	} catch (...) {
		getConnection()->setErrorMessage("Unknown exception caught. File "__FILE__".");\
		return SQL_ERROR;
	}
*/
	return SQL_ERROR;
}

SQL_METHOD_IMP CPGLCommand::getParameter (UINT16 uParameterNumber, UINT16 uChildPos, pVOID pData, UINT32 ulLength, pINT32 plInd)
{
	TRACE_MESSAGE(getConnection()->getCallback())(traceDATAOUT, "get parameter number=%hu", uParameterNumber);

	return DBXERR_NOTSUPPORTED;
}

SQL_METHOD_IMP CPGLCommand::prepare (pCHAR pszSQL, UINT16 uParamCount) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLCommand::prepare: SQL: !%s!\n", (char*)pszSQL ) ;
#endif

	CLicenseManager::checkLicense();

	TRACE_MESSAGE(getConnection()->getCallback())(traceQPREPARE, "prepare %s", pszSQL);
	
	PGresult *res = PQprepare(_connection->getPGconn(), 
							  "",  // default - will be replaced by next prepare
							  pszSQL,
							  0,
							  NULL) ;

#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLCommand::prepare: PQprepare ended\n") ;
#endif
	CPGLResultGuard resGuard(res) ;/* Theoretically, this memory SHOULD NOT BE FREED (See libpq Reference Manual) -- But the guard avoids a memory leakage... (TBC) */
	checkPGLResultStatus(res);


	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCommand::execute (ppSQLCursor ppCur) SAFE_SQL_METHOD
{
	TRACE_MESSAGE(getConnection()->getCallback())(traceQEXECUTE, "execute");

	*ppCur = 0 ;
	PGresult *res = 0 ;
	try {
#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLCommand::execute: calling PQexecPrepared\n") ;
#endif
		res = PQexecPrepared(_connection->getPGconn(),
							 "",
							 0,
							 NULL, 
							 NULL,
							 NULL,
							 1) ; //  binary results
		checkPGLResultStatus(res);
	}
	catch (...) {
		if(res) PQclear(res) ;
		throw ;
	}
#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLCommand::execute: PQexecPrepared called res = %p\n", (void *)res) ;
		printf("CPGLCommand::execute: PQexecPrepared returned %d tuples\n", PQntuples(res)) ;
#endif
	*ppCur = new CPGLCursor(res, this) ;
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: %s\n", PQresultErrorMessage(res)) ;
#endif
	/* No guard for PGresult is needed, since the free is in charge to CPGLCursor */
	checkPGLResultStatus(res);

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: about to ask for row count\n") ;
#endif
	_rowsAffected =  atoi(PQcmdTuples(res)) ;

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: row count is: %d\n", _rowsAffected) ;
#endif


	RETURN_SQL_RESULT;
}


SQL_METHOD_IMP CPGLCommand::executeImmediate (pCHAR pszSQL, ppSQLCursor ppCur) SAFE_SQL_METHOD
{
	TRACE_MESSAGE(getConnection()->getCallback())(traceQEXECUTE, "execute immediate");

	*ppCur = 0 ;
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: calling PQexec\n") ;
    printf("CPGLCommand::execute: SQL: !%s!\n", pszSQL) ;
#endif

    PGresult *res = 0 ;
	try {
		res = PQexec(_connection->getPGconn(), pszSQL) ; 
  
  		checkPGLResultStatus(res);
	}
	catch (...) {
		if(res) PQclear(res) ;
		throw ;
	}
         		                    
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: PQexec called res = %p\n", (void *)res) ;
	printf("CPGLCommand::execute: PQexec returned %d tuples\n", PQntuples(res)) ;
#endif

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: about to ask for affected rows\n") ;
#endif
	_rowsAffected =  atoi(PQcmdTuples(res)) ;

    if (PQntuples(res) != 0) {
		/* No guard for PGresult is needed, since the free is in charge to CPGLCursor */
    	*ppCur = new CPGLCursor(res, this) ;
//		*ppCur = getConnection()->getTrimChar() ? new CPGLTrimingCursor(this) : new CPGLCursor(res, this);
	} else {
		CPGLResultGuard resguard(res) ;
		*ppCur = 0 ;
	}

// #ifdef ACDBEXPPGL_DEBUG
// 	printf("CPGLCommand::execute: %s\n", PQresultErrorMessage(res)) ;
// #endif
// 	checkPGLResultStatus(res);

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLCommand::execute: affected row count is: %d\n", _rowsAffected) ;
#endif

	RETURN_SQL_RESULT;
}


SQL_METHOD_IMP CPGLCommand::getNextCursor (ppSQLCursor ppCur)
{
	*ppCur = 0;
	return SQL_SUCCESS;
}

SQL_METHOD_IMP CPGLCommand::getRowsAffected (pINT32 plRows)
{
	*plRows = _rowsAffected;
	return SQL_SUCCESS;
}

SQL_METHOD_IMP CPGLCommand::close ()
{
	return SQL_SUCCESS;
}


