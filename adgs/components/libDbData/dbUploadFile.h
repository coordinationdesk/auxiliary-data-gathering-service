// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/09/29 17:03:35  marpas
	removing compilation warnings for deprecated code
	
	Revision 5.2  2016/05/10 15:03:51  lucio.pulvirenti
	PDSEV-34: _deleteUploadedFilepathOnSuccess boolean attribute added to IF along with set and get methods.
	
	Revision 5.1  2015/02/04 14:53:42  marfav
	S2PDGS-1072 adding support to options noactions and restoreonsync
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.6  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.5  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.4  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2011/04/05 16:52:58  marpas
	Tree management work in progress
	
	Revision 2.2  2009/06/26 10:40:13  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/09/26 13:26:03  marpas
	interface changed
	
	Revision 1.2  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.4  2004/11/12 11:29:09  paoscu
	dbFileAction interface robustness improved
	
	Revision 1.3  2004/10/07 09:40:56  ivafam
	dbFileAction added
	
	Revision 1.2  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.1  2002/11/15 12:11:48  paoscu
	Imported.
	
		
*/


#ifndef _dbUploadFile_H_
#define _dbUploadFile_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbFileAction.h>
#include <Thread.h>
#include <vector>
#include <string>
#include <utility>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnection ;

//
//
//
// class dbUploadFile
//
//
//
class dbUploadFile : public dbFileAction, protected Thread // PRQA S 1703, 2109, 2194
{
public:	

    // make this method public
    using Thread::waitExitOrTimeout ;
    
	/*! class SaveException declaration 
	 *
	 * This exception is thrown by uploading() if there was an error
	 * while saving record in database.
	 */
	exDECLARE_EXCEPTION(SaveException,dbFileAction::Exception) ; // PRQA S 2131, 2153, 2502

	
	/*! class UploadException declaration 
	 *
	 */
	exDECLARE_EXCEPTION(UploadException,dbFileAction::Exception) ;// PRQA S 2131, 2153, 2502

	explicit dbUploadFile(dbConnectionPool &,
                         dbPersistent &, 
                         const std::string &path = "");
	virtual ~dbUploadFile() ACS_NOEXCEPT ;
	dbUploadFile() = delete ;
	dbUploadFile(const dbUploadFile &) = delete ;
	dbUploadFile &operator=(const dbUploadFile &) = delete ;
	dbUploadFile(dbUploadFile &&) = delete ;
	dbUploadFile &operator=(dbUploadFile &&) = delete ;

	virtual void start() = 0 ; // PRQA S 2121

	virtual bool isComplete() const ;
	virtual bool uploading() = 0 ;
	
	const std::string &getPath() const ;

	/*! Restore on Sync option 
	 *
	 * This option, when set, will signal the actual UploadFile object
	 * to upload the file on all the Synchronous available storage managers
     * without attempting to insert a new record in the t_inventory table
     * It should be used to allow the migration from Async to Sync storage managers
	 */ 
    bool restoreOnSync() const throw() {return _restoreOnSync;}

	/*! No Action declaration 
	 *
	 * This option, when set, will signal the actual UploadFile object
	 * to immediately remove the t_pendinginventory record just after the t_inventory insert
     * before modifying the validity flag to true and to perform all the needed actions to
     * block any automatic post inventory operation based on database triggers and stored procedures
     * It should be used to allow the re-inventory of deleted files and to distinguish it
     * from the very first inventory of the data
	 */
    bool noActions() const throw() {return _noActions;}

    void restoreOnSync(bool b) throw() {_restoreOnSync = b;}
    void noActions(bool b) throw() {_noActions = b;}
	
	void setDeleteUploadedFilepathOnSuccess(bool v) ;
	bool deleteUploadedFilepathOnSuccess() const ;

	
	// promote it to public
	using Thread::runException;
	using Thread::threadName ;
	
private:
	std::string _path ;
    
    bool _restoreOnSync;
    bool _noActions;
	
	bool _deleteUploadedFilepathOnSuccess;
	Mutex _deleteUploadedFilepathMutex; // not static since not for thread safety need: _deleteUploadedFilepathOnSuccess in principle could be set of get by class client and run method at the same time.

	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbUploadFile)
	
} ; 

_ACS_END_NAMESPACE

#endif // _dbUploadFile_H_
