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


#include <ObjStorageManager.h>
			
#include <StorageFileNamingAlgoSingleton.h>
#include <StorageFileNamingAlgoFactory.h>
						
#include <AwsS3Wrapper.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SignatureAlgoFactory.h>
#include <SignatureAlgoScopeGuard.h>
#include <SignatureAlgo.h>
#include <dbPersistentOperatorIF.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <ConfigurationSingleton.h>

#include <SpawnProc.h>
#include <File.h>
#include <RegExp.h>
#include <rsPushNSpace.h>
#include <MCFilterables.h>
#include <Statistics.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::mc ;

namespace {
	
	enum Operation {
		UPLOAD ,
		DOWNLOAD,
		REMOVE
	};

	
	class ObjWorker
	{
	public:
			
		exDECLARE_EXCEPTION(ObjWorkerException,exException) ; // PRQA S 2131, 2153, 2502

		ObjWorker(  const string & smpath,
					const string & bucket,
					const ObjStorageManager::StorageConfiguration &,
					const string & localpath = string() );
		virtual ~ObjWorker() throw() {}
		void doWork( Operation, const ACS_SSL::EncryptionAlgo &, int retryingInt, int retryTimeout, const string & originalFilename = string() );
		
		string getStorageConfigurationEndPoint () const { return _storageConfiguration.endpoint ; }
		string getStorageConfigurationRegion () const { return _storageConfiguration.region ; }
	private:
		ObjWorker() ; // not implemented
		ObjWorker(const ObjWorker &) ; // not implemented
		ObjWorker& operator= (const ObjWorker &) ; // not implemented
		
		string getDecryptedPassword(const string &p, const ACS_SSL::EncryptionAlgo &) const ;
		
	private:
		string _smpath;
		string _bucket;
		ObjStorageManager::StorageConfiguration _storageConfiguration ;
		string _localpath;

		ACS_CLASS_DECLARE_DEBUG_LEVEL(ObjWorker) ;


	} ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ObjWorker)
		
ObjWorker::ObjWorker( string const & smpath,
					string const & bucket,
					const ObjStorageManager::StorageConfiguration & sc,
					string  const & localpath ):
    _smpath(smpath),
	_bucket(bucket),
	_storageConfiguration(sc),
	_localpath(localpath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
}

string ObjWorker::getDecryptedPassword(const string & password, const ACS_SSL::EncryptionAlgo & pwdEncryptionAlgo) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
 	string awsPassword = password;
	const char* encPassword = awsPassword.c_str() ;
	char decPassword [1024] ;  // PRQA S 4403
	memset( decPassword, 0, sizeof(decPassword) ) ;	/* clear buffer */
   
	// Get decPassword from encPassword  
	{
		vector <void*> vin ;
        vector <void *> vout ;
		BF_KEY bfkey ; // PRQA S 4102
		unsigned char key_in[] = { 'm', 'a', 'n', 'u', 'e', 'l' } ;	/* Blowfish passphrase. Provided by RobRic */
		int key_len = 6 ;
		
		ACS_SSL::ACS_BLOWFISH_setkey( key_in, key_len, &bfkey) ;	/* Get Actual key */
		vin.push_back ( &bfkey ) ;
		
		ACS_COND_THROW( strlen(encPassword)>sizeof(decPassword),exIllegalValueException("ftp password too long") ) ; // PRQA S 3081
   		
		size_t s = ACS_SSL::ACS_ALGO_decode( pwdEncryptionAlgo, 
                                             reinterpret_cast<const unsigned char*>(encPassword), // PRQA S 3030, 3081
                                             strlen(encPassword), 
                                             reinterpret_cast<unsigned char*>(decPassword),  // PRQA S 3030, 3081
                                             vin, 
                                             vout ) ;
		ACS_COND_THROW( s < 0, exIllegalValueException("password decoding failure") ) ; // PRQA S 3081
   
    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Password decrypted successfully.")  ;
#if(0)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Decrypted password: \"" << decPassword << "\"")  ;
#endif  
	}
	
	if( 0 != decPassword[0] ) {
		awsPassword = decPassword ;
    }
	
	return awsPassword;
}

void ObjWorker::doWork( Operation op, const ACS_SSL::EncryptionAlgo & pwdEncryptionAlgo,  int retryingInt, int retryTimeout, const string & originalFilename )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
	
	string awsPassword = getDecryptedPassword(_storageConfiguration.key_sk, pwdEncryptionAlgo);
	
	AwsS3Wrapper::Enum_S3Provider S3Provider = AwsS3Wrapper::to_S3Provider(_storageConfiguration.provider) ;
	Aws::String key_ak, key_sk ;

	const bool credentials_set ( (false == _storageConfiguration.key_ak.empty()) && (false == awsPassword.empty() ) ) ;

	if (true == credentials_set) {
		// Set AK/SK
		key_ak = AwsS3Wrapper::to_AwsString( _storageConfiguration.key_ak ) ; 
		key_sk = AwsS3Wrapper::to_AwsString( awsPassword ) ; 
	}

	const Aws::Auth::AWSCredentials credentials ( (false == credentials_set)? Aws::Auth::AWSCredentials() : Aws::Auth::AWSCredentials(key_ak, key_sk) ) ;

	string aws_s3_region = _storageConfiguration.region;
	string aws_s3_endpoint = _storageConfiguration.endpoint;
	ACS_SMARTPTR<AwsS3Wrapper> s3handler_p ;
	StopController localStopController ;
	if ( false == aws_s3_endpoint.empty() ) {
		s3handler_p = ACS_SMARTPTR<AwsS3Wrapper>( new AwsS3Wrapper(S3Provider, credentials, aws_s3_region, aws_s3_endpoint, localStopController) ) ; 
		_storageConfiguration.region = s3handler_p->get_region() ; 
	}
	else if ( false == aws_s3_region.empty() )  {	
		s3handler_p = ACS_SMARTPTR<AwsS3Wrapper>( new AwsS3Wrapper(S3Provider, credentials, aws_s3_region, localStopController) ) ; 
		_storageConfiguration.endpoint = s3handler_p->get_endpoint() ; 
	}
	else {	
		s3handler_p = ACS_SMARTPTR<AwsS3Wrapper>( new AwsS3Wrapper(S3Provider, credentials, localStopController) ) ;
		_storageConfiguration.endpoint = s3handler_p->get_endpoint() ; 
		_storageConfiguration.region = s3handler_p->get_region() ; 
	}


	bool tryOperation = true;	
	int currentRetryInt = 0	;
		
	while( tryOperation ) {

		int attempt = 1 + currentRetryInt / retryingInt;

		try {

			switch(op)
			{
            	case UPLOAD:
                	{

						multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> tagsMap;
						if( not originalFilename.empty() ) {
							tagsMap.insert( pair<AwsS3Wrapper::Enum_S3MetadataTag, string>(AwsS3Wrapper::S3TAG_TO_BE_RENAMED, originalFilename) ) ;
						}
						
						if( not _storageConfiguration.uploadPoolSize.empty() ) {
							unsigned int uploadPoolSize ;
							istringstream iss ( _storageConfiguration.uploadPoolSize, istringstream::in);
							iss >> uploadPoolSize ;
							s3handler_p->set_upload_pool_size(uploadPoolSize);
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Upload Pool Size set to " << uploadPoolSize );
						}

						if( not _storageConfiguration.uploadBufferSize.empty() ) {
							unsigned int uploadBufferSize ;
							istringstream iss ( _storageConfiguration.uploadBufferSize, istringstream::in);
							iss >> uploadBufferSize ;
							s3handler_p->set_upload_buffer_size(uploadBufferSize);
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Upload Buffer Size set to " << uploadBufferSize );
						}
						
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to upload=\"" << _localpath << "\" on bucket=\"" << _bucket << "\" remote path=\""
								 			<< _smpath << "\" original name=\"" << originalFilename << '\"') ;
						s3handler_p->uploadObject( _localpath, _bucket, _smpath, tagsMap ) ; 

						tryOperation = false;
                    	break;
                	}
            	case DOWNLOAD:
                	{
						
						if( not _storageConfiguration.downloadPoolSize.empty() ) {
							unsigned int downloadPoolSize ;
							istringstream iss ( _storageConfiguration.downloadPoolSize, istringstream::in);
							iss >> downloadPoolSize ;
							s3handler_p->set_download_pool_size(downloadPoolSize);
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Download Pool Size set to " << downloadPoolSize );
						}

						if( not _storageConfiguration.downloadBufferSize.empty() ) {
							unsigned int downloadBufferSize ;
							istringstream iss ( _storageConfiguration.downloadBufferSize, istringstream::in);
							iss >> downloadBufferSize ;
							s3handler_p->set_download_buffer_size(downloadBufferSize);
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Download Buffer Size set to " << downloadBufferSize );
						}
						
						ACS_COND_THROW( not s3handler_p->isFileObject(_bucket, _smpath), ObjStorageManager::ObjSMUnknownFileException("Cannot found \""+_smpath+"\" path on \""+_bucket+"\" bucket") ) ;
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to download=\"" << _smpath << "\" on bucket=\"" << _bucket << "\" into \"" << _localpath << "\" path" );
						s3handler_p->downloadObject(_bucket, _smpath, _localpath) ;
						tryOperation = false;
                    	break;
                	}
				case REMOVE:
        			{
						// Remove from AWS S3, if the entry exists	
  						if ( true == s3handler_p->isFileObject(_bucket, _smpath) ) 
  						{
 							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to delete \"" << _smpath << "\" path from bucket=\"" << _bucket << '\"' );
 							s3handler_p->removeSingleObject(_bucket, _smpath) ;
  						}
						else { // just for debug
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << _smpath << "\" path does not exist on bucket=\"" << _bucket << '\"' );
						}
						tryOperation = false;
						break;
        			}
				default:
        			{
						ostringstream exMsg;
						exMsg << "Internal ERROR: transfer mode unrecognized (" << int(op) << ")" ; // PRQA S 3081 2
						ACS_THROW( exIllegalValueException( exMsg.str() ) );
        			}			
			
			}

			if( !tryOperation ) {
    			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "attempt number " << attempt << "; operation successfull") ; 					
				break;
			}

		}
		catch( AwsS3Wrapper::AwsS3WrapperConnectionException &e )
		{
			ostringstream msg	;
			msg << SimpleDebugSignature
				<< " exception caught: " << e.what() << "\n"
				<< " bucket = \"" << _bucket << "\"\n"
				<< " remote full file name  = \"" << _smpath << "\"\n"
				<< " local full file name  = \"" << _localpath << "\"\n"
				<< " attempt number " << attempt << "; " ;
			if( (currentRetryInt+retryingInt) > retryTimeout ) {
				ACS_LOG_WARNING("TIMEOUT EXPIRED: " << msg.str()) ;
				throw;
			}

			msg << "going to wait " << retryingInt << " seconds and then try again";
			ACS_LOG_WARNING(msg.str()) ;

			sleep(retryingInt); // PRQA S 3000
			currentRetryInt += retryingInt;
		}
		catch( exception &e )
		{
			throw;	
		}
	} // while( tryOperation ) 
		
}



class StorageParametersFounder {
    public:
        StorageParametersFounder(const std::string & filename) : 
								_filename(filename)
        {}
        
        StorageParametersFounder(const StorageParametersFounder &r) : _filename(r._filename) {} // PRQA S 2528
        ~StorageParametersFounder() throw() {}
		bool operator()(const ObjStorageManager::StorageParameters &p)  
        {
			RegExp theRegExp (p.regExp);
			return theRegExp.match(_filename);
        }
    private:
        StorageParametersFounder() ; // not implemented
        StorageParametersFounder& operator=(const StorageParametersFounder &) ; // not implemented
		
		string _filename;
    
} ;

static const string defaultHashExeName = "xxhsum -q";

	
} // end unnamed namespace

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ObjStorageManager::StorageConfiguration::StorageConfiguration() :
	provider(),
	key_ak(),
	key_sk(),
	region(),
	endpoint(),
	endpointIsSecure(false),
	options(),
	downloadPoolSize(),
	downloadBufferSize(),
	uploadPoolSize(),
	uploadBufferSize()

{}


ObjStorageManager::StorageConfiguration::StorageConfiguration(StorageConfiguration const & other):
										provider(other.provider),
										key_ak(other.key_ak),
										key_sk(other.key_sk),
										region(other.region),
										endpoint(other.endpoint),
										endpointIsSecure(other.endpointIsSecure),
										options(other.options),
										downloadPoolSize(other.downloadPoolSize),
										downloadBufferSize(other.downloadBufferSize),
										uploadPoolSize(other.uploadPoolSize),
										uploadBufferSize(other.uploadBufferSize)

{
}


ObjStorageManager::StorageConfiguration::~StorageConfiguration() throw()
{}


ObjStorageManager::StorageConfiguration & ObjStorageManager::StorageConfiguration::operator=(StorageConfiguration const &other)
{
	if ( this != &other)
	{
		provider = other.provider;
		key_ak = other.key_ak;
		key_sk = other.key_sk;
		region = other.region;
		endpoint = other.endpoint;
		endpointIsSecure = other.endpointIsSecure;
		options = other.options;
		downloadPoolSize = other.downloadPoolSize;
		downloadBufferSize = other.downloadBufferSize;
		uploadPoolSize = other.uploadPoolSize ;
		uploadBufferSize = other.uploadBufferSize ;
	}
	return *this ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ObjStorageManager::StorageParameters::StorageParameters() :
	regExp(),
	bucket(),
	archiveMode(),
	archiveModeKey(),
	namingAlgoKey()
{}


ObjStorageManager::StorageParameters::StorageParameters(StorageParameters const & other):
										regExp(other.regExp),
										bucket(other.bucket),
										archiveMode(other.archiveMode),
										archiveModeKey(other.archiveModeKey),
										namingAlgoKey(other.namingAlgoKey)
{
}


ObjStorageManager::StorageParameters::~StorageParameters() throw()
{}


ObjStorageManager::StorageParameters & ObjStorageManager::StorageParameters::operator=(StorageParameters const &other)
{
	if ( this != &other)
	{
		regExp = other.regExp;
		bucket = other.bucket;
		archiveMode = other.archiveMode; 
		archiveModeKey = other.archiveModeKey; 
		namingAlgoKey = other.namingAlgoKey ;
	}
	return *this ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ACS_CLASS_DEFINE_DEBUG_LEVEL(ObjStorageManager) 
acs::math::StatNode &ObjStorageManager::statN() { return (*acs::math::Statistics::instance())()["ObjStorageManager"] ; }

ObjStorageManager::ObjStorageManager(const string &smtype, dbPersistent::IdType smid,const dbPersistent* obj, dbConnectionPool &pool) : 
    ISMStorageManager(smtype,smid,obj, pool),
	_storageConfiguration(),
	_storageParametersV(),
	_signatureAlgo(),
	_computeSignature(false),
	_checkSignature(false),
	_retryingInterval(5),
	_retryingTimeout(10),
	_pwdEncryptionAlgo(ACS_SSL::ENC_NONE),
	_bucketPartitioning(NO_BKT_PART),
	_hashtagComputationTimeout(600),
	_invRelationTable()
{
    statN()["instances"]+=1 ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Type "<< smtype << " ID " << smid ) ;

}

ObjStorageManager:: ~ObjStorageManager() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
}

void ObjStorageManager::download(dbPersistent::IdType sminvid, const string &localpath, const dbPersistentOperatorIF& oper)
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
	
	getParameters();
	
    Timer timer ;
    timer.start() ;
    statN()["download"]+=1 ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMId() " << SMId()) ;
	
	ACS_COND_THROW(getObject() == 0, ObjStorageManagerException("Base class _obj attr pointer attribute is NULL!")) ; // PRQA S 3081
	string filename = oper.getFileName(*getObject());
	string fileversion = oper.getFileVersion(*getObject());
	
	string smRemotepath; 
	try {
		
		ACS_COND_THROW( File::exists( localpath ), ObjStorageManagerException("Error: file \"" + localpath + "\" already exists.") ) ; // PRQA S 3081
		
		{
		// the same behaviour of IsmSoapClient
			string parentPath = File::getDirPath(localpath, true);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "parent path of local file \"" << parentPath << '\"') ;
			ACS_COND_THROW( not parentPath.empty() && not File::exists( parentPath ) , ObjStorageManagerException("Error: dir \"" + parentPath + "\" does not exist.") ) ; // PRQA S 3081
		}
	
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
        const string &dbKey = theConnection.getKey() ;
				
    	SQLString sqlstring( dbKey, SQLString::SELECT ) ;
        sqlstring.addSelectList("s3_path");
		if(_checkSignature) {
        	sqlstring.addSelectList("signature");
        }
        sqlstring.addTableList(_invRelationTable);
		{
			dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, _invRelationTable,"Id" ,  dbParam::EQUAL_TO, db::toSQL(dbKey, sminvid))) ; 
			sqlstring.addANDWhereParameter(pp) ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following query: " << sqlstring.getSQL()) ;
		dbQuery query(theConnection, sqlstring, "SelectPathSignature") ;	

		string storedSignature;
		dbRow myRow(dbKey);
		{
			dbSet set;
			query.execForSet(set) ;
			if( not set.getNextRow(myRow) ) {
				ostringstream exMsg;
				exMsg << "No " << _invRelationTable << " record found with id=" << sminvid;
				ACS_THROW(ObjStorageManagerException( exMsg.str() )); // PRQA S 3081
			}
		}

		myRow.column(0,smRemotepath);
		if(_checkSignature) {
			myRow.column(1,storedSignature);
        }
		
		string bucket;
		string smKey;
		AwsS3Wrapper::absolute_to_relative_path(smRemotepath, bucket, smKey) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << smRemotepath << "\" remote path disassembled as \"" << bucket << "\" \"" << smKey << '\"');

		// events represent just the SM I/F status, not the whole upload status
		ACS_LOG_EVENT("ObjSM_IF",EXFE_BUSY) ;

		ObjWorker worker(smKey, bucket, _storageConfiguration, localpath);

		Timer dwntimer;
		dwntimer.start();
		
		worker.doWork(DOWNLOAD, _pwdEncryptionAlgo, _retryingInterval, _retryingTimeout);
		
		dwntimer.stop();
		double jobTime = dwntimer.elapsed() * 0.001; // PRQA S 3011, 3084
		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("fileid", smRemotepath) << mc_param("smid", SMId()) << // PRQA S 3081
						mc_param("filesize", File::size(localpath)) << mc_param("jobtime", jobTime) <<
						 mc_metric(Metric_OUT) << MCCode ("SM") << MCFunction ("Archive")) ;


		// events represent just the SM I/F status, not the whole upload status
		ACS_LOG_EVENT("ObjSM_IF",EXFE_IDLE) ;
        
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "download from Obj SM successful.") ;

		/////// Check Signature ////////
		if(_checkSignature)
		{
            Timer sig_timer ;
            sig_timer.start() ;
			SignatureAlgoFactory* factory = SignatureAlgoFactory::instance();
			SignatureAlgo* algo = factory->getAlgo(_signatureAlgo, localpath);
			SignatureAlgoScopeGuard guard(algo);
			if (!algo) {
				ACS_THROW(SignatureAlgoFactory::NotFoundException("No SignatureAlgo got with name " + _signatureAlgo  )) ; // PRQA S 3081
			}
			string signature = algo->computeSignature();
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed signature: " << signature << " Stored signature: " << storedSignature) ;
			
            sig_timer.stop() ;
            statN()["download"]["signature"]["elapsed [ms]"]+= double(sig_timer.elapsed()) ; // PRQA S 3081
			
			if(signature!=storedSignature) {
				ostringstream exMsg;
				exMsg << "Check Signature Failed for file \"" << filename << "\" version \"" << fileversion + "\". Algo is \"" << _signatureAlgo 
						<< "\", stored signature \"" << storedSignature << "\".";
				ACS_THROW(ObjStorageManagerException( exMsg.str() )); // PRQA S 3081
			}
		
		}
		
	}
    catch (ObjSMUnknownFileException& ex)
    {
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", filename) << mc_param("fileversion", fileversion) // PRQA S 3081, 3380, 3385
						<< mc_param("fileid", smRemotepath) << mc_param("smid", SMId())
						<< mc_param("message", "File unknown to ISM.") << mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;

        statN()["download"]["unknown"]+= 1 ; // PRQA S 3081
        ACS_LOG_EVENT("ObjSM_IF",EXFE_ERROR) ;
        ACS_THROW(ObjSMUnknownFileException (ex, "File \"" + filename + "\" version \"" + fileversion + "\" unknown to Obj SM.")); // PRQA S 3081
    }
    catch(exception &ex)
    {
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", filename) << mc_param("fileversion", fileversion)  // PRQA S 3081, 3380, 3385
						<< ( smRemotepath.empty() ? mc_null_param() : mc_param("fileid", smRemotepath) ) << mc_param("smid", SMId())
						<< mc_param("message", string("Downloading error ") + ex.what()) << mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;
		
        statN()["download"]["error"]+= 1 ; // PRQA S 3081
        ACS_LOG_EVENT("ObjSM_IF",EXFE_ERROR) ;
        ACS_THROW(ObjStorageManagerException(ex, "Error downloading to path \"" + localpath + "\" from Obj SM")); // PRQA S 3081
    }
    timer.stop() ;
    statN()["download"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081
	
}	

void ObjStorageManager::run()
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
	
	getParameters();
	
    Timer timer ;
    timer.start() ;
    statN()["upload"]+=1 ;
	
	ACS_COND_THROW(this->getOper() == 0, ObjStorageManagerException("dbPersistentOperatorIF base class pointer attribute is NULL!")) ; // PRQA S 3081
	string filename = this->getOper()->getFileName(*getObject());
	string fileversion = this->getOper()->getFileVersion(*getObject());
	
	if (this->getSimulationChecker().simulate()) {
		ACS_LOG_WARNING("Obj SM in simulation mode. ") ;
	}
	
	string path = getPath(); // from base class
	off_t file_size = File::size(path);
	try {	
		
		/////// Compute Signature ////////
		string signature;
		if(_computeSignature)
		{
            Timer sig_timer ;
            sig_timer.start() ;

			SignatureAlgoFactory* factory = SignatureAlgoFactory::instance();
			SignatureAlgo* algo = factory->getAlgo(_signatureAlgo,path);
			SignatureAlgoScopeGuard guard(algo);
			if (!algo) {
				ACS_THROW(SignatureAlgoFactory::NotFoundException("No SignatureAlgo got with name \"" + _signatureAlgo + '\"' )) ; // PRQA S 3081
			}
			signature = algo->computeSignature();
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed signature: \"" << signature << '\"') ;
            sig_timer.stop() ;
            statN()["upload"]["signature"]["elapsed [ms]"]+= double(sig_timer.elapsed()) ; // PRQA S 3081
		}
		
		string bucket;
		string originalFilename ; 
		string smpath = buildSmPath(filename, path, bucket, originalFilename);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SM path is \"" <<  smpath << "\"; bucket is \"" << bucket << "\"; original filename is \"" << originalFilename << '\"') ;
		string storageConfigurationEndPoint ;

		try {
			ACS_LOG_EVENT("ObjSM_IF",EXFE_BUSY) ;
			
			if ( not this->getSimulationChecker().simulate() ) {
				//finally upload file.
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Before upload in Obj SM: Path \"" << path << "\" smpath \"" << smpath << "\" bucket \"" << bucket 
													<< "\" original filename \"" << originalFilename << '\"') ;
				
				ObjWorker worker(smpath, bucket, _storageConfiguration, path);
				
				Timer upltimer;
				upltimer.start();
				worker.doWork(UPLOAD, _pwdEncryptionAlgo, _retryingInterval, _retryingTimeout, originalFilename);
				storageConfigurationEndPoint = worker.getStorageConfigurationEndPoint();
				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Storage configuration end point: \"" << storageConfigurationEndPoint << '\"') ;
				upltimer.stop();
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "after put in Obj SM") ;
				
				double jobTime = upltimer.elapsed() * 0.001; // PRQA S 3011, 3084
				
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("fileid", smpath) << mc_param("smid", SMId()) // PRQA S 3081
								<< mc_param("filesize", file_size) << mc_param("jobtime", jobTime)
								<< mc_metric(Metric_IN) << MCCode ("SM") << MCFunction ("Archive")) ;
				
			}
			else {
				ACS_LOG_WARNING("Simulating Obj SM upload of file \"" << path << "\"") ;
				// simulate an upload delay proportional to the file size as speed 
                // would be 10MB/sec
				usleep(file_size/10) ; // PRQA S 3000, 3010, 4400
			}
			
			// events represent just the SM I/F status, not the whole upload status
			ACS_LOG_EVENT("ObjSM_IF",EXFE_IDLE) ;
		}
		catch(exception &) {	
			ACS_LOG_EVENT("ObjSM_IF",EXFE_ERROR) ;
			throw ;
		}

		string smKey ;
		AwsS3Wrapper::relative_to_absolute_path(bucket, smpath, smKey, true ) ;
		try {		
            
        	dbConnectionWrapper pw(getPool()) ;
        	dbConnection &myconn = pw ; // PRQA S 3050
	    	const string &dbKey = myconn.getKey() ;
		
        	db::Transaction transaction(myconn) ;
			transaction.begin_work("INS InvRel") ;
			
            dbPersistent::IdType newID =  insertInvSM(myconn, this->getOper()->getId(*getObject())) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "new " << getInvSmTableName() << " record just inserted; ID=" << newID) ;
			
			SQLString sqlTAB( dbKey, SQLString::INSERT ) ;
        	sqlTAB.addTableList(_invRelationTable);
	       	// Add Values
            sqlTAB.addValuesList("id", db::toSQL(dbKey, newID) ) ; 
			
			string httpPath = StringUtils::pathJoin(storageConfigurationEndPoint, bucket, smpath);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http path=\"" << httpPath << '\"');
            sqlTAB.addValuesList("http_path", db::toSQL(dbKey, httpPath) ) ;
			
 			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Sm Key=\"" << smKey << '\"');
            sqlTAB.addValuesList("s3_path", db::toSQL(dbKey, smKey) ) ;
			
			if(_computeSignature) {
            	sqlTAB.addValuesList("signature", db::toSQL(dbKey, signature) ) ; // PRQA S 3050, 3081
            }
			
			dbQuery queryTAB(myconn, sqlTAB, "InsertSMInventoryRel") ;	
			if (!queryTAB.execForStatus()) {
			   ACS_THROW( dbQuery::QueryExecuteException("Cannot insert into " + _invRelationTable + ": Command : " + sqlTAB.getSQL() + " Error : " + queryTAB.errorString()) ) ;
			}
 			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "new " << _invRelationTable << " record created") ;
           
            transaction.commit() ;
		}
		catch (exception &)
		{
			//Erase the file from SM.
			try{
				ObjWorker worker(smpath, bucket, _storageConfiguration);
				worker.doWork(REMOVE, _pwdEncryptionAlgo, _retryingInterval, _retryingInterval);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "An error occurred, \"" + smpath + "\" file removed from Obj SM"); 
			}
			catch(exception &e)
			{
				ACS_LOG_OPALERT("Probable wrong remotion of \"" + smpath + "\" from Obj SM: please check, exception follows.") ;
			    ACS_LOG_NOTIFY_EX(e) ;
            }

			throw;
		}
			
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Obj SM uploading done. Reference = \"" << smKey << '\"');
	}
	catch(exception &be)
	{
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("smid", SMId()) << // PRQA S 3081
						 mc_param("message", string("Uploading error ") + be.what() ) <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("SM") << MCFunction ("Archive")) ;
		
        statN()["upload"]["error"]+= 1 ; // PRQA S 3081
		ostringstream os1 ;
		os1 << "Uploading inventory: " << filename 
		   << " version " << fileversion 
		   << " on SmId " << SMId() << " failed from path: " << path ;
		ACS_THROW(ObjStorageManagerException (be, os1.str())) ; // PRQA S 3081
	}
    timer.stop() ;
    statN()["upload"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081 3
    statN()["upload"]["size [Kb]"]+= file_size/1024.0 ; // PRQA S 3011, 4400
    statN()["upload"]["ok"]+= 1 ;

}	

void ObjStorageManager::erase(dbPersistent::IdType sminvid, const dbPersistentOperatorIF& oper,string*) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
	
	getParameters();
	
    Timer timer ;
    timer.start() ;
    statN()["erase"]+= 1 ;


    dbConnectionWrapper pw(getPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	string smpath;
	
	ACS_COND_THROW(getObject() == 0, ObjStorageManagerException("Base class _obj attr pointer attribute is NULL!")) ; // PRQA S 3081
	string filename = oper.getFileName(*getObject());
	string fileversion = oper.getFileVersion(*getObject());

	try {	
	
    	SQLString sqlstring( dbKey, SQLString::SELECT ) ;
        sqlstring.addSelectList("s3_path");
        sqlstring.addTableList(_invRelationTable);
		{
			dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, _invRelationTable,"Id" ,  dbParam::EQUAL_TO, db::toSQL(dbKey, sminvid))) ; 
			sqlstring.addANDWhereParameter(pp) ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << sqlstring.getSQL());

		dbQuery query(theConnection, sqlstring, "SelectSMPath");

		dbRow myRow(dbKey);
		{
			dbSet set;
			query.execForSet(set) ;
			if( not set.getNextRow(myRow) ) {
				ostringstream exMsg;
				exMsg << "No " << _invRelationTable << " record found with id=" << sminvid;
				ACS_THROW(ObjStorageManagerException( exMsg.str() )); // PRQA S 3081
			}
		}
		myRow.column(0, smpath);
		
		string bucket;
		string smKey;
		AwsS3Wrapper::absolute_to_relative_path(smpath, bucket, smKey) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << smpath << "\" remote path disassembled as \"" << bucket << "\" \"" << smKey << '\"');

		// events represent just the SM I/F status, not the whole upload status
		ACS_LOG_EVENT("ObjSM_IF",EXFE_BUSY) ;
		
		ObjWorker worker(smKey, bucket, _storageConfiguration);
		worker.doWork(REMOVE, _pwdEncryptionAlgo, _retryingInterval, _retryingTimeout);

		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("fileid", smpath) << mc_param("smid", SMId()) // PRQA S 3081
						 << mc_metric(Metric_DELETE) << MCCode ("SM") << MCFunction ("Archive")) ;
	
		// events represent just the SM I/F status, not the whole upload status
		ACS_LOG_EVENT("ObjSM_IF",EXFE_IDLE) ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "erasing done.") ;
	} //try
	catch(exception &ex)
	{
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("fileid", smpath) << mc_param("smid", SMId()) // PRQA S 3081
							<< mc_param("message", string("Erase error ") + ex.what()) 
							<< mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;
        ACS_LOG_EVENT("ObjSM_IF",EXFE_ERROR) ;
		ACS_THROW(ObjStorageManagerException(ex, "Error erasing file \"" + filename + "\" version \"" + fileversion + "\".")) ;
	}
		
	try
	{
        deleteInvSM(theConnection,sminvid ) ;
	}
	catch (exception &e)
	{
		ostringstream mcMessage;
		mcMessage << getInvSmTableName() << " record[" << sminvid << "] deletion failed ";
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("fileid", smpath) << mc_param("smid", SMId()) // PRQA S 3081
							<< mc_param("message", mcMessage.str() + e.what()) 
							<< mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;
		ostringstream os1 ;
		os1 << getInvSmTableName() << " record[" << sminvid << "] deletion failed. Inventory could be incoherent for SmId." << SMId() 
		   << " inventory["<< oper.getId(*getObject()) <<"]: " << filename << " version " << fileversion ;
		ACS_THROW(ObjStorageManagerException(e, os1.str() )) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getInvSmTableName() << " record deletion done. Id = \"" << sminvid <<"\" .") ;
    timer.stop() ;
    statN()["erase"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081
	
}

void ObjStorageManager::getAvailableStorageInfo(vector<pair<string, long long int> > &v) 
{
	ACS_THROW(ObjStorageManagerException("Method still not implemented!")) ;
}

bool ObjStorageManager::hasTreeManagement() const 
{
	return false ;
}


string ObjStorageManager::buildSmPath(const string & invFilename, const string & filepath, string & bucket, string & originalFilename) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631

	vector<StorageParameters>::const_iterator it = find_if (_storageParametersV.begin(), _storageParametersV.end(), StorageParametersFounder(invFilename)) ;
	ACS_COND_THROW( it == _storageParametersV.end(), ObjStorageManagerException( '\"'+invFilename+"\" filename match no configured regular expression" ) );
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Storage parameters found for \"" << invFilename << "\" filename:\n" << *it);
	bucket = it->bucket;	
	
	string smpath("/");
	switch (_bucketPartitioning)  {
    	case NO_BKT_PART: 
			{ 
				break;
			}
		case DATE_BKT_PART: 
			{ 
				smpath = StringUtils::pathJoin( smpath, getDateFolder() );
				break;
			}
		case FILETYPE_BKT_PART: 
			{ 
				smpath = StringUtils::pathJoin( smpath, this->getOper()->getFileType(*getObject()) );
				break;
			}
		default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: bucket partitioning (" << int(_bucketPartitioning) << ") unrecognized"; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
	}
	
	
	
	switch (it->archiveMode)  {
    	case STATIC: 
			{ 
				smpath = StringUtils::pathJoin( smpath, it->archiveModeKey );
				break;
			}
		case HASH: 
			{ 
				string hashExeName = it->archiveModeKey ;
				if( hashExeName.empty() ) {
					hashExeName = defaultHashExeName ;
				}
				smpath = StringUtils::pathJoin( smpath, getHashFolder(hashExeName, filepath) );
				break;
			}
		default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: archive mode (" << int(it->archiveMode) << ") unrecognized"; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Archive mode \"" << getArchiveModeString(it->archiveMode) << "\": smpath so far is \"" << smpath << '\"');

	// build a factory FileNamingAlgo
	StorageFileNamingAlgoFactory fileNamingAlgoFactory ;
	StorageFileNamingAlgo *storageFileNamingAlgo = StorageFileNamingAlgoSingleton::instance()->newStorageFileNamingAlgo(StringUtils::uppercase(it->namingAlgoKey), getObject() ) ;
	ACS_COND_THROW( ! storageFileNamingAlgo, ObjStorageManagerException("Can't Instantiate object with \"" + it->namingAlgoKey + "\" key: got NULL pointer from StorageFileNamingAlgoFactory") );	 // PRQA S 3081

	string filename;
	storageFileNamingAlgo->buildName( this->getOper(), filename, originalFilename ) ;

	smpath = StringUtils::pathJoin( smpath, filename );
	
	return smpath;
}


ObjStorageManager::ArchiveMode ObjStorageManager::getArchiveModeEnum(const string & archiveModeStr) // PRQA S 4020
{
	string localArchiveMode = StringUtils::uppercase(archiveModeStr);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to set archive mode corresponding to \"" << localArchiveMode << '\"') ;
	
	ArchiveMode archiveMode ;
	if( localArchiveMode == "STATIC" ) {
		archiveMode = STATIC;
    }
	else if( localArchiveMode == "HASH" ) {
		archiveMode = HASH;
    }
	else {
		ACS_THROW( ObjStorageManagerException( "Unrecognized archive mode: \"" + localArchiveMode + '\"') );
	}
	return archiveMode;
	
}

string ObjStorageManager::getArchiveModeString(const ObjStorageManager::ArchiveMode & archiveMode) // PRQA S 4020
{	
	string archiveModeString ;
	switch (archiveMode)  {
    	case STATIC: 
			{ 
				archiveModeString = "STATIC"; 
				break;
			}
		case HASH: 
			{ 
				archiveModeString = "HASH"; 
				break;
			}
		default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: archive mode (" << int(archiveMode) << ") unrecognized"; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
	}
	return archiveModeString	;
}


ObjStorageManager::BucketPartitioning ObjStorageManager::getBucketPartitioningEnum(const string & bucketPartitioningStr) // PRQA S 4020
{
	string localBucketPartitioning = StringUtils::uppercase(bucketPartitioningStr);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to set bucket partitioning corresponding to \"" << localBucketPartitioning << '\"') ;
	
	BucketPartitioning bucketPartitioning ;
	if( localBucketPartitioning == "NO_BKT_PART" ) {
		bucketPartitioning = NO_BKT_PART;
    }
	else if( localBucketPartitioning == "DATE_BKT_PART" ) {
		bucketPartitioning = DATE_BKT_PART;
    }
	else if( localBucketPartitioning == "FILETYPE_BKT_PART" ) {
		bucketPartitioning = FILETYPE_BKT_PART;
    }
	else {
		ACS_THROW( ObjStorageManagerException( "Unrecognized bucket partitioning: \"" + localBucketPartitioning + '\"') );
	}
	return bucketPartitioning;
	
}

string ObjStorageManager::getBucketPartitioningString(const ObjStorageManager::BucketPartitioning & bucketPartitioning) // PRQA S 4020
{	
	string bucketPartitioningStr ;
	switch (bucketPartitioning)  {
    	case NO_BKT_PART: 
			{ 
				bucketPartitioningStr = "NO_BKT_PART"; 
				break;
			}
		case DATE_BKT_PART: 
			{ 
				bucketPartitioningStr = "DATE_BKT_PART"; 
				break;
			}
		case FILETYPE_BKT_PART: 
			{ 
				bucketPartitioningStr = "FILETYPE_BKT_PART"; 
				break;
			}
		default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: bucket partitioning (" << int(bucketPartitioning) << ") unrecognized"; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
	}
	return bucketPartitioningStr	;
}


string ObjStorageManager::getDateFolder() const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
	DateTime::UTCD utcd = DateTime(); // PRQA S 3050
	string year;
	{
		ostringstream os;
		os << setfill('0') << setw(4) << utcd.year; // PRQA S 4400
		year = os.str();		
	}
	string month;
	{
		ostringstream os;
		os << setfill('0') << setw(2) << utcd.month; // PRQA S 4400
		month = os.str();		
	}
	string day;
	{
		ostringstream os;
		os << setfill('0') << setw(2) << utcd.day; // PRQA S 4400
		day = os.str();		
	}

	return ( year + '-' + month + '-' + day );
	
}

string ObjStorageManager::getHashFolder(const string & exeName, const string & filePath) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Input exe name=\"" << exeName << '\"');
	vector<string> commandTokens;
	StringUtils::Tokenize(StringUtils::trim(exeName), commandTokens, string(" "));
// add the filepath
	commandTokens.push_back(filePath);
	size_t commandTokensSize = commandTokens.size();
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ostringstream msg;
		msg << "Command: ";
		for(size_t i = 0; i < commandTokensSize; i++) {
			msg << commandTokens[i] << ' ';
		}
		ACS_LOG_DEBUG(SimpleDebugSignature << msg.str() );
	ACS_CLASS_END_DEBUG
	
	string command = commandTokens[0];		
	SpawnProc proc(command) ;
	for(size_t i = 1; i < commandTokensSize; i++) {
		proc.args()[i-1] = commandTokens[i];
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Command to be executed " << command );

	int outfiledes[2] ; // PRQA S 4403

	ACS_COND_THROW(pipe(outfiledes) != 0 , exIOException("Cannot compute hash on \""+filePath+"\": cannot open pipe")) ; // PRQA S 3081

	proc.setOutDescriptor(outfiledes[1]) ;
	proc.setErrDescriptor(outfiledes[1]) ;

	// outfiledes[0] will be closed by the ifdstream dtor
	string hashOut;
	{
		File::ifdstream hashOutStream(outfiledes[0]) ;

		proc.spawn();
		close(outfiledes[1]) ;
		
		Timer watchdog ;
		bool wdTimeout = false ;
		watchdog.start() ;

		unsigned long int hashTimeout = _hashtagComputationTimeout * 1000;
		while(proc.isRunning()) {
			Timer::delay(100); // PRQA S 4400
			if(watchdog.isOlder(hashTimeout)) { // PRQA S 3000, 4400
				proc.kill(); // it took more than timeout: kill it
				wdTimeout = true ;
			}
		}

		int exitCode = -1 ;
		bool goodExit=true;
		if( !proc.exitCode(exitCode) ) {
			goodExit=false;
    	}

		if(exitCode != 0) {
			ostringstream os ;
			if (wdTimeout) {
				os << watchdog.elapsed() / 1000 // PRQA S 3084, 4400
				   << " [sec] elapsed since " << command << " was started on \"" << filePath << "\" file path. It is bigger than the timeout (" << _hashtagComputationTimeout
					<< " [sec]). The process was killed. " ;
				ACS_THROW(exSystemException(os.str())) ; // PRQA S 3081
			}
			
			os << command << " command on \"" << filePath << "\" file path failed; exitCode: " << exitCode;
			int exitSignal = 0 ;
			proc.exitSignal(exitSignal) ;
			if( 0 != exitSignal ) {
				os << " exitSignal: " << exitSignal ;
			}
			ACS_THROW(exSystemException(os.str())) ; // PRQA S 3081
		}
		else if( not goodExit ) {
			ostringstream exMsg;
			exMsg << command << " command on \"" << filePath << "\" file path not terminated";
			ACS_THROW(exSystemException( exMsg.str() ));
		}
		else { // just for debug
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, command << " command on \"" << filePath << "\" file path successfull");
		}
		
		int n = 0;
		do {
			getline (hashOutStream, hashOut) ;
		} while( (hashOut.empty()) && (++n < 10) ); // to prevent and infinite loop
		ACS_COND_THROW( hashOut.empty(), ObjStorageManagerException("Empty output from "+command+" command on \""+filePath+"\" file path") );
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Read from hash out: " << hashOut );
	}
	
	string hashFolder;
	{
		istringstream is(hashOut);
		is >> hashFolder;
	}

	return hashFolder;	
}

void ObjStorageManager::getParameters() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string completepath = "/tmp/ObjSMConf";
	{
		ostringstream os;
		os << SMId();
		completepath += os.str();
	}
    string paramfile = File::tempname(completepath, "rs") ; // PRQA S 3050
	FileGuard paramfile_guard(paramfile) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "param file " << paramfile) ;
	
	rsResourceSet conf ;
	try {
		ISMStorageManager::getParameters(dbConnectionWrapper( getPool() ), paramfile, conf, _invRelationTable) ;
	}
	catch(exception &e) {
		ACS_THROW(ObjStorageManagerException(e,"unable to get parameters")) ; // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG( ACS_INS_VERB, "Configuration\n" << conf) ;
	//*********************   load parameters *********************************
	{
		
		rsPushNSpace rspn(conf, "SMConfiguration");
		
		conf.getValue ("Provider", _storageConfiguration.provider);
        if ( not conf.getValue ("Key_ak", _storageConfiguration.key_ak, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Key_ak key found.");
        }
        if ( not conf.getValue ("Key_sk", _storageConfiguration.key_sk, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Key_sk key found.");
        }
        if ( not conf.getValue ("Region", _storageConfiguration.region, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Region key found.");
        }
        if ( not conf.getValue ("Endpoint", _storageConfiguration.endpoint, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Endpoint key found.");
        }
        if ( not conf.getValue ("EndpointIsSecure", _storageConfiguration.endpointIsSecure, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.EndpointIsSecure key found. Set to default value: " << boolalpha << _storageConfiguration.endpointIsSecure);
        }
        if ( not conf.getValue ("Options", _storageConfiguration.options, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Options key found.");
        }
        if ( not conf.getValue ("Download_Pool_Size", _storageConfiguration.downloadPoolSize, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Download_Pool_Size key found.");
        }
        if ( not conf.getValue ("Download_Buffer_Size", _storageConfiguration.downloadBufferSize, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Download_Buffer_Size key found.");
        }
        if ( not conf.getValue ("Upload_Pool_Size", _storageConfiguration.uploadPoolSize, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Upload_Pool_Size key found.");
        }
        if ( not conf.getValue ("Upload_Buffer_Size", _storageConfiguration.uploadBufferSize, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.Upload_Buffer_Size key found.");
        }
		
		conf.getArray("ConfigurationParametersList.ConfigurationParameters", _storageParametersV);
		ACS_COND_THROW(_storageParametersV.empty(), ObjStorageManagerException("No ConfigurationParameters occurs in configuration space!") );
		
		try {
			conf.getValue("Signature.Algo", _signatureAlgo);
			conf.getValue("Signature.Compute", _computeSignature);
			conf.getValue("Signature.Check", _checkSignature);
		}
		catch(rsResourceSet::NotFoundException &)
		{
			ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Signature keys not found: no signature will be neither computed nor checked");
		}
		
		if ( not conf.getValue ("RetryingInterval", _retryingInterval, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.RetryingInterval key found. Set to default value: " << _retryingInterval << " s");
        }
		
		if ( not conf.getValue ("RetryingTimeout", _retryingTimeout, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.RetryingInterval key found. Set to default value: " << _retryingTimeout << " s");
        }
		
		{
			int algoId = 0;
			if ( not conf.getValue ("PasswordEncryptionAlgo", algoId, false)) {
            	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.PasswordEncryptionAlgo key found: ASSUMING UNENCRYPTED") ;
        	}
			else {
				_pwdEncryptionAlgo = static_cast<ACS_SSL::EncryptionAlgo>(algoId); // PRQA S 3013, 3081
			}
		}
		
		{
			string tmp;
			if ( not conf.getValue ("BucketPartitioning", tmp, false)) {
            	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.BucketPartitioning key found. Set to default value: " << _bucketPartitioning );
        	}
			else {
				_bucketPartitioning = getBucketPartitioningEnum(tmp) ;
			}
		}
		
		if ( not conf.getValue ("HashtagComputationTimeout", _hashtagComputationTimeout, false)) {
            ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "No ObjSMClient.HashtagComputationTimeout key found. Set to default value: " << _hashtagComputationTimeout << " s");
        }


	}
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg;
		msg << '\n' << _storageConfiguration;
		const size_t storageParametersVSize = _storageParametersV.size();
		for(size_t i = 0; i < storageParametersVSize; i++) {
			msg << _storageParametersV[i] ;
		}
		msg << "\tSignature Algo=\"" << _signatureAlgo << '\"'
			<< "\n\tSignature computation=" << boolalpha << _computeSignature
			<< "\n\tSignature check=" << boolalpha << _checkSignature
			<< "\n\tRetrying interval=" << _retryingInterval << " s"
			<< "\n\tRetrying timeout=" << _retryingTimeout << " s" 
			<< "\n\tPassword encryption algo=0x" << hex << int(_pwdEncryptionAlgo)
			<< "\n\tBucket partitioning=" << _bucketPartitioning 
			<< "\n\tHashtag computation timeout=" << dec <<_hashtagComputationTimeout << " s"
			<< "\n\tInv Relation table=\"" << _invRelationTable << '\"' ; // PRQA S 3081
		ACS_LOG_DEBUG(SimpleDebugSignature << msg.str() );
	ACS_CLASS_END_DEBUG
	
}


ostream &operator<<(ostream &s, const ObjStorageManager::StorageConfiguration & v)
{
	s	<< "************************* StorageConfiguration ************************\n"
		<< "\tprovider=\"" 				<< v.provider 						<< "\"\n"
		<< "\tkey_ak=\"" 				<< v.key_ak  						<< "\"\n"
		<< "\tkey_sk=\"" 				<< v.key_sk 						<< "\"\n"
		<< "\tregion=\"" 				<< v.region 						<< "\"\n"
		<< "\tendpoint=\"" 				<< v.endpoint 						<< "\"\n"
		<< "\tendpointIsSecure=" 		<< boolalpha << v.endpointIsSecure 	<< "\n"
		<< "\toptions=\"" 				<< v.options 						<< "\"\n"
		<< "\tdownloadPoolSize=\"" 		<< v.downloadPoolSize 				<< "\"\n"
		<< "\tdownloadBufferSize=\"" 	<< v.downloadBufferSize 			<< "\"\n"
		<< "\tuploadPoolSize=\"" 		<< v.uploadPoolSize 				<< "\"\n"
		<< "\tuploadBufferSize=\"" 		<< v.uploadBufferSize 				<< "\"\n"
		<< "*********************************************************************\n";
	
	return s ;
}

exostream &operator<<(exostream &s, const ObjStorageManager::StorageConfiguration & v)
{
	s	<< "************************* StorageConfiguration ************************\n"
		<< "\tprovider=\"" 				<< v.provider 						<< "\"\n"
		<< "\tkey_ak=\"" 				<< v.key_ak  						<< "\"\n"
		<< "\tkey_sk=\"" 				<< v.key_sk 						<< "\"\n"
		<< "\tregion=\"" 				<< v.region 						<< "\"\n"
		<< "\tendpoint=\"" 				<< v.endpoint 						<< "\"\n"
		<< "\tendpointIsSecure=" 		<< boolalpha << v.endpointIsSecure 	<< "\n"
		<< "\toptions=\"" 				<< v.options 						<< "\"\n"
		<< "\tdownloadPoolSize=\"" 		<< v.downloadPoolSize 				<< "\"\n"
		<< "\tdownloadBufferSize=\"" 	<< v.downloadBufferSize 			<< "\"\n"
		<< "\tuploadPoolSize=\"" 		<< v.uploadPoolSize 				<< "\"\n"
		<< "\tuploadBufferSize=\"" 		<< v.uploadBufferSize 				<< "\"\n"
		<< "*********************************************************************\n";
	
	return s ;
}

ostream &operator<<(ostream &s, const ObjStorageManager::StorageParameters & v)
{
	s	<< "*********************************** StorageParameters ****************************************\n"
		<< "\tregExp=\"" 		<< v.regExp 														<< "\"\n"		 
		<< "\tbucket=\"" 		<< v.bucket  														<< "\"\n"		 
		<< "\tarchiveMode=\"" 	<< ObjStorageManager::getArchiveModeString(v.archiveMode) 			<< "\"\n"
		<< "\tarchiveModeKey=\""<< v.archiveModeKey 												<< "\"\n"
		<< "\tnamingAlgoKey=\"" << v.namingAlgoKey 													<< "\"\n"
		<< "**********************************************************************************************\n";
	
	return s ;
}

exostream &operator<<(exostream &s, const ObjStorageManager::StorageParameters & v)
{
	s	<< "*********************************** StorageParameters ****************************************\n"
		<< "\tregExp=\"" 		<< v.regExp 														<< "\"\n"		 
		<< "\tbucket=\"" 		<< v.bucket  														<< "\"\n"		 
		<< "\tarchiveMode=\"" 	<< ObjStorageManager::getArchiveModeString(v.archiveMode) 			<< "\"\n"
		<< "\tarchiveModeKey=\""<< v.archiveModeKey 												<< "\"\n"
		<< "\tnamingAlgoKey=\"" << v.namingAlgoKey 													<< "\"\n"
		<< "**********************************************************************************************\n";
	
	return s ;
}

template<>
void rsResourceSet::getValue(const std::string &res, ObjStorageManager::StorageParameters &v) const 
{
    Lock const lock(_mutex) ;
	getValue(res+".RegExp",v.regExp) ;
	getValue(res+".Bucket",v.bucket) ;
	{
		string tmp;
		getValue(res+".ArchiveMode", tmp) ;
		v.archiveMode = ObjStorageManager::getArchiveModeEnum(tmp);
	}
    getValue (res+".ArchiveMode.key.value", v.archiveModeKey, false) ;
	getValue(res+".NamingAlgoKey",v.namingAlgoKey) ;
	
} 

ostream &operator<<(ostream &s, const ObjStorageManager::BucketPartitioning & v)
{
	s << '\"' << ObjStorageManager::getBucketPartitioningString(v) << '\"';
	return s ;
}

exostream &operator<<(exostream &s, const ObjStorageManager::BucketPartitioning & v)
{
	s << '\"' << ObjStorageManager::getBucketPartitioningString(v) << '\"';
	return s ;
}

_ACS_END_NAMESPACE
