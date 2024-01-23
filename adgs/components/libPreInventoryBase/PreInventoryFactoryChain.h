/*
	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPreInventoryBase$

	$Id$

	$Author$

	$Log$
	Revision 1.1  2008/10/10 16:27:03  nicvac
	Factory added.
	
        
*/

#ifndef _PreInventoryFactoryChain_H_
#define _PreInventoryFactoryChain_H_

#include <acs_c++config.hpp>
#include <PreInventoryBase.h>
#include <StringKeyFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * PreInventory Factory
 **/

typedef StringKeyFactory<PreInventoryBase>::SingletonType PreInventoryFactoryChain;


_ACS_END_NAMESPACE

#endif //_PreInventoryFactoryChain_H_

