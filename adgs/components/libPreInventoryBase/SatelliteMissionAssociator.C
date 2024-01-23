// PRQA S 1050 EOF
/*
	Copyright 2013-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2015/05/25 14:15:43  marpas
	class methods and data members rationalization
	coding best practice applied
	
	Revision 1.2  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.1  2013/08/09 12:26:18  lucio.pulvirenti
	First issue.
	
	
	
*/ 

#include <SatelliteMissionAssociator.h>

#include <StringUtils.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SatelliteMissionAssociator)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SatelliteMissionAssociator::SatelliteMissionAssociator()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);			
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SatelliteMissionAssociator::~SatelliteMissionAssociator() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);		// PRQA S 4631	
}

string SatelliteMissionAssociator::getSatelliteAcronym(string const & satelliteCode)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	string key = StringUtils::uppercase(satelliteCode);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to get satellite acronym value corresponding to key \"" << key << "\"");
	
	map<string, string> satelliteMap = getSatelliteAcronymMap();
	map<string, string>::const_iterator it = satelliteMap.find(key);
	ACS_COND_THROW(it == satelliteMap.end(), exKeyNotFoundException("Can't find \"" + key + "\" key in satellites acronym map"));	 // PRQA S 3081
	string value = it->second;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"" << key << "\" -> \"" << value << "\"");
	return value;
		
}

int SatelliteMissionAssociator::getMissionId(string const & satelliteCode)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	string key = StringUtils::uppercase(satelliteCode);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to get mission id corresponding to key \"" << key << "\"");
	
	map<string, int> missionMap = getMissionIdMap();
	map<string, int>::const_iterator it = missionMap.find(key);
	ACS_COND_THROW(it == missionMap.end(), exKeyNotFoundException("Can't find \"" + key + "\" key in missions map"));	 // PRQA S 3081
	int value = it->second;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"" << key << "\" -> " << value );
	return value;
}


_ACS_END_NAMESPACE
