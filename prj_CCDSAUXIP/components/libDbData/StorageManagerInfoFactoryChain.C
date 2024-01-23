// PRQA S 1050 EOF
/*
	Copyright 2017-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2017/11/07 16:18:07  lucio.pulvirenti
	S2PDGS-1843: classes added.
	
	
*/


#include <StorageManagerInfoFactoryChain.h>
#include <StorageManagerInfoFactory.h>
#include <algorithm>

#include <cstdlib>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(StorageManagerInfoFactoryChain)

StorageManagerInfoFactoryChain::StorageManagerInfoFactoryChain() : 
    Singleton<StorageManagerInfoFactoryChain >(),
    _factories()
{
}

StorageManagerInfoFactoryChain::~StorageManagerInfoFactoryChain() noexcept {}


void StorageManagerInfoFactoryChain::registerFactory(StorageManagerInfoFactory *f)
{
	_factories.insert(_factories.begin(),f) ;
}

void StorageManagerInfoFactoryChain::unregisterFactory(StorageManagerInfoFactory *f) noexcept 
{
	
	vector<StorageManagerInfoFactory *>::iterator tbd ;
	while( (tbd = find(_factories.begin(), _factories.end(), f)) != _factories.end()) {
		_factories.erase(tbd) ;
    }
}

StorageManagerInfo *StorageManagerInfoFactoryChain::newStorageManagerInfo(dbConnectionPool &pool) // PRQA S 4020 
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    StorageManagerInfo *smi = _factories[i]->newStorageManagerInfo(pool) ;
		if (0 != smi) {
			return smi ;
        }
    }
	return 0 ;
}

string StorageManagerInfoFactoryChain::capabilities() const 
{
	string cap ;
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
		cap += _factories[i]->capabilities() ;
    }
	return cap ;
}


_ACS_END_NAMESPACE
