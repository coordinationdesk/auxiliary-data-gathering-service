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
	Revision 2.2  2014/03/18 12:08:34  marpas
	Coding best practices applied
	warnings removed (g++ & qa)
	interface rationalization
	

*/


#include <TimeConverterSingleton.h>
#include <tcV.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TimeConverterSingleton)


namespace{
	tcV Version;
}

//
//
//	Singleton Implementation
//
//

TimeConverterSingleton* TimeConverterSingleton::_instance = 0;
acs::ThreadSafe TimeConverterSingleton::_mutex;

//constructor
TimeConverterSingleton::TimeConverterSingleton()
{
}

//destructor
TimeConverterSingleton::~TimeConverterSingleton()
{
	if(_instance != 0) {
		_instance=0;
	}
}
 

TimeConverterSingleton* TimeConverterSingleton::Instance()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	if(_instance != 0) {
		return _instance ;
	}
	
    ACS_THROW(exIllegalValueException("Error: No singleton registered till now")) ; // PRQA S 3081
}

void TimeConverterSingleton::Register(TimeConverterSingleton* singlPtr) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	if(_instance == 0)
	{
		acs::ThreadSafe::Lock guard(_mutex);
		{
			if(_instance == 0) {
				_instance = singlPtr;
                return ;				
			}
		}
	}
	
    ACS_THROW(exIllegalValueException("Singleton already registered")) ; // PRQA S 3081
}
_ACS_END_NAMESPACE
