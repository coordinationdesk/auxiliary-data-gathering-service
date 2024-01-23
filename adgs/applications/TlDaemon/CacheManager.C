// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: ThinLayerDaemon File Cache Manager $

	$Id$

	$Author$

	$Log$
	Revision 3.3  2017/09/12 08:16:58  marfav
	Message added when providing files in WD
	
	Revision 3.2  2016/11/04 22:09:25  marfav
	Change permissions management when dealing with directory products
	
	Revision 3.1  2016/10/12 08:09:06  marfav
	S3MPM-843
	Cache is now able to use copy instead of hardlink if configured
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.19  2016/10/11 08:23:37  marfav
	S3MPM-842
	Using libFile Cache object to manage a rolling cache
	Cache size is configurable and all filetypes are cached by default
	Linked products are put read-only (mode 0444)
	In case of Header+Binary split, unsplit files are removed ASAP to avoid cache misuse
	
	Revision 2.18  2016/05/11 11:53:57  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.17  2016/03/29 17:16:35  marpas
	adopting new db I/F
	
	Revision 2.16  2015/12/01 15:49:53  marfav
	Fixing metric types
	
	Revision 2.15  2015/11/26 15:14:25  marfav
	mc_params are now lowercase
	
	Revision 2.14  2015/08/12 14:21:43  marfav
	APF-271 work in progress
	
	Revision 2.13  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.12  2013/03/14 15:05:38  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.11  2013/03/12 17:57:34  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.10  2013/03/11 16:10:33  marfav
	FileTypeDetails now can get connection for initialization
	
	Revision 2.9  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.8  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.7  2011/09/16 14:17:18  marfav
	Addedd support to stream input files
	
	Revision 2.6  2009/07/09 13:42:15  marfav
	splitMatchExt parameters modified to get header from the bottom of the file
	
	Revision 2.5  2009/03/18 13:29:57  marfav
	Using database table to retrieve downloaded file extensions
	
	Revision 2.4  2009/03/17 15:59:02  marfav
	Addedd modification date in cached files prefix
	to handle replaceFile inventory mode
	
	Revision 2.3  2008/04/24 14:48:40  marfav
	Addedd support for file version information
	Extensions retrieved from SDF will not be stripped anymore
	
	Revision 2.2  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.1  2006/07/28 09:04:06  marfav
	Download strategy improved
	If file exists in local WD no action are performed on the cache
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.5  2006/01/19 18:00:58  marfav
	Using the local stored file if multiple download requests for the same file are issued
	
	Revision 1.4  2005/06/15 08:47:42  integrator
	Added libStorageManager.
	Now include and use dbDownloadFile and dbUploadFile
	
	Revision 1.3  2005/05/31 16:21:26  marpas
	file action interface changed
	
	Revision 1.2  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.1  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	

*/


#include <CacheManager.h>
#include <dbDownloadFile.h>

#include <dbFileActionScopeGuard.h>
#include <dbFileActionFactoryChain.h>
#include <dbDownloadFile.h>
#include <dbInventoryObject.h>

#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <Dir.h>
#include <Application.h>
#include <dbConnectionPool.h>

#include <FileTypeDetails.h>

#include <MCFilterables.h>

#include <unistd.h>


using namespace std;
using namespace acs;
using namespace acs::mc;


// define this to activate the "dummy" downloader
// that acts like all the downloads from the archive happen
// but doesn't download a bit !!
// #define BYPASS_ARCHIVE_DOWNLOAD

// The files in the cache are will be called 
// MODIFICATIONDATE_VERSION_NAME.extensions

namespace { // Unnamed namespace

static const char *_tldRootKey = "TlDaemon";
static const char *_cacheableListCountKey = "List_Of_Cacheable_Filetypes.count.value";
static const char *_cacheableListKey = "List_Of_Cacheable_Filetypes";
static const char *_cacheableTypeKey = "Cacheable_Filetype";
static const char *_enableCacheKey = "EnableCache"; // Default is true
static const char *_cacheSizeKey = "CacheSize"; // this is in gigabytes (float) default is 10 giga
static const char *_useHardLinksKey = "UseHardLinksFromCache";

};

ACS_CLASS_DEFINE_DEBUG_LEVEL(CacheManager)


CacheManager::CacheManager () :
_cachePath (""),
_cacheActive (false),
_cacheableTypes (),
_cacheSize (10),
_fileCache(),
_useHardLinks(true)
{
	// Cache disabled and default values set... no init performed
}


CacheManager::CacheManager (const std::string& cachePath) : 
    _cachePath (cachePath),
    _cacheActive (true),
    _cacheableTypes (),
	_cacheSize (10),
	_fileCache(),
	_useHardLinks(true)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	init();
}


CacheManager::CacheManager(const CacheManager& m) :
    _cachePath(m._cachePath),
	_cacheActive(m._cacheActive),
	_cacheableTypes(m._cacheableTypes),
	_cacheSize (m._cacheSize),
	_fileCache(),
	_useHardLinks(m._useHardLinks)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	initFileCache();
}

CacheManager& CacheManager::operator= (const CacheManager& m)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	ThreadSafe::Lock lock (m);
	ThreadSafe::Lock lockme (*this);
	if (this != &m)
	{
		_cachePath = m._cachePath;
		_cacheActive = m._cacheActive;
		_cacheableTypes = m._cacheableTypes;
		_cacheSize = m._cacheSize;
		_useHardLinks = m._useHardLinks;
		initFileCache();
	}
	return *this;
}


void CacheManager::init()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		rsResourceSet& rs = ConfigurationSingleton::instance() ->get();
		rsPushNSpace nspace_tld (rs, _tldRootKey);

		{
			// Optional key - Reset to default values just for clarity
			// Active cache and 10 GigaBytes of cache area
			_cacheActive = true;
			_cacheSize = 10; 
			_useHardLinks = true;
			rs.getValue (_enableCacheKey, _cacheActive, false);
			rs.getValue (_cacheSizeKey,_cacheSize, false);
			rs.getValue (_useHardLinksKey, _useHardLinks, false);
			size_t filetypes = 0;
			if (rs.hasKey (_cacheableListCountKey))
			{
				rs.getValue(_cacheableListCountKey, filetypes);
				rsPushNSpace nspace_ft (rs, _cacheableListKey);
				for (size_t i=0; i<filetypes; i++)
				{
					ostringstream os;
					os << _cacheableTypeKey <<"["<<i<<"]";
					string theFileType;
					rs.getValue(os.str(), theFileType);
					ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Adding cacheable filetype [" << theFileType << "]")
					_cacheableTypes.push_back (theFileType);
				}
			}
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Added " << _cacheableTypes.size() << " cacheable filetypes")
		}
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("CacheManager:: Unable to load configuration settings. Exception dump follows.");
		ACS_LOG_NOTIFY_EX (ex);
	}

	// Enter here just in the case the cache is active
	if (_cacheActive)
	{

		// Check if the path exists, if not try to create it
		// If creation is not possible the manager will always download
		// the requested files into the working directory


		if (!File::exists(_cachePath))
		{
			// The path does not exists... try to create it
			try
			{
				File::mkdir(_cachePath);
			}
			catch (exception&)
			{
				_cacheActive = false;
				ACS_LOG_WARNING ("CacheManager:: Unable to create cache path [" << _cachePath << "]");
			}
		}
		else
		{
			try
			{
				if (!File::isDir(_cachePath))
				{
					_cacheActive = false;
					ACS_LOG_WARNING ("CacheManager:: The cache path [" << _cachePath << "] is not a directory");
				}
			}
			catch (exception&)
			{
				_cacheActive = false;
				ACS_LOG_WARNING ("CacheManager:: WARNING Unable to check cache path [" << _cachePath << "]");
			}
		}

	}

	// The final step is to init also the internal file cache
	// This will delete any cache instance case the cache is not active at all
	initFileCache();

	ACS_LOG_INFO ("Cache status on path [" << _cachePath << "] is " << (_cacheActive?"ENABLED":"DISABLED"))
	if (_cacheActive)
	{
		ACS_LOG_INFO ("Cache size is " << _cacheSize << " Gb. Cache deliveries by means of " << (_useHardLinks?"HardLink.":"Copy."));	
	} 

}


void CacheManager::initFileCache ()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	if (_cacheActive)
	{
		_fileCache.reset ( new Cache (_cachePath, off_t(_cacheSize * 1024.0 * 1024.0 * 1024.0 ), 0.4));
		_fileCache->setInitHighAccuracy();
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Cache is not active. There will be no caching.")
		_fileCache.reset(0);
	}

	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

CacheManager::~CacheManager () throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

bool CacheManager::isCacheable (const std::string& fileType) const
{
	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "requested check for filetype [" << fileType << "]")

	bool retval = false;

	// In case of empty list report that any filetype can be cached
	if (_cacheableTypes.empty())
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Cacheable filetype list is empty. Will cache anything.")
		retval = true;
	}
	else
	{
		retval = ( find (_cacheableTypes.begin(), _cacheableTypes.end(), fileType) != _cacheableTypes.end());
	}

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "filetype [" << fileType << "] is " << (retval?"Cacheable":"NOT Cacheable"))

	return retval;
}

pair<string, string> CacheManager::getFileNameToSearch (dbInventoryObject& obj , const string& thePath, bool addExtraInfo) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	string prefix;

	if (addExtraInfo)
	{
		// Add the filename prefix
		prefix = computePrefix (obj);
	}

	string binaryName(""), headerName("");

	dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);
	FileTypeDetailsStruct ftypeDetails = FileTypeDetails::instance(conn.getKey())->getData(obj.getFileType());

	// Evaluete the parts composing the name
	File::evalNamesSplitMatchExt (
		prefix + obj.getFileName(), 
		binaryName, 
		headerName, 
		ftypeDetails.singleExt,
		ftypeDetails.dblExt,
		ftypeDetails.hdrExt,
		ftypeDetails.manageSingleExt,
		obj.getHeaderSize() > 0 );


	string fileToSearch = thePath + "/" + binaryName;
	string headerToSearch ("");

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "The right filename to check is [" << fileToSearch << "]");

	if (obj.getHeaderSize() != 0)
	{
		// Split file - return also the header part
		headerToSearch = thePath + "/" + headerName;
	}
	else
	{
		// empty header
		headerToSearch = "";
	}

	return make_pair (fileToSearch, headerToSearch);
	//return fileToSearch;
}


bool CacheManager::existsInPath (dbInventoryObject& obj, const string& pathToSearch, bool addExtraInfo) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	pair<string, string> fileToSearch = getFileNameToSearch (obj, pathToSearch, addExtraInfo);

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Searching for [" << fileToSearch.first << "] and header [" << fileToSearch.second << "]");

	bool retval = File::exists (fileToSearch.first);

	// If the HDR file makes sense take it into account for files availability 
	if (!fileToSearch.second.empty())
	{
		retval &= File::exists (fileToSearch.second);
	}

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "File [" << fileToSearch.first << "] " << (retval?"exists ":"DOES NOT exist ") << "in the path")

	return retval;
}

bool CacheManager::isInCache (dbInventoryObject & obj) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	bool retval = existsInPath (obj, _cachePath, true);

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "File [" << obj.getFileName() << "] " << (retval?"":"NOT ") << "present in cache")

	return retval;
}


string CacheManager::deliverFromCache (acs::dbInventoryObject &obj, const std::string& path, bool hardlink) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	vector< pair<string, string> > filesToLink;
	string destinationName;
	string sourceName;

	string binaryName, headerName;

	dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);
	FileTypeDetailsStruct ftypeDetails = FileTypeDetails::instance(conn.getKey())->getData(obj.getFileType());

	// Compute the cached filename prefix
	string prefix = computePrefix (obj);

	// Evaluete the parts composing the name

	File::evalNamesSplitMatchExt (
		obj.getFileName(), 
		binaryName, 
		headerName, 
		ftypeDetails.singleExt,
		ftypeDetails.dblExt,
		ftypeDetails.hdrExt,
		ftypeDetails.manageSingleExt,
		obj.getHeaderSize() > 0 );

	if (obj.getHeaderSize())
	{
		// This file has been split into header and binary part.. have to hardLink both

		// Binary part
		destinationName = binaryName;
		sourceName = prefix + destinationName;
		filesToLink.push_back (make_pair (sourceName, destinationName));

		// Header Part
		destinationName = headerName;
		sourceName = prefix + destinationName;
		filesToLink.push_back (make_pair (sourceName, destinationName));    
	}
	else
	{
		// Apply unsplit extension
		destinationName = binaryName;
		sourceName = prefix + destinationName;
		filesToLink.push_back (make_pair (sourceName, destinationName));    
	}

#ifndef BYPASS_ARCHIVE_DOWNLOAD
	for (vector< pair<string,string> >::iterator it = filesToLink.begin(); it != filesToLink.end(); it++)
	{
		string source = _cachePath + "/" + it->first;
		string dest = path + "/" + it->second;

		// Touch the file to move it away from the cache dispose list head
		// Use the file in the cache, to create the lock entry and allow cache management
		// but do not lock it
		_fileCache->use (it->first, false);
		File::touch (source);

		if (hardlink)
		{
			// Hardlink here
			ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Linking [" << source << "] to [" << dest << "]")
			if (link(source.c_str(), dest.c_str()))
			{
				// error during hardlink... throw an exception to the caller
				ostringstream os;
				os << "Unable to link [" << source << "] to [" << dest << "]";
				ACS_THROW (exIOException(os.str(), errno));
			}

			// Put read only permissions on the file to avoid overwrite
			File::chmod_rec (dest, 0222, File::SubPerm);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Copying [" << source << "] to [" << dest << "]")

			// Copy
			File::copy (source, dest);

			// Allow modification of the file. Files in cache should have the read only permissions
			File::chmod (dest, 0220, File::AddPerm);
		}

	}
#endif //BYPASS_ARCHIVE_DOWNLOAD

	// the file to be used in the JobOrder is the first of the array
	string retval = path + "/" + filesToLink[0].second;

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Returning [" << retval << "]")

	return retval;

}

string CacheManager::getFinalFileName (dbInventoryObject & obj, const string& path) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// Acquire the lock on the thread
	ThreadSafe::Lock theLock(*this);

	return getFileNameToSearch(obj, path, false).first;
}

string CacheManager::getFile (dbInventoryObject& obj, const string& path, bool isStream) const
{   
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	ThreadSafe::Lock theLock(*this);
	string downloadedFile("");

	// APF-271
	size_t filesize = obj.getDataSize() + obj.getHeaderSize();

	if (existsInPath (obj, path, false))
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "File [" << obj.getFileName() << "] detected in the WD. Using local version.")

		downloadedFile = getFinalFileName (obj, path);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "File [" << obj.getFileName() << "] not detected in the WD. Needs download");

		if (_cacheActive && isCacheable (obj.getFileType()))
		{
			try
			{
				// Try to use the cache
				if (!isInCache (obj))
				{
					string relPathName = computePrefix(obj)+obj.getFileName();
					string cacheName = _cachePath+"/"+relPathName;
					// Make free space for the files in the cache area
					_fileCache->makeSpaceReserveAndUse(relPathName, filesize);
					_fileCache->release (relPathName);

					ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Cachig file [" << obj.getFileName() << "]");

					string realName = downloadFile (obj, _cachePath, true, isStream);

					if (realName != cacheName)
					{
						// In case the binary part has a name different than the unsplit part 
						// get rid of the unsplit file at all
						bool lockFileFound = false;
						string lockFile = _fileCache->searchLockFilePath( relPathName, lockFileFound );
						File::unlink ( _cachePath + "/" + relPathName);
						if (lockFileFound)
						{
							File::unlink ( _cachePath + "/" + lockFile);
						}
					}

				}
				ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Using cache version of [" << obj.getFileName() << "]");
				downloadedFile = deliverFromCache (obj, path, _useHardLinks);
			}
			catch (exception& ex)
			{
				ACS_LOG_NOTIFY_EX(ex);
				ACS_LOG_WARNING ("Unable to use cached version. Downloading directly in the working directory.");
				downloadedFile = downloadFile (obj, path, false, isStream);
			}
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Downloading directly in WD [" << obj.getFileName() << "]")

			downloadedFile = downloadFile (obj, path, false, isStream);
		}
	}

   
	ACS_LOG_MC_INFO( mc_param("filename", obj.getFileName()) << mc_param("fileversion", obj.getFileVersion()) <<
                     mc_param ("filesize", filesize) << mc_metric(Metric_IN)) ;
	return downloadedFile;
}


string CacheManager::computePrefix (dbInventoryObject& obj) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// Compute the prefix of the cached files

	// Add the modification date (ISO8601) + "_"
	// plus the version of the file + "_"
	// as filename trailer

	ostringstream os;
	ostringstream result;

	obj.getModificationDate().asISO8601 (os, 6);
	result << os.str() << "_" << obj.getFileVersion() + "_"; 

	return result.str();
}

string CacheManager::downloadFile (dbInventoryObject& obj, const std::string& path, bool addExtraInfo, bool isStream ) const
{

	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	string prefix;

	if (addExtraInfo)
	{
		// Add the filename prefix
		prefix = computePrefix (obj);
	}

	string fileName = prefix;

	string completePath = path + "/" + prefix + obj.getFileName();
	string returnPath ("");

#ifdef BYPASS_ARCHIVE_DOWNLOAD
	excout << "CacheManager::download: simulating download of " << completePath << "\n" ;
#else //BYPASS_ARCHIVE_DOWNLOAD

	if (File::exists (completePath))
	{
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Cleaning up existing file [" << completePath << "]")
		File::unlink (completePath);
	}

	dbDownloadFile* downloader = dynamic_cast<dbDownloadFile*>(dbFileActionFactoryChain::instance()->newDownloadAction(*(Application::instance()->getDefaultConnectionPool()),obj, completePath ));
	if (!downloader)
	{
		exIllegalValueException e("CacheManager::download: Dynamic_cast failure on dbDownloadInventoryFile object.");
		ACS_THROW(e);
	}
	dbFileActionScopeGuard guard(downloader);

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Start downloading " << completePath);

	downloader->start() ;
	while (downloader->downloading() )
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Still downloading " << completePath);

		downloader->waitExitOrTimeout (1000) ;
	}

#endif //BYPASS_ARCHIVE_DOWNLOAD

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Download completed")


	string binaryName, headerName;
	dbConnectionWrapper connWrp ( *( Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);

	FileTypeDetailsStruct ftypeDetails = FileTypeDetails::instance(conn.getKey())->getData(obj.getFileType());

	// Evaluete the parts composing the name
	File::evalNamesSplitMatchExt (
		completePath, 
		binaryName, 
		headerName, 
		ftypeDetails.singleExt,
		ftypeDetails.dblExt,
		ftypeDetails.hdrExt,
		ftypeDetails.manageSingleExt,
		obj.getHeaderSize() > 0 );


	// Now split the files
#ifndef BYPASS_ARCHIVE_DOWNLOAD
	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Splitting header part from binary file if needed");

	// Let the File class do the dirty work
	File::splitMatchExt (
		completePath, 
		binaryName, 
		headerName, 
		obj.getDataSize(), 
		obj.getHeaderSize(), 
		ftypeDetails.singleExt,
		ftypeDetails.dblExt,
		ftypeDetails.hdrExt,
		ftypeDetails.manageSingleExt,
		false,
		true,
		!isStream ); // check size if not a stream

	if (binaryName != completePath)
	{
		File::unlink (completePath);
	}

#endif //BYPASS_ARCHIVE_DOWNLOAD

	returnPath = binaryName;

	return returnPath;
}


