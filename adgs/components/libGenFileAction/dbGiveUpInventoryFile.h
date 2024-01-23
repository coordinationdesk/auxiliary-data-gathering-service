// PRQA S 1050 EOF
/*

	Copyright 2021-, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Generic File Action Library $	

*/


#ifndef _dbGiveUpInventoryFile_H_
#define _dbGiveUpInventoryFile_H_ 

#include <dbGiveUpFile.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject ;
class SQLString;

//
//
//
// class dbGiveUpInventoryFile
//
//
//


/**
 * The class is specialized in cencelling previous download requests
 * User must provide the dbInventoryObject whose download shall be cancelled
 */
class dbGiveUpInventoryFile : public dbGiveUpFile // PRQA S 2109
{
public:	

	friend class dbGenFileActionFactory ; // PRQA S 2107

	virtual ~dbGiveUpInventoryFile() ACS_NOEXCEPT ;
	dbGiveUpInventoryFile() = delete ;
	dbGiveUpInventoryFile(const dbGiveUpInventoryFile &) = delete ;
	dbGiveUpInventoryFile &operator=(const dbGiveUpInventoryFile &) = delete ;
	dbGiveUpInventoryFile(dbGiveUpInventoryFile &&) = delete ;
	dbGiveUpInventoryFile &operator=(dbGiveUpInventoryFile &&) = delete ;
	
	
	// inherited from dbGiveUpFile
	/**
	 * Start the cancellation process.
	 */
	virtual void start() ;
	
protected:
	dbGiveUpInventoryFile(dbConnectionPool &, dbInventoryObject &) ;

private:
	// declared but not implemented to prevent their use
	SQLString getSyncSMsForDownloadQuery(bool hasADCfield, const std::string& dbKey) const;

private :
	dbInventoryObject &_inventory ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGiveUpInventoryFile) ;
} ; 

_ACS_END_NAMESPACE
 


#endif // _dbGiveUpInventoryFile_H_
