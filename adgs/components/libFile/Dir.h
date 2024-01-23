// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.10  2016/03/08 14:59:13  marpas
	removing mutex in File class
	This was originally due to some system call thread safety issues (e.g. readdir)
	The latter was explicitely changed into readdir_r call
	Other syscall were checked and up to now seems there is none having thread safety problems
	
	Revision 5.9  2015/05/20 10:28:55  marfav
	S2PDGS-1224: Dir object can now return also broken links if requested
	QACPP warnings removed
	
	Revision 5.8  2014/11/04 11:03:50  marpas
	qa warning
	
	Revision 5.7  2014/10/10 12:21:12  enrcar
	EC:: include was missing
	
	Revision 5.6  2014/10/10 12:09:37  enrcar
	EC:: dirent replaced with stat to handle the type of a dir.entry (dir, reg.file, symlink...)
	
	Revision 5.5  2014/02/11 10:45:23  marpas
	qa warning fixed
	
	Revision 5.4  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2014/02/07 15:34:49  marpas
	introducing predicate concept to filter directory's entries
	
	Revision 5.2  2013/07/04 18:49:27  marpas
	finishing adoption of libException 5.x standards
	qa warnings removed
	compilation warnings removed
	coding best practices applied
	
	Revision 5.1  2013/07/02 17:31:26  marpas
	adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
	
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 2.19  2013/01/23 16:41:01  chicas
	Added setDifference
	
	Revision 2.18  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.17  2012/10/04 13:25:59  lucio.pulvirenti
	DirTree: show_files and add_files methods ported from PDS2 environment implementation.
	
	Revision 2.16  2012/02/13 10:59:04  marpas
	refactoring in progress
	
	Revision 2.15  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.14  2011/08/02 16:13:04  enrcar
	EC:: useless methods removed
	
	Revision 2.13  2011/08/02 14:26:20  enrcar
	EC:: sort() method renamed: reset(). Added three new modifiers: sort_alpha (alphabetic sort); sort_ialpha(case-insents. alphabetic sort); sort_default(default sort)
	
	Revision 2.12  2011/04/19 16:21:44  marpas
	DirTree doc and behaviour improved
	
	Revision 2.11  2011/04/08 12:41:47  enrcar
	EC:: Added public method DirTree::bytes()
	
	Revision 2.10  2011/03/29 15:26:36  marant
	EC:: size_t instead of ulong used for overloading of stl members: size() and operator[] for classes Dir and DirTree
	
	Revision 2.9  2011/03/07 09:27:57  enrcar
	EC:: Added modifier regexp_reject in order to REJECT entries matching a pattern
	
	Revision 2.8  2011/02/15 15:10:41  enrcar
	EC:: Dir::bytes() member added
	
	Revision 2.7  2011/02/11 13:52:43  enrcar
	EC:: comment added
	
	Revision 2.6  2011/02/11 13:25:11  enrcar
	EC:: regexp modifier added.
	
	Revision 2.5  2011/01/27 14:20:23  lucio.pulvirenti
	EC:: comment added
	
	Revision 2.4  2011/01/25 16:48:51  enrcar
	EC:: Several changes, in order to internally manage directory paths as string vectors (instead of a slash-separated string), to allow a faster navigation and easier evaluation of path depth.
	Modifiers handled.
	Added modifiers: relative(), absolute(), mindepth(), maxdepth()
	
	Revision 2.3  2011/01/24 10:03:48  enrcar
	EC::  Dir::DirTree reverse sort added
	
	Revision 2.2  2011/01/21 16:58:04  enrcar
	EC:: DirTree class added
	
	Revision 2.1  2009/03/13 15:26:23  marpas
	method sort added - no regression on Class behaviour
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/30 22:06:41  marpas
	headers fixed
	
	Revision 1.2  2003/04/30 16:25:56  marpas
	using namespace std no longer into acs includes
	
	Revision 1.1.1.1  2002/10/04 16:58:31  paoscu
	Import libFile
	
		

*/
 
#ifndef _Dir_H_
#define _Dir_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <dirent.h>
#include <sys/stat.h> // PRQA S 1013
#include <File.h>
#include <Guard.hpp>

#include <cstddef>

_ACS_BEGIN_NAMESPACE(acs)

inline bool DirNopPredicate(const std::string &path, const std::string &entry, bool isDir) { return true ; } // PRQA S 2134


/*! \class Dir
	
	Utility class to support dir convenience functions.
*/
class Dir // PRQA S 2109
{
public:
	explicit Dir(const std::string &, bool hidden = true, bool brokenln = false) ;
	
    template<typename Predicate>
    inline
    Dir(const std::string &path, Predicate p, bool hidden = true, bool brokenln = false) :
        _files(),
	    _path(path),
	    _hidden(hidden),
	    _reportBrokenLinks (brokenln)
    {
	    load(p) ;
    }

    
	Dir(const Dir &) ;
	Dir &operator=(const Dir &) ;
	~Dir() throw() ;
	
	template<typename Predicate>
    inline
	void load(Predicate p) 
    {
	    // Scan directory
	    ::DIR *dir = ::opendir(_path.c_str()) ;
	    ACS_COND_THROW(!dir,exIOException("Cannot read directory \"" + _path +"\"", errno)) ; // PRQA S 3081

	    //Delete previous results to prevent duplications
	    _files.clear();

		// reentrant version of readdir is used here
		// see man 3 readdir for details on the malloc used here

#ifndef PRQA_ANALYZER
        size_t len = offsetof(struct dirent, d_name) + pathconf(_path.c_str(), _PC_NAME_MAX) + 1;
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

//	    for(struct ::dirent *e = 0; errno=0, e=::readdir(dir);) { // PRQA S 4231, 4238, 4244
//            err = errno ;
		    std::string item = entry->d_name ;
		    if ((item == ".") || (item == "..")) {
			    continue ;	/* Ignore . and .. */
            }
		    if (!_hidden && (item[0] == '.')) {
			    continue ;	/* Ignore hidden files */
            }
			
			const std::string fullPath (_path + "/" + item ) ;
			struct stat stat_struct ; // PRQA S 4102
			if ( stat(fullPath.c_str(), &stat_struct) != 0 )
			{ 
				if (errno == ENOENT)  /* Probably a broken symlink */
				{
					if (_reportBrokenLinks && File::isSymbolicLink (fullPath) )
					{
						// Keep it in the files
						_files.push_back (item);
					}
					continue;
				}
				else { readdirval = errno ; break ; } // fill the control variable readdirval
			}
			
            if (p( _path, item, S_ISDIR(stat_struct.st_mode))) {
		        _files.push_back(item) ;
            }
	    }
	    if (0 != readdirval) {
		    ::closedir(dir) ;
		    ACS_THROW(exIOException("Cannot read directory \"" + _path+"\"", readdirval)) ; // PRQA S 3081
	    }
	    ::closedir(dir) ;
    }

    //! mathematical set difference between vector of files inside dirs
    /*! all the elements that are in dir but not in otherDir are placed in
     * the result set 
     * Warning: otherDir is sorted
     * */
    void setDifference(  Dir & otherDir, std::vector<std::string > & output ); 

	bool showHidden() const throw() ;
	bool reportBrokenLinks() const throw() ;
	const std::string &operator[](size_t) const ;      // PRQA S 2141
	size_t size() const ;                           // PRQA S 2502
	bool empty() const ;
	static bool empty(const std::string & path, const bool includeHidden = true) ;
	const std::string path() const ;
	void sort() ;                                  // PRQA S 2502
	signed long long int bytes() const ;	/* Since even on 32-bit hosts with 32-bit file offset, a directory might be larger */
	static signed long long int bytes(const std::string &) ;

private:
	// declared but not used 
	Dir() ;
    	
private:
	std::vector<std::string> _files ;
	std::string _path ;
	bool _hidden ;
	bool _reportBrokenLinks;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Dir) ;
} ;


/*! \class DirTree
	
	Utility class to support recursive access to directory contents
*/
class DirTree // PRQA S 2109
{
public:
	
	/**
	  *	DirTree	(	const std::string &path, 
	  *				const std::string& dirEntry, 
	  *				bool hidden) 
	  *	dirEntry, if present, is simply added to each directory entry. So /raid0 might be: /raid0 , /raid0/ or /raid0/. using respectively "", "/" and "/."
	  * 			If dirEntry not present, directory entries are omitted
	  */
	DirTree(const std::string &, bool hidden) ; 	/* OMIT directory entries (so, display /raid0/f and /raid0/dir/f but not "/raid0" or "/raid0/dir") */
	DirTree(const std::string &, const std::string& dirEntry, bool hidden) ;	/* ALSO SHOW directory entries, appending to each the suffix "dirEntry" */	
	~DirTree() throw() ;
	DirTree(const DirTree &) ;				// all modifiers are RESET in the destination object !  this->sort() is called.
	DirTree &operator=(const DirTree &) ;	// all modifiers are RESET in the destination object !  this->sort() is called.
	
	void load() ;
	
	bool showHidden() const ;
	const std::string &operator[](size_t) ;
	size_t size() ;
	off_t bytes() ;
	const std::string path() const ;
	signed short depth(size_t);

	// modifiers:
	void reset() ;									/* RESET ALL MODIFIERS TO DEFAULT */
	void reverse() ;                                // PRQA S 2502
	void relative(const std::string& newPath="") ;	/* made relative,also replace with a optional NEW path */
	void absolute() ;	/* Reset relative path */
	void regexp(const std::string& pattern="") ;	/* Apply regular expression to MATCH entries -- <EMPTY STRING> to reset */	
	void regexp_reject(const std::string& pattern="") ;	/* Apply regular expression to REJECT entries -- <EMPTY STRING> to reset */	
	void mindepth(signed short d=-1) ;	/* -1 for default value (any depth) */
	void maxdepth(signed short d=-1) ;	/* -1 for default value (any depth) */
	void sort_default() ;			/* Default sort order (it should be inode-ordered) */
	void sort_alpha() ;				/* Alphabetic sort order */
	void sort_ialpha() ; 			/* Alphabetic case-insensitive sort order */
	void show_files() ;					/* Show files */
	void hide_files() ;					/* Hide files */
	
private:
	enum SortOrder { SORT_DEFAULT=0x00, SORT_ALPHA=0x01, SORT_IALPHA=0x02 } ;

	DirTree() ;	/* declared but not used */
	const std::string &operator[](size_t) const ;

	// Container structure -- recursive, of course !
	struct DirStruct // PRQA S 2173, 2175
	{
        // PRQA S 2100 4
		std::vector <std::string> dirPath ;
		std::vector <std::string> fileEntries ;
		std::vector <DirStruct> dirEntries ;
        DirStruct() : 
            dirPath(), 
            fileEntries(), 
            dirEntries()
        {}
        DirStruct(const DirStruct &r) :
            dirPath(r.dirPath), 
            fileEntries(r.fileEntries), 
            dirEntries(r.dirEntries)
        {}
           
        DirStruct & operator= (const DirStruct &r) 
        {
            if (&r != this) {
                dirPath = r.dirPath ;
                fileEntries = r.fileEntries ;
                dirEntries = r.dirEntries ;
            }
            return *this ;
        } 
	    ~DirStruct() throw() {}

	} ;
	
	void generateTree(std::vector< std::pair<std::string, signed short> >&, const DirStruct& dirStruct, signed short int depth) ;
	void updateFiles() ;

	static void load(DirStruct& dirStruct, const std::vector<std::string>& dirPath, size_t depth, bool hidden) ;
	static std::string pathVectorToString(const std::vector<std::string>&, bool relative=false, const std::string &newPath="") ;
	static std::string lastChar(const std::string&) ;
	static off_t fileSize(const std::string&) ;

	static void dumpDirStruct(const DirStruct&) ;


private:
	
	struct DirStruct _rootDir ;
	
	std::vector< std::pair<std::string, signed short> > _files ; // <file, depth>
	std::string _path ;
	bool _hidden ;
	off_t _bytes ;
	bool _dumpDirEntries ;
	std::string _dirEntry ;
	signed short _minDepth ;
    signed short _maxDepth ;
	bool _relative ;
	std::string _newPath ;
    std::string _regExpPattern ;
    std::string _regExpRejectPattern ;
	bool _reverse ;
	bool _regExp ;
    bool _regExpReject  ;
	enum SortOrder _sortOrder ;
	bool _showFiles ;
	bool _updated ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DirTree) ;
} ;

_ACS_END_NAMESPACE

#endif	/* _Dir_H_ */

