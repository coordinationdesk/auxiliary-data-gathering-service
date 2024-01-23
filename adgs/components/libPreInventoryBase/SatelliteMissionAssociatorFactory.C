// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2013/08/09 12:24:52  lucio.pulvirenti
	First issue.
	
	

*/


#include <SatelliteMissionAssociatorFactory.h>

#include <SatelliteMissionAssociatorSingleton.h>
#include <StdSatelliteMissionAssociator.h>
#include <PlainSatelliteMissionAssociator.h>

#include <exMacros.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(SatelliteMissionAssociatorFactory);

SatelliteMissionAssociatorFactory::SatelliteMissionAssociatorFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	SatelliteMissionAssociatorSingleton::instance()->registerFactory("STANDARD",&buildStdSatelliteMissionAssociator ) ;	
	SatelliteMissionAssociatorSingleton::instance()->registerFactory("PLAIN",&buildPlainSatelliteMissionAssociator ) ;	
	
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
}


SatelliteMissionAssociatorFactory::~SatelliteMissionAssociatorFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB); 
			
	SatelliteMissionAssociatorSingleton::instance()->unregisterFactory("STANDARD",&buildStdSatelliteMissionAssociator ) ;	
	SatelliteMissionAssociatorSingleton::instance()->unregisterFactory("PLAIN",&buildPlainSatelliteMissionAssociator ) ;	
		
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
    // PRQA L:L1
}

// *************** buildStdSatelliteMissionAssociator ***************************
SatelliteMissionAssociator* SatelliteMissionAssociatorFactory::buildStdSatelliteMissionAssociator()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	return new StdSatelliteMissionAssociator();
}

// *************** buildPlainSatelliteMissionAssociator ***************************
SatelliteMissionAssociator* SatelliteMissionAssociatorFactory::buildPlainSatelliteMissionAssociator()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	return new PlainSatelliteMissionAssociator();
}

_ACS_END_NAMESPACE
