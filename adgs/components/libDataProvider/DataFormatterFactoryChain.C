// PRQA S 1050 EOF
/*

 Copyright 1995-2013, Advanced Computer Systems , Inc.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.acsys.it

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Advanced Computer Systems, Inc.

 $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author$

 $Log$
 Revision 5.1  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.0  2013/06/06 18:51:19  marpas
 adopting libException 5.x standards

*/


#include "DataFormatterFactoryChain.h"

#include <Filterables.h>
#include <vector>
#include <algorithm>
#include <sstream>


_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL (DataFormatterFactoryChain);

DataFormatterFactoryChain::DataFormatterFactoryChain() :
        _factories() {
    //empty
}

DataFormatterFactoryChain::~DataFormatterFactoryChain() throw () {
    //empty
}
void DataFormatterFactoryChain::registerFactory(DataFormatterFactory* factory) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Registering a new factory " );
    _factories.push_back(factory);
}

void DataFormatterFactoryChain::unregisterFactory(DataFormatterFactory* factory) throw() 
{
    vector<DataFormatterFactory*>::iterator found = find(_factories.begin(), _factories.end(), factory);
    if (found != _factories.end()) {
        _factories.erase(found);
    }
}

DataFormatter* DataFormatterFactoryChain::newFormatter(string const& algo) // PRQA S 4020
{

    size_t fsize = _factories.size() ;
    for (size_t i = 0; i < fsize; ++i) {
        DataFormatter *formatter = _factories[i]->newFormatter(algo) ;
        if (formatter != 0) {
            return formatter ;
        }
    }
    return 0 ;;
}

string DataFormatterFactoryChain::capabilities() const {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"[capabilities()]");
    vector < string > cap;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"number of factories installed: " << _factories.size());
    size_t fsize = _factories.size() ;
    for (size_t i = 0; i < fsize; ++i) {
        _factories[i]->capabilities(&cap);
    }
    ostringstream stream;
    size_t capsize = cap.size() ;
    for (size_t i = 0; i < capsize; ++i) {
        stream << cap[i];
    }
    return stream.str();
}


_ACS_END_NESTED_NAMESPACE
