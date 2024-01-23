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
 *  the contents of this file may not be disclosed to thl1261ird parties, copied or
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
 *  Revision 7.13  2018/07/30 09:38:55  lucio.pulvirenti
 *  S3EVOL-159: doWork: if circulation queue status is blocking, do not update status, do not verify target and do not transfer item with curl.
 *  postTransferOperations: if circulation queue status is blocking, call the new method moveIntoBlockedCirculationsQueue to move record into t_blockedcirculationsqueue
 *  table.
 *
 *  Revision 7.12  2018/06/21 12:46:14  damdec
 *  get/insert methods renamed.
 *
 *  Revision 7.11  2018/06/14 14:42:24  damdec
 *  PDSEV-24: implementation finalized.
 *
 *  Revision 7.10  2018/06/06 09:37:17  lucio.pulvirenti
 *  S2PDGS-2004: postTransferOperations: CirculationRemoveService::removeSourceAfterComplete method called even if removeSource flag is false
 *  for possible past concurrency with circulations with removeSource=true. No remove performed if circulation remove queue is empty
 *
 *  Revision 7.9  2017/11/15 13:55:48  chicas
 *  some message to PRIVINFO
 *
 *  Revision 7.8  2017/11/15 10:28:33  chicas
 *  some message to PRIVINFO
 *
 *  Revision 7.7  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.6  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.5  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.4  2017/02/06 09:11:22  lucio.pulvirenti
 *  S2PDGS-1656: id() method called by CTOR to valorize _objectId attribute. In operator() method connection opening (dbConnectionWrapper) in try block
 *  to manage related exceptions properly.
 *
 *  Revision 7.3  2017/02/03 15:49:11  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 7.2  2016/12/21 14:04:50  marfav
 *  Fixed regression on local transfer mode management
 *
 *  Revision 7.1  2016/12/20 08:52:55  marfav
 *  S2PDGS-176 Limiting use of database connections
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.1  2016/08/31 08:58:42  damdec
 *  PDSEV-24: Circulation status change info saved in a FIFO
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.51  2016/07/26 09:34:29  lucio.pulvirenti
 *  S1PDGS-31765: local transfer mode id got by circulation record and used CirculationCurl proper IF to set the proper value.
 *
 *  Revision 5.50  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.49  2016/03/22 16:32:25  francesco.avanzi
 *  S2PDGS-1434: the ConsumableCirculation now sets EarlyRemoval in FtpCurl
 *
 *  Revision 5.48  2016/03/21 18:26:31  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.47  2016/03/18 14:08:20  francesco.avanzi
 *  verifyAndRemove now in line with new signature
 *
 *  Revision 5.46  2015/11/26 14:29:21  marfav
 *  mc_params are now lowercase
 *
 *  Revision 5.45  2015/11/17 08:49:52  lucio.pulvirenti
 *  S2PDGS-1373: message improved: added circulation id to id() method return.
 *
 *  Revision 5.44  2015/11/16 16:33:21  lucio.pulvirenti
 *  S2PDGS-1373: _logExMsgAsError bool par added to IF with set/get method. In op() method check if proper flag is set to prevent log of recoverable errors
 *  as ERRORs: they will be logged ad INFO.
 *
 *  Revision 5.43  2015/08/21 14:57:52  marfav
 *  S2PDGS-1295 Error messages logging improved
 *
 *  Revision 5.42  2015/08/17 13:05:46  marfav
 *  APF-273 adding messages for OUT and FAIL metrics
 *
 *  Revision 5.41  2015/08/04 07:49:33  lucio.pulvirenti
 *  S2PDGS-1247: old logic based on media protocol removed.
 *
 *  Revision 5.40  2015/03/10 15:18:55  lucio.pulvirenti
 *  S2PDGS-984: UnrecoverableErrorException thrown in some cases. operator(): in catch block exception type is discriminated to pass the CirculationDBService::updateErrorStatus
 *  the right flag to set either ERROR or RECOVERABLEERROR.
 *
 *  Revision 5.39  2015/03/09 16:59:37  lucio.pulvirenti
 *  S2PDGS-984 work in progress.
 *
 *  Revision 5.38  2015/03/06 16:46:19  lucio.pulvirenti
 *  S2PDGS-984: wokr in progress.
 *
 *  Revision 5.37  2015/02/19 14:56:52  francesco.avanzi
 *  getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
 *
 *  Revision 5.36  2015/01/14 11:20:15  marfav
 *  S2PDGS-1039: robustness of id computation improved against database failures
 *
 *  Revision 5.35  2014/11/18 08:35:44  lucio.pulvirenti
 *  S2PDGS-915: verifyAndRemove: exception message changed.
 *
 *  Revision 5.34  2014/10/29 16:20:12  lucio.pulvirenti
 *  S2PDGS-955: Info added about proxy use.
 *
 *  Revision 5.33  2014/10/06 10:54:59  lucio.pulvirenti
 *  Message in exc fixed.
 *
 *  Revision 5.32  2014/09/03 15:46:40  lucio.pulvirenti
 *  S3PDGS-1980: Improved message issued when cannot remove the target if already existing. verifyAndRemove method IF changed: now void and throw exception
 *  if cannot work properly.
 *  Bug fixed: configuration parameter to overwrite target files no more checked in operator() method, but in verifyAndRemove only.
 *
 *  Revision 5.31  2014/09/02 13:19:28  lucio.pulvirenti
 *  S2PDGS-841: user password removed by messages. Message issued when circulation is over now improved.
 *
 *  Revision 5.30  2014/08/08 16:26:54  lucio.pulvirenti
 *  S2PDGS-841: work in progress.
 *
 *  Revision 5.29  2014/08/06 16:13:06  lucio.pulvirenti
 *  S2PDGS-835: Exceptions notificated by the proper macro not to send stack on syslog.
 *  verifyAndRemove: bug fixed. Target temporary file occurence check to remove it.
 *
 *  Revision 5.28  2014/07/30 14:36:39  lucio.pulvirenti
 *  S2PDGS-806: operator(): if haltImmediate detected in catch block, no more update status performed since it may cause crashes. At restart transferring records properly managed.
 *
 *  Revision 5.27  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.26  2014/06/24 10:58:44  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.25  2014/06/20 14:53:41  marpas
 *  adopting FtpCurl 5.47 interface
 *
 *  Revision 5.24  2014/06/18 13:53:23  lucio.pulvirenti
 *  s2PDGS-726: operator(): if exception due to immediate stop, a warning is issued instead of an error and base class StopException is thrown.
 *
 *  Revision 5.23  2014/06/17 09:58:37  lucio.pulvirenti
 *  S2PDGS-764: postTransferOperations: _localhost ptr is checked before invoking methods since it could be null if local host temporary not configured on db.
 *
 *  Revision 5.22  2014/06/05 18:48:09  marpas
 *  getting rid of asserts
 *
 *  Revision 5.21  2014/04/14 12:54:11  lucio.pulvirenti
 *  operator(): try-catch block into catch not to cause programme crash.
 *
 *  Revision 5.20  2014/04/11 17:21:28  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.19  2014/03/10 15:48:45  marfav
 *  Removed ACS_TABLES_FUSION_318 dependency and dead code
 *
 *  Revision 5.18  2014/02/21 16:34:04  lucio.pulvirenti
 *  No more derived by BandPriority.
 *
 *  Revision 5.17  2014/02/14 14:00:51  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.16  2014/02/07 18:53:36  marpas
 *  adopting ACS_THROW and ACS_COND_THROW macros
 *
 *  Revision 5.15  2014/02/06 10:37:10  lucio.pulvirenti
 *  S2PDGS-625: dbSchemaMetaDataProvider class used to detect occurrence of unkn_from_host and unkn_to_host columns. If not occurring, previous behavious is maintened:
 *  no circulation statistic record inserted if source/destination host not occurring in db.
 *
 *  Revision 5.14  2014/02/05 15:21:26  lucio.pulvirenti
 *  S2PDGS-625: postTransferOperations: if source/destination host not occurring in db, the host name is inserted into unkw_from_host/unkw_to_host column of t_circulationstats record
 *  going to be created.
 *
 *  Revision 5.13  2014/01/30 17:35:35  lucio.pulvirenti
 *  Message improved.
 *
 *  Revision 5.12  2014/01/28 11:02:04  lucio.pulvirenti
 *  S2PDGS-611 Messages and debug improved.
 *
 *  Revision 5.11  2014/01/27 16:15:09  lucio.pulvirenti
 *  Message and debug improved.
 *
 *  Revision 5.10  2013/11/21 14:49:59  marpas
 *  fixing debug variable and messages
 *
 *  Revision 5.9  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.8  2013/07/23 16:24:05  lucio.pulvirenti
 *  if ACS_TABLES_FUSION_318 defined, use TmpNameStrategyTable class instead of CirculationTempNames.h: S2PDGS-427
 *
 *  Revision 5.7  2013/07/22 13:19:18  marpas
 *  tyoes fixing
 *  coding best practices applied
 *  test fixed
 *
 *  Revision 5.6  2013/07/18 12:29:51  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.5  2013/07/16 14:52:16  marpas
 *  coding best practices applied,
 *  performances and robustness improved
 *
 *  Revision 5.4  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.3  2013/06/14 11:59:26  lucio.pulvirenti
 *  operator(): tempname_id from circulationqueue record now always taken. If got value is not valid (in circulationtempnames
 *  records) no strategy is set.
 *
 *  Revision 5.2  2013/06/12 11:49:09  lucio.pulvirenti
 *  operator(): temporary patch not to originate error on explicit/dynamic circulation: if t_circulationqueue record
 *  is not related to a t_circulationpolicy record, tempname_id and tempnamevalue field could be empty. In that
 *  case leave transfer strategy unknown as in previous version 5.0.
 *
 *  Revision 5.1  2013/06/11 15:06:27  lucio.pulvirenti
 *  According to db schema changes (jira issue S3PDGS-1358) tempname_id col has been added to t_circulationqueue
 *  so there is no need to find referenced t_circulationpolicy record. tempnamevalue string extracted from t_circulationqueue
 *  record too and passed sourceSite::setRenameStrategy method.
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.68  2013/04/22 11:00:25  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.67  2013/04/17 20:42:03  marpas
 *  propagation of interface change
 *
 *  Revision 1.66  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.65  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.64  2013/01/18 13:23:24  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.63  2013/01/18 11:44:01  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.62  2012/12/07 17:49:33  micmaz
 *  updated media checker with new libFile.
 *
 *  Revision 1.61  2012/12/04 17:43:34  marpas
 *  getting rid of FilenameUtils class
 *
 *  Revision 1.60  2012/12/03 16:59:39  marpas
 *  new StringUtils interface
 *
 *  Revision 1.59  2012/11/14 10:32:17  micmaz
 *  now when a circualation reach  the final status , it will be updated in one shot.
 *
 *  Revision 1.58  2012/10/25 10:17:47  micmaz
 *  The removeSource flag must be read when the circulation has been completed.
 *
 *  Revision 1.57  2012/05/16 17:15:47  micmaz
 *  IN PROGRESS - issue S1PDGS-3408: DC Regression on the use of the table T_KNOWNHOST by the CirculationAgent
 *  http://jira.acsys.it/browse/S1PDGS-3408
 *
 *  Revision 1.56  2012/04/04 15:31:36  micmaz
 *  work in progress
 *
 *  Revision 1.55  2012/04/03 17:45:48  micmaz
 *  work in progress
 *
 *  Revision 1.54  2012/04/03 10:11:06  micmaz
 *  work in progress
 *
 *  Revision 1.53  2012/03/30 15:15:32  micmaz
 *  work in progress
 *
 *  Revision 1.52  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.51  2012/03/29 13:32:35  micmaz
 *  work in progress
 *
 *  Revision 1.50  2012/03/27 18:07:27  micmaz
 *  work in progress
 *
 *  Revision 1.49  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.48  2012/03/09 17:43:34  micmaz
 *  added media functionality
 *
 *  Revision 1.47  2012/03/08 13:20:45  micmaz
 *  added media functionality
 *
 *  Revision 1.46  2012/02/08 16:22:49  micmaz
 *  probe() method moved to FtpCurl class.
 *
 *  Revision 1.45  2012/02/07 10:46:54  marpas
 *  warnings no longer forced
 *
 *  Revision 1.44  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.43  2012/02/02 17:19:57  micmaz
 *  IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 *  http://jira.acsys.it/browse/S1PDGS-2769
 *
 *  Revision 1.42  2012/01/27 20:44:52  micmaz
 *  work in progress..
 *
 *  Revision 1.41  2012/01/24 17:02:15  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 1.40  2012/01/20 15:59:29  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 1.39  2012/01/17 16:50:52  micmaz
 *  OPEN - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 1.38  2011/09/23 09:14:57  micmaz
 *  removed unused headers.
 *
 *  Revision 1.37  2011/09/19 15:43:26  micmaz
 *  OPEN - issue S1PDGS-1910: Wrong status changing in case of RECOVERABLE_ERROR
 *  http://jira.acsys.it/browse/S1PDGS-1910
 *
 *  Revision 1.36  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.35  2011/09/09 15:13:47  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.34  2011/09/09 09:36:10  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.33  2011/09/08 09:26:50  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.32  2011/09/05 16:16:41  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.31  2011/08/29 10:24:29  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *
 *  Revision 1.30  2011/08/26 15:54:03  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *
 *  Revision 1.29  2011/08/09 13:49:44  micmaz
 *  changed  log
 *
 *  Revision 1.28  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.27  2011/07/16 11:02:50  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.26  2011/07/14 12:26:37  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.25  2011/07/14 11:32:50  micmaz
 *  The circulator must remove sources first and then update the status.
 *
 *  Revision 1.24  2011/07/07 10:06:55  micmaz
 *  RESOLVED - issue S1PDGS-1600: CirculationAgent error message
 *  http://jira.acsys.it/browse/S1PDGS-1600
 *
 *  Revision 1.23  2011/07/07 10:05:48  micmaz
 *
 *  Revision 1.22  2011/07/07 09:49:17  micmaz
 *  REOPENED - issue S1PDGS-1600: CirculationAgent error message
 *  http://jira.acsys.it/browse/S1PDGS-1600
 *
 *  Revision 1.21  2011/07/06 17:59:14  micmaz
 *  the temporary file strategy is now prefix.
 *
 *  Revision 1.11  2011/05/03 14:04:10  micmaz
 *  corrected some circulation bugs
 *
 *  Revision 1.10  2011/04/29 17:22:14  micmaz
 *  if cannot create a log file, redirect to stderr
 *
 *  Revision 1.9  2011/04/27 09:50:52  micmaz
 *  errored ftp transfers must be logged.
 *
 *  Revision 1.8  2011/04/26 17:34:11  micmaz
 *  added rename remote directory.
 *
 *  Revision 1.4  2011/03/11 17:55:46  micmaz
 *  reworked time outs.
 *
 *  Revision 1.3  2011/03/08 18:15:26  micmaz
 *  Added the ovewrite flag to the configurer
 *
 *  Revision 1.2  2011/03/08 15:04:08  micmaz
 *  improved logs.
 *
 *  Revision 1.1  2011/01/21 18:04:34  micmaz
 *  added a copyable ftpCurl wrapper.
 *
 *
 *
 */

#include <ConsumableCirculation.h>
#include <ConsumableFtpCurl.h>
#include <CurlConfigurer.h>
#include <CirculationCallbackSingleton.h>

#include <TmpNameStrategyTable.h>	
#include <AllowedCiphersSingleton.h>

#include <DataConsumerConfigurer.h>
#include <TemporaryFileService.h>
#include <CirculationStatistics.h>
#include <LocalTransferMode.h>
#include <Repository.h>
#include <File.h>
#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>
#include <StringUtils.h>
#include <dbQuery.h>
#include <dbConnectionPool.h>
#include <exMacros.h>

#include <boost/bind.hpp> // PRQA S 1013

#include <iostream>
#include <fstream>

#include <MCFilterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;
using namespace std;
using namespace boost;
using namespace acs::curl;
using namespace acs::curl::temp;
using namespace acs::dc;
using namespace acs::mc;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ConsumableCirculation);

ConsumableCirculation::ConsumableCirculation(
        StopController& stopper, 
        dbConnectionPool& connectionPool,
        Circulation const& entity) :
    ConsumableItem(), 
    pool_(connectionPool), // PRQA S 2528
    _curl(stopper), 
    _manager(), 
    _localhost(_manager.findLocalhost(dbConnectionWrapper(pool_))), // PRQA S 3050, 3081
    _itemToConsume(entity), 
    _removeService(),
    _stopper(stopper), // PRQA S 2528
	_logExMsgAsError(true),
	_gotRecoverableError(false),
	_objectId()
{
	initializeObjectId();
	
	// Initialize remove service
	//_removeService.enableMoveOnLocalUnlinkFailure(acs::conf::CirculationConfigurerSingleton::instance()->getEnableMoveOnLocalUnlinkFailure());
	//_removeService.setLocalUnlinkFailureTrashPath(acs::conf::CirculationConfigurerSingleton::instance()->getLocalUnlinkFailureTrashPath());
}

ConsumableCirculation::~ConsumableCirculation() throw () {
    //empty.
}

void ConsumableCirculation::verifyAndRemove(RemoteSite* sourceSite, RemoteSite* targetSite) { // PRQA S 4020
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Verify and Remove target file/dir");
	ACS_COND_THROW( !sourceSite || !targetSite, UnrecoverableErrorException("Either source or target sites not valorized") ); // PRQA S 3081
	
    if (sourceSite->isLocalhost()) {
        ACS_COND_THROW(not File::exists(sourceSite->getPath()), UnrecoverableErrorException("The local resource does not exists (" + sourceSite->getPath() + ")", EAGAIN)); // PRQA S 3081
        bool isDir = File::isDir(sourceSite->getPath());
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "isDir: " << isDir);
        sourceSite->setIsDir(isDir);
        targetSite->setIsDir(isDir);
    } else {
        // no need to verify a file/dir if the protocol does not support directories
        if (targetSite->supportsDirectory()) {
            RemoteResourceType response = RemoteSiteUtility::probeSite(sourceSite, _stopper, true);
            ACS_COND_THROW(response == RRTNotExist, RecoverableErrorException("The remote resource does not exists (" + sourceSite->getPath() + ")") ); // PRQA S 3081
            targetSite->setIsDir(response == RRTIsDir);
        }
        else {
            targetSite->setIsDir(false) ;
        }
    }
	
    if (targetSite->isLocalhost()) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The target site is on local host.");

		string temporaryTargetFile = targetSite->getTemporaryDir() + targetSite->getTemporaryFile();
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Temporary target file: \"" << temporaryTargetFile << '\"');
		if( File::exists(temporaryTargetFile) ) {
            try {
                File::remove(temporaryTargetFile);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << temporaryTargetFile << "\" temporary target file already exists and removed.");
            } catch (std::exception const& ex) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, ex);
				string err = "The temporary name target resource \"" + temporaryTargetFile + "\" already exists and cannot remove it";
                ACS_LOG_ERROR(err);
                ACS_THROW( CannotRemoveTargetException(err) ); // PRQA S 3081
            }
			
		}
		
		string localFile = targetSite->getPath();
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Target file: \"" << localFile << '\"');
        if (File::exists(localFile) ) {

            bool overwrite_completed = conf::dcConfigurerSingleton::instance()->getOverwriteCompletedFiles() ;
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The destination file already exist, must overwrite? " << boolalpha << overwrite_completed);
            if (!overwrite_completed) {
				string wrn = "The target resource \"" + localFile + "\" already exists and cannot overwrite it (change the conf parameters if we must overwrite)";
                ACS_LOG_WARNING(wrn);
                ACS_THROW( CannotRemoveTargetException(wrn) ); // PRQA S 3081
            } else {
                try {
                    File::remove(localFile);
                	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The target resource already exists and removed.");
                } catch (std::exception const& ex) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, ex);
					string err = "The target resource \"" + localFile + "\" already exists and cannot remove it";
                    ACS_LOG_ERROR(err);
                	ACS_THROW( CannotRemoveTargetException(err) ); // PRQA S 3081
                }
            }
        } 
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Target resource does not exist");
        }
    } else {
        if (targetSite->supportsRemove()) { // nothing if the protocols does not support remotion
            ConsumableFtpCurl curl(_stopper);
            curl.setSite(*targetSite);
            BandPriority no_priority ;
            curl.initFtpCurl(no_priority);
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Verify and remove the target " << (sourceSite->getIsDir() ? "dir:" : "file: ") << targetSite->str()); // PRQA S 3380
		    curl.setLogError(false);
            bool removeLater = false ;
            bool var_res = curl.verifyAndRemove(sourceSite->getIsDir(), removeLater) ;
            ACS_COND_THROW(not var_res, CannotRemoveTargetException("The target resource \"" + targetSite->getPath() + "\" already exists and cannot remove it") ); // PRQA S 3081
        }
    }
}

void ConsumableCirculation::doWork() {

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Start Circulation");
	bool blockingCirculationStatus = false;
    try 
	{
    	const string &dbKey = pool_.getKey() ;
			
		TempRenameStrategy strategy = unknownStrategy;
		string tempnameValue;
		{
	    	dbConnectionWrapper w(pool_) ;
	    	dbConnection &conn = w ; // PRQA S 3050 
    		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Working on : " << _itemToConsume.str(conn));
   				
			bool null = false ;
			blockingCirculationStatus = ( _itemToConsume.getStatus(null, conn) == Status::instance(dbKey)->id(Status::blocking ) ) ;
			if( blockingCirculationStatus ) { // just for debug
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Circulation status is BLOCKING");
			}
			else {
            	 // set the BandPriority from the item.
				_curl.setBandPriority(_itemToConsume.getPriority(null, conn)) ;
		    	if (Status::instance(dbKey)->name(_itemToConsume.getStatus(null, conn)) == Status::recoverableError) {
		        	_manager.updateCirculationStatus(conn, Status::processing, _itemToConsume);
					dbPersistent::IdType circ_id = _itemToConsume.getId(); // get circulation id
					// fill Pending Circulations queue: save circulation info after status change
					DateTime modificationDate = _itemToConsume.getModificationDate(null, conn);
					try
					{
						CirculationCallbackSingleton::instance()->insertCallbackEventsInQueue (conn, circ_id, Status::instance(dbKey)->id(Status::processing), modificationDate);	
					}
					catch (std::exception const & ex)
					{
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Callback pending circulation queue cannot be updated!" << ex.what());
					}
		    	}
		    	_manager.updateCirculationStatus(conn, Status::transferring, _itemToConsume);
				dbPersistent::IdType circ_id = _itemToConsume.getId(); // get circulation id
				DateTime modificationDate = _itemToConsume.getModificationDate(null, conn);
				try
				{
					//save circulation info
					CirculationCallbackSingleton::instance()->insertCallbackEventsInQueue (conn, circ_id, Status::instance(dbKey)->id(Status::transferring), modificationDate);	
				}
				catch (std::exception const & ex)
				{
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Callback pending circulation queue cannot be updated!" << ex.what());
				}
				
			}
			
			// at this point status is either TRANSFERRING or BLOCKED		
			string tempName;
			try {
				tempName = TmpNameStrategyTable::instance(dbKey)->name(short(_itemToConsume.getTempNameId(null, conn))); // PRQA S 3081
    			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Name corresponding to tempname_id value (temporary strategy):" << tempName);
    			strategy = TemporaryFileService::makeStrategy(tempName);
    			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Recognized strategy : " << strategy);

				// get strategy string from tempnamevalue column
				tempnameValue = _itemToConsume.getTempNameValue(null, conn);
        		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Temp name value: \"" << tempnameValue << "\"");
			} 
			catch (std::exception const & ex) 
			{
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "t_circulationqueue tempname_id not valid: unknown rename strategy");
			}
						
		} // limited scope for connection 
          
        pair<std::string, std::string> fromNToCiphersPair = make_pair( "" , "" );
		RemoteSite sourceSite;
		RemoteSite targetSite;    
		string localTransferMode;
		{
		    dbConnectionWrapper w(pool_) ;
		    dbConnection &conn = w ; // PRQA S 3050 
   			bool null = false ;
            
			localTransferMode = LocalTransferMode::instance(dbKey)->name(short(_itemToConsume.getLocalTransferModeId(null, conn))); // PRQA S 3081

		    AllowedCiphersSingleton::instance(dbKey)->getCiphers( _itemToConsume.getCirculationPolicy(null, conn), fromNToCiphersPair );

		    if( "" == fromNToCiphersPair.first)
		    {
		        fromNToCiphersPair.first = acs::conf::CirculationConfigurerSingleton::instance()->getAllowedCiphers() ; 
		    }    

		    if( "" == fromNToCiphersPair.second)
		    {
		        fromNToCiphersPair.second = acs::conf::CirculationConfigurerSingleton::instance()->getAllowedCiphers() ; 
		    }    

		    sourceSite = RemoteSiteFactory::makeSite(_itemToConsume.getFromURL(null, conn), true, fromNToCiphersPair.first);
		    targetSite = RemoteSiteFactory::makeSite(_itemToConsume.getToURL(null, conn), true, fromNToCiphersPair.second);
		
			if (strategy != unknownStrategy) {
		    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Set temporary strategy to: " << strategy);
		    	sourceSite.setRenameStrategy(strategy, tempnameValue);
		    	targetSite.setRenameStrategy(strategy, tempnameValue);
			}

			if (dbSchemaMetaDataProvider::hasField("t_circulationsqueue", "streamer_id", conn)) {
				bool streamerIdNull = true;
				dbPersistent::IdType streamerId = _itemToConsume.getStreamerId(streamerIdNull, conn);
				if (! streamerIdNull) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Set streamer id to: " << streamerId);
					_curl.setStreamerId(streamerId);
				}
			}
		}
		
		if( not blockingCirculationStatus ) {	
        	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Verify source site: " << sourceSite); // PRQA S 3050 2
        	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Verify target site: " << targetSite);
	// verify the source file exists and remove the target file if existing before transferring
        	try {
				verifyAndRemove(&sourceSite, &targetSite);
			}
			catch( CannotRemoveTargetException const &e )
			{
            	ACS_LOG_ERROR(
                    	"Error during transferring file/dir: the destination file/dir is already present in the target site: "
                            	<< targetSite.str());
            	ACS_THROW( // PRQA S 3081
                    	FtpCurl::FtpCurlRemoteWriteErrorException(
                            	string("The destination file/dir is already present in the target site: ")
                                    	+ targetSite.str()));

			}

			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Set local transfer mode to: " << localTransferMode);
			_curl.setLocalTransferMode( conf::ImplicitCirculationConfigurer::String2LocalTransferMode(localTransferMode) );
			
        	_curl.setSourceSite(sourceSite);
        	_curl.setTargetSite(targetSite);

			try {
		// only to issue message
				string targetHost = targetSite.getHost();
				Proxy proxy ;
            	conf::CurlConfigurerSingleton::instance()->findProxy(targetHost, proxy) ;
    			if (proxy != no_proxy()) {
        			ACS_LOG_INFO("Circulation id# " << _itemToConsume.getId() << ": using proxy " << proxy.host() << " to connect with: " << targetHost) ;
    			}
			}
			catch(std::exception &) {}

			// The transfer shall not keep any open connection
        	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Setting earlyFinalRemoval as:" << boolalpha << conf::dcConfigurerSingleton::instance()->getEarlyFinalRemoval());
        	_curl.setEarlyFinalRemoval(conf::dcConfigurerSingleton::instance()->getEarlyFinalRemoval()) ;
        	_curl.doTransfer();
		}
		
		long long dataSize = 0;
		// This operation uses a connection inside
        postTransferOperations(sourceSite, targetSite, dataSize);

		try {
		    dbConnectionWrapper w(pool_) ;
		    dbConnection &conn = w ; // PRQA S 3050 
            bool null = false ;
			// APF-273
			ACS_LOG_MC_INFO( mc_param("filename", File::getFileName (_itemToConsume.getFromURL(null, conn)) ) << 
							 mc_param("queueid", _itemToConsume.getId() ) <<
							 mc_param("tourl", _itemToConsume.getToURL(null, conn) ) <<
							 mc_param("filesize", dataSize ) <<
							 mc_metric(Metric_DC_OUT)) ;
		}
		catch (std::exception const & ex)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Cannot log MC INFO: " << ex.what());
		}

		
    } catch (std::exception const & ex) {

	    dbConnectionWrapper w(pool_) ;
	    dbConnection &conn = w ; // PRQA S 3050 
		
		string exMsgStr;
		string itemToConsumeStr("UNKNOWN");
		string itemToConsumeIds("UNKNOWN");
		string itemToConsumeToUrl("UNKNOWN");
		
		try {
            bool null = false ;
			itemToConsumeStr = _itemToConsume.str(conn, true);
            ostringstream os ; os << _itemToConsume.getId(); 
			itemToConsumeIds = os.str() ;
			itemToConsumeToUrl = _itemToConsume.getToURL(null, conn);
		}
		catch( std::exception const & ex2)
		{
			ACS_LOG_WARNING("Cannot even get information on item to consume: " << ex2.what());
		}
		
		if( _stopper.haltImmediate() ) {
			ostringstream exMsg;
			exMsg << "Immediate stop requested during transferring item: " << itemToConsumeStr;
			exMsgStr = exMsg.str();
			ACS_LOG_WARNING(exMsg.str());
		}
		else {	
			bool logErrorsVerbose = conf::CirculationConfigurerSingleton::instance()->getLogErrorsVerbose();
            /*
				S3MARINE-499: the following log message has to respect EUM ICD: don't change the log without achieving an agreement on the new format!
			*/
			ostringstream errOs;
			errOs << "An error occurred during transferring item: " << itemToConsumeStr;
// 			if( logErrorsVerbose ) {
//      			// S2PDGS-1295 adding the circulation id to the log
// 				ACS_LOG_ERROR(errOs.str()) ;
// 
// 				// APF-273
// 				ACS_LOG_MC_INFO( mc_param("queueid", itemToConsumeIds ) <<
// 								 mc_param("tourl", itemToConsumeToUrl ) <<
// 								 mc_param("message", "An error occurred during the item transfer" ) <<
// 								 mc_metric(Metric_DC_FAIL)) ;
// 			}
// 			else {
// 				ACS_LOG_INFO(errOs.str()) ;
// 			}

			
			if( dynamic_cast<const dbConnection::ConnectionException*> (&ex) != 0 ) { // in this case do not try to update here record status to ERROR. // PRQA S 3081
				_logExMsgAsError = true;
				if( logErrorsVerbose ) {
					// S2PDGS-1295 adding the circulation id to the log
					ACS_LOG_ERROR(errOs.str()) ;
					// APF-273
					ACS_LOG_MC_ERROR( mc_param("queueid", itemToConsumeIds ) <<
								 mc_param("tourl", itemToConsumeToUrl ) <<
								 mc_param("message", "An error occurred during the item transfer" ) <<
								 mc_metric(Metric_DC_FAIL)) ;
				}
				else {
					ACS_LOG_INFO(errOs.str()) ;
				}
			}
			else if( blockingCirculationStatus ) { // move record into t_blockedcirculationsqueue to avoid endless loop if resetting into BLOCKED
				if( logErrorsVerbose ) {
				// S2PDGS-1295 adding the circulation id to the log
					ACS_LOG_ERROR(errOs.str()) ;

					// APF-273
					ACS_LOG_MC_ERROR( mc_param("queueid", itemToConsumeIds ) <<
								 mc_param("tourl", itemToConsumeToUrl ) <<
								 mc_param("message", "An error occurred during the item transfer" ) <<
								 mc_metric(Metric_DC_FAIL)) ;
				}
				else {
					ACS_LOG_INFO(errOs.str()) ;
				}
				_logExMsgAsError = true;
				moveIntoBlockedCirculationsQueue( conn); 
			}
			else {
				bool recoverableError = true;
				if( (dynamic_cast<const FtpCurl::FtpCurlUnrecoverableErrorException*> (&ex) != 0) ||  // PRQA S 3081
                    (dynamic_cast<const Curl::CurlUnrecoverableException*> (&ex) != 0) ) { // in this case only not to cause regression! // PRQA S 3081
					recoverableError = false;
					ACS_LOG_INFO ("The detected error is not recoverable and no retry will be attempted");
				}

				bool recoverableErrorSet = false;
				try {
					_manager.updateErrorStatus(conn, _itemToConsume, recoverableError, &recoverableErrorSet);
					if( logErrorsVerbose ) {
						// S2PDGS-1295 adding the circulation id to the log

						if (!recoverableErrorSet) // not recoverable error
						{
							ACS_LOG_ERROR(errOs.str()) ;
							// APF-273
							ACS_LOG_MC_ERROR( mc_param("queueid", itemToConsumeIds ) <<
								mc_param("tourl", itemToConsumeToUrl ) <<
								mc_param("message", "An error occurred during the item transfer" ) <<
								mc_metric(Metric_DC_FAIL)) ;
						}
						else
						{
							_gotRecoverableError = true;
							ACS_LOG_INFO(errOs.str()) ;
						}
					}
					else
					{
						ACS_LOG_INFO(errOs.str()) ;
					}

					dbPersistent::IdType circ_id = _itemToConsume.getId(); // get circulation id
					bool isNull(false);
					short status_id = _itemToConsume.getStatus(isNull, conn);
					DateTime updatetime = _itemToConsume.getModificationDate(isNull, conn);
					try {
						// fill Pending Circulations queue
						CirculationCallbackSingleton::instance()->insertCallbackEventsInQueue (conn, circ_id, status_id, updatetime);	
					}
					catch (std::exception const & exx) {
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Pending Circulations queue not updated: " << exx.what() );
						
					}

				}
				catch (std::exception const & exx) {
					if( logErrorsVerbose ) {
						// S2PDGS-1295 adding the circulation id to the log
						ACS_LOG_ERROR(errOs.str()) ;

						// APF-273
						ACS_LOG_MC_ERROR( mc_param("queueid", itemToConsumeIds ) <<
							 mc_param("tourl", itemToConsumeToUrl ) <<
							 mc_param("message", "An error occurred during the item transfer" ) <<
							 mc_metric(Metric_DC_FAIL)) ;
					}
					else {
						ACS_LOG_INFO(errOs.str()) ;
					}
					ACS_LOG_NOTIFY_EX(exx);
					ACS_LOG_ERROR("Cannot even update status of circulation record with id = " << itemToConsumeIds );
				}
			
				_logExMsgAsError = true;
				if( not logErrorsVerbose && recoverableErrorSet ) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Circulation exception errors will not be logged as errors");
					_logExMsgAsError = false;
				}
			}
		}
		
		
        ACS_COND_THROW( _stopper.haltImmediate(), StopException(exMsgStr) ); // no need to take into account original exception! // PRQA S 3081

        throw;
    }
}

void ConsumableCirculation::postTransferOperations(RemoteSite const& sourceSite, RemoteSite const& targetSite, long long & transferSize) {
    //Remove source file/folder.
    //refresh on the fly the remove flag.

	// cleanup transferSize value. It will be set to zero if statistics are disabled
	transferSize=0;

    dbConnectionWrapper w(pool_) ;
    dbConnection &conn = w ;// PRQA S 3050 
    const string &dbKey = conn.getKey() ;    
    Circulation circ(conn) ;
    if (not circ.load(conn, _itemToConsume.getId())) {
        ostringstream os ; os << "Cannot load Circulation id: " << _itemToConsume.getId() ;
        ACS_THROW(Circulation::ExecutionError(os.str())) ; // PRQA S 3081
    }
    bool null = false ;
    if (circ.getRemoveSource(null, conn) ) {
        ACS_LOG_PRIVINFO("Adding source to remove queue: " << RemoteSiteFactory::toURL(sourceSite, false) ); 	// remove the password from url
        _removeService.removeSource(conn, _itemToConsume, sourceSite, _stopper);
    } else {
        ACS_LOG_PRIVINFO("The source must not be removed: " << RemoteSiteFactory::toURL(sourceSite, false) );  	// remove the password from url
    }
	
	bool blockingCirculationStatus = ( circ.getStatus(null, conn) == Status::instance(dbKey)->id(Status::blocking ) ) ;
	if( blockingCirculationStatus ) { // for debug only
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Status is BLOCKING: no update of record will be performed" );
		// move record into t_blockedcirculationsqueue
		moveIntoBlockedCirculationsQueue( conn); 
		
	}
	else {
    	//If the transfer was correct, update the status to completed.
    	string finalStatus = Status::completed;

    	_manager.updateToEndStatus(conn, finalStatus, _itemToConsume);
		dbPersistent::IdType circ_id = _itemToConsume.getId(); // get circulation id

		DateTime updatetime = _itemToConsume.getModificationDate(null, conn);

		try
		{
			// fill Pending Circulations queue: save circulation info, after status change
			CirculationCallbackSingleton::instance()->insertCallbackEventsInQueue (conn, circ_id, Status::instance(dbKey)->id(Status::completed), updatetime);	
		}
		catch (std::exception const & exx)
		{
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Pending Circulations queue not updated: " << exx.what() );
		}

		{
			ACS_LOG_INFO("Circulation id# " << _itemToConsume.getId() << " of " << File::getFileName(_itemToConsume.getFromURL(null, conn)) << ' ' << finalStatus );
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Updated status item details: " << _itemToConsume.str(conn, true) );
	}

	// WARNING: if status was BLOCKING _itemToConsume record removed!
    if (circ.getRemoveSource(null, conn) ) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "The source must be removed: " << _itemToConsume.getFromURL(null, conn) << "; going to check the remove queue and other circulation from the same url");
    } 
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "The source must not be removed: " << _itemToConsume.getFromURL(null, conn) << "; going to check anyway the remove queue and possibly remove the source");
		// Other circulations from the same url with removeFlag=true could have been processed at the same time and already finished: check the remove queue. If not empty and no circulations
		// from the same url neither in not final state nor in ERROR occur, remove the source  and empty the queue
	}
	_removeService.removeSourceAfterComplete(conn, _itemToConsume, sourceSite, _stopper);
	
	if( not blockingCirculationStatus ) {

    	//Collect statistics.
    	vector < CirculationStatistics > stats;
    	_curl.getStats(stats);
    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Statistics found: " << stats.size());
    	if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics() && !stats.empty()) {
        	CirculationStatistics acc = *(stats.begin());
        	for (vector<CirculationStatistics>::iterator iter = ++(stats.begin()); iter != stats.end(); ++iter) { // PRQA S 4238
            	acc.mergeStatistics(conn, *iter);
        	}
			// Pass the data size in order to put it in the MC messages
			transferSize = acc.getDataSize(null, conn);
        	dbPersistent::IdType fromHostId = 0 ;
        	dbPersistent::IdType toHostId = 0 ;
			bool newCirculationStatsColumns = false;
			if( dbSchemaMetaDataProvider::hasField("t_circulationstats", "unkn_from_host", conn) && dbSchemaMetaDataProvider::hasField("t_circulationstats", "unkn_to_host", conn) ) {
				newCirculationStatsColumns = true;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Both columns unkn_from_host and unkn_to_host occur into t_circulationstats table") ;
			}

			string unknFromHost;
        	if (acc.getSourceSite().isLocalhost()) {
            	try {
                	ACS_COND_THROW(not _localhost, exIllegalValueException("Local host name not found on the database") ); // PRQA S 3081
                	fromHostId = _localhost->getId();
            	} catch (std::exception const& ex) {
					unknFromHost = conf::CirculationConfigurerSingleton::instance()->getHostName();
					if( newCirculationStatsColumns ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The source host (localhost) does not occur in database " << unknFromHost);
					}
					else { 
						ACS_LOG_WARNING( "Cannot write the statistics because there is not the localhost (source host) in database " << unknFromHost);
					}
            	}
        	} else {
            	try {
                	fromHostId = KnownHost::findHostByName(conn, acc.getSourceSite().getHost()).getId();
            	} catch (std::exception const& ex) {
					unknFromHost = acc.getSourceSite().getHost();
					if( newCirculationStatsColumns ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The source host does not occur in database " << unknFromHost);
					}
					else {
                		ACS_LOG_WARNING( "Cannot write the statistics because there is not the source host in database " << unknFromHost);
					}
            	}
        	}

			string unknToHost;
        	if (acc.getDestSite().isLocalhost()) {
            	try {
					ACS_COND_THROW(not _localhost, exIllegalValueException("Local host name not found on the database") ); // PRQA S 3081
                	toHostId = _localhost->getId();
            	} catch (std::exception const& ex) {
					unknToHost = conf::CirculationConfigurerSingleton::instance()->getHostName();
					if( newCirculationStatsColumns ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The destination host (localhost) does not occur in database " << unknToHost);
					}
					else {
                		ACS_LOG_WARNING( "Cannot write the statistics because there is not the localhost (destination host) in database " << unknToHost);
					}

            	}
        	} else {
            	try {
                	toHostId = KnownHost::findHostByName(conn, acc.getDestSite().getHost()).getId();
            	} catch (std::exception const& ex) {
					unknToHost = acc.getDestSite().getHost();
					if( newCirculationStatsColumns ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The destination host does not occur in database " << unknToHost);
					}
					else {
                		ACS_LOG_WARNING( "Cannot write the statistics because there is not the destination host in database " << unknToHost);
					}
            	}
        	}

        	if( ( (fromHostId == 0) || (toHostId == 0) ) && !newCirculationStatsColumns ) {
            	ACS_LOG_WARNING("Cannot Create statistics because not all hosts have been found in db.");
			}
			else {
				try {
					if (fromHostId > -1) {
						acc.setFromHostId(fromHostId, conn);
					}
					else { 
						ACS_COND_THROW(StringUtils::isBlank(unknFromHost), exIllegalValueException("The source host does not occur in db and its name is not valorized")); // PRQA S 3081
						acc.setUnknFromHost(unknFromHost, conn);
					}

					if (toHostId > -1) {
            			acc.setToHostId(toHostId, conn);
					}
					else { 
						ACS_COND_THROW(StringUtils::isBlank(unknToHost), exIllegalValueException("The destination host does not occur in db and its name is not valorized")); // PRQA S 3081
						acc.setUnknToHost(unknToHost, conn);
					}

            		acc.setCirculationsQueueId(_itemToConsume.getId(), conn);
            		acc.save(conn);
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "New CirculationStatistics record created: " << acc.str(conn, true));
				}
				catch(std::exception const& ex) {
					ACS_LOG_NOTIFY_EX(ex);
					ACS_LOG_WARNING("Cannot Create statistics.");
        		}
			}
    	}
	} // if( not blockingCirculationStatus ) 
}

#ifdef OLD_CODE
void ConsumableCirculation::makeDigest(RemoteSite const& site, vector<SignedFile>* signedFiles) const { // PRQA S 4214
    MD5Signer signer;
    signer.sign(dbConnectionWrapper(pool_), _itemToConsume, site.getPath(), signedFiles); // PRQA S 3050, 3081
}
void ConsumableCirculation::writeDigest(RemoteSite const& site, vector<SignedFile> const& signedFiles) const {// PRQA S 4214
    MD5Signer signer ;
    string reportName = makeManifestFilename(site);
    File::mkdir(File::getDirPath(reportName), true);
    ofstream reportFile;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Report file to write : " << reportName);
    reportFile.open(reportName.c_str());
    signer.toStream(reportFile, signedFiles);
    reportFile.close();
}
std::string ConsumableCirculation::makeManifestFilename(curl::RemoteSite const& site) const {// PRQA S 4214
    string reportFilename = conf::CirculationConfigurerSingleton::instance()->getMediaReportFilename() ;
    reportFilename = StringUtils::transform(reportFilename, "${DIR_NAME}", site.getRemoteDir());
    reportFilename = StringUtils::transform(reportFilename, "${FILE_NAME}", site.getRemoteFileName());
    return reportFilename;
}
#endif
string ConsumableCirculation::getSourceSite() {
    bool null = false ;
    return _itemToConsume.getFromURL(null, dbConnectionWrapper(pool_)); // PRQA S 3050, 3081
}
string ConsumableCirculation::getTargetSite() {
    bool null = false ;
    return _itemToConsume.getToURL(null, dbConnectionWrapper(pool_)); // PRQA S 3050, 3081
}

void ConsumableCirculation::initializeObjectId()
{
	if( _objectId.empty() ) {
        bool null = false ;
		std::string fromUrl = _itemToConsume.getFromURL(null, dbConnectionWrapper(pool_)); // PRQA S 3050, 3081
		try {
			fromUrl = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(fromUrl, true), false);
		}
		catch( std::exception & )
		{
			ACS_LOG_WARNING("Wrong fromUrl format: \"" << fromUrl << '\"');
		}

		std::string toUrl = _itemToConsume.getToURL(null, dbConnectionWrapper(pool_)); // PRQA S 3050, 3081
		try {
			toUrl = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(toUrl, true), false);
		}
		catch( std::exception & )
		{
			ACS_LOG_WARNING("Wrong toUrl format: \"" << toUrl << '\"');
		}

    	ostringstream os;
    	os << "[Circulation id# " << _itemToConsume.getId() << " from: " << fromUrl << " to: " << toUrl << "]"; // PRQA S 3050, 3081
		_objectId = os.str();
	}
}

std::string ConsumableCirculation::id() const 
{
	return _objectId;
}


std::string ConsumableCirculation::groupId() const {
    return id(); //the circulations are not grouped.
}
std::string ConsumableCirculation::str() const {
    return _curl.str();
}
int ConsumableCirculation::getPriority() const {

    dbConnectionWrapper cw(pool_) ;
    dbConnection &conn = cw ; // PRQA S 3050
    bool null = false ;
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Get Priority of: " << _itemToConsume.str(conn) << "]");
    return _itemToConsume.getPriority(null, conn);
}
void ConsumableCirculation::setPriority(int priority) {
    dbConnectionWrapper cw(pool_) ;
    dbConnection &conn = cw ; // PRQA S 3050
    _itemToConsume.setPriority(priority, conn);
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Set Priority of: " << _itemToConsume.str(conn) << "]");
}

#ifdef OLD_CODE
void ConsumableCirculation::getFromNToCiphers(pair<std::string, std::string>&p , dbPersistent::IdType id, dbConnection &conn)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    
    string dbKey (conn.getKey());

    SQLString theString(dbKey, SQLString::SELECT );
/*
*    select S.allowedciphers as SourceCiph, D.allowedciphers as DestCiph from
*    t_circulationpolicies as C 
*    inner join t_repositories as S on (C.source_id = S.id_repository)
*    inner join t_repositories as D on (C.destination_id = D.id_repository)
*    where C.id= id(from signature);
*/

    // Add Select List
    theString.addSelectList("S.allowedciphers", "SourceCiph" );
    theString.addSelectList("D.allowedciphers", "DestCiph" );

    // Table
    theString.addTableList ("t_circulationpolicies", "C");

    // The Join

    {
   		dbQueryParameters pp( dbKey);
 		pp.addParameter(dbParam( dbKey, "S","id_repository" , dbParam::EQUAL_TO, "C.source_id")) ;
		theString.addJoinList(SQLDialect::INNER,"t_repositories S",pp) ;
    }
    {
   		dbQueryParameters pp( dbKey);
 		pp.addParameter(dbParam( dbKey, "D","id_repository" , dbParam::EQUAL_TO, "C.destination_id")) ;
		theString.addJoinList(SQLDialect::INNER,"t_repositories D",pp) ;
    }

    // Build Where clause
    {
        dbQueryParameters pp(dbKey) ;

        pp.addParameter(dbParam(dbKey, "C","id" , dbParam::EQUAL_TO , db::toSQL(dbKey, id))) ;

       theString.addANDWhereParameter(pp) ;
    }
    dbQuery q(conn, theString, "Ciphers from Circulation" );
	dbSet set ;
	q.execForSet(set) ;
 	dbRow selectrow ;
    set.getNextRow(selectrow) ; // first and only 
    selectrow.column(0, p.first);
    selectrow.column(1, p.second);
}
#endif

void ConsumableCirculation::moveIntoBlockedCirculationsQueue( dbConnection & conn )
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    
	bool blockedCircQueueRecExists = false;
	const string newTable = "t_blockedcirculationsqueue";
	dbPersistent::IdType newItemId = 0;
	try {
		
		string dbKey = conn.getKey();
		bool null = false ;
		dbSmartQuery smartQuery(newTable, dbKey);
		{
    		dbQueryParameters pp (dbKey);
			pp.setParamOperator( dbQueryParameters::AND ) ;
    		pp.addParameter(dbParam(dbKey, newTable, "fromurl" , dbParam::EQUAL_TO , db::toSQL(dbKey, _itemToConsume.getFromURL(null, conn))));
    		pp.addParameter(dbParam(dbKey, newTable, "tourl" , dbParam::EQUAL_TO , db::toSQL(dbKey, _itemToConsume.getToURL(null, conn))));
    		pp.addParameter(dbParam(dbKey, newTable, "host_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _itemToConsume.getHostId(null, conn))));
    		pp.addParameter(dbParam(dbKey, newTable, "circulationpolicy_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _itemToConsume.getCirculationPolicy(null, conn))));
    		smartQuery.addParameter(pp) ;
		}
		
		if( smartQuery.doQuery(conn) > 0 ) {
			blockedCircQueueRecExists = true;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, newTable << " record with fromurl=\"" << _itemToConsume.getFromURL(null, conn) << "\" tourl=\"" << _itemToConsume.getToURL(null, conn) 
					<< "\" host_id=" << _itemToConsume.getHostId(null, conn) << " circulationpolicy_id=" << _itemToConsume.getCirculationPolicy(null, conn) << " already occurring: no new record will be inserted");
		}
		else {
			vector<string> fieldNames = _itemToConsume.getFieldNames(conn) ;
			size_t avalFields = fieldNames.size();
			dbSmartPersistent newItem(newTable, conn);
			for (size_t field=0; field<avalFields; ++field) {
				dbColumn element(dbKey);
				_itemToConsume.get<dbColumn>(field, element, null, conn);
				// set it only if it is not null
				if ( not element.isNull()) {
    				newItem.set (fieldNames[field], element, conn); 
				}
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  newTable<< " record populated");
			newItem.unsetId();
			newItem.set( "status_id", Status::instance(conn.getKey())->id(Status::blocked), conn);
			newItem.set( "modificationdate", DateTime(), conn);
			newItemId = newItem.save(conn);
		}
		
		_itemToConsume.remove(conn);
		
		if( not blockedCircQueueRecExists ) {
			ACS_LOG_INFO( "t_circulationsqueue record with id=" << _itemToConsume.getId() << ", item=\"" << File::getFileName(_itemToConsume.getFromURL(null, conn)) 
						<< "\" successfully moved to " << newTable << " table (id=" << newItemId << ')');
		}
		else {
			ACS_LOG_INFO( "t_circulationsqueue record with id=" << _itemToConsume.getId() << ", item=\"" << File::getFileName(_itemToConsume.getFromURL(null, conn)) 
						<< "\" not moved to " << newTable << " table since corresponding record already occurring: just removed");
		}


	}
	catch( std::exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		if( not blockedCircQueueRecExists && 0 == newItemId ) {
			ACS_LOG_WARNING("Cannot move t_circulationsqueue record with id=" << _itemToConsume.getId() << " to " << newTable << " table");
		}
	}
	
}
bool ConsumableCirculation::getLogExMsgAsError() const
{
	if (_logExMsgAsError)
	{
		bool filterRecoverableErrors = conf::CirculationConfigurerSingleton::instance()->getFilterRecoverableError();
		if (filterRecoverableErrors && _gotRecoverableError)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "RecoverableError filter enabled and RecError found.");
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

_ACS_END_NESTED_NAMESPACE;

