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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2017/10/05 16:02:16  marpas
	deprecated using declaration fixed
	
	Revision 5.3  2016/04/28 09:25:01  marpas
	Thread is now a real base calss (no longer virtual)
	
	Revision 5.2  2015/11/03 15:11:53  damdec
	PDSEV-12: Added possibility to disable actions on t_smactions table.
	
	Revision 5.1  2013/11/04 13:21:04  marpas
	Thread::waitExitOrTimeout in public scope
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.7  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.6  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.5  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/05/17 09:38:34  marfav
	Addedd SMUnknownFileException declaration as per S1PDGS-3291
	
	Revision 2.3  2009/06/26 10:40:13  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.1  2008/11/11 10:47:44  ivafam
	Exception Added
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.6  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.5  2004/10/07 09:40:56  ivafam
	dbFileAction added
	
	Revision 1.4  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.3  2003/06/04 17:38:22  paoscu
	Class inherits from Sender.
	
	Revision 1.2  2002/11/27 11:38:20  paoscu
	Doxygen doc.
	
	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
	Import libDbCryosat
		

*/


#ifndef _dbDownloadFile_H_
#define _dbDownloadFile_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <dbFileAction.h>
#include <Thread.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnection ;

//
//
//
// class dbDownloadFile
//
//
//

/**
 * The class in in charge to download a file to the local filesystem.
 * It implements the download in a separate thread
 * in order to download multiple files at time and optimize the transfer efficiency.
 */

class dbDownloadFile : public dbFileAction, protected Thread // PRQA S 1703, 2108, 2109, 2153, 2194
{
public:

	/*! class DownloadException declaration 
	 *
	 * This exception is thrown by downloading() if there was an error
	 * in download process.
	 */
	exDECLARE_EXCEPTION(DownloadException,dbFileAction::Exception) ; // PRQA S 2131, 2153, 2502
	
	/*! class DownloadAsynchException declaration */
	exDECLARE_EXCEPTION(DownloadAsynchException,dbFileAction::Exception) ; // PRQA S 2131, 2153, 2502

	/*! class SMUnknownFileException declaration */
	exDECLARE_EXCEPTION(SMUnknownFileException,dbFileAction::Exception) ; // PRQA S 2131, 2153, 2502

	/*! class FileToDownloadIsNearlineExc declaration */
	exDECLARE_EXCEPTION(FileToDownloadIsNearlineExc,dbFileAction::Exception) ; // PRQA S 2131, 2153, 2502

	/*! class FileToDownloadIsOfflineExc declaration */
	exDECLARE_EXCEPTION(FileToDownloadIsOfflineExc,dbFileAction::Exception) ; // PRQA S 2131, 2153, 2502

	// promote it to public
	using Thread::runException;
	using Thread::threadName;

public:

	explicit dbDownloadFile(dbConnectionPool &, dbPersistent &, const std::string &path = "");
	virtual ~dbDownloadFile() throw() ;

	virtual void start() = 0 ; // PRQA S 2121
	virtual bool isComplete() const ;
	virtual bool downloading() = 0 ;
    
    using Thread::waitExitOrTimeout ; // public scope
	/*! Action on t_smactions disabled
	 *
	 * This option, when set, will signal the actual dbDownloadFile object
	 * to disable actions on t_smactions table
	 */
    bool smActionsDisabled() const throw() {return _smActionsDisabled;};
    void smActionsDisabled(bool b) throw() {_smActionsDisabled = b;};
	
private:
	// declared but not implemented to prevent their use
	dbDownloadFile() ;
	dbDownloadFile(const dbDownloadFile &) ;
	dbDownloadFile &operator=(const dbDownloadFile &) ;
	
protected:
	std::string _path ; // PRQA S 2101
	
private:
	bool _smActionsDisabled;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDownloadFile) ;
	
} ; 

_ACS_END_NAMESPACE

#endif // _dbDownloadFile_H_
