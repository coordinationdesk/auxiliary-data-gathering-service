// PRQA S 1050 EOF
/*
 *
	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.12  2018/07/30 09:25:21  lucio.pulvirenti
 *  S3EVOL-159: findCirculationsToConsume: status = blocked added in OR to where conditions on circulation queue records.
 *  manageInterruptedCirculations: circulation records in blocking status reset to blocked.
 *
 *  Revision 7.11  2018/01/18 14:27:18  chicas
 *  messages to PRIVINFO
 *
 *  Revision 7.10  2017/11/15 13:55:48  chicas
 *  some message to PRIVINFO
 *
 *  Revision 7.9  2017/11/15 10:28:33  chicas
 *  some message to PRIVINFO
 *
 *  Revision 7.8  2017/11/07 15:22:05  fabrizio.tringali
 *  adapted by MF for C++ 11 compiler
 *
 *  Revision 7.7  2017/10/26 14:12:40  marpas
 *  enforcing multiyhreaded safety and cleaning the I/F
 *
 *  Revision 7.6  2017/10/25 13:57:54  marpas
 *  getting rid of ThreadGroup and using Thread class' range functions
 *
 *  Revision 7.5  2017/10/19 16:18:03  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.4  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.3  2017/02/06 09:00:34  lucio.pulvirenti
 *  S2PDGS-1656: addCirculationFromRow: catch exception thrown by CirculationItem CTOR and put circulation record into _interruptedCirculations list.
 *  manageInterruptedCirculations and fillInterruptedCirculationsFromChannel methods developed and used to manage circulations that have never been started.
 *
 *  Revision 7.2  2017/02/03 15:49:11  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 7.1  2017/01/31 09:29:51  marfav
 *  Fixing implicit cast from ptr to reference in 'ref' boost ctor
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.3  2016/10/12 10:06:44  lucio.pulvirenti
 *  CSGACQ-86: findCirculationsToConsume: used SQLString for both Postgres and Oracle query. Pre S1PDGS-31034 schema no more supported.
 *  Deprecated method execOneRow replaced.
 *
 *  Revision 6.2  2016/10/11 10:04:14  lucio.pulvirenti
 *  CSGACQ-86: findCirculationsToConsume: query written for Oracle too.
 *
 *  Revision 6.1  2016/08/31 08:39:27  damdec
 *  PDSEV-24: getManager method added.
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.38  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.37  2015/11/06 14:49:31  marfav
 *  In case host id is nullable in the queue, select also records with null host id
 *
 *  Revision 5.36  2015/10/09 08:24:42  lucio.pulvirenti
 *  APF-324: findCirculationsToConsume: row filtered by host_id = localhost id; it should have been already done by S2PDGS-869.
 *
 *  Revision 5.35  2015/08/04 08:15:29  lucio.pulvirenti
 *  S2PDGS-1247: findCirculationsToConsume: bug fixed in the last modification.
 *
 *  Revision 5.34  2015/07/24 15:40:48  marfav
 *  S2PDGS-1247 Taking into accont the destination protocol id if available
 *
 *  Revision 5.33  2015/07/21 10:46:23  damdec
 *  PDSEV-7: implementation finalized.
 *
 *  Revision 5.32  2015/07/16 15:26:57  damdec
 *  PDSEV-7: Selection loop efficiency improved.
 *
 *  Revision 5.31  2015/06/08 17:42:48  marfav
 *  S2PDGS-1253 including records with NULL dest_host_id when knownhosts_limits is applied
 *
 *  Revision 5.30  2015/03/09 13:24:15  marfav
 *  S2PDGS-1128 avoid thread asynch cancellation
 *
 *  Revision 5.29  2014/09/22 08:31:52  lucio.pulvirenti
 *  Bug fixed: findCirculationsToConsume: records in recoverable error must be managed if relat_host_id is null only.
 *
 *  Revision 5.28  2014/09/04 09:47:50  lucio.pulvirenti
 *  S3PDGS-1980: verifyConfigurationFile: messages added with names of loaded configuration files.
 *
 *  Revision 5.27  2014/09/02 13:16:28  lucio.pulvirenti
 *  S2PDGS-841: addCirculationFromRow: message of circulation queue insertion improved.
 *
 *  Revision 5.26  2014/08/08 16:26:54  lucio.pulvirenti
 *  S2PDGS-841: work in progress.
 *
 *  Revision 5.25  2014/08/06 16:05:20  lucio.pulvirenti
 *  S2PDGS-835: Exception notificated by dedicated macro.
 *  S2PDGS-836: fillInCirculations: recognized exception caught so that no description is needed.
 *
 *  Revision 5.24  2014/07/30 13:28:14  lucio.pulvirenti
 *  S2PDGS-806: message improved.
 *
 *  Revision 5.23  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.22  2014/06/18 13:50:14  lucio.pulvirenti
 *  S2PDGS-726: fillInCirculations: exStopChannelException managed.
 *
 *  Revision 5.21  2014/06/05 18:48:09  marpas
 *  getting rid of asserts
 *
 *  Revision 5.20  2014/04/15 15:40:48  marpas
 *  getting rid of bad log macros
 *
 *  Revision 5.19  2014/04/07 08:18:08  marfav
 *  Try to fill any empty slot as soon as a worker completes its duties
 *
 *  Revision 5.18  2014/03/13 15:54:10  marfav
 *  Added support to reverse priority order
 *
 *  Revision 5.17  2014/03/12 12:56:09  marfav
 *  S2PDGS-727 adding notification of dynamic priority when start working on a circulation item
 *
 *  Revision 5.16  2014/02/07 18:53:36  marpas
 *  adopting ACS_THROW and ACS_COND_THROW macros
 *
 *  Revision 5.15  2014/01/31 12:13:17  marpas
 *  new db::Transaction adopted
 *
 *  Revision 5.14  2014/01/30 17:37:24  lucio.pulvirenti
 *  Message improved.
 *
 *  Revision 5.13  2014/01/27 16:14:02  lucio.pulvirenti
 *  Message improved.
 *
 *  Revision 5.12  2014/01/13 16:15:53  marfav
 *  S1PDGS-31349 Implementation completed
 *
 *  Revision 5.11  2014/01/10 17:37:20  marfav
 *  S1PDGS-31349 Support of NextRetryDate id circulations queue dev in progress
 *
 *  Revision 5.10  2014/01/10 16:04:28  marfav
 *  Configuration management implementation completed
 *
 *  Revision 5.9  2013/11/04 14:26:22  marpas
 *  coding best practices applied
 *
 *  Revision 5.8  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.7  2013/09/30 12:39:33  marpas
 *  removing useless include
 *
 *  Revision 5.6  2013/09/12 07:40:30  lucio.pulvirenti
 *  Log messages improved.
 *
 *  Revision 5.5  2013/08/08 10:51:03  lucio.pulvirenti
 *  In run method, if an exception is launched from init method, the retry loop is not endless but it stops when the number
 *  of attempts is equal to _maxInitAttempts.
 *
 *  Revision 5.4  2013/07/23 17:35:01  marpas
 *  interface rationalization
 *
 *  Revision 5.3  2013/07/23 17:15:15  marpas
 *  compilation warnings removed
 *  getting rid of boost datetime functions in favour of DateTime
 *
 *  Revision 5.2  2013/07/22 13:19:18  marpas
 *  tyoes fixing
 *  coding best practices applied
 *  test fixed
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.50  2013/04/29 16:17:16  marpas
 *  using exFPrivateInfo for some info
 *
 *  Revision 1.49  2013/04/22 11:00:24  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.48  2013/04/17 21:37:08  marpas
 *  messaging improved
 *
 *  Revision 1.47  2013/04/12 12:21:39  marpas
 *  SQLDialectFactory::dialectName() needs a db connection key
 *
 *  Revision 1.46  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.45  2013/03/21 20:16:22  marpas
 *  if localhost is not found, Circulation continue running in degraded state: ApplicationStatus is Critical
 *
 *  Revision 1.44  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.43  2013/02/19 11:33:15  micmaz
 *  added db broadcast listener support
 *
 *  Revision 1.42  2013/01/25 13:47:36  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.41  2012/10/25 10:17:47  micmaz
 *  The removeSource flag must be read when the circulation has been completed.
 *
 *  Revision 1.40  2012/06/12 10:48:36  marpas
 *  PRQA instrumentation
 *
 *  Revision 1.39  2012/04/04 17:15:21  micmaz
 *  work in progress
 *
 *  Revision 1.38  2012/04/04 15:31:36  micmaz
 *  work in progress
 *
 *  Revision 1.37  2012/03/01 11:40:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.36  2012/02/20 15:11:10  marpas
 *  refactoring
 *
 *  Revision 1.35  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.34  2012/02/08 16:23:09  micmaz
 *  modified logs.
 *
 *  Revision 1.33  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.32  2012/02/01 16:05:00  micmaz
 *  removed loading local host from db.
 *
 *  Revision 1.31  2012/01/20 15:58:25  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 1.30  2012/01/17 16:50:52  micmaz
 *  OPEN - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 1.29  2011/11/02 16:45:40  micmaz
 *  findLocalHost() must not return an object, instead it must return a pointer to avoid useless copies.
 *
 *  Revision 1.28  2011/10/28 16:41:18  micmaz
 *  IN PROGRESS - issue S1PDGS-2342: The Circulation Service does not update the end date in case of error and the start date.
 *  http://jira.acsys.it/browse/S1PDGS-2342
 *
 *  Revision 1.27  2011/09/22 15:08:04  micmaz
 *  removed unused define.
 *
 *  Revision 1.26  2011/09/20 16:58:27  micmaz
 *  work in progress
 *
 *  Revision 1.25  2011/09/19 17:16:13  micmaz
 *  updated circulations  sort.
 *
 *  Revision 1.24  2011/09/19 15:43:26  micmaz
 *  OPEN - issue S1PDGS-1910: Wrong status changing in case of RECOVERABLE_ERROR
 *  http://jira.acsys.it/browse/S1PDGS-1910
 *
 *  Revision 1.23  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.22  2011/09/13 16:30:43  micmaz
 *  RESOLVED - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.21  2011/09/05 16:16:41  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.20  2011/09/05 10:05:11  micmaz
 *  the application workspace now stops if the circulation consumer cannot correctly initialize.
 *
 *  Revision 1.19  2011/08/31 14:18:27  micmaz
 *  RESOLVED - issue S1PDGS-1805: the CirculationAgent does not create a new consumer quickly when a single consumer finished.
 *  http://jira.acsys.it/browse/S1PDGS-1805
 *
 *  Revision 1.18  2011/08/12 14:48:36  micmaz
 *  work in progress..
 *
 *  Revision 1.17  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.16  2011/07/13 18:04:08  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.15  2011/06/10 16:26:52  micmaz
 *  work in progress..
 *
 *  Revision 1.14  2011/06/09 17:04:53  micmaz
 *  work in progress...
 *
 *  Revision 1.13  2011/06/09 15:38:16  micmaz
 *  work in progress
 *
 *  Revision 1.12  2011/06/07 17:20:32  micmaz
 *  work in progress
 *
 *  Revision 1.11  2011/05/03 14:04:10  micmaz
 *  corrected some circulation bugs
 *
 *  Revision 1.10  2011/04/29 17:22:14  micmaz
 *  if cannot create a log file, redirect to stderr
 *
 *  Revision 1.9  2011/04/27 13:49:26  micmaz
 *  work in progress...
 *
 *  Revision 1.8  2011/04/27 12:46:17  micmaz
 *  corrected the stop procedure
 *
 *  Revision 1.7  2011/04/15 18:02:54  micmaz
 *  work in progress...
 *
 *  Revision 1.6  2011/04/15 15:34:02  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/04/13 14:25:44  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/12 18:00:29  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/04/11 14:05:50  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/07 15:02:41  micmaz
 *  work in progress...
 *
 *  Revision 1.1  2011/04/05 17:14:24  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#include <CirculationConsumer.h>
#include <Application.h>
#include <CurlConfigurer.h>
#include <CirculationRegister.h>
#include <dbConnectionPool.h>
#include <Timer.h>
#include <StringUtils.h>
#include <Transaction.h>
#include <dbSchemaMetaDataProvider.h>
#include <ImplicitCirculationConfigurer.h>

#include <MCFilterables.h>

#include<boost/utility.hpp> // PRQA S 1013 2
#include<boost/bind.hpp>

#include <cstdlib>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;
using namespace std;
using namespace boost;
using namespace acs::mc;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CirculationConsumer);

/**
 * Ctor with number of consumers.
 */
CirculationConsumer::CirculationConsumer( dbConnectionPool &connectionPool, unsigned int numberOfConsumers) :
    Thread("CirculationConsumer"), // PRQA S 4052
    _manager(), 
    _dataConsumerChannel(*this, "Circulation Curl Channel", numberOfConsumers),  // PRQA S 3000
    _nConsumer(numberOfConsumers), 
    _group(), 
    _isAvailable(true), 
    _mutex(), 
    _localhost(dbConnectionWrapper(connectionPool)),  // PRQA S 3050, 3081
    _connectionPool(connectionPool),    // PRQA S 2528 
    _waitTime(true), _lastTimeFillInCirculations("01-Jan-1970 00:00:00"),
    _errorMessage(),
    _logconv(),
	_hostIdNullable (true),
    _slotResource(),
	_interruptedCirculations()
{
    //empty.
}

CirculationConsumer::~CirculationConsumer() throw () {
    //empty.
}

unsigned int CirculationConsumer::maxNumberOfConsumers() const { // PRQA S 4120
    return _nConsumer;
}

void CirculationConsumer::init() {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "finding the localhost in the database...");
    dbConnectionWrapper cw(_connectionPool) ;
    dbConnection &conn = cw ; // PRQA S 3050
    KnownHost * lh = _manager.findLocalhost(conn) ;
    ACS_COND_THROW(lh == 0, exIllegalValueException("No local host in t_knownhost")) ; // PRQA S 3081
    _localhost = *lh ;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "found: " << _localhost.str(conn));
}

/**
 * Returns the number of items that are being consuming.
 */
size_t CirculationConsumer::sizeOfConsuming() const {
    return _dataConsumerChannel.sizeOfConsuming();
}

void CirculationConsumer::getConsumingItems(vector<CirculationItem*> &items) {
	_dataConsumerChannel.getConsumingItems(items);
}


void CirculationConsumer::discardFinishedItems() {
    // retrieves all finished items from the consumer channel.
    size_t sizeOfFinished = _dataConsumerChannel.sizeOfFinished();
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Size of Finished:  " << sizeOfFinished);
    for (size_t counter = 0; counter < sizeOfFinished; ++counter) {
        try {
            // removes from the channel the finished items.
            CirculationItem item = _dataConsumerChannel.popOfFinished();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Finished:  " << item.id());
        } catch (std::exception const& ex) {
            //ignore any error.
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex);
        }
    }
}

void CirculationConsumer::getFinishedItems(vector<CirculationItem>& distributedPackages,
        vector<CirculationItem>& failedPackages) {
    // retrieves all finished items from the consumer channel.
    size_t sizeOfFinished = _dataConsumerChannel.sizeOfFinished();
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Size of Finished:  " << sizeOfFinished);
    for (size_t counter = 0; counter < sizeOfFinished; ++counter) {
        try {
            // removes from the channel the finished items.
            CirculationItem item = _dataConsumerChannel.popOfFinished();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Finished:  " << item.id());
            if (0 != item.lastErrorCode()) {
                // adds to the failed items vector.
                failedPackages.push_back(item);
            } else {
                // adds to the completed items vector.
                distributedPackages.push_back(item);
            }
        } catch (std::exception const& ex) {
            //ignore any error.
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex);
        }
    }
}

void CirculationConsumer::verifyConfigurationFile() {
    try {
        //Verify if the configuration file is present and is well formed.
        //this method creates the unique instance of the Configurer class
        //that reads the configuration file and fills in all the values.
        ACS_LOG_PRIVINFO("Verifying configuration files...");
        acs::conf::CirculationConfigurerSingleton::instance();
		ACS_LOG_PRIVINFO(acs::conf::CirculationConfigurerSingleton::instance()->spaceName << " file verified");
        acs::conf::CurlConfigurerSingleton::instance();
		ACS_LOG_PRIVINFO(acs::conf::CurlConfigurerSingleton::instance()->CURL_CONFIGURER_SPACE_NAME << " file verified");
        acs::conf::dcConfigurerSingleton::instance();
		ACS_LOG_PRIVINFO(acs::conf::dcConfigurerSingleton::instance()->spaceName << " file verified");
        ACS_LOG_PRIVINFO("...configuration files verified.");
    } catch (std::exception const& ex) {
		ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_ERROR("Error in configuration file.");
        throw;
    }
}

void CirculationConsumer::addCirculationFromRow(Circulation& circ) 
{
	string circId ;
	try {
		using acs::curl::RemoteSiteFactory ;
		dbConnectionWrapper cw(_connectionPool) ;
        dbConnection &conn = cw ; // PRQA S 3050
		const string &dbKey = conn.getKey() ;
		bool null = false ;
		string fromUrl = circ.getFromURL(null, conn);
		{
			ostringstream os;
			os << circ.getId();
			circId = os.str();
		}
		ostringstream os;
		os << "Circulation id# " << circId << " of " << File::getFileName(fromUrl);
/* already logged when ELIGIBLE 
		try {
			fromUrl = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(fromUrl, true), false); // remove the password from url
		}
		catch( std::exception &e )
		{
			ACS_LOG_ERROR("Wrong fromUrl format: \"" << fromUrl << '\"');
		}
    	os << " FROM " << fromUrl;
	
		string toUrl = circ.getToURL(null, conn);
		try {
			toUrl = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(toUrl, true), false); // remove the password from url
		}
		catch( std::exception &e )
		{
			ACS_LOG_ERROR("Wrong toUrl format: \"" << toUrl << '\"');
		}
    	os << " TO " << toUrl;

    	if (not circ.nullStartDate(conn)) {
        	os << ", StartDate " << circ.getStartDate(null, conn).YYYYMMDDThhmmss();
    	}
		if( not circ.nullCirculationPolicy(conn)) {
		    os << ", policy id# " << circ.getCirculationPolicy(null, conn);
		}
*/
		os << ", " << Status::instance(dbKey)->name(circ.getStatus(null, conn));
		
    	ACS_LOG_INFO(os.str()); // PRQA S 3050, 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got circulation item from db queue: " << circ.str(conn, true)); // PRQA S 3050, 3081
	}
	catch( std::exception &e )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, e.what());
		if(circId.empty() && circ.isFromDatabase()) { 
			ostringstream os;
			os << circ.getId();
			circId = os.str();
		}
		ACS_LOG_WARNING( "Cannot issue the message related to start processing circulation item " << ( circId.empty() ? "UNKNOWN" : circId) ); // PRQA S 3380, 3385
	}

	try {
    	CirculationItem item ;
    	item.reset(new ConsumableCirculation(*this, _connectionPool, circ));
    	_dataConsumerChannel.push(item);
	}
	catch( std::exception &e )
	{
		ACS_LOG_NOTIFY_EX(e);
		if(circId.empty() && circ.isFromDatabase() ) { 
			ostringstream os;
			os << circ.getId();
			circId = os.str();
		}
		ACS_LOG_ERROR("Cannot start processing of circulation item " << ( circId.empty() ? "UNKNOWN" : circId) ); // PRQA S 3380, 3385
		_interruptedCirculations.push_back(circ);
	}

}

void CirculationConsumer::initSlotResource()
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    dbConnectionWrapper cw(_connectionPool) ;
    dbConnection &conn = cw ; // PRQA S 3050
    const string &dbKey = conn.getKey() ;
	ostringstream querystream;
	querystream << " select kl.dest_host_id, kl.max_upload_slots, count (cq.id) as progress_count from t_knownhosts_limits as kl left join t_circulationsqueue as cq on ( cq.dest_host_id = kl.dest_host_id and "
	 << " cq.status_id in ( " << Status::instance(dbKey)->id(Status::transferring) << "," << Status::instance(dbKey)->id(Status::processing) << ") ) "
	 << " where kl.circulator_node_id = " << _localhost.getId() << " group by kl.dest_host_id, kl.max_upload_slots ;";

	SQLString sqlstring(dbKey, querystream.str());
	dbQuery getQuery(conn, sqlstring, "SEL Circ possibile hosts with limits");
	dbSet availableSet;
    getQuery.execForSet(availableSet);
    vector<dbRow> const& rows = availableSet.set(); // dest_host_id, max_upload_slots, active.progress_count
	
	if (!_slotResource.empty())
	{
		_slotResource.clear();
	}
	for (size_t k = 0; k < rows.size(); ++k)
	{
		int dest_id(0);
		int max_slots(0);
		int current_slots(0);
		bool isSet(false);
		rows.at(k).columnSet(0, dest_id, isSet);  
		if (isSet)
		{
			rows.at(k).columnSet<int>(1, max_slots, isSet);
			rows.at(k).columnSet<int>(2, current_slots, isSet); // PRQA S 4400

			_slotResource[dest_id]= make_pair(max_slots, current_slots);
		}
	}
}


void CirculationConsumer::findCirculationsToConsume(vector<Circulation> & circs) {	//PRQA S 5500, 5510
    
    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB,"Finding circulations to consume....");
    // set a data structure containing for each dest_host_id the number of max slots and the number of active slots
    initSlotResource();
    const uint consumersLimit (50);
    uint numberOfConsumers = min(2*maxNumberOfConsumers(), 2*consumersLimit);	//PRQA S 3084
    static bool emitMsg(true);
    if ((maxNumberOfConsumers() > consumersLimit) && emitMsg)
    {
    	ACS_LOG_INFO("Max number of consumers configured " << maxNumberOfConsumers()<< ". Only " <<  numberOfConsumers << " queries at time shall be performed. Please consider to review the application flow." );
	emitMsg = false;
    }		
    while (true) {
        const string &dbKey = _connectionPool.getKey() ;
        //extract the list of all servers that are still transferring a big amount of data
        //and thus cannot be select as destination hosts.
        vector < string > disabledIds;
        {
		// loop on _slotResource
		std::map<int, std::pair<int, int> >::iterator it = _slotResource.begin();
		while(it != _slotResource.end())
		{
			if ((it->second.second - it->second.first) >=0)
			{
				ostringstream os;
				os << it->first;
				disabledIds.push_back(os.str());
				ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB,"Destination host with id <" << it->first << "> no nore available.");
			}
			++it;
		}
	
        }
        string disabledHostsWhereClause;
        if (!disabledIds.empty()) {
            ostringstream os;
            string disabledHosts = StringUtils::join(disabledIds, ",");
            os << " ( dest_host_id not in (" << disabledHosts << ") OR dest_host_id is NULL) AND ";
            disabledHostsWhereClause = os.str();
	    
        }
        discardFinishedItems();
        short eligible = Status::instance(dbKey)->id(Status::eligible);
        short recoverableError = Status::instance(dbKey)->id(Status::recoverableError);
		const short blocked = Status::instance(dbKey)->id(Status::blocked);
        const string dialectName = SQLDialectFactory::dialectName(dbKey);
        size_t tim_weight = conf::CirculationConfigurerSingleton::instance()->getCirculatorCreationTimeWeight() ;
        
        // use the reciprocal of the priority in case the circulation service is configured
        // to reverse the priority field sorting criteria
        
        string priorityField = " ( 1 / (priority + 1) )" ;
        if ( acs::conf::CirculationConfigurerSingleton::instance()->getCirculatorPriorityPlainOrder()) {
            priorityField  = " ( priority + 1) "  ;
        }

		SQLString sqlstring(dbKey, SQLString::QUERY_UNDEFINED);

	    if (dialectName == "Informix9") {
		
        	dbConnectionWrapper cw(_connectionPool) ;
        	dbConnection &conn = cw ; // PRQA S 3050
		
			sqlstring.setType(SQLString::COMMAND) ;

        	stringstream query;
			query << "select first " << numberOfConsumers << " ((CURRENT - creationdate)/"
    			  << tim_weight << "+1 units second) * " << priorityField << " as SORTER,"
    			  << " id as ID , dest_host_id as dest_id from t_circulationsqueue where host_id = " << _localhost.getId() << " AND " << disabledHostsWhereClause
    			  << " ( relay_host_id is null and ( status_id = " << eligible << " or status_id = " << blocked << " or  (   status_id= "<< recoverableError << " and  (nextretrydate <= CURRENT) ) )";
			if (_manager.checkHasProtocolIdField(conn))
			{
				query << " and (T_Circulationsqueue.dest_protocol_id IN ( SELECT  id FROM  t_transferprotocols   WHERE  (t_transferprotocols.protocol <> \"MEDIA\"))) ";
			}
			query << " ) order by SORTER desc";
			sqlstring.forceSQLString(query.str());

        }
		else {
			sqlstring.setType(SQLString::SELECT) ;

// 				SELECT * FROM 
// 				( 
// 				SELECT  ((f_interval_to_seconds_diff( creationdate, LOCALTIMESTAMP ))/10 + 1) *  ( priority + 1) AS SORTER, 
// 				cq.id AS ID, cq.dest_host_id AS dest_id 
// 				FROM  t_circulationsqueue cq  
// 				INNER JOIN t_transferprotocols tp ON  (( (tp.id = cq.dest_protocol_id) ) AND (  (tp.protocol <> 'MEDIA') ))  
// 				WHERE  (( (cq.host_id = 10) ) OR (  (cq.host_id IS NULL ) )) 
// 				AND  (( (cq.relay_host_id IS NULL ) )) 
// 				AND  (( (( (cq.status_id = 7) )) ) OR  cq.status_id = 41  ((( (cq.status_id = 15) ) AND (  (cq.nextretrydate <= 'LOCALTIMESTAMP') )) ))  
// 				ORDER BY  SORTER DESC  
// 				) 
// 				WHERE ( ROWNUM <= 20 )

			string now;
			if( dialectName == "Postgres8") {
				now = "now()";
			}
			else if (dialectName == "Oracle11") {
				now = "LOCALTIMESTAMP";
			}
			else {
	    		ACS_THROW( exIllegalValueException("Database dialect not supported: " + dialectName) ); // PRQA S 3081
			}

			string sorter;
			{
				ostringstream os;
				os << '(' << SQLString::intervalToSeconds(dbKey, "creationdate", now) << '/' << tim_weight << " + 1) * " << priorityField;
				sorter = os.str();
			}
			sqlstring.addSelectList( sorter, "SORTER" );
			sqlstring.addSelectList( "cq.id", "ID" );
			sqlstring.addSelectList( "cq.dest_host_id", "dest_id" );
  			sqlstring.addTableList("t_circulationsqueue", "cq");

        	dbConnectionWrapper cw(_connectionPool) ;
        	dbConnection &conn = cw ; // PRQA S 3050

			// Add Join
			if( _manager.checkHasProtocolIdField(conn) ) {
   				dbQueryParameters pp( dbKey);
 				pp.addParameter(dbParam( dbKey, "tp","id" , dbParam::EQUAL_TO, "cq.dest_protocol_id")) ;
				pp.addParameter(dbParam(dbKey, "tp" ,"protocol" , dbParam::DIFFERENT_FROM , db::toSQL(dbKey, "MEDIA") )) ;
				sqlstring.addJoinList(SQLDialect::INNER,"t_transferprotocols tp",pp) ;
			}

			// Add Where
			{
    			dbQueryParameters pp(dbKey) ;
				pp.addParameter( dbParam(dbKey, "cq" ,"host_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _localhost.getId())) ) ;
				if (_hostIdNullable) {
					// Add a clause for the null host_id (old schema back compatibility)
					pp.setParamOperator( dbQueryParameters::OR ) ;
					pp.addParameter( dbParam(dbKey, "cq" ,"host_id" , dbParam::ISNULL) ) ;
				}
				sqlstring.addANDWhereParameter(pp) ;
			}

        	if ( not disabledIds.empty() ) {

				dbQueryParameters pp(dbKey) ;
				pp.setParamOperator( dbQueryParameters::OR ) ;
				pp.addParameter( dbParam(dbKey, "cq" ,"dest_host_id" , dbParam::NOT_IN, disabledIds) );
				pp.addParameter( dbParam(dbKey, "cq" ,"dest_host_id" , dbParam::ISNULL) ) ;
				sqlstring.addANDWhereParameter(pp) ;
        	}

			{
    			dbQueryParameters pp(dbKey) ;
				pp.addParameter( dbParam(dbKey, "cq" ,"relay_host_id" , dbParam::ISNULL) ) ;
				sqlstring.addANDWhereParameter(pp) ;
			}

			{
    			dbQueryParameters pp1(dbKey) ;
				pp1.addParameter( dbParam(dbKey, "cq" ,"status_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, eligible)) ) ;

				dbQueryParameters pp2(dbKey) ;
				pp2.addParameter( dbParam(dbKey, "cq" ,"status_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, recoverableError)) ) ;
				pp2.addParameter( dbParam(dbKey, "cq" ,"nextretrydate" , dbParam::MINOR_OR_EQUAL_OF , db::toSQL(dbKey, now)) ) ;

    			dbQueryParameters pp3(dbKey) ;
				pp3.addParameter( dbParam(dbKey, "cq" ,"status_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, blocked)) ) ;

				dbQueryParameters pp(dbKey) ;
				pp.setParamOperator( dbQueryParameters::OR ) ;
				pp.addQueryParameter(pp1) ;
				pp.addQueryParameter(pp3) ;
				pp.addQueryParameter(pp2) ;
				sqlstring.addANDWhereParameter(pp) ;
			}	

			sqlstring.addOrderByList("SORTER", SQLString::DESC);
			sqlstring.setLimit(numberOfConsumers) ;

		}
        

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << sqlstring.getSQL() );
        dbSet set;
		
		{
        	dbConnectionWrapper cw(_connectionPool) ;
        	dbConnection &conn = cw ; // PRQA S 3050
        	dbQuery getQuery(conn, sqlstring,"SEL Circulation Queue");
        	getQuery.execForSet(set);
		}
        if (set.empty()) {
            break;
        }
       	vector<dbRow> const& rows = set.set();
	// loop on Circulation Queue, if some consumers are available
        for (size_t k = 0 ; (k < rows.size()) && ( ( int(maxNumberOfConsumers()) - int(sizeOfConsuming()) - int(circs.size()) ) > 0 ); ++k)	// PRQA S 3081, 4238, 4244
	    {
		    dbPersistent::IdType circId = 0 ;
        	double currentPriority = 0;
        	if (dialectName == "Postgres8" || dialectName == "Oracle11" )
        	{
        	    rows[k].column(0, currentPriority);
        	}
        	else {
// MarFav - Dymanic priority cannot be extracted and notified if database is not Postgres8 or Oracle
        	    ACS_LOG_WARNING("no current priority for: " << dialectName) ;
        	}
        	rows[k].column(1, circId);
            int destId (0);
            bool destHostNull(true);
            rows[k].column(2, destId, &destHostNull);// PRQA S 4400
            bool isEnabled = true;

            if(!destHostNull && (_slotResource.find(destId) != _slotResource.end()))
            {
	            isEnabled = ((_slotResource[destId].first - _slotResource[destId].second) > 0);
            }

			if ( isEnabled) // slots and consumers available
			{
				dbConnectionWrapper cw(_connectionPool) ;
				dbConnection &conn = cw ; // PRQA S 3050
				
				Circulation circ(conn);
				bool found = circ.load(conn, circId) ;
				if (not found) { // should never occur by construction ...
					ostringstream os ; os << "Cannot load Circulation id: " << circId ;
					ACS_THROW(Circulation::ExecutionError(os.str())) ; // PRQA S 3081
				}
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Got circulation item from db queue: " << circ.str(conn, true));
				// in case the circulation is currently under processing shall skip it without touching the database status
				bool isConsumerManagingThisItem = false;
				// Verify if the data consumer channel is already managing the item
				// this is needed due to a race condition found in http://projectit.acsys.it/browse/S3MARINE-471
				{
					CirculationItem item ;
					item.reset(new ConsumableCirculation(*this, _connectionPool, circ));
					isConsumerManagingThisItem = _dataConsumerChannel.isManagingItem (item);
				}
				if ( false == isConsumerManagingThisItem )
				{
					bool null = false;
					//S2PDGS-727 notifying the db priority and dynamic priority
					if( (dialectName == "Postgres8" || dialectName == "Oracle11" ) && ( circ.getStatus(null, conn) != blocked ) )
					{
						ostringstream os;
						os << "Circulation id# " << circId << " (priority " << circ.getPriority(null,conn) << " - Dynamic priority " << currentPriority << ") ";
						if (! destHostNull)
						{
							os  << " on hostId# " <<  destId;
						}
						os << ": PROCESSING";
						ACS_LOG_PRIVINFO (os.str());
					}
					
					try {
						_manager.startWorkingOnCirculation(conn, circ);
						ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Circulation item id# " << circId << " status has just been set to " << Status::instance(dbKey)->name(circ.getStatus(null, conn)) ); 
						// APF-273
						ACS_LOG_MC_INFO( mc_param("filename", File::getFileName (circ.getFromURL(null, conn)) ) << 
										 mc_param("queueid", circId ) <<
										 mc_param("status", "QUEUE_OUT" ) <<
										 mc_metric(Metric_RUNNING)) ;
						circs.push_back(circ);
						
						// update _slotResource, increasing the number of progress count
						if(!destHostNull && (_slotResource.find(destId) != _slotResource.end()))
						{
							_slotResource[destId].second++;
						}
					}
					catch(std::exception &) 
					{}
				}
		}
		
	}
	if ( ( int(maxNumberOfConsumers()) - int(sizeOfConsuming()) - int(circs.size()) ) < 1)	// PRQA S 3084, 3081
	{
		ACS_LOG_INFO("Consumers no more available...used " << sizeOfConsuming() + circs.size()<< " of " << maxNumberOfConsumers());	// PRQA S 3084
		break;
	}
    }
}

bool CirculationConsumer::fillInCirculations() { // PRQA S 4020

    try {

        if ( (_dataConsumerChannel.sizeOfFinished() != 0) && _isAvailable )
        {
                ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "A worker has completed its work. Forcing a queue reload simultaing a broadcast event.");
                _waitTime = false;
        }

        if (_waitTime && // PRQA S 3230
            !_manager.isOlderThan(conf::CirculationConfigurerSingleton::instance()->getCirculatorReprocessInterval() / 2, // PRQA S 4400
                                               _lastTimeFillInCirculations)) {
            return true ;
        }
        if ( !_waitTime ) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "DB Broadcast signal received");
            _waitTime=true;
        }
        //if this main thread is still available and if the number of items have been consuming is
        //less than the number of consumeres, try to read new records from the db.
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "consuming : " << sizeOfConsuming() << " consumers: " << maxNumberOfConsumers());
        if (_logconv.isWorkingConsumerTime()) { 
            ACS_LOG_PRIVINFO("Parallel transfers (working consumer(s)) #" << sizeOfConsuming() << " of " << maxNumberOfConsumers()) ; 
        }
        int numberOfFreeConsumers = int(maxNumberOfConsumers()) - int(sizeOfConsuming()); // PRQA S 3084, 3081
        if (_isAvailable && (numberOfFreeConsumers != 0)) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "number of free consumers: " << numberOfFreeConsumers);
            discardFinishedItems();
			fillInterruptedCirculationsFromChannel();
			// put in error interrupted circulations
			manageInterruptedCirculations();
			
            vector<Circulation> rows;
            findCirculationsToConsume(rows);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Found : " << rows.size() << " records.");
    #ifndef PRQA_ANALYZER
    // problema del tool di analisi nel leggere codice boost
            for_each(rows.begin(), rows.end(), bind(&CirculationConsumer::addCirculationFromRow, boost::ref(*this), _1));
    #endif
            Application::setStatus(Application::Ok) ;
        }
        return true ;
    } catch(pc::exStopChannelException &x) {
		ACS_LOG_WARNING("Stop requested: no new circulations will be accepted.");
        return false ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        Application::setStatus(Application::Critical, "errors in finding circulations") ;
        return false ;
    }
}


dbPersistent::IdType CirculationConsumer::getDbParamId (dbConnection& conn, const string& param) const
{
    string dbKey = conn.getKey();
    dbPersistent::IdType retVal = 0;

    SQLString selectSQL( dbKey, SQLString::SELECT ) ;
    selectSQL.addTableList("t_dbparameters");
    selectSQL.addSelectList("id");

    dbQueryParameters pp (dbKey);
    pp.addParameter(dbParam(dbKey, "t_dbparameters", "parameter" , dbParam::EQUAL_TO , db::toSQL(dbKey, param)));
    selectSQL.addORWhereParameter(pp) ;

    try
    {
        dbQuery query(conn, selectSQL, "SEL dbp." + param + "_id" ) ;

		dbSet set ;
		query.execForSet(set) ;
        
		dbRow myRow(dbKey) ;  
		if( set.getNextRow(myRow) ) {
        	// Got the ID
        	myRow.column(0,retVal);
		}
    }
    catch (std::exception& ex)
    {
        // Not an issue. The record does not exists and will be created later
    }

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "t_dbparameters.id of " <<  param << " is " << retVal);

    return retVal;
}



void CirculationConsumer::updateDbMaxRetryConfiguration()
{
    // Get the configuration settings
    acs::conf::ImplicitCirculationConfigurer::RetryConfiguration retconf;

    // Implicit
    retconf = acs::conf::CirculationConfigurerSingleton::instance()->getImplicitRetryConfiguration();
    unsigned int implicitmax = retconf.maxRetries;

    // Explicit
    retconf = acs::conf::CirculationConfigurerSingleton::instance()->getExplicitRetryConfiguration();
    unsigned int explicitmax = retconf.maxRetries;

    // Dynamic
    retconf = acs::conf::CirculationConfigurerSingleton::instance()->getDynamicRetryConfiguration();
    unsigned int dynamicmax = retconf.maxRetries;

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Going to update max retries settings (Implicit, Explicit, Dynamic) to (" << implicitmax << ", "
        << explicitmax << ", " << dynamicmax << ")");

    // Do the update in a single transaction
    dbConnectionWrapper cw(_connectionPool) ;
    dbConnection &conn = cw ; // PRQA S 3050

    // Try to gather ids of old parameters (if any)
    dbPersistent::IdType implId = getDbParamId (conn, "circ_impl_max_ret"); // PRQA S 4412 3
    dbPersistent::IdType explId = getDbParamId (conn, "circ_expl_max_ret");
    dbPersistent::IdType dynId = getDbParamId (conn, "circ_dyn_max_ret");

    db::Transaction transaction(conn) ;
    transaction.begin_work("DbParms max retry update");

    // Then insert new values
    setDbParamId (conn, implId, "circ_impl_max_ret", implicitmax);
    setDbParamId (conn, explId, "circ_expl_max_ret", explicitmax);
    setDbParamId (conn, dynId, "circ_dyn_max_ret", dynamicmax);

    transaction.commit();

}

void CirculationConsumer::setDbParamId (dbConnection& conn, const dbPersistent::IdType& id, const string& param, unsigned int value) const // PRQA S 4214
{
    dbSmartPersistent therecord ("t_dbparameters", conn);
    if (id != 0)
    {
        // Load from the database
        therecord.setId (id);
        therecord.load(conn);
    }
 
    // Update the value
    therecord.set( "parameter", param, conn );
    therecord.set( "parametertype_id", 1, conn );
    therecord.set( "value_int", value, conn );
    
    if (id != 0)
    {
        // Update the existing one
        therecord.update(conn);
    }   
    else
    {
        // create the new record
        therecord.save(conn);
    }
}

void CirculationConsumer::checkCirculationsQueueFields()
{
    // Use the metadata schema provider to detect if the t_circulationsqueue host_id is nullable 
    dbConnectionWrapper cw(_connectionPool) ;
    dbConnection &conn = cw ; // PRQA S 3050
    const string &dbKey = conn.getKey() ;

	_hostIdNullable = not dbSchemaMetaDataProvider::instance(dbKey) -> notNull ("t_circulationsqueue", "host_id", conn);

}

void CirculationConsumer::run() {	// PRQA S 5500
    try {
        verifyConfigurationFile();
    } catch (std::exception const& ex) {
        //TODO: the configuration file is absent: error ignored (for now)
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Error in reading configuration file...ignore it (for now).");
    }

    try 
    {
        updateDbMaxRetryConfiguration ();
    } 
    catch (const std::exception& x) 
    {
        // Notify and continue with defaults
        ACS_LOG_NOTIFY_EX (x);
        ACS_LOG_WARNING ("Unable to update max retry parameters in the database. Defaults/previous settings apply.");
    }
    
    // Check the database schema features
    checkCirculationsQueueFields();

    vector < boost::shared_ptr<Thread> > handlerConsumers;
    try {
        for (int i = 0; not requestedCancel() || !_isAvailable ; i++) {
            try {
                init(); // try to keep application alive
                break ;
            }
            catch(std::exception &x) {
				if( _maxInitAttempts == i ) {
					throw;
				}
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, x);
                Timer::delay(227) ; // PRQA S 4400
                continue ;
            }
        }
        ACS_LOG_INFO("Circulation loop starting");
        ACS_LOG_INFO("Retrieving all interrupted Circulations and putting them in error");
		// interrupted circulations are ins transferring/processing status
        unsigned long cleanedUp = _manager.cleanupRecords(dbConnectionWrapper(_connectionPool)); // PRQA S 3050, 3081
        if (cleanedUp)
        {
            ACS_LOG_INFO("Cleanup of " << cleanedUp << " record(s) executed.");
        }
        if (0 == _nConsumer) {
            _nConsumer = conf::dcConfigurerSingleton::instance()->getNumberOfParallelConsumers();// PRQA S 3010
        }
        ACS_LOG_INFO("Number of parallel consumers: " << _nConsumer);
        {
            Lock lock(_mutex) ;
            //starts up all the consumers
            for (size_t counter = 0; counter < _nConsumer; ++counter) {
                acs::Thread* consumer = new SyncCirculationConsumer(_dataConsumerChannel, *this);
                handlerConsumers.push_back(boost::shared_ptr < Thread > (consumer)); // PRQA S 3081
                _group.push_back(consumer);
            }
            _dataConsumerChannel.start();
            Thread::start(_group.begin(), _group.end()) ;
        }
        
        ACS_LOG_INFO("Entering main work loop");
        while (true) {
            //extracts all circulation items from the database.
            fillInCirculations();
            size_t consumerSize = size();
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Consumer is available: " << boolalpha << _isAvailable);
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Consumer size     : " << consumerSize);
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, threadsAlive() << " alive threads");
            
            if (haltImmediate()) { //TERM immediately: don't care about the consumers.
                ACS_LOG_INFO("Main work loop: immediate termination requested.");
                break;
            }
            if (!_isAvailable && (0 == consumerSize)) { // STOP gracefully (the queue must be empty).
                ACS_LOG_INFO("Main work loop: graceful termination requested and no more records to consume.");
                break;
            }
            Thread::yield(); //breath.
            Timer::delay(200); // 200 ms // PRQA S 4400
        }
        //terminates all consumers and the register thread.
        terminateAll();
        ACS_LOG_INFO("Join all consumer threads...");
        {
            Lock lock(_mutex) ;
            //waits until all threads have been stopped.
            Thread::wait_stop(_group.begin(), _group.end());
            _group.clear();
        }
        
        ACS_LOG_INFO("...consumer threads joined.");
    } catch (std::exception const& err) {
        ACS_LOG_NOTIFY_EX(err);
        _errorMessage = err.what();
        Lock lock(_mutex) ;
        _group.clear();
    }
}

void CirculationConsumer::joinAll(bool syncStart) {
    try {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Join All...");
        //waits for the consumer channel to stop.
        //only if syncStart is true (this is used when this method is called before the
        //main thread (this thread) has started the channel thread.
        while (syncStart && (!_dataConsumerChannel.running())) {
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Sync start.");
            Thread::yield();
            Timer::delay(7); // 7 ms // PRQA S 4400
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Join All...finishing main thread...");
        while (running()) {
            Thread::yield();
            waitExitOrTimeout(100); // 100 ms // PRQA S 4400
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Join All...waiting for last consumers to stop...");
        {
            Lock lock(_mutex) ;
            Thread::wait_stop(_group.begin(),_group.end()) ;
            _group.clear() ;
        }
    } catch (std::exception const& ex) {
        ACS_LOG_NOTIFY_EX(ex);
        Lock lock(_mutex) ;
        _group.clear();
    }
}

/**
 * Stops the consumer.
 */
void CirculationConsumer::stopImmediate() {
    stopGracefully();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TERM all immediate.\n");
    setHaltLevel(StopController::IMMEDIATE);
    _dataConsumerChannel.wakeUpAllAndStop();
    {
        Lock lock(_mutex) ;
        // S2PDGS-1128 do not cancel threads!!
        // set the halt level and join all of them gracefully
        // threads must honour the IMMEDIATE exit request
        Thread::halt(_group.begin(), _group.end(),StopController::IMMEDIATE) ;
        _group.clear() ;
    }
}

/**
 * Terminates the consumer.
 */
void CirculationConsumer::stopGracefully() {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, 
            "Terminate signal received: all consumers will complete last incomplete packages transfers and will stop. "
                    << "\n They will not receive any more packages but will complete the old ones.");
    setIsAvailable(false);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The current consumer is not available: " << _isAvailable);
    acs::conf::dcConfigurerSingleton::instance()->setMaxNumberOfRetries(0); // does not retry errored items.
    acs::conf::dcConfigurerSingleton::instance()->setMaxDbRetries(0); // does not retry errored items.
}

/**
 * Alias for stopGracefully() : from outside "stop" is intended as "do not accept
 * more packages but terminate all the remaining queue".
 *
 */
void CirculationConsumer::stop(bool joinAlso) {
    stopGracefully();
}

size_t CirculationConsumer::threadsAlive() const {
    Lock lock(_mutex) ;
    return Thread::run_count(_group.begin(), _group.end()) ;
}

/**
 * Returns the number of items that must be consumed.
 */
size_t CirculationConsumer::size() // PRQA S 4211
{
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Items to transfer: " << _dataConsumerChannel.size());
    return _dataConsumerChannel.size();
}

void CirculationConsumer::terminateAll() {
    //set to terminate status the stopper.
    setHaltLevel(StopController::LAZY);
    //wake up and terminate all waiting consumers for this channel.
    _dataConsumerChannel.wakeUpAllAndTerminate();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Terminate All.");
    //waits untill this channel is still in running.
    if (_dataConsumerChannel.running()) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Join consumers...");
        while (_dataConsumerChannel.running()) {
            Thread::yield();
            _dataConsumerChannel.waitExitOrTimeout(100) ; // PRQA S 4400
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "...consumers joined.");
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "All channels closed.");
}

void CirculationConsumer::setIsAvailable(bool available) throw() // PRQA S 4121
{
    _isAvailable = available;
}

bool CirculationConsumer::isAvailable() const throw() // PRQA S 4120
{
    return _isAvailable;
}

const string &CirculationConsumer::getLastErrorMessage() const throw() // PRQA S 4120
{
    return _errorMessage;
}

void CirculationConsumer::setLastErrorMessage(string const& msg) // PRQA S 4121
{
    _errorMessage =msg;
}

void CirculationConsumer::dbBroadcastSignalReceived() throw() 
{
    _waitTime=false;
}

void CirculationConsumer::manageInterruptedCirculations() // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	size_t intCircSize = _interruptedCirculations.size();
	if( intCircSize == 0 ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No interrupted circulations item occur" );
		return;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to put in ERROR/BLOCKED " << intCircSize << " interrupted circulations item (in PROCESSING/BLOCKING status)" );
	
	for( list<Circulation>::iterator it = _interruptedCirculations.begin(); it != _interruptedCirculations.end(); ) { // PRQA S 4238
		Circulation circ = *it;
		string circId("UNKNOWN");
		try {
			ostringstream os;
			os << circ.getId();
			circId = os.str();
		}
		catch( std::exception & )
		{}

		string finalStatus = "ERROR"; // just for messages
		try {
        	dbConnectionWrapper cw(_connectionPool) ;
        	dbConnection &conn = cw ; // PRQA S 3050
			
			bool null = false ;
			if( circ.getStatus(null, conn) == Status::instance(conn.getKey())->id(Status::blocking ) ) { 
        		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to reset circulation item " << circ.str(conn, true) << " to BLOCKED");
				_manager.updateCirculationStatus(conn, Status::blocked, circ);
				finalStatus = "BLOCKED";
			}
			else {
        		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to set circulation item " << circ.str(conn, true) << " to ERROR");
				bool emitExceptionAfterUpdate = false;
				_manager.updateErrorStatus(conn, circ, true, 0, emitExceptionAfterUpdate);
			}
			ACS_LOG_INFO("Not finished Circulation id# " << circId << " just updated to " << finalStatus << " status" );
			it = _interruptedCirculations.erase(it);
		}
		catch( std::exception &e )
		{
			ACS_LOG_WARNING("Cannot update Circulation id# " << circId << " status to " << finalStatus << ": " << e.what());
			++it;
		}

	}

}

void CirculationConsumer::fillInterruptedCirculationsFromChannel()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    size_t sizeOfNotAbleToConsume = _dataConsumerChannel.sizeOfNotAbleToConsume();
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Size of not able to consume items: " << sizeOfNotAbleToConsume);
    for (size_t counter = 0; counter < sizeOfNotAbleToConsume; ++counter) {
		CirculationItem item ;
        try {
            // removes from the channel the item.
            item = _dataConsumerChannel.popOfNotAbleToConsume();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Not able to consume: " << item.id());
		}
        catch (std::exception const& ex) {
            //ignore any error.
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex);
			continue;
        }
		
		try {
			dbConnectionWrapper w(_connectionPool) ;
    		dbConnection &conn = w ;// PRQA S 3050 
    		Circulation circ(conn) ;
			(item.pItem())->getItemToConsume(circ);
			_interruptedCirculations.push_back(circ);
			
        } catch (std::exception const& ex) {
			ACS_LOG_NOTIFY_EX(ex);
			ostringstream wmsg;
			wmsg << "Cannot retrieve Circulation object " << item.id() << " from CirculationItem, going to enqueue into the channel";
			try {
				// enqueue again into not able to consume vector
				_dataConsumerChannel.enqueueOfNotAbleToConsume(item);
			}
			catch (std::exception const& ex2) {
				wmsg << "...Cannot even enqueue: " << ex2.what() << ". Such an item WILL BE PROCESSED AFTER RE-START!";
			}
			ACS_LOG_WARNING(wmsg.str());
        }
    }


}

_ACS_END_NESTED_NAMESPACE;

