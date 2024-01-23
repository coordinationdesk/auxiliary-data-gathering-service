// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2021, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.7  2018/06/21 12:46:14  damdec
 *  get/insert methods renamed.
 *
 *  Revision 7.6  2018/06/14 14:42:24  damdec
 *  PDSEV-24: implementation finalized.
 *
 *  Revision 7.5  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.4  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.3  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.2  2017/02/06 08:45:42  lucio.pulvirenti
 *  S2PDGS-1656: updateErrorStatus: boolean added (default true) to throw exception when updating record columns other than status.
 *  startWorkingOnCirculation: exception thrown if cannot set circulation queue record status to PROCESSING.
 *
 *  Revision 7.1  2017/02/03 15:49:11  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.1  2016/08/31 08:42:58  damdec
 *  PDSEV-24: Method fillPendingCirculation added.
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.25  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.24  2015/11/16 16:29:35  lucio.pulvirenti
 *  S2PDGS-1373: updateErrorStatus IF changed with addition of an output parameter (bool ptr) added to return if recoverable error hase been set.
 *
 *  Revision 5.23  2015/09/03 14:36:04  damdec
 *  S2PDGS-1247: t_circulationsqueue.media_circulation_id unset when recoverable error occurs.
 *
 *  Revision 5.22  2015/08/04 07:45:16  lucio.pulvirenti
 *  S2PDGS-1247: MEDIA_WAIT status mo more to be set if protocol is media.
 *
 *  Revision 5.21  2015/07/24 15:40:48  marfav
 *  S2PDGS-1247 Taking into accont the destination protocol id if available
 *
 *  Revision 5.20  2015/03/10 15:12:40  lucio.pulvirenti
 *  S2PDGS-984: updateErrorStatus: added a boolean parameter to IF to set possibly given t_circulationsqueue record to ERROR status (before it was always
 *  put to RECOVERABLEERROR.
 *
 *  Revision 5.19  2015/03/06 16:46:19  lucio.pulvirenti
 *  S2PDGS-984: wokr in progress.
 *
 *  Revision 5.18  2014/08/06 16:06:23  lucio.pulvirenti
 *  S2PDGS-835: Exception nofificated before warning message.
 *
 *  Revision 5.17  2014/07/30 13:27:35  lucio.pulvirenti
 *  S2PDGs-806: fillInInterruptedCirculations method modified to query also records with status PROCESSING and not to update returned records.
 *  cleanupRecords: on every circulations record returned by fillInInterruptedCirculations, updateErrorStatus is called.
 *
 *  Revision 5.16  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.15  2014/06/16 14:37:04  lucio.pulvirenti
 *  S2PDGS-764: findLocalhost: boolean paramaters added to IF to reload t_knownhost table every time and not to log exception.
 *
 *  Revision 5.14  2014/06/05 18:48:09  marpas
 *  getting rid of asserts
 *
 *  Revision 5.13  2014/02/20 14:36:44  lucio.pulvirenti
 *  Debug improved.
 *
 *  Revision 5.12  2014/02/07 18:53:36  marpas
 *  adopting ACS_THROW and ACS_COND_THROW macros
 *
 *  Revision 5.11  2014/02/05 15:14:01  lucio.pulvirenti
 *  findLocalhost: check on value returned by conf::CirculationConfigurerSingleton::instance()->getHostName() no more needed.
 *
 *  Revision 5.10  2014/01/13 16:15:53  marfav
 *  S1PDGS-31349 Implementation completed
 *
 *  Revision 5.9  2013/11/21 14:49:59  marpas
 *  fixing debug variable and messages
 *
 *  Revision 5.8  2013/11/05 17:23:49  marpas
 *  substitute error message with log
 *
 *  Revision 5.7  2013/11/04 14:26:22  marpas
 *  coding best practices applied
 *
 *  Revision 5.6  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.5  2013/07/23 17:16:35  marpas
 *  getting rid of boost datetime functions in favour of DateTime
 *
 *  Revision 5.4  2013/07/22 13:19:18  marpas
 *  tyoes fixing
 *  coding best practices applied
 *  test fixed
 *
 *  Revision 5.3  2013/07/16 14:52:16  marpas
 *  coding best practices applied,
 *  performances and robustness improved
 *
 *  Revision 5.2  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.1  2013/07/12 09:06:55  marpas
 *  porting circulation business intelligence into the library
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.51  2013/04/22 11:00:24  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.50  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.49  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.48  2012/11/29 16:55:05  micmaz
 *  work in progress
 *
 *  Revision 1.47  2012/11/28 12:36:42  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.46  2012/11/27 15:24:33  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.45  2012/11/23 16:28:38  micmaz
 *  work in progress
 *
 *  Revision 1.44  2012/11/14 10:32:17  micmaz
 *  now when a circualation reach  the final status , it will be updated in one shot.
 *
 *  Revision 1.43  2012/06/21 12:03:06  micmaz
 *  removed error logs
 *
 *  Revision 1.42  2012/04/03 17:45:48  micmaz
 *  work in progress
 *
 *  Revision 1.41  2012/03/27 10:54:15  micmaz
 *  work in progress
 *
 *  Revision 1.40  2012/03/01 11:40:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.39  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.38  2012/02/01 18:12:50  micmaz
 *  the find localhost function must search this host on the database, if it is not  exist, skip and return only the host name without id (primary key on the database).
 *  This host is not present on the database only if there are no policies associated with it.
 *  Thus for the Circulation Agent there is no need to have the host recorded on the db, on the other hand for
 *  the CirculationService it is important that this host is recorded on db.
 *
 *  Revision 1.37  2012/02/01 16:05:00  micmaz
 *  removed loading local host from db.
 *
 *  Revision 1.36  2012/01/30 17:58:55  micmaz
 *  IN PROGRESS - issue S1PDGS-2736: CirculationService hangs in case of multiple policies installed on the same circulation node and using local baskets
 *  http://jira.acsys.it/browse/S1PDGS-2736
 *
 *  Revision 1.35  2012/01/11 16:45:31  micmaz
 *  IN PROGRESS - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 1.34  2011/11/03 16:43:26  micmaz
 *  IN PROGRESS - issue S1PDGS-2343: CirculationService and DataStorageService hostname problem
 *  http://jira.acsys.it/browse/S1PDGS-2343
 *
 *  Revision 1.33  2011/11/03 16:02:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.32  2011/11/02 16:45:40  micmaz
 *  findLocalHost() must not return an object, instead it must return a pointer to avoid useless copies.
 *
 *  Revision 1.31  2011/10/28 16:41:18  micmaz
 *  IN PROGRESS - issue S1PDGS-2342: The Circulation Service does not update the end date in case of error and the start date.
 *  http://jira.acsys.it/browse/S1PDGS-2342
 *
 *  Revision 1.30  2011/09/23 09:14:57  micmaz
 *  removed unused headers.
 *
 *  Revision 1.29  2011/09/19 17:16:13  micmaz
 *  updated circulations  sort.
 *
 *  Revision 1.28  2011/09/19 15:43:26  micmaz
 *  OPEN - issue S1PDGS-1910: Wrong status changing in case of RECOVERABLE_ERROR
 *  http://jira.acsys.it/browse/S1PDGS-1910
 *
 *  Revision 1.27  2011/09/09 15:13:47  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.26  2011/09/01 09:22:27  micmaz
 *  removed excerr.
 *
 *  Revision 1.25  2011/09/01 08:52:49  micmaz
 *  removed excerr.
 *
 *  Revision 1.24  2011/08/31 14:18:27  micmaz
 *  RESOLVED - issue S1PDGS-1805: the CirculationAgent does not create a new consumer quickly when a single consumer finished.
 *  http://jira.acsys.it/browse/S1PDGS-1805
 *
 *  Revision 1.23  2011/08/10 16:48:06  micmaz
 *  the start date was not filled in.
 *
 *  Revision 1.22  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.21  2011/07/22 09:00:57  micmaz
 *  work in progress..
 *
 *  Revision 1.20  2011/07/15 17:10:20  micmaz
 *  work in progress
 *
 *  Revision 1.19  2011/07/13 18:04:08  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.18  2011/07/13 17:06:03  micmaz
 *  REOPENED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.17  2011/07/12 17:22:33  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.16  2011/07/12 14:30:04  micmaz
 *  RESOLVED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.15  2011/06/20 16:26:23  micmaz
 *  work in progress
 *
 *  Revision 1.14  2011/06/20 16:01:25  micmaz
 *  work in progress.
 *
 *  Revision 1.13  2011/06/16 12:49:45  micmaz
 *  work in progress..
 *
 *  Revision 1.12  2011/06/14 15:02:29  micmaz
 *  correct date arithmetic
 *
 *  Revision 1.11  2011/06/14 13:36:02  micmaz
 *  the circulation queues related to this host must have the relay host id null.
 *
 *  Revision 1.10  2011/06/10 16:26:52  micmaz
 *  work in progress..
 *
 *  Revision 1.9  2011/06/10 13:47:09  micmaz
 *  work in progress
 *
 *  Revision 1.8  2011/06/09 17:04:53  micmaz
 *  work in progress...
 *
 *  Revision 1.7  2011/06/09 15:38:16  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/06/07 17:20:32  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/05/10 10:25:39  micmaz
 *  added a while() loop to slow the reading stream in order to limit the amount of bytes in memory.
 *
 *  Revision 1.4  2011/04/27 14:29:25  micmaz
 *  added version var
 *
 *  Revision 1.3  2011/04/20 16:38:24  micmaz
 *  the "errored" circulation records must be re-worked only after ELAPSED_TIME_TO_RETRY seconds.
 *
 *  Revision 1.2  2011/04/11 12:25:16  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/08 17:00:39  micmaz
 *  work in progress...
 *

 *
 *
 *
 */

#include <CirculationDBService.h>
#include <CirculationCallbackSingleton.h>
#include <CirculationMode.h>
#include <SQLString.h>
#include <DateTime.h>
#include <DataConsumerConfigurer.h>
#include <CirculationVersion.h>
#include <RemoteSiteFactory.h>
#include <RemoteSite.h>
#include <HWScan.h>
#include <Transaction.h>

#include <boost/bind.hpp> // PRQA S 1013 2

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

using namespace std;
using namespace acs::curl;
using namespace acs::db;
using namespace boost;

namespace {
    CirculationVersion Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(CirculationDBService);

CirculationDBService::CirculationDBService() :
    _mutex(),
    // the following date is a trick in order to invalidate the first pass time.
    _lastTimeFillInCirculations("01-Jan-1970 00:00:00"), 
    _localhost()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
    
    // empty
}

CirculationDBService::~CirculationDBService() throw(){
    // empty
}
void CirculationDBService::updateCirculationStatus(dbConnection &conn, string const& statusName, Circulation & circ) const  // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[updateCirculationStatusWithStatus: " << statusName << " WithCirculation: " << circ.str(conn, false) << "]");
    circ.setStatus(Status::instance(conn.getKey())->id(statusName, false), conn);
    circ.update(conn);
    DateTime transferringDate;
    circ.setModificationDate(transferringDate, conn);
    circ.update(conn);
}

void CirculationDBService::updateNextRetryDate(dbConnection& conn, const Circulation& circ) const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computing next retry date for circulation : " << circ.getId());

    // Get the correct policy from the configuration
    conf::ImplicitCirculationConfigurer::RetryConfiguration retryConf;
    const string &key = conn.getKey() ;
    bool null = false ;
    int circMode = circ.getCirculationModeId(null, conn);
    if (circMode == CirculationMode::instance(key)->id(CirculationMode::implicitMode, false))
    {
        // Implicit mode
        retryConf = acs::conf::CirculationConfigurerSingleton::instance()->getImplicitRetryConfiguration();
    }
    else if (circMode == CirculationMode::instance(key)->id(CirculationMode::explicitMode, false))
    {
        // Explicit Mode
        retryConf = acs::conf::CirculationConfigurerSingleton::instance()->getExplicitRetryConfiguration();
    }
    else
    {
        // Dynamic mode by default
        retryConf = acs::conf::CirculationConfigurerSingleton::instance()->getDynamicRetryConfiguration();
    }

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Using the following retry configuration: " << conf::ImplicitCirculationConfigurer::RetryAlgo2String (retryConf.algo));

    DateTime nextRetryDate (getCurrentDbDate(conn));
    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Current database time " << nextRetryDate.asISO8601(3));

    unsigned long offsetInSeconds = 0 ;

    switch (retryConf.algo)
    {
        case conf::ImplicitCirculationConfigurer::LINEAR:
        {
            offsetInSeconds = linearRetryOffset (conn, nextRetryDate, retryConf, circ);
            break;
        }
        case conf::ImplicitCirculationConfigurer::EXP2LIMITED:
        {
            offsetInSeconds = exp2LimitedRetryOffset (conn, nextRetryDate, retryConf, circ);
            break;
        }
        case conf::ImplicitCirculationConfigurer::STEPPED:
        default:
        {
            offsetInSeconds = steppedRetryOffset (conn, nextRetryDate, retryConf, circ);
        }
    }

    nextRetryDate += (offsetInSeconds / 86400.0); // add the offset // PRQA S 3011, 3084, 4400

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Retry offset in seconds is " << offsetInSeconds << ". Next retry timeout is " << nextRetryDate.asISO8601(3));


    // Do the record update
    // In case of error let the exception go outside
    Circulation therecord(circ) ;
    // Update the value
    therecord.setNextRetryDate(nextRetryDate, conn );
    if( dbSchemaMetaDataProvider::instance(key) -> hasField (Circulation::table(), "media_circulation_id", conn) ) {
    	// set media Id to NULL
	    therecord.unset( "media_circulation_id", conn );
    }
    // And save
    therecord.update(conn);  	
}


unsigned long CirculationDBService::linearRetryOffset (dbConnection& conn, const DateTime& currentDbTime, 
        const conf::ImplicitCirculationConfigurer::RetryConfiguration& conf, const Circulation& circ) const
{
    return conf.retryTimeout;
}

unsigned long CirculationDBService::exp2LimitedRetryOffset (dbConnection& conn, const DateTime& currentDbTime, // PRQA S 4214, 4020
        const conf::ImplicitCirculationConfigurer::RetryConfiguration& conf, const Circulation& circ) const
{
    // in case maxRetry is less than retryTimeout do never use the exponential function
    long nmax = -1;
    bool null = false ;
    // retry counter is updated before calling the offset computation function
    long n = circ.getRetries(null, conn) - 1; // PRQA S 3000, 3084

    if (conf.maxRetryTimeout > conf.retryTimeout)
    {
        nmax = rint ( trunc ( log2 (conf.maxRetryTimeout / (conf.retryTimeout / 1.0)))); // PRQA S 3011, 3084
    }

    if ( n <= nmax)
    {
        // exponential
        return exp2 (n) * conf.retryTimeout; // PRQA S 3011, 3084
    }
    else
    {
        // limited
        return conf.maxRetryTimeout;
    }

}

unsigned long CirculationDBService::steppedRetryOffset (dbConnection& conn, const DateTime& currentDbTime, // PRQA S 4020, 4214
        const conf::ImplicitCirculationConfigurer::RetryConfiguration& conf, const Circulation& circ) const
{
    bool null = false ;
	
    short int n = circ.getRetries(null, conn);
	unsigned long stepTimeout = 0;
	if( n < 0 ) {
		stepTimeout = conf.step1Timeout;
	}
    else if( static_cast<unsigned int>(n) <= conf.step1Threshold)
    {
        stepTimeout = conf.step1Timeout;
    }
    else
    {
        stepTimeout = conf.step2Timeout;
    }
	
	return stepTimeout;
}

void CirculationDBService::updateErrorStatus(dbConnection& conn, Circulation& circ, bool recoverableError, bool *recoverableErrorSet, bool emitExcAfterUpdate) const // PRQA S 4214
{
	bool null = false ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Changing : " << circ.getId() << " from: " << circ.getStatus(null, conn) << " to error status (" // PRQA S 3380
							<< ( recoverableError ? "recoverable error" : "unrecoverable error" ) << "). ");
	const string &key = conn.getKey() ;

	// Must update the record in a single transaction to avoid that
	// the SP updates the status AND the the retrydate is updated in a second operation
	// if the query operates in the middle of the two updatest, the retrydate will not be consistent
	db::Transaction transaction(conn) ;

    DateTime errorDate;
	if( recoverableError ) {
		// This block shall not have any intermediate exit point to avoid auto rollback of the transaction
		// Begin the transaction .. in case of exception the rollback is automatic
		transaction.begin_work ("CIRC_RECERR_UPD");
		
    	SQLString updateErroredStatus(key, SQLString::CUSTPROCEDURE);
    	updateErroredStatus.setProcedureName("p_circulationSetErrorType");
    	updateErroredStatus.addProcedureParameter(circ.getId());
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following command: \"" <<  updateErroredStatus.getSQL() << "\"");
    	dbQuery query(conn, updateErroredStatus, "calling p_circulationSetErrorType");
		if (not query.execForStatus()) {
			ACS_THROW ( dbQuery::QueryExecuteException( // PRQA S 3081
                            "Query Execution error SQL command is \"" + 
                            updateErroredStatus.getSQL() + "\". Error is \"" + query.errorString()  + "\".") 
                      ) ;
		}
		
		try {
			circ.load(conn);
			if( recoverableErrorSet ) {
				*recoverableErrorSet = false;
				if( Status::instance(key)->name(circ.getStatus(null, conn)) == Status::recoverableError ) {
            		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Status of circulations queue record with id " << circ.getId() << " has been set to " << Status::recoverableError);
					*recoverableErrorSet = true;
				}
			}

    		circ.setModificationDate(errorDate, conn);
			
			if (circ.getStatus(null, conn) == Status::instance(key)->id(Status::error, false)) {
        		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Updating the end date.");
        		circ.setEndDate(errorDate, conn);
        		circ.update(conn);
    		}
			else  {
			    circ.update(conn);
        		// Update also the next retry date
        		updateNextRetryDate (conn, circ);
    		}


		}
		catch( std::exception const& ex )
		{
			if(emitExcAfterUpdate) {
				throw;
			}
			else {
				ACS_LOG_WARNING("Error trying to update circulation queue record ALREADY SET IN ERROR status: " << ex.what());
			}
		} 

		// Commit all the changes
		transaction.commit();
	}
	else {
		// Begin the transaction .. in case of exception the rollback is automatic
		transaction.begin_work ("CIRC_ERR_UPD");
    	circ.load(conn);
    	circ.setModificationDate(errorDate, conn);
		circ.setStatus(Status::instance(key)->id(Status::error, false), conn);
       	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Updating the end date.");
        circ.setEndDate(errorDate, conn);
    	circ.update(conn);
		// Commit all the changes
		transaction.commit();
	}


    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Updated circulation last modification time: [" << circ.str(conn, true) << "]");
	

}

void CirculationDBService::updateToEndStatus(dbConnection &conn, // PRQA S 4214
                                             string const& finalStatus, 
                                             Circulation & circ) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[updateToCompleteWithStatus: " << finalStatus << " WithCirculation: " << circ.str(conn, false) << "]");
    circ.setStatus(Status::instance(conn.getKey())->id(finalStatus, false), conn);
    DateTime endDate;
    circ.setModificationDate(endDate, conn);
    circ.setEndDate(endDate, conn);
    circ.update(conn);
}

size_t CirculationDBService::fillInInterruptedCirculations(dbConnection &conn, // PRQA S 4211
                                                           vector<Circulation> & results,
                                                           size_t const maxItems) {
    ThreadSafe::Lock lock(_mutex);

    try {
        const string &key = conn.getKey() ;
        short transferring = Status::instance(key)->id(Status::transferring, false);
		short processing = Status::instance(key)->id(Status::processing, false);
		short blocking = Status::instance(key)->id(Status::blocking, false);
        CirculationQuery theQuery(key);
        if (maxItems != 0) {
            theQuery.maxItems(maxItems); // PRQA S 3000
        }
		
		{
        	dbQueryParameters qp(key);
			qp.setParamOperator( dbQueryParameters::OR ) ;
        	qp.addParameter( dbParam(key, Circulation::table(), "status_id", dbParam::EQUAL_TO, dbConv(key, transferring))); // PRQA S 3050
			qp.addParameter( dbParam(key, Circulation::table(), "status_id", dbParam::EQUAL_TO, dbConv(key, processing))); // PRQA S 3050
 			qp.addParameter( dbParam(key, Circulation::table(), "status_id", dbParam::EQUAL_TO, dbConv(key, blocking))); // PRQA S 3050
       	theQuery.addParameter(qp);
		}
		
		{
        	dbQueryParameters qp(key);
            qp.addParameter( dbParam(key, Circulation::table(), "relay_host_id", dbParam::ISNULL) );
        	theQuery.addParameter(qp);
		}

		// Do not take into account media circulations if the relevant column exists in the queue
		if ( checkHasProtocolIdField(conn) )
		{

			// conditioning subquery
            SQLString insub (key, SQLString::SELECT);
            insub.addSelectList ("id");
            insub.addTableList ("t_transferprotocols");
            dbQueryParameters insubqp(key) ;
            insubqp.addParameter(dbParam(key, "t_transferprotocols","protocol" , dbParam::DIFFERENT_FROM , toSQL(key, "MEDIA"))) ;
            insub.addANDWhereParameter(insubqp) ;

        	dbQueryParameters qp(key);
            qp.addParameter( dbParam(key, Circulation::table(), "dest_protocol_id", dbParam::IN, insub) );
        	theQuery.addParameter(qp);
		}

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Do interrupted query.");
        size_t size = theQuery.doQuery(conn);
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Interrupted Circulation records (transferring/processing/blocking) found :" << size);
        theQuery.fill(results) ;
        return results.size();
    } catch (std::exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex.what());
    }
    ACS_THROW(Exception("Cannot find interrupted circulations for this host.")) ; // PRQA S 3081
}



void CirculationDBService::findAlreadyCreatedQueues( // PRQA S 4214
	CirculationQuery &loopQuery, 
	dbConnection &conn, 
	string const& fromURL, 
	string const& toURL,
	vector<int> const& stati,
	dbPersistent::IdType hostId) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[findAlreadyCreatedQueuesWithFromURL: " << fromURL << " AndToURL: " << toURL << " HostId " << hostId << "]");
    Lock lock(_mutex);

    const string &key = conn.getKey() ;
    {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "host_id", dbParam::EQUAL_TO, dbConv(key, hostId))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }

    {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "status_id", dbParam::IN, stati));
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);

    }
    {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "fromurl", dbParam::EQUAL_TO, dbConv(key, fromURL))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
    if (StringUtils::isNotBlank(toURL)) {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "tourl", dbParam::EQUAL_TO, dbConv(key, toURL))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
    {
        int mode_id = CirculationMode::instance(key)->id(CirculationMode::implicitMode, false);
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "circulationmode_id", dbParam::EQUAL_TO, dbConv(key, mode_id))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
}

void CirculationDBService::findDuplicatedQueues( // PRQA S 4214
	CirculationQuery &loopQuery, 
	dbConnection &conn, 
	string const& fromURL, 
	string const& toURL,
	dbPersistent::IdType hostId) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[findDuplicatedQueues WithFromURL: " << fromURL << " AndToURL: " << toURL << " HostId " << hostId << "]");
    Lock lock(_mutex);
    const string &key = conn.getKey() ;
    {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "host_id", dbParam::EQUAL_TO, dbConv(key, hostId))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
    {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "fromurl", dbParam::EQUAL_TO, dbConv(key, fromURL))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
    if (StringUtils::isNotBlank(toURL)) {
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "tourl", dbParam::EQUAL_TO, dbConv(key, toURL))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
    {
        int mode_id = CirculationMode::instance(key)->id(CirculationMode::implicitMode, false);
        dbQueryParameters params(key);
        params.addParameter(
                dbParam(key, Circulation::table(), "circulationmode_id", dbParam::EQUAL_TO, dbConv(key, mode_id))); // PRQA S 3050
        loopQuery.addParameter(params, dbGeneralQueryIF::AND);
    }
}


void CirculationDBService::startWorkingOnCirculation(dbConnection &conn, Circulation & item) {
    const string &key = conn.getKey() ;
	short eligible = Status::instance(key)->id(Status::eligible, false);
    short processing = Status::instance(key)->id(Status::processing, false);
	short blocked = Status::instance(key)->id(Status::blocked, false);
	short newStatusId = processing;
    DateTime now;
    try {
        bool null = false ;
		short statusId = item.getStatus(null, conn);
        if ( statusId == eligible || statusId == blocked ) {
            item.setStartDate(now, conn);
			if( statusId == blocked ) {
				newStatusId = Status::instance(key)->id(Status::blocking, false);
			}
        }
        item.setStatus(newStatusId, conn);
        item.setModificationDate(now, conn);
        item.update(conn);
    } catch (std::exception const& ex) {
		ACS_LOG_NOTIFY_EX(ex);
		string circulationId("UNKNOWN");
		string errMsg;
		try {
			if( item.isFromDatabase() ) {
				ostringstream os;
				os << item.getId();
				circulationId = os.str();
			}
		}
		catch( std::exception const& ee)
		{
			ostringstream os;
			os << "...cannot even get queue id from Circulation: " << ee.what();
			errMsg = os.str();
		}
		ACS_LOG_ERROR("Circulation id# " << circulationId << ": could not change status to " << Status::instance(key)->name(newStatusId) << errMsg );
		
		try {
			 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Error got on circulation record " << item.str(conn) );
		}
		catch( std::exception const& )
		{}
		
		throw;
    }
}

size_t CirculationDBService::cleanupRecords(dbConnection& conn, size_t maxItems) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Retrieving all Circulations in trasferring and putting them all in error, resetting the ones in blocking to blocked");
    vector<Circulation> circulations;
    size_t results = fillInInterruptedCirculations(conn, circulations, maxItems);
	size_t resetRecords = 0;
    for (size_t counter = 0; counter < results; ++counter) {
        Circulation item = circulations[counter];
		bool blockingStatus = false;
		try {
			bool isNull(false);
			blockingStatus = ( item.getStatus(isNull, conn) == Status::instance(conn.getKey())->id(Status::blocking) );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to set record #" << counter << ' ' << item.str(conn) << " to " << ( blockingStatus ? "blocked" : "error" ) );
			if( blockingStatus ) {
				updateCirculationStatus(conn, Status::blocked, item);
			}
			else {
				updateErrorStatus(conn, item);
				dbPersistent::IdType circ_id = item.getId(); // get circulation id
				short status_id = item.getStatus(isNull, conn);
				DateTime updatetime = item.getModificationDate(isNull, conn);

				try
				{
					// fill Pending Circulations queue: save circulation info
					CirculationCallbackSingleton::instance()->insertCallbackEventsInQueue (conn, circ_id, status_id, updatetime);	
				}
				catch (std::exception const & ex)
				{
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Callback pending circulation queue cannot be updated!" << ex.what());
				}
			}
			
			resetRecords++;
		}
		catch( std::exception &e )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot update record #" << counter << " status to " << ( blockingStatus ? "blocked: " : "error: " ) << e);
		}
    }
	
	if( results != resetRecords ) {
		ACS_LOG_WARNING(results << " interrupted circulations found but " << resetRecords << " only successfully updated to error status or reset to blocked status");
	}

    return resetRecords;
}

KnownHost* CirculationDBService::findLocalhost(dbConnection &conn, bool reload, bool logException) const // PRQA S 4214
{ 
    if (!_localhost.get() || reload ) { // not loaded or re-load request
        ThreadSafe::Lock lock(_mutex); // protect from other threads
        if (!_localhost.get() || reload ) { // still not loaded or re-load request
            string hostname = conf::CirculationConfigurerSingleton::instance()->getHostName() ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "localhost name returned: " << hostname);
            _localhost.reset();
            try {
                _localhost.reset(new KnownHost(KnownHost::findHostByName(conn, hostname)));
            } catch (std::exception const& ex) {
				if( logException ) {
                	ACS_LOG_ERROR( "Cannot find this host on the database: this host does not have any policy associated: " << hostname);
				}
            }
        }
    }
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to return: " << _localhost.get());
    return _localhost.get();
}

bool CirculationDBService::isOlderThan(time_t seconds, DateTime& dateTime) // PRQA S 4020
{
    DateTime t(time(0) - seconds) ;
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[elapsedTimeWithBegin: " << dateTime.getSqlString() << " seconds: " << seconds << "]");
    if (dateTime < t) { 
        dateTime = DateTime(); // now
        return true;
    }
    return false;

}


vector<int> CirculationDBService::getNotEndStati(const string &dbkey) const { // PRQA S 4214
    vector<int> stati ;
    stati.push_back(Status::instance(dbkey)->id(Status::recoverableError));
    stati.push_back(Status::instance(dbkey)->id(Status::eligible));
    stati.push_back(Status::instance(dbkey)->id(Status::processing));
    stati.push_back(Status::instance(dbkey)->id(Status::standby));
    stati.push_back(Status::instance(dbkey)->id(Status::transferring));
    stati.push_back(Status::instance(dbkey)->id(Status::blocked));
    stati.push_back(Status::instance(dbkey)->id(Status::blocking));
    return stati ;
}


bool CirculationDBService::checkHasNextRetryField (dbConnection& conn) const
{
    const string &dbKey = conn.getKey() ;

    return dbSchemaMetaDataProvider::instance(dbKey) -> hasField ( "t_circulationsqueue", "nextretrydate", conn);
}

bool CirculationDBService::checkHasProtocolIdField (dbConnection& conn) const
{
    const string &dbKey = conn.getKey() ;
    return dbSchemaMetaDataProvider::instance(dbKey) -> hasField ("t_circulationsqueue", "dest_protocol_id", conn);
}

DateTime CirculationDBService::getCurrentDbDate (dbConnection &conn) const
{
    static const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
    ACS_COND_THROW(dialectName != "Postgres8" && dialectName != "Informix9", // PRQA S 3081
        exIllegalValueException("Database dialect not supported : " + dialectName) );

    string query;
    if (dialectName == "Postgres8") {
        query = "select localtimestamp;";
    } else {
        query = "select first 1 CURRENT from systables;";
    }

    DateTime retVal; // Localhost now time

    try
    {
        SQLString sqlstring(conn.getKey(), query);
        dbQuery nowQuery(conn, sqlstring,"SEL Current time");

	    dbSet set ;
	    nowQuery.execForSet(set) ;
        dbRow myRow(conn.getKey()) ;   

        set.getNextRow(myRow) ; // first and only 

        myRow.column(0,retVal);
    }
    catch (std::exception& ex)
    {
        ACS_LOG_NOTIFY_EX (ex);
        ACS_LOG_WARNING ("Unable to get current time from database. Using local host current time");
    }

    return retVal;

}

_ACS_END_NESTED_NAMESPACE;
