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
    
    $Log$

*/

#include <ProdTypeDescriptorSingleton.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProdTypeDescriptorSingleton)


void ProdTypeDescriptorSingleton::registerFactory(string const & key, ProductTypeDescriptor* (*builder)() )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\"");

	map<string, ProductTypeDescriptor* (*)()>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, ProductTypeDescriptor* (*)() >(key,builder) );
	}
}


void ProdTypeDescriptorSingleton::unregisterFactory(string const & key, ProductTypeDescriptor* (*builder)() )  // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\"");
	map<string, ProductTypeDescriptor* (*)()>::iterator it = _map.find(key); 
	//see if the key is already registered in _map
	if(it == _map.end() )
	{
		return ; //the builder is not present.
	}
	_map.erase(it);
}



ProductTypeDescriptor *ProdTypeDescriptorSingleton::newObject(string const & key)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\"");
	map<string, ProductTypeDescriptor* (*)()>::iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("key \""+key+"\" is unknown")) ; // PRQA S 3081
	}
	return ( *(it->second ) )();
}

ProdTypeDescriptorSingleton::ProdTypeDescriptorSingleton() :
    Singleton <ProdTypeDescriptorSingleton>(), 
	_map()
{
}

ProdTypeDescriptorSingleton::~ProdTypeDescriptorSingleton() throw()
{
}

_ACS_END_NAMESPACE
