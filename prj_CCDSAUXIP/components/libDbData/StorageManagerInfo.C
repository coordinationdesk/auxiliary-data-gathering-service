// PRQA S 1050 EOF
/*

	Copyright 2017-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	

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



StorageManagerInfo::~StorageManagerInfo() noexcept
{
}


dbConnectionPool &StorageManagerInfo::getPool()   // PRQA S 4120
{
    return _pool ; // PRQA S 4024
}



_ACS_END_NAMESPACE
