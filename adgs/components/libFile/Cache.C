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

	Revision 5.24  2015/11/25 16:18:49  enrcar
	S2PDGS-1328 : Warning(s) of cache full removed if this is due to misconfiguration (cache quota lower than free disk space).
	
	Revision 5.23  2015/05/25 14:55:36  marpas
	coding best practices applied
	
	Revision 5.22  2015/01/20 14:10:17  marfav
	close syscall exit code checked
	
	Revision 5.21  2014/06/05 15:50:50  enrcar
	EC:: Added a ex-lock token file (as multi-process/thread semaphore) in order to prevent multiple cleaning of the Cache (this might overload the system)
	
	Revision 5.20  2014/05/30 09:15:49  enrcar
	EC:: debug improved
	
	Revision 5.19  2014/05/28 15:25:48  enrcar
	EC:: improved robustness in case of a data file is removed during the stat of another process
	
	Revision 5.18  2014/05/26 14:59:29  enrcar
	EC:: Added method removeOrphanLockOlderThanDays to recursively remove orphan lockfile as long as older then a minimum number of days (threshold)
	
	Revision 5.17  2014/05/22 09:10:07  enrcar
	EC:: In order to increase the robustness of the file locking (the system call flock) when another process is contually trying to create and remove the lock file, the default behaviour was changed. From: one single locking attempt to trying-to-acquire the lock within 1 second (several attempts might occur)
	
	Revision 5.16  2014/05/21 09:26:51  enrcar
	EC:: improved robustness if a lock file is removed by a different process exactly during a tryLock
	
	Revision 5.15  2014/05/20 09:52:54  enrcar
	EC:: Added robustness when a lock-payload is removed/modified by a different process
	
	Revision 5.14  2014/03/17 16:11:10  enrcar
	EC:: a different, faster approach was introduce to evaluate the occupation of the Cache root directory (in lowAccuracy Mode)
	
	Revision 5.13  2014/03/07 11:15:23  enrcar
	EC:: MAJOR MODIFICATION :
	In earlier versions, if the Cache contained several files (n) and was used in a multi process environment, a full-dir-scan was performed when locking/unlocking each resource.
	Thus resulting in a complexity O(n) per operation.
	To dramaticaly reduce the workout, a new approach was introduced, reducing the complexity to O(1), by reducing/avoiding any directory scan.
	The scan will be actually performed only when a cache-cleaning seems to be needed. The fastest possible scan will be applied and the oldest files removed until threashold is met.
	This new approach (called lowAccuracy) is now the default. To force the cache using the original approach (maximum accuracy, lower performances) just call the method setInitHighAccuracy after the c'tor.
	Also, a new optional parameter was provided inside the c'tor: A percentage PCG (range 0.0-1.0) relative to the maximum cache size.
	After each automatic cleanup to make space, a further portion of size (PCG * max_size) will be freed, if available. The aim is to reduce the number of automatic cleanups.
	
	Revision 5.12  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.11  2014/01/28 16:50:30  marfav
	S2PDGS-597 Irregular entries management failures are not critical and CTOR can complete
	
	Revision 5.10  2014/01/23 11:14:01  marpas
	lock file remotion is prohibited: if the lockfile is removed, it could cause the race condition
	to occur, invalidating the meaning of the lock itself: prevent the race.
	As example of this behaviour let's assume 3 processes: A,B,C
	A puts a lock, B and C wait for the unlock (correct)
	A - removes the lock, let's assume B get the lock and C waits for it (correct).
	Now A erases from disk the file previously unlocked: B still have the lock, C creates a new lockfile and locks it (wrong, B and C are in race cond).
	If we suppose to remove the lockfile and only then unlock it, does not prevent the lock: B could create a new lockfile locking it while A still own the lock on the erase file and we are now in the race cond.
	So from now on lockfiles cannot be removed from the File::Lock class.
	
	Revision 5.9  2013/12/05 09:56:43  enrcar
	EC:: Default timeout for the File::tryLock modified to immediately emit exception in case of problems when acquiring a lock.
	
	Revision 5.8  2013/12/04 19:04:05  marpas
	qa warnings and MP warnings removed
	
	Revision 5.7  2013/12/04 15:28:01  enrcar
	EC:: The timeout of the basic libFile::lock (expected to acquire lock or return exception) is now configurable via Cache::setLockTimeout(sec). Default 1 sec, could be even set to zero.
	
	Revision 5.6  2013/11/12 10:35:36  marfav
	Rollback to version 5.4
	
	Revision 5.5  2013/11/11 17:07:19  marfav
	Work in progress. Patch for cache release errors when in multiprocessing Cache::release
	
	Revision 5.4  2013/07/24 19:24:58  marpas
	using universal_time instead of local_time
	
	Revision 5.3  2013/07/03 16:41:24  marpas
	continuing adoption of libException 5.x standards
	qa warnings
	
	Revision 5.2  2013/07/02 17:31:26  marpas
	adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
	
	Revision 5.1  2013/06/21 12:05:24  marpas
	adoption of libExcpetion 5.x standards in progress
	
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 2.48  2013/03/11 14:50:18  marpas
	File::tempname returns a FileGuard
	
	Revision 2.47  2013/03/11 13:04:54  marpas
	FDGuard is from libException (Guard.hpp)
	FileGuard completely rewritten
	interface rationalization, test compilation fiexed
	
	Revision 2.46  2012/11/29 18:55:23  marpas
	optimizing STL algo
	
	Revision 2.45  2012/05/21 12:19:28  matteo.airoldi
	Wrong Class instantation
	
	Revision 2.44  2012/05/18 13:05:14  marpas
	EC:: Bug fixed: a multiplication between two large 32bit operands was performed. The result was still a 32bit number, obviously overflowed. As solution, the const has been declared to be size_t in the class.
	MP: NB, thi bug was already fixed in a clirsen branch, but erroneously applied to 2.40 version of Cache.C instead than the 2.42 (that was the HEAD).
	
	Revision 2.42  2012/04/11 12:39:46  enrcar
	EC:: status message only shown in debug mode
	
	Revision 2.41  2012/04/11 10:41:37  enrcar
	EC:: Method makeSpaceReserveAndUse : an optional parameter "shared" was added, to allow the creation of SHARED reservation locks (default is: EXCLUSIVE)
	
	Revision 2.40  2012/02/08 13:55:25  enrcar
	MP:: Refactoring in progress
	
	Revision 2.39  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.38  2011/09/28 10:11:54  marpas
	namespaces fixed
	
	Revision 2.37  2011/09/26 15:53:39  enrcar
	EC:: Modifications in order to handle more complex payloads (e.g. the delayed entry-erase)
	Public method eraseEntryAndForget totally implemented
	
	Revision 2.36  2011/09/14 09:16:55  marpas
	include style changed
	
	Revision 2.35  2011/05/16 12:41:07  enrcar
	EC:: Cache-root dirname "normalized" (removing trailing / etc) bfr. being used
	
	Revision 2.34  2011/05/13 15:23:59  enrcar
	EC:: debug levels modified
	
	Revision 2.33  2011/05/13 12:47:38  enrcar
	EC:: added static method: ReserveSpace::checkAndCreateReservationDir
	
	Revision 2.32  2011/05/13 09:20:22  enrcar
	EC:: Bug fixed in ReserveSpace::deleteReservation when file already removed. FIXED (0=GOOD is returned both if file doesn't exist and if removed successfully)
	
	Revision 2.31  2011/05/12 11:50:00  enrcar
	EC:: Added method Cache::ReserveSpace::reservationFileExists()
	
	Revision 2.30  2011/05/11 14:19:27  enrcar
	EC:: invalid exception was issued. Fixed.
	
	Revision 2.29  2011/05/10 15:20:48  enrcar
	EC:: comments improved
	
	Revision 2.28  2011/05/06 15:41:40  enrcar
	EC:: added throw() to prevent dtor launching exceptions
	
	Revision 2.27  2011/04/22 12:34:04  marpas
	rules fixing in progress
	
	Revision 2.26  2011/04/21 15:01:03  enrcar
	EC:: Cache c'tor: quota can be <0, in this case the quota is assumed the occupation itself (100% full).
	
	Revision 2.25  2011/04/19 13:26:21  enrcar
	EC:: eraseEntry: return code was useless, removed.
	
	Revision 2.24  2011/04/19 12:54:28  enrcar
	EC:: Public method eraseEntry added
	Bug: updateCacheAndEntries wasn't able to create/update cacheUpdateFile if it was missing. FIXED.
	
	Revision 2.23  2011/04/12 15:49:00  enrcar
	EC:: missing try/catch added
	
	Revision 2.22  2011/04/12 09:59:50  enrcar
	EC:: Method getLockfilePayload: an exception was thrown/handled even if payload was good but empty. 
	FIXED, exception only thrown if not possible to access file.
	
	Revision 2.21  2011/04/11 15:26:43  enrcar
	EC:: complex payloads can now be stored inside lockfiles.
	Methods added: preventAutoErase,  setAutoErase
	
	Revision 2.20  2011/04/05 09:14:41  enrcar
	EC:: deleteReservation: last parameter defaulted to TRUE (most common use)
	
	Revision 2.19  2011/04/04 16:02:10  enrcar
	EC:: lockfile is used as a container for the type of the lock (so far: EX_WRITE, EX_ERASE, any).
	Two specific exceptions were added: CacheExWriteLockException and CacheExEraseLockException
	
	Revision 2.18  2011/04/01 15:05:50  enrcar
	EC:: added Cache::ReserveSpace static methods: createReservation() deleteReservation() readReservation() stillReserve()
	Added method: makeSpaceAndReserve(). Modified method makeSpaceReserveAndUse in order to accept an optional Reservation File
	
	Revision 2.17  2011/03/31 08:56:07  enrcar
	EC:: All reserve files are now stored in a subdirectory (.reservedir)
	A reserve file could contain an optional expiration date, as time_t
	Portability: data in reserve files is always stored as long64 bigend
	
	Revision 2.16  2011/03/28 10:24:25  enrcar
	EC:: debug improved
	
	Revision 2.15  2011/03/28 08:28:42  enrcar
	EC:: addEntryToList, readEntries: A mutex was misplaced, this resulted in a random removal of new entries just after the insertion. 
	Fixed, also performed a test with several threads.
	internalUse: forgot to touch file if lock not required, thus preventing method add() from working.
	
	Revision 2.14  2011/03/25 17:11:23  enrcar
	EC:: Added method useIfFileExists. Method Cache::release: added boolean to re-lock the file
	
	Revision 2.13  2011/03/25 14:53:23  enrcar
	EC:: _lastUpdate now stored as timeeval (in order to distinguish different updates in the same second).
	Run several tests on multi-thread
	
	Revision 2.12  2011/03/22 14:04:38  enrcar
	EC:: Some threadsafe guard was missing. Added.
	Debug dramatically improved in order to help distinguishing separate threads
	
	Revision 2.11  2011/03/21 15:17:57  enrcar
	EC:: updateReservedSpace modified in order to always check the available space (and free more if needed)
	
	Revision 2.10  2011/03/16 15:28:22  enrcar
	EC:: added method updateReservedSpace
	
	Revision 2.9  2011/03/09 10:38:20  enrcar
	EC:: Added class: Cache::AutoRelease
	
	Revision 2.8  2011/03/08 13:31:05  enrcar
	EC:: Private method checkAndUpdateEntries not used anymore (removed).
	Private method removeNestedLocks added.
	
	Revision 2.7  2011/03/07 16:22:59  enrcar
	EC:: private methods manageCacheIrregularEntries and addLockExtensionToEntries added.
	
	Revision 2.6  2011/03/07 11:17:32  enrcar
	EC:: Cache::CacheLockException and Cache::CacheReleaseException exception introduced and thrown.
	makeSpaceReserveAndUse now returns the space freed (as off_t)
	
	Revision 2.5  2011/03/03 18:34:01  marpas
	no DateTime dependency - using boost instead
	
	Revision 2.4  2011/03/03 16:57:45  enrcar
	EC:: Several modifications !
	
	Revision 2.3  2011/02/03 16:00:14  micmaz
	work in progres...
	
	Revision 2.2  2010/11/29 20:41:31  marpas
	work in progress
	
	Revision 2.1  2010/11/25 20:26:31  marpas
	cache work in progress
	

*/
 
#include <acs_c++config.hpp>

#include <RegExp.h>
#include <Dir.h>
#include <File.h>
#include <Cache.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <ACS_ByteSwap.h>
#include <functional>
#include <algorithm>
#include <list>
#include <boost/bind.hpp> // PRQA S 1013 4
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/file.h>
#include <fcntl.h>
#include <boost/date_time/posix_time/posix_time.hpp> // PRQA S 1013 2
#include <boost/date_time/local_time/local_time.hpp>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

using namespace boost ;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace boost::local_time ;
using acs::pattern::FDGuard ;

// PRQA S 1020, 1030 2
#define CACHE_CLASS_WRITE_DEBUG_TH(level, message) ACS_CLASS_WRITE_DEBUG_TH(level,"[" << Cache::getTID() << "] " << message) 
//Use this for timestamping: #define CACHE_CLASS_WRITE_DEBUG_TH(level, message) ACS_CLASS_WRITE_DEBUG_TH(level, to_iso_extended_string(microsec_clock::universal_time()) << " " << message) 



////////////////////////////////////////////////////////////
//							
//     Cache::AutoRelease
//							
////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(Cache::AutoRelease) 

//
//  Cache::AutoRelease NULL C'TOR
//
Cache::AutoRelease::AutoRelease(Cache* pcache, const std::string &filename) : _pcache(pcache), _filename(filename) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;

	ACS_COND_THROW( !_pcache , exIOException( "NULL cache ptr!" ) ) ; // PRQA S 3081 2
	ACS_COND_THROW( _filename.empty() , exIOException( "NULL filename!" ) ) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "cache ptr: " << static_cast<void*>(_pcache) <<  "  cache path: \"" <<  _pcache->getPath() << "\""   // PRQA S 3081
		  << "  filename: \"" << _filename << "\"") ;
}


//
//  Cache::AutoRelease D'TOR
//
Cache::AutoRelease::~AutoRelease() throw()  
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
		
	try
	{
		release() ;
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't release \"" << _filename << "\": Got exception: " << e) ;
	}
    // PRQA L:L1
}


//
//  Cache::AutoRelease COPY C'TOR
//
Cache::AutoRelease::AutoRelease(const AutoRelease &rhs) :
	_pcache(rhs._pcache), 
	_filename(rhs._filename) 

{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "called") ;
}


//
//  Cache::AutoRelease ASSIGNMENT C'TOR
//
Cache::AutoRelease& Cache::AutoRelease::operator=(const AutoRelease &rhs) 
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "assignment Method called") ;
	
	if (this != &rhs) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "this: \"" << _pcache->getPath() << "\"  \"" << _filename << "\"" 
					<< "rhs: \"" << rhs._pcache->getPath() << "\"  \"" << rhs._filename << "\"") ;
		if ( !( (_pcache->getPath()==rhs._pcache->getPath()) && (_filename == rhs._filename) ) ) { // different file:
			release () ;
		}
		
		// copy parameters
		_pcache = rhs._pcache ; 
		_filename = rhs._filename ; 
	
	}
	
	return *this ;
}


//
//  Cache::AutoRelease::release
//
void Cache::AutoRelease::release() const // PRQA S 4214
{	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Releasing: \"" << _filename << "\"") ;
	
	_pcache->release(_filename) ;
	return ;
} 


////////////////////////////////////////////////////////////
//							
//     Cache::ReserveSpace
//							
////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(Cache::ReserveSpace) 
namespace {
    string ptime2s(const ptime &t) 
    {
	    time_facet* output_facet = new time_facet("%Y-%m-%d %T.%f"); // PRQA S 4412

	    ostringstream os ;
	    os.imbue(locale(locale::classic(), output_facet));

	    os << t ;

	    return os.str() ;
    }
}
//
//  Cache::ReserveSpace NULL C'TOR
//
Cache::ReserveSpace::ReserveSpace(const string &path, off_t bytes) : _absFileName(), _path(path), _bytes(bytes)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "path: \"" << path << "\"  bytes: " << bytes) ;

	srand( int(time(0) * getpid()) ) ; // PRQA S 3000, 3081
	
	const bool returnRelFileName = false ;	/* Absolute Path */
	_absFileName = createUniqueFileName( _path, returnRelFileName ) ;

	stillReserve() ;
}


//
//  Cache::ReserveSpace D'TOR
//
Cache::ReserveSpace::~ReserveSpace() throw()
{
    // PRQA S 4631 L2
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
		
	try
	{
		release() ;
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't release path: Got exception: " << e) ;
	}
    // PRQA L:L2
}


//
//  Cache::ReserveSpace COPY C'TOR
//
Cache::ReserveSpace::ReserveSpace(const ReserveSpace &rhs) :
	_absFileName(rhs._absFileName),
	_path(rhs._path),
	_bytes(rhs._bytes)

{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "copy c'tor called") ;
    
    stillReserve() ;
}


//
//  Cache::ReserveSpace ASSIGNMENT C'TOR
//
Cache::ReserveSpace& Cache::ReserveSpace::operator=(const ReserveSpace &rhs) 
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "assignment method called") ;
	
	if (this != &rhs) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "this: \"" << _absFileName << "\"" 
				   << " rhs: \"" << rhs._absFileName << "\"") ;
		if (_absFileName != rhs._absFileName) { // different file:
			release() ;
		}
		
		// copy parameters
		_absFileName = rhs._absFileName;
		_path = rhs._path;
		_bytes = rhs._bytes;

		stillReserve() ;
	}
	
	return *this ;
}


//
//  Cache::ReserveSpace::stillReserve
//
void Cache::ReserveSpace::stillReserve(off_t bytes)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o ; 
		o << "(not static) method called for file: \"" << _absFileName << "\"" ;
		if (bytes >= 0) { o << "  New occupation: " << bytes ; }
		else { o << "  Occupation: " << _bytes ; }
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG
	
	ACS_COND_THROW( _absFileName.empty(), exIOException( "Empty filename: \"" +_absFileName+"\"" ) ) ; // PRQA S 3081

	if (bytes >= 0) { _bytes = bytes ; }

	ReserveSpace::stillReserve(_absFileName, _bytes) ;	// No expiration date provided

	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "(not static) Method finished") ;

	return ;
}  


//
//  Cache::ReserveSpace::release
//
void Cache::ReserveSpace::release() const // PRQA S 4214
{	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for file: \"" << _absFileName << "\"" ) ;
		
	const bool useRelFileName = false ;	// Absolute path
	int status_fordebug = ReserveSpace::deleteReservation(_path, _absFileName, useRelFileName) ;

	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished for file: \"" << _absFileName 
		  << "\"  file removed with status: " << status_fordebug ) ;

	return ;
} 


//
//  STATIC  Cache::ReserveSpace::readReservation
//
/** Analyze reservation file (if existent). Fill bytes and expdate, if available. 
	Set each boolean to TRUE if information available. And vice-versa.
*/
void Cache::ReserveSpace::readReservation(
	const string& cachePath, 
	const string& fileName, 
	bool useRelFileName, 
	off_t& bytes, 
	bool& bbytes, 
	time_t& expdate, 
	bool& bexpdate) 
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for file: \"" << fileName << "\"  useRelFileName: " << boolalpha << useRelFileName) ;

	
	// Check if cache root path is OK (o/w: exception). Then, if not existent, the reservationfiles dir is created.
	checkAndCreateReservationDir(cachePath) ;


	bytes=0; bbytes = false ;
	expdate=0; bexpdate = false ;
	
	string absFileName = (useRelFileName ? // PRQA S 3384, 3385
		(StringUtils::pathJoin(cachePath, Cache::_reserveSpaceRelDir, fileName)) : 
		fileName ) ;

	if (!fileName.empty() && File::exists(absFileName))	/* Not a typo: check if filename!="" AND check the existence of ABSFileName */
	{
		int fd = open(absFileName.c_str(), O_RDONLY) ; if (fd >= 0) {
					
			FDGuard g(fd) ;	// Self close file
			const int storedBigEnd = 1;
			ssize_t r = 0 ;
			bool success = true ;	// Setup value


			if (success)
			{
				unsigned long long int bufr = 0; 
				r = read (fd, &bufr, sizeof(bufr)) ; 
                success = (r == sizeof(bufr)) ; // PRQA S 3000
				if (success) { 
                    ACS_ByteSwap_SWAP(&bufr,sizeof(bufr),storedBigEnd) ; // PRQA S 3030, 3081, 3084, 3260, 4090
                    bytes = off_t(bufr) ; // PRQA S 3081 
                } 
                bbytes = success ; 
			}
			else {
				bbytes = false ;
            }

			if (success)
			{
				unsigned long long int bufr = 0 ; 
				r = read (fd, &bufr, sizeof(bufr)) ; 
                success = (r == sizeof(bufr)) ; // PRQA S 3000
				if (success) { 
                    ACS_ByteSwap_SWAP(&bufr,sizeof(bufr),storedBigEnd) ;  // PRQA S 3030, 3081, 3084, 3260, 4090
                    expdate = time_t(bufr) ;  // PRQA S 3081 
                } 
                bexpdate = success ;
			}
			else {
				bexpdate = false ;
            }
		}
		else
		{	
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Couldn't open file: \"" << fileName << "\"  useRelFileName: " << boolalpha << useRelFileName) ;
		}	// END if (fd >= 0)
	
	}	
	else
	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Couldn't file file: \"" << fileName << "\"  useRelFileName: " << boolalpha << useRelFileName) ;
	}	// END if (!fileName.empty() && File::exist(absFileName))


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o ; 
		o << "Method finished for file: \"" << fileName << "\"  useRelFileName: " << boolalpha << useRelFileName ;
		if (bbytes) { o << "  bytes: " << bytes ; }
		if (bexpdate) { 
			time_t now=time(0) ; 
			ptime d=from_time_t(expdate) ; 
            ptime n=from_time_t(now); 
			o << "  expiration: " << expdate << "  [" << ptime2s(d) << "]   ( now: " << now << "  [" << ptime2s(n) << "] )" ;  
		}
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG
}


//
//  STATIC  Cache::ReserveSpace::deleteReservation
//
int			/* OUT: -1 if empty string passed as filename. 0 if file doesn't exist, or the return code of unlink command (0==GOOD)  */ 
	Cache::ReserveSpace::deleteReservation(const string& cachePath, const string& fileName, bool useRelFileName) 
{	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for file: \"" << fileName << "\"  useRelFileName: " << boolalpha << useRelFileName) ;
	
	// Check if cache root path is OK (o/w: exception). Then, if not existent, the reservationfiles dir is created.
	checkAndCreateReservationDir(cachePath) ;


	int status = -1 ;	// Returned if empty string passed as filename
		
	if ( !fileName.empty() ) 
	{
		string absFileName = (useRelFileName ? // PRQA S 3384, 3385
			(StringUtils::pathJoin(cachePath, Cache::_reserveSpaceRelDir, fileName)) : 
			fileName ) ;

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Removing file: \"" << absFileName << "\"") ;

		if (File::exists(absFileName))
		{
			status = ::unlink(absFileName.c_str()) ;	// Ignore errors !!

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "File: \"" << absFileName << "\"  removed with status: " << status) ;
		}
		else {
			status = 0 ;	// No errors if file not present
        }
	}

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished for file: \"" << fileName << "\"  useRelFileName: "  << boolalpha << useRelFileName 
		  << "  returning: " << status) ;

	return status  ;
} 


//
//  STATIC  Cache::ReserveSpace::reservationFileExists
//
bool Cache::ReserveSpace::reservationFileExists(
	const string& cachePath, 
	const string& fileName, 
	bool useRelFileName) 
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for file: \"" << fileName << "\"  useRelFileName: " << boolalpha << useRelFileName) ;

	// Check if cache root path is OK (o/w: exception). Then, if not existent, the reservationfiles dir is created.
	checkAndCreateReservationDir(cachePath) ;


	string absFileName = (useRelFileName ? // PRQA S 3384, 3385
		(StringUtils::pathJoin(cachePath, Cache::_reserveSpaceRelDir, fileName)) : 
		fileName ) ;

	bool status = (!fileName.empty() && File::exists(absFileName)) ; /* Not a typo: check if filename!="" AND check the existence of ABSFileName */


	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Returning: " << boolalpha << status) ;
	
	return status ;
}


//
//  STATIC  Cache::ReserveSpace::createUniqueFileName
//
string Cache::ReserveSpace::createUniqueFileName(const string& cachePath, bool useRelFileName) 
{
/*	
	Unique filename random pattern is: .r_aaaaaabbbbbcccccc.reserve
		TT: Temporary Table
		aaaaaa: seconds from epoch, last 24 bit (resolution: 2^24 seconds, i.e. 194 days)
		bbbbb:  usec 
		cccccc:  24 bit random number
		Total: 3+(6+5+6)+8 = 28 bytes
*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for cachePath: \"" << cachePath << "\"  useRelFileName: " 
			<< boolalpha << useRelFileName) ;


	// Check if cache root path is OK (o/w: exception). Then, if not existent, the reservationfiles dir is created.
	checkAndCreateReservationDir(cachePath) ;


	bool success = false ; 
	string fileName ; 
    string relFileName ;

    for (size_t i=0; ((!success) && (i<4096)); ++i) // PRQA S 4400
	{
		struct timeval tv ; // PRQA S 4102
		unsigned int r[3] ; // PRQA S 4403 2
		char rn[64] ;
		ACS_COND_THROW( 0!=gettimeofday(&tv, 0), exIllegalValueException( "Error calling gettimeofday") ) ; // PRQA S 3081

		r[0] = static_cast<unsigned int> ( tv.tv_sec & 0x00ffffff ) ;	// Second from Epoch -- lower 24 bit are used (resolution: 194 days) // PRQA S 3003, 3081, 4400 3
		r[1] = static_cast<unsigned int> ( tv.tv_usec & 0x000fffff ) ;	// usec: range is 0-1e6 i.e. <= 20 bit
		r[2] = rand() & 0x00ffffff; // 24 bit random number // PRQA S 3000

		sprintf(rn, ".r_%06X%05X%06X.reserve", r[0], r[1], r[2]) ; // PRQA S 4400

		relFileName = rn ;
		fileName = StringUtils::pathJoin(cachePath, Cache::_reserveSpaceRelDir, relFileName) ;
		int fd = open (fileName.c_str(), O_CREAT|O_EXCL, 0666) ; // EXCLUSIVE CREATION MODE: will fail if file already exists !
		success = (fd>=0) ; 
        if (success) { 
            if  ( close(fd) != 0 ) 
		ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
	}
        else {
            usleep(1000* 1) /* 1 ms */ ; // PRQA S 4400
        }
	
	}	// for
	
	ACS_COND_THROW( !success, exIllegalValueException( "Error creating file") ) ; // PRQA S 3081

	string ret = (useRelFileName ? relFileName : fileName) ; // PRQA S 3384

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  Returning: \"" << ret << "\"") ;
	
	return ret ;
}


//
//  STATIC  Cache::ReserveSpace::stillReserve
//
void Cache::ReserveSpace::stillReserve(	const string& absFileName, 	/* Reserve filename, absolute path */
										off_t bytes,	
										time_t expdate)	/* payload valid only if (expdate>0). o/w expiration is omitted */
{
	ACS_CLASS_BGN_DEBUG(20)
		ostringstream o ; 
		o << "Method called for file: \"" << absFileName << "\"" ; ;
		o << "  bytes: " << bytes ; 
		if (expdate > 0) { 
			time_t now=time(0) ; 
			ptime d = from_time_t(expdate) ;
            ptime n = from_time_t(now) ;
			o << "  expiration: " << expdate << "  [" << ptime2s(d) << "]   ( now: " << now << "  [" << ptime2s(n) << "] )" ;  
		}
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG
	
	ACS_COND_THROW( absFileName.empty(), exIOException( "Empty filename: \"" +absFileName+"\"" ) ) ; // PRQA S 3081

	int fd = open (absFileName.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0666) ; 
	ACS_COND_THROW( (fd<0), exIOException( "Error opening file: \"" +absFileName+"\"" ) ) ; // PRQA S 3081
	FDGuard g(fd) ;	// self close(fd) 

	const int storeBigEnd = 1 ;

	// Write payload: bytes
	{
		unsigned long long int bufr = bytes ; // PRQA S 3000
		ACS_ByteSwap_SWAP(&bufr, sizeof(bufr), storeBigEnd) ;  // PRQA S 3030, 3081, 3084, 3260, 4090
		ssize_t w = write(fd, &bufr, sizeof(bufr)) ;
		ACS_COND_THROW( (w!=sizeof(bufr)) , exIOException( "Error writing file: \"" +absFileName+"\"" ) ) ; // PRQA S 3000, 3081
	}
	
	if (expdate > 0)	// Write payload: expdate
	{
		unsigned long long int bufr = expdate ; // PRQA S 3000
		ACS_ByteSwap_SWAP(&bufr, sizeof(bufr), storeBigEnd) ;   // PRQA S 3030, 3081, 3084, 3260, 4090
		ssize_t w = write(fd, &bufr, sizeof(bufr)) ;
		ACS_COND_THROW( (w!=sizeof(bufr)) , exIOException( "Error writing file: \"" +absFileName+"\"" ) ) ; // PRQA S 3000, 3081
	}


	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished for file: \"" << absFileName << "\"") ;
	
	return ;
}  


//
//  STATIC  Cache::ReserveSpace::createReservation
//
/** Create unique reservation file, regardless the free space available. This method is deprecated for final user. 
	USE Cache::makeSpaceAndReserve INSTEAD ! */
string Cache::ReserveSpace::createReservation(const string& cachePath, bool useRelFileName, off_t bytes, time_t expdate)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		time_t now=time(0) ; 
        ptime d = from_time_t(expdate) ;
        ptime n = from_time_t(now) ;
		ostringstream o ; o << "Method called for cachePath: \"" << cachePath << "\"  bytes: " << bytes
			<< "  expiration: " << expdate << "  [" << ptime2s(d) << "]   ( now: " << now << "  [" << ptime2s(n) << "] )" 
			<< "  useRelFileName: " << boolalpha << useRelFileName ;  
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW( (bytes<0) , exIOException( "Occupation cannot be NEGATIVE" ) ) ; // PRQA S 3081

	string absFileName ;
	string fileName = ReserveSpace::createUniqueFileName( cachePath, useRelFileName ) ;	/* Get unique filename */

	absFileName = (useRelFileName ? // PRQA S 3380, 3385
		(StringUtils::pathJoin(cachePath, Cache::_reserveSpaceRelDir, fileName)) : 
		fileName ) ;

	ReserveSpace::stillReserve(absFileName, bytes, expdate) ;	/* create and fill reserve file */

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: \"" << fileName << "\"") ;
	
	return fileName ;
}


//
//  STATIC  Cache::ReserveSpace::checkAndCreateReservationDir
//
void Cache::ReserveSpace::checkAndCreateReservationDir(const string& cachePath)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called for cache root: \"" + cachePath + "\"") ;
	
	// Check cache root is OK
	ACS_COND_THROW( !(File::exists(cachePath) && File::isDir(cachePath)),  // PRQA S 3081
		exIllegalValueException( "Invalid cache root: \"" + cachePath + "\"") ) ;	

	// Create absolute path for reservation dir:
	string reserveSpaceAbsDir = StringUtils::pathJoin(cachePath, _reserveSpaceRelDir) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Checking reserve files container: \"" + reserveSpaceAbsDir + "\"") ;

	// Check reservation dir is valid (it is not a file)
	ACS_COND_THROW( (File::exists(reserveSpaceAbsDir) && File::isRegularFile(reserveSpaceAbsDir)),  // PRQA S 3081
		exIllegalValueException( "Invalid reserve files container found in: \"" \
			+ cachePath + "\": \"" + reserveSpaceAbsDir + "\"") ) ;	/* found, but it's a file and not a directory */

	// Create reservation dir if needed
	if ( !(File::exists(reserveSpaceAbsDir) && File::isDir(reserveSpaceAbsDir)) )
	{
		::mkdir(reserveSpaceAbsDir.c_str(), 0777) ;	// Ignore errors (directory might exist, etc. -- A test will be run below) // PRQA S 4400
		FileGuard tmpName = File::tempname( StringUtils::pathJoin(reserveSpaceAbsDir, "tempfile."), "") ;/* Will be self-removed */
		File::touch(tmpName) ;	// Try to create a test entry. O/W: EXCEPTION ! // PRQA S 3050
	}

	return ;
}


////////////////////////////////////////////////////////////
//							
//     Cache::Entry
//							
////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(Cache::Entry) 
ThreadSafe Cache::Entry::_mutex ;

const string Cache::Entry::_lockFileExt = "lock" ;	// NO "." DOT
const string Cache::Entry::_lockFilePattern = "^.*" + ("\\." + (Cache::Entry::_lockFileExt)) + "$" ; 

//
//  Cache::Entry NULL C'TOR
//
Cache::Entry::Entry() : Lock("", false), _useCount(0), _bytes(0), _mtime(0), _marker(false), _shared(true), _reserveSpace(0), _tryLockTimeout(Cache::_tryLockDefaultTimeout)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
}


//
//  Cache::Entry C'TOR
//
Cache::Entry::Entry(const string &lockFileAbsPath, const unsigned short int tryLockTimeout, time_t mtime) : 
	Lock(lockFileAbsPath, false), 
	_useCount(0), 
	_bytes(0), 
	_mtime(mtime), 
	_marker(false), 
	_shared(true), 
	_reserveSpace(0),
	_tryLockTimeout(tryLockTimeout)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "lockFileAbsPath: \"" << lockFileAbsPath << "\"  tryLockTimeout: " << _tryLockTimeout << " mtime: " << _mtime ) ;
	
	RegExp regexp(_lockFilePattern) ; 
	
	// Given filename MUST BE A LOCKFILE
	ACS_COND_THROW( !regexp.match(lockFileAbsPath), exIOException("Invalid lockfile: " + filename()) ) ; // PRQA S 3081

	updateStat() ;
}


//
//  Cache::Entry D'TOR
//
Cache::Entry::~Entry() throw () 
{
    // PRQA S 4631 L3
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called") ; 

	try 
	{
		bool force = true ;
		release(force) ;
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't release entry. Absolute Path: \"" << filename() << "\" Got exception: " << e) ; 
	}
    // PRQA L:L3
}


//
//  Cache::Entry COPY C'TOR
//
Cache::Entry::Entry(const Entry &rhs) : 
    Lock(rhs.filename(), false),
    _useCount(0),
    _bytes(rhs._bytes),	
    _mtime(rhs._mtime),
    _marker(false),
    _shared(rhs._shared),	
    _reserveSpace(0),
    _tryLockTimeout(rhs._tryLockTimeout)	
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Copy Method called") ;

	ACS_COND_THROW(rhs.isLocked(), // PRQA S 3081
		exIllegalValueException(rhs.filename() + " is alredy locked")) ;
	ACS_COND_THROW(rhs._reserveSpace, // PRQA S 3081
		exIllegalValueException(rhs.filename() + " has reserved space")) ;
	
	updateStat() ;
	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Copy Method finished") ;
}


//
//  Cache::Entry ASSIGNMENT C'TOR
//
Cache::Entry& Cache::Entry::operator=(const Entry &rhs) // PRQA S 4020
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Assignment Method called") ;
	
	if (this != &rhs) {
		ACS_COND_THROW(rhs.isLocked(), // PRQA S 3081
			exIllegalValueException(rhs.filename() + " is alredy locked")) ;
		ACS_COND_THROW(rhs._reserveSpace, // PRQA S 3081
			exIllegalValueException(rhs.filename() + " has reserved space")) ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "this: \"" << filename() << "\"" 
				   << " rhs: \"" << rhs.filename() << "\"") ;
        
        Entry tmp(*this) ;
        
        try {
		    if (filename() != rhs.filename()) { // different file:
			    if (_reserveSpace) { delete _reserveSpace ;	_reserveSpace=0; } /* Avoid memory leakages */
			    unlock() ;	// Unlock this->file, otherwise it will be overwritten !
			    const bool addExt = false ;
			    setLock(rhs.filename(), addExt) ;	// Set new (a different) file
		    }
        }
        catch(...) {
            std::swap(*this, tmp) ;
            throw ;
        }
        return *this ;
        
		
		// copy parameters
		_useCount = 0 ; 
		_bytes = rhs._bytes ; 
		_mtime = rhs._mtime ;
		_shared = rhs._shared ;
		_tryLockTimeout = rhs._tryLockTimeout ;	

		_marker = false ;	/* Copied objects aren't marked */
		_reserveSpace = 0 ;	/* Copied object don't reserve space */
		updateStat() ;
	}

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Assignment Method finished") ;
	
	return *this ;
}


//
//  Cache::Entry::swap
//
void Cache::Entry::swap(Entry &rhs ) 
{
    if (this != &rhs) {
        Lock::swap(rhs) ;
	    std::swap(_useCount, rhs._useCount) ;
	    std::swap(_bytes, rhs._bytes) ;
	    std::swap(_mtime, rhs._mtime) ;
	    std::swap(_marker, rhs._marker) ;
	    std::swap(_shared, rhs._shared) ;
	    std::swap(_reserveSpace, rhs._reserveSpace) ;
 	    std::swap(_tryLockTimeout, rhs._tryLockTimeout) ;
   }
}


//
//  Cache::Entry::debugString
//
std::string Cache::Entry::debugString() const 
{
	ostringstream s ;
	s << "Entry: \"" << filename() << "\"  useCounter: " << _useCount 
	  << "  bytes: " << _bytes << "  mtime: " << _mtime 
	  << "  reservedSpace[Ptr]: " << _reserveSpace << "  tryLockTimeout[s]: " << _tryLockTimeout ;
	return s.str() ;
}


//
//  Cache::Entry::updateStat
//
void Cache::Entry::updateStat() // PRQA S 4020
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "lockfile: \"" << filename() << "\"") ;
	
	if (filename().empty()) { return ; }
	
	// Evaluating size (bytes) for entry corresponding to lockfile :
	
	const string entry = File::removeExt(filename()) ;
	
	try {
		if ( !File::exists(entry) )
		{
			// Lockfile doesn't have any correspondence -- size is 0

			_bytes = 0 ;
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Filename: \"" << filename() << "\"  Entry: \"" << entry << "\"  not found. Size assumed zero.") ;
		}
		else
		{
			if ( File::isDir(entry) ) {
				_bytes = Dir::bytes(entry) ;
        	}
			else {
				_bytes = File::size(entry) ;
        	}

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Entry: \"" << entry << "\"  is a " << (File::isDir(entry)? "direct." : "file") // PRQA S 3380
					   << "  size: " << _bytes << " bytes") ;
		}
	} 
	catch(std::exception &e) 
	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Warning: Couldn't not evaluate the size of entry. Assiming zero. Absolute Path: \"" << entry << "\"  Got exception: " << e) ; 
		_bytes = 0 ;
	}

	ACS_CLASS_BGN_DEBUG(20)
		ptime d = from_time_t(_mtime) ;
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. lockfile: \"" << filename() << "\"  mtime: " 
		  << _mtime << " (" << ptime2s(d) << ")  size: " << _bytes << " bytes") ;
	ACS_CLASS_END_DEBUG
}


//
//  Cache::Entry::updateStat
//
void Cache::Entry::updateStat(time_t mtime)
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "lockfile: \"" << filename() << "\"  mtime: " << mtime) ;
	
	_mtime = mtime ;	// set new value
	
	updateStat() ;
}


//
//  Cache::Entry::useCount
//
size_t Cache::Entry::useCount() // PRQA S 4120, 4211
{
	return _useCount ;
}


//
//  Cache::Entry::use
//
size_t Cache::Entry::use(bool shared, Cache::LockType type)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "lock type: " << (shared? "SH" : "EX" ) << "  [" << Cache::lockTypeToString(type) << "]"  // PRQA S 3380
		  << "  Filename: \"" << filename() << "\"  isLocked: " << boolalpha << isLocked() <<"  tryLockTimeout: " << _tryLockTimeout ) ;

	if (! isLocked())	
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "lock type: " << (shared? "SH" : "EX" ) << "  [" << Cache::lockTypeToString(type)   // PRQA S 3380
				<< "]  Trying to lock... (isLocked: " << boolalpha << isLocked() ) ;

		// Not yet locked (at least, by this thread)
		unsigned short attempts = _tryLockTimeout ;	/* This is the number of attempts, i.e. the numer of SECONDS to wait while trying to acquire the lock */
		size_t attemptTimeout = 1000 ; 		/* Timeout for each attempt: 1000ms, I.E. ALWAYS 1 SECOND */ 		

		// NOTE: attempt == 0 cannot be used, since it means: wait forever. Therefore if minimum waiting is requested, one single attempt will be performed
		if (attempts<1) // PRQA S 3000
		{
			attempts = 1 ; attemptTimeout = 0 ;	/* This is the minimum amount of time: 1 single attempt and no timeout */
		}
		
		try {
		
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Trying to aquire lock. Type: " << (shared? "SH" : "EX" ) << "  Attempts: " << attempts << "  Timeout per each [ms]: " << attemptTimeout ) ; // PRQA S 3380
			tryLock(*this, attempts, attemptTimeout, shared) ;	// flock the lockfile // PRQA S 3000
		}
		catch(exIOException &e)	/* Since exceptions due to lock-denial belong to this type */
		{
			ostringstream o ;
			o << "Couldn't lock entry (" << __LINE__ << ")  Absolute Path: \"" << filename() << "\"  Got exception: " << e.what() ; 
			throwExceptionFromFile (o.str()) ; 	/* Forge a specific lock-denial exception */	 // PRQA S 3081
		}
		catch(std::exception &e) 
		{
			ACS_LOG_ERROR("Couldn't lock entry. Absolute Path: \"" << filename() << "\"  Got exception: " << e) ; 
			throw ;		/* Throw all other exceptions */
		}

		try 
		{
			// IF LOCK SUCCESSFUL : Prepare the new payload for the lockfile :
			vector<Cache::LockOperDateStruct> empty_v_add ; 	// Nothing to add
			vector<Cache::LockOperDateStruct> v_remove ; 

			Cache::LockOperDateStruct oper ;// PRQA S 4102
			memset(&oper, 0, sizeof(oper)) ;
			oper.op = Cache::PREV_AUTOERASE ;					// Remove from current payload (if present): PREV_AUTOERASE
			v_remove.push_back(oper) ;

			setLockfilePayload(type, empty_v_add, v_remove) ;	// Build new payload
		}
		catch(std::exception &e)
		{
			ACS_LOG_ERROR("Couldn't set lockfiletpe for entry (" << __LINE__ << ")  Absolute Path: \"" << filename() << "\"  Got exception: " << e) ; 
		}

	}
	else if ( (!shared) || (!_shared) )
	{
		// Already locked with incompatible lock type
		ostringstream o ;
		o << "Already locked with incompatible lock type  (" << __LINE__ << ")  Absolute Path: \"" << filename() << "\"" ;
		throwExceptionFromFile (o.str()) ; 		
	}
    else { /* nothing */ }

	try 
	{
		touch() ;	// Update access time
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't update access time for entry (" << __LINE__ << ")  Absolute Path: \"" << filename() << "\"  Got exception: " << e) ; 
	}

	updateStat() ;	// Keep statistics up-to-date
	
	ThreadSafe::Lock lock(_mutex) ;	// since counter is about to be modified
	_shared = shared ;
	_useCount ++ ;

	return _useCount ;
}


//
//  Cache::Entry::reserve
//
size_t Cache::Entry::reserve(const string& absPathOfCache, const off_t& bytes, bool shared)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Bytes to reserve: " << bytes << "  Filename: \"" << filename() << "\"  isLocked: " << boolalpha << isLocked() // PRQA S 3380
			<< "  lock type: " << (shared? "SHARED": "EXCLUSIVE")) ;

	ACS_COND_THROW(_reserveSpace, // PRQA S 3081
		Cache::CacheReserveException("Cannot Reserve: already reserved. Absolute Path: \"" + filename() + "\"" )) ;


	/* LOCK ENTRY: */
	Cache::LockType type ; // PRQA S 4101
	if (shared) {
		type = Cache::SH_GENERIC ;	/* SHARED - GENERIC */
    }
	else {
		type = Cache::EX_WRITE ;	/* EXCLUSIVE - WRITE */
    }

	size_t useCount = use(shared, type) ;

	/* If above lock successfull */
	ThreadSafe::Lock lock(_mutex) ;	// since shared variable is about to be modified

	if (bytes>0) {
		_reserveSpace = new Cache::ReserveSpace(absPathOfCache, bytes) ;
    }
	else {
		_reserveSpace = 0 ;
    }

	return useCount ;
}


//
//  Cache::Entry::reservedSpace
//
void Cache::Entry::reservedSpace(string& reserveFileAbsPath, off_t& bytes) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "isLocked: " << boolalpha << isLocked()) ;

	ThreadSafe::Lock lock(_mutex) ;	// since shared variable is about to be modified

	ACS_COND_THROW( !isLocked(),  // PRQA S 3081
		CacheLockException("File not locked. Absolute Path: \"" + filename() + "\"") ) ;

	ACS_COND_THROW(! _reserveSpace, // PRQA S 3081
		CacheReserveException("No space reserved. Absolute Path: \"" + filename() + "\"") ) ;

	bytes = _reserveSpace->bytes();
	reserveFileAbsPath = _reserveSpace->filename() ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Reserve file abs.path: \"" << reserveFileAbsPath << "\"  bytes: " << bytes) ;

	return ;
}


//
//  Cache::Entry::updateReservedSpace
//
void Cache::Entry::updateReservedSpace(const off_t& new_bytes) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "isLocked: " << boolalpha << isLocked() << "  new_bytes: " << new_bytes) ;

	ThreadSafe::Lock lock(_mutex) ;	// since shared variable is about to be modified

	ACS_COND_THROW( !isLocked(),  // PRQA S 3081
		CacheLockException("File not locked. Absolute Path: \"" + filename() + "\"") ) ;

	ACS_COND_THROW(! _reserveSpace, // PRQA S 3081
		CacheReserveException("No space reserved. Absolute Path: \"" + filename() + "\"") ) ;

	_reserveSpace->stillReserve(new_bytes) ;

	return ;
}


//
//  Cache::Entry::release
//
size_t Cache::Entry::release(bool force) // PRQA S 4020
{	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Filename: \"" << filename() << "\"  Force: " << boolalpha << force) ;

	bool toUnlock = force ;
	size_t value = 0 ; 

	if (!force)
	{
		updateStat() ;	// Keep statistics up-to-date

		if (!isLocked()) { return _useCount ; }

		{
			ThreadSafe::Lock lock(_mutex) ;	// since counter is about to be modified
			if (_useCount >= 0)
			{
				value = --_useCount ;	// Decrease counter and store in a temporary, constant variable
				toUnlock = (_useCount == 0) ; 
			}
			else
			{
				value =_useCount ;	
				toUnlock = false ;
			}
		}	// lock(_mutex)
	}	
	else {
		toUnlock = isLocked() ;
    }

	if (toUnlock) { 
		{
			ThreadSafe::Lock lock(_mutex) ;	// since shared variable is about to be modified
			if (_reserveSpace) { delete _reserveSpace ;	_reserveSpace=0; } 
		}	// lock(_mutex)
				
		if (!force)
		{
			try 
			{
				// Prepare the new payload for the lockfile :
				vector<Cache::LockOperDateStruct> empty_v_add; 
            	vector<Cache::LockOperDateStruct>  empty_v_remove; 	// Nothing to add/remove
				setLockfilePayload(Cache::UN_GENERIC, empty_v_add, empty_v_remove) ;	// Build new payload
			}
			catch(std::exception &e)
			{
				ACS_LOG_ERROR("Couldn't set lockfiletpe for entry (" << __LINE__ << ")  Absolute Path: \"" << filename() << "\"  Got exception: " << e) ; 
			}
		}
		
		unlock() ;	// UNLOCK ITEM, WITHOUT REMOVING THE .lock FILE

	}	// END if (toUnlock)

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  Filename: \"" << filename() << "\"  Returning: " << value) ;

	return value ;
}


//
//  Cache::Entry::touch
//
void Cache::Entry::touch() // PRQA S 4211
{	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Filename: \"" << filename() << "\"") ;
	
	File::touch(filename()) ;	// Update access time

	return ;
}


//
//  Cache::Entry::isErasable
//
/** Check payload and return TRUE if no active preventAutoErase record is found
*/
bool Cache::Entry::isErasable() const // PRQA S 4214
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  Filename: \"" << filename() << "\"") ;

	bool isErasable = false ;

	Cache::LockType t ; // PRQA S 4101
	vector<Cache::LockOperDateStruct> v ;
	
	getLockfilePayload(t, v) ; // Try to acquire LockOperDateStruct records
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "LockOperDateStruct size: " << v.size()) ;


	isErasable = true ; 
    size_t v_size = v.size() ;
    for (size_t i=0; i<v_size; ++i)
	{	
		Cache::LockOperDateStruct s = v[i] ;
		if (s.op == Cache::PREV_AUTOERASE)
		{
			time_t now=time(0) ;
            time_t expdate = time_t(s.expdate) ; // PRQA S 3081
			bool e = ( (s.expdate >= 0) && (now > s.expdate) ) ;
			if (!e) { isErasable = false ; }
			
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			    ptime n=from_time_t(now) ;
                ptime d=from_time_t(expdate) ; 
                string r = ((expdate<0)? "NEGATIVE": ptime2s(d)) ; // PRQA S 3384, 3385
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Found operation: " << hex << int(s.op) << dec << "  expdate: " << expdate << " (" << r // PRQA S 3081
				  << ")  now: " << now << " (" << ptime2s(n) << ")  isErasable: " << boolalpha << e) ;
			ACS_CLASS_END_DEBUG
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Skipped operation: " << hex << int(s.op)) ; // PRQA S 3081
		}
	}	// for


	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished.  Filename: \"" << filename() << "\"  Returning: " << boolalpha << isErasable) ;
	
	return isErasable ;
}



struct same_oper { // PRQA S 2171, 2173, 2175
    explicit same_oper(Cache::LockOper op) : op_(op) {}
    bool operator() (const Cache::LockOperDateStruct &i)  { return i.op == op_ ; } // PRQA S 4211
private:
    Cache::LockOper op_ ;
    same_oper(); // not implemented
} ;



struct match_remove_vector // PRQA S 2171, 2173, 2175
{
	explicit match_remove_vector(const std::vector<Cache::LockOperDateStruct> &v) : _v(v) {} ; // PRQA S 2528

	bool operator() (const Cache::LockOperDateStruct& s) { // PRQA S 4211
        return find_if(_v.begin(), _v.end(), same_oper(s.op)) != _v.end() ; // PRQA S 3081
    }
/* previous implementation
    {    
		for (std::vector<Cache::LockOperDateStruct>::const_iterator k=_v.begin() ; 
             k!=_v.end() ; 
             ++k) { 
            if ( k->op == s.op ) {
                return true ; 
            }
        }
		return false ; 
	}
*/
private:
	const std::vector<Cache::LockOperDateStruct> &_v ;
    match_remove_vector() ; // not implemented
} ;






//
//  Cache::Entry::setLockfilePayload()
//
void Cache::Entry::setLockfilePayload(Cache::LockType t, // PRQA S 4214
                                      const vector<Cache::LockOperDateStruct>& v_add, 
	                                  const vector<Cache::LockOperDateStruct>& v_remove, 
                                      bool reset) const
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o ; 
        o << "Method called.  Filename: \"" << filename() << " " ;
		if (!reset) { 
			o << "  v_add.size: " << v_add.size() << "  v_remove.size: " << v_remove.size() ;
			if (not v_add.empty()) { Cache::dumpLockOperDateStructVector(v_add) ; }
			if (not v_remove.empty()) { Cache::dumpLockOperDateStructVector(v_remove) ; }
		} else {
			o << "  reset: " << boolalpha << reset ; 
		}
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG


	unsigned char buffer[64*1024];  // PRQA S 4403 2 
    unsigned char buf_in[64*1024] ;
    ssize_t bufsize = 0 ;
	bool write_payload = false ;
    bool create_file = false ; 
	const int storeBigEnd = 1 ; 


	ssize_t retries = 3 ; 
    while (retries-- > 0)
	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Still " << (retries+1) << " retries available...") ;

		memset(buffer, 0, sizeof(buffer)) ;

		bufsize=0 ;

		vector<Cache::LockOperDateStruct> v_current ; 
		
		if (!reset)
		{
			// Obtain v_current and keep into account v_remove
		
			// Get v_current: 
			Cache::LockType t_ignore ; // PRQA S 4101
            getLockfilePayload(t_ignore, v_current) ; 
		
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Before merging:  Size of v_current: " 
					<< v_current.size() << "  v_remove: " << v_remove.size() << "  v_add: " << v_add.size()) ; 

			// Build tmp_remove: concatenating actual v_remove with v_add.
			// If the entries in v_add was not removed from the current vector, multiple entries would appear 
			vector<Cache::LockOperDateStruct> tmp_remove = v_remove ;  
			tmp_remove.insert( tmp_remove.end(), v_add.begin(), v_add.end() ) ;	// tmp_remove is the concatenation between v_remove and v_add ; 

			// Process v_current, removing entries in tmp_remove :
			// (In first step elements are just moved at the end of the vector) 
			vector<Cache::LockOperDateStruct>::iterator discarded = remove_if(v_current.begin(), v_current.end(), match_remove_vector(tmp_remove)) ; // PRQA S 3081
			v_current.erase(discarded, v_current.end()) ;

		}	// END: if (!reset)
		
		
		// Add to v_current all the structures contained into v_add 
		v_current.insert( v_current.end(), v_add.begin(), v_add.end() );

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "After merging:  Size of v_current: " << v_current.size()) ; 

		create_file = true ;	// Create lockfile (even if it doesn't exists) except if UNLOCKING and FILE DOESN'T EXIST.
		write_payload = not v_current.empty() ;	// Payload is written only if needed
		
		try
		{
			switch (t)
			{
			case Cache::UN_GENERIC :
                {
				    create_file = ( File::exists(filename()) || write_payload ) ;  
				    break ;
                }

			case Cache::EX_ERASE : // fallthrough
			case Cache::EX_WRITE :
                {
				    create_file = write_payload = true ;	// Always write payload
				    break ;
                }

			default:
                { break ; }
			}	/* switch (t) */
		
			if (create_file)
			{
				bufsize=0 ; 
                if (write_payload)
				{
					unsigned char c = static_cast<unsigned char>(lockTypeToInt(t) & int(0xff)) ; // PRQA S 3003, 3081
					memcpy(buffer, &c, sizeof(c)) ;	// Set locktype
					
					bufsize = sizeof(c) ; 
                    size_t v_c_size= v_current.size() ;
                    for (size_t i=0; i<v_c_size; ++i)
					{
						LockOperDateStruct oper = v_current[i] ;
						ACS_ByteSwap_SWAP(&oper.op, sizeof(oper.op), storeBigEnd) ; // PRQA S 3030, 3081, 3084, 3260, 4090 2
						ACS_ByteSwap_SWAP(&oper.expdate, sizeof(oper.expdate), storeBigEnd) ; 
						memcpy(buffer+bufsize, &oper, sizeof(oper)) ;	// Set lockoperdate
						bufsize += sizeof(oper)	; // PRQA S 3084
					}
		
				}	/* if (write_payload) */
	
				CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Writing payload: " << bufsize << " bytes") ;

				// Write file, and payload if needed
				bool success = false ;
				int fd = open (filename().c_str(), O_TRUNC|O_WRONLY) ;	// File length reset to zero 
				ssize_t w(-1) ;
				if (fd>=0)
				{
					FDGuard g(fd) ;	// Self close file

					if (bufsize>0)
					{
						w = write(fd, buffer, bufsize) ; // PRQA S 3000
						success = (w == bufsize) ;		
					}
					else {
						success = true ;
                    }

				}	/* if (fd>=0) */

				if (!success)
				{
					ostringstream  o ; o <<"Couldn't set payload for file: \"" + filename() + "\"  fd: " << fd << "  wrote: " << w << " / " << bufsize ;
					ACS_THROW( CacheLockPayloadException( o.str() ) ) ; // PRQA S 3081
				}

				// Verify file
				success = false ;
				fd = open (filename().c_str(), O_RDONLY) ;
				ssize_t r(-1) ;	
				if (fd>=0)
				{
					FDGuard g(fd) ;	// Self close file

					r = read(fd, buf_in, sizeof(buf_in)) ;
					success = (r == bufsize) ;	

					if (success && (bufsize>0) ) {
						success = ( memcmp( buffer, buf_in, bufsize) == 0 ) ; // PRQA S 3000
                    }

				}	/* if (fd>=0) */
				
				if (!success)
				{
					ostringstream  o ; o <<"Couldn't verify payload for file: \"" + filename() + "\"  fd: " << fd << "  read: " << r << " / " << bufsize ;
					ACS_THROW( CacheLockPayloadException( o.str() ) ) ; // PRQA S 3081
				}

			}	/* if (create_file) */
			
			break ; // quit loop on retries
		}
		catch(CacheLockPayloadException& e)
		{
			if (retries <= 0) {
				throw ;
            }
			else { /* still retries to do */
				usleep(1000*100) ;	// 0.1 s // PRQA S 4400
			}
		}
		catch(std::exception& e)
		{
			throw ;
		}
		
	}	// while (retries-- > 0) ...
	
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o ; o << "Method finished.  Filename: \"" << filename() << "\"" ;
			if (create_file && !write_payload) { o << "  file created" ; }
			if (create_file && write_payload) { o << "  wrote: " << bufsize ; }
			if (!create_file) { o << "  lockfile not found" ; }
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG
}


//
//  Cache::Entry::getLockfilePayload()
//
void Cache::Entry::getLockfilePayload(Cache::LockType& t, vector<Cache::LockOperDateStruct>& v) const
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Filename: \"" << filename() << "\"") ;

	unsigned char buffer[64*1024] ; // PRQA S 4403
	ssize_t bufsize=0 ;
	const int storeBigEnd = 1 ; 

	try
	{
		ACS_COND_THROW(! File::exists(filename()), CacheLockPayloadException("Lockfile not found: \"" + filename() + "\"")) ; // PRQA S 3081
	
		ssize_t retries = 3 ; 
        while (retries-- > 0)
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Still " << (retries+1) << " retries available...") ;

			v.clear() ; 

			memset(buffer, 0, sizeof(buffer)) ;

			bufsize=0 ; 
            ssize_t bufread=0 ;
			bool success = false ; 
            int fd = open (filename().c_str(), O_RDONLY) ;
			if (fd>=0)
			{
				FDGuard g(fd) ;	// Self close file
				ssize_t r = read(fd, buffer, sizeof(buffer)) ;
				if (r>=0) { success=true ; bufsize=r ; } 
			}

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Open with descriptor: " << fd << "  bufsize: " << bufsize) ;
			
			if (success)
			{
				if (bufsize>0)
				{
					const bool emitException = false ;
					t = intToLockType(int(buffer[0]), emitException) ; // PRQA S 3081
					bufread = 1 ;

					while ( (bufread+ssize_t(sizeof(LockOperDateStruct))) <= bufsize) // PRQA S 3081
					{
						LockOperDateStruct oper ; // PRQA S 4102
						memcpy(&oper, buffer+bufread, sizeof(LockOperDateStruct)) ;
						bufread += sizeof(LockOperDateStruct) ; // PRQA S 3084

						ACS_ByteSwap_SWAP(&oper.op, sizeof(oper.op), storeBigEnd) ; // PRQA S 3030, 3081, 3084, 3260, 4090 2
						ACS_ByteSwap_SWAP(&oper.expdate, sizeof(oper.expdate), storeBigEnd) ; 

						v.push_back(oper) ;
					} 
				}	// if (bufsize>0)
				else
				{
					v.clear() ; t = Cache::ER_ERROR ;	// Unable to get lock type
				}
				
				break ; // quit loop on retries
			}
			else 	
			{
				ACS_COND_THROW(0 == retries, CacheLockPayloadException("Retries finished")) ; // PRQA S 3081
				/* ELSE: still retries to do */
				usleep(1000*100) ;	// 0.1 s    // PRQA S 4400
				v.clear() ; t = Cache::ER_ERROR ;	// Unable to get lock type
			}
		}	// while (retries-- > 0) ...
	}
	catch(std::exception& e)
	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Filename: \"" << filename() << "\"  Unable to obtain locktype: " << e.what() ) ;

		v.clear() ; t = Cache::ER_ERROR ;	// Unable to get lock type
	}


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Filename: \"" << filename() << "\"  read buffer: " << bufsize << "  v.size: " << v.size());
		if (not v.empty()) { Cache::dumpLockOperDateStructVector(v) ; }
	ACS_CLASS_END_DEBUG
}


//
//  Cache::Entry::throwExceptionFromFile()
//
void Cache::Entry::throwExceptionFromFile(const string& msg_in) // PRQA S 4211
{	
	Cache::LockType l ; // PRQA S 4101
	vector <LockOperDateStruct> ignore ;
	
	getLockfilePayload(l, ignore) ;
		
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Filename: \"" << filename() 
			   	<< "\"  Lock Type: " << hex << Cache::lockTypeToInt(l) << dec << "  [" << lockTypeToString(l) << "]")  ;

	switch(l)
	{
    case EX_WRITE:
        {
			ACS_THROW( CacheExWriteLockException( msg_in ) ) ; // PRQA S 3081
			break ;
        }
    case EX_ERASE:
        {
			ACS_THROW( CacheExEraseLockException( msg_in ) ) ; // PRQA S 3081
			break ;
        }
    default:
        {
			ACS_THROW( CacheLockException( msg_in ) ) ; // PRQA S 3081
			break ;
        }
	}	// switch(l)

	return ;
}


////////////////////////////////////////////////////////////
//							
//     Cache
//							
////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(Cache) 

const string Cache::_lockFileExt = "lock" ;	// NO "." DOT
const string Cache::_lockFilePattern = "^.*" + ("\\." + (Cache::_lockFileExt)) + "$" ; 
const string Cache::_cacheUpdateFileNameRelPath = ".cacheupdated.ignore" ;	// No .lock extension. Relative to Cache_path
const string Cache::_cacheEraseInProgressFileNameRelPath = ".cache_erasing.ignore" ;	// No .lock extension. Relative to Cache_path
const string Cache::_reserveSpaceRelDir = ".reservedir" ;	// Relative to Cache_path
const string Cache::_reserveSpaceFileNamePattern = "^\\.r_.{17}\\.reserve$" ; // Relative to _reserveSpaceRelDir

//
//  Cache C'TOR
//
//////////////////////////////////////////////////////////////////////////////////////////
Cache::Cache(const string &path, off_t maxSize, double eraseQuotaPercentage, bool updateCacheEveryLock) : 
    ThreadSafe(),
	_path(StringUtils::pathNormalize(File::getAbsolutePath(path))), 
	_updateCacheEveryLock(updateCacheEveryLock),
    _entries(),
    _entriesMTime(),
    _lastUpdate(),
    _fullScanDone(false),
	_lowAccuracy(true),
	_lowAccuracyBytes(0),
	_eraseQuotaPercentage(eraseQuotaPercentage),
	_maxSize(maxSize),
	_tryLockTimeout(_tryLockDefaultTimeout),
	_updateListRemovedFiles(false),
	_userRemovedFiles_V(), 
	_quotaMinThresholdPercentage(0.0)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "path: \"" << _path << "\"  maxSize: " << maxSize << "  updateCacheEveryLock: " << boolalpha << _updateCacheEveryLock) ;
		
	memset(&_lastUpdate, 0, sizeof(_lastUpdate)) ;

	// Check if valid cache was provided :
	ACS_COND_THROW( (!(File::exists(_path) && File::isDir(_path) && File::isExecutable(_path) && File::isReadable(_path))),  // PRQA S 3081
		exIllegalValueException( "Invalid path: \"" + _path + "\"") ) ;
		
	// Check if valid erase quota was provided. Enforce protection against invalid value (typical, a range of 0-100) 
	if ((_eraseQuotaPercentage<-0.001) || (_eraseQuotaPercentage>1.001)) { // PRQA S 4400
		ostringstream m ; m << "Invalid eraseQuotaPcg: \"" << _eraseQuotaPercentage << "\"  Out of valid range: [ 0.00 - 1.00 ]" ;
		ACS_THROW( exIllegalValueException( m.str() ) ) ; // PRQA S 3081
	}
	
	if (_eraseQuotaPercentage < 0.0) { _eraseQuotaPercentage = 0.0 ; } // To amend little floating-point errors 
	if (_eraseQuotaPercentage > 1.0) { _eraseQuotaPercentage = 1.0 ; } // To amend little floating-point errors 

	// Check if cache root path is OK (o/w: exception). Then, if not existent, the reservationfiles dir is created.
	ReserveSpace::checkAndCreateReservationDir(_path) ;

	
	// WHEN HERE: Cache directory found. Reserve files container found (created from scratch if needed) and working
 	bool updated = false;

	if (_lowAccuracy == true) {
		setInitLowAccuracy() ;
	} else {
		updated = setInitHighAccuracy() ;
	}

	if (maxSize<0) { _maxSize = bytes() ; }	// Automatically set cache size

	const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ; // PRQA S 3000, 3084
	const off_t bytesInCache = bytes() ;
	const double perc = (_maxSize>0)? ( (bytesInCache * 100.0) / (_maxSize * 1.0) ) : 100.0 ; // PRQA S 3011, 3384

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Cache successfully instantiated on: \"" << _path << "\" Cache total size: " << _maxSize << " bytes  -  containing " 
			<< size() << " entries = " << bytesInCache << " bytes  (full: " << setprecision(3) << perc << "\045)   logical free space: " 
			<< (_maxSize-bytesInCache) << "  free space on filesystem: " << freeDisk << "  Irregular entries fixed: " 
			<< boolalpha << updated) ;


	// Check for available space (a warning might be issued)
	if ( (_maxSize-bytesInCache) > freeDisk) 
	{
		ostringstream o ; 
		ptime d(microsec_clock::universal_time()) ;
		o << "WARNING  [" << ptime2s(d) << "]   Possible cache misconfiguration:  Cache quota (total size): " << _maxSize << "  -  bytes in cache: " <<  bytesInCache << "  --  Logical free space: " 
			<< (_maxSize-bytesInCache) << " LOWER THAN free space on filesystem: " << freeDisk ;
		ACS_LOG_WARNING(o.str()) ; 
	}

}


//
//  Cache D'TOR
//
//////////////////////////////////////////////////////////////////////////////////////////
Cache::~Cache() throw () 
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "D'TOR CALLED. path: \"" << _path << "\"") ;

	Lock lock(*this) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		debugDumpCache() ;
	ACS_CLASS_END_DEBUG

	// Force release of locked entries
 	for( map<string, Entry >::iterator it = _entries.begin(); it != _entries.end(); ++it ) { // PRQA S 4238
		try {
			if (it->second.isLocked())
			{
				const bool force = true ;
				it->second.release(force) ;
			}
		}
		catch(std::exception &) {
            // nothing
		}
	}	// for
    // PRQA L:L1
}


//
//  Cache::setInitLowAccuracy
//
//////////////////////////////////////////////////////////////////////////////////////////
void Cache::setInitLowAccuracy()  
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called." ) ;

	_lowAccuracy = true ; 
	
	if (_fullScanDone == false) {
		evaluateLowAccuracyBytes() ;
	} else {
		/*
			DO NOTHING
			THIS MEANS THAT IF A FULL SCAN WAS ALREADY PERFORMED (E.G. THE CACHE WAS EARLIER IN HIGH ACCURACY MODE), 
			THE SAME SCAN WILL BE RETAINED.
		*/
	}

	const bool cacheModified = false ;
	const bool forceRescan = false ;
	updateCacheAndEntries(cacheModified, forceRescan, _lowAccuracy) ;	/*	Might update the CacheUpdateFile if needed.
																			This is only useful as IPC to notify the update to other cache processes */

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished." ) ;
	
	return ;
}


//
//  Cache::setInitHighAccuracy
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::setInitHighAccuracy()  
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called." ) ;

	_lowAccuracy = false ; 
	bool updated = false ; 
	
	// HIGH ACCURACY : Manage irregular entries (i.e. without a matching .lock file) 
 	try
 	{
 		// Manage irregular entries (i.e. without a matching .lock file) 
 		updated = manageCacheIrregularEntries() ;	/* Called only the very first time. Add .lock to all irregular entries. */
 	}
 	catch (std::exception& ex)
 	{
 		ACS_LOG_WARNING ("Managing of irregular entries failed. Check skipped. Exception follows:");
 		ACS_LOG_NOTIFY_EX (ex);
 	}

	const bool cacheModified = updated ;
	const bool forceRescan = true ;
	updateCacheAndEntries(cacheModified, forceRescan, _lowAccuracy) ;	// Create Entries list (if accuracy high)

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished." ) ;
	
	return updated ;
}


//
//  Cache::evaluateLowAccuracyBytes
//
//////////////////////////////////////////////////////////////////////////////////////////
void Cache::evaluateLowAccuracyBytes()  
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called." ) ;
	// Check if this method is called only when lowAccuracy set
	ACS_COND_THROW( (_lowAccuracy == false), exIllegalValueException( "INTERNAL ERROR: DO NOT CALL evaluateLowAccuracyBytes" ) ) ; // PRQA S 3081

	// Compute the ACTUAL value of the Cache occupation, in bytes
	_lowAccuracyBytes = (1048576.0 * File::getDirectoryOccupationInMB(_path)) ; // PRQA S 3011, 4400
	
	// The value, computed above, will be decreased by the current occupation of the cache (i.e. the known files)
	// in order to obtain the space occupied by all the files in cache, except the ones locked by this section (known files).
	//
	// The reason is that the occupation of the cache will be the sum of the space occupied by all the unknown files
	// plus the space occupied by the known files (see method bytes() when  _lowAccuracy = TRUE) 
	{
		Lock lock(*this) ;
		off_t tot = 0 ;

		/* Get the size from the content of memory map */
		for (map<string, Entry >::const_iterator i = _entries.begin(); i != _entries.end(); ++i) { // PRQA S 4238
			tot+= i->second.getBytes() ; // wrong qa warning off_t is 64 bit // PRQA S 3010
    	}
		
		_lowAccuracyBytes = ( (_lowAccuracyBytes>tot)? (_lowAccuracyBytes-tot) : 0) ; // PRQA S 3380
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. _lowAccuracyBytes evaluated: " << _lowAccuracyBytes ) ;
	
	return ;
}


//
//  Cache::manageCacheIrregularEntries
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::manageCacheIrregularEntries()  // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called." ) ;

	const bool isMainEntry = true ;
	bool updated = addLockExtensionToEntries( _path, isMainEntry ) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Cache updated: " << boolalpha << updated) ;
	
	return updated ;
}


//
//  (STATIC) Cache::addLockExtensionToEntries
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::addLockExtensionToEntries(const string& entry, bool isMainEntry) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  entry: \"" << entry << "\"  isMainEntry: " << boolalpha << isMainEntry) ;

	if (! File::exists(entry) )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Rejected (entry doesn't exist): \"" << entry << "\"") ;
	
		return false ;
	}	

	string entryRelPath = File::getFileName(entry) ;

	if (isMainEntry)
	{
		if ( entryRelPath == _cacheUpdateFileNameRelPath )
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Rejected (service file: CACHE UPDATE MARKER): \"" << entry << "\"") ;

			return false ;
		}	
		
		if ( entryRelPath == _cacheEraseInProgressFileNameRelPath )
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Rejected (service file: CACHE ERASE TOKEN): \"" << entry << "\"") ;

			return false ;
		}	
	}
	
	if (File::isDir(entry) && (entryRelPath == _reserveSpaceRelDir) )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Rejected (service entry: RESERVED SPACE CONTAINER): \"" << entry << "\"") ;

		return false ;
	}	

	{
		// Check if is lockfile (ignore it)
		RegExp regexp(_lockFilePattern) ;

		if ( regexp.match(entryRelPath) )
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Rejected (is a lock file): \"" << entryRelPath << "\"") ;

			return false ;	
		}	
	}
	

	// STILL HERE: ENTRY EXISTS AND IT ISN'T A LOCKFILE
	bool updated = false ;
	string entry_lock = File::addExt(entry, _lockFileExt) ;	// Forge .lock filename (absolute path) 
		
	if ( File::isRegularFile(entry) )	/* i.e. is not a directory */
	{
		if (! File::exists(entry_lock) )
		{
			// Found file without matching .lock entry. CREATE lockfile

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Created lockfile: \"" << entry_lock << "\" for file entry: \"" << entry_lock << "\"") ;
		
			File::touch(entry_lock) ;
			
			updated = true ;
		}
	}
	else if ( File::isDir(entry) )
	{
		if ( isMainEntry || !File::exists(entry_lock) )
		{
			// Directory file without matching .lock entry. RECURSIVELY ACCESS DIRECTORY
	
			Dir d(entry) ;
			for (size_t i=0; i<d.size(); i++)
			{
				string fullEntry = StringUtils::pathJoin(entry, d[i]) ;
				
				// If processing main path (i.e. isMainEntry==true AND found a file, the file still belongs to main path. Otherwise, false)
				bool newIsMainEntryFlag = (isMainEntry && !File::isDir(fullEntry))? true : false ;  // PRQA S 3384
				
				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Adding entry: \"" << fullEntry << "\"  Type: " << (File::isDir(fullEntry)?"DIR":"FILE")  // PRQA S 3380
						<< "  newIsMainEntryFlag: " << boolalpha << newIsMainEntryFlag) ;
				
				updated = (addLockExtensionToEntries( fullEntry, newIsMainEntryFlag ) || updated) ;			
			}
		}
		else if ( !isMainEntry && File::exists(entry_lock))
		{
			// Directory with .lock entry found: CHECK IF THERE ARE NESTED LOCKFILES AND REMOVE THEM
			updated = (removeNestedLocks(entry) || updated) ;
		}
        else { 
            // nothing 
        }
	}
    else {
        // nothing 
    }
				
	return updated ;
}


//
//  Cache::removeOrphanLockOlderThanDays()
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::removeOrphanLockOlderThanDays(const ssize_t& days) 
{
	return removeOrphanLockOlderThanDays("", days) ;
}


//
//  Cache::removeOrphanLockOlderThanDays()
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::removeOrphanLockOlderThanDays(const string& relPath, const ssize_t& days) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  Relative path: \"" << relPath << "\"") ;
	bool updated = false ; 
	
	const string absPath = StringUtils::pathJoin(_path, relPath) ;	

	if (! File::exists(absPath) )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Rejected (entry doesn't exist): \"" << absPath << "\"") ;
	
		return false ;
	}	


	if ( relPath == _cacheUpdateFileNameRelPath )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Rejected (service file: CACHE UPDATE MARKER): \"" << absPath << "\"") ;

		return false ;
	}	

	if ( relPath == _cacheEraseInProgressFileNameRelPath )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Rejected (service file: CACHE ERASE TOKEN): \"" << absPath << "\"") ;

		return false ;
	}	

	const bool isEntryDir( File::isDir(absPath) ) ;

	if (isEntryDir && (relPath == _reserveSpaceRelDir) )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Rejected (service entry: RESERVED SPACE CONTAINER): \"" << absPath << "\"") ;

		return false ;
	}	


	if ( isEntryDir )
	{
			Dir d(absPath) ;
			for (size_t i=0; i<d.size(); i++)
			{
				const string fullEntry ( StringUtils::pathJoin(absPath, d[i]) ) ;
				const string relEntry ( StringUtils::pathJoin(relPath, d[i]) ) ;
				if ( File::isDir(fullEntry) )
				{
					const string fullEntry_lock ( File::addExt(fullEntry, _lockFileExt) ) ;	// Forge .lock filename (absolute path) 
					if (!File::exists(fullEntry_lock))
					{
						// Found a not-lockable directory. It might contain other lockfiles. Recursively call this method
						updated = removeOrphanLockOlderThanDays( relEntry, days ) ;			
					} else {
						/* Do nothing (just found a directory that is lockable itself) */
					} 
				}
				else
				{
					// Found a file -- recursively call this method
					updated = removeOrphanLockOlderThanDays( relEntry, days ) ;			
				}
			}

	} else {

		// Processing a file (might be a datafile or a lockfile, either orphan or not orphan)

		RegExp regexp(_lockFilePattern) ;

		if (regexp.match(relPath)) 
		{
			// Found a lockfile
			// Create the filename associated to the lockfile (e.g. if lockfile is "ABC.lock", the filename would be "ABC")
			const string rel_entry_no_lock ( File::removeExt(relPath, _lockFileExt) ) ;
			const string abs_entry_no_lock ( StringUtils::pathJoin(_path, rel_entry_no_lock) ) ;	

			if (!File::exists(abs_entry_no_lock))
			{
				// Found an orphan lockfile -- Check the last modification date of the lockfile
				bool success = false ;
				time_t now = time(0) ;	// Unit is s ;
				time_t mtime = File::getModificationTime(absPath, &success );	
				if ( (success) && (mtime<=now) && ((now-mtime)>=(86400*days)) ) // PRQA S 4400
				{
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Removing orphan entry: \"" << abs_entry_no_lock << "\"  last access relative to now [s]: " << (now-mtime)
						<< "  threshold [s]: " << (86400*days) ) ;
					
					try {
						Cache::eraseEntry(rel_entry_no_lock) ;
						updated = true ; 
					} catch(std::exception &e) {
						ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Exception while trying to erase entry: \"" << abs_entry_no_lock << "\": " << e) ;
						updated = false ; 
					}	// catch 
	
				} else {
					// Orphan lockfile, but not enough old. DO NOTHING
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Skipping orphan entry: \"" << abs_entry_no_lock << "\"  last access relative to now [s]: " << (now-mtime)
						<< "  threshold [s]: " << (86400*days) ) ;
				}
			} else {
				// Not a orphan lockfile -- JUST IGNORE IT
			}
		
		} else {
			// Not a lockfile -- JUST IGNORE IT
		}

	}	// END: if ( isEntryDir )

	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished. Relative path: \"" << relPath << "\"") ;

	return updated ;
}


//
//  (STATIC) Cache::removeNestedLocks
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::removeNestedLocks(const string& path) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  path: \"" << path << "\"") ;

	if (!File::exists(path) || !File::isDir(path) || !File::isExecutable(path) || !File::isReadable(path) )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Invalid path (either doesn't exist or invalid permissions): \"" << path << "\"") ;
	
		return false ;
	}	

	bool updated = false ;
	DirTree d(path, true) ;
	d.regexp(_lockFilePattern) ;
	size_t dirsize = d.size() ;
	for (size_t i=0; i<dirsize; ++i)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Found invalid lockfile: \"" << d[i] << "\"") ;
	
		Cache::Entry temporaryLock(d[i], _tryLockDefaultTimeout) ;

		bool lockSuccessfully = false ;
		try {
			const bool shared = false ;	/* EXCLUSIVE LOCK */
			const Cache::LockType type = Cache::EX_ERASE ;	/* ERASE */
			temporaryLock.use(shared, type) ;	/* Try lock */
			lockSuccessfully = true ;	/* Success ! */
		} catch(std::exception &) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Invalid lockfile: \"" << d[i] << "\"  LOCKED BY ANOTHER PROCESS. COULDN'T REMOVE IT.") ;
			lockSuccessfully = false ;
		}

		if (lockSuccessfully)
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Removing lockfile: \"" << d[i] << "\"") ;

			temporaryLock.release() ;	/* Release lock */
			File::unlink(d[i]) ;
			
			updated = true ;
		}
	}


	return updated ;
}


//
//  (STATIC) Cache::cmpTimeval
//
//////////////////////////////////////////////////////////////////////////////////////////
/** cmpTimeval(a,b) returns (a-b)
*/
signed long int Cache::cmpTimeval(const struct timeval& a, const struct timeval& b) throw() // PRQA S 4020
{
	if (a.tv_sec != b.tv_sec) {
		return static_cast<signed long int>(a.tv_sec - b.tv_sec) ; // PRQA S 3081
    }
	else {
		return static_cast<signed long int>(a.tv_usec - b.tv_usec) ; // PRQA S 3081
    }
}


//
//  (STATIC) Cache::get_time_t
//
//////////////////////////////////////////////////////////////////////////////////////////
time_t Cache::get_time_t(const struct timeval& t) throw() 
{
	return t.tv_sec ;
}


//
//  (STATIC) Cache::getFileModificationTime
//
//////////////////////////////////////////////////////////////////////////////////////////
struct timeval Cache::getFileModificationTime (const std::string& path, bool* success) 
{
	struct timeval tv ; // PRQA S 4102
	memset(&tv, 0, sizeof(tv)) ;
	
	bool succ = false ;
	time_t t = File::getModificationTime(path, &succ) ;
	if (succ) { tv.tv_sec = t ; }
	if (success) { (*success) = succ ; } 

	return tv ;
}


//
//  (STATIC) Cache::dumpTimeval
//
//////////////////////////////////////////////////////////////////////////////////////////
string Cache::dumpTimeval(const struct timeval& a)
{
	char msg[4096] ; // PRQA S 4403
    sprintf(msg, "%ld.%06d", static_cast<signed long int>(a.tv_sec), static_cast<signed int>(a.tv_usec)) ; // PRQA S 3081
	return msg ;
}


//
//  Cache::debugDumpCache
//
//////////////////////////////////////////////////////////////////////////////////////////
void Cache::debugDumpCache() const
{
	Lock lock(*this) ;
    ostringstream o ; 
	o << "===========================================================[ Begin ]==\n" ;
	o << "\tEntries: " << _entries.size() << "   Cache Path: !" << _path << "!\n" ;

	size_t j = 0 ; 
    for (map<string, Entry>::const_iterator i=_entries.begin(); i!=_entries.end(); i++)
	{
		o << "\t" << j++ << "  key: \"" << i->first << "\"  " << i->second.debugString() << "\n" ;
	}
	o << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n" ;

	j = 0 ; 
    for (map<time_t, string>::const_iterator i=_entriesMTime.begin(); i!=_entriesMTime.end(); i++)
	{
		ptime tmp = from_time_t(i->first) ; string d = "(" + ptime2s(tmp) + ")" ;
		o << "\t" << j++ << "  key: " << i->first << "  " + d + "  \"" << i->second << "\"\n" ;
	}
	o << "===========================================================[ End ]==\n" ;
 
	ACS_LOG_DEBUG(SimpleDebugSignatureThread << o.str()) ;
}


//
//  Cache::debugDumpVector
//
//////////////////////////////////////////////////////////////////////////////////////////
string Cache::debugDumpVector(const vector<string> &v)
{
        string ret = "{ " ;
        for (size_t i = 0; i<v.size(); i++) { 
            ret += ( "\"" + v[i] + "\"" + " " ) ; 
        }
        ret += "}" ; 
        return ret ;
}


//
//  Cache::readEntries
//
//////////////////////////////////////////////////////////////////////////////////////////
void Cache::readEntries(const std::string &absPath) 
{		
	/** Inconditionally performs a full scan */

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Path: !" << absPath << "!") ;

	Lock lock(*this) ;	/* Do not move from here -- DirTree and Entries must be syncronized ! */

	const bool showHidden = true ;
	DirTree lockEntries(absPath, "", showHidden) ;

	lockEntries.relative("") ;	// MODIFIER: path is made relative (E.g. /raid0/cache/myfile.lock -> "myfile.lock")
	lockEntries.regexp(_lockFilePattern) ;	// MODIFIER: files must match the regular expression (that is, "must end with .lock")


	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "DirTree on: \"" << absPath << "\" successfully.  Found: " << lockEntries.size() << " entries") ;
	
	// Mark all current elements:
	for( map<string, Entry >::iterator it = _entries.begin(); it != _entries.end(); ++it ) {  // PRQA S 4238
        it->second.setMarker(true) ;
    }
	
    size_t le_size = lockEntries.size() ;
	// Scan DirTree and update (and also, unmark) all given elements:
	for (size_t j=0; j<le_size; ++j)
	{
		const string lockEntry = lockEntries[j] ;
		const bool forceRescan = false ;
		size_t useCount = addEntryToList(lockEntry, forceRescan) ;	

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			const size_t scanStep = max(size_t(1), lockEntries.size()/10)  ; // PRQA S 3081, 3084
			if (0 == (j%scanStep)) // PRQA S 3084
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Scanning: " << (j+1) << "/" << lockEntries.size()) ; // PRQA S 3084
			}
		ACS_CLASS_END_DEBUG
			
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "just added file: \"" << lockEntry << "\"  useCount: " << useCount) ;
	
	} // for j

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Scanned cache directory. Found: " << lockEntries.size() 
			<< "  _entries size: " << _entries.size() << " / _entriesMTime size: " << _entriesMTime.size()) ;

	// Remove marked (i.e. not updated) entries
	
	for ( map<string, Entry>::iterator j = _entries.begin();  ( (_entries.begin()!=_entries.end()) && (j!=_entries.end()) ); )  // PRQA S 4238
	{
	    map<string, Entry>::iterator i=j ;j++; // Prepare next loop

	   if (i->second.getMarker())	{
		   if (!i->second.isLocked())	{
			   string lockFileRelPath = i->first ;
			   time_t mtime = i->second.getMTime() ;
			   ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Removing marked lockfile: \"" << lockFileRelPath << "\"  with mtime: " << mtime) ;

			   _entries.erase(i) ;	// Remove from _entries
			   
			   removeFromEntriesMTime(_entriesMTime, mtime, lockFileRelPath) ;	// Remove from _entriesMTime
			   
		   } else {
			   i->second.setMarker(false) ; // Locked 
		   }
	   }	// if (i->second.getMarker())
	}	// for i,j...

	_fullScanDone = true ; 

 	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Scan finished.  _entries size: " << _entries.size() 
		  << " / _entriesMTime size: " << _entriesMTime.size()) ;

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		debugDumpCache() ;
	ACS_CLASS_END_DEBUG

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  Path: !" << absPath << "!") ;
}


//
//    Cache::updateCacheAndEntries
//
//////////////////////////////////////////////////////////////////////////////////////////
bool 														/* OUT: True if rescan execuded */
		Cache::updateCacheAndEntries(	bool cacheModified,	/* IN: True if cache was modified (lockfile added/removed) */ 
										bool rescanForced,	/* IN: True if a rescan is forced (e.g. when the cache is started) */
										bool lowAccuracy)	/* IN:	If set to lowaccuracy, the rescan is not actually performed, 
																	but the cache modification time is updated, allowing other process to 
																	realize that another cache is running */
{	
	/** 
		- If lowAccuracy SET, the method is almost ignored. The cacheUpdateFile might be updated, just to warn other Cache processes that another
		instance is running.
		- If lowAccuracy UNSET, the cacheUpdateFile might be updated accordingly to the cacheModified flag and the time of last execution of this method.
		A scan will be performed if needed. If rescanForced is SET, a scan will be performed anyway.
	*/

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  cacheModified: " << boolalpha << cacheModified << "  rescanForced: " << boolalpha << rescanForced) ;

	const string cacheUpdateFileName = StringUtils::pathJoin(_path, _cacheUpdateFileNameRelPath) ;
	string mtimeStr ;
	struct timeval mtimev; // PRQA S 4102 2 
    struct timeval curtime ;
	bool cacheUpdateFileRead = false ;
    bool rescanNeeded = false ;
	
	// DO NOT LOCK HERE ! The following time must be evaluated before acquiring the mutex !

	// curtime is evaluated BEFORE the mutex. If _lastUpdate GREATER than curtime, 
    // another thread already performed the update
	ACS_COND_THROW( 0 != gettimeofday(&curtime, 0), exIllegalValueException( "Error calling gettimeofday", errno) ) ; // PRQA S 3081

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Waiting for mutex acquiring...  curtime: " << dumpTimeval(curtime)) ;

	Lock lock(*this) ;	// MUTEX: Do not move from here!
	

	// DO NOT OPTIMIZE code below ("success" is evaluated in the first "IF")
	if (!cacheModified) {
		mtimev = getFileModificationTime(cacheUpdateFileName, &cacheUpdateFileRead );	// Get mtime from cacheUpdateFileName (last touch)
	}
    else {
		cacheUpdateFileRead = true ;
    }
	
	if (cacheModified || !cacheUpdateFileRead) {
		ACS_COND_THROW( 0 != gettimeofday(&mtimev, 0), exIllegalValueException( "Error calling gettimeofday", errno) ) ; // PRQA S 3081
    }
	
	if (!cacheModified && (cmpTimeval(mtimev, _lastUpdate)>0) ) {
        rescanNeeded=true; 
    }

	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
	{	ptime tmp = from_time_t(get_time_t(mtimev)); mtimeStr = ptime2s(tmp) ;	}	/* DO NOT MOVE FROM HERE */
	ACS_CLASS_END_DEBUG

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o ; o << "mutex acquired.  mtime: " 
			<< dumpTimeval(mtimev) << " (" << mtimeStr << ")  _lastUpdate: " << dumpTimeval(_lastUpdate) ;
		if (cacheModified) {
			o << "  Cache was modified  ***********************************************" ;
        }
		else if (rescanNeeded || rescanForced) {
			o << "  Cache wasn't modified but a rescan is needed/forced  ***********************************************" ;
        }
		else {
			o << "  Nothing to do  ***********************************************" ;
        }
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG
	
	bool executeReadEntries = (rescanNeeded || rescanForced) ;

	if (cacheModified || (!cacheUpdateFileRead))
	{
		if ( (cmpTimeval(_lastUpdate,curtime)<=0)	/* last updated preceding the time of this call */
				|| (!cacheUpdateFileRead))			/* OR:  unable to read cacheUpdateFileName */
		{
			executeReadEntries = true ;		/* Force read entries */	

			File::touch(cacheUpdateFileName) ;
			ACS_COND_THROW( 0 != gettimeofday(&mtimev, 0), exIllegalValueException( "Error calling gettimeofday", errno) ) ; // PRQA S 3081

			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			{	ptime tmp = from_time_t(get_time_t(mtimev)); mtimeStr = ptime2s(tmp) ;	}	/* DO NOT MOVE FROM HERE */
			ACS_CLASS_END_DEBUG

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "cacheUpdateFile: " << cacheUpdateFileName << " updated to: " << dumpTimeval(mtimev) 
				  << " (" << mtimeStr << ")") ;
		}
		else if(!executeReadEntries)	/* last update FOLLOWING the time of this call, and not forced */
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "method called at: " <<  dumpTimeval(curtime) << "  _lastUpdate: " << dumpTimeval(_lastUpdate)
					<< "  Update not needed") ;
		}
        else {
            // nothing
        }
		
	}	/* if (cacheModified) */ 

	
	if (executeReadEntries)
	{
		if (lowAccuracy == false) {
			readEntries(_path) ; /* cacheUpdate file newer than last check: rescan entries */
		}
		_lastUpdate = mtimev ;	// Since the cacheUpdateFileName was updated even if entries not read
		
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_lastUpdate updated to: " << dumpTimeval(mtimev) << "(" << mtimeStr << ")") ; // PRQA S 3380, 3385
	}	
	
	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "touch executed? " << (cacheModified? "T ("+mtimeStr+")":"F") << "  readEntries executed? " << boolalpha << executeReadEntries) ; // PRQA S 3380, 3385

	return executeReadEntries ;
}


//
//    Cache::addEntryToList
//
//////////////////////////////////////////////////////////////////////////////////////////
/**  Method used both if a new file has been added/released/locked, etc. (use with forceUpdate=true) or in case of routinely rescan
*/
size_t Cache::addEntryToList(	const std::string &lockFileRelPath,	// lockFile is relative to cache. So, /cache/myfile.lock should be "myfile.lock"
								bool forceUpdate)					// TRUE to force update
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "adding file to map: \"" << lockFileRelPath << "\"") ;

	bool success = false ;
    bool toAdd = false ; 
    time_t lockMTime = 0;
	const string lockFileAbsPath = StringUtils::pathJoin(_path, lockFileRelPath) ;
	size_t ct = 0 ;

	// DO NOT OPTIMIZE code below ("success" is evaluated in the first "IF")
	if (!forceUpdate) {
		lockMTime = File::getModificationTime(lockFileAbsPath, &success );	// Get mtime from lockfile (last touch)
    }
	
	if (forceUpdate || !success) { 
        lockMTime = time(0); 
    }

	{
		Lock lock(*this) ;

		map<string, Entry >::iterator it = _entries.find(lockFileRelPath) ;
		if (it == _entries.end()) {

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "File: \"" << lockFileRelPath << "\"  not found.") ;

			toAdd = true ;	
			Entry e (lockFileAbsPath, _tryLockTimeout, lockMTime) ;	// Also, update Statistics
		
			_entries.insert(map<std::string, Entry>::value_type(lockFileRelPath, e) ) ;

			_entriesMTime.insert( pair<time_t, string>(lockMTime, lockFileRelPath ) );	// Insert in _entriesMTime

		} else {
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "File: \"" << lockFileRelPath << "\"  was found.") ;

			toAdd = false ;	

			if ( forceUpdate || ((it->second).getMTime() < lockMTime) )
			{
				CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Rescanning: \"" << lockFileRelPath << "\"") ;

				time_t previousMTime = (it->second).getMTime() ;

				(it->second).updateStat(lockMTime) ;	// Keep entry up-to-date

				removeFromEntriesMTime(_entriesMTime, previousMTime, lockFileRelPath) ;	// Remove previous entry from _entriesMTime ;
				
				_entriesMTime.insert( pair<time_t, string>(lockMTime, lockFileRelPath ) );	// Insert in _entriesMTime
			
			}

			(it->second).setMarker(false) ;
		}

		ct = _entries[lockFileRelPath].useCount() ;

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, (toAdd? "added": "updated") << " file: \"" << lockFileRelPath << "\"  Returning: " << ct ) ; // PRQA S 3380
	
	} // Lock

	return ct ;
}


//
//  Cache::isLockFile
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Cache::isLockFile(const std::string &s) const
{
	RegExp regexp(_lockFilePattern) ;

	return ( (0 == File::size(s)) && regexp.match(s) ) ;
}


//
//    Cache::bytes
//
//////////////////////////////////////////////////////////////////////////////////////////
/** returns the size actually used by all files [bytes]: locked, not locked, reservations
*/
off_t Cache::bytes() const 
{
	Lock lock(*this) ;
	off_t tot = 0 ;
	
	/* Get the size from the content of memory map */
	for (map<string, Entry >::const_iterator i = _entries.begin(); i != _entries.end(); ++i) {
		tot+= i->second.getBytes() ; // wrong qa warning off_t is 64 bit // PRQA S 3010
    }
	
	if (_lowAccuracy && !_fullScanDone) {
		/*	If _fullScan never done, the map above is just the memory image of the files locked by this process.
			So, the physical occupation of the cache is added */
		tot += _lowAccuracyBytes ;
	}
	
	off_t reservedBytes = getReservedBytesAndCleanOldFiles() ;
	
	return (tot + reservedBytes) ;
}


//
//    Cache::bytesUsed
//
//////////////////////////////////////////////////////////////////////////////////////////
/** returns the size [bytes] used only by file actually being locked 
	(so, excluding unlocked and reservations)
*/
off_t Cache::bytesUsed() const
{
	Lock lock(*this) ;
	map<string, Entry >::const_iterator i ;
	off_t tot = 0 ;
	for (i = _entries.begin(); i != _entries.end(); ++i) {
		if (i->second.isLocked()) { tot+= i->second.getBytes() ; }  // wrong qa warning off_t is 64 bit // PRQA S 3010
    }
	
	return tot ;
}


//
//    Cache::add
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path without locking it
*/
size_t Cache::add(const std::string &relPath) 
{
	const bool lockFile = false ;
	return use(relPath, lockFile) ;
}

//
//    Cache::reserveAndUse
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path, locks it (EXCLUSIVE LOCK) and reserve space
*/
size_t 						
		Cache::reserveAndUse(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
								off_t bytes, 				/* Bytes to reserve (>=0) */
								bool shared)				/* TRUE for shared lock. FALSE for exclusive lock */
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\"  bytes: " << bytes) ;

	const bool lk = true ;
	return internalUse(relPath, bytes, lk, shared) ;
}


//
//    Cache::useIfFileExists
//
//////////////////////////////////////////////////////////////////////////////////////////
/** if (payload) file exists, lock it and return TRUE. Otherwise return FALSE 
*/
bool 						
		Cache::useIfFileExists(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
								size_t& count)
{

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\"") ;
	
	RegExp regexp(_lockFilePattern) ;
	count = 0 ;
	
	// Quick pre-check, filename should not be empty or a lock file
	if ( relPath=="" || relPath=="." || relPath==".." || relPath=="/" )
	{	ACS_THROW(CacheInvalidFilenameException("Empty or invalid filename: \"" + relPath + "\"") );	} // PRQA S 3081
	else if ( regexp.match(relPath) )
	{	ACS_THROW(CacheInvalidFilenameException("Invalid filename (LOCK EXTENSION NOT ALLOWED) \"" + relPath + "\"") );	} // PRQA S 3081
    else { /* nothing */ }
    
	string absPath = StringUtils::pathJoin(_path, relPath) ;	// absolute path (used for the touch command)

	bool exists = File::exists(absPath) ;
	

	if (exists)
	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "inputpath (relative): \"" << relPath << "\"  About to lock file...") ;

		const bool lk = true ;
		off_t bytes = -1 ;	// Don't reserve space
		count = internalUse(relPath, bytes, lk) ;

		// IF HERE: Shared lock was successfully (no exclusive lock on file).
	
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "inputpath (relative): \"" << relPath << "\"  Lock was successful") ;

		// Check again if file still exists (it might be removed before lock above)
		exists = File::exists(absPath) ;

		if (!exists) 
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "inputpath (relative): \"" << relPath << "\"  File disappeared. Removing lock") ;

			release(relPath) ;
		}
	}

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. inputpath (relative): \"" << relPath << "\"\tfile exists? " << boolalpha << exists)  ;

	return exists ;
}


//
//    Cache::use
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
size_t 						
		Cache::use(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
					bool lock) /* True to lock */
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\"  lock? " << boolalpha << lock) ;

	const off_t bytes = -1 ;	/* Don't reserve space */
	return internalUse(relPath, bytes, lock) ;
}


//
//    Cache::makeSpaceReserveAndUse
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
off_t 						
		Cache::makeSpaceReserveAndUse(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
										off_t size,				/* Requested space (bytes) */
										const string &relReservedFileName)	/* Optional INPUT reserve file, as rel.path (might also not exist). It will be ERASED if method successfull */
{
	const bool shared = false ;
	vector <string> ignored_list ; // List is not used
	return makeSpaceReserveAndUse(relPath, size, ignored_list, relReservedFileName, shared) ;
}

//
//    Cache::makeSpaceReserveAndUse
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
off_t 						
		Cache::makeSpaceReserveAndUse(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
										off_t size,				/* Requested space (bytes) */
										vector <string>& filenamesRemoved,
										const string &relReservedFileName)	/* Optional INPUT reserve file, as rel.path (might also not exist). It will be ERASED if method successfull */
{
	const bool shared = false ;
	return makeSpaceReserveAndUse(relPath, size, filenamesRemoved, relReservedFileName, shared) ;
}

//
//    Cache::makeSpaceReserveAndUse
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
off_t 						
		Cache::makeSpaceReserveAndUse(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
										off_t size,				/* Requested space (bytes) */
										const string &relReservedFileName,	/* Optional INPUT reserve file, as rel.path (might also not exist). It will be ERASED if method successfull */
										bool shared)					/* TRUE for shared lock. FALSE for exclusive lock */
{
	vector <string> ignored_list ; // List is not used
	return makeSpaceReserveAndUse(relPath, size, ignored_list, relReservedFileName, shared) ;
}

//
//    Cache::makeSpaceReserveAndUse
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
off_t 						
		Cache::makeSpaceReserveAndUse(	const string &relPath,			/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
										off_t size,					/* Requested space (bytes) */
										vector <string>& filenamesRemoved,
										const string &relReservedFileName,	/* INPUT reserve file (optional, it might be ""), as rel.path (might also not exist). It will be ERASED if method successfull */
										bool shared)					/* TRUE for shared lock. FALSE for exclusive lock */
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o	; o << "Method called. inputpath (relative): \"" << relPath << "\"\trequested bytes: " << size 
			<< "  Lock: " << (shared? "SHARED" : "EXCLUSIVE") ; // PRQA S 3380
		if (!relReservedFileName.empty()) { o << " relReservedFileName: \"" << relReservedFileName << "\"" ; } 
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG

	{
		// Run a scan if needed
		const bool cacheModified = false ;
        const bool forceRescanEntries = false ;
		updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	
	}

	/*
		Step #1: Reserve bytes+20%
		Step #2: Check if at least 1 one byte is free (so at least "bytes" are available, since the reserved space is virtual!)
		Step #3: Acquire an exclusive lock for the file and reserve, for a very short time, another bytes+20% -long file
		Step #4: Release the space in Step #1
	*/

	const off_t reserved = (size + (size/5)) ;	// Increased by 20%
	const bool useRelFileName = true ;
	off_t to_reserve = reserved ;
	off_t freed = 0;

	if (!relReservedFileName.empty())
	{
		bool res_bytes_success=false; 
        off_t res_bytes=0; 
		bool bexpdateTD = false ; 
        time_t expdateTD = 0 ;	// Not used -- TO DISCARD

		// Read Reservation file (if possible)
		ReserveSpace::readReservation( _path, relReservedFileName, useRelFileName, res_bytes, res_bytes_success, expdateTD, bexpdateTD) ;

		if (res_bytes_success && (res_bytes>0))
		{
			/* Compute new value for bytes to be still reserved, net of the bytes reserved bt the provided file */
			to_reserve = ( (res_bytes >= reserved)? 0 : (reserved-res_bytes) ) ; // PRQA S 3380
		}
	
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Read file: \"" << relReservedFileName << "\"  success: " << boolalpha << res_bytes_success << "  res_bytes: " << res_bytes
				<< "  bytes to be initially reserved: " << reserved << "  remainder: " << to_reserve) ;
	}

	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Creating a temporary reserve file to be added to the (optional) input persistent reserve file."
				<< "\tSize to reserve (the remainder): " 
				<< to_reserve << " bytes") ; 

		ReserveSpace temporaryReservedSpace(_path, to_reserve) ;	/* Reserve space. It will be released when (this) scope is lost */

		// Since the space has been ALREADY RESERVED, just one (1) bytes (that is "at least 0") should be available as free space.
		const off_t wished = 1 /*byte*/ ;
		off_t available = 0;
		freed = emptySpace(wished, available, filenamesRemoved) ;	// available is the returned value: THE FREE SPACE AVAILABLE AFTER THE CLEANUP. S
												// If few space was available, since the reservations are subtracted from the ph. space, IT CAN BE NEGATIVE
												// (E.G. If all the files was locked, 0 bytes are free. But available could be <0 due to reservations)

		const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ;	/* Physical space */ // PRQA S 3000, 3084

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Requested: " << size << "  freed: " << freed 
			  << "  Available: " << available << "  Reserved: " << reserved 
			  << "\tAvailable+reserved: " << (available + reserved) << "  Free space on device: " << freeDisk) ;

		/* Check if there is enought phyisical space (freeDisk) to store the logical file */
		ACS_COND_THROW( freeDisk < reserved, CacheFullException("Device Full. Cannot store file: " + relPath) ); // PRQA S 3081

		/* Check if there is enought logical space in cache (available) to store the logical file */
		ACS_COND_THROW( ((available+reserved) <= size), CacheFullException("Cache Full. Cannot store file: " + relPath) ); // PRQA S 3081

		// IF HERE: At least "bytes" available both in Cache (actually:  available+reserved ) and on filesystem 

		/* In order to lock the file, the method reserveAndUse is called.
			This method will create ANOTHER reserved space for a very short time, until it returns.
			Then the temporaryReservedSpace above will be freed and the only reserved space will be the one created by 
			reserveAndUse method (freed by release) 
		*/

		reserveAndUse( relPath, size, shared ) ;	/* Get exclusive lock (within temporaryReservedSpace scope) */

		// If method successfull, remove the input reserve file
		if (!relReservedFileName.empty())
		{
			int status_fordebug = ReserveSpace::deleteReservation( _path, relReservedFileName, useRelFileName ) ;	// Ignore errors !!

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Reserve file: \"" 
					<< relReservedFileName << "\"  removed with status: " << status_fordebug) ;
		}
	
	}	// temporaryReservedSpace out of scope

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Freed: " << freed) ;
	
	return freed ;
}


//
//    Cache::makeSpaceAndReserve
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Create space, and return a reservation file (rel.path). The file, persistent, will expire on a specific date
	If not enough free space is available a CacheFullException will be released.
*/
string 						
		Cache::makeSpaceAndReserve(	off_t bytes,						/* Requested space */
									time_t expdate)						/* Expiration date */
{
	vector <string> ignored_list ; // List is not used
	return makeSpaceAndReserve(bytes, expdate, ignored_list) ; 
}


//
//    Cache::makeSpaceAndReserve
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Create space, and return a reservation file (rel.path). The file, persistent, will expire on a specific date
	If not enough free space is available a CacheFullException will be released.
*/
string 						
		Cache::makeSpaceAndReserve(	off_t bytes,						/* Requested space */
									time_t expdate,						/* Expiration date */
									vector <string>& filenamesRemoved)	/* List of files removed */
{
	time_t now = time(0) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ptime d = from_time_t(expdate) ; 
        ptime n = from_time_t(now) ;
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. requested bytes: " 
			<< bytes << "  now: " << now << "[" << ptime2s(n) << "]  expiration date: " << expdate << "[" << ptime2s(d) << "]") ;
	ACS_CLASS_END_DEBUG

	{
		// Run a scan if needed
		const bool cacheModified = false ; 
        const bool forceRescanEntries = false ;
		updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	
	}

	/*
		Step #1: Reserve bytes+20%
		Step #2: Check if at least 1 one byte is free (so at least "bytes" are available, since the reserved space is virtual!)
		Step #3: Create a persistent reservation file, thus reserving, for a very short time, another bytes+20% -long file
		Step #4: Release the space in Step #1
		Step #5: Return filename in Step #3
	*/

	string relFileName = "" ;
	const off_t reserved = (bytes + (bytes/5)) ;	// Increased by 20%
	off_t freed = 0 ;

	{
		ReserveSpace temporaryReservedSpace(_path, reserved) ;	/* Reserve space. It will be released when (this) scope is lost */

		// Since the space has been ALREADY RESERVED, just one (1) bytes (that is "at least 0") should be available as free space.
		const off_t wished = 1 /*byte*/ ;
		off_t available = 0;
		freed = emptySpace(wished, available, filenamesRemoved) ;

		const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ;	/* Physical space */ // PRQA S 3000, 3084

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Requested: " << bytes << "  freed: " << freed 
			<< "  Available: " << (available + reserved) << "  Free space on device: " << freeDisk) ;

		/* Check if there is enought phyisical space (freeDisk) to store the logical file */
		ACS_COND_THROW( freeDisk < reserved, CacheFullException("Device Full. Cannot reserve space") ); // PRQA S 3081

		/* Check if there is enought logical space in cache (available) to store the logical file */
		ACS_COND_THROW( ((available+reserved) <= bytes), CacheFullException("Cache Full. Cannot reserve space") ); // PRQA S 3081

		// IF HERE: At least "bytes" available both in Cache (actually:  available+reserved ) and on filesystem 

		/* In order to prevent other processes to use the (temporary) reserved space, a persistent reservation file 
				is  created.
			Then the temporaryReservedSpace above will be freed and the only reserved space will be the persistent one. 
		*/


		const bool useRelFileName = true ;
		relFileName = ReserveSpace::createReservation(_path, useRelFileName, bytes, expdate) ;	// Create a persistent reservation
	
	}	// temporaryReservedSpace out of scope

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  Freed: " << freed << "  (relative) ReservationFile: \"" << relFileName << "\"") ;
	
	return relFileName ;
}


//
//    Cache::updateReservedSpace
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Update reserved space for a file locked by THIS process. The free space will be ALWAYS checked (and freed to ensure requested space)
	If not enough free space is available a CacheFullException will be released. 
	Return the freed space (unrelated with the wished space).
*/
off_t 						
		Cache::updateReservedSpace(	const std::string &relPath,			/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
									off_t bytes)						/* New requested space */
{
	vector <string> ignored_list ; // List is not used
	return updateReservedSpace(relPath, bytes, ignored_list) ; 
}


//
//    Cache::updateReservedSpace
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Update reserved space for a file locked by THIS process. The free space will be ALWAYS checked (and freed to ensure requested space)
	If not enough free space is available a CacheFullException will be released. 
	Return the freed space (unrelated with the wished space).
*/
off_t 						
		Cache::updateReservedSpace(	const std::string &relPath,			/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
									off_t bytes,						/* New requested space */
									vector <string>& filenamesRemoved)	/* List of files removed */
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\"  new value: " << bytes) ;

	Lock lock(*this) ;

	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;

	ACS_COND_THROW(!foundInList, CacheInvalidFilenameException("File not currently locked: \"" + relPath + "\"") );	 // PRQA S 3081

	string reserveFileAbsPath ;
	off_t old_bytes = 0;
    off_t freed = 0 ;

	_entries[lockFileRelPath].reservedSpace(reserveFileAbsPath, old_bytes) ;	// Exception if space not actually reserved

	_entries[lockFileRelPath].touch() ;	// Since lockfile is being accessed

	{
		// Run a scan if needed
		const bool cacheModified = true ;		/* Since size of the file is likely to be changed */
		const bool forceRescanEntries = false ;
		updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	
	}
	
	try
	{
		if (bytes>=0)
		{
			off_t reserved = (bytes + (bytes/5)) ;	// Increased by 20% -- It might be zero // PRQA S 4400
			ReserveSpace temporaryReservedSpace(_path, reserved) ;	/* Reserve space. It will be released when (this) scope is lost */

			_entries[lockFileRelPath].updateReservedSpace(0) ;	// Turn-off actual space reservation
			

			// Since the space has been ALREADY RESERVED, just one (1) bytes (that is "at least 0") should be available as free space.
			const off_t wished = 1 /*byte*/ ;
			off_t available = 0;	
			freed = emptySpace(wished, available, filenamesRemoved) ;

			const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ;	/* Physical space */ // PRQA S 3000, 3084

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Requested: " << bytes << "  freed: " << freed 
				  << "  Available: " <<	(available + reserved) << "  Free space on device: " << freeDisk) ;

			/* Check if there is enought phyisical space (freeDisk) to store the logical file */
			ACS_COND_THROW( freeDisk < reserved, CacheFullException("Device Full. Cannot store file: " + relPath) ); // PRQA S 3081

			/* Check if there is enought logical space in cache (available) to store the logical file */
			ACS_COND_THROW( ((available+reserved) <= bytes), CacheFullException("Cache Full. Cannot store file: " + relPath) ); // PRQA S 3081

			// IF HERE: successfull

			_entries[lockFileRelPath].updateReservedSpace(bytes) ;	// Turn-on actual space reservation
		}
		else {
			freed = 0 ;	// Nothing to do !
        }
	}
	catch(CacheFullException& e)
	{
		_entries[lockFileRelPath].updateReservedSpace(old_bytes) ;	// Undo Operation
		
		throw ;
	}
	catch(std::exception &e) 
	{
		throw ;
	}

	
	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  freed: " << freed ) ;

	return freed ;
}


//
//    Cache::preventAutoErase
//
//////////////////////////////////////////////////////////////////////////////////////////
/** 
	Set value: prevent_date for a file locked by THIS process. After the release, the file cannot be auto-erase till prevent_date. 
	Anyway if the file is used BEFORE prevent_date, this information will be removed.
	Return the count of shared users (unrelated with the wished space).
*/
off_t 						
		Cache::preventAutoErase(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
									time_t expdate)				/* Prevent AutoErase till this date */
{
	time_t now = time(0) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ptime d = from_time_t(expdate) ;
        ptime n = from_time_t(now) ;
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\""
		  << "  now: " << now << "[" << ptime2s(n) << "]  expiration date: " << expdate << " [" << ptime2s(d) << "]") ;
	ACS_CLASS_END_DEBUG


	Lock lock(*this) ;

	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;
	ACS_COND_THROW( !foundInList,  // PRQA S 3081
		CacheInvalidFilenameException("File unknown: \"" + relPath + "\"") );	
	ACS_COND_THROW( !_entries[lockFileRelPath].isLocked(),  // PRQA S 3081
		CacheInvalidFilenameException("File not locked: \"" + relPath + "\"") );	

	try
	{
		// Prepare the new payload for the lockfile :
		const Cache::LockType t = ( _entries[lockFileRelPath].getShared()? Cache::SH_GENERIC : Cache::EX_WRITE ) ; // PRQA S 3384

		vector<Cache::LockOperDateStruct> empty_v_remove ; 	// Nothing to remove
		vector<Cache::LockOperDateStruct> v_add ;
		Cache::LockOperDateStruct oper ; // PRQA S 4102
		memset(&oper, 0, sizeof(oper)) ;
		oper.op = Cache::PREV_AUTOERASE ;					// Add to current payload: PREV_AUTOERASE
		oper.expdate = static_cast<signed long long int>(expdate) ; // PRQA S 3081
		v_add.push_back(oper) ;

		_entries[lockFileRelPath].setLockfilePayload(t, v_add, empty_v_remove) ;	// Build new payload
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't run preventAutoErase for entry (" << __LINE__ << ")  Absolute Path: \"" << _entries[lockFileRelPath].filename() << "\"  Got exception: " << e) ; 
	}

	size_t c = _entries[lockFileRelPath].useCount() ;

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << c ) ;

	return c ; // PRQA S 3000
}


//
//    Cache::setAutoErase
//
//////////////////////////////////////////////////////////////////////////////////////////
/** 
	Reset value: prevent_date for a file locked by THIS process. After the release, the file might be auto-erased
	Return the count of shared users (unrelated with the wished space).
*/
off_t 						
		Cache::setAutoErase(	const std::string &relPath)	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\"") ;

	Lock lock(*this) ;

	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;
	ACS_COND_THROW( !foundInList,  // PRQA S 3081
		CacheInvalidFilenameException("File unknown: \"" + relPath + "\"") );	
	ACS_COND_THROW( !_entries[lockFileRelPath].isLocked(),  // PRQA S 3081
		CacheInvalidFilenameException("File not locked: \"" + relPath + "\"") );	
	
	try {
		// Prepare the new payload for the lockfile :
		const Cache::LockType t = ( _entries[lockFileRelPath].getShared()? Cache::SH_GENERIC : Cache::EX_WRITE ) ; // PRQA S 3384

		vector<Cache::LockOperDateStruct> empty_v_add ; 	// Nothing to add
		vector<Cache::LockOperDateStruct> v_remove ; 

		Cache::LockOperDateStruct oper ; // PRQA S 4102
		memset(&oper, 0, sizeof(oper)) ;
		oper.op = Cache::PREV_AUTOERASE ;					// Remove from current payload (if present): PREV_AUTOERASE
		v_remove.push_back(oper) ;

		_entries[lockFileRelPath].setLockfilePayload(t, empty_v_add, v_remove) ;	// Build new payload
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't run setAutoErase for entry (" << __LINE__ << ")  Absolute Path: \"" << _entries[lockFileRelPath].filename() << "\"  Got exception: " << e) ; 
	}

	size_t c = _entries[lockFileRelPath].useCount() ;

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << c) ;

	return c ; // PRQA S 3000
}


//
//    Cache::setDeleteASAP_Flag
//
//////////////////////////////////////////////////////////////////////////////////////////
/** 
	Set value in lockfile payload: DELETE_ENTRY_ASAP for a given entry. Entry must be already locked
	Return the count of shared users.
*/
off_t 						
		Cache::setDeleteASAP_Flag(	const std::string &relPath)
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called. inputpath (relative): \"" << relPath );

	Lock lock(*this) ;

	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;
	ACS_COND_THROW( !foundInList,  // PRQA S 3081
		CacheInvalidFilenameException("setDeleteASAP_Flag: File unknown: \"" + relPath + "\"") );	
	ACS_COND_THROW( !_entries[lockFileRelPath].isLocked(),  // PRQA S 3081
		CacheInvalidFilenameException("setDeleteASAP_Flag: File not locked: \"" + relPath + "\"") ); 

	try 
	{
		// Prepare the new payload for the lockfile :
		const Cache::LockType t = ( _entries[lockFileRelPath].getShared()? Cache::SH_GENERIC : Cache::EX_WRITE ) ; // PRQA S 3384

		vector<Cache::LockOperDateStruct> empty_v_remove ; 	// Nothing to remove
		vector<Cache::LockOperDateStruct> v_add ;
		Cache::LockOperDateStruct oper ; // PRQA S 4102
		memset(&oper, 0, sizeof(oper)) ;
		oper.op = Cache::DELETE_ENTRY_ASAP ;				// Add to current payload: DELETE_ENTRY_ASAP
		v_add.push_back(oper) ;

		_entries[lockFileRelPath].setLockfilePayload(t, v_add, empty_v_remove) ;	// Build new payload
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't run setDeleteASAP_Flag for entry (" << __LINE__ << ")  Absolute Path: \"" << _entries[lockFileRelPath].filename() << "\"  Got exception: " << e) ; 
	}

	size_t c = _entries[lockFileRelPath].useCount() ;

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished. Returning: \"" << c) ;

	return c ; // PRQA S 3000
}


//
//    Cache::clearDeleteASAP_Flag
//
//////////////////////////////////////////////////////////////////////////////////////////
/** 
		Clear value in lockfile payload: DELETE_ENTRY_ASAP for a given entry, if set. Entry must be already locked
		Return the count of shared users.
*/
off_t 						
		Cache::clearDeleteASAP_Flag(	const std::string &relPath)
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called. inputpath (relative): \"" << relPath << "\"") ;


	Lock lock(*this) ;

	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;
	ACS_COND_THROW( !foundInList,  // PRQA S 3081
		CacheInvalidFilenameException("clearDeleteASAP_Flag: File unknown: \"" + relPath + "\"") );	
	ACS_COND_THROW( !_entries[lockFileRelPath].isLocked(),  // PRQA S 3081
		CacheInvalidFilenameException("clearDeleteASAP_Flag: File not locked: \"" + relPath + "\"") ); 

	try
	{
		// Prepare the new payload for the lockfile :
		const Cache::LockType t = ( _entries[lockFileRelPath].getShared()? Cache::SH_GENERIC : Cache::EX_WRITE ) ; // PRQA S 3384

		vector<Cache::LockOperDateStruct> empty_v_add ; 	// Nothing to add
		vector<Cache::LockOperDateStruct> v_remove ;
		Cache::LockOperDateStruct oper ; // PRQA S 4102
		memset(&oper, 0, sizeof(oper)) ;
		oper.op = Cache::DELETE_ENTRY_ASAP ;				// Remove from current payload: DELETE_ENTRY_ASAP
		v_remove.push_back(oper) ;

		_entries[lockFileRelPath].setLockfilePayload(t, empty_v_add, v_remove) ;	// Build new payload
	}
	catch(std::exception &e)
	{
		ACS_LOG_ERROR("Couldn't run clearDeleteASAP_Flag for entry (" << __LINE__ << ")  Absolute Path: \"" << _entries[lockFileRelPath].filename() << "\"  Got exception: " << e) ; 
	}

	size_t c = _entries[lockFileRelPath].useCount() ;

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished. Returning: \"" << c);


	return c ; // PRQA S 3000
}


//
//    Cache::internalUse  (PRIVATE METHOD)
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
size_t 						
		Cache::internalUse(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
							off_t bytes,	/* If bytes>=0 : Reserve space */
							bool lk) 		/* True to lock */
{
	const bool shared = (bytes < 0) ;	/* This is the default behaviour: the lock is SHARED if no space has to be reserved. 
											EXCLUSIVE if space has to be reserved. If lk is false, the lock type will be ignored */
	return internalUse(relPath, bytes, lk, shared) ;
}


//
//    Cache::internalUse  (PRIVATE METHOD)
//
//////////////////////////////////////////////////////////////////////////////////////////
/** adds the file path and locks it (or doesn't lock, accordingly to flag)
*/
size_t 						
		Cache::internalUse(	const std::string &relPath,	/* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
							off_t bytes,	/* If bytes>=0 : Reserve space */
							bool lk, 		/* True to lock */
							bool shared)	/* If a lock is required, it can be SHARED (true) or EXCLUSIVE (false) */
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream o ; 
		o << "Method called. inputpath (relative): \"" << relPath << "\"  bytes: " 
		  << bytes << " lock? " << boolalpha << lk ;
		if (lk) { o << "  type: " << ( shared? "SHARED": "EXCLUSIVE" ) ; } // PRQA S 3380
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG


	RegExp regexp(_lockFilePattern) ;
	
	// Quick pre-check, filename should not be empty or a lock file
	if ( relPath=="" || relPath=="." || relPath==".." || relPath=="/" )
	{	ACS_THROW(CacheInvalidFilenameException("Empty or invalid filename: \"" + relPath + "\"") );	} // PRQA S 3081
	else if ( regexp.match(relPath) )
	{	ACS_THROW(CacheInvalidFilenameException("Invalid filename (LOCK EXTENSION NOT ALLOWED) \"" + relPath + "\"") );	} // PRQA S 3081
    else { /* nothing */ }
	// Check inputs
	ACS_COND_THROW( ((bytes>=0) && !lk), CacheInvalidFilenameException("Internal Error: can't reserve space without locking the entry") ) ; // PRQA S 3081

	// Check physical space in cache 
	const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ;	/* Physical space */ // PRQA S 3000, 3084
	ACS_COND_THROW( ((bytes>=0) && (freeDisk<bytes)), CacheFullException("Disk Full") ) ; // PRQA S 3081


	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;

	if (foundInList)
	{
		string lockFileAbsPath = StringUtils::pathJoin(_path, lockFileRelPath) ;	// absolute path (used for the touch command)
	
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB,"Inputpath (relative): \"" << relPath << "\"  to_lock: " << boolalpha << lk  
			  << "  lockfile was found: \"" << lockFileRelPath << "\"  with absPath: \"" << lockFileAbsPath << "\"") ;
	}
	else
	{	
		lockFileRelPath = File::addExt( StringUtils::pathNormalize(relPath), _lockFileExt ) ;	// relative path
		string lockFileAbsPath = StringUtils::pathJoin(_path, lockFileRelPath) ;	// absolute path (used for the touch command)

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB,"Inputpath (relative): \"" << relPath << "\"\tto_lock: " << boolalpha << lk 
			  << "  lockfile not found. Creating new lock: \"" << lockFileRelPath << "\"\twith absPath: \"" << lockFileAbsPath << "\"") ;
	}

	
	Lock lock(*this) ;	// Do not move from here


	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,"ACQ -- inputpath (relative): \"" << relPath << "\"") ;

	const bool entryRescan = true ;
	size_t uc = addEntryToList(lockFileRelPath, entryRescan) ;	// Add entry to map

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		debugDumpCache() ;
	ACS_CLASS_END_DEBUG

	if (lk) 
	{
		if (bytes<0)
		{
			Cache::LockType type = (shared ? Cache::SH_GENERIC : Cache::EX_WRITE) ; // PRQA S 3384
            
			uc = _entries[ lockFileRelPath ].use(shared, type) ;	// Shared-Generic lock and touch
		}
		else
		{
			uc = _entries[ lockFileRelPath ].reserve(_path, bytes, shared) ;	// Exclusive lock and touch. Also reserve space
		}

	} // if (lk)
	else {
		_entries[ lockFileRelPath ].touch() ;	// Touch file
    }

		
	const bool cacheModified = (!foundInList || _updateCacheEveryLock) ;
	const bool forceRescanEntries = false ;
	bool rescan = updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	// Forced only if a file was actually added
		
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,"Method finished. Inputpath (relative): \"" << relPath << "\"\tlockFile: \""
		  << lockFileRelPath << "\"\tto_lock: " << boolalpha << lk << " bytes: " << bytes 
		  << "  Rescan was needed: "  << boolalpha << rescan << "  Returning: " << uc) ;
	
	return uc ;
}


//
//    Cache::release
//
//////////////////////////////////////////////////////////////////////////////////////////
/** locks counter for file path is decremented. If locks counter reaches 0, the file is unlocked
*/
size_t Cache::release(	const std::string &relPath, /* Relative path of file, w/o lock ext. (e.g. "file", "dir1" or "dir2/file5" ) */
						bool relock_shared)
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called." 
			<< " inputpath (relative): \"" << relPath << "\"  relock_shared: " << boolalpha << relock_shared) ;

	Lock lock(*this) ;
	
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "ACQ. inputpath (relative): \"" << relPath << "\"") ;
	
	size_t uc = 0 ;
	bool foundInList = false ;
	string lockFileRelPath = searchLockFilePath(relPath, foundInList) ;

	ACS_COND_THROW(!foundInList, CacheInvalidFilenameException("Cache::release: Empty or invalid filename: \"" + relPath + "\"") );	 // PRQA S 3081

	// If lock was exclusive, probably the cache was somehow modifed
	const bool cacheModified = ( _entries[lockFileRelPath].isLocked() && (!_entries[lockFileRelPath].getShared() || _updateCacheEveryLock )) ;

	if (!relock_shared)
	{
		uc = _entries[lockFileRelPath].release() ;
		
		vector<Cache::LockOperDateStruct> v ; 
		try {
			Cache::LockType t ; // PRQA S 4101
			_entries[lockFileRelPath].getLockfilePayload(t, v) ; // Try to acquire LockOperDateStruct records
		}
		catch(std::exception& e)
		{
			v.clear() ; 

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Unable to retrieve payload for entry: \"" << relPath << "\" " << e) ;
		}
		
		try {
	
			for (vector<Cache::LockOperDateStruct>::const_iterator i=v.begin() ; i != v.end() ; ++i ) // PRQA S 4238
			{
				if (i->op == DELETE_ENTRY_ASAP)
				{
					CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to remove entry: \"" << relPath << "\"") ;

					eraseEntry(relPath) ;
					break ; 
				}
			}
		
		}
		catch(std::exception& e)
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Unable to remove entry: \"" << relPath << "\" " << e) ;
		}	
		
	}
	else
	{
		// Set PREV_AUTOERASE to -1, in order to prevent erase during lock transition
		const time_t prevent_date = -1 ;
		preventAutoErase(relPath, prevent_date) ;
		
		// Lock transition #1/2: Release exclusive lock 
		_entries[ lockFileRelPath ].release() ;

		// Lock transition #2/2: Acquire shared lock 
		const bool shared = true ;
		const Cache::LockType type = Cache::SH_GENERIC ;
		uc = _entries[ lockFileRelPath ].use(shared, type) ;	// Also, turn off PREV_AUTOERASE
	}
	
	const bool forceRescanEntries = false ;
	bool rescan = updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	// Forced only if a file was actually added

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Inputpath (relative): \"" << relPath 
			<< "\"\trelock_shared: " << boolalpha << relock_shared  << "  Rescan was needed: "  << boolalpha << rescan << "  Returning: " << uc) ;
	
	return uc ;
}


//
//    Cache::freeSpace
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Computes and returns the free space left on cache (>=0). Also keeping into account the reservations
*/
off_t Cache::freeSpace() const
{
	off_t usedBytes = bytes() ; // how much space is actually used ?
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, usedBytes << " used bytes") ;

	off_t left = (_maxSize > usedBytes) ? _maxSize - usedBytes : 0 ; // how much space left (for maxSize) // PRQA S 3384
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, left << " bytes left") ;
	const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ; // PRQA S 3000, 3084

	if (left > freeDisk) { // cache could be misconfigured: there is much virtual space left than real
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ptime d(microsec_clock::universal_time()) ;
			ostringstream o ; 
			o << "Cache::freeSpace() : WARNING [" << ptime2s(d) << "]"
				<< "  potential cache misconfiguration: " << freeDisk << " bytes left on device, while " << left << " left by conf in cache(" << _path << ")" ;
			ACS_LOG_WARNING(o.str()); 				
		ACS_CLASS_END_DEBUG
	}

	off_t ret = min(left, freeDisk) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, left << " bytes free in cache. " << freeDisk << " bytes free in filesystem. Returning: " << ret) ;
	
	return ret ;
}


//
//    Cache::freeSpaceWithSign
//
//////////////////////////////////////////////////////////////////////////////////////////
/** computes and returns the free space left on cache (it might also be NEGATIVE)
*/
off_t Cache::freeSpaceWithSign() const
{
	off_t usedBytes = bytes() ; // how much space is actually used ?
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, usedBytes << " used bytes") ;

	off_t left = _maxSize - usedBytes ;	/* It might be also NEGATIVE */
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, left << " bytes left") ;
	const off_t freeDisk = File::getFreeSpaceInMB(_path) * megabyte_ ; // PRQA S 3000, 3084

	if (left > freeDisk) { // cache could be misconfigured: there is much virtual space left than real
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ptime d(microsec_clock::universal_time()) ;
        	ostringstream o ; 
			o << "Cache::freeSpaceWithSign() : WARNING [" << ptime2s(d) << "]" 
			  << "  potential cache misconfiguration: " << freeDisk << " bytes left on device, while " << left << " left by conf in cache(" << _path << ")" ;
			ACS_LOG_WARNING(o.str()); 
		ACS_CLASS_END_DEBUG
	}

	off_t ret = min(left, freeDisk) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, left << " bytes free in cache. " << freeDisk << " bytes free in filesystem. Returning: " << ret) ;
	
	return ret ;
}


//
//    Cache::deleteEntry
//
//////////////////////////////////////////////////////////////////////////////////////////
/** remove one entry from both maps (_entriesMTime and _entries) and disk
*/
bool Cache::deleteEntry(const multimap<time_t, string>::iterator &i,	/* IN: Iterator to _entriesMTime */
						const map<string, Entry>::iterator &e, 		/* IN: Iterator to _entry */
						bool forceErase, 							/* IN: TRUE if manual erase (skip lock & erasable check) */
						vector <string>& list_of_removed_files,		/* OUT: append to the list of removed files */
						off_t& freedBytes)							/* OUT: bytes freed */
{
	bool success = false ;
	freedBytes = 0 ;

	if ( forceErase || (!e->second.isLocked() && e->second.isErasable()) ) // PRQA S 3230
	{
		const string lockFileRelPath = i->second ;
		const bool shared = false ;	// That is: EXCLUSIVE LOCK
		const Cache::LockType type = Cache::EX_ERASE ;
		e->second.use(shared, type) ;

		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Exclusive lock successfully for lockfile: " << lockFileRelPath) ;

		const bool abortOnError = true ;	/* TRUE: Abort in case of errors */
		success = deleteEntryFromDisk(	e->second.filename(), list_of_removed_files, freedBytes, abortOnError) ;	// Remove entry and lockfile

		if (success)
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Removing/unlocking entry: \"" << e->first << "\"") ;
			_entries.erase(e) ;	/* Also, unlock it ! */

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Removing _entriesMTime: " << i->first << "  (\"" << lockFileRelPath << "\")") ;
			_entriesMTime.erase(i) ;
		}	
		else
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Error removing entry. Releasing lock: \"" << e->first << "\"") ;

			e->second.release() ;	// Relase exclusive lock
		}	// END if (success)

	}	// if (!e->second.isLocked())
	else
	{
		CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Skipping entry \"" << e->first << "\" since it's locked") ;
		
		success = false ;
	}	// END if (!e->second.isLocked())


	return success ;
}


//
//    Cache::deleteEntryFromDisk
//
//////////////////////////////////////////////////////////////////////////////////////////
/** remove a complete path (files and subdirectories). Input is Lockfile absolute path
*/
bool					 													/* OUT: success (if TRUE is returned) */
		Cache::deleteEntryFromDisk(	const string &lockFileAbsPath, 			/* IN: Lockfile absolute path */
									vector <string>& list_of_removed_files,	/* OUT: append to the list of removed files */
									off_t& freedBytes, 						/* OUT: Freed space (bytes) */
									bool abortOnError) const				/* IN: TRUE to abort at the first error. FALSE to ignore errors */
{
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. lockFileAbsPath: \"" << lockFileAbsPath << "\"  abortOnError: " << boolalpha << abortOnError) ;

	size_t errors = 0 ;
	RegExp regexp(_lockFilePattern) ;

	freedBytes = 0 ;
		
	/* Check lockfile extension: */
	ACS_COND_THROW( !regexp.match(lockFileAbsPath),  // PRQA S 3081
		CacheInvalidFilenameException("Invalid lockfile name: \"" + lockFileAbsPath + "\"") );	

	/* Check lockfile existence: */
	ACS_COND_THROW( !File::exists(lockFileAbsPath),  // PRQA S 3081
		CacheInvalidFilenameException("lockfile not found: \"" + lockFileAbsPath + "\"") );	


	// IF HERE: Lockfile is valid
	
	string absPath = File::removeExt(lockFileAbsPath) ;


	if ( File::exists (absPath) && File::isDir(absPath) )
	{
		// About to (recursively) delete a directory
		try
		{
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Creating tree for absPath: \"" << absPath << "\"") ;
					
			const bool showHidden = true ;	// DO NOT MODIFY
			DirTree dir(absPath, "", showHidden) ;
			dir.reverse() ;	// After reverse, sort is from deepest path, i.e. /d1/d2/f2, /d1/d2/f1. /d1/d2/, /d1/f1, d1/f2, d1/

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Tree for path \"" << absPath << "\":  " << dir.size() << " entries") ;

            size_t d_size = dir.size() ;
			for (size_t i=0; i < d_size; ++i) 
			{
				try
				{	
					CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Removing entry: \"" << dir[i] << "\"") ;

					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						const size_t scanStep = max(size_t(1), dir.size()/10)  ; // PRQA S 3081, 3084
						if (0 ==  (i%scanStep)) // PRQA S 3084
						{
							ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Removing: " << (i+1) << "/" << dir.size()) ; // PRQA S 3084
						}
					ACS_CLASS_END_DEBUG

					if (File::isDir(dir[i]))
					{
						File::rmdir(dir[i]) ;
					}
					else
					{
						off_t tmp = File::size(dir[i]) ;
						CACHE_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "File: \"" << dir[i] << "\"  has size: " << tmp) ;
					
						File::unlink(dir[i]) ;
						// IF STILL HERE (No exception):
						if (true == getUpdateListRemovedFilesFlag()) { list_of_removed_files.push_back(dir[i]) ; }  // Append the file to the list of removed files
						freedBytes += tmp ;
					}
				}
				catch(std::exception &e) {
					ACS_LOG_ERROR(SimpleDebugSignatureThread << "Exception while trying to delete entry: \"" << dir[i] << "\": " << e) ;
					errors ++ ;
				}	// catch 

				if (( errors > 0 )&& abortOnError) { break ; }
			
			}	// for 
	
		}
		catch(std::exception &e) {
			ACS_LOG_ERROR(SimpleDebugSignatureThread << "Exception while trying to remove path: \"" << absPath << "\": " << e) ;
			errors ++ ;
		}	// catch 
	
	}	/* if ( File::exists (absPath) && isDir(absPath) ) */
	else if ( File::exists (absPath) )
	{
		// Delete a single file

		try
		{	
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Removing file: \"" << absPath << "\"") ;
			
			off_t tmp = File::size(absPath) ;
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "File: \"" << absPath << "\"  has size: " << tmp ) ;

			File::unlink(absPath) ;
			// IF STILL HERE (No exception):
			if (true == getUpdateListRemovedFilesFlag()) { list_of_removed_files.push_back(absPath) ; }  // Append the file to the list of removed files
			freedBytes += tmp ;

		}
		catch(std::exception &e) {
			ACS_LOG_ERROR(SimpleDebugSignatureThread << "Exception while trying to delete file: \"" << absPath << "\": " << e) ;
			errors ++ ;
		}	// catch 
	
	}	/* if ( File::exists (absPath) ) */
    else { /* nothing */ }

	// WHEN HERE: Payload (file or directory) removed

	if (0 == errors)
	{
		// lockfile cannot be removed if errors encountered !
		
		try
		{	
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Removing lockfile: \"" << lockFileAbsPath << "\"") ;

			::unlink(lockFileAbsPath.c_str()) ;
		}
		catch(std::exception &e) {
			ACS_LOG_ERROR(SimpleDebugSignatureThread << "Exception while trying to erase lockfile: \"" << lockFileAbsPath << "\": " << e) ;
			errors ++ ;
		}	// catch 
	}	// if (!errors)


	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "lockFileAbsPath: \"" << lockFileAbsPath 
			<< "\"  Method finished with: " << errors << " error.") ;

	return (errors == 0) ;	/* success */
} 


//
//    Cache::emptySpace
//
//////////////////////////////////////////////////////////////////////////////////////////
off_t Cache::emptySpace(off_t wished, off_t& left, size_t *filesFreed)  
{
/** THIS I/F ONLY PROVIDED FOR LEGACY COMPATIBILITY. */
	vector <string> ignored_list ; // List is not used
	return emptySpace(wished, left, ignored_list, filesFreed) ; 
}


//
//    Cache::emptySpace
//
//////////////////////////////////////////////////////////////////////////////////////////
/** tries to free an amount of bytes in order to allow the requester to write new data.
	 Returns the number of bytes actually freed. The returned number could satisfy the request even 
	 if no files has been actually removed e.g. the actual used size is less than the maximum size \see setMaxSize()
	 The method check also if there is enough (real) disk free.
	 Note that the cleaning is triggered when (cache_total_size - (cache_occupation + wished_space)) is < _quota_min_threshold [ not: < 0 ] 
	 When triggered, the cleaning will remove the oldest files, up to  (wished_space +  max(_quota_min_threshold, erase_quota))
	 Where: erase_quota = (_eraseQuotaPercentage * cache_total_size_ ) ;
	 In a valid configuration should result that: erase_quota > _quota_min_threshold [e.g., 2x], to prevent frequent calls to the cleaning method.
	 \param wished - how many bytes to free
	 \param filesFreed - pointer to a variable that will contain the number of files really unlinked
*/
// ENRCAR TODO FIXME
// After EACH SINGLE removal (or e.g. every 10), touch .cacheupdated.ignore (without updating all)
// So, if the whole erase process is very long (minutes), all other caches are notified that something is changing
off_t Cache::emptySpace(off_t wished, off_t& left, vector <string>& filenamesRemoved, size_t *filesFreed)  // PRQA S 4020
{
	Lock lock(*this) ;
	
	// Drop the list of User-Removed files, moving the content to the filenamesRemoved.
	// Note that this method is executed inside a LOCK, therefore the list of user-removed file CANNOT INCREASE in this scope
	if (false == _userRemovedFiles_V.empty())
	{
		// If here, the list of User-Removed files is not empty, and should be merged with the list of filenamesRemoved
		if (true == filenamesRemoved.empty()) { _userRemovedFiles_V.swap(filenamesRemoved) ; }	// Swap the two lists, full and empty
		else
		{
			// Both lists are full
			vector <string> tmp ; _userRemovedFiles_V.swap(tmp) ;
			filenamesRemoved.insert(filenamesRemoved.end(), tmp.begin(), tmp.end());			
		}
	}	// END: if (false == _userRemovedFiles_V.empty())
	
    off_t gtot = 0 ;
	size_t counter = 0 ;
	bool found = false ;

	{
		// Run a pre scan
		const bool cacheModified = false ;
        const bool forceRescanEntries = false ;
		updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	
	}

	if (filesFreed) {*filesFreed = counter ; }
	left = freeSpaceWithSign() ;	/* Free space in cache (free quota) */
	off_t required = ((wished+getQuotaMinThreshold()) > left) ? ((wished+getQuotaMinThreshold()) - left) : 0 ; // PRQA S 3384
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\twished: " << wished << "  ->  required to free: " << required << " bytes" 
			<< "  ( maxSize: " << maxSize() << "  res.bytes: " <<  getReservedBytesAndCleanOldFiles() << "  bytes: " << bytes() 
			<< "  free Space: " << freeSpace() << "  filenamesRemoved.size: " << filenamesRemoved.size() << " )") ;
	
	if (required <= 0) { // nothing to remove
		return wished ;
    }

	/*
		IF HERE:
		(required > 0), meaning that (wished > left), i.e. the space needed is larger than the free space immediately available.
		So a cache erase will be performed.
		If in low accuracy mode, the scan might be undone or stale. So a full scan will be performed
		(In high accuracy mode the full scan was performed above) 
		Also, "wished" will be INCREASED by eraseQuotaPcg, in order to exploit the overhead of the scan (this is more
		noticeable in lowAccuracy mode)
	*/

	
	// Since the cache erase is a resource-consuming task (expecially the scan) an EX-LOCK is used in order to have only one
	// process to perform this operation. The EX-LOCK is applied to a token-file
	{
		// Prepare the token filename
		const string erase_token_filename( StringUtils::pathJoin(_path, _cacheEraseInProgressFileNameRelPath)) ;	// Absolute path 
		const bool addExt(false); 
		const string pfx("") ; 
		
		// Setup the lockfile
		File::Lock erase_token(erase_token_filename, addExt, pfx) ;	// It will be UNLOCKED WHEN LEAVING THIS SCOPE
		const int attempts (30) ;	// 30 attempts
		const size_t attemptTimeout(60*1000) ; // 60 * (1000msec) = 1 minute
		const bool shared(false) ;  // EX-LOCK
		
		// ATTEMPTING THE EX-LOCK (30 minues) ...
		File::Lock::tryLock(erase_token, attempts, attemptTimeout, shared) ; // Get lock or emit exception !

		// IF HERE: THE EXCLUSIVE LOCK WAS ACQUIRED

		if (_lowAccuracy) {

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Performing a low accuracy scan") ;
			evaluateLowAccuracyBytes() ;

			left = freeSpaceWithSign() ;	/* Free space in cache (free quota) */
			off_t inner_required = ((wished+getQuotaMinThreshold()) > left) ? ((wished+getQuotaMinThreshold()) - left) : 0 ; // PRQA S 3384
			CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\twished: " << wished << "  ->  required to free: " << inner_required << " bytes" 
					<< "  ( maxSize: " << maxSize() << "  res.bytes: " <<  getReservedBytesAndCleanOldFiles() << "  bytes: " << bytes() 
					<< "  free Space: " << freeSpace() << " )") ;

			if (inner_required <= 0) { // nothing to remove
				return wished ;
    		}

			// Perform an actual disk scan, even if in low accuracy mode
			const bool cacheModified ( true ) ;
    		const bool forceRescanEntries ( false ) ;
			const bool lowAccuracy ( false ) ; // To force an actual scan
			updateCacheAndEntries(cacheModified, forceRescanEntries, lowAccuracy) ;	
		}	// END: if (_lowAccuracy)


		// First of all, compute the eraseQuota to be added to the wished space:
		const off_t eraseQuota ( 1.0 * (_eraseQuotaPercentage * _maxSize) ) ; // PRQA S 3011

		// Increase the wished space.
		// The maximum between the eraseQuota (which is the amount supposed to free, after the file allocation) and the minimum threshold trigger
		// otherwise the cleaning trigger wou, otherwise the cleaning condition would not be satisfied.
		// To be reasonable, the eraseQuota should be larger (twice at minimum???) that the minimum threshold trigger
		
		wished += max(eraseQuota, getQuotaMinThreshold()) ; // PRQA S 3011

		// Compute the new required space
 		required = (wished - left) ;


		size_t k=0; 	
		do
		{
			// EXTERNAL LOOP:
			// The internal loop is performed till enought space is freed. 
			// If no enought space was freed, the external loop performs a rescan and force a further loop. If nothing to remove is found, 
			// the external loop will be interrupted. 

        	multimap<time_t, string>::iterator j ;
			off_t tot = 0 ;

			// Perform a pre-scan
			const bool cacheModified = true ;
        	const bool forceRescanEntries = false ;
			updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	


			left = freeSpaceWithSign() ;	/* Free space in cache (free quota) */
			required = (wished > left) ? wished - left : 0 ; // PRQA S 3380

			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "External loop #" << ++k <<"  wished: " << wished << "  still required to free: " << required
					<< "  freed so far: " << gtot << " bytes (" << counter << " entries)") ;


			found = false ; 
			for (j=_entriesMTime.begin();  // PRQA S 4231, 4235
				( (required>0) && (tot < required) && (j!=_entriesMTime.end()) && (_entriesMTime.begin() != _entriesMTime.end()) );
				 )
        	{
				// INTERNAL LOOP (all entries are checked, trying to remove the first not locked entry. 
				// If enought space is freed, the internal loop is interrupted)

				multimap<time_t, string>::iterator i = j ; j++ ;	// Set current value (i) and prepare next loop (j)

				//(unused) const time_t mtime = i->first ; 
				const string lockFileRelPath = i->second ;
				map<std::string, Entry>::iterator e = _entries.find(lockFileRelPath) ;

				if (e == _entries.end())
				{
					// INTERNAL ERROR (TOO BAD, IGNORED...!)
					ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Internal Error: _entries and _entriesMTime misalignment: " 
							<< _entries.size() << " vs. " << _entriesMTime.size()) ;

					continue ;	// Error is ignored, for-loop is continued
				}

				try
				{
					off_t freedBytes = 0 ;

					const bool forceErase = false ;	/* Perform all checks (prevent erase of locked entries and check payload) */
					if ( deleteEntry(i, e, forceErase, filenamesRemoved, freedBytes) == true )
					{
						tot += freedBytes ; gtot += freedBytes ;
						counter ++ ;
						found = true ;
					}

				}
				catch(std::exception &ex) {
					ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Exception while trying to remove lockfile: " << lockFileRelPath << ": " << ex) ;
				}

	    	} // END for j

		} while ((required>0) && found) ;

	}	// END SCOPE: THE EX-LOCK WILL BE RELEASED
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  wished: " << wished << "  still required to free: " << required 
			<< "  FREED: " << gtot << " bytes (" << counter << " entries). AVAILABLE FREE SPACE IS: " << left << "  filenamesRemoved.size: " << filenamesRemoved.size() ) ;
	
	if (filesFreed) {
		*filesFreed = counter ;
    }
	return gtot ; 	
}

//
//    Cache::emptySpaceAndCheck
//
//////////////////////////////////////////////////////////////////////////////////////////
/** As above, but emits an exception if it couldn't free requested space
*/
off_t Cache::emptySpaceAndCheck(off_t wished, 
								size_t *filesFreed) 
{
/** THIS I/F ONLY PROVIDED FOR LEGACY COMPATIBILITY. */
	vector <string> ignored_list ; // List is not used
	return emptySpaceAndCheck(wished, ignored_list, filesFreed) ; 
}

//
//    Cache::emptySpaceAndCheck
//
//////////////////////////////////////////////////////////////////////////////////////////
/** As above, but emits an exception if it couldn't free requested space
*/
off_t Cache::emptySpaceAndCheck(off_t wished, 
								vector <string>& filenamesRemoved,
								size_t *filesFreed) 
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  wished: " << wished << "  Free space [before]: " << freeSpaceWithSign()) ;


	off_t toFree = (wished + (wished/5)) ; // Increased by 20%
	off_t availableSpace = 0;

	off_t ret = emptySpace(toFree, availableSpace, filenamesRemoved, filesFreed) ;

	ACS_COND_THROW( (availableSpace < wished), CacheFullException("Cache full. Couldn't free enought space") ); // PRQA S 3081
	
	return ret ;
}


//
//    Cache::getReservedBytesAndCleanOldFiles
//
//////////////////////////////////////////////////////////////////////////////////////////
/** returns reserverd space -- also remove "reservation files" that are too old
*/
off_t Cache::getReservedBytesAndCleanOldFiles() const 
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called") ;

	off_t tot = 0;
	size_t files = 0 ;
		
	const string reserveSpaceAbsDir = StringUtils::pathJoin(_path, _reserveSpaceRelDir) ; 
	const bool showHidden = true ; Dir dir(reserveSpaceAbsDir, showHidden) ;	// Non recursive

	RegExp regexp(_reserveSpaceFileNamePattern) ; for (size_t i=0; i<dir.size(); i++) {
		if ( regexp.match(dir[i]) )
		{
			bool success = false ;
			string absPath = StringUtils::pathJoin(reserveSpaceAbsDir, dir[i]) ;
			time_t now = time(0) ;
			time_t mtime = File::getModificationTime(absPath, &success );	
			if ((success) && (mtime<=now) && ((now-mtime)>_reserveSpaceFilesSelfRemoveTime) )
			{
				// File is orphan (modification time lapse > _reserveSpaceFilesSelfRemoveTime). To be removed.
				files ++ ;	// Debug-only counter
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Removing file \"" << absPath << "\"  #" << files << "  mtime: " << mtime << "  now: " << now) ;
				::unlink(absPath.c_str());	// File ORPHAN: Remove it (errors are ignored)
			}
			else
			{
				// File not orphan. Processing it
				
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Processing file \"" << absPath << "\"  mtime: " << mtime << "  now: " << now) ;

				// Reading Reserve File:	
				off_t bytes=0 ; bool bytes_success=false ;
				time_t expdate=0 ; bool expdate_success=false ;

				const bool useRelFileName = false ;
				ReserveSpace::readReservation(_path, absPath, useRelFileName, bytes, bytes_success, expdate, expdate_success) ;

				if (expdate_success)
				{
					ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
						ptime d = from_time_t(expdate) ;
                        ptime n = from_time_t(now) ;
						ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Processed file \"" << absPath << "\"  expiration date: "
							<< expdate << "(" << ptime2s(d) << ")  current time: " << now << "(" << ptime2s(n) << ")") ;
					ACS_CLASS_END_DEBUG 

					if (now > expdate)
					{
						files ++ ;	// Debug-only counter
						ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Removing EXPIRED file: \"" << absPath << "\"  #" << files) ;
						::unlink(absPath.c_str());	// File ORPHAN: Remove it (errors are ignored)

						continue ;	// for
					}
				}

				if (bytes_success) { 
					tot += bytes ;
					ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Read file \"" << absPath << "\"  bytes: " << bytes << "  so far: " << tot) ; 
				}	/* if (bytes_success) */

		
		
			}	/* if ((success) && (mtime<=now) && ((now-mtime)>_reserveSpaceFilesSelfRemoveTime) ) */
			
		}	/* END if ( regexp.match(dir[i]) ) */
	}	/* END for (size_t i=0; i<dir.size(); i++) */
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished.  Bytes reserved: " << tot << "  removed files: " << files) ;

	return tot ;
}


//
//    Cache::eraseEntryAndForget
//
//////////////////////////////////////////////////////////////////////////////////////////
/** removes one entry (and its lockfile) from the cache. If entry not locked, eraseEntry will be immediately called. 
If entry is shared-locked, payload DELETE_ENTRY_ASAP will be set. If entry is exclusively locked, an exception will be raised.
		Return TRUE is operation was executed successfully. FALSE if operation is delayed
*/
bool Cache::eraseEntryAndForget(const std::string &relPath) 
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Entry to remove: \"" << relPath << "\"") ;

	Lock lock(*this) ;

	bool immediate = false ; 
	
	try
	{
		// Assuming file is not locked, standard method eraseEntry is called:
		eraseEntry(relPath) ; 
		
		// IF HERE: SUCCESS
		immediate = true ;
	}
	catch(CacheLockException& e)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "It couldn't erase entry: \"" << relPath << "\"  operation will be delayed") ;

		immediate = false ;

		// Standard eraseEntry method failed. Assuming that the file is already SHARED-LOCKED, a new SHARED-LOCK is performed: 
		use(relPath) ;	// In case of problems, exception will be returned to the caller
			
		setDeleteASAP_Flag(relPath) ;
		
		release(relPath) ;
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Entry to remove: \"" << relPath << "\"  Returning: " << boolalpha << immediate) ;

	return immediate ; 
}


//
//    Cache::eraseEntry
//
//////////////////////////////////////////////////////////////////////////////////////////
/** removes one entry (and its lockfile) from the cache. Entry must not be locked of course ! 
*/
void Cache::eraseEntry(const std::string &relPath) 
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Entry to remove: \"" << relPath << "\"") ;


	Lock lock(*this) ;

	RegExp regexp(_lockFilePattern) ;
	
	// Quick pre-check, filename should not be empty or a lock file
	if ( relPath=="" || relPath=="." || relPath==".." || relPath=="/" )
	{	ACS_THROW(CacheInvalidFilenameException("Empty or invalid filename: \"" + relPath + "\"") );	} // PRQA S 3081
	else if ( regexp.match(relPath) )
	{	ACS_THROW(CacheInvalidFilenameException("Invalid filename (LOCK EXTENSION NOT ALLOWED) \"" + relPath + "\"") );	} // PRQA S 3081
    else { /* nothing */ }

	bool foundInList = false ;
	string lockFileRelPath ;
	
	//
	// Check if lockfile already inserted in map -- otherwise add it
	for (size_t i=0; (!foundInList && (i<2)); i++) // PRQA S 4400
	{
		lockFileRelPath = searchLockFilePath(relPath, foundInList) ;
	
		if (!foundInList)
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Adding entry \"" << relPath << "\"") ;

			add(relPath) ;
		}
	}
	
	
	ACS_COND_THROW( (!foundInList), exIllegalValueException("Internal Error: Unable to add entry: \"" + relPath + "\"" )) ; // PRQA S 3081

	string lockFileAbsPath = StringUtils::pathJoin(_path, lockFileRelPath) ;	// absolute path (used for the touch command)
	time_t mtime = _entries[lockFileRelPath].getMTime() ;


	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Lockfile found: \"" << lockFileRelPath  << "\" (\"" 
			<< lockFileAbsPath << "\")  mtime: " << mtime) ;


	bool found = false ; 
	size_t counter = 0 ;
	off_t tot = 0 ;
    multimap<time_t, string>::iterator j ;
    pair<multimap<time_t, string>::iterator,multimap<time_t, string>::iterator> ret;

    ret = _entriesMTime.equal_range(mtime) ;	// Find a range of iterators matching the hash-key

	for (j=ret.first; ((j!=ret.second) && (ret.first != ret.second)); )
	{
        multimap<time_t, string>::iterator i = j ; j++ ;	// Set current value (i) and prepare next loop (j)

        if ( i->second == lockFileRelPath ) 
		{  
			/* A matching pair was found ! */

			//(unused) const time_t mtime = i->first ; 
			const string lockFileRelPath2 = i->second ;
			map<std::string, Entry>::iterator e = _entries.find(lockFileRelPath2) ;

			if (e == _entries.end())
			{
				// INTERNAL ERROR (TOO BAD, IGNORED...!)
				ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Internal Error: _entries and _entriesMTime misalignment: " 
						<< _entries.size() << " vs. " << _entriesMTime.size()) ;

				continue ;	// Error is ignored, for-loop is continued
			}


			const bool forceErase = true ;	/* Manual override: skip all checks (prevent erase of locked entries and check payload) */
			off_t freedBytes = 0 ;

			if ( deleteEntry(i, e, forceErase, _userRemovedFiles_V, freedBytes) == true )
			{
				tot += freedBytes ; 
				counter ++ ;
				found = true ;
			}
			else
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Error removing file: \"" << relPath << "\"") ; 
			}

			
		}	/* if ( i->second == value ) */

    } // END for j


	if (found)
	{
		// Run a post-scan:
		const bool cacheModified = true ;
        const bool forceRescanEntries = false ;
		updateCacheAndEntries(cacheModified, forceRescanEntries, _lowAccuracy) ;	
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream o ; o << "Method finished." ;
		if (found) {
			o	<< "  FREED: " << tot << " bytes (" << counter << " entries)." ;
        }
		else {
			o	<< "  NOTHING TO DO." ;
        }
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, o.str()) ;
	ACS_CLASS_END_DEBUG
	
	return ;
}


//
//    Cache::size
//
//////////////////////////////////////////////////////////////////////////////////////////
/** the number of files in the cache
*/
size_t Cache::size() const
{
	Lock lock(*this) ;

	size_t s = _entries.size() ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning: " << s ) ;

	return s ;
}


//
//    Cache::sizeUsed
//
//////////////////////////////////////////////////////////////////////////////////////////
/** the number of locked files in the cache (whose counter is greater than 0) 
*/
size_t Cache::sizeUsed() const
{
	Lock lock(*this) ;
	// count records not locked
	ptrdiff_t size = count_if(_entries.begin(), _entries.end(), 
		boost::bind(&Entry::isLocked, 
					boost::bind<const Entry&>(&map<string, Entry >::value_type::second, _1))
		) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning: " << size) ;
	return size ; // PRQA S 3000
}


//
//    Cache::setMaxSize
//
//////////////////////////////////////////////////////////////////////////////////////////
/** \brief set cache max size[bytes]
	set maximum size in bytes the cache should allow. 
	It is used when asking for free space to ompute how max size should be freed. \see freeSpace()
	\param maxSize bytes

 */
void Cache::setMaxSize(off_t maxSize)
{
	Lock lock(*this) ;
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "old value: " << _maxSize << "  new value: " << maxSize) ;

	off_t byt = bytes() ;
	_maxSize = maxSize ;
	
	if (byt > maxSize) {
		ACS_LOG_WARNING("Cache::setMaxSize  [" << Cache::getTID() << "]  WARNING: used bytes (" << byt << ") are more than maxSize (" // PRQA S 3011, 3081
		  << maxSize << ")  [" << maxSize/double(megabyte_) << " MB] in cache \"" << _path << "\"") ;
	}
}


//
//    Cache::searchLockFilePath
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Search for lockfile given cache entry
*/
string 																/* OUT: relative pathname of lockfile (e.g. dir1/dir2/file.lock or dir1/dir2.lock) */
		Cache::searchLockFilePath(	const std::string &relPathIN,	/* IN: file/dir relative path (e.g. dir1/dir2/file ) */ // PRQA S 4020
									bool &found) const				/* OUT: TRUE if a lockfile was found */
{
	const string relPath = StringUtils::pathNormalize(relPathIN) ;

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. inputpath (relative): \"" << relPath << "\"") ;

	Lock lock(*this) ;

	vector <string> v_rel = StringUtils::pathDisassemble(relPath) ;
	
	if (v_rel.empty()) { found = false ; return "" ; }

	string subpath; 
    found = false ; 
	if (not v_rel.empty()) {
		// Subdirectory, or file a subdirectory. Searching for the lockfile, or a upper-directory lock
		for (ssize_t j=v_rel.size(); (!found && (j>0)) ; j--) // PRQA S 3000
		{
			vector <string> v_subpath ;
			for (size_t i=0; ssize_t(i)<j; i++) { v_subpath.push_back(v_rel[i]) ; } // PRQA S 3081
			subpath = StringUtils::pathAssemble(v_subpath) ;

			subpath = StringUtils::pathNormalize ( File::addExt(subpath, _lockFileExt) ) ;

			found = ( _entries.find(subpath) != _entries.end() ) ;

			CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "checking lockfile: \"" << subpath << "\"" << "  found: " << boolalpha << found) ;
		}	// for (ssize_t j=v_rel.size(); (!found && j>0); j--)

	} else {
		// "Flat" entry -- lockfile MUST BE SEARCHED only in root directory:
		subpath = StringUtils::pathNormalize ( File::addExt(v_rel[0], _lockFileExt) ) ;
		found = ( _entries.find(subpath) != _entries.end() ) ;
	}
	
		
	CACHE_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Inputpath (relative): \"" << relPath << "\"\tfound: " // PRQA S 3380, 3385
		  << (found? ("T  lockfile: \"" + subpath + "\""): "F")) ;

	return subpath ;	/* Only defined if found == true: the relative path (i.e. the key of _entries map) is returned */
}


//
//    Cache::removeFromEntriesMTime
//
//////////////////////////////////////////////////////////////////////////////////////////
/** Remove one or more identical entries (as pair of inputs) from a multimap. key is used just as HASH (since it's not-unique)
*/
bool 						// OUT: True if at least one pair was found
		Cache::removeFromEntriesMTime(multimap<time_t, string> &m, const time_t& key, const string& value) 
{
    pair<multimap<time_t, string>::iterator,multimap<time_t, string>::iterator> ret;

    ret = m.equal_range(key) ;	// Find a range of iterators matching the hash-key

    bool found = false ; 
    for (multimap<time_t, string>::iterator j=ret.first; 
         ((j!=ret.second) && (ret.first != ret.second)); )
    {
            multimap<time_t, string>::iterator i = j ; ++j ;	// Set current value (i) and prepare next loop (j)
            if ( i->second == value ) {   /* Also "Value" matched: A pair was found ! */
                m.erase(i) ; 
                found = true ;     /* Remove (Key, Value) pair */  
            }
    } // END for j

    return found ;
}


//
//    (STATIC)  Cache::lockTypeToString
//
//////////////////////////////////////////////////////////////////////////////////////////
string Cache::lockTypeToString(Cache::LockType t) 
{
	string s ;

	switch (t)
	{
    case (Cache::UN_GENERIC) :
        {
			s = "UN_GENERIC" ;
			break ;
        }
    case (Cache::SH_GENERIC) :
        {
			s = "SH_GENERIC" ;
			break ;
        }
    case (Cache::EX_GENERIC) :
        {
			s = "EX_GENERIC" ;
			break ;
        }
    case (Cache::EX_WRITE) :
        {
			s = "EX_WRITE" ;
			break ;
        }
    case (Cache::EX_ERASE) :
        {
			s = "EX_ERASE" ;
			break ;
        }
    case (Cache::ER_ERROR) :
        {
			s = "ER_ERROR" ;
			break ;
        }
    default :
		{
			ostringstream o ; o << "Cache::lockTypeToString  [" << Cache::getTID() << "]  " << __LINE__ 
				<< "  INTERNAL ERROR. Invalid type: 0x" << hex << int(t) ; // PRQA S 3081
			ACS_THROW( exIllegalValueException( o.str() ) ) ; // PRQA S 3081
		}
	}

	return s ;
}   
   

//
//    (STATIC)  Cache::intToLockType
//
//////////////////////////////////////////////////////////////////////////////////////////
Cache::LockType Cache::intToLockType(int t, bool emitException) 
{
	Cache::LockType lt ; // PRQA S 4101
	
	if ( t == int(Cache::UN_GENERIC) ) { // PRQA S 3081
		lt = Cache::UN_GENERIC ;
    }
	else if ( t == int(Cache::SH_GENERIC) ) { // PRQA S 3081
		lt = Cache::SH_GENERIC ;
    }
	else if ( t == int(Cache::EX_GENERIC) ) { // PRQA S 3081
		lt = Cache::EX_GENERIC ;
    }
	else if ( t == int(Cache::EX_WRITE) ) { // PRQA S 3081
		lt = Cache::EX_WRITE ;
    }
	else if ( t == int(Cache::EX_ERASE) ) { // PRQA S 3081
		lt = Cache::EX_ERASE ;
    }
	else if ( t == int(Cache::ER_ERROR) ) { // PRQA S 3081
		lt = Cache::ER_ERROR ;
    }
	else
	{
		if (emitException)
		{
			ostringstream o ; 
            o << "[" << Cache::getTID() << "] INTERNAL ERROR. Invalid type: 0x" << hex << t ;
			ACS_THROW( exIllegalValueException( o.str() ) ) ; // PRQA S 3081
		}
		else {
			lt = Cache::ER_ERROR ;
        }
	}

	CACHE_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, ": " << hex << t << dec << "  Returning: 0x" << hex << int(lt)) ; // PRQA S 3081
	
	return lt ;
}


//
//    (STATIC)  Cache::lockTypeToInt
//
//////////////////////////////////////////////////////////////////////////////////////////
int Cache::lockTypeToInt(Cache::LockType t) 
{
	return t ;
}


//
//    (STATIC)  Cache::dumpLockOperDateStructVector
//
//////////////////////////////////////////////////////////////////////////////////////////
void Cache::dumpLockOperDateStructVector(const vector<Cache::LockOperDateStruct>& v) 
{

	ACS_LOG_DEBUG(SimpleDebugSignature << "Method called.  vector size: " << v.size()) ;

	ostringstream o ; 
	for (size_t i=0; i<v.size(); i++)
	{
		Cache::LockOperDateStruct s = v[i] ;
		time_t t = time_t(s.expdate) ; // PRQA S 3081 3
		o << "[" << i << "]:  oper: " << hex << int(s.op) << dec << "   expdate: " << t << "  (" 
			<< ptime2s(from_time_t(t)) << ")\n" ;
	}
    ACS_LOG_DEBUG(o.str()) ;
	ACS_LOG_DEBUG(SimpleDebugSignature << "Method finished.") ;
}


_ACS_END_NAMESPACE
