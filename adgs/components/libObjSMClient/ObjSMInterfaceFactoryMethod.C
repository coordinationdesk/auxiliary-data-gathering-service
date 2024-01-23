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


#include <ObjSMInterfaceFactoryMethod.h>
#include <ObjStorageManager.h>
#include <Filterables.h>
#include <ObjSmV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    ObjSmV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ObjSMInterfaceFactoryMethod);
ObjSMInterfaceFactoryMethod ObjSMInterfaceFactoryMethod::_theMethod("OBJSM") ;

ObjSMInterfaceFactoryMethod::ObjSMInterfaceFactoryMethod(const std::string &smtype) : SMInterfaceFactoryMethod(smtype)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


ObjSMInterfaceFactoryMethod::~ObjSMInterfaceFactoryMethod() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}



SMInterface * ObjSMInterfaceFactoryMethod::createInterface(dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &pool) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "returning OBJStorageManager(" << type() << "," << smid << ",..)") ;
	return new ObjStorageManager(type(),smid, obj, pool);
}


_ACS_END_NAMESPACE
