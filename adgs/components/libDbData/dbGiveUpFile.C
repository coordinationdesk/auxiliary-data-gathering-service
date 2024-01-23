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

	$Prod: A.C.S. Db Data Library $
*/

#include <dbGiveUpFile.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGiveUpFile)

// constructor
dbGiveUpFile::dbGiveUpFile(dbConnectionPool &pool, dbPersistent &p) :
	dbFileAction(pool, p)
{
}



dbGiveUpFile::~dbGiveUpFile() ACS_NOEXCEPT 
{
}

bool dbGiveUpFile::isComplete() const 
{
	return getCompleted() ;
}



_ACS_END_NAMESPACE
