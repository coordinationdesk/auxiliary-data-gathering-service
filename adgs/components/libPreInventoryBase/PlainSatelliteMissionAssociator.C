// PRQA S 1050 EOF
/*
	Copyright 2013-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.1  2013/08/09 12:29:36  lucio.pulvirenti
	First issue.
	
	
	
*/ 

#include <PlainSatelliteMissionAssociator.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
namespace {

	 map<string, string> createSatelliteAcronymMap()
     {
       map<string, string>  m;

	   m["S1A"] 		= "S1";
	   m["S1B"] 		= "S1";
	   m["S1_"] 		= "S1";

	   m["S2A"] 		= "S2";
	   m["S2B"] 		= "S2";
	   m["S2_"] 		= "S2";

	   m["S3A"] 		= "S3";
	   m["S3B"] 		= "S3";
	   m["S3C"] 		= "S3";
	   m["S3D"] 		= "S3";
	   m["S3_"] 		= "S3";

	   m["S6A"] 		= "S6";
	   m["S6B"] 		= "S6";
	   m["S6_"] 		= "S6";

	   m["S5P"] 		= "S5";

       return m;
     }

	 map<string, int> createMissionIdMap()
     {
       map<string, int>  m;

	   m["S1A"] 		= 1;
	   m["S1B"] 		= 2; // PRQA S 4400
	   m["S1_"] 		= 0;

	   m["S2A"] 		= 1;
	   m["S2B"] 		= 2; // PRQA S 4400
	   m["S2_"] 		= 0;

	   m["S3A"] 		= 1;
	   m["S3B"] 		= 2; // PRQA S 4400
	   m["S3C"] 		= 3; // PRQA S 4400
	   m["S3D"] 		= 4; // PRQA S 4400
	   m["S3_"] 		= 0;

	   m["S6A"] 		= 1;
	   m["S6B"] 		= 2; // PRQA S 4400
	   m["S6_"] 		= 0;

	   m["S5P"] 		= 1;

       return m;
     }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(PlainSatelliteMissionAssociator)

const map< string, string > PlainSatelliteMissionAssociator::_satelliteAcronymMap = createSatelliteAcronymMap();
const map< string, int > PlainSatelliteMissionAssociator::_missionIdMap = createMissionIdMap();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlainSatelliteMissionAssociator::PlainSatelliteMissionAssociator():
								SatelliteMissionAssociator() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);			
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlainSatelliteMissionAssociator::~PlainSatelliteMissionAssociator() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);		// PRQA S 4631	
}

_ACS_END_NAMESPACE
