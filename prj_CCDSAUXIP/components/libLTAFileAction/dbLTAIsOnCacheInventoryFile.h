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


#ifndef _dbLTAIsOnCacheInventoryFile_H_
#define _dbLTAIsOnCacheInventoryFile_H_ 

#include <dbIsOnCacheFile.h>
#include <LTAInventoryObject.h>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbLTAIsOnCacheInventoryFile
//
//
//


/**
 * The class is specialized in asking to SMs (or just one) if the file is on cache
 * User must provide the LTAInventoryObject to be asked for.
 * The cache status will be asked to any StorageManage that has the file, or just a specific one
 */
class dbLTAIsOnCacheInventoryFile : public dbIsOnCacheFile  // PRQA S 2109
{
public:
	friend class dbLTAFileActionFactory ; // PRQA S 2107
		
	dbLTAIsOnCacheInventoryFile() = delete ;
	dbLTAIsOnCacheInventoryFile(const dbLTAIsOnCacheInventoryFile &) = delete ;
	dbLTAIsOnCacheInventoryFile &operator=(const dbLTAIsOnCacheInventoryFile &) = delete ;
	dbLTAIsOnCacheInventoryFile(dbLTAIsOnCacheInventoryFile &&) = delete ;
	dbLTAIsOnCacheInventoryFile &operator=(dbLTAIsOnCacheInventoryFile &&) = delete ;

	virtual ~dbLTAIsOnCacheInventoryFile() = default ;

	// inherited from dbIsOnCacheFile
	/**
	 * Start the erase.
	 * \throw OnCacheException if an SM has been provided and it does not allow to download the inventory
	 */
	virtual void start() ;
    
    /** 
     * \brief cache status
     * The method returns the cache status. It is valid only after getCompleted is true.
     * The returned status can be one of:
     * ON_CACHE the file is on cache ready to be downloaded
     * OFF_CACHE the file is on some media even offline
     * NOT_AVAILABLE the file is unknown
     */    
    virtual CacheStatus getCacheStatus() { return _status ; }

protected:

	dbLTAIsOnCacheInventoryFile(dbConnectionPool &, db::LTAInventoryObject &,int storageid=0);

private: 
	SQLString getSyncSMsForIsOnCacheQuery(int) const;

private :
	db::LTAInventoryObject &_inventory ;
	std::string const _inventoryName;
    CacheStatus _status ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAIsOnCacheInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 

#endif // _dbLTAIsOnCacheInventoryFile_H_
