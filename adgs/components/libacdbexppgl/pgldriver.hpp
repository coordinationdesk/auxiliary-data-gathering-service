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
	Revision 1.3  2008/12/05 16:45:02  lucio.pulvirenti
	cleanup and DEBUG (tmp)
	
	Revision 1.2  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.2  2008/07/04 14:42:57  enrcar
	EC:: setOption and getOption now handles 64bit lvalues
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include "dbxpress.hpp"
#include "dbxcommon.hpp"


/////////////////////////////////////////////////////////////////////////////
// API declaration

extern "C" SQLResult API_CALL_TYPE getSQLDriverPOSTGRESQL (
	pCHAR pszVendorLib, pCHAR pszResourceFile, ppSQLDriver ppDrv
	);


/////////////////////////////////////////////////////////////////////////////
// CPGLDriver - SQLDriver implementation for DB2

class CPGLDriver : public CUnknownImpl<SQLDriver>
{
public:
	CPGLDriver ();
	virtual ~CPGLDriver() throw() ;
	INT32 getBlobSize ();
	CCallback* getCallback ();
	
// SQLDriver
	SQL_METHOD getSQLConnection (ppSQLConnection ppConn);
	SQL_METHOD setOption (eSQLDriverOption eDOption, INT32 PropValue);
	SQL_METHOD getOption (eSQLDriverOption eDOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength);
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLDriverOption eDOption, INT64 PropValue);
	SQL_METHOD getOption (eSQLDriverOption eDOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength);
#endif

	
private:

	                          // dbExpress driver options
	                          // ________________________
	                          //
	INT32 _blobSize;		  // eDrvBlobSize
	CCallback _callback;      // eDrvCallBack
	                          // eDrvCallBackInfo
};
