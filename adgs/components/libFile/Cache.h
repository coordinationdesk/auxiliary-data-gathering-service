// PRQA S 1050 EOF
/*

	Copyright (C) 2010-2019, Advanced Computer Systems , S.p.A.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Cache Library $

	$Id$

	$Author$

	$Log$
	Revision 5.89.0  2019/02/11 00:00:00  enrico.carta
	S1PDGS-32131: EC:: 1. A list of all the files removed by the cache (both automatically and as-per-user request) is handled, if the cache is requested to do it (default: legacy, no list).
	2. The default behavior of the cache is to start the automatic clean-up when is full (no free space available). Optionally, a parameters to set up a minimum threshold can be passed: in this case the automatic clean-up will be triggered when the free space is below that threshold. Default: legacy value (0 bytes)

	Revision 5.12  2014/06/05 15:50:50  enrcar
	EC:: Added a ex-lock token file (as multi-process/thread semaphore) in order to prevent multiple cleaning of the Cache (this might overload the system)
	
	Revision 5.11  2014/05/26 14:59:29  enrcar
	EC:: Added method removeOrphanLockOlderThanDays to recursively remove orphan lockfile as long as older then a minimum number of days (threshold)
	
	Revision 5.10  2014/05/22 09:10:07  enrcar
	EC:: In order to increase the robustness of the file locking (the system call flock) when another process is contually trying to create and remove the lock file, the default behaviour was changed. From: one single locking attempt to trying-to-acquire the lock within 1 second (several attempts might occur)
	
	Revision 5.9  2014/04/02 15:40:00  enrcar
	EC:: a constant value was expressed on a 32bit number, therefore the final result was downcasted. Fixed.
	
	Revision 5.8  2014/03/07 11:15:23  enrcar
	EC:: MAJOR MODIFICATION :
	In earlier versions, if the Cache contained several files (n) and was used in a multi process environment, a full-dir-scan was performed when locking/unlocking each resource.
	Thus resulting in a complexity O(n) per operation.
	To dramaticaly reduce the workout, a new approach was introduced, reducing the complexity to O(1), by reducing/avoiding any directory scan.
	The scan will be actually performed only when a cache-cleaning seems to be needed. The fastest possible scan will be applied and the oldest files removed until threashold is met.
	This new approach (called lowAccuracy) is now the default. To force the cache using the original approach (maximum accuracy, lower performances) just call the method setInitHighAccuracy after the c'tor.
	Also, a new optional parameter was provided inside the c'tor: A percentage PCG (range 0.0-1.0) relative to the maximum cache size.
	After each automatic cleanup to make space, a further portion of size (PCG * max_size) will be freed, if available. The aim is to reduce the number of automatic cleanups.
	
	Revision 5.7  2013/12/05 09:56:43  enrcar
	EC:: Default timeout for the File::tryLock modified to immediately emit exception in case of problems when acquiring a lock.
	
	Revision 5.6  2013/12/04 19:04:05  marpas
	qa warnings and MP warnings removed
	
	Revision 5.5  2013/12/04 15:28:01  enrcar
	EC:: The timeout of the basic libFile::lock (expected to acquire lock or return exception) is now configurable via Cache::setLockTimeout(sec). Default 1 sec, could be even set to zero.
	
	Revision 5.4  2013/10/31 11:23:55  marpas
	qa warning fixed
	
	Revision 5.3  2013/07/03 16:41:25  marpas
	continuing adoption of libException 5.x standards
	qa warnings
	
	Revision 5.2  2013/07/02 17:31:26  marpas
	adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
	
	Revision 5.1  2013/06/21 12:05:25  marpas
	adoption of libExcpetion 5.x standards in progress
	
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 2.40  2012/12/02 11:53:07  marpas
	CompressionMode insertion operators added
	
	Revision 2.39  2012/11/30 14:09:44  marpas
	qa rules and optimization
	
	Revision 2.38  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.37  2012/07/24 14:31:14  clanas
	friend statement fixed in class Entry
	
	Revision 2.36  2012/05/18 13:05:14  marpas
	EC:: Bug fixed: a multiplication between two large 32bit operands was performed. The result was still a 32bit number, obviously overflowed. As solution, the const has been declared to be size_t in the class.
	MP: NB, thi bug was already fixed in a clirsen branch, but erroneously applied to 2.40 version of Cache.C instead than the 2.42 (that was the HEAD).
	
	Revision 2.35  2012/04/11 10:41:37  enrcar
	EC:: Method makeSpaceReserveAndUse : an optional parameter "shared" was added, to allow the creation of SHARED reservation locks (default is: EXCLUSIVE)
	
	Revision 2.34  2012/02/08 13:55:25  enrcar
	MP:: Refactoring in progress
	
	Revision 2.33  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.32  2011/09/26 15:53:39  enrcar
	EC:: Modifications in order to handle more complex payloads (e.g. the delayed entry-erase)
	Public method eraseEntryAndForget totally implemented
	
	Revision 2.31  2011/09/22 14:48:44  marpas
	method eraseEntryAndForget added, waiting for EC implementation: actually calls eraseEntry
	
	Revision 2.30  2011/05/13 15:24:00  enrcar
	EC:: debug levels modified
	
	Revision 2.29  2011/05/13 12:47:38  enrcar
	EC:: added static method: ReserveSpace::checkAndCreateReservationDir
	
	Revision 2.28  2011/05/13 09:20:22  enrcar
	EC:: Bug fixed in ReserveSpace::deleteReservation when file already removed. FIXED (0=GOOD is returned both if file doesn't exist and if removed successfully)
	
	Revision 2.27  2011/05/12 11:50:00  enrcar
	EC:: Added method Cache::ReserveSpace::reservationFileExists()
	
	Revision 2.26  2011/05/10 15:20:48  enrcar
	EC:: comments improved
	
	Revision 2.25  2011/05/06 15:41:40  enrcar
	EC:: added throw() to prevent dtor launching exceptions
	
	Revision 2.24  2011/04/21 15:01:03  enrcar
	EC:: Cache c'tor: quota can be <0, in this case the quota is assumed the occupation itself (100% full).
	
	Revision 2.23  2011/04/19 13:26:21  enrcar
	EC:: eraseEntry: return code was useless, removed.
	
	Revision 2.22  2011/04/19 12:54:28  enrcar
	EC:: Public method eraseEntry added
	Bug: updateCacheAndEntries wasn't able to create/update cacheUpdateFile if it was missing. FIXED.
	
	Revision 2.21  2011/04/12 15:49:00  enrcar
	EC:: missing try/catch added
	
	Revision 2.20  2011/04/11 15:26:43  enrcar
	EC:: complex payloads can now be stored inside lockfiles.
	Methods added: preventAutoErase,  setAutoErase
	
	Revision 2.19  2011/04/05 09:14:41  enrcar
	EC:: deleteReservation: last parameter defaulted to TRUE (most common use)
	
	Revision 2.18  2011/04/04 16:02:10  enrcar
	EC:: lockfile is used as a container for the type of the lock (so far: EX_WRITE, EX_ERASE, any).
	Two specific exceptions were added: CacheExWriteLockException and CacheExEraseLockException
	
	Revision 2.17  2011/04/01 15:05:50  enrcar
	EC:: added Cache::ReserveSpace static methods: createReservation() deleteReservation() readReservation() stillReserve()
	Added method: makeSpaceAndReserve(). Modified method makeSpaceReserveAndUse in order to accept an optional Reservation File
	
	Revision 2.16  2011/03/31 08:56:07  enrcar
	EC:: All reserve files are now stored in a subdirectory (.reservedir)
	A reserve file could contain an optional expiration date, as time_t
	Portability: data in reserve files is always stored as long64 bigend
	
	Revision 2.15  2011/03/25 17:11:23  enrcar
	EC:: Added method useIfFileExists. Method Cache::release: added boolean to re-lock the file
	
	Revision 2.14  2011/03/25 14:53:23  enrcar
	EC:: _lastUpdate now stored as timeeval (in order to distinguish different updates in the same second).
	Run several tests on multi-thread
	
	Revision 2.13  2011/03/22 14:04:38  enrcar
	EC:: Some threadsafe guard was missing. Added.
	Debug dramatically improved in order to help distinguishing separate threads
	
	Revision 2.12  2011/03/21 15:17:57  enrcar
	EC:: updateReservedSpace modified in order to always check the available space (and free more if needed)
	
	Revision 2.11  2011/03/16 15:28:22  enrcar
	EC:: added method updateReservedSpace
	
	Revision 2.10  2011/03/09 10:38:20  enrcar
	EC:: Added class: Cache::AutoRelease
	
	Revision 2.9  2011/03/08 13:31:05  enrcar
	EC:: Private method checkAndUpdateEntries not used anymore (removed).
	Private method removeNestedLocks added.
	
	Revision 2.8  2011/03/07 16:22:59  enrcar
	EC:: private methods manageCacheIrregularEntries and addLockExtensionToEntries added.
	
	Revision 2.7  2011/03/07 11:17:32  enrcar
	EC:: Cache::CacheLockException and Cache::CacheReleaseException exception introduced and thrown.
	makeSpaceReserveAndUse now returns the space freed (as off_t)
	
	Revision 2.6  2011/03/03 18:34:01  marpas
	no DateTime dependency - using boost instead
	
	Revision 2.5  2011/03/03 16:57:45  enrcar
	EC:: Several modifications !
	
	Revision 2.4  2011/02/03 16:00:14  micmaz
	work in progres...
	
	Revision 2.3  2010/11/29 23:38:14  marpas
	work in progress
	
	Revision 2.2  2010/11/29 20:41:31  marpas
	work in progress
	
	Revision 2.1  2010/11/25 20:26:31  marpas
	cache work in progress
	

*/
 
#ifndef _Cache_H_
#define _Cache_H_


#include <acs_c++config.hpp>
#include <File.h>
#include <exException.h>

#include <ctime>


_ACS_BEGIN_NAMESPACE(acs)

/*! \class Cache
	
	Utility class to manage caches.
*/
class Cache : public ThreadSafe  // PRQA S 2109, 2153
{
/* FORWARD DECLARATIONS: */
public:		/* Public Classes */
	class ReserveSpace ;
private:	/* Private Classes */
   	class Entry ;
/* END */
	
public:
    // PRQA S 2131, 2153, 2502 L1
	/* Generic exceptions: (derived directly from exException) */
	exDECLARE_EXCEPTION(CacheInvalidFilenameException,exException) ;
	exDECLARE_EXCEPTION(CacheFullException,exException) ;
	exDECLARE_EXCEPTION(CacheReserveException,exException) ;
	/* Lock-denied specific: exceptions (derived from CacheLockException) */
	exDECLARE_EXCEPTION(CacheLockException, exException) ;
    exDECLARE_EXCEPTION(CacheExWriteLockException, CacheLockException) ;
    exDECLARE_EXCEPTION(CacheExEraseLockException, CacheLockException) ;
	/* Internal exceptions: (never thrown out) */
	exDECLARE_EXCEPTION(CacheLockPayloadException,exException) ;
	// PRQA L:L1
    
	/** builds a cache on path :
	  *   Cache(const std::string &path, 		Absolute Path of the cache root. E.g. /raid0/CacheDir
	  *			off_t maxSize, 					Quota [bytes] of the cache -- <0 means: set to cache occupation (so cache will be 100% full)
	  *			double percentage,				Percentage (from 0.0 to 1.0) relative of the whole quota to be freed every time a cache-cleaning in required.
	  *											So if quota=100MB and perc=0.20, every time that a cleaning is needed, 20MB plus the requested size would be freed.
	  *											E.G. If the cache is full and 5 MB are requested, 25 MB would be freed. Use 0.0 to disable the feature. 		
	  *			bool updateCacheEveryLock		Set TRUE to update cache modification file even for locks (instead of real modification)
	  */
	Cache(const std::string &path, off_t maxSize, double eraseQuotaPercentage=0.0, bool updateCacheEveryLock = false) ;
	virtual ~Cache() throw () ;

public:

	/* Types of lock (to be stored on lockfile payload) */
	enum LockType { UN_GENERIC	= 0x00,		/* UNLOCKED */ 
					SH_GENERIC	= 0x10,		/* SHARED_LOCK:		OP_UNDEFINED (0x0) */
					EX_GENERIC	= 0x20,		/* EXCLUSIVE_LOCK:	OP_UNDEFINED (0x0) */
					EX_WRITE	= 0x21,		/* EXCLUSIVE_LOCK:	OP_WRITE (0x1) */
					EX_ERASE	= 0x22,  	/* EXCLUSIVE_LOCK:	OP_ERASE (0x2) */
					ER_ERROR	= 0xf0		/* UNABLE TO OBTAIN LOCK TYPE*/
	} ;	/* enum LockType */


	enum LockOper { NOP					= 0x00,		/* UNDEFINED / N/A */ 
					PREV_AUTOERASE		= 0x01,		/* Prevent AutoErase */
					DELETE_ENTRY_ASAP	= 0x10		/* A file that a user wanted to delete was locked: delete as soon as possible */
	} ;	/* enum LockOper */

	
	struct LockOperDateStruct {
		LockOper		op ;			/* When in memory: the endianess of the CPU is used. When on disk: int32 BIGEND */
		unsigned char 	__pad[4] ;		/* In order to force alignment every 8 bytes (longest type in structure) */ // PRQA S 4403
		signed long long int expdate ;	/* When in memory: the endianess of the CPU is used. When on disk: int64 BIGEND */
	} ; /* struct LockOperDateStruct (size: 4+4+8 bytes) */


private:	
	// declared but not defined
	Cache() ;
	Cache(const Cache &) ;
	Cache &operator=(const Cache &) ;


private:
	
	
	class Entry : public File::Lock {   // PRQA S 2109, 2153
	public:
		Entry() ;
		explicit Entry(const std::string &lockFileAbsPath, const unsigned short int tryLockTimeout, time_t mtime=time(0)) ; 
		virtual ~Entry() throw () ;
		Entry(const Entry &) ;
		Entry &operator=(const Entry &) ;
        void swap(Entry &) ;            // PRQA S 2121, 2502

		size_t useCount() ;
		size_t use(bool shared, Cache::LockType) ;
		size_t reserve(const std::string &absPathOfCache, const off_t& bytes, bool shared) ;
		void reservedSpace(std::string& reserveFileAbsPath, off_t& bytes) ;
		void touch() ;
		void updateReservedSpace(const off_t& new_bytes) ;
		size_t release(bool force=false) ;
		
		// equal if path is the same
		bool operator==(const Entry &) ;        // PRQA S 2070
		std::string debugString() const ; 
	
		void updateStat(time_t) ;
		void updateStat() ;
		signed long long int getBytes() const { return _bytes ; } 
		time_t getMTime () const { return _mtime ; }  
		bool setMarker(bool n) { bool old = _marker ; _marker = n ; return old ; } 
		bool getMarker() const { return _marker; }
		bool getShared() const { return _shared; }
	
		bool isErasable() const ;

		void setLockfilePayload(Cache::LockType t, const std::vector<Cache::LockOperDateStruct> &v_add, \
			const std::vector<Cache::LockOperDateStruct> &v_remove, bool reset=false) const ;
		void getLockfilePayload(Cache::LockType& t, std::vector<Cache::LockOperDateStruct>& v) const ;

	private:
		using Lock::lock ;
		using Lock::unlock ;
		using Lock::tryLock ;
		using Lock::setLock ;
	private:
    
		void throwExceptionFromFile(const std::string&) ;

		static const std::string _lockFilePattern;
        static const std::string _lockFileExt ;
		static ThreadSafe  _mutex ;
		size_t _useCount ;
		signed long long int _bytes ;	/* 64bit even on 32-bit hosts */
		time_t _mtime ;
		bool _marker ;
		bool _shared ;	
		Cache::ReserveSpace* _reserveSpace ;
		unsigned short int _tryLockTimeout ;	/* Unit is [s] */

		ACS_CLASS_DECLARE_DEBUG_LEVEL(Entry) ;
	} ;


public:
	class ReserveSpace {    // PRQA S 2109
	public:
		ReserveSpace(const std::string &path, off_t bytes) ;
		~ReserveSpace() throw() ;
		ReserveSpace(const ReserveSpace &) ;
		ReserveSpace &operator=(const ReserveSpace &) ;
		
		void stillReserve(off_t bytes=-1) ;
		void release() const ;
		off_t bytes() const { return _bytes ; } 
		std::string path() const { return _path ; } 
		std::string filename() const { return _absFileName ; } 
		
		/* STATIC METHODS  --  U S E   T H E S E  */
		/** Delete reservation file (if existent). Returns: -1 if empty string passed as filename. 0 if file doesn't exist, 
			or the return code of unlink command (0==GOOD)  
		*/
		static int deleteReservation(const std::string& cachePath,const std::string& fileName, bool useRelFileName=true) ;
		/** Return TRUE if reservation file (still) exists on filesystem. FALSE if already removed.
		*/
		static bool reservationFileExists(const std::string& cachePath,const std::string& fileName, bool useRelFileName=true) ;
	
		/* INTERNAL STATIC METHODS -- SHOULD NOT BE USED */
		static void checkAndCreateReservationDir(const std::string& cachePath) ;
		/** Create unique reservation file, regardless the free space available. This method is deprecated for final user. 
			USE Cache::makeSpaceAndReserve INSTEAD ! */
		static std::string createReservation(const std::string& cachePath, bool useRelFileName, off_t bytes, time_t expdate) ;
		static std::string createUniqueFileName(const std::string& cachePath, bool useRelFileName=false) ;
		/** Analyze reservation file (if existent). Fill bytes and expdate, if available. 
			Set each boolean to TRUE if information available. And vice-versa. */
		static void readReservation(const std::string& cachePath, const std::string& fileName, bool useRelFileName, off_t& bytes, bool& bbytes, time_t& expdate, bool& bexpdate) ;
		static void stillReserve(const std::string& absFileName, off_t bytes, time_t expdate=0) ;
    private:
        ReserveSpace() ; // unimplemented
	private:
		std::string _absFileName ;
		std::string _path ;
		off_t _bytes ;
		
		ACS_CLASS_DECLARE_DEBUG_LEVEL(ReserveSpace) ;
	} ;


public:
	class AutoRelease { // PRQA S 2109
	public:
		AutoRelease(Cache*, const std::string &filename) ;
		~AutoRelease() throw() ;
		AutoRelease(const AutoRelease &) ;
		AutoRelease &operator=(const AutoRelease &) ;
	
	private:
		void release() const ;
        AutoRelease() ; // unimplemented
	private:
		Cache* _pcache ;
		std::string _filename ;
	
		ACS_CLASS_DECLARE_DEBUG_LEVEL(AutoRelease) ;

	} ;
	
	
public:

	/** \brief set cache max size[bytes]
		set maximum size in bytes the cache should allow. 
		It is used when asking for free space to ompute how max size should be freed. \see freeSpace()
		\param maxSize bytes
	 
	 */
	void setMaxSize(off_t maxSize) ;
	
	/** returns reserverd space -- also remove "reservation files" that are too old
	*/
	off_t getReservedBytesAndCleanOldFiles() const ;

	/** returns max size (cache quota)
	*/
	off_t maxSize() const { return _maxSize; } 
	
	/** returns the size actually used by all files [bytes]: locked, not locked, reservations
	*/
	off_t bytes() const ;

	/** returns the size [bytes] used only by file actually being locked (so, excluding unlocked and reservations)
	*/
	off_t bytesUsed() const ;
	
	/** tries to free an amount of bytes in order to allow the requester to write new data.
	     Returns the number of bytes actually freed. The returned number could satisfy the request even 
		 if no files has been actually removed e.g. the actual used size is less than the maximum size \see setMaxSize()
		 The method check also if there is enogh (real) disk free.
		 \param wished - how many bytes to free
		 \param left - how many bytes available
		 \param filenamesRemoved - filled (updated) with the name of removed files, also including the files removed as per user request.
		 \param filesFreed - pointer to a variable that will contain the number of files really unlinked
	*/
	off_t emptySpace(off_t wished, off_t& left, size_t *filesFreed = 0 ) ;
	off_t emptySpace(off_t wished, off_t& left, std::vector <std::string>& filenamesRemoved,  size_t *filesFreed = 0 ) ;

	/** As above, but emits an exception if it couldn't free requested space
	*/
	off_t emptySpaceAndCheck(off_t wished, size_t *filesFreed = 0 ) ;
	off_t emptySpaceAndCheck(off_t wished, std::vector <std::string>& filenamesRemoved, size_t *filesFreed = 0 ) ;

	/** Computes and returns the free space left on cache (>=0). Also keeping into account the reservations
	*/
	off_t freeSpace() const ;
	/** removes one entry (and its lockfile) from the cache. Entry must not be locked of course ! 
	*/
	void eraseEntry(const std::string &relPath) ;
	/** removes one entry (and its lockfile) from the cache. 
     *  in case entry is locked, the function should register for file entry remotion as soon as possible.
	 */
	bool eraseEntryAndForget(const std::string &relPath) ;
	/** adds the file path without locking it
	*/
	size_t add(const std::string &relPath) ;
	/** adds the file path and locks it (or doesn't lock, accordingly to flag)
	*/
	size_t use(const std::string &relPath, bool lock=true) ;
	/** if (payload) file exists, lock it and return TRUE. Otherwise return FALSE 
	*/
	bool useIfFileExists(const std::string &relPath, size_t& count) ;
	/** adds the file path and locks it. Plus reserve space
	*/
	size_t reserveAndUse(const std::string &relPath, off_t bytes, bool shared=false) ;

	/** Create space, and return a reservation file (rel.path). The file, persistent, will expire on a specific date
		If not enough free space is available a CacheFullException will be released.
	*/
	std::string makeSpaceAndReserve(off_t bytes, time_t expdate) ;
	std::string makeSpaceAndReserve(off_t bytes, time_t expdate, std::vector <std::string>& filenamesRemoved) ;

	/** Create (and reserve) space, using the reservatiom ticket if provided. 
	    Then adds the file and exclusively locks. Return the freed space (unrelated with the wished space).
		If not enough free space is available a CacheFullException will be released.
	*/
	off_t makeSpaceReserveAndUse(const std::string &relPath, off_t size, const std::string & relReservedFileName="") ; // PRQA S 4412
	off_t makeSpaceReserveAndUse(const std::string &relPath, off_t size, std::vector <std::string>& filenamesRemoved, const std::string & relReservedFileName="") ; // PRQA S 4412

	/** Create (and reserve) space, using the reservatiom ticket if different than "". 
	    Then adds the file and locks (shared or exlusive). Return the freed space (unrelated with the wished space).
		If not enough free space is available a CacheFullException will be released.
	*/
	off_t makeSpaceReserveAndUse(const std::string &relPath, off_t size, const std::string & relReservedFileName, bool shared) ;
	off_t makeSpaceReserveAndUse(const std::string &relPath, off_t size, std::vector <std::string>& filenamesRemoved, const std::string & relReservedFileName, bool shared) ;

	/** Update reserved space for a file locked by THIS process. The free space will be ALWAYS checked (and freed to ensure requested space)
		If not enough free space is available a CacheFullException will be released. 
		Return the freed space (unrelated with the wished space).
	*/
	off_t updateReservedSpace(const std::string &relPath, off_t bytes) ;
	off_t updateReservedSpace(const std::string &relPath, off_t bytes, std::vector <std::string>& filenamesRemoved) ;

	/** 
		Set value: prevent_date for a file locked by THIS process. After the release, the file cannot be auto-erase till prevent_date. 
		Anyway if the file is used BEFORE prevent_date, this information will be removed.
		-1 means "keep forever" (or: "keep till next use, whenever")
		Return the count of shared users.
	*/
	off_t preventAutoErase(const std::string &relPath, time_t expdate) ;

	/** 
		Reset value: prevent_date for a file locked by THIS process. After the release, the file might be auto-erased
		Return the count of shared users.
	*/
	off_t setAutoErase(const std::string &relPath) ;

	/** 
		Set value in lockfile payload: DELETE_ENTRY_ASAP for a given entry. Entry must be already locked
		Return the count of shared users.
	*/
	off_t setDeleteASAP_Flag(const std::string &relPath) ;

	/** 
		Clear value in lockfile payload: DELETE_ENTRY_ASAP for a given entry, if set. Entry must be already locked
		Return the count of shared users.
	*/
	off_t clearDeleteASAP_Flag(const std::string &relPath) ;

	/** locks counter for file path is decremented. If locks counter reaches 0, the file is unlocked
	*/
	size_t release(const std::string &relPath, bool relock_shared=false) ;

	/** find the proper lock file, given a relative path
	*/
	std::string searchLockFilePath(const std::string &relPathIN, bool& found) const ;

	/** returns Cache Path
	*/
	std::string getPath() const { return _path; }
	
	/** the number of files in the cache
	*/
	size_t size() const ;
	
	/** the number of locked files in the cache (whose counter is greater than 0) 
	*/
	size_t sizeUsed() const ;
	
	/** Set the timeout while trying to acquire a lock. This means that when a lock is attempted, in case of problems the operation is retried for <= N seconds.
		Use 0 (default) to prevent any retry in case of problems (exception would be returned immedately */
	void setLockTimeout(unsigned short timeout) throw() { _tryLockTimeout = timeout ; /* Unit is [s]. Use 0 for NO TIMEOUT */ ; }
	
	/** Get the configured timeout while trying to acquire a lock 
	*/
	unsigned short getLockTimeout() const throw() { return _tryLockTimeout ; }

	/** Reset the timeout to a default value
	*/
	void resetLockTimeout() { _tryLockTimeout = _tryLockDefaultTimeout; } 

	
	/** provide debug informations: 
	*/
	void debugDumpCache() const ;
	static std::string debugDumpVector(const std::vector<std::string> &v) ;
	
	static std::string getTID() { std::ostringstream o ; o << pthread_self() ; return o.str() ; } 


	/** Set the flag and also init the Low-Accuracy mode 
	*/ 
	void setInitLowAccuracy() ;

	/** Set the flag and also init the High-Accuracy mode 
	*/ 
	bool setInitHighAccuracy() ;
	
	/** Return the value of flag lowAccuracy
	*/ 
	bool isLowAccuracy() const { return _lowAccuracy ; } 

	/** Set the EraseQuotaPercentage 
	*/ 
	void setEraseQuotaPercentage(double p) { _eraseQuotaPercentage = p; }

	/** Get the EraseQuotaPercentage 
	*/ 
	double getEraseQuotaPercentage() const { return _eraseQuotaPercentage ; }

	/** Set the QuotaMinThresholdPercentage [ unit: pcg 0.0-1.0 ] 
	*/ 
	void setQuotaMinThresholdPercentage(const double& v) { _quotaMinThresholdPercentage = v; } 

	/** Get the QuotaMinThresholdPercentage [ unit: pcg 0.0-1.0 ] 
	*/ 
	double getQuotaMinThresholdPercentage() const { return _quotaMinThresholdPercentage ; }

	/** Set the UpdateListRemovedFiles flag 
	*/ 
	void setUpdateListRemovedFilesFlag(const bool v=true) { _updateListRemovedFiles = v; } 

	/** Set the UpdateListRemovedFiles flag  
	*/ 
	bool getUpdateListRemovedFilesFlag() const { return _updateListRemovedFiles ; }


	/** Recursively remove all the orphan locks in the Cache, provided that the last access occurred more than D days ago
	*/ 
	bool removeOrphanLockOlderThanDays(const ssize_t& days) ;

private:
	
	/** adds the file path and locks it (or doesn't lock, accordingly to flag).
		The lock is SHARED if bytes<0 (no space to reserve), EXCLUSIVE if bytes>=0 (reserve space)
	*/
	size_t internalUse(	const std::string &relPath, off_t bytes, bool lk) ;

	/** adds the file path and locks it (or doesn't lock, accordingly to flag)
	*/
	size_t internalUse(	const std::string &relPath, off_t bytes, bool lk, bool shared) ;

	/** returns the use Count of the file being added NB it could be already present
	*/
	size_t addEntryToList(const std::string &, bool forceUpdate = false) ;

	/** remove one entry from both maps (_entriesMTime and _entries) and disk
	*/
	bool deleteEntry(const std::multimap<time_t, std::string>::iterator &i, 
                     const std::map<std::string, Entry>::iterator &e, 
                     bool forceErase, 
                     std::vector <std::string>& list_of_removed_files,
                     off_t& freedBytes) ;

	/** remove a complete path (files and subdirectories). Input is Lockfile absolute path
	*/
	bool deleteEntryFromDisk(const std::string &lockFileAbsPath, std::vector <std::string>& list_of_removed_files, off_t& freedBytes, bool abortOnError) const ;

	/** computes and returns the free space left on cache (it might also be NEGATIVE)
	*/
	off_t freeSpaceWithSign() const ;

	/** Check if provided file is a lockFile
	*/
	bool isLockFile(const std::string &s) const  ;

	/** recursively read entries 
	*/
	void readEntries(const std::string &absPath) ; 

	/** remove one or more entries (as pair of inputs) from EntriesMTime multimap. key is used just as HASH
	*/
	static bool removeFromEntriesMTime(std::multimap<time_t, std::string>&, const time_t&, const std::string&) ;
	
	/** update cache (if modified) and entries (if needed, or forced). OUT: true if updated.
	*/
	bool updateCacheAndEntries(bool cacheModified, bool rescanForced, bool lowAccuracy) ;

	/** manage irregular entries (i.e. without a matching .lock file). Called only the very first time
	*/ 
	bool manageCacheIrregularEntries() ;

	/** update the current size of the cache directory, useful if lowaccuracy is requested
	*/ 
	void evaluateLowAccuracyBytes() ;

	/** Recursively remove all the orphan locks inside a directory, provided that the last access occurred more than D days ago
	*/ 
	bool removeOrphanLockOlderThanDays(const std::string& relPath, const ssize_t& days) ;

	/** Get the QuotaMinThreshold [ unit: bytes ] 
	*/ 
	off_t getQuotaMinThreshold() const { return ( 1.0 * (_maxSize * _quotaMinThresholdPercentage) ) ; }

	/*
		Static methods :
	*/
	static bool addLockExtensionToEntries(const std::string& entry, bool isMainEntry=false) ;
	static signed long int cmpTimeval(const struct timeval& a, const struct timeval& b) throw () ;
	static std::string dumpTimeval(const struct timeval& a) ;
	static time_t get_time_t(const struct timeval& t) throw () ;
	static struct timeval getFileModificationTime (const std::string& path, bool* success=0) ;
	static bool removeNestedLocks(const std::string& path) ;
	
	static void dumpLockOperDateStructVector(const std::vector<Cache::LockOperDateStruct>&) ;
	static std::string lockTypeToString(Cache::LockType) ;
	static Cache::LockType intToLockType(int, bool emitException) ;
	static int lockTypeToInt(Cache::LockType) ;


	static const std::string _lockFilePattern ;
    static const std::string _lockFileExt ;
    static const std::string _cacheUpdateFileNameRelPath ;
    static const std::string _cacheEraseInProgressFileNameRelPath ;
	static const std::string _reserveSpaceFileNamePattern ;
    static const std::string _reserveSpaceRelDir ;
	static const time_t _reserveSpaceFilesSelfRemoveTime = 3600*12 ; // 12 hrs
    static const off_t megabyte_ = 1048576 ; 
	static const short int _tryLockDefaultTimeout = 1 /*[s]*/ ;	/**	If 0:	never retry when acquiring a lock: an exception is immedately thrown in case of problems. 
															If >=1:	allowed to retry in case of problems when acquiring a lock for <= N seconds [Unit: s] */


	std::string _path ;
	bool _updateCacheEveryLock ;
	std::map<std::string, Entry> _entries ;
	std::multimap<time_t, std::string> _entriesMTime ;
	struct timeval _lastUpdate ;
	bool _fullScanDone ;	/*	TRUE if an actual disk scan was performed at least once */
	bool _lowAccuracy ;		/*	TRUE if disk scan should be avoided until a free space is required. 
								FALSE if a disk scan can be performed every time is needed to keep the accuracy high */ 
	off_t _lowAccuracyBytes ;
	double _eraseQuotaPercentage ; /* Relative to the whole quota, range [0.0 - 1.0]. So 0.01 means that 1% of the quota will we freed for each cleanup */
	off_t _maxSize ;
	unsigned short int _tryLockTimeout ;	/* Unit is [s] */
	bool _updateListRemovedFiles ; 			/** Set to TRUE to keep a list of removed files */
	std::vector <std::string> _userRemovedFiles_V ; /**  List of files removed as per user request */
	double _quotaMinThresholdPercentage ; 	/** Used to trigger the automatic cleaning. Range [0.0 - 1.0]. So 0.1 means that when the free space is below 10% 
												the cache should be considered full.
												Let: quotaMinThreshold = (_quotaMinThresholdPercentage * _maxSize) [ actualized, from pcg. to bytes ] 
												Cache is full when (_maxSize - (cache_occupation + reserving_file)) < quotaMinThreshold */
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Cache) ;
} ;

_ACS_END_NAMESPACE
 
#endif	/* _Cache_H_ */

