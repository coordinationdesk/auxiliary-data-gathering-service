/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	Prod: A.C.S. File Tools Library

	$Log$
	Release 6.0.1  2019/03/20 17:45:39  enrico.carta@acsys.it
	EC:: Method getAbsolutePath (and relative test) improved to also take into account of ".." entries (previously not resolved)
	

*/

#ifndef _File_H_
#define _File_H_

#include <acs_c++config.hpp>

#include <exException.h>
#include <FileGuard.h>
#include <sstream>
#include <fstream>
#include <map>
#ifdef __linux__
  #if (__GNUC__ >= 3)
    #include <ext/stdio_filebuf.h>  
  #endif
#endif
#include <stdint.h>
#include <sys/stat.h>               
#include <sys/types.h>              

_ACS_BEGIN_NAMESPACE(acs)

/*!	\mainpage
	This library is intended to support file convenience functions.
	Among them there are access methods, compression status and methods 
	to deal with extensions
*/

class exostream ;

/*! \class File
	
	Utility class to support file convenience functions.
*/
class File                                  
{
public:

	// the class will be closed when destroyed
	class ifdstream : public std::ifstream { 
	public:
		explicit ifdstream(int fd, bool autoclose=true) ;                
		explicit ifdstream(FILE *file) ;
		~ifdstream() noexcept override ;
        ifdstream(const ifdstream &) = delete ;
		ifdstream &operator=(const ifdstream &) = delete ;

	private:
#ifdef __linux__
  #if (__GNUC__ >= 3)
		FILE *_file = nullptr ;	
		std::unique_ptr<std::basic_filebuf<char>> _bfilebuf = {} ;
        bool _autoclose = true;
  #endif
#endif
	} ;


	// the class will be closed when destroyed
	class ofdstream : public std::ofstream { 
	public:
		explicit ofdstream(int fd, bool autoclose=true) ;                
		explicit ofdstream(FILE *file) ;
		~ofdstream() noexcept override ;
		ofdstream(const ofdstream &) = delete ;
		ofdstream &operator=(const ofdstream &) = delete ;

	private:
#ifdef __linux__
  #if (__GNUC__ >= 3)
		FILE *_file = nullptr ;	
		std::unique_ptr<std::basic_filebuf<char>> _bfilebuf = {} ;
        bool _autoclose = true ;
  #endif
#endif
	} ;

public: // NOSONAR - for clarity 

	class Lock { 
	public:
		/*! class LockException declaration */
		exDECLARE_EXCEPTION(LockException,exIOException) ; 
	
		/*! the given file will be locked ( a file.lock will be created )
         */
		explicit Lock(const std::string &file, bool addExt=true, const std::string &pfx="") ; 
        Lock(const Lock &) = delete ; 
        Lock& operator=(const Lock &) = delete ; 
		/*! if the file was locked, it will be unlocked
            \see unlock 
            \see autoRemoveLockFile
         */
		virtual ~Lock() noexcept ;
			
		// to change the file to lock at runtime.
		// will raise an exception if the file is already locked
		void setLock(const std::string &file, bool addExt=true) ;
		
		/*! Tries to lock the file 
		 in case the timeout expires and the file has a lock held from
         someone else it returns false, else if lock is succesfull returns true.
         it raises exception for any other problem (access, disk full, wrong path etc)
        */
		bool lock(unsigned long timeout_ms=1000, bool shared=false) ; 
		
		const std::string &filename() const ;
		bool addExt() const ;	/* Returns TRUE if _addExt flag set */

		bool isLocked() const ;
		
        /*!
            This method unlock the lock file and will erase the actual lock if the autoRemoveLockFile has been called with 
            the true value before unlock. If the autoRemoveLockFile was left false - the lock file will stay on disk.
         */
		void unlock() ;
        
        /*! this method allows the unlink of the lock file - the one actually locked e.g. the one with lock extension if addExt was true
            
            Default (false) behaviour is to not remove the lock file. Setting this to true will cause the successful unlock 
            to also remove the lock file from the file system.
            Note that the unlink is done just before the unlock. Opposite behaviour could lead to other threads/processes 
            to lock succesfully a file that is about to be removed, causing the locking process to fall in the false assumption
            that the successful lock will prevent any other to do the same.
            
        */
        void autoRemoveLockFile(bool v) noexcept { _autoRemoveLockFile = v ; }

    
    	/*! 
         Tries to have a succesfull lock over the Lock instance. 
		 If it exhausts the attempt number without have a lock, it throws a LockException
		 if attempts is 0, it tries forever. NOTE: attempts is interpreted as absolute value !
		 The shared boolean means that a shared lock can be created instead of an exclusive one
		 note that only a process can hold an exclusive lock at time, more than one process can hold a shared one
		*/
        static void tryLock(Lock &, int attempts=10, size_t attemptTimeout_ms=1000, bool shared=false) ; 
	        
	private:
		std::string _lockedFile ;
		bool _addExt ;
		int _lockedStreamBase = -1;
        std::string _pfx ;
        bool _autoRemoveLockFile = false ;
        std::string _lockFileName = "" ;

		static const unsigned long _defaultRetryTime_ms ; // = 100 ; // msec

		ACS_CLASS_DECLARE_DEBUG_LEVEL(Lock) ;
	} ;


	
public: // NOSONAR - for clarity
	enum CompressionMode {  // NOSONAR - old enum
			Plain, 
			GZipped, 
			Compressed,
#ifdef __linux__
			BZipped,
#endif
			TARred,
			UNDEFINED
	} ;
	
	enum PermissionSet { // NOSONAR - old enum
		SetPerm,
		AddPerm,
		SubPerm
	} ;
	
    struct PerfStatistics { 
        
        double speed = 0 ;      // bytes/sec
        double total_time = 0 ; // [s]
        double size = 0 ;       // bytes 
        std::string spath = "" ;
        std::string dpath = "" ;
    } ;

	/*! This method returns the enumerant describing the compressions 
	    status of the given file.
	*/
	static CompressionMode compressionMode(const std::string &) ;
	static bool readOnly(const std::string &) ;
	static bool exists(const std::string &) ;
	static bool isDir(const std::string &) ;
	static bool isRegularFile(const std::string &) ;
    static bool isSymbolicLink(const std::string &, bool recursive=false) ;
	static bool isExecutable(const std::string &) ;
	static bool isReadable(const std::string &) ;
	/*! This method returns the size of a file. NB if the i/o interface to the os is 64 for bit, 
	    the returned value will be defined accrodingly.
		If entry is a directory, the sum of files sizes will be returned. Unless forceStat is true 
		(in this case, the size of the first inode or a conventional value is returned)
	*/
	static off_t size(const std::string &, bool forceStat=false) ;
#ifdef __linux__
	static blkcnt_t blocks512(const std::string &) ;
#endif

	// add file extension, avoid mistakes (e.g. ".<BLANK>" or "file..ext")
	static std::string addExt(const std::string &path, const std::string &ext) ;
	
	// returns the extension (from the LAST dot) e.g a.ext returs "ext"
	static std::string getExt(const std::string &) ;
		
    // returns the extension (from the FIRST dot)
	static std::string getFExt(const std::string &) ;

    // remove the extension (from the LAST dot)
    static std::string removeExt(const std::string &) ;
	
    
    // remove the given extension (if found)
    static std::string removeExt(const std::string &, const std::string& extIN) ;


	// check if the file matches the given extension
	static bool testMatchExt(const std::string &, const std::string& extIN);


    // remove the extension (from the FIRST dot)
    static std::string removeFExt(const std::string &) ;
	
    
	// returns the file with a new extension -- replace from the LAST dot
	static std::string fileWExt(const std::string &, const std::string &, bool autodot = true) ;
	
    
    // returns the file with a new extension -- replace from the FIRST dot
	static std::string fileWFExt(const std::string &, const std::string &, bool autodot = true) ;

  
  	/** returns true if the file is under the given path
		 use only string inference rules to check.
		\param path - the path from which re search starts - if it is relative uses getAbsolutePath() to find out the absolute one.
		\fileWPath - the file with absolute path. If the path is relative (dows not starts with '/', it uses getAbsolutePath() to find out the absolute one.
		
	*/
	static bool isFileInPathTree(const std::string &path, const std::string &fileWPath) ; 
    
	/// returns current working directory
	static std::string getCurrentPath() ;

	/// returns absolutepath if path is relative
	static std::string getAbsolutePath(const std::string &) ;
	
	/// returns file directory path 
	static std::string getDirPath(const std::string &, bool normalizePath=false) ;

	/// returns filename as basename
	static std::string getFileName (const std::string &);

	/// returns the complete path of the executable passed as input (uses current PATH)
	static std::string which(const std::string &) ;

 	/// builds cacheFilename from Filename, Fileversion and Inventory Date
	static std::string getCacheFileName(const std::string&, const std::string&, const time_t&) ;
	
	/// splits a cache filename in its components (Filename, Fileversion and Inventory Date)
	static void  splitCacheFileName( const std::string&, std::string&, std::string&, time_t& );
	
	/// Get free space into a volume (In Megabytes, i.e. (2^20) bytes) 
	static long int getFreeSpaceInMB(const std::string &volumeName) ;
	   
    /// Get free space into a volume (In Blocks) 
	static long int getFreeSpaceInBlocks(const std::string &volumeName, long int& blockSize) ;
    
    /// Get directory occupation (In Megabytes) 
	static double getDirectoryOccupationInMB(const std::string& rootDir, bool raiseException=false) ;
  
    /// Get volume total size (In Blocks) 
	static long int getVolumeSizeInBlocks(const std::string &volumeName, long int& blockSize) ;
    
    
	/// Just check if the final target is a link and expands it (not middle directories (if any)                
    static std::string readtargetlink(const std::string&, bool recursive=false) ;
    static std::string readlink(const std::string&, bool recursive=false) ;
	/// returns file type as returned from the file command
	static std::string file(const std::string &, const std::string &options="") ;
	static void rename(const std::string &old, const std::string &n) ;
	static void copy(const std::string &src, const std::string &dest, std::vector<PerfStatistics> *stats = nullptr, bool preserveMode = true) ;
	static void link(const std::string &src, const std::string &dest, bool hard=true) ;
	static void hardlink(const std::string &src, const std::string &dest, bool copyOnError=false, 
                         bool *copiedOnError = nullptr, std::vector<PerfStatistics> *stats = nullptr) ;
	/**
	 *  If src is a directory clone it in dest using hardlink strategy and returns the depth reached, otherwise only an hardlink is computed.
	 *  It allows to configure the maximum depth that can be reached by the algorithm.
	 */
	static short depthLimitedHardlink(const std::string &src, const std::string &dest, signed short maxdepth = -1, bool copyOnError=false, 
                                      bool *copiedOnError = nullptr, std::vector<PerfStatistics> *stats = nullptr);
	static void symlink(const std::string &src, const std::string &dest) ;
 	static bool unlink(const std::string &, bool changePerm=false, bool raiseException=false) ; /* TRUE if command actually removed the file. FALSE if file didn't exist or command failed; if changePerm is set, directories can be removed even if read-only (perm changed as u+wx), if requested exceptions will be raised */
	static bool remove(const std::string &) ; /* TRUE if command actually removed the file. FALSE if file didn't exist */
	static void touch(const std::string &);
	static void touch(const std::string &, time_t amtime);	
	static void concat(const std::string &file1, const std::string &file2, const std::string & fileOut);
	static void split(const std::string &fileIn, const std::string &file1, const std::string & file2, off_t fileSize, bool sizeFile1=true);
	// see chmod(2) man page for mode detail
	static void setgroup(const std::string &path, const std::string &group_name);
	static void setgroup(const std::string &path, const gid_t &gid);
	static void setgroup_rec(const std::string &path, const std::string &group_name);
	static void setgroup_rec(const std::string &path_in, const gid_t &gid);
	
	
	static gid_t getGroupIDFromPath(const std::string &path, std::string &group_name);
	static gid_t getGroupIDFromPath(const std::string &path);
	static uid_t getUserIDFromPath(const std::string &path, std::string &user_name);
	static uid_t getUserIDFromPath(const std::string &path);
	
	static uid_t getUserID(const std::string& username);
	static gid_t getGroupID(const std::string& groupname);

    static void chown(const std::string &path, uid_t owner, gid_t group) ;
    static void chown(const std::string &path, const std::string &owner, const std::string &group) ;
    static void chown_rec(const std::string &path_in, const std::string &owner, const std::string &group, bool dirs_only=false) ;
	static void chown_rec(const std::string &path_in, uid_t owner, gid_t group, bool dirs_only=false);
    
	static void chmod(const std::string &path, mode_t mode, PermissionSet perm=SetPerm);
	static void chmod_rec(const std::string &path_in, mode_t mode, PermissionSet, bool dirs_only=false);
	static mode_t getmod(const std::string &path);

	/** Verifies if an applicant is allowed to perform an operation to a resource */
	static bool check_access(	const uid_t& applicant_uid, const gid_t& applicant_gid, const uint32_t& applicant_oper_in,  
								const uid_t& resource_uid, const gid_t& resource_gid, const uint32_t& resource_mask_in)	;

	/** returns a new temporary valid dirname and change its permissions as requested */
	static FileGuard tempdir(const std::string& prefix, mode_t mode = 0775, PermissionSet perm = SetPerm); 
	// return a temporary valid filename
	static FileGuard tempname(const std::string &prefix, const std::string &ext="");
	static void truncate(const std::string &path, off_t size);
    
	static bool entry_is_broken_symlink(const std::string &s, bool prevent_exception=false) ;
	static bool entry_is_symlink(const std::string &s, bool prevent_exception=false) ;

 
 
    /**
        void evalNameSplitMatchExt:
        JUST EVALUATE THE OUTPUT FILENAMES, DON'T ACTUALLY SPLIT
		I: fileIn. Single Input filename to be splitted
        O: file1, file2: Splitted Filenames (as Output) 
        I: extS, ext1, ext2: Files extensions
        I: handleSingleExt: TRUE if the input (and the output, if no split required) must be matched w/ single-file extension extS
   		I: splitInput: TRUE to use extH+extS. FALSE to use only extS (accordingly to handleSingleExt)
    */
 	static void evalNamesSplitMatchExt(const std::string &fileIn, std::string& file1, std::string& file2, \
		const std::string& extS, const std::string& ext1, const std::string& ext2, bool handleSingleExt, bool splitInput=true) ;

 
    /**
        void splitMatchExt:
		I: fileIn. Single Input file to be splitted
        O: file1, file2: Splitted Files (as Output) 
        I: file1Size, file2Size: size of required files, file1 and file2. If one is zero, 
			no split will be actually performed (the output file will be treated as a single file)
        I: extS, ext1, ext2: Files extensions
        I: handleSingleExt: TRUE if the input (and the output, if no split required) must be matched w/ single-file extension extS
        I: file1OnTheBottom: TRUE if file1 was merged at the bottom of file2. FALSE for vice-versa
        I: overwriteSingleFile: Only applies if no split is required: TRUE to rename the source file to destination. FALSE to copy it, preserving the source.
    */
	static void splitMatchExt(const std::string &fileIn, 
                             std::string& file1, 
                             std::string& file2, 
                             off_t file1Size,
                             off_t file2Size, 
                             const std::string& extS, 
                             const std::string& ext1, 
                             const std::string& ext2,
                             bool handleSingleExt, 
                             bool file1OnTheBottom=true, 
                             bool overwriteSingleFile=false, 
                             bool checkSize=true) ;
 
    /**
        void splitMatchExtHandleTrailer:
		The file has a trailer, e.g. File.ext@trailer. The extension is handled regardless of the trailer
		I: fileIn. Single Input file to be splitted
        O: file1, file2: Splitted Files (as Output) 
        I: file1Size, file2Size: size of required files, file1 and file2. If one is zero, 
			no split will be actually performed (the output file will be treated as a single file)
        I: extS, ext1, ext2: Files extensions
        I: trailerPattern: the beginning of the trailer
		I: handleSingleExt: TRUE if the input (and the output, if no split required) must be matched w/ single-file extension extS
        I: file1OnTheBottom: TRUE if file1 was merged at the bottom of file2. FALSE for vice-versa
        I: overwriteSingleFile: Only applies if no split is required: TRUE to rename the source file to destination. FALSE to copy it, preserving the source.
    */
	static void splitMatchExtHandleTrailer(const std::string &fileInWTrailer, 
                                          std::string& file1, 
                                          std::string& file2, 
                                          off_t file1Size, 
                                          off_t file2Size, 
                                          const std::string& extS, 
                                          const std::string& ext1, 
                                          const std::string& ext2,  
                                          const std::string& trailerPattern, 
                                          bool handleSingleExt, 
                                          bool file1OnTheBottom=true, 
                                          bool overwriteSingleFile=false, 
                                          bool checkSize=true) ;
	
    /**
        void concatMatchExt:
        I: file1, file2: Input files to be merged
        O: fileOut. Splitted Files (as Output) 
        I: file1Size, file2Size: size of provided files, file1 and file2 (to double-check). If one file is not provided, its size should be zero.
        I: extS, ext1, ext2: Files extensions
        I: handleSingleExt: TRUE if the output must be matched w/ single-file extension extS
        I: file1OnTheBottom: TRUE to merge file1 at the bottom of file2. FALSE for vice-versa
        I: overwriteSource: The file on the top will be overwritten (and renamed) after the merge 
    */
    static void concatMatchExt(const std::string &file1, 
                              const std::string& file2, 
                              std::string &fileOut,
                              const off_t&, 
                              const off_t&, 
                              const std::string& extS, 
                              const std::string& ext1, 
                              const std::string& ext2, 
		                      bool handleSingleExt, 
                              bool file1OnTheBottom=true, 
                              bool overwriteSource=false) ;
	
    /**
        void mergeMatchExt:
  		ALIAS of concatMatchExt, with overwriteSource always set to TRUE
    */
    static void mergeMatchExt(const std::string &file1, const std::string& file2, std::string &fileOut, \
		const off_t&, const off_t&, const std::string& extS, const std::string& ext1, const std::string& ext2, \
		bool handleSingleExt, bool file1OnTheBottom=true) ;
	
     /**
        void mergeMatchExtRenameLink:
		Like mergeMatchExt. Buf if the file on the top is a symlink, both the link itself and the actual file (got following the link) will be overwritten	
    */
    static void mergeMatchExtRenameLink(const std::string &file1, const std::string& file2, std::string &fileOut, \
		const off_t&, const off_t&, const std::string& extS, const std::string& ext1, const std::string& ext2, \
		bool handleSingleExt, bool file1OnTheBottom=true) ;

    

	static void chdir(const std::string &) ;
	static void mkdir(const std::string &, bool allowExistingDir = false) ;
	static void mkdirtree(const std::string &) ;	/* works like mkdir -p */
	static void rmdir(const std::string &) ;
	static bool isOlder(const std::string &, size_t seconds) ;

    
    
	// returns the modification time in seconds (absolute time) given by stat.st_mtime
	static time_t getModificationTime (const std::string& path, bool* success=nullptr) ;

    /** \function
        contains a map with all mounted devices (like unix-cmd "mount")
        KEY: Device [string]
        VALUE: Mount_point [string]
    */
    static std::multimap<std::string, std::string> getMountEntries () ; 
	
    /**
        EC:: Return a Filesystem UID (Unique-ID) or ULLONG_MAX in case of error.
        Useful to discover if two files belong to THE SAME filesystem.
	*/    
    static unsigned long long int getFSUID (const std::string&, bool followSymLinks = false) ; 
 
 	static bool checkFileSystemsMatching(const std::string&, const std::string&, bool followSymLinks = false, 
                                         unsigned long long int * FSUID1 = nullptr, unsigned long long int * FSUID2 = nullptr) ;

    static void fileStat(const std::string &, struct stat &, bool resolveLinks = true) ; 
	
	static std::string computeMd5Sum(const std::string&);
		
	static void lockGlobalMutex();
	static void unlockGlobalMutex();
	static void reInitGlobalMutex();

#ifdef TEST
public:
#else
private:
#endif
	
    // computes and returns the lock file name 
    // used by lock method below and consequently by the File::Lock class to compute the name of the lock file
    static std::string computeLockName(const std::string &fname, bool addExt) ;
    

private: // NOSONAR - for clarity and break test visibility above

	// creates a lockfile name as fname (with the .lock extension if addExt) if successful
	// the returned file descritptor will be closed by the unlock method.
	// The shared boolean means that a shared lock can be created instead of an exclusive one
	// note that only a process can hold an exclusive lock at time, more than one process can hold a shared one
	// -errno is retuned if lock fails - errno signals the reason
	static int lock(const std::string &fname, bool addExt=true, bool shared=false) ;
	
	// unlock the file locked with the lock method. Return true if the file was unlocked.
	// As side effect, the file descriptor will be closed and set to -1 if the unlock is succesfull
	static bool unlock(int &) ;

	static std::string trimAllSlashes(const std::string& );
    static std::string trimTrailingSlashes(const std::string& ) ;
	static void splitTrailer(const std::string& inString, const std::string& pattern, std::string& header, std::string& trailer) ;
	static std::string readlinkRec(const std::string&, const std::string& prefix_in, bool& isSymLink) ;

// private data member
	static ThreadSafe _fileGlobalMutex;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(File) ;
} ;

exostream & operator<< (exostream &, File::CompressionMode) ; 
std::ostream & operator<< (std::ostream &, File::CompressionMode) ;


_ACS_END_NAMESPACE

#endif	/* _File_H_ */
