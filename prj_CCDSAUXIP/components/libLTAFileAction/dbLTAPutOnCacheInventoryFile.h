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


#ifndef _dbLTAPutOnCacheInventoryFile_H_
#define _dbLTAPutOnCacheInventoryFile_H_ 

#include <dbPutOnCacheFile.h>
#include <LTAInventoryObject.h>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbLTAPutOnCacheInventoryFile
//
//
//


/**
 * The class is specialized in asking to SMs (or just one) to put a file on cache
 * User must provide the LTAInventoryObject to be asked for.
 */
class dbLTAPutOnCacheInventoryFile : public dbPutOnCacheFile  // PRQA S 2109
{
public:
	friend class dbLTAFileActionFactory ; // PRQA S 2107
		
	dbLTAPutOnCacheInventoryFile() = delete ;
	dbLTAPutOnCacheInventoryFile(const dbLTAPutOnCacheInventoryFile &) = delete ;
	dbLTAPutOnCacheInventoryFile &operator=(const dbLTAPutOnCacheInventoryFile &) = delete ;
	dbLTAPutOnCacheInventoryFile(dbLTAPutOnCacheInventoryFile &&) = delete ;
	dbLTAPutOnCacheInventoryFile &operator=(dbLTAPutOnCacheInventoryFile &&) = delete ;

	virtual ~dbLTAPutOnCacheInventoryFile() = default ;

	// inherited from dbPutOnCacheFile
	/**
	 * Start the request.
	 * \throw OnCacheException if an SM has been provided and it does not allow to download the inventory
	 */
	virtual void start() ;
        
    /** 
     * \brief cache status
     * The method returns the cache status. It is valid only after getCompleted is true.
     * The returned status can be one of:
     * ON_CACHE the file is on cache ready to be downloaded
     * IN_PROGRESS the request has been got and will be served
     * NOT_AVAILABLE the file is unknown
     */    
    virtual CacheStatus getCacheStatus() { return _status ; }

protected:

	dbLTAPutOnCacheInventoryFile(dbConnectionPool &, db::LTAInventoryObject &,int storageid=0);

private: 
	SQLString getSyncSMsForPutOnCacheQuery(int) const;

private :
	db::LTAInventoryObject &_inventory ;
	std::string const _inventoryName;
    CacheStatus _status ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAPutOnCacheInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 

#endif // _dbLTAPutOnCacheInventoryFile_H_
