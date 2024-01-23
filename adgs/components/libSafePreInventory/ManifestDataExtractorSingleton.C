// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
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
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	
	
*/ 

#include <ManifestDataExtractorSingleton.h>

#include <Filterables.h>


using namespace acs;
using namespace std;



ACS_CLASS_DEFINE_DEBUG_LEVEL(ManifestDataExtractorSingleton);


ManifestDataExtractorSingleton *ManifestDataExtractorSingleton::_instance = 0;
ThreadSafe ManifestDataExtractorSingleton::_mutex;

ManifestDataExtractorSingleton* ManifestDataExtractorSingleton::instance()
{
	ThreadSafe::Lock guard(_mutex);
	if(_instance == 0)
	{
       _instance=new ManifestDataExtractorSingleton;
#ifdef RELEASE_SINGLETON_AT_EXIT
		atexit(release) ;
#endif

	}
	return _instance;
}


void ManifestDataExtractorSingleton::registerFactory(string const & key, ManifestDataExtractor* (*builder)() )
{
	ThreadSafe::Lock guard(_mutex);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< reinterpret_cast<void*>(builder)<<"\".");

	map<string, ManifestDataExtractor* (*)()>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, ManifestDataExtractor* (*)() >(key,builder) );
	}
}


void ManifestDataExtractorSingleton::unregisterFactory(string const & key, ManifestDataExtractor* (*builder)() )
{
	ThreadSafe::Lock guard(_mutex);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< reinterpret_cast<void*>(builder)<<"\".");

	map<string, ManifestDataExtractor* (*)()>::iterator it = _map.find(key);
	if(it == _map.end() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is not present.");
		return ; //the builder is not present.
	}
	_map.erase(it);

}


ManifestDataExtractor *ManifestDataExtractorSingleton::newManifestDataExtractor(string const & key) const
{
	ThreadSafe::Lock guard(_mutex);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Input key is \"" << key << "\".");
	
	//Build the Valid key to return the expected pointer
	map<string, ManifestDataExtractor* (*)()>::const_iterator it= _map.find(key);
	if(it == _map.end() )
	{
		string error;
		error=string("key \"")+key+string("\" is unknown.");
		throwException(KeyNotFound(error));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input key is \"" << key << "\". builder address is \""<< reinterpret_cast<void*>(it->second)<<"\".");
	
	return ( *(it->second ) )();
}

ManifestDataExtractorSingleton::ManifestDataExtractorSingleton():
							_map() 
{
  	ThreadSafe::Lock guard(_mutex);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Instance now.");
}

void ManifestDataExtractorSingleton::release() 
{
	if (_instance)
		delete _instance ;
	_instance = 0 ;
}
