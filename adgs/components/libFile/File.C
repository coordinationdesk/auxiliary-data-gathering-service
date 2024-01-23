// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	Prod: A.C.S. File Tools Library

	Release 6.0.1  2019/03/20 17:45:39  enrico.carta@acsys.it
	EC:: Method getAbsolutePath (and relative test) improved to also take into account of ".." entries (previously not resolved)
	

*/


#include <File.h>
#include <CRC_Digest_md5.h>
#include <Password.h>
#include <exException.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <Dir.h>
#include <Timer.h>
#include <string>
#include <fstream>
#include <sstream>

#include <unistd.h>
#include <sys/types.h> // PRQA S 1013 3
#include <sys/file.h> 
#include <sys/statfs.h>
#include <grp.h>

#include <cstdio>
#include <utime.h>
#include <cstring>
#include <climits>
#include <deque>
#include <mountlist.h>

#include <lfV.h>


namespace {
    acs::lfV version ; // versioning 
}
_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using acs::pattern::FDGuard ;



//////////////////////////////
//							//
// classe File::ifdstream	//
//							//
//////////////////////////////

File::ifdstream::ifdstream(int fd, bool autoclose) :
#ifdef __linux__
  #if (__GNUC__ >= 3)
    ifstream(), // PRQA S 4052
  #else
	ifstream(fd),
  #endif
#endif
	_file(0),
	_bfilebuf(0),
    _autoclose (autoclose)
{
#ifdef __linux__
  #if (__GNUC__ >= 3)
	_file = fdopen(fd, "r") ; 
	if (!_file) {
		int err = errno;
		ostringstream os ;
		os << "Cannot fdopen on file descriptor #" << fd ;
		ACS_THROW(exIOException(os.str(),err)) ; // PRQA S 3081
	}
	_bfilebuf = new __gnu_cxx::stdio_filebuf<char>(_file,ios::in) ;
	basic_ios<char>::rdbuf(_bfilebuf) ;
  #endif
#else
	attach(fd) ;
#endif
}

File::ifdstream::ifdstream(FILE *file) :
#ifdef __linux__
  #if (__GNUC__ >= 3)
    ifstream(), // PRQA S 4052
  #else
	ifstream(fileno(file)),
  #endif
#endif
	_file(0),
	_bfilebuf(0),
    _autoclose (true)
{
#ifdef __linux__
  #if (__GNUC__ >= 3)
	_bfilebuf = new __gnu_cxx::stdio_filebuf<char>(file,ios::in) ;
	basic_ios<char>::rdbuf(_bfilebuf) ;
  #endif
#else
	attach(fileno(file)) ;
#endif
}

File::ifdstream::~ifdstream() throw() 
{
// PRQA S 4631 L1
	try { delete _bfilebuf ; }
    catch(exception &x) { ACS_LOG_NOTIFY_EX(x) ; }
	if (_file && _autoclose) { 
		if ( 0 != fclose(_file) )
		{
			int err = errno;
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::fclose syscall failed", err));
		}
	}
// PRQA L:L1
}




//////////////////////////////
//							//
// classe File::ofdstream	//
//							//
//////////////////////////////

File::ofdstream::ofdstream(int fd, bool autoclose) :
#ifdef __linux__
  #if (__GNUC__ >= 3)
    ofstream(), // PRQA S 4052
  #else
	ofstream(fd),
  #endif
#endif
	_file(0),
	_bfilebuf(0),
    _autoclose (autoclose)
{
#ifdef __linux__
  #if (__GNUC__ >= 3)
	_file = fdopen(fd, "w") ; 
	if (!_file) {
		int err = errno;
		ostringstream os ;
		os << "Cannot fdopen on file descriptor #" << fd ;
		ACS_THROW(exIOException(os.str(),err)) ; // PRQA S 3081
	}
	_bfilebuf = new __gnu_cxx::stdio_filebuf<char>(_file,ios::out) ;
	basic_ios<char>::rdbuf(_bfilebuf) ;
  #endif
#else
	attach(fd) ;
#endif
}

File::ofdstream::ofdstream(FILE *file) :
#ifdef __linux__
  #if (__GNUC__ >= 3)
    ofstream(), // PRQA S 4052
  #else
	ofstream(fileno(file)),
  #endif
#endif
	_file(0),
	_bfilebuf(0),
    _autoclose (true)
{
#ifdef __linux__
  #if (__GNUC__ >= 3)
	_bfilebuf = new __gnu_cxx::stdio_filebuf<char>(file,ios::out) ;
	basic_ios<char>::rdbuf(_bfilebuf) ;
  #endif
#else
	attach(fileno(file)) ;
#endif
}

File::ofdstream::~ofdstream() throw() 
{
// PRQA S 4631 L2
	try { delete _bfilebuf ; }
    catch(exception &x) { ACS_LOG_NOTIFY_EX(x) ;  }
	if (_file && _autoclose)  
	{ 
		if (0 != fclose(_file) )
		{
			int err = errno;
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::fclose syscall failed", err));
		}
	}
// PRQA L:L2
}



//////////////////////////////
//							//
// classe File				//
//							//
//////////////////////////////


ACS_CLASS_DEFINE_DEBUG_LEVEL(File) 

// Static attribute to protect fork and popen/plcose
ThreadSafe File::_fileGlobalMutex;

void File::lockGlobalMutex()
{
	_fileGlobalMutex.lock();
}

void File::unlockGlobalMutex()
{
	_fileGlobalMutex.unlock();
}

void File::reInitGlobalMutex()
{
	// Try to re-init and do not throw in case of failures, it will be used in the atfork handlers
	_fileGlobalMutex.reInit(false);
}

string File::file(const string &name, const string &options) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
	
	// check if the file is compressed or not
	string pcmd = "file " + options + " " ;	// execute unix command file
	pcmd += name ;
	
	FILE *filep = 0;
	{
		// Protect popen
		ThreadSafe::Lock lock (_fileGlobalMutex);
		filep = popen(pcmd.c_str(),"r") ;	// open pipe for command to be executed // PRQA S 4412
	}
	int err = errno;
	ACS_COND_THROW(!filep,exFileOpenException("Cannot stat file type on \"" + name +"\"",err)) ;  // PRQA S 3081


	size_t bufSize = 0 ;
	char *buffer=0;
	ssize_t retSize = ::getline( &buffer , &bufSize , filep) ;
	err = errno;
	if( retSize == -1)
	{
		try {
			ACS_THROW(exIOException( "Cannot read from cmd \"" + pcmd +"\"", err )) ; // PRQA S 3081
		}
		catch(exception &e) {
			// Protect pclose	
			ThreadSafe::Lock lock (_fileGlobalMutex);
			if ( -1 == pclose(filep) )
			{
				int errC = errno;
				ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::pclose syscall failed", errC));
			}
			if (buffer) { free(buffer); }
			ACS_LOG_NOTIFY_EX(e) ;
			return "" ;
		}
	}

	// Protect pclose	
	{
		ThreadSafe::Lock lock (_fileGlobalMutex);
		if (-1 == pclose(filep) ) {
			int errC = errno;
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::pclose syscall failed", errC));
		}
	}
	
	// get rid of ending spaces ff etc ...
	if (0 != isspace(buffer[retSize-1])) {
		buffer[retSize-1] = 0 ;
    }
	string result(buffer) ;
	free(buffer);
	
	// remove "xxx: " at the beginning if any
	string sep = ": " ;
	size_t b = result.find(sep);
	if (b != string::npos) {
		result = result.substr(b+sep.length()) ; // PRQA S 3084
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file type " << result) ;

	return result ;	// Return filetype
}


/* Get directory occupation (In Megabytes) */
/* if raiseException is SET an exception is raised if path not accessible */  
double File::getDirectoryOccupationInMB(	const string& rootDir,  // PRQA S 5500
													bool raiseException) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Called with input: " << rootDir << " raiseException: " << boolalpha << raiseException) ;

    
	double totSize = 0.0 ;
	double unit = (1024.0 * 1024.0) ;	/* Unit is Megabytes */

	deque <string> dirList ;
	dirList.push_back(rootDir) ;	/* Initialize the deque */


	while (dirList.size() != 0)
	{
		const string dirName ( dirList.front() ) ;
		dirList.pop_front() ; 
	
	    // Scan directory
	    ::DIR *dir = ::opendir(dirName.c_str()) ;
		int err = errno;
		if (dir == 0) {	
			if (!raiseException) {
				ACS_LOG_WARNING("WARNING: Cannot access path: " << dirName) ;
			} else {
				ACS_THROW(exIOException("Cannot access path: " + dirName,err)) ;	 // PRQA S 3081
			}
		} else {	

		    // reentrant version of readdir is used here
		    // see man 3 readdir for details on the malloc used here

#ifndef PRQA_ANALYZER
            size_t len = offsetof(struct dirent, d_name) + pathconf(dirName.c_str(), _PC_NAME_MAX) + 1;
#else
            size_t len = 16384 ; // just a number to avoid QA analyzer warning // PRQA S 4400
#endif
            struct dirent* entry = reinterpret_cast<struct dirent*> ( malloc(len) ); // PRQA S 3081
		    struct dirent* result = 0;

		    // This will automatically free the memory used by entry at dtor time
		    pattern::CMemoryScopeGuard theGuard (entry);

   		    int readdirval = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		    while ( ( 0 == (readdirval = readdir_r(dir, entry, &result) ) ) && ( 0 != result) ) { // PRQA S 4236
#pragma GCC diagnostic pop
	    	// for(struct ::dirent* e = 0; errno=0, e=::readdir(dir);) {  // PRQA S 4238, 4244
				
		    	const string item ( entry->d_name ) ;
		    	const string itemFullPath ( dirName + "/" + item ) ;
		    	if ((item == ".") || (item == "..")) {
			    	continue ;	/* Ignore . and .. */
            	}

            	struct stat s ; // PRQA S 4102
            	if (stat(itemFullPath.c_str(), &s) != 0)
            	{
            		// Used for both calculate the item size AND the item type (aka st_mode)
            		if (errno == ENOENT) { continue ; } /* Probably it's just a broken symlink */
            		ACS_LOG_WARNING("Error during stat on path: " << itemFullPath) ;
            		if (raiseException) { readdirval = errno ; break ; } // Quit for, emit exception 	
            		else { continue ; } // Entry is just skipped
            	}
				
				// Note: dirent's e->d_type might be used to check the ENTRY TYPE (directory, regular file...) but it isn't universally handled (notably Linux XFS)
						
				if ( S_ISDIR(s.st_mode) ) { 							 
					// Directory										 
					dirList.push_back(itemFullPath) ;  				 
				} else if ( S_ISREG(s.st_mode) ) {  					 

					// Regular file (including HDLINK, excluding SFLINK) 
					totSize += ( (s.st_size) / unit) ; // PRQA S 3011
					
				} else {
					// Not a DIR, nor a regular file (might be a socket, a symlink, .. ..) -- Just skip it
					continue ; 
				}
	    	}	// END: for 
			
		    ::closedir(dir) ;
	    	
			if (readdirval != 0) {	
				if (!raiseException) {
					ACS_LOG_WARNING("WARNING: Error while accessing path: " << dirName) ;
				} else {
					ACS_THROW(exIOException("Error while accessing path: " + dirName,readdirval)) ;	 // PRQA S 3081
				}
			} 
			
		}	// END ELSE: if (dir == NULL)
    }	// END while
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Done. Input: " << rootDir << " Size (MB): " << totSize) ;

	return totSize ;

}


//
//  EC::Ported from PDS2
//
bool File::entry_is_symlink(const string &s, bool prevent_exception) 
{
	
	// NOTE: The only way to discover if an entry is a symlink is to call ::readlink
	//   Usual ways (stat, etc.) would fail in case of BROKEN SYMLINK ! 
	
	const size_t bufferSize=PATH_MAX ;
	char buffer[bufferSize];
	memset(buffer, 0, bufferSize);	// from man: readlink() does not append a null byte to buf.

	bool isSymLink = true ; 
    if ( ::readlink( s.c_str(), buffer, bufferSize ) <0 )
	{
		int err = errno;
		if (err == EINVAL) {
			isSymLink = false ;	// EINVAL was returned, i.e. not a symlink. Original input is returned
		} else {
			isSymLink = false ; 
            ACS_COND_THROW( !prevent_exception, exIOException("Couldn't run readlink (non-recursive call): \"" + s + "\"", err ) ) ;  // PRQA S 3081
		}
	}	// END: if ( ::readlink ... )


	return isSymLink ;
}


//
//  EC::Ported from PDS2
//
bool File::entry_is_broken_symlink(const string &s, bool prevent_exception) 
{	
	// NOTE: The only way to discover if an entry is a symlink is to call :

	//   Usual ways (stat, etc.) would fail in case of BROKEN SYMLINK ! 

	bool isBrokenSymlink = false ; 
	int acc_status = ::access(s.c_str(), F_OK) ;		// ENOENT returned in case of broken symlink
	int err = errno;
	if (0 == acc_status) { 
		isBrokenSymlink = false ;
	} else if ((err==ENOENT) && entry_is_symlink(s, prevent_exception)) { isBrokenSymlink = true ; }
	else { 
        isBrokenSymlink = false ; 
        ACS_COND_THROW( !prevent_exception, exIOException("Error accessing entry: \"" + s +"\"", err ) ) ;  // PRQA S 3081
    } 
	 
	return isBrokenSymlink ;
}


void File::rename(const string &old, const string &n)  // PRQA S 5500
{
/*
 Tries to perform a rename -- if fails because among different volumes, a cp+rm
 is performed (this is the behaviour of "mv" command on linux systems)
*/
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  old: \"" << old 
			<< "\"  new: \"" << n << "\"") ;

	// Handle broken symlink
	ACS_COND_THROW( !File::exists(old) && !entry_is_broken_symlink(old),  // PRQA S 3081
		exIOException("Source file not found: \"" + old +"\"" )) ; 
	
	const bool followSymLinks = true ;
	if( entry_is_broken_symlink(old) || checkFileSystemsMatching( getDirPath(old), getDirPath(n), followSymLinks ) )  // PRQA S 3230
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << old << "\" and \"" << n << "\"  are in the same filesystem") ;

		int status = (::rename(old.c_str(), n.c_str())) ; 
		int err = errno;
		ACS_COND_THROW(0 != status, exIOException("Couldn't rename \"" + old +"\" in \"" + n + "\"",err)) ;	 // PRQA S 3081
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << old << "\" and \"" << n << "\"  are not in the same filesystem") ;

		if (File::isDir(old))
		{
			// Performing furher checks
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "\"" << old << "\" is a directory") ;
		
			// Check if target exists and it's a file (specific error message)
			ACS_COND_THROW((File::exists(n) && File::isRegularFile(n)),  // PRQA S 3081
				exIOException("Target exists and it's not a directory! \"" + n + "\"")) ;	

			// If target exists and is a directory, at least it must be empty :
			if (File::exists(n) && File::isDir(n))
			{
					const bool showHidden = true ; const string dirEntry = "" ;
					DirTree t(n, dirEntry, showHidden) ;
					ACS_COND_THROW( (t.size()!=1),  // PRQA S 3081
						exIOException("Target exists and it's not empty! \"" + n + "\"")) ;	
			}
		}	// END if (File::isDir(old))
		
		try
		{
			File::copy(old, n) ;
		}
		catch (exception& e)			
		{
			// Error during copy -- target corrupted: trying to remove it
			try { File::unlink(n) ; } catch (exception&) {} // Ignore errors while removing target
			
			ACS_LOG_NOTIFY_EX(e) ;	
			throw ;	// relaunch copy exception
		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Directories copy on target was successfull") ;

		// Copy successfull -- try removing the source
		try
		{
			File::unlink(old) ;

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Removal of source was successfull") ;
		}
		catch (exception& e)			
		{
			ACS_LOG_ERROR("Couldn't remove: \"" << old << "\"") ;
			ACS_LOG_NOTIFY_EX(e) ;	// notify but don't relaunch
		}

	}	// END if( checkFileSystemsMatching( ... ))

	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  old: \"" << old 
			<< "\"  new: \"" << n << "\"") ;
}

string File::readtargetlink(const string &o, bool recursive)  // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
 
	const unsigned long bufferSize=8192 ;
	char buffer[bufferSize];
    memset(buffer, 0, bufferSize);

	string trimmedPath = trimTrailingSlashes(o) ;	/* req'd, o/w readlink will fail */

    ssize_t status = ::readlink( trimmedPath.c_str(), buffer, bufferSize );
    int err = errno;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "::readlink(" << trimmedPath << ") -> " << status 
		    << " errno: " << err) ; 
    if (status == -1)
    {
		// EINVAL if not a sym link
		ACS_COND_THROW(err != EINVAL,exIOException("::readlink error: "+o , err )) ;  // PRQA S 3081
        return o ; // no exception - returns the input
    }

    buffer[bufferSize-1] = 0 ; // avoid segfault // PRQA S 3084
	if (!recursive) {
		return buffer ;
    }
	return File::readtargetlink(buffer, true) ;
}


string File::readlink(const string &o, bool recursive) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
 
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called. Input: \"" << o << "\"  recursive? " << boolalpha << recursive) ;

	const unsigned long bufferSize=8192 ;   // PRQA S 4400
	char buffer[bufferSize];
    memset(buffer, 0, bufferSize);

	string trimmedPath = trimTrailingSlashes(o) ;	/* req'd, o/w readlink will fail */

	if ( !recursive || trimmedPath.empty() )
	{
    	ssize_t status = ::readlink( trimmedPath.c_str(), buffer, bufferSize );
	int err = errno;
    	if (status == -1)
    	{
			ACS_COND_THROW(err != EINVAL,exIOException("Cannot run readlink: "+o ,err )) ;  // PRQA S 3081
        	return o ; // no exception - returns the input
    	}

    	buffer[bufferSize-1] = 0 ; // avoid segfault // PRQA S 3084

    	return buffer ;
	}
	else
	{
		bool ignored = false ;
		const string full_path = ( (trimmedPath[0] == '/' ) ? trimmedPath : StringUtils::pathJoin(getCurrentPath(), trimmedPath) ) ;  // PRQA S 3384, 3385
		const string prefix = "" ; 
		return readlinkRec(full_path, prefix, ignored) ;
	}
}


/* This method is PRIVATE. Allows BOTH to recursively expand all symlinks in a path (the same path will be returned if no symlinks), 
    AND to check if at least one symlink is contained inside a path (including the path itself)  */
string 													/* OUT: Processed PATH */
		File::readlinkRec(	const string& s_in,			/* IN: PATH to be processed */ // PRQA S 4020
							const string& prefix_in,	/* IN. Optional prefix, useful to handle symlinks to relative paths */ 
							bool& isSymLink)			/* OUT: It must be set FALSE by the caller -- Will be set TRUE if at least one symlink is found processing the whole path  */ 
{
// EC: this method has multiple returns to keep its complexity as low as possible

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called. Input: !" << s_in << "!  prefix: !" << prefix_in << "!") ;

	const string prefix = StringUtils::pathNormalize( prefix_in ) ;
	const string s = StringUtils::pathNormalize( s_in ) ;
	string s_out ;
    string prefix_out ;

	if (s.empty()) { return prefix ; }
	
	{
		// Extract the first level (e.g. /raid0/raid1/file -> /raid0 ; /raid1/file)
		pair <string, string> p =  StringUtils::pathExtractFirstLevel(s) ;

		// Update the new prefix with the first level extracted, to be processed
		prefix_out = StringUtils::pathNormalize( StringUtils::pathJoin(prefix, p.first) ) ; 

		// The remainder (second part) of the extraction, if any, will be processed further
		s_out = p.second ;
	}


	// WHEN HERE:  
	//    s_out is the remainder of the original path, removing the first level of the path each time
	//   prefix_out  is the prefix built to far


	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "prefix_out: \"" << prefix_out << "\"   s_out: \"" << s_out << "\"") ; 

	const bool prevent_exception = true ; 
	const bool recursive = false ;			/* NOT RECURSIVE, to avoid a loop !!! */
	off_t iter = 0 ; 						/* Counter the iterations of nested symlinks, to avoid looping */

	while ( (File::exists(prefix_out) && File::isSymbolicLink( prefix_out, recursive ) )
		|| (File::entry_is_broken_symlink(prefix_out, prevent_exception)) )
	{
        ++ iter ;
        
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Iteration: " << iter << "  Processing symlink: !" << prefix_out << "!") ;

		isSymLink = true ;	/* At least a symlink was found processing the path */
		const string r = File::readlink( prefix_out, (false /* NOT RECURSIVE, to avoid a loop !!! */) ) ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value from not recursive File::readlink is: !" << r << "!") ;
		
		// A new iteration will be called, with the same prefix and the entry returned by the readlink:
		if ( (!r.empty()) && (r[0]=='/') ) {
			prefix_out = r ;									/* The symlink was an absolute path */
		}
        else {
			prefix_out =  StringUtils::pathJoin(prefix, r) ;	/* The symlink was a relative path */
		}
        	
		if (iter>=1024) // PRQA S 4400
		{
			ACS_LOG_ERROR("Too many nested symlinks: " << iter) ;
			break ; 
		}
	}


	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Iteration finished. Starting a new iteration with entry: \"" << s_out << "\"  prefix: \"" << prefix_out 
					<< "\"  isSymLink: " << boolalpha << isSymLink) ;

    return File::readlinkRec(s_out, prefix_out, isSymLink) ;  /* Recursive call */
}


void File::link(const string &src, const string &dest, bool hard) 
{
	if (hard) {
		File::hardlink(src,dest) ;	// execute hardlink
    }
	else {
		File::symlink(src,dest) ;	// execute softlink
    }
}


void File::copy( const string &src, const string &dest, vector<PerfStatistics> *stats, bool preserveMode)  // PRQA S 4020, 5500, 5510
{
// EC: this method has multiple returns to keep its complexity as low as possible
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  src: \"" << src 
			<< "\"  dest: \"" << dest << "\"") ;
	
    if (src == dest) { return ; }	/* Nothing to do! */

	if ( File::exists(src) && File::isDir(src) )
	{
		// Source is a directory: target cannot be a (already existent) file
		ACS_COND_THROW((File::exists(dest) && File::isRegularFile(dest)),  // PRQA S 3081
			exIOException("Target exists and it's not a directory! \"" + dest + "\"")) ;	
		
		// HERE: Source is a directory and Target either doesn't exist or is a directory
		
		vector< pair<string, mode_t> > modes ;	// Container for source tree permissions (to be set to dest. tree)
		
		const string dirEntry = "" ; const bool showHidden = true ;
		DirTree dir(src, dirEntry, showHidden) ;
		dir.relative() ;	// Force entries to relative path

        size_t d_size = dir.size();
		for (size_t i=0; i<d_size; ++i)
		{
			string s ;
            string d ;
			if  ( dir[i] != "." )
			{
				s = StringUtils::pathNormalize( StringUtils::pathJoin(src, dir[i])) ;
				d = StringUtils::pathNormalize( StringUtils::pathJoin(dest, dir[i])) ;
			}
			else
			{
				s = StringUtils::pathNormalize( src ) ; 
                d = StringUtils::pathNormalize( dest ) ;
			}


			if (isDir(s))
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Creating destination directory: \"" << d << "\"") ;

				mkdir(d.c_str(), 0777) ;	// Create destination (sub)dir. Ignore error code // PRQA S 4400

				if( preserveMode ) {
					// Read permissions from src directory tree :
					try
					{
						mode_t m = File::getmod(s); 
						modes.push_back( pair <string, mode_t>(d, m) ) ;
					}
					catch(exception &e) 
					{
						ACS_LOG_ERROR("Couldn't read permissions from: \"" << s << "\"") ; 
						ACS_LOG_NOTIFY_EX(e) ;
					}
				}
			}
			else
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Copying file: \"" << s 
						<< "\"  to:  \"" << d << "\"") ;

				File::copy(s, d, stats, preserveMode) ;	// Semi-recursive call (a different portion of code, non recursive, will be called to copy a file) 
			}
		}	//	END for (size_t i=0; i<dir.size(); i++)
		
		// Set permissions to dest directory tree (as very last operation, since some dest. directory could be... NOT WRITABLE!!!)
        size_t modessize = modes.size() ;
		for (size_t i=0; i<modessize; ++i)
		{
			string d = modes[i].first ; mode_t m = modes[i].second ; 
			try { File::chmod(d, m) ; } 
			catch(exception &e) 
			{
				ACS_LOG_ERROR("Couldn't set permissions: 0" << oct << m << dec << " to directory: \"" << d << "\"") ; 
				ACS_LOG_NOTIFY_EX(e) ;
			}
		}	// END for (size_t i=0; i<modes.size(); i++)
	}
	else
	{
		// Source either doesn't exist, or is a file
		try {
            PerfStatistics ps ;
            ps.spath=src ;
			errno=0 ; string ndest ;
			ifstream in(src.c_str());
			if (!in) {  // error cannot open
				// note isReadable use access and set errno
				ACS_COND_THROW(!File::isReadable(src), exIOException("Cannot access \"" + src + "\"", errno)) ; // PRQA S 3081
			}
			in.exceptions(ios_base::badbit | ios_base::failbit) ;

			ndest = ( (File::exists(dest) && File::isDir(dest))?	// PRQA S 3380, 3385
				StringUtils::pathNormalize( StringUtils::pathJoin(dest, getFileName(src)) )	: dest ) ;

            ps.dpath=ndest ;

			// If both source and destination exists 
			// be sure that they do not are 2 hardlinks to the same i-node
			if ( File::exists(ndest) )
			{
				struct stat srcStat ;
				File::fileStat (src, srcStat);
                struct stat destStat;
				File::fileStat (ndest, destStat);
				ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Source      : " << src   << " - devid: " <<  srcStat.st_dev << " - inode : " << srcStat.st_ino);
				ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Destination : " << ndest << " - devid: " << destStat.st_dev << " - inode : " << destStat.st_ino);

				// Throw excetpion if both dev id and inode id are the same 
				// and the reported inode is not zero (protect against FUSE user space file system implementation)
				if ( (srcStat.st_dev == destStat.st_dev) && (srcStat.st_ino == destStat.st_ino) && (0 != srcStat.st_ino) )
				{
					ACS_THROW ( exIOException("Source and Destination are the same file! Src:\"" + src + "\" - Dst:\"" + ndest + "\"")) ;	// PRQA S 3081
				}
				
				// Now try to unlink the file before starting the copy
				// This will give another layer of protection in case of undetected linked files
				try
				{
					File::unlink (ndest);
				}
				catch (exception& ex)
				{
					// Ok. We gave it a try.
					// Do not mind and go ahead.
					ACS_CLASS_WRITE_DEBUG_TH (ACS_LOW_VERB, ex);
				}

			}    

			ofstream out(ndest.c_str(), ios_base::trunc);
			if (!out) {  // error cannot open
				// note isReadable use access and set errno
				ACS_COND_THROW(!File::isReadable(ndest), exIOException("Cannot access \"" + ndest + "\"", errno)) ; // PRQA S 3081
			}
			out.exceptions(ios_base::badbit | ios_base::failbit) ;

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to copy file (low-level): \"" << src 
					<< "\"  to:  \"" << ndest << "\"") ;

            RTTimer pt ; 
            pt.start() ;
			char buffer[1048576]; // PRQA S 4403
			off_t n = 0 ;
			do {
				try {
					in.read(buffer, sizeof(buffer));
				}
				catch(...) {
					if (!in.eof()) { throw ; }
				}
				n=in.gcount() ;
				out.write(buffer , n);
			} while(n == sizeof(buffer)) ; // PRQA S 3000
			in.close();
			out.close(); 					
            pt.stop() ;
			if( preserveMode ) {
			// Preserve file permissions
				try
				{
					mode_t m = File::getmod(src); File::chmod(ndest, m) ;
				}
				catch(exception &e) 
				{
					ACS_LOG_ERROR("Couldn't read permissions from: \"" << src << "\" / set permissions to: \"" << ndest << "\"") ; 
					ACS_LOG_NOTIFY_EX(e) ;
				}
			}
            
            if (stats) {
                ps.total_time = pt.elapsed() ; // PRQA S 3012
                ps.size = File::size(ps.spath) ; // PRQA S 3011
                ps.speed = ps.size / ps.total_time ;
                stats->push_back(ps) ;
            }
    	}
    	catch (exception& e)
    	{
			// forward errno 
			int errnoex = errno ;
            exException* ex = dynamic_cast<exException*>(&e) ; // PRQA S 3081
			if (ex) {
				errnoex = ex->errorNumber() ;
            }
        	ACS_THROW(exIOException(e,"Cannot copy \"" + src +"\" to \""+ dest + "\"",errnoex)) ;	 // PRQA S 3081
    	}
	}        
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  src: \"" << src 
			<< "\"  dest: \"" << dest << "\"") ;
}

void File::hardlink(const string &src, // PRQA S 4020, 5500, 5510
					const string &dest,
					bool copyOnError, /* If files belong to different filesystems, a COPY will be performed */
					bool *copiedOnError, /* If a COPY has been performed, valorize if allocated */
					vector<PerfStatistics> *stats
					)
{
	depthLimitedHardlink(src, dest, -1, copyOnError, copiedOnError, stats);
}

short File::depthLimitedHardlink(const string &src, // PRQA S 4020, 5500, 5510
					const string &dest,
					signed short maxdepth, /* the maximum depth that can be reached when src is a directory. -1 for any depth */
					bool copyOnError, /* If files belong to different filesystems, a COPY will be performed */
					bool *copiedOnError, /* If a COPY has been performed, valorize if allocated */
					vector<PerfStatistics> *stats
					)
{
// EC: this method has multiple returns to keep its complexity as low as possible
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called: src: " << src 
			<< " dest: " << dest << " maxdepth: " << maxdepth << " copyOnError flag: " << boolalpha << copyOnError) ;
	
	short actualDepth = 0;
	// check if it is a directory
	if (File::exists(src) && File::isDir(src)) {
		vector < pair <string, mode_t> > modes ;
		vector < gid_t > gids ;
		FileGuard theGuard ;
		
		// check if destination exists and is a dir 
		if (exists(dest) && !isDir(dest)) {
			ACS_THROW(exIOException("Cannot link (hard) to "+ dest + ": it is a regular file")) ;	 // PRQA S 3081
		}
		else if (!exists(dest)) { // create the dest directory

			mkdir(dest, 0777) ; // PRQA S 4400
			FileGuard destGuard(dest) ;
			theGuard = destGuard ; // theGuard owns the dest dir
		}
		else { /* nothing */ }

		try
		{
			mode_t m = File::getmod(src); 
			gid_t g = File::getGroupIDFromPath(src); 
			modes.push_back( pair <string, mode_t>(dest, m) ) ;
			gids.push_back( g ) ;
		} 
		catch(exception &e) 
		{
			ACS_LOG_ERROR("Couldn't get permission/group from root directory: \"" << src << "\"") ; 
			ACS_LOG_NOTIFY_EX(e) ;
		}
		
		std::string normEntry = "";
		DirTree dt(src,"",true) ; // hidden files too
		dt.relative("") ;
		if (maxdepth >= 0) {
			dt.maxdepth(maxdepth);
		}

		short currDepth = 0;
		size_t dt_size = dt.size();
		for (size_t i = 0; i < dt_size; ++i) {
			string currEntry = dt[i];
			currDepth = dt.depth(i);
			if (currEntry == ".") {
				continue ;
			}
			string source = src+"/"+currEntry ;
			string destination = dest +"/" + currEntry ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, source << " to: " << destination) ;
			
			// If source is a broken symlink skip the file and set the flag to true to notify the problem
			if (entry_is_broken_symlink(source, true)) {
				ACS_LOG_WARNING("Found broken symlink " + source + ". It will be ignored");
				continue;
			}
			
			if (isDir(source)) 
			{
				try
				{
					mode_t m = File::getmod(source); 
					gid_t g = File::getGroupIDFromPath(source); 
					modes.push_back( pair <string, mode_t>(destination, m) ) ;
					gids.push_back( g ) ;
				} 
				catch(exception &e) 
				{
					ACS_LOG_ERROR("Couldn't get permission/group from directory: \"" << source << "\"") ; 
					ACS_LOG_NOTIFY_EX(e) ;
				}
				
				mkdir(destination, 0777) ;	// is a directory // PRQA S 4400
			}
			else {
				depthLimitedHardlink(source, destination,maxdepth,copyOnError, 0, stats) ;	// is a file
			}
			
			actualDepth = std::max(actualDepth, currDepth);
		}
		theGuard.detach() ; // do not delete on exit
		
		// Set permissions to dest directory tree (as very last operation, since some dest. directory could be... NOT WRITABLE!!!)
		if (modes.size() == gids.size()) {
			size_t msize = modes.size(); 
			for (size_t i=0; i<msize; ++i)
			{
				string d = modes[i].first ; 
				mode_t m = modes[i].second ; 
				gid_t g = gids[i] ; 
				try
				{
					File::chmod(d, m) ; 
					File::setgroup(d, g) ; 
				} 
				catch(exception &e) 
				{
					ACS_LOG_ERROR("Couldn't set permissions / gid to directory: \"" << d << "\"") ; 
					ACS_LOG_NOTIFY_EX(e) ;
				}
			}	// END:  for (size_t i=0; i<modes.size(); i++)
		}	// END: if (modes.size() == gids.size()) {
		
		return actualDepth;
	}
	
	// Single file hardlink (or copy)
	RTTimer pt ;
	pt.start() ;

	if (0 != ::link(src.c_str(), dest.c_str())) 
	{
		int err = errno;
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Error hardlinking: " << src << " to: " << dest << " Errno: " << err << " " << strerror(err)) ;
			if ( copyOnError ) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Trying to copy files...") ;
			}
		ACS_CLASS_END_DEBUG

		if (copyOnError)
		{
			// Try to copy file instead of hardlinking
			// The check of the same filesystem for both source and destination has been replaced by a warning (once per process)
			// because there are lots of possibilities to not have grants to make hardlink across the same filesystem
			// due for example to SeLinux or Capabilities
			// See "man 5 proc" section "/proc/sys/fs/protected_hardlinks" to get some examples
			
			// Emit the warning just once and do never check again if emitted
			static bool copyOnErrorSameFSWarningEmitted = false;
			if ( false == copyOnErrorSameFSWarningEmitted )
			{
				const bool followSymLinks = true ;
				unsigned long long int FSUID1 = 0;
				unsigned long long int FSUID2 = 0;
				if( checkFileSystemsMatching( getDirPath(src),getDirPath(dest), followSymLinks, &FSUID1, &FSUID2 ) ) 
				{
					copyOnErrorSameFSWarningEmitted = true;
					ACS_LOG_WARNING ("Hardlink on the same filesystem [ from " << src << " to " << dest << " ] failed. Using copy. Check security and FS configuration.");
				}
			}
			
			File::copy(src, dest, stats) ;
			if( copiedOnError ) {
				*copiedOnError = true;
			}
			chmod(dest,getmod(src)) ;
		}
		else
		{
			// Unable to link and not allowed to copy as fallback solution
			ACS_THROW(exIOException("Cannot link (hard) \"" + src +"\" to \""+ dest + "\"" ,err)) ;	 // PRQA S 3081
		}
	}
	else 
	{
		// Hardlink successfully executed. 
		// Report that it has not been copied on error, if possible
		if(copiedOnError) {
			*copiedOnError = false;
		}
	}

	// Update statistics on the single file
	if (stats) {
		pt.stop() ;
		PerfStatistics ps ;
		ps.spath=src ;
		ps.dpath=dest ;
		ps.total_time = pt.elapsed() ; // PRQA S 3012
		ps.size = File::size(ps.spath) ; // PRQA S 3011
		ps.speed = ps.size / ps.total_time ;
		stats->push_back(ps) ;
	}
	
	return actualDepth;

	// Work complete!
}


void File::symlink(const string &src, const string &dest ) 
{
	if (0 != ::symlink(src.c_str(), dest.c_str())) {
		ACS_THROW(exIOException("Cannot link (soft) \"" + src +"\" to \""+ dest + "\"" ,errno)) ;	 // PRQA S 3081
	}
}


bool File::unlink(const string &f, bool changePerm, bool raiseException) // PRQA S 4020
{
	// EC: this method has multiple returns to keep its complexity as low as possible

	if(!File::exists(f))  // Either not-existent, or it's a broken symlink
	{
		if (::unlink(f.c_str()) != 0)
		{
			// The following exception notification has been made visible only in debug, because 
			// there are tons of code asking to remove unexisting files and we cannot pollute their log 
			// with exceptions.
			// Examples of those classes is those creating a tmp file that can be moved (or renamed) in nominal flow.        
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) ;
				int err = errno;
				ACS_LOG_TRYCATCHNOTIFY_EX( exIOException  ("Unable to unlink file " + f, err));
			ACS_CLASS_END_DEBUG  ;

		}
		return false ;
	}

	if (File::isSymbolicLink(f) || !File::isDir(f))
	{
		if (::unlink(f.c_str()) != 0)
		{
			int err = errno;
			if (raiseException)
			{
				ACS_THROW ( exIOException  ("Unable to unlink file " + f, err));
			}

			// see comment above
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) ;
				ACS_LOG_TRYCATCHNOTIFY_EX( exIOException  ("Unable to unlink file " + f, err));
			ACS_CLASS_END_DEBUG ;

			// Failed!
			return false;
		}
	}
	else
	{
		if( changePerm ) 
		{
			try 
			{
				File::chmod(f, 0300, AddPerm); // PRQA S 4400
			}
			catch( exception &)
			{
				// see comment above about log pollution
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) ;
					int err = errno;
					ACS_LOG_TRYCATCHNOTIFY_EX( exIOException  ("Unable to change permission u+wx of directory " + f, err));
				ACS_CLASS_END_DEBUG ;
			}
		}

		const bool showHidden = true ;
		const bool reportBrokenLinks = true;

		Dir dir(f, showHidden, reportBrokenLinks) ;
		for (size_t i=0; i<dir.size(); i++)
		{
			string entry = f + "/" + dir[i] ;
			File::unlink(entry, changePerm, raiseException) ;
		}

		if (0 != ::rmdir(f.c_str()) )
		{
			int err = errno;
			if (raiseException)
			{
				ACS_THROW(exIOException("Cannot remove directory: '" + f + "' ", err)) ;	 // PRQA S 3081
			
			}

			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) ;
				ACS_LOG_TRYCATCHNOTIFY_EX( exIOException("Cannot remove directory: '" + f + "' ", err)); 
			ACS_CLASS_END_DEBUG ;

			return false;
		}

	}

	return true ;
}


bool File::remove(const string &f) // PRQA S 4020
{
	if ( File::exists(f) && !File::isDir(f) )
	{
		if ( 0 != ::unlink(f.c_str()) )
		{
			int err = errno;
			ACS_THROW(exIOException("It couldn't remove entry: '" + f + "' ", err)) ;	 // PRQA S 3081
		}
		return true ;
	}
	else
	{
		return File::unlink(f) ; // EC:: Preferred
	}
}


void File::touch(const string &f) 
{
	if (exists(f)) {
		if (0 != ::utime(f.c_str(),0)) {
			int err = errno;	
			ACS_THROW(exIOException("Cannot touch \"" + f +"\"" ,err)) ;	 // PRQA S 3081
		}
	}
	else {
		FDGuard fdguard(open(f.c_str(), O_CREAT | O_WRONLY, 0666)) ;	// file doesn't exist: open
		if (fdguard < 0) { // PRQA S 3050
			int err = errno;
			ACS_THROW(exIOException("Cannot create \"" + f +"\"" ,err)) ;	 // PRQA S 3081
		}
	}
}


void File::touch(const string &f, time_t amtime) 	// Force touch to amtime (secs from EPOCH). 
{
	struct utimbuf utimbuf_struct ; // PRQA S 4102
	
	utimbuf_struct.actime = utimbuf_struct.modtime = amtime ;
	
	// Create (touch) file if doesn't exist
	if (!exists(f))
	{
		FDGuard fdguard(open(f.c_str(), O_CREAT | O_WRONLY, 0666)) ;
		if (fdguard < 0) { // PRQA S 3050
			int err = errno;	
			ACS_THROW(exIOException("Cannot create \"" + f +"\"" ,err)) ;	 // PRQA S 3081
		}
	}
	
	// COMMAND BELOW WILL ONLY SUCCEED IF THE PROCESS HAS THE SAME ID AS FILE OWNER.
	// OTHERWISE, ONLY THE ::utime WITH POINTER TO 0 (ACC. AND MOD. TIME SET TO NOW)
	// WOULD BE ALLOWED (SEE File::touch(const string &f) )
	if (0 != ::utime(f.c_str(), &utimbuf_struct)) {
		int err = errno;	
		ACS_THROW(exIOException("Cannot touch/set a new time to file: \"" + f +"\"" ,err)) ;	 // PRQA S 3081
	}
}


// fileOut == file1 + file2
void File::concat(const string &file1, const string &file2, const string & fileOut)
{
	const off_t bufferSize=65536;
	char buffer[bufferSize];

	ofstream streamOut ;
	ifstream stream2 ;
	if (file1 == fileOut) { // append requested
		ACS_COND_THROW(file2 == fileOut,exIOException("Cannot append over the same file: " + fileOut)); // PRQA S 3081
		streamOut.open(file1.c_str(), ios::out | ios::app | ios::ate) ;
		int err = errno;
		ACS_COND_THROW(!streamOut,exIOException("Error while opening file \"" + file1 + "\" .",err)) ;  // PRQA S 3081

		stream2.open(file2.c_str() );
		int err2 = errno;
		ACS_COND_THROW(!stream2,exIOException("Error while opening file \"" + file2 + "\" .",err2)) ;  // PRQA S 3081
	}
	else {	
	
		// Check if target file already exists
		ACS_COND_THROW(File::exists(fileOut),exIOException("File \"" + fileOut + "\" already exists .")) ;  // PRQA S 3081

		ACS_COND_THROW(file2 == fileOut,exIOException("Input file and output file must have different path")); // PRQA S 3081

		// Open file1
		ifstream stream1(file1.c_str() );
		int err = errno;
		ACS_COND_THROW(!stream1,exIOException("Error while opening file \"" + file1 + "\" .",err)) ;  // PRQA S 3081

		// Open file2
		stream2.open(file2.c_str(), ios::in );
		int err2 = errno;
		ACS_COND_THROW(!stream2,exIOException("Error while opening file \"" + file2 + "\" .",err2)) ;  // PRQA S 3081

		// Open fileOut
		streamOut.open(fileOut.c_str(), ios::out);
		int err3 = errno;
		ACS_COND_THROW(!streamOut,exIOException("Error while creating file \"" + fileOut + "\" .",err3)) ;  // PRQA S 3081

		off_t n=bufferSize;
		while(n== bufferSize)
		{
			stream1.read(buffer, bufferSize);
			n=stream1.gcount();
			streamOut.write(buffer , n);
		}
		stream1.close();
	}
	
	streamOut.seekp(0, ios_base::end) ; // position at the end 
	off_t n=bufferSize;
	while(n== bufferSize)
	{
		stream2.read(buffer, bufferSize);
		n=stream2.gcount();
		streamOut.write(buffer , n);
	}
	stream2.close() ;
	streamOut.close () ;
	
}


/*
	If the file on the top is a symlink, both the link _and_ the actual file will be renamed.
*/
void
		File::mergeMatchExtRenameLink(	const string& file1, const string& file2, string &fileOut, 
                                        const off_t& file1Size, const off_t& file2Size, const string& extS, const string& ext1, 
                                        const string& ext2, bool handleSingleExt, 
                                        bool file1OnTheBottom)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called: file1: " << file1 << " file2: " 
        << file2 << " extS: " << extS << " ext1: " << ext1 << " ext2: " << ext2
        << " handleSingleExt: " << boolalpha << handleSingleExt << " file1OnTheBottom: " << boolalpha << file1OnTheBottom) ;


    mergeMatchExt(file1, file2, fileOut, file1Size, file2Size, extS, ext1, ext2, handleSingleExt, file1OnTheBottom) ;
   
   	
    if ( File::isSymbolicLink(fileOut) )
    {
        string origFile = File::readlink(fileOut) ;	/* Get the actal FileName*/

        string origFileDir = File::getDirPath(origFile) ;	/* Get the actual directory */

        string linkName = File::getFileName(fileOut) ;	/* Get the link name */

        string newName = origFileDir + "/" + linkName ;	/* Set the new actual name */ 


        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing symlink: \"" << fileOut << "\"") ;
        File::remove (fileOut) ; /* remove sym link ... since original file will be renamed */
     
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming: \"" << origFile << "\" to \"" << newName << "\"" ) ;
        File::rename(origFile, newName );	/* Rename actual file */

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creating a link from: \"" << newName << "\" to \"" << fileOut << "\"") ; 
        File::symlink(newName, fileOut) ;   /* sym link is re-created */
    }
}


/*
    Alias: just call concatMatchExt w/ overwriteSource always TRUE.
    That is: the file on the top is RENAMED as destination, and removed.
*/
void
		File::mergeMatchExt(	const string& file1, const string& file2, string &fileOut, 
							    const off_t& file1Size, const off_t& file2Size, const string& extS, const string& ext1, 
                                const string& ext2, bool handleSingleExt, 
                                bool file1OnTheBottom)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called:  file1: " << file1 << " file2: " 
        << file2 << " extS: " << extS << " ext1: " << ext1 << " ext2: " << ext2
        << " handleSingleExt: " << boolalpha << handleSingleExt << " file1OnTheBottom: " << boolalpha << file1OnTheBottom) ; 

	// Call method concatMatchExt:
	const bool overwriteSource = true ;
    return concatMatchExt(	file1, file2, fileOut, file1Size, file2Size, extS, ext1, ext2, 
                            handleSingleExt, file1OnTheBottom, overwriteSource) ;
}


void
		File::concatMatchExt(	const string& file1, // PRQA S 5500, 5510, 5520
                                const string& file2, 
                                string &fileOut, 
							    const off_t& file1Size, 
                                const off_t& file2Size, 
                                const string& extS, 
                                const string& ext1, 
                                const string& ext2, 
                                bool handleSingleExt, 
                                bool file1OnTheBottom,
                                bool overwriteSource)	/* File on the top is REMOVED */
{
// EC: this method has multiple returns to keep its complexity as low as possible
// EC: the method can be divided into sections
 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called:  file1: " << file1 << " file2: " 
        << file2 << " extS: " << extS << " ext1: " << ext1 << " ext2: " << ext2
        << " handleSingleExt: " << boolalpha << handleSingleExt << " file1OnTheBottom: " 
        << boolalpha << file1OnTheBottom << " overwriteSource: " << boolalpha << overwriteSource) ;
		
	// Check if at least one file provided
    ACS_COND_THROW( ( file1.empty() && file2.empty() ),  // PRQA S 3081
		exIOException("File::concatMatchExt  No file name provided.") ); 
	
	// Check if provided file(s) exist(s)
    ACS_COND_THROW( ( not file1.empty() && !File::exists(file1) ),  // PRQA S 3081
		exIOException("File::concatMatchExt  Input File #1 \"" + file1 + "\" not Found.")) ; 
    ACS_COND_THROW( ( not file2.empty() && !File::exists(file2) ),  // PRQA S 3081
		exIOException("File::concatMatchExt  Input File #2 \"" + file2 + "\" not Found.")) ; 

    string newPath ;
    string newName ;
	fileOut = "" ;	// Reset output

	if ( file1.empty() || file2.empty() )
    {
    	// IF HERE: ONLY ONE INPUT PROVIDED. DO NOT MERGE, JUST RENAME
       	
        string foundFile ;

	    if (not file1.empty())
        {    
			// Check size of file1
            ACS_COND_THROW( ( File::size(file1) != file1Size ),  // PRQA S 3081
				exIOException("File::concatMatchExt  Input File #1 \"" + file1 + "\" has an invalid size.")) ; 
			// Check size of file2 (it shouldn't exist)
            ACS_COND_THROW( 0 != file2Size,  // PRQA S 3081
				exIOException("File::concatMatchExt  Input File #2 \"" + file2 + "\" has an invalid size.")) ; 

            foundFile = file1 ;
            newName = (not ext1.empty() && (getExt(file1) == ext1))? File::removeExt(file1): file1 ; // PRQA S 3380, 3385
        }
        else
        {      
			// Check size of file1 (it shouldn't exist)
            ACS_COND_THROW( 0 != file1Size,  // PRQA S 3081
				exIOException("File::concatMatchExt  Input File #1 \"" + file1 + "\" has an invalid size.")) ; 
			// Check size of file2
            ACS_COND_THROW( ( File::size(file2) != file2Size ),  // PRQA S 3081
				exIOException("File::concatMatchExt  Input File #2 \"" + file2 + "\" has an invalid size.")) ; 

            foundFile = file2 ;
            newName = (not ext2.empty() && (getExt(file2) == ext2))? File::removeExt(file2): file2 ; // PRQA S 3380, 3385
		}

        // Extension will be added only if it has to be managed and doesn't ALREADY match the provided one
	    if ( handleSingleExt && not extS.empty() && (getExt(foundFile) != extS) ) {
	        fileOut = fileWExt(newName, extS) ;
        }
        else {
            fileOut = newName ;	// Nothing to do, since the file already has the right estension
        }
    
    
    	if (fileOut != foundFile)
    	{
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Copying file: \"" << foundFile << "\" as: \"" << fileOut << "\"" ) ;
        
        	unlink(fileOut) ;
            File::copy(foundFile, fileOut) ;
        }
        else
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input file: \"" << foundFile << "\" NOTHING TO DO! ") ;
        }
    
    }
    else if ( !isDir(file1) && !isDir(file2) ) 
    {
    	//IF HERE: MERGE THE TWO FILES
        
        ACS_COND_THROW( ( File::size(file1) != file1Size ),  // PRQA S 3081
			exIOException("File::concatMatchExt  Input File #1 \"" + file1 + "\" has an invalid size.")) ; 
        ACS_COND_THROW( ( File::size(file2) != file2Size ),  // PRQA S 3081
			exIOException("File::concatMatchExt  Input File #2 \"" + file2 + "\" has an invalid size.")) ; 


        // File1 has to be concatenated. So File2 will be assumed to get parameters 
        newPath = getDirPath(file2) ;

        if ( !ext2.empty() && (getExt(file2) == ext2) ) { 
            newName = removeExt(file2) ; 
        }
        else { newName = file2 ; }

        if ( handleSingleExt && not extS.empty() ) { 
            fileOut = fileWExt(newName, extS) ; 
        }
        else { fileOut = newName ; }

		if (file1OnTheBottom)
        {      
			if (file1 != fileOut)
			{
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Concatenating file: \"" << file1 << "\"  (size: " << file1Size 
                    << ")at the bottom of file: \"" << file2 << "\". ") ;
            
                if ( overwriteSource && (file2 != fileOut) )
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Resulting file is: " << file2 ) ;

                    concat(file2, file1, file2) ;
                    
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming: \"" << file2 << "\" as: \"" << fileOut << "\"" ) ;
                    unlink(fileOut); File::rename(file2, fileOut) ;
                }
                else
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Resulting file is: " << fileOut) ;

                    if (file2 != fileOut) { unlink(fileOut); }
                    concat(file2, file1, fileOut) ;
                }
           
            }
            else
            {
            	// file1 == fileOut
                string tmpFile = File::tempname( newPath + "/File" ) ;	/* Guard is automatic */ // PRQA S 3050

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Concatenating file: \"" << file1 << "\"  (size: " << file1Size 
                    << ")at the bottom of file: \"" << file2 << "\". Resulting file is: " << tmpFile) ;
            
                concat(file2, file1, tmpFile) ;   
                
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming: \"" << tmpFile << "\" as \"" << file1 << "\"" ) ;

                if (File::exists(tmpFile)) { unlink(file1); File::rename(tmpFile, file1) ; }
            }  // END: if (file1 != fileOut)  
                
        }
        else
        {
            // file1OnTheBottom is false -> so file1OnTheTop
        
        	if (file2 != fileOut) 
            {

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Concatenating file: \"" << file2 << "\"  (size: " << file1Size 
                    << ")at the bottom of file: \"" << file1 << "\". ") ;	/* Unfinished */

        
               	if ( overwriteSource && (file1 != fileOut) )
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Resulting file is: " << file1) ;
                
                    concat(file1, file2, file1) ;     
                    
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming: \"" << file1 << "\" as: \"" << fileOut << "\"") ;
                    unlink(fileOut); File::rename(file1, fileOut) ;
                }
                else
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Resulting file is: " << fileOut ) ;
              
                    if (file1 != fileOut) { unlink(fileOut);  }
                    concat(file1, file2, fileOut) ;
                }
        
        	}
            else
            {
                // file2 == fileOut
                string tmpFile = File::tempname( newPath + "/File" ) ;	/* Guard is automatic */ // PRQA S 3050
            
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Concatenating file: \"" << file1 << "\"  (size: " << file1Size 
                    << ")at the top of file: \"" << file2 << "\". Resulting file is: " << tmpFile) ;

                concat(file1, file2, tmpFile) ;
                
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming: \"" << tmpFile << "\" as \"" << file2 << "\"" ) ;

                if (File::exists(tmpFile)) { unlink(file2); File::rename(tmpFile, file2) ; }
            }	// END: if (file2 != fileOut) 
        }	// END: if (file1OnTheBottom)
    }
	else
    	ACS_THROW( exIOException("File::concatMatchExt. Cannot merge directories !")) ;  // PRQA S 3081
	;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fileOut: \"" << fileOut << "\"  fileOut Size: " << File::size(fileOut)
        << " file1Size: " << file1Size << " file2Size: " << file2Size) ;

	// Final check on size:
    ACS_COND_THROW( (File::size(fileOut) != (file1Size+file2Size)), exIOException("File::concatMatchExt. Invalid size of output file.")) ;  // PRQA S 3081

	return ; 
}


void File::evalNamesSplitMatchExt(	const string &fileIn, 
                                    string& file1, 
                                    string& file2,                            
                                    const string& extS, const string& ext1, const string& ext2, 
                                    bool handleSingleExt,
                                    bool splitInput)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. fileIn: " << fileIn 
        << " extS: " << extS << " ext1: " << ext1 << " ext2: " << ext2
        << " handleSingleExt: " << boolalpha << handleSingleExt) ;

	file1 = "" ; file2 = "" ;	// Reset output

	string newName ;
    string newPath = getDirPath(fileIn) ;
	
    // Extension will be removed only if it has to be managed and matches the provided one
	if ( handleSingleExt && not extS.empty() && (getExt(fileIn) == extS) ) {
	    newName = removeExt(fileIn) ;
    } else {
        newName = fileIn ;
	}
   
   
	if (splitInput)
    {

	    // Create new filenames 
	    file1 = (ext1 != "")? (newName + "." + ext1): newName ; // PRQA S 3380, 3385 2
	    file2 = (ext2 != "")? (newName + "." + ext2): newName ;

        // ACS_COND_THROW( (file1==file2),  // PRQA S 3081
		//	exIOException("Internal error. Obtained the same names for File1 and File2: \"" + file1 + "\"")) ; 
	}
    else
    {
        // Create new filename 
	    // file1 = (extS != "")? fileWExt(newName, extS): newName ;
	
		if ( (handleSingleExt) && (extS != "") )
		{
			file1 = (newName+"."+extS) ; 
		}
		else
		{
			file1 = newName ; 
		}
		
    }

     
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. file1: \"" 
        << file1 << "\" file2: \"" << file2 << "\"") ;

}


void File::splitMatchExt(	const string &fileIn, 
                            string& file1, 
                            string& file2,                            
                            off_t file1Size, 
                            off_t file2Size, 
                            const string& extS, 
                            const string& ext1, 
                            const string& ext2, 
                            bool handleSingleExt, 
                            bool file1OnTheBottom,
                            bool overwriteSingleFile,
                            bool checkSize)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Calling splitMatchExtHandleTrailer with empty pattern") ;

	return splitMatchExtHandleTrailer(fileIn, file1, file2, file1Size, file2Size, extS, ext1, ext2, "", handleSingleExt, file1OnTheBottom, overwriteSingleFile,checkSize) ;
}


void File::splitMatchExtHandleTrailer(	const string &fileInWTrailer, // PRQA S 5500, 5510
                            			string& file1, 
                            			string& file2,                            
                            			off_t file1Size, 
                            			off_t file2Size, 
                            			const string& extS, 
                                        const string& ext1, 
                                        const string& ext2, 
										const string& trailerPattern,
                            			bool handleSingleExt, 
                            			bool file1OnTheBottom,
                            			bool overwriteSingleFile,
                                        bool checkSize)
{
// EC: this method has multiple returns to keep its complexity as low as possible
// EC: file is easy to be read, it's divided into sections


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. fileInWTrailer: " << fileInWTrailer 
        << " file1Size: " << file1Size << " file2Size: " << file2Size
        << " extS: " << extS << " ext1: " << ext1 << " ext2: " << ext2
        << " handleSingleExt: " << boolalpha << handleSingleExt << " file1OnTheBottom: " 
        << boolalpha << file1OnTheBottom << " overwriteSingleFile: " << boolalpha << overwriteSingleFile) ;
		
	// Check inputs
    ACS_COND_THROW( (file1Size < 0), exIOException("File::splitMatchExtHandleTrailer Invalid (negative) file1 size.")) ;     // PRQA S 3081
    ACS_COND_THROW( (file2Size < 0), exIOException("File::splitMatchExtHandleTrailer Invalid (negative) file2 size.")) ;  // PRQA S 3081
    ACS_COND_THROW( ( fileInWTrailer.empty() || !File::exists(fileInWTrailer) ), // PRQA S 3081
		exIOException("File::splitMatchExtHandleTrailer  Input File \"" + fileInWTrailer + "\" not Found.")) ; 
    ACS_COND_THROW( checkSize && ( File::size(fileInWTrailer) != (file1Size+file2Size) ),  // PRQA S 3081
		exIOException("File::splitMatchExtHandleTrailer  Input File \"" + fileInWTrailer + "\" doesn't match expected size.")) ; 

	file1 = "" ; 
    file2 = "" ;	// Reset output

	string newName ;
    string fileIn = fileInWTrailer ;
    string trailer ;
	string newPath = getDirPath(fileInWTrailer) ;
    
	splitTrailer(fileInWTrailer, trailerPattern, fileIn, trailer) ;
	if ( not trailerPattern.empty() )  { trailer = trailerPattern + trailer ;	}/* since trailer doesn't begin with the pattern */
		 

	
    // Extension will be removed only if it has to be managed and matches the provided one
	if ( handleSingleExt && not extS.empty() && (getExt(fileIn) == extS) ) {
	    newName = removeExt(fileIn) ;
    }
    else {
        newName = fileIn ;
    }
	
    if ( (file1Size > 0) && (file2Size > 0) && !isDir(fileInWTrailer) )
	{
		// IF HERE: SPLIT FILES

	    // Create new filenames 
	    file1 = (ext1 != "")? (newName + "." + ext1 + trailer): (newName + trailer); // PRQA S 3380, 3385 2
	    file2 = (ext2 != "")? (newName + "." + ext2 + trailer): (newName + trailer);

        ACS_COND_THROW( (file1==file2), exIOException("Internal error. Obtained the same names for File1 and File2: \"" + file1 + "\"")) ;  // PRQA S 3081


	    // Create temporary filenames (to easily handle contingencies where file1 or 2 matches fileInWTrailer)
	    string tmpFile1 = File::tempname( newPath + "/File1" ) ;   /* Guard is automatic */ // PRQA S 3050 2
        string tmpFile2 = File::tempname( newPath + "/File2" ) ;   /* Guard is automatic */


        if (file1OnTheBottom)
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Splitting file \"" << fileInWTrailer << "\"  as: \"" << tmpFile1 << "\" and \""
                << tmpFile2 << "\". File1 is on the bottom (size: " << file1Size << ")") ;

            split(fileInWTrailer, tmpFile2, tmpFile1, file1Size, false) ;
        }
	    else
	    { 
    	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Splitting file \"" << fileInWTrailer << "\"  as: \"" << tmpFile1 << "\" and \""
                << tmpFile2 << "\". File1 is on the top (size: " << file1Size << ")") ;

            split(fileInWTrailer, tmpFile1, tmpFile2, file1Size, true) ;
        }


        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming file \"" << tmpFile1 << "\"  as: \"" << file1 << "\" and \""
            << tmpFile2 << "\"  as: \"" << file2 << "\"") ;


        unlink(file1); if (File::exists(tmpFile1))
        {
            File::rename(tmpFile1, file1) ;
            ACS_COND_THROW( (File::size(file1) != file1Size), exIOException("Internal error. Output file1 has an invalid size.")) ;  // PRQA S 3081
        }
        
        unlink(file2); if (File::exists(tmpFile2)) 
        {
            File::rename(tmpFile2, file2) ;
            ACS_COND_THROW( (File::size(file2) != file2Size), exIOException("Internal error. Output file2 has an invalid size.")) ;  // PRQA S 3081
        }
    }
    else if ( ((0 == file1Size) || (0 == file2Size)) && (handleSingleExt && not extS.empty()) )
	{	
    	// IF HERE: DON'T SPLIT, JUST COPY FILES.
        
        // Create new filename 
        file1 = newName + "." + extS + trailer;
        
		if (file1 != fileInWTrailer)
        {
            
			if (overwriteSingleFile)
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming file \"" << fileInWTrailer << "\"  as: \"" << file1 ) ;

          		File::rename(fileInWTrailer, file1) ;
            }
			else
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Copying file \"" << fileInWTrailer << "\"  as: \"" << file1 ) ;
 
        	    unlink(file1) ;
                File::copy(fileInWTrailer, file1) ;

                ACS_COND_THROW( (File::size(file1) != (file1Size+file2Size) ),  // PRQA S 3081
					exIOException("Internal error. Output file has an invalid size.")) ; 
			}
        }
        else
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input file: \"" << fileInWTrailer << "\"  Nothing to do! [1]" ) ;
        }
        
    }
	else
    {
        // IF HERE: DON'T SPLIT, NO NEED TO COPY/RENAME.

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input file: \"" << fileInWTrailer << "\"  Nothing to do! [2]") ;

    	file1 = fileInWTrailer ;
    }
   
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. file1: \"" 
        << file1 << "\" file2: \"" << file2 << "\"") ;

}

 
void File::split(const string &fileIn, const string &file1, const string & file2, off_t fileSize, bool sizeFile1)
{
// EC: this method has multiple returns to keep its complexity as low as possible

	// Check files existence
	ACS_COND_THROW(File::exists(file1),exIOException("File \"" + file1 + "\" already exists .")) ;  // PRQA S 3081

	ACS_COND_THROW(File::exists(file2),exIOException("File \"" + file2 + "\" already exists .")) ;  // PRQA S 3081

	// Check if filenames differ
	if( fileIn == file1  || fileIn == file2 || file1 == file2)
	{
		ACS_THROW(exIOException("Files must have different path")); // PRQA S 3081
	}
	off_t FileInSize= File::size(fileIn) ;
	
	if(fileSize >  FileInSize)
	{
		ostringstream err;
		err << "File \"" << fileIn << "\" size is " << FileInSize << " requested split size is " << fileSize << "\n";
		ACS_THROW(exIOException(err.str())); // PRQA S 3081
	}
	
	off_t sizeFirst = 0;	// evaluate size of required output file1
	if(sizeFile1) {
		sizeFirst=fileSize; 
    }
	else {
		sizeFirst=FileInSize - fileSize; 
    }
	

	ifstream streamIn(fileIn.c_str() );	// open input file
	int err = errno;
	ACS_COND_THROW( !streamIn,exIOException("Error while opening file \"" + fileIn + "\" .",err)); // PRQA S 3081

	ofstream stream1(file1.c_str() );	// open first output file
	int err2 = errno;
	ACS_COND_THROW( !stream1,exIOException("Error while creating file \"" + file1 + "\" .",err2)) ; // PRQA S 3081
	
	ofstream stream2(file2.c_str() );	// open second output file
	int err3 = errno;
	ACS_COND_THROW( !stream2,exIOException("Error while creating file \"" + file2 + "\" .",err3)) ; // PRQA S 3081
	
	const off_t bufferSize=65536;
	char buffer[bufferSize];

	off_t bytesToRead=sizeFirst;	// set amount of bytes to transfer to file1	
	off_t n = 0 ;
	while(bytesToRead > 0)
	{
		if(bytesToRead > bufferSize) {
			n=bufferSize;
        }
		else {
			n=bytesToRead;
        }
		streamIn.read(buffer, n);
		stream1.write(buffer , n);
		bytesToRead-=n;
	}
	stream1.close();

	n=bufferSize;
	while(n == bufferSize)
	{
		streamIn.read(buffer, bufferSize);
		n=streamIn.gcount();
		stream2.write(buffer , n);
	}
}




File::CompressionMode File::compressionMode(const string &name) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	string result = file(name)  ;
	
	if (result.find("compress'd data") != string::npos) {
		return Compressed ;
    }
	else if (result.find("gzip compressed data") != string::npos) {
		return GZipped ;
    }
	else if (result.find("bzip2 compressed data") != string::npos) {
		return BZipped ;
    }
	else if (result.find("tar archive") != string::npos) {
		return TARred ;
    }
    else {
	    return Plain ;
    }
}


bool File::readOnly(const string &path) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file \"" << path << "\"") ;
	return ::access(path.c_str(),W_OK) != 0 ;	// check access flags
}

bool File::exists(const string &path) 
{
	return ::access(path.c_str(),F_OK) == 0 ;	// check access flags
}

bool File::isExecutable(const string &path) 
{
#ifdef __linux__
	return ::access(path.c_str(),X_OK) == 0 ;	// check access flags
#else	
	return ::access(path.c_str(),EX_OK) == 0 ;	// check access flags
#endif	
}


bool File::isReadable(const string &path) 
{
	return ::access(path.c_str(),R_OK) == 0 ;	// check access flags
}


bool File::isDir(const string &path) 
{
	struct stat s; // PRQA S 4102
    fileStat(path, s) ;
		
	return S_ISDIR(s.st_mode) ;
}


bool File::isRegularFile(const string &path) 
{
	struct stat s; // PRQA S 4102
    fileStat(path, s) ;

	return S_ISREG(s.st_mode) ;
}


bool File::isSymbolicLink(  const string &path, // PRQA S 4020
                            bool recursive)         /* Recursively follow symlinks */
{
// EC: this method has multiple returns to keep its complexity as low as possible

    string trimmedPath = trimTrailingSlashes(path) ;

	if (!recursive)
	{
		struct stat s; // PRQA S 4102
		if( ::lstat(trimmedPath.c_str() , &s) == -1 )
		{
			int err = errno;
			ACS_THROW(exFileOpenException("Cannot lstat  \"" + path +"\"",err)) ; // PRQA S 3081
		}

		return S_ISLNK(s.st_mode) ;
	}
	else
	{
		// Recursive: the readlinkRec method will be used instead
		bool isSymlink = false ;
		readlinkRec(trimmedPath, "", isSymlink) ;
		return isSymlink ;
	}
}

void File::fileStat(const string &path, struct stat &s, bool resolveLinks) 
{
	if (resolveLinks)
	{
		ACS_COND_THROW( 0 != stat(path.c_str(), &s), exFileOpenException("Cannot stat  \"" + path +"\"",errno)) ; // PRQA S 3081
	}
	else
	{
		ACS_COND_THROW( 0 != lstat(path.c_str(), &s), exFileOpenException("Cannot lstat  \"" + path +"\"",errno)) ; // PRQA S 3081
	}
}


off_t File::size(const string &path, bool forceStat) 
{
	struct stat s; // PRQA S 4102
	
    fileStat(path, s) ;

	off_t r = s.st_size ;
	
	if (!forceStat && S_ISDIR(s.st_mode))
	{ 
		const bool showHidden = true; 
#warning MP this is inefficient: a recursive function would do the job in more efficient way
		DirTree d(path, showHidden) ;
		r = d.bytes() ;
	}

	return r ;
}


#ifdef __linux__

blkcnt_t File::blocks512(const string &path) 
{
	struct stat s; // PRQA S 4102
    fileStat(path, s) ;
	return s.st_blocks ;
}
#endif


void File::chown(const std::string &path, uid_t owner, gid_t group) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: " << path << " owner: " << owner << " group: " << group) ;
    int r = ::chown(path.c_str(), owner, group) ;
    if (r != 0) {
	int err = errno ;
        ostringstream os ;
        os << "Cannot chown(" << path << "," << owner << "," << group << ")" ;
        ACS_THROW(exIOException (os.str(),err)) ; // PRQA S 3081
    }
}

void File::chown(const std::string &path, const std::string &owner, const std::string &group) 
{
    try {
        File::chown(path, getUserID(owner), getGroupID(group)) ;
    } catch(exception &x) {
        ACS_THROW(exIOException(x, "chown(" + path + "," + owner + "," + group + ")")) ; // PRQA S 3081
    }
}


void File::chown_rec(const std::string &path_in, const std::string &owner, const std::string &group, bool dirs_only) 
{
    try {
        File::chown_rec(path_in,getUserID(owner), getGroupID(group), dirs_only) ;
    }
    catch(exception &x) {
        ACS_THROW(exIOException(x, "chown_rec(" + path_in + "," + owner + "," + group + ")")) ; // PRQA S 3081
    }
}

void File::chown_rec(const string &path_in, uid_t owner, gid_t group, bool dirs_only)
{	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		const string m = (dirs_only ? "only to directories" : "to files and directories") ; // PRQA S 3384
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for path \"" << path_in << "\" owner: " << owner << " group: " << group 
				<< "  Applies " << m ) ;
	ACS_CLASS_END_DEBUG
	
	ACS_COND_THROW(!File::exists(path_in),exIOException("Path \"" + path_in + "\" do not exists .")) ;  // PRQA S 3081

	char fsName_resolved[16*1024] ; // PRQA S 4403
	const string path = ( (realpath(path_in.c_str(), fsName_resolved)!=0 ) ? fsName_resolved : path_in ) ; // PRQA S 3384, 3385
     
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called.  After realpath: \"" << path << "\"") ;
	
	if (! isDir(path))
	{
		File::chown(path, owner, group) ;
	}
	else
	{
		const bool show_hiddden = true ;
		const string dir_entry = "" ; 
		DirTree d (path, dir_entry, show_hiddden) ;

		if (dirs_only) { d.hide_files(); } else { d.show_files() ; }
		size_t d_size = d.size() ;
		for (size_t i=0; i<d_size; ++i)
		{
			try
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to apply chown to entry: \"" 
						<< d[i] << "\"") ;

				File::chown(d[i], owner, group) ;

				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Successfully applied chown to entry: \"" 
						<< d[i] << "\"") ;
			}
			catch(exception &e)
			{
				ACS_LOG_ERROR(SimpleDebugSignature << "Problem applying chown to entry: \"" 
					<< d[i] << "\" " << e) ;
			}
		}
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished") ;
}


void File::chmod_rec(const string &path_in, mode_t mode, PermissionSet perm, bool dirs_only)
{	
	char prefix = '\0'; 
    switch (perm)
	{
    case AddPerm:
		{	prefix = '+'; break ; }
    case SubPerm:
		{	prefix = '-'; break ; }
    default:
		{	prefix = '\0'; break ; }
	} ;

	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		const string m = (dirs_only ? "only to directories" : "to files and directories") ; // PRQA S 3384
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for path \"" << path_in << "\" mode: " << prefix << "0" << oct << mode << dec 
				<< "  Applies " << m ) ;
	ACS_CLASS_END_DEBUG
	
	ACS_COND_THROW(!File::exists(path_in),exIOException("Path \"" + path_in + "\" do not exists .")) ;  // PRQA S 3081

	char fsName_resolved[PATH_MAX] ; // PRQA S 4403
	const string path = ( (realpath(path_in.c_str(), fsName_resolved)!=0 ) ? fsName_resolved : path_in ) ; // PRQA S 3384, 3385
     
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called.  After realpath: \"" << path << "\"") ;
	
	if (! isDir(path))
	{
		File::chmod(path, mode, perm) ;
	}
	else
	{
		const bool show_hiddden = true ;
		const string dir_entry = "" ; 
		DirTree d (path, dir_entry, show_hiddden) ;

		if (dirs_only) { d.hide_files(); } else { d.show_files() ; }
		size_t d_size = d.size() ;
		for (size_t i=0; i<d_size; ++i)
		{
			try
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to apply chmod to entry: \"" 
						<< d[i] << "\"  mode: 0" << oct << mode) ;

				File::chmod(d[i], mode, perm) ;

				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Successfully applied chmod to entry: \"" 
						<< d[i] << "\"  mode: 0" << oct << mode) ;
			}
			catch(exception &e)
			{
				ACS_LOG_ERROR(SimpleDebugSignature << "Problem applying chmod to entry: \"" 
					<< d[i] << "\"  mode: 0" << oct << mode << " " << e) ;
			}
		}
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished") ;
}


void File::chmod(const string &path, mode_t mode, PermissionSet perm)
{
	char prefix = '\0'; 
    switch (perm)
	{
    case AddPerm:
		{	prefix = '+'; break ; }
    case SubPerm:
		{	prefix = '-'; break ; }
    default:
		{	prefix = '\0'; break ; }
	} ;

	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for file \"" << path << "\" mode: " << prefix << "0" << oct << mode) ;

	const mode_t current_mode = File::getmod(path) ;
	
	bool skip = false ;
	mode_t new_mode = mode ;
	
	switch (perm)
	{
    case AddPerm:
		{
			if ((current_mode & mode) == mode) {
				skip = true ;
			} else {
				skip = false ; 
				new_mode = current_mode | mode ; 
			}
			break ; 
		}
		
    case SubPerm:
		{
			if ((current_mode & mode) == 0x0) {
				skip = true ;
			} else {
				skip = false ; 
				new_mode = current_mode & (~mode) ;
			}
			break ; 
		}
		
    default:
		{
			if ((current_mode & 07777) == mode) { // PRQA S 4400
				skip = true ;
			} else {
				skip = false ; 
				new_mode = mode ; 
			}
			break ; 
		}
	}	// END: switch
	
	
	if (skip)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file \"" << path << "\" current_mode: 0" << oct << current_mode << dec << "  matched with the requested one: " 
				<< prefix << "0" << oct << mode) ;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file \"" << path << "\" current_mode: 0" << oct << current_mode << dec << "  setting new mode: 0"
				<< oct << new_mode) ;
		
		if( ::chmod(path.c_str() , new_mode) == -1 ) {	// set new mode
			int err = errno ;
			ostringstream os ;
			os << "Cannot chmod  \"" << path << "\" to [octal] mode: 0" << oct << mode ;
			ACS_THROW(exIOException (os.str(),err)) ; // PRQA S 3081
		}
	}	
}


mode_t  File::getmod(const string &path)
{
	ACS_COND_THROW(!File::exists(path),exIOException("File \"" + path + "\" do not exists .")) ;  // PRQA S 3081
	struct stat s; // PRQA S 4102
    fileStat(path, s) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file \"" << path << "\" mode: " << oct << s.st_mode) ;
	
	return s.st_mode ;
}


gid_t  File::getGroupIDFromPath(const string &path)
{
	ACS_COND_THROW(!File::exists(path),exIOException("Entry \"" + path + "\" do not exists ."));  // PRQA S 3081
	
	struct stat s; // PRQA S 4102
    fileStat(path, s);
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file \"" << path << "\" gid: " << s.st_gid );
	
	return s.st_gid ;
}


gid_t File::getGroupIDFromPath(const string &path, string& group_name)
{
	const gid_t gid = getGroupIDFromPath(path); 		/* Get gid from path */
	ostringstream gidstr;  
    gidstr << gid;
	try {
        using acs::os::Password ;
	    Password::group_w group_struct= Password::getGrGid(gid) ;	/* Get group-structure from gid */

	    group_name = group_struct.gr_name;		/* Read group-name from group-structure */

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "entry \"" << path << "\" gid: " << gidstr.str() << "  group_name: \"" << group_name << "\"" ) ; 

	    return gid;
    }
    catch(exception &x) {
	    ACS_THROW( exIOException(x, "Unable to get group name for entry \"" + path + "\"  gid: " + gidstr.str() )) ; // PRQA S 3081
    }
}


uid_t  File::getUserIDFromPath(const string &path)
{
	ACS_COND_THROW(!File::exists(path),exIOException("Entry \"" + path + "\" do not exists ."));  // PRQA S 3081
	
	struct stat s; // PRQA S 4102
    fileStat(path, s);
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file \"" << path << "\" uid: " << s.st_uid );
	
	return s.st_uid;
}

uid_t File::getUserIDFromPath(const string &path, string& user_name)
{
	const uid_t uid = getUserIDFromPath(path); 		/* Get gid from path */
	ostringstream uidstr;  
    uidstr << uid;
	try {
        using acs::os::Password ;
	    Password::passwd_w pwd_struct= Password::getPwUid(uid);	/* Get pwd-structure from uid */

	    user_name = pwd_struct.pw_name;		/* Read user_name from pwd-structure */

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "entry \"" << path << "\" uid: " << uidstr.str() << "  user_name: \"" << user_name << "\"" ); 

	    return uid;
    }
    catch(exception &x) {
	    ACS_THROW( exIOException(x, "Unable to get user name for entry \"" + path + "\"  uid: " + uidstr.str() )) ; // PRQA S 3081
    }
}

uid_t File::getUserID(const std::string& username) {

	uid_t uid;
	try {
		os::Password::passwd_w pwd = os::Password::getPwNam(username);
		uid = pwd.pw_uid;
	} catch(exception &x) {
		ACS_THROW(exIOException(x, "Unable to get user ID from user name " + username));
	}
	
	return uid;
}


gid_t File::getGroupID(const std::string& groupname) {
	gid_t gid;
	try {
		os::Password::group_w grp = os::Password::getGrNam(groupname);/* Try getting GID for groupname */
		gid = grp.gr_gid;
	} catch(exception &x) {
		ACS_THROW(exIOException(x, "Unable to get group ID from group name " + groupname));
	}
	
	return gid;
}


void  File::setgroup(const std::string &path, const gid_t &gid)
{
	ACS_COND_THROW(!File::exists(path),exIOException("Entry \"" + path + "\" do not exists .")) ;  // PRQA S 3081
	  
	const uid_t same_owner = -1 ;	/* DO NOT CHANGE */ // PRQA S 2890
	ostringstream gidstr ;  gidstr << gid ;
    try {
	    File::chown(path.c_str(), same_owner, gid);
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file \"" << path << "\" gid: " <<gid ) ;
    }catch(exception &x) {
	    ACS_THROW(exIOException(x,"Cannot change group to gid: " + gidstr.str() + "  for entry: \"" + path +"\"")) ; // PRQA S 3081
	}
}


void  File::setgroup(const std::string &path, const string& group_name)
{
    using acs::os::Password ;
	try {
	    Password::group_w group_struct = Password::getGrNam( group_name );	/* Get group-structure from group_name */
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "about to set gid: " << group_struct.gr_gid << "  (group_name: \"" << group_name << "\"  for entry: \"" << path ) ;  
	    setgroup(path, group_struct.gr_gid) ;
    }
    catch(exception &x) {
	    ACS_THROW(exIOException(x, "Unable to get gid for entry \"" + path + "\"  group_name: " + group_name)) ;  // PRQA S 3081
    }
}


void File::setgroup_rec(const string &path, const std::string& group_name)
{
    using acs::os::Password ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Method called for path \"" << path << "\" group_name: " << group_name ) ;
	try {
	    Password::group_w group_struct = Password::getGrNam( group_name.c_str() );	/* Get group-structure from group_name */
    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "about to recursively set gid: " << group_struct.gr_gid << "  (group_name: \"" << group_name << "\")  for entry: \"" << path ) ;

	    setgroup_rec(path, group_struct.gr_gid) ;
    }
    catch(exception &x) {
	    ACS_THROW(exIOException(x, "Unable to get gid for entry \"" + path + "\"  group_name: " + group_name)) ;  // PRQA S 3081
    }
}


void File::setgroup_rec(const string &path_in, const gid_t &gid)
{	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Method called for path \"" << path_in << "\" gid: " << gid ) ;
	
	ACS_COND_THROW(!File::exists(path_in),exIOException("Path \"" + path_in + "\" do not exists .")) ;  // PRQA S 3081

	char fsName_resolved[16*1024] ; // PRQA S 4403
	const string path = ( (realpath(path_in.c_str(), fsName_resolved)!=0 ) ? fsName_resolved : path_in ) ; // PRQA S 3384, 3385
     
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  After realpath: \"" << path << "\"") ;
	
	if (! isDir(path))
	{
		File::setgroup(path, gid) ;
	}
	else
	{
		const bool show_hiddden = true ;
		const string dir_entry = "" ; 
		DirTree d (path, dir_entry, show_hiddden) ;
		d.show_files() ;
		size_t d_size = d.size() ;
		for (size_t i=0; i<d_size; ++i)
		{
			try
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to apply setgrp to entry: \"" 
						<< d[i] << "\"  gid: " << gid) ;

				File::setgroup(d[i], gid) ;

				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Successfully applied setgrp to entry: \"" 
						<< d[i] << "\"  gid: " << gid ) ;
			}
			catch(exception &e)
			{
				ACS_LOG_ERROR(SimpleDebugSignatureThread << "Problem applying setgrp to entry: \"" 
					<< d[i] << "\"  gid: " << gid) ;
				ACS_LOG_NOTIFY_EX(e);
			}
		}
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished") ;
}


/****************************************************************************
**		check_access
****************************************************************************/
bool File::check_access(	const uid_t& applicant_uid, 
							const gid_t& applicant_gid, 
							const uint32_t& applicant_oper_in,  // bitwise-OR mask with R_OK, W_OK, X_OK
							const uid_t& resource_uid, 
							const gid_t& resource_gid, 
							const uint32_t& resource_mask_in)	// uint-32 mask. Printed in octal-base would appear like e.g. 0755
{
	/**
		An applicant (client interested to access a resource) identified by a uid/gid pair
		applies to a resource to perform an opearation.
		The resource has a specific permissions mask (for u/g/o) and of course must a resource uid/gid pair
		TRUE means that the applicant is allowed to perform the operations
	*/

	bool allowed (false) ;

	uint32_t operation(0x00), mask(0x00) ;
	if (applicant_oper_in & R_OK) { operation |= 04 ; }
	if (applicant_oper_in & W_OK) { operation |= 02 ; }
	if (applicant_oper_in & X_OK) { operation |= 01 ; }

	if (applicant_uid == resource_uid) 
	{
		// Match user. Compute the actual mask by extracting the user's permissions
		mask = ( (resource_mask_in & (0700)) >> (3+3) ) ; // AND-mask and shift the bits 8,7,6 (devoted to user's permissions)
	}
	else if (applicant_gid == resource_gid)  
	{
		// Match group. Compute the mask by extracting the group's permissions
		mask = ( (resource_mask_in & (0070)) >> 3 ) ; // AND-mask and shift the bits 5,4,3 (devoted to group's permissions)
	}
	else
	{	
		// No match. Compute the mask by extracting others' permissions
		mask = ( resource_mask_in & (0007) )  ; // AND-mask bits 2,1,0 (devoted to others' permissions)
	}

	allowed = ( ((operation & mask) == operation) ) ;

	return allowed ; 
}

FileGuard File::tempdir(const std::string& prefix, mode_t mode, PermissionSet perm) {
	char* ret = 0;
	char tmpl[4096]; // PRQA S 4403
	
	ostringstream tmpnameos;
	tmpnameos << prefix << "_" << ::pthread_self() << "_";
	
	strcpy(tmpl, tmpnameos.str().c_str());
	strcat(tmpl, "XXXXXX");
	ret = mkdtemp(tmpl);
	int err = errno;
	ACS_COND_THROW(0 == ret, exIOException(string("Cannot create ") + tmpl + " directory.", err));

	std::string tmpdir = tmpl;
	
	// By default the directory is created with permissions 0700. Change it if needed
	chmod(tmpdir, mode, perm);
	
	return FileGuard(tmpdir);
}

FileGuard File::tempname(const string &prefix, const string &ext)
{
	// In some case is impossible to keep the temp file existing on the File System
	// Enforce the name uniquity by using pthread_self as a signature for the temp name template

	ostringstream tmpnameos;
	tmpnameos << prefix << "_" << ::pthread_self() << "_";
	char tmpl[4096] ; // PRQA S 4403
	strcpy(tmpl, tmpnameos.str().c_str()) ;
	strcat(tmpl, "XXXXXX") ;
	int fd = mkstemp(tmpl);
	int err = errno;
	ACS_COND_THROW(fd < 0,exIOException(string("Cannot create ") + tmpl + " file.", err)) ; // PRQA S 3081
	
	string ret(tmpl) ;
	if ( 0 != ::close(fd) )
	{
		int errC = errno;
		ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errC));
	}
	
	/* if an extension has been passed to this method then it needs to be appended to the
	name of the temporary file. Also it is necessary to unlink and delete the file without the
	extension that has been built by mkstemp because it has no extension.*/
	if (!ext.empty()) 
	{
	
		File::unlink(ret);  //unlink the file without the extension
		ret += ("." + ext) ; //return a file name with the specified extension
		File::touch(ret) ;  //build this file
	}

	return FileGuard(ret) ; // PRQA S 3081
}

void File::truncate(const string &path, off_t size) 
{
	File::touch(path) ;
	ACS_COND_THROW(0 != ::truncate(path.c_str(),size), // PRQA S 3081
			exIOException("Cannot truncate " + path + " file.", errno)) ;
}

void File::chdir(const string &path) 
{
	if(::chdir(path.c_str()) == -1 )	// change directory
	{
		int err = errno;
		ACS_THROW(exIOException("Cannot change to directory  \"" + path +"\"",err)) ; // PRQA S 3081
	}
}

void File::mkdir(	const string &path, 
					bool allowExistingDir)	/* If TRUE: No error is reported if directory already exists */
{
	if(::mkdir(path.c_str() , 0777) == -1 )	// Create directory (must not already exist) // PRQA S 4400
	{
    	/* Got error (errno should be properly set) */
    	bool raiseException = false ;
    	
	int err = errno;
    	switch (err)
        {
        case EEXIST :
		    {
				/* Directory already exists */
                raiseException = (!allowExistingDir) ;
                break ;
            }
            
        default:
            {
        	    raiseException = true ;
            }
		
        } /* switch */
        
    	ACS_COND_THROW(raiseException,exIOException("Cannot create directory  \"" + path +"\"",err)) ; // PRQA S 3081
    }
}


void File::mkdirtree(const string &path_in)
{
	/* works like mkdir -p */
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method called for path: \"" << path_in << "\"") ; 

	const bool removeLeadingSlash = false ;
	const bool removeLeadingDot = false ;
	
	vector<string> vSoFar ;	
	vector<string> v = StringUtils::pathDisassemble(path_in, removeLeadingSlash, removeLeadingDot) ;	// disassemble path as vector

    size_t v_size = v.size();
	for (size_t i=0; i<v_size; ++i)
	{
		vSoFar.push_back(v[i]) ;
		if ((0 != i) || ((v[i]!="") && (v[i]!=".")))
		{
			// Not processing a fake entry, like "" (that is "/") or "."
			string pathSoFar = StringUtils::pathAssemble(vSoFar) ;
		
			if (!( File::exists(pathSoFar) && File::isDir(pathSoFar)))
			{
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "creating directory: \"" << pathSoFar << "\"") ;

				// Either directory doesn't exist, or exists and it's a file: in this case, a proper error will be issued by mkdir
				File::mkdir(pathSoFar, true) ;
			}
			else
			{
				// DO NOTHING :
				
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "directory already exists: \"" << pathSoFar << "\"") ;
			}
		}	// END: if (i || ((v[i]!="") && (v[i]!=".")))
	
	}	// END: for

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method finished. Path: \"" << path_in << "\" created") ; 
}


void File::rmdir(const string &path) 
{
	if(::rmdir(path.c_str()) == -1 )	// remove (empty) directory
	{
		int err = errno;
		ACS_THROW(exIOException("Cannot remove directory  \"" + path +"\"",err)) ; // PRQA S 3081
	}
}

bool File::isOlder(const string &path, size_t seconds) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	struct stat s; // PRQA S 4102
	if( stat(path.c_str() , &s) == -1 )
	{
		int err = errno;
		if (!exists(path)) {
			return true ;
        	}
		
		ACS_THROW(exFileOpenException("Cannot stat  \"" + path +"\"",err)) ; // PRQA S 3081
	}
	time_t now = time(0) ;
	return static_cast<long long>(s.st_mtime) < (static_cast<long long>(now) - static_cast<long long>(seconds)) ; // PRQA S 3081
}


time_t File::getModificationTime (	const string& path, 
									bool* success)	// IN/OUT: If a valid pointer is passed, a bool is returned insted of an exception
{
	struct stat s; // PRQA S 4102
		
	int status = stat(path.c_str() , &s) ;
	int err = errno;
	if (success) { *success = (status == 0)	; }
	else if (0 != status)
	{
		// Exception returned only if no bool provided
		ACS_THROW(exFileOpenException("Cannot stat  \"" + path +"\"",err)) ; // PRQA S 3081
	}
    else { /* nothing */ }
	
	return s.st_mtime;
}


string File::computeLockName(const string &fname, bool addExt) // PRQA S 4020
{
	if(addExt) { return File::addExt (fname, "lock") ; }
    return fname ; 
}



int File::lock(const string &fname, bool addExt, bool shared) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
	
	string lockname = computeLockName(fname, addExt) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "lockfile \"" << lockname) ;

    int f = open(lockname.c_str(), O_CREAT|O_RDWR, 0666) ;	// open lockfile
	if (f<0)
	{
        	int err = errno;
		// Exception disabled, since in case of a file removal by another process during this lock attempt, an error might be raised
		//	ACS_THROW(exFileOpenException("Cannot create/open \"" + lockname +"\"",errno)) ; 
	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "flock " << (shared ? "SH":"EX") << " could not open lockfile " // PRQA S 3380
			<< strerror(err) << " errno: " << err) ;
		
		int reason = -err ;
		return reason ; // lock failed since it could not open the lockfile
	}
	
	FDGuard fd(f) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fd is " << fd) ; // PRQA S 3050


	// note the shared argument to lock in SHARED or EXCLUSIVE mode (non blocking)
	int fail = ::flock(fd, shared ? (LOCK_SH|LOCK_NB): (LOCK_EX|LOCK_NB)) ;	// lock is non-blocking (either shared or exclusive) // PRQA S 3050, 3081, 3380
    
	int err = errno ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "flock " << (shared ? "SH":"EX") << " returned " << fail << " " // PRQA S 3380, 3385
			   << ((fail < 0) ? strerror(err) : "") << " errno: " << err) ;

	if ( 0 == fail) { // lock succesfull
		int fdret = fd ; // PRQA S 3050
		fd.detach() ; // no close fd when out of scope
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "flock succeed returning " << fdret) ;
		return fdret ;
	}
	int reason = -err;
	return reason ;	// lock failed
}

bool File::unlock(int &fd) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	if (fd>=0) {
		int fail = ::flock(fd, LOCK_UN) ;
		if (0 == fail) {
			if ( 0 != ::close(fd) )
			{
				int err = errno;
				ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", err));
			}
			// sets the given fd to -1
			fd = -1 ;
			return true ;
		}
		return false ; // not unlocked wait for another call
	}
	
	// the pointer was null. potentially the file is unlocked
	return true ;
}


/* Get free space into a volume (In Megabytes, i.e. (2^20) bytes) */
long int File::getFreeSpaceInMB(const string &volumeName)
{	
 	struct statfs struct_statf ; // PRQA S 4102
	long int bfree = -1 ;


    if (0 != statfs( volumeName.c_str(), &struct_statf ))
    {
		int err = errno;
		ACS_THROW(exIOException("Cannot stat on volume " + volumeName, err)) ;  // PRQA S 3081
    }
    
	if (struct_statf.f_bsize >= 1024) // PRQA S 4400
    {
        /* In order to reduce the loss of precision, a 10bit-shift is performed twice */
        long int bsize = (struct_statf.f_bsize >> 10) ; // PRQA S 3003
        bfree = (struct_statf.f_bavail >> 10) * bsize ; // PRQA S 3000, 3084, 4400
	}
    else
    {
        /* A 20bit-shift is performed once */
        bfree = (struct_statf.f_bavail >> 20) ; // PRQA S 3000, 4400
        bfree = bfree * struct_statf.f_bsize ;
    }

	return bfree ;

}


/* Get free space into a volume (In blocks) */
long int File::getFreeSpaceInBlocks(	const string &volumeName, 	/* IN: Volume name */
                    					long int& blockSize)				/* OUT: Block size */
{
 	struct statfs struct_statf ; // PRQA S 4102

    if (0 != statfs( volumeName.c_str(), &struct_statf ))
    {
		int err = errno;
		ACS_THROW(exIOException("Cannot stat on volume " + volumeName, err)) ; // PRQA S 3081
    }
    
    blockSize = struct_statf.f_bsize ;	// Block size (bytes)
	
    return struct_statf.f_bavail ;		// Number of blocks // PRQA S 3000
}


/* Get volume total size (In Blocks) */
long int File::getVolumeSizeInBlocks(const string &volumeName, 	/* IN: Volume name */
                    					long int& blockSize)				/* OUT: Block size */
{     
 	struct statfs struct_statf ; // PRQA S 4102

    if (0 != statfs( volumeName.c_str(), &struct_statf ))
    {
		int err = errno;
		ACS_THROW(exIOException("Cannot stat on volume " + volumeName, err)) ;  // PRQA S 3081
    }
    
    blockSize = struct_statf.f_bsize ;	// Block size (bytes)
	
    return struct_statf.f_blocks ;		// Number of blocks // PRQA S 3000
}


// add the extension, avoid mistakes (e.g. ".<BLANK>" or "file..ext")
string File::addExt(const string &path, const string &ext)  // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	ACS_COND_THROW(path.empty(), exIOException("Empty file passed")) ; // PRQA S 3081
	ACS_COND_THROW((path.at(path.length()-1)=='/'), exIOException("Invalid file passed")) ; // PRQA S 3081 // PRQA S 3084
	
	if (ext.empty()) { return path ; }
	
	if ( (path.at(path.length()-1)=='.') || (ext.at(0)=='.')) { return (path + ext) ; } // PRQA S 3084
	
	return ( path + "." + ext ) ;
}


// returns the extension
string File::getExt(const string &path) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
	// here is supposed the extension comes after the last dot
	// not followed by a slash

	// find the last 'slash' if any
	string::size_type slash = path.rfind('/') ;
	if (slash == string::npos) {
		slash = 0 ;
    }

	string::size_type dot = path.rfind('.') ;
	if ((dot != string::npos) && (dot > slash)) {
		return path.substr(dot+1) ; // PRQA S 3084
    }

	return "" ; 
}


// returns the extension after the FIRST dot
string File::getFExt(const string &path) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	// here is supposed the extension comes after the last dot
	// not followed by a slash

	// find the last 'slash' if any
	string::size_type slash = path.rfind('/') ;
	if (slash == string::npos) {
		slash = 0 ;
    }

	string::size_type dot = path.find('.', slash) ;
	if (dot != string::npos) {
		return path.substr(dot+1) ; // PRQA S 3084
    }

	return "" ; 
}


// remove the extension from a file (from LAST dot)
string File::removeExt(const string &path) 
{
	return fileWExt(path,"",false) ;
}


// remove a given extension from filename (if found)
string File::removeExt(const string &path, const string& extIN)  // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

    if (extIN == "") { return path;	} /* Extension is empty ! */
    
    if ( testMatchExt(path, extIN) )
    {
    	/* Extension was found! */
    
		string ext = extIN ;
        if ( ext.substr(0, 1) != ".") { ext = "." + ext ; }

        return path.substr(0, path.length()-ext.length()) ; // PRQA S 3084	
	}
    
    
	return path ;	/* Extension not found */
}


// test if a given extension matches the filename (e.g. "Pippo.a.b", "a.b" returns TRUE)
bool File::testMatchExt(const string &path, const string& extIN) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
	string ext = extIN ;
    
    if (ext == "") { return true ; }	/* Extension is empty ! */
    
    if (ext.find('/') != string::npos) {
    	return false ;	/* Extension is invalid! */
    }
    
    if ( ext.substr(0, 1) != ".") { ext = "." + ext ; }	/* to make easier the matching/erasing */

    if( path.length() < ext.length() ) { return false ; }	/* File name shorter than extension */
    
    
	if (path.substr(path.length()-ext.length(), ext.length()) == ext) { // PRQA S 3084
    	return true ;	/* extension was found! */
    }

	return false ;
}


// remove the extension from a file (from FIRST dot)
string File::removeFExt(const string &path) 
{
	return fileWFExt(path,"",false) ;
}

// returns the file with a new extension -- replace from the LAST dot
string File::fileWExt( // PRQA S 4020
	const string &path, 
	const string &ext, 
	bool autodot
) 
{
// EC: this method has multiple returns to keep its complexity as low as possible
	string res = path ;
	
	// find the last 'slash' if any
	string::size_type slash = res.rfind('/') ;
	if (slash == string::npos) {
		slash = 0 ;
    }
	
	// find the last 'dot' if any
		
	string::size_type dot = res.rfind('.') ;
	
	// if the last 'dot' exists and it comes after the last 'slash'
	// removes char from the dot on...
	if ((dot != string::npos) && (dot > (slash+1))) {
		// WHEN HERE: a "." has been found at it is the last valid dot, since it's after the last slash. Furthermore it's NOT the beginning of the filename
		// > (slash+1) means: greater than one characther after the slash, so /dir/.test does not match, /dir.test/file does not match
		// /dir/a.txt matches and so on.
		res.erase(dot,string::npos) ;
    }
	
	if (ext.empty()) {
		return res ;
    }
		
	// if an extension to append was given and it does not begin with a dot
	// and the caller wants the dot (autodot == true) append a dot before the extension
	if ((ext[0] != '.') && autodot) { 
		res += "." ;
    }
	
	// finally append the requested extension	
	res += ext ;

	return res ;
}

// returns the file with a new extension -- replace from the FIRST dot
string File::fileWFExt( // PRQA S 4020
	const string &path, 
	const string &ext, 
	bool autodot
) 
{
// EC: this method has multiple returns to keep its complexity as low as possible
	string res = path ;
	
	// find the last 'slash' if any
	string::size_type slash = res.rfind('/') ;
	if (slash == string::npos) {
		slash = 0 ;
	}
    
	// find the first 'dot', if any, after the last slash
		
	string::size_type dot = res.find('.', slash) ;
	
   
	// if the last 'dot' exists removes char from the dot on...
	if (dot != string::npos) {
		res.erase(dot,string::npos) ;
    }
   
	if (ext.empty()) {
		return res ;
    }
		
	// if an extension to append was given and it does not begin with a dot
	// and the caller wants the dot (autodot == true) append a dot before the extension
	if ((ext[0] != '.') && autodot) {
		res += "." ;
    }
	
	// finally append the requested extension	
	res += ext ;

	return res ;
}

bool File::isFileInPathTree(const string &path, const string &fileWPath) 
{
	ACS_COND_THROW(fileWPath.empty(), exIOException("Empty file passed")) ; // PRQA S 3081
	ACS_COND_THROW(path.empty(), exIOException("Empty path passed")) ; // PRQA S 3081
	return getAbsolutePath(fileWPath).find(getAbsolutePath(path)) == 0 ;
} 

string File::getCurrentPath()
{
	char p[PATH_MAX] ;
	::memset(p,0,sizeof(p)) ;
	
	char * ret = ::getcwd(p,sizeof(p));
	int err = errno;
	ACS_COND_THROW(ret == NULL, exIOException("Cannot determine current working dir", err)) ; // PRQA S 3081
	return p ;
}


string File::getAbsolutePath(const string & path_in) // PRQA S 4020
{
	string ret_path ; 
	
	if (true == path_in.empty()) { ret_path="" ; }
	else if (("." == path_in) || ("./" == path_in)) { ret_path=File::getCurrentPath() ; }
	else
	{
		list <string> lst ;
		vector <string> v ( StringUtils::pathDisassemble(path_in) ); // If the path starts with "/", the first element in the vector is, conventionally, ""
		if (false == v.empty())
		{
			if (false == v.at(0).empty())
			{
				/* The given path DOES NOT START WITH "/", so it must be a relative path */
				const vector <string> v_cwd(StringUtils::pathDisassemble(File::getCurrentPath())) ;
				lst = list <string> (v_cwd.begin(), v_cwd.end()) ;	// The list starts with the current working dir.
			}
			
			// The input path is turned to a list
			const list <string> lst2 ( list <string> (v.begin(), v.end()) ) ; 
			
			// Concatenate the input path to the working list (which might already contain the cwd)
			lst.insert(lst.end(), lst2.begin(), lst2.end());
			
			// The working list contains the input path, made absolute.

			// The last step is handle "." (suppressed) and ".." (the previous level is suppressed)
			
			for (list <string>::iterator it=lst.begin(); it!=lst.end(); /*No automatic increment, to allow erasing*/)
			{
				if ("." == *it) 
				{ 
					it = lst.erase(it) ; // Suppress current element and get the following one. This will be used as next element in the for/loop.
				}
				else 
				{
					if ( (".." == *it) && (it != lst.begin()) && ((--it)++ != lst.begin()) )
					{ 
						// dot-dot (..) found and neither the element nor the parents are head of the list. Remove this element and the parent.
						it = lst.erase(--it) ;	// Remove the parent element, and get the following one [the element following the parent is the current one] 
						it = lst.erase(it) ;	// Remove current element and get the following one. This will be used as next element in the for/loop.
					} else {				
						++it ; // Increase the for/loop iterator
					}
				}
			}	// END: for (list <string>::iterator it=lst.begin(); it!=lst.end(); )
		}

		// If list not empty, it's turned in vector and assembled as string again:
		if (false == lst.empty()) { ret_path = StringUtils::pathAssemble( vector<string>(lst.begin(), lst.end()) ) ; }
	}

	return ret_path ;
}


string File::getDirPath(const string & path_in, bool normalizePath)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file is \"" << path_in << "\"") ;
	

	string path = (normalizePath? StringUtils::pathNormalize(path_in) : path_in) ; // PRQA S 3380, 3384, 3385

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		if (normalizePath) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "normalized path: \"" << path << "\"") ;
        }
	ACS_CLASS_END_DEBUG

	string res ;
	if (!path.empty())
	{
		size_t pos = path.length() ;
		string::size_type slash = 0;

		do
		{
			slash = path.rfind('/', pos-1) ;	// the slash we choose MUST NOT BE the last caracther in the path // PRQA S 3084

			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "(WHILE/LOOP) slash: " << slash << " pos: " << pos) ;

		} while ((pos>0) && (slash != string::npos) && (slash == --pos)) ; // PRQA S 3230

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "(LOOP FINISHED) slash: " << slash << " pos: " << pos) ;

		if (slash != string::npos)
		{
			if (slash>0) {
				res.append(path, 0, slash);
            }
			else {
				res = "/" ;	// path doesn't contain directories and begins with "/", so output is normalized as "/"
            }
				
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "output dir is \"" << res << "\"") ;
		}
		else
		{
			// no slashes. PathName is current directory
			res = ".";
		}
	}	// if (!path.empty())
	else
	{
		res=".";
	}

	return res;
}


// splits a cache filename in its components (Filename, Fileversion and Inventory Date)
void File::splitCacheFileName(const string& cacheName,/* IN: CacheName */ 
					string& fileName,			/* OUT: FileName */ 
					string& fileVersion,		/* OUT: FileVersion */
					time_t & inventoryDate)	/* inventoryDate */
{
	if (cacheName.find("#") == string::npos)
	{
		// Not a cachefile
		ostringstream msg;
		msg << "File::splitCacheFileName " << __LINE__ << "  invalid format; expected filename#fileversion#inventorydate" ;
		ACS_THROW(exCriticalException( msg.str())); // PRQA S 3081
	}
	

	fileName = cacheName.substr(0, cacheName.find("#")) ;
	if (fileName.empty())
	{	
		// Invalid filename payload
		ostringstream msg;
		msg << "File::splitCacheFileName " << __LINE__ << "  fileName resulted Empty" ;
		ACS_THROW(exCriticalException( msg.str())); // PRQA S 3081
	}
	

	string tmp = cacheName.substr(cacheName.find("#")+1) ; // PRQA S 3084
	fileVersion = tmp.substr(0, tmp.find("#")) ;
	if (fileVersion.empty())
	{		
		// Invalid fileversion payload
		ostringstream msg;
		msg << "File::splitCacheFileName " << __LINE__ << "  fileVersion resulted Empty" ;
		ACS_THROW(exCriticalException( msg.str())); // PRQA S 3081
	}


	string invDate = tmp.substr(tmp.find("#")+1) ; // PRQA S 3084
	if (invDate.empty())
	{		
		// Invalid invdate payload
		ostringstream msg;
		msg << "File::splitCacheFileName " << __LINE__ << "  invDate resulted Empty" ;
		ACS_THROW(exCriticalException( msg.str())); // PRQA S 3081
	}
	inventoryDate = time_t(atoll (invDate.c_str())) ; // PRQA S 3081
	return ;
}


// builds cacheFilename from Filename, Fileversion and Inventory Date
string File::getCacheFileName(	const string& fileName,	/* IN: FileName */ 
								const string& fileVersion,	/* IN: FileVersion */ 
								const time_t & inventoryDate)	/* IN: InventoryDate */
{	
	string cacheName;
	ostringstream d ; 
	
	// setw(10) for inv.date allows fixed-length digits till 9 billion of secs (year 2286..), thus preserving lexicographical order.
	if (inventoryDate>0) { d << setw(10) << setfill('0') << inventoryDate ; } // PRQA S 4400
	else if (inventoryDate<0) { d << inventoryDate ; }
	else { d << "" ; }
	
	if (fileName.find("#") != string::npos) { cacheName = fileName; }
	else
	{
		cacheName = fileName + "#" + fileVersion + "#" + d.str() ;
	}


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fileName: !" 
        	<< fileName << "!   fileVersion: !" << fileVersion  << "!   inventoryDate: " 
            << d.str() << "  ->  cacheName: !" << cacheName << "!" ) ;


	return cacheName ;
}


string File::getFileName (const string & thepath)
{
	string path = thepath;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file is \"" << thepath << "\"") ;

	// Normalize the path
	path = StringUtils::pathNormalize(path);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Normalized file is \"" << path << "\"") ;
	string res ;

	// find the last 'slash' if any
	string::size_type slash = path.rfind('/') ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "last slash position is " << slash) ;
	if (slash != string::npos)
	{
		res += path.substr(slash+1,path.length()-slash ); // PRQA S 3084
	}
	else { res=path; }
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "filename is \"" << res << "\"") ;
	
	return res;	// Prune the directory (if any) from filename
}


string File::which(const string &exe)
{
	string pcmd ;
	// check the existence of which command in /usr/bin
	if (File::exists("/usr/bin/which")) {
		pcmd = "/usr/bin/which ";
    }
	else {
		pcmd = "which ";
    }
	
#ifndef WHICH_FORCE_NO_SKIP_ALIAS
    pcmd += "--skip-alias ";
#endif
	
	// check if the file is compressed or not
	pcmd += exe + " 2> /dev/null | egrep -v 'alias.*=' ";
	
	FILE *filep = 0;
	// Thread safe guard around popen
	{
		ThreadSafe::Lock lock (_fileGlobalMutex);
		filep = popen(pcmd.c_str(),"r") ; // PRQA S 4412
	}
	int err = errno;
	ACS_COND_THROW(!filep,exFileOpenException("Cannot run which on \"" + exe +"\"",err)) ; // PRQA S 3081

	//******** PAOLO ************
	size_t bufSize = 0 ;
	char *buffer=0;
	ssize_t retSize = 0 ;
	if( (retSize = ::getline( &buffer , &bufSize , filep))  == -1)
	{
		err = errno;
		ThreadSafe::Lock lock (_fileGlobalMutex);
		if (( -1 == pclose(filep) ) )
		{
			int errC = errno;
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::pclose syscall failed", errC));
		}
		free(buffer);
		ACS_THROW(exFileOpenException("Cannot run which on \"" + exe +"\"", err)) ; // PRQA S 3081
	}

	// Thread safe guard around pclose
	{
		ThreadSafe::Lock lock (_fileGlobalMutex);
		if (-1 == pclose(filep) ) 
		{
			int errC = errno;
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::pclose syscall failed", errC));
		}
	}
	// get rid of ending spaces ff etc ...
	if (0 != isspace(buffer[retSize-1])) {
		buffer[retSize-1] = 0 ;
    }

	string result(buffer) ;
	free(buffer);

	//get rid of newline
	size_t pos=result.find('\n');
	if(pos != result.npos) {
		result[pos]='\0';
    }
	
	return result;

}

string File::computeMd5Sum(const string & path)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file is \"" << path << '\"') ;
	
	FILE* fp = 0 ;
	ACS_CRC_Digest_Retbuf retbuf ;
	
	fp = fopen(path.c_str(), "r");
	ACS_COND_THROW(0 == fp, exFileOpenException("Cannot open file \""+path+'\"') ); // PRQA S 3081
	CRC_Digest_md5 md5 ;
	
	// Create buffer using digest's expected Size 
	unsigned char buffer[ md5.expectedSize() ] ;

	size_t readSize = 0;

	while ( (readSize = fread (buffer, 1,  sizeof(buffer), fp)) > 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, readSize << " bytes read") ;
		md5.addBuffer(buffer, readSize) ;
	}
	if( 0 != ferror(fp) ) {
		int error = fileno(fp) ;
		ostringstream exMsg;
		exMsg << "Error occurred reading from \"" << path << "\"" ;
		fclose(fp);
		ACS_THROW( exIOException(exMsg.str(), error) ); // PRQA S 3081
	}
	fclose(fp);
	
	return md5.getResult(retbuf);

}


//////////////////////////////////////////////////////////////////////////////////////////
//
// Class File::Lock
//
//////////////////////////////////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(File::Lock) 
const unsigned long File::Lock::_defaultRetryTime_ms = 100 ; // msec

File::Lock::Lock(const std::string &file, bool addExt, const std::string &pfx) : // std nspace added for doxygen
	_lockedFile(file),
	_addExt(addExt),
	_lockedStreamBase(-1),
    _pfx(pfx),
    _autoRemoveLockFile(false),
    _lockFileName()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  File: \"" << _lockedFile << "\"  addExt: " << boolalpha << addExt) ;
}

File::Lock::~Lock() throw() 
{
    // PRQA S 4631 L3
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  File: \"" << _lockedFile << "\"  fd: " << _lockedStreamBase)  ;
	try {
	    unlock() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L3
}


File::Lock::Lock(const File::Lock &o) :
	_lockedFile(o._lockedFile),
	_addExt(o._addExt),
	_lockedStreamBase(o._lockedStreamBase),
    _pfx(o._pfx),
    _autoRemoveLockFile(o._autoRemoveLockFile),
    _lockFileName(o._lockFileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Copy Method called") ;
}

void File::Lock::swap(Lock &rhs) 
{
    if (this != &rhs) {
        std::swap(_lockedFile, rhs._lockedFile) ;
        std::swap(_addExt, rhs._addExt) ;
        std::swap(_lockedStreamBase, rhs._lockedStreamBase) ;
        std::swap(_pfx, rhs._pfx) ;
        std::swap(_autoRemoveLockFile, rhs._autoRemoveLockFile) ;
        std::swap(_lockFileName, rhs._lockFileName) ;
    }
}

File::Lock &File::Lock::operator=(const File::Lock &o) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Assignment Method called") ;

	// = operator
	if (this != &o) {
		// Copy parameters set in original c'tor :
		_lockedFile = o._lockedFile ;
		_addExt = o._addExt ;
		_lockedStreamBase = o._lockedStreamBase ;
		_pfx = o._pfx ;
		_autoRemoveLockFile = o._autoRemoveLockFile ;
		_lockFileName = o._lockFileName ;
	}
	
	return *this ;
}


void File::Lock::unlock()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. _lockedFile: \"" << _lockedFile 
			<< "\"  fd: " << _lockedStreamBase) ;
	
	if (_lockedStreamBase>=0) {
		// just to be clean
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file \"" << _lockedFile << "\"  fd: " << _lockedStreamBase 
    		<< "  (lock file: \"" << _lockFileName << "\") unlocked") ;

        if (_autoRemoveLockFile && not _lockFileName.empty()) {
            try {
                File::unlink(_lockFileName) ;
		        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file \"" << _lockFileName << "\"  unlinked - but still locked") ; 
            }
            catch(exception &x) {
                ACS_LOG_NOTIFY_EX(x) ;
                ACS_LOG_ERROR("Cannot unlink: " << _lockFileName <<  " ignored - left on file system") ;
            }
        }
		File::unlock(_lockedStreamBase) ;	// flock with UNLOCK (LOCK_UN) parameter
		_lockedStreamBase = -1 ;
	} // if (_lockedStreamBase>=0)


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished.  _lockedFile: \"" << _lockedFile << "\"  fd: " << _lockedStreamBase) ;
}


void File::Lock::setLock(const std::string &file, bool addExt) // std added for doxygen
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  Stored file: \"" << _lockedFile << "\"  addExt: " << boolalpha << _addExt  
			<< "  fd: " << _lockedStreamBase << "  New file: " << file << " New addExt: " << boolalpha << addExt) ;

	if ((_lockedStreamBase>=0) && (file != _lockedFile)) {
		// A file is already locked !
		ACS_THROW(LockException("Cannot change the lock status, the file " + _lockedFile + " is already locked")) ; // PRQA S 3081
	}
	_lockedFile = file ;
	_addExt = addExt ;
}


bool File::Lock::lock(unsigned long timeout_ms, bool shared) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File: \"" << _lockedFile << "\"  fd: " << _lockedStreamBase  // PRQA S 3380
			<< "  Timeout " << timeout_ms << "[ms]  Lock type: " << (shared? "SH" : "EX" ) << "  _addExt: " << boolalpha << _addExt) ;

	if (_lockedStreamBase >= 0) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file \"" << _lockedFile << "\" already locked") ;
		return true ;
	}

    string lockFile = _lockedFile ;
    if (not _pfx.empty()) {
        string path = getDirPath(lockFile) ;
        if (not path.empty()) { path += "/" ; }
        string file = _pfx+getFileName(lockFile) ;
        lockFile = path + file ; 
    }
	
	
    Timer::Delay delay = min(_defaultRetryTime_ms,timeout_ms) ; 
	Timer::Delay current_ms = 0 ;
    
	do {
    
		_lockedStreamBase = File::lock(lockFile, _addExt, shared) ;
		if ( _lockedStreamBase < 0) {
		    if (-_lockedStreamBase != EWOULDBLOCK) {
			    ACS_THROW(exIOException("Cannot open lockfile \"" + lockFile + "\"", -_lockedStreamBase)) ;  // PRQA S 3081
            }
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " waiting " << delay << " for file \"" << lockFile << "\"") ;
			Timer::delay(delay) ;

		} else {
            _lockFileName = File::computeLockName(lockFile, _addExt) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " file \"" << lockFile << "\" LOCKED - name of lock is: " << _lockFileName) ;
			break ; // locked
		}
		current_ms+=delay ; // PRQA S 3084
	} while (current_ms < timeout_ms) ;	// if equal returns 
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "total wait was " << current_ms << " returning " << boolalpha << (_lockedStreamBase >= 0)) ;

	return _lockedStreamBase >= 0 ; 
}

bool File::Lock::isLocked() const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  File: \"" << _lockedFile << "\"  fd: " << _lockedStreamBase 
			<< "  Returning: " << boolalpha << (_lockedStreamBase >= 0)) ;
	
	return _lockedStreamBase >= 0 ; 
}

const string & File::Lock::filename() const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  File: \"" << _lockedFile << "\"  fd: " << _lockedStreamBase);
	
	return _lockedFile ; 
}


bool File::Lock::addExt() const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called.  File: \"" << _lockedFile << "\" addExt: " << boolalpha << _addExt) ;
	
	return _addExt ; 
}


//
// STATIC METHOD: tryLock
void File::Lock::tryLock(Lock &l, int attempts, size_t attemptTimeout_ms, bool shared)  // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called.  File: \"" << l._lockedFile // PRQA S 3380
			<< "\"  fd: " << l._lockedStreamBase << "  LockType: " << (shared? "SH" : "EX")) ;
	
	if (0 != abs(attempts)) {
		for (int i=0; i < attempts; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "try # \"" << i << "\"") ;
			if (l.lock(attemptTimeout_ms, shared)) { return ; }
		}
		int err = errno;
	    	ACS_THROW(LockException("Cannot lock over " + l.filename(), err)) ; // PRQA S 3081
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "looping until lock succesful") ;
		for(;!l.lock(attemptTimeout_ms, shared);) { /* empty */ } ; // PRQA S 4231, 4238, 4244
		return ;
	}
}


/**
	EC:: Return a map with all mounted devices (like unix-cmd "mount")
    KEY: Device
    VALUE: Mount_point
    
    Example:
    ("/dev/sda1", "/"), ("/dev/sdb1", "/raid0"), ..., ("proc", "/proc"), ... 
*/
multimap<string, string> File::getMountEntries () 
{
    struct mount_entry* pMount_entry_list = read_file_system_list(true) ;	/* Entry point */
	multimap <string, string> mountEntries ;


    for ( struct mount_entry* pMount_entry = pMount_entry_list; pMount_entry; (pMount_entry = pMount_entry->me_next) )
    {

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
        {
        	char tmp[2048] ; // PRQA S 4403
        
            sprintf(tmp, "File::getMountEntries:\n\tme_devname:!%s!\n\tme_mountdir:!%s!\n\tme_type:!%s!\n\tme_dummy:!%u!\n" \
						"\tme_remote:!%u!\n\tme_type_malloced:!%u!", 
            					pMount_entry->me_devname, 
        					    pMount_entry->me_mountdir, 
                                pMount_entry->me_type, 
                                pMount_entry->me_dummy,
                                pMount_entry->me_remote,
                                pMount_entry->me_type_malloced );
            ACS_LOG_DEBUG(SimpleDebugSignature << tmp) ;
        }
        ACS_CLASS_END_DEBUG
 
 		mountEntries.insert ( pair<string, string>(pMount_entry->me_devname, pMount_entry->me_mountdir) )  ;
 
    }	/* for */


	if ( pMount_entry_list ) { free_file_system_list ( pMount_entry_list ) ; }	/* deallocate entry point */


	return mountEntries ;
}
	

/**
	EC:: Return a Filesystem UID (Unique-ID) or ULLONG_MAX in case of error.
    Useful to discover if two files belong to THE SAME filesystem.
    
    Example:
    "/" -> 1
    "/raid0" -> 3
    "/extdisk" -> 5
    ...
    
*/
unsigned long long int 
						File::getFSUID (const string& fsName, // PRQA S 4020, 5500
                                        bool followSymLinks)    /* Recursively expand symlinks */ 
{
// EC: this method has multiple returns to keep its complexity as low as possible

	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  fsName: \"" << fsName 
			<< "\"  followSymLinks: " << boolalpha << followSymLinks) ;
	
	string filteredFsName = StringUtils::pathNormalize(fsName) ;	/* Trasforms /x//y/ in /x/y  and ./x in "x"  */

    // here you can get an empty filteredFsName 
    // In the case it is empty it has to be converted to the current absolute path
	if ( (filteredFsName.empty()) || ( (!filteredFsName.empty()) && (filteredFsName[0]!='/') ) )/* Turn relative path in absolute path */
	{ filteredFsName = StringUtils::pathNormalize(StringUtils::pathJoin(getCurrentPath(), filteredFsName)) ; }
	
	
    if (followSymLinks && File::exists(filteredFsName) )
    {
		/* Must resolve link, file exists AND is a symlink: */    
		char fsName_resolved[16*1024] ; // PRQA S 4403
    
		filteredFsName = ( (realpath(fsName.c_str(), fsName_resolved)!=0 ) ? fsName_resolved : fsName ) ; // PRQA S 3380, 3385
     
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method called.  After realpath: \"" << filteredFsName << "\"") ;
    }
	
	filteredFsName = trimAllSlashes(filteredFsName) ;
	
	multimap <string, string> mountEntries = getMountEntries () ;
    multimap <string, string>::iterator i ;

	if ( filteredFsName.empty() ) 
	{ return 0 ; }
	else
	{ filteredFsName = trimAllSlashes( (filteredFsName) + "/" ) ; }

	bool found(false) ; 
	size_t sz_found(0) ; 
	string mt_found ;
	
	for (i=mountEntries.begin(); i != mountEntries.end(); ++i)
    {
		const string mountItem ( trimAllSlashes( (i->second) + "/" ) ) ; 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Comparing: \"" << mountItem << "\"  with: \"" << filteredFsName << "\"  size: " << mountItem.size() ) ;
	
    	if ( 0 == strncmp(mountItem.c_str(), filteredFsName.c_str(), mountItem.size() ) )
        {
			if ( (!found) || (sz_found < mountItem.size())) 
			{
				sz_found = mountItem.size() ;
				mt_found = mountItem ;
				found = true ;
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Best match so far: \"" << mountItem << "\"  (size: " << sz_found << ")" ) ;
			}
			
    	} // END: if

	}	// END: for

	unsigned long long int hash = 0 ;
	if (found) 
	{
		hash = 5381 ; // PRQA S 4400
		const char* str (mt_found.c_str()) ; 
		int c = 0; while ( (c = *str++) != 0) { hash = ((hash << 5) + hash) + c; /* hash * 33 + c */ }  // PRQA S 3000, 3084, 4400, 3700
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Addedto hash: \"" << mt_found << "\"  hash: " << hash) ;
	}
	
	if (!found) { 
		hash = ULLONG_MAX ;	// Which is maxint in the ulonglong domain, denoting some error
		ACS_LOG_WARNING("No mount point matches the input \"" << filteredFsName << '\"' ); 
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  Returning: " << hash ) ;

	return hash ;
}

bool File::checkFileSystemsMatching(const string& path1, const string& path2, bool followSymLinks, unsigned long long int * FSUID1, unsigned long long int * FSUID2) 
{
	const unsigned long long int localFSUID1 ( getFSUID(path1, followSymLinks) ) ;
	if( NULL != FSUID1 ) { 
		*FSUID1 = localFSUID1;
	}
	
	const unsigned long long int localFSUID2 ( getFSUID(path2, followSymLinks) ) ;
	if( NULL != FSUID2 ) {
		*FSUID2 = localFSUID2;
	}
	
	bool ret = ( (localFSUID1 != ULLONG_MAX) && (localFSUID1 == localFSUID2) ) ;	// Match the FS UniqueIDs, also verifying good status

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, " path1: \"" << path1 << "\"  path2: \"" << path2 
			<< "\"  followSymLinks: " << boolalpha << followSymLinks << "  Returning: " << boolalpha << ret) ;

	return ret ;

}


string File::trimAllSlashes(const string& inPath) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	string outString = "" ;
    bool found = false ;

	if (inPath.empty()) { return "" ; }
    
    for (unsigned int i=0; i<inPath.size(); i++)
    {
    	char c = (inPath.c_str())[i] ;
        
        if ( (c == '/') && found ) {
        	continue ;
        }
        else if (c == '/') { found = true ; }
        else {
        	found = false ;
        }
            
        outString.push_back(c) ;
    
    }

	return outString ;
}



string File::trimTrailingSlashes(const string& inPath) // PRQA S 4020
{
// EC: this method has multiple returns to keep its complexity as low as possible

	string outString = "" ;
    signed int i = 0 ;

	if (inPath.empty()) { return "" ; }
    
    for (i=( static_cast<signed int>(inPath.size())-1); i>=0; i--) // PRQA S 4230, 3081
    {
    	char c = (inPath.c_str())[i] ;
        
        if (c != '/') { break ; }
    }
        
    if (i<0) { i=0 ; }	/* if the path begins with /, the returned index will be -1 */
    
    return inPath.substr(0, i+1) ; // PRQA S 3000
	
}


/*
	Split a string containing a trailer (e.g. "File123@456" pattern "@" will return: "File123", "456")
*/
void File::splitTrailer(const string& inString,	/* I: Input string: e.g. Test#4#5*/ // PRQA S 4020
						const string& pattern,  /* I: Pattern string: e.g. "#" */
						string& header,			/* O: Header: e.g. "Test" */			 
						string& trailer)		/* O: Trailer: e.g. "4#5" */
{
	header = inString ;
	trailer = "" ;

	if ( inString.empty() || pattern.empty() || (inString.find(pattern) == string::npos)) { return ; }

	size_t ppos = inString.find(pattern) ;	// search pattern

	header = inString.substr(0, ppos) ;
	trailer = inString.substr(ppos+pattern.length()) ; // PRQA S 3084
}

exostream & operator<< (exostream &os, File::CompressionMode m) // PRQA S 4222
{
    switch (m) {
    case File::Plain:
    {
        os << "Plain" ;
        break ;
    }
	case File::GZipped: 
    {
        os << "GZipped" ;
        break ;
    }
    case File::Compressed:
    {
        os << "Compressed" ;
        break ;
    }
#ifdef __linux__
	case File::BZipped:
    {
        os << "BZipped" ;
        break ;
    }
#endif
    case File::TARred:
    {
        os << "TARred" ;
        break ;
    }
    default:
    {
        os << "<unrecognized File::CompressionMode: "<<int(m) << ">" ; // PRQA S 3081
        break ;
    }
    
    }
    return os ; // PRQA S 4028
}

ostream & operator<< (ostream &os, File::CompressionMode m) 
{
    switch (m) {
    case File::Plain:
    {
        os << "Plain" ;
        break ;
    }
	case File::GZipped: 
    {
        os << "GZipped" ;
        break ;
    }
    case File::Compressed:
    {
        os << "Compressed" ;
        break ;
    }
#ifdef __linux__
	case File::BZipped:
    {
        os << "BZipped" ;
        break ;
    }
#endif
    case File::TARred:
    {
        os << "TARred" ;
        break ;
    }
    default:
    {
        os << "<unrecognized File::CompressionMode: "<<int(m) << ">" ; // PRQA S 3081
        break ;
    }
    
    }
    return os ; // PRQA S 4028
}



_ACS_END_NAMESPACE
