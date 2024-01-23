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

#include <InvPolicyCommonSelector.h>

#include <Filterables.h>

#include <RollingAlgoUtils.h>

#include <InvPolicyPerformerFactory.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(InvPolicyCommonSelector)


InvPolicyCommonSelector::InvPolicyCommonSelector(const InvPolicy & policy):
				InvPolicySelector(policy),
				_polId               (),
				_polAction           (),
				_polStorage          (),
				_polFileType         (),
				_polFileClass        (),
				_polFetchRowsLimit   (),
				_polIdleNumItems     (),
				_polParameters       (),
				_polStatus           (),
				_invIdAttributeName  ("id_inv"),
				_operatorSelections  ("and"),
				_sqlSelections       (),
				_preserveItems       (true)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	policy.get_id            ( _polId            );
	policy.get_action        ( _polAction        );
	policy.get_storageid     ( _polStorage       );
	policy.get_filetype      ( _polFileType      );
	policy.get_fileclass     ( _polFileClass     );
	policy.get_fetchrowslimit( _polFetchRowsLimit);
	policy.get_idlenumitems  ( _polIdleNumItems  );

	policy.get_parameters( _polParameters );
	policy.get_status    ( _polStatus     );

	//Print debug
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, endl<<
			"_polId             : "<< _polId             <<endl<<
			"_polAction         : "<< _polAction         <<endl<<
			"_polStorage        : "<< _polStorage        <<endl<<
			"_polFileType       : "<< _polFileType       <<endl<<
			"_polFileClass      : "<< _polFileClass      <<endl<<
			"_polParameters     : "<<  (_polParameters.empty()?"NO":"YES")<<endl<<
			"_polFetchrowslimit : "<< _polFetchRowsLimit <<endl<<
			"_polIdlenumitems   : "<< _polIdleNumItems   <<endl<<
			"_operatorSelections: "<< _operatorSelections<<endl
	);

}

InvPolicyCommonSelector::~InvPolicyCommonSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


void InvPolicyCommonSelector::getSessionResult( std::string& newPolicyStatus, InvPolicySelector::SessionInfo& sessionInfo ) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	newPolicyStatus = _polStatus;
	sessionInfo = InvPolicySelector::SessionInfo(false, 0, 0);
}


void InvPolicyCommonSelector::getSessionResult( const dbSet& queryResult, const size_t & distinctCounter,
		std::string& newPolicyStatus, InvPolicySelector::SessionInfo& sessionInfo ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool sessionCompleted(false);

	//Note: in InvPolicy constructor fields are set coherently, fixing potential misconfigurations.
	// Selector session is completed when retrieved items are less than the configured threshold.

	sessionCompleted = ( 0 == distinctCounter ) ||
			(
				(
					InvPolicy::Unlimited_fetchrowslimit == _polFetchRowsLimit
					||
					static_cast<int64_t>(queryResult.size()) < _polFetchRowsLimit
				)
				&&
				static_cast<int64_t>(distinctCounter) < _polIdleNumItems
			);

	bool partialResult = not sessionCompleted;
	sessionInfo = InvPolicySelector::SessionInfo(partialResult, queryResult.size(), distinctCounter);

	newPolicyStatus = ""; //By default. Specific selector can override this method.
}



void InvPolicyCommonSelector::selectRun( dbConnection & conn,
		InvPolicySelector::SessionInfo& sessionInfo,
		std::string& newPolicyStatus,
		std::set<RollingAction> & listOfActions,
		ConfigurationSingleton::StopPredicate & stop_pred ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( stop_pred.eval() ) {
		ACS_LOG_INFO( SimpleDebugSignature << "Skipping since stop requested."  );
		this->getSessionResult( newPolicyStatus, sessionInfo );
		return;
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		std::ostringstream sqlSelectionsOss;
		std::copy(_sqlSelections.begin(), _sqlSelections.end(), std::ostream_iterator<string>(sqlSelectionsOss, "\n- - - - -\n"));
		ACS_LOG_DEBUG("_sqlSelections: "<< sqlSelectionsOss.str());
	ACS_CLASS_END_DEBUG

	if ( _sqlSelections.empty() ) {
		ACS_LOG_INFO("Policy id ["<<_polId<<"]: Returned 0 items" );
		this->getSessionResult( newPolicyStatus, sessionInfo );

		return;
	}

	const string dialectName = SQLDialectFactory::dialectName( conn.getKey() );

	//Prepare the filtering query

	//NOTE: Oracle11 TO BE IMPLEMENTED. In current implementation configuration _confFetchRowsLimit is ignored in case of Oracle11 (to be implemented!)
	// E.g. Postgress and Oracle12 are ok because they support the standard SQL 'fetch first n row'.
	string limitClause="";
	if ( (_polFetchRowsLimit > 0) && ("Oracle11" != dialectName) ) {
		ostringstream oss;
		oss << "fetch first "<< _polFetchRowsLimit <<" rows only";
		limitClause = oss.str();
	}

	ostringstream ossSql;
	ossSql <<
			"select invToRoll."<<_invIdAttributeName<<"\n"
			"FROM ( \n";

	//Perform all the configured selections
	for ( vector<string>::const_iterator selIt=_sqlSelections.begin(); selIt!=_sqlSelections.end(); ++selIt ) {
		//Build SQL query

		if ( selIt != _sqlSelections.begin() ) {
			ossSql << "\n" << (_operatorSelections == "and" ? "INTERSECT" : "UNION") << "\n"; // PRQA S 3380
		}

		ossSql << " ( " << *selIt <<" ) ";
	}

	ossSql<<" ) as invToRoll \n";

	//Except Inventory Items to be preserved
	if ( _preserveItems ) {

		string preserveInvIdAttName;
		const auto& pairs_QueryName_SqlQuery = RollingAlgoUtils::instance()->getInventoryItemsToBePreservedQueries(preserveInvIdAttName);

		for ( const auto & pair_QueryName_SqlQuery : pairs_QueryName_SqlQuery ) {
			const string& preserveQueryName = pair_QueryName_SqlQuery.first;
			const string& preserveQuerySql  = pair_QueryName_SqlQuery.second;

			ossSql<< "LEFT JOIN (\n";
			ossSql<< preserveQuerySql;
			ossSql<< ") as "<<preserveQueryName<<"\n";
			ossSql<< "ON invToRoll."<<_invIdAttributeName<<" = "<<preserveQueryName<<"."<<preserveInvIdAttName<<"\n";
		}

		for ( vector<RollingAlgoUtils::Pair_QueryName_SqlQuery>::const_iterator it = pairs_QueryName_SqlQuery.begin(); it != pairs_QueryName_SqlQuery.end(); ++it ) {
			const string& preserveQueryName = it->first;

			string prefix = ( it == pairs_QueryName_SqlQuery.begin() ) ? "WHERE" : "AND"; // PRQA S 3384

			ossSql<<" "<<prefix<<" "<<preserveQueryName<<"."<<preserveInvIdAttName<<" is Null ";
		}
	}

	//If configured, fetch only N element to be rolled, that are not involved in on-going operations
	ossSql<<"\n"<<limitClause<<"\n";

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<< ossSql.str() );

	SQLString sqlString( conn.getKey(), ossSql.str() );
	dbSet res; dbQuery query( conn, sqlString, "InvPolicyCommonSelector" );
	query.execForSet( res );

	const size_t totCounter = res.size();
	{
		ostringstream msgLmt;
		msgLmt << "fetching threshold set to [";
		if ( InvPolicy::Unlimited_fetchrowslimit == _polFetchRowsLimit ) { msgLmt << "Unlimited"; } else {msgLmt << _polFetchRowsLimit;}
		msgLmt << "]";

		ostringstream msgIdl; msgIdl << "policy idle limit set to [";
		if ( InvPolicy::Unlimited_idlenumitems == _polIdleNumItems   ) { msgIdl << "Unlimited"; } else { msgIdl << _polIdleNumItems;}
		msgIdl << "]";

		ostringstream msgPrv; if ( totCounter > 0 ) { msgPrv << " Returned items are not in the List of items to be preserved."; }

		ACS_LOG_INFO("Policy id ["<<_polId<<"]: Returned " << totCounter << " items ("<<msgLmt.str()<<"; "<<msgIdl.str()<<")." << msgPrv.str() );
	}

	std::set<dbPersistent::IdType> distinctIds;

	dbRow row(conn.getKey());
	while ( res.getNextRow(row) ) {
		dbPersistent::IdType invId = 0 ;
		bool isNull = false ;
		row.column(0, invId, &isNull);
		if (!isNull) {

			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Inventory Id from current policy: "<< invId );
			distinctIds.insert( invId );

			RollingAction rollingAction( invId, _polAction, _polStorage );
			InvPolicyPerformerFactory::populateActionToPerfom(conn, rollingAction, listOfActions);
		}
	}

	size_t distinctCounter = distinctIds.size();
	if (  distinctCounter != totCounter ) {
		ACS_LOG_INFO("Policy id ["<<_polId<<"]: Returned " << distinctCounter << " distinct items over a total of "<<totCounter<<" items" );
	}

	//Each policy can format the policy status (that will be stored in the policy tuple).
	res.rewind();
	this->getSessionResult( res, distinctCounter, newPolicyStatus, sessionInfo );
	return;
}


_ACS_END_NAMESPACE

