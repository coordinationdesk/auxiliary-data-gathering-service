/*

	Copyright 1995-2004, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TlDaemon Default PCC factory library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2015/06/22 12:42:23  damdec
	Compilation warnings removed.
	
	Revision 1.2  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
*/

#include <DbAlternative_PCC.h>
#include <DefaultAlternative_PCC.h>
#include <DefaultTl_PCC.h>

#include <TlPreconditionCheckerFactory.h>
#include <AlternativeCheckerFactoryChain.h>
#include <PreconditionCheckerFactoryChain.h>

#include <tldpccfV.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(TlPreconditionCheckerFactory)

static tldpccfV Version;

TlPreconditionCheckerFactory::TlPreconditionCheckerFactory () 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	StringKeyElement wildCard = StringKeyElement::getWildCard();	

	StringKey dbKey (wildCard, wildCard, wildCard, wildCard, StringKeyElement("DB"));
	AlternativeCheckerFactoryChain::instance()->registerFactory(dbKey, &buildDbAlternative_PCC, "Default-DBAlt");

	StringKey defaultKey (wildCard, wildCard, wildCard, wildCard, wildCard);
	AlternativeCheckerFactoryChain::instance()->registerFactory(defaultKey, &buildDefaultAlternative_PCC, "Default-Alt");
	PreconditionCheckerFactoryChain::instance()->registerFactory (defaultKey, &buildDefaultTl_PCC, "Default-PCC");
}

TlPreconditionCheckerFactory::~TlPreconditionCheckerFactory ()  throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

    try {
	    StringKeyElement wildCard = StringKeyElement::getWildCard();	

	    StringKey dbKey (wildCard, wildCard, wildCard, wildCard, StringKeyElement("DB"));
	    AlternativeCheckerFactoryChain::instance()->unregisterFactory(dbKey, &buildDbAlternative_PCC);

	    StringKey defaultKey (wildCard, wildCard, wildCard, wildCard, wildCard);
	    AlternativeCheckerFactoryChain::instance()->unregisterFactory(defaultKey, &buildDefaultAlternative_PCC);
	    PreconditionCheckerFactoryChain::instance()->unregisterFactory (defaultKey, &buildDefaultTl_PCC);
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}


Alternative_PCC*
TlPreconditionCheckerFactory::buildDbAlternative_PCC (const PCC_Base::PCC_ParamsPtr& parameters)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return new DbAlternative_PCC(parameters);
}

Alternative_PCC*
TlPreconditionCheckerFactory::buildDefaultAlternative_PCC (const PCC_Base::PCC_ParamsPtr& parameters)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return new DefaultAlternative_PCC(parameters);
}

ThinLayer_PCC* 
TlPreconditionCheckerFactory::buildDefaultTl_PCC (const StringKey& parameter)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return new DefaultTl_PCC(parameter);
}

// Embedded Factory
TlPreconditionCheckerFactory _embeddedTlPreconditionCheckerFactory;

_ACS_END_NAMESPACE
