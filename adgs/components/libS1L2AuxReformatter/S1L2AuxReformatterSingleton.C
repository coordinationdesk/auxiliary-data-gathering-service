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

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.3  2013/01/25 13:59:28  marpas
	using pattern::Singleton from libException
	
	Revision 1.2  2012/06/12 11:00:41  marpas
	old-style casts removed
	
	Revision 1.1  2012/04/23 08:02:39  damdec
	Class added.
	
        
*/ 

#include <S1L2AuxReformatterSingleton.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1L2AuxReformatterSingleton)


S1L2AuxReformatterSingleton::S1L2AuxReformatterSingleton() : _mapReformatter(){
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
}

S1L2AuxReformatterSingleton::~S1L2AuxReformatterSingleton() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void S1L2AuxReformatterSingleton::registerFactory(std::string const & key, S1L2AuxReformatter * (*builder)())
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "registerFactory called. key is \""<<key<<"\". builder address is \""<< builder<<"\".");
	
	std::map<std::string, S1L2AuxReformatter * (*)()>::iterator it =_mapReformatter.find(key);
	//register only if not registered yet
	if(it == _mapReformatter.end() )
		_mapReformatter.insert( pair<string, S1L2AuxReformatter* (*)() >(key, builder) );
	
}
void S1L2AuxReformatterSingleton::unregisterFactory(std::string const & key, S1L2AuxReformatter * (*builder)())
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "unregisterFactory called. key is \""<<key<<"\". builder address is \""<<builder<<"\".");
	
	std::map<std::string, S1L2AuxReformatter * (*)()>::iterator it =_mapReformatter.find(key);
	
	if(it == _mapReformatter.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is not present.");
	
		return; // the builder is not present
	}
	_mapReformatter.erase( it );
	
}

S1L2AuxReformatter * S1L2AuxReformatterSingleton::newReformatter(std::string const & key)
{
	std::map<std::string, S1L2AuxReformatter * (*)()>::iterator it =_mapReformatter.find(key);
	if(it == _mapReformatter.end() )
	{
		ostringstream error ;
		error << "S1L2AuxReformatterSingleton : key \"" << key << "\" is unknown.";
		ACS_THROW(exS1L2AuxReformatterSingletonException(error.str()));
	
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is \""<<key<<"\". builder address is \""<< it->second <<"\".");

	return ( *(it->second ) )();

}

void registerS1L2AuxReformatterFactoryMethod(string const & key, S1L2AuxReformatter* (*builder)() )
{
	S1L2AuxReformatterSingleton* c=S1L2AuxReformatterSingleton::instance();
	c->registerFactory(key,builder);
}


void unregisterS1L2AuxReformatterFactoryMethod(string const & key, S1L2AuxReformatter* (*builder)() )
{
	S1L2AuxReformatterSingleton* c=S1L2AuxReformatterSingleton::instance();
	c->unregisterFactory(key,builder);
}

_ACS_END_NAMESPACE
