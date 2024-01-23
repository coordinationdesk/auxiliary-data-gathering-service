/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.14  2016/10/11 07:56:09  marfav
	S3MPM-842
	CacheManager is now a pointer
	
	Revision 2.13  2016/04/11 13:45:45  marpas
	adopting new db interfaces
	
	Revision 2.12  2016/03/29 17:16:35  marpas
	adopting new db I/F
	
	Revision 2.11  2015/05/13 12:39:04  damdec
	Compilation warning fixed.
	
	Revision 2.10  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.9  2013/03/14 15:05:38  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.8  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.7  2013/02/26 16:36:23  marfav
	Some compiler warning removed
	
	Revision 2.6  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.5  2011/09/16 14:17:19  marfav
	Addedd support to stream input files
	
	Revision 2.4  2009/09/10 14:44:16  marfav
	Using different entries in the SymTable for the file to download and
	the file to be used in the time intervals
	
	Revision 2.3  2008/08/01 15:38:28  marfav
	PreConditionChecker plugins framework added
	
	Revision 2.2  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.1  2006/10/24 15:58:14  marfav
	Addedd support for passing stream-encapsulated parameters file to special queries
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.38  2006/01/19 18:05:58  marfav
	Added support for mapping alternatives instead of filetypes when downloading inputs
	
	Revision 1.37  2005/12/06 17:03:45  marpas
	
	original extensions preserved
	
	Revision 1.36  2005/10/14 15:39:07  ivafam
	ARFClient and HSM libraries removed
	
	Revision 1.35  2005/06/15 08:47:42  integrator
	Added libStorageManager.
	Now include and use dbDownloadFile and dbUploadFile
	
	Revision 1.34  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.33  2005/03/09 09:30:43  marfav
	Order queue management changed
	now the orders are put in a waitlist when precondition fails
	the database will resume the waiting orders when the right input is inventoried
	
	Revision 1.32  2005/03/03 11:49:09  marfav
	Precondition check queries now use only one db connection per order
	
	Revision 1.31  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	
	Revision 1.30  2005/01/10 11:02:31  marfav
	Using factory for instantiate dbDownloadInventoryFile objects
	
	Revision 1.29  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.28  2004/10/19 14:19:36  marfav
	Old code removed
	
	Revision 1.27  2004/10/19 08:14:50  marfav
	Added support for satellite and mission in queries
	
	Revision 1.26  2004/09/22 14:19:35  marfav
	Align to new implementation of query OrderBy clause
	
	Revision 1.25  2004/08/03 14:01:52  marfav
	Align to new version of libTask using TimeRange instead of dbGeoTimeRange
	
	Revision 1.24  2004/04/22 07:24:55  integrator
	CalVal Environement support added
	
	Revision 1.23  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.22  2004/02/04 14:43:52  marfav
	Align to new version of dbPersisent
	
	Revision 1.21  2003/11/19 10:13:13  marfav
	Prepared for use new sortable SpecialQuery
	
	Revision 1.19  2003/09/25 09:37:13  marfav
	Dead code removed
	
	Revision 1.18  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.17  2003/09/17 09:31:56  fracar
	handling hostname and missionwide valid files
	
	Revision 1.16  2003/08/29 12:00:46  fracar
	fixed policy for cleaning up existing working directories
	reading file extensions from config file for downloaded inputs
	issuing a critical exception message to Patrol if cannot update order queue
	
	Revision 1.15  2003/08/08 09:16:01  marfav
	Download of SUBS files not allowed
	
	Revision 1.14  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time
	
	Revision 1.13  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError
	
	Revision 1.12  2003/06/06 10:07:33  marfav
	DOWNLOAD_INPUT_FILES_STATUS fixed
	
	Revision 1.11  2003/05/21 15:02:39  marfav
	Removed some dead code
	
	Revision 1.10  2003/05/13 08:45:19  marfav
	Fixed memory leak at cleanup
	
	Revision 1.9  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.8  2003/04/30 16:20:16  marfav
	Align to ICD v2.0 and some logging improvement
	
	Revision 1.7  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.6  2003/03/26 09:56:11  marfav
	Robustness improved
	
	Revision 1.5  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.4  2003/02/10 18:50:19  fracar
	prepared for multi-interval management
	
	Revision 1.3  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/



#include <InputDownloader.h>


#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>

#include <Filterables.h>
#include <rsPushNSpace.h>
#include <exStream.h>

#include <dbDownloadFile.h>
#include <dbInventoryObject.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <TlStatusMgrSingleton.h>

#include <Application.h>
#include <dbConnectionPool.h>

#include <functional>


using namespace std;
using namespace acs;


ACS_CLASS_DEFINE_DEBUG_LEVEL(InputDownloader)


InputDownloader::InputDownloader() :
	_symtableParams(),
	_stopRequested(false),
	_cacheManager (0)

{
}

InputDownloader::InputDownloader (const acs::PCC_Base::SymTableParams& theTable, CacheManager* theCacheManager) :
	_symtableParams(theTable),
	_stopRequested(false),
	_cacheManager (theCacheManager)
{

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << "InputDownloader::CTOR: creating downloader\n" ;
		excerr << "InputDownloader::CTOR: symbol table query size is " << _symtableParams.fileList.size() << "\n" ;
		excerr << "InputDownloader::CTOR: check passed " << (_symtableParams.checkPassed?"TRUE":"FALSE") << "\n" ;
		excerr << "InputDownloader::CTOR: to be downloaded " << (_symtableParams.toBeDownloaded?"TRUE":"FALSE") << "\n" ;
		excerr << "InputDownloader::CTOR: query trigger " << _symtableParams.queryTrigger << "\n" ;
	ACS_CLASS_END_DEBUG
}

InputDownloader::~InputDownloader() throw() { }

InputDownloader::InputDownloader(const InputDownloader& d) :
	_symtableParams (d._symtableParams),
	_stopRequested(d._stopRequested),
	_cacheManager(d._cacheManager)

{
    // empty 
}

InputDownloader&
InputDownloader::operator = (const InputDownloader& d)
{
	if (this != &d)
	{
		_symtableParams = d._symtableParams;
		_stopRequested = d._stopRequested;
		_cacheManager = d._cacheManager; 
	}
	return *this;
}


PCC_Base::SymTableParams 
InputDownloader::getSymTableParams()
{
	return _symtableParams;
}

void
InputDownloader::stopDownload()
{
    _stopRequested = true;
}

void
InputDownloader::download(const string& destDir)
{

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << "InputDownloader::download: starting the download\n" ;
		excerr << "InputDownloader::download: symbol table query size is " << _symtableParams.fileList.size() << "\n" ;
		excerr << "InputDownloader::download: check passed " << (_symtableParams.checkPassed?"TRUE":"FALSE") << "\n" ;
		excerr << "InputDownloader::download: to be downloaded " << (_symtableParams.toBeDownloaded?"TRUE":"FALSE") << "\n" ;
		excerr << "InputDownloader::download: query trigger " << _symtableParams.queryTrigger << "\n" ;
		excerr << "InputDownloader::download: stream " << (_symtableParams.isStream?"TRUE":"FALSE") << "\n" ;
	ACS_CLASS_END_DEBUG

	// check if we've been stopped
	if ( _stopRequested )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "InputDownloader::download: download ignored because stop requested in the meantime") ;
		return;
	}

	exFMessage msg;
	vector<string> pathnames;

	// Get a database connection and use always it
	dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);

	// Loop the symbol tables filenames
	for (unsigned int i=0; i<_symtableParams.fileList.size(); ++i)
	{
		string fileName = _symtableParams.fileList.at(i).fileName;
		string fileVersion = _symtableParams.fileList.at(i).fileVersion;
		bool isStream = _symtableParams.isStream;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "InputDownloader::download: managing file #" << i << "[" << fileName << ", " << fileVersion << "]") ;
		if (fileName != "")
		{
			// This input has to be downloaded
			dbInventoryObject *obj= dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey()));
			dbPersistentScopeGuard theGuard (obj);
			obj->setFileName (fileName);
			obj->setFileVersion (fileVersion);

			// Retrieve information from the Inventory DB table
			obj->load(conn);

			// now pass the object to the cache manager ad let the download go

			// stay in BUSY status while downloading
			DownloadStatusMgrSingleton::StatusGuard _g( *DownloadStatusMgrSingleton::instance()->getMgr(), DL_BUSY);

			// The ERROR state is pulsed here while handling exceptions

			try
			{
				ACS_COND_THROW ( ( 0 == _cacheManager ), InvalidUse ("Class not initialized") );
				// ECICMF-74 - Virtual Download management
				// In case the symbol table reports the input alternative as virtual
				// Just put the name in the structure without actually download anything
				if ( _symtableParams.virtualDownload)
				{
					// Just keep the filename without triggering the file transfer
					ACS_LOG_INFO ("Virtual file [" << obj->getFileName() << ", " << obj->getFileVersion() << "] in WD# " << File::getFileName (destDir) );
					_symtableParams.fileList.at(i).fileName = _cacheManager->getFinalFileName(*obj, destDir);
				}
				else
				{
					// This will perform the actual download interacting with the cache
					ACS_LOG_INFO ("Downloading file [" << obj->getFileName() << ", " << obj->getFileVersion() << "] in WD# " << File::getFileName (destDir) );
					_symtableParams.fileList.at(i).fileName = _cacheManager->getFile(*obj, destDir, isStream);
				}
			}

			// Catching exceptions
			// we notify here, but then re-throw to allow exit code management at upper level
			catch(dbDownloadFile::PreconditionException& e)
			{
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "InputDownloader::download: exception caught - found an error in InventoryFile entry");
			
					// excerr.notify(e);
					// pulsing WARNING to the TL status and sending the ERROR status to the Download_file_status....
					(TlStatusMgrSingleton::instance()->getMgr())->pulseStatus (TL_WARNING);
					(DownloadStatusMgrSingleton::instance()->getMgr())->pulseStatus (DL_ERROR);
			
					// re-throw exception
					throw;
			}

			catch(dbDownloadFile::DownloadException& e)
			{
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "InputDownloader::download: db download exception caught - download error");
			
					// excerr.notify(e);
			
					// pulsing WARNING to the TL status and sending the ERROR status to the Download_file_status....
					(TlStatusMgrSingleton::instance()->getMgr())->pulseStatus (TL_WARNING);
					(DownloadStatusMgrSingleton::instance()->getMgr())->pulseStatus (DL_ERROR);

					// re-throw exception
					throw;
			}

			catch(exception& e)
			{
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "InputDownloader::download: exception caught - download error");
			
					// pulsing WARNING to the TL status and sending the ERROR status to the Download_file_status....
					(TlStatusMgrSingleton::instance()->getMgr())->pulseStatus (TL_WARNING);
					(DownloadStatusMgrSingleton::instance()->getMgr())->pulseStatus (DL_ERROR);

					// re-throw exception
					throw;
			}

		}
	} // loop on filenames
}
