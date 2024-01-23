//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 1995-2015, Advanced Computer Systems , Inc.
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
//    $Prod: A.C.S. CRC_DigestTreeTree_sha1 HEADER $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 5.1  2015/09/22 16:11:28  lucio.pulvirenti
//    EC:: LP:: Added support for computing GIT-style Digest (MD5/SHA1) for files and directories
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _DigestTree_sha1_H_
#define _DigestTree_sha1_H_
 
#include <acs_c++config.hpp>
#include <DigestTree_base.h>
#include <CRC_Digest_sha1.h>


_ACS_BEGIN_NAMESPACE(acs)

class DigestTree_sha1 : public DigestTree_base
{

public :
	DigestTree_sha1() throw() : DigestTree_base(), _the_digest() { setDigest(&_the_digest) ; } ; 
    virtual ~DigestTree_sha1() throw() {} ;

	virtual size_t getHashSize() { return (160/8); /* 160 bits = 20 bytes */ } 

private:
    DigestTree_sha1(const DigestTree_sha1 &) ; // not implemented
    DigestTree_sha1& operator=(const DigestTree_sha1 &) ; // not implemented

private:
	CRC_Digest_sha1 _the_digest ;
	
} ;

_ACS_END_NAMESPACE
 
#endif	// _DigestTree_sha1_H_
