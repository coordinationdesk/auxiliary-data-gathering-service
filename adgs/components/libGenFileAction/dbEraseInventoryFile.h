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
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.5  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.4  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.3  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.2  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	

*/


#ifndef _dbEraseInventoryFile_H_
#define _dbEraseInventoryFile_H_ 

#include <dbEraseFile.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject ;

//
//
//
// class dbEraseInventoryFile
//
//
//


/**
 * The class is specialized in erasing inventoried objects.
 * User must provide the dbInventoryObject to be erased.
 * The file will be erased from any StorageManage that allows it.
 * The file is optionally erased from the database if its blobsize is different from 0.
 * The dbInventoryObject must satisfy the following preconditions:
 * \li It must have already be saved in database
 * During the whole process the dbInventoryObject will be locked.
 */
class dbEraseInventoryFile : public dbEraseFile  // PRQA S 2109
{
public:
	friend class dbGenFileActionFactory ; // PRQA S 2107
	
	exDECLARE_EXCEPTION(SyncEraseException, exException) ; // SyncEraseException Exception. // PRQA S 2131, 2502 2
	
	virtual ~dbEraseInventoryFile() ACS_NOEXCEPT ;
	dbEraseInventoryFile() = delete ;
	dbEraseInventoryFile(const dbEraseInventoryFile &) = delete ;
	dbEraseInventoryFile &operator=(const dbEraseInventoryFile &) = delete ;
	dbEraseInventoryFile(dbEraseInventoryFile &&) = delete ;
	dbEraseInventoryFile &operator=(dbEraseInventoryFile &&) = delete ;


	// inherited from dbDownloadFile
	/**
	 * Start the erase.
	 * \throw dbPersistent::InvalidUse Record has already been saved or thread is already running.
	 */
	virtual void start() ;

protected:

	dbEraseInventoryFile(dbConnectionPool &, dbInventoryObject &,int storageid=0);

private :
	dbInventoryObject &_inventory ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbEraseInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 

#endif // _dbEraseInventoryFile_H_
