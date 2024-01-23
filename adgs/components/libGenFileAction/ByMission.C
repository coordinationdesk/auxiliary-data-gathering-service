// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Signature Algo Library $

	$Id$

	$Author$

	$Log$
	Revision 7.3  2017/11/16 15:02:29  lucio.pulvirenti
	S3EVOL-78: CTOR: if input empty string, set configuration tag in base class with the proper method.
	
	Revision 7.2  2017/11/15 16:25:28  lucio.pulvirenti
	S3EVOL-78: If proper tag not found in configuration, exception thrown. Debug improved.
	
	Revision 7.1  2017/11/14 16:35:01  lucio.pulvirenti
	S3EVOL-78: work in progress.
	
	

*/


#include <ByMission.h>

#include <dbInventoryObject.h>
#include <NameIdAssociator.h>
#include <dbSmartPersistent.h>
#include <ConfigurationSingleton.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
	const string defaultConfTag = "ByMission";
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ByMission)

ByMission::ByMission(const std::string & confTag) : 
			UploadPredicate(confTag)
{
	if( confTag.empty() ) {
		setConfigurationTag(defaultConfTag);
	}
}
	
ByMission::~ByMission() throw()
{
}

ByMission::ByMission(const ByMission &r) : UploadPredicate(r) 
{
}

ByMission& ByMission::operator=(const ByMission &r) 
{
    if (this != &r) {
        UploadPredicate::operator=(r) ;
    }
    return *this ;
}

bool ByMission::pass(dbPersistent::IdType smId, const dbInventoryObject & inv, dbConnection &conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	bool isSet = false ;
	int satelliteId = inv.getSatelliteId(&isSet);
	ACS_COND_THROW( not isSet, UploadPredicateException("Satellite id not set in inventory object")); // PRQA S 3081
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite id="  << satelliteId );
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
	string invSatellite = nameIdAssociator.name(satelliteId);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << invSatellite << '\"' );
	
	bool missionSet = false ;
	int invMission = inv.getMission(&missionSet)  ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission="  << invMission << ( missionSet ? "" : " ( not set)" ) ); // PRQA S 3380
	
	dbSmartPersistent rec("t_storagemanager", conn);
	if( not rec.load(conn, smId) ) { // it should not occur
		ostringstream exMsg;
		exMsg << "t_storagemanager record with id=" << smId << " not found!";
		ACS_THROW( UploadPredicateException(exMsg.str()) ); // PRQA S 3081
	}
	string smName;
	bool isNull = false ;
	rec.get("name", smName, isNull, conn); 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SM name=\""  << smName << '\"' );
	
	string localRootTag = getConfigurationNode() + ".List_Of_Elements.Element";
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "localRootTag = \"" << localRootTag << '\"') ;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, rs);
	size_t numOfEl = rs.getArraySize(localRootTag);
	ACS_COND_THROW( numOfEl == 0, UploadPredicateException("Cannot find \""+localRootTag+ "\" tag in configuration")); // PRQA S 3081
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Elements=" << numOfEl) ;
	
	bool passed = false;
	for( size_t i = 0; i < numOfEl; i++ ) {
		
		string parentTag = localRootTag;
		if( numOfEl > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			parentTag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "parentTag=\"" << parentTag << '\"') ;
		
		string satellite;
		rs.getValue(parentTag+".Satellite", satellite);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite from configuration=" << satellite);
		if( not StringUtils::equals(invSatellite, satellite) ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Satellite from configuration does not match the inventory record one: \"" << invSatellite << "\", skipping");
			continue;
		}

// The check goes on if one of the following conditions occur:
// 1. Mission is configured AND matching the inventory obj one
// 2. Mission is not configured AND the inventory obj one not set
		
		string tmpTag = parentTag+".Mission";
		int mission = 0 ;
		if( rs.hasKey(tmpTag) ) {
			rs.getValue(tmpTag, mission);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "mission from configuration=" << mission);
			if( not missionSet || ( mission != invMission ) ) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Mission from configuration does not match the inventory record one: "  << invMission << ( missionSet ? "" : " ( not set)" ) << ", skipping" ); // PRQA S 3380
				continue;
			}
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "mission not configured");
			if( missionSet ) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "mission from inventory record is " << invMission << ": skipping" );
				continue;
			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "mission from inventory record not set: going on" );
			}
		}

		vector<string> smV;
		size_t smSize = rs.getArray(parentTag+".List_Of_SMs.SM", smV); // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, smSize << " SMs read from configuration") ;
		
		if( find (smV.begin(), smV.end(), smName) != smV.end() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' <<smName << "\" SM found in the configured list for satellite=\"" << invSatellite << "\" mission=" << invMission << ( missionSet ? "" : " ( not set)" ) ); // PRQA S 3380
			passed = true;
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' <<smName << "\" SM  not found in the configured list" );
		}
		
		break;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "passed: " << boolalpha << passed);
	
	return passed;
}


_ACS_END_NAMESPACE
