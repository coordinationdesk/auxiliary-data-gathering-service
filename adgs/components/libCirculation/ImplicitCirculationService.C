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
 *  Revision 7.7  2018/01/09 09:31:47  chicas
 *  S2PDGS-1898 : added notification in case of error during local url delete
 *
 *  Revision 7.6  2017/10/25 13:57:54  marpas
 *  getting rid of ThreadGroup and using Thread class' range functions
 *
 *  Revision 7.5  2017/10/24 15:39:36  marpas
 *  fixing query parameter contruction
 *
 *  Revision 7.4  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.3  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.2  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.1  2017/02/01 15:27:05  marfav
 *  ACSMAINT-88
 *  Files are sorted before insertion in the queue
 *  Sort criteria is modification date ascending (FIFO in repository)
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.60  2016/08/01 15:21:48  lucio.pulvirenti
 *  S1PDGS-31765: If local transfer mode not set in circulation policy record, set value got from implicit circulation configuration.
 *
 *  Revision 5.59  2016/08/01 08:07:31  lucio.pulvirenti
 *  S1PDGS-31765: circulation policy localtransfer_id column managed and inserted into circulation queue record if not nullable.
 *
 *  Revision 5.58  2016/05/16 09:41:03  chicas
 *  modified Waiting data in paths msg
 *
 *  Revision 5.57  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.56  2016/03/07 15:32:23  marfav
 *  S2PDGS-1428, PDSEV-28
 *  Filename scan algorithm completely rewritten to improve the speed
 *  Stop controller managed during directory contents scan (3 passes)
 *  Error messages in case of bad repositories issued only once
 *
 *  Revision 5.55  2016/01/21 14:34:17  lucio.pulvirenti
 *  S2PDGS-1384: CannotLoadEntityFromDbException turned into EntityBeamLoadException.
 *
 *  Revision 5.54  2016/01/19 15:57:32  lucio.pulvirenti
 *  S2PDGS-1384: calls to makeSourcePath method now put in try block to catch CannotLoadEntityFromDbException if
 *  circulation policy record disappears. In this case a warning is issued and processing goes on.
 *
 *  Revision 5.53  2015/11/26 14:29:21  marfav
 *  mc_params are now lowercase
 *
 *  Revision 5.52  2015/10/07 12:23:24  lucio.pulvirenti
 *  APF-324: insertIntoCirculationQueue: t_circulationpolicies agent_node_id col managed to fill circulation queue host_id if not null.
 *  Debug improved.
 *
 *  Revision 5.51  2015/09/04 15:01:48  lucio.pulvirenti
 *  S2PDGS-1247: INFO for M&C issued to warn operator that request to circulate items via media occurs.
 *
 *  Revision 5.50  2015/08/27 16:03:32  damdec
 *  Log messages added to warn operator in case of media circulation.
 *
 *  Revision 5.49  2015/08/14 16:08:11  tergem
 *  APF-273: IN messages
 *
 *  Revision 5.48  2015/08/04 07:54:10  lucio.pulvirenti
 *  S2PDGS-1247: no more MEDIA_WAIT status is set if protocol is media.
 *
 *  Revision 5.47  2015/03/10 11:35:51  marfav
 *  S2PDGS-1110 Added the function to manage local and remote repositories in different controlling threads
 *
 *  Revision 5.46  2014/10/10 16:31:10  lucio.pulvirenti
 *  S2PDGS-939: composition of destination path performed via RemotesiteUtility method to remove extra slashes.
 *
 *  Revision 5.45  2014/10/03 14:01:46  lucio.pulvirenti
 *  S2PDGS-869: if localhost not occurring in t_knownhosts table set status to DEGRADED instead of FAILURE.
 *
 *  Revision 5.44  2014/09/10 14:17:27  marfav
 *  Added define CIRC_STATUS_CHECKED
 *  if defined makes a transition from StandBy to the real needed inital status
 *
 *  Revision 5.43  2014/09/05 15:01:14  lucio.pulvirenti
 *  S1PDGS-31437: ItemService: transaction roll back performed after loop on policies for every item to circulate if not all circulation records have been inserted
 *  (alreadyCreated method check).
 *  insertIntoCirculationQueue method IF changed: returning bool (true if inserted record).
 *
 *  Revision 5.42  2014/09/02 13:24:14  lucio.pulvirenti
 *  S2PDGS-841: Message issued when file to circulate is detected. Message corresponding to circulation queue record creation improved.
 *
 *  Revision 5.41  2014/08/08 16:26:54  lucio.pulvirenti
 *  S2PDGS-841: work in progress.
 *
 *  Revision 5.40  2014/08/06 16:14:49  lucio.pulvirenti
 *  S2PDGS-835: Exceptions notificated before error message.
 *
 *  Revision 5.39  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.38  2014/06/20 14:53:41  marpas
 *  adopting FtpCurl 5.47 interface
 *
 *  Revision 5.37  2014/06/16 14:49:59  lucio.pulvirenti
 *  S2PDGS-764: Query on t_knownhost table performed at every iteration. If exception returned application status set to FAILURE.
 *  DiskEventNotifier setLogExcStack method called not to log exception stack if paths not existing.
 *
 *  Revision 5.36  2014/06/05 18:48:09  marpas
 *  getting rid of asserts
 *
 *  Revision 5.35  2014/05/16 10:21:13  lucio.pulvirenti
 *  S2PDGS-728: Removed reset of polling time at the end of each iteration to speed up policies reprocessing. Polling time is updated when it is time to reprocess policies.
 *
 *  Revision 5.34  2014/04/30 10:09:13  lucio.pulvirenti
 *  S2PDGS-726: ImplicitCirculationService::ItemService in run loop check on requested cancel performed to stop t_circulationsqueue records insertion.
 *
 *  Revision 5.33  2014/04/14 15:28:59  marfav
 *  In case of exception in the ItemService thread force a rescan of the affected local path
 *
 *  Revision 5.32  2014/04/14 13:20:28  lucio.pulvirenti
 *  ItemService CTOR is passed pointer to calling class.
 *  stopService: lazy stop sent
 *  stopImmediateService method added to send an immediate stop
 *  ItemService::run: if immediate stop insertion loop is interrupted.
 *
 *  Revision 5.31  2014/04/11 17:21:28  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.30  2014/03/11 17:42:56  marfav
 *  Added support to date based filtering
 *
 *  Revision 5.29  2014/03/10 15:48:45  marfav
 *  Removed ACS_TABLES_FUSION_318 dependency and dead code
 *
 *  Revision 5.28  2014/02/14 14:00:51  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.27  2014/02/07 18:53:36  marpas
 *  adopting ACS_THROW and ACS_COND_THROW macros
 *
 *  Revision 5.26  2014/02/04 15:37:12  lucio.pulvirenti
 *  Delay step was 11 ms in waiting loop. Set to 100 ms.
 *  Polling time reset at the end of every iteration.
 *  removeIfNotMatch temporary removed: waiting for next version for definitive deletion.
 *
 *  Revision 5.25  2014/01/31 17:04:28  lucio.pulvirenti
 *  Debug changed.
 *
 *  Revision 5.24  2014/01/31 12:12:19  marpas
 *  a circulation item is not created if the file access time is less of the modification date of any record which circulated  it.
 *  Interface rationalization and robustness improved
 *  removed useless bosst algorithm to compare strings for equality.
 *
 *  Revision 5.23  2014/01/30 17:34:55  lucio.pulvirenti
 *  When inserting circulation queue from circulation policies, transaction performed on the same file instances and not on the whole set of policies.
 *
 *  Revision 5.22  2014/01/10 12:31:41  lucio.pulvirenti
 *  insertIntoCirculationQueue: if record must not be created an info message is issued instead of debug.
 *
 *  Revision 5.21  2014/01/08 08:48:29  marfav
 *  Group creation logic rewritten to consider all the policies at any loop
 *  saving the skipped policies avoiding to lose them
 *
 *  Revision 5.20  2013/12/17 17:49:53  marpas
 *  policies thread fillup fixed and slightly simplified
 *
 *  Revision 5.19  2013/12/17 13:27:03  lucio.pulvirenti
 *  Check on ItemService boost ptr vector not empty before erasing to avoid possible crashes.
 *
 *  Revision 5.18  2013/12/11 14:08:54  lucio.pulvirenti
 *  Debug added.
 *
 *  Revision 5.17  2013/12/11 14:04:37  lucio.pulvirenti
 *  If db schema aligned to S2PDGS-318 AND removesource col occurs in t_circulationpolicies table, removesource value put in t_circulationqueue record is taken
 *  from the corresponding t_circulationpolicies record; otherwise it is taken from configuration.
 *
 *  Revision 5.16  2013/11/04 14:26:22  marpas
 *  coding best practices applied
 *
 *  Revision 5.15  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.14  2013/10/24 09:51:29  chicas
 *  messages improved
 *
 *  Revision 5.13  2013/10/10 14:03:22  chicas
 *  Log Message improved
 *
 *  Revision 5.12  2013/10/09 10:06:39  chicas
 *  Fixing destination bug for ACS_TABLE_FUSION_318
 *
 *  Revision 5.11  2013/10/09 08:03:23  chicas
 *  Big fixed in Implicit Circulation destination
 *
 *  Revision 5.10  2013/10/08 16:40:11  chicas
 *  bug fixed in implicit circulation in circulationqueue destination building
 *
 *  Revision 5.9  2013/08/07 14:37:36  lucio.pulvirenti
 *  Changes needed after schema modifications as per jira issue S2PDGS-318. ACS_TABLES_FUSION_318 flag is used for changes
 *  makeSourcePath: Repository class (t_repositories table) instead of CirculationUser (t_circulationusers).
 *  filterSourceFiles: t_circulationpolicies filetype_id type now is integer.
 *  makeDestination: t_circulationpolicies destURL col occurs no more: remote path from t_repository corresponding to
 *  destination_id is taken.
 *  insertIntoCirculationQueue: both t_circulationpolicies tempname_id and tempname_value cols occur no more: values are taken
 *  from t_repository corresponding to destination_id.
 *
 *  Revision 5.8  2013/08/05 13:01:18  lucio.pulvirenti
 *  According to jira issue S2PDGS-422 the following modifications have been implemented:
 *  DiskEventNotifier thread is started at beginning of every iteration of the main while loop: if events are detected
 *  check on local host source site is performed at first.
 *  Check on both local and remote host source directory is anyway performed with periodicity of CIRCULATOR_POLLING_PERIOD seconds
 *  as in the past.
 *  A configuration flag may be set not to use inotify to detect stable files to be circulated.
 *  alreadyCreated: CIRCULATOR_POLLING_PERIOD instead of CIRCULATOR_SECONDS_TO_REPROCESS is used to check if t_circulationqueue records
 *  with the same source and destination URL already exist.
 *
 *  Revision 5.7  2013/08/02 16:34:20  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.6  2013/07/23 17:35:01  marpas
 *  interface rationalization
 *
 *  Revision 5.5  2013/07/23 16:27:03  lucio.pulvirenti
 *  Work in progress for S2PDGS-427 modifications.
 *  MP Changes for DateTime.
 *
 *  Revision 5.4  2013/07/22 13:19:18  marpas
 *  tyoes fixing
 *  coding best practices applied
 *  test fixed
 *
 *  Revision 5.3  2013/07/18 12:29:51  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.2  2013/07/16 14:52:16  marpas
 *  coding best practices applied,
 *  performances and robustness improved
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.82  2013/06/07 15:22:39  lucio.pulvirenti
 *  Debug macros fixed.
 *  Circulationqueue record tempname_id and tampnamevalue fields filled by circulationpolicy.
 *
 *  Revision 1.81  2013/04/29 15:44:43  marpas
 *  file type recognition class interface changed
 *
 *  Revision 1.80  2013/04/22 11:00:25  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.79  2013/04/19 13:05:36  marpas
 *  fixing case when table has no id column
 *  management of OLD/NEW pds schema fixed
 *
 *  Revision 1.78  2013/04/09 19:06:19  marpas
 *  fix a crash if localhost is not found.
 *
 *  Revision 1.77  2013/04/08 16:29:08  micmaz
 *  corrected errors.
 *
 *  Revision 1.76  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.75  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.74  2013/01/25 13:47:36  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.73  2012/12/20 15:44:41  micmaz
 *  work in progress
 *
 *  Revision 1.72  2012/12/20 15:37:06  micmaz
 *  work in progress
 *
 *  Revision 1.71  2012/11/29 16:31:16  micmaz
 *  work in progress
 *
 *  Revision 1.70  2012/11/28 18:42:55  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.69  2012/11/28 18:37:05  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.68  2012/11/28 17:56:27  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *
 *  Revision 1.62  2012/11/27 17:11:43  micmaz
 *  PDS_OLD_SCHEMA compatibility.
 *
 *  Revision 1.61  2012/11/27 15:24:33  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.60  2012/11/21 17:27:34  micmaz
 *  modified comment.
 *
 *  Revision 1.59  2012/06/21 12:10:53  micmaz
 *  the error status must not considered as finish status.
 *
 *  Revision 1.58  2012/04/04 17:15:21  micmaz
 *  work in progress
 *
 *  Revision 1.57  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.56  2012/03/16 17:01:19  micmaz
 *  to lower case for protocols
 *
 *  Revision 1.55  2012/03/15 16:58:07  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.54  2012/03/09 17:43:34  micmaz
 *  added media functionality
 *
 *  Revision 1.53  2012/03/08 13:20:45  micmaz
 *  added media functionality
 *
 *  Revision 1.52  2012/02/29 14:47:14  micmaz
 *  IN PROGRESS - issue S1PDGS-2952: Implicit circulation service: when there are multiple policies with the same source host/path, not all the circulations have been created.
 *  http://jira.acsys.it/browse/S1PDGS-2952
 *
 *  Revision 1.51  2012/02/20 15:11:10  marpas
 *  refactoring
 *
 *  Revision 1.50  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.49  2012/02/09 17:03:20  micmaz
 *  probing remote sites
 *
 *  Revision 1.48  2012/02/07 10:25:29  micmaz
 *  removed warnings.
 *
 *  Revision 1.47  2012/01/30 18:12:16  micmaz
 *  IN PROGRESS - issue S1PDGS-2736: CirculationService hangs in case of multiple policies installed on the same circulation node and using local baskets
 *  http://jira.acsys.it/browse/S1PDGS-2736
 *
 *  Revision 1.46  2012/01/30 17:58:55  micmaz
 *  IN PROGRESS - issue S1PDGS-2736: CirculationService hangs in case of multiple policies installed on the same circulation node and using local baskets
 *  http://jira.acsys.it/browse/S1PDGS-2736
 *
 *  Revision 1.45  2012/01/24 17:02:15  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 1.44  2012/01/20 19:32:40  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 1.43  2012/01/20 16:00:05  micmaz
 *  modified logs
 *
 *  Revision 1.42  2011/11/14 10:46:20  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.41  2011/11/10 15:59:03  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.40  2011/11/03 16:02:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.39  2011/11/02 16:45:40  micmaz
 *  findLocalHost() must not return an object, instead it must return a pointer to avoid useless copies.
 *
 *  Revision 1.38  2011/10/27 14:11:52  micmaz
 *  RESOLVED - issue S1PDGS-2312: CirculationService log illeggibile
 *  http://jira.acsys.it/browse/S1PDGS-2312
 *
 *  Revision 1.37  2011/10/27 09:50:18  micmaz
 *  OPEN - issue S1PDGS-2312: CirculationService log illeggibile
 *  http://jira.acsys.it/browse/S1PDGS-2312
 *
 *  Revision 1.36  2011/09/23 09:14:57  micmaz
 *  removed unused headers.
 *
 *  Revision 1.35  2011/09/09 09:36:10  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.34  2011/08/30 14:04:28  micmaz
 *  removed excerr.
 *
 *  Revision 1.33  2011/08/29 17:12:04  micmaz
 *  synchronized NameIdAssociator creation.
 *
 *  Revision 1.32  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.31  2011/07/22 15:53:48  micmaz
 *  work in progress..
 *
 *  Revision 1.30  2011/07/20 16:09:37  micmaz
 *  removed log
 *
 *  Revision 1.29  2011/07/15 17:10:20  micmaz
 *  work in progress
 *
 *  Revision 1.27  2011/07/14 14:27:45  micmaz
 *  added application logs
 *
 *  Revision 1.25  2011/07/14 12:19:30  micmaz
 *  removed unused code.
 *
 *  Revision 1.24  2011/07/13 17:06:03  micmaz
 *  REOPENED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.22  2011/07/12 14:30:04  micmaz
 *  RESOLVED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.19  2011/07/12 12:45:19  micmaz
 *  RESOLVED - issue S1PDGS-1627: CirculationService problem in implicit mode
 *  http://jira.acsys.it/browse/S1PDGS-1627
 *
 *  Revision 1.18  2011/07/06 16:54:36  micmaz
 *  the implicit circolation service is now managed with smart pointers.
 *  When the application will stop, the main thread waits for the implicit circulation thread.
 *
 *  Revision 1.17  2011/07/06 14:55:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1607: CirculationService destination URL Problem
 *  http://jira.acsys.it/browse/S1PDGS-1607
 *
 *  Revision 1.14  2011/07/01 16:41:20  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.13  2011/06/28 17:27:20  micmaz
 *  wrong makefile
 *
 *  Revision 1.12  2011/06/28 17:04:59  micmaz
 *  Added the application status in order to verify when a policy must be reprocessed
 *
 *  Revision 1.1  2011/06/16 12:50:29  micmaz
 *  work in progress.
 *
 *
 *
 *
 */

#include <ImplicitCirculationService.h>
#include <DiskEventNotifier.h>
#include <Application.h>
#include <RemoteSiteFactory.h>
#include <Repository.h>
#ifdef USE_LIBPDSDB 
  // from libPdsDb
  #include <FileTypeRecognition.h>
#else // from libDbCryosat
  #include <dbFileTypeRecognition.h>
#endif
#include <dbConnectionPool.h>
#include <TransferProtocol.h>
#include <MediaCirculationMngFunctions.h>
#include <CirculationRemoveQueue.h>
#include <LocalTransferMode.h>
#include <CirculationMode.h>
#include <FtpCurl.h>
#include <FtpListReader.h>
#include <RemoteSiteUtility.h>
#include <Transaction.h>
#include <MCFilterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

using namespace std;
/*
using namespace boost;
*/
using namespace acs::mc;
using namespace acs::curl;
using namespace acs::os;
using namespace acs::conf;

ACS_CLASS_DEFINE_DEBUG_LEVEL (ImplicitCirculationService);
ACS_CLASS_DEFINE_DEBUG_LEVEL(ImplicitCirculationService::ItemService) ;

const size_t ImplicitCirculationService::millisecondsToWait_ = 1000;
const time_t ImplicitCirculationService::lastModificationTimeInSecs_ = 60;

namespace {

    class SourceItemsInserter {
    public:
        SourceItemsInserter( const string & sourcePath, 
							set<string> &o ) : 
            out_(o),  // PRQA S 2528 1
			sourcePath_(sourcePath)
        {}
        
        SourceItemsInserter(const SourceItemsInserter &r) : out_(r.out_), sourcePath_(r.sourcePath_) {} // PRQA S 2528
        ~SourceItemsInserter() throw() {}
		
		void operator()(const FtpListItem &item)  
        {
			out_.insert( StringUtils::pathJoin(sourcePath_, item.getName() ) );
        }
    private:
        SourceItemsInserter() ; // not implemented
        SourceItemsInserter& operator=(const SourceItemsInserter &) ; // not implemented
    
		set<string> &out_;
		const string & sourcePath_;
    } ;
	
}  // unnamed

ImplicitCirculationService::ImplicitCirculationService(
        dbConnectionPool& connectionPool, 
        bool enableFilter, 
        bool localPolicies) : 
    Thread(),
    manager_(), 
    connectionPool_(connectionPool), // PRQA S 2528
    policiesFound_(INT_MAX),
    localPathScans_(),
    enableFilter_ (enableFilter),
    localPolicies_ (localPolicies),
	localPoliciesDebugId_()
{
    localPoliciesDebugId_ = localPolicies_ ? "<Local policies> " : "<Remote policies> "; // PRQA S 3380
}

ImplicitCirculationService::~ImplicitCirculationService() throw () {
    // empty
}

void ImplicitCirculationService::run() 
{
    operator()(true);
}

void ImplicitCirculationService::operator()(bool runForever) 
{
    ImplicitCirculationConfigurer *cconf = conf::CirculationConfigurerSingleton::instance() ;
    
	// Must keep track of the failures on the ItemService threads to avoid 
	// too many messages issuing
	map <string, int> urlScanErrors;	

    KnownHost* thisHost = 0;
    DateTime last_poll_time(time(0) - (cconf->getCirculatorPollingPeriod() + 1)) ;

	// The eventsList must survive the notifierGuard destructor
	DiskEventNotifier::EventElementsList eventsList;

    ACS_SMARTPTR<DiskEventNotifier>  notifierGuard;
	Notify::EventsMask evMask = static_cast<Notify::EventsMask> (Notify::NOTIFY_MOVED_TO | Notify::NOTIFY_CLOSE_WRITE) ; // PRQA S 3013, 3081
	
	bool waitToReprocess = false;
	bool theFirstTime = true;	
    
    vector<string> pathsToWatch_old;
	bool logHostException = true;
	Timer mediaCirculationCheckTimer;
	mediaCirculationCheckTimer.start();
	
	map < string, set<string> > localFilepathsToSkipMap;
	
    while (!requestedCancel()) {
		
		try {
			thisHost = findHostToFilter(logHostException);
    		ACS_COND_THROW(0 == thisHost, exIllegalValueException("Current host is not well configured")); // PRQA S 3081
		}
		catch(std::exception &e)
		{
			if( logHostException ) {
				Application::setStatus(Application::Warning, "Exception finding host: " + string(e.what()) ); // PRQA S 3081
				logHostException = false;
			}
			Timer::delay(1000); // PRQA S 4400
			continue;
		}
		
		if( not logHostException ) {
			logHostException = true;
			Application::setStatus(Application::Ok);
		}
		
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "thisHost: " << thisHost);
        vector<CirculationPolicy> dbpolicies ;
        vector<CirculationPolicy> remotepolicies ;
        vector<CirculationPolicy> localpolicies;
		
		
		bool checkQueueRecordsViaMedia = false;
		try {
        	findCirculationPolicies(*thisHost, dbpolicies);

			if( localPolicies_ ) {
				checkQueueRecordsViaMedia = true;
			}

        	if (enableFilter_)
        	{
            	// Local-remote filter is enabled
            	// bi-partition of the policies in locals and remote
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, localPoliciesDebugId_ << "Going to partition policies into local and remote");
            	size_t dbs = dbpolicies.size() ;
                for (size_t idx=0; idx<dbs; ++idx)
            	{
                	RemoteSite sourceSite;
                	makeSourcePath(dbpolicies[idx], sourceSite);
                	// if not local host, go on
                	if(sourceSite.isLocalhost()) 
                	{
                    	localpolicies.push_back (dbpolicies[idx]);
                	}
                	else
                	{
                    	remotepolicies.push_back (dbpolicies[idx]);
                	}
					
            	}
        	}
        	else
        	{
            	// Manage every policy like a local one
            	localpolicies = dbpolicies;
            	localPolicies_ = true;
        	}
		}
		catch ( dbSmartPersistent::ExecutionError &e )
		{
            ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ACS_LOG_NOTIFY_EX(e);
            ACS_CLASS_END_DEBUG 
			ACS_LOG_WARNING("Cannot load record from db, going to reload all circulation policies: " << e.what()) ;
			continue;
		}
			
        // Select which policies have to be considered in this thread
        vector<CirculationPolicy>& policies ( localPolicies_ ? localpolicies : remotepolicies); // PRQA S 3384

        size_t policiesSize = policies.size();
        if (policiesFound_ != policiesSize) {
            string preamble;
            if (enableFilter_)
            {
                preamble = ( localPolicies_ ? "Local" : "Remote"); // PRQA S 3380
            }
            ACS_LOG_INFO( preamble + " Policies found for current host  : " << policiesSize);
            policiesFound_ = policiesSize;
        }

		try {

			if( CirculationConfigurerSingleton::instance()->useInotify() && !theFirstTime ) {
			
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, localPoliciesDebugId_ << "Going to fill paths to watch with inotify");
				
				vector<string> pathsToWatch;
                size_t pol_size = policies.size() ;
				for (size_t i = 0; i < pol_size; ++i) {
            		RemoteSite sourceSite;
            		makeSourcePath(policies[i], sourceSite);

            		// if not local host, go on
					if( !sourceSite.isLocalhost() ) {
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Source Site is not on local host");
						continue;
					}
            		string url = RemoteSiteFactory::toURL(sourceSite);
            		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Source Site url is on local host: \"" << url << "\"");
					string pathToWatch = sourceSite.getPath();
					vector<string>::iterator it = find (pathsToWatch.begin(), pathsToWatch.end(), pathToWatch );
					if( it == pathsToWatch.end() ) {
						pathsToWatch.push_back(pathToWatch);
           				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Path to watch \"" << pathToWatch << "\" added");
					}
					else {
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Path to watch \"" << pathToWatch << "\" already occurring");
					}

				} // paths to watch added

				if( !pathsToWatch.empty() ) {
                    if ( pathsToWatch_old != pathsToWatch)
                    {

                        // Create a new thread only if the paths to observe has been changed
					    notifierGuard.reset(new DiskEventNotifier(pathsToWatch, false, evMask, eventsList)) ; // PRQA S 3050, 3081
					    notifierGuard->setLogExcStack(false);
					    notifierGuard->start();
					    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Notifier just started with " << pathsToWatch.size() << " paths to watch");   

                        size_t pwsize = pathsToWatch.size() ;
                        for (size_t i=0; i < pwsize; ++i) {
                            ACS_LOG_INFO(" Waiting data in path: " << pathsToWatch[i] ) ;
                         }

                        pathsToWatch_old = pathsToWatch;
                    }
				}
			}

        	while ( !requestedCancel() && // PRQA S 3230
                    !manager_.isOlderThan(cconf->getCirculatorPollingPeriod(), last_poll_time) && 
                    ( waitToReprocess || eventsList.empty() ) ) 
            { 
	            // wait in loop until
	            // * stop requested
	            // * cconf->getCirculatorPollingPeriod() seconds elapsed since the last time
	            // * events vector is empty
				ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Waiting in loop...");
            	Timer::delay(100); // PRQA S 4400 
				waitToReprocess = false;
        	}
			if( requestedCancel() ) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stop requested");
				break;
			}
		}
		catch ( dbSmartPersistent::ExecutionError &e )
		{
            ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ACS_LOG_NOTIFY_EX(e);
            ACS_CLASS_END_DEBUG 
			ACS_LOG_WARNING("Cannot load record from db, going to reload all circulation policies: " << e.what()) ;
			continue;
		}	
		
		bool eventsDetected = false;
		waitToReprocess = true; // not to reprocess two consecutive times for inotify
		if( !theFirstTime ) {
			if( not eventsList.empty() ) {
        		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, localPoliciesDebugId_ << "Policies reprocessing triggered by inotify");
				eventsDetected = true;
			}
			else {
        		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, localPoliciesDebugId_ << "Policies reprocessing due to polling period of " << cconf->getCirculatorPollingPeriod() << " seconds.");
			}
		}
		theFirstTime = false;
		
		eventsList.clear();
        while (!policies.empty() && !requestedCancel() ) {
            vector < boost::shared_ptr<ItemService> > handles;
            size_t nthreads =  (localPolicies_ ? cconf->getCirculatorNumberOfThreads() : cconf->getCirculatorNumberOfRemoteThreads()); // PRQA S 3384
            map<string, ItemService*> sites;

            // During the policies scan some policies could be skipped
            // because all the threads have been used by different groups
            // The skipped policies will be saved in this temporary structure and restored
            // in the original policies vector just before closing the while block
            vector<CirculationPolicy> skippedPolicies;
            size_t counter (0);

            // This inner while loop will consume all the policies vector
            // but after its block the skippedPolicies will be used to refill
            // the vector before executing another loop
            // the outer while (apparently on the same condition) will exit in case no policies will be skipped
            while (!policies.empty() && !requestedCancel() ) {
                CirculationPolicy policy = policies.back();
                policies.pop_back();
                
				RemoteSite sourceSite;
				try {
                	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
                    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Process Policy: " << policy.getId());
                	ACS_CLASS_END_DEBUG 
                	// This will not modify the policy content
                	makeSourcePath(policy, sourceSite);
				}
				catch( dbSmartPersistent::ExecutionError &e )
				{
                	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
						ACS_LOG_NOTIFY_EX(e);
                	ACS_CLASS_END_DEBUG 
					ACS_LOG_WARNING("Cannot load record from db, going to process the next circulation policy: " << e.what()) ;
					continue;
				}
				
				if( eventsDetected && !sourceSite.isLocalhost() ) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Events detected but source site not on local host");
					continue;
				}
                string url = RemoteSiteFactory::toURL(sourceSite);
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Source Site url: " << url);
                if (sites.find(url) == sites.end()) {
                    if (counter >= nthreads)
                    {
                      ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Skipping the policy because threads are fully booked");
                      skippedPolicies.push_back (policy);
                    }
                    else
                    {
                      ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Found a new Item Service to add.");
                      time_t now = time(0);
                      time_t checkTime = now; //it is now
                      bool firstUrlOccurence = (localPathScans_.find(url) == localPathScans_.end());
                      if (!firstUrlOccurence)
                      {
                          checkTime = localPathScans_[url] - cconf->getLocalSourceFilesRetrieveDTime();
						  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Local path last scan: " << DateTime(localPathScans_[url]) << " (time_t=" << localPathScans_[url] << ") check time: " << DateTime(checkTime) << " (time_t=" << checkTime << ')' );
                      }
                      localPathScans_[url] = now;
                      ItemService* service = new ItemService(connectionPool_,policy, sourceSite, &manager_, url, !firstUrlOccurence, checkTime, this);
					  map < string, set<string> >::iterator it = localFilepathsToSkipMap.find(url);
					  if( it != localFilepathsToSkipMap.end() ) {
					  	service->setLastRunLocalFiles(it->second);
					  }
                      handles.push_back(boost::shared_ptr < ItemService > (service)); // PRQA S 3081
                      sites[url] = service;
                      ++counter;
                    }
                } else {
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Item service already present: policy added.");
                    ItemService* item = sites.find(url)->second;
                    item->addPolicy(policy);
                    //We use only one thread per source site, thus in this case we do not increment the counter.
                }
            } // end threads services creation
			
			
			// WARNING: if stop requested, erasing of handles is still needed.
            for (map<string, ItemService*>::const_iterator iter = sites.begin(); iter != sites.end() && !requestedCancel(); ++iter) { // PRQA S 4238
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Start producing circulations for the policy(ies): " << iter->second->getIds());
                iter->second->start(); // start every service just created above
            }
            sites.clear(); // no longer needed ;
            if (!handles.empty()) {
                vector<boost::shared_ptr<ItemService> >::iterator iter = handles.begin();
                do {
                    while (iter->get()->running()) {
                        iter->get()->waitExitOrTimeout(50) ; // PRQA S 4400 
                    }
                    // Check if the thread faced issues during the directory scan
                    std::exception* ex = 0;
                    iter->get()->runException(ex);
                    string localURL = iter->get()->getSourcePath();

                    if (0 != ex)
                    {
						// Got a new error on a scan. Take it into account.
						urlScanErrors[localURL]++;
						if ((urlScanErrors[localURL]==1) && (not this->requestedCancel()) )
						{
							// write it in the log
	                        ACS_LOG_ERROR ("Got an exception while scanning " << localURL << ". Exception desription info follows.");
	                        ACS_LOG_INFO  (ex->what());
						}
						else
						{
							// Debug messages
	                        ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Got an exception while scanning " << localURL << ". Attempt# " << urlScanErrors[localURL]);
	                        ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, ex->what());
						}

                        // Removing the URL from the last scan time containers will force the rescan
						// but no more than 10 times
						if (urlScanErrors[localURL] < 10) // PRQA S 4400
						{
	                        ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Forcing a rescan of " << localURL);
	                        localPathScans_.erase (localURL);
						}
                        delete ex;
                    }
					else
					{
						// No exception got. Reset the counter for the URL to allow notification of following temporary errors
						urlScanErrors[localURL] = 0;
						set<string> filepaths ;
						iter->get()->getCurrentRunLocalFiles( filepaths);
						
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The number of current run locally processed file paths that have been saved is " << filepaths.size());
               			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
							for( set<string>::const_iterator it = filepaths.begin(); it != filepaths.end(); it++ ) {
                    			ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << *it << '\"' );
							}
                		ACS_CLASS_END_DEBUG 
						
						localFilepathsToSkipMap[localURL] = filepaths; 
					}
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Removing a finished (Multiple) Circulator Provider from the queue.");
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Here after follows the complete policies list (if the size > 1 the provider is multiple): "
                            << iter->get()->getIds());
                    handles.erase(iter);
                    iter = handles.begin();
                } while(!handles.empty());
            }
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Restoring " << skippedPolicies.size() << " skipped policies in the loop");
            policies=skippedPolicies;

            // This is just to preserve the correct order of the retrieved policies. Maybe useless.
            ::std::reverse (policies.begin(), policies.end());
        }
		
		if( checkQueueRecordsViaMedia ) {
			mediaCirculationCheckTimer.stop();
			if( mediaCirculationCheckTimer.elapsed() >= cconf->getMediaCirculationInfoPeriod() * 1000UL ) { // PRQA S 4400 // PRQA S 3000, 3084
				// query circulations via media protocol
				checkWaitingCircQueueViaMedia( *thisHost );
				mediaCirculationCheckTimer.start();
			}
		}
		
        if (!runForever) {
            break;
        }
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "End of iteration");
    }
}

void ImplicitCirculationService::stopService() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    setHaltLevel(StopController::LAZY);
}

void ImplicitCirculationService::stopImmediateService() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    setHaltLevel(StopController::IMMEDIATE);
}

KnownHost* ImplicitCirculationService::findHostToFilter(bool logException) const { // PRQA S 4214
    return manager_.findLocalhost(dbConnectionWrapper(connectionPool_), true, logException); // PRQA S 3050, 3081
}

void ImplicitCirculationService::findCirculationPolicies(KnownHost const& hostToFilter, // PRQA S 4214
                                                         vector<CirculationPolicy> & policies, 
                                                         bool filterWithApplicationStatus) const 
{
    dbConnectionWrapper w (connectionPool_) ;
    dbConnection &conn = w ; // PRQA S 3050 
    const string &key = conn.getKey() ;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[findCirculationPoliciesWitHost: " << hostToFilter.str(conn) << "]");
    CirculationPolicyQuery q(key) ;
    {
        dbQueryParameters pp(key) ;
        pp.addParameter(dbParam(key, CirculationPolicy::table(),"circulator_node_id", dbParam::EQUAL_TO, dbConv(key, hostToFilter.getId()))) ; // PRQA S 3050
		pp.addParameter(dbParam(key, CirculationPolicy::table(), "status_id" , dbParam::DIFFERENT_FROM, db::toSQL(key, Status::instance(key)->id(Status::disabled) ) )) ;
        q.addParameter(pp) ;
    }
    q.doQuery(conn);
    q.fill(policies) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, localPoliciesDebugId_ <<  "Policies found: " << policies.size());
}

void ImplicitCirculationService::makeSourcePath(CirculationPolicy const& policy, RemoteSite& sourceSite) const { // PRQA S 4214
    dbConnectionWrapper w (connectionPool_) ;
    dbConnection &conn = w ; // PRQA S 3050
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, localPoliciesDebugId_ << " [makeSourcePathWithPolicy: " << policy.str(conn, true) << "]");
    
    Repository repository(conn) ;
    bool null = false ;
    dbPersistent::IdType rep_id = policy.getSourceId(null, conn) ;
    if (not repository.load(conn, rep_id)) { // not found
        ostringstream os ;
        os << "Cannot load repository with id: " << rep_id ;
        ACS_THROW(Repository::ExecutionError(os.str() )) ; // PRQA S 3081
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, localPoliciesDebugId_ << "Repository:" << repository.str(conn));
    bool isNull = 0 ;
    int hostId = repository.getHost(isNull, conn);
    if (isNull) {
        ostringstream os ;
        os << "Repository with id: " << rep_id << " has null host_id" ;
        ACS_THROW(Repository::ExecutionError(os.str() )) ; // PRQA S 3081
    }   
    KnownHost host(conn) ;
    if (not host.load(conn, hostId)) {
        ostringstream os ;
        os << "Cannot load KnownHost with id: " << hostId ;
        ACS_THROW(KnownHost::ExecutionError(os.str() )) ; // PRQA S 3081
    }   
    bool s = false ;
    sourceSite.setHost(host.getHostName(s,conn));
    sourceSite.setUser(repository.getUsername(s,conn));
    sourceSite.setPassword(repository.getPassword(s,conn));
    sourceSite.setProtocol(StringUtils::lowercase(TransferProtocol::instance(conn.getKey())->name(repository.getTransferProtocol(s,conn))));
    sourceSite.setPath(repository.getRemotePath(s,conn));
	
    sourceSite.setIsPasswordEncrypted(true);
}

void ImplicitCirculationService::queryWaitingCircQueueViaMedia( KnownHost const& hostToFilter, map< string, size_t > & waitingCircQueueViaMedia ) const // PRQA S 4214
{
    dbConnectionWrapper w (connectionPool_) ;
    dbConnection &conn = w ; // PRQA S 3050 
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Query waiting circulations queue records to be circulated via media protocol from host:" << hostToFilter.str(conn));
	
// SELECT h.host_name, COUNT(*) AS cnt
// FROM t_circulationsqueue c
// JOIN t_transferprotocols p ON p.id = c.dest_protocol_id AND p.protocol = 'MEDIA'
// JOIN t_knownhosts h ON h.id = c.dest_host_id
// WHERE (c.status_id = 7 OR c.status_id = 15)
// AND c.host_id = 23
// GROUP BY h.host_name

	string dbKey = conn.getKey();

	SQLString sqlString(dbKey, SQLString::SELECT);

	sqlString.addTableList("t_circulationsqueue", "cq");
	sqlString.addSelectList("h.host_name") ;
	{
		SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        tmp.setFunctionName("COUNT"); tmp.addFunctionParameter("*");
		sqlString.addSelectList(tmp);
	}


	{
   		dbQueryParameters pp( dbKey);
 		pp.addParameter(dbParam( dbKey, "tp","id" , dbParam::EQUAL_TO , "cq.dest_protocol_id" )) ;
 		pp.addParameter(dbParam( dbKey, "tp","protocol" , dbParam::EQUAL_TO , db::toSQL(dbKey, "MEDIA") )) ;
		sqlString.addJoinList(SQLDialect::INNER,"t_transferprotocols tp",pp) ;
	}

	{
   		dbQueryParameters pp( dbKey);
 		pp.addParameter(dbParam( dbKey, "h","id" , dbParam::EQUAL_TO , "cq.dest_host_id" )) ;
		sqlString.addJoinList(SQLDialect::INNER,"t_knownhosts h",pp) ;
	}

	{
        dbQueryParameters pp(dbKey);
		pp.setParamOperator( dbQueryParameters::OR ) ;
        pp.addParameter( dbParam(dbKey, "cq", "status_id", dbParam::EQUAL_TO, db::toSQL(dbKey, Status::instance(dbKey)->id(Status::eligible)) ) ); // PRQA S 3050
		pp.addParameter( dbParam(dbKey, "cq", "status_id", dbParam::EQUAL_TO, db::toSQL(dbKey, Status::instance(dbKey)->id(Status::recoverableError)) ) ); // PRQA S 3050
        sqlString.addANDWhereParameter(pp);
	}
	
	{
        dbQueryParameters pp(dbKey);
		pp.addParameter( dbParam(dbKey, "cq", "host_id", dbParam::EQUAL_TO, db::toSQL(dbKey, hostToFilter.getId())) ); // PRQA S 3050
        sqlString.addANDWhereParameter(pp);
	}
	
	sqlString.addGroupByList("h.host_name");
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << sqlString.getSQL() );

	dbQuery query(conn, sqlString, "Waiting media circulations");

	dbSet set ;
	query.execForSet(set) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, set.size() << " t_circulationsqueue records set grouped per host to be circulated by media returned" );

	waitingCircQueueViaMedia.clear();
	dbRow row(dbKey);
	while( set.getNextRow(row) ) {
	
		unsigned short i = 0;
		string hostname ;
		row.column(i++,hostname);
		size_t records = 0 ;
		row.column(i++,records);
		
		waitingCircQueueViaMedia[hostname] = records;
	
	}


}


void ImplicitCirculationService::checkWaitingCircQueueViaMedia( KnownHost const& hostToFilter ) const // PRQA S 4214
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	map< string, size_t > waitingCirQueueViaMedia;
	queryWaitingCircQueueViaMedia( hostToFilter, waitingCirQueueViaMedia );
	for( map< string, size_t >::iterator it = waitingCirQueueViaMedia.begin(); it != waitingCirQueueViaMedia.end(); it++ ) { // PRQA S 4238
		
		string hostname = it->first;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to check hostname \"" << hostname << '\"');
		{
			exFOpEventAlert eventalert(MediaCirculationMngFunctions::getMcStatusTag());
			eventalert << it->second << " circulation queue items found to be circulated towards host \"" <<  hostname << '\"';
			excerr << eventalert;
			excmcf << eventalert;
		}
	}


}
//
//ItemService implementation.
//

ImplicitCirculationService::ItemService::ItemService(
        dbConnectionPool &pool, 
        CirculationPolicy const& policy, 
        RemoteSite const& sourceSite,
        CirculationDBService* manager, 
        string const& sourcePath,
        bool filterByDate,
        const time_t& lastScan,
		StopController *sp) : 
    Thread(),
    pool_(pool), // PRQA S 2528
    policies_(), 
    manager_(manager), 
    sourceSite_(sourceSite), 
    sourcePath_(sourcePath),
    filterByDate_(filterByDate),
    lastScan_(lastScan),
	stopController_(sp),
	lastRunLocalFiles_(),
	currentRunLocalFiles_()
{
    policies_.push_back(policy);
    if (sourcePath_.empty()) {
        sourcePath_ = RemoteSiteFactory::toURL(sourceSite_);
    }
}

ImplicitCirculationService::ItemService::~ItemService() throw () {
    // empty
}

namespace {
    class circInfo
    {
    public:
	    circInfo() :_circId(), _protocol(), _url() {} ;
	    circInfo(const dbPersistent::IdType & id, 
                 const std::string & prot, 
                 const std::string & url) : _circId(id), _protocol(prot), _url(url) 
        {} ;
	    circInfo(const  circInfo& r) : _circId(r._circId), _protocol(r._protocol), _url(r._url) 
        {} ; 
        circInfo& operator= (const circInfo& i)
        {
            if (this != &i)
            {	
	    	    _circId = i._circId;
		        _protocol = i._protocol;
		        _url = i._url;
            }
            return *this;
        }
        
	    ~circInfo() throw() {};
	
        dbPersistent::IdType getId() const throw() { return _circId; }
        const std::string & getProtocol() const throw() { return _protocol; } 
        const std::string & getUrl() const { return _url; }

    private:
	     dbPersistent::IdType _circId;
	     std::string _protocol;
	     std::string _url;
    };

}


bool ImplicitCirculationService::ItemService::matchFType( const FtpListItem &file, const string &ftype, dbConnection &conn) {
	
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "File=\"" << file.getName() << "\" file type=\"" << ftype << '\"')
    // check if fname match the policy ftype
#ifdef USE_LIBPDSDB
   typedef acs::db::FileTypeRecognition Ftr ;
#else
   typedef acs::dbFileTypeRecognition Ftr ;
#endif
    Ftr ftr ; 
    
    return ftr.fnameBelong2Ftype(ftype, file.getName(), conn) ; 

}

void ImplicitCirculationService::ItemService::run() {
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "ItemService::run() for policies : " << policies_.size());
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Working on the same Data Provider for Multiple Policies");
    dbConnectionWrapper w(pool_) ;
    dbConnection &conn = w ; // PRQA S 3050
    vector<FtpListItem> directoryList;
    retrieveSourceFiles(directoryList);
// by definition, the policies have the same source (url)
    
    size_t files = directoryList.size() ;
	if ( sourceSite_.isLocalhost() ) {
		// populate currentRunLocalFiles_ set with at most the last getLocalItemsToSkipMaxNum() items of directoryList 
		reverse_iterator< vector<FtpListItem>::iterator > endIt = directoryList.rend();
		size_t maxItems = conf::CirculationConfigurerSingleton::instance()->getLocalItemsToSkipMaxNum();
		if( files > maxItems ) {
			endIt = maxItems + directoryList.rbegin() ;
		}
		SourceItemsInserter itemsInserter( sourcePath_, currentRunLocalFiles_ );
   		for_each(directoryList.rbegin(), endIt, itemsInserter);
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, currentRunLocalFiles_.size() << '/' << files << " items stored in local files set of current run");
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			for( set<string>::const_iterator it = currentRunLocalFiles_.begin(); it != currentRunLocalFiles_.end(); it++ ) {
            	ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << *it << '\"' );
			}
    	ACS_CLASS_END_DEBUG

	}
	
    for (size_t f= 0 ; (f < files) && !stopController_->requestedCancel(); ++f) {
		
		
		
        FtpListItem &itemFile = directoryList[f] ;

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Processing file: " << itemFile.getName()) ;
    
        vector<size_t> interested_policies ;
        size_t policies = policies_.size() ;
        for (size_t p = 0; (p < policies) && !stopController_->requestedCancel(); ++p) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Policy #" << p << ": " << policies_[p].str(conn,true))
            if (matchFType(itemFile, policies_[p].getFileTypeString(conn), conn)) {
                interested_policies.push_back(p) ;
            }   
        } 
        
        if( stopController_->requestedCancel() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stop requested") ;
			break;
		}
		
        const size_t int_policies = interested_policies.size() ;
        if (int_policies == 0 ) { 
        	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "No policy occurring for file: " << itemFile.getName()) ;
			continue ; 
		}
         
        db::Transaction transaction(conn) ;
        try {
			
			vector<string> itemsDetection;
			vector<string> recordsInsertion;
            // begin transaction 
            transaction.begin_work("ImplicitCirculationInsert");
			bool commitTransaction=true;

	    vector< circInfo > circsInfo;
            for (size_t ip= 0; ip < int_policies; ++ip) {
				
// if immediate stop requested roll back
                ACS_COND_THROW(stopController_->haltImmediate(), ThreadException("Immediate stop requested")) ;  // PRQA S 3081
				
#warning MP: se source o destination fossero locali ???        
                curl::RemoteSite source(RemoteSite::ftp) ;
                curl::RemoteSite destination(RemoteSite::ftp);
                makeSource(itemFile, source);
                CirculationPolicy &policy = policies_[interested_policies[ip]] ;
                string policy_str = policy.str(conn,true);
                makeDestination(policy, directoryList[f], destination);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "For policy: " << policy_str)
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t Verifying and adding source: " << source.str());
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t Verifying and adding destination: " << destination.str());
                bool removeSource = conf::CirculationConfigurerSingleton::instance()->getCirculatorRemoveSources();
		        // get removesource value from t_circulationpolicy record
                if( dbSchemaMetaDataProvider::hasField("t_circulationpolicies", "removesource", conn) ) {
                    bool set = false ;
                    removeSource = policy.getRemoveSource(set, conn);
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "removeSource got from t_circulationpolicies record: " << boolalpha << removeSource);
			    }
			    else {
				    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "removeSource got from configuration: " << boolalpha << removeSource);
			    }
				string detectionMsg;
				string insertionMsg;
				dbPersistent::IdType id = 0 ;
                
				 // the following flag needs to create queue records for a policy where queue records already occurring in not final status, but with undefined persistence (e.g. SUSPENDED) 
				bool allowInsertionsForTheSamePolicy = false;
                bool inserted = insertIntoCirculationQueue(conn, itemFile, policy, source, destination, removeSource, detectionMsg, insertionMsg, id, allowInsertionsForTheSamePolicy);
                commitTransaction = (commitTransaction && ( inserted || allowInsertionsForTheSamePolicy) );
				
                itemsDetection.push_back(detectionMsg);
				recordsInsertion.push_back(insertionMsg);
				circInfo info ( id, destination.getProtocol(), RemoteSiteFactory::toURL(source));
				circsInfo.push_back(info); 
            }
			
            if (commitTransaction) {
				
                size_t cis = circsInfo.size() ;
				for (size_t ic=0; ic<cis; ++ic)
				{
					// APF-273
					// Issue the MC message for the status update
					ACS_LOG_MC_INFO( 
						mc_metric(Metric_IN) << mc_param("filename", itemFile.getName()) << 
						mc_param("queueid", circsInfo[ic].getId()) << mc_param ("fromurl", circsInfo[ic].getUrl())
					) ;
					    
				}
				circsInfo.clear();
		
				for (size_t i = 0; i < int_policies; ++i) {
					ACS_LOG_PRIVINFO(itemsDetection[i]);
					ACS_LOG_INFO(recordsInsertion[i]);
				}
				
				transaction.commit() ;

			}       
            else {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, int_policies << " circulation(s) cannot start for file " << itemFile.getName() << ": some already occurring.");
                transaction.roll_back();
			}
        }
        catch(std::exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
            ACS_LOG_ERROR("An error occurred during processing of file: " << itemFile.getName()) ;
            transaction.roll_back() ;   
			
        }
		

    }
}

bool findFtpListItemByName(FtpListItem const& iter, string const &name) {
    return name == iter.getName() ;
}


void ImplicitCirculationService::ItemService::retrieveSourceLocalFiles(vector<FtpListItem> & sourceFiles) const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filter by date: " << boolalpha << filterByDate_ << ", last run processed files number: " << lastRunLocalFiles_.size());
    ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		for( set<string>::const_iterator it = lastRunLocalFiles_.begin(); it != lastRunLocalFiles_.end(); it++ ) {
            ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << *it << '\"' );
		}
    ACS_CLASS_END_DEBUG
    FtpListReader::createLocalFtpListItems(sourcePath_, sourceFiles, filterByDate_, lastScan_, *stopController_, lastRunLocalFiles_);
}


namespace  // The unnamed namespace
{

bool ftpItemLessThan ( const FtpListItem& a, const FtpListItem& b)
{
	bool retvalue = false;

	// Check, if filled with actual values, the DateTime of the items and
	// in case the DateTime is not filled or in case the datetimes are the same
	// use the embedded string lexicographical comparison operator
	if ( a.getDateTime().isFilled() && b.getDateTime().isFilled() )
	{
		// OK date time are both filled.. can try to use them, but only if are not equal
		if ( a.getDateTime() == b.getDateTime() )
		{
			// Use lexicographical comparison of names
			retvalue = ( a.getName() < b.getName() );
		}
		else
		{
			// Use date-times sorting
			retvalue = ( a.getDateTime() < b.getDateTime() );
		}
	}
	else
	{
		// Date-time are not filled, there is no way to use them
		// use the names lexicographical comparison as fallback solution
		retvalue = ( a.getName() < b.getName() );
	}

	return retvalue;
}

};


void ImplicitCirculationService::ItemService::retrieveSourceFiles(vector<FtpListItem> &sourceFiles) const { // PRQA S 4214, 4020

	// This function has multiple returns to permit interruption in case of cancellation request
	// without impacting the complexity of the function itself

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Retrieve Source Files from site: " << sourceSite_.str());
    vector<FtpListItem> sourceFiles1;
    vector<FtpListItem> sourceFiles2;
    if (sourceSite_.isLocalhost()) {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Retrieve from localhost. ");		

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}

        retrieveSourceLocalFiles(sourceFiles1);
        Timer::delay(millisecondsToWait_);

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}
        retrieveSourceLocalFiles(sourceFiles2);
        Timer::delay(millisecondsToWait_);

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}

        retrieveSourceLocalFiles(sourceFiles);
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Retrieve from remote site. ");
        BandPriority no_priority ;
        FtpCurl curl(no_priority, sourceSite_);
        string errorMessage;

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}
        curl.probeRemoteResource(errorMessage, *stopController_);

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}
        curl.list(sourceFiles1, *stopController_);
        Timer::delay(millisecondsToWait_);

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}
        curl.list(sourceFiles2, *stopController_);
        Timer::delay(millisecondsToWait_);

		// Exit immediately in case of cancellation request
		if (stopController_->requestedCancel())
		{
			return;
		}
        curl.list(sourceFiles, *stopController_);
    }
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Source Files found: " << sourceFiles.size());
	// convert the vectors to a map of structure vectors using
	// the filename as the key (the filename is assumed to be unique as the old algorithm did)
	map < string, vector <FtpListItem> > theFileList;
	accumulateListItems ( sourceFiles,  theFileList );
	accumulateListItems ( sourceFiles1, theFileList );
	accumulateListItems ( sourceFiles2, theFileList );
	
	// Remove the dir and its parent
	theFileList.erase (".");
	theFileList.erase ("..");

	// Keep only the files having the same size in all the 3 passes
	map < string, vector <FtpListItem> >::iterator it = theFileList.begin();
	sourceFiles.clear();

	for (; it != theFileList.end() && !stopController_->requestedCancel(); ++it)  // PRQA S 4238
	{
		if ( isItemStable (it->second) )
		{
			sourceFiles.push_back ( (it->second)[0] );
		}
	}

	// re-order the list according to the modification date (increasing)
	// ACSMAINT-88
	sort (sourceFiles.begin(), sourceFiles.end(), ftpItemLessThan );	

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Source Files filtered to transfer: " << sourceFiles.size());
}

void ImplicitCirculationService::ItemService::accumulateListItems (vector <curl::FtpListItem> const& v, map < string, vector <curl::FtpListItem> > & theMap) const
{ 
	for (size_t idx=0; 
         (idx<v.size()) && !stopController_->requestedCancel(); 
         ++idx)
	{
		theMap[ v[idx].getName() ].push_back ( v[idx] );
	}
}

bool ImplicitCirculationService::ItemService::isItemStable (std::vector < curl::FtpListItem> const & v) const // PRQA S 4020
{
	if (v.size() < 3) // PRQA S 4400
	{
		// not stable - detected just in one or two scans 
		return false;
	}
	else if (v[0].getSize() == v[1].getSize())
	{
		// Has the same size in 2 passes
		// return true if has the same size in the third one
		return ( v[1].getSize() == v[2].getSize() ); // PRQA S 4400
	}
	else
	{
		// Size mismatch - not stable
		return false;
	}
}

void ImplicitCirculationService::ItemService::makeSource(FtpListItem const& item, RemoteSite & site) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[makeSourceWithItem: " << item.str() << "]");
    site = sourceSite_;
    site.setPath(site.getPath() + "/" + item.getName());
}


void ImplicitCirculationService::ItemService::makeDestination(CirculationPolicy const& policy,  // PRQA S 4214
                                                              FtpListItem const& item, 
                                                              RemoteSite & destination) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[makeDestinationWithPolicy: " << policy.str(dbConnectionWrapper(pool_), true) << "]"); // PRQA S 3050, 3081
    dbConnectionWrapper w(pool_) ;
    dbConnection &conn = w ; // PRQA S 3050
	
	Repository repository(conn) ;
    bool null = false ;
    dbPersistent::IdType rep_id = policy.getDestinationId(null, conn) ; 
    if (not repository.load(conn, rep_id )) { // not found
        ostringstream os ;
        os << "Cannot load repository with id: " << rep_id ;
        ACS_THROW(Repository::ExecutionError(os.str() )) ; // PRQA S 3081
    }

    int hostId = repository.getHost(null, conn);
    if (null) {
        ostringstream os ;
        os << "Repository with id: " << rep_id << " has null host_id" ;
        ACS_THROW(Repository::ExecutionError(os.str() )) ; // PRQA S 3081
    }   
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Repository:" << repository.str(conn));
    KnownHost host(conn) ;
    if (not host.load(conn, hostId)) {
        ostringstream os ;
        os << "Cannot load KnownHost with id: " << hostId ;
        ACS_THROW(KnownHost::ExecutionError(os.str() )) ; // PRQA S 3081
    }   
    destination.setHost(host.getHostName(null, conn));
    destination.setUser(repository.getUsername(null,conn));
    destination.setPassword(repository.getPassword(null,conn));
    destination.setProtocol(StringUtils::lowercase(TransferProtocol::instance(conn.getKey())->name(repository.getTransferProtocol(null,conn))));
    destination.setPath(curl::RemoteSiteUtility::composeUrl(repository.getRemotePath(null,conn), item.getName()));
}
/**
 * This method must ensure that a circulation can be created.
 * <P>
 * Steps (every query is executed only on implicit circulations):
 * <P>
 * 1. Verify if there are any not ended circulations with the same from/to url,
 *  if it's so: do not create it (because we are still transferring the same product thus we must not do it twice);
 * <P>
 * 2. Verify if there are ended circulations with the same from/to url,
 *  if it's so: create it because it belongs to a newly created policy (in this case there are no circulations with the same
 *  from/to url in any status, thus create it);
 * <P>
 *  At this step there are some ended circulations with the same from or to url.
 * <P>
 * 3. find every circulation with the same source url and still not ended (possibly with different dest url),
 * In this case, if there is a circulation (still in progress) created before this Circulation Service loop time, then
 * we must not create the new circulation because it would mean that we have already circulated the same product and
 * we must not re-circulate it; othewise the circulation must be created.
 * <P>
 */
bool ImplicitCirculationService::ItemService::alreadyCreated( // PRQA S 4020, 4214
	curl::FtpListItem const& item, 
	dbPersistent::IdType policyId,
	string const& fromURL, 
	string const& destinationURL,
	bool & allowRecordsInsertionForPolicy,
	dbPersistent::IdType hostId) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, 
            "[alreadyCreatedAndRunningWithPolicyId: " << policyId << ", AndFromURL: " << fromURL << ", AndDestinationURL: " << destinationURL << ", hostId " << hostId << "]");

	allowRecordsInsertionForPolicy = false;
    dbConnectionWrapper cw(pool_) ;
    dbConnection &conn = cw ; // PRQA S 3050
    const string &key = conn.getKey() ;
    //find all circulations with the same from/to URLs.
    CirculationQuery loopQuery(key) ;
    

    manager_->findDuplicatedQueues(loopQuery, conn, fromURL, destinationURL, hostId);
    loopQuery.doQuery(conn);
    //check if there is no other circulation , in this case create it.
    if (loopQuery.empty()) {
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "The item is not duplicated.");
		allowRecordsInsertionForPolicy = true;
        return false;
    }
    vector<int> notFinalStatus = manager_->getNotEndStati(key);
    notFinalStatus.push_back(Status::instance(key)->id(Status::error));
    notFinalStatus.push_back(Status::instance(key)->id(Status::mediaWait));
    notFinalStatus.push_back(Status::instance(key)->id(Status::mediaAcknowledge));
	
	vector<short> undefinedPersistenceStatus;
	undefinedPersistenceStatus.push_back(Status::instance(key)->id(Status::suspended));
 
    bool null = false ;
    size_t dup_count = loopQuery.size() ;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Duplicated circulations found: " << dup_count);
    for (size_t counter = 0; counter < dup_count; ++counter) {
        Circulation entity = *loopQuery[counter] ;
		short statusId = entity.getStatus(null, conn);
		
        //if the circulation is not in a final status, do not recreate it.
        if (find(notFinalStatus.begin(), notFinalStatus.end(), statusId) != notFinalStatus.end()) {
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cannot add a new circulation because there is another one at a non-final.");
            return true;
        }
		
		//if the circulation is in a undefinedPersistenceStatus status, do not recreate it, but allow creation of others records associated to the same policy
        if (find(undefinedPersistenceStatus.begin(), undefinedPersistenceStatus.end(), statusId) != undefinedPersistenceStatus.end()) {
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cannot add a new circulation because there is another one at undefined persistence status. Allow possible creation of records associated to the same policy");
			allowRecordsInsertionForPolicy = true;
            return true;
        }

        CirculationRemoveQueueQuery itemsToRemove(key) ;
        dbQueryParameters parameter(key) ;
        parameter.addParameter(
                dbParam(key, CirculationRemoveQueue::table(), "url",
                        dbParam::EQUAL_TO, dbConv(key, fromURL))); // PRQA S 3050
        itemsToRemove.addParameter(parameter, dbGeneralQueryIF::AND);
        itemsToRemove.doQuery(conn) ;
        //if the item is registered to be deleted, do not recreate it.
        if (itemsToRemove.size() > 0) {
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cannot add a new circulation because there is an item in the remove queue.");
            return true;
        }
        //If I could read the time of the last status change of the file, and this time is before the current entity modification, then do not
        //create it because it was already inspected.
        const DateTime &itemDate = item.getDateTime() ;
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			if( itemDate.isFilled() ) {
            	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,  item.getName() << " file time: " << itemDate.getSqlString() << "; record modification time: " << entity.getModificationDate(null, conn).getSqlString());
			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,  item.getName() << " file time not filled");
			}
        ACS_CLASS_END_DEBUG 
        if (itemDate.isFilled() && (itemDate < entity.getModificationDate(null, conn))) { // PRQA S 3230
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cannot add a new circulation because this file status changed before the current circulation has been modified.");
            return true;
        }
    }
    //find if there are any circulations with the same from url and not ended (completed).
    manager_->findAlreadyCreatedQueues(loopQuery, conn, fromURL, "", notFinalStatus, hostId);
    unsigned long size = loopQuery.doQuery(conn);
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Not ended circulation records found: " << size);
    for (unsigned long counter = 0; counter < size; ++counter) {
        DateTime date = loopQuery[counter]->getCreationDate(null , conn);
        DateTime now(time(0) - max(lastModificationTimeInSecs_, conf::CirculationConfigurerSingleton::instance()->getCirculatorPollingPeriod())); 
        ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB," now " << now.YYYYMMDDThhmmss());
        if (date < now) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, 
                    "There is another circulation with the same source url still not ended and created in a previous Circulation Service loop.");
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The circulation must not be created.");
            return true;
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, 
                    "There is another circulation with the same source url still not ended but created during the same Circulation Service loop.");
        }
    }
	
	allowRecordsInsertionForPolicy = true;
    return false;
}

bool ImplicitCirculationService::ItemService::insertIntoCirculationQueue( // PRQA S 4214
        dbConnection& conn, curl::FtpListItem const& item,
        CirculationPolicy const& policy, RemoteSite const& source, RemoteSite const& destination,
        bool removeSource,
		string & detectionMsg,
		string & insertionMsg,
		dbPersistent::IdType & circId,
		bool & allowRecordsInsertionForPolicy ) const {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[insertIntoCirculationQueue]");
    string fromURL = RemoteSiteFactory::toURL(source);
    string toURL = RemoteSiteFactory::toURL(destination);
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[WithPolicy:    " << policy.str(conn, true)<<"]");
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[AndWithSource: " << fromURL<<"]");
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[AndWithDest:   " << toURL<<"]");
    dbPersistent::IdType policyId = policy.getId();
	allowRecordsInsertionForPolicy = false;

	// Retrieve information about host_id (responsible) 
	dbPersistent::IdType hostId = 0;
	if( dbSchemaMetaDataProvider::hasField("t_circulationpolicies", "agent_node_id", conn) && (not policy.nullAgentNode(conn)) ) { // PRQA S 3230
		bool null = false ;
		hostId = policy.getAgentNode(null, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "circulation host id got by t_circulationpolicies record agent_node_id: " << hostId);
	}
	else { // host id got by local host id used to filter t_circulationpolicies records by circulator_node_id
		hostId = manager_->findLocalhost(conn)->getId();
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "circulation host id got by t_circulationpolicies records circulator_node_id: " << hostId);
	}

	bool insert =  not alreadyCreated(item, policyId, fromURL, toURL, allowRecordsInsertionForPolicy, hostId);
    if (insert) {
		
		{
			ostringstream os;
			os << "Detected file " << item.getName() << " to be circulated according to policy id# " << policyId;
			detectionMsg = os.str();
		}
        Circulation circ(conn) ;
        circ.setRemark("Implicit Circulation", conn);
        circ.setRemoveSource(removeSource, conn);
        int key = CirculationMode::instance(conn.getKey())->id(CirculationMode::implicitMode, false);
        circ.setCirculationModeId(key, conn);
        circ.setFromURL(fromURL, conn);
        circ.setToURL(toURL, conn);
		

        circ.setHostId(hostId, conn);
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Policy ID: " << policyId);
        circ.setCirculationPolicy(policyId, conn);
        bool null = false ;
        circ.setPriority(policy.getPriority(null, conn), conn);

		Repository repository(conn) ;
        dbPersistent::IdType rep_id = policy.getDestinationId(null, conn) ;
        if (not repository.load(conn, rep_id)) {
            ostringstream os ;
            os << "Cannot load repository with id: " << rep_id ;
            ACS_THROW(Repository::ExecutionError(os.str() )) ; // PRQA S 3081
        }

		circ.setTempNameValue(repository.getTempNameValue(null, conn), conn);
		circ.setTempNameId(repository.getTempNameMode(null, conn), conn);
		
		int transferModeId = 0;
		if( not policy.nullLocalTransferModeId(conn) ) {
			transferModeId = policy.getLocalTransferModeId(null, conn);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Local transfer mode id got by t_circulationpolicies records with id=" << policyId << ": " << transferModeId);
		}
		else {
			transferModeId = LocalTransferMode::instance(conn.getKey())->id(conf::CirculationConfigurerSingleton::instance()->getLocalTransferMode());
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Local transfer mode id not occurring t_circulationpolicies records with id=" << policyId << ", getting default: " << transferModeId);
		}
		circ.setLocalTransferModeId(transferModeId, conn );
		
        try {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creating new circulation");
            if (destination.isLocalhost()) {
                circ.setDestHostId(manager_->findLocalhost(conn)->getId(), conn);
            } else {
                const std::string &dbKey = conn.getKey() ;
                KnownHostQuery hosts(dbKey) ;
                {
                    dbQueryParameters pp(dbKey) ;
                    pp.addParameter(dbParam(dbKey, "t_knownhosts","host_name", dbParam::EQUAL_TO, db::toSQL(dbKey,destination.getHost()))) ;
                    hosts.addParameter(pp) ;
                }
                hosts.doQuery(conn);
                
                if (not hosts.empty()) {
                    circ.setDestHostId(hosts[0]->getId(), conn);
                }
            }

#ifdef CIRC_STATUS_CHECKED
            circ.setStatus(Status::instance(conn.getKey())->id(Status::standby), conn);
#else //CIRC_STATUS_CHECKED
			short polStatusId = policy.getStatus(null, conn);
			short circStatusId = Status::instance(conn.getKey())->id(Status::eligible);
// A check on circulation policy status is preferred not to insert into circulation queue record a spurious status. Advantage of this approach is immediate identification of problem, disadvantage is a more complex and
// less flexible code. Note that DISABLED policies have been already filtered away.			
			if( polStatusId == Status::instance(conn.getKey())->id(Status::enabled) ) {
				circStatusId = Status::instance(conn.getKey())->id(Status::eligible); 
			}
			else if( polStatusId == Status::instance(conn.getKey())->id(Status::suspended) ) {
				circStatusId = Status::instance(conn.getKey())->id(Status::suspended); 
			} 
			else if( polStatusId == Status::instance(conn.getKey())->id(Status::blocked) ) {
				circStatusId = Status::instance(conn.getKey())->id(Status::blocked); 
			} 
			else {
				ostringstream exMsg; 
				exMsg << "policy id# " << policyId << " status not allowed: " << Status::instance(conn.getKey())->name(polStatusId) << ". Allowed statuses are ENABLED, SUSPENDED, BLOCKED";
				ACS_THROW( exIllegalValueException(exMsg.str()) );						
    		}
			circ.setStatus(circStatusId, conn);
#endif //CIRC_STATUS_CHECKED

#ifdef CIRC_STATUS_CHECKED
	        {
		        // Transaction automatically rolls-back in case of exceptions
		        db::Transaction transaction (conn);
		        transaction.begin_work ("CIRC_INSERT");
		        // Create in Standby 
		        circ.save(conn);
				
				short polStatusId = policy.getStatus(null, conn);
				short circStatusId = Status::instance(conn.getKey())->id(Status::eligible);
	// A check on circulation policy status is preferred not to insert into circulation queue record a spurious status. Advantage of this approach is immediate identification of problem, disadvantage is a more complex and
	// less flexible code. Note that DISABLED policies have been already filtered away.			
				if( polStatusId == Status::instance(conn.getKey())->id(Status::enabled) ) {
					circStatusId = Status::instance(conn.getKey())->id(Status::eligible); 
				}
				else if( polStatusId == Status::instance(conn.getKey())->id(Status::suspended) ) {
					circStatusId = Status::instance(conn.getKey())->id(Status::suspended); 
				} 
				else if( polStatusId == Status::instance(conn.getKey())->id(Status::blocked) ) {
					circStatusId = Status::instance(conn.getKey())->id(Status::blocked); 
				} 
				else {
					ostringstream exMsg; 
					exMsg << "policy id# " << policyId << " status not allowed: " << Status::instance(conn.getKey())->name(polStatusId) << ". Allowed statuses are ENABLED, SUSPENDED, BLOCKED";
					ACS_THROW( exIllegalValueException(exMsg.str()) );						
    			}
				circ.setStatus(circStatusId, conn);
				
		        circ.update(conn);
		        transaction.commit();
	        }
#else //CIRC_STATUS_CHECKED
            circ.save(conn);
#endif //CIRC_STATUS_CHECKED

			{
				// log with no password
				ostringstream os;
				os << "Circulation id# " << circ.getId() << " of " << item.getName() << " FROM " << RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(fromURL, true), false) << " TO " 
						<< RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(toURL, true), false) << " policy id# " << policyId << ' ' << Status::instance(conn.getKey())->name(circ.getStatus(null, conn));
				insertionMsg = os.str();
			}

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Created new CirculationQueue: " << circ.str(conn, true));
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " from Policy:    " << policy.str(conn, true));

        } catch (std::exception const& ex) {
			ACS_LOG_ERROR("Error trying to create new CirculationQueue for file " << item.getName() << ": " << circ.str(conn));
            throw;
        }
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "New CirculationQueue for item \"" << fromURL << "\" will not be inserted because already created by the same Policy (id=" << policyId << 
							"). Records insertion for the same policy is " << (allowRecordsInsertionForPolicy ? "" : "NOT ") << "allowed" );
    }
	circId = policyId;
	
	return insert;
}


void ImplicitCirculationService::ItemService::addPolicy(CirculationPolicy const& policy) 
{
	ACS_COND_THROW(running(),ThreadException("Thread already running.")) ; // PRQA S 3081
    policies_.push_back(policy);
}

string ImplicitCirculationService::ItemService::getIds() const { // PRQA S 4214
    stringstream results;
    results << "[ ";
    for (vector<CirculationPolicy>::const_iterator iter = policies_.begin(); iter != policies_.end(); ++iter) {
        results << iter->getId(); // PRQA S 3050
        results << " ";
    }
    results << "]";
    return results.str();
}

void ImplicitCirculationService::ItemService::setLastRunLocalFiles(const std::set<std::string> & v)
{
	ACS_COND_THROW(running(),ThreadException("Thread already running.")) ; // PRQA S 3081
	lastRunLocalFiles_ = v;
}

void ImplicitCirculationService::ItemService::getCurrentRunLocalFiles( std::set<std::string> & v) const
{
	ACS_COND_THROW(running(),ThreadException("Thread already running.")) ; // PRQA S 3081
	v = currentRunLocalFiles_ ;
}


_ACS_END_NESTED_NAMESPACE;
