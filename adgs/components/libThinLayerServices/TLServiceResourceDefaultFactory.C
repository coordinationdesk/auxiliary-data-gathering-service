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

#include <TLServiceResourceDefaultFactory.h>

#include <ThinLayerServicesFactoryChain.h>

#include <TLServiceSlotClient.h>
#include <TLServiceSlotSrv.h>

#include <Filterables.h>

#include "ThinLayerServicesV.h"

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(TLServiceResourceDefaultFactory)

static ThinLayerServicesV version;

TLServiceResourceDefaultFactory::TLServiceResourceDefaultFactory() {
	// CTOR of the factory
	// calls the register method
	// to allow default task table instantiation

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	registerFactory();
}

TLServiceResourceDefaultFactory::~TLServiceResourceDefaultFactory() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    try {
	    unregisterFactory();
    } catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}

void TLServiceResourceDefaultFactory::registerFactory () {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Registering default builder") ;

	// Generate a key composed of wildcards
	// to use this builder in any case but the overriden ones
	StringKeyElement wildCard = StringKeyElement::getWildCard();
	StringKey defaultKey (wildCard, wildCard, wildCard, wildCard, wildCard);

	// Register with the default key
	TLServiceResourceSrvFactoryChain::instance() -> registerFactory( defaultKey, &TLServiceResourceDefaultFactory::buildDefaultTLServiceResourceSrv );
	TLServiceResourceClientFactoryChain::instance() -> registerFactory( defaultKey, &TLServiceResourceDefaultFactory::buildDefaultTLServiceResourceClient );
}

void TLServiceResourceDefaultFactory::unregisterFactory () {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "un-registering default builder") ;

	// Generate again the default key
	StringKeyElement wildCard = StringKeyElement::getWildCard();
	StringKey defaultKey (wildCard, wildCard, wildCard, wildCard, wildCard);

	// Unregister the bulder from the factory chain
	TLServiceResourceSrvFactoryChain::instance() -> unregisterFactory( defaultKey, &TLServiceResourceDefaultFactory::buildDefaultTLServiceResourceSrv );
	TLServiceResourceClientFactoryChain::instance() -> unregisterFactory( defaultKey, &TLServiceResourceDefaultFactory::buildDefaultTLServiceResourceClient );
}

TLServiceResourceSrvBase*
TLServiceResourceDefaultFactory::buildDefaultTLServiceResourceSrv() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	return new TLServiceSlotSrv;
}

TLServiceResourceClientBase*
TLServiceResourceDefaultFactory::buildDefaultTLServiceResourceClient() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	return new TLServiceSlotClient;
}



_ACS_END_NAMESPACE

