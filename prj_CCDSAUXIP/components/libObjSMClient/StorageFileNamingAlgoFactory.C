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


#include <StorageFileNamingAlgoFactory.h>

#include <StorageFileNamingAlgoSingleton.h>
#include <StorageFileNamingAlgoIdentity.h>
#include <StorageFileNamingAlgoPdsLike.h>

#include <dbPersistent.h>
#include <Filterables.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(StorageFileNamingAlgoFactory);

StorageFileNamingAlgoFactory::StorageFileNamingAlgoFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	StorageFileNamingAlgoSingleton::instance()->registerFactory("IDENTITY",&buildStorageFileNamingAlgoIdentity ) ;	
	StorageFileNamingAlgoSingleton::instance()->registerFactory("PDSLIKE",&buildStorageFileNamingAlgoPdsLike ) ;	
	
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
}


StorageFileNamingAlgoFactory::~StorageFileNamingAlgoFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB); 
			
	StorageFileNamingAlgoSingleton::instance()->unregisterFactory("IDENTITY",&buildStorageFileNamingAlgoIdentity ) ;	
	StorageFileNamingAlgoSingleton::instance()->unregisterFactory("PDSLIKE",&buildStorageFileNamingAlgoPdsLike ) ;	
		
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
    // PRQA L:L1
}

// *************** buildStorageFileNamingAlgoIdentity ***************************
StorageFileNamingAlgo* StorageFileNamingAlgoFactory::buildStorageFileNamingAlgoIdentity(const dbPersistent * o)
{
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Going to allocate object");
	
	return new StorageFileNamingAlgoIdentity(o);
}

// *************** StorageFileNamingAlgoPdsLike ***************************
StorageFileNamingAlgo* StorageFileNamingAlgoFactory::buildStorageFileNamingAlgoPdsLike(const dbPersistent * o)
{
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Going to allocate object");
	
	return new StorageFileNamingAlgoPdsLike(o);
}


_ACS_END_NAMESPACE
