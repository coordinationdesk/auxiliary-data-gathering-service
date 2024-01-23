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


#ifndef _dbLTAGiveUpInventoryFile_H_
#define _dbLTAGiveUpInventoryFile_H_ 

#include <LTAInventoryObject.h>

#include <dbGiveUpFile.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class SQLString;

/**
 * The class is specialized in cencelling previous download requests
 * User must provide the inventory whose download shall be cancelled
 */
class dbLTAGiveUpInventoryFile : public dbGiveUpFile // PRQA S 2109
{
public:	

	friend class dbLTAFileActionFactory ; // PRQA S 2107

	virtual ~dbLTAGiveUpInventoryFile() = default ;
	dbLTAGiveUpInventoryFile() = delete ;
	dbLTAGiveUpInventoryFile(const dbLTAGiveUpInventoryFile &) = delete ;
	dbLTAGiveUpInventoryFile &operator=(const dbLTAGiveUpInventoryFile &) = delete ;
	dbLTAGiveUpInventoryFile(dbLTAGiveUpInventoryFile &&) = delete ;
	dbLTAGiveUpInventoryFile &operator=(dbLTAGiveUpInventoryFile &&) = delete ;
	
	
	// inherited from dbGiveUpFile
	/**
	 * Start the process.
	 */
	virtual void start() ;
	
protected:
	dbLTAGiveUpInventoryFile(dbConnectionPool &, db::LTAInventoryObject &);

private:

	SQLString getSyncSMsForDownloadQuery() const;
	
private :
	db::LTAInventoryObject &_inventory ;
    std::string _inventoryName ; 	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAGiveUpInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 


#endif // _dbLTAGiveUpInventoryFile_H_
