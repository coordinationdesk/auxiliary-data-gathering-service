// PRQA S 1050 EOF
/*
	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA


*/


#include <ObjSMInterfaceFactoryMethod.h>
#include <ObjStorageManager.h>
#include <Filterables.h>
#include <ObjSmV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
    ObjSmV version ;
	
	class Init {
		public:
			Init() { _theMethod = new ObjSMInterfaceFactoryMethod("OBJSM") ; }
		
			ObjSMInterfaceFactoryMethod * _theMethod = nullptr ;
		
	} ;
	
	Init embeddedInit ;

}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ObjSMInterfaceFactoryMethod);

ObjSMInterfaceFactoryMethod::ObjSMInterfaceFactoryMethod(const std::string &smtype) : SMInterfaceFactoryMethod(smtype)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


SMInterface * ObjSMInterfaceFactoryMethod::createInterface(dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &pool) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "returning OBJStorageManager(" << type() << "," << smid << ",..)") ;
	return new ObjStorageManager(type(),smid, obj, pool);
}


_ACS_END_NAMESPACE
