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
	Revision 1.5  2015/05/29 16:12:51  francesco.avanzi
	Aeolus added
	
	Revision 1.4  2015/05/25 14:15:43  marpas
	class methods and data members rationalization
	coding best practice applied
	
	Revision 1.3  2013/12/05 17:22:02  marpas
	moving static initialization methods in unnamed namespace into compilation unit
	
	Revision 1.2  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.1  2013/08/09 12:28:43  lucio.pulvirenti
	first issue.
	
	
	
*/ 

#include <StdSatelliteMissionAssociator.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(StdSatelliteMissionAssociator)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StdSatelliteMissionAssociator::StdSatelliteMissionAssociator():
								SatelliteMissionAssociator() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);			
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StdSatelliteMissionAssociator::~StdSatelliteMissionAssociator() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);		// PRQA S 4631	
}


map< std::string, std::string > StdSatelliteMissionAssociator::getSatelliteAcronymMap() 
{ 
   map<string, string>  m;

   m["S1A"] = "S1A";
   m["S1B"] = "S1B";
   m["S1_"] = "S1_";

   m["S2A"] = "S2A";
   m["S2B"] = "S2B";
   m["S2_"] = "S2_";

   m["S3A"] = "S3A";
   m["S3B"] = "S3B";
   m["S3C"] = "S3C";
   m["S3D"] = "S3D";
   m["S3_"] = "S3_";
	
   m["AE_"] = "AE" ;
   
   // Earth CARE
   m["ECA"] = "ECA";
   
   // MUSTER
   m["MUS"] = "MUS";

   m["S6A"] = "S6A";		
   m["S6B"] = "S6B";		
   m["S6_"] = "S6_";		

   m["S5P"] = "S5P";
   return m;
} 

map< std::string, int > StdSatelliteMissionAssociator::getMissionIdMap() 
{

   map<string, int>  m;

   m["S1A"] = 1;
   m["S1B"] = 1;
   m["S1_"] = 1;

   m["S2A"] = 1;
   m["S2B"] = 1;
   m["S2_"] = 1;

   m["S3A"] = 1;
   m["S3B"] = 1;
   m["S3C"] = 1;
   m["S3D"] = 1;
   m["S3_"] = 1;
   
   m["AE_"] = 1;
   
   m["ECA"] = 1;
   
   m["MUS"] = 1;
   
   m["S6A"] = 1;			    	   
   m["S6B"] = 1;	   
   m["S6_"] = 1;			    	   
   
   m["S5P"] = 1;			    	   

   return m; 
}


_ACS_END_NAMESPACE
