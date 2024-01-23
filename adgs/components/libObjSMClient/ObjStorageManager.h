// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	

*/


#ifndef _ObjStorageManager_H_
#define _ObjStorageManager_H_ 

#include <ISMStorageManager.h>
#include <dbPersistent.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;

_ACS_BEGIN_NAMESPACE(math)
class StatNode ;
_ACS_END_NAMESPACE 

/**
 * Class to deal with AWS S3 (or compatible) Object Storage manager.
 */
class ObjStorageManager : public ISMStorageManager // PRQA S 2109
{

public:
		
	struct StorageConfiguration
	{
		std::string provider;
		std::string key_ak;
		std::string key_sk;
		std::string region;
		std::string endpoint;
		bool endpointIsSecure ;
		std::string options;
		std::string downloadPoolSize;
		std::string downloadBufferSize;
		std::string uploadPoolSize;
		std::string uploadBufferSize;
		
		StorageConfiguration() ;
		StorageConfiguration(StorageConfiguration const &) ;
		~StorageConfiguration()  throw();
		StorageConfiguration & operator=(StorageConfiguration const &) ;
		
	} ;

	enum ArchiveMode {
		STATIC,
		HASH
	};
		
	enum BucketPartitioning {
		NO_BKT_PART,
		DATE_BKT_PART,
		FILETYPE_BKT_PART
	};
		
	struct StorageParameters // PRQA S 2171, 2173, 2175
	{
		std::string regExp ;
		std::string bucket ;
		ArchiveMode archiveMode ; 
		std::string archiveModeKey ; 
		std::string namingAlgoKey;

		StorageParameters() ;
		StorageParameters(StorageParameters const &) ;
		~StorageParameters()  throw();
		StorageParameters & operator=(StorageParameters const &) ;
	} ;
	

	
	/*! class ObjStorageManagerException declaration */
	exDECLARE_EXCEPTION(ObjStorageManagerException,ThreadException) ; // PRQA S 2131, 2153, 2502

	/*! class ObjSMUnknownFileException declaration */
	exDECLARE_EXCEPTION(ObjSMUnknownFileException, dbDownloadFile::SMUnknownFileException) ; // PRQA S 2131, 2153, 2502

	/** smid is the primary key into the t_storagemanager,
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 */
	ObjStorageManager(const std::string &smtype, dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &) ;
	virtual ~ObjStorageManager() throw() ;

	/**
	 *
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 * path is where to put the file.
	 */
	
	virtual void download(dbPersistent::IdType sminvid, const std::string &localpath, const dbPersistentOperatorIF&);

	virtual void erase(dbPersistent::IdType sminvid, const dbPersistentOperatorIF&, std::string* p=0) ;
	
	virtual void getAvailableStorageInfo(std::vector<std::pair<std::string, long long int> > &v) ;
		
	virtual bool hasTreeManagement() const ;
	
	static ArchiveMode getArchiveModeEnum(const std::string & archiveMode) ;
	static std::string getArchiveModeString(const ArchiveMode & ) ;
	
	static BucketPartitioning getBucketPartitioningEnum(const std::string & bucketPartitioning) ;
	static std::string getBucketPartitioningString(const BucketPartitioning & ) ;
private:
	// declared but not implemented to prevent their use
	ObjStorageManager() ;
	ObjStorageManager(const ObjStorageManager &) ;
	ObjStorageManager &operator=(const ObjStorageManager &) ;
	
	virtual void run();
    static math::StatNode &statN() ;

#ifdef ACS_TEST
public:
#else
private:
#endif
	
	std::string buildSmPath(const std::string & invFilename, const std::string & filepath, std::string & bucket, std::string & originalFilename) const ;
	
	std::string getDateFolder() const;
	std::string getHashFolder(const std::string & exeName, const std::string & filePath) const ;
	
	void getParameters();
	
private:
	
	StorageConfiguration _storageConfiguration;
	std::vector<StorageParameters> _storageParametersV;
	std::string _signatureAlgo;
	bool _computeSignature ;
	bool _checkSignature;
	int _retryingInterval ;
	int _retryingTimeout ;
	ACS_SSL::EncryptionAlgo _pwdEncryptionAlgo ;
	BucketPartitioning _bucketPartitioning ;
	unsigned long int _hashtagComputationTimeout ; // seconds
	std::string _invRelationTable;
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ObjStorageManager) ;
} ; 

std::ostream &operator<<(std::ostream &s, const ObjStorageManager::StorageConfiguration & v) ;

exostream &operator<<(exostream &s, const ObjStorageManager::StorageConfiguration & v);

std::ostream &operator<<(std::ostream &s, const ObjStorageManager::StorageParameters & v) ;

exostream &operator<<(exostream &s, const ObjStorageManager::StorageParameters & v);

template<>
void rsResourceSet::getValue(const std::string &res, ObjStorageManager::StorageParameters &v) const ;

std::ostream &operator<<(std::ostream &s, const ObjStorageManager::BucketPartitioning & v) ;

exostream &operator<<(exostream &s, const ObjStorageManager::BucketPartitioning & v);

_ACS_END_NAMESPACE


#endif // _ObjStorageManager_H_
