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

#include <PackagingAlgoSingleton.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagingAlgoSingleton);


PackagingAlgoSingleton::PackagingAlgoSingleton():
	pattern::Singleton<PackagingAlgoSingleton> (),
    _map() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Instance now.");
}

PackagingAlgoSingleton::~PackagingAlgoSingleton() throw()
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;// PRQA S 4631
}


void PackagingAlgoSingleton::registerFactory(string const & key, PackagingAlgo* (*builder)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &) )
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< builder<<"\".");

	map<string, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &)>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &) >(key,builder) );
	}
}


void PackagingAlgoSingleton::unregisterFactory(string const & key, PackagingAlgo* (*builder)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &) ) // PRQA S 4020
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is \"" << key <<"\". builder address is \""<< builder<<"\".");

	map<string, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &)>::iterator it = _map.find(key);
	if(it == _map.end() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Key is not present.");
		return ; //the builder is not present.
	}
	_map.erase(it);

}


PackagingAlgo *PackagingAlgoSingleton::newPackagingAlgo(string const & key, const std::map<std::string, std::string> & arcOpts, const std::map<std::string, std::string> & xtrOpts) const
{
	Lock lock(_M_mutex()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Input key is \"" << key << "\".");
	
	//Build the Valid key to return the expected pointer
	map<string, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &)>::const_iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("key \""+key+"\" is unknown.")); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input key is \"" << key << "\". builder address is \""<< it->second<<"\".");
	
	return ( *(it->second ) )(arcOpts, xtrOpts);
}


_ACS_END_NAMESPACE
