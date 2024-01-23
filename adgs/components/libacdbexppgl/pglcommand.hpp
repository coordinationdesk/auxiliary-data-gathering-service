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

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include "pglconnection.hpp"


/////////////////////////////////////////////////////////////////////////////
// CPGLCommand - SQLCommand implementation for DB2

class CPGLCommand : public CErrorHandlingImpl< CUnknownImpl<SQLCommand> >
{
public:
	CPGLCommand (CPGLConnection *connection);
	virtual ~CPGLCommand () throw() ;

	CPGLConnection * getConnection ();
	INT32 getBlobSize ();
#ifdef PGL_SHOW_WARNING
#warning MP CPGLCommand::getParameters temporary disabled
#endif
//	CPGLParameters& getParameters ();

// SQLCommand
public:
	SQL_METHOD setOption (eSQLCommandOption eSOption, INT32 lValue);
	SQL_METHOD getOption (eSQLCommandOption eSOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength);
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLCommandOption eSOption, INT64 lValue);
	SQL_METHOD getOption (eSQLCommandOption eSOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength);
#endif
	SQL_METHOD setParameter (UINT16 uParameterNumber, UINT16 uChildPos, STMTParamType ePType, UINT16 uLogType, UINT16 uSubType, INT32 lMaxPrecision, INT32 lMaxScale, UINT32 ulLength, pVOID pBuffer, BOOL bIsNull);
	SQL_METHOD getParameter (UINT16 uParameterNumber, UINT16 uChildPos, pVOID pData, UINT32 ulLength, pINT32 plInd);
	SQL_METHOD prepare (pCHAR pszSQL, UINT16 uParamCount);
	SQL_METHOD execute (ppSQLCursor ppCur);
	SQL_METHOD executeImmediate (pCHAR pszSQL, ppSQLCursor ppCur);
	SQL_METHOD getNextCursor (ppSQLCursor ppCur);
	SQL_METHOD getRowsAffected (pINT32 plRows);
	SQL_METHOD close ();
private:
	CPGLCommand(const CPGLCommand & ) ;	
   	CPGLCommand & operator= (const CPGLCommand & ) ;	
private:
	CPGLConnection *_connection;
#ifdef PGL_SHOW_WARNING
#warning MP CPGLCommand::_parameters member temporary disabled
#endif
//	CPGLParameters *_parameters;
	INT32 _rowsAffected;

	                             // dbExpress command options
	                             // _________________________
	                             //
	INT32 _rowsetSize;            // eCommRowsetSize
	INT32 _blobSize;              // eCommBlobSize
	bool _blockRead;             // eCommBlockRead
	bool _prepared ;
	char _curName[256] ;
};
