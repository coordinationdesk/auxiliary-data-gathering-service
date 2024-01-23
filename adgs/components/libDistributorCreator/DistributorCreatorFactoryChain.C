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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/13 18:26:27  marpas
	coding best practices applied
	qa rules enforced
	new namespace model
	
	Revision 2.2  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	Revision 2.1  2013/01/25 12:16:43  marpas
	using pattern::Singleton from libException
	
	Revision 2.0  2013/01/21 17:52:32  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.6  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.5  2012/03/08 11:18:27  marpas
	refactoring
	
	Revision 1.4  2012/03/08 10:12:14  marpas
	capabilities messages with fewer newlines
	
	Revision 1.3  2011/03/01 17:28:04  marpas
	GCC 4.4.x support
	
	Revision 1.2  2007/03/15 15:04:15  crivig
	coding and compiling...
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
*/



#include <DistributorCreatorFactoryChain.h>
#include <DistributorCreatorFactory.h>

#include <algorithm>
#include <cstdlib>

#include <dgenV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ; 
ACS_CLASS_DEFINE_DEBUG_LEVEL(DistributorCreatorFactoryChain) ;

namespace {
    dgenV version;
}

DistributorCreatorFactoryChain::DistributorCreatorFactoryChain() : 
    Singleton<DistributorCreatorFactoryChain >(),
    _factories()
{
}

DistributorCreatorFactoryChain::~DistributorCreatorFactoryChain() throw() 
{
}

void DistributorCreatorFactoryChain::registerFactory(DistributorCreatorFactory *f)
{
	_factories.push_back(f) ;
}

void DistributorCreatorFactoryChain::unregisterFactory(DistributorCreatorFactory *f)
{
	vector<DistributorCreatorFactory *>::iterator tbd = find(_factories.begin(), _factories.end(), f) ;
	if (tbd != _factories.end()) {
		_factories.erase(tbd) ;
    }
}

DistributorCreator *DistributorCreatorFactoryChain::newCreator(const string &algo)  // PRQA S 4020
{
	DistributorCreator *oc = 0;
	for (size_t i=0; i < _factories.size(); ++i) { // PRQA S 4238, 4244
		if ((oc = _factories[i]->newCreator(algo)))  {
			return oc ;
        }
    }
	return 0 ;
}

string DistributorCreatorFactoryChain::capabilities() const 
{
	vector <string> cap ;
	for (size_t i=0; i < _factories.size(); ++i) { // PRQA S 4238, 4244
		_factories[i]->capabilities(cap) ;
    }
	string ret ;
	for (unsigned int i = 0; i < cap.size(); ++i) { // PRQA S 4238, 4244
		ret += cap[i] ;
	}

	return ret ;
}


_ACS_END_NAMESPACE

