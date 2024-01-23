// PRQA S 1050 EOF
/*
	Copyright 2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
        
*/


#include <IsmServerVersionManagerSingleton.h>
			
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(IsmServerVersionManagerSingleton);


Mutex IsmServerVersionManagerSingleton::_mapMutex ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IsmServerVersionManagerSingleton::IsmServerVersionManagerSingleton():
										_serversMap()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IsmServerVersionManagerSingleton::~IsmServerVersionManagerSingleton() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int IsmServerVersionManagerSingleton::getVersion( const string & serverName )
{
	ACS_COND_THROW(serverName.empty(), exIllegalValueException("Server name not valorized")) ;
	
	Lock lock(_mapMutex) ;
	map<string, int>::const_iterator it = _serversMap.find(serverName);
	int version = -1;
	if( it != _serversMap.end() ) {
		version = it->second;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << serverName << "\" server version found: " << version );
	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << serverName << "\" server version not found, returning " << version );
	}
	return version;	
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IsmServerVersionManagerSingleton::setVersion( const string & serverName, int version)
{
	ACS_COND_THROW(serverName.empty(), exIllegalValueException("Server name not valorized")) ;
	
	if( version <= 0 ) {
		ostringstream exMsg;
		exMsg << "For \"" << serverName << " server input version is " << version << " (it should be >0)";
		ACS_THROW( exIllegalValueException( exMsg.str() ) );
	}
	
	Lock lock(_mapMutex) ;
	map<string, int>::const_iterator it = _serversMap.find(serverName);
	if( it != _serversMap.end() ) {
		if( it->second != version ) {
			ostringstream exMsg;
			exMsg << '\"' << serverName << "\" server already occurring in map with version " << it->second << "; trying to insert version " << version << " (they should match)";
			ACS_THROW( exIsmServerVersionManagerSingletonException( exMsg.str() ) );
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << serverName << "\" server already occurring in map with version " << it->second );
		}
	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << serverName << "\" server version not found, going to insert " << version );
		_serversMap[serverName] = version;
	}
	
}

_ACS_END_NAMESPACE
