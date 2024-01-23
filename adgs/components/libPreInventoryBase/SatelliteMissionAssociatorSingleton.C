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
	Revision 1.2  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.1  2013/08/09 12:23:13  lucio.pulvirenti
	First issue.
	
	
*/ 

#include <SatelliteMissionAssociatorSingleton.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;



ACS_CLASS_DEFINE_DEBUG_LEVEL(SatelliteMissionAssociatorSingleton);


void SatelliteMissionAssociatorSingleton::registerFactory(string const & key, SatelliteMissionAssociator* (*builder)() )
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< builder<<"\".");

	map<string, SatelliteMissionAssociator* (*)()>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, SatelliteMissionAssociator* (*)() >(key,builder) );
	}
}


void SatelliteMissionAssociatorSingleton::unregisterFactory(string const & key, SatelliteMissionAssociator* (*builder)() ) // PRQA S 4020
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< builder<<"\".");

	map<string, SatelliteMissionAssociator* (*)()>::iterator it = _map.find(key);
	if(it == _map.end() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is not present.");
		return ; //the builder is not present.
	}
	_map.erase(it);

}


SatelliteMissionAssociator *SatelliteMissionAssociatorSingleton::newSatelliteMissionAssociator(string const & key) const
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Input key is \"" << key << "\".");
	
	//Build the Valid key to return the expected pointer
	map<string, SatelliteMissionAssociator* (*)()>::const_iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("key \""+key+"\" is unknown.")); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input key is \"" << key << "\". builder address is \""<< it->second<<"\".");
	
	return ( *(it->second ) )();
}

SatelliteMissionAssociatorSingleton::SatelliteMissionAssociatorSingleton():
	pattern::Singleton<SatelliteMissionAssociatorSingleton> (),
    _map() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Instance now.");
}

SatelliteMissionAssociatorSingleton::~SatelliteMissionAssociatorSingleton() throw() { 

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;// PRQA S 4631
}

_ACS_END_NAMESPACE
