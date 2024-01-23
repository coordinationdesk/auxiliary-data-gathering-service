/*
	Copyright 2009-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

*/

#include <Mappleton.h>
#include <Filterables.h>

using std::string;
using std::boolalpha;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL (Mappleton)

string Mappleton::getValue (const string& theKey, const string& defValue, bool throwIfNotFound) const
{

	ACS_CLASS_ANNOUNCE_DEBUG (ACS_VLO_VERB) ;
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Searching key \"" << theKey << "\" in mappleton. Default is \"" << defValue << "\" Throw if not found is " << 
	                       boolalpha << throwIfNotFound << "\n") ;

	// Init a return value
	string retVal (defValue);
	{ // NOSONAR scope to get rid of lock as soon as possible 
		// Lock the structure
		ThreadSafe::Lock lock (_mapMutex);
		
		auto it = _theMap.find(theKey);
		if (it != _theMap.end() )
		{
			// Key found. has to return it.
			retVal = it->second;
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Key \"" << theKey << "\" found in mappleton" << "\n") ;
		}
		else
		{
			// OOpps. not found. Throw according to the passed parameters
			ACS_COND_THROW (throwIfNotFound, exKeyNotFoundMappletonException ("Key not found in mappleton \"" + theKey + "\""));
		}
	}

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Returning value \"" << retVal << "\" for key \"" << theKey << "\"\n") ;
	return retVal;
}


bool Mappleton::hasKey (const string& theKey) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_VLO_VERB) ;
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Checking existance of the key \"" << theKey << "\" in mappleton.\n") ;

	// Lock the structure
	ThreadSafe::Lock lock (_mapMutex);

	// Find it
	auto it = _theMap.find(theKey);
	bool found = (it != _theMap.end() );

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Checking existance of the key \"" << theKey << "\" in mappleton returning." << boolalpha << found << "\n") ;
	return found;
}

void Mappleton::setValue (const string& theKey, const string& theValue)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_VLO_VERB) ;
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Setting key \"" << theKey << "\" in mappleton to value \"" << theValue << "\"\n") ;

	// Lock the structure
	ThreadSafe::Lock lock (_mapMutex);

	_theMap[theKey] = theValue;

}

void Mappleton::removeKey (const string& theKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_VLO_VERB) ;
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Removing the key \"" << theKey << "\" from mappleton.\n") ;

	// Lock the structure
	ThreadSafe::Lock lock (_mapMutex);
	_theMap.erase (theKey);
}

_ACS_END_NAMESPACE
