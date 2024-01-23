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

#include <S2ParametersBasedSelector.h>
#include <RollingAlgoUtils.h>

#include <Application.h>

#include <InvPolicy.h>

#include <SQLDialectFactory.h>

#include <ConfigurationSingleton.h>

#include <DateTime.h>
#include <UtilXml.h>
#include <File.h>

#include <Filterables.h>

#include <boost/algorithm/string.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S2ParametersBasedSelector)



S2ParametersBasedSelector::S2ParametersBasedSelector(const InvPolicy & policy):
InvPolicyCommonSelector(policy)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

S2ParametersBasedSelector::~S2ParametersBasedSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

string S2ParametersBasedSelector::getExpertQuery( std::string const& parExpertWhereStatement, dbConnection& conn ) const { // PRQA S 4214

	ostringstream ossSql;
	ossSql<<
		"select id_inv as "<< _invIdAttributeName <<"\n"<<
		"from 	t_inventorysm \
				inner join t_inventory on inv_id = id_inv \
		where \
		validityflag = " <<db::toSQL(conn, true)<< " and "<<
		((0 == _polStorage)    ? "" : ("storagemanid = "+db::toSQL(conn, _polStorage  )+" and ") ) <<
		(_polFileClass.empty() ? "" : ("fileclass = "   +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
		filetype in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<"	and "<<
		parExpertWhereStatement;

	ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with Expert condition ["<<parExpertWhereStatement<<"]" );

	return ossSql.str();
}

string S2ParametersBasedSelector::getCloudCoverQuery(std::string const& parCloudCovThreshold, dbConnection& conn) const { // PRQA S 4214

	long double cloudPctThr(0);
	istringstream ss(parCloudCovThreshold);
	if ( !(ss >> cloudPctThr) ) {
		ACS_THROW(InvPolicySelector::exInvPolicySelectorException("Cannot convert cloud percentage ["+parCloudCovThreshold+"] to numeric value.")); // PRQA S 3081
	}

	ostringstream ossSql;
	ossSql<<
		"select inv.id_inv as "<< _invIdAttributeName <<"\n"<<
		"from 	t_inventorysm 				invsm \
				inner join t_inventory 		inv 	on invsm.inv_id	= inv.id_inv \
		where \
		inv.validityflag 	= "	<<db::toSQL(conn, true)<<" 			and "<<
		((0 == _polStorage)    ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<
		(_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
		inv.filetype 		in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<" and \
		inv.cloud_pctg		>= "<<db::toSQL(conn, cloudPctThr);

	ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with Cloud Coverage >= "<<cloudPctThr<<"%" );

	return ossSql.str();
}

string S2ParametersBasedSelector::getTimeThresholdQuery( string const& parTimeEvent, string const& parTimeThreshold, dbConnection& conn ) const { // PRQA S 4214

	string dialect = SQLDialectFactory::dialectName( conn.getKey() ) ;

	string limitClause = "limit";
	if ( "Oracle11" == dialect ) {
		limitClause = "where ROWNUM <=";
	}

	long double hoursThreshold(0);
	istringstream ss(parTimeThreshold);
	if ( !(ss >> hoursThreshold) ) {
		ACS_THROW(InvPolicySelector::exInvPolicySelectorException("Cannot convert hours ["+parTimeThreshold+"] to numeric value.")); // PRQA S 3081
	}

	DateTime dtThreshold;
	{
		DateTime dtNow;
		DateTime::JD50 jdNow = dtNow; // PRQA S 3050
		DateTime::JD50 jdThreshold( jdNow.jd() - hoursThreshold/24.0 );
		dtThreshold = jdThreshold;
	}

	ostringstream ossSqlTime;

	if (parTimeEvent == "inventoried") {

		//ACS_LOG_INFO(" Filtering ["<<_polFileType<<"] Inventory items with inventory date <= "<<dtThreshold.YYYYMMDDThhmmss() );
		ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with inventory date <= "<<dtThreshold.asISO8601(0) );

		ossSqlTime <<
				"select inv.id_inv as "<< _invIdAttributeName <<"\n"<<
				"from \
				 t_inventory inv, \
				 t_inventorysm invsm \
				 where \
				 invsm.inv_id = inv.id_inv and \
				 inv.validityflag = "	<<db::toSQL(conn, true)<<	" and \
				 inv.filetype in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<" and "<<
				 ((0 == _polStorage)      ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<
				 (_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
				 inv.inventorydate <= "<<db::toSQL(conn, dtThreshold, dbConv::YearToFractionExtended);

	} else if (parTimeEvent == "ltastored") {

		//	ACS_LOG_INFO(" Filtering ["<<_polFileType<<"] Inventory items with Long Term Archive migration date <= "<<dtThreshold.YYYYMMDDThhmmss() );
		ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with Long Term Archive migration date <= "<<dtThreshold.asISO8601(0) );

		ossSqlTime <<
				"With theSmMigrated as ( \
					select inv.id_inv, sm.type as smtype \
					from   t_inventorysm 			invsm \
						inner join t_inventory 		inv 	on inv.id_inv 	= invsm.inv_id \
						inner join t_storagemanager 	sm 	on sm.id 	= invsm.storagemanid \
					where \
					inv.validityflag 	= "<<db::toSQL(conn, true)<<    " and "<<
					((0 == _polStorage)      ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<
					(_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
					inv.filetype 		in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<" \
				), asynchMigrated as ( \
					select invsm.inv_id, invsm.stored_date \
					from   t_inventorysm 			invsm \
						inner join t_storagemanager 	sm 	on sm.id 	 = invsm.storagemanid \
						inner join theSmMigrated	tSmMig	on tSmMig.id_inv = invsm.inv_id \
					where \
					(sm.syncupload = "<<db::toSQL(conn, false)<<" or sm.syncdownload = "<<db::toSQL(conn, false)<<" ) and \
					sm.type 		not in (select smtype from theSmMigrated "<<limitClause<<" 1) \
				), itemsToMigratePending as ( \
					select inv.id_inv \
					from 	t_smactions 		smAct \
						inner join t_inventory	inv	on inv.id_inv = smAct.inv_id \
					where \
					smAct.operation		= "<<db::toSQL(conn, "UPLOAD")<<" and \
					smAct.packageid		is Null \
				), itemsToMigrateNotCompleted as ( \
					select inv.id_inv \
					from 	t_smactions 		smAct \
						inner join t_inventory	inv	on inv.id_inv = smAct.inv_id \
						inner join t_smqueue	smQ	on smQ.packageid = smAct.packageid \
					where \
					smAct.operation	=  "<<db::toSQL(conn, "UPLOAD")<<" and \
					smQ.status		!= "<<db::toSQL(conn, "COMPLETED")<<" \
				) \
				select  asyMig.inv_id as "<< _invIdAttributeName <<"\n"<<
				"from 	asynchMigrated 			asyMig \
				where \
				asyMig.inv_id not in (select id_inv from itemsToMigratePending UNION select id_inv from itemsToMigrateNotCompleted) and \
				asyMig.stored_date <= "<<db::toSQL(conn, dtThreshold, dbConv::YearToFractionExtended);


	} else if (parTimeEvent == "ltarestored") {

		ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with restore date <= "<<dtThreshold.asISO8601(0) );
		//		ACS_LOG_INFO(" Filtering ["<<_polFileType<<"] Inventory items with restore date <= "<<dtThreshold.YYYYMMDDThhmmss() );

		ossSqlTime <<
				"With theSmMigrated as ( \
					select inv.id_inv, invsm.stored_date, sm.type as smtype \
					from   t_inventorysm invsm \
							inner join t_inventory 		inv	on invsm.inv_id	= inv.id_inv \
							inner join t_storagemanager sm 	on sm.id		= invsm.storagemanid \
					where  \
					inv.validityflag 	= "	<<db::toSQL(conn, true)<<		" and "<<
					((0 == _polStorage)     ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<
					(_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
					inv.filetype 		in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<" \
				), asynchMigrated as ( \
					select invsm.inv_id, invsm.stored_date \
					from   t_inventorysm invsm \
							inner join t_storagemanager sm 	on invsm.storagemanid = sm.id \
					where \
					(sm.syncupload = "<<db::toSQL(conn, false)<<" or sm.syncdownload = "<<db::toSQL(conn, false)<<" ) and \
					sm.type 			not in (select smtype from theSmMigrated "<<limitClause<<" 1) and \
					invsm.inv_id 		in (select id_inv from theSmMigrated) \
				), restoredItems as ( \
					select smMig.id_inv, smMig.stored_date \
					from theSmMigrated smMig \
							inner join asynchMigrated asmMig on smMig.id_inv = asmMig.inv_id \
					where \
					smMig.stored_date 	>= asmMig.stored_date \
				) \
				select id_inv as "<< _invIdAttributeName <<"\n"<<
				"from restoredItems \
				where \
				stored_date <= "<<db::toSQL(conn, dtThreshold, dbConv::YearToFractionExtended);

	} else if (parTimeEvent == "downloaded") {

		vector< string > confClientList;
		rsResourceSet& conf = ConfigurationSingleton::instance()->get();
		conf.getArray("RollingArchive.S2ParametersBasedSelector.DownloadedTimeEvent.List_Of_Client.Client", confClientList);

		ACS_COND_THROW( confClientList.empty(), // PRQA S 3081
				InvPolicySelector::exInvPolicySelectorException("Missing Client List in Configuration for TimeThreshold event ["+parTimeEvent+"]."));

		ostringstream ossClients;
		for ( vector<string>::iterator it=confClientList.begin(); it!=confClientList.end(); ++it) { // PRQA S 4238
			if ( it!=confClientList.begin() ) { ossClients<<", "; }
			ossClients<<db::toSQL(conn, *it);
		}

		//	ACS_LOG_INFO(" Filtering ["<<_polFileType<<"] Inventory items with download date from Archive to Clients "<<ossClients.str()<<" <= "<<dtThreshold.YYYYMMDDThhmmss() );
		ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with download date from Archive to Clients "<<ossClients.str()<<" <= "<<dtThreshold.asISO8601(0) );

		ossSqlTime <<
				"select invDownl.id_inv as "<< _invIdAttributeName <<"\n"<<
				"from \
					( \
						select inv.id_inv, max(invDownl.downloadtime) as lastDownloadtime \
						from 	t_inventorysm invsm \
								inner join t_inventory 		inv 		on invsm.inv_id 		= inv.id_inv \
								inner join t_inv_downloads 	invDownl 	on invDownl.inv_id 		= inv.id_inv \
								inner join t_inv_client 	invCl		on invCl.id_inv_client 	= invDownl.client_id \
						where \
						inv.validityflag 		= "		<<db::toSQL(conn, true)<<			" and "<<
						((0 == _polStorage)      ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<
						(_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
						inv.filetype 			in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<" and \
						invCl.name 				in ( "	<<ossClients.str()<<" ) \
						group by \
						inv.id_inv \
					) as invDownl  \
				where \
				invDownl.lastDownloadtime 	<= "	<<db::toSQL(conn, dtThreshold, dbConv::YearToFractionExtended);

	} else if (parTimeEvent == "exported") {

		//		ACS_LOG_INFO(utility::FuncName_(__PRETTY_FUNCTION__)<<": Filtering ["<<_polFileType<<"] Inventory items with Export date <= "<<dtThreshold.YYYYMMDDThhmmss() );
		ACS_LOG_INFO("Policy id ["<<_polId<<"] ["<<_polFileType<<"] Inventory items with Export date <= "<<dtThreshold.asISO8601(0) );

		ossSqlTime <<
				"select inv.id_inv as "<< _invIdAttributeName <<"\n"<<
				"from 	t_inventorysm invsm \
						inner join t_inventory 			inv 	on invsm.inv_id		= inv.id_inv \
						inner join t_inv_distributions	invDist	on invDist.inv_id	= inv.id_inv \
				where \
				inv.validityflag 	= "	<<db::toSQL(conn, true)<<"			and "<<
				((0 == _polStorage)      ? "" : ("invsm.storagemanid = " +db::toSQL(conn, _polStorage  )+" and ") ) <<
				(_polFileClass.empty() ? "" : ("inv.fileclass = "      +db::toSQL(conn, _polFileClass)+" and ") ) <<" \
				inv.filetype 		in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") "<<"	and \
				invDist.retain_counter	<= 0 								and \
				invDist.last_update  <= "<<db::toSQL(conn, dtThreshold, dbConv::YearToFractionExtended);

	} else {
		ACS_THROW( // PRQA S 3081
				InvPolicySelector::exInvPolicySelectorException("Error in Parameter file: TimeThreshold event ["+parTimeEvent+"] not supported."));
	}

	return ossSqlTime.str();

}

void S2ParametersBasedSelector::selectInit(dbConnection &conn) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream polId; polId<<_polId;

	try {

		// - Read Parameters File
		XmlDocSptr xmlDocSptr( new XmlDoc );
		xmlDocSptr->initWithString(_polParameters);

		//ACS_LOG_INFO(" Filtering ["<<_polFileType<<"] Inventory items with Following conditions (All matching):" );

		//Read configured CloudCoverage Threshold condition
		bool parCloudCoverEnabled(false);
		string parCloudCovThreshold("");
		try {
			xmlDocSptr->getValue("//CloudCoverageThreshold/@enabled", parCloudCoverEnabled, true);
			if ( parCloudCoverEnabled ) {
				xmlDocSptr->getValue("//CloudCoverageThreshold/text()", parCloudCovThreshold, true);
			}
		} catch (exception& ex) {
			ACS_THROW(
					InvPolicySelector::exInvPolicySelectorException(ex, "Error in Parameter file for policy Id ["+polId.str()+"]: Check CloudCoverageThreshold section configuration."));
		}
		//Init select statements in the base class according to read configuration.
		if ( parCloudCoverEnabled ) {
			_sqlSelections.push_back( this->getCloudCoverQuery( parCloudCovThreshold, conn ) );
		}

		//Read Expert condition
		bool parExpertEnabled(false);
		string parExpertWhereStatement("");
		try {
			xmlDocSptr->getValue("//Expert/@enabled", parExpertEnabled, true);
			if (parExpertEnabled) {
				xmlDocSptr->getValue("//Expert/SQL_where_statement/text()", parExpertWhereStatement, true);
			}
		} catch (exception& ex) {
			ACS_THROW(
					InvPolicySelector::exInvPolicySelectorException(ex, "Error in Parameter file for policy Id ["+polId.str()+"]: Check Expert section configuration."));
		}
		if ( parExpertEnabled ) {
			_sqlSelections.push_back( this->getExpertQuery( parExpertWhereStatement, conn ) );
		}


		//Read configured Time Threshold conditions
		vector<XmlDocSptr> timeThresholds = xmlDocSptr->xPath("//TimeThreshold");
		for ( vector<XmlDocSptr>::iterator it = timeThresholds.begin(); it!=timeThresholds.end(); ++it ) { // PRQA S 4238

			bool parTimeThresholdEnabled(false);
			string parTimeThreshold("");
			string parTimeEvent("");
			try {
				(*it)->getValue("/TimeThreshold/@enabled", parTimeThresholdEnabled, true);

				if ( parTimeThresholdEnabled ) {
					(*it)->getValue("/TimeThreshold/text()", parTimeThreshold, true);
					(*it)->getValue("//TimeThreshold/@event", parTimeEvent, true);
					boost::to_lower( parTimeEvent );
				}

			} catch (exception& ex) {
				ACS_THROW( // PRQA S 3081
						InvPolicySelector::exInvPolicySelectorException(ex, "Error in Parameter file for policy Id ["+polId.str()+"]: Check TimeThreshold section configuration."));
			}

			//Init select statements in the base class according to read configuration.
			if ( parTimeThresholdEnabled ) {
				_sqlSelections.push_back( this->getTimeThresholdQuery( parTimeEvent, parTimeThreshold, conn ) );
			}
		}

		if ( _sqlSelections.empty() ) {
			ACS_LOG_PRIVINFO(" No conditions configured. Filter on ["<<_polFileType<<"] Inventory items will be ignored.");
		}

		// -

	} catch(exception& ex) {
		ACS_THROW(InvPolicySelector::exInvPolicySelectorException(ex, "Error in processing policy Id ["+polId.str()+"]" )); // PRQA S 3081
	}
}


_ACS_END_NAMESPACE
