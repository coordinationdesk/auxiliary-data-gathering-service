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
//    $Prod: A.C.S. CRC_Digest_sha1 HEADER $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 1.1  2010/12/16 13:08:11  enrcar
//    EC:: Imported source file
//
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _CRC_Digest_sha1_H_
#define _CRC_Digest_sha1_H_
 
#include <CRC_Digest_base.h>

namespace acs {

    class CRC_Digest_sha1 : public CRC_Digest_base
    {
        public :
	        CRC_Digest_sha1() noexcept : CRC_Digest_base::CRC_Digest_base( 1024, EVP_sha1(), NULL ) {} ;
	        virtual std::string getDigestName() { return "sha1"; }

    } ;

} // Close namespace

#endif	// _CRC_Digest_sha1_H_
