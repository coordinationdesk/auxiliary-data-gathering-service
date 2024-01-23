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

 $Prod: DataDownloader HEADER FILE $

 $Id$

 $Author$

 $Log$
 Revision 1.13  2014/05/27 17:13:39  nicvac
 Cache entry cannot be erased.

 Revision 1.12  2014/05/16 15:09:28  nicvac
 Configuration setting to keep the cache cleaned. Cleaned cache means more performances when cache does stats on disk.

 Revision 1.11  2014/05/06 18:17:11  nicvac
 Min threshold for download waiting.

 Revision 1.10  2014/05/01 08:38:49  nicvac
 S2PDGS-667: test and fixing.

 Revision 1.9  2014/04/30 16:25:05  nicvac
 S2PDGS-667: cache group implementation.

 Revision 1.8  2014/04/29 15:53:35  nicvac
 DownloadingStatus added.

 Revision 1.7  2014/04/28 14:32:40  nicvac
 Refactoring. Main topics: shared lock during check phase (no one can write now during checking). Timer handling to prevent starvation and contingency in concurrent downloading.

 Revision 1.6  2013/10/03 13:45:21  nicvac
 acs sdk warnings processed.

 Revision 1.5  2013/06/21 16:16:39  nicvac
 Can Download also using inventory id.

 Revision 1.4  2013/05/20 14:24:35  enrcar
 EC:: greatest version returned if no version provided

 Revision 1.3  2013/05/20 13:46:07  enrcar
 EC:: First working version released

 Revision 1.2  2013/05/16 14:26:54  enrcar
 EC:: I/F fixed

 Revision 1.1.1.1  2013/05/16 09:35:23  enrcar
 Imported source



*/


#ifndef _DataDownloader_H_
#define _DataDownloader_H_

#include <acs_c++config.hpp>
#include <Thread.h>
#include <Cache.h>
#include <StringUtils.h>
#include <dbConnectionPool.h>
#include <dbFileTypeRecognition.h>

_ACS_BEGIN_NAMESPACE(acs)

//Needed prototypes
class dbInventoryObject;


///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// DataDownloader
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 *\brief A Class for downloading the files inventory from db
 *
 */
class DataDownloader: public StopController { // PRQA S 2109, 2153

public:


	struct InventoryEntry // PRQA S 2173, 2175
	{
		/**
			Used as INPUT STRUCTURE:
		        inventoryId: key to identify the entry on T_INVENTORY. If set has priority on inventoryFilename, inventoryFileversion
				inventoryFilename, inventoryFileversion : key to identify the entry on T_INVENTORY
				cacheGroupName (or ""): optional group name to be used by cache. If the item to be downloaded is in a group (e.g. it is in a
						child-parent relation in inventory, or you want group by day), it is convenient to specify a group. In this case the cache
						performs better (less locks and less items in the root cache's path).
						If used, the cache keeps lock on the group and not on the item. Item will be rolled along with the whole group.
				symlinkDir (or "") : optional directory to create a symlink
				copyDir (or "") : optional directory to create a copy/hardlink
		 * 
		 * Note: Can request download by: inventoryId XOR inventoryFilename XOR inventoryFilename, inventoryFileversion
		*/
	
		InventoryEntry() :
			inventoryId(0), 
            inventoryFilename(), 
            inventoryFileversion(), 
            cacheGroupName(), 
            symlinkDir(), 
            copyDir()
		{}

		InventoryEntry(const InventoryEntry &r) :
			inventoryId(r.inventoryId),
			inventoryFilename(r.inventoryFilename), 
            inventoryFileversion(r.inventoryFileversion),
			cacheGroupName(r.cacheGroupName),
			symlinkDir(r.symlinkDir), copyDir(r.copyDir)
		{}

		InventoryEntry& operator= (const InventoryEntry &r)
        {
            if (this != &r) {
				inventoryId = r.inventoryId ;
				inventoryFilename = r.inventoryFilename ; 
                inventoryFileversion = r.inventoryFileversion ;
				cacheGroupName = r.cacheGroupName ;
				symlinkDir = r.symlinkDir ; 
                copyDir = r.copyDir ; 
 		    }
            return *this ;
        }
        ~InventoryEntry() throw () {}

        std::string str() const throw() {
                std::ostringstream o ;
                o << "InventoryEntry{\n" 
					<< "\tinventoryId: \"" << inventoryId << "\"\n"
					<< "\tinventoryFilename: \"" << inventoryFilename << "\"\n" << "\tinventoryFileversion: \"" << inventoryFileversion << "\"\n"
					<< "\tcacheGroupName: \"" << cacheGroupName <<"\"\n"
					<< "\tsymlinkDir: \"" << symlinkDir << "\"\n" << "\tcopyDir: \"" << copyDir << "\"\n" ;
				o << "\t}  /* END: InventoryEntry */ \n" ;
				return o.str() ;
        }

		dbPersistent::IdType inventoryId; // PRQA S 2100 6
		std::string inventoryFilename ;
        std::string inventoryFileversion ;
        std::string cacheGroupName ;
        std::string symlinkDir ;
        std::string copyDir ;
	} ;	/* END: struct InventoryEntry */


	struct DownloadedEntry // PRQA S 2173, 2175
	{	
		/**
			Used as OUTPUT STRUCTURE:
				cacheRoot: Root of the cache the file is downloaded in
				cacheEntryRelPath: downloaded filename, relative to the cacheRoot (so "/file.txt" means "in the root cache")
				cacheLockedEntryRelPath: locked cache item. It is the same as cacheEntryRelPath if no cache group used. It is the cache group if cache group used.
				cacheLockedEntryIsGroup: locked cache item is a group.
				fileName: the requested filename (without any tag and any path, it usually differs from the name in the cache)
				symlinkPath (or "") : if defined: absolute path of the symlink
				copyPathr (or "") : if defined: absolute path of the hardlink / copy
				isDataFile: TRUE for the datafile, FALSE for the headerfile
		*/

		DownloadedEntry():
			cacheRoot(), 
            cacheEntryRelPath(), 
            cacheLockedEntryRelPath(), 
            cacheLockedEntryIsGroup(false), 
            fileName(), 
            symlinkPath(), 
            copyPath(), 
            isDataFile(false)
		{}

		DownloadedEntry(const DownloadedEntry &r) :
			cacheRoot(r.cacheRoot),
			cacheEntryRelPath(r.cacheEntryRelPath), 
            cacheLockedEntryRelPath(r.cacheLockedEntryRelPath), 
            cacheLockedEntryIsGroup(r.cacheLockedEntryIsGroup),
			fileName(r.fileName), 
            symlinkPath(r.symlinkPath), 
            copyPath(r.copyPath),
			isDataFile(r.isDataFile)
		{}

		DownloadedEntry& operator= (const DownloadedEntry &r)
        {
            if (this != &r) {
				cacheRoot = r.cacheRoot ;
				cacheEntryRelPath = r.cacheEntryRelPath ;
				cacheLockedEntryRelPath = r.cacheLockedEntryRelPath; 
                cacheLockedEntryIsGroup = r.cacheLockedEntryIsGroup;
				fileName = r.fileName ; 
                symlinkPath = r.symlinkPath ; 
                copyPath = r.copyPath ; 
				isDataFile = r.isDataFile ; 
 		    }
            return *this ;
        }
        ~DownloadedEntry() throw () {}

        std::string str() const throw() {
                std::ostringstream o ;
                o << "\tDownloadedEntry{\n" << "\t\tisDataFile: " << std::boolalpha << isDataFile << "\n" 
					<< "\t\tcacheRoot: \"" << cacheRoot << "\"\n" << "\t\tcacheEntryRelPath: \"" << cacheEntryRelPath << "\"\n" 
					<< "\t\tcacheLockedEntryRelPath: \"" << cacheLockedEntryRelPath << "\"\n"
					<< "\t\tcacheLockedEntryIsGroup: \"" << cacheLockedEntryIsGroup << "\"\n"
					<< "\t\tcacheEntryAbsPath: \"" << StringUtils::pathJoin(cacheRoot, cacheEntryRelPath) << "\"\n" 
					<< "\t\tfileName: \"" << fileName << "\"\n" ;
				if (!symlinkPath.empty()) { o << "\t\tsymlinkPath: \"" << symlinkPath << "\"\n" ; }
				if (!copyPath.empty()) { o << "\t\tcopyPath: \"" << symlinkPath << "\"\n" ; }
				o << "\t}  /* END: DownloadedEntry */ \n" ;
				return o.str() ;
        }

        // PRQA S 2100 L1
		std::string cacheRoot; 			/* The root of the cache, e.g. /raid0/cache1 */
		std::string	cacheEntryRelPath; 	/* The entry downloaded, e.g. dir1/filename.DBL#0001#123. This entry is locked in the cache.
								   	   	   NOTE: The full abs. pathname will be: cacheRoot+"/"+cacheEntryRelPath */
		std::string	cacheLockedEntryRelPath; 	/* The entry locked on cache for this item.
		 	 	 	 	 	 	 	 	 	 	 	InventoryEntry::cacheGroupName  = "" ==> it is the same as cacheEntryRelPath (locked the item itself);
													InventoryEntry::cacheGroupName != "" ==> it is the cacheGroupName (locked the group), e.g. dirGroup */
		bool		cacheLockedEntryIsGroup; /* True if the locked entry is a group (as requested in InventoryEntry) */
		std::string	fileName;			/* The actual filename, without any path or tag: e.g. "filename.DBL"*/
		std::string	symlinkPath;		/* Optional: a symlink, full path ("/tmp/filename.DBL") or "" */
		std::string	copyPath ;			/* Optional: a copy/hdlink, full path ("/tmp/filename.DBL") or "" */
		bool		isDataFile ;		/* False if header */
        // PRQA L:L1
	} ;	/* END: struct DownloadedEntry */


public:
    /*! class DataDownloaderException declaration */
    exDECLARE_EXCEPTION(DataDownloaderException,exException); // PRQA S 2131, 2502 2 
    exDECLARE_EXCEPTION(DataDownloaderStoppedException, DataDownloaderException); // PRQA S 2153
  
    DataDownloader(dbConnectionPool& , Cache& , const std::string& relative_download_path);	/** The path is relative to cache root. So "/" or "" means: cache root */
    virtual ~DataDownloader() throw ();

    /** Type of download definition */
    enum DownloaderType {
    	StillUnknown,
    	RealDownloader,		//I'm actually downloading the item
    	PollingDownloader, 	//I'm waiting another thread/process completes the download for me
    };

	/**	Input: inventory entry to download
	 *  Output: vector of DownloadedEntry (e.g. in case HDR/DBL split, two entries are returned) */
	std::vector <DownloadedEntry> download_files(const InventoryEntry&);

	/** Return the Downloader Type.
	 *  For example a client can choose to run another downloading thread if type is
	 *  PollingDownloader (because if this thread is a PollingDownloader, it is not actually using relevant resources) */
	DownloaderType getDownloaderType() const;


private:
    DataDownloader();
    DataDownloader(const DataDownloader&);
    DataDownloader & operator=(const DataDownloader&);

	void setError ( std::vector <DownloadedEntry>&  ) ;
	void createLinks ( const InventoryEntry&, std::vector <DownloadedEntry>& ) const ;

	/** Put a shared lock on the main Inventory item,
	 *   check the downloading status,
	 *   share lock formatted file(s),
	 *   release the shared lock on the main Inventory item.
	 *  Input:
	 *  - inv: Inventory object to download
	 *  - cacheGroup: name of item's cache group. Set to "" if cache grouping not needed.
	 *  Output:
	 *  - itemDownloaded: true if inventory item has been downloaded and formatted; Related files have been also shared locked.
	 *  - sharedLockFailed: true if shared lock on main inventory item failed: someone else locked exclusively the item (download in progress by other thread/process);
	 *  - downloadedEntryVec: downloaded files (formatted inventory item) */
    void sharedLockInventoryItem_checkDownload_sharedLockDownloadedFiles_unlockInventoryItem(
    		const dbInventoryObject& inv, const std::string& cacheGroup,
    		bool& itemDownloaded,
    		bool& sharedLockFailed,
    		std::vector <DownloadedEntry>& downloadedEntryVec) ;

    /** Download the Inventory Locked (exclusive) item. Format the item (split, rename, leave as is). Remove the exclusive lock.
     *  - inv: Inventory object to download
     *  - cacheGroup: cache the group, not the entry. Use "" if you want to cache the entry. */
    void downloadReservLockedInventoryItem_formatFiles_unlockInventoryItem(
    		dbInventoryObject& inv, const std::string& cacheGroup);

    /** Return the cache entry's name for a given inventory item.
     *  - inv: Inventory object
     *  - cacheGroup: name of item's cache group. Set to "" if cache grouping not needed. */
    std::string getCacheNameEntryForInventoryItem(const dbInventoryObject& inv, const std::string& cacheGroup ) const;


private:
    unsigned int _confMaxConnRetry;
	dbConnectionPool& _connectionPool;
	Cache & _cache;
	std::string _absDownloadPath ;
    std::string _relDownloadPath ; 
	dbFileTypeRecognition _filetyperec ; 

	/** Cf. DownloadingStatus */
	DownloaderType _downloaderType;

	/** Configured max time [sec] I can wait before the actual inventory item's download starts */
	double _confMaxTimeWaitingDownloadStart;

	/** Configured worst estimated download data rate [Mb/sec].
	 * Used internally to estimate max time I can wait that other processes/threads downloads the inventory item for me.
	 * Set to lower than actual rate: Lower values are safer: In contingency scenario the process will just wait more than needed.
	 * Contingency scenario is: another thread/process declares it is downloading for me, but it's not for some reason.
	 * High values could cause a systematic download failure. */
	double _confDownloadDataRate;

	/** Computed Max allowed elapsed time [sec] for actual downloading (or for waiting concurrent downloading)
	 *  cannot be less than this threshold [sec].  */
	double _confMinThresholdTimeWaitingDownload;

	/** Advanced configuration. Delay used for a Polling Downloader [ms] */
	Timer::Delay _confAdvDelayPollingDownloader;
	/** Advanced configuration. Delay used for a Real Downloader [ms] */
	Timer::Delay _confAdvDelayRealDownloader;
	/** Advanced configuration. Delay used for Starvation handling [ms] */
	Timer::Delay _confAdvDelayPreventStarvation;


    ACS_CLASS_DECLARE_DEBUG_LEVEL(DataDownloader);
};

_ACS_END_NAMESPACE // acs

#endif /* _DataDownloader_H_ */
