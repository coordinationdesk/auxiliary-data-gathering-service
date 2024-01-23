/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TlDaemon$

	$Id$

	$Author$

	$Log$
        
*/ 

#include <OrderReservedResources.h>

#include <OrderExecutionMgr.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderReservedResources)

OrderReservedResources::OrderReservedResources( OrderExecutionMgr& orderExecutionMgr,
		const bool& isService,
		const string& serviceResManagerKey,
		const string& clientId,
		const string& resManagerUrl
):
	_orderExecutionMgr(orderExecutionMgr),

	_isService            (isService           ),
	_serviceResManagerKey (serviceResManagerKey),
	_clientId             (clientId            ),
	_resManagerUrl        (resManagerUrl       )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

OrderReservedResources::~OrderReservedResources() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	_orderExecutionMgr.releaseClientResources ( _clientId );
}

_ACS_END_NAMESPACE

