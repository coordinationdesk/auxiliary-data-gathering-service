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


#ifndef _dbIsOnCacheFile_H_
#define _dbIsOnCacheFile_H_ 


#include <acs_c++config.hpp>
#include <dbFileAction.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnectionPool ;

//
//
//
// class dbIsOnCacheFile
//
//
//

/**
 * The class in in charge to giveup a previous download request.
 */

class dbIsOnCacheFile : public dbFileAction // PRQA S 1703, 2108, 2109, 2153, 2194
{
public:

	dbIsOnCacheFile(dbConnectionPool &, dbPersistent &, int storageid=0);
	virtual ~dbIsOnCacheFile() noexcept ;

	// declared but not implemented to prevent their use
	dbIsOnCacheFile() = delete ;
	dbIsOnCacheFile(const dbIsOnCacheFile &) = delete ;
	dbIsOnCacheFile &operator=(const dbIsOnCacheFile &) = delete ;
	dbIsOnCacheFile(const dbIsOnCacheFile &&) = delete ;
	dbIsOnCacheFile &operator=(const dbIsOnCacheFile &&) = delete ;
	
	virtual void start() = 0 ; // PRQA S 2121
	virtual bool isComplete() const ; 
    virtual CacheStatus getCacheStatus() { return ON_CACHE ; }
	
    int getStorageId() const noexcept { return _storageId ; }

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbIsOnCacheFile) ;
	
    int _storageId ;
	
} ; 

_ACS_END_NAMESPACE

#endif // _dbIsOnCacheFile_H_
