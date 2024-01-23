// PRQA S 1050 EOF
/*

	Copyright 2022-, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $
*/

#include <dbPutOnCacheFile.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbPutOnCacheFile)

// constructor
dbPutOnCacheFile::dbPutOnCacheFile(dbConnectionPool &pool, dbPersistent &p, int storageid) :
	dbFileAction(pool, p),
    _storageId(storageid)
{
}



dbPutOnCacheFile::~dbPutOnCacheFile() noexcept 
{
}

bool dbPutOnCacheFile::isComplete() const 
{
	return getCompleted() ;
}



_ACS_END_NAMESPACE
