// PRQA S 1050 EOF
/*
 *
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *
 *
 */

#ifndef _InvRelationTypes_H_
#define _InvRelationTypes_H_

#include<NameIdAssociator.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs) ;


using pattern::ParamConstructibleSingletonMap ;

class InvRelationTypes : // PRQA S 2153
    public NameIdAssociator<short> ,
    public ParamConstructibleSingletonMap<InvRelationTypes, std::string> {

    friend InvRelationTypes* ParamConstructibleSingletonMap<InvRelationTypes, std::string>::instance(const std::string &); // PRQA S 2107
public:
	InvRelationTypes &operator=(InvRelationTypes &&) = delete ;
	InvRelationTypes (InvRelationTypes &&) = delete ;
	InvRelationTypes &operator=(const InvRelationTypes &) = delete ;
	InvRelationTypes (const InvRelationTypes &) = delete ;
	InvRelationTypes () = delete ;
 	virtual ~InvRelationTypes() = default ;
protected:
	explicit InvRelationTypes(const std::string &key) ;
};

_ACS_END_NAMESPACE;


#endif /* InvRelationTypes_H_ */
