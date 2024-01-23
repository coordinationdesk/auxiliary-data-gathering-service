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

#ifndef _FlowTags_H_
#define _FlowTags_H_

#include<NameIdAssociator.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs) ;


using pattern::ParamConstructibleSingletonMap ;

class FlowTags : // PRQA S 2153
    public NameIdAssociator<short> ,
    public ParamConstructibleSingletonMap<FlowTags, std::string> {

    friend FlowTags* ParamConstructibleSingletonMap<FlowTags, std::string>::instance(const std::string &); // PRQA S 2107
public:
	FlowTags &operator=(const FlowTags &) = delete ;
	FlowTags (const FlowTags &) = delete ;
	FlowTags () = delete ;
 	~FlowTags() override = default ;
protected:
	explicit FlowTags(const std::string &key) ;
};

_ACS_END_NAMESPACE;


#endif /* FlowTags_H_ */
