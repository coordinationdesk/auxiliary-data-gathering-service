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
	Revision 1.9  2012/12/10 17:02:29  marpas
	qa rules
	
	Revision 1.8  2012/02/13 11:29:56  marpas
	useless include and message removed
	
	Revision 1.7  2012/02/13 11:28:58  marpas
	useless include removed
	
	Revision 1.6  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.5  2008/12/05 16:45:02  lucio.pulvirenti
	cleanup and DEBUG (tmp)
	
	Revision 1.4  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.3  2008/07/21 17:13:04  paoscu
	namespace acs removed
	
	Revision 1.2  2008/07/21 17:12:08  paoscu
	version files added
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.3  2008/07/07 14:19:46  enrcar
	EC:: debug improved
	
	Revision 1.2  2008/07/04 14:42:57  enrcar
	EC:: setOption and getOption now handles 64bit lvalues
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/

#include "pgldriver.hpp"

#include "pglconnection.hpp"

#include "acdbexppglV.h"

static acdbexppglV version ;

/////////////////////////////////////////////////////////////////////////////
// API implementation

extern "C" SQLResult API_CALL_TYPE getSQLDriverPOSTGRESQL(
	pCHAR pszVendorLib, pCHAR pszResourceFile, ppSQLDriver ppDrv)
{
	try {
		*ppDrv = new CPGLDriver;
	} catch (std::bad_alloc&) {
		return DBXERR_NOMEMORY;
	}
	return SQL_SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLDriver - SQLDriver implementation for DB2

CPGLDriver::CPGLDriver () :
    _blobSize(-1), 
    _callback()
{
//	_blobSize = -1;
}


CPGLDriver::~CPGLDriver() throw()
{
}


INT32 CPGLDriver::getBlobSize ()
{
	return _blobSize;
}

CCallback* CPGLDriver::getCallback()
{
	if (_callback.getHandler() == 0)
		return 0;
	return &_callback;
}

SQL_METHOD_IMP CPGLDriver::getSQLConnection (ppSQLConnection ppConn)
{
	try {
		*ppConn = new CPGLConnection(this);
		return SQL_SUCCESS;
	} catch (std::bad_alloc&) {
		return DBXERR_NOMEMORY;
	} catch (std::exception& e) {
		fprintf(stderr,e.what());
		return SQL_ERROR;
	} catch (SQLResult sqlResult) {
		return sqlResult;
	} catch (...) {
		fprintf(stderr,"Unknown exception caught. File " __FILE__ ".");
		return SQL_ERROR;
	}
}


SQL_METHOD_IMP CPGLDriver::setOption (eSQLDriverOption eDOption, INT32 PropValue)
{
    #ifdef ACDBEXPPGL_DEBUG
    	printf("CPGLDriver::setOption32\n");
    #endif
    
	switch (eDOption)
	{
	case eDrvBlobSize:
		_blobSize = PropValue; break;

#ifndef __x86_64__
	case eDrvCallBack:
		_callback.setHandler(reinterpret_cast<pfCallBack>(PropValue)); break;

	case eDrvCallBackInfo:
		_callback.setInfo(PropValue); break;
#endif

	case eDrvRestrict:
		return SQL_SUCCESS;

	default:
		return DBXERR_NOTSUPPORTED;
	}

	return SQL_SUCCESS;
}


#ifdef __x86_64__
SQL_METHOD_IMP CPGLDriver::setOption (eSQLDriverOption eDOption, INT64 PropValue)
{
    #ifdef ACDBEXPPGL_DEBUG
    	printf("CPGLDriver::setOption64\n");
    #endif
    
	switch (eDOption)
	{

	case eDrvCallBack:
		_callback.setHandler(reinterpret_cast<pfCallBack>(PropValue)); break;

	case eDrvCallBackInfo:
		_callback.setInfo(PropValue); break;

	case eDrvRestrict:
		return SQL_SUCCESS;

	default:
		return DBXERR_NOTSUPPORTED;
	}

	return SQL_SUCCESS;
}
#endif


SQL_METHOD_IMP CPGLDriver::getOption (eSQLDriverOption eDOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength)
{
    #ifdef ACDBEXPPGL_DEBUG
    	printf("CPGLDriver::getOption32\n");
    #endif
    
	try {
		switch (eDOption)
		{
		    case eDrvBlobSize:
			    *getVerifiedValueDestination(sizeof(INT32), plValue, iMaxLength, piLength) = _blobSize; break;

#ifndef __x86_64__
		    case eDrvCallBack:
			    *getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = reinterpret_cast<ACS_PTRASINT_T>(_callback.getHandler()); break;

		    case eDrvCallBackInfo:
			    *getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = _callback.getInfo(); break;
#endif

		    case eDrvRestrict:
			    *piLength = 0; break;

		    default:
			    return DBXERR_NOTSUPPORTED;
		}
	} catch (SQLResult sqlResult) {
		return sqlResult;
	}
	return SQL_SUCCESS;
}


#ifdef __x86_64__
SQL_METHOD_IMP CPGLDriver::getOption (eSQLDriverOption eDOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength)
{
    #ifdef ACDBEXPPGL_DEBUG
    	printf("CPGLDriver::getOption64\n");
    #endif

	try {
		switch (eDOption)
		{
		        case eDrvCallBack:
			        *getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = reinterpret_cast<ACS_PTRASINT_T>(_callback.getHandler()); break;

		        case eDrvCallBackInfo:
			        *getVerifiedValueDestination(sizeof(ACS_PTRASINT_T), plValue, iMaxLength, piLength) = _callback.getInfo(); break;

		        default:
			        return DBXERR_NOTSUPPORTED;
		}
	} catch (SQLResult sqlResult) {
		return sqlResult;
	}
	return SQL_SUCCESS;
}
#endif
