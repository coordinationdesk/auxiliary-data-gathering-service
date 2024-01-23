//////////////////////////////////////////////////////////////////////////////////////
//
/*
    Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
*/
//
//    $Prod: A.C.S. CRC_Digest_md5 HEADER $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 2.2  2012/02/07 17:54:56  marpas
//    refactoring
//
//    Revision 2.1  2006/03/08 15:47:58  ivafam
//    Added in PDS Rose Model
//
//    Revision 2.0  2006/02/28 08:31:43  marpas
//    Exception 2.1 I/F adopted
//
//    Revision 1.3  2006/01/10 10:31:24  ivafam
//    Interface unified between older and newer libssl
//
//    Revision 1.2  2006/01/10 09:03:09  enrcar
//    ACS_LIBSSL_PRE_0_9_7 code added to handle libssl < 0.9.7
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _CRC_Digest_md5_H_
#define _CRC_Digest_md5_H_
 
#include <acs_c++config.hpp>
#include <CRC_Digest_base.h>

_ACS_BEGIN_NAMESPACE(acs)

class CRC_Digest_md5 : public CRC_Digest_base
{
public :
	CRC_Digest_md5() noexcept : CRC_Digest_base( 1024, EVP_md5(), 0 ) {} ; // PRQA S 4400
    virtual ~CRC_Digest_md5() noexcept {} ; // PRQA S 2131

    virtual std::string getDigestName() { return "md5"; } // PRQA S 2131

private:
    CRC_Digest_md5(const CRC_Digest_md5 &) ; // not implemented
    CRC_Digest_md5& operator=(const CRC_Digest_md5 &) ; // not implemented
} ;

_ACS_END_NAMESPACE
 
#endif	// _CRC_Digest_md5_H_
