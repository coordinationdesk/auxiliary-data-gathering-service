// PRQA S 1050 EOF
/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ISM SM client library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/09/20 08:03:33  marpas
	new t_inventorysm schema supported
	
	Revision 5.0  2013/06/20 13:37:44  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa rules
	
	Revision 1.4  2013/03/12 17:48:00  marpas
	dbCOnnectionPool is mandatory in SM interfaces and factory methods
	
	Revision 1.3  2012/02/13 17:09:49  marpas
	refactoring in progress
	
	Revision 1.2  2010/08/11 08:20:03  marpas
	debug improved
	
	Revision 1.1.1.1  2008/06/16 15:14:58  marpas
	Importing libIsmSMClient
	

*/


#include <ISMInterfaceFactoryMethod.h>
#include <ISMStorageManager.h>
#include <Filterables.h>
#include <ismclientV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    ismclientV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ISMInterfaceFactoryMethod);
ISMInterfaceFactoryMethod ISMInterfaceFactoryMethod::_theMethod("ACSISM") ;

ISMInterfaceFactoryMethod::ISMInterfaceFactoryMethod(const std::string &smtype) : SMInterfaceFactoryMethod(smtype)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


ISMInterfaceFactoryMethod::~ISMInterfaceFactoryMethod() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}



SMInterface * ISMInterfaceFactoryMethod::createInterface(dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &pool) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "returning ISMStorageManager(" << type() << "," << smid << ",..)") ;
	return new ISMStorageManager(type(),smid, obj, pool);
}


_ACS_END_NAMESPACE
