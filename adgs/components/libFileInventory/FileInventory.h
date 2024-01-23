// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 5.8  2015/05/26 15:28:00  marpas
	useless warnings emitted only in debug
	
	Revision 5.7  2015/02/04 14:58:02  marfav
	S2PDGS-1072 adding support to options noactions and restoreonsync
	
	Revision 5.6  2015/02/02 16:08:42  marfav
	S2PDGS-1072 upload without triggers implemented
	
	Revision 5.5  2015/01/30 17:22:49  marfav
	S2PDGS-976 managing restore on sync storage managers
	
	Revision 5.4  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.3  2013/10/23 15:27:40  marfav
	Added support to delay notification in inventory anomalies
	
	Revision 5.2  2013/10/09 17:05:15  marpas
	some statistics added
	
	Revision 5.1  2013/07/04 13:39:53  marpas
	fixing a bug where checkOptionalField can return true even if the metadata was not found, because the string it should fill was not empty entering the method
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.24  2013/06/07 16:56:33  nicvac
	S2PDGS-308: Inventory notifications fixed.
	
	Revision 2.23  2013/05/24 18:28:40  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 2.22  2013/03/12 17:50:49  marpas
	using dbConnectionPool
	
	Revision 2.21  2013/02/21 19:25:15  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.20  2012/08/02 16:37:33  marpas
	refactoring and new test
	
	Revision 2.19  2012/06/11 13:07:58  marpas
	template method definition fixed
	
	Revision 2.18  2012/03/12 16:21:31  marpas
	fixed regression about already present inventory
	
	Revision 2.17  2012/03/06 15:54:04  marpas
	messages improved and standardized
	
	Revision 2.16  2012/03/06 13:28:05  marpas
	changes in progress
	
	Revision 2.15  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.14  2011/06/20 10:25:26  marpas
	file number check can be relaxed
	
	Revision 2.13  2011/04/19 16:15:45  marpas
	useless messages removed
	
	Revision 2.12  2011/04/18 13:11:57  chicas
	Dir inventory added
	
	Revision 2.11  2011/04/13 13:09:10  marpas
	work in progress
	
	Revision 2.10  2011/03/21 18:22:09  marpas
	VEN-1193 change.
	-noCriticalOnDuplicates option management. New exception if a duplicate was found
	
	Revision 2.9  2010/05/19 09:26:54  marpas
	new exception declaration added
	
	Revision 2.8  2010/02/22 10:10:39  marpas
	jms file management added
	
	Revision 2.7  2009/12/03 13:45:16  marpas
	trying to raise an exception when an element in a list goes wrong
	
	Revision 2.6  2009/04/03 17:31:25  marpas
	check file type periodicity added
	
	Revision 2.5  2008/11/18 13:58:50  marpas
	new test added, debug improved
	
	Revision 2.4  2007/03/22 09:00:58  valspa
	VDS: ready to test pre dry-run including handling of originator and MDSs
	
	Revision 2.3  2007/01/08 13:48:40  giucas
	The main loop has changed: the current dir is checked before and if and only if no lists are found the configured directory is checked for inventory.
	
	Revision 2.2  2006/07/03 14:46:03  giucas
	Removed convTime method. Using DateTime ctor instead for a correct use of mseconds
	
	Revision 2.1  2006/04/28 14:59:05  giucas
	InventoryGaps management implemented
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.19  2006/01/25 09:34:51  enrcar
	Now symbolic link is removed from input dir
	
	Revision 1.18  2005/09/29 16:19:52  marpas
	failedOrders method implemented
	
	Revision 1.17  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.16  2004/10/18 10:49:55  seralb
	Validity flag: solved a bug (getFieldValue) and now extracted from db
	
	Revision 1.15  2004/10/14 15:01:20  seralb
	overwrite option added
	
	Revision 1.14  2004/05/26 15:01:03  seralb
	Uploaded files now removed at the end by a list
	
	Revision 1.13  2004/05/07 12:22:14  seralb
	Catalog file gestion
	
	Revision 1.12  2004/04/14 12:44:17  seralb
	GeographicLocalization field: now it can work on geo object of different shape
	
	Revision 1.11  2003/09/09 14:48:16  seralb
	Added convTime method
	
	Revision 1.10  2003/09/04 15:46:07  seralb
	Added list of extension to remove
	FileInventory
	Revision 1.9  2003/07/16 16:24:36  seralb
	Bug Solved
	
	Revision 1.8  2003/07/09 10:19:58  seralb
	Methods added
	
	Revision 1.7  2003/05/20 14:57:54  seralb
	Exception added
	
	Revision 1.6  2003/05/07 10:57:48  seralb
	dbInventoryObject upgrade
	
	Revision 1.5  2003/04/30 16:46:59  marpas
	using namespace std no longer into acs includes
	
	Revision 1.4  2003/02/20 14:53:05  giucas
	Added libSignal handling
	
	Revision 1.3  2003/01/31 14:07:06  seralb
	
	nsert removed comment
	
	Revision 1.2  2003/01/28 09:50:28  seralb
	Removed version control and related methods
	
	Revision 1.1.1.1  2003/01/14 14:52:23  seralb
	Import libFileInventory
	


*/

#ifndef _FileInventory_H_
#define _FileInventory_H_ 

#include <acs_c++config.hpp>
#include <dbUploadFile.h>
#include <InventoryEmailNotification.h>
#include <Metadata.h>
#include <exException.h>
#include <Filterables.h>
#include <Statistics.h>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObjectTEC ;
class dbConnectionPool ;

class FileInventory // PRQA S 2109
{
public:

	/*! class FileInventoryException declaration */
	exDECLARE_EXCEPTION(FileInventoryException, exException) ;  // PRQA S 2131, 2502 3
	exDECLARE_EXCEPTION(GeoInventoryException, exException) ;
	exDECLARE_EXCEPTION(DuplicateInventoryException, FileInventoryException) ; // PRQA S 2153

	/**
	 * \brief  Default Class constructor 
	 *    
	 */
	explicit FileInventory(dbConnectionPool &);

	/**
	 * \brief  Class destructor 
	 *    
	 */    	
	~FileInventory() throw() ;

	//methods

	void makeInventory(const std::string &,const std::string &, const std::string &);
	const std::string &failedOrders() const throw() ;
	void relaxFNumberCheck(bool) throw()  ;

    void restoreOnSync (bool) throw();
    void noActions (bool) throw();

    static std::string usage() ; 	

    static math::StatNode &statN() ;
#if defined(TEST_FILE_INVENTORY)
public:
#else
private:
#endif

	FileInventory(); // NOT implemented 
	FileInventory(const FileInventory & );  // NOT implemented 
	FileInventory &operator=(const FileInventory &); // NOT implemented 

	//methods
	void    sendNotifications(dbInventoryObjectTEC const& invObj);
	void 	getConfiguration();
    void    fillObjMandatory(const Metadata &, dbInventoryObjectTEC &) ;
    void    fillObjOptional(const Metadata &, dbInventoryObjectTEC &) ;
    void	lockDbTable(const std::string& table, const std::string& mode, dbConnection& conn);
    dbPersistent::IdType insertOriginator(const std::string& originatorName, dbConnection& conn);
	std::string	fillObj(Metadata &, dbInventoryObjectTEC &);
	std::string findListInDir(const std::string &) ;
	void    executeInventory(const std::string& currDir, const std::string &);
	void 	moveFileInBackupDir(const std::string &currdir, const std::string &);
	void 	moveListAndLockFile(const std::string &);
	void 	removeListAndLockFile(const std::string &);
	void	checkFiles(const std::string &currdir, const std::string &, std::vector<std::string>&);
	void 	wrongList(const std::string &currdir, const std::string &);
	int 	localMoveInDir(const std::string &, const std::string &);
	void 	loopOnDir(const std::string &) ;
	void 	moveJmsFile(const std::string &) throw() ;
	
	template<typename T> 
	T & checkMandatoryField(T& v,const Metadata &m,const std::string &f) ;

	template<typename T> 
	bool checkOptionalField(T& v,const Metadata &m,const std::string &f) ;

	template<typename T> 
	bool checkField(T& v,const Metadata &m,const std::string &f, bool) ;
private:
	//attributes
	std::string 			 _backupDir;
	std::string 			 _workingDir ;
	std::string 			 _inventoryListFileExt;
	std::string 			 _mtd;
	std::string 			 _jms;
	std::string 			 _rejectedDir;
	std::vector<std::string> _backupList;
	bool 					 _rmFlag;
	bool					 _testFlag;
	bool 					 _allFilesSuccesfullyInventoried;
	bool 					 _isTheListGood;
	int 					 _numSigInt;
	std::vector<std::string> _configuredExt;
	std::vector<std::string> _filesToBeRemovedList;
	std::string 			 _option;
	std::string				 _failedOrders ;
	int 					 _numberOfLists ;
	bool 					 _checkFTypePeriodicity ;
	bool					 _duplicateFound ; 
	bool 					 _relaxFnCheck ;
	bool 					 _rejectedDirSet ;
    dbConnectionPool         &_connectionPool ;

	std::deque<InventoryEmailNotificationSptr> _notifications;
    bool                    _manageInventoryAnomalies;
    bool                    _restoreOnSync;
    bool                    _noActions;
	bool					_deleteUploadedFilepathsOnSuccess;
	bool					_setDefaultUniqueId;
	
	bool					_insertOriginatorIfMissing;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(FileInventory) ;

};

template<typename T> inline
bool FileInventory::checkField(T& v,const Metadata &m,const std::string &f, bool exc)  // PRQA S 4020
{
	std::string s ;
	try {
		s = m.getMetadataValue(f) ;
	}
	catch(std::exception &x) {
		ACS_COND_THROW(exc,  // PRQA S 3081
			FileInventoryException(x,"Missing metadata " + f ))  ;
	}

	if (s.empty()) {
		ACS_COND_THROW(exc,  // PRQA S 3081
			FileInventoryException("Missing metadata mandatory field " + f ))  ;
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field " << f ) ;
		return false ;
	}

	try {
		m.getFieldValue<T>(f, v );
	}
	catch(std::exception &x) 
	{
		ACS_THROW(FileInventoryException(x,"FileInventory: unable to get or set the mandatory field " + f));  // PRQA S 3081
	}
	return true ;

}

template <> inline
bool FileInventory::checkField(std::string &v,const Metadata &m,const std::string &f, bool exc)  // PRQA S 2133, 2134, 4020
{
	try {
		v = m.getMetadataValue(f) ;
	}
	catch(std::exception &x) {
		ACS_COND_THROW(exc,  // PRQA S 3081
			FileInventoryException(x,"Missing metadata " + f ))  ;
        // just in case v was filled entering the function
        // if no exception has to be raised, ensure the following if will be true
        v.clear() ;
        
	}
	if (v.empty()) {
		ACS_COND_THROW(exc, // PRQA S 3081
			FileInventoryException("Missing metadata mandatory field " + f ))  ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Warning: Missing metadata optional field " << f) ;
		return false ;
	}
	return true ;

}



template<typename T> inline
T & FileInventory::checkMandatoryField(T& v,const Metadata &m,const std::string &f) 
{
	checkField(v, m, f, true) ;
	return v ;  // PRQA S 4028
}

template<typename T> inline
bool FileInventory::checkOptionalField(T& v,const Metadata &m,const std::string &f) 
{
	return checkField(v, m, f, false) ;
}

_ACS_END_NAMESPACE

#endif //_FileInventory_H_
