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

#ifndef _LTAStat_H_
#define _LTAStat_H_

#include <Statistics.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

inline
acs::math::StatNode &stat() { return (*acs::math::Statistics::instance())()["LTAFileAction"] ; } 

_ACS_END_NESTED_NAMESPACE

#endif // _LTAStat_H_
