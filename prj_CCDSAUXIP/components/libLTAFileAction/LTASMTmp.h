// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA File Action library $	

*/


#ifndef _LTASMTmp_H_
#define _LTASMTmp_H_ 

#include <dbPersistent.h>

namespace acs {

struct LTASMTmp // PRQA S 2173, 2175
{

	LTASMTmp(dbPersistent::IdType smid, const std::string &smtype, dbPersistent::IdType recid) ;
	LTASMTmp() = default ;
	~LTASMTmp() = default ;
	LTASMTmp(const LTASMTmp &) = default ;
	LTASMTmp &operator=(const LTASMTmp &) = default ;
	LTASMTmp(LTASMTmp &&) = default ;
	LTASMTmp &operator=( LTASMTmp &&) = default ;

    // PRQA S 2100 L1
	dbPersistent::IdType id ;
	int cost ;
	std::string type ;
	dbPersistent::IdType sminvrecordid ;
    // PRQA L:L1
} ;


} // namespace


#endif // _dbUploadInventoryFile_H_
