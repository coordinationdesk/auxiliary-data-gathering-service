/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	Revision 5.7  2016/03/10 15:46:06  lucio.pulvirenti
	Bug fixed in load method: errno set to 0 at every loop iteration.
	
	Revision 5.6  2016/03/08 14:59:13  marpas
	removing mutex in File class
	This was originally due to some system call thread safety issues (e.g. readdir)
	The latter was explicitely changed into readdir_r call
	Other syscall were checked and up to now seems there is none having thread safety problems
	
	Revision 5.5  2015/05/20 10:28:55  marfav
	S2PDGS-1224: Dir object can now return also broken links if requested
	QACPP warnings removed
	
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
	
	Revision 2.26  2013/01/23 16:41:01  chicas
	Added setDifference
	
	Revision 2.25  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.24  2012/10/04 13:25:59  lucio.pulvirenti
	DirTree: show_files and add_files methods ported from PDS2 environment implementation.
	
	Revision 2.23  2012/10/04 12:54:45  enrcar
	EC:: DirTree modified. The scan cycle is NOT interrupted if a broken symlink is found or a not-accessible (errno: EACCES) directory is encountered.
	
	Revision 2.22  2012/05/21 12:19:28  matteo.airoldi
	Wrong Class instantation
	
	Revision 2.21  2012/02/13 10:59:04  marpas
	refactoring in progress
	
	Revision 2.20  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.19  2011/08/10 10:01:08  marpas
	missing include added (cstring)
	
	Revision 2.18  2011/08/02 16:13:04  enrcar
	EC:: useless methods removed
	
	Revision 2.17  2011/08/02 14:26:20  enrcar
	EC:: sort() method renamed: reset(). Added three new modifiers: sort_alpha (alphabetic sort); sort_ialpha(case-insents. alphabetic sort); sort_default(default sort)
	
	Revision 2.16  2011/05/19 15:49:24  chicas
	EC:: When scanning a directory, the type of each entry is no more obtain using readdir(3), since this would be unsupported on LINUX XFS filesystems (05/2011). A more standard stat(2) is performed instead. 
	
	Revision 2.15  2011/04/29 13:43:34  enrcar
	EC:: c++rules
	
	Revision 2.14  2011/04/19 16:21:44  marpas
	DirTree doc and behaviour improved
	
	Revision 2.13  2011/04/08 12:41:47  enrcar
	EC:: Added public method DirTree::bytes()
	
	Revision 2.12  2011/03/29 15:26:36  marant
	EC:: size_t instead of ulong used for overloading of stl members: size() and operator[] for classes Dir and DirTree
	
	Revision 2.11  2011/03/07 09:27:57  enrcar
	EC:: Added modifier regexp_reject in order to REJECT entries matching a pattern
	
	Revision 2.10  2011/02/24 15:38:43  enrcar
	EC:: dirTree::generateTree method. This method happened to be too slow to reverse a large amount of files -- modified in order to always PUSH_BACK entries (even in reverse mode)
	
	Revision 2.9  2011/02/15 15:10:41  enrcar
	EC:: Dir::bytes() member added
	
	Revision 2.8  2011/02/11 13:25:11  enrcar
	EC:: regexp modifier added.
	
	Revision 2.7  2011/01/27 14:18:14  lucio.pulvirenti
	EC:: copy/assignment operator. _dumpDirEntries parameter was not copied ! FIXED.
	
	Revision 2.6  2011/01/27 13:33:51  enrcar
	EC:: DirTree::  Copy/Assignment c'tors re-read the tree from filesystem. FIXED: the tree is merely copied from the source object.
	
	Revision 2.5  2011/01/25 16:48:51  enrcar
	EC:: Several changes, in order to internally manage directory paths as string vectors (instead of a slash-separated string), to allow a faster navigation and easier evaluation of path depth.
	Modifiers handled.
	Added modifiers: relative(), absolute(), mindepth(), maxdepth()
	
	Revision 2.4  2011/01/24 10:05:06  enrcar
	EC::  DirTree:  functions re-sorted in file.  reverse method added.
	
	Revision 2.3  2011/01/21 16:58:04  enrcar
	EC:: DirTree class added
	
	Revision 2.2  2009/05/14 13:52:13  enrcar
	EC:: c++ rules
	
	Revision 2.1  2009/03/13 15:26:23  marpas
	method sort added - no regression on Class behaviour
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/12/30 22:06:41  marpas
	headers fixed
	
	Revision 1.5  2003/04/30 16:25:56  marpas
	no more namespace std used
	
	Revision 1.4  2003/04/15 17:28:06  marpas
	GCC3.2
	
	Revision 1.3  2003/02/12 18:35:59  marpas
	Exception text changed
	
	Revision 1.2  2003/02/04 14:27:10  paoscu
	Now load() delete previous results to prevent duplications.
	
	Revision 1.1.1.1  2002/10/04 16:58:31  paoscu
	Import libFile
	
		

*/

#include <exException.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <Dir.h>
#include <RegExp.h>

#include <sstream>
#include <algorithm>
#include <cstring>

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
   


_ACS_BEGIN_NAMESPACE(acs)

using std::string ;
using std::vector ;
using std::pair ;

//****************************************************************************************************************************
//
//  CLASS Dir :
//
//****************************************************************************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(Dir) 


Dir::Dir(const string &path, bool hidden, bool brokenln) : 
	_path(path),
	_hidden(hidden),
	_reportBrokenLinks (brokenln)
{
	load(DirNopPredicate) ;
}


bool Dir::showHidden() const noexcept 
{
	return _hidden ;
}


bool Dir::reportBrokenLinks() const noexcept 
{
	return _reportBrokenLinks ;
}


const string &Dir::operator[](size_t i) const 
{
	ACS_COND_THROW(i >= size(),exBoundsException() << "Dir::entry " << i << " greater than " << size()-1) ; 
	
	return _files[i] ;	/* return the i-th file */
}

void Dir::setDifference(  Dir & otherDir, std::vector<std::string > & output )  
{
    sort();
    otherDir.sort();

    output.clear();
    
    set_difference( _files.begin(), _files.end(), 
                    otherDir._files.begin(), otherDir._files.end(), back_inserter(output));


}

size_t Dir::size() const
{
	return _files.size() ;
}

bool Dir::empty() const
{
	return _files.empty() ;
}

bool Dir::empty(const string& path, const bool includeHidden)
{
	bool returnValue = true ;

	// Scan directory
	::DIR *dir = ::opendir(path.c_str()) ;
	ACS_COND_THROW(!dir,exIOException("Cannot read directory \"" + path +"\"", errno)) ; 

	// reentrant version of readdir is used here
	// see man 3 readdir for details on the malloc used here

	size_t len = offsetof(struct dirent, d_name) + pathconf(path.c_str(), _PC_NAME_MAX) + 1;

	struct dirent* entry = reinterpret_cast<struct dirent*> ( malloc(len) ); // NOSONAR - tricky: size to be computed
	// This will automatically free the memory used by entry at dtor time
	pattern::CMemoryScopeGuard theGuard (entry);

	struct dirent* result = nullptr ;

	int readdirval = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	while ( ( 0 == (readdirval = readdir_r(dir, entry, &result) ) ) && ( result != nullptr) ) { 
#pragma GCC diagnostic pop

		std::string item = entry->d_name ;
		if ((item == ".") || (item == "..")) {
			continue ;	/* Ignore . and .. */
		}
		if (!includeHidden && (item[0] == '.')) {
			continue ;	/* Ignore hidden files */
		}

		returnValue = false;
		break;
	}

	::closedir(dir) ;

	ACS_COND_THROW(0 != readdirval, exIOException("Cannot read directory \"" + path+"\"", readdirval)) ; 

	return returnValue;
}

string Dir::path() const 
{
	return _path ;
}

namespace {
  bool compare_string_no_case(const string &x, const string &y) 
  { return StringUtils::compareNoCase(x,y) <= 0 ; } 
}


void Dir::sort() 
{
	std::sort(_files.begin(), _files.end(), compare_string_no_case) ;
}


signed long long int Dir::bytes() const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  Directory path: \"" << _path << "\"") ;

	signed long long int r=0 ;
	for (auto const &f : _files)
	{
		const string fullPath = ( _path + "/" + f ) ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Processing entry: \"" << fullPath << "\"") ;

		struct stat stat_struct ; 
		ACS_COND_THROW( 0 != stat(fullPath.c_str(), &stat_struct), exIOException ("Cannot stat entry \"" + _path +"\"", errno) ) ;

		// Return the stat (if it's a file) or recursively process directory
		signed long long s = ( S_ISDIR(stat_struct.st_mode)? Dir::bytes(fullPath) : stat_struct.st_size ) ; 
		
		r += s ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Size for entry: \"" << fullPath << " is: " << s << " bytes  (so far: " << r << ")") ;
	}
	
	return r ;	// Occupation of all entries (bytes)
}	


signed long long int Dir::bytes(const string& d)  
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for directory: \"" << d << "\"") ;

	Dir tmpDir(d) ;

	return tmpDir.bytes() ;	// Occupation of all directory entries (bytes)
}


//****************************************************************************************************************************
//
//  CLASS DirTree :
//
//****************************************************************************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(DirTree) 

//
//  DirTree::DirTree (2 params)
//
DirTree::DirTree(const string &path, bool hidden) : 
	_path(path),
	_hidden(hidden)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: !" << _path << "!  showHidden: " << std::boolalpha << _hidden) ;

	load() ;
}


//
//  DirTree::DirTree (3 params)
//
DirTree::DirTree(const string &path, const string &dirEntry, bool hidden) : 
	_path(path),
	_hidden(hidden),
	_dirEntry (dirEntry)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: !" << _path << "!  dirEntry: !" << _dirEntry << "!  showHidden: " << std::boolalpha << _hidden) ;

	load() ;
}


//
//  DirTree::DirTree (copy)
//
DirTree::DirTree(const DirTree &o) :
	_rootDir(o._rootDir),
	_path(o._path),
	_hidden(o._hidden),
	_bytes(o._bytes),
	_dumpDirEntries(o._dumpDirEntries),
	_dirEntry(o._dirEntry),
	_minDepth(o._minDepth), 
	_maxDepth(o._maxDepth),
	_relative(o._relative), 
	_newPath(o._newPath),
	_regExpPattern(o._regExpPattern),
	_regExpRejectPattern(o._regExpRejectPattern),
	_reverse(o._reverse),
	_regExp(o._regExp),
	_regExpReject(o._regExpReject),
	_sortOrder(o._sortOrder)
{
	reset() ;    // Reset all modifiers to default
	updateFiles() ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: !" << _path << "!  dirEntry: !" << _dirEntry << "!  showHidden: " << std::boolalpha << _hidden
			<< "\t_updated: " << std::boolalpha << _updated << " size :" << size()) ;
}


//
//  DirTree::DirTree (assignment)
//
DirTree &DirTree::operator=(const DirTree &o) 
{
	// = operator
	if (this != &o) {
		// Copy parameters set in original c'tor :
		_path = o._path ;
		_hidden = o._hidden ;
		_dirEntry = o._dirEntry ;
		_dumpDirEntries = o._dumpDirEntries ;
		_bytes = o._bytes ;

		// Copy DiRStruct container :
		_rootDir = o._rootDir ;

		// Modifiers: force default view :
		_updated = true ;
		reset() ;    // Reset all modifiers to default
		updateFiles() ;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: !" << _path 
			<< "!  dirEntry: !" << _dirEntry << "!  showHidden: " << std::boolalpha << _hidden 
			<< "\t_updated: " << std::boolalpha << _updated << " size :" << size()) ;
	
	return *this ;
}


//
//  load
//
void DirTree::load() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. _path: !" << _path << "!") ;

	vector <string> pathAsVector ;
	pathAsVector.push_back(_path) ;

	load (_rootDir, pathAsVector, 0, _hidden) ;
	
	reset() ;		// Reset parameters 
	
	updateFiles() ;	// Generate _files vector

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ;
}


namespace {
  bool compare_dirtree_files_no_case(const pair<std::string, signed short> &x, 
                                     const pair<std::string, signed short> &y) 
  { return StringUtils::compareNoCase(x.first,y.first) <= 0 ; }


  bool compare_dirtree_files(const pair<std::string, signed short> &x, 
                             const pair<std::string, signed short> &y) 
  { return strcmp(x.first.c_str(), y.first.c_str()) <= 0 ; } 
}

void DirTree::updateFiles()
{   
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		dumpDirStruct(_rootDir) ;
	ACS_CLASS_END_DEBUG

	_files.clear() ; 
	_bytes = 0 ;
	generateTree(_files, _rootDir, 0) ;
	switch(_sortOrder)
	{
    case DirTree::SORT_ALPHA :
		sort(_files.begin(), _files.end(), compare_dirtree_files); 
        break ;
    case DirTree::SORT_IALPHA :
		sort(_files.begin(), _files.end(), compare_dirtree_files_no_case); 
        break ;
    default:
		break ;
	}

	_updated = false ;	// reset update flag
			
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		string absPath = pathVectorToString(_rootDir.dirPath) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished.  size: " << size() << "  Rootdir: " << absPath) ;
    ACS_CLASS_END_DEBUG
} 


//
//  sort MODIFIER (the default one)
//
void DirTree::reset() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	
	// Reset all modifiers to standard values
	_reverse = false ;	// RESET reverse setting ;
	_regExp = false ;	// RESET regular expression ;
	_regExpReject = false ;	// RESET reject regular expression ;
	absolute() ;		// RESET (TURN OFF) relative setting ;
	mindepth(-1) ;		// RESET mindepth setting ;
	maxdepth(-1) ;		// RESET mindepth setting ;
	_sortOrder = SORT_DEFAULT ;	// RESET output order
	_showFiles = true;
	_updated = true ;
}


//
//  reverse MODIFIER
//
void DirTree::reverse() 
{
 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	_reverse = true ; 
    _updated = true ;
}


//
//  regexp MODIFIER
//
void DirTree::regexp(const std::string& pattern) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Pattern: \"" << pattern << "\"") ;
	
	_regExp = (pattern.length() != 0) ; // if pattern not provided, no regexp is applied
	
	_regExpPattern = pattern; 
    _updated = true ;
}


//
//  regexp_reject MODIFIER
//
void DirTree::regexp_reject(const std::string& pattern) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  Pattern: \"" << pattern << "\"") ;
	
	_regExpReject = (pattern.length() != 0) ; // if pattern not provided, no regexp is applied
	
	_regExpRejectPattern = pattern; 
    _updated = true ;
}



//
//  relative MODIFIER
//
void DirTree::relative(const std::string& newPath) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called.  New path: \"" << newPath << "\"") ;
	
	_relative = true ; _newPath = newPath; _updated = true ;
}


//
//  absolute MODIFIER
//
void DirTree::absolute() 
{
 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	_relative = false ; 
    _newPath = ""; 
    _updated = true ;
}


//
//  mindepth MODIFIER (-1 == default value [any depth])
//
void DirTree::mindepth(signed short d) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called:  minDepth = " << d ) ;
	
	_minDepth = d ; 
    _updated = true ; 	
}


//
//  maxdepth MODIFIER (-1 == default value [any depth])
//
void DirTree::maxdepth(signed short d) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called:  maxDepth = " << d ) ;
	
	_maxDepth = d ; 
    _updated = true ; 	
}


//
//  sort_alpha MODIFIER 
//
void DirTree::sort_alpha()
{
 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	_sortOrder = DirTree::SORT_ALPHA ; 
    _updated = true ; 	
}


//
//  sort_ialpha MODIFIER 
//
void DirTree::sort_ialpha()
{
 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	_sortOrder = DirTree::SORT_IALPHA ; 
    _updated = true ; 	
}


//
//  sort_default MODIFIER 
//
void DirTree::sort_default()
{
 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
	
	_sortOrder = DirTree::SORT_DEFAULT ; 
    _updated = true ; 	
}

//
//  hide_files MODIFIER 
//
void DirTree::hide_files()
{
 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	_showFiles = false ; 
    _updated = true ;    
}


//
//  show_files MODIFIER 
//
void DirTree::show_files()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	_showFiles = true ; 
    _updated = true ;    
}


//
//  showHidden
//
bool DirTree::showHidden() const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called:  returning: " << std::boolalpha << _hidden) ;
	
	return _hidden ;
}


//
//  operator[]
//
const string &DirTree::operator[](size_t i)  
{
	if (_updated) { updateFiles() ; }

	ACS_COND_THROW(i >= size(), exBoundsException()<< "DirTree::entry " << i << " greater than " << size()-1) ; 

	return _files[i].first ;	/* return the i-th file */
}


//
//  bytes
//
off_t DirTree::bytes() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;

	if (_updated) { updateFiles() ; }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. Returning: " << _bytes) ;
	return _bytes ;
}


//
//  size
//
size_t DirTree::size() 
{
	if (_updated) { updateFiles() ; }
	
	return _files.size() ;
}


//
//  path
//
string DirTree::path() const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called:  returning: " << _path) ;
	
	return _path ;
}


signed short DirTree::depth(size_t i) {
	if (_updated) { updateFiles() ; }

	ACS_COND_THROW(i >= size(), exBoundsException()<< "DirTree::entry " << i << " greater than " << size()-1) ; 

	return _files[i].second ;	/* return the i-th file depth */
}

//
//  generateTree(...) RECURSIVE FUNCTION
//
void DirTree::generateTree(vector<std::pair<string, signed short> >& entries, const DirStruct& dirStruct, signed short int depth ) 
{	
// EC:: Method is recursive in order to increase efficency

	const string sep = "/" ;

	string absPath = pathVectorToString(dirStruct.dirPath) ; 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "absPath: \"" << absPath << "\"") ;
    if (! absPath.empty() && (lastChar(absPath)!=sep)) { absPath += sep ; }
	string path = pathVectorToString(dirStruct.dirPath, _relative, _newPath) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: \"" << path << "\"") ;
	
	RegExp regExp ; 
    if (_regExp) { regExp.setReg(_regExpPattern) ; }
	RegExp regExpReject ; 
    if (_regExpReject) { regExpReject.setReg(_regExpRejectPattern) ; }
	
		  
	// recur for directories (before the files, if reversed)
	if ( _reverse && ((_maxDepth < 0) || (depth<=_maxDepth)) )
	{
		for (const auto &dir : dirStruct.dirEntries) {
			generateTree(entries, dir, depth+1 ) ; 
                                                                       // Recursive: concatenate each directory 
        }
	}


		// Dump directory entry (Before the files, if NOT reversed)
	if ((!_reverse) && _dumpDirEntries && ((_minDepth < 0) || (depth >= _minDepth)) && ((_maxDepth < 0) || (depth <= _maxDepth)) )
	{
		string e = path + _dirEntry ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dir: \"" << e << "\"" ) ;
		if ( e.empty() ) { e = "." ; }	// This is possible, for example in the very first entry, when relative is set to "" 
		if ( (!_regExp || regExp.match(e)) && (!_regExpReject || !regExpReject.match(e)) ) {

            entries.emplace_back(e, depth) ;	// Add current directory entry
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "de: \"" << e << "\" d" << depth << " size: " << entries.size() ) ;
        }
	}

	// Dump files contained in the directory (note that "depth+1" will be used for the comparisons)
	if( _showFiles && ((_minDepth < 0) || ((depth+1) >= _minDepth)) && ((_maxDepth < 0) || ((depth+1) <= _maxDepth)) )
	{ 
		string s = ( (! path.empty() && (lastChar(path)!=sep) )? sep : "" ) ; 
		for (const auto &file : dirStruct.fileEntries)
		{
			string e = (path + s +  file) ;
			string a = absPath + file ;
			if ( (!_regExp || regExp.match(e)) && (!_regExpReject || !regExpReject.match(e)) ) 
			{
				entries.emplace_back(e, depth + 1) ;	// Add current directory entry
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fe: \"" << e << "\" d" << depth + 1 << " size: " << entries.size() ) ;
				_bytes += fileSize(a) ;
			}
		}
	}

	// Dump directory entry (After all files, if reversed)
	if (_reverse && _dumpDirEntries && ((_minDepth < 0) || (depth >= _minDepth)) && ((_maxDepth < 0) || (depth <= _maxDepth)) )
	{
		string e = path + _dirEntry ;
		if ( e.empty() ) { e = "." ; }	// This is possible, for example in the very first entry, when relative is set to "" 
		if ( (!_regExp || regExp.match(e)) && (!_regExpReject || !regExpReject.match(e)) ) {
            entries.emplace_back(e, depth) ;	// Add current directory entry
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "de: \"" << e << "\" d" << depth << " size: " << entries.size() ) ;
        }
	}



	// recur for directories (after all the files, if NOT reversed)
	if ( (!_reverse) && ((_maxDepth < 0) || (depth<=_maxDepth)) )
	{
		for (const auto & dir : dirStruct.dirEntries ) {
			generateTree(entries, dir, depth+1 ) ; 
                                                                       // Recursive: concatenate each directory 
        }
	}
	
}


//
//  STATIC load(...) RECURSIVE
//
void DirTree::load(DirStruct& dirStruct, const vector<string>& dirPath, size_t depth, bool hidden) 
{
	// Obtain path from vector :
	const string path = pathVectorToString(dirPath) ;	
	struct stat stat_struct ; 
	
    ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "method called.  dirPath depth: " << depth << "  dirPath.size: " << dirPath.size()) ;
        std::ostringstream os ;
        os << "\tpathV: " ; 
        size_t dp_size = dirPath.size() ;
        for (size_t i=0; i<dp_size; ++i)
        {
            if (i>0) { os << "/ "; }
            os << "\"" << dirPath[i] << "\" " ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "\tpath: \"" << path << "\"") ;
    ACS_CLASS_END_DEBUG


	// Reset dirStruct 
	dirStruct.fileEntries.clear() ;
	dirStruct.dirEntries.clear() ;
	dirStruct.dirPath = dirPath ; 
	
	
	// Scan directory
	::DIR *dir = ::opendir(path.c_str()) ;
	if (!dir) {
		int err = errno;
		if (err == EACCES)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "  --  ignoring directory \"" << path << "\" (permission denied)") ;
			return ; 
		}
		else if (err == ENOENT)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "  --  ignoring directory \"" << path << "\" (probably removed during scan)") ;
			return ; 
		}
        else {
		    ACS_THROW(exIOException("Cannot read directory \"" + path +"\"", err)) ;
        }
	}
	
	
	try {
		
		// reentrant version of readdir is used here
		// see man 3 readdir for details on the malloc used here
        size_t len = offsetof(struct dirent, d_name) + pathconf(path.c_str(), _PC_NAME_MAX) + 1;

        struct dirent* entry = reinterpret_cast<struct dirent*> ( malloc(len) ); // NOSONAR - tricky: len 
		struct dirent* result = nullptr;

		// This will automatically free the memory used by entry at dtor time
		pattern::CMemoryScopeGuard theGuard (entry);

   		int readdirval = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		// Process all entries 
		while ( ( 0 == (readdirval = readdir_r(dir, entry, &result) ) ) && ( result != nullptr) ) {
#pragma GCC diagnostic pop
			errno = 0;
			string item = entry->d_name ;
			if ((item == ".") || (item == "..")) {
				continue ;	/* Ignore . and .. */
            }
			if (!hidden && (item[0] == '.')) {
				continue ;	/* Ignore hidden files */
            }

			const string fullPath = (path + "/" + item ) ;
			int status = ::stat(fullPath.c_str(), &stat_struct) ;
			int err = errno;
			bool isDir  = false ; 
			if ((0 != status) && (err == ENOENT))
			{
				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "  --  ignoring item \"" << fullPath << "\" (probably a broken symlink)") ;

				isDir = false ;	/* A broken symlink cannot be a directory */
			}
			else
			{
				ACS_COND_THROW( 0 != status, exIOException ("Cannot stat item \"" + fullPath +"\"", err) ) ;
				isDir = S_ISDIR(stat_struct.st_mode) ;
			}
			

			if (isDir)
			{
   				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					string s; 
                    for (size_t j=0; j<(depth+1); j++) { s += "*---" ; } 
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, s << "[" << fullPath << "]") ;
    			ACS_CLASS_END_DEBUG

				// Create new DirStruct :
				vector <string> newPath = dirPath ;
				newPath.push_back(item) ;	 
				struct DirStruct newDir ;
				load(newDir, newPath, (depth+1), hidden) ;	// Recursively load dirEntry 
						
				// Push dirEntry
				dirStruct.dirEntries.emplace_back(newDir) ;
			}
			else
			{
    			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					string s; 
                    for (size_t j=0; j<(depth+1); j++) { s += "*---" ; } 
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, s << fullPath) ;
    			ACS_CLASS_END_DEBUG

				// Push filename
				dirStruct.fileEntries.emplace_back(item) ;
			}
		}	// while
	    if (readdirval) { // an error interrupted cycle above
		    ::closedir(dir) ;
		    ACS_THROW(exIOException("Cannot read directory \"" + path+"\"", readdirval)) ; 
	    }
	}
	catch(const std::exception &) {
		::closedir(dir) ;
		throw ;
	}

	const int status ( ::closedir(dir) ) ;
	if ( (status < 0) && (0 != errno) ) {
		ACS_THROW(exIOException("Cannot read directory \"" + path +"\"", errno)) ;
	}
}


//
//  STATIC dumpDirStruct(...) RECURSIVE
//
void DirTree::dumpDirStruct(const DirStruct& dirStruct)
{
#if(0)
	ACS_LOG_DEBUG(SimpleDebugSignature << "Size of dirPath: "  << dirStruct.dirPath.size() 
		<< "  size of fileEntries: " << dirStruct.fileEntries.size() << "  size of dirEntries: " << dirStruct.dirEntries.size()); 
#endif

	for (auto const &path : dirStruct.dirPath ) {
		ACS_LOG_DEBUG(SimpleDebugSignature << "dirPath: \"" << path << "\"") ;	//	Dump pathname
	}
    
	for (auto const &file : dirStruct.fileEntries ) {
		ACS_LOG_DEBUG(SimpleDebugSignature << "fileEntries: \"" << file << "\"") ;	// Dump all file entries
    }

	for (auto const &dir : dirStruct.dirEntries ) {
		dumpDirStruct(dir) ;	// Dump all directory entries
    }
}


//
//  STATIC fileSize(...)
//
off_t DirTree::fileSize(const string& entry)
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called for file: \"" << entry << "\"") ;

	off_t sz = 0 ;
	struct stat s ; 
	if( 0 == stat( entry.c_str(), &s ) )
	{
		if (S_ISREG(s.st_mode)) 
		{
			sz = s.st_size ;	// Regular file: add entry size
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Processed entry: \"" << entry << "\"  size: " << sz) ;
		}
		else
		{
			// Skipped. Not a regular file
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Skipped since not a regular file: \"" << entry << "\"") ;
		}
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Invalid stat for entry: \"" << entry << "\"") ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished for file: \"" << entry << "\"  returning: " << sz) ;

	return sz ;
}


//
//  STATIC pathVectorToString(...)
//
string DirTree::pathVectorToString(const vector<string>& pathVector, bool relative, const string &newPath)  
{
// EC:: this method has multiple returns to keep its complexity as low as possible

	// EC:: Modify with care -- it should be the fastest possible
	size_t sz = pathVector.size() ;
	
	if ( 0 == sz ) { return "" ; } 	// 0 entries
	
	if ( (sz == 1) && relative) { return newPath ; }	// 1 entry, but relative ;
	
	const string sep = "/" ;
	string path ;
	
	if (!relative) { path = pathVector[0] ; }  // If relative, replace path with newPath
    else { path = newPath ;	}

	string s = ( (! path.empty() && (lastChar(path)!=sep))? sep : "" ) ; 

	if ( sz > 1 )
	{
		// 1+ entries
        size_t pv_size = pathVector.size() ;
		for (size_t i=1; i<pv_size; ++i)
		{
			path += ( s + pathVector[i] ) ;
			s = sep ;	
		}
	}

	return path ;
}


//
//  STATIC lastChar(...)
//
string DirTree::lastChar(const string& s) 
{
// EC:: this method has multiple returns to keep its complexity as low as possible

	size_t sl = s.length() ;
	
	if (!sl) { return "" ; }
	if (sl == 1) { return s ; }
	
	return string(s.c_str() + (sl - 1)) ; 
}

_ACS_END_NAMESPACE
 
