// PRQA S 1050 EOF
/*
 *
 *  Copyright 2018, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *
 *
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
 	virtual ~FlowTags() throw() ;
protected:
	explicit FlowTags(const std::string &key) ;
private:
	FlowTags &operator=(const FlowTags &) ; 	// declared but not implemented
	FlowTags (const FlowTags &) ; 			// declared but not implemented
	FlowTags () ; 									// declared but not implemented
};

_ACS_END_NAMESPACE;


#endif /* FlowTags_H_ */
