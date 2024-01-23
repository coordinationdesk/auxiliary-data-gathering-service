// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $

*/


#ifndef _dbLTAEraseInventoryFile_H_
#define _dbLTAEraseInventoryFile_H_ 

#include <dbEraseFile.h>
#include <LTAInventoryObject.h>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbLTAEraseInventoryFile
//
//
//


/**
 * The class is specialized in erasing inventoried objects.
 * User must provide the LTAInventoryObject to be erased.
 * The file will be erased from any StorageManage that allows it.
 * The LTAInventoryObject must satisfy the following preconditions:
 * \li It must have already be saved in database
 */
class dbLTAEraseInventoryFile : public dbEraseFile  // PRQA S 2109
{
public:
	friend class dbLTAFileActionFactory ; // PRQA S 2107
	
	exDECLARE_EXCEPTION(SyncEraseException, EraseException) ; // SyncEraseException Exception. // PRQA S 2131, 2502 2
	
	dbLTAEraseInventoryFile() = delete ;
	dbLTAEraseInventoryFile(const dbLTAEraseInventoryFile &) = delete ;
	dbLTAEraseInventoryFile &operator=(const dbLTAEraseInventoryFile &) = delete ;
	dbLTAEraseInventoryFile(dbLTAEraseInventoryFile &&) = delete ;
	dbLTAEraseInventoryFile &operator=(dbLTAEraseInventoryFile &&) = delete ;

	virtual ~dbLTAEraseInventoryFile() = default ;

	// inherited from dbEraseFile
	/**
	 * Start the erase.
	 * \throw EraseException 
	 */
	virtual void start() ;

protected:
	dbLTAEraseInventoryFile(dbConnectionPool &, db::LTAInventoryObject &,int storageid=0);

private :
	db::LTAInventoryObject &_inventory ;
	std::string const _inventoryName;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAEraseInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 

#endif // _dbLTAEraseInventoryFile_H_
