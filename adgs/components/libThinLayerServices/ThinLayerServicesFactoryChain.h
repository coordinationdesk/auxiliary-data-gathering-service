/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libThinLayerServices$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _ThinLayerServicesFactoryChain_H_
#define _ThinLayerServicesFactoryChain_H_

#include <TLServiceResourceSrvBase.h>
#include <TLServiceResourceClientBase.h>

#include <StringKeyFactory.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Thin Layer Services Factories
**/

//Resource Server Factory
typedef StringKeyFactory<TLServiceResourceSrvBase>::SingletonType TLServiceResourceSrvFactoryChain;

//Resource Client Factory
typedef StringKeyFactory<TLServiceResourceClientBase>::SingletonType TLServiceResourceClientFactoryChain;

//Add here other factories implementing services for Thin Layer


_ACS_END_NAMESPACE

#endif //_ThinLayerServicesFactoryChain_H_

