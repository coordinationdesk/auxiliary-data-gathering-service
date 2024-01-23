/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libRollingAlgo$

 */

#include <AsynchSmRestoredSizeSelector.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <XmlDoc.h>

#include <Filterables.h>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(AsynchSmRestoredSizeSelector)



AsynchSmRestoredSizeSelector::AsynchSmRestoredSizeSelector(const InvPolicy & policy):
InvPolicyCommonSelector(policy)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

AsynchSmRestoredSizeSelector::~AsynchSmRestoredSizeSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

namespace {

float gb( const uint64_t & byte ) {
	return byte/1073741824.0;
}

string formatStatus( size_t numItems ) {

	ostringstream val; val<<numItems;

	XmlDocSptr xmlDocSptr( new XmlDoc );
	xmlDocSptr->setParameters(XmlDoc::parDefault).initWithString("<status/>");
	xmlDocSptr->addTag("NumItemsPrevLoop", val.str() );

	return xmlDocSptr->str();
}

}

string AsynchSmRestoredSizeSelector::getRestoredSizeQuery(dbConnection &conn, uint64_t const& hiQuota, uint64_t const& lowQuota ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//

	/* ALGO SPECS:
	cf. http://jira.acsys.it/browse/S2PDGS-1967

 	Compute the total size of restored items (select sum(size) as size from t_inv_data_restored where sm_id = policySmId)

 	if total size <= hiQuota then
 		return empty set
 	else:

 	Compute the deletePlan:

	time;                       inv_id;  parent_inv_id; item size; totSize; size_after_delete;
	2018-04-24 14:09:39.81338   3756     3500           10         40       30
	2018-04-24 14:09:40.81338   3757     3500           10         40       20
	2018-04-24 14:09:41.81338   4000                    10         40       10
	2018-04-24 14:09:41.81338   4001                    10         40       00

	ordered by time, asc.

	Return items in deletePlan where size_after_delete >= lowQuota

	Returned items are, for each row:
	- inv_id if parent_inv_id is null
	- parent_inv_id if it is not null


	//Implementation hint: Compute the delete plan using the window functions.
	//https://www.postgresql.org/docs/9.1/static/tutorial-window.html
	//https://www.postgresql.org/docs/9.1/static/sql-expressions.html#SYNTAX-WINDOW-FUNCTIONS

	The above algorithm is implemented by the following black magic's spell:

	select distinct
	   case when deletePlan.parent_inv_id is NULL then deletePlan.inv_id
	   else deletePlan.parent_inv_id
	   end as inv_id

	from (

		with w_totSize as (select sum(size) as size from t_inv_data_restored)

		select
		invRes.tstamp, invRes.inv_id, invRes.parent_inv_id, invRes.size,
		w_totSize.size as totSize,

		sum( invRes.size) OVER (ORDER BY tstamp ASC ROWS BETWEEN UNBOUNDED PRECEDING AND 1 FOLLOWING) as usedStoreSize
		-- note: use "1 FOLLOWING" instead of "CURRENT ROW" to include one more item to delete (i.e. to go under the lowQuota size)

		from
		t_inv_data_restored as invRes,
		w_totSize

	) as deletePlan

	where (63 - deletePlan.usedStoreSize) >= 15

	 */

	//Compute the total size of restored items

	uint64_t totalSize(0);
	bool totalSizeIsNull(true);
	{
		//This query is sql standard - Cast operator is needed here to get right result!
		ostringstream ossSql;
		ossSql << "select CAST ( SUM(invRest.size) as BIGINT) as size_sm from t_inv_data_restored invRest where sm_id = "<<db::toSQL(conn, _polStorage);
		SQLString sqlString( conn.getKey(), ossSql.str() );


		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<< sqlString.getSQL() );
		dbSet res; dbQuery query( conn, sqlString, "AsynchSmRestoredSizeSelector");
		query.execForSet( res );

		dbRow row(conn.getKey());
		//If any, Returned only one row
		if ( res.getRow(1, row) ) {
			row.column(0, totalSize, &totalSizeIsNull);
		}
	}

	bool is_rolling_triggered_in_previous_loops(false);
	{
		//This rolling policy must check if in the previous main loop
		// the number of rolled items reached the configured threshold.
		//
		// For example:
		//  - In the main loop N, I reached the used storage hiQuota = 100 Gb and I planned to roll 200 items to get the used storage to lowQuota = 50 Gb
		//  - Because the configured thresholds, only 100 items have been rolled
		//  - In the main loop N+1, because the 100 rolled items, I reached the used storage hiQuota = 60 Gb ==> no items will be rolled
		//  - So, I rolled less items than the ones planned at main loop N.
		// To fix this misbehavior:
		//  - Check if the number of rolled items in previous main loop is > 0.
		//  - If so, force hiQuota = lowQuota in this main loop to force the rolling of items up to lowQuota.
		//  - If not, use HiQuota nominally so that the rolling policy will be triggered again at hiQuota level.

		size_t numItemsPrevMainLoop;
		if ( not _polStatus.empty() ) {
			XmlDocSptr xmlDocSptr( new XmlDoc );
			xmlDocSptr->setParameters(XmlDoc::parDefault).initWithString( _polStatus );
			if ( xmlDocSptr->getValue("//NumItemsPrevLoop/text()", numItemsPrevMainLoop, false) ) {
				is_rolling_triggered_in_previous_loops = _polFetchRowsLimit && ( numItemsPrevMainLoop > 0  );
			}
		}
	}
	const uint64_t & totalSizeLimit = is_rolling_triggered_in_previous_loops ? lowQuota : hiQuota;

	if ( totalSizeIsNull || totalSize <= totalSizeLimit ) {

		if ( not is_rolling_triggered_in_previous_loops ) {

			ACS_LOG_INFO("Policy id ["<<_polId<<"]. Filetype [N/A]. Restored items' size "<<setprecision(2)<<fixed<<gb(totalSize)<<" GB ("<<totalSize<<" bytes) "
					<<"LESS THAN configured threshold of "<<gb(hiQuota)<<" GB ("<<hiQuota<<" bytes) on Storage manager id ["<<_polStorage<<"]. "
					<<"Rolling on restored items not needed.");
		} else {
			ACS_LOG_INFO("Policy id ["<<_polId<<"]. Filetype [N/A]. Rolling loop completed for this policy: Restored items' size "<<setprecision(2)<<fixed<<gb(totalSize)<<" GB ("<<totalSize<<" bytes) "
					<<"LESS THAN configured threshold of "<<gb(lowQuota)<<" GB ("<<lowQuota<<" bytes) on Storage manager id ["<<_polStorage<<"]. "
					<<"Rolling policy will be triggered again at configured threshold of "<<gb(hiQuota)<<" GB ("<<hiQuota<<" bytes).");
		}

		return "";
	}

	//HERE IF THERE IS SOMETHING TO ROLL

	if ( not is_rolling_triggered_in_previous_loops ) {

		ACS_LOG_INFO("Policy id ["<<_polId<<"]. Filetype [N/A]. Restored items' size "<<setprecision(2)<<fixed<<gb(totalSize)<<" GB ("<<totalSize<<" bytes) "
				<<"EXCEEDS configured threshold of "<<gb(hiQuota)<<" GB ("<<hiQuota<<" bytes) - on Storage manager id ["<<_polStorage<<"]. "
				<<"Rolling restored items to get the used storage less than "<<gb(lowQuota)<<" GB  ("<<lowQuota<<" bytes)");
	} else {

		ACS_LOG_INFO("Policy id ["<<_polId<<"]. Filetype [N/A]. Rolling loop in progress for this policy: Restored items' size "<<setprecision(2)<<fixed<<gb(totalSize)<<" GB ("<<totalSize<<" bytes) "
				<<"on Storage manager id ["<<_polStorage<<"]. "
				<<"Rolling restored items to get the used storage less than "<<gb(lowQuota)<<" GB  ("<<lowQuota<<" bytes). "
				<<"Policy triggered on previous loops at used storage threshold "<<gb(hiQuota)<<" GB  ("<<hiQuota<<" bytes)");
	}

	ostringstream ossSql;
	{
		// Note: in OVER clause used "1 FOLLOWING" instead of "CURRENT ROW" to include one more item to delete (i.e. to go under the lowQuota size)

		ossSql<<
				"select distinct                                                              \
				   case when deletePlan.parent_inv_id is NULL then deletePlan.inv_id          \
				   else deletePlan.parent_inv_id                                              \
				   end as "<< _invIdAttributeName <<"                                         \
				                                                                              \
				from (                                                                        \
                                                                                              \
					select                                                                    \
					invRes.tstamp, invRes.inv_id, invRes.parent_inv_id, invRes.size,          \
                                                                                              \
					sum(invRes.size) OVER (ORDER BY tstamp ASC ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) as usedStoreSize \
                                                                                              \
					from                                                                      \
					t_inv_data_restored as invRes                                             \
				                                                                              \
					where 				                                                      \
				    invRes.sm_id = " <<db::toSQL(conn, _polStorage)<< "                       \
				) as deletePlan                                                               \
                                                                                              \
				where ("<<db::toSQL(conn, totalSize)<<" - deletePlan.usedStoreSize) >= "<<db::toSQL(conn, lowQuota);
	}


	return ossSql.str();
}

void AsynchSmRestoredSizeSelector::selectInit(dbConnection &conn) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream polId; polId<<_polId;

	try {
		// - Read Parameters File
		XmlDocSptr xmlDocSptr( new XmlDoc );
		xmlDocSptr->initWithString( _polParameters );

		uint64_t polParamHiQuota(0);
		xmlDocSptr->getValue("//HiQuota/text()", polParamHiQuota, true );

		uint64_t polParamLowQuota(0);
		xmlDocSptr->getValue("//LowQuota/text()", polParamLowQuota, true );

		string sqlQuery = this->getRestoredSizeQuery(conn, polParamHiQuota, polParamLowQuota);

		if ( sqlQuery.empty() ) {
			_sqlSelections.clear();
		} else {
			_sqlSelections.push_back( sqlQuery );
		}

	} catch(exception& ex) {
		ACS_THROW( InvPolicySelector::exInvPolicySelectorException(ex, "Error in processing policy Id ["+polId.str()+"]" )); // PRQA S 3081
	}

}


void AsynchSmRestoredSizeSelector::getSessionResult( const dbSet& queryResult, const size_t & distinctCounter,
		std::string& newPolicyStatus, InvPolicySelector::SessionInfo & sessionInfo ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	InvPolicyCommonSelector::getSessionResult( queryResult, distinctCounter, newPolicyStatus, sessionInfo );
	newPolicyStatus = formatStatus( queryResult.size() );
}

void AsynchSmRestoredSizeSelector::getSessionResult( std::string& newPolicyStatus, InvPolicySelector::SessionInfo & sessionInfo ) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	InvPolicyCommonSelector::getSessionResult( newPolicyStatus, sessionInfo );
}


_ACS_END_NAMESPACE

