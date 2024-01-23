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

	$Prod: A.C.S. LTA File Action Library $

*/


#ifndef _dbLTAReleaseCacheInventoryFile_H_
#define _dbLTAReleaseCacheInventoryFile_H_ 

#include <dbReleaseCacheFile.h>
#include <LTAInventoryObject.h>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbLTAReleaseCacheInventoryFile
//
//
//


/**
 * The class is specialized in asking SM's cache removal of inventoried objects.
 * User must provide the LTAInventoryObject to be released.
 * The file will be released from any StorageManage that allows it or just the specific one
 */
class dbLTAReleaseCacheInventoryFile : public dbReleaseCacheFile  // PRQA S 2109
{
public:
	friend class dbLTAFileActionFactory ; // PRQA S 2107
		
	dbLTAReleaseCacheInventoryFile() = delete ;
	dbLTAReleaseCacheInventoryFile(const dbLTAReleaseCacheInventoryFile &) = delete ;
	dbLTAReleaseCacheInventoryFile &operator=(const dbLTAReleaseCacheInventoryFile &) = delete ;
	dbLTAReleaseCacheInventoryFile(dbLTAReleaseCacheInventoryFile &&) = delete ;
	dbLTAReleaseCacheInventoryFile &operator=(dbLTAReleaseCacheInventoryFile &&) = delete ;

	virtual ~dbLTAReleaseCacheInventoryFile() = default ;

	// inherited from dbReleaseCacheFile
	/**
	 * Start the release.
	 */
	virtual void start() ;
        
    virtual CacheStatus getCacheStatus() { return ON_CACHE ; }

protected:

	dbLTAReleaseCacheInventoryFile(dbConnectionPool &, db::LTAInventoryObject &,int storageid=0);

private: 
	SQLString getSyncSMsForReleaseQuery(int) const;

private :
	db::LTAInventoryObject &_inventory ;
	std::string const _inventoryName;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAReleaseCacheInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 

#endif // _dbLTAReleaseCacheInventoryFile_H_
