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
	Revision 1.6  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.5  2008/12/01 14:40:00  lucio.pulvirenti
	PQresultErrorMessage called on PGresult obj only if PQresultStatus >= 0
	
	Revision 1.4  2008/11/27 11:12:48  marpas
	File and Line infos added when creating CPGLServerError exception with a PGresult
	
	Revision 1.3  2008/11/24 18:10:02  paoscu
	diagnostics improved
	
	Revision 1.2  2008/11/20 14:18:41  marpas
	rewriting and fixing
	
	Revision 1.1  2008/07/21 16:46:18  paoscu
	aligning with dev env
	
	Revision 1.4  2008/05/09 13:28:14  lucio.pulvirenti
	EC:: useless include removed
	
	Revision 1.3  2008/02/25 17:20:56  marpas
	work in progres
	
	Revision 1.2  2007/12/17 18:38:30  marpas
	work in progress
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/

#include "pglerror.hpp"

#include <sstream>
#include <stdio.h>


#define MAX_ERROR_MSG_LEN 0x7fff


CPGLException::CPGLException (const char *message) : _message(message) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLException::CPGLException called. \"%s\"\n", message) ;
#endif

}

CPGLException::CPGLException(const char *message, const char *file, int line) : _message()
{
	_cstr_t strSQLMessage(MAX_ERROR_MSG_LEN);
	sprintf(strSQLMessage,"%s at line %d (%s)",file, line, message) ;
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLException::CPGLException called. \"%s\"\n", (char *)strSQLMessage) ;
#endif
	_message = strSQLMessage ;
  
}

CPGLException::CPGLException(const CPGLException& r)  : exception(r), _message(r._message)
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLException copy ctor\n") ;
#endif
}

CPGLException &CPGLException::operator=(const CPGLException &r) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLException::operator=\n") ;
#endif
	if (this != &r) {
		exception::operator=(r) ;
		_message = r._message ;
	}
	
	return *this ;
}

const char * CPGLException::what () const throw() { return _message; }

void CPGLException::setMessage(const _cstr_t & message) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLException::setMessage\n") ;
#endif
	_message = message ;
}


CPGLDriverError::CPGLDriverError (const char *message) : CPGLException(message) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLDriverError::CPGLDriverError called. \"%s\"\n", message) ;
#endif
}

CPGLDriverError::CPGLDriverError (const char *message, const char *file, int line) : CPGLException(message, file, line) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLDriverError::CPGLDriverError called. \"%s\"\n", message) ;
#endif
}


CPGLDriverError::CPGLDriverError(const CPGLDriverError &r) : CPGLException(r) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLDriverError copy ctor\n") ;
#endif
};

CPGLDriverError &CPGLDriverError::operator=(const CPGLDriverError &r) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLDriverError::operator=\n") ;
#endif
	CPGLException::operator=(r) ; return *this ; 
}



/////////////////////////////////////////////////////////////////////////////
// CPGLServerError - exception class for Postgres server error

// MARCO 

CPGLServerError::CPGLServerError (PGconn *conn, const char *dbName) : CPGLException("")
{
	_cstr_t strSQLMessage(MAX_ERROR_MSG_LEN);
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLServerError::CPGLServerError called.\n") ;
#endif
	
 	sprintf(strSQLMessage, "Connection to database \"%s\" failed: %s",dbName, PQerrorMessage(conn));
	setMessage(strSQLMessage) ;
  
} 

CPGLServerError::CPGLServerError (const PGresult *res, const char *f, int l) : CPGLException("")
{
	_cstr_t strSQLMessage(MAX_ERROR_MSG_LEN);

	char *errorMessage = 0;

	if (PQresultStatus(res) != PGRES_BAD_RESPONSE)
	{
		if ( PQresultStatus(res) >= 0) errorMessage = PQresultErrorMessage(res) ;
		
 		sprintf(strSQLMessage, "(%s:%d) Server error: [%s] %d / %s",f,l,PQresStatus(PQresultStatus(res)),PQresultStatus(res),errorMessage);
	}
	else
 		sprintf(strSQLMessage, "(%s:%d) Server error: [%s] %d / PGRES_BAD_RESPONSE",f,l,PQresStatus(PQresultStatus(res)),PQresultStatus(res));
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLServerError::CPGLServerError called. \"%s\"", (char *)strSQLMessage) ;
#endif
	setMessage(strSQLMessage) ;
} 


CPGLServerError::CPGLServerError(const CPGLServerError& r) : CPGLException(r)
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLServerError copy ctor\n") ;
#endif
//	*this = r ;
}

CPGLServerError &CPGLServerError::operator=(const CPGLServerError &r) 
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLServerError::operator=\n") ;
#endif
	if (this != &r) {
		CPGLException::operator=(r) ;
	}
	
	return *this ;
}

// const char * CPGLServerError::what () const throw() { return _message; }

