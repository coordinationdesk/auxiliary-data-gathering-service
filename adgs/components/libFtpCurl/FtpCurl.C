// PRQA S 1050 EOF
/*

 Copyright 1995-2021, Exprivia SpA.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.com

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia SpA, Inc.

 $Prod: A.C.S. libFtpCurl Library $

 $Id$

 $Author$

 $Log$
 Revision 5.62  2018/06/22 14:37:44  lucio.pulvirenti
 APF-426: bug fixed in probeRemoteResource method: if emitException flag is true, set original path in catch block before throwing exc.

 Revision 5.61  2018/03/21 13:17:16  chicas
 Added CURLE_FTP_WEIRD_PASV_REPLY for RecoverableError

 Revision 5.60  2017/10/19 15:14:20  marpas
 qa warning removed
 code best practice implemented

 Revision 5.59  2017/04/12 13:04:10  marfav
 Added configurable support to fast probing
 avoiding to "ls" the entire site but consuming two connections per probe

 Revision 5.58  2017/04/11 16:31:06  marfav
 work in progress

 Revision 5.57  2017/04/06 15:35:52  marfav
 S2PDGS-1748
 Work In Progress

 Revision 5.56  2017/02/14 16:15:57  marpas
 Curl::post and Curl::httpget signature is more robust now

 Revision 5.55  2016/10/04 14:42:11  marfav
 OPMAN-196
 Management of basenames and dirnames now relies on libFile
 Class FtpDirUtility removed from the library

 Revision 5.54  2016/03/26 21:42:30  marpas
 using new db I/F

 Revision 5.53  2016/03/22 16:27:45  francesco.avanzi
 S2PDGS-1434: put and putRneame  modified to remove targetFinalFile if needed

 Revision 5.52  2016/03/21 18:25:54  francesco.avanzi
 WORKINPROGRESS s2pdgs-1434

 Revision 5.51  2016/03/18 13:16:29  francesco.avanzi
  S2PDGS-1434 FtpCurl::put modified: a bool is now taken and passed to Curl::put to regulate target cleanup

 Revision 5.50  2016/03/15 11:18:44  marfav
 Extending support to older libCurl version with specific defines

 Revision 5.49  2016/03/07 15:33:55  marfav
 S2PDGS-1428, PDSEV-28
 Stop controllers managed during FTP file listing and collection

 Revision 5.48  2015/09/18 15:03:50  matteo.airoldi
 Symbolic link: abosulte path handling (only relative was supported).

 Revision 5.47  2015/09/18 12:43:38  nicvac
 Symbolic link: abosulte path handling (only relative was supported)

 Revision 5.46  2015/08/21 14:53:16  marfav
 S2PDGS-1295 added a new code to the retry-able ones (QUOTE ERROR)

 Revision 5.45  2015/05/06 16:13:29  matteo.airoldi
 definition and use of various CURL symbolic error codes is conditioned to the actual curl installed in the system at compilation time.

 Revision 5.44  2015/04/23 17:45:55  marpas
 when putting a file to a remote system, in case o renaming, the latter is performed in the same connection that actually transfered the file.
 the concept is still valid for stream transfers where the endpoint is a file.
 Even the final chmod is done accordingly (valid only for FtpCurl transfers, not fo CirculationCurl ones).
 It does not applies to directory because their children have to be transferred BEFORE the renaming.

 Revision 5.43  2015/04/23 11:54:35  marpas
 coding best practices applied
 qa warning fixed

 Revision 5.42  2015/04/16 13:22:13  marpas
 qa warnings rfixing in progress

 Revision 5.41  2015/04/16 13:06:38  marpas
 statistical methods rationalization

 Revision 5.40  2015/04/09 15:55:38  marfav
 S1PDGS-31623 adding HTTP GET operation (on string)

 Revision 5.39  2015/03/10 16:43:15  lucio.pulvirenti
 S2PDGS-984: CURLE_FTP_ACCEPT_TIMEOUT defined for version < 7.24. CURLE_NO_CONNECTION_AVAILABLE removed (internal use only).

 Revision 5.38  2015/03/10 14:51:50  lucio.pulvirenti
 S2PDGS-984: checkCURLcode added to IF. FtpCurlRecoverableErrorException and FtpCurlUnrecoverableErrorException defined to identify macro exc groups.
 Code analyzed to check where throwing FtpCurlUnrecoverableErrorException.
 throwCurlSpecificException: cURL codes checked to launch either FtpCurlRecoverableErrorException or FtpCurlUnrecoverableErrorException

 Revision 5.37  2015/03/09 17:00:20  lucio.pulvirenti
 S2PDGS-984: work in progress.

 Revision 5.36  2015/03/06 16:45:55  lucio.pulvirenti
 S2PDGS-984: work in progress.

 Revision 5.35  2014/12/09 11:40:45  francesco.avanzi
 modified to use new ContentType

 Revision 5.34  2014/11/18 15:51:29  lucio.pulvirenti
 S2PDGS-915: ouput of ls by http protocol managed to generate a known format xml file to be read to properly instance FtpListItem objects.

 Revision 5.33  2014/11/14 16:34:23  lucio.pulvirenti
 S2PDGS-915: work in progress.

 Revision 5.32  2014/11/13 16:49:51  lucio.pulvirenti
 S2PDGS-915: Work in progress.

 Revision 5.31  2014/10/30 10:32:32  marpas
 qa warnings removed

 Revision 5.30  2014/10/29 12:02:11  lucio.pulvirenti
 S2PDGS-955: setFtpProxy method removed: Proxy class introduced in RemoteSite.h

 Revision 5.29  2014/10/28 16:31:43  lucio.pulvirenti
 S2PDGS-955 work in progress.

 Revision 5.28  2014/08/07 08:17:55  lucio.pulvirenti
 S2PDGS-836: FtpCurlInitializationFailedException thrown if CURLE_FAILED_INIT error returned not to log exception description in error. In probeRemoteResource
 if FtpCurlLoginDeniedException caught no exception description logged in error.
 S2PDGS-835: Exceptions notificated by the proper macro not to send stack on syslog.

 Revision 5.27  2014/07/03 09:59:53  marpas
 qa warning fixed

 Revision 5.26  2014/07/03 07:41:45  marpas
 useless method removed

 Revision 5.25  2014/06/27 13:46:48  marpas
 deprecated methods removed: setRenameTarget setCreateRemoteDirFlag

 Revision 5.24  2014/06/20 14:52:33  marpas
 useless private variables removed
 statistics management (log) fixed
 statistics management (t_circulationstats) fixed
 operation are now an enum

 Revision 5.23  2014/06/19 18:03:16  francesco.avanzi
 no longer \'using namespace acs::curl::temp\'
 FtpListReader no longer dynamically allocated

 Revision 5.22  2014/06/05 18:45:58  marpas
 getting rid of asserts
 interface robustness improved
 tests fixed

 Revision 5.21  2014/06/04 17:29:21  marfav
 QA warning removed
 HTTP sessions managed without SSL layer
 CURL locking callback functions strategy revised

 Revision 5.20  2014/03/12 11:10:53  francesco.avanzi
 minor issue taken in account

 Revision 5.19  2014/02/24 20:48:41  marpas
 simplified

 Revision 5.18  2014/02/21 16:43:32  lucio.pulvirenti
 Useless defined removed and performance method of member used.

 Revision 5.17  2014/02/20 09:41:40  lucio.pulvirenti
 passiveActiveDiscover bug fixed: if exception thrown, host element in discovering vector was not erased, causing hanging of other threads trying to probe the same host.

 Revision 5.16  2014/02/14 13:54:31  marpas
 attempt to refactor band shaping mechanism

 Revision 5.15  2014/02/11 17:39:52  lucio.pulvirenti
 The same host was probed many times, now it is probed just once.

 Revision 5.14  2014/02/11 14:00:53  lucio.pulvirenti
 Debug improved.

 Revision 5.13  2014/02/11 10:14:49  marpas
 proning remote site is synchronized for each specific site

 Revision 5.12  2014/02/10 17:21:58  lucio.pulvirenti
 Mutex must be static because check static map access.

 Revision 5.11  2014/02/07 18:47:33  marpas
 adopting ACS_THROW and ACS_COND_THRO macros

 Revision 5.10  2014/01/31 11:56:28  marpas
 interface rationalization, robustness improved

 Revision 5.9  2013/12/17 18:09:22  marpas
 minor changes

 Revision 5.8  2013/12/10 13:44:26  lucio.pulvirenti
 MP: Work in progress.

 Revision 5.7  2013/11/21 14:45:22  marpas
 statistics in case of local files - media too

 Revision 5.6  2013/10/28 13:42:14  marpas
 cleaning macros and curl versions.

 Revision 5.5  2013/10/24 18:27:59  marpas
 work in progress

 Revision 5.4  2013/10/23 17:28:11  marpas
 work in progress

 Revision 5.3  2013/10/22 17:31:50  marpas
 work in progress

 Revision 5.2  2013/07/23 17:10:11  marpas
 classes rationalization and robustness improved

 Revision 5.1  2013/06/18 12:54:14  marpas
 debug macros rationalized

 Revision 5.0  2013/06/06 18:08:20  marpas
 adopting linException 4.x standards

 Revision 2.38  2013/04/17 20:37:56  marpas
 interface robustness, statistics management improved

 Revision 2.37  2013/04/16 16:12:28  marpas
 getDegaultConnectionPool call fixed

 Revision 2.36  2013/04/16 16:08:01  marpas
 getDegaultConnectionPool call fixed

 Revision 2.35  2013/04/15 09:12:34  marpas
 coding best practices applied
 interfaces rationalization and simplification

 Revision 2.34  2013/04/03 15:39:23  marpas
 EntityBean and related class interface changed

 Revision 2.33  2013/02/28 13:30:31  marpas
 libMessage dependencies removed

 Revision 2.32  2013/02/14 16:28:31  micmaz
 executed regression tests.

 Revision 2.31  2013/01/15 11:22:32  micmaz
 work in progress

 Revision 2.30  2012/12/05 10:31:19  marpas
 getting rid of FilenameUtils

 Revision 2.29  2012/12/03 16:56:48  marpas
 using new StringUtils interface

 Revision 2.28  2012/11/28 18:16:23  marpas
 qa rules,
 optimization and robustness
 still work in progress

 Revision 2.27  2012/09/06 15:55:23  micmaz
 added two new paramters: low speed time and low speed limit in order to stop a low transfer.

 Revision 2.26  2012/06/06 18:02:07  micmaz
 IN PROGRESS - issue S1PDGS-3416: Gestione permessi dei files scaricati tramite ISMClient
 http://jira.acsys.it/browse/S1PDGS-3416

 Revision 2.25  2012/06/06 13:47:50  micmaz
 IN PROGRESS - issue S1PDGS-3416: Gestione permessi dei files scaricati tramite ISMClient
 http://jira.acsys.it/browse/S1PDGS-3416

 Revision 2.24  2012/06/01 10:10:43  lucio.pulvirenti
 LP Work in progress: overloaded get method added taking mode_t parameter as input. Change of
 permission for got file still to be implemented.

 Revision 2.23  2012/05/30 18:23:49  micmaz
 added chmod to FtpCurl::put()

 Revision 2.22  2012/03/28 17:06:30  micmaz
 work in progress

 Revision 2.21  2012/03/28 12:59:18  micmaz
 work in progress

 Revision 2.20  2012/03/27 18:08:49  micmaz
 work in progress

 Revision 2.19  2012/03/27 10:52:22  micmaz
 work in progress

 Revision 2.18  2012/03/16 16:20:32  micmaz
 the remote directory test fails

 Revision 2.17  2012/03/16 15:49:29  micmaz
 work in progress

 Revision 2.16  2012/03/15 14:35:13  micmaz
 *** empty log message ***

 Revision 2.15  2012/03/15 11:45:26  micmaz
 work in progress

 Revision 2.14  2012/03/08 14:22:46  micmaz
 updated tests to cppunit

 Revision 2.13  2012/03/08 13:11:40  micmaz
 added media functionality

 Revision 2.11  2012/02/13 16:06:57  micmaz
 IN PROGRESS - issue S1PDGS-2902: The Circulation takes a core dump when trying to transfer a file without correct rights
 http://jira.acsys.it/browse/S1PDGS-2902

 Revision 2.10  2012/02/13 15:03:45  micmaz
 removed log info

 Revision 2.9  2012/02/09 16:48:24  micmaz
 added ftps CCC  configuration parameter.

 Revision 2.8  2012/02/08 16:18:10  micmaz
 added probe method in order to verify if the remote site (ftp/ftps) is passive/active.

 Revision 2.7  2012/02/07 15:49:26  micmaz
 When transferring a directory FtpCurl must not create immediately the destination dir.

 Revision 2.6  2012/02/07 10:25:47  micmaz
 removed warnings .

 Revision 2.5  2012/02/02 17:19:58  micmaz
 IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 http://jira.acsys.it/browse/S1PDGS-2769

 Revision 2.4  2012/01/23 17:00:52  micmaz
 removed logs.

 Revision 2.3  2012/01/20 16:06:20  micmaz
 added statistics.

 Revision 2.2  2011/12/13 10:20:09  micmaz
 IN PROGRESS - issue S1PDGS-2595: CirculationAgent problem with the temporary directory
 http://jira.acsys.it/browse/S1PDGS-2595

 Revision 2.1  2011/12/12 17:52:37  micmaz
 IN PROGRESS - issue S1PDGS-2595: CirculationAgent problem with the temporary directory
 http://jira.acsys.it/browse/S1PDGS-2595

 Revision 2.0  2011/10/18 14:43:50  marpas
 new statistics adopted

 Revision 1.109  2011/09/29 18:33:04  marpas
 fixing codecheck crashes

 Revision 1.108  2011/09/22 15:08:52  micmaz
 updated tests.

 Revision 1.107  2011/09/16 13:20:59  micmaz
 reworked namespaces.

 Revision 1.106  2011/09/08 13:12:12  micmaz
 IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 http://jira.acsys.it/browse/S1PDGS-1605

 Revision 1.105  2011/09/08 09:28:23  micmaz
 IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 http://jira.acsys.it/browse/S1PDGS-1605

 Revision 1.104  2011/08/26 15:52:05  micmaz
 IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 http://jira.acsys.it/browse/S1PDGS-1604
 IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 http://jira.acsys.it/browse/S1PDGS-1808

 Revision 1.103  2011/07/18 12:46:53  micmaz
 corrected the putDir() wrapped dir problem.

 Revision 1.102  2011/07/16 13:36:52  micmaz
 progress...

 Revision 1.101  2011/07/15 17:08:26  micmaz
 work in progress

 Revision 1.100  2011/07/15 09:24:55  micmaz
 work in progress..

 Revision 1.99  2011/07/14 14:26:16  micmaz
 updated getDir()

 Revision 1.98  2011/07/14 14:25:10  micmaz
 *** empty log message ***

 Revision 1.94  2011/07/08 17:49:09  micmaz
 RESOLVED - issue S1PDGS-1650: CirculationAgent problem remove source
 http://jira.acsys.it/browse/S1PDGS-1650

 Revision 1.93  2011/07/05 12:15:15  micmaz
 work in progress.

 Revision 1.91  2011/07/01 17:04:20  micmaz
 added two deprecated methods in order to ensure compatibility.

 Revision 1.90  2011/07/01 16:42:11  micmaz
 changed the temporary file name strategy.

 Revision 1.89  2011/06/24 16:28:41  micmaz
 work in progress

 Revision 1.85  2011/05/30 18:01:14  micmaz
 correct putDir()

 Revision 1.84  2011/05/30 15:24:45  micmaz
 added localhost as valid remote host

 Revision 1.83  2011/05/13 08:55:50  micmaz
 work in progress

 Revision 1.79  2011/05/11 15:47:53  micmaz
 rules modifications

 Revision 1.78  2011/05/11 13:52:47  micmaz
 corrected the rollback for put and get operations

 Revision 1.77  2011/05/10 17:10:46  micmaz
 corrected bug of ProgressData callback: destroyed while working.

 Revision 1.76  2011/05/10 12:58:27  micmaz
 added getFullLocalFilename() and setFullLocalFilename() methods

 Revision 1.75  2011/05/09 16:24:45  micmaz
 work in progress...

 Revision 1.72  2011/05/04 09:52:10  micmaz
 added a method to decide if a  remote resource exits and if it is a file or directory.

 Revision 1.71  2011/05/03 14:01:34  micmaz
 corrected some circulation bugs and add CurlStatistics class

 Revision 1.70  2011/04/29 17:20:41  micmaz
 check style changes

 Revision 1.69  2011/04/15 15:33:22  micmaz
 work in progress...

 Revision 1.64  2011/03/09 13:53:12  micmaz
 added new test and FtpCirculation class.

 Revision 1.63  2011/03/08 15:04:08  micmaz
 improved logs.

 Revision 1.62  2011/03/07 15:22:03  micmaz
 added the new remote file not found error code from curl ( for versions prior than 1.19)

 Revision 1.61  2011/03/03 14:28:46  micmaz
 work in progress

 Revision 1.60  2011/03/02 18:53:00  micmaz
 corrected crash when log performance during/after closing connection.

 Revision 1.59  2011/03/01 17:32:12  micmaz
 removed boost::thread for  added acs::Thread

 Revision 1.58  2011/02/24 11:41:10  micmaz
 Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.

 Revision 1.57  2011/02/23 14:39:26  micmaz
 Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.

 Revision 1.56  2011/02/10 15:53:32  micmaz
 added statistics.

 Revision 1.55  2011/02/04 11:31:46  micmaz
 work in progress...

 Revision 1.51  2011/02/02 10:16:06  micmaz
 corrected the list callback function.

 Revision 1.50  2011/02/01 15:12:14  micmaz
 work in  progress...

 Revision 1.47  2011/01/27 16:13:28  micmaz
 adapted the library to old libcURL 1.15

 Revision 1.46  2011/01/27 09:50:21  micmaz
 work in progress...

 Revision 1.42  2011/01/19 16:42:59  micmaz
 added the openConnection() and closeConnection() to cURL operations.

 Revision 1.41  2011/01/13 14:00:58  micmaz
 changed file permissions

 Revision 1.40  2011/01/13 10:57:28  micmaz
 added mput() and mget() method in order to open only one connection if you must trasfer more than one file to/from the same host.

 Revision 1.39  2010/12/21 18:11:14  micmaz
 corrected the rename procedure.

 Revision 1.38  2010/12/20 16:12:46  lucio.pulvirenti
 FtpCurlWrite: handled bytes written number in order to remove got files
 of zero-length in case of errors

 Revision 1.37  2010/12/20 11:28:17  micmaz
 renamed some methods.

 Revision 1.36  2010/12/17 16:09:33  lucio.pulvirenti
 Exception derived by FtpCurlException base class added to handle different
 error codes by ftpcurl
 errno returned only when effectively available (previously a misleading value was returned)
 Ftpfile open method added explicitely called to handle 0-length files
 (open removed from FtpCurlFwrite callback function)
 FtpCurlFwrite and FtpCurlBwrite now return 0 in case of error accordingly to ftpcurl protocol
 throwCurlSpecificException method added to handle and throw different exceptions

 Revision 1.35  2010/11/24 13:04:54  marpas
 FTP_PROXY env var used by curl now controlled via setFtpProxy method and set to empty string by default
 debug improved

 Revision 1.34  2010/11/11 16:17:24  micmaz
 added the rename remote directory function.

 Revision 1.33  2010/11/04 10:16:06  micmaz
 added the slowDown field in order to slow the file transfer.

 Revision 1.32  2009/10/12 08:44:35  crivig
 sftp implemented and tested

 Revision 1.31  2009/10/07 15:32:06  marpas
 diagnostics messages improved
 _perfo are stopped in case of error too.

 Revision 1.30  2009/10/06 15:33:10  marpas
 list method completed
 performances added

 Revision 1.29  2009/10/06 09:51:36  marpas
 passive mode implemented
 rules applied

 Revision 1.28  2008/09/05 09:20:08  manuel.maccaroni
 better exception thrown message for non-existent local paths when using the get() method.

 Revision 1.27  2008/09/04 11:52:44  manuel.maccaroni
 get() method throws an exception if the directory in the local path does not exist

 Revision 1.26  2008/06/13 13:00:32  manuel.maccaroni
 First release of documnted version.
 Section usage to be added.

 Revision 1.25  2008/06/12 07:54:12  manuel.maccaroni
 Fixed an issue in the default constructor which generated segmentation fualts when the user did not introduce and set a custom progress callback.

 Introduced some exception handling inside the callbacks.

 Revision 1.24  2008/06/11 19:14:56  paoscu
 Fixed segmentation fault when using default constructor

 Revision 1.23  2008/06/09 13:09:31  manuel.maccaroni
 Custom progress callback added.

 Revision 1.22  2008/06/09 12:19:30  manuel.maccaroni
 Fixed an issue that didn't permit to use paths ending without a /

 Revision 1.21  2008/06/09 10:06:16  manuel.maccaroni
 Added a small progress indicator message in the progress callback.

 Revision 1.20  2008/06/09 08:08:50  manuel.maccaroni
 StopController check implemented inside callback.

 Revision 1.19  2008/06/06 14:24:02  manuel.maccaroni
 first implentation of stop controlling in a callback (to be refined).

 Revision 1.18  2008/06/06 13:27:13  manuel.maccaroni
 set(), get() and del() now use a StopController as a defaulted reference in the argument list.

 Revision 1.17  2008/06/06 08:39:39  manuel.maccaroni
 set(), get() and del() now use a StopController as a defaulted reference in the argument list.

 Revision 1.16  2008/06/05 12:30:22  manuel.maccaroni
 better implementation of present/non present _remoteFile for put() using ternary operator; in order to be consistent with the get() method.

 Revision 1.15  2008/06/05 12:14:32  manuel.maccaroni
 fixed the issue with put() method when it was requested to perform a direct upload (withuot using a temporary intermediate file).

 Revision 1.14  2008/06/05 11:41:51  manuel.maccaroni
 get() method downloads a file with the same name as the source if no target name has been provided (that is _localFile is empty),
 if not then the file is downloaded as specified (_localFile).

 Revision 1.13  2008/06/05 11:16:08  manuel.maccaroni
 put function checks a flag to allow or not remote missing directory creation.
 This flag is a bool private class attribute (_createRemoteDirFlag) defaulted to false by the class constructor.

 Revision 1.12  2008/06/05 09:43:20  manuel.maccaroni
 minor fixes in get() method

 Revision 1.11  2008/06/05 09:14:03  matteo.airoldi
 minor changes

 Revision 1.10  2008/06/05 09:05:38  matteo.airoldi
 Modified set/get renameTarget

 Revision 1.9  2008/06/05 08:17:05  manuel.maccaroni
 More consistent order in constructor's list of arguments.
 Corrected a minor issue in put() for arguments check (exception mechanism)

 Revision 1.8  2008/06/04 13:24:21  matteo.airoldi
 Put with RNFR RNTO fixed

 Revision 1.7  2008/06/04 10:38:34  manuel.maccaroni
 minor issues corrected

 Revision 1.6  2008/06/03 14:18:41  manuel.maccaroni
 Better implementation of target renomination logic: a bool private attribute defaulted to true along with getter and setter methods are introduced.

 Revision 1.5  2008/06/03 13:45:37  manuel.maccaroni
 String argument checking and diagnostics replaced with an exception-based mechanisms for del, put and get methods

 Revision 1.4  2008/06/03 13:04:16  manuel.maccaroni
 Introduced a bool parameter (defaulted to true) to control target file rename during a put (to be tested).

 Revision 1.3  2008/06/03 11:36:49  manuel.maccaroni
 Introduced _localFile and _remoteFile private attributes,
 in order to make it possible to control over remote and local file resources; therefore _file attribute has been removed.
 Provided set and get methods for the two attributes defined above.
 Fixed minor incongruences in comments and exception trown messages.

 Revision 1.2  2008/06/03 09:52:35  manuel.maccaroni
 Default constructor added.

 Revision 1.1.1.1  2008/05/14 12:44:00  matteo.airoldi
 libFtpCurl first Issue


 */

#include <FtpCurl.h>
#include <RemoteSiteFactory.h>
#include <CurlConfigurer.h>
#include <FtpListReader.h>
#include <Application.h>
#include <dbConnectionPool.h>
#include <StringUtils.h>
#include <ftarstream.h>
#include <File.h>
#include <Filterables.h>
#include <Dir.h>
#include <ftmpstream.h>
#include <SpawnProc.h>
#include <XMLIstream.h>
#include <XMLIstringstream.h>
#include <rsPushNSpace.h>

#include <lfcV.h>

#include<boost/shared_ptr.hpp> // PRQA S 1013

#include <deque>

_ACS_BEGIN_NAMESPACE(acs) 

using namespace acs::curl;
using namespace std;

using acs::curl::Curl;
namespace {
    lfcV _module_version_ ;
    
    DateTime creationdatePropFindWrapper( const string & creationDate )
    {
        // manage creation date: format is e.g. 2019-12-03T11:54:26Z (Zulu time) or 1997-12-01T18:27:21-08:00 (time zone 8 hours west of GMT). 
        DateTime output(creationDate.substr(0, 19)) ;
        // To manage: transformation in GMT if delta hours occur"
        return output;
    }
    
    
    bool propFindItemOrderingByName ( const FtpCurl::PropFindStatItem& a, const FtpCurl::PropFindStatItem& b)
    {
        return ( a.getName() < b.getName() ) ;
    }
    
    bool propFindItemOrderingByCreationDate ( const FtpCurl::PropFindStatItem& a, const FtpCurl::PropFindStatItem& b)
    {
        bool isSet;
        return ( a.getCreationDate(isSet) < b.getCreationDate(isSet) ) ;
    }

    bool propFindItemOrderingByModificationDate ( const FtpCurl::PropFindStatItem& a, const FtpCurl::PropFindStatItem& b)
    {
        bool isSet;
        return ( a.getModificationDate(isSet) < b.getModificationDate(isSet) ) ;
    }
    
    map< FtpCurl::PropFindRequests, string > getPropFindRequestsMap() 
    { 
        map<FtpCurl::PropFindRequests, string>  m;

        m[FtpCurl::PROPFINDREQ_TYPE]            = "resourcetype";
        m[FtpCurl::PROPFINDREQ_LENGTH]          = "getcontentlength";
        m[FtpCurl::PROPFINDREQ_CREATION]        = "creationdate";
        m[FtpCurl::PROPFINDREQ_MODIFICATION]    = "getlastmodified";

        return m;
    } 
    
    const map< FtpCurl::PropFindRequests, string > propFindRequestsMap = getPropFindRequestsMap();
	
}
ACS_CLASS_DEFINE_DEBUG_LEVEL(FtpCurl);

map<string, RemoteSite> FtpCurl::_sites;
Condition FtpCurl::_discoverCondition;
vector<string> FtpCurl::_inDiscover;
map<string, bool> FtpCurl::_passiveFtpSiteOverrides;
map<string, map<string, string> > FtpCurl::_verifyHostCertificate;
Mutex FtpCurl::_overridesMutex;
bool FtpCurl::_config_loaded_OverrideFTPMode = false;
const bool FtpCurl::_default_VerifyHostCertificate = false; // Used when the host is not found in configuration
bool FtpCurl::_config_loaded = false ;
const size_t FtpCurl::_default_tar_block_factor = 20 ; // 20 * 512 = 10kB
size_t FtpCurl::_tar_block_factor = FtpCurl::_default_tar_block_factor ;
// Only if a file has size > (_tar_block_factor * _tar_block_factor_threshold) the non-standard block factor is used
// to create a new tar. O/W the default block size is used. Use 0 to disable the check.
double FtpCurl::_tar_block_factor_threshold = 0.0 ;	


FtpCurl::FtpCurl(const curl::BandPriority& bp) :
    band_priority_(bp), // PRQA S 2528
    _site("ftp"), 
    _localPath(), 
    _localFile(), 
    _listFileNamesOnly(false), 
    _nextLineCallback(), 
    _perfo(), 
    _currentLine(), 
    _data(), 
    _curl(0), 
    _transaction(0), 
    _handleProgressData(), 
    _handleHttpProgressData(), 
    _statistics()
{
    //empty.
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtpCurl::FtpCurl c'tor called");
}


FtpCurl::FtpCurl(const curl::BandPriority& bp,
                 const string &host, 
                 const string &user, 
                 const string &pwd,
                 const string &remotePath, 
                 const string &remoteFile, 
                 const string &localPath,
                 const string &localFile) :
    band_priority_(bp), // PRQA S 2528
    _site(RemoteSite::ftp, host, user, pwd), 
    _localPath(localPath), 
    _localFile(localFile), 
    _listFileNamesOnly(false), 
    _nextLineCallback(), 
    _perfo(), 
    _currentLine(), 
    _data(), 
    _curl(0), 
    _transaction(0), 
    _handleProgressData(), 
    _handleHttpProgressData(), 
    _statistics() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtpCurl::FtpCurl c'tor called");
    _site.setPath(StringUtils::trimEnd(remotePath, '/') + "/" + remoteFile);
}

FtpCurl::FtpCurl(const curl::BandPriority& bp,
                 acs::curl::RemoteSite const& site) :
    band_priority_(bp), // PRQA S 2528
    _site(site), 
    _localPath(), 
    _localFile(), 
    _listFileNamesOnly(false), 
    _nextLineCallback(), 
    _perfo(), 
    _currentLine(), 
    _data(), 
    _curl(0), 
    _transaction(0), 
    _handleProgressData(), 
    _handleHttpProgressData(), 
    _statistics()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtpCurl::FtpCurl c'tor called");
}

FtpCurl::FtpCurl(const curl::BandPriority& bp,
                 acs::curl::RemoteSite const& site, 
                 string const& localDir, 
                 string const& localFile) :
    band_priority_(bp), // PRQA S 2528
    _site(site), 
    _localPath(localDir), 
    _localFile(localFile), 
    _listFileNamesOnly(false), 
    _nextLineCallback(), 
    _perfo(), 
    _currentLine(), 
    _data(), 
    _curl(0), 
    _transaction(0), 
    _handleProgressData(), 
    _handleHttpProgressData(), 
    _statistics() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtpCurl::FtpCurl c'tor called");
}

FtpCurl::~FtpCurl() throw () 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtpCurl::FtpCurl d'tor called");
    // PRQA S 4631 L1
    try {
        closeConnection();
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtpCurl::FtpCurl d'tor finished");
}

string FtpCurl::toString(Operation op) // PRQA S 4020 
{
    switch(op) {
    case OpHttpPost: { return "OpHttpPost" ; }
    case OpDel:  { return "OpDel" ; }
    case OpDelTree: { return "OpDelTree" ; }
    case OpList: { return "OpList" ; }
    case OpGet: { return "OpGet" ; }
    case OpPut: { return "OpPut" ; }
    case OpPutAsTar: { return "OpPutAsTar" ; }
    case OpPutTree: { return "OpPutTree" ; }
    case OpGetTree: { return "OpGetTree" ; }
    case OpRename:  { return "OpRename" ; }
    case OpChMod: { return "OpChMod" ; }
    case OpHttpGet: { return "OpHttpGet" ; }
    case OpHttpPropFind: { return "OpHttpPropFind" ; }
    case OpHttpPut: { return "OpHttpPut" ; }
    case OpHttpDel: { return "OpHttpDel" ; }
    case OpHttpPatch: { return "OpHttpPatch" ; }
    default:
        {
            ostringstream os ; 
            os << "Unrecognized Operation (" << int(op) << ")" ; // PRQA S 3081 2
            ACS_THROW(exIllegalValueException(os.str())) ;
        }
    }
}

bool FtpCurl::hasPerfo(Operation op) // PRQA S 4020 
{
    switch(op) {
    case OpGet: 
    case OpPut: 
    case OpPutAsTar: 
    case OpPutTree: 
    case OpGetTree: 
        { return true ; }
    default:
        { return false ; }
    }
}


void FtpCurl::initializeCurl(Operation op, StopController const& stopController) // PRQA S 4020
{
    //create the curl internal object;
    setOperation(op);
    createCurl();
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Object dump, Operation:  " << toString(op) << endl << str());
    
    // In case of HTTP GET of a large object to be put on a file
    // consider creating another operation OpHttpLOGet
    // httppost and httpget at the moment cannot be stopped and get results on strings. httpPropFind get directory content info so that it will not be stopped.
    if ((op == OpHttpPost) || (op == OpHttpGet) || (op == OpHttpPropFind) || (op == OpHttpPut) || (op == OpHttpDel) || (op == OpHttpPatch)) { 
        _handleHttpProgressData.reset(new HttpProgressData(stopController));
        _curl->setProgressCallback(HttpProgressData::progress_callback, _handleHttpProgressData.get());
        return;
    }
    
    _handleProgressData.reset(new FtpProgressData(stopController, band_priority_));
    _handleProgressData->setPerfMonitor(&_perfo) ;
    _curl->setProgressCallback(FtpProgressData::progress_callback, _handleProgressData.get());
    if (hasPerfo(op)) { _perfo.start(); }
}

void FtpCurl::finalizeCurl(CURLcode& res, string const& target) 
{
    _curl->deregisterProgressCallback();
    _perfo.stop();
    if (_handleProgressData.get()) {
        _handleProgressData->setPerfMonitor(0) ;
    }
    throwConditionedError(res, target, __PRETTY_FUNCTION__, __LINE__);
}

void FtpCurl::setOperation(Operation op) 
{
    _perfo.setOperation(toString(op));
}

Curl& FtpCurl::createCurl() 
{
    //create the internal curl handler (if not already present)
    if (!_curl) {
        _curl = new acs::curl::Curl();
    }
    _curl->setSite(_site);
    _perfo.setRemoteSite(_site);
    return *_curl;
}

void FtpCurl::openConnection(bool skipIfAlreadyCreated) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Open Connection to cURL");
    //A connection can be opened only once, otherwise an exception will be raised.
    //neverthless if requested to skip this check only a warning message will printed out.
    if (!skipIfAlreadyCreated) {
        ACS_COND_THROW(0 != _transaction, FtpCurlException("cURL handler already created.")) ; // PRQA S 3081
    }
    if (!_transaction) {
        _transaction = new CurlTransaction(createCurl());
        _transaction->begin();
    } 
    else {
        ACS_LOG_WARNING(exDebugSignatureThread << "Warning: Same connection reopened, is exactly what you wanted ?");
    }
}

void FtpCurl::closeConnection() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Close Connection to cURL");
    //if the _transactin handler has been created, it will be closed and deleted.
    if (_transaction) {
        _transaction->close();
        delete _transaction;
        _transaction = 0;
    }
    //if the _curl handler has been created, it will be deleted.
    if (_curl) {
        delete _curl;
        _curl = 0;
    }
}

void FtpCurl::rollbackConnection() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Rollback Connection to cURL");
    //rollback a transaction if it was created.
    if (_transaction) {
        _transaction->rollback();
    }
    closeConnection();
}

void FtpCurl::ldel(const string &localFile) // PRQA S 4211
{
    _curl->ldel(localFile);
}

long FtpCurl::httpRequest(const curl::Curl::CurlHttpOpt& opt, const Operation& op, std::string& responsePayload, bool failOnError, StopController const& stopController) {
    std::string url = opt.getUri();
    
    // This check is only performed for backward compatibility... An url should ALWAYS have a protocol specified...
    size_t protocolSeparator = url.find(RemoteSite::protocolSeparator);
    if (protocolSeparator == string::npos) {
        url = "http://" + url;
    }
    
    RemoteSite tmpSite = RemoteSiteFactory::makeSite(url);
    _site.setProtocol(tmpSite.getProtocol());

    setupVerifyHostCertificate(tmpSite.getHost()) ;
    
    initializeCurl(op, stopController);
    
    // Fail on error option must be disabled in in order to be able to retrieve the response payload in case of http response codes >= 400
    _curl->setHttpFailOnError(failOnError);
    
    // Execute the http command
    CURLcode res = CURLE_OK;
    switch (op) {
    case OpHttpPost:
        res = _curl->post(opt, responsePayload);
        break;
    case OpHttpGet:
        res = _curl->httpget(opt, responsePayload);
        break;
    case OpHttpPut:
        res = _curl->httpput(opt, responsePayload);
        break;
    case OpHttpPatch:
        res = _curl->httppatch(opt, responsePayload);
        break;
    case OpHttpDel:
        res = _curl->httpdel(opt, responsePayload);
        break;
    default:
        ACS_THROW(FtpCurlUnrecoverableErrorException("Unsupported http operation: " + toString(op)));
    }
    
    // Reset the fail on error option to true
    _curl->setHttpFailOnError(true);
    
    // It throws exception if res is different from CURLE_OK
    finalizeCurl(res, url);
     
    return _curl->getHttpResponseCode();
}

long FtpCurl::httpPost(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http post: " << opt.getUri());
	return httpRequest(opt, OpHttpPost, responsePayload, false, stopController);
}

std::string FtpCurl::httpPost(const Curl::CurlHttpOpt& opt, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http post: " << opt.getUri());
    string result;
    httpRequest(opt, OpHttpPost, result, true, stopController);
    return result;
}

string FtpCurl::httpPost(string const& uri, string const& postMessage, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http post: " << uri);
    Curl::CurlHttpOpt opt;
    opt.setUri(uri);
    opt.setMessage(postMessage);
    return httpPost(opt, stopController);
}

string FtpCurl::httpPost(string const& uri, string const& postMessage, const string& contentType, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http post: " << uri);
    Curl::CurlHttpOpt opt;
    opt.setUri(uri);
    opt.setMessage(postMessage);
    opt.setContentType(contentType);
    return httpPost(opt, stopController);
}

long FtpCurl::httpGet(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http get: " << opt.getUri());
    return httpRequest(opt, OpHttpGet, responsePayload, false, stopController);
}

string FtpCurl::httpGet (const Curl::CurlHttpOpt& opt, StopController const& stopController)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http get: " << opt.getUri());
    string result;
    httpRequest(opt, OpHttpGet, result, true, stopController);
    return result;
}

string FtpCurl::httpGet (std::string const& uri, StopController const& stopController)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http get: " << uri);
    Curl::CurlHttpOpt opt;
    opt.setUri(uri);
    return httpGet(opt, stopController);
}

long FtpCurl::httpPut(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http put: " << opt.getUri());
    return httpRequest(opt, OpHttpPut, responsePayload, false, stopController);
}

string FtpCurl::httpPut (const Curl::CurlHttpOpt& opt, StopController const& stopController)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http put: " << opt.getUri());
    string result;
    httpRequest(opt, OpHttpPut, result, true, stopController);
    return result;
}

long FtpCurl::httpDel(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http del: " << opt.getUri());
    return httpRequest(opt, OpHttpDel, responsePayload, false, stopController);
}

string FtpCurl::httpDel (const Curl::CurlHttpOpt& opt, StopController const& stopController)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http del: " << opt.getUri());
    string result;
    httpRequest(opt, OpHttpDel, result, true, stopController);
    return result;
}

long FtpCurl::httpPatch(const curl::Curl::CurlHttpOpt& opt, std::string& responsePayload, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http patch: " << opt.getUri());
    return httpRequest(opt, OpHttpPatch, responsePayload, false, stopController);
}

string FtpCurl::httpPatch (const Curl::CurlHttpOpt& opt, StopController const& stopController)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http patch: " << opt.getUri());
    string result;
    httpRequest(opt, OpHttpPatch, result, true, stopController);
    return result;
}

void FtpCurl::propFind( string const& remotePath, int depth, vector<PropFindStatItem> & statItems, const PropFindRequests & request , bool dirOnly, const PropFindStatItemOrder & orderBy, bool reverse, bool isDir, StopController const& sc )
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    
    // consistency check
    if( dirOnly ) {
        ACS_COND_THROW( (request & PROPFINDREQ_TYPE) == 0, FtpCurlUnrecoverableErrorException("Cannot list directories only since resource type property not requested") );
    }
    
    string remotePathLocal;
    if( isDir ) { // add a slash at the end
        remotePathLocal = StringUtils::trimEnd(remotePath, '/') + '/';
    }
    else { // remove trailing slash at the end
        remotePathLocal = StringUtils::trimEnd(remotePath, '/');
    }
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << remotePath << "\" remote path expected to be a " << ( isDir ? "directory" : "file") << " turned as \"" << remotePathLocal << '\"');
    
    vector<string> properties = turnRequestsIntoProperties(request);
    propFind( remotePathLocal, depth, properties, dirOnly, isDir, statItems, sc ) ;
    switch(orderBy) {
    case PROPFINDITEMS_NONE_ORDER:
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "No ordering requested" );
            break;
        }
    case PROPFINDITEMS_NAME_ORDER:
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to order items by name" << ( reverse ? " in reverse order" : "" ) );
            sort (statItems.begin(), statItems.end(), propFindItemOrderingByName );
            if( reverse ) {
                sort (statItems.rbegin(), statItems.rend(), propFindItemOrderingByName );
            }
            break;
        }
    case PROPFINDITEMS_CREATIONDATE_ORDER:
        {
            ACS_COND_THROW( ( request & PROPFINDREQ_CREATION ) == 0, FtpCurlUnrecoverableErrorException("Items requested in creation date order, but creation date property not requested" ) ); 
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to order items by creation date" << ( reverse ? " in reverse order" : "" ) );
            sort (statItems.begin(), statItems.end(), propFindItemOrderingByCreationDate );
            if( reverse ) {
                sort (statItems.rbegin(), statItems.rend(), propFindItemOrderingByCreationDate );
            }
            break;
        }
    case PROPFINDITEMS_MODIFICATIONDATE_ORDER:
        {
            ACS_COND_THROW( ( request & PROPFINDREQ_MODIFICATION ) == 0, FtpCurlUnrecoverableErrorException("Items requested in modification date order, but modification date property not requested" ) ); 
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to order items by modification date" << ( reverse ? " in reverse order" : "" ) );
            sort (statItems.begin(), statItems.end(), propFindItemOrderingByModificationDate );
            if( reverse ) {
                sort (statItems.rbegin(), statItems.rend(), propFindItemOrderingByModificationDate );
            }
            break;
        }
    default:
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Order by not managed: " << int(orderBy) );
            break ;
        }
    
    }
}



void FtpCurl::del(const StopController &stopController) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Delete remote file: " << _site.getPath());
    verifyRemoteSite();
    initializeCurl(OpDel, stopController);
    CURLcode res = _curl->del(_site.getPath());
    finalizeCurl(res, _site.getPath());
}

void FtpCurl::delDir(const StopController &stopController) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Delete remote dir: " << _site.getPath());
    verifyRemoteSite();
    initializeCurl(OpDelTree, stopController);
    CURLcode res = _curl->delDir(_site.getPath());
    finalizeCurl(res, _site.getPath());
}

void FtpCurl::list(NextLineCallback nextLineCallback, void* data, StopController const& stopController) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List remote directory: " << _site.str());
    // Verify remote site but also check if the site is passive or active (FTP sites only)
    verifyRemoteSite(true);
    initializeCurl(OpList, stopController);
    setNextLineCallback(nextLineCallback, data);
    _curl->setListCallback(readLine_callback, this);
    CURLcode res = _curl->ls(_site.getPath());
    finalizeCurl(res, _site.getPath());
}

void defaultNextLineCallback(const string &line, void* data) 
{
    FtpListReader* reader = reinterpret_cast<FtpListReader*>(data); // PRQA S 3081
    FtpListItem item;
    item.setLine(line) ;
    reader->addListItem(item);
}

void httpNextLineCallback(const string &line, void* data) 
{
    oftmpstream* outfile = reinterpret_cast<oftmpstream*>(data); // PRQA S 3081
    (*outfile)() << line << '\n';
}

void FtpCurl::list(vector<FtpListItem> & items, StopController const& stopController) 
{
    FtpListReader reader(items);
    list(defaultNextLineCallback, &reader, stopController);
}

string FtpCurl::list(StopController const& stopController) 
{
    //Returns a string containing all the files in the remote site.
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List remote directory.");
    // Verify remote site but also check if the site is passive or active (FTP sites only)
    verifyRemoteSite(true);
    initializeCurl(OpList, stopController);
    ostringstream output;
    _curl->setDefaultListCallback();
    CURLcode res = _curl->ls(_site.getPath(), &output);
    finalizeCurl(res, _site.getPath());
    return output.str();
}


bool FtpCurl::loadConfigurationOnce()
{
	const bool loaded (not _config_loaded) ; 

    // Read configuration locking it for the whole time
    ThreadSafe::Lock lock (_overridesMutex);
    if (false == _config_loaded)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loading configuration");

        // It is the first time... try to load the configuration

        _config_loaded = true;
		
		{
        	// In the FtpCurl configuration space search for a key like:

        	// <VerifyHostOverride>
        	//     <Host verify="false">
        	// 		<Hostname>localhost01</Hostname>
        	//     </Host>
        	//     <Host verify="true">
        	// 		<Hostname>ftps.com</Hostname>
        	//     </Host>
        	//     <Host verify="true">
        	// 		<Hostname>example.com</Hostname>
        	// 		<cacert>/tmp/CA_CERT_FILE</cacert>
        	// 		<cert>/tmp/CERT_FILE</cert>
        	// 		<key>/tmp/KEY_FILE</key>
        	//     </Host>
        	// </VerifyHostOverride>

			const string rootTag("FtpCurl.VerifyHostOverride.Host");
			rsResourceSet rs=ConfigurationSingleton::instance()->get();

        	bool isSingle;
        	const size_t elements ( rs.getArraySize( rootTag, &isSingle ) ) ; // PRQA S 3000, 3010, 4412
        	for( size_t i=0; i < elements; i++)
        	{
            	string tag = rootTag;
            	if( not isSingle ) 
            	{
                	ostringstream os ;
                	os << "[" << i << "]" ;
                	tag += os.str();
            	}

				// any string that is not "verify" will be managed as do not verify
            	bool checkCertificate = false;
            	string theMode = "";
            	// Read attribute "verify.value"
				rs.getValue ( (tag+"."+Curl::Confkey_verifycert_verifyhost+".value"), theMode, false); 
																
			    if ( StringUtils::equalsNoCase (theMode, "true") )
            	{
					// Value found and is TRUE
                	checkCertificate = true;
            	}	
				
				{
            		string theHost = "";
					rsPushNSpace pusher1 (rs, tag);

            		// take the lowercase version of the given host
            		rs.getValue (Curl::Confkey_verifycert_hostname, theHost, false);
            		theHost = StringUtils::lowercase(theHost);

            		if ( not theHost.empty() )
            		{
						map <string, string> parameters_M ;
						parameters_M[Curl::Confkey_verifycert_hostname] = theHost ;
						if (true == checkCertificate)
						{ 
							string param_value ; 
							parameters_M[Curl::Confkey_verifycert_verifyhost] = Curl::Confkey_verifycert_verifyhost ; 
							
							if (rs.getValue (Curl::Confkey_verifycert_cacert, param_value, false))
							{
								parameters_M[Curl::Confkey_verifycert_cacert] = param_value ;
							}
							if (rs.getValue (Curl::Confkey_verifycert_cert, param_value, false))
							{
								parameters_M[Curl::Confkey_verifycert_cert] = param_value ;
							}
							if (rs.getValue (Curl::Confkey_verifycert_key, param_value, false))
							{
								parameters_M[Curl::Confkey_verifycert_key] = param_value ;
							}
						}
						else
						{ /* Omit all the parameters, including the verify host */ }
									
						// Set the payload to the map			
						_verifyHostCertificate[theHost] = parameters_M;
              
			 			ostringstream o ; o << "Configuring the certificate verification flag to " << (checkCertificate?"TRUE":"FALSE") << " for the host with certificate parameters: { " ;
						for (map<string,string>::const_iterator it=parameters_M.begin(); it != parameters_M.end(); it++)
						{ o << "\"" << it->first << "\": \"" << it->second << "\" ; " ; }
						o << "}" ; 	

			  	  		ACS_LOG_INFO(o.str());
					}	// END: if ( not theHost.empty() )
				
				}	// END: ns scope
        	}	// END: for i 
    	}	// END: scope
		
		{
        	// In the FtpCurl configuration space search for a key like
        	// <FtpCurl> <TarBlockFactor> </TarBlockFactor> <TarBlockFactorThreshold> </TarBlockFactorThreshold> 

			const rsResourceSet& rs=ConfigurationSingleton::instance()->get();
			string key ; 
						
			key = "FtpCurl.TarBlockFactor" ;  if (true == rs.hasKey(key)) 
			{
				rs.getValue(key, _tar_block_factor) ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found configuration key: \"" << key \
					<< "\" with value: " << _tar_block_factor) ;
			}

			key = "FtpCurl.TarBlockFactorThreshold" ;  if (true == rs.hasKey(key)) 
			{
				rs.getValue(key, _tar_block_factor_threshold) ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found configuration key: \"" << key \
					<< "\" with value: " << _tar_block_factor_threshold) ;
			}
		}	
	}

	return loaded ; // True if loaded now, false if already loaded in the past
}
   

bool FtpCurl::mustOverrideFTPMode (string const& thehost, bool& isPassive )
{
    {
        // Read configuration locking it for the whole time
        ThreadSafe::Lock lock (_overridesMutex);
        if (false == _config_loaded_OverrideFTPMode)
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loading override active/passive configuration settings");

            // It is the first time... try to load the configuration
            // In the FtpCurl configuration space search for a key like
            // <FtpCurl> <FTPHostModeOverride> <Host active="true">thehost</Host> .... <Host active="true">thehostn</Host> </FTPHostModeOverride> </FtpCurl>

            _config_loaded_OverrideFTPMode = true;
            const string rootTag("FtpCurl.FTPHostModeOverride.Host");
            const rsResourceSet& rs=ConfigurationSingleton::instance()->get();

            bool isSingle;
            size_t elements = rs.getArraySize( rootTag, &isSingle ); // PRQA S 3000, 3010, 4412
            for( size_t i=0; i < elements; i++)
            {
                string tag = rootTag;
                if( not isSingle ) 
                {
                    ostringstream os ;
                    os << "[" << i << "]" ;
                    tag += os.str();
                }
                
                bool setToPassive = true;
                string theMode = "";
                string theHost = "";
                // take the lowercase version of the given host
                rs.getValue (tag, theHost, false);
                StringUtils::lowercase(theHost);
                // any string that is not "active" will be managed as passive
                rs.getValue (tag+".active.value", theMode, false); 
                if ( StringUtils::compareNoCase (theMode, "true") == 0 )
                {
                    // An active host
                    setToPassive = false;
                }
                
                if ( not theHost.empty() )
                {
                    _passiveFtpSiteOverrides[theHost]=setToPassive;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding to FTPMode overrides the host " << theHost << " as " << (setToPassive?"PASSIVE":"ACTIVE") );
                }
            }
        }
    }
    
    // Identify overrides using the lowercase version of the hostname
    string hostToCheck = _site.getHost();
    StringUtils::lowercase(hostToCheck);
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking override active/passive configuration for the remote host " << hostToCheck );

    auto search = _passiveFtpSiteOverrides.find(hostToCheck);
    
    if ( search == _passiveFtpSiteOverrides.end() )
    {
        // Override setting not found
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Override active/passive configuration for the remote host " << _site.getHost() << " NOT FOUND" );
        return false;
    }

    // Use the override value and return true
    isPassive = search->second;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Override active/passive configuration for the remote host " << _site.getHost() << " FOUND and set to " << (isPassive?"PASSIVE":"ACTIVE") );

    // Configuration found.. return true
    return true;
}


bool FtpCurl::mustVerifyHostCertificate (const string& hostToCheck_in, map<string, string>& parameters_M)
{
 	// Load the configuration file (just once)   
	loadConfigurationOnce() ; 
	
    // Identify overrides using the lowercase version of the hostname
    const string hostToCheck ( StringUtils::lowercase(hostToCheck_in) ) ;
    parameters_M.clear() ; 
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking cerficate check configuration for the remote host " << hostToCheck );

    if ( _verifyHostCertificate.end() == _verifyHostCertificate.find(hostToCheck) )
    {
        // Override setting for hostToCheck not found
		
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking certificate verification configuration for the remote host " \
			<< hostToCheck << " NOT FOUND" );
    	// Configuration not found.. return false
        return false;
    }
	
	parameters_M = _verifyHostCertificate.at(hostToCheck) ;

	// Next parameter is used only for debug:
	const bool to_verify = ( \
		(parameters_M.end() != parameters_M.find(Curl::Confkey_verifycert_verifyhost)) \
		&& (Curl::Confkey_verifycert_verifyhost == parameters_M.at(Curl::Confkey_verifycert_verifyhost)) ) ;
		    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking certificate verification configuration for the remote host " \
		<< hostToCheck << " FOUND set to " << (to_verify?"TRUE":"FALSE") );

    // Configuration found.. return true
    return true;
}


// Set the attribute VerifyCertificate for the site (use the configuration, /see mustVerifyHostCertificate)
// Returns true if the hostname was found in configuration, otherwise false
bool FtpCurl::setupVerifyHostCertificate(const string& host_name)
{
    // Check configuration for the host
	bool found (false) ; 
	map <string, string> parameters_M ;  

	// Check if the hostname is valid and found in the configuration, and load the flag
    if ( (true == host_name.empty()) || (false == (found = mustVerifyHostCertificate (host_name, parameters_M ))) )
	{
		// Create an empty parameter map, with the verification set to true
		parameters_M[Curl::Confkey_verifycert_hostname] = StringUtils::lowercase(host_name) ;
		
		// Not found in the configuration (default is used instead).
		if (true == _default_VerifyHostCertificate)
		{
			// Create an empty parameter map, with the verification set to true (if the default is true)
			parameters_M[Curl::Confkey_verifycert_verifyhost] = Curl::Confkey_verifycert_verifyhost ; 
		}
	}
   
	{
		ostringstream o ; o << "Setting the " << (found?"configured":"default/fallback") \
		<< " certificate verification with certificate parameters: { " ;
		for (map<string,string>::const_iterator it=parameters_M.begin(); it != parameters_M.end(); it++)
		{ o << "\"" << it->first << "\": \"" << it->second << "\" ; " ; }
		o << "}" ; 	
   		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, o.str() + "\n" ) ; 
	}	

	// Store the parameter map in the RemoteSite
	_site.setVerifyCertificateParameters ( parameters_M );

	return found ; 
}


void FtpCurl::passiveActiveDiscover() // PRQA S 4020
{
    setupVerifyHostCertificate(_site.getHost());
    if (!_site.supportsActivePassive()) { // nop if the protocol does not support the active/passive concept
        return;
    }
    const string &phost = _site.getHost() ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Site: " << phost);
    
    // check if someone else is discovering the same host
    for (;;) 
    {
        // Lock on _discoveryMutex is enough to protect both shared resources _inDiscover and _sites
        Lock dis_lock(_discoverCondition);
        if (find(_inDiscover.begin(), _inDiscover.end(), phost) == _inDiscover.end()) 
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, phost << " is not in the probing list ");
            { 
                // check if it is already in the map of sites 
                map<string,RemoteSite>::iterator it = _sites.find(phost) ;
                if (it != _sites.end()) 
                {
                    _site.setIsPassive(it->second.getIsPassive());
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Site already probed: " << _site.str());
                    return ;
                }
                else 
                { 
                    // not found and nobody is discovering it
                    _inDiscover.push_back(phost) ; // I am discovering it
                    break ;
                }
            } // unlock the map of sites
        }
        else 
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, phost << " is in the probing list [path = " << _site.getPath() << "]");
            // In this case someone else is going to probe the same site
            // Can wait and use a timed-condition (1 second)
            // This will release the lock on the condition mutex
            _discoverCondition.timedwait (1000);
        }
    } 
        
    try 
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Probing: " << _site.str());
        
        // If there is an override of Active/Passive mode by configuration use it
        bool isPassive;
        if ( mustOverrideFTPMode (_site.getHost(), isPassive ) )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Overriding the active/passive configuration for the remote host " << _site.getHost() << " to " << (isPassive?"PASSIVE":"ACTIVE"));
            _site.setIsPassive ( isPassive );
        }
        // otherwise use Curl to identify the configuration on the server side
        else
        {
            Curl curl;
            curl.setSite(_site);
            bool success = curl.probe(""); // PRQA S 4412
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Probe result: " << success);
            if (!success) 
            {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Failing to probe the remote site in passive mode. Retry using active mode.");
                _site.setIsPassive(!_site.getIsPassive());
                curl.setSite(_site);
                CURLcode res = CURLE_OK;
                string noPath;
                bool success2 = curl.probe(noPath, true, &res); // PRQA S 4412
                if (not success2) 
                {
                    ACS_LOG_ERROR("Failed to probe the remote site. Cannot connect to: " << _site.str());
                }
                throwConditionedError(res, noPath, __PRETTY_FUNCTION__, __LINE__);

                ACS_LOG_INFO("Changed remote site passive/active mode: " << _site.str());
            }
        }
    }
    catch(exception &x) {
        // Get the lock on the shared resources
        Lock dis_lock(_discoverCondition); 
        // update the discovering vector
        _inDiscover.erase(find(_inDiscover.begin(), _inDiscover.end(), phost)) ;
        // wake up all other waiting on this condition
        _discoverCondition.broadcast();
        throw ;
    }
    
    {
        // Get the lock on the shared resources
        Lock dis_lock(_discoverCondition); 
        // Change them
        _inDiscover.erase(find(_inDiscover.begin(), _inDiscover.end(), phost)) ;
        _sites[_site.getHost()] = _site;
        // wake up all other waiting on this condition
        _discoverCondition.broadcast();
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Site probed: " << _site.str());
}

RemoteResourceType FtpCurl::probeRemoteResource(string &lastErrorMessage, StopController const& stopController, bool emitException, bool logError)  // PRQA S 4020
{
    //Tests if the remote resource exists.
    //First of all tests if the remote site is ftp/ftps and active or passive.
    //Then we list the containing remote parent folder and we search if the remote site file name is present.
    passiveActiveDiscover(); // nop if the protocol does not support active/passive
    RemoteResourceType result = RRTNotExist;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exist remote path? " << _site.getPath());
    verifyRemoteSite(); // nop if protocol does not supports active/passive, but checks site consistency
    if (_site.isLocalhost()) {
        if (!File::exists(_site.getPath())) {
            return RRTNotExist;
        }
        if (File::isDir(_site.getPath())) {
            return RRTIsDir;
        }
        return RRTIsFile;
    }

    if( _site.getIsHttp() ) { // if http(s) protocol, cannot perform ls so we assume the path is a file
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "http(s) protocol: assuming \"" << _site.getPath() << "\" is a file");
        if( existRemoteFile (true, stopController, emitException) ) {
            return RRTIsFile;
        }
        else {
            return RRTNotExist;
        }
    }

    string originalPath = _site.getPath();
    string baseName = _site.getRemoteFileName();
    string parentPath = _site.getRemoteDir();
    try {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Remote Dir: " << parentPath << " Remote file: " << _site.getRemoteFileName());
        //set the parent folder.
        _site.setPath(parentPath);
        vector<FtpListItem> items;
        //Makes the list of the parent folder's children.
        list(items);
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "items found : " << items.size());
        for (vector<FtpListItem>::iterator iter = items.begin(); iter != items.end(); ++iter) { // PRQA S 4238
            FtpListItem item = *iter;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Current Item: " << item.str() << endl << "\t" << item.getPermissions().str());
            //if this item is not a link , then verify if it is a file or a directory.
            if (!item.getPermissions().isLink()) {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Item found: " << item.getName() << "\t" << baseName);
                if (item.getName() ==  baseName) {
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Item found: " << item.str() << endl << "\t" << item.getPermissions().str());
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is file? " << item.getPermissions().isFile());
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is dir? " << item.getPermissions().isDirectory());
                    if (item.getPermissions().isFile()) {
                        result = RRTIsFile;
                    }
                    if (item.getPermissions().isDirectory()) {
                        result = RRTIsDir;
                    }
                    break;
                }
            } 
            else {
                //if this item is a link, follow the link and try to verify if
                //it is a directory or a file.
                string linkedItem = StringUtils::substringAfter(item.getName(), "->");
                string link = StringUtils::substringBefore(item.getName(), "->");
                linkedItem = StringUtils::trim(linkedItem);
                link = StringUtils::trim(link);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Linked item: ["<<linkedItem<< "] baseName: ["<<baseName<<"]");
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Link item: ["<<link<<"]");

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "link == baseName: ["<<(link == baseName)<<"]; !linkedItem.empty(): "<<(!linkedItem.empty()));

                if ( (link == baseName) && !linkedItem.empty() ) {

                    _site.setPath( ( linkedItem.at(0) == '/' ) ? // PRQA S 3380, 3385
                            linkedItem :                            //It's a Link to absolute path
                            ( parentPath + "/" + linkedItem ) );    //It's a Link to relative path

                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Following link, new path to probe: ["<<_site.getPath()<<"]");

                    try {
                        string dummy ;
                        result = probeRemoteResource(dummy, stopController);
                        break;
                    } 
                    catch (exception const& ignore) {
                        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ignore.what());
                        break;
                    }
                }
            }
        }
    } 
    catch (exception const& ex) {
    
        if( ( dynamic_cast<const FtpCurlLoginDeniedException*> (&ex) != 0 ) ||  // PRQA S 3081 2 
            ( dynamic_cast<const Curl::CurlUnrecoverableException*> (&ex) != 0) || emitException ) {
            
            try {
                //restore the original path in order to not change this object behaviour.
                _site.setPath(originalPath);
            }
            catch( exception & ee)
            {
                ACS_LOG_ERROR("Cannot even restore original path: " << ee.what() );
            }
            throw;
        }
        
        
        if( logError ) {
            ACS_LOG_NOTIFY_EX(ex);
            ACS_LOG_ERROR("Error probing site: " << _site.str() );
        }
        else {
            ACS_LOG_WARNING("Cannot probe site " << _site.str() << ": " << ex.what());
        }

        lastErrorMessage.append(ex.what());
        
        
    }
    //restore the original path in order to not change this object behaviour.
    _site.setPath(originalPath);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Remote resource probed: " << int(result)); // PRQA S 3081
    return result;
}

bool FtpCurl::existRemoteResource (bool& isDir, StopController const& sc, bool emitException) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking existance of resource: " << _site.getPath() ); // PRQA S 3081

    // Start assuming it is not a directory
    isDir = false;

    // check for file existance and, if not found, check for directory
    bool retval = existRemoteFile (true, sc, emitException);
    if (false == retval)
    {
        // If found means it is a directory
        retval = existRemoteFile (false, sc, emitException) ;
        isDir = retval;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Resource exists: " << boolalpha << retval << " isDir = " << boolalpha << isDir ); // PRQA S 3081

    return retval;
}

bool FtpCurl::existDestinationFile(bool testFile, StopController const& stopController, bool emitException)
{
    bool retval = false;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking existance of: " << _site.getPath() << " (" << (testFile ? "file" : "directory") << ")"); // PRQA S 3081, 3380
    
    if (_site.isLocalhost()) {
        return File::exists(_site.getPath());
	}

    // Check if the site (FTP) supports passive data transfers
    passiveActiveDiscover(); // nop if the protocol does not supports active/passive

    // do not test for directories for protocols that cannot manage them
    if (testFile == false and _site.supportsDirectory() == false) {
        return false ;
    }

    // Start Curl activities    
    initializeCurl(OpList, stopController);
    CURLcode cval = _curl->probeElement (testFile, _site.getPath());

    if (cval == CURLE_OK)
    {
        // Ok. Found it
        retval=true;
    }
    else if ( (CURLE_REMOTE_FILE_NOT_FOUND == cval) || (CURLE_REMOTE_ACCESS_DENIED == cval) || (CURLE_RECV_ERROR  == cval) )
    {
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Remapping expected error (" << int(cval) <<  ") \"" <<  curl_easy_strerror(cval) << "\" to CURLE_OK"); // PRQA S 3081
        cval = CURLE_OK;
        // Element has not been found
        retval=false;
    }
    else if ( false == emitException)
    {
        // A problem has been found but it has been requested to avoid exception to exit from here
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Masking curl code (" << int(cval) <<  ") \"" <<  curl_easy_strerror(cval) << "\" to CURLE_OK to avoid exceptions"); // PRQA S 3081
        retval=false;
        cval = CURLE_OK;
    }
    else { /* nothing */ }

    // This will throw in case of other problems not expected during the probe phase
    finalizeCurl(cval, _site.getPath());

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Remote item exists: " << boolalpha << retval <<  " (" << (testFile ? "file" : "directory") << ")" ); // PRQA S 3081, 3380


    // Return the boolean
    return retval;
}

bool FtpCurl::existRemoteFile(bool testFile, StopController const& stopController, bool emitException) 
{
    return existDestinationFile(testFile, stopController, emitException);
}

void FtpCurl::get(StopController const& stopController) 
{
    get(0, stopController);
}

void FtpCurl::get(mode_t permissions, StopController const& stopController) 
{
    FtpCurlPermissions ftpCurlPerm;
    ftpCurlPerm.permissions = permissions;
    get(ftpCurlPerm, stopController);
}

void FtpCurl::get(const FtpCurlPermissions& ftpCurlPerm, StopController const& stopController) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GetWithStatus :" << str());
    string localFile;
    string tempLocalFile;
    string targetFile;
    // Verify remote site but also check if the site is passive or active (FTP sites only)
    verifyRemoteSite(true); // should only check if active/passive - nop if the protocol does not support it
    if (_localPath.size() < 1) {
        localFile = _localFile;
    } 
    else {
        localFile = StringUtils::trimEnd(_localPath, '/') + "/" + _localFile;
    }
    if (_site.hasTemporaryTargetName()) {
        targetFile = tempLocalFile = _site.getTemporaryPath(StringUtils::trimEnd(_localPath, '/') + "/" + _localFile);
        string destDir = File::getDirPath(targetFile);
        if (!File::exists(destDir)) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Create directory: " << destDir);
            try {
                File::mkdirtree(destDir);
            }
            catch( exception &e )
            {
                ACS_THROW( FtpCurlUnrecoverableErrorException(e, "Unable to create \"" + destDir + '\"') );
            }
        }
    } 
    else {
        targetFile = localFile;
    }

    initializeCurl(OpGet, stopController);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Get local file:" << targetFile << " , from remote file: " << _site.getPath());
    CURLcode res = _curl->get(targetFile, _site.getPath());
    if (CURLE_OK == res)
    {
        // The target rename, if local, is part of the transfer
        // If unable to rename shall fail
        if (_site.hasTemporaryTargetName()) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,  "Rename local file :" << targetFile << " to : " << localFile);
            res = _curl->lrename(targetFile, localFile);
        }
    }
    finalizeCurl(res, _site.getPath());
    fillGetStats();
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "permissions : " << ftpCurlPerm.permissions );
    if (ftpCurlPerm.permissions != 0) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Change local permissions : " << localFile << " to " << ftpCurlPerm.permissions);
        _curl->lchmod(localFile, ftpCurlPerm.permissions);
    }
    if (ftpCurlPerm.userId != uid_t(-1) || ftpCurlPerm.groupId != gid_t(-1)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Change ownership: File: " << localFile << " user_id: " << ftpCurlPerm.userId << " group_id " << ftpCurlPerm.groupId);
        // If userId is equal to -1, the ownership is not changed
        // If groupId is equal to -1, the ownership is not changed
        _curl->lchown(localFile, ftpCurlPerm.userId, ftpCurlPerm.groupId);
    }
    
}


void FtpCurl::getUnTar(const File::CompressionMode& cm, const mode_t& permissions, StopController const& stopController) 
{
    vector <string> decompress_options ; // Empty
    getUnTar(cm, permissions, decompress_options, stopController);
}

void FtpCurl::getUnTar(const File::CompressionMode& cm, const mode_t& permissions, const vector <string>& decompress_options, \
                        StopController const& stopController) 
{
    FtpCurlPermissions ftpCurlPerm;
    ftpCurlPerm.permissions = permissions;
    getUnTar(cm, ftpCurlPerm, decompress_options, stopController);
}

void FtpCurl::getUnTar(const File::CompressionMode& cm, const FtpCurlPermissions& ftpCurlPerm, 
                        const vector <string>& decompress_options, 
                        StopController const& stopController) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GetWithStatus :" << str());
    string targetFile;
    // Verify remote site but also check if the site is passive or active (FTP sites only)
    verifyRemoteSite(true);
    const string localPath (_localPath) ; 
  
    if (_site.hasTemporaryTargetName()) {
        // The destination (local) directory should be nonexisting only if site has temporary path strategy.
        // - If there is no temporary target, the destination directory will be created below
        //    (theoretically, this condition might be relaxed, e.g. with an optional flag to skip this check)
        // - If there is a temporary target, it will be created below and eventually will be renamed as the nonexisting destination directory
        //   The point is that the rename command WOULD FAIL if the destination directory already exists!
        //   This check cannot be relaxed: in case of temporary directory, the target MUST NOT EXIST !
        ACS_COND_THROW ( (true==File::exists(localPath)), exIllegalValueException( "Destination local directory already exists: " + localPath)) ; 
    
        targetFile = _site.getTemporaryPath(localPath);
    } 
    else {
        targetFile = localPath;
    }

    if (!File::exists(targetFile)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Create directory: " << targetFile);
        File::mkdirtree(targetFile);
    }


    initializeCurl(OpGet, stopController);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Get local file:" << targetFile << " , from remote file: " << _site.getPath());
     
    {
		// Handle the tar block multiplication factor (when decompressing, the file size is not matched)
		const size_t tar_block_factor ( (get_tar_block_factor() > 0)? get_tar_block_factor() : _default_tar_block_factor) ;
		
        // Instance the ofuntarstream processing engine (forked as a separate process)
        ofuntarstream untar_ostream (targetFile.c_str(), cm, tar_block_factor, decompress_options) ;
        
        // A CurlFile container is created. payload pointer is: (&untar_ostream)
        CurlFile data_handler(&untar_ostream) ; 
    
        // Setup the callback, providing the data handler to call (intance of CurlFile) and the callback function
        CURLcode res = _curl->getWithCallback(targetFile, _site.getPath(), data_handler, Curl::ostreamGetCallback);
        if (CURLE_OK == res)
        {
            // The target rename, if local, is part of the transfer
            // If unable to rename shall fail
            if (_site.hasTemporaryTargetName()) {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,  "Rename local file :" << targetFile << " to : " << localPath);
                res = _curl->lrename(targetFile, localPath);
            }
        }
        finalizeCurl(res, _site.getPath()); 
  
    }   // END SCOPE: untar_ostream

    fillGetStats();
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "permissions : " << ftpCurlPerm.permissions );
    if (ftpCurlPerm.permissions != 0) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Change local permissions : " << localPath << " to " << ftpCurlPerm.permissions);
        _curl->lchmod(localPath, ftpCurlPerm.permissions);
    }
    if (ftpCurlPerm.userId >= 0 || ftpCurlPerm.groupId >= 0) {
        // If userId is equal to -1, the ownership is not changed
        // If groupId is equal to -1, the ownership is not changed
        _curl->lchown(localPath, ftpCurlPerm.userId, ftpCurlPerm.groupId);
    }
    
    

}


void FtpCurl::put(bool alsoRemove, StopController const& stopController) 
{
    put(alsoRemove, 0, stopController);
}

void FtpCurl::put(bool alsoRemove, mode_t permissions, StopController const& stopController) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "PutWithStatus: " << str());
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Permissions : " << permissions);
    // Verify remote site but also check if the site is passive or active ( only protools which support it) 
    verifyRemoteSite(true);
    string remoteFile = (_site.getRemoteFileName().empty() ? _localFile : _site.getRemoteFileName()); // PRQA S 3384
    remoteFile = StringUtils::pathJoin(_site.getRemoteDir(), remoteFile) ;
    string remoteFilename = _site.getTemporaryPath();
    string localFilename = StringUtils::pathJoin(_localPath, _localFile) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[putWithLocalFile: " << localFilename << " AndRemoteFile: " << remoteFile << " ]");

    initializeCurl(OpPut, stopController);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Put " << localFilename << " to " << remoteFilename);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_site.hasTemporaryTargetName() " << boolalpha << _site.hasTemporaryTargetName() << " final " << remoteFile);
    CURLcode res = CURLE_OK ;
    if (_site.hasTemporaryTargetName()) {
        res = _curl->putRename(localFilename, remoteFilename, remoteFile, alsoRemove, permissions);
    }
    else {
        res = _curl->put(localFilename, remoteFilename, permissions);
    }
    
    finalizeCurl(res, localFilename);
    fillPutStats() ;
}


void FtpCurl::putAsTar(const File::CompressionMode& cm, const bool encapsulate_dir,  const mode_t& permissions, StopController const& stopController) 
{
    /**
        Note that to keep the I/F simpler, the flag alsoRemove to remove the destination file BEFORE renaming is always supposed true.
    */
    const bool alsoRemove ( true ) ;    // It might be added to the I/F if needed.

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "PutWithStatus: " << str());
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Permissions : " << permissions);
    // Verify remote site but also check if the site is passive or active (FTP sites only)
    verifyRemoteSite(true);
    // Get the localFilename -- it might be just a directory ("/raid0") or a file full path ("/raid0/file1") accordingly to generate the tar of a directory or a single file.
    const string localFilename ( _localFile.empty()? _localPath : StringUtils::pathJoin(_localPath, _localFile) ) ;
    string remoteFile ;
    if (true ==  _site.getRemoteFileName().empty() )
    { remoteFile = File::getFileName(localFilename) + generate_compression_ext(cm) ; }
    else 
    { remoteFile = _site.getRemoteFileName() ; } 
    remoteFile = StringUtils::pathJoin(_site.getRemoteDir(), remoteFile) ;
    string remoteFilename = _site.getTemporaryPath();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Local entry: \"" << localFilename << "\" RemoteFile: \"" << remoteFile << "\"  remoteFilename: \"" << remoteFilename << "\"");

    initializeCurl(OpPutAsTar, stopController);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Put " << localFilename << " to " << remoteFilename);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_site.hasTemporaryTargetName() " << boolalpha << _site.hasTemporaryTargetName() << " final " << remoteFile);
    CURLcode res = CURLE_OK ;

    // Generate a local iftarstream instance
    {
		// Handle the tar block multiplication factor (also check the file-threshold size)
		size_t tar_block_factor (_default_tar_block_factor) ;

		if ( (get_tar_block_factor() > 0) && (get_tar_block_factor() != _default_tar_block_factor) )
		{
			if ( (true == File::exists(localFilename)) && (false == File::isDir(localFilename)) )
			{
				// For normal files: compute the size and verify vs. threshold:
				const off64_t file_size (File::size(localFilename)) ;
				const off64_t threshold_bytes (1LL * (512.0 * get_tar_block_factor() * get_tar_block_factor_threshold())) ;
								
				if (file_size > threshold_bytes) { tar_block_factor = get_tar_block_factor() ; }
			} else {
				// For directories: just apply the provided block factor (skip computation of size)
				tar_block_factor = get_tar_block_factor() ;
			}
		}
		
        iftarstream tar_istream(localFilename.c_str(), cm, tar_block_factor, encapsulate_dir) ;

        if (_site.hasTemporaryTargetName()) {
            res = _curl->putRenameWithCallback(localFilename, remoteFilename, remoteFile, alsoRemove, permissions, &(tar_istream()), Curl::istreamPutCallback);
        }
        else {
            res = _curl->putWithCallback(localFilename, remoteFilename, permissions, &(tar_istream()), Curl::istreamPutCallback);
        }
    }   // END SCOPE: tar_istream
        
    finalizeCurl(res, localFilename);
    fillPutStats() ;
}



void FtpCurl::putDir(string const& localDirectory, string const& remoteDirectory, FileFilter fileFilter,
        StopController const& stopController) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[putDirWithLocalDirectory: " << localDirectory << " AndRemoteDirectory: "<< remoteDirectory <<"]");
    CURLcode res = CURLE_OK;
    vector < string > transferredFiles;
    vector < string > rootFiles;
    string targetDirectory = remoteDirectory;
    if (StringUtils::isBlank(remoteDirectory)) {
        targetDirectory = _site.getPath();
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Put Directory: " << localDirectory << " into: " << targetDirectory);
    ACS_COND_THROW(_site.getHost().empty(), FtpCurlUnrecoverableErrorException("Host name not provided")); // PRQA S 3081
    DirTree localDir(localDirectory, "/", true);
    _localPath = localDirectory;
    string remoteFile;
    _site.setPath(targetDirectory);
    string destinationRootPath = _site.getPath();
    string tempRootPath = _site.getTemporaryPath();
    string remotePath = tempRootPath;
    size_t filesTransferred = 0;
	setupVerifyHostCertificate(_site.getHost()) ;
    try {
        initializeCurl(OpPutTree, stopController);
        _curl->setCreateMissingDirs(true);
        _curl->setDefaultListCallback();

        bool resourceExists = false;
        FtpCurl probe(band_priority_, _site);
        if ( conf::CurlConfigurerSingleton::instance()->getFastProbeModeEnabled() )
        {
            // Fast mode (using two connections in the worst case)
            // do not emit exception just check if resource exists
            bool isDir = false ;
            resourceExists = probe.existRemoteResource (isDir, stopController, false);
        }
        else
        {
            // Old mode. Using just one connection.
            string dummy;
            RemoteResourceType rrt = probe.probeRemoteResource(dummy, stopController, false, false);
            resourceExists = (rrt != RRTNotExist);
        }

        ACS_COND_THROW(resourceExists, FtpCurlUnrecoverableErrorException("Cannot transfer the directory: it already exists!"));  // PRQA S 3081
        
        string tempTargetDirectory = _site.getTemporaryPath(targetDirectory);
        tempTargetDirectory = StringUtils::trimEnd(tempTargetDirectory, '/');
        size_t localDir_size = localDir.size() ;
        for (size_t counter = 0; counter < localDir_size; ++counter) {
            _localFile = localDir[counter];
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "localFilename :  " << _localFile);
            if (!acs::File::isRegularFile(_localFile)) {
                if (acs::File::isDir(_localFile)) {
                    //create the correspondent remote directory (even if this dir has no files inside).
                    //It should not be needed, but in this manner whe make sure
                    //we create remotely empty directories.
                    try {
                        remoteFile = remotePath + "/" + _localFile.substr(localDirectory.length(), _localFile.npos);
                        _curl->mkDir(remoteFile);
                    } 
                    catch (exception const& ex) {
                        //ignored.
                    }
                }
                continue;
            }
            remoteFile = remotePath + "/" + _localFile.substr(localDirectory.length(), _localFile.npos);
            if (fileFilter(_localFile.substr(localDirectory.length(), _localFile.npos))) {
                ++filesTransferred;
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file to copy:    " << _localFile);
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "to:              " << remoteFile);
                string destRemoteFile = destinationRootPath + "/"
                        + _localFile.substr(localDirectory.length(), _localFile.npos);
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "dest:            " << destRemoteFile);
                CURLcode res2 = _curl->put(_localFile, remoteFile);
                string parentFolder = File::getDirPath(remoteFile, true);
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "parent dir:      " << parentFolder);
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "root target dir: " << targetDirectory);
                throwConditionedError(res2, remoteFile, __PRETTY_FUNCTION__, __LINE__);
                fillPutStats();
                transferredFiles.push_back(remoteFile);
            }
        }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Must rename:           " << _site.getTemporaryPath(targetDirectory));
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "        -->:           " << targetDirectory);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " Is change strategy?: " << _site.hasTemporaryTargetName());
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Strategy temp name: " << _site.getRenameStrategy()); 
        if (_site.hasTemporaryTargetName()) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "renaming: " << tempRootPath << " to: " << targetDirectory);
            CURLcode res2 = _curl->rename(tempRootPath, targetDirectory);
            throwConditionedError(res2, targetDirectory, __PRETTY_FUNCTION__, __LINE__);
        }
        finalizeCurl(res, remoteDirectory);
    } 
    catch (exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex.what());
        try {
            for (vector<string>::const_iterator iter = transferredFiles.begin(); iter != transferredFiles.end(); // PRQA S 4238
                    ++iter) {
                string tempFile = *iter;
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "deleting: " << tempFile);
                CURLcode res2 = _curl->del(tempFile);
                throwConditionedError(res2, tempFile, __PRETTY_FUNCTION__, __LINE__);
            }
        } 
        catch (exception const& ex2) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex2.what());
        }
        finalizeCurl(res, remoteDirectory);
        throw;
    }
}

void delDirCallback(const string &line, void* data) 
{
    vector<FtpListItem> *lines = reinterpret_cast<vector<FtpListItem>*>(data); // PRQA S 3081
    FtpListItem item;
    item.setLine(line);
    if (!StringUtils::equalsNoCase("..", item.getName()) && !StringUtils::equalsNoCase(".", item.getName())) {
        lines->push_back(item);
    }
}

void FtpCurl::delDirTree(string const &remoteDirectory, StopController const& stopController, bool logError)  // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Delete remote directory: " << remoteDirectory);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Object dump:  " << endl << str());
    ACS_COND_THROW(_site.getHost().empty(), FtpCurlException("Host name not provided")); // PRQA S 3081 2 
    ACS_COND_THROW(remoteDirectory.empty(), FtpCurlException("Remote Directory not specified"));
    
    ACS_COND_THROW( _site.getIsHttp(), FtpCurlUnrecoverableErrorException("Cannot delete \""+remoteDirectory+"\" directory: for http(s) protocol directories are not managed") ) ;
    
    createCurl();
    string remoteFile;
    string remotePath = remoteDirectory;
    string originalPath = _site.getPath();
    _site.setPath(remoteDirectory);
    string errorMessage;
    

    if ( conf::CurlConfigurerSingleton::instance()->getFastProbeModeEnabled() )
    {
        // Fast mode (using two connections in the worst case)
        bool isDir = false;
        bool resourceExists = false;
        try
        {
            // Emit exceptions if needed
            resourceExists = existRemoteResource (isDir, stopController, true);
        }
        catch (exception& ex)
        {
            // Unable to detect if the resource exists
            // Try to delete on the filesystem if local
#warning MF Cosa succede se i siti sono ftp con chroot e si passa a path assoluti? Rivedere tutta questa logica

            // Just to stick with the old implementation. 
            // If logError is false errorMessage will be always empty and no check on the localhost is performed
            if (_site.isLocalhost() && logError ) 
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot remove in localhost using ftp/sftp/ftpes protocols. ");
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Using directly filesystems utilities.");
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing path: " << _site.getPath());
                File::remove(_site.getPath());
            } 
            else 
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, string("An Error occurred: ") + ex.what()); // PRQA S 3081
                ACS_THROW(FtpCurlRemoteRetrieveErrorException(ex, "An Error occurred")); // PRQA S 3081
            }
            
        }

        // what if the resource does not exist? Throw exception
        if ( false == resourceExists )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The remote directory does not exist.");
            ACS_THROW(FtpCurlRemoteRetrieveErrorException("The remote directory does not exist.")); // PRQA S 3081
        }
    }
    else
    {
        // Old mode. Using just one connection.
        // 0 is do not exists
        if (0 == probeRemoteResource(errorMessage, stopController, false, logError)) 
        {
            if (! errorMessage.empty() ) 
            {
                if (_site.isLocalhost()) 
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot remove in localhost using ftp/sftp/ftpes protocols. ");
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Using directly filesystems utilities.");
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing path: " << _site.getPath());
                    File::remove(_site.getPath());
                } 
                else 
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "An Error occurred: " + errorMessage);
                    ACS_THROW(FtpCurlRemoteRetrieveErrorException("An Error occurred: " + errorMessage)); // PRQA S 3081
                }
            } 
            else 
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The remote directory does not exist.");
                ACS_THROW(FtpCurlRemoteRetrieveErrorException("The remote directory does not exist.")); // PRQA S 3081
            }
            _site.setPath(originalPath);
            return;
        }
    }


    deque < string > subDirs;
    deque < string > emptyDirs;
    subDirs.push_back(remoteDirectory);
    emptyDirs.push_back(remoteDirectory);
    while (!subDirs.empty()) {
        remotePath = subDirs.front();
        subDirs.pop_front();
        vector<FtpListItem> lines;
        _site.setPath(remotePath);
        list(delDirCallback, &lines);

        size_t lines_size = lines.size() ;
        for (size_t counter = 0; counter < lines_size; ++counter) {
            remoteFile = lines[counter].getName();
            if (!lines[counter].getPermissions().isDirectory()) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "deleting remote file: " << remoteFile);
                _site.setPath(remotePath + "/" + remoteFile);
                del();
            } 
            else {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found a new subdir : " << remotePath + "/" + remoteFile);
                subDirs.push_back(remotePath + "/" + remoteFile);
                emptyDirs.push_back(remotePath + "/" + remoteFile);
            }
        }
    }
    while (!emptyDirs.empty()) {
        remotePath = emptyDirs.back();
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Remove the empty dir: " << remotePath);
        emptyDirs.pop_back();
        _site.setPath(remotePath);
        delDir();
    }
    _site.setPath(originalPath);
}

void FtpCurl::getDir(string const &localTargetDir, string const &remoteSourceDir, StopController const& stopController) 
{
    FtpCurlPermissions ftpCurlPerm;
    getDir(localTargetDir, ftpCurlPerm, remoteSourceDir, stopController);
}

void FtpCurl::getDir(string const &localTargetDir, const FtpCurlPermissions& ftpCurlPerm, string const &remoteSourceDir, StopController const& stopController) 
{
    string remoteDirectory = remoteSourceDir ;
    if (StringUtils::isBlank(remoteDirectory)) {
        remoteDirectory = _site.getPath();
    }
    string localDirectory = localTargetDir;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Get Directory: " << remoteDirectory << " into: " << localDirectory);
    ACS_COND_THROW(_site.getHost().empty(), FtpCurlUnrecoverableErrorException("Host name not provided")); // PRQA S 3081
    ACS_COND_THROW(remoteDirectory.empty(), FtpCurlUnrecoverableErrorException("Remote directory not specified."));  // PRQA S 3081 2
    ACS_COND_THROW(localDirectory.empty(), FtpCurlUnrecoverableErrorException("Local directory not specified."));
    ACS_COND_THROW( _site.getIsHttp(), FtpCurlUnrecoverableErrorException("Cannot delete \""+remoteDirectory+"\" directory: for http(s) protocol directories are not managed") ) ;
	setupVerifyHostCertificate(_site.getHost()) ;
	
    initializeCurl(OpGetTree, stopController);
    deque < string > subDirs;
    subDirs.push_back(remoteDirectory);
    string lastDirname = StringUtils::substringAfterLast(StringUtils::trimEnd(remoteDirectory, '/'), "/");
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "lastDirname:  " << lastDirname);
    ACS_COND_THROW(lastDirname.empty(), FtpCurlUnrecoverableErrorException("Wrong directory name"));  // PRQA S 3081
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exist local target dir? " << localDirectory);
    ACS_COND_THROW(File::exists(localDirectory), FtpCurlUnrecoverableErrorException("The local target directory already exist."));  // PRQA S 3081
    bool isChangeName = _site.hasTemporaryTargetName();
    if (isChangeName) {
        string localTempFile = _site.getTemporaryFile(localTargetDir); // returns the temporary dir name (without path).
        // the method getTemporaryDir() , for strategies like PREFIX or POSTFIX, return the parent folder of localTargetDir.
        string localTempDir = _site.getTemporaryDir(localTargetDir);
        localDirectory = localTempDir + "/" + localTempFile;
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exist local temp dir? " << localDirectory);
        ACS_COND_THROW(File::exists(localDirectory), FtpCurlUnrecoverableErrorException("The local temp directory already exist."));  // PRQA S 3081
    }

    try {
        File::mkdirtree(localDirectory);
    }
    catch( exception &e )
    {
        ACS_THROW( FtpCurlUnrecoverableErrorException(e, "Unable to create \"" + localDirectory + '\"') );
    }
    
    try {
        while (!subDirs.empty()) {
            string remotePath = subDirs.front();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "remotePath :  " << remotePath);
            subDirs.pop_front();
            vector<FtpListItem> lines;

            setNextLineCallback(delDirCallback, &lines);
            _curl->setListCallback(readLine_callback, this);
            CURLcode res = _curl->ls(remotePath);
            throwConditionedError(res, remotePath, __PRETTY_FUNCTION__, __LINE__);
            
            size_t lines_size = lines.size() ;
            for (size_t counter = 0; counter < lines_size; ++counter) {
                string remoteFile = lines[counter].getName();
                if (StringUtils::equalsNoCase("..", remoteFile) || StringUtils::equalsNoCase(".", remoteFile)) {
                    continue;
                }
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "remotePath :  " << remotePath);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "remoteFile :  " << remoteFile);
                string relativeRemoteName = remotePath.substr(remoteDirectory.length());
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relativeRemoteName  :  " << relativeRemoteName);
                if (!lines[counter].getPermissions().isDirectory()) {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mkdir :  " << localDirectory << "/" << relativeRemoteName);
                    File::mkdir(localDirectory + "/" + relativeRemoteName, true);
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Get From:  " << remotePath << "/" << remoteFile);
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " to :  " << localDirectory << "/" << relativeRemoteName << "/" << remoteFile);
                    
                    std::string localFile = localDirectory + "/" + relativeRemoteName + "/" + remoteFile;
                    CURLcode res = _curl->get(localDirectory + "/" + relativeRemoteName + "/" + remoteFile, // PRQA S 4412 2
                            remotePath + "/" + remoteFile);
                    throwConditionedError(res, remotePath + "/" + remoteFile, __PRETTY_FUNCTION__, __LINE__);
                    fillGetStats();
                    
                    if (ftpCurlPerm.permissions != 0) {
                        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Change local permissions : " << localFile << " to " << ftpCurlPerm.permissions);
                        _curl->lchmod(localFile, ftpCurlPerm.permissions);
                    }
                    if (ftpCurlPerm.userId != uid_t(-1) || ftpCurlPerm.groupId != gid_t(-1)) {
                        // If userId is equal to -1, the ownership is not changed
                        // If groupId is equal to -1, the ownership is not changed
                        _curl->lchown(localFile, ftpCurlPerm.userId, ftpCurlPerm.groupId);
                    }
                } 
                else {
                    File::mkdir(localDirectory + "/" + relativeRemoteName + "/" + remoteFile, true);
                    subDirs.push_back(remotePath + "/" + remoteFile);
                }
            }
        }
        _perfo.stop();
        if (isChangeName) {
            File::rename(localDirectory, localTargetDir);
        }
    } 
    catch (exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rollback in getDir(): deleting local dir recursively." << ex.what());
        _perfo.stop();
        try {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "deleting directory : " << localDirectory);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File::unlink(" << localDirectory << ");");
            File::unlink(localDirectory);
        } 
        catch (exception const& ex1) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Ignored error: " << ex1.what());
        }
        throw;
    }
}

void FtpCurl::rename(string const& remoteItem, string const& newRemoteItem, StopController const& stopController) 
{
    //rename the remote item (file) to a new name.
    verifyRemoteSite();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rename remote file: " << remoteItem << " to: " << newRemoteItem);
    initializeCurl(OpRename, stopController);
    CURLcode res = _curl->rename(remoteItem, newRemoteItem);
    finalizeCurl(res, remoteItem);
}

void FtpCurl::chmod(string const& remoteItem, mode_t permissions, StopController const& stopController) {
    //chmod the remote item (file).
    verifyRemoteSite();
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "chmod remote file: " << remoteItem << " to: " << permissions);
    initializeCurl(OpChMod, stopController);
    CURLcode res = _curl->chmod(remoteItem, permissions);
    finalizeCurl(res, remoteItem);
}

void FtpCurl::throwConditionedError(CURLcode res, const string &url, char const* function, int line) // PRQA S 4211
{
    if (res != CURLE_OK) {
        if (_curl && StringUtils::isNotBlank(_curl->getLastErrorMessage())) {
            throwCurlSpecificException(_curl->getLastErrorMessage(), res, function, line);
        } 
        else {
            ostringstream os;
            os << "FtpCurl error url: \"" << url << " curl returned: " << int(res) << " (" << curl_easy_strerror(res) // PRQA S 3081
                    << ")";
            throwCurlSpecificException(os.str(), res, function, line);
        }
    }
}

void FtpCurl::checkCURLcode(CURLcode res, const string &msg, char const* function, int line) const
{
    if (res != CURLE_OK) {
        throwCurlSpecificException(msg, res, function, line);
    }
}

void FtpCurl::getStats(vector<CirculationStatistics> & stats) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Storing " << _statistics.size() << " statistics.");
    stats.insert(stats.end(),_statistics.begin(), _statistics.end()) ;
    _statistics.clear();
}

long FtpCurl::getHttpResponseCode() const {
    ACS_COND_THROW(0 == _curl, exIllegalValueException("Curl object not initialized."));
    return _curl->getHttpResponseCode();
}

int FtpCurl::spawn(const string & exeName, const WaitPid::ProcArgs & procArgs, int timeout, bool & goodExit, string & spawnArgs)
{
    SpawnProc sp(exeName);
    sp.args(procArgs);
    
    ostringstream spawnArgsOs;
    size_t argSize = procArgs.size();
    for( size_t i = 0; i < argSize; i++ ) {
        spawnArgsOs << procArgs[i].arg() << " ";
    }
    spawnArgs = spawnArgsOs.str();

    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
        ostringstream msg;
        msg  << "Going to spawn executable \"" << exeName << "\" with the following arguments: " << spawnArgs;
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, msg.str()) ;
    ACS_CLASS_END_DEBUG
    sp.spawn();
    
    Timer watchdog ;
    bool wdTimeout = false ;
    watchdog.start() ;
    while(sp.isRunning())
    {
        Timer::delay(100); // PRQA S 4400
        if(watchdog.isOlder(timeout*1000)) // PRQA S 3000, 4400
        {
            sp.kill(); // too long: kill it
            wdTimeout = true ;
        }
    }
    
    int exitCode = -1 ;
    goodExit=true;
    if( !sp.exitCode(exitCode) ) {
        goodExit=false;
    }
    
    if(exitCode != 0) {
        ostringstream os ;
        if (wdTimeout) {
            os << watchdog.elapsed() / 1000 // PRQA S 3084, 4400
               << " [sec] elapsed since " << exeName << " was started. It is bigger than the timeout (" << timeout << " [sec]). The process was killed. " ;
            ACS_THROW(exIOException(os.str())) ; // PRQA S 3081
        }
        int exitSignal = 0 ;
        sp.exitSignal(exitSignal) ;
        if( 0 != exitSignal ) {
            os << exeName << " terminated with exitCode: " << exitCode  << " exitSignal: " << exitSignal ;
            ACS_THROW(exIOException(os.str())) ; // PRQA S 3081
        }
    }

    return exitCode;

}


void FtpCurl::fillFtpListReader(const string & filepath, curl::FtpListReader & reader) const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
    
    XMLIstream xml( filepath);
    rsResourceSet rs;
    
    xml >> rs;
    
    const string rootTag("List_of_files.File");
    size_t elements = rs.getArraySize( rootTag ); // PRQA S 3000, 3010, 4412
    for( size_t i=0; i < elements; i++)
    {
        string tag = rootTag;
        if( elements > 1 ) {
            ostringstream os ;
            os << "[" << i << "]" ;
            tag += os.str();
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=\"" << tag << '\"');
        
        string filename;
        rs.getValue( tag + ".Name", filename);
        if( filename == "Parent Directory" ) {
            continue;
        }
        
        string modificationDate;
        // date format is with no seconds e.g. 11-Nov-2014 09:41 
        rs.getValue( tag + ".ModificationDate", modificationDate);
        
        string day;
        string month;
        string year;
        string hour;
        {
            istringstream is(modificationDate);
            string date;
            is >> date;
            is >> hour;
            
            day = date.substr(0,2); // PRQA S 4400 3
            month = date.substr(3,3);
            year = date.substr(7,4);
            
        }
        
        size_t fileSize = 0 ;
        bool isDir = false ;
        rs.getValue( tag + ".IsDir", isDir);

// build the line to get to FtpListItem     
// -rw-r--r--    1 505      400      61675520 May 20  2013 ToPlot.tar
// drwxr-xr-x    2 505      400          4096 Nov 11 09:40 dir1

        ostringstream osLine;
        if( not isDir ) {
        
            string fileSizeStr;
            rs.getValue( tag + ".Size", fileSizeStr);
            // it must be parsed, since it could be e.g. 20M
            
            size_t fileSizeStrLen = fileSizeStr.length();

            size_t mFactor = 1;
            size_t numberLength = fileSizeStrLen-1; // PRQA S 3084 2
            char code = fileSizeStr[fileSizeStrLen-1];
            switch(code) {
            case 'K':
            case 'k': 
                {
                    mFactor = 1024;             // PRQA S 4400
                    break ;
                }
            case 'M':
                {
                    mFactor = 1024*1024;        // PRQA S 4400
                    break ;
                }
            case 'G':
                {
                    mFactor = 1024*1024*1024;   // PRQA S 4400
                    break ;
                }
            default:
                {
                    numberLength = fileSizeStrLen;
                    break ;
                }
            }
            string numberString = fileSizeStr.substr(0, numberLength);
            istringstream is( numberString );
            is >> fileSize;
            fileSize *= mFactor; // PRQA S 3084
            
            osLine << "-rw-r--r--    1";
        }
        else {
            fileSize = 4096;    // PRQA S 4400
            osLine << "drw-r--r--    2";
        }
        
        string user = "xxxxx";
        string group = "zzzzz";
        osLine << " " << user << " " << group << " " << fileSize << " " << month << " " << day; 
        
        
        {
            int yearNum = 0 ;
            istringstream is(year);
            is >> yearNum;
        
            DateTime now;
            DateTime::UTCD utcd;
            now.toUTCD(utcd) ;
            
        // if the same year of the current one print hour, otherwise year itself
            if( utcd.year == yearNum ) { // PRQA S 3000
                osLine << " " << hour;
            }
            else {
                osLine << " " << year;
            }
        }
        
        osLine << " " << StringUtils::trimEnd(filename, '/');
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ls line = \"" << osLine.str() << '\"');
        
        FtpListItem item;
        item.setLine(osLine.str()) ;
        reader.addListItem(item);
        
    }


}

void FtpCurl::fillPropFindStatItems( string const& remotePath, int depth, const string & xmlIn, const vector<string> & properties, bool directory, bool pathIsDir, vector<PropFindStatItem> & statItems ) const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
    
    XMLIstringstream xmlis( xmlIn);
    rsResourceSet rs;
    
    xmlis >> rs;

    ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, rs );


    const string rootTag("response");
    const size_t elements = rs.getArraySize( rootTag ); // PRQA S 3000, 3010, 4412
    
    const string remotePathFilename ( File::getFileName( StringUtils::trim(remotePath, '/') ) );
    for( size_t i=0; i < elements; i++) {
        string tag = rootTag;
        if( elements > 1 ) {
            ostringstream os ;
            os << "[" << i << "]" ;
            tag += os.str();
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=\"" << tag << '\"');
        
        PropFindStatItem item;
        string name;
        rs.getValue( tag + ".href", name);
        item.setName( File::getFileName( StringUtils::trimEnd(name, '/') ) );
        
        const string propStatRootTag = tag + ".propstat" ;
        const size_t propStatElements = rs.getArraySize( propStatRootTag );
        for( size_t j=0; j < propStatElements; j++) {
            tag = propStatRootTag;
            if( propStatElements > 1 ) {
                ostringstream os ;
                os << "[" << j << "]" ;
                tag += os.str();
            }
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "propstat TAG=\"" << tag << '\"');
            
            // check status
            string status;
            rs.getValue( tag + ".status", status);  
            
            // it relies on the following format as example: HTTP/1.1 200 OK
            if( status.find("200") == string::npos ) {
                continue;
            }
            
            rsPushNSpace pushedSpace( rs, tag );
            fillPropFindStatItemField( rs, properties, item ) ;
            
        }
        
        bool addItem = true;
        string itemName = item.getName();
        bool dirSet = false;
        bool isDir = item.getIsDir(dirSet);
        if( 0 != depth && StringUtils::equals( itemName, remotePathFilename ) && pathIsDir ) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemotePath \"" << remotePath << "\" is a directory and depth=" << depth << ": not added to output vector");
            addItem = false;
        }
        
        if( addItem && directory ) {
            if( not dirSet ) {
                // turn into error?
                ostringstream exMsg;
                exMsg << "Directories only requested to PROPFIND, but directory not set for \"" << itemName << "\" item: resourcetype property request returned error";
                ACS_THROW( FtpCurlUnrecoverableErrorException( exMsg.str() ) );
            }
            
            if( isDir ) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << item.getName() << "\" item is a directory");
            }
            else {
                addItem = false;
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << item.getName() << "\" item is a file");
            }
        }
        
        if( addItem ) {
            statItems.push_back(item);
        }
    }   
    
    
}

void FtpCurl::fillPropFindStatItemField( const rsResourceSet &rs, const vector<string> & properties, PropFindStatItem & item ) const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
    
    size_t propertiesSize = properties.size();
    for(size_t i = 0; i < propertiesSize; i++) {
    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to extract the following property: \"" << properties[i] << '\"');
        string tag =  "prop."+properties[i] ;
        if( not rs.hasKey(tag) ) {
            continue;
        }
        
        if( StringUtils::equals( properties[i], "resourcetype" ) ) {
            if( rs.hasKey( tag + ".collection") ) {
                item.setIsDir(true);
            }
            else {
                item.setIsDir(false);
            }
        }
        else {
            string propertyValue;
            
            rs.getValue( tag, propertyValue);
            if( StringUtils::equals( properties[i], "creationdate" ) ) {
// extract creation date: format is e.g. 2019-12-03T11:54:26Z (Zulu time) or 1997-12-01T18:27:21-08:00 (time zone 8 hours west of GMT).
                item.setCreationDate( creationdatePropFindWrapper(propertyValue) );
            }
            else if( StringUtils::equals( properties[i], "getlastmodified" ) ) {
// extract creation date: format is e.g. Tue, 03 Dec 2019 15:40:15 GMT
                struct tm tm0;
                if( strptime( const_cast<char*>(propertyValue.c_str()), "%a, %d %b %Y %H:%M:%S %z", &tm0 ) != 0 ) {
                    ACS_LOG_ERROR("Cannot extract modification time from \"" << item.getName() << "\" item" );
                }
                else {
                    item.setModificationDate( DateTime( DateTime::UTCD( (1900 + tm0.tm_year), (tm0.tm_mon + 1), tm0.tm_mday, tm0.tm_hour, tm0.tm_min, tm0.tm_sec )) );
                }


            }
            else if( StringUtils::equals( properties[i], "getcontentlength" ) ) {
                // get item length
                istringstream is(propertyValue);
                size_t tmpSize;
                is >> tmpSize ;
                item.setSize(tmpSize);
            }
        }
    
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The following item has been filled: \n" << item );

}

string FtpCurl::buildHttpPropFindBody( const vector<string> & properties )
{
// The request body format is the following : <propfind xmlns=\"DAV:\"><prop><resourcetype/><creationdate/><getlastmodified/><getcontentlength/></prop></propfind>

    string httpPropFindBody = "<propfind xmlns=\"DAV:\"><prop>";
    
    size_t propertiesSize = properties.size();
    for( size_t i = 0 ; i < propertiesSize; ++i ) {
        httpPropFindBody += ('<' + properties[i] + "/>" );
    }
    
    httpPropFindBody += "</prop></propfind>";
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Http PropFind Body=\"" << httpPropFindBody << '\"');
    return httpPropFindBody;

}

void FtpCurl::propFind( string const& remotePath, int depth, const vector<string> & properties, bool directory, bool isDir, vector<PropFindStatItem> & statItems, StopController const& sc )
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Remote path=\"" << remotePath << "\"; depth=" << depth << "; list directory=" << directory );
    
    string user = _site.getUser();
    string passwd = _site.getPassword();
    string userpwd;
    if( not user.empty() && not passwd.empty() ) {
        userpwd = user + ':' + passwd;
    } 
	setupVerifyHostCertificate(_site.getHost());

    initializeCurl(OpHttpPropFind, sc);
    
    string httpPropFindBody = buildHttpPropFindBody(properties);
    string output;
    CURLcode res = _curl->httpPropFind (remotePath, depth, output, httpPropFindBody, userpwd);
    try {
        finalizeCurl(res, remotePath);
    }
    catch( exception &e )
    {
        ACS_COND_THROW( _curl->getHttpResponseCode() == 404, FtpCurlHttpNotFoundException(e, "Resource not found") );
        throw;
    }
    
    long int httpResponseCode = _curl->getHttpResponseCode() ;
    if( httpResponseCode != 207 ) {
        ostringstream exMsg;
        exMsg << "Http response code: " << httpResponseCode << "; 207 expected for Multi-Status";
        ACS_THROW( FtpCurlUnrecoverableErrorException(exMsg.str()) );
    }
    
    fillPropFindStatItems( remotePath, depth, output, properties, directory, isDir, statItems );    
}

vector<string> FtpCurl::turnRequestsIntoProperties(const PropFindRequests & request)
{
    vector<string> properties;
    
    if( request & PROPFINDREQ_TYPE ) {
        map<PropFindRequests,string>::const_iterator it = propFindRequestsMap.find(PROPFINDREQ_TYPE);
        if( it == propFindRequestsMap.end() ) {
            ostringstream exMsg ;
            exMsg << "Internal error: " << int(PROPFINDREQ_TYPE) << " element not found in const static map";
            ACS_THROW(  FtpCurlUnrecoverableErrorException(exMsg.str() ) );
        }
        properties.push_back(it->second);
    }
    
    if( request & PROPFINDREQ_LENGTH ) {
        map<PropFindRequests,string>::const_iterator it = propFindRequestsMap.find(PROPFINDREQ_LENGTH);
        if( it == propFindRequestsMap.end() ) {
            ostringstream exMsg ;
            exMsg << "Internal error: " << int(PROPFINDREQ_LENGTH) << " element not found in const static map";
            ACS_THROW(  FtpCurlUnrecoverableErrorException(exMsg.str() ) );
        }
        properties.push_back(it->second);
    }

    if( request & PROPFINDREQ_CREATION ) {
        map<PropFindRequests,string>::const_iterator it = propFindRequestsMap.find(PROPFINDREQ_CREATION);
        if( it == propFindRequestsMap.end() ) {
            ostringstream exMsg ;
            exMsg << "Internal error: " << int(PROPFINDREQ_CREATION) << " element not found in const static map";
            ACS_THROW(  FtpCurlUnrecoverableErrorException(exMsg.str() ) );
        }
        properties.push_back(it->second);
    }
    
    if( request & PROPFINDREQ_MODIFICATION ) {
        map<PropFindRequests,string>::const_iterator it = propFindRequestsMap.find(PROPFINDREQ_MODIFICATION);
        if( it == propFindRequestsMap.end() ) {
            ostringstream exMsg ;
            exMsg << "Internal error: " << int(PROPFINDREQ_MODIFICATION) << " element not found in const static map";
            ACS_THROW(  FtpCurlUnrecoverableErrorException(exMsg.str() ) );
        }
        properties.push_back(it->second);
    }
    
    ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
        size_t propertiesSize = properties.size();
        ostringstream msg;
        for(size_t i = 0; i < propertiesSize; i++) {
            msg << "property[" << i << "]=\"" << properties[i] << "\"; ";
        }
        ACS_LOG_DEBUG(SimpleDebugSignature << msg.str() );
    ACS_CLASS_END_DEBUG

    
    return properties;

}

string FtpCurl::str() const // PRQA S 4214
{
    //ToString method: used for debug purpose only: it prints out all the internal object's status.
    ostringstream os;
    os << "FtpCurl{";
    os << "site:" << _site.str() << ",";
    os << "ldir:" << _localPath << ",";
    os << "lfile:" << _localFile << ",";
    os << "list:" << _listFileNamesOnly << ",";
    os << "priority:" << band_priority_.getBandPriority() << ",";
    BandPriority::Speed_bps speed = BandShaperSingleton::instance()->getBandLimit(band_priority_) ;
    if (0 != speed) {
        os << "speed:" << speed << ",";
    }
    return os.str();
}


void FtpCurl::handleCurlErrorBehaviour(CurlErrorBehaviourSingleton::CurlErrorBehaviour beh, const std::string& msg, const std::string& file, const std::string& func, int line) {
    switch (beh) {
    case CurlErrorBehaviourSingleton::UNRECOVERABLE:    // Throw an unrecoverable error exception
        exceptionThrower(FtpCurlUnrecoverableErrorException(msg), file, func, line); // PRQA S 3081
        break;
    case CurlErrorBehaviourSingleton::RECOVERABLE: // Default behaviour is recoverable error
        exceptionThrower(FtpCurlRecoverableErrorException(msg), file, func, line); // PRQA S 3081
        break;
    case CurlErrorBehaviourSingleton::UNDEFINED:
    default:
        // Noop
        std::ostringstream ss;
        ss << msg << " (Undefined Behaviour)";
        exceptionThrower(FtpCurlUnrecoverableErrorException(ss.str()), file, func, line); // PRQA S 3081
        break;
    }
}


void FtpCurl::throwCurlSpecificException(const string & msg, CURLcode res, string const& function, int line) 
{
    CurlErrorBehaviourSingleton::CurlErrorBehaviour ceb = CurlErrorBehaviourSingleton::instance()->getBehaviour(res);
    
    //convert the return code from Curl to an internal excpetion.
    //if the curl code is OK does nothing.
    switch (res) {
    // Use as-hoc cases for all the errors that uses specific exceptions.
    
    // With the introduction of the configurability of the curl error behaviours for all
    // those errors that have an ad-hoc exception defined, in order to maintain backward 
    // compatibility, the specific exceptions are thrown only if the behaviour for each curl
    // error (RECOVERABLE/UNRECOVERABLE) is the same as the type of the specific exception 
    // itself. UNDEFINED behaviours are always treated as UNRECOVERABLE errors.
    
    // E.g. Since FtpCurlLoginDeniedException is an unrecoverable exception it is thrown 
    // iff the configured behaviour for the CURLE_LOGIN_DENIED is not RECOVERABLE.
    
    case CURLE_OK: { break ; } // nothing 
    
    // UNRECOVERABLE
    case CURLE_LOGIN_DENIED:
        {
            if (ceb != CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlLoginDeniedException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlRecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // RECOVERABLE
    case CURLE_COULDNT_CONNECT:
        {
            if (ceb == CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlConnectionFailedException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlUnrecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // UNRECOVERABLE
    case CURLE_COULDNT_RESOLVE_HOST:
        {
            if (ceb != CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlHostnameNotFoundException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlRecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // UNRECOVERABLE
    case CURLE_FTP_COULDNT_RETR_FILE:
    case CURLE_REMOTE_FILE_NOT_FOUND: // PRQA S 1021
        {
            if (ceb != CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlRemoteRetrieveErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlRecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // UNRECOVERABLE
    case CURLE_REMOTE_ACCESS_DENIED:
        {
            if (ceb != CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlRemoteWriteErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlRecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // UNRECOVERABLE
    case CURLE_WRITE_ERROR:
        {
            if (ceb != CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlLocalWriteErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlRecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // RECOVERABLE
    case CURLE_FAILED_INIT:
        {
            if (ceb == CurlErrorBehaviourSingleton::RECOVERABLE) {
                exceptionThrower(FtpCurlInitializationFailedException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            else {
                exceptionThrower(FtpCurlUnrecoverableErrorException(msg), string(__FILE__), function, line); // PRQA S 3081
            }
            break;
        }
    // For all other curl errors use the generic handler
    default:
        {
            handleCurlErrorBehaviour(CurlErrorBehaviourSingleton::instance()->getBehaviour(res), msg, string(__FILE__), function, line);
            break;
        }
    }
}

void FtpCurl::produceStringLine(FtpCurl* ptrToFtpCurl) 
{
    //reads the current characters and produce a single line reading untill the carriage return or new line
    //has been reached. When the line is complete the callback will be invoked.
    size_t newLine = 0;
    size_t carriageReturn = 0;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Produce string line " << ptrToFtpCurl->_currentLine.length());
    while ((newLine != string::npos) || (carriageReturn != string::npos)) {
        newLine = ptrToFtpCurl->_currentLine.find('\n');
        if (newLine != string::npos) {
            ptrToFtpCurl->_nextLineCallback(ptrToFtpCurl->_currentLine.substr(0, newLine), ptrToFtpCurl->_data);
            ptrToFtpCurl->_currentLine.erase(0, newLine + 1); // PRQA S 3084
        }
        carriageReturn = ptrToFtpCurl->_currentLine.find('\r') ;
        if (carriageReturn != string::npos) {
            ptrToFtpCurl->_nextLineCallback(ptrToFtpCurl->_currentLine.substr(0, carriageReturn), ptrToFtpCurl->_data);
            ptrToFtpCurl->_currentLine.erase(0, carriageReturn + 1); // PRQA S 3084
        }
    }
}



size_t FtpCurl::readLine_callback(void* buffer, size_t size, size_t nmemb, void* data) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "readLine Callback called");
    //strSize is the size of the buffer read.
    size_t strSize = size * nmemb; // PRQA S 3084
    //if read something...
    if (0 != strSize) {
        FtpCurl* ptrToFtpCurl = reinterpret_cast<FtpCurl *>(data); // PRQA S 3081
        if (!ptrToFtpCurl) {
            ACS_LOG_DEBUG(SimpleDebugSignature << "cannot write: null stream") ;
            strSize = 0;
        } 
        else {
            //copy the buffer read from the remote curl connection .
            char* charBuffer = reinterpret_cast<char *>(buffer); // PRQA S 3081
            char str[strSize + 1]; // PRQA S 3084
            memset(str, 0, sizeof(str));
            strncpy(str, charBuffer, strSize);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Next Line: " << str);
            //append the new line read and produce a single line.
            ptrToFtpCurl->_currentLine.append(str);
            produceStringLine(ptrToFtpCurl);
        }
    }
    return strSize;
}
//
//End of Callbacks section
//

//
//Setters and Getters
//
void FtpCurl::setHost(const string &host) 
{
    _site.setHost(host);
}

string FtpCurl::getHost() const 
{
    return _site.getHost();
}

void FtpCurl::setUser(const string &user) 
{
    _site.setUser(user);
}

string FtpCurl::getUser() const 
{
    return _site.getUser();
}

void FtpCurl::setPwd(const string &pwd) 
{
    _site.setPassword(pwd);
}

string FtpCurl::getPwd() const 
{
    return _site.getPassword();
}

void FtpCurl::setRempath(const string &remotePath) 
{
    //@deprecated
    _site.setPath(StringUtils::trimEnd(remotePath, '/'));
}

const string &FtpCurl::getRempath() const ACS_NOEXCEPT 
{
    return _site.getPath();
}

void FtpCurl::setLocalFile(const string &localFile) // PRQA S 4121
{
    _localFile = localFile;
}

void FtpCurl::setRemoteFile(const string &remoteFile) 
{
    //@deprecated
    _site.setPath(_site.getPath() + "/" + remoteFile);
}

const string &FtpCurl::getLocalFile() const ACS_NOEXCEPT // PRQA S 4120
{
    return _localFile;
}

string FtpCurl::getRemoteFile() const 
{
    return _site.getRemoteFileName();
}

void FtpCurl::setLocalPath(const string &lpath) 
{
    _localPath = StringUtils::trimEnd(lpath, '/');
}

const string &FtpCurl::getLocalPath() const ACS_NOEXCEPT // PRQA S 4120
{
    return _localPath;
}

void FtpCurl::setPassiveMode(bool p) 
{
    _site.setIsPassive(p);
}

bool FtpCurl::getPassiveMode() const 
{
    return _site.getIsPassive();
}

void FtpCurl::setListFileNamesOnly(bool namesOnly) ACS_NOEXCEPT // PRQA S 4121
{
    _listFileNamesOnly = namesOnly;
}

bool FtpCurl::getListFileNamesOnly() const ACS_NOEXCEPT // PRQA S 4120
{
    return _listFileNamesOnly;
}

void FtpCurl::setSecureFtp(bool secure) 
{
    _site.setIsSecure(secure);
}

bool FtpCurl::getSecureFtp() const 
{
    return _site.getIsSecure();
}

void FtpCurl::setFtps() 
{
    _site.setIsFtps();
}

void FtpCurl::setFtp() 
{
    _site.setIsFtp();
}

void FtpCurl::setFtpes() 
{
    _site.setIsFtpes();
}

bool FtpCurl::getFtps() const ACS_NOEXCEPT
{
    return _site.getIsFtps();
}

bool FtpCurl::getFtpes() const ACS_NOEXCEPT
{
    return _site.getIsFtpes();
}

bool FtpCurl::getFtp() const ACS_NOEXCEPT
{
    return _site.getIsFtp();
}

void FtpCurl::setNextLineCallback(FtpCurl::NextLineCallback callBack, void* data) 
{
    _nextLineCallback = callBack;
    _data = data;
}

void FtpCurl::verifyRemoteSite(bool discover)
{
    ACS_COND_THROW(_site.getHost().empty(), FtpCurlException("Host name not provided")); // PRQA S 3081 2
    ACS_COND_THROW(_site.getPath().empty(), FtpCurlException("Path to remote resource not provided"));
	setupVerifyHostCertificate(_site.getHost()) ;
    if ( discover )
    {
        passiveActiveDiscover();
    }
}

FtpCurl::NextLineCallback FtpCurl::getNextLineCallback() const // PRQA S 4120
{
    return _nextLineCallback; // PRQA S 4628
}

const string &FtpCurl::getFullFilename() const ACS_NOEXCEPT 
{
    return _site.getPath();
}

void FtpCurl::setFullFilename(const string &filename) 
{
    _site.setPath(filename);
}

const RemoteSite &FtpCurl::getSite() const ACS_NOEXCEPT // PRQA S 4120
{
    return _site;
}

string FtpCurl::getFullLocalFilename() const // PRQA S 4020
{
    //concatenates the local path with the local file name.
    if (not _localPath.empty()) {
        return _localPath + "/" + _localFile;
    }
    return _localFile;
}

void FtpCurl::setFullLocalFilename(const string &filename) 
{
    _localPath = File::getDirPath(filename, true) ;
    _localFile = File::getFileName(filename) ;
}

void FtpCurl::setSite(RemoteSite const& site) // PRQA S 4121
{
    _site = site;
}

void FtpCurl::fillStats(bool isFromURL) 
{
    if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics() && 
        Application::exists() && 
        (Application::instance()->getDefaultConnectionPool() != 0)) {
        
        dbConnectionWrapper w(*Application::instance()->getDefaultConnectionPool()) ;
        dbConnection &conn = w ; // PRQA S 3050
        CirculationStatistics stats(conn) ;
        if (isFromURL) {
            stats.setSourceSite(_site);
        } 
        else {
            stats.setDestSite(_site);
        }
        _curl->fillInCirculationStatistics(isFromURL, &stats);
        _statistics.push_back(stats);
    }
}


//
//End of Setters and Getters section
//


/***********************************************************************************
******                                                                        ******
******                      generate_compression_ext                          ******
******                                                                        ******
***********************************************************************************/
string FtpCurl::generate_compression_ext(const File::CompressionMode& cm)
{
    string ext ; 
    switch (cm)
    {
        case File::Plain:       { ext = ".tar" ;    break ; } 
        case File::TARred:      { ext = ".tar" ;    break ; }
        case File::GZipped:     { ext = ".tgz" ;    break ; }
        case File::BZipped:     { ext = ".tbz" ;    break ; }
        case File::Compressed:  { ext = ".tar.Z" ;  break ; }
        default:        { ACS_THROW ( exIllegalValueException("Invalid File::CompressionMode") ) ; break ; }    
    }

    return ext ;
}


/***********************************************************************************
******                                                                        ******
******                      self_test__write_to_file                          ******
******                                                                        ******
***********************************************************************************/
void FtpCurl::self_test__write_to_file(const string& s, const string& filename) 
{
    ofstream ofile( filename.c_str(), ios::out|ios::binary|ios::trunc|ios::ate );

    if (ofile.fail()) { ostringstream o ; o << "Error opening file: \"" << filename << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }  
    ofile << s ; 
    if (ofile.bad()) { ostringstream o ; o << "Error writing to file: \"" << filename << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }    
    ofile.close() ; 

    return ; 
}


/***********************************************************************************
******                                                                        ******
******                      self_test__read_from_file                         ******
******                                                                        ******
***********************************************************************************/
string FtpCurl::self_test__read_from_file(const string& filename) 
{
    ACS_COND_THROW( (false==File::exists(filename)), exIOException("File not found: \"" + filename + "\"") ) ; 
    
    ifstream ifs( filename.c_str(), ios::in|ios::binary );
    if (ifs.fail()) { ostringstream o ; o << "Error opening file: \"" << filename << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }    

    ostringstream s ; s << ifs.rdbuf() ;
    ifs.close();

    return s.str() ; 
}


/***********************************************************************************
******                                                                        ******
******                     self_test__compare_from_file                       ******
******                                                                        ******
***********************************************************************************/
bool FtpCurl::self_test__compare_from_file(const string& s, const string& filename) 
{
    const string s_from_file( self_test__read_from_file(filename) ) ;
    
    return (s == s_from_file) ;
}


/***********************************************************************************
******                                                                        ******
******                          self_test__tar_untar                          ******
******                                                                        ******
***********************************************************************************/
int FtpCurl::self_test__tar_untar(const curl::RemoteSite& remote_site_in)
{
    /*
        An unsupervised self-test is run: if no exception is thrown to the caller, the test is successfull.
        The first test batch is executed 16 times, accordingly to the combination of: CM x E x R
          where: CM = compression mode (4: tar, tarZ, tgz, tbz), E = encapsulation (t/f), R = Rename Strategy (y/n): 4x2x2=16.
        The aim of each run is the following:
        - A local, constant reference directory tree is created
        - The tree is transferred to the remote site, with the mechanism of tar-on-the-fly
        - The tar is transferred from remote and locally untarred-on-the-fly in a output directory
        - The reference directory and the output directory should contain the same files.

        The second test batch is a contingency test, executed 4 times (tar, tarZ, tgz, tbz).
        Each time, an invalid compressed file is created and the decompression is attempted.
        The compressed file is quite short, so the decompression process will be stuck in starvation.
        Aim of the test is prove that when the curl terminates the underlaying process is terminated and an error code is returned as expected.
    */

    srand(time(NULL)) ; 

    const string dir1("dir1") ; 
    const string dir2("dir2") ; 
    const string file1("file1") ; 
    const string file2("file2") ; 
    const string file3(StringUtils::pathJoin(dir2,"file3")) ; 

    const string content_of_file1("Content_of_file1") ; 
    const string content_of_file2("CONTENT_of_file_2") ;
    const string content_of_file3("THE_CONTENT_OF_FILE3") ;
    
    {
        // Test-Batch #1 (Nominal Scenario)
    
        FileGuard FG__reference_dir( File::tempdir("/tmp/refdir") ) ;   const string reference_dir (FG__reference_dir) ;
        const string reference_dir_nopath(File::getFileName(reference_dir)) ; // Use in case of encapsulation, as further layer
        const string dir1_refpath( StringUtils::pathJoin(reference_dir, dir1) ) ; 

        File::mkdir( dir1_refpath ) ; 
        File::mkdir( StringUtils::pathJoin(dir1_refpath, dir2) ) ; 

        self_test__write_to_file( content_of_file1,  StringUtils::pathJoin(dir1_refpath, file1) ) ; 
        self_test__write_to_file( content_of_file2,  StringUtils::pathJoin(dir1_refpath, file2) ) ; 
        self_test__write_to_file( content_of_file3,  StringUtils::pathJoin(dir1_refpath, file3) ) ; 


        for (size_t k=0; k<16; k++)
        {
            FileGuard FG__output_dir( File::tempdir("/tmp/outdir") ) ;  const string output_dir (FG__output_dir) ; File::rmdir(output_dir) ; 

            File::CompressionMode tar_mode ; 

            switch (k & 0x3)
            {
                case 0x00: { tar_mode = File::Plain ;       break ; }   /* Binary: 0000 */
                case 0x01: { tar_mode = File::Compressed ;  break ; }   /* Binary: 0001 */
                case 0x02: { tar_mode = File::GZipped ;     break ; }   /* Binary: 0010 */
                case 0x03: { tar_mode = File::BZipped ;     break ; }   /* Binary: 0011 */
                default:    
                {}
            }
            const bool encapsulate_dir( (k & 0x4) != 0 ) ;              /* Binary: 0100 */
            const bool rename_strategy( (k & 0x8) != 0 ) ;              /* Binary: 1000 */ 

            // WHEN HERE: One among 16 scenarios of: (tar_mode; encapsulate_dir; rename_strategy) has been selected.

            curl::RemoteSite remote_site ( remote_site_in ) ; 
            if (false == rename_strategy) { remote_site.setRenameStrategy( temp::noTemp ,"") ; }   

            string remote_destination_tarfile ;
            {
                ostringstream o ; o << "/tmp/file_" << rand() ; 
                remote_destination_tarfile = o.str() ; 
            }
            const string remote_file ( remote_destination_tarfile + generate_compression_ext(tar_mode) ) ;  // Add the extension
            const mode_t permissions(0) ; // Use default

            cout << "------------------------------------------------------------------------------------------------------------------------" << endl ;
            cout << endl << "Test " << (k+1) << "/16: " << "Compression mode: " << tar_mode << "  Encapsulate dir: " << (encapsulate_dir?"T":"F") << "  Renaming strategy: " << (rename_strategy?"Y":"N") << endl << endl; 

            // Transfer from local reference to remote-tar
            {
                BandPriority no_priority ;
                FtpCurl ftpCurl(no_priority, remote_site);

                // Set the local entry and the remote directory:
                cout << "Setting local input reference directory: " << reference_dir << endl ;
                ftpCurl.setLocalFile(reference_dir);
                cout << "Setting remote file: " << remote_file << endl ;
                ftpCurl.setRempath(remote_file);

                // Transfer from local to remote
                ftpCurl.putAsTar(tar_mode, encapsulate_dir, permissions) ; 
            }

            // Transfer from remote-tar to local output and remove the remote-tar, to avoid garbage
            {
                BandPriority no_priority ;
                FtpCurl ftpCurl(no_priority, remote_site);

                // Set the local entry and the remote directory:
                cout << "Setting local output directory: \"" << output_dir << "\"" << endl ;
                ftpCurl.setLocalPath(output_dir);
                cout << "Setting remote file: \"" << remote_file << "\"" << endl ;
                ftpCurl.setRempath(remote_file);

                // Transfer from remote to local
                ftpCurl.getUnTar(tar_mode, permissions);    

                // Remote the remote file
                cout << "Test Successfull. After transfer, removing the remote file: \"" << remote_file << "\"" << endl ;
                ftpCurl.del() ;
            }

            // Verify the content:
            {
                string f ;

                string comp_dir ( output_dir ) ;
                if (true == encapsulate_dir) { comp_dir = StringUtils::pathJoin(comp_dir, reference_dir_nopath) ; }
                comp_dir = StringUtils::pathJoin(comp_dir, dir1) ; 

                f = StringUtils::pathJoin(comp_dir, file1) ;  cout << "Checking file content for: \"" << f << "\" ...\t" ; 
                ACS_COND_THROW( (false == self_test__compare_from_file(content_of_file1, f)), \
                    exIllegalValueException("Error comparing the content of file1") ) ;  
                cout << "Successfull." << endl ;

                f = StringUtils::pathJoin(comp_dir, file2) ;  cout << "Checking file content for: \"" << f << "\" ...\t" ; 
                ACS_COND_THROW( (false == self_test__compare_from_file(content_of_file2, f)), \
                    exIllegalValueException("Error comparing the content of file2") ) ;  
                cout << "Successfull." << endl ;

                f = StringUtils::pathJoin(comp_dir, file3) ;  cout << "Checking file content for: \"" << f << "\" ...\t" ; 
                ACS_COND_THROW( (false == self_test__compare_from_file(content_of_file3, f)), \
                    exIllegalValueException("Error comparing the content of file3") ) ;  
                cout << "Successfull." << endl ;
            }

        }   // END: for k
        
    }   // END: Test-Batch #1 (Nominal Scenario)


    {
        // Test-Batch #2 (Contingencies)

        for (size_t k=0; k<4; k++)
        {
            FileGuard FG__reference_dir( File::tempdir("/tmp/refdir") ) ;   const string reference_dir (FG__reference_dir) ;
            
            FileGuard FG__output_dir( File::tempdir("/tmp/outdir") ) ;  const string output_dir (FG__output_dir) ; File::rmdir(output_dir) ; 

            File::CompressionMode tar_mode ; 

            switch (k & 0x3)
            {
                case 0x00: { tar_mode = File::Plain ;       break ; }   /* Binary: 0000 */
                case 0x01: { tar_mode = File::Compressed ;  break ; }   /* Binary: 0001 */
                case 0x02: { tar_mode = File::GZipped ;     break ; }   /* Binary: 0010 */
                case 0x03: { tar_mode = File::BZipped ;     break ; }   /* Binary: 0011 */
                default:    
                {}
            }

            curl::RemoteSite remote_site ( remote_site_in ) ; 
            
            string file_name_1 ; { ostringstream o ; o << "file_" << rand() << generate_compression_ext(tar_mode) ; file_name_1 = o.str() ; }   
            const string remote_file (StringUtils::pathJoin("/tmp", file_name_1)) ; 
            const string reference_file (StringUtils::pathJoin(reference_dir, file_name_1)) ; 
            self_test__write_to_file( content_of_file1,  reference_file ) ; // The content is _NOT_ compressed -- so the file won't be decompressed.
            const mode_t permissions(0) ; // Use default

            cout << "------------------------------------------------------------------------------------------------------------------------" << endl ;
            cout << endl << "Contingency Test " << (k+1) << "/4: " << "Compression mode: " << tar_mode << endl << endl; 

            // Transfer from local reference to remote-tar
            {
                BandPriority no_priority ;
                FtpCurl ftpCurl(no_priority, remote_site);

                // Set the local entry and the remote directory:
                cout << "Setting local input reference file: " << reference_file << "  [THIS IS INTENTIONALLY A BROKEN COMPRESSED FILE]" << endl ;
                ftpCurl.setLocalFile(reference_file);
                cout << "Setting remote file: " << remote_file << endl ;
                ftpCurl.setRempath(File::getDirPath(remote_file));
                ftpCurl.setRemoteFile(File::getFileName(remote_file));

                // Transfer from local to remote
                ftpCurl.put(false) ; 
            }

            // Transfer from remote-tar to local output and remove the remote-tar, to avoid garbage
            {
                BandPriority no_priority ;
                FtpCurl ftpCurl(no_priority, remote_site);

                // Set the local entry and the remote directory:
                cout << "Setting local output directory: \"" << output_dir << "\"" << endl ;
                ftpCurl.setLocalPath(output_dir);
                cout << "Setting remote file: \"" << remote_file << "\"  [THIS IS INTENTIONALLY A BROKEN COMPRESSED FILE]" << endl ;
                ftpCurl.setRempath(remote_file);

                try 
                {
                    // Transfer from remote to local
                    ftpCurl.getUnTar(tar_mode, permissions);    
                } 
                catch (acs::curl::Curl::CurlException& e)
                {
                    excerr << "\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl ;
                    excerr << "Received exception, as expected since compressed file is indeed broken: " << endl ;
                    excerr << "\t" << e ; 
                    excerr << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n" << endl ;
                }

                // Remote the remote file
                cout << "Contingency test successfull. After interruped transfer, removing the remote file: \"" << remote_file << "\"" << endl ;
                ftpCurl.del() ;
            }
            
        }   // END: for k
        
    }   // END: Test-Batch #2 (Contingencies)

    return 0 ; 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////********  PropFindStatItem struct ctor, copy ctor and assignment operator **********/////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FtpCurl::PropFindStatItem::PropFindStatItem():
                            _name(),
                            _dir(false),
                            _dirSet(false),
                            _size(0),
                            _sizeSet(false),
                            _creationDate(),
                            _creationDateSet(false),
                            _modificationDate(),
                            _modificationDateSet(false)
{
}                               

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FtpCurl::PropFindStatItem::PropFindStatItem(FtpCurl::PropFindStatItem const &item):
                                _name(item._name),
                                _dir(item._dir),
                                _dirSet(item._dirSet),
                                _size(item._size),
                                _sizeSet(item._sizeSet),
                                _creationDate(item._creationDate),
                                _creationDateSet(item._creationDateSet),
                                _modificationDate(item._modificationDate),
                                _modificationDateSet(item._modificationDateSet)
{
}                               

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FtpCurl::PropFindStatItem & FtpCurl::PropFindStatItem::operator = (FtpCurl::PropFindStatItem const &item)
{
    if( this != &item ) {
        _name = item._name;
        _dir = item._dir;
        _dirSet = item._dirSet;
        _size = item._size;
        _sizeSet = item._sizeSet;
        _creationDate = item._creationDate;
        _creationDateSet = item._creationDateSet;
        _modificationDate = item._modificationDate;
        _modificationDateSet = item._modificationDateSet;
    }
    return (*this);
}

void FtpCurl::PropFindStatItem::setName( const string & v)
{
    _name = v;
}                       

void FtpCurl::PropFindStatItem::setIsDir( const bool & v)
{
    _dir = v;
    _dirSet = true;
}   

void FtpCurl::PropFindStatItem::setSize( const long long int & v)
{
    _size = v;
    _sizeSet = true;
}                       


void FtpCurl::PropFindStatItem::setCreationDate( const DateTime & v)
{
    _creationDate = v;
    _creationDateSet = true;
}                       

void FtpCurl::PropFindStatItem::setModificationDate( const DateTime &v)
{
    _modificationDate = v;
    _modificationDateSet = true;
}                       




string FtpCurl::PropFindStatItem::getName() const
{
    return _name ;
}                       

bool FtpCurl::PropFindStatItem::getIsDir( bool &isSet) const
{
    isSet = _dirSet;
    return _dir ;
}   

long long int FtpCurl::PropFindStatItem::getSize( bool &isSet) const
{
    isSet = _sizeSet;
    return _size ;
}                       


DateTime FtpCurl::PropFindStatItem::getCreationDate( bool &isSet) const
{
    isSet = _creationDateSet;
    return _creationDate ;
}                       

DateTime FtpCurl::PropFindStatItem::getModificationDate( bool &isSet) const
{
    isSet = _modificationDateSet;
    return _modificationDate ;
}                       
                    

ostream &operator<<(ostream &s, const FtpCurl::PropFindStatItem & v)
{
    s   << "************************** PropFind Stat Item ************************\n"
        << "\tname=\""              << v.getName()                          << "\"\n";
        
    bool isSet = false;
    bool isDir = v.getIsDir(isSet);
    if( isSet ) {
        s << "\tdir=" << boolalpha  << isDir                                << '\n';
    }
    else {
        s << "\tdir=*************************UNDEFINED****************************\n";
    }
    
    long long int itemSize = v.getSize(isSet);
    if( isSet ) {
        s << "\tsize="              << itemSize                             <<  "\n" ;
    }
    else {
        s << "\tsize=************************UNDEFINED****************************\n";
    }
    
    DateTime creationDate = v.getCreationDate(isSet) ;
    if( isSet ) {
        s << "\tcreationDate=\""    << creationDate.YYYYMMDDThhmmss()       << "\"\n";
    }
    else {
        s << "\tcreationDate=****************UNDEFINED****************************\n";
    }
    
    DateTime modificationDate = v.getModificationDate(isSet) ;
    if( isSet ) {
        s << "\tmodificationDate=\"" << modificationDate.YYYYMMDDThhmmss()  << "\"\n" ;
    }
    else {
        s << "\tmodificationDate=************UNDEFINED****************************\n";
    }
    s << "**************************************************************************\n";
    
    return s ;
}

exostream &operator<<(exostream &s, const FtpCurl::PropFindStatItem & v)
{
    s   << "************************** PropFind Stat Item ************************\n"
        << "\tname=\""              << v.getName()                          << "\"\n";
        
    bool isSet = false;
    bool isDir = v.getIsDir(isSet);
    if( isSet ) {
        s << "\tdir=" << boolalpha  << isDir                                << '\n';
    }
    else {
        s << "\tdir=*************************UNDEFINED****************************\n";
    }
    
    long long int itemSize = v.getSize(isSet);
    if( isSet ) {
        s << "\tsize="              << itemSize                             <<  "\n" ;
    }
    else {
        s << "\tsize=************************UNDEFINED****************************\n";
    }
    
    DateTime creationDate = v.getCreationDate(isSet) ;
    if( isSet ) {
        s << "\tcreationDate=\""    << creationDate.YYYYMMDDThhmmss()       << "\"\n";
    }
    else {
        s << "\tcreationDate=****************UNDEFINED****************************\n";
    }
    
    DateTime modificationDate = v.getModificationDate(isSet) ;
    if( isSet ) {
        s << "\tmodificationDate=\"" << modificationDate.YYYYMMDDThhmmss()  << "\"\n" ;
    }
    else {
        s << "\tmodificationDate=************UNDEFINED****************************\n";
    }
    s << "**************************************************************************\n";
    
    return s ;
}



_ACS_END_NAMESPACE
