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
 */

#include <InvRelationTypes.h>


_ACS_BEGIN_NAMESPACE(acs) ;

InvRelationTypes::~InvRelationTypes() throw() {
    //empty.
}

InvRelationTypes::InvRelationTypes(const std::string &key) :
    NameIdAssociator<short>("t_inv_relation_types", "id", "relation_type", key),
    ParamConstructibleSingletonMap<InvRelationTypes, std::string>()
{
    NameIdAssociator<short>::load() ;
}


_ACS_END_NAMESPACE;

