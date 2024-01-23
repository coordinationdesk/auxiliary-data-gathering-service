// PRQA S 1050 EOF
/*

 Copyright 1995-2013, Advanced Computer Systems , Inc.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.acsys.it

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Advanced Computer Systems, Inc.

 $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author$

 $Log$
 Revision 5.18  2017/02/23 16:36:58  nicvac
 S3EVOL-13: Distribution working area is not related anymore to download cache.

 Revision 5.17  2017/02/22 16:41:35  nicvac
 S3EVOL-13: Log improved

 Revision 5.16  2017/02/21 17:02:44  nicvac
 S3EVOL-13: Inter process cache mode

 Revision 5.15  2017/02/02 09:49:20  davide.tiriticco
 S2PDGS-1655: packageId type changed from string to dbPersistent::IdType

 Revision 5.14  2016/12/16 16:55:45  nicvac
 S3MPM-860: PackageExporter resources improvement.

 Revision 5.13  2016/09/08 11:25:24  danalt
 removed include ProductTreeBuilder

 Revision 5.12  2016/05/10 21:46:39  danalt
 use of ACS_THROW and ACS_COND_THROW

 Revision 5.11  2016/04/14 09:58:32  marpas
 Thread does not inheriths any longer from ThreadSafe

 Revision 5.10  2015/05/28 09:33:31  marpas
 useless parameters no more read from configuration (BufrExtension, BufrConverter, CompressScriptName)
 coding best practice applied

 Revision 5.9  2014/05/05 19:10:20  nicvac
 S2PDGS-667: implementation.

 Revision 5.8  2014/01/30 13:44:29  francesco.avanzi
 coding best practices applied & new db::Transaction used

 Revision 5.7  2013/11/25 17:34:05  nicvac
 Handled contingency in DownloadPackData::optimiser. concurrent downloading from checkDownloadedFile.

 Revision 5.6  2013/11/12 17:46:25  nicvac
 Log improved.

 Revision 5.5  2013/11/12 13:22:11  nicvac
 S2PDGS-518: Fixed checkDownloadedFile. fileName was I/O parameter without reason.

 Revision 5.4  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.3  2013/10/28 17:34:09  marpas
 work in progress

 Revision 5.2  2013/09/30 12:37:13  marpas
 adopting libDbCryosat 4.x

 Revision 5.1  2013/07/15 16:47:18  marpas
 compilation warnings fixed

 Revision 5.0  2013/06/06 18:51:19  marpas
 adopting libException 5.x standards

 Revision 1.25  2013/03/12 17:55:47  marpas
 dbConnectionPool is mandatory in file actions

 Revision 1.24  2013/01/25 13:34:27  marpas
 using pattern::Singleton from libException

 Revision 1.23  2013/01/24 17:56:15  nicvac
 Added important log.

 Revision 1.22  2013/01/18 16:39:13  nicvac
 ADC-99: if not found on ISM, propagate the error.

 Revision 1.21  2012/12/06 18:33:37  nicvac
 Delivery Slip can be skipped by configuration.

 Revision 1.20  2012/10/19 15:04:06  micmaz
 updated library

 Revision 1.19  2012/10/08 13:59:19  micmaz
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.18  2012/09/19 15:57:11  micmaz
 ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.17  2012/09/14 16:31:32  micmaz
 S2GPP-611
 Development of Distribution Formatter framework for Export

 Revision 1.15  2012/09/11 16:42:31  micmaz
 removed compiling warning

 Revision 1.14  2012/01/23 12:03:53  micmaz
 removed LOG(...)

 Revision 1.13  2012/01/23 11:51:41  micmaz
 removed script LOG(..)

 Revision 1.12  2011/05/06 10:48:36  crivig
 modified for quality check; i++ modified into ++i; packFiles method simplified: no changedir

 Revision 1.11  2011/04/22 07:55:35  crivig
 eraseEntry method used

 Revision 1.10  2011/04/18 10:06:17  crivig
 cache filename methods moved in libFile

 Revision 1.9  2011/03/11 11:15:23  crivig
 bug fixed in cache use; Timer::delay used in db retries

 Revision 1.8  2011/03/09 13:05:43  crivig
 Cache autorelease introduced

 Revision 1.7  2011/03/04 11:39:47  crivig
 integrated with new libCache

 Revision 1.6  2011/02/25 14:53:05  crivig
 bug on cartId fixed

 Revision 1.5  2010/12/17 10:24:20  crivig
 connection pool passed

 Revision 1.4  2010/12/03 15:00:49  crivig
 cache cleaning handled with Cache class

 Revision 1.3  2010/11/29 11:05:59  crivig
 changed Package to DataPackage

 Revision 1.2  2010/11/22 12:45:12  crivig
 modified disk cleaning algo

 Revision 1.1  2010/11/22 10:28:17  crivig
 imported new Class from libDownloadOptimiser

 */

#include <DownloadPackData.h>
#include <DataProviderConfigurer.h>

#include <DataDownloader.h>

#include <File.h>
#include <Dir.h>

#include <exException.h>
#include <Filterables.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs,dataprovider)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL ( DownloadPackData );

DownloadPackData::DownloadPackData ( DataPackage & package, dbConnectionPool& connectionPool, Cache & cache ) :
    		Thread(),
    		_package ( package ),
    		_maxConnRetry ( acs::conf::DPConfigurerSingleton::instance()->getMaxDBRetry() ),
    		_done ( false ),
    		_connectionPool ( connectionPool ), // PRQA S 2528
    		_cache ( cache ), // PRQA S 2528
    		_stop ( false )
{
}

DownloadPackData::~DownloadPackData() throw () {
}

void DownloadPackData::exit() {
	_stop = true;
}

bool DownloadPackData::isDone() const throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Download pack is done " << _done );
	return _done;
}

//
// implement the run method
//
void DownloadPackData::run() // PRQA S 4020
{ 
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Start Download Pack Data Thread..." );
	Lock guard ( mutex() ); //automatically acquire the lock
	if ( requestedCancel() ) {
		//cancel was requested
		return;
	}
	// assign correctly the product fileType (i.e. there can be more files in a _package
	// some of which might be reports
	string fileType;
	findProductFileType ( &fileType );
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Found product file type " << fileType );
	_package.setFileType ( fileType );

	// start the optimising and the downloadong
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " Start downloading for package: " << _package.getPackageId() );

	//Start downloading optimization
	this->downloadWithCacheAndLinkToProdPath();

	//Delivery slip, package compression, package formatting are no longer implemented here: do it in a PackageExporter Task Tables

	_done = true;
}


void DownloadPackData::downloadWithCacheAndLinkToProdPath() {

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " optimising the packageId " << _package.getPackageId() );

	//Remove Product Path directory if already exists and make a new one
	std::stringstream ss;
	ss <<  acs::conf::DPConfigurerSingleton::instance()->getProductPath() << "/" << _package.getPackageId();
	string pathDir = ss.str();
	File::remove ( pathDir );
	File::mkdir ( pathDir );

	//Download and link to Product Path
	vector < string > files;
	_package.getInvFiles ( files );
	size_t files_size = files.size() ;

	ACS_LOG_PRIVINFO( "[DP "<<_package.getPackageId()<<"] Downloading "<<files_size<<" Inventory Items." );

	for ( size_t i = 0; i < files_size; ++i ) {
		string key = files[i];
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Set the inv file " << key << " to downloading" );

		string fileName = key.substr ( 0, key.find ( "@" ) );
		string version = key.substr ( key.find ( "@" ) + 1, key.length() ); // PRQA S 3084
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Start the downloading " << fileName << " version " << version );

		DataDownloader::InventoryEntry inventoryEntry;
		inventoryEntry.inventoryFilename = fileName;
		inventoryEntry.inventoryFileversion = version;
		inventoryEntry.cacheGroupName = ""; //To evaluate if cache group handling are really needed in PackageExporter.
		inventoryEntry.symlinkDir = "";
		inventoryEntry.copyDir = pathDir; //Do hardlink (or copy if not possible) from cache to Product Path;

		//Important Note:
		// Using copyDir (hardlink/copy) instead of symlinkDir you put the item in the distribution processing working area and
		// you detach the item itself from cache. This means:
		// - The item in cache can be rolled without interfering with the distribution processing working area
		// - Before calling this method remember to chech the distribution processing working area has enough space.

		DataDownloader dataDownloader( _connectionPool, _cache, "" );

		//Concurrent download (threads/processes) through cache synch
		vector <DataDownloader::DownloadedEntry> downloadedEntry = dataDownloader.download_files( inventoryEntry );

		// add in DataPackage. Cache group not handled here! ==> cache entry = cache locked entry.
		for (vector<DataDownloader::DownloadedEntry>::iterator it=downloadedEntry.begin(); it!=downloadedEntry.end(); ++it) { // PRQA S 4238
			_package.addCacheFile ( it->cacheLockedEntryRelPath ); //Cache group not handled here ==> the same as it->cacheEntryRelPath.

			//The item has been hardlinked/copied to distribution processing working. It's not related to cache anymore.
			_cache.release( it->cacheLockedEntryRelPath );
		}
	}

}


const DataPackage &DownloadPackData::getPackage() const throw() // PRQA S 4120
{
	return _package;
}


void DownloadPackData::findProductFileType ( string* fileType ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[findProductFileType ]" );
	string productFormat = _package.getFormat();
	vector < string > fileTypes;
	_package.getInvFilesFileTypes ( &fileTypes );
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Inventory file types found: " << fileTypes.size() );
	// if there is only a product then assign its file type to fileType and return
	if ( fileTypes.size() >= 1 ) {
		*fileType = fileTypes[0];
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file type found: " << *fileType );
	}
}


_ACS_END_NESTED_NAMESPACE // acs

