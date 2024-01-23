// PRQA S 1050 EOF
/*

 Copyright 1995-2015, Advanced Computer Systems , Inc.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.acsys.it

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Advanced Computer Systems, Inc.

 $Prod: DataDownloader $

 $Id$

 $Author$

 $Log$
 Revision 1.32  2018/02/28 17:46:08  nicvac
 S1PDGS-32018: improved check downloaded Vs timeout.

 Revision 1.31  2018/02/20 10:04:31  chicas
 S2PDGS-1940

 Revision 1.30  2017/03/03 15:48:38  nicvac
 S2PDGS-1705: stack trace log messages.

 Revision 1.29  2016/10/04 07:19:17  nicvac
 OPMAN-192: Fixed the Download entry in header case.

 Revision 1.28  2016/09/16 08:52:59  nicvac
 S2PDGS-1544: Log handling revision (PRIVINFO messages).

 Revision 1.27  2016/04/11 13:38:28  marpas
 adopting new db interfaces

 Revision 1.26  2016/03/26 21:36:33  marpas
 using new db I/F

 Revision 1.25  2015/05/28 09:29:03  marpas
 coding best practice applied
 warnings mostly removed

 Revision 1.24  2014/05/30 16:25:19  nicvac
 Refined timers to cope with stressed cache.

 Revision 1.23  2014/05/29 13:24:28  nicvac
 _confMinThresholdTimeWaitingDownload set to 20 minute to cope with cache rolling time on million of files.

 Revision 1.22  2014/05/27 17:53:55  nicvac
 Message is debug.

 Revision 1.21  2014/05/27 17:13:39  nicvac
 Cache entry cannot be erased.

 Revision 1.20  2014/05/16 15:09:28  nicvac
 Configuration setting to keep the cache cleaned. Cleaned cache means more performances when cache does stats on disk.

 Revision 1.19  2014/05/09 09:08:40  nicvac
 Fixed header entry.

 Revision 1.18  2014/05/08 17:27:44  nicvac
 Delete before downloading.

 Revision 1.17  2014/05/07 13:11:46  nicvac
 Format after downloading fixed.

 Revision 1.16  2014/05/06 18:17:11  nicvac
 Min threshold for download waiting.

 Revision 1.15  2014/05/01 08:38:49  nicvac
 S2PDGS-667: test and fixing.

 Revision 1.14  2014/05/01 08:19:08  nicvac
 S2PDGS-667: test and fixing.

 Revision 1.13  2014/04/30 19:19:04  nicvac
 S2PDGS-667: test and fixing.

 Revision 1.12  2014/04/30 16:25:05  nicvac
 S2PDGS-667: cache group implementation.

 Revision 1.11  2014/04/29 15:53:35  nicvac
 DownloadingStatus added.

 Revision 1.10  2014/04/29 08:53:48  nicvac
 Better Timers handling.

 Revision 1.9  2014/04/28 14:32:40  nicvac
 Refactoring. Main topics: shared lock during check phase (no one can write now during checking). Timer handling to prevent starvation and contingency in concurrent downloading.

 Revision 1.8  2013/11/13 10:05:25  marfav
 S2PDGS-519: limiting the number of concurrent open connections releasing them ASAP

 Revision 1.7  2013/10/03 13:45:21  nicvac
 acs sdk warnings processed.

 Revision 1.6  2013/09/30 12:41:22  marpas
 removing useless includes

 Revision 1.5  2013/06/21 16:16:39  nicvac
 Can Download also using inventory id.

 Revision 1.4  2013/05/20 14:24:35  enrcar
 EC:: greatest version returned if no version provided

 Revision 1.3  2013/05/20 13:46:07  enrcar
 EC:: First working version released

 Revision 1.2  2013/05/16 14:26:54  enrcar
 EC:: I/F fixed

 Revision 1.1  2013/05/16 09:37:57  enrcar
 EC:: imported source file


 */


#include <DataDownloader.h>

#include <dbDownloadFile.h>
#include <ConfigurationSingleton.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbFileTypeRecognition.h>
#include <dbInventoryObject.h>
#include <dbFileTypeQuery.h>
#include <dbFileType.h>
#include <dbUserQuery.h>
#include <RegExp.h>
#include <dbConnectionPool.h>
#include <dbUser.h>
#include <dbGenQueryScopeGuard.h>
#include <dbQueryFactoryChain.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <StringUtils.h>
#include <File.h>
#include <Dir.h>

#include <Filterables.h>

#include <algorithm>
#include <unistd.h>

#include <DDV.h> /* versioning */

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
namespace {
    DDV Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL ( DataDownloader );


DataDownloader::DataDownloader (
        dbConnectionPool& connectionPool,
		Cache & cache,
		const string& relative_download_path /* Relative to the cache root */) :
    StopController(),
	_confMaxConnRetry ( 3 ), // PRQA S 4400
 	_connectionPool ( connectionPool ), // PRQA S 2528 2
 	_cache ( cache ),
 	_absDownloadPath( StringUtils::pathJoin(cache.getPath(), relative_download_path) ),
 	_relDownloadPath( relative_download_path ),
 	_filetyperec(),
 	_downloaderType(StillUnknown),
 	_confMaxTimeWaitingDownloadStart(1500.0), // PRQA S 4400 6
 	_confDownloadDataRate(0.1),
 	_confMinThresholdTimeWaitingDownload(1500.0),
	_confAdvDelayPollingDownloader(1000),
	_confAdvDelayRealDownloader(100),
	_confAdvDelayPreventStarvation(5)
{

	//Init from configuration
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
    string dd = "DataDownloader." ;
	try { conf.getValue(dd + "MaxDbConnRetry", _confMaxConnRetry); }										catch(exception& ex) {}
	try { conf.getValue(dd + "MaxTimeWaitingDownloadStart", _confMaxTimeWaitingDownloadStart); } 			catch(exception& ex) {}
	try { conf.getValue(dd + "DownloadDataRate", _confDownloadDataRate); } 								catch(exception& ex) {}
	try { conf.getValue(dd + "MinThresholdTimeWaitingDownload", _confMinThresholdTimeWaitingDownload); } 	catch(exception& ex) {}

	try { conf.getValue(dd + "Advanced.DelayPollingDownloader", _confAdvDelayPollingDownloader); } 	catch(exception& ex) {}
	try { conf.getValue(dd + "Advanced.DelayRealDownloader", _confAdvDelayRealDownloader); } 			catch(exception& ex) {}
	try { conf.getValue(dd + "Advanced.DelayPreventStarvation", _confAdvDelayPreventStarvation); } 	catch(exception& ex) {}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,
			"DataDownloader Configuration:\n"
			<< " Cache path: [" << cache.getPath() <<"];\n"
			<< " Relative download path: [" << _relDownloadPath << "];\n"
			<< " Absolute download path: [" << _absDownloadPath << "];\n"
			<< " Configured waiting download start max time [sec]: [" << _confMaxTimeWaitingDownloadStart << "];\n"
			<< " Configured download data rate [Mb/sec]: " << _confDownloadDataRate << "];\n"
			<< " Configured min threshold for waiting download [sec]: ["<< _confMinThresholdTimeWaitingDownload << "];\n"
			<< " Configured Delay for Polling Downloader [ms]: [" << _confAdvDelayPollingDownloader << "];\n"
			<< " Configured Delay for Real Downloader [ms]: [" <<_confAdvDelayRealDownloader << "];\n"
			<< " Configured Delay for Starvation prevention [ms]: [" << _confAdvDelayPreventStarvation << "];"
	);

	/**
		relative_download_path is assumed not to be a collection (i.e. a directory with it own lock, so that all the contents are handled as a single entry) 
		it's just a directory in the filesystem (the .lock file will be created INSIDE the directory, one per each file)
		Note that the full path: cache_path + relative_path MUST EXIST (so the user should create it before using this class)
		The effect is the user can choose if the download_path is just a directory -or- is a collection (the user will add the directory name to the cache). 
	 */

	ACS_COND_THROW ( (! File::isDir(_absDownloadPath)), exIOException("Could not find download path: \"" + _absDownloadPath + "\"") ) ;  // PRQA S 3081
}


DataDownloader::~DataDownloader() throw () {}

DataDownloader::DownloaderType DataDownloader::getDownloaderType() const // PRQA S 4120
{
	return _downloaderType;
}

string DataDownloader::getCacheNameEntryForInventoryItem(
    const dbInventoryObject& inv, 
    const std::string& cacheGroup) const 
{
 	return File::getCacheFileName ( cacheGroup+inv.getFileName(), inv.getFileVersion(), (inv.getInventoryDate()).Datetime2time_t() );
}


vector<DataDownloader::DownloadedEntry> DataDownloader::download_files(const DataDownloader::InventoryEntry &entryToDownload)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Entry to download: " << entryToDownload.str());

	vector<DownloadedEntry> downloadedFiles ; 
	dbConnectionWrapper pw ( _connectionPool );
	dbConnection &conn = pw; // PRQA S 3050

	//Set and fill dbInventoryObject
	dbInventoryObject* inventoryObject = dynamic_cast<dbInventoryObject*> ( dbPersistentFactoryChain::instance()->newObject ( "dbInventoryObject", conn.getKey() ) ); // PRQA S 3081, 4412
	ACS_COND_THROW ( !inventoryObject, exIllegalValueException ( "DataDownloader::download_files : Dynamic_cast failure." ) ); // PRQA S 3081
	dbPersistentScopeGuard sGuard ( inventoryObject );

	string fileName ; 
    string fileVersion ;

	try {

		/* Do not move from here: */
		ACS_COND_THROW(requestedCancel(), DataDownloaderStoppedException( "Operation stopped" ) ) ;  // PRQA S 3081

		fileName	= entryToDownload.inventoryFilename ;
		fileVersion	= entryToDownload.inventoryFileversion ;  

		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "CHECKING IF DOWNLOAD IS NEEDED. Id: ["<< entryToDownload.inventoryId <<"] Name: [" << fileName << "]  version: [" << fileVersion << "]");

		size_t connRetry = 0;
		while ( connRetry < _confMaxConnRetry ) {

			ACS_COND_THROW(requestedCancel(), DataDownloaderStoppedException( "Operation stopped" )  ) ;   // PRQA S 3081

			try {
				inventoryObject->reset();

				if ( 0 == entryToDownload.inventoryId ) {
					inventoryObject->setFileName ( fileName );
					if (!fileVersion.empty()) { inventoryObject->setFileVersion ( fileVersion ); }
				}

				// load record having the id OR filename and version
				// If if not used (set to 0) ==> load considers filename and fileversion
				{
					if ( !inventoryObject->load ( conn, entryToDownload.inventoryId ) ) {
						ostringstream msg; msg<<" Error in inventory query for id=["<<entryToDownload.inventoryId<<"] name= ["<<fileName<<"] version= ["<<fileVersion<<"]";
						ACS_THROW ( DataDownloaderException ( msg.str() ) );  // PRQA S 3081
					}
				}

				break;
			} catch ( exception &e ) {
				if ( 0 == connRetry ) {
					ACS_LOG_NOTIFY_EX(e);
				}
				if ( connRetry == ( _confMaxConnRetry - 1 ) ) { // PRQA S 3084
					ostringstream msg;
					msg << " maximum number of db connection retries ("<<_confMaxConnRetry<<") performed!" << '\n';
					ACS_THROW ( DataDownloaderException ( e, msg.str() ) ); // PRQA S 3081
				}
				Timer::delay ( 100 ); // PRQA S 4400
				connRetry++;

			}
		} // end while

		/* If fileVersion was empty, the greatest version found on DB was used. In this case, the version is returned */
		if (fileVersion.empty()) { fileVersion = inventoryObject->getFileVersion() ; }
		if (fileName.empty())    { fileName =    inventoryObject->getFileName() ; } //In the case id is used to get item

		const string cacheName = getCacheNameEntryForInventoryItem(*inventoryObject, entryToDownload.cacheGroupName);
		const string cacheNameRelPath = StringUtils::pathJoin(_relDownloadPath, cacheName) ;

		//Initialize Timers
		Timer timerWaitingDownloadStart;		timerWaitingDownloadStart.start();
		Timer timerWaitingConcurrentDownload;	timerWaitingConcurrentDownload.stop(); //Maybe start later, here stop needed for init.
		const double maxTimeWaitingConcurrentDownload =
			std::max( _confMinThresholdTimeWaitingDownload, // [s]
				 ( ( (inventoryObject->getDataSize() + inventoryObject->getHeaderSize()) ) / 1024.0 / 1024.0 ) // [Mb] // PRQA S 3011, 3084
				 * ( 1.0 / _confDownloadDataRate ) // [s/Mb]
				
			);


		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Can wait download by other processes/threads up to "<<maxTimeWaitingConcurrentDownload<<" [s]");


		// Try concurrently to be the downloader. If you fail, check if someone else downloaded for you
		bool downloaded(false);
		bool canStillWaitForTimersExpiration(true);
		while ( ( !downloaded ) && (canStillWaitForTimersExpiration) ) {

			ACS_COND_THROW(requestedCancel(), DataDownloaderStoppedException( "Operation stopped" ) ) ; // PRQA S 3081

			//Computed here, checked on next loop. So that if an item has been downloaded just after time expiration, the download is still ok.
			canStillWaitForTimersExpiration = (
				//can still wait for download start
				( timerWaitingDownloadStart.running() 		&& (timerWaitingDownloadStart.life()/1000.0 < _confMaxTimeWaitingDownloadStart) ) // PRQA S 3011, 3084, 4400 3
				||
				//can still wait for concurrent download
				( timerWaitingConcurrentDownload.running()	&& (timerWaitingConcurrentDownload.life()/1000.0 < maxTimeWaitingConcurrentDownload) )
			);

			// Check if Inventory item have been already downloaded
			bool itemDownloaded(false) ;
            bool sharedLockFailed(false);
			sharedLockInventoryItem_checkDownload_sharedLockDownloadedFiles_unlockInventoryItem (*inventoryObject, entryToDownload.cacheGroupName,
																			itemDownloaded, sharedLockFailed, downloadedFiles);
			bool& itemBeingDownloaded = sharedLockFailed; //Shared lock failed: another thread/process locked exclusively the item (it's downloading it)

			if ( itemDownloaded ) {

				downloaded = true;
				// WHEN HERE: Item is available and shared locked in cache.
				ACS_LOG_PRIVINFO("Inventory Item: ["<<fileName<<"]  version: ["<<fileVersion<<"] cache group: ["<<entryToDownload.cacheGroupName<< "] IS AVAILABLE IN CACHE AND LOCKED");

			} else {

				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "INVENTORY ITEM NOT IN CACHE: ["<<fileName<<"]  version: ["<<fileVersion<<"] cache group: ["<<entryToDownload.cacheGroupName<< "]");

				if ( itemBeingDownloaded ) {

					_downloaderType = PollingDownloader;

					//Handle timers:
					//Stop (once) timer for actual download start timing.
					if ( timerWaitingDownloadStart.running() ) { timerWaitingDownloadStart.stop(); }
					//Start (once) timer for concurrent downloading.
					if ( ! timerWaitingConcurrentDownload.running() ) { timerWaitingConcurrentDownload.start(); }

					//Someone else is working for you. Go to play soccer for a while...
					Timer::delay ( _confAdvDelayPollingDownloader /* unit: ms */ );	// Wait (default is 1s) ...

				} else {

					//Try to be the downloader, in competition with Other threads / processes.

					//Wait very few and random (0..5) milliseconds to reduce a starvation risk.
					// Starvation is improbable but possible loop when
					// concurrent threads/processes try to download, but every time an exclusive lock is requested it cannot be given
					// because some other(s) thread/process owns (for very short time) the shared lock needed for the download check.
					// This random delay let to try a different instructions' execution "interleaving" each loop.
					// Anyway, a timer is set to exit the loop if download does not start within the configured time.
					Timer::randomDelay(_confAdvDelayPreventStarvation, 0); /* unit: ms */

					try {

						// clean cache if necessary and reserve disk space
						const off_t req = 2 * ( inventoryObject->getDataSize() + inventoryObject->getHeaderSize() ) ; // PRQA S 3000, 3084

						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "  Entry: ["<<cacheName<<"] Requesting: " << req << " bytes");

						//Lock exclusively the inventory item.
						off_t reserved = _cache.makeSpaceReserveAndUse ( cacheNameRelPath, req );

						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"  Entry: ["<<cacheName<<"] Reserved: " << reserved << " bytes");

						//Ehi! I'm the winner! Now let's download.
						// Now any other locks (shared or exclusive) will fail.
						_downloaderType = RealDownloader;

						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DOWNLOADING ENTRY: [" << cacheName << "]");

						ACS_COND_THROW(requestedCancel(), DataDownloaderStoppedException( "Operation stopped" ) ) ; // PRQA S 3081

						//Handle timers:
						//Stop (once) timer for actual download start timing.
						if ( timerWaitingDownloadStart.running() ) { timerWaitingDownloadStart.stop(); }
						//Start (once) timer for concurrent downloading.
						if ( ! timerWaitingConcurrentDownload.running() ) { timerWaitingConcurrentDownload.start(); }

						// download, format (split/rename/as is) the inventory item, unlock it.
						downloadReservLockedInventoryItem_formatFiles_unlockInventoryItem ( *inventoryObject, entryToDownload.cacheGroupName );

						ACS_LOG_PRIVINFO("Inventory Item: ["<<fileName<<"]  version: ["<<fileVersion<<"] cache group: ["<<entryToDownload.cacheGroupName<< "] DOWNLOADED");

						canStillWaitForTimersExpiration = true; //Time expiration check does not matter anymore for me. Product has been downloaded.

					} catch ( Cache::CacheReserveException const& unused ) {
						//Looser: Try again next time
						ACS_LOG_PRIVINFO( "Trying to be the downloader, entry: [" << cacheName << "] is being checked or downloaded by another thread...");

					} catch ( Cache::CacheLockException const& unused ) {
						//Looser: Try again next time
						ACS_LOG_PRIVINFO( "Trying to be the downloader, entry: [" << cacheName << "] is being checked or downloaded by another process...");
					}

				}

			}

		}

		//Stop timers:
		if ( timerWaitingDownloadStart.running() ) { timerWaitingDownloadStart.stop(); }
		if ( timerWaitingConcurrentDownload.running() ) { timerWaitingConcurrentDownload.stop(); }

		ostringstream msg; 
        msg<<"Downloading timing info: Elapsed time waiting download starting: "<<fixed<<setprecision(2)<<(timerWaitingDownloadStart.elapsed()/1000.0) // PRQA S 3011, 3084, 4400 4
			<<" [s]; max allowed: "<<_confMaxTimeWaitingDownloadStart<<" [s]. "
			<<" Elapsed time for actual downloading (or waiting concurrent downloading): "<<(timerWaitingConcurrentDownload.elapsed()/1000.0)
			<<" [s]; max allowed: "<<maxTimeWaitingConcurrentDownload<< " [s] for item size "<< (inventoryObject->getDataSize() + inventoryObject->getHeaderSize())/1024.0/1024.0<<" [Mb]";

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str());

		// if not downloaded... throw exception
		ACS_COND_THROW( !downloaded, DataDownloaderException ( "Could not download entry: [" + cacheName + "]. "+msg.str() ) );// PRQA S 3081

		/* Do not move from here: */
		ACS_COND_THROW(haltImmediate(), DataDownloaderStoppedException( "Operation stopped" ) ) ; // PRQA S 3081

		createLinks(entryToDownload, downloadedFiles) ;
	}
	catch(DataDownloader::DataDownloaderStoppedException& e)
	{
		ACS_LOG_ERROR( "Stop received while downloading/handling item: ["<<fileName<<"]  version: ["<<fileVersion<<"] cache group: ["<<entryToDownload.cacheGroupName<< "]");
		ACS_LOG_NOTIFY_EX(e);

		setError(downloadedFiles) ; /* Remove links and set error */
		throw ;
	}
	catch(exception& e)
	{
		ACS_LOG_ERROR( "Problem downloading inventory item id: ["<<entryToDownload.inventoryId<<"]  name: ["<<fileName<<"]  version: ["<<fileVersion<<"] cache group: ["<<entryToDownload.cacheGroupName<< "]");
		ACS_LOG_NOTIFY_EX(e);

		setError(downloadedFiles) ; /* Remove links and set error */ 
		throw ;
	}


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. Returning: " << downloadedFiles.size() << " entries");

	return downloadedFiles ;
}

void DataDownloader::downloadReservLockedInventoryItem_formatFiles_unlockInventoryItem (
		dbInventoryObject& inv,
		const std::string& cacheGroup )
{

	//Here the inventory item in cache is exclusively locked. Nobody else can take the lock on it for read/write

	const string cacheName = getCacheNameEntryForInventoryItem(inv, cacheGroup);
	const string cacheNameRelPath = StringUtils::pathJoin( _relDownloadPath, cacheName ) ;
	const string cacheNameAbsPath = StringUtils::pathJoin( _absDownloadPath, cacheName ) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for downloading locked entry: [" << cacheNameRelPath << "] to path: ["<< _cache.getPath() <<"]");

	ACS_COND_THROW(haltImmediate(), DataDownloaderStoppedException( "Operation stopped" ) ) ; /* Only for immediate stop */ // PRQA S 3081

	{

		//Start the downloading.
		//At the end (even error) release the exclusive lock.
		Cache::AutoRelease autorelease ( &_cache, cacheNameRelPath );
		try {
			dbDownloadFile* dwldInv = dynamic_cast<dbDownloadFile*> ( dbFileActionFactoryChain::instance()->newDownloadAction ( // PRQA S 3081
					_connectionPool, inv, cacheNameAbsPath) );

			ACS_COND_THROW ( !dwldInv, exIllegalValueException ( "DataDownloader::optimiser: Dynamic_cast failure." ) ); // PRQA S 3081
			dbFileActionScopeGuard guard ( dwldInv );

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START DOWNLOADING ITEM: [" << cacheNameAbsPath << "]");

			//I'm the exclusive owner of cacheNameAbsPath. Delete (if exist) before downloading
			File::unlink( cacheNameAbsPath );

			try {
				dwldInv->start ();
				while ( dwldInv->downloading() ) {
					ACS_COND_THROW(haltImmediate(), DataDownloaderStoppedException( "Operation stopped" ) ) ;/* Only for immediate stop */  // PRQA S 3081
					Timer::delay ( _confAdvDelayRealDownloader /* unit: ms */);
				}

			} catch (exception& ex) {
				//This is important. A downloaded file is good primarily if ISM checks are completed.
				// Function sharedLockInventoryItem_checkDownload_sharedLockDownloadedFiles_unlockInventoryItem will check the item againts its
				// expected name and size, but it is important to rely on ISM check first.
				ACS_LOG_WARNING("Anomaly detected while downloading the Inventory item ["<<cacheNameAbsPath<<"]. Removing partially downloaded files (if any).");
				File::unlink( cacheNameAbsPath );
				throw;
			}

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ITEM DOWNLOADED: [" << cacheNameAbsPath << "]");

			//Format the downloaded file
			{

				//No group case:
				// - Downloaded file is:		/cacheRoot/_relDownloadPath/name#ver#date
				// - Formatted is going to be:	/cacheRoot/_relDownloadPath/name[.ext|(.hdr,.dbl)]#ver#date
				//
				//Group case:
				// - Downloaded file is:		/cacheRoot/_relDownloadPath/group+name#ver#date
				// - Formatted is going to be:	/cacheRoot/_relDownloadPath/group/name[.ext|(.hdr,.dbl)]#ver#date

				const string filetype = inv.getFileType() ;

				dbConnectionWrapper pw ( _connectionPool );
				dbConnection &conn = pw; // PRQA S 3050

				dbFileTypeRecognition::ftype_traits traits( _filetyperec.getTraits(filetype, conn) ) ; /* Or returns exception */

				string headerFile ;
                string dblFile ;

				if ( cacheGroup.empty() ) {
					//Split/rename/leave as is
					File::splitMatchExtHandleTrailer ( cacheNameAbsPath,
							headerFile, dblFile, inv.getHeaderSize(), inv.getDataSize(), // PRQA S 3000 2
							traits.sngext, traits.hdrext, traits.datext,
							"#", traits.mngsngext,
							true, true );

					//If splitted, remove the original file
					if ( File::exists(cacheNameAbsPath) && cacheNameAbsPath != headerFile && cacheNameAbsPath != dblFile ) {
						File::unlink(cacheNameAbsPath);
					}

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Formatted Files: ["<<headerFile<<"], ["<<dblFile<<"];");

				} else {
					//Handle Group

					string cacheGroupRelPath = StringUtils::pathJoin( _relDownloadPath, cacheGroup);
					string cacheGroupAbsPath = StringUtils::pathJoin( _absDownloadPath, cacheGroup);

					File::mkdirtree( cacheGroupAbsPath ); 	//Create group if not existing
					_cache.use( cacheGroupRelPath );		//Be sure Cache does not roll the group (only shared locks on groups)
					Cache::AutoRelease autorelease2 ( &_cache, cacheGroupRelPath ); //Release the group after formatting

					//Create a temporary working dir for item formatting (split/rename/leave as is)
					// this working dir must be 1:1 with cacheNameAbsPath.
					string formattingPath = cacheNameAbsPath+"_FMT";
					std::replace(formattingPath.begin(), formattingPath.end(), '#', '_'); // '#' is a trailer used in File:: functions. Remove it.
					File::mkdirtree( formattingPath );

					//Move the downloaded inventory item in the temporary working dir
					string tmpCacheNameAbsPath = StringUtils::pathJoin( formattingPath, getCacheNameEntryForInventoryItem(inv, "") );
					File::rename( cacheNameAbsPath, tmpCacheNameAbsPath);

					//Format the downloaded inventory item in the temporary working dir
					File::splitMatchExtHandleTrailer ( tmpCacheNameAbsPath,
							headerFile, dblFile, inv.getHeaderSize(), inv.getDataSize(), // PRQA S 3000 2
							traits.sngext, traits.hdrext, traits.datext,
							"#", traits.mngsngext,
							true, true );

					//Move formatted files (splitted/renamed/left as is) in the shared locked group.
					if ( ! headerFile.empty() ) {
						string newCacheNameAbsPath = StringUtils::pathJoin( cacheGroupAbsPath, File::getFileName(headerFile)  );
						File::rename( headerFile, newCacheNameAbsPath );
					}
					if ( ! dblFile.empty() ) {
						string newCacheNameAbsPath = StringUtils::pathJoin( cacheGroupAbsPath, File::getFileName(dblFile)  );
						File::rename( dblFile, newCacheNameAbsPath );
					}

					//Formatted files are safe in the group. Can delete the temporary working dir now.
					File::unlink( formattingPath );

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Formatted Files: ["<<headerFile<<"], ["<<dblFile<<"];");
				}
			}								   

		} catch ( std::exception const& ex ) {
			ACS_LOG_ERROR("Error in downloading item " << ex.what());
			throw;
		}
	}
	//Here the downloaded item has been unlocked. Is it a risk? Could cache roll it before the check & share lock phase?
	//Technically yes, but in that case cache is not well dimensioned (a just touched entry is going to be rolled in next few milliseconds).
}


void DataDownloader::sharedLockInventoryItem_checkDownload_sharedLockDownloadedFiles_unlockInventoryItem (
		const dbInventoryObject& inv,
		const std::string& cacheGroup,
		bool& itemDownloaded,
		bool& sharedLockFailed,
		vector <DataDownloader::DownloadedEntry>& downloadedEntryVec)
{

	//Init input
	const string& fileName = inv.getFileName();
	const string& version = inv.getFileVersion();
	const string& fileType = inv.getFileType();
	const DateTime& inventoryDate = inv.getInventoryDate();
	const off_t& headerSize = inv.getHeaderSize(); // PRQA S 3000 2
	const off_t& dataSize = inv.getDataSize();

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called. Name: \"" << fileName << "\"  version: \"" << version << "\"");

	//Init output
	itemDownloaded=false;
	sharedLockFailed=false;
	downloadedEntryVec.clear() ;

	dbFileTypeRecognition::ftype_traits splitInfo;
	{
		// Release the connection as soon as possible 
		dbConnectionWrapper pw ( _connectionPool );
		dbConnection &conn = pw; // PRQA S 3050
		splitInfo = _filetyperec.getTraits(fileType, conn) ; /* Or returns exception */
	}

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, splitInfo.str());

	//Cache item on which threads/processes synch (through locks)
	const string cacheName = getCacheNameEntryForInventoryItem(inv, cacheGroup);
	const string cacheNameRelPath = StringUtils::pathJoin(_relDownloadPath, cacheName) ;

	try {

		//Try to Lock (shared) the main inventory item.

		_cache.use( cacheNameRelPath );
		Cache::AutoRelease autorelease ( &_cache, cacheNameRelPath ); //Release the inventory item when I finish the checks.

		//HERE I've locked the main inventory item.
		// This means: nobody is downloading it and nobody can start downloading it. So I can do all checks about its status (already downloaded and formatted).

		// if headerseparation is true check for the header + data  files
		if ( splitInfo.header_separator ) {

			string headerFile ;
            string dataFile;
			File::evalNamesSplitMatchExt ( fileName, headerFile, dataFile,
					splitInfo.sngext, splitInfo.hdrext, splitInfo.datext, splitInfo.mngsngext,
					true );

			const string headerFile_cache_entry = File::getCacheFileName ( headerFile, version, inventoryDate.Datetime2time_t() ) ;
			const string headerFile_abspath = StringUtils::pathJoin(StringUtils::pathJoin( _cache.getPath(), _relDownloadPath, cacheGroup), headerFile_cache_entry ) ;

			const string dataFile_cache_entry = File::getCacheFileName ( dataFile, version, inventoryDate.Datetime2time_t() ) ;
			const string dataFile_abspath = StringUtils::pathJoin(StringUtils::pathJoin( _cache.getPath(), _relDownloadPath, cacheGroup), dataFile_cache_entry );

			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Checking for item files: \"" << headerFile_abspath << "\" and \""<< dataFile_abspath << "\": ");

			if ( (File::exists(headerFile_abspath) && File::isReadable(headerFile_abspath)) \
					&& (File::exists(dataFile_abspath) && File::isReadable(dataFile_abspath)) ) {

				// Both files were found. Check size:

				if ( (File::size(headerFile_abspath) != headerSize) || (File::size(dataFile_abspath) != dataSize) ) {

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Files: \"" << headerFile_abspath << "\" and \""<< dataFile_abspath << "\" have invalid size (one or both)");

					itemDownloaded = false ;

				} else {

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Files: \"" << headerFile_abspath << "\" and \""<< dataFile_abspath << "\" already downloaded. Locking them.");

					// Shared lock datafile and header
					try {
						if ( cacheGroup.empty() ) {
							//Not using cache group: shared lock the items
							_cache.use (  StringUtils::pathJoin(_relDownloadPath, headerFile_cache_entry ) );
							_cache.use (  StringUtils::pathJoin(_relDownloadPath, dataFile_cache_entry ) );
						} else {
							//Using cache group: shared lock the group
							_cache.use( StringUtils::pathJoin(_relDownloadPath, cacheGroup) );
							_cache.use( StringUtils::pathJoin(_relDownloadPath, cacheGroup) ); //twice: for header and datafile.
						}
					} catch(exception& ex) {
						//Should never be here
						ACS_THROW ( DataDownloaderException (ex, "Inventory item ["+cacheName+"] downloaded and formatted, but shared lock failed.") );
					}

					{
						DataDownloader::DownloadedEntry downloadedEntry_data ;
						DataDownloader::DownloadedEntry downloadedEntry_hdr ;

						downloadedEntry_data.cacheRoot = _cache.getPath() ;
						downloadedEntry_data.cacheEntryRelPath = StringUtils::pathJoin( _relDownloadPath, cacheGroup, dataFile_cache_entry ) ;
						downloadedEntry_data.cacheLockedEntryIsGroup = !cacheGroup.empty();
						downloadedEntry_data.cacheLockedEntryRelPath = cacheGroup.empty()? downloadedEntry_data.cacheEntryRelPath : cacheGroup ; // PRQA S 3380
						downloadedEntry_data.fileName = dataFile ;
						downloadedEntry_data.isDataFile = true ;	
						downloadedEntryVec.push_back(downloadedEntry_data) ;

						downloadedEntry_hdr.cacheRoot = _cache.getPath() ;
						downloadedEntry_hdr.cacheEntryRelPath = StringUtils::pathJoin( _relDownloadPath, cacheGroup, headerFile_cache_entry ) ;
						downloadedEntry_hdr.cacheLockedEntryIsGroup = !cacheGroup.empty();
						downloadedEntry_hdr.cacheLockedEntryRelPath = cacheGroup.empty()? downloadedEntry_hdr.cacheEntryRelPath : cacheGroup ; // PRQA S 3380
						downloadedEntry_hdr.fileName = headerFile ;
						downloadedEntry_hdr.isDataFile = false ;	
						downloadedEntryVec.push_back(downloadedEntry_hdr) ;
					}

					itemDownloaded = true;	/* If ALL SUCCESSFULL the file(s) was downloaded and shaded locked */
				}
			}

		} else {

			// if headerseparation is false check for  data  file
			string dataFile ;
            string nullFile;
			// consider single file (eventually renamed because extension)
			File::evalNamesSplitMatchExt ( fileName, dataFile, nullFile,
					splitInfo.sngext, "", "", splitInfo.mngsngext, false );

			const string dataFile_cache_entry = File::getCacheFileName ( dataFile, version, inventoryDate.Datetime2time_t() ) ;
			const string dataFile_abspath = StringUtils::pathJoin(StringUtils::pathJoin( _cache.getPath(), _relDownloadPath, cacheGroup), dataFile_cache_entry );

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dataFile_cache_entry: "<<dataFile_cache_entry<<". dataFile_abspath: "<<dataFile_abspath);

			if (File::exists(dataFile_abspath) && File::isReadable(dataFile_abspath)) {
				// File exists. Check size:
				if (File::size(dataFile_abspath) != dataSize) {

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File: \"" << dataFile_abspath << "\" has invalid size.");

					itemDownloaded = false ;

				} else {

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File: \"" << dataFile_abspath << "\" already downloaded. Locking it.");

					// Shared lock on datafile
					try {

						if ( cacheGroup.empty() ) {
							//Not using cache group: shared lock the items
							_cache.use (  StringUtils::pathJoin(_relDownloadPath, dataFile_cache_entry ) );
						} else {
							//Using cache group: shared lock the group
							_cache.use( StringUtils::pathJoin(_relDownloadPath, cacheGroup) );
						}

					} catch(exception& ex) {
						//Should never be here
						ACS_THROW ( DataDownloaderException (ex, "Inventory item ["+cacheName+"] downloaded and formatted, but shared lock failed.") );
					}

					{
						DataDownloader::DownloadedEntry downloadedEntry_data ;
						downloadedEntry_data.cacheRoot = _cache.getPath() ;
						downloadedEntry_data.cacheEntryRelPath = StringUtils::pathJoin( _relDownloadPath, cacheGroup, dataFile_cache_entry ) ;
						downloadedEntry_data.cacheLockedEntryIsGroup = !cacheGroup.empty();
						downloadedEntry_data.cacheLockedEntryRelPath = cacheGroup.empty()? downloadedEntry_data.cacheEntryRelPath : cacheGroup ;  // PRQA S 3380
						downloadedEntry_data.fileName = dataFile ;
						downloadedEntry_data.isDataFile = true ;	

						downloadedEntryVec.push_back(downloadedEntry_data) ;
					}

					itemDownloaded = true;	/* If ALL SUCCESSFULL the file was downloaded and shared locked */
				}
			}

		}	// END: if ( splitInfo.header_separator )


	} catch ( Cache::CacheReserveException const& ex ) {

		//Cannot lock (shared) the main inventory item ==> download in progress

		ACS_LOG_PRIVINFO("Checking download status, Inventory item: \"" << fileName << "\"  version: \"" << version << "\"  is being downloaded by another thread.");

		itemDownloaded = false ;	/* Since the file is not ALREADY available */
		sharedLockFailed = true;

	} catch ( Cache::CacheLockException const& ex ) {

		//Cannot lock (shared) the main inventory item ==> download in progress

		ACS_LOG_PRIVINFO("Checking download status, Inventory item: \"" << fileName << "\"  version: \"" << version << "\"  is being downloaded by another process.");

		itemDownloaded = false ;	/* Since the file is not ALREADY available */
		sharedLockFailed = true;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory item: \"" << fileName << "\"  version: \"" << version << "\"  "  // PRQA S 3380
			<< (itemDownloaded? "FOUND in cache with correct size and successfully locked": "NOT FOUND in cache")
	);

	//Here the first shared lock on main inventory cache item has been released
	// During the downloaded phase:
	//  - If split, the main inventory cache item is not here anymore. Its lock file is here but it is not used (cache will delete it at rolling time).
	//  - If renamed, the main inventory cache item is not here anymore. Its lock file is here but it is not used (cache will delete it at rolling time).
	//  - If left as is, the main inventory cache item is here and shared locked by this method. Of course Its lock file is here and it is used.
	//  - If used group, the main inventory cache item is not here anymore. Its lock file is here but it is not used (cache will delete it at rolling time).
	//Erasing lock files (eg. using Cache::eraseEntryAndForget) is not allowed! It would compromise right cache functioning!!!

	//Are these unused files compromising the cache performance?
	// Not significantly because by default the cache runs in low accuracy mode (it does a "du" like operation instead a "find" like operation) to
	// compute the cache size. These unused files impacts cache performances at "rolling time" (where a "find" like operation is performed ).
	// It's acceptable because cache rolling is performed once a time.

}


void DataDownloader::setError ( std::vector <DownloadedEntry>& downloadedFiles )
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        ostringstream msg;
	    msg << "Method called. Entries downloaded: " << '\n' ;
        size_t dfsize = downloadedFiles.size() ;
	    for (size_t i=0; i<dfsize; ++i) { msg << downloadedFiles[i].str() ; }
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str());
	ACS_CLASS_END_DEBUG


	/*
		1: Removing the entries downloaded 
	 */
    size_t dfsize = downloadedFiles.size() ;
	for (size_t i=0; i<dfsize; ++i)
	{
		const string absPath = StringUtils::pathJoin( downloadedFiles[i].cacheRoot, downloadedFiles[i].cacheEntryRelPath ) ;
		::unlink( absPath.c_str() ) ;

	}	// END: for i 


	/*
		2: Removing the links (if any)
	 */
	for (size_t i=0; i<dfsize; ++i)
	{
		if (!downloadedFiles[i].symlinkPath.empty())
		{
			::unlink( downloadedFiles[i].symlinkPath.c_str() ) ;
			downloadedFiles[i].symlinkPath = "" ; 
		}

		if (!downloadedFiles[i].copyPath.empty())
		{
			::unlink( downloadedFiles[i].copyPath.c_str() ) ;
			downloadedFiles[i].copyPath = "" ; 
		}

	}	// END: for i 


	/*
		3: Unlocking entries
	 */
	for (size_t i=0; i<dfsize; ++i) {
		try {
			_cache.release ( downloadedFiles[i].cacheEntryRelPath );

		} catch(exception& e) {
			ACS_LOG_ERROR("Problems unlocking entry: " << downloadedFiles[i].cacheEntryRelPath << "\n" << e.what());
		}
	}

	downloadedFiles.clear() ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished");
}



void DataDownloader::createLinks ( const InventoryEntry& entryDownloaded, vector <DownloadedEntry>& downloadedFiles) const
{
	ACS_CLASS_BGN_DEBUG(10)
        ostringstream msg;
	    msg << "Method called. Entries downloaded: " << '\n' ;
        size_t dfsize = downloadedFiles.size() ;
	    for (size_t i=0; i<dfsize; ++i) { msg << downloadedFiles[i].str() ; }
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str());
	ACS_CLASS_END_DEBUG

	if (entryDownloaded.symlinkDir.empty() && entryDownloaded.copyDir.empty())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No link required");
	}
	else
	{
        size_t dfsize = downloadedFiles.size() ;
		for (size_t i=0; i<dfsize; ++i)
		{
			ACS_COND_THROW(haltImmediate(), DataDownloaderStoppedException( "Operation stopped" ) ) ; /* Only for immediate stop */ // PRQA S 3081

			if (! entryDownloaded.copyDir.empty())
			{
				if (! File::exists(entryDownloaded.copyDir) || ! File::isDir(entryDownloaded.copyDir) )
				{
					ACS_THROW(exIllegalValueException("Invalid directory: \"" + entryDownloaded.copyDir + "\"" ) ) ;// PRQA S 3081
				}

				const string src = StringUtils::pathJoin( downloadedFiles[i].cacheRoot, downloadedFiles[i].cacheEntryRelPath ) ;
				const string dst = StringUtils::pathJoin( entryDownloaded.copyDir, downloadedFiles[i].fileName ) ;	

				::unlink(dst.c_str()) ;	/* Error not checked */ 

				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to create the hardlink/copy: \"" << dst << "\"  from entry: \"" << src << "\"");

				const bool copyOnError = true ;	/* copy in case of hardlink error */
				bool copiedOnError = false ;
				File::hardlink(src,dst, copyOnError, &copiedOnError ) ;

				ACS_LOG_PRIVINFO("Successfully created the " << (copiedOnError? "copy" : "hardlink") // PRQA S 3380
                             << ": \"" << dst << "\"  from entry: \"" << src << "\"");

				downloadedFiles[i].copyPath = dst ; 
			}

			if (! entryDownloaded.symlinkDir.empty())
			{
				if (! File::exists(entryDownloaded.symlinkDir) || ! File::isDir(entryDownloaded.symlinkDir) )
				{
					ACS_THROW(exIllegalValueException("Invalid directory: \"" + entryDownloaded.symlinkDir + "\"" ) ) ; // PRQA S 3081
				}

				const string src = StringUtils::pathJoin( downloadedFiles[i].cacheRoot, downloadedFiles[i].cacheEntryRelPath ) ;
				const string dst = StringUtils::pathJoin( entryDownloaded.symlinkDir, downloadedFiles[i].fileName ) ;	

				::unlink(dst.c_str()) ;	/* Error not checked */ 

				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to create the symlink: \"" << dst << "\"  from entry: \"" << src << "\"");

				File::symlink(src, dst) ;

				ACS_LOG_PRIVINFO("Successfully created the symlink: \"" << dst << "\"  from entry: \"" << src << "\"");

				downloadedFiles[i].symlinkPath = dst ;
			}
		}	// END: for i 
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished");
}

_ACS_END_NAMESPACE // acs

