// PRQA S 1050 EOF
/*

	Copyright 2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Generic FileAction Library $

	$Id$

	$Author$

	$Log$
	Revision 1.9  2016/12/05 13:01:10  lucio.pulvirenti
	S3MPM-843: In download, dirty file existance is analogous to file not existance. Configurable timeout is condition to exit wait loops for locks it not got.
	As to directories hardlink, directories permissions are set to 770. Static attribute to emit warning for hard link across devices.
	
	Revision 1.8  2016/11/30 10:31:54  lucio.pulvirenti
	S3MPM-843: mkdtemp function used to create temporary directory. In download method, if file does not exist, lock it and exit the method.
	Cache pointer added to member functions.
	
	Revision 1.7  2016/11/29 16:17:17  lucio.pulvirenti
	S3MPM-843: work in progress.
	
	Revision 1.6  2016/11/24 14:31:00  lucio.pulvirenti
	S3MPM-843: work in progress.
	
	Revision 1.5  2016/11/23 16:06:11  lucio.pulvirenti
	S3MPM-843: Multi processes concurrency tests performed, both in uploading and in downloading; cache working tested.
	
	Revision 1.4  2016/11/22 16:51:58  lucio.pulvirenti
	S3MPM-843: work in progress.
	
	Revision 1.3  2016/11/18 15:47:07  lucio.pulvirenti
	S3MPM-843: work in progress.
	
	Revision 1.2  2016/11/17 16:38:42  lucio.pulvirenti
	S3MPM-843: work in progress.
	
	Revision 1.1  2016/11/16 16:35:04  lucio.pulvirenti
	S3MPM-843: first issue.
	
	
	
*/


#ifndef _dbInventoryObjectCacheManager_H_
#define _dbInventoryObjectCacheManager_H_ 

#include <exException.h>
#include <FileGuard.h>

#include <memory>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent;
class Cache;

//
//
//
// class dbInventoryObjectCacheManager
//
//
//
class dbInventoryObjectCacheManager // PRQA S 2109
{
public:

	enum TransferMode {
		NONE,
		COPY,
		HLINK
	} ;
		
	/*! class dbInventoryObjectCacheManager exceptions */
	exDECLARE_EXCEPTION(dbInventoryObjectCacheManagerException, exException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(TimeoutException, dbInventoryObjectCacheManagerException) ; // PRQA S 2131, 2153, 2502

	dbInventoryObjectCacheManager();
	virtual ~dbInventoryObjectCacheManager() throw() ;

	
/*! \brief Check cache is configured: it must be called before any other method 
	\param t_inventory record 
	\return true if object has been configured and initialized
    \throw dbInventoryObjectCacheManagerException if dbPersistent cast to dbInventoryObject fails or if cache path is empty
*/
	bool checkIsConfigured(const dbPersistent&);

/*! \brief get the configured cache path 
	\return cache path 
*/
	std::string getCachePath() const;
	
/*! \brief upload file in cache 
	\param path to upload 
	\return true if file has been uploaded
*/
	bool uploadFile(const std::string & path) const ;
	
/*! \brief download file from cache 
	\param path where download file 
	\param gotExclusiveLock: if ptr is valorized, return value is true if exclusive lock has been acquired (useful if next uploading is foreseen)
	\return true if file has been downloaded
*/
	bool downloadFile(const std::string & path, bool * gotExclusiveLock = 0) const ;
	
	
private:
	dbInventoryObjectCacheManager(const dbInventoryObjectCacheManager &) ;
	dbInventoryObjectCacheManager &operator=(const dbInventoryObjectCacheManager &) ;

	void extractInventoryParameters( const dbPersistent& )  ;
	std::string getDirtyFilename( const std::string & filename ) const;
	std::string transferModeToString(TransferMode ) const;
	TransferMode stringToTransferMode( const std::string & s ) const;
	bool overrideTransferMode(TransferMode &) const;
	TransferMode getTransferMode(const std::string & path1, const std::string & path2) const;
	void transferFile( const std::string & src, const std::string & dest ) const;
	FileGuard createTemporaryEntity(const std::string & src, const std::string & dest) const;
	bool emitWarning() const;
		
private:
	std::string _invFilename;
	std::string _invFileVersion;
	off_t _invSize;
	std::string _cacheFilename;
	ACS_SMARTPTR<Cache> _cachePtr;
	mutable bool _exclusiveLockSet;
	int _lockTimeout;
	static bool _emitWarning;
    static acs::ThreadSafe _warningMutex;  
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObjectCacheManager) ;

} ; 


_ACS_END_NAMESPACE


#endif // _dbInventoryObjectCacheManager_H_
