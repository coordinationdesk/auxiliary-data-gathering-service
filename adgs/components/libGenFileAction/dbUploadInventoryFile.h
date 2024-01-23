// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.1  2013/10/10 17:42:46  marpas
	dbUploadInventoryFile can be configured to upload blobs possibly conditioned by size
	cond space gives an example of configuration
	PDS_OLD_SCHEMA no longer supported.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.11  2013/04/03 13:47:32  marpas
	run does not longer gets a void*
	
	Revision 1.10  2013/04/03 12:49:28  marpas
	Thread::run gets a void*
	
	Revision 1.9  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.8  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.7  2013/03/11 12:51:26  marpas
	FileGuard is a template specialization now
	deprecated functions substituted
	
	Revision 1.6  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.5  2012/11/20 09:04:04  marpas
	define PDS_OLD_SCHEMA to use old schema features
	
	Revision 1.4  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.3  2011/04/05 16:52:35  marpas
	Tree management work in progress
	
	Revision 1.2  2009/06/26 10:43:07  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
*/


#ifndef _dbUploadInventoryFile_H_
#define _dbUploadInventoryFile_H_ 

#include <acs_c++config.hpp>
#include <dbUploadFile.h>
#include <dbPersistent.h>
#include <FileGuard.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject ;
class dbFileType;
class SMInterface;
//
//
//
// class dbUploadInventoryFile
//
//
//


/**
 * The class is specialized in saving  objects to be inventoried.
 * User must provide the dbInventoryObject to be saved and the file path.
 * The file will be saved in external repository if the externalReferenceSet
 * in the dbInventoryObject has not been set.
 * The file is optionally saved in the database, while saving the record, 
 * depending on the file type.
 * The dbInventoryObject must satisfy the following preconditions:
 * \li It must not have already be saved in database
 * \li Its file type field must be set and must be valid.
 * \li Storage field for the specific file type must be "ARF" or "BOTH"
 * A separate thread will actually perform the upload process. 
 * During the whole process the dbInventoryObject will be locked.
 */
class dbUploadInventoryFile : public dbUploadFile // PRQA S 2109
{
public:
	
	friend class dbGenFileActionFactory ; // PRQA S 2107
	
	virtual ~dbUploadInventoryFile() ACS_NOEXCEPT ;
	dbUploadInventoryFile() = delete ;
	dbUploadInventoryFile(const dbUploadInventoryFile &) = delete ;
	dbUploadInventoryFile &operator=(const dbUploadInventoryFile &) = delete ;
	dbUploadInventoryFile(dbUploadInventoryFile &&) = delete ;
	dbUploadInventoryFile &operator=(dbUploadInventoryFile &&) = delete ;


	// inherited from dbDownloadFile
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

protected:
	/**
	 * \param path local downloaded file path
	 */
	dbUploadInventoryFile(dbConnectionPool &, dbInventoryObject &, const std::string &path);

private:
	virtual void run();
	std::string getFile(int storageManId,const std::string& smtype,int id,bool defaultpacking);
	void putFile(const std::string &path, dbPersistent::IdType storageManId,const std::string& smtype,bool defaultpacking, std::vector<std::pair<SMInterface*, bool> >& tocheck) throw() ;
	void pack(dbPersistent::IdType storageManId , bool defaultpacking, std::string& path);
	void unpack(dbPersistent::IdType storageManId , bool defaultpacking, std::string& newpath);
	void removeInventoryForced(bool, dbConnection &) ;

	bool storeOnDatabase(bool emitException, bool valflag, const std::string &path) ;

	static void debugMessage() ;
	
private:
	dbInventoryObject &_inventory ;
	dbFileType *_fileType;
	bool _preconditionPassed;
	bool _externalRepositoryPassed;
	bool _savePassed;
	std::vector<FileGuard *> _tmpfile;
	static bool _debugMessageEmitted ;
	static const int simulateFirstDbCommunicationFailure = 1357 ;
	static const int simulateNoCommunicationWithSM = 2468 ;
	static const int simulateDatabaseUpdateFailure = 3579 ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbUploadInventoryFile) ;
} ; 


_ACS_END_NAMESPACE
 
#endif // _dbUploadInventoryFile_H_
