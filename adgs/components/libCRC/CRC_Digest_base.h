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
//    $Prod: A.C.S. CRC_Digest_base HEADER $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 2.6  2013/06/11 08:46:18  marpas
//    useless debug removed
//
//    Revision 2.5  2012/11/15 12:24:21  marpas
//    qa rules
//
//    Revision 2.4  2012/11/15 11:53:47  marpas
//    qa rules
//
//    Revision 2.3  2012/02/07 17:54:56  marpas
//    refactoring
//
//    Revision 2.2  2012/02/07 10:22:56  marpas
//    removing compiler warnings
//
//    Revision 2.1  2006/03/08 15:47:58  ivafam
//    Added in PDS Rose Model
//
//    Revision 2.0  2006/02/28 08:31:43  marpas
//    Exception 2.1 I/F adopted
//
//    Revision 1.4  2006/01/10 10:31:24  ivafam
//    Interface unified between older and newer libssl
//
//    Revision 1.3  2006/01/10 09:03:09  enrcar
//    ACS_LIBSSL_PRE_0_9_7 code added to handle libssl < 0.9.7
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _CRC_Digest_base_H_
#define _CRC_Digest_base_H_


#ifdef COSMO
    #define ACS_LIBSSL_PRE_0_9_7 
    #warning "Using libssl pre 0.9.7. Engines unsupported."
#endif


#include <acs_c++config.hpp>
#include <exException.h>
#include <openssl/evp.h>    // PRQA S 1013
#include <string>



_ACS_BEGIN_NAMESPACE(acs)

const int ACS_CRC_DIGEST_SUCCESS = 1 ;

typedef unsigned char ACS_CRC_Digest_Retbuf[EVP_MAX_MD_SIZE] ; // PRQA S 2411

class CRC_Digest_base // PRQA S 2109
{
public:

	/*! class ACS_CRC_DigestError declaration. */
	exDECLARE_EXCEPTION(ACS_CRC_DigestError,exException) ; // PRQA S 2131, 2502

   	CRC_Digest_base(size_t blkSize, const EVP_MD* digest, void* engine=0) ;
	virtual ~CRC_Digest_base() throw() ;

	void addBuffer(const unsigned char* buffer, size_t buflen) ;
	std::string getResult(ACS_CRC_Digest_Retbuf retbuf_IN=0, unsigned int* s=0, bool validity=false) ;
	void resetContext() ;
	size_t expectedSize() const ;
	virtual std::string getDigestName() =0;

private:
    CRC_Digest_base() ; // not implemented
    CRC_Digest_base(const CRC_Digest_base &) ; // not implemented
    CRC_Digest_base & operator= (const CRC_Digest_base &) ; // not implemented

private:
	EVP_MD_CTX* 	_ctx ;
	size_t			_blkSize ;
	const EVP_MD* 	_digest ;
	void*           _engine ; /* to ensure compatibily between older and newer libssl */
	bool 			_valid ;  
} ;

_ACS_END_NAMESPACE
 
#endif	// _CRC_Digest_base_H_
