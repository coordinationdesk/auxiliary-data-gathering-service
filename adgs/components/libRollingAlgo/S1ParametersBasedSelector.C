// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libRollingAlgo$

 */

#include <S1ParametersBasedSelector.h>
#include <RollingAlgoUtils.h>

#include <Application.h>

#include <InvPolicy.h>

#include <ConfigurationSingleton.h>

#include <DateTime.h>
#include <UtilXml.h>
#include <File.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1ParametersBasedSelector)



S1ParametersBasedSelector::S1ParametersBasedSelector(const InvPolicy & policy):
InvPolicyCommonSelector(policy),
_latencyHours()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631

	policy.get_latencyhours ( _latencyHours );
}

S1ParametersBasedSelector::~S1ParametersBasedSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

string S1ParametersBasedSelector::getSpecificTimeAndCentreQuery( string const& localCentreID, // PRQA S 4214
		double const& maxDeltaOldProducts_days, double const& rollingPolicyNotUsedProducts_days, dbConnection& conn ) const {

	DateTime dtNow;
	DateTime inventoryDateThresholdA;
	{
		DateTime::JD50 jdNow = dtNow;  // PRQA S 3050
		DateTime::JD50 jdThreshold( jdNow.jd() - _latencyHours/24.0 ); // PRQA S 3011, 3084
		inventoryDateThresholdA = jdThreshold;
	}

	DateTime inventoryDateThresholdB;
	{
		DateTime::JD50 jdNow = dtNow; // PRQA S 3050
		DateTime::JD50 jdThreshold( jdNow.jd() - rollingPolicyNotUsedProducts_days );
		inventoryDateThresholdB = jdThreshold;
	}


	ACS_LOG_INFO("Policy id ["<<_polId<<"]. Filtering ["<<_polFileType<<"] Inventory items matching: 1. Republished/Migrated (i.e. NOT produced at ["<<localCentreID<<"] OR inventory date - validity stop > "<<maxDeltaOldProducts_days<<" [days]); 2. Inventory date < "<<inventoryDateThresholdB.YYYYMMDDThhmmss() );
	ACS_LOG_INFO("Policy id ["<<_polId<<"]. Filtering ["<<_polFileType<<"] Inventory items matching: 1. NOT (Republished/Migrated) ; 2. Inventory date < "<<inventoryDateThresholdA.YYYYMMDDThhmmss() );

	ostringstream days;
	days<<fixed<<setprecision(2)<<maxDeltaOldProducts_days<<" days"; // PRQA S 4400

	ostringstream ossRepublishedOrMigrated;
	ossRepublishedOrMigrated<<
		"( "
		"  ( inv.processingstationid is not NULL AND inv.processingstationid != (select stationid from t_stations where stationacronym = "<<db::toSQL(conn, localCentreID)<<") ) "
		"  OR "
		"  ( (inv.inventorydate - inv.validitystop) > (interval "<<db::toSQL(conn, days.str())<<") ) "
		")";

	ostringstream ossSql;
	ossSql<<
	"select inv.id_inv as "<< _invIdAttributeName <<" "
	"from "
	"t_inventory inv, "
	"t_inventorysm invsm "
	"where "
	"invsm.inv_id = inv.id_inv and "
	"inv.validityflag 		= "		<<db::toSQL(conn, true)<<			" and "<<
	 (0 == _polStorage      ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<" "<<
	 (_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" "<<
	"inv.filetype in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "
	" and "
	"( "
	"   ( "<<ossRepublishedOrMigrated.str()<<" AND inv.inventorydate < "<<db::toSQL(conn, inventoryDateThresholdB, dbConv::YearToFractionExtended) << ") "
	"   OR "
	"   ( NOT "<<ossRepublishedOrMigrated.str()<<" AND inv.inventorydate < "<<db::toSQL(conn, inventoryDateThresholdA, dbConv::YearToFractionExtended) << ") "
	")";
	return ossSql.str();

}


void S1ParametersBasedSelector::selectInit(dbConnection &conn) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream polId; polId<<_polId;

	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	try {

		//*/Read Parameters File - Now not used. Maybe in the future to add other types of rolling criteria (cf. S2 example)
		//XmlDocSptr xmlDocSptr( new XmlDoc );
		//xmlDocSptr->initWithFile(_paramFileName);

		string localCentreID("");
		double maxDeltaOldProducts_days(0.0);
		double rollingPolicyNotUsedProducts_days(0.0);
		try {

			//Get parameter for the rolling policy
			conf.getValue( "Station.CentreID", localCentreID );
			conf.getValue( "RollingArchive.S1ParametersBasedSelector.MaxDeltaOldProducts_days", maxDeltaOldProducts_days);
			conf.getValue( "RollingArchive.S1ParametersBasedSelector.RollingPolicyNotUsedProducts_days", rollingPolicyNotUsedProducts_days);

		} catch( exception& ex ) {
			ACS_THROW( InvPolicySelector::exInvPolicySelectorException(ex, "Error getting parameters for policy Id ["+polId.str()+"]: Check configuration.") );

		}
		_sqlSelections.push_back( this->getSpecificTimeAndCentreQuery(localCentreID, maxDeltaOldProducts_days, rollingPolicyNotUsedProducts_days, conn ) );


	} catch(exception& ex) {
		ACS_THROW(InvPolicySelector::exInvPolicySelectorException(ex, "Error in processing policy Id ["+polId.str()+"]" ));
	}
}


_ACS_END_NAMESPACE

