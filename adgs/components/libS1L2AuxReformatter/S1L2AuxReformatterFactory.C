// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.1  2012/04/23 08:02:39  damdec
	Class added.
	
        
*/ 

#include <S1L2AuxReformatterFactory.h>
#include <S1AuxIceReformatter.h>
#include <S1AuxWndReformatter.h>
#include <S1AuxWavReformatter.h>
#include <S1AuxTroReformatter.h>
#include <S1AuxTecReformatter.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1L2AuxReformatterFactory)


S1L2AuxReformatterFactory::S1L2AuxReformatterFactory() {

	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
	
	registerS1L2AuxReformatterFactoryMethod("AUX_ICE", &buildS1AuxIceReformatter);
	registerS1L2AuxReformatterFactoryMethod("AUX_WAV", &buildS1AuxWavReformatter);
	registerS1L2AuxReformatterFactoryMethod("AUX_WND", &buildS1AuxWndReformatter);
	registerS1L2AuxReformatterFactoryMethod("AUX_TRO", &buildS1AuxTroReformatter);
	registerS1L2AuxReformatterFactoryMethod("AUX_TEC", &buildS1AuxTecReformatter);
}

S1L2AuxReformatterFactory::~S1L2AuxReformatterFactory() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	unregisterS1L2AuxReformatterFactoryMethod("AUX_ICE", &buildS1AuxIceReformatter);
	unregisterS1L2AuxReformatterFactoryMethod("AUX_WAV", &buildS1AuxWavReformatter);
	unregisterS1L2AuxReformatterFactoryMethod("AUX_WND", &buildS1AuxWndReformatter);
	unregisterS1L2AuxReformatterFactoryMethod("AUX_TRO", &buildS1AuxTroReformatter);
	unregisterS1L2AuxReformatterFactoryMethod("AUX_TEC", &buildS1AuxTecReformatter);
}


S1L2AuxReformatter * S1L2AuxReformatterFactory::buildS1AuxIceReformatter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	return new S1AuxIceReformatter;

}

S1L2AuxReformatter * S1L2AuxReformatterFactory::buildS1AuxWavReformatter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	return new S1AuxWavReformatter;

}

S1L2AuxReformatter * S1L2AuxReformatterFactory::buildS1AuxWndReformatter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	return new S1AuxWndReformatter;

}
S1L2AuxReformatter * S1L2AuxReformatterFactory::buildS1AuxTroReformatter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	return new S1AuxTroReformatter;

}
S1L2AuxReformatter * S1L2AuxReformatterFactory::buildS1AuxTecReformatter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	return new S1AuxTecReformatter;

}
_ACS_END_NAMESPACE
