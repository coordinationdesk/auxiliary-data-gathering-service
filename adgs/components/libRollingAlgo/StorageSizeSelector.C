/*
	Copyright 1995-2021, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libRollingAlgo$
        
*/ 

#include <StorageSizeSelector.h>
#include <RollingAlgoUtils.h>
#include <RollingPoliciesActions.h>
#include <dbSmartPersistent.h>
#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(StorageSizeSelector)



StorageSizeSelector::StorageSizeSelector(const InvPolicy & policy) :
InvPolicyCommonSelector(policy)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Get the configured size from

}

StorageSizeSelector::~StorageSizeSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void StorageSizeSelector::selectInit( dbConnection &conn ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//The items' preservation feature is handled directly by this specialization.
	// Disable it in the base class.
	// This because the delete plan must skip a priori the items to be preserved.
	_preserveItems = false;

	_sqlSelections.clear();

	if ( 0 == _polStorage ) {
		ACS_LOG_INFO("Policy id ["<<_polId<<"] Filetype [N/A] SKIPPED: Mandatory field Storage id has not been set in the rolling policy" );

	} else {
		//Storage id is set in the rolling policy
		//Get the size threshold for this storage manager
		uint64_t sizeLimitMb(0);
		bool  sizeLimitIsNull(true);
		dbSmartPersistent dbEntry( "t_storagerollingthresholds", conn );
		if ( dbEntry.load(conn, _polStorage) ) {
			dbEntry.get("threshold_mb", sizeLimitMb, sizeLimitIsNull, conn);
		}

		if ( sizeLimitIsNull ) {
			ACS_LOG_INFO("Policy id ["<<_polId<<"] Filetype [N/A] SKIPPED: Cannot find a configured size limit for Storage Id ["<<_polStorage<<"]. Please check values in table t_storagerollingthresholds." );
		} else {
			//Storage id and its size limit are set.
			const string & sqlCommand = this->getQuery( conn, sizeLimitMb*1024*1024 );

			if ( not sqlCommand.empty() ) {
				_sqlSelections.push_back( sqlCommand );
			}
		}
	}

}

namespace {
inline string getSizeMsg( float bytes ) {

	float gb = bytes / 1024.0 / 1024.0 / 1024.0;
	float mb = bytes / 1024.0 / 1024.0;

	ostringstream msg;
	if ( gb > 1 ) {
		msg << gb << " GB";
	} else {
		msg << mb << " MB";
	}
	return msg.str();
}
}

string StorageSizeSelector::getQuery (dbConnection &conn, uint64_t thresholdBytes) {

	ostringstream sqlToRoll;

	try {
        const string &dbKey = conn.getKey() ;
		RollingPoliciesActions*  actions = RollingPoliciesActions::instance(dbKey);

		ACS_LOG_INFO("Policy id ["<<_polId<<"] Filetype [N/A]: Checking Size of Storage id [" << _polStorage
				<<"]. Threshold set to [" << getSizeMsg(thresholdBytes) << "]. Action set to [" << actions->name(_polAction)<<"]" );

		//Compute used size
		uint64_t usedsizeBytes=0;
		{

			ostringstream ossSql;
			ossSql <<
					"select CAST ( SUM(i.datasize) as BIGINT ) "
					"from "
					"t_inventory as i, t_inventorysm as s WHERE "
					"i.id_inv = s.inv_id and "
					"s.storagemanid = "<<db::toSQL(conn, _polStorage);

			SQLString sqlStringCommand(dbKey, ossSql.str());
			sqlStringCommand.setType( SQLString::COMMAND );

			dbQuery sumQuery(conn, sqlStringCommand, "StorageSizeSelector") ;
	        dbSet set ;
	        sumQuery.execForSet(set) ;
	        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081

	        dbRow rowsum(dbKey);
	        set.getNextRow(rowsum) ; // first and only

			rowsum.column(0,usedsizeBytes);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,
					"Used size " << usedsizeBytes <<  "\n"
					<< "Query " <<ossSql.str()
			);
		}

		ACS_LOG_INFO("Policy id ["<<_polId<<"] Filetype [N/A]: Used Size of Storage id [" << _polStorage
				<<"] is ["<< getSizeMsg(usedsizeBytes) <<"]. Threshold set to ["<< getSizeMsg(thresholdBytes) <<"]." );

		if (usedsizeBytes > thresholdBytes) {

			//SAME CRITERIA USED IN AsynchSmRestoredSizeSelector. Refer that class to get detailed explanation.

			sqlToRoll <<
			"select distinct                                                                                             \n"
			"   case when deletePlan.parent_inv_id is NULL then deletePlan.id_inv                                        \n"
			"   else deletePlan.parent_inv_id                                                                            \n"
			"   end as "<< _invIdAttributeName <<"                                                                       \n"
            "                                                                                                            \n"
			"from (                                                                                                      \n"
            "                                                                                                            \n"
			"	select                                                                                                   \n"
			"	inv.inventorydate, inv.id_inv, invRel.inv_id2 as parent_inv_id, inv.datasize,                            \n"
            "                                                                                                            \n"
			"	sum(inv.datasize) OVER (ORDER BY inv.inventorydate ASC ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) \n"
			"      as usedStoreSize                                                                                      \n"
            "                                                                                                            \n"
			"	from                                                                                                     \n"
			"	t_inventory as inv                                                                                       \n"
			"	inner join t_inventorysm        as invSm      on inv.id_inv = invSm.inv_id                               \n"
			"	left  join t_inv_relations      as invRel     on inv.id_inv = invRel.inv_id1                             \n"
			"	left  join t_inv_relation_types as invRelType on invRel.rel_type_id = invRelType.id                      \n";

			//"items preservation" PART HERE (and do not do this in base class)
			string preserveInvIdAttName;
			const auto& pairs_QueryName_SqlQuery = RollingAlgoUtils::instance()->getInventoryItemsToBePreservedQueries(preserveInvIdAttName);

			for ( const auto & pair_QueryName_SqlQuery: pairs_QueryName_SqlQuery ) {
				const string& preserveQueryName = pair_QueryName_SqlQuery.first;
				const string& preserveQuerySql  = pair_QueryName_SqlQuery.second;

				sqlToRoll<< "LEFT JOIN (\n";
				sqlToRoll<< preserveQuerySql;
				sqlToRoll<< ") as "<<preserveQueryName<<" ON inv.id_inv = "<<preserveQueryName<<"."<<preserveInvIdAttName<<"\n";
			}

			sqlToRoll <<
            "                                                                                                            \n"
			"	where                                                                                                    \n"
			"	(invRelType.relation_type is NULL OR invRelType.relation_type = 'CHILD_TO_PARENT' ) and                  \n"
			"	invSm.storagemanid = " <<db::toSQL(conn, _polStorage)<< "                                                \n"
            "                                                                                                            \n";

			//"items preservation" PART HERE (and do not do this in base class)
			for ( const auto & pair_QueryName_SqlQuery: pairs_QueryName_SqlQuery ) {
				const string& preserveQueryName = pair_QueryName_SqlQuery.first;

				sqlToRoll<<" AND "<<preserveQueryName<<"."<<preserveInvIdAttName<<" is Null ";
			}

			sqlToRoll <<
			") as deletePlan                                                                                             \n"
            "                                                                                                            \n"
			"where ( "<<db::toSQL(conn, usedsizeBytes)<<" - deletePlan.usedStoreSize) >= "<<db::toSQL(conn, thresholdBytes);

		}

	} catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e);
	}

	return sqlToRoll.str();
}



_ACS_END_NAMESPACE

