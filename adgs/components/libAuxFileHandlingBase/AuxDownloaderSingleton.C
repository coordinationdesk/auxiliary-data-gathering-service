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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2013/11/19 13:36:15  ricfer
	Compiler warnings removed. New logging macros adopted.
	
	Revision 2.6  2013/03/12 17:49:01  marpas
	dbCOnnectionPool is mandatory in file actions
	
	Revision 2.5  2011/03/01 17:18:36  marpas
	GCC 4.4.x support
	
	Revision 2.4  2008/01/18 14:35:11  enrcar
	MP: delete was used instead of delete[]. Fixed
	
	Revision 2.3  2006/09/18 13:05:04  giucas
	toUppercase method modified in order L102to transform the first letter in upper case in the remaining letters in lower case
	

*/

#include <AuxDownloaderSingleton.h>
#include <exStream.h>
#include <StringUtils.h>
#include <algorithm>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AuxDownloaderSingleton);

void AuxDownloaderSingleton::registerFactory(string const & key, AuxDownloader* (*builder)(dbConnectionPool &) )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is \""<<key<<"\". builder address is \""<<builder<<"\".");
	map<string, AuxDownloader* (*)(dbConnectionPool &)>::iterator it = _map.find(key);
	//register only if not registered yet
	if(it == _map.end() ) {
		_map.insert( pair<string, AuxDownloader* (*)(dbConnectionPool &) >(key,builder) );
    }
}


void AuxDownloaderSingleton::unregisterFactory(string const & key, AuxDownloader* (*builder)(dbConnectionPool &) ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is \""<<key<<"\". builder address is \""<<builder<<"\".");
	map<string, AuxDownloader* (*)(dbConnectionPool &)>::iterator it = _map.find(key); 
	//see if the key is already registered in _map
	if(it == _map.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is not present.");
		return ; //the builder is not present.
	}
	_map.erase(it);
}


///////////////////////
// public methods
///////////////////////

AuxDownloader* AuxDownloaderSingleton::newAuxDownloader(string const & mkey, dbConnectionPool &pool)
{
	string key = StringUtils::uppercase(mkey) + "AuxDownloader";
	return ( newObject(key, pool) );
}

AuxDownloader *AuxDownloaderSingleton::newObject(string const & key, dbConnectionPool &pool)
{
	map<string, AuxDownloader* (*)(dbConnectionPool &)>::iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("AuxDownloaderSingleton : key \""+key+"\" is unknown.")) // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is \""<<key<<"\". builder address is \""<<it->second<<"\".");
	return ( *(it->second ) )(pool);
}

AuxDownloaderSingleton::AuxDownloaderSingleton() :
    Singleton<AuxDownloaderSingleton>(),
    _map()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}



void AuxDownloaderSingleton::registerAuxDownloaderFactoryMethod(string const & key, AuxDownloader* (*builder)(dbConnectionPool &) )
{
	AuxDownloaderSingleton::instance()->registerFactory(key,builder);
}


void AuxDownloaderSingleton::unregisterAuxDownloaderFactoryMethod(string const & key, AuxDownloader* (*builder)(dbConnectionPool &) )
{
	AuxDownloaderSingleton::instance()->unregisterFactory(key,builder);
}

_ACS_END_NAMESPACE
