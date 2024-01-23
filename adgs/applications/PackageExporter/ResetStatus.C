/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageExporter$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/05/29 17:53:43  nicvac
	OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
	
	Revision 1.2  2016/12/16 16:56:15  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.1  2016/12/13 16:39:57  nicvac
	OPMAN-199: Reset intermediate status improved.
	
        
*/ 

#include <ResetStatus.h>

#include <Filterables.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <Transaction.h>
#include <dbPersistent.h>

#include <Status.h>

#include <ConfigurationSingleton.h>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ResetStatus)



ResetStatus::ResetStatus(std::string const& jobResponsible, dbConnection& conn):
	_jobResponsible(jobResponsible),
	_conn(conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ResetStatus::~ResetStatus() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


void ResetStatus::fillPackageidTemporaryTable(string const& tempTableName, vector<string> const& statuses  ) const {

	const string ck( _conn.getKey() );

	string strInSqlValues("");
	for ( vector<string>::const_iterator it = statuses.begin(); it!=statuses.end(); ++it ) {
		strInSqlValues += (it != statuses.begin()) ? ", " : "";
		strInSqlValues += db::toSQL(ck, *it);
	}

	ostringstream ossSql;
	ossSql<<"INSERT INTO "<<tempTableName<<" ( \
			select packageid from t_distributionqueue where \
			 jobresponsible="<<db::toSQL(ck, _jobResponsible)<<" and \
			 status in ("<<strInSqlValues<<" ) );";

	SQLString sqlString(ck, ossSql.str() );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");

	dbQuery insert(_conn, sqlString, "INS " + tempTableName);
	ACS_COND_THROW( ! insert.execForStatus(),
			exResetStatusException("Problem occurred retrieving distribution queues' items in interim status. Sql was ["+sqlString.getSQL()+"]") );

}

size_t ResetStatus::logPackageidFromTemporaryTable(string const& tempTableName, string const& statusesStr ) const {

	const string &ck = _conn.getKey() ;

	ostringstream ossSql;
	ossSql<<"select packageid from "<<tempTableName;

	SQLString sqlString(ck, ossSql.str() );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");

	dbQuery select(_conn, sqlString, "SEL "+tempTableName);
	dbSet set;
	try { select.execForSet( set ); } catch(exception& ex) {
		ACS_THROW( exResetStatusException("Problem occurred retrieving distribution queues' items in ["+statusesStr+"] status. Sql was ["+sqlString.getSQL()+"]") );
	}

	ostringstream msg; msg<<"Package ids in Distribution queue found in ["+statusesStr+"] Status: [";
	dbRow row(ck);
	while ( set.getNextRow(row) ) {
		string packageid("");
		row.column(0, packageid);
		msg<<" "<<packageid<<" ";
	}
	msg<<"]";
	ACS_LOG_PRIVINFO(msg.str());

	return set.size();

}

void ResetStatus::setErrorPackageidFromTemporaryTable(string const& tempTableName ) const {

	//Update to Error the packages in temporary table

	const string ck( _conn.getKey() );

	DateTime curdate;
	ostringstream ossSql;
	ossSql<<"update t_distributionqueue \
			set status="<<db::toSQL(ck, acs::Status::error)<<", \
			lastattempt="<<db::toSQL(ck, curdate, dbConv::YearToFraction )<<" \
			where packageid in (select packageid from "<<tempTableName<<");";

	SQLString sqlString(ck, ossSql.str() );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");

	dbQuery update(_conn, sqlString, "SetDistError");
	ACS_COND_THROW( ! update.execForStatus(),
			exResetStatusException("Problem occurred updating distribution queues' items in interim status. Sql was ["+sqlString.getSQL()+"]") );

}

void ResetStatus::setEligiblePackageidFromTemporaryTable(string const& tempTableName ) const {

	const string ck( _conn.getKey() );

	DateTime curdate;
	ostringstream ossSql;
	ossSql<<"update t_distributionqueue \
			set status="<<db::toSQL(ck, acs::Status::eligible)<<" \
			where packageid in (select packageid from "<<tempTableName<<");";

	SQLString sqlString(ck, ossSql.str() );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");

	dbQuery update(_conn, sqlString, "SetDistEligible");
	ACS_COND_THROW( ! update.execForStatus(),
			exResetStatusException("Problem occurred uptading distribution queues' items in interim status. Sql was ["+sqlString.getSQL()+"]") );
}


void ResetStatus::resetDistributionQueueStatus() const {

	//NOTE: you can handle only Intermediate statuses: Preconditioncheck, Distribution and Downloading.
	// Other status (eg. Transient) is not used by PackageExporter ==> it's not under PackageExporter's control.

	const string nop("NOP");
	const string toError("TOERROR");
	const string toEligible("TOELIGIBLE");

	const string confKey("PackageExporter.InterimStatusReset.RecoveryAction");

	string recoveryAction( nop );

	try {

		rsResourceSet& conf = ConfigurationSingleton::instance()->get();

		conf.getValue(confKey, recoveryAction);

		vector<string> allowedValues;
		allowedValues.push_back(nop); allowedValues.push_back(toError); allowedValues.push_back(toEligible);

		if ( find( allowedValues.begin(), allowedValues.end(), recoveryAction ) == allowedValues.end() ) {

			std::ostringstream values;
			std::copy(allowedValues.begin(), allowedValues.end(), std::ostream_iterator<string>(values, " "));

			ACS_LOG_PRIVINFO("Configuration setting for "<<confKey<<" ["<<recoveryAction<<"] is not valid. Allowed values are [ "<<values.str()<<"]");

			recoveryAction = nop;

			ACS_LOG_PRIVINFO("Using default setting ["<<recoveryAction<<"] for configuration "<<confKey);
		}

	} catch ( exception& ex ) {
		ACS_LOG_PRIVINFO("Using default setting ["<<recoveryAction<<"] for configuration "<<confKey);
	}


	try {

		const string ck( _conn.getKey() );

		{ // Transaction scope

			// Preconditioncheck and Downloading status reset.
			//  Preconditioncheck and Downloading status can be safely reset to ELIGIBLE.
			//  At startup time, Preconditioncheck and Downloading status means that in previous run something went wrong updating the status in DB
			//  booking the order or after a download from Synch storage manager to Local cache.
			//  Booking can be safely repeated in the export process.
			//  Downloading from Synch storage manager to Local cache can be safely repeated in the export process.

			db::Transaction transaction(_conn);
			transaction.begin_work("PackageExporterResetDownloadingStatus");

			// Create a temporary table containing package ids from distribution queue's that are in downloading status

			vector<dbPersistent::IdType> v ;
			dbTemporaryTable ttable(_conn, true); /* Create temporary table (will be dropped leaving the scope) */
			ttable.setColumnName("packageid");
			string tempTable_Distributionqueue_downloading_status = ttable.createTable(v);

			//Populate the Temporary Table
			vector<string> statuses;
			statuses.push_back( acs::Status::preconditioncheck );
			statuses.push_back( acs::Status::downloading);
			this->fillPackageidTemporaryTable(tempTable_Distributionqueue_downloading_status, statuses);

			//Log Packages found in downloading status
			string statusesStr("");
			for ( vector<string>::const_iterator it = statuses.begin(); it!=statuses.end(); ++it ) {
				statusesStr += (it != statuses.begin()) ? ", " : "";
				statusesStr += *it;
			}

			size_t count = this->logPackageidFromTemporaryTable(tempTable_Distributionqueue_downloading_status, statusesStr);

			if (count > 0) {
				ACS_LOG_PRIVINFO("Action for Package ids in Distribution queue found in ["<<statusesStr<<"] Status: Reset to "<<acs::Status::eligible);

				//Eligible has to transit from Error
				this->setErrorPackageidFromTemporaryTable(tempTable_Distributionqueue_downloading_status );
				this->setEligiblePackageidFromTemporaryTable(tempTable_Distributionqueue_downloading_status );

			}

			transaction.commit();
		}

		{ // Transaction scope

			// Distribution status reset.
			//  Distribution status is reset according to action configured in conf space.
			//  At startup time, Distribution status means that in previous run something went wrong during an export or
			//  updating the status in DB after an export.
			//  Here PackageExporter does not know if the item has been actually transferred to destination and reset status according to configuration.

			db::Transaction transaction(_conn);
			transaction.begin_work("PackageExporterResetIntermediateStatus");

			// Create a temporary table containing package ids from distribution queue's that are in intermediate status
			//   (distribution queue could contain a lot of id. Building "select where in ..." can overflow max sql string)

			vector<dbPersistent::IdType> v ;
			dbTemporaryTable ttable(_conn, true); /* Create temporary table (will be dropped leaving the scope) */
			ttable.setColumnName("packageid");
			string tempTable_Distributionqueue_interim_status = ttable.createTable(v);

			//Populate the Temporary Table
			vector<string> statuses;
			statuses.push_back(acs::Status::distribution);

			this->fillPackageidTemporaryTable(tempTable_Distributionqueue_interim_status, statuses);

			//Log Packages found in Intermediate status
			string statusesStr("");
			for ( vector<string>::const_iterator it = statuses.begin(); it!=statuses.end(); ++it ) {
				statusesStr += (it != statuses.begin()) ? ", " : "";
				statusesStr += *it;
			}

			size_t count = this->logPackageidFromTemporaryTable(tempTable_Distributionqueue_interim_status, statusesStr);

			if ( count > 0 ) {
				if ( recoveryAction == nop ) {

					ACS_LOG_PRIVINFO("Action for Package ids in Distribution queue found in ["<<statusesStr<<"] Status: Left as is");

				} else {

					ACS_LOG_PRIVINFO("Action for Package ids in Distribution queue found in ["<<statusesStr<<"] Status: Reset "<<recoveryAction);

					//Eligible has to transit from Error
					if ( recoveryAction == toError || recoveryAction == toEligible ) { //Update to Error the retrieved packages
						this->setErrorPackageidFromTemporaryTable(tempTable_Distributionqueue_interim_status);
					}

					if ( recoveryAction == toEligible ) { //Update to Eligible the retrieved packages
						this->setEligiblePackageidFromTemporaryTable(tempTable_Distributionqueue_interim_status );
					}

				}
			}

			transaction.commit();
		}

	} catch (exception& ex) {

		ACS_LOG_WARNING("Error resetting intermediate status for items in distribution queue.  This operation is not critical and will be retried on next start.");
        ACS_LOG_NOTIFY_EX(ex) ;

	}

}



_ACS_END_NAMESPACE

