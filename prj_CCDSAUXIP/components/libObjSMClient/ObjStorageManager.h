// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
	

*/


#ifndef _ObjStorageManager_H_
#define _ObjStorageManager_H_ 

#include <SMInterface.h>
#include <dbDownloadFile.h>
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
class ObjStorageManager : public SMInterface // PRQA S 2109
{

public:
		
	class Simulate {
	public:
		Simulate() ;
		~Simulate() throw() ;
		Simulate(const Simulate &) {}
		Simulate& operator=(const Simulate &) { return *this ;} // PRQA S 4072
		bool simulate() const ;
	private:
		ACS_CLASS_DECLARE_DEBUG_LEVEL(Simulate) ;
	} ;

		
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
	 * ddl is a flag so that if valorized it manages OBJDDLSM Storage manager types
	 */
	ObjStorageManager(const std::string &smtype, dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &, bool ddl = false) ;
	virtual ~ObjStorageManager() throw() ;
	
	/**
	 *
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 * path is where to put the file.
	 */
	
	virtual void download(dbPersistent::IdType sminvid, const std::string &localpath, const dbPersistentOperatorIF&);

	virtual void upload(const std::string &path, const dbPersistentOperatorIF&);
	
	virtual void erase(dbPersistent::IdType sminvid, const std::string &fname, const std::string &fvers, std::string* p=0) ;

	[[deprecated("Use erase(dbPersistent::IdType sminvid, const std::string &fname, const std::string &fvers, std::string* p=0) instead.")]]
	virtual void erase(dbPersistent::IdType sminvid, const dbPersistentOperatorIF&, std::string* p=0) ;
	
	virtual void getAvailableStorageInfo(std::vector<std::pair<std::string, long long int> > &v) ;
	
	/**
	 * \brief If a specific file is UNKNOWN for the storage manager, the link between inventory and the archive handler (t_inventorysm record) can be either removed or not depending on configuration (default do not remove).
     * \returns true/false: the link can/cannot be removed 
	 */
	virtual bool shallRemoveUnknownLinks () ;
	
	virtual bool uploading();
	virtual bool isUploadSynchronous() const ;
	virtual bool isDownloadSynchronous() const ;
	virtual bool isEraseSynchronous() const ;
		
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
	bool _readConfiguration ;
	bool _removeUnknownLinks ;
	bool _ddl ; 
	Simulate _check ;

		
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
