// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
	
*/


#ifndef _dbLTAUploadInventoryFile_H_
#define _dbLTAUploadInventoryFile_H_ 

#include <acs_c++config.hpp>
#include <dbUploadFile.h>
#include <LTAInventoryObject.h>
#include <FileType.h>
#include <dbPersistent.h>


_ACS_BEGIN_NAMESPACE(acs)

class SMInterface;
//
//
//
// class dbLTAUploadInventoryFile
//
//
//


/**
 * The class is specialized in saving  objects to be inventoried.
 * User must provide the LTAInventoryObject to be saved and the file path.
 * The file is optionally saved in the database, while saving the record, 
 * depending on the file type.
 * The LTAInventoryObject must satisfy the following preconditions:
 * \li Its file type field must be set and must be valid.
 * A separate thread will actually perform the upload process. 
 */
class dbLTAUploadInventoryFile : public dbUploadFile // PRQA S 2109
{
public:

	exDECLARE_EXCEPTION(UpdateValidityException, UploadException) ; // PRQA S 2131, 2153, 2502
	
	friend class dbLTAFileActionFactory ; // PRQA S 2107
	
	dbLTAUploadInventoryFile() = delete ;
	dbLTAUploadInventoryFile(const dbLTAUploadInventoryFile &) = delete ;
	dbLTAUploadInventoryFile &operator=(const dbLTAUploadInventoryFile &) = delete ;
	dbLTAUploadInventoryFile(dbLTAUploadInventoryFile &&) = delete ;
	dbLTAUploadInventoryFile &operator=(dbLTAUploadInventoryFile &&) = delete ;

	virtual ~dbLTAUploadInventoryFile() = default ;

	// inherited from dbUploadFile
	/**
	 * Start the thread.
	 * \throw dbPersistent::InvalidUse Record has already been saved or thread is already running.
	 */
	virtual void start() ;

	// inherited from dbUploadFile
	/**
	 * Check if thread is still uploading the file.
	 * \throw PreconditionException Specified filedoesn't  exists or  the dbInventoryObject doesn't satisfy some preconditions.
	 * \throw ExternalRepositoryException An error rised while uploading file on external repository.
	 * \throw SaveException An error rised while saving record in database (the file has however been saved in external repository).
	 * 
	 */
	virtual bool uploading() ;
	
	/**
	 * Sets the doNotUpdateValidityFlag. If set to true the uploading thread must not update the validity flag of the uploaded file to true
	 */
	void setDoNotUpdateValidityFlag(bool);
	bool getDoNotUpdateValidityFlag() const;

	/**
	 * Sets the invokeSpOnInventorySuccessFlag. If set to true the uploading thread must invoke p_successful_inventory
	 */
	void setInvokeSpOnInventorySuccessFlag(bool);
	bool getInvokeSpOnInventorySuccessFlag() const;

	
protected:
	/**
	 * \param path local downloaded file path
	 */
	dbLTAUploadInventoryFile(dbConnectionPool &, db::LTAInventoryObject &, const std::string &path);

private:
	virtual void run();
	void putFile(const std::string &path, dbPersistent::IdType storageManId,const std::string& smtype, std::vector<std::pair<SMInterface*, bool> >& tocheck) throw() ;
	void removeInventoryForced(bool, dbConnection &) ;

	void loadConf();

	bool storeOnDatabase(bool emitException, bool valflag, bool callSp = false) ;
	void invokeSpSuccessfulInventory(dbPersistent::IdType invid, dbConnection& ) const ;
	
	SQLString buildSpSuccessfulInventoryCmd(dbPersistent::IdType invid, dbConnection& ) const ;
	std::string writeTransactionFile (const SQLString & , dbConnection& ) const ;
	void executeSQLCmd (const SQLString&, const std::string & label, dbConnection& ) const ;
	static void debugMessage() ;
	
	// declared but not implemented to prevent their use

#ifdef ACS_TEST
public:
#else
private:
#endif
	static void setSimulateNoCommunicationWithSM() ;
	static void setSimulateDatabaseUpdateFailure() ;
	static void setSimulateUpdateValidityException(bool v) ;
	static void unsetSimulationFlags() ;


private:
	db::LTAInventoryObject _inventory ;
	db::FileType _fileType;
	
	bool _preconditionPassed;
	bool _externalRepositoryPassed;
	bool _savePassed;
	bool _doNotUpdateValidityFlag;
	bool _invokeSpOnInventorySuccessFlag;
	bool _deleteUploadedFilepathOnSuccess ;
	bool _errorInUpdateValidityTransaction ; 
	std::string _transactionFileDir;
	const std::string _inventoryName;
	static bool _debugMessageEmitted ;
	static int _notSimulatedDebugLevel ;
	static bool _simulateUpdateValidityException ; 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAUploadInventoryFile) ;
} ; 


_ACS_END_NAMESPACE
 
#endif // _dbLTAUploadInventoryFile_H_
