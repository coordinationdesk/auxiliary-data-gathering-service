// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.3  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.2  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.2  2013/04/09 08:07:53  marpas
 *  new Statistics namespace
 *
 *  Revision 2.1  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.2  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.1  2011/05/03 14:01:04  micmaz
 *  *** empty log message ***
 *
 *
 *
 *
 */

#ifndef _CurlStatistics_H_
#define _CurlStatistics_H_

#include <RemoteSite.h>
#include <Statistics.h>
#include <curl/curl.h> // PRQA S 1013


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

class CurlStatistics {
public:
    static acs::math::StatNode & findNode(const std::string &path);

    template <class T>  
    static void accumulate(const std::string &path, T t) { findNode(path) += t ; }
    
    template <class T>  
    static void totalAvrg(const std::string &path, T value) { accumulate(path + "/total", value) ; }
    
    static std::string path(const RemoteSite& site, const std::string &operation);
    static void log(const RemoteSite& site, const std::string &operation); // PRQA S 2502
    static void logResult(CURLcode res, const RemoteSite& site, const std::string &operation);
    static acs::math::StatNode& root();
} ;

_ACS_END_NESTED_NAMESPACE

#endif /* _CurlStatistics_H_ */
