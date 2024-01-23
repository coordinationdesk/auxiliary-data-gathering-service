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
 *  Revision 5.3  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.2  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.4  2013/04/09 08:07:53  marpas
 *  new Statistics namespace
 *
 *  Revision 2.3  2012/12/03 16:56:48  marpas
 *  using new StringUtils interface
 *
 *  Revision 2.2  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.1  2012/01/20 19:32:09  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
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

#include <CurlStatistics.h>
#include <StringUtils.h>
#include <boost/algorithm/string/split.hpp> // PRQA S 1013 2
#include <boost/algorithm/string/classification.hpp>
#include <vector>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

using namespace std;

using acs::math::StatNode ;

StatNode & CurlStatistics::findNode(const string &path) {
    StatNode *node = &(*acs::math::Statistics::instance())();
    vector < string > strs;
    boost::split(strs, path, boost::is_any_of("/"));
    size_t st_size = strs.size() ;
    for (size_t counter = 0; counter < st_size; ++counter) {
        if (strs[counter].size() > 0) {
            string value = acs::StringUtils::transform(strs[counter], "&#47;", "/");
            value = acs::StringUtils::transform(value, "&slash;", "/");
            node = &(node->operator[](value));
        }
    }
    return *node ;
}

string CurlStatistics::path(const RemoteSite& site, const string &operation) 
{
    return "curl/" + site.getProtocol() + "/" + site.getHost() + "/" + operation ;
}

void CurlStatistics::log(const RemoteSite& site, const string &operation) 
{
    accumulate(path(site, operation),1);
}


void CurlStatistics::logResult(CURLcode res, const RemoteSite& site, const string &operation) {
    string osPath = path(site, operation);
    osPath += "/";
    if (res == CURLE_OK) {
        osPath += "CURLE_OK";
    } 
    else {
        osPath += "CURLE_ERROR";
    }
    accumulate(osPath,1);
}

StatNode& CurlStatistics::root() {
    return (*acs::math::Statistics::instance())();
}


_ACS_END_NESTED_NAMESPACE
