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

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.4  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.3  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.2  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.1  2011/03/01 16:06:55  marpas
	GCC 4.4.x support
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/10/14 09:01:02  marpas
	Strategy interface changed - there are cases where the strategy could update the inventoryobject itself
	
	Revision 1.1  2005/08/05 07:02:59  ivafam
	Added classes for packing strategy management
	
	
*/


#include <SMPackingStrategyFactoryChain.h>
#include <SMPackingStrategyFactory.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



SMPackingStrategyFactoryChain::SMPackingStrategyFactoryChain() :
    pattern::Singleton<SMPackingStrategyFactoryChain>(),
    _factories()
{
}


SMPackingStrategyFactoryChain::~SMPackingStrategyFactoryChain() throw() { }


void SMPackingStrategyFactoryChain::registerFactory(SMPackingStrategyFactory *f)
{
	_factories.insert(_factories.begin(),f) ;
}

void SMPackingStrategyFactoryChain::unregisterFactory(SMPackingStrategyFactory *f) throw() 
{
	
	vector<SMPackingStrategyFactory *>::iterator tbd ;
	while( (tbd = find(_factories.begin(), _factories.end(), f)) != _factories.end()) {
		_factories.erase(tbd) ;
    }
}


SMPackingStrategy *SMPackingStrategyFactoryChain::getStrategy(const std::string &strategy, dbPersistent & obj, dbConnectionPool &p)  // PRQA S 4020
{
	SMPackingStrategy *oc = 0 ;
    size_t fsize = _factories.size() ;
	for (size_t i=0; i < fsize; ++i) {
		if ((oc = _factories[i]->getStrategy(strategy,obj, p))) {
			return oc ;
        }
    }
	return 0 ;
	
}

_ACS_END_NAMESPACE

