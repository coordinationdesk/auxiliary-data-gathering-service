// PRQA S 1050 EOF
/*
	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

*/ 

#include <StorageFileNamingAlgoSingleton.h>

#include <dbPersistent.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(StorageFileNamingAlgoSingleton);


StorageFileNamingAlgoSingleton::StorageFileNamingAlgoSingleton():
	pattern::Singleton<StorageFileNamingAlgoSingleton> (),
    _map() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Instance now.");
}

StorageFileNamingAlgoSingleton::~StorageFileNamingAlgoSingleton() throw() { 

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;// PRQA S 4631
}


void StorageFileNamingAlgoSingleton::registerFactory(string const & key, StorageFileNamingAlgo* (*builder)(const dbPersistent *) )
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< builder<<"\".");

	map<string, StorageFileNamingAlgo* (*)(const dbPersistent *)>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, StorageFileNamingAlgo* (*)(const dbPersistent *) >(key,builder) );
	}
}


void StorageFileNamingAlgoSingleton::unregisterFactory(string const & key, StorageFileNamingAlgo* (*builder)(const dbPersistent *) ) // PRQA S 4020
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< builder<<"\".");

	map<string, StorageFileNamingAlgo* (*)(const dbPersistent *)>::iterator it = _map.find(key);
	if(it == _map.end() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is not present.");
		return ; //the builder is not present.
	}
	_map.erase(it);

}


StorageFileNamingAlgo *StorageFileNamingAlgoSingleton::newStorageFileNamingAlgo(string const & key, const dbPersistent * o) const
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Input key is \"" << key << "\".");
	
	//Build the Valid key to return the expected pointer
	map<string, StorageFileNamingAlgo* (*)(const dbPersistent *)>::const_iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("key \""+key+"\" is unknown.")); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input key is \"" << key << "\". builder address is \""<< it->second<<"\".");
	
	return ( *(it->second ) )(o);
}


_ACS_END_NAMESPACE
