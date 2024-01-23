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
	
	Revision 6.7  2015/02/03 10:31:36  lucio.pulvirenti
	S2PDGS-977: FileNotDownloadableException declared.
	
	Revision 6.6  2015/02/02 16:08:06  marfav
	S2PDGS-1072 upload without triggers implemented
	
	Revision 6.5  2015/01/29 17:05:30  lucio.pulvirenti
	S2PDGS-1010: dbInventoryObjectOperatorIF canGetFromSynchSM and restoreOnSynchSM IFs integrated.
	
	Revision 6.4  2015/01/29 10:35:34  lucio.pulvirenti
	S2PDGS-1010 work in progress.
	
	Revision 6.3  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.2  2013/11/13 14:00:26  marfav
	Limiting the scope of any connection
	
	Revision 6.1  2013/09/25 16:58:18  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.11  2013/04/19 15:19:41  lucio.pulvirenti
	simulateDownload method added.
	
	Revision 1.10  2013/04/03 13:47:32  marpas
	run does not longer gets a void*
	
	Revision 1.9  2013/04/03 12:49:28  marpas
	Thread::run gets a void*
	
	Revision 1.8  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.7  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.6  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.5  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.4  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.3  2009/06/26 10:43:07  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.2  2008/11/11 14:58:20  ivafam
	Work in progess
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
	
		

*/


#ifndef _dbDownloadInventoryFile_H_
#define _dbDownloadInventoryFile_H_ 

#include <dbDownloadFile.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject ;
class SQLString;

//
//
//
// class dbDownloadInventoryFile
//
//
//


/**
 * The class is specialized in downloading inventoried objects into the local filesystem. 
 * The download will be done from the BLOB stored into the database or from the ARF 
 * depending on the real storage of the file.
 * User must provide the dbInventoryObject to be saved and the file path.
 * During the whole process the dbInventoryObject will be locked.
 */
class dbDownloadInventoryFile : public dbDownloadFile // PRQA S 2109
{
public:	

	friend class dbGenFileActionFactory ; // PRQA S 2107

	exDECLARE_EXCEPTION(dbDownloadInventoryClientRegisterException, dbFileAction::Exception); //PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(FileNotDownloadableException, DownloadException); //PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(FileDefinetelyNotDownloadableException, FileNotDownloadableException); //PRQA S 2131, 2153, 2502

	virtual ~dbDownloadInventoryFile() ACS_NOEXCEPT ;
	dbDownloadInventoryFile() = delete ;
	dbDownloadInventoryFile(const dbDownloadInventoryFile &) = delete ;
	dbDownloadInventoryFile &operator=(const dbDownloadInventoryFile &) = delete ;
	dbDownloadInventoryFile(dbDownloadInventoryFile &&) = delete ;
	dbDownloadInventoryFile &operator=(dbDownloadInventoryFile &&) = delete ;
		
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
	dbDownloadInventoryFile(dbConnectionPool &, dbInventoryObject &, const std::string &path,
							const std::string& clientName="", bool downloadInvalidFile=false);

private:
	virtual void run();
	void unpack(dbPersistent::IdType storageManId , bool defaultpacking, std::string& newpath);
	void simulateDownload(dbInventoryObject &, const std::string &path);
	void registerClient();
	
	SQLString getSyncSMsForDownloadQuery(bool hasADCfield) const;
	
	void deleteUnknownInventorysm( const std::vector<dbPersistent::IdType> &, dbConnection & ) const;


private :
	dbInventoryObject &_inventory ;
	std::string const _path ;
	std::string const _clientName;
	bool _preconditionPassed;
	bool _downloadInvalidFile;
	const std::string _invSmTableName;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDownloadInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 


#endif // _dbDownloadInventoryFile_H_
