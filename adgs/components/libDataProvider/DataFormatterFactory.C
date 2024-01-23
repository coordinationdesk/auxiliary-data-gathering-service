// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2012, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. libDataProvider Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.1  2013/11/04 13:56:31  marpas
 *  coding best practices applied
 *  robustness improved
 *  performances improved
 *  compilation & qa warnings removed
 *
 *  Revision 5.0  2013/06/06 18:51:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.4  2013/01/25 13:34:27  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.3  2012/10/19 15:04:06  micmaz
 *  updated library
 *
 *  Revision 1.2  2012/10/08 13:59:19  micmaz
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 1.1  2012/09/14 16:31:32  micmaz
 *  S2GPP-611
 *  Development of Distribution Formatter framework for Export
 *
 *
 */

#include "DataFormatter.h"
#include <DataFormatterFactoryChain.h>
#include <SingleNodeFtpFormatter.h>
#include <S2GppMediaFormatter.h>
#include <StringUtils.h>
#include <Filterables.h>

#include <sstream>

using namespace std;

_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)

std::string const DataFormatterFactory::singleNodesFtp = "single_nodes_ftp";
std::string const DataFormatterFactory::s2gppMedia = "s2gpp_media";

DataFormatterFactory::DataFormatterFactory() {
    registerToChain();
}

DataFormatterFactory::~DataFormatterFactory() throw () {
    unregisterFromChain();
}

void DataFormatterFactory::registerToChain() 
{
    DataFormatterProvider::instance()->registerFactory(this);
}

void DataFormatterFactory::unregisterFromChain() throw() 
{
    DataFormatterProvider::instance()->unregisterFactory(this);
}

DataFormatter* DataFormatterFactory::newFormatter(std::string const& algo) { // PRQA S 4020
    if (StringUtils::equalsNoCase(algo, singleNodesFtp)) {
        return new SingleNodeFtpFormatter();
    }
    if (StringUtils::equalsNoCase(algo, s2gppMedia)) {
        return new S2GppMediaFormatter();
    }
    return 0;
}
void DataFormatterFactory::capabilities(std::vector<std::string>* capabilities) const {
    {
        ostringstream stream;
        stream << "<Formatter>" << singleNodesFtp << "</Formatter>";
        capabilities->push_back(stream.str());
    }
    {
        ostringstream stream;
        stream << "<Formatter>" << s2gppMedia << "</Formatter>";
        capabilities->push_back(stream.str());
    }
}

namespace {
    class DataFormatterFactoryLoader {
    public:
        DataFormatterFactoryLoader() {
            DataFormatterFactorySingleton::instance();
        }
    } _DataFormatterFactoryLoader;
}

_ACS_END_NESTED_NAMESPACE
