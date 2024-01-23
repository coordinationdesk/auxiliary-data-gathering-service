// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libCirculationCallback$

	$Id$

	$Author$

	$Log$
        
*/ 

#include <DC_CallbackSingleton.h>

#include <Filterables.h>
#include <Application.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DC_CallbackSingleton)
ACS_CLASS_DEFINE_DEBUG_LEVEL(CirculationCallbackInterface)

Mutex DC_CallbackSingleton::_mutex ;
CirculationCallbackInterface::CirculationCallbackInterface() :
    	_connPool(*Application::instance()->getDefaultConnectionPool()) // PRQA S 2528
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

CirculationCallbackInterface::~CirculationCallbackInterface() ACS_NOEXCEPT
{}

dbConnectionPool & CirculationCallbackInterface::getPool()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return _connPool;
}

DC_CallbackSingleton::DC_CallbackSingleton(): Singleton <DC_CallbackSingleton>(), _thePtr(0) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);// PRQA S 4631
}

CirculationCallbackInterface* DC_CallbackSingleton::getCallbackManager() const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    ThreadSafe::Lock lock(_mutex) ;
 	return _thePtr;
}
void DC_CallbackSingleton::setCallbackManager(CirculationCallbackInterface* ptr)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    ThreadSafe::Lock lock(_mutex) ;
 	_thePtr = ptr;
}




_ACS_END_NAMESPACE

