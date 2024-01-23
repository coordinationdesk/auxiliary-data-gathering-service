// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
*/


#ifndef _dbLTADownloadInventoryFile_H_
#define _dbLTADownloadInventoryFile_H_ 

#include <LTAInventoryObject.h>

#include <dbDownloadFile.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class SQLString;

//
//
//
// class dbLTADownloadInventoryFile
//
//
//


/**
 * The class is specialized in downloading inventoried objects into the local filesystem. 
 * The download will be done from the ARF 
 * depending on the real storage of the file.
 * User must provide the LTAInventoryObject to be saved and the file path.
 */
class dbLTADownloadInventoryFile : public dbDownloadFile // PRQA S 2109
{
public:	

	friend class dbLTAFileActionFactory ; // PRQA S 2107

	exDECLARE_EXCEPTION(FileNotDownloadableException, DownloadException); //PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(SMUnknownFileException, DownloadException); //PRQA S 2131, 2153, 2502

	dbLTADownloadInventoryFile() = delete ;
	dbLTADownloadInventoryFile(const dbLTADownloadInventoryFile &) = delete ;
	dbLTADownloadInventoryFile &operator=(const dbLTADownloadInventoryFile &) = delete ;
	dbLTADownloadInventoryFile(dbLTADownloadInventoryFile &&) = delete ;
	dbLTADownloadInventoryFile &operator=(dbLTADownloadInventoryFile &&) = delete ;
	
	virtual ~dbLTADownloadInventoryFile() = default ;
	
	// inherited from dbDownloadFile
	/**
	 * Start the thread.
	 * \throw dbPersistent::InvalidUse File has already been downloaded or thread is already running.
	 */
	virtual void start() ;
	
	// inherited from dbDownloadFile
	/**
	 * Check if thread is still downloading the file.
	 * \throw PreconditionException Specified file already exists or  external reference has not been set in the dbInventoryObject
	 * \throw DownloadException An error rised while downloading.
	 */
	virtual bool downloading() ;

protected:
	/**
	 * \param path: local downloaded file path;
	 * \param clientName: declare the client name. Will be registered on DB if download is ok.
	 * 						According to configuration clientName="" is not registered / registered as UnknownClient;
	 * \param downloadInvalidFile: put the param to true to download objects having the validity flag set to false;
	 */
	dbLTADownloadInventoryFile(dbConnectionPool &, db::LTAInventoryObject &, const std::string &path, bool downloadInvalidFile=false);

private:
	virtual void run();
	void simulateDownload( db::LTAInventoryObject &, const std::string &path, const off_t & fileSize);

	SQLString getSyncSMsForDownloadQuery(const std::string & invSmTableName, const std::string& dbKey) const;
	
	void deleteUnknownInventorysm( dbPersistent::IdType unknownInventorysmId, const std::string& dbKey, const std::string & table, dbConnection & ) const;
	void invalidateInventory() ;
	
#ifdef ACS_TEST
public:
#else
private:
#endif
	void simulateUnknownFileException(bool v) { _simulateUnknownFileException = v; }
	void simulateFileToDownloadIsNearlineException(bool v) { _simulateNearlineException = v; }
	void simulateFileToDownloadIsOfflineException(bool v) { _simulateOfflineException = v; }

private :
	db::LTAInventoryObject &_inventory ;
	std::string const _path ;
	bool _preconditionPassed;
	bool _downloadInvalidFile;
	std::string const _inventoryName;
	bool _simulateUnknownFileException ;
	bool _simulateNearlineException ;
	bool _simulateOfflineException ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTADownloadInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 


#endif // _dbLTADownloadInventoryFile_H_
