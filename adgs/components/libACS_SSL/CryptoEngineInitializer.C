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
 *  Revision 2.6  2015/10/22 15:33:03  marpas
 *  messages moved from info filterbale class to private info one.
 *
 *  Revision 2.5  2014/07/25 16:20:53  marfav
 *  gcrypt init has its own operation. Crypto engine init order rearranged
 *
 *  Revision 2.4  2014/06/04 17:27:20  marfav
 *  Debug messages improved
 *  gcrypt init sequence fixed
 *
 *  Revision 2.3  2014/05/08 16:57:11  marfav
 *  Check on libssh2 version added for global initialization
 *
 *  Revision 2.2  2014/05/06 17:29:01  marfav
 *  Moving GCRY declaration from h to C module
 *
 *  Revision 2.1  2014/05/06 16:45:19  marfav
 *  Addedd global initialization support for crypto engines
 *
 *
 *
 */

#ifdef FTPCURL_USE_GNUTLS
  #include <gcrypt.h>
  #warning FTPCURL_USE_GNUTLS
#endif 

#include <libssh2.h>

#include <CryptoEngineInitializer.h>

#include <Filterables.h>

#include <openssl/ssl.h> // PRQA S 1013


#ifdef FTPCURL_USE_GNUTLS
// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast in GCRY_THREAD_OPTION_PTHREAD_IMPL
#pragma GCC diagnostic ignored "-Wold-style-cast"

GCRY_THREAD_OPTION_PTHREAD_IMPL ; // PRQA S 2313, 2314, 3054, 3080, 3081, 4060, 4061

// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"

#endif

#undef ACS_SSL_DEBUG_ENABLED
#ifdef ACS_SSL_DEBUG_ENABLED
#warning ACS_SSL_DEBUG_ENABLED defined - ssl sync functions will log their call
#endif


_ACS_BEGIN_NESTED_NAMESPACE(acs, utils) 

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(CryptoEngineInitializer)

CryptoEngineInitializer::CryptoEngineInitializer() : 
    Singleton<CryptoEngineInitializer>()
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    , 
    ssl_mutex_(),
    ssl_mutex_size(),
    warntimer_() 
#endif // #if OPENSSL_VERSION_NUMBER < 0x10100000L
{
    gcrypt_init();
    ssh2_global_initialization();
    openSSL_global_initialization();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    ssl_thread_init() ;
#endif // #if OPENSSL_VERSION_NUMBER < 0x10100000L
    ACS_LOG_PRIVINFO ("Crypto-Engines initialized");
}


CryptoEngineInitializer::~CryptoEngineInitializer() throw() 
{

    // PRQA S 4631 L3
    ssh2_housekeeping();

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    try {
        ssl_thread_destroy() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
#endif // #if OPENSSL_VERSION_NUMBER < 0x10100000L

    // PRQA L:L3
}


void CryptoEngineInitializer::ssh2_global_initialization()
{
#if LIBSSH2_VERSION_NUM >= 0x010205
    int res = libssh2_init(0) ;
    ACS_COND_THROW(res != 0, CryptoEngineException("Cannot initialize libSSH2 (libssh2_init)")); // PRQA S 3081
#else
#warning LIBSSH2 version is less than 1.2.5 and global initialization is not available
#endif
}  

void CryptoEngineInitializer::ssh2_housekeeping()
{
#if LIBSSH2_VERSION_NUM >= 0x010205
    libssh2_exit();
#else
#warning LIBSSH2 version is less than 1.2.5 and global initialization is not available
#endif
}   

void CryptoEngineInitializer::openSSL_global_initialization()
{
    // cannot fail
    SSL_library_init () ;
}


void CryptoEngineInitializer::gcrypt_init()
{
#ifdef FTPCURL_USE_GNUTLS
    Lock l_(_M_mutex()) ;

    ACS_LOG_PRIVINFO ("Initializing gcrypt engine");
    
    gcry_control (GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
    
    if (!gcry_check_version (GCRYPT_VERSION)) 
    {
        ACS_THROW(CryptoEngineException("libgcrypt version mismatch")) ;
    }
    ACS_LOG_PRIVINFO ("gcrypt engine initialization completed");
#endif

}


// From here all the code is dependant frm OpenSLL version - see comments in the 
// CryptoEngineInitializer.h header file, in the openssl/crypto.h header file and in the
// OpenSSL release notes available online
#if OPENSSL_VERSION_NUMBER < 0x10100000L

  void CryptoEngineInitializer::ssl_lock(int mode, int n, const char *file, int line) throw() // PRQA S 4211 
  {
      try {
	      if (0 != (mode & CRYPTO_LOCK)) { // PRQA S 3003
              try {   
		          /* XXX What should be the reaction for an error occurring here?! */
                  ssl_mutex_[n].lock() ;
  #ifdef ACS_SSL_DEBUG_ENABLED
                  ACS_LOG_PRIVINFO(exDebugSignatureThread << "CRYPTO_LOCK #" << n << " file: " << file << ":" << line);
  #endif
              }
              catch(std::exception &x) {
                  ostringstream os ;
                  os << "Exception got locking mutex # " << n << " of " << ssl_mutex_size
                     << " - called by " << file 
                     << " at line #" << line ;
                  ACS_THROW(CryptoEngineException(x,os.str())) ;
              }
	      }
	      else
	      {
              try {   
		          /* XXX What should be the reaction for an error occurring here?! */
                  ssl_mutex_[n].unlock() ;
  #ifdef ACS_SSL_DEBUG_ENABLED
                  ACS_LOG_PRIVINFO(exDebugSignatureThread << "CRYPTO_UNLOCK #" << n << " file: " << file << ":" << line);
  #endif
              }
              catch(std::exception &x) {
                  ostringstream os ;
                  os << "Exception got unlocking mutex # " << n << " of " << ssl_mutex_size
                     << " - called by " << file 
                     << " at line #" << line ;
                  ACS_THROW(CryptoEngineException(x,os.str())) ;
              }
	      }
      }
      catch(std::exception &x) {
          ACS_LOG_NOTIFY_EX(x) ;
      }
  }

  void CryptoEngineInitializer::ssl_locking_function(int mode, int n, const char *file, int line) throw()
  {
      CryptoEngineInitializer::instance()->ssl_lock(mode, n, file, line) ;
  }

  void CryptoEngineInitializer::ssl_thread_init()
  {
      Lock l_(_M_mutex()) ;
	  ACS_LOG_PRIVINFO("SSL Thread callback initialization");

	  if (0 == ssl_mutex_) {
          try {
              ssl_mutex_size = CRYPTO_num_locks() ; // PRQA S 3000
		      ssl_mutex_ = new ThreadSafe[ ssl_mutex_size ] ; // PRQA S 3000
		  }
          catch(std::exception &x) {
              ostringstream os ;
              os << "Exception got asking for " << ssl_mutex_size ;
              ACS_THROW(CryptoEngineException(x,os.str())) ;
          }
	  }

  // version newer will define all those to nothing

  #if OPENSSL_VERSION_NUMBER >= 0x010000000L
      CRYPTO_THREADID_set_callback(ssl_threadid_function);
  #else
	  CRYPTO_set_id_callback(ssl_threadid_function);
  #endif

	  CRYPTO_set_locking_callback(ssl_locking_function);

	  CRYPTO_set_dynlock_create_callback(ssl_dyn_create_function);
	  CRYPTO_set_dynlock_lock_callback(ssl_dyn_lock_function);
	  CRYPTO_set_dynlock_destroy_callback(ssl_dyn_destroy_function);
  }  

  void CryptoEngineInitializer::checkSSL() 
  {
	   Lock l_(_M_mutex()) ;

  #if OPENSSL_VERSION_NUMBER >= 0x010000000L
      if (CRYPTO_THREADID_get_callback() != ssl_threadid_function) {
  #else
	  if (CRYPTO_get_id_callback() != ssl_threadid_function) {
  #endif
          if (not warntimer_.running() || // if not running (very first time)
              warntimer_.stopElapsed() > _defaultWarningPeriod ) { // every period
		      ACS_LOG_WARNING("SSL Thread callback re-initialization: some other libraries changed it! (libssh2, gsoap, libcurl ...)");
              warntimer_.start() ; // now is running
          }
          ssl_thread_init() ;
      }
  }

  void CryptoEngineInitializer::ssl_thread_destroy() 
  {
      Lock l_(_M_mutex()) ;
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
      if (0 != ssl_mutex_) {

  #if OPENSSL_VERSION_NUMBER >= 0x010000000L
		  CRYPTO_THREADID_set_callback(0);
  #else
		  CRYPTO_set_id_callback(0);
  #endif
    	  CRYPTO_set_locking_callback(0);

    	  CRYPTO_set_dynlock_create_callback(0);
    	  CRYPTO_set_dynlock_lock_callback(0);
    	  CRYPTO_set_dynlock_destroy_callback(0);

          delete [] ssl_mutex_ ;
          ssl_mutex_size = 0 ;
          ssl_mutex_ = 0 ;
      }
  }

  struct CRYPTO_dynlock_value *CryptoEngineInitializer::ssl_dyn_create_function(const char *file, int line) throw() // PRQA S 4020
  {
      try {
	      try {
              CRYPTO_dynlock_value * n = new CRYPTO_dynlock_value ;
  #ifdef ACS_SSL_DEBUG_ENABLED
              ACS_LOG_PRIVINFO(exDebugSignatureThread << "0x" << n << " file: "<< file << ":"<< line);
  #endif
              return n ;
          }
          catch(std::exception &x) {
              ostringstream os ;
              os << "Exception got creating a dyn lock structure"
                 << " - called by " << file 
                 << " at line #" << line ;
              ACS_THROW(CryptoEngineException(x,os.str())) ;
          }
      }
      catch(std::exception &x) {
          ACS_LOG_NOTIFY_EX(x) ;
          return 0 ;
      }
  }

  void CryptoEngineInitializer::ssl_dyn_lock_function(int mode, struct CRYPTO_dynlock_value *dl, const char *file, int line) throw()
  {
      try {
	      if (0 != (mode & CRYPTO_LOCK)) // PRQA S 3003
	      {
              try {   
		          /* XXX What should be the reaction for an error occurring here?! */
                  dl->mutex.lock() ;
  #ifdef ACS_SSL_DEBUG_ENABLED
                 ACS_LOG_PRIVINFO(exDebugSignatureThread << "CRYPTO_LOCK 0x" << dl << " file: "<< file << ":"<< line);
  #endif
              }
              catch(std::exception &x) {
                  ostringstream os ;
                  os << "Exception got locking a dyn lock structure at 0x" << hex << dl
                     << " - called by " << file 
                     << " at line #" << line ;
                  ACS_THROW(CryptoEngineException(x,os.str())) ;
              }
	      }
	      else
	      {
              try {   
		          /* XXX What should be the reaction for an error occurring here?! */
                  dl->mutex.unlock() ;
  #ifdef ACS_SSL_DEBUG_ENABLED
                  ACS_LOG_PRIVINFO(exDebugSignatureThread << "CRYPTO_UNLOCK 0x" << dl << " file: "<< file << ":"<< line);
  #endif
              }
              catch(std::exception &x) {
                  ostringstream os ;
                  os << "Exception got unlocking a dyn lock structure at 0x" << hex << dl
                     << " - called by " << file 
                     << " at line #" << line ;
                  ACS_THROW(CryptoEngineException(x,os.str())) ;
              }
	      }
      }
      catch(std::exception &x) {
          ACS_LOG_NOTIFY_EX(x) ;
      }
  }

  void CryptoEngineInitializer::ssl_dyn_destroy_function(struct CRYPTO_dynlock_value *dl, const char *file, int line)
  {
  #ifdef ACS_SSL_DEBUG_ENABLED
      ACS_LOG_PRIVINFO(exDebugSignatureThread << "0x" << dl << " file: "<< file << ":"<< line);
  #endif
      delete dl ;
  }
#endif // OPENSSL_VERSION_NUMBER < 0x10100000L

_ACS_END_NESTED_NAMESPACE
