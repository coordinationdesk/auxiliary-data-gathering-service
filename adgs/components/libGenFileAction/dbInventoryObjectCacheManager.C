// PRQA S 1050 EOF
/*

	Copyright 2016-2021, Advanced Computer Systems , Inc.
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
	Revision 1.12  2016/12/06 16:11:18  lucio.pulvirenti
	S3MPM-843: getCachePath public method added to IF. Message improved.
	
	Revision 1.11  2016/12/05 13:01:10  lucio.pulvirenti
	S3MPM-843: In download, dirty file existance is analogous to file not existance. Configurable timeout is condition to exit wait loops for locks it not got.
	As to directories hardlink, directories permissions are set to 770. Static attribute to emit warning for hard link across devices.
	
	Revision 1.10  2016/12/01 08:11:01  lucio.pulvirenti
	S3MPM-843: Warning messages issued in uploading. ACS_SMARTPTR to Cache::AutoRelease class used in download method instead to call explicitely Cache release method.
	
	Revision 1.9  2016/11/30 10:31:54  lucio.pulvirenti
	S3MPM-843: mkdtemp function used to create temporary directory. In download method, if file does not exist, lock it and exit the method.
	Cache pointer added to member functions.
	
	Revision 1.8  2016/11/29 16:17:17  lucio.pulvirenti
	S3MPM-843: work in progress.
	
	Revision 1.7  2016/11/25 11:30:54  lucio.pulvirenti
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


#include <dbInventoryObjectCacheManager.h>

#include <dbInventoryObject.h>
#include <ConfigurationSingleton.h>
#include <Application.h>
#include <rsPushNSpace.h>
#include <Cache.h>




_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObjectCacheManager)
		
namespace { // unnamed

#ifdef HL_DBINV_CACHE_TRANSFER
#warning "HL_DBINV_CACHE_TRANSFER flag is set"
	const dbInventoryObjectCacheManager::TransferMode defaultTransferMode = dbInventoryObjectCacheManager::HLINK; 
#else
	const dbInventoryObjectCacheManager::TransferMode defaultTransferMode = dbInventoryObjectCacheManager::COPY; 
#endif

	const string cacheConfigurationConfKey = "CacheParameters";
	const string dirtyFileExt = "DIRTY";
	const int lockTimeoutDefault = 20 ;// seconds
}

ThreadSafe dbInventoryObjectCacheManager::_warningMutex;
bool dbInventoryObjectCacheManager::_emitWarning = true;


// constructor
dbInventoryObjectCacheManager::dbInventoryObjectCacheManager():
								_invFilename(),
								_invFileVersion(),
								_invSize(0),
								_cacheFilename(),
								_cachePtr(),
								_exclusiveLockSet(false),
								_lockTimeout(lockTimeoutDefault)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;	
}



dbInventoryObjectCacheManager::~dbInventoryObjectCacheManager() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631
}


bool dbInventoryObjectCacheManager::checkIsConfigured(const dbPersistent &p) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	ACS_COND_THROW(_cachePtr.get(), dbInventoryObjectCacheManagerException("Object already initialized!")); // PRQA S 3081
	
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Configuration:\n" << conf);
	bool configured = conf.hasKey(cacheConfigurationConfKey);
	if( configured ) {
		
		extractInventoryParameters( p );
		
		rsPushNSpace ns(conf, cacheConfigurationConfKey);

		string cachePath;
		conf.getValue("path", cachePath);
		ACS_COND_THROW(cachePath.empty(), dbInventoryObjectCacheManagerException("Cache path not valorized in configuration")); // PRQA S 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cache path from configuration: \"" << cachePath << '\"'); 
		ACS_COND_THROW(not File::exists(cachePath), dbInventoryObjectCacheManagerException("Cache path \""+cachePath+"\" does not exist")); // PRQA S 3081
		
		off_t cacheSize = 0 ;
		conf.getValue("size", cacheSize);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cache size from configuration: " << cacheSize); 
		
		const string ttag = "LockTimeout";
		if( conf.hasKey(ttag) ) {
			conf.getValue(ttag, _lockTimeout);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Lock timeout from configuration: " << _lockTimeout << " s"); 
		}
		
		_cachePtr.reset(new Cache(cachePath, cacheSize));
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot found " << cacheConfigurationConfKey << " tag in configuration: cache not configured");
	}
	return configured;
}

string dbInventoryObjectCacheManager::getCachePath() const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ACS_COND_THROW(not _cachePtr.get(), dbInventoryObjectCacheManagerException("Object not initialized!")); // PRQA S 3081
	
	return _cachePtr->getPath();
}

bool dbInventoryObjectCacheManager::uploadFile(const string & path) const // PRQA S 4020, 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ACS_COND_THROW(not _cachePtr.get(), dbInventoryObjectCacheManagerException("Object not initialized!")); // PRQA S 3081
	
	ACS_SMARTPTR<Cache::AutoRelease> autoReleasePtr;
	
// some checks
	if( _exclusiveLockSet ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cache exclusive lock on \"" << _cacheFilename << "\" file already set: going to set autorelease");
		autoReleasePtr.reset(new Cache::AutoRelease( _cachePtr.get(), _cacheFilename));
	}
	
	ACS_COND_THROW(not File::exists(path), dbInventoryObjectCacheManagerException('\"'+path+"\" file to upload does not exist")); // PRQA S 3081

	if( _exclusiveLockSet && (File::size(path) != _invSize)) {
		ostringstream exMsg;
		exMsg << _invSize << " bytes already reserved for filename \"" << _invFilename << "\" version \"" << _invFileVersion << "\"; size of file to upload is " << File::size(path) << " bytes (they should match)";
		ACS_THROW( dbInventoryObjectCacheManagerException(exMsg.str()) ); // PRQA S 3081
	}
	
	bool success = false;
	const string dirtyFilename = getDirtyFilename(_cacheFilename);
	const string cachePath = _cachePtr->getPath();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cache path \"" << cachePath << '\"');
	const string dirtyFilePath = StringUtils::pathJoin(cachePath, dirtyFilename);
	bool inloop = true;
	for (int counter = 0; inloop; counter++ ) {
		try {
			
			if( _exclusiveLockSet ) { 
				if( counter > 0 ) { // logically it must not occur: ASSERT
					ostringstream exMsg;
					exMsg << "UNEXPECTED: exclusive lock on \"" << _cacheFilename << "\" file already set but loop gone through " << counter << " times!";
					ACS_THROW( dbInventoryObjectCacheManagerException(exMsg.str()) ); // PRQA S 3081
				}
			}
			else {
				// Reserve space and lock exclusively the inventory item
				_cachePtr->makeSpaceReserveAndUse ( _cacheFilename, File::size(path) );
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cache exclusive lock got on \"" << _cacheFilename << "\" file after " << counter << " iterations") ;
				autoReleasePtr.reset(new Cache::AutoRelease( _cachePtr.get(), _cacheFilename));
			}
			
			try {
			// removes dirty file by cache
				_cachePtr->eraseEntry(dirtyFilename);
			} 
			catch( exception &e )
			{
				if( dynamic_cast<Cache::CacheReserveException*> (&e) || dynamic_cast<Cache::CacheLockException*> (&e) ) { // PRQA S 3081
			// if here, it means that another thread found the file locked and is writing dirty file... but it seldom occurs since other threads write dirty file at the second attempt after waiting 100 ms
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot erase dirty file \"" << dirtyFilename << "\" since locked." );
					ACS_LOG_WARNING("Before starting upload in cache layer of inventory file \"" << _invFilename << "\", version \"" << _invFileVersion << "\" a concurrent access found! Going to exit");
					return success;
        		}
				else {
					throw;
				}
			}
	
			const string filePathInCache = StringUtils::pathJoin(cachePath, _cacheFilename) ;
			
			bool changePermission = true; // if the file is a directory and it already exists, nested dir cannot be removed without changing permissions if they are r-x 
			File::unlink(filePathInCache, changePermission);
			
			if( counter == 0 ) { // From here on someone else could create a dirty file, bot does not matter... the file will remain in cache along with the dirty file.
				
				// go on
				FileGuard temporaryPath = createTemporaryEntity(path, filePathInCache);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << temporaryPath << "\" temporary path successfully created" ); // PRQA S 3050
				// lock the temporary path
				string temporaryFilename =  File::getFileName(temporaryPath); // PRQA S 3050
				
				// reserve 0 bytes for temporary file even if this file will effectively written: sum of this file size + real file will be the size reserved by the cache. 
				_cachePtr->makeSpaceReserveAndUse ( temporaryFilename, 0 ); // no Cache::CacheReserveException/Cache::CacheLockException can be launched!!!
				Cache::AutoRelease autoreleaseTemp ( _cachePtr.get(), temporaryFilename );
				transferFile( path, temporaryPath); // PRQA S 3050
				
				File::rename(temporaryPath, filePathInCache);// PRQA S 3050 2
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << temporaryPath << "\" temporary file successfully renamed as \"" << filePathInCache << '\"' );
				
				success = true;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << path << "\" successfully uploaded as \"" << filePathInCache << '\"'); 
				
			} // FileGuard out of scope: temporary path removed if not renamed 
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Some other thread/process has just written \"" << path << "\" file in cache: just removed from cache and going to exit...");
				// no file, no need of dirty file...
				ACS_LOG_WARNING("Concurrent access on inventory file \"" << _invFilename << "\", version \"" << _invFileVersion << "\": no upload in cache layer");
			}
			inloop = false; 
			
		} // lock is released at the end of scope.
		catch( exception &e )
		{
			if( dynamic_cast<Cache::CacheReserveException*> (&e) || dynamic_cast<Cache::CacheLockException*> (&e) ) { // PRQA S 3081
				if( counter == 0 ) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot acquire exclusive lock: waiting..." ); 
					Timer::delay(100); // milliseconds // PRQA S 4400
				}
				else {
 				// Exclusive lock on dirty file to touch at the second attempt
					Timer::Delay lockTimeout_ms = _lockTimeout * 1000;  // PRQA S 3000
					Timer timer ;
					timer.start();
					while(inloop) {
						try {
							_cachePtr->makeSpaceReserveAndUse ( dirtyFilename, off_t(0) ); // PRQA S 3081
							Cache::AutoRelease autoreleaseDirtyfile ( _cachePtr.get(), dirtyFilename );
							File::touch(dirtyFilePath);
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << dirtyFilePath << "\" dirty file just created." ); 
							
							// dirty file will be never removed by the cache mechamism.
							_cachePtr->preventAutoErase(dirtyFilename, -1);
							inloop = false;
							ACS_LOG_WARNING("Concurrent access on inventory file \"" << _invFilename << "\", version \"" << _invFileVersion << "\": no upload in cache layer and invalidating the one being written");
							
						} // release excl lock on dirty file
						catch( exception &ee )
						{
							if( dynamic_cast<Cache::CacheReserveException*> (&ee) || dynamic_cast<Cache::CacheLockException*> (&ee) ) { // PRQA S 3081
								ACS_COND_THROW( timer.stopElapsed() > lockTimeout_ms, TimeoutException("Cannot acquire shared lock on dirty file \"" + dirtyFilename + "\": timeout expired") ); // PRQA S 3081
								ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Waiting to acquire exclusive lock on dirty file \"" << dirtyFilename << '\"' ); 
								Timer::delay(100); // milliseconds // PRQA S 4400
        					}
							else {
								ACS_THROW( dbInventoryObjectCacheManagerException(ee, '\"'+_cacheFilename +"\" file is excl locked, but cannot create dirty file" ) );
							}
						}
					}
				}
			}
			else {
				throw;
			}

		}
	} // for (int counter = 0; ; counter++ )

	return success;
	
}

bool dbInventoryObjectCacheManager::downloadFile(const string & path, bool * gotExclusiveLock ) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ACS_COND_THROW(not _cachePtr.get(), dbInventoryObjectCacheManagerException("Object not initialized!")); // PRQA S 3081
	ACS_COND_THROW( File::exists( path ), dbInventoryObjectCacheManagerException("Error: file \"" + path + "\" already exists.") ) ; // PRQA S 3081
	bool downloaded = false;
	if( gotExclusiveLock ) {
		*gotExclusiveLock = false;
	}
	const string cachePath = _cachePtr->getPath();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cache path \"" << cachePath << '\"');
	const string filePathInCache = StringUtils::pathJoin(cachePath, _cacheFilename) ;
	const string dirtyFilePath = getDirtyFilename(filePathInCache);
	
	Timer::Delay lockTimeout_ms = _lockTimeout * 1000; // PRQA S 3000
	Timer timer ;
	timer.start();
	for (int counter = 0; ; counter++ ) {
		try {

		// Try to lock (shared) file
			_cachePtr->use( _cacheFilename );
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cache shared lock got on \"" << _cacheFilename << "\" file");
			ACS_SMARTPTR<Cache::AutoRelease> autoReleasePtr(new Cache::AutoRelease( _cachePtr.get(), _cacheFilename));

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				if( counter > 0 ) {
					ACS_LOG_DEBUG(SimpleDebugSignature << "Lock acquired after " << counter << " iterations") ;
				}
			ACS_CLASS_END_DEBUG

			// check if dirty file exists (it could have been created by the upload thread that cannot lock exclusively the same file this thread is downloading) or if the file to download does not exist
			if( File::exists( dirtyFilePath ) || not File::exists(filePathInCache) ) {
				
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					if( not File::exists(filePathInCache) ) {
						ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << filePathInCache << "\" path in cache does not occurs: cannot download");
					}
					else {
						ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << dirtyFilePath << "\" dirty file occurs: cannot download \"" << _cacheFilename << "\" file from cache");
					}
				ACS_CLASS_END_DEBUG

				if( gotExclusiveLock ) {
					
				// getting an exclusive lock not to release at the end of the scope...
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Trying to get exclusive lock on \"" << _cacheFilename << "\" file in cache");
					try {
						
						Cache::AutoRelease *ptr = autoReleasePtr.release(); // destroy the object (release the lock) and release the pointer
						delete ptr;
						
						// Here someone else could acquire the lock. The exclusive lock could not be got for one of the following reaons:
						// 1) More than one reader occurred: the first one will release the lock, but shared lock is kept by the other ones and so on: the last one to enter will obtain the lock
						// 2) One writer obtained the lock: he will write the file so no need to lock it....
						
						_cachePtr->makeSpaceReserveAndUse ( _cacheFilename, _invSize );
						*gotExclusiveLock = true;
						_exclusiveLockSet = true;
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exclusive lock on \"" << _cacheFilename << "\" file in cache GOT!");
					}
					catch( exception &e ) 
					{
						if( dynamic_cast<Cache::CacheReserveException*> (&e) || dynamic_cast<Cache::CacheLockException*> (&e) ) { // PRQA S 3081
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot get exclusive lock on \"" << _cacheFilename << "\" file in cache");
						}
						else {
							throw;
						}
					}
				}
				
			} // if( File::exists( dirtyFilePath ) || not File::exists(filePathInCache) ) 
			else { // download
				// check file size....
				const off_t fSize = File::size(filePathInCache);
				if( fSize != _invSize ) {
					ostringstream exMsg;
					exMsg << "Size of \"" << _cacheFilename << "\" in cache (\"" << cachePath << "\") is " << fSize << " bytes and does not match with inventory size: " << _invSize << " bytes";
					ACS_THROW( dbInventoryObjectCacheManagerException( exMsg.str() ) ); // PRQA S 3081
				} 

				// download
				FileGuard temporaryPath = createTemporaryEntity(filePathInCache, path);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << temporaryPath << "\" temporary path successfully created" ); // PRQA S 3050

				transferFile( filePathInCache, temporaryPath); // PRQA S 3050

				// it could be checked if in the meanwhile a dirty file has been created (see version 1.7), but we trust in what occurred at start...
				File::rename(temporaryPath, path); // PRQA S 3050
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << temporaryPath << "\" temporary file successfully renamed as \"" << path << '\"' ); // PRQA S 3050
				downloaded = true;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _cacheFilename << "\" file successfully downloaded in \"" << path << "\" path" );
			}

			break;

		} // lock is released; FileGuard out of scope: temporary path removed if not renamed 
		catch( exception &e )
		{
			if( dynamic_cast<Cache::CacheReserveException*> (&e) || dynamic_cast<Cache::CacheLockException*> (&e) ) { // PRQA S 3081
				ACS_COND_THROW( timer.stopElapsed() > lockTimeout_ms, TimeoutException("Cannot acquire shared lock on \"" + _cacheFilename + "\" file: timeout expired") ); // PRQA S 3081
				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cannot acquire shared lock..." );
				Timer::delay(100); // milliseconds // PRQA S 4400
			}
			else {
				throw;
			}
		}
	}
	
	return downloaded;
	
}



void dbInventoryObjectCacheManager::extractInventoryParameters( const dbPersistent & p ) 
{
	
	const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	ACS_COND_THROW(not obj, dbInventoryObjectCacheManagerException("dynamic_cast fails")); // PRQA S 3081
	
	bool isSet = false ;
	_invFilename = obj->getFileName(&isSet);
	ACS_COND_THROW(not isSet, dbInventoryObjectCacheManagerException("Inventory object filename not set")); // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory filename=\"" << _invFilename << '\"' ); 
	
	_invFileVersion = obj->getFileVersion(&isSet);
	ACS_COND_THROW(not isSet, dbInventoryObjectCacheManagerException("Inventory object file version not set")); // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory file version=\"" << _invFileVersion << '\"' ); 
	
	DateTime invDate = obj->getInventoryDate(&isSet);
	ACS_COND_THROW(not isSet, dbInventoryObjectCacheManagerException("Inventory object inventory date not set")); // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory date=" << invDate.getSqlString() ); 
	
	_invSize = obj->getHeaderSize() + obj->getDataSize();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory size=" << _invSize ); 

	_cacheFilename = File::getCacheFileName( _invFilename, _invFileVersion, invDate.Datetime2time_t() );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename in cache: \"" << _cacheFilename << '\"'); 
	
}

string dbInventoryObjectCacheManager::getDirtyFilename( const string & filename ) const
{
	return File::addExt( filename, dirtyFileExt); 
}

string dbInventoryObjectCacheManager::transferModeToString(dbInventoryObjectCacheManager::TransferMode tm) const // PRQA S 4020
{
    switch(tm) {
    case NONE: { return "NONE" ; }
    case COPY: { return "COPY" ; }
    case HLINK: { return "HLINK" ; }
	default: { return "**UNKNOWN**" ; }
    }
}
	
dbInventoryObjectCacheManager::TransferMode dbInventoryObjectCacheManager::stringToTransferMode( const string & s ) const // PRQA S 4020
{
    if(StringUtils::uppercase(s) == "NONE") {
        return NONE;
    }
    else if(StringUtils::uppercase(s) == "COPY") {
        return COPY;
    }
    else if(StringUtils::uppercase(s) == "HLINK") {
        return HLINK;
    }
    else {
        ACS_LOG_WARNING ("Unable to recognize the transfer mode \"" << s << "\". NONE will be returned");
        return NONE;
    }
}


dbInventoryObjectCacheManager::TransferMode dbInventoryObjectCacheManager::getTransferMode(const string & path1, const string & path2) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	TransferMode tm = defaultTransferMode;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Default transfer mode is " << transferModeToString(tm));
	try {
// first of all application name must be defined!!!
		string applicationName = Application::instance()->applicationName() ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current application name is \"" << applicationName << '\"') ;
		rsResourceSet &conf = ConfigurationSingleton::instance()->get();
		string keyField = applicationName + ".CacheTransferMode";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "looking for \"" << keyField << "\" field occurrence in configuration") ;
		if( conf.hasKey(keyField) ) {
			string transferMode;
			conf.getValue(keyField, transferMode);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configuration tag \"" << keyField << "\" value is \"" << transferMode << '\"') ;
			
			TransferMode configuredTm = stringToTransferMode(transferMode);
			if( configuredTm != NONE ) {
				tm = configuredTm;
			}
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configured transfer mode=" << transferModeToString(tm));
		}
	}
	catch( exception &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot access to Client Application configuration space: " << e.what());
	}
	
	if( tm == HLINK ) {
	// check if hard link is possibile
		bool followSymLinks = true;
		string fs1 = File::getDirPath(path1);
		string fs2 = File::getDirPath(path2);
		if( not File::checkFileSystemsMatching( fs1, fs2, followSymLinks ) ) {
			if( emitWarning() ) {
				ACS_LOG_WARNING("Bidirectional Cache Layer Manager: Cannot perform hard link since \"" << File::getAbsolutePath(fs1) << "\" and \"" << File::getAbsolutePath(fs2) 
								<< "\" file systems are different! A copy will be executed.");
			}
			tm = COPY;
		}
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return " << transferModeToString(tm) << " transfer mode");
	return tm;
}

void dbInventoryObjectCacheManager::transferFile( const string & src, const string & dest ) const // PRQA S 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to transfer from \"" << src << "\" to \"" << dest << '\"');
	
	string source = src;
	if( File::isSymbolicLink( source, false ) ) {
		string tmp = File::readtargetlink( source, true );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "source file \"" << src << "\" is a symbolic link; the target file is \"" << tmp << '\"') ;
		source = tmp;
	}
	
	// transfer file with copy or hard link.
	TransferMode tMode = getTransferMode(src, dest);
	if( tMode == COPY ) {
		File::copy(source, dest);
		File::chmod_rec(dest, 0664, File::AddPerm); // PRQA S 4400
	}
	else if( tMode == HLINK ) {
		
// http://www.gnu.org/software/libc/manual/html_node/Hard-Links.html: EEXIST: There is already a file named newname. If you want to replace this link with a new link, you must remove the old link explicitly first
// An alternative way could be not to generate temporary file, but for plain files only (recursive hard link on directories is not atomic). In that case, transfer mode must be inspected before and 
// check on file system must be performed in order to do effectively hard link or copy...
		bool changePermission = true; // if the file is a directory and it already exists, nested dir cannot be removed without changing permissions if they are r-x 
		File::unlink(dest, changePermission);
		
		File::hardlink(source, dest);
		File::chmod_rec(dest, 0222, File::SubPerm); // PRQA S 4400
		if( File::isDir(dest) ) {
			// directories must be writable
			File::chmod_rec(dest, 0770, File::SetPerm, true); // PRQA S 4400
		}
	}
	else {
	// it should not occur.
		ACS_THROW( dbInventoryObjectCacheManagerException("Unexpected transfer mode!") ); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File successfully transferred: \"" << dest << '\"');
	
}


FileGuard dbInventoryObjectCacheManager::createTemporaryEntity(const string & src, const string & dest) const // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string source = src;
	if( File::isSymbolicLink( source, false ) ) {
		string tmp = File::readtargetlink( source, true );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "source file \"" << src << "\" is a symbolic link; the target file is \"" << tmp << '\"') ;
		source = tmp;
	}
	
	if( File::isDir(source) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "source file \"" << source << "\" is a directory" ) ;	
		
		string tmpname = dest + "_XXXXXX";
		char tmpl[4096] ; // PRQA S 4403
		strcpy(tmpl, tmpname.c_str()) ;
		char *res = mkdtemp(tmpl);
		ACS_COND_THROW(not res, dbInventoryObjectCacheManagerException("Cannot create temporary directory", errno) );
		return FileGuard(res); // PRQA S 3081
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "source file \"" << source << "\" is a plain file" ) ;
		return File::tempname(dest);
	}
				

}

bool dbInventoryObjectCacheManager::emitWarning() const // PRQA S 4214
{
	Lock guard(_warningMutex); 
	bool w = _emitWarning;
	if( w ) {
		_emitWarning = false;
	}
	return w;
	
}
_ACS_END_NAMESPACE
