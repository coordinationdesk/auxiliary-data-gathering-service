/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
	Revision 2.3  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 2.2  2012/04/18 08:28:16  giucas
	Compilation warnings fixed
	
	Revision 2.1  2006/10/30 17:22:25  enrcar
	debug improved
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/02 17:52:40  giucas
	First release
	

*/

#include <OBTConverterSingleton.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OBTConverterSingleton)

void OBTConverterSingleton::registerFactory(string const & key, OBTConverter* (*builder)() )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\"");		

	map<string, OBTConverter* (*)()>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, OBTConverter* (*)() >(key,builder) );
	}
}


void OBTConverterSingleton::unregisterFactory(string const & key, OBTConverter* (*builder)() )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\"");		
	map<string, OBTConverter* (*)()>::iterator it = _map.find(key); 
	//see if the key is already registered in _map
	if(it == _map.end() )
	{
		return ; //the builder is not present.
	}
	_map.erase(it);
}

///////////////////////
// public methods
///////////////////////

OBTConverter* OBTConverterSingleton::newObtConverter(string const & mkey)
{
	string key = mkey + "OBTConverter";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"key is \""<<key<<"\" and input mkey = " << mkey);	
	map<string, OBTConverter* (*)()>::iterator it = _map.find(key);
	ACS_COND_THROW(it == _map.end() , KeyNotFound("OBTConverterFactory::newObtConverter ERROR : key "+key+" is unknown") )
	return ( *(it->second ) )();
}


OBTConverterSingleton::OBTConverterSingleton() :
    Singleton <OBTConverterSingleton>(), 
	_map()
{
}

OBTConverterSingleton::~OBTConverterSingleton() throw()
{
}

_ACS_END_NAMESPACE
