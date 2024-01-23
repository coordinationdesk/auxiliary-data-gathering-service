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
//    $Prod: A.C.S. CRC_DigestTreeTree_md5 HEADER $
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

#ifndef _DigestTree_md5_H_
#define _DigestTree_md5_H_
 
#include <acs_c++config.hpp>
#include <DigestTree_base.h>
#include <CRC_Digest_md5.h>


_ACS_BEGIN_NAMESPACE(acs)

class DigestTree_md5 : public DigestTree_base
{

public :
	DigestTree_md5() throw() : DigestTree_base(), _the_digest() { setDigest(&_the_digest) ; } ; 
    virtual ~DigestTree_md5() throw() {} ;

	virtual size_t getHashSize() { return (128/8); /* 128 bits = 16 bytes */ } 

private:
    DigestTree_md5(const DigestTree_md5 &) ; // not implemented
    DigestTree_md5& operator=(const DigestTree_md5 &) ; // not implemented

private:
	CRC_Digest_md5 _the_digest ;
	
} ;

_ACS_END_NAMESPACE
 
#endif	// _DigestTree_md5_H_
