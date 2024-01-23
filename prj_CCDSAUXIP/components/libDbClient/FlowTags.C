// PRQA S 1050 EOF
/*
 *
 *  Copyright 2018-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 *
	$Prod: A.C.S. DataBase Client Library $
 *

 */

#include <FlowTags.h>


_ACS_BEGIN_NAMESPACE(acs) ;


FlowTags::FlowTags(const std::string &key) :
    NameIdAssociator<short>("t_flowtags", "id", "flowtagname", key),
    ParamConstructibleSingletonMap<FlowTags, std::string>()
{
    NameIdAssociator<short>::load() ;
}


_ACS_END_NAMESPACE;

