// PRQA S 1050 EOF
//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 1995-2016, Advanced Computer Systems , Inc.
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
//    $Prod: A.C.S. DigestTree_base HEADER $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 5.6  2016/09/29 13:24:37  enrcar
//    EC:: Warning in compilation removed.
//
//    Revision 5.5  2016/03/16 17:02:45  marpas
//    coding best practices applied
//    qa warnings removed
//
//    Revision 5.4  2016/03/08 14:59:13  marpas
//    removing mutex in File class
//    This was originally due to some system call thread safety issues (e.g. readdir)
//    The latter was explicitely changed into readdir_r call
//    Other syscall were checked and up to now seems there is none having thread safety problems
//
//    Revision 5.3  2015/10/14 13:38:20  enrcar
//    EC:: Bug fixed: an ostringstream was used to keep the binary hash, filled with operator<<. It turned out that this resulted in unspecified values if the hash contained a binary 0 (binary 0 is interpreted as end-of-string and the copy of buffer is stopped). Now a C-style buffer is used instead of ostringstream.
//
//    Revision 5.2  2015/10/02 09:44:52  enrcar
//    EC:: when computing the digest of a directory, the permissions of each file it contains are no more enforced by default. A mandatory parameter (check/don't check) must be provided.
//
//    Revision 5.1  2015/09/22 16:11:27  lucio.pulvirenti
//    EC:: LP:: Added support for computing GIT-style Digest (MD5/SHA1) for files and directories
//
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _DigestTree_base_H_
#define _DigestTree_base_H_


#include <acs_c++config.hpp>
#include <CRC_Digest_base.h>
#include <exException.h>
#include <openssl/evp.h>  // PRQA S 1013
#include <string>



_ACS_BEGIN_NAMESPACE(acs)


class DigestTree_base // PRQA S 2109
{
public:

	/*! class ACS_CRC_DigestTreeError declaration. */
	exDECLARE_EXCEPTION(ACS_CRC_DigestTreeError,exException) ; // PRQA S 2131, 2502

   	DigestTree_base() ;
   	explicit DigestTree_base(CRC_Digest_base*) ;
	virtual ~DigestTree_base() throw() ;

	void setObject(const std::string&, bool ) ;	
	
	void setDigest(CRC_Digest_base* digestP) { _digestP = digestP ; } 
	
	std::string getObjectID(ACS_CRC_Digest_Retbuf retbuf=0, unsigned int* s=0, bool validity=false) ;

	std::string getDigestName() ;
	
	static std::string bufferDump(unsigned char* p, const size_t s) ;
	
	virtual size_t getHashSize()=0 ;
	
private:
    DigestTree_base(const DigestTree_base &) ; // not implemented
    DigestTree_base & operator= (const DigestTree_base &) ; // not implemented

private:
	void hashFile(const std::string& , bool resetContext=true) ;
	void getBlobID(const std::string& ) ;
	void getTreeID(const std::string& , bool check_perm) ;

private:
	CRC_Digest_base* _digestP ; 	
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DigestTree_base) ;
} ;

_ACS_END_NAMESPACE
 
#endif	// _DigestTree_base_H_
