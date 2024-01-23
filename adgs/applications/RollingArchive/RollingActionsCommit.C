/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. RollingArchive$
        
*/ 

#include <RollingActionsCommit.h>
#include <RollingActionsCommitWarnings.h>
#include <RollingPoliciesActions.h>
#include <RollingStopPredicate.h>

#include <InvPolicyPerformerFactory.h>

#include <Application.h>
#include <StopController.h>
#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingActionsCommit)



RollingActionsCommit::RollingActionsCommit():
_severityLevel()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// Get the default connection Pool
	string dbKey = Application::instance()->getDefaultConnectionPool()->getKey();

	RollingPoliciesActions*  actions = RollingPoliciesActions::instance( dbKey );

	const bool caseInsensitive(true);
	_severityLevel[ actions->id("KEEP",                            caseInsensitive) ] = 0;
	_severityLevel[ actions->id("CLEANSTORAGE",                    caseInsensitive) ] = 10;
	_severityLevel[ actions->id("INVALIDATE",                      caseInsensitive) ] = 20;
	_severityLevel[ actions->id("INVALIDATEPARENT-REMOVECHILDREN", caseInsensitive) ] = 30;
	_severityLevel[ actions->id("AUTOPARENT-REMOVECHILDREN",       caseInsensitive) ] = 40;
	_severityLevel[ actions->id("REMOVE",                          caseInsensitive) ] = 50;
	_severityLevel[ actions->id("REMOVEPARENT-REMOVECHILDREN",     caseInsensitive) ] = 60;

}


namespace {

void diff_set (std::set<RollingAction> & a, std::set<RollingAction> & b ) {

	std::set<RollingAction> c;
	std::set_difference(a.begin(), a.end(),
	                    b.begin(), b.end(),
	                    std::inserter(c, c.begin()));
	a.swap(c);
}

}

void RollingActionsCommit::resolveConflicts( RollingPolicyOutput::Sptr & rollingPolicyOutput_A, RollingPolicyOutput::Sptr & rollingPolicyOutput_B ) {

	string dbKey = Application::instance()->getDefaultConnectionPool()->getKey();
	RollingPoliciesActions*  actionsTable = RollingPoliciesActions::instance( dbKey );

	std::set<RollingAction>& actions_A = rollingPolicyOutput_A->getListRollingActions();
	std::set<RollingAction>& actions_B = rollingPolicyOutput_B->getListRollingActions();

	std::set<RollingAction> actions_removed_A;
	std::set<RollingAction> actions_removed_B;

	//For each action of policy A
	for ( auto it = actions_A.begin(); it != actions_A.end(); ++it ) {
		const RollingAction & action_A = *it;

		//Cannot use actions_B.find(action_A), because I'm interested in same item id, not same complete action structure.
		bool stopScan = false;
		for ( auto jt = actions_B.begin(); not stopScan && jt != actions_B.end(); ++jt ) {
			const RollingAction & action_B = *jt;

			//because RollingAction 'less than' operator, set of actions is order by inventory ids
			stopScan = action_A.getObject() < action_B.getObject();

			//Two actions from different rolling policy on the same item
			const bool conflictItem = ( action_A.getObject() == action_B.getObject() );
			if ( conflictItem ) {

				string actionA = actionsTable->name( action_A.getRollingAction() );
				string actionB = actionsTable->name( action_B.getRollingAction() );

				ostringstream msgConflict;
				if ( "KEEP" == actionA || "KEEP" == actionB ) { msgConflict<< "Action KEEP detected"; }
				else                                          { msgConflict<< "Action conflict detected"; }

				ACS_LOG_WARNING( msgConflict.str() << " on inventory item id ["<<action_A.getObject()<<"]. "
						"Action ["<<actionA<<"] from policy id ["<<rollingPolicyOutput_A->getPolicyId()<<"] Vs "
						"Action ["<<actionB<<"] from policy id ["<<rollingPolicyOutput_B->getPolicyId()<<"]."
						);

				if (  _severityLevel[ action_A.getRollingAction() ] <= _severityLevel[ action_B.getRollingAction() ]  ) {
					actions_removed_B.insert( action_B );
					rollingPolicyOutput_B->getListRollingActionsSkipped().insert( action_B );
					ACS_LOG_WARNING( "SKIP Action ["<<actionB<<"] from policy id ["<<rollingPolicyOutput_B->getPolicyId()<<"]." );
				} else {
					actions_removed_A.insert( action_A );
					rollingPolicyOutput_A->getListRollingActionsSkipped().insert( action_A );
					ACS_LOG_WARNING( "SKIP Action ["<<actionA<<"] from policy id ["<<rollingPolicyOutput_A->getPolicyId()<<"]." );
				}
			}
		}
	}

	diff_set( actions_A,  actions_removed_A );
	diff_set( actions_B,  actions_removed_B );


}

void RollingActionsCommit::execute(	std::deque< RollingPolicyOutput::Sptr >& actionsQueue, RollingPolicyOutput::Sptr & actionsSpecialKeepSptr,
									RollingReport& rollingReport,
									RollingPolicyManager::Sptr & rollingPolicyManagerSptr ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( actionsQueue.empty() ) {
		return;
	}

	//Filter out Keep actions

	//For each policy's output
	for ( auto it = actionsQueue.begin(); it!=actionsQueue.end(); ++it ) {

		//A set of rolling actions from a single rolling policy
		RollingPolicyOutput::Sptr & rollingPolicyOutputSptr = *it;

		this->resolveConflicts( rollingPolicyOutputSptr, actionsSpecialKeepSptr );
	}

	//Resolve conflicts among policies' outputs
	//For each policy's output
	for ( auto it = actionsQueue.begin(); it!=actionsQueue.end(); ++it ) {
		//A set of rolling actions from a single rolling policy
		RollingPolicyOutput::Sptr & rollingPolicyOutput_A = *it;

		for ( auto jt = it+1; jt != actionsQueue.end(); ++jt ) {
			RollingPolicyOutput::Sptr & rollingPolicyOutput_B = *jt;

			this->resolveConflicts( rollingPolicyOutput_A, rollingPolicyOutput_B );
		}
	}

	//Log configured warnings on items going to be rolled
	try {
		RollingActionsCommitWarnings commitWarnings;
		commitWarnings.logWarnings( actionsQueue );
	} catch(exception& ex) {
		ACS_LOG_NOTIFY_EX(ex);
	}

	//ROUND ROBIN IMPLEMENTATION: In turn, for each policy's output, perform only an action

	//Apply actions in Round Robin strategy (one action for each policy in turn)

	//Init round robin structures
	map< RollingPolicyOutput::Sptr *, set<RollingAction>::iterator > policiesToPerform;
	for ( auto it = actionsQueue.begin(); it!=actionsQueue.end(); ++it ) {
		//A set of rolling actions from a single rolling policy
		RollingPolicyOutput::Sptr & rollingPolicyOutput = *it;

		policiesToPerform[ & rollingPolicyOutput ] = rollingPolicyOutput->getListRollingActions().begin();
	}

	//Do round robin: for each policy perform an action
	ACS_LOG_PRIVINFO("Apply actions requested from "<<policiesToPerform.size()<<" processed policies. Round robin order.");

	// Get the default connection Pool
	dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
	RollingPoliciesActions*  actions = RollingPoliciesActions::instance( cp->getKey() );

	RollingStopPredicate rollingStopPredicate;

	while ( not policiesToPerform.empty() ) {

		vector< RollingPolicyOutput::Sptr * > policiesToRemoveFromRoundRobin;
		vector< RollingPolicyOutput::Sptr   > listOfConsumedOutputs;

		for ( auto it = policiesToPerform.begin(); it != policiesToPerform.end(); ++it ) {
			RollingPolicyOutput::Sptr    & currentRollingPolicyOutputSptr = *(it->first);
			set<RollingAction>::iterator & currentPolicyActionIt = it->second;

			const bool allActionsPerformedOnCurrentPolicy = ( currentPolicyActionIt == currentRollingPolicyOutputSptr->getListRollingActions().end() );
			if ( allActionsPerformedOnCurrentPolicy ) {
				ACS_LOG_PRIVINFO("Applied all actions requested by Policy id ["<<currentRollingPolicyOutputSptr->getPolicyId()<<"].");

				policiesToRemoveFromRoundRobin . push_back( & currentRollingPolicyOutputSptr );
				listOfConsumedOutputs          . push_back(   currentRollingPolicyOutputSptr );

				//Once a Policy output has been processed, notify its producer that the policy status can be updated and a new output can be created.
				rollingPolicyManagerSptr->notifyConsumptions( listOfConsumedOutputs );

				listOfConsumedOutputs.clear();

			} else {

				string special = currentRollingPolicyOutputSptr->isSpecialPolicy() ? "SPECIAL ":"";
				ACS_LOG_PRIVINFO("Action queued: "<<special<<"Policy id: ["<<currentRollingPolicyOutputSptr->getPolicyId()
						<<"]; Inventory item id: ["<<currentPolicyActionIt->getObject()<<"]; Action: ["<<actions->name( currentPolicyActionIt->getRollingAction() )
						<<"]; Storage: ["<< currentPolicyActionIt->getStorage()<<"]" );

				//Perform the action for current policy
				dbConnectionWrapper pw(*cp); dbConnection &conn = pw ;

				const RollingAction& currentAction = *currentPolicyActionIt;
				bool errorOccurred(false);
				InvPolicyPerformerFactory::instance()->performAction( currentAction, errorOccurred, rollingReport, rollingStopPredicate, conn );

				if ( errorOccurred ) {
					set<RollingAction> & actionsSkipped = currentRollingPolicyOutputSptr->getListRollingActionsSkipped();
					actionsSkipped.insert( currentAction );
				}

				//Point to the next action of the policy just processed
				++currentPolicyActionIt;
			}
		}

		//Remove performed policies
		for ( auto it = policiesToRemoveFromRoundRobin.begin(); it!=policiesToRemoveFromRoundRobin.end(); ++it ) {
			RollingPolicyOutput::Sptr & rollingPolicyOutputSptr = *(*it);
			policiesToPerform.erase( & rollingPolicyOutputSptr );
		}

		policiesToRemoveFromRoundRobin.clear();

	}

}



_ACS_END_NAMESPACE

