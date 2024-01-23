// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2021, Exprivia SpA
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.exprivia.com
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Exprivia SpA
 *
 *  $Prod: A.C.S. libACS_SSL  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 2.2  2014/05/06 17:29:01  marfav
 *  Moving GCRY declaration from h to C module
 *
 *  Revision 2.1  2014/05/06 16:45:19  marfav
 *  Addedd global initialization support for crypto engines
 *
 *
 *
 */

// From OpenSSL 1.1.0 on the locking callbacks functions are disabled so 
// if OPENSSL_VERSION_NUMBER is gt than 0x10100000L the new light approach is used 
// a lot of code is hence commented (ifdef) away from old code.

#ifndef _CryptoEngineInitializer_H_
#define _CryptoEngineInitializer_H_

#include <exException.h>
#include <Singleton.hpp>

// don't move we need OPENSSL_VERSION_NUMBER and a number of other things 

#include <openssl/crypto.h>     // PRQA S 1013
#include <openssl/err.h>        // PRQA S 1013

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  #warning OpenSSL version do not need locking callback support 
  
  #include <Timer.h>
  
  #ifndef OPENSSL_THREADS
    #warning OpenSSL has no thread support
  #endif

  // MP: This must be in the global namespace
  struct CRYPTO_dynlock_value { // PRQA S 2171, 2173, 2175
      acs::ThreadSafe mutex ;  // PRQA S 2100
      CRYPTO_dynlock_value() : mutex() {} 
      ~CRYPTO_dynlock_value() throw () {} 
  private:
      CRYPTO_dynlock_value(const CRYPTO_dynlock_value &) ;
      CRYPTO_dynlock_value & operator= (const CRYPTO_dynlock_value &) ;
  } ;
  
#endif // OPENSSL_VERSION_NUMBER < 0x10100000L


_ACS_BEGIN_NESTED_NAMESPACE(acs, utils) 


using acs::pattern::Singleton ;

class CryptoEngineInitializer : public Singleton<CryptoEngineInitializer> // PRQA S 2109, 2153
{
    friend CryptoEngineInitializer* Singleton<CryptoEngineInitializer>::instance(); // PRQA S 2107

public:

	exDECLARE_EXCEPTION(CryptoEngineException,exException) ;    // PRQA S 2131, 2502

	virtual ~CryptoEngineInitializer() throw() ;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    void checkSSL() ;
#else 
    void checkSSL() {} // inline nop
#endif // OPENSSL_VERSION_NUMBER < 0x10100000L

protected:
    CryptoEngineInitializer() ;

private:
    CryptoEngineInitializer(const CryptoEngineInitializer &) = delete ;
    CryptoEngineInitializer & operator= (const CryptoEngineInitializer &) = delete ;
    
    void openSSL_global_initialization();

    void ssh2_global_initialization();
    void ssh2_housekeeping();

    void gcrypt_init();
        
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    void ssl_thread_init() ;
    void ssl_thread_destroy() ;
    void ssl_lock(int mode, int n, const char *file, int line) throw() ;

    static void ssl_locking_function(int mode, int n, const char *file, int line) throw() ;
	static CRYPTO_dynlock_value * ssl_dyn_create_function(const char *file, int line) throw() ;
    static void ssl_dyn_lock_function(int mode, struct CRYPTO_dynlock_value *dl, const char *file, int line) throw() ;
    static void ssl_dyn_destroy_function(struct CRYPTO_dynlock_value *dl, const char *file, int line) ;
  #if OPENSSL_VERSION_NUMBER >= 0x010000000L
    static void ssl_threadid_function(CRYPTO_THREADID *id) { CRYPTO_THREADID_set_numeric(id, pthread_self()); }
  #else
  #warning OpenSSL version is too old - using deprecated thread safety methods
    static unsigned long ssl_threadid_function() { return pthread_self(); }
  #endif
#endif // OPENSSL_VERSION_NUMBER < 0x10100000L

private:

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    ThreadSafe *ssl_mutex_ ;
    size_t ssl_mutex_size ;

    Timer warntimer_ ;
    static const int _defaultWarningPeriod = 120000 ; // 120 sec  
#endif // OPENSSL_VERSION_NUMBER < 0x10100000L

	ACS_CLASS_DECLARE_DEBUG_LEVEL(CryptoEngineInitializer) ;
};

_ACS_END_NESTED_NAMESPACE

#endif //_CryptoEngineInitializer_H_
