// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 7.1  2017/11/07 16:22:29  lucio.pulvirenti
	S2PDGS-1843: classes added.
	

*/


#include <GenStorageManagerInfoFactory.h>

#include <GenStorageManagerInfo.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

GenStorageManagerInfoFactory GenStorageManagerInfoFactory::_theFactory ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(GenStorageManagerInfoFactory)

GenStorageManagerInfoFactory::GenStorageManagerInfoFactory() : 
    StorageManagerInfoFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
}

GenStorageManagerInfoFactory::~GenStorageManagerInfoFactory() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
    // PRQA L:L1
}


StorageManagerInfo * GenStorageManagerInfoFactory::newStorageManagerInfo(dbConnectionPool &pool) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	return new GenStorageManagerInfo(pool) ; // PRQA S 3382, 3385
}


string GenStorageManagerInfoFactory::capabilities() const
{
	string ret ;
	ret += string("Nothing on InventoryObject") + "\n" ; // PRQA S 3081
	ret += string("Storage Manager Info") + "\n" ; // PRQA S 3081
	return ret ;
}

_ACS_END_NAMESPACE
 
