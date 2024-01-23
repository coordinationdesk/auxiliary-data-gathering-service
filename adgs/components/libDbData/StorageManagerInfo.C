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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	

*/


#include <StorageManagerInfo.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(StorageManagerInfo)

// constructor
StorageManagerInfo::StorageManagerInfo(dbConnectionPool & pool) :
    _pool(pool)    // PRQA S 2528 2
{
}



StorageManagerInfo::~StorageManagerInfo() throw()
{
}


dbConnectionPool &StorageManagerInfo::getPool()   // PRQA S 4120
{
    return _pool ; // PRQA S 4024
}



_ACS_END_NAMESPACE
