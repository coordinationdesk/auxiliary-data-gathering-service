/*
	Copyright 1995-2021, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. RollingArchive$
        
*/ 

#include <RollingActionsCommitWarnings.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <Transaction.h>
#include <dbPersistent.h>

#include <ConfigurationSingleton.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingActionsCommitWarnings)

RollingActionsCommitWarnings::ConfWarning RollingActionsCommitWarnings::_confWarning;

RollingActionsCommitWarnings::RollingActionsCommitWarnings()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RollingActionsCommitWarnings::~RollingActionsCommitWarnings() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void RollingActionsCommitWarnings::logWarnings(std::deque< RollingPolicyOutput::Sptr >& actionsQueue ) {
	try {

		//Load warnings from configuration
		rsResourceSet & conf =  ConfigurationSingleton::instance()->get();

		bool warnEnabled(false);
		bool warnMigration(false);
		bool warnExport(false);

		if ( conf.hasKey("RollingArchive.Warnings") ) {

			conf.getValue( "RollingArchive.Warnings.enabled.value", warnEnabled, false );
			if ( warnEnabled ) {
				ostringstream baseKey; baseKey<<"RollingArchive.Warnings.warning";
				size_t count = conf.getArraySize( baseKey.str() );
				for ( size_t i(0); i<count; ++i ) {
					ostringstream keyEnabled; keyEnabled<< baseKey.str() <<"["<<i<<"].enabled.value";
					ostringstream keyValue  ; keyValue  << baseKey.str() <<"["<<i<<"].event.value";

					bool enabled(false);
					string value;
					conf.getValue( keyEnabled.str(), enabled, false);
					conf.getValue( keyValue.str()  , value  , false);

					if ( not warnMigration ) {	warnMigration = ( enabled && "migration_error" == value ); }
					if ( not warnExport    ) {	warnExport    = ( enabled && "export_error"    == value ); }
				}
			}

			//Log configuration if change detected
			ConfWarning newConfWarning( warnEnabled, warnMigration, warnExport );
			if ( not ( _confWarning == newConfWarning ) ) {
				_confWarning = newConfWarning;
				ostringstream msg; msg<<"Configuration settings for warning messages: enabled ["<<boolalpha<<warnEnabled<<"]; "
				                        "migration ["<<warnMigration<<"]; exporting ["<<warnExport<<"]";
				ACS_LOG_INFO( msg.str() );
			}

			//Apply warnings
			if ( _confWarning.enabled && ( _confWarning.migration || _confWarning.exporting ) ) {

				//Create a temporary table with inventory items going to be rolled
			    dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
			    dbConnection& conn = pw;
			    const string & connKey = conn.getKey();

			    { // Transaction scope

					db::Transaction transaction(conn);
					transaction.begin_work("RollingArchiveCommitChecks");

					// Create a temporary table containing item ids going to be rolled
					// Note: here the size of actionsQueue is meant to be not huge and workable in terms of
					//       available resources. This because the Rolling Framework.
					//       If not the case, consider tune better the Rolling in configuration (fetch row limit per policy).

					vector<dbPersistent::IdType> v ;

					for ( auto rollingPolicyOutputSptr : actionsQueue ) {
						auto & listRollingActions = rollingPolicyOutputSptr->getListRollingActions();
						for ( auto rollingAction : listRollingActions ) {
							v.push_back( rollingAction.getObject() );
						}
					}

					dbTemporaryTable ttable(conn, true); /* Create temporary table (will be dropped leaving the scope) */
					ttable.setColumnName("inventory_id");
					string tempTable_Rolling_check = ttable.createTable(v);

					if ( _confWarning.migration ) {

						ostringstream ossSql;
						ossSql <<
						"select inv.id_inv, inv.filename, inv.fileversion \n"
						"from   "<<tempTable_Rolling_check<<" invIdToRoll \n"
						"inner join t_inventory inv   on inv.id_inv    = invIdToRoll.inventory_id \n"
						"inner join t_smactions smAct on smAct.inv_id  = invIdToRoll.inventory_id \n"
						"inner join t_smqueue   smQ   on smQ.packageid = smAct.packageid \n"
						"where smQ.status = "<< db::toSQL(connKey, "ERROR")<<" \n";

						this->queryAndWarning(conn, ossSql, "Rolling an inventory item that was not migrated. Migration error previously occurred.");

					}
					if ( _confWarning.exporting ) {

						ostringstream ossSql;
						ossSql <<
						"select inv.id_inv, inv.filename, inv.fileversion \n"
						"from   "<<tempTable_Rolling_check<<" invIdToRoll \n"
						"inner join t_inventory         inv   on inv.id_inv = invIdToRoll.inventory_id \n"
						"inner join t_distributionitems disIt on (inv.filename = disIt.filename AND inv.fileversion = disIt.fileversion) \n"
						"inner join t_distributionqueue disQ  on disIt.packageid = disQ.packageid \n"
						"where disQ.status = "<< db::toSQL(connKey, "ERROR")<<" \n";

						this->queryAndWarning(conn, ossSql, "Rolling an inventory item that was not exported. Exporting error previously occurred.");

					}

					transaction.commit();
				}


			}

		}

	} catch( exception& e ) {
		ACS_LOG_NOTIFY_EX(e);
	}
}

void RollingActionsCommitWarnings::queryAndWarning( dbConnection& conn, const ostringstream& ossSql, const string& message ) {


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": Query Rolling warning items ["<<ossSql.str()<<"]");

	try {
		SQLString sqlString(conn.getKey(), ossSql.str() );
		dbQuery query(conn, sqlString, "queryAndWarning");

		dbSet set; dbRow row(conn.getKey());
		query.execForSet( set );

		if ( set.size() > 0 ) {
			ostringstream msg;
			msg<<message<<" Number of items: ["<<set.size()<<"].";
			ACS_LOG_WARNING( msg.str() );
		}

		while ( set.getNextRow(row) ) {
			int f(0);
			dbPersistent::IdType invId = 0 ;
			string invName;
			string invVersion;
			row.column(f++, invId);
			row.column(f++, invName);
			row.column(f++, invVersion);

			ostringstream msg;
			msg<<message<<" Inventory id ["<<invId<<"] name ["<<invName<<"] version ["<<invVersion<<"].";
			ACS_LOG_WARNING( msg.str() );
		}
	} catch( exception& ex ) {
		ACS_LOG_WARNING("Problem occurred getting Rolling inventory items warning list. Sql was ["<<ossSql.str()<<"]");
		throw;
	}



}




_ACS_END_NAMESPACE

