// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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

	$log: $

*/

#include <AuxFileParserSingleton.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AuxFileParserSingleton);


void AuxFileParserSingleton::registerFactory(string const & key, AuxFileParser* (*builder)() )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\".") ;

	map<string, AuxFileParser* (*)()>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, AuxFileParser* (*)() >(key,builder) );
    }
}


void AuxFileParserSingleton::unregisterFactory(string const & key, AuxFileParser* (*builder)() ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\".") ;
	map<string, AuxFileParser* (*)()>::iterator it = _map.find(key); 
	//see if the key is already registered in _map
	if(it == _map.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is not present.") ;
		return ; //the builder is not present.
	}
	_map.erase(it);
}

///////////////////////
// public methods
///////////////////////

AuxFileParser* AuxFileParserSingleton::newPredictedFileParser(string const & mkey)
{
	return newObject(mkey + "POFileParser") ;
}

AuxFileParser* AuxFileParserSingleton::newObtFileParser(string const & mkey)
{
	return newObject(mkey + "OBTFileParser") ;
}

AuxFileParser* AuxFileParserSingleton::newOrbitScenarioFileParser(string const & mkey)
{
	return newObject(mkey + "OSFileParser") ;
}

AuxFileParser* AuxFileParserSingleton::newOrbitDataFileParser(string const & mkey)
{
	return newObject(mkey + "OrbitDataFileParser") ;
}

AuxFileParser* AuxFileParserSingleton::newSTDMFileParser(string const & mkey)
{
	return newObject(mkey + "STDMFileParser");
}


AuxFileParser *AuxFileParserSingleton::newObject(string const & key)
{
	map<string, AuxFileParser* (*)()>::iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("AuxFileParserSingleton : key \""+key+"\" is unknown.")) ; // PRQA S 3081
		
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\"") ;
	return ( *(it->second ) )();
}

AuxFileParserSingleton::AuxFileParserSingleton() :
    Singleton<AuxFileParserSingleton>(),
	_map()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}




void registerAuxFileParserFactoryMethod(string const & key, AuxFileParser* (*builder)() )
{
	AuxFileParserSingleton::instance()->registerFactory(key,builder);
}


void unregisterAuxFileParserFactoryMethod(string const & key, AuxFileParser* (*builder)() )
{
	AuxFileParserSingleton::instance()->unregisterFactory(key,builder);
}

_ACS_END_NAMESPACE
