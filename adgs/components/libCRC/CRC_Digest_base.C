// PRQA S 1050 EOF
//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 1995-2013, Advanced Computer Systems , Inc.
//    Via Della Bufalotta, 378 - 00139 Roma - Italy
//    http://www.acsys.it
//
//    All Rights Reserved.
//
//    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
//    the contents of this file may not be disclosed to third parties, copied or
//    duplicated in any form, in whole or in part, without the prior written
//    permission of Advanced Computer Systems, Inc.
//
//    $Prod: A.C.S. CRC_Digest_base $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 2.9  2013/10/18 11:47:59  marpas
//    using namespaces
//
//    Revision 2.8  2013/06/11 08:46:18  marpas
//    useless debug removed
//
//    Revision 2.7  2013/02/26 14:34:27  enrcar
//    EC:: warning removed
//
//    Revision 2.6  2012/11/15 12:24:21  marpas
//    qa rules
//
//    Revision 2.5  2012/11/15 11:53:47  marpas
//    qa rules
//
//    Revision 2.4  2012/02/09 14:11:38  marpas
//    deprecated macro removed
//
//    Revision 2.3  2012/02/07 17:54:56  marpas
//    refactoring
//
//    Revision 2.2  2012/02/07 10:22:56  marpas
//    removing compiler warnings
//
//    Revision 2.1  2011/03/01 11:17:28  marpas
//    GCC 4.4.x support
//
//    Revision 2.0  2006/02/28 08:31:43  marpas
//    Exception 2.1 I/F adopted
//
//    Revision 1.4  2006/01/10 10:31:24  ivafam
//    Interface unified between older and newer libssl
//
//    Revision 1.3  2006/01/10 09:41:38  enrcar
//    version (tag) handling added
//
//    Revision 1.2  2006/01/10 09:03:09  enrcar
//    ACS_LIBSSL_PRE_0_9_7 code added to handle libssl < 0.9.7
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#include <CRC_Digest_base.h>
#include <cstring>
#include <crcV.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace { // unnamed
    crcV version ;
}


CRC_Digest_base::CRC_Digest_base(size_t blkSize, const EVP_MD* digest, void* engine):
_ctx(0), _blkSize(blkSize), _digest(digest), _engine(engine), _valid(false)
{
	ACS_COND_THROW(0 == _digest,ACS_CRC_DigestError("CRC_Digest_base::CRC_Digest_base(): invalid digest")) ;  // PRQA S 3081

	ACS_COND_THROW(_blkSize < 1,ACS_CRC_DigestError("CRC_Digest_base::CRC_Digest_base(): invalid blkSize")) ;  // PRQA S 3081

	// Create and allocate Context-Handler

#if OPENSSL_VERSION_NUMBER < 0x10100000L

#ifndef ACS_LIBSSL_PRE_0_9_7 
    _ctx = EVP_MD_CTX_create() ;    // initialize CTX. REQUIRES EVP_MD_CTX_destroy
#else
    _ctx = new EVP_MD_CTX ;         // Just allocate CTX. 
#endif

#else
    _ctx = EVP_MD_CTX_new() ;    	// initialize CTX. REQUIRES EVP_MD_CTX_free
#endif


	ACS_COND_THROW(0 == _ctx,ACS_CRC_DigestError("CRC_Digest_base::CRC_Digest_base(): Cannot allocate context-handler")) ; // PRQA S 3081

	resetContext() ; // Also, set _valid as true
}


CRC_Digest_base::~CRC_Digest_base() throw()
{

 	if (_ctx)
    {
	
#if OPENSSL_VERSION_NUMBER < 0x10100000L

#ifndef ACS_LIBSSL_PRE_0_9_7 
        EVP_MD_CTX_destroy(_ctx); // Since a EVP_MD_CTX_create was used. // PRQA S 4631
#else
        delete _ctx ; // Since "new" was used.    // PRQA S 4631
#endif

#else
        EVP_MD_CTX_free(_ctx); // Since a EVP_MD_CTX_new was used. // PRQA S 4631
#endif
	_ctx = 0 ;
    }
}


size_t CRC_Digest_base::expectedSize() const { return _blkSize; } // PRQA S 4120


void CRC_Digest_base::resetContext()
{
	ACS_COND_THROW(0 == _ctx,ACS_CRC_DigestError("CRC_Digest_base::resetContext(): CTX unallocated.")) ; // PRQA S 3081

    #ifndef ACS_LIBSSL_PRE_0_9_7 
        int status = EVP_DigestInit_ex(_ctx, _digest, reinterpret_cast<ENGINE*>(_engine)); // PRQA S 3081
        if (status != ACS_CRC_DIGEST_SUCCESS)
        {
            ostringstream o ;
            o << "CRC_Digest_base::resetContext(): Error during EVP_DigestInit_ex: " << status ;
            ACS_THROW(ACS_CRC_DigestError(o.str())); // PRQA S 3081
        }
    #else
        EVP_DigestInit(_ctx, _digest) ; // void
    #endif
  
	_valid = true ;
}


void CRC_Digest_base::addBuffer(const unsigned char* buffer, size_t buflen) // PRQA S 4211
{
	if (!_valid)
	{
        ostringstream o ;
        o << "CRC_Digest_base::addBuffer(): Digest in Invalid state" ;
		ACS_THROW(ACS_CRC_DigestError(o.str())); // PRQA S 3081
    }
    
#ifndef ACS_LIBSSL_PRE_0_9_7 
    int status = EVP_DigestUpdate(_ctx, buffer, buflen);
    if (status != ACS_CRC_DIGEST_SUCCESS)
    {
        ostringstream o ;
        o << "CRC_Digest_base::addBuffer(): Error during EVP_DigestUpdate: " << status ;
		ACS_THROW(ACS_CRC_DigestError(o.str())); // PRQA S 3081
    }
#else
    EVP_DigestUpdate(_ctx, buffer, buflen); // void
#endif
}


string CRC_Digest_base::getResult(ACS_CRC_Digest_Retbuf retbuf_IN, unsigned int* s, bool validity)
{
	//
	//	In order to call this function several times, a local copy of digest if performed
	//	A flag is set in order to avoid CALL again addBuffer (to easily discover bugs).
	// 	Usually flag is FALSE (to make status invalid) but can be set TRUE
	//
	unsigned char retbuf[EVP_MAX_MD_SIZE]  ;
	int status ; 
	unsigned int r = 0 ;
	
#if OPENSSL_VERSION_NUMBER < 0x10000000L

	// For openssl 0.x

	EVP_MD_CTX ctx_copy ;   // create temporary object  // PRQA S 4102
  
    // create local digest copy:
    status =  EVP_MD_CTX_copy(&ctx_copy, _ctx); // no init required 
	if (status != ACS_CRC_DIGEST_SUCCESS)
	{
        ostringstream o ;
        o << "CRC_Digest_base::getResult(): Cannot copy digest: " << status ;
		ACS_THROW(ACS_CRC_DigestError(o.str())); // PRQA S 3081
    }
	_valid = validity ; // Set validity (standard behaviour is to make it invalid it)
	
#ifndef ACS_LIBSSL_PRE_0_9_7 
    status = EVP_DigestFinal(&ctx_copy, retbuf, &r); 
 	// Check status
 	if (status != ACS_CRC_DIGEST_SUCCESS)
	{
        ostringstream o ;
        o << "CRC_Digest_base::getResult(): Error during EVP_DigestFinal: " << status ;
		ACS_THROW(ACS_CRC_DigestError(o.str())); // PRQA S 3081
    }
#else
    EVP_DigestFinal(&ctx_copy, retbuf, &r);  // void
#endif

#else	
	// For openssl 1.0.0 or greater

    EVP_MD_CTX *ctx_copy = 0 ;
    try { 
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        ctx_copy = EVP_MD_CTX_create() ; 
#else
		// For openssl 1.1.0 or greater
        ctx_copy = EVP_MD_CTX_new() ; 
#endif
        // create local digest copy:
        status =  EVP_MD_CTX_copy(ctx_copy, _ctx); // no init required 
	    if (status != ACS_CRC_DIGEST_SUCCESS)
	    {
            ostringstream o ;
            o << "CRC_Digest_base::getResult(): Cannot copy digest: " << status ;
		    ACS_THROW(ACS_CRC_DigestError(o.str())); // PRQA S 3081
        }
	    _valid = validity ; // Set validity (standard behaviour is to make it invalid it)

        EVP_DigestFinal(ctx_copy, retbuf, &r);  // void
    }
    catch(exception &) {
        if (0 != ctx_copy) {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
            EVP_MD_CTX_destroy(ctx_copy) ;
#else
			// For openssl 1.1.0 or greater
            EVP_MD_CTX_free(ctx_copy) ;
#endif
			ctx_copy = 0 ; 
        }
	    throw ;
    }

    if (0 != ctx_copy) {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        EVP_MD_CTX_destroy(ctx_copy) ;
#else
		// For openssl 1.1.0 or greater
        EVP_MD_CTX_free(ctx_copy) ;
#endif
		ctx_copy = 0 ; 
    }

#endif	

    if (r < EVP_MAX_MD_SIZE) { memset(retbuf+r, 0, EVP_MAX_MD_SIZE-r) ; } // avoid garbage // PRQA S 3084
	if (retbuf_IN) { memcpy(retbuf_IN, retbuf, EVP_MAX_MD_SIZE); } // return buffer if pointer not-null
	if (s) { (*s)=r ; } // return length if pointer not-null

	// binary-to-ascii conversion
	ostringstream o ; 
	for ( ssize_t i=0; i<r; ++i ) // PRQA S 3084, 4400, 4107
	{
		o << hex << setfill('0') << setw(2) << static_cast<int>(retbuf[i]) ;
	}
		
	return o.str() ; // return C++ string
}

_ACS_END_NAMESPACE
 
