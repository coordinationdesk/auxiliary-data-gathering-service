// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S. libFtpCurl  $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.2  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *
 *
 *
 */

#ifndef _FtpCurlConstants_H_
#define _FtpCurlConstants_H_

#include <acs_c++config.hpp>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

struct Constants {
    static const long Kilobyte = 1024L ;
    static const long Megabyte = 1048576L ;
    static const long Gigabyte = 1073741824L ;
    static const long long Terabyte = 1099511627776LL ;
} ;

_ACS_END_NESTED_NAMESPACE 

#endif /* _FtpCurlConstants_H_ */
