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

#include <LTASMTmp.h>


using namespace acs ;
using namespace std ;


LTASMTmp::LTASMTmp(dbPersistent::IdType smid, const std::string &smtype, dbPersistent::IdType recid) :
	id(smid),
	cost(0),
	type(smtype),
	sminvrecordid(recid)
{
}

