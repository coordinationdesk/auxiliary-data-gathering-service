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
	Revision 1.5  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.4  2008/11/27 11:12:48  marpas
	File and Line infos added when creating CPGLServerError exception with a PGresult
	
	Revision 1.3  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.2  2008/07/21 17:03:51  paoscu
	work in progress
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.4  2008/06/13 09:44:15  lucio.pulvirenti
	EC:: CPGLResultGuardVector class added (unused)
	
	Revision 1.3  2008/02/25 17:20:56  marpas
	work in progres
	
	Revision 1.2  2007/12/17 18:38:30  marpas
	work in progress
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/


#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include <dbxpress.hpp>
#include <dbxcommon.hpp>
#include "cstr.hpp"
#include <libpq-fe.h>
#include <exception>


class CPGLResultGuard 
{
public:
	CPGLResultGuard (PGresult *r) : _res(r) {}
	virtual ~CPGLResultGuard () throw() { if (_res) PQclear(_res) ;}
private:
    CPGLResultGuard(const CPGLResultGuard &) ;
    CPGLResultGuard & operator= (const CPGLResultGuard &) ;
private:
	PGresult *_res ;
};



/////////////////////////////////////////////////////////////////////////////
// CPGLException - exception class for Informix driver error

class CPGLException : public std::exception
{
public:
	CPGLException (const char *message) ;
	CPGLException (const char *message, const char *file, int line) ;
	virtual ~CPGLException () throw() {}
	CPGLException(const CPGLException&) ;
	CPGLException &operator=(const CPGLException&) ;
	virtual const char * what () const throw() ;
	void setMessage(const _cstr_t &) ;
private:
	_cstr_t _message;
};


/////////////////////////////////////////////////////////////////////////////
// CPGLDriverError - exception class for Informix driver error

class CPGLDriverError : public CPGLException
{
public:
	CPGLDriverError (const char *message) ;
	CPGLDriverError (const char *message, const char *file, int line) ;
	virtual ~CPGLDriverError () throw() {}
	CPGLDriverError(const CPGLDriverError &r) ;
	CPGLDriverError &operator=(const CPGLDriverError &r) ;

};


#define ACSNOTIMPLEMENTED(m)	throw CPGLDriverError(m,__FILE__,__LINE__) 

/////////////////////////////////////////////////////////////////////////////
// CInvalidCastError

class CInvalidCastError : public CPGLException
{
public:
	CInvalidCastError () : CPGLException("Invalid cast") {}
	CInvalidCastError (const char *message, const char *file, int line) : CPGLException(message, file, line) {}
	virtual ~CInvalidCastError () throw() {}
};


/////////////////////////////////////////////////////////////////////////////
// CInvalidParameterTypeError

class CInvalidParameterTypeError : public CPGLException
{
public:
	CInvalidParameterTypeError () : CPGLException("Allowed parameter type is only INPUT") {}
	CInvalidParameterTypeError (const char *message, const char *file, int line) : CPGLException(message, file, line) {}
	virtual ~CInvalidParameterTypeError () throw() {}
};


/////////////////////////////////////////////////////////////////////////////
// CPGLServerError

class CPGLServerError : public CPGLException
{
public:
	CPGLServerError (PGconn *, const char *dbName);
	CPGLServerError (const PGresult *, const char *, int);
	virtual ~CPGLServerError ()  throw() {}
//	virtual const char * what () const throw() ;
	CPGLServerError(const CPGLServerError &r) ;
	CPGLServerError &operator=(const CPGLServerError &r) ;
private:
//	_cstr_t _message;
};


/////////////////////////////////////////////////////////////////////////////
// DB2 API result check functions

inline void checkPGLResult (PGconn *conn, const char *dbName)
{
	if (PQstatus(conn) != CONNECTION_OK)
		throw CPGLServerError(conn, dbName);
}

#define checkPGLResultStatus(res) checkPGLResultStatusP(res,__FILE__,__LINE__)

inline void checkPGLResultStatusP(const PGresult *res, const char *f, int l)
{
	switch(PQresultStatus(res)) {
	case PGRES_COMMAND_OK: // Successful completion of a command returning no data.
	case PGRES_TUPLES_OK: // Successful completion of a command returning data (such as a SELECT or SHOW).
	case PGRES_COPY_OUT: // Copy Out (from server) data transfer started.
	case PGRES_COPY_IN: // Copy In (to server) data transfer started.
		break ;
	case PGRES_EMPTY_QUERY: // The string sent to the server was empty.
	case PGRES_BAD_RESPONSE: // The server's response was not understood.
	case PGRES_NONFATAL_ERROR: // A nonfatal error (a notice or warning) occurred.
	case PGRES_FATAL_ERROR: // A fatal error occurred.
	default:
		throw CPGLServerError(res, f, l) ;
	}
}

