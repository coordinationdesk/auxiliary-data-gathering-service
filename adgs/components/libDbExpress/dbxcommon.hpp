// PRQA S 1050 EOF
/*
 *  dbxcommon.hpp - dbExpress common part for drivers realization
 *
 *    Copyright (c) 2001-2004, Luxena Software Company. All rights reserved.
 *
 *  Purpose:
 *
 */

#ifndef __dbxcommon_hpp__
#define __dbxcommon_hpp__

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include <exception>
#include <new>
#include <memory.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "cstr.hpp"
#include "dbxpress.hpp"
#include <pthread.h>


/////////////////////////////////////////////////////////////////////////////
// CUnknownImpl - SQLUnknown implementation

template <class T>
class CUnknownImpl : public T // PRQA S 2109
{
public:
	CUnknownImpl () ;
	virtual ~CUnknownImpl () throw() ;

// SQLUnknown
public:
	COM_METHOD(INT32) QueryInterface (const SQLUnknown::GUID& riid, void **ppvObject);
	COM_METHOD(UINT32) AddRef ();
	COM_METHOD(UINT32) Release ();
private:
    CUnknownImpl(const CUnknownImpl &) ; // not implemented
    CUnknownImpl& operator= (const CUnknownImpl &) ; // not implemented

private:
	UINT32 _refCount;
	pthread_mutexattr_t _mutexAttribute ;
	mutable pthread_mutex_t  _mutex ;
};

template <class T>
CUnknownImpl<T>::CUnknownImpl () : T(), _refCount(1), _mutexAttribute(), _mutex()
{
	::pthread_mutexattr_init(&_mutexAttribute) ;
	::pthread_mutexattr_settype(&_mutexAttribute,PTHREAD_MUTEX_RECURSIVE) ;
	::pthread_mutex_init(&_mutex, &_mutexAttribute) ;
}

template <class T>
CUnknownImpl<T>::~CUnknownImpl() throw() 
{
	::pthread_mutex_destroy(&_mutex) ;
	::pthread_mutexattr_destroy(&_mutexAttribute) ;
}


template <class T>
COM_METHOD_IMP(INT32) CUnknownImpl<T>::QueryInterface (const SQLUnknown::GUID& riid, void **ppvObject)
{
	*ppvObject = static_cast<SQLUnknown *>(this);
	AddRef();
	return 0;
}

template <class T>
COM_METHOD_IMP(UINT32) CUnknownImpl<T>::AddRef ()
{
	pthread_mutex_lock(&_mutex) ;
	++_refCount;
	UINT32 ret = _refCount ;
	pthread_mutex_unlock(&_mutex) ;
	return ret;
}

template <class T>
COM_METHOD_IMP(UINT32) CUnknownImpl<T>::Release ()
{
	pthread_mutex_lock(&_mutex) ;
	UINT32 l = --_refCount;
	pthread_mutex_unlock(&_mutex) ;	
	if (l == 0) {
		delete this;
    }
	return l;
}


/////////////////////////////////////////////////////////////////////////////
// CErrorHandlingImpl - implementation for error handling methods

template <class T>
class CErrorHandlingImpl : public T  // PRQA S 2109
{
public:
    CErrorHandlingImpl() : T(),  _errorMessage() {} 
	virtual ~CErrorHandlingImpl() throw() {}

	// !!! add tracing of error message setting in descendent classes
	void setErrorMessage (const char* errorMessage) { _errorMessage = errorMessage; }
	const char * getErrorMessage() { return _errorMessage; }

	SQL_METHOD getErrorMessage (pBYTE pszError);
	SQL_METHOD getErrorMessageLen (pUINT16 puErrorLen);
private:
	CErrorHandlingImpl(const CErrorHandlingImpl &) ; // not implemented
	CErrorHandlingImpl& operator= (const CErrorHandlingImpl &) ; // not implemented
private:
	_cstr_t _errorMessage;
};

template <class T>
SQL_METHOD_IMP CErrorHandlingImpl<T>::getErrorMessage (pBYTE pszError)
{
	memset(pszError, 0, 1) ; 
	if (_errorMessage == 0)
		return SQL_NULL_DATA;
	memcpy(pszError, _errorMessage, strlen(_errorMessage) + 1);
	return SQL_SUCCESS;
}

template <class T>
SQL_METHOD_IMP CErrorHandlingImpl<T>::getErrorMessageLen (pUINT16 puErrorLen)
{
	*puErrorLen = 1 ; // Since one byte is returned to zero-terminate
	if (_errorMessage == 0)
		return SQL_NULL_DATA;
	*puErrorLen = strlen(_errorMessage) + 1;
	return SQL_SUCCESS;
}

// PRQA S 1020, 1023 EOF
#define SAFE_SQL_METHOD {\
	try

#define RETURN_SQL_RESULT \
		return SQL_SUCCESS;\
	} catch (std::bad_alloc&) {\
		return DBXERR_NOMEMORY;\
	} catch (std::exception& e) {\
		setErrorMessage(e.what());\
		return SQL_ERROR;\
	} catch (SQLResult sqlResult) {\
		return sqlResult;\
	} catch (...) {\
		setErrorMessage("Unknown exception caught. File " __FILE__ ".");\
		return SQL_ERROR;\
	}


/////////////////////////////////////////////////////////////////////////////
// CCallback

class CCallback // PRQA S 2109
{
public:
	CCallback(): _handler(0), _info() {}

	void setHandler (pfCallBack handler) { _handler = handler; }
	pfCallBack getHandler () const { return _handler; } // PRQA S 4628

	void setInfo (ACS_PTRASINT_T info) { _info = info; }
	ACS_PTRASINT_T getInfo () const { return _info; }

	void traceMessage (TRACECat traceCat, const char * message, ...) const;
private:
    CCallback(const CCallback& ) ; // not implemented 
    CCallback &operator= (const CCallback& ) ; // not implemented 
    
private:
	pfCallBack _handler;
	ACS_PTRASINT_T _info;
};

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

inline void CCallback::traceMessage (TRACECat traceCat, const char * message, ...) const // PRQA S 2133, 2134
{
	TRACEDesc traceDesc;    // PRQA S 4102

	traceDesc.eTraceCat = traceCat;
	traceDesc.iCBInfo = _info;

	va_list argptr; // PRQA S 4101
	va_start(argptr, message);  // PRQA S 3080, 3081
	traceDesc.uTotalMsgLen = vsnprintf(traceDesc.szTrace, sizeof(traceDesc.szTrace), message, argptr); // PRQA S 3000, 3010
	va_end(argptr);  // PRQA S 3080, 3081

	_handler(cbTRACE, &traceDesc);
}

#define TRACE_MESSAGE(callback) \
	if ((callback) != 0)\
		(callback)->traceMessage


/////////////////////////////////////////////////////////////////////////////
// Helper for getOption method realization

inline pINT32 getVerifiedValueDestination (INT16 iLength, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) // PRQA S 2134
{
	if (iMaxLength < iLength) {
		throw DBXERR_INVALIDLEN; // PRQA S 3500
    }
	*piLength = iLength;
	return plValue;
}

#ifdef __x86_64__
inline pINT64 getVerifiedValueDestination (INT16 iLength, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) // PRQA S 2134
{
	return reinterpret_cast<pINT64>(getVerifiedValueDestination(iLength, reinterpret_cast<pINT32>(plValue), iMaxLength, piLength)) ; // PRQA S 3030, 3033, 3081
}
#endif

#endif /* __dbxcommon_hpp__ */

