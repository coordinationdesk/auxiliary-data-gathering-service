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
	Revision 2.1  2014/02/13 11:03:12  giucas
	New log macros adopted
	
	Revision 2.0  2006/02/28 09:40:42  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/04/21 10:50:08  giucas
	Removed dependencies from ACE. Using ThreadSafe class instead.
	

*/


#include <OrbitPropagatorSingleton.h>
#include <opV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
	opV Version;
}

OrbitPropagatorSingleton* OrbitPropagatorSingleton::_instance = 0;
acs::ThreadSafe OrbitPropagatorSingleton::_mutex;

//constructor
OrbitPropagatorSingleton::OrbitPropagatorSingleton()
{
}

//destructor
OrbitPropagatorSingleton::~OrbitPropagatorSingleton()
{
	if(_instance != 0) {
		_instance=0;
	}
}
 

OrbitPropagatorSingleton* OrbitPropagatorSingleton::Instance()
{
	if(_instance != 0) {
		return _instance ;
	}
	ACS_THROW(exIllegalValueException("No singleton registered")); // PRQA S 3081
	
}

void OrbitPropagatorSingleton::Register(OrbitPropagatorSingleton* singlPtr)
{
	if(_instance == 0)
	{
		acs::ThreadSafe::Lock guard(_mutex);
		{
			if(_instance == 0) {
				_instance = singlPtr;				
            }
		}
	}
	else
	{
		ACS_THROW(exIllegalValueException("Singleton already registered")) ; // PRQA S 3081
	}
}
_ACS_END_NAMESPACE
