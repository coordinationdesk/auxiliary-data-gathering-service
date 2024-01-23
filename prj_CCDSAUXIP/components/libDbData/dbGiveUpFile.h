// PRQA S 1050 EOF
/*

	Copyright 2021-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $
*/


#ifndef _dbGiveUpFile_H_
#define _dbGiveUpFile_H_ 


#include <acs_c++config.hpp>
#include <dbFileAction.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnectionPool ;

//
//
//
// class dbGiveUpFile
//
//
//

/**
 * The class in in charge to giveup a previous download request.
 */

class dbGiveUpFile : public dbFileAction // PRQA S 1703, 2108, 2109, 2153, 2194
{
public:

	dbGiveUpFile(dbConnectionPool &, dbPersistent &);
	virtual ~dbGiveUpFile() noexcept ;

	// declared but not implemented to prevent their use
	dbGiveUpFile() = delete ;
	dbGiveUpFile(const dbGiveUpFile &) = delete ;
	dbGiveUpFile &operator=(const dbGiveUpFile &) = delete ;
	dbGiveUpFile(const dbGiveUpFile &&) = delete ;
	dbGiveUpFile &operator=(const dbGiveUpFile &&) = delete ;
	
	virtual void start() = 0 ; // PRQA S 2121
	virtual bool isComplete() const ; 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGiveUpFile) ;
	
} ; 

_ACS_END_NAMESPACE

#endif // _dbGiveUpFile_H_
