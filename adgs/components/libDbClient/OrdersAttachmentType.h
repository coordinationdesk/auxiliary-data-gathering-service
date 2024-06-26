// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Client Library $

	$Id$

	$Author$

	$Log$
	
*/

// This file is automatically generated by dbGenerator tool

#ifndef _OrdersAttachmentType_H_
#define _OrdersAttachmentType_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::ParamConstructibleSingletonMap ;

class OrdersAttachmentType: // PRQA S 2153
    public NameIdAssociator<unsigned int>,
    public ParamConstructibleSingletonMap<OrdersAttachmentType, std::string>
{
    friend OrdersAttachmentType* ParamConstructibleSingletonMap<OrdersAttachmentType, std::string>::instance(const std::string &); // PRQA S 2107

public:
	virtual ~OrdersAttachmentType() throw() {}  // PRQA S 2131
	
protected:
	explicit OrdersAttachmentType(const std::string &key) :

        NameIdAssociator<unsigned int>("T_OrdersAttachmentTypes", "Id", "Name", key),
        ParamConstructibleSingletonMap<OrdersAttachmentType, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	OrdersAttachmentType &operator=(const OrdersAttachmentType &) ; 	// declared but not implemented
	OrdersAttachmentType (const OrdersAttachmentType &) ; 			// declared but not implemented
	OrdersAttachmentType () ; 									// declared but not implemented
} ; 


_ACS_END_NAMESPACE

#endif // _OrdersAttachmentType_H_
