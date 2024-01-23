/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S. libFtpCurl  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.74  2017/12/04 09:43:08  marpas
 *  getHttpResponseCode method added to retrun the last http response code got
 *
 *  Revision 5.73  2017/11/23 09:54:10  marpas
 *  custom headers are now stored as strings and not pairs
 *  this allow to offer a method to set a custom header externally built
 *
 *  Revision 5.72  2017/11/22 14:02:41  marpas
 *  custom headers concept added (will be used when in transaction)
 *  httppost method added (allows user authentication before the host)
 *  custom headers can be added calling addCustomHeader
 *
 *  Revision 5.71  2017/11/22 11:38:52  marpas
 *  post method now accepts "username:password" as last argument (defaulted to empty string)
 *
 *  Revision 5.70  2017/10/19 15:14:20  marpas
 *  qa warning removed
 *  code best practice implemented
 *
 *  Revision 5.69  2017/10/16 16:51:56  marpas
 *  findProxy signature improved
 *
 *  Revision 5.68  2017/07/26 08:03:45  lucio.pulvirenti
 *  S1PDGS-31936: setConnectionTimeout_ms: added optional bool parameter (default true) to adjust connection timeout.
 *  ls: proper timeout set and old saved to be reset at the end.
 *
 *  Revision 5.67  2017/05/17 14:05:43  marpas
 *  httpget has a default parameter to set "user:pwd" during http request - NB THIS DO NOT AFFECT INTERNAL CLASS MEMBER
 *
 *  Revision 5.66  2017/05/05 16:14:06  lucio.pulvirenti
 *  S3PDGS-3137: CURLOPT_FTP_RESPONSE_TIMEOUT set by a dedicated timeout parameter got by configuration.
 *
 *  Revision 5.65  2017/04/13 10:16:22  marfav
 *  Added support to ftp command execution without asking for directory content (GET)
 *
 *  Revision 5.64  2017/04/06 15:35:52  marfav
 *  S2PDGS-1748
 *  Work In Progress
 *
 *  Revision 5.63  2017/03/28 16:07:47  marpas
 *  httpget method now reports the error
 *
 *  Revision 5.62  2017/02/14 16:15:57  marpas
 *  Curl::post and Curl::httpget signature is more robust now
 *
 *  Revision 5.61  2016/09/26 16:17:24  francesco.avanzi
 *  Now curl::performTransferRenameChmod does a runtime version check to decide
 *  whether to use *-prefix or not on the quote remove commands
 *
 *  Revision 5.60  2016/05/09 14:29:56  francesco.avanzi
 *  performTransfer() and performTransferRenameChmod() calls have been modified according to
 *  new signatures. default parameters are no longer allowed, in order to avoid unwanted casts
 *  and therfore unexpected behaviours
 *
 *  Revision 5.59  2016/05/05 13:10:26  marpas
 *  warning removed and doc updated
 *
 *  Revision 5.58  2016/04/21 13:24:42  francesco.avanzi
 *  S2PDGS-1431 Probing is now done using the new probeLs() function
 *  such functions stops the LS (therefore the data transfert) the very moment
 *  it finds out whether the site is passive or active.
 *
 *  Revision 5.57  2016/03/22 16:28:43  francesco.avanzi
 *  S2PDGS-1434: performtransferRenameChmod  modified to remove targetFinalFile if needed
 *
 *  Revision 5.56  2016/03/21 18:25:54  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.55  2016/03/18 13:17:12  francesco.avanzi
 *   S2PDGS-1434  Curl::put  modified:a bool is now taken in account to regulate target cleanup
 *
 *  Revision 5.54  2016/03/15 11:18:44  marfav
 *  Extending support to older libCurl version with specific defines
 *
 *  Revision 5.53  2015/11/27 10:30:12  francesco.avanzi
 *  WARNING: STILL TO BE TESTED
 *  Curl now sunders between the two following cases:
 *  1) No Proxy is configured, therefore CURLOPT_NOPROXY is not set by curl_easy_setopt
 *  2) a NoProxy is set into configuration, therefore curl_easy_setopt sets CURLOPT_NOPROXY
 *
 *  Revision 5.52  2015/10/22 15:37:41  marpas
 *  some messages moved from info filterable class to private info
 *
 *  Revision 5.51  2015/06/03 13:23:52  damdec
 *  CURL initialization performed by means of InitCallbackManagerSingleton.
 *
 *  Revision 5.50  2015/05/11 16:21:58  marfav
 *  S2PDGS-1218 SSL library initialization enforced at link time
 *
 *  Revision 5.49  2015/04/28 09:57:39  marfav
 *  CURLOPT_FTP_USE_EPRT option parameter fixed
 *
 *  Revision 5.48  2015/04/23 17:45:55  marpas
 *  when putting a file to a remote system, in case o renaming, the latter is performed in the same connection that actually transfered the file.
 *  the concept is still valid for stream transfers where the endpoint is a file.
 *  Even the final chmod is done accordingly (valid only for FtpCurl transfers, not fo CirculationCurl ones).
 *  It does not applies to directory because their children have to be transferred BEFORE the renaming.
 *
 *  Revision 5.47  2015/04/16 13:22:51  marpas
 *  new putRename method implementation in progress
 *
 *  Revision 5.46  2015/04/09 15:55:38  marfav
 *  S1PDGS-31623 adding HTTP GET operation (on string)
 *
 *  Revision 5.45  2015/03/26 10:23:02  marfav
 *  Curl global init shall not init again SSL crypto engine
 *
 *  Revision 5.44  2015/03/10 14:44:35  lucio.pulvirenti
 *  S2PDGS-984: openConnection: try catch to throw CurlUnrecoverableException if RemoteSiteUtility::CryptException detected.
 *  get: try-catch block to throw CurlUnrecoverableException if CurlFile::open method fails.
 *  probe: added default parameters to valorize CURLCode.
 *
 *  Revision 5.43  2015/03/09 17:00:20  lucio.pulvirenti
 *  S2PDGS-984: work in progress.
 *
 *  Revision 5.42  2015/03/09 15:13:59  marfav
 *  cout debug strings removed
 *
 *  Revision 5.41  2015/02/19 14:49:19  francesco.avanzi
 *  Ciphers are now set if requested.
 *  Set takes place at CURLOPT_SSL_CIPHER_LIST.
 *
 *  Revision 5.40  2014/12/09 11:40:45  francesco.avanzi
 *  modified to use new ContentType
 *
 *  Revision 5.39  2014/11/18 15:42:48  lucio.pulvirenti
 *  S2PDGS-915: set of CURLOPT_FAILONERROR moved in openConnection method.
 *
 *  Revision 5.38  2014/11/13 16:49:51  lucio.pulvirenti
 *  S2PDGS-915: Work in progress.
 *
 *  Revision 5.37  2014/11/04 18:37:04  lucio.pulvirenti
 *  CURLOPT_HTTPPROXYTUNNEL option set in case of proxy http protocol.
 *
 *  Revision 5.36  2014/11/04 17:21:11  marpas
 *  if proxy protocol is not empty, it will be used as in <prtcl>://
 *
 *  Revision 5.35  2014/11/04 11:06:36  marpas
 *  CurlFile handling improved
 *
 *  Revision 5.34  2014/11/03 17:08:33  marpas
 *  setting http response > 400 as error
 *
 *  Revision 5.33  2014/11/03 17:03:11  marpas
 *  simplified, init condition shall not be verified from external
 *
 *  Revision 5.32  2014/10/30 10:32:32  marpas
 *  qa warnings removed
 *
 *  Revision 5.31  2014/10/29 16:05:42  lucio.pulvirenti
 *  S2PDGS-955: INFO turned into DEBUG msg.
 *
 *  Revision 5.30  2014/10/29 13:26:23  lucio.pulvirenti
 *  S2PDGS-955: setProxy(string) method removed. New setProxy method implemented setting proxy options if a proxy is configured for the current host.
 *
 *  Revision 5.29  2014/10/28 16:31:43  lucio.pulvirenti
 *  S2PDGS-955 work in progress.
 *
 *  Revision 5.28  2014/07/03 07:43:03  marpas
 *  useless method removed
 *
 *  Revision 5.27  2014/06/20 14:52:33  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.26  2014/06/19 17:58:11  francesco.avanzi
 *  better coding rules applied
 *
 *  Revision 5.25  2014/06/05 10:51:15  marfav
 *  Ifdef on CURL share. Disabled by default
 *
 *  Revision 5.24  2014/06/04 17:29:20  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.23  2014/05/06 17:28:11  marfav
 *  Using ACS_SSL for ssl global initialization
 *
 *  Revision 5.22  2014/04/07 08:14:00  marfav
 *  libSSH2 initialization managed outside threades (it is not thread safe)
 *
 *  Revision 5.21  2014/03/28 10:44:12  marfav
 *  Using RWMutex in CURL callback
 *  Using curl_global_init outside from threads before start any work
 *
 *  Revision 5.20  2014/03/26 15:52:38  marfav
 *  Using a CURL SHARE for ssl/ssh sessions
 *
 *  Revision 5.19  2014/03/12 11:10:53  francesco.avanzi
 *  minor issue taken in account
 *
 *  Revision 5.18  2014/02/26 10:36:50  lucio.pulvirenti
 *  Timeout values now correctly managed.
 *  Band shaping effectively used in ftp get and put methods only (no used in rename, ls, etc.).
 *
 *  Revision 5.17  2014/02/21 16:40:19  lucio.pulvirenti
 *  Progress callback no more used: to remove next version,
 *
 *  Revision 5.16  2014/02/07 18:47:33  marpas
 *  adopting ACS_THROW and ACS_COND_THRO macros
 *
 *  Revision 5.15  2013/12/17 18:09:22  marpas
 *  minor changes
 *
 *  Revision 5.14  2013/12/10 18:49:05  marpas
 *  qa warnings and code simplified
 *
 *  Revision 5.13  2013/12/10 18:30:07  marpas
 *  dump_curl_vers_info method implemented:
 *  once per run it dumps in the logfile the main version info for the actual curl, including (protocols and versions of major libs used)
 *
 *  Revision 5.12  2013/12/10 17:05:07  lucio.pulvirenti
 *  ssl_lock: lock on _M_mutex removed at beginning.
 *  No more sftp transfer crashes in conjunction with libssh2-1.4.2-1, libcurl-7.19.7-37, openssl-1.0.1e-15.
 *
 *  Revision 5.11  2013/12/10 13:44:26  lucio.pulvirenti
 *  MP: Work in progress.
 *
 *  Revision 5.10  2013/12/09 14:25:13  marpas
 *  triggering OPENSSL_VERSION_NUMBER is 0x010000000L for new OpenSSL thread safety.
 *  Before was conservatively set to 0x01000000fL
 *
 *  Revision 5.9  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.8  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.7  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.6  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.5  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/09/10 12:40:09  marfav
 *  Delete a remote file does not perform a download if file exists
 *
 *  Revision 5.3  2013/06/18 12:54:13  marpas
 *  debug macros rationalized
 *
 *  Revision 5.2  2013/06/14 10:54:15  marpas
 *  debug levels and messages improved
 *
 *  Revision 5.1  2013/06/12 15:21:58  marpas
 *  wrong macro LOG_WARNING, fixed
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.45  2013/04/27 14:46:27  marpas
 *  compilation warnings
 *
 *  Revision 2.44  2013/04/17 20:36:43  marpas
 *  robustness and debug improved
 *
 *  Revision 2.43  2013/04/16 16:12:28  marpas
 *  getDegaultConnectionPool call fixed
 *
 *  Revision 2.42  2013/04/15 09:12:34  marpas
 *  coding best practices applied
 *  interfaces rationalization and simplification
 *
 *  Revision 2.41  2013/04/03 15:39:23  marpas
 *  EntityBean and related class interface changed
 *
 *  Revision 2.40  2013/03/28 20:26:16  marpas
 *  trying to fix chmod issue
 *
 *  Revision 2.39  2013/02/28 13:30:31  marpas
 *  libMessage dependencies removed
 *
 *  Revision 2.38  2013/01/18 17:04:25  micmaz
 *  remove useless printouts
 *
 *  Revision 2.37  2013/01/15 11:24:26  micmaz
 *  work in progress
 *
 *  Revision 2.35  2012/12/20 16:12:47  lucio.pulvirenti
 *  openssl version management for thread id callbacks
 *
 *  Revision 2.34  2012/12/05 17:54:44  micmaz
 *  work in progress
 *
 *  Revision 2.33  2012/12/05 10:31:18  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 2.32  2012/12/03 16:56:48  marpas
 *  using new StringUtils interface
 *
 *  Revision 2.31  2012/11/28 18:16:23  marpas
 *  qa rules,
 *  optimization and robustness
 *  still work in progress
 *
 *  Revision 2.30  2012/11/27 13:27:51  micmaz
 *  corrected gcrypto call backs
 *
 *  Revision 2.29  2012/11/14 15:34:10  micmaz
 *  corrected test if app connect time is used.
 *
 *  Revision 2.28  2012/09/07 08:38:36  micmaz
 *  minor changes.
 *
 *  Revision 2.27  2012/09/06 15:55:23  micmaz
 *  added two new paramters: low speed time and low speed limit in order to stop a low transfer.
 *
 *  Revision 2.26  2012/06/25 12:54:10  lucio.pulvirenti
 *  removed check statistics if disabled and removed use of certificates for
 *  old cURL versions.
 *
 *  Revision 2.25  2012/06/19 16:51:14  micmaz
 *  libFtpCurl now can transfer files without password.
 *
 *  Revision 2.24  2012/06/19 13:27:31  micmaz
 *  libFtpCurl now can transfer files without password.
 *
 *  Revision 2.23  2012/06/08 12:57:09  lucio.pulvirenti
 *  MM & LP: chmod and lchmod now accept mode_t permissions in octal format
 *
 *  Revision 2.22  2012/06/07 15:08:15  micmaz
 *  IN PROGRESS - issue S1PDGS-3416: Gestione permessi dei files scaricati tramite ISMClient
 *  http://jira.acsys.it/browse/S1PDGS-3416
 *
 *  Revision 2.21  2012/06/06 18:02:07  micmaz
 *  IN PROGRESS - issue S1PDGS-3416: Gestione permessi dei files scaricati tramite ISMClient
 *  http://jira.acsys.it/browse/S1PDGS-3416
 *
 *  Revision 2.20  2012/06/06 13:47:50  micmaz
 *  IN PROGRESS - issue S1PDGS-3416: Gestione permessi dei files scaricati tramite ISMClient
 *  http://jira.acsys.it/browse/S1PDGS-3416
 *
 *  Revision 2.19  2012/05/30 17:36:28  micmaz
 *  added local and remote chmod
 *
 *  Revision 2.18  2012/03/30 12:51:00  micmaz
 *  work in progress
 *
 *  Revision 2.17  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.16  2012/03/08 13:11:40  micmaz
 *  added media functionality
 *
 *  Revision 2.15  2012/02/17 16:35:34  micmaz
 *  added compatibility with old curl versions.
 *
 *  Revision 2.12  2012/02/13 16:06:57  micmaz
 *  IN PROGRESS - issue S1PDGS-2902: The Circulation takes a core dump when trying to transfer a file without correct rights
 *  http://jira.acsys.it/browse/S1PDGS-2902
 *
 *  Revision 2.11  2012/02/10 17:41:12  micmaz
 *  now the ftps connection will not encrypt data by default.
 *
 *  Revision 2.10  2012/02/09 16:48:24  micmaz
 *  added ftps CCC  configuration parameter.
 *
 *  Revision 2.9  2012/02/08 16:18:10  micmaz
 *  added probe method in order to verify if the remote site (ftp/ftps) is passive/active.
 *
 *  Revision 2.8  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.7  2012/01/31 12:25:02  micmaz
 *  added a new delay method.
 *
 *  Revision 2.6  2012/01/23 14:29:45  micmaz
 *  build break.
 *
 *  Revision 2.5  2012/01/23 11:16:57  micmaz
 *  RESOLVED - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.4  2012/01/20 16:04:59  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.3  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.2  2011/10/25 15:38:36  marpas
 *  compilatin prolem fixed
 *
 *  Revision 2.1  2011/10/18 15:51:16  marpas
 *  FTPCURL_USE_GNUTLS or FTPCURL_USE_OPENSSL compilation flag to be defined
 *  FTPCURL_NO_ENCRYPT_DECRYPT to be set to remove encrypt/decrypt capabilities
 *  test compilation improved
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.42  2011/08/31 15:28:42  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.41  2011/08/31 15:11:09  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.40  2011/08/31 14:08:40  micmaz
 *  IN PROGRESS - issue S1PDGS-1785: ssl thread safety
 *  http://jira.acsys.it/browse/S1PDGS-1785
 *
 *  Revision 1.39  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.38  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.37  2011/06/24 16:28:41  micmaz
 *  work in progress
 *
 *  Revision 1.36  2011/05/12 17:22:13  micmaz
 *  work in progress
 *
 *  Revision 1.35  2011/05/11 15:47:53  micmaz
 *  rules modifications
 *
 *  Revision 1.34  2011/05/11 13:52:46  micmaz
 *  corrected the rollback for put and get operations
 *
 *  Revision 1.33  2011/05/10 17:10:57  micmaz
 *  corrected bug of ProgressData callback: destroyed while working.
 *
 *  Revision 1.32  2011/05/06 16:31:21  micmaz
 *  work in progress...
 *
 *  Revision 1.31  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.30  2011/05/03 14:01:34  micmaz
 *  corrected some circulation bugs and add CurlStatistics class
 *
 *  Revision 1.29  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.28  2011/04/22 16:47:11  micmaz
 *  work in progress...
 *
 *  Revision 1.27  2011/04/08 15:37:40  micmaz
 *  work in progress
 *
 *  Revision 1.26  2011/04/07 15:03:04  micmaz
 *  work in progress
 *
 *  Revision 1.25  2011/04/01 14:43:55  micmaz
 *  implemented the Circulations WP
 *
 *  Revision 1.24  2011/03/23 10:04:02  micmaz
 *  work in progress...
 *
 *  Revision 1.23  2011/03/15 10:20:02  micmaz
 *  work in progress
 *
 *  Revision 1.22  2011/03/11 17:55:46  micmaz
 *  reworked time outs.
 *
 *  Revision 1.21  2011/03/10 16:34:38  micmaz
 *  added new time out parameters. improved logs on failures.
 *
 *  Revision 1.20  2011/03/09 11:37:27  micmaz
 *  moved curle errors definition to the header file.
 *
 *  Revision 1.19  2011/03/08 10:44:15  marpas
 *  interface changed
 *
 *  Revision 1.18  2011/03/01 17:32:12  micmaz
 *  removed boost::thread for  added acs::Thread
 *
 *  Revision 1.17  2011/02/28 13:25:34  micmaz
 *  work in progress...
 *
 *  Revision 1.16  2011/02/23 14:39:25  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *  Revision 1.15  2011/02/10 16:28:30  micmaz
 *  added statistics.
 *
 *  Revision 1.14  2011/02/10 15:53:32  micmaz
 *  added statistics.
 *
 *  Revision 1.13  2011/02/08 17:44:53  micmaz
 *  work in progress...
 *
 *  Revision 1.12  2011/02/04 11:31:46  micmaz
 *  work in progress...
 *
 *  Revision 1.11  2011/02/02 17:33:32  micmaz
 *  work in  progress...
 *
 *  Revision 1.10  2011/02/02 10:16:06  micmaz
 *  corrected the list callback function.
 *
 *  Revision 1.9  2011/02/01 15:12:14  micmaz
 *  work in  progress...
 *
 *  Revision 1.8  2011/01/28 18:55:55  micmaz
 *  work in  progress...
 *
 *  Revision 1.7  2011/01/27 16:13:28  micmaz
 *  adapted the library to old libcURL 1.15
 *
 *  Revision 1.6  2011/01/27 10:35:57  marpas
 *  CURLE_REMOTE_FILE_NOT_FOUND was added after curl 7.15.5
 *
 *  Revision 1.5  2011/01/26 18:39:25  micmaz
 *  work in  progress...
 *
 *  Revision 1.4  2011/01/26 16:39:24  micmaz
 *  work in  progress...
 *
 *  Revision 1.3  2011/01/26 13:38:21  micmaz
 *  work in  progress...
 *
 *  Revision 1.2  2011/01/25 19:35:42  micmaz
 *  work in  progress...
 *
 *  Revision 1.1  2011/01/19 16:42:59  micmaz
 *  added the openConnection() and closeConnection() to cURL operations.
 *
 *
 *
 */

#include <Curl.h>
#include <CurlFile.h>
#include <CurlStatistics.h>
#include <CurlTransaction.h>
#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>
#include <Application.h>
#include <dbConnectionPool.h>
#include <ftarstream.h>

#include <Thread.h>
#include <StringUtils.h>
#include <File.h>
#include <Dir.h>
#include <Filterables.h>
#include <Singleton.hpp>

#include <CryptoEngineInitializer.h>
#include <InitCallbackManagerSingleton.h>


// This always uses libSSH2 to enable SSH support in libCurl
// the crypto engine can be switched between openSSL and gcrypt
// both of them are initialized (gcrypt is ifdeffed)
// because the libCurl can use OpenSSL in its internals and thread safety
// must be provided from the extern (namely here)

#if ( (LIBCURL_VERSION_MAJOR <= 7) && (LIBCURL_VERSION_MINOR < 21) )
#include <curl/types.h>
#endif

#include <curl/easy.h> // PRQA S 1013

#include <boost/scoped_ptr.hpp> // PRQA S 1013

#include <sys/types.h>          // PRQA S 1013
#include <sys/stat.h>           // PRQA S 1013


#define DO_NOT_USE_CURL_SHARES

#ifdef DO_NOT_USE_CURL_SHARES
#warning CURL SHARES are still DISABLED
#endif //DO_NOT_USE_CURL_SHARES

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

using std::string ; 

const string Curl::Confkey_verifycert_hostname("Hostname") ;	// Configured host
const string Curl::Confkey_verifycert_verifyhost("verify") ;	// Parameters: verification is active
const string Curl::Confkey_verifycert_cacert("cacert") ;		// Parameter: cacert (abs. path)
const string Curl::Confkey_verifycert_cert("cert") ;			// Parameter: cert (abs. path)
const string Curl::Confkey_verifycert_key("key") ;				// Parameters: key (abs. path)

namespace {

    using pattern::Singleton ;

    class CurlInitializer : public Singleton<CurlInitializer> // PRQA S 2109, 2153
    {
        friend CurlInitializer* Singleton<CurlInitializer>::instance(); // PRQA S 2107

    public:
	    ~CurlInitializer() noexcept override ;

        void checkSSL() ;
#ifndef DO_NOT_USE_CURL_SHARES
        CURLSH* getCURLShare () const { return curl_share_; } // PRQA S 4628
#endif //DO_NOT_USE_CURL_SHARES        

    protected:
	    CurlInitializer() ;
    private:

        void dump_curl_vers_info() const ;

        void curl_global_initialization();
        
#ifndef DO_NOT_USE_CURL_SHARES        
        void curl_share_initialization();
        void curl_share_housekeeping() noexcept ;
        
        static void curl_lock_function (CURL* handle, curl_lock_data data, curl_lock_access access, void* userptr);
        static void curl_unlock_function (CURL* handle, curl_lock_data data, void* userptr);
#endif //DO_NOT_USE_CURL_SHARES        

    private: // NOSONAR - for clarity
#ifndef DO_NOT_USE_CURL_SHARES
        vector<Mutex> curl_mutex_ ;
        CURLSH* curl_share_ = nullptr ;
#endif //DO_NOT_USE_CURL_SHARES        

	    ACS_CLASS_DECLARE_DEBUG_LEVEL(CurlInitializer) ;
    };

    ACS_CLASS_DEFINE_DEBUG_LEVEL(CurlInitializer)

#ifndef DO_NOT_USE_CURL_SHARES

    void CurlInitializer::curl_lock_function (CURL* handle, // PRQA S 4020
                                              curl_lock_data data, 
                                              curl_lock_access access, 
                                              void* userptr)
    {
	    ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB, "CURL lock function called thread id is " << pthread_self())

	    if (CURL_LOCK_ACCESS_SHARED == access)
	    {
     	    ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB, "CURL lock going to READ LOCK thread id is " << pthread_self()) ;
		    CurlInitializer::instance()->curl_mutex_[data].lock();
	        ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"CURL lock READ LOCK acquired thread id is " << pthread_self()) ;
	    }
	    else if (CURL_LOCK_ACCESS_SINGLE == access)
	    {
	        ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"CURL lock going to WRITE LOCK thread id is " << pthread_self()) ;
		    CurlInitializer::instance()->curl_mutex_[data].lock();
	        ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"CURL lock WRITE LOCK acquired thread id is " << pthread_self()) ;
	    }
	    else
	    {
		    ACS_LOG_WARNING ("Unexpected lock mode required by CURL library. Using Exclusive lock.");
		    CurlInitializer::instance()->curl_mutex_[data].lock();
	        ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"CURL lock WRITE LOCK acquired thread id is " << pthread_self()) ;
	    } 
    }

    void CurlInitializer::curl_unlock_function (CURL* handle, curl_lock_data data, void* userptr) // PRQA S 4020
    {
	    ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"CURL lock going to UNLOOCK thread id is " << pthread_self());
        while (CurlInitializer::instance()->curl_mutex_[data].unlock(true))
        {
    	    ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"One LOCK released by thread id is " << pthread_self()) ;
        }
	    ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"CURL lock LOCK released thread id is " << pthread_self()) ;
    }


    void CurlInitializer::curl_share_initialization()
    {

        curl_share_ = curl_share_init();
        ACS_COND_THROW ( !curl_share_ , Curl::CurlException("Unable to create CURL shared object")); // PRQA S 3081

        // define the callbacks here
        ACS_COND_THROW (CURLSHE_OK != curl_share_setopt (curl_share_, CURLSHOPT_LOCKFUNC, curl_lock_function), // PRQA S 3081
            Curl::CurlException("Unable to set CURLSHOPT_LOCKFUNC"));
        ACS_COND_THROW (CURLSHE_OK != curl_share_setopt (curl_share_, CURLSHOPT_UNLOCKFUNC, curl_unlock_function), // PRQA S 3081
            Curl::CurlException("Unable to set CURLSHOPT_UNLOCKFUNC"));

        // start the share
        ACS_COND_THROW (CURLSHE_OK != curl_share_setopt (curl_share_, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION), // PRQA S 3081
            Curl::CurlException("Unable to set CURLSHOPT_SHARE"));
        
    }

    void CurlInitializer::curl_share_housekeeping() noexcept 
    {
        if (curl_share_)
        {
            if (CURLSHE_OK != curl_share_setopt (curl_share_, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_SSL_SESSION)) {
                curl_share_cleanup (curl_share_);
                curl_share_ = nullptr ;
            }
            else {
                ACS_LOG_ERROR("Unable to set CURLSHOPT_UNSHARE") ;
            }
        }
    }
#endif //DO_NOT_USE_CURL_SHARES

    void CurlInitializer::curl_global_initialization() // NOSONAR - non const 
    {
      // any init on SSL has been performed by the CryptoEngineInitializer singleton and Base Soap library linking
      // Warning this could initialize the SSL library again
      // if some crash will occur on ssl_init call define the following in the makefile
#ifdef FTPCURL_DO_NOT_INIT_SSL
#warning SSL will not be initialized here because FTPCURL_DO_NOT_INIT_SSL is defined
        CURLcode res = curl_global_init(CURL_GLOBAL_NOTHING) ;
#else //FTPCURL_DO_NOT_INIT_SSL
        CURLcode res = curl_global_init(CURL_GLOBAL_ALL) ;
#endif //FTPCURL_DO_NOT_INIT_SSL
        ACS_COND_THROW(res != CURLE_OK, Curl::CurlException("Cannot initialize curl (curl_global_init)")); // PRQA S 3081
    }      
      
    CurlInitializer::CurlInitializer() : 
        Singleton<CurlInitializer>()
#ifdef DO_NOT_USE_CURL_SHARES
#else //DO_NOT_USE_CURL_SHARES
        ,
        curl_mutex_(CURL_LOCK_DATA_LAST)
#endif //DO_NOT_USE_CURL_SHARES        
    {
        utils::CryptoEngineInitializer::instance()->checkSSL();

        curl_global_initialization();
#ifndef DO_NOT_USE_CURL_SHARES        
        curl_share_initialization() ;
#endif //DO_NOT_USE_CURL_SHARES
        
        dump_curl_vers_info();
    }
    
	CurlInitializer::~CurlInitializer() noexcept // NOSONAR - keep in case of undefined DO_NOT_USE_CURL_SHARES 
    {
#ifndef DO_NOT_USE_CURL_SHARES    
        try {
            curl_share_housekeeping() ;
        }
        catch(const std::exception &x) {
            ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
        }
#endif //DO_NOT_USE_CURL_SHARES        
    }


    void CurlInitializer::checkSSL() // NOSONAR - non const !
    {
        utils::CryptoEngineInitializer::instance()->checkSSL();
    }

namespace {
    string cp_to_string(const char *s) { // avoid null version info problems: see dump_curl_vers_info below
        return s ? s : "<null>" ; // PRQA S 3383
    }
}

// see http://curl.haxx.se/libcurl/c/curl_version_info.html
void CurlInitializer::dump_curl_vers_info() const 
{
    const curl_version_info_data *data = curl_version_info( CURLVERSION_NOW );
    ACS_LOG_PRIVINFO("cURL version info") ;
    
    if (data->age >= CURLVERSION_FIRST) {
        ACS_LOG_PRIVINFO("cURL        version: " << cp_to_string(data->version)) ;
        ACS_LOG_PRIVINFO("cURL  configured on: " << cp_to_string(data->host)) ;
        ACS_LOG_PRIVINFO("cURL    ssl_version: " << cp_to_string(data->ssl_version)) ;
        ACS_LOG_PRIVINFO("cURL   libz_version: " << cp_to_string(data->libz_version)) ;
        std::ostringstream prot ;
        const char * const *p = data->protocols ;
        for (size_t i= 0 ; p[i]; ++i) {
            if (i != 0) { prot << ", " ; }
            prot << p[i] ;
        }
        ACS_LOG_PRIVINFO("cURL      protocols: " << prot.str()) ;
    }
#if defined(CURLVERSION_FOURTH) 
    if (data->age >= CURLVERSION_FOURTH) {
        ACS_LOG_PRIVINFO("cURL libssh_version: " << cp_to_string(data->libssh_version)) ;
    }
#endif
}

void doCurlInit ()
{
	CurlInitializer::instance();
}

class initCaller
{
	public:
		initCaller()
		{
			InitCallbackManagerSingleton::addInitCallback (&doCurlInit, "CURL Initializer", 4, true); // PRQA S 4400
		}
	
};

const initCaller myInitCaller;


} // unnamed namespace 

ACS_CLASS_DEFINE_DEBUG_LEVEL(Curl)

using std::max ;
using std::min ;


Curl::Curl() :
    _site(RemoteSite::ftp)
{
    CurlInitializer::instance()->checkSSL() ;
	checkFtpCommandResponseTimeout();
}

Curl::Curl(const RemoteSite &site) :
    _site(site)
{
    CurlInitializer::instance()->checkSSL() ;
	checkFtpCommandResponseTimeout();
}

Curl::~Curl() noexcept 
{
    try {
        closeConnection();
    }
    catch(const std::exception &x) { // NOSONAR - any exception 
        ACS_LOG_NOTIFY_EX(x) ;
    }
    
}

void Curl::clearSlist()
{
    if (slist_) { 
        curl_slist_free_all(slist_); 
        slist_ = nullptr ;
    }
}

void Curl::prepareCustomOptions(bool use_progress_callback) // PRQA S 4020, 4211
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "use progress callback: " << std::boolalpha << use_progress_callback);
	
    //enable progress callback call
    if (_progressCallback || !use_progress_callback) {
        curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, _progressCallback);
        if (_progressCallbackData) {
            curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, _progressCallbackData);
        }
    }
	
	if ( _site.getIsHttp() ) {
        clearSlist(); //If slist is not empty, clears it down
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Content type before if statement: "<< httpContentType_);
        if (httpContentType_ != "" ) {
            string baseName = "Content-Type: " + httpContentType_ ;
            slist_ = curl_slist_append(slist_, baseName.c_str()  ); 
		}
 	    size_t ch_size = _custhdrs.size() ;
	    for (size_t i=0; i < ch_size; ++i) {
            slist_ = curl_slist_append(slist_, _custhdrs[i].c_str());
	    }

        std::vector<std::string> customHeaders = _site.getCustomHeaders();
        for(auto const &ch : customHeaders) {
            slist_ = curl_slist_append(slist_, ch.c_str());
        }
        
		if (slist_)
		{
     	   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">>>>>Going to setOpt Http_header<<<<<");
			curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, slist_);
        }	// END: if (httpContentType_ != ""   ) 
        return;

    }

    if (_createMissingDirs) {
#if ( (LIBCURL_VERSION_MAJOR > 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 19)) )
        curl_easy_setopt(_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR_RETRY);
#else
#warning libcurl does not allow parallel creation of remote directories, please upgrade it to a version >= 7.20
        curl_easy_setopt(_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

#endif
    }
    
}

void Curl::setProxy() // PRQA S 4020
{
    // When searching for the proxy configuration there are three possible cases
    // case 1: the host is not configured at all
    // case 2: the host is configured and set to not use any proxy
    // case 3: the host is configured and set to use a specific proxy configuration

    Proxy proxy;
    bool proxyFound = conf::CurlConfigurerSingleton::instance()->findProxy(_site.getHostAndPort(), proxy);
    if (! proxyFound) {
        // case 1: host not configured. libCurl will use its internal logic
        // https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html
        // ibcurl respects the proxy environment variables named http_proxy, ftp_proxy, sftp_proxy etc. 
        // If set, libcurl will use the specified proxy for that URL scheme. So for a "FTP://" URL, the ftp_proxy is considered. 
        // all_proxy is used if no protocol specific proxy was set.
        // WARNING: the behaviour will depend on env variable !!
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "ignoring proxy setting for host: " << _site.getHostAndPort());
        return ;
    }

    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "proxy: " << proxy << " for host: " << _site.getHostAndPort());

#if ( (LIBCURL_VERSION_MAJOR > 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 19)) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 19) && (LIBCURL_VERSION_PATCH > 4)))
    
    if (proxy == no_proxy()) {
      // case 2: for that host there will be no proxy at all
      // CURLOPT_NOPROXY will override any env variable set and force libcurl
      // to not use any proxy for the specific host _site.getHostAndPort()
      curl_easy_setopt(_curl, CURLOPT_NOPROXY, _site.getHostAndPort().c_str());
      return;
    }
    
#warning "Set of CURLOPT_PROXYUSERPWD CURLOPT_PROXYUSERNAME CURLOPT_PROXYPASSWORD still to test"
    // case 3: set a configured PROXY for the host _site.getHostAndPort()
    if (! proxy.user().empty()) {
        if (! proxy.password().empty()) {
            tmp_proxyauth_ = proxy.user() + ":" + proxy.password() ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Going to set CURLOPT_PROXYUSERPWD option: \"" << tmp_proxyauth_ << '\"');
            curl_easy_setopt(_curl, CURLOPT_PROXYUSERPWD, tmp_proxyauth_.c_str());
        }
        else {
            tmp_proxyauth_ = proxy.user() ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Going to set CURLOPT_PROXYUSERNAME option: \"" << tmp_proxyauth_ << '\"');
            curl_easy_setopt(_curl, CURLOPT_PROXYUSERNAME, tmp_proxyauth_.c_str());
        }
    }
    else {
        tmp_proxyauth_ = proxy.password() ;
        if (! proxy.password().empty()) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Going to set CURLOPT_PROXYPASSWORD option: \"" << tmp_proxyauth_ << '\"');
            curl_easy_setopt(_curl, CURLOPT_PROXYPASSWORD, tmp_proxyauth_.c_str());
        }
    }
    
    std::ostringstream os ;
    if (! proxy.protocol().empty()) {
        os << proxy.protocol() << "://" ;
        if (StringUtils::equalsNoCase(proxy.protocol(), "http")) {
            curl_easy_setopt(_curl, CURLOPT_HTTPPROXYTUNNEL, 1);
        }

    }
    else { // http 
        curl_easy_setopt(_curl, CURLOPT_HTTPPROXYTUNNEL, 1);
    }
    
    os << proxy.host() ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Using proxy: " << proxy.host() << " to connect with: " << _site.getHostAndPort());

    if (proxy.port() > 0) { // PRQA S 3000
        os << ':' << proxy.port() ;
    }
    
    tmp_proxy_ = os.str() ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Going to set CURLOPT_PROXY option: \"" << tmp_proxy_ << '\"');
    curl_easy_setopt(_curl, CURLOPT_PROXY, tmp_proxy_.c_str());
#else
#warning Proxy settings not supported on this version of Curl
    ACS_THROW(exNotImplemented(string("CURL proxy support not implemented on this version: ") + LIBCURL_VERSION));
#endif    
}

CURL* Curl::openConnection() // NOSONAR - external type
{
    ACS_COND_THROW(0 != _curl, CurlException("Curl already initialized")) ; // PRQA S 3081

    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "going to call curl_easy_init");
    _curl = curl_easy_init(); // NOSONAR - SSL & TLS are used according to configuration and context
    ACS_COND_THROW(0 == _curl, CurlException("Cannot initialize curl")); // PRQA S 3081
	
	try { 
    	_errorMessage.fill(0) ;

    	curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _errorMessage.data());
    	curl_easy_setopt(_curl, CURLOPT_STDERR, 0);

      if ((true == _site.getIsHttp()) || (true == _site.getIsHttps()))
      {
        // Enable the http(s) redirect
        curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(_curl, CURLOPT_UNRESTRICTED_AUTH, 1);
        curl_easy_setopt(_curl, CURLOPT_MAXREDIRS, 20);
        curl_easy_setopt(_curl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_ALL);
		}

    	/* Switch on full protocol/debug output */
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB);
    	 ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "going to set CURLOPT_DEBUGFUNCTION, CURLOPT_DEBUGDATA, CURLOPT_VERBOSE");
        	curl_easy_setopt(_curl, CURLOPT_DEBUGFUNCTION, debugCallback);
        	curl_easy_setopt(_curl, CURLOPT_DEBUGDATA, this);
        	curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1);
		ACS_CLASS_END_DEBUG;

		// Authentication section if requested
		#if ( (LIBCURL_VERSION_MAJOR > 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 19)) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 19) && (LIBCURL_VERSION_PATCH > 0)))
		if (!StringUtils::isBlank(_site.getUser()))
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "user name: " << _site.getUser() );
			curl_easy_setopt( _curl, CURLOPT_USERNAME, _site.getUser().c_str());
			_userPassword = _site.getUser();
			if (!StringUtils::isBlank(_site.getPassword()))
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "password: " << _site.getPassword() );
				curl_easy_setopt( _curl, CURLOPT_PASSWORD, _site.getPassword().c_str());
				_userPassword = _userPassword + ":" + _site.getPassword();
			}
		}
		#else
		if (!StringUtils::isBlank(_site.getPassword()) && !StringUtils::isBlank(_site.getUser())) 
		{
			_userPassword = _site.getUser() + ":" + _site.getPassword();
			curl_easy_setopt(_curl, CURLOPT_USERPWD, _userPassword.c_str());
		} 
		else if (!StringUtils::isBlank(_site.getUser())) {
			ACS_LOG_WARNING("Warning: the password is blank and you are using an old cURL version that does not support it.");
		}
		#endif
		
	    /* here set the proxy if needed 
	    */
    	setProxy() ;
    	{ // NOSONAR - scope
        	string cipher = _site.getCipher() ;
        	if(!cipher.empty() )
        	{
            	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Using Cipher: " << cipher.c_str() ); 
            	curl_easy_setopt(_curl, CURLOPT_SSL_CIPHER_LIST, cipher.c_str());

        	}
    	}
		
		
		size_t oldTimeout = _connectionTimeout_ms;
		if (_site.getIsHttp()) {
			oldTimeout = setConnectionTimeout_ms(conf::CurlConfigurerSingleton::instance()->getHttpConnectionTimeout_ms());
		}
		
		// Set timeouts
		#ifdef CURLOPT_TIMEOUT_MS
			curl_easy_setopt(_curl, CURLOPT_TIMEOUT_MS, _connectionTimeout_ms);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl whole transfer time out [ms] " << _connectionTimeout_ms);
		#else
			curl_easy_setopt(_curl, CURLOPT_TIMEOUT, _connectionTimeout_s);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl whole transfer time out [s]" << _connectionTimeout_s);
		#endif

		#ifdef CURLOPT_CONNECTTIMEOUT_MS
			curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT_MS, _response_timeout_ms);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl response time out [ms] " << _response_timeout_ms);
		#else
			curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _response_timeout_s);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl response time out [s]" << _response_timeout_s);
		#endif
		
		// Reset original timeout
		setConnectionTimeout_ms(oldTimeout);
		
    	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl connection Type: " << _site.getProtocol());
    	if ( _site.getIsHttp() ) {
        	curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1); // MP http://horstr.blogspot.it/2008/04/on-libcurl-openssl-and-thread-safety.html
			
			if (_httpFailOnError) {
				// request failure on HTTP response >= 400, by default it is disabled in libcurl
				curl_easy_setopt(_curl, CURLOPT_FAILONERROR, 1);
			}

      if (_site.getIsHttps())
      {
				if (false == _site.getVerifyCertificate())
				{ 
					ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Skipping certificate verification");
					curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0);
					curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0);
				}
				else
				{
					string param_value ; CURLcode res ;
					ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Setting certificate verification");
					curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 1);
					curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 2);

					if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_cacert, param_value)) && (! param_value.empty()) ) 
					{
						res = curl_easy_setopt(_curl, CURLOPT_CAINFO, param_value.c_str());
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                              << Confkey_verifycert_cacert << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
          }

          if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_cert, param_value)) && (! param_value.empty()) ) 
					{
						res = curl_easy_setopt(_curl, CURLOPT_SSLCERT, param_value.c_str());
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                              << Confkey_verifycert_cert << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
          }

          if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_key, param_value)) && (! param_value.empty()) ) 
					{
						res = curl_easy_setopt(_curl, CURLOPT_SSLKEY, param_value.c_str());
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                              << Confkey_verifycert_key << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
          }
        }
      }  // END: if( _site.getIsHttps()

          return _curl;
    	}

#ifndef DO_NOT_USE_CURL_SHARES
    	// Using CURL shared SSL session IDs
    	curl_easy_setopt(_curl, CURLOPT_SHARE, CurlInitializer::instance()->getCURLShare());
#endif //DO_NOT_USE_CURL_SHARES    

    	if (_site.getIsFtpes() || _site.getIsFtps()) {
        	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "using ssl...");
        	//        curl_easy_setopt(_curl, CURLOPT_FTPSSLAUTH, ftpesAuthenticationType.c_str());

        	curl_easy_setopt(_curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
          ACS_LOG_INFO("FTPS/FTPES Host: " << _site.getHostAndPort() << " Certificate verification: "
                                           << (_site.getVerifyCertificate() ? "T" : "F"));
          if (false == _site.getVerifyCertificate())
          {
        		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Skipping certificate verification");
        		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0);
        		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0);
			}
			else
			{
        		string param_value ; CURLcode res ;
        		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Setting Certificate verification");
        		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 1);
        		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 2);

				if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_cacert, param_value)) && (! param_value.empty()) )
				{
					res = curl_easy_setopt(_curl, CURLOPT_CAINFO, param_value.c_str());
          ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                            << Confkey_verifycert_cacert << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
        }

				if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_cert, param_value)) && (! param_value.empty()) ) 
				{
					res = curl_easy_setopt(_curl, CURLOPT_SSLCERT, param_value.c_str());
          ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                            << Confkey_verifycert_cert << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
        }

				if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_key, param_value)) && (! param_value.empty()) ) 
				{
					res = curl_easy_setopt(_curl, CURLOPT_SSLKEY, param_value.c_str());
          ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                            << Confkey_verifycert_key << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
        }

			}
	#ifdef ACS_CURL_CCC_SUPPORTED // in CurlConfigurer.h
        	curl_easy_setopt(_curl, CURLOPT_FTP_SSL_CCC, conf::CurlConfigurerSingleton::instance()->getFtpsCCC());
	#endif
        	//When using ftpes connections, it must turn off the ssl/tls layer after
        	//user/password verification, in this manner the data will be sent unencrypted.
        	//PROT C is for: PROTection Clear, viceversa PROT P is for: PROTection Private
        	if ((_site.getIsFtpes() || _site.getIsFtps()) && !conf::CurlConfigurerSingleton::instance()->getIsFtpsProtectData()) { // PRQA S 3230
            	curl_easy_setopt(_curl, CURLOPT_USE_SSL, CURLUSESSL_CONTROL);
        	}
        	//        curl_easy_setopt(_curl, CURLOPT_CAINFO, "/data/pki/tls/certs/weblab.pem");
        	//      curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");
    	} 
    	else {
        if (!_site.getIsSFTP())
        {
          ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl connection is passive: " << std::boolalpha << _site.getIsPassive());
        }
          if (StringUtils::equalsNoCase(_site.getProtocol(), RemoteSite::media)) {
            	ACS_LOG_ERROR("Curl connection is Media! Treat is as FS not as FTP!!!");
        	}
          if (!_site.getIsSFTP() && !_site.getIsPassive())
          {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting CURLOPT_FTPPORT as: \"" << _ftpDefaultPort << "\"");
            curl_easy_setopt(_curl, CURLOPT_FTPPORT, _ftpDefaultPort.c_str()); /* system default port */
		        // Enable the EPRT support
            	curl_easy_setopt(_curl, CURLOPT_FTP_USE_EPRT, 1L);
          }
      }

        curl_easy_setopt(_curl, CURLOPT_FTP_RESPONSE_TIMEOUT, _ftp_command_response_timeout_s);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl ftp command response time out [s] " << _ftp_command_response_timeout_s);

		// SSH Certificate management in case of secure connections
    if (_site.getIsSFTP())
    {
      ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "ssh public key: " << conf::CurlConfigurerSingleton::instance()->getSshPublicKey());
      ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "ssh private key: " << conf::CurlConfigurerSingleton::instance()->getSshPrivateKey());
      curl_easy_setopt(_curl, CURLOPT_SSH_PUBLIC_KEYFILE, conf::CurlConfigurerSingleton::instance()->getSshPublicKey().c_str());
      curl_easy_setopt(_curl, CURLOPT_SSH_PRIVATE_KEYFILE, conf::CurlConfigurerSingleton::instance()->getSshPrivateKey().c_str());
        }

		
    	size_t low_speed_limit = conf::CurlConfigurerSingleton::instance()->getLowSpeedLimit() ;
    	size_t low_speed_time = conf::CurlConfigurerSingleton::instance()->getLowSpeedTime() ;
    	if ((low_speed_limit > 0) && (low_speed_time > 0)) { // PRQA S 3230
			curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, low_speed_limit);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl CURLOPT_LOW_SPEED_LIMIT (bytes/sec.) " << low_speed_limit);
			curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, low_speed_time);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl CURLOPT_LOW_SPEED_TIME (sec.) " << low_speed_time);
    	}
    	else{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl CURLOPT_LOW_SPEED_LIMIT not set ");
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl CURLOPT_LOW_SPEED_TIME not set  ");
    	}

      // TCP KeepAlive section
      // Can be effectively used if the kernel allows the TCP KeepAlive in the TCP Stack settings
      // If no firewall/NAT are in the middle of the transfer it is not actually needed
      // but it does not harm to set it in any case
      // This function has been added in libcurl 7.25.00
#if ((LIBCURL_VERSION_MAJOR > 7) || ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR >= 25)))
      CURLcode res = curl_easy_setopt(_curl, CURLOPT_TCP_KEEPALIVE, 1L);
      // check to ensure that this is supported
      if (res == CURLE_OK)
      {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "TCP Keepalive active");
        // wait for at least 120 seconds before sending keep-alive probes every 60 seconds
        curl_easy_setopt(_curl, CURLOPT_TCP_KEEPIDLE, 120L);
        curl_easy_setopt(_curl, CURLOPT_TCP_KEEPINTVL, 60L);
      }
      else
      {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Unable to set the CURLOPT_TCP_KEEPALIVE (" << curl_easy_strerror(res) << "): " << _errorMessage.data());
      }
#else
#warning KEEPALIVE TCP will not be sent (available from libcurl 7.25.0)
#endif

      curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1);  // MP http://horstr.blogspot.it/2008/04/on-libcurl-openssl-and-thread-safety.html
      return _curl;
  }
  catch( const std::exception & ex )
	{
		if( dynamic_cast<const RemoteSiteUtility::CryptException*> (&ex) ) { // PRQA S 3081
			ACS_THROW( CurlUnrecoverableException(ex, "Error got") );
		}
		throw;
	}

}

void Curl::closeConnection() {
    if (_curl) {
        _isCleaning = true;
        deregisterProgressCallback();
        _ptrToHeaders.clear();
        curl_easy_cleanup(_curl);
        _curl = nullptr ;
        _isCleaning = false;
    }
}

void Curl::rollback() {
    _transaction->rollback();
    _ptrToHeaders.clear();
}

bool Curl::isOpenedTransaction() const {
    return _transaction != nullptr ;
}

CURL* Curl::getCurlHandle()
{  // NOSONAR - external type (CURL)
  return _curl;
}

void Curl::ldel(const string &localFile) { // NOSONAR - non const !
    if (File::exists(localFile) && !File::readOnly(localFile) && File::isRegularFile(localFile)) {
        File::unlink(localFile);
    }
}

void Curl::setHTTPOpts(const CurlHttpOpt& opt)
{
	curl_easy_setopt(_curl, CURLOPT_URL, opt.getUri().c_str());

	// Set timeouts
	bool timeoutSet = false;
	size_t connectionTimeout_ms;
	size_t connectionTimeout_s;
	size_t responseTimeout_ms;
	size_t responseTimeout_s;
	opt.getTimeouts(connectionTimeout_ms, connectionTimeout_s, responseTimeout_ms, responseTimeout_s, &timeoutSet);
	if (timeoutSet)
	{
#ifdef CURLOPT_TIMEOUT_MS
		curl_easy_setopt(_curl, CURLOPT_TIMEOUT_MS, connectionTimeout_ms);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl whole transfer time out overwritten [ms] " << connectionTimeout_ms);
#else
		curl_easy_setopt(_curl, CURLOPT_TIMEOUT, connectionTimeout_s);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl whole transfer time out overwritten [s]" << connectionTimeout_s);
#endif

#ifdef CURLOPT_CONNECTTIMEOUT_MS
		curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT_MS, responseTimeout_ms);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl response time out overwritten [ms] " << responseTimeout_ms);
#else
		curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, responseTimeout_s);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Curl response time out overwritten [s]" << responseTimeout_s);
#endif
	}
	
	bool userPasswdSet = false;
	bool userSet = false;
	bool passwdSet = false;
	std::string userpasswd = opt.getUserPassword(&userPasswdSet);
	std::string user = opt.getUser(&userSet);
	std::string passwd = opt.getPassword(&passwdSet);
	
	// User-password on a unique string is mutually exclusive with separate username and passwords
	// and implies no password encoding
	if (userPasswdSet && !userpasswd.empty()) {
		curl_easy_setopt(_curl, CURLOPT_USERPWD, userpasswd.c_str());
	}
	else {
		// Username provided
		if (userSet && !user.empty()) {
			// Username is automatically encoded
			curl_easy_setopt(_curl, CURLOPT_USERNAME, user.c_str());
		}
		
		// Password provided
		if (passwdSet && !passwd.empty()) {
			// Password is automatically encoded
			curl_easy_setopt(_curl, CURLOPT_PASSWORD, passwd.c_str());
		}
	}
	
	// Requested secure connection
	if (opt.getSecureConnection()) 
	{
		if ( opt.getStrictAuthentication() || _site.getVerifyCertificate() )
		{
			string param_value ; CURLcode res ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Setting Certificate verification");
			curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 1);
			curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 2);

			if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_cacert, param_value)) && (! param_value.empty()) ) 
			{
				res = curl_easy_setopt(_curl, CURLOPT_CAINFO, param_value.c_str());
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                          << Confkey_verifycert_cacert << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
      }

			if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_cert, param_value)) && (! param_value.empty()) ) 
			{
				res = curl_easy_setopt(_curl, CURLOPT_SSLCERT, param_value.c_str());
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                          << Confkey_verifycert_cert << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
      }

			if ( (_site.getVerifyCertificateParameter(Confkey_verifycert_key, param_value)) && (! param_value.empty()) ) 
			{
				res = curl_easy_setopt(_curl, CURLOPT_SSLKEY, param_value.c_str());
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Site: \"" << _site.getHostAndPort() << "\" setting certificate verification parameter: "
                                                          << Confkey_verifycert_key << " to: \"" << param_value << "\"  with success: " << ((res == CURLE_OK) ? "T" : "F"));
      }

		}
		else
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Skipping Certificate verification");
			curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0);
		}
	}	// END: if (opt.getSecureConnection())
}

CURLcode Curl::post(const CurlHttpOpt& opt, std::string& output) {
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "HTTP POST \nURI " << opt.getUri() << "\ndata: \n" << opt.getMessage());
	CURLcode res = CURLE_OK;
	setHttpContentType(opt.getContentType());
	std::ostringstream oss;
	
	BEGIN_TRANSACTION(TrHTTPPOST, false) // PRQA S 4236
	{
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &oss);
		curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, opt.getMessage().c_str());

		setHTTPOpts(opt);
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "All options set: go");
		res = curl_easy_perform(_curl);
		curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code);
		logError(res, "");
	}
	END_TRANSACTION

	output  = oss.str();
	return res;
}

CURLcode Curl::post(string const& uri, string const& data, string & output, const string &contentType) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
	CurlHttpOpt opt;
	opt.setUri(uri);
	opt.setMessage(data);
	opt.setContentType(contentType);
	return post(opt, output);
}

CURLcode Curl::httppost(string const& uri, string const& data, string & output, const string &contentType, const string &userpwd) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
	CurlHttpOpt opt;
	opt.setUri(uri);
	opt.setMessage(data);
	opt.setContentType(contentType);
	opt.setUserPassword(userpwd);
	return post(opt, output);
}


CURLcode Curl::httpget(const CurlHttpOpt& opt, std::string& output) {
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "HTTP GET \nURI " << opt.getUri() );
	CURLcode res = CURLE_OK;
	setHttpContentType(opt.getContentType());
	std::ostringstream oss;
		
	BEGIN_TRANSACTION(TrHTTPPOST, false) // PRQA S 4236
	{
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &oss);
		curl_easy_setopt(_curl, CURLOPT_HTTPGET, 1L);
		
		setHTTPOpts(opt);
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "All options set: go");
		res = curl_easy_perform(_curl);
		curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code);
		logError(res, "");
	}
	END_TRANSACTION

	output  = oss.str();
	return res;
}

CURLcode Curl::httpget (string const& uri, string& output, const string &userpwd) {
	//put the local file to the remote _site using the remoteFile name.
	CurlHttpOpt opt;
	opt.setUri(uri);
	opt.setUserPassword(userpwd);
	return httpget(opt, output);
}

CURLcode Curl::httpput(const CurlHttpOpt& opt, std::string& output) {
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "HTTP PUT \nURI " << opt.getUri() );
	CURLcode res = CURLE_OK;
	setHttpContentType(opt.getContentType());
	std::ostringstream oss;
	
	BEGIN_TRANSACTION(TrHTTPPOST, false) // PRQA S 4236
	{
		FILE* fid = nullptr;
		
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &oss);
		
		setHTTPOpts(opt);

		bool uploadSet = false;
		std::string fileToUpload = opt.getFileToUpload(&uploadSet);
		if (uploadSet) {	// Upload a file 
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE    
			curl_easy_setopt(_curl, CURLOPT_UPLOAD_BUFFERSIZE, conf::CurlConfigurerSingleton::instance()->getUploadBufferSize());
#endif

			struct stat finfo;
			if (0 != stat(fileToUpload.c_str(), &finfo)) {
				ACS_THROW(exIOException("Cannot open '" + fileToUpload + "'", errno)) ; 
			}
			
			fid = fopen(fileToUpload.c_str(), "rb");
			ACS_COND_THROW(!fid, exIOException("Cannot open '" + fileToUpload + "'", errno)) ; 

			curl_easy_setopt(_curl, CURLOPT_INFILESIZE_LARGE, curl_off_t(finfo.st_size)); // PRQA S 3081
			curl_easy_setopt(_curl, CURLOPT_READFUNCTION, _putCallback);
			curl_easy_setopt(_curl, CURLOPT_READDATA, fid);
			curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1);
		}
		else {	// This allow to use put with a string passed as message without uploading any file
			curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, opt.getMessage().c_str()); // Used to specify the payload as if the request is a post
			curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "PUT"); // Specify the request type
		}
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "All options set: go");
		res = curl_easy_perform(_curl);
		curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code);
		logError(res, "");
		
		if (fid) {
			fclose(fid);
		}
	}
	END_TRANSACTION

	output  = oss.str();
	return res;
}


CURLcode Curl::httppatch(const CurlHttpOpt& opt, std::string& output) {
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "HTTP PATCH \nURI " << opt.getUri() );
	CURLcode res = CURLE_OK;
	setHttpContentType(opt.getContentType());
	std::ostringstream oss;
		
	BEGIN_TRANSACTION(TrHTTPPOST, false) // PRQA S 4236
	{
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &oss);
		curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, opt.getMessage().c_str()); // Used to specify the payload as if the request is a post
		curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "PATCH"); // Change the request type from post to patch
		
		setHTTPOpts(opt);
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "All options set: go");
		res = curl_easy_perform(_curl);
		curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code);
		logError(res, "");
		
		curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, 0); // Reset the internal state

	}
	END_TRANSACTION

	output  = oss.str();
	return res;
}

CURLcode Curl::httpdel(const CurlHttpOpt& opt, std::string& output) {
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "HTTP DELETE \nURI " << opt.getUri() );
	CURLcode res = CURLE_OK;
	setHttpContentType(opt.getContentType());
	std::ostringstream oss;
		
	BEGIN_TRANSACTION(TrHTTPPOST, false) // PRQA S 4236
	{
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &oss);
		curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, opt.getMessage().c_str()); // Used to specify the payload as if the request is a post
		curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "DELETE"); // Change the request type from post to patch
		
		setHTTPOpts(opt);
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "All options set: go");
		res = curl_easy_perform(_curl);
		curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code);
		logError(res, "");
	
		curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, 0); // Reset the internal state
	}
	END_TRANSACTION

	output  = oss.str();
	return res;
}



CURLcode Curl::httpPropFind (string const& path, int depth, string & output, const string & requestBody, const std::string & userpwd)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH( ACS_MID_VERB );
	
	ACS_COND_THROW( ! _site.getIsHttp(), CurlUnrecoverableException("Protocol is \""+_site.getProtocol()+"\": http(s) expected") );
	
	if( depth > 1 ) {
		ACS_COND_THROW(depth > 1, CurlUnrecoverableException() << "Depth request set to " << depth << ": expected values are: 0, 1, or negative values (infinity)" );
	}

  RemoteSite newSite(_site);
  newSite.setPath(StringUtils::trimBegin(path, '/'));
  string uri = RemoteSiteFactory::toURL(newSite, URLComponents::NoAuth);
  ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "HTTP PROPFIND \nURI \"" << uri << "\"\ndepth: " << depth << "\nrequestBody: \"" << requestBody << '\"');

  setHttpContentType("text/xml");

  string depthString;
  if (depth < 0)
  {
    depthString = "infinity";
  }
  else
  {
    depthString = std::to_string(depth);
  }
  addCustomHeader("Depth", depthString);

  CURLcode res=CURLE_OK;
	std::ostringstream oss;

	BEGIN_TRANSACTION(TrHTTPPROPFIND, false) // PRQA S 4236
		
		if (! userpwd.empty()) {
			curl_easy_setopt(_curl, CURLOPT_USERPWD, userpwd.c_str());
		}
		
		if( ! requestBody.empty() ) {
			curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, requestBody.c_str());
		}
		
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &oss);
		curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "PROPFIND");
		curl_easy_setopt(_curl, CURLOPT_URL, uri.c_str());

		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "All options set: go");
		res = curl_easy_perform(_curl);
		curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code);
		logError(res, "");
	END_TRANSACTION
	
	resetCustomHeaders();
	
	output  = oss.str();
	return res;


}


CURLcode Curl::put(const string &localFile, const string &remoteFile, mode_t permissions) {
   
   	void* default_data_pointer = nullptr ; 

    return putWithCallback(localFile, remoteFile, permissions, default_data_pointer, _putCallback) ;
}


CURLcode Curl::putWithCallback(const string &localFile, 
                               const string &remoteFile, 
                               mode_t permissions, 
                               void* data_pointer,           // NOSONAR - callback
                               PutCallback put_func_pointer) // NOSONAR - callback
{
    //put the local file to the remote _site using the remoteFile name.
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Put file " << localFile << " into " << remoteFile);
    CurlStatistics::log(_site, "put");
    ACS_COND_THROW(! File::exists(localFile), CurlException("Put error: local entry" + localFile + ": does not exist.")) ; // PRQA S 3081 2
    ACS_COND_THROW(! File::isReadable(localFile), CurlException("Put error: local entry" + localFile + ": it is not readable.")) ;
    ACS_COND_THROW( (File::isDir(localFile) && (! File::isExecutable(localFile)) ), CurlException("Put error: local directory" + localFile + ": it is not accessible.")) ;
    CURLcode res = CURLE_OK;
    BEGIN_TRANSACTION(TrPut, true) // PRQA S 4236

#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE    
        curl_easy_setopt(_curl, CURLOPT_UPLOAD_BUFFERSIZE, conf::CurlConfigurerSingleton::instance()->getUploadBufferSize());
#endif
        curl_easy_setopt(_curl, CURLOPT_READFUNCTION, put_func_pointer);
        curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1);
        res = performTransfer(false, localFile, remoteFile, permissions, data_pointer);
        LOG_TRANSACTION(TrPut, remoteFile, res);
        logError(res, remoteFile);
    END_TRANSACTION
    updateStatistics(res);
    CurlStatistics::logResult(res, _site, "put");
    return res;
}

CURLcode Curl::lrename(const string &oldFilename, const string &newFilename) 
{
    //rename the local file logging the operation in the transaction .
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[lrenameFrom: " << oldFilename << " To: " << newFilename << "]");
    CURLcode res = CURLE_WRITE_ERROR;
    if (acs::File::exists(newFilename) || !acs::File::exists(oldFilename)) {
        if (!acs::File::exists(oldFilename)) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "the local file " << oldFilename << "  does not exit: cannot rename it. ");
        }
        if (acs::File::exists(newFilename)) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "the local file " << newFilename << "  exits: cannot overwrite it. ");
        }
    } 
    else {
        BEGIN_TRANSACTION(TrLocalRename, false) // PRQA S 4236
            File::rename(oldFilename, newFilename);
            res = CURLE_OK;
            LOG_RENAME_TRANSACTION(TrLocalRename, newFilename, oldFilename, res);
        END_TRANSACTION;
    }
    return res;

}

CURLcode Curl::get(const string &localFile, const string &remoteFile) // PRQA S 4020
{
    //get the /remote/file/name and write to the /local/file/name.
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file " << remoteFile << " into " << localFile);
    if (acs::File::exists(localFile)) {
        ACS_LOG_ERROR("The local file/directory " << localFile << " already exists: cannot overwrite it. ");
        return CURLE_WRITE_ERROR;
    }

    CurlFile targetFile(localFile) ;
	try {
        targetFile.fopen();
	}
	catch( const std::exception &e )
	{
		ACS_THROW( CurlUnrecoverableException(e, "Error got") );
	}

	return getWithCallback(localFile, remoteFile, targetFile, _getCallback) ; 
}


CURLcode Curl::getWithCallback(const string &localFile, 
                               const string &remoteFile, 
                               CurlFile& dataHandler, 
                               GetCallback get_func_pointer) // NOSONAR - callback
{
    //get the /remote/file/name and write to the /local/file/name.
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file " << remoteFile << " into " << localFile);
    CurlStatistics::log(_site, "get");

    CURLcode res=CURLE_OK;
	
    BEGIN_TRANSACTION(TrGet, true) // PRQA S 4236
	
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, get_func_pointer);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &dataHandler);
		
		// Note that, below, the data handler ptr. is always set to ZERO. The reason is that CURLOPT_WRITEDATA has been already done here, and the
		// zero pointer PREVENTS another CURLOPT_WRITEDATA set by method: CurlHeaderMaker::makeGet
		const mode_t default_permissions = 0 ; 
		void* data_handler_forced_zero = nullptr ; 
       	res = performTransfer(true, remoteFile, localFile, default_permissions, data_handler_forced_zero);
  
  		// When here, transferring process finished, UNLESS UNFLASHED DATA BUFFER STILL NOT TRANSMITTED.
  
		if (CURLE_OK == res)
		{
  			// Check the status of the process (call "flush" method, which flush and send EOF to the process' input stream is called. This will terminate the process, unless stuck).
			// Note that, before the "flush", the process might be starving, or have received garbage data and it's still on hold waiting for new data.
			// Therefore the "OK" status, so far, it's meaningless untill termination.
			
			int exit_code = 0 ;
            int exit_signal = 0 ; 
			const bool process_success( dataHandler.flush(exit_code, exit_signal) ) ;
			
			if (false == process_success)
			{
				// Process in call-back returned an error.
				// To report it, forge a curl-like error (numeric code + message), it will be prompted by the caller:
				std::ostringstream o ; o << "Error running the callback method during get. Exit code: " << exit_code << "  exit signal: " << exit_signal ;
				strncpy(_errorMessage.data(), o.str().c_str(), _errorMessage.size()-1) ;  // NOSONAR - safe
                _errorMessage[_errorMessage.size()-1] = 0 ; // safety
				res = CURLE_CHUNK_FAILED ;
			}
		}
		else
		{
			// Curl is already reporting an error. Just flush and terminate (if still running) the process in the callback.
			dataHandler.flush() ;
		}
		
		// WHEN HERE: res contain the result of the curl transfer (finished) and callback processing (finished)
		
        LOG_TRANSACTION(TrGet, localFile, res);
        if (res != CURLE_OK) {
            //Even if the remote file could not be transferred,
            //cURL creates an empty file on local dir that must be deleted manually.
            try { // NOSONAR - to ignore exceptions
                acs::File::unlink(localFile);
            } 
            catch (std::exception const& ) { // NOSONAR - any exception 
                ; //ignore
            }
        }
        logError(res, remoteFile);
    END_TRANSACTION;
    updateStatistics(res);
    CurlStatistics::logResult(res, _site, "get");
    return res;
}



void Curl::updateStatistics(CURLcode res) {
    if (res == CURLE_OK) {
        //fill in statistics:
        curl_easy_getinfo(_curl, CURLINFO_TOTAL_TIME, &_stats.total_time);
        curl_easy_getinfo(_curl, CURLINFO_SPEED_DOWNLOAD, &_stats.speed_download);
        curl_easy_getinfo(_curl, CURLINFO_SPEED_UPLOAD, &_stats.speed_upload);
        curl_easy_getinfo(_curl, CURLINFO_NAMELOOKUP_TIME, &_stats.nlt);
        curl_easy_getinfo(_curl, CURLINFO_CONNECT_TIME, &_stats.ct);
#if ( (LIBCURL_VERSION_MAJOR > 7) || ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR >= 19)) )
        curl_easy_getinfo(_curl, CURLINFO_APPCONNECT_TIME, &_stats.act);
#else
#warning CURLINFO_APPCONNECT_TIME Not available: app_connection_time will be always 0.
#endif
        curl_easy_getinfo(_curl, CURLINFO_PRETRANSFER_TIME, &_stats.ptt);
        curl_easy_getinfo(_curl, CURLINFO_STARTTRANSFER_TIME, &_stats.stt);
        curl_easy_getinfo(_curl, CURLINFO_REDIRECT_TIME, &_stats.rt);
        curl_easy_getinfo(_curl, CURLINFO_SIZE_UPLOAD, &_stats.sizeUpload);
        curl_easy_getinfo(_curl, CURLINFO_SIZE_DOWNLOAD, &_stats.sizeDownload);
    }

}

CURLcode Curl::ls(string const &dir, void* output) // NOSONAR - output to be passed to curl
{
    string remoteDir = dir;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "dir " << _site.getHostAndPort() << "/" << remoteDir);
    CurlStatistics::log(_site, "list");
    if ('/' != *remoteDir.rbegin()) {
        remoteDir = remoteDir + "/";
    }
    CURLcode res = CURLE_OK ;

	// save old value and set new timeout
	bool adjustConnTimeout = false;
    size_t oldTimeOut_ms = setConnectionTimeout_ms(conf::CurlConfigurerSingleton::instance()->getLsConnectionTimeout_ms(), adjustConnTimeout);
	
	try {
    	BEGIN_TRANSACTION(TrList, false) // PRQA S 4236
        	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _listCallback);
        	if (output) {
            	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "ls stream is not null");
            	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, output);
        	} 
        	else {
            	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "ls stream is null");
            	if (_listCallbackData) {
                	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "ls _listCallbackData is not null");
                	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, _listCallbackData);
            	} 
            	else {
                	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "ls _listCallbackData is null");
                	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, 0);
            	}
        	}

        	res = performTransfer(true, remoteDir, "", 0, nullptr);
        	LOG_TRANSACTION(TrList, remoteDir, res);

        	logError(res, remoteDir);
    	END_TRANSACTION;
	}
	catch( const std::exception & )
	{
		// Restore the connection timeout
    	setConnectionTimeout_ms(oldTimeOut_ms, adjustConnTimeout);
		throw;
	}
		
	// Restore the connection timeout
    setConnectionTimeout_ms(oldTimeOut_ms, adjustConnTimeout);

    CurlStatistics::logResult(res, _site, "list");
    return res;

}

void Curl::addCustomHeader(const std::string &name, const std::string &value) 
{
    _custhdrs.push_back(name + ": " + value) ;
}

void Curl::addCustomHeader(const std::string &h) 
{
    _custhdrs.push_back(h) ;
}


CURLcode Curl::del(const string &remoteFile) 
{
  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file: " << _site.getHostAndPort() << "/" << remoteFile);
  CurlStatistics::log(_site, "del");
  CURLcode res = CURLE_OK;
  BEGIN_TRANSACTION(TrRemove, false)  // PRQA S 4236
  auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl);
  _ptrToHeaders.push_back(maker);  // PRQA S 3081
  if (!_site.getIsSFTP())
  {
    maker->addHeader("DELE " + remoteFile);
    maker->makeHeader();
  }
        else {
            maker->addHeader("rm " + remoteFile) ;
            maker->makeHeader();
        }
		
		// Execute the command without listing the directory
        maker->makeGet("", nullptr, true);

        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "file to delete: " << remoteFile);

        res = curl_easy_perform(_curl);
        LOG_TRANSACTION(TrRemove, remoteFile, res);

        maker->close();

        if (res == CURLE_REMOTE_FILE_NOT_FOUND) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "The remote file (to be deleted) is not present on the remote site.");
            res = CURLE_OK;
        }
        logError(res, remoteFile);
    END_TRANSACTION;
    CurlStatistics::logResult(res, _site, "del");
    return res;
}

CURLcode Curl::delDir(const string &remoteDir) 
{
  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file: " << _site.getHostAndPort() << "/" << remoteDir);
  CurlStatistics::log(_site, "delDir");
  CURLcode res = CURLE_OK;
  BEGIN_TRANSACTION(TrRemove, false)  // PRQA S 4236
  auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl);
  _ptrToHeaders.push_back(maker);  // PRQA S 3081

  if (!_site.getIsSFTP())
  {
    maker->addHeader("RMD " + remoteDir);
  }
        else {
            maker->addHeader("rmdir " + remoteDir) ;
        }

        maker->makeHeader();
        maker->makeGet("", nullptr, true);

        res = curl_easy_perform(_curl);
        LOG_TRANSACTION(TrRemove, remoteDir, res);

        maker->close();

        logError(res, remoteDir);
    END_TRANSACTION;
    CurlStatistics::logResult(res, _site, "delDir");
    return res;
}

CURLcode Curl::mkDir(string const &remoteDir) 
{
  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "dir: " << _site.getHostAndPort() << "/" << remoteDir);
  CurlStatistics::log(_site, "mkDir");
  CURLcode res = CURLE_OK;
  BEGIN_TRANSACTION(TrMkDir, false)  // PRQA S 4236
  auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl);
  _ptrToHeaders.push_back(maker);  // PRQA S 3081

  if (!_site.getIsSFTP())
  {
    maker->addHeader("MKDIR " + remoteDir);
  }
        else {
            maker->addHeader("mkdir " + remoteDir) ;
        }

        maker->makeHeader();
        maker->makeGet("", nullptr, true);

        res = curl_easy_perform(_curl);
        LOG_TRANSACTION(TrMkDir, remoteDir, res);

        maker->close();

        logError(res, remoteDir);
    END_TRANSACTION;
    CurlStatistics::logResult(res, _site, "mkDir");
    return res;
}


CURLcode Curl::putRename(
    const string &localFile, 
    const string &remoteFile, 
    const string &finalRemoteFName,
    bool renameAlso,
    mode_t permissions) 
{
	void* default_data_pointer = nullptr ; 

	return putRenameWithCallback(localFile, remoteFile, finalRemoteFName, renameAlso, permissions, default_data_pointer, _putCallback) ; 
}


CURLcode Curl::putRenameWithCallback(
    const string &localFile, 
    const string &remoteFile, 
    const string &finalRemoteFName,
    const bool renameAlso,
    const mode_t& permissions,
	void* data_pointer,         // NOSONAR - callback 
	PutCallback put_pointer)    // NOSONAR - callback 
{
    //put the local file to the remote _site using the remoteFile name.
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Put file " << localFile 
                        << " into (final) " << finalRemoteFName << " temp: " << remoteFile);
    CurlStatistics::log(_site, "put");
    ACS_COND_THROW(! File::exists(localFile), CurlException("Put error: local entry" + localFile + ": does not exist.")) ; // PRQA S 3081 2
    ACS_COND_THROW(! File::isReadable(localFile), CurlException("Put error: local entry" + localFile + ": it is not readable.")) ;
    ACS_COND_THROW( (File::isDir(localFile) && (! File::isExecutable(localFile)) ), CurlException("Put error: local directory" + localFile + ": it is not accessible.")) ;
    CURLcode res = CURLE_OK;
    BEGIN_TRANSACTION(TrPut, true) // PRQA S 4236
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE    
        curl_easy_setopt(_curl, CURLOPT_UPLOAD_BUFFERSIZE, conf::CurlConfigurerSingleton::instance()->getUploadBufferSize());
#endif
        curl_easy_setopt(_curl, CURLOPT_READFUNCTION, put_pointer);
        curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1);
        res = performTransferRenameChmod(localFile, remoteFile, finalRemoteFName, permissions, renameAlso, data_pointer);
        LOG_TRANSACTION(TrPut, remoteFile, res); // NB put and rename to be virtually undone
        LOG_TRANSACTION(TrRename, finalRemoteFName, res);
        logError(res, remoteFile);
        END_TRANSACTION
    updateStatistics(res);
    CurlStatistics::logResult(res, _site, "put");
    return res;
}

CURLcode Curl::rename(const string &remoteFilename, const string &newRemoteFilename) 
{
  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file: " << _site.getHostAndPort() << "/" << remoteFilename << " to: " << newRemoteFilename);
  CurlStatistics::log(_site, "rename");
  CURLcode res = CURLE_OK;
  BEGIN_TRANSACTION(TrRename, false)  // PRQA S 4236
  auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl);
  _ptrToHeaders.push_back(maker);  // PRQA S 3081

  if (!_site.getIsSFTP())
  {
    maker->addHeader("RNFR " + remoteFilename);
    maker->addHeader("RNTO " + newRemoteFilename);
  }
        else {
            maker->addHeader("rename " + remoteFilename + " " + newRemoteFilename) ;
        }

        maker->makeHeader();
        maker->makeGet("", nullptr, true);

        res = curl_easy_perform(_curl);
        LOG_RENAME_TRANSACTION(TrRename, newRemoteFilename, remoteFilename, res);
        maker->close();
        logError(res, remoteFilename);
    END_TRANSACTION;
    CurlStatistics::logResult(res, _site, "rename");
    return res;
}

CURLcode Curl::chmod(const string &remoteFilename, mode_t permissions) 
{
  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file/dir: " << _site.getHostAndPort() << "/" << remoteFilename << " to: " << std::oct << permissions);
  CurlStatistics::log(_site, "chmod");
  CURLcode res = CURLE_OK;
  BEGIN_TRANSACTION(TrChmod, false)  // PRQA S 4236
  auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl);
  _ptrToHeaders.push_back(maker);  // PRQA S 3081
  std::ostringstream os;
  if (!_site.getIsSFTP())
  {
    os << "SITE CHMOD 0" << std::oct << permissions << " " << remoteFilename;
  }
        else {
            os << "chmod 0" << std::oct << permissions << " " << remoteFilename;
        }
        maker->addHeader(os.str());
        maker->makeHeader();
        maker->makeGet("", nullptr, true);

        res = curl_easy_perform(_curl);
        maker->close();
        logError(res, remoteFilename);
    END_TRANSACTION;
    CurlStatistics::logResult(res, _site, "chmod");
    return res;
}

CURLcode Curl::lchmod(const string &localFilename, mode_t permissions) 
{
    //rename the local file logging the operation in the transaction .
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[lchmodFrom: " << localFilename << " Permissions: " << permissions << "]");
    CURLcode res = CURLE_WRITE_ERROR;
    if (!acs::File::exists(localFilename)) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "the local file " << localFilename << "  does not exit: cannot change mode on it. ");
    } 
    else {
        BEGIN_TRANSACTION(TrLocalChmod, false) // PRQA S 4236
            try { // NOSONAR - to catch and trasnform in CURLE_WRITE_ERROR
                File::chmod(localFilename, permissions);
                res = CURLE_OK;
            } 
            catch (std::exception const& ) { // NOSONAR - any exception 
                ACS_LOG_ERROR("Could not change permissions to file " << localFilename);
                res = CURLE_WRITE_ERROR;
            }
        END_TRANSACTION;
    }
    return res;
}

CURLcode Curl::lchown(const string &localFilename, uid_t owner, gid_t group) 
{
    //rename the local file logging the operation in the transaction .
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[lchownFrom: " << localFilename << " Owner: " << owner << " Group: " << group << "]");
    CURLcode res = CURLE_WRITE_ERROR;
    if (!acs::File::exists(localFilename)) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "the local file " << localFilename << "  does not exit: cannot change ownership on it. ");
    } 
    else {
        BEGIN_TRANSACTION(TrLocalChmod, false) // PRQA S 4236
            try { // NOSONAR - to catch and transform into CURLE_WRITE_ERROR
                File::chown(localFilename, owner, group);
                res = CURLE_OK;
            } 
            catch (std::exception const& ex) { // NOSONAR - any exception
                ACS_LOG_ERROR("Could not change ownership to file " << localFilename << ". Reason was: " << ex.what());
                res = CURLE_WRITE_ERROR;
            }
        END_TRANSACTION;
    }
    return res;
}

CURLcode Curl::performTransferRenameChmod(
    const string &fileToTransfer, 
    const string &targetFilename, 
    const string &finalTargetFName,
    mode_t permissions,
    bool removeAlso,
    void* readData)  // NOSONAR - callback
{
    auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl) ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Perform transfer to " << targetFilename 
                            << " and rename to target file name: " << finalTargetFName << " removeAlso: " << std::boolalpha << removeAlso); // PRQA S 3380
    _ptrToHeaders.push_back(maker); // PRQA S 3081
    maker->makePut(fileToTransfer, targetFilename, readData);
    const curl_version_info_data *d = curl_version_info(CURLVERSION_NOW);

#if ( (LIBCURL_VERSION_MAJOR < 7) || ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR < 24)) )             
#warning FranAva: Although this lib can be used with any version of libCurl, Curl::performTransferRenameChmod() might not work on version prior 7.24 . Consider installing at least libcurl v7.24
#endif

	if( d->version_num >= 0x071800 )    //Curl only supports '*'-prefix for bot  FTP & SFTP since version 7.24 // PRQA S 4400
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ftp: *RM " << std::hex << d->version_num )
		maker->addPreCommand("*DELE " + targetFilename) ;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ftp: RM " << std::hex << d->version_num )
		maker->addPreCommand("DELE " + targetFilename) ;
	}

  if (!_site.getIsSFTP())
  {
    if (removeAlso)
    {
      if (d->version_num >= 0x071800)  // Curl only supports '*'-prefix for bot  FTP & SFTP since version 7.24 // PRQA S 4400
      {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftp: *RM " << std::hex << d->version_num)
        maker->addHeader("*DELE " + finalTargetFName);
      }
      else
      {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftp: RM " << std::hex << d->version_num)
        maker->addHeader("DELE " + finalTargetFName);
      }
    }

    if (permissions)
    {
      std::ostringstream os;
      if (!_site.getIsSFTP())
      {
        os << "SITE CHMOD 0" << std::oct << permissions << " " << targetFilename;
      }
      else
      {
        os << "chmod 0" << std::oct << permissions << " " << targetFilename;
      }
      maker->addHeader(os.str());
    }

    maker->addHeader("RNFR " + targetFilename);
    maker->addHeader("RNTO " + finalTargetFName);
  }
    else {                                      
        if(removeAlso)
        {
            if( d->version_num >= 0x071800 )   //Curl only supports '*'-prefix for bot  FTP & SFTP since version 7.24 // PRQA S 4400
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"sftp: *RM " << std::hex << d->version_num )
                maker->addHeader("*rm " + finalTargetFName) ;
            }
            else
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"sftp: RM "<< std::hex << d->version_num )
                maker->addHeader("rm " + finalTargetFName) ;
            }
        }
        
        if (permissions) {
            std::ostringstream os;
            if (!_site.getIsSFTP())
            {
              os << "SITE CHMOD 0" << std::oct << permissions << " " << targetFilename;
            }
            else {
                os << "chmod 0" << std::oct << permissions << " " << targetFilename;
            }
            maker->addHeader(os.str());
        }
        
        maker->addHeader("rename " + targetFilename + " " + finalTargetFName) ;
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "rename " << targetFilename << " " << finalTargetFName) ;
    }

    maker->makeHeader();
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "curl perform PUT+CHMOD+RN"); // PRQA S 3380
    CURLcode code = curl_easy_perform(_curl);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Return code  " << int(code));  // PRQA S 3081
    return code;
}


CURLcode Curl::performTransfer(
    bool get, 
    const string &fileToTransfer, 
    const string &targetFilename, 
    mode_t permissions,
    void* readData) // NOSONAR - callback
{
    auto maker = std::make_shared<CurlHeaderMaker>(_site, _curl) ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Perform transfer of :" << (get ? "GET" : "PUT") << " target file name: " << targetFilename << " file to transfer: " << fileToTransfer); // PRQA S 3380
    _ptrToHeaders.push_back(maker); // PRQA S 3081
    if (get) {
        maker->makeGet(fileToTransfer, readData);
    } 
    else {
        maker->makePut(fileToTransfer, targetFilename, readData);
        if (permissions) {
            std::ostringstream os;
            if (!_site.getIsSFTP())
            {
              os << "SITE CHMOD 0" << std::oct << permissions << " " << targetFilename;
            }
            else {
                os << "chmod 0" << std::oct << permissions << " " << targetFilename;
            }
            maker->addHeader(os.str());
        }
    }
    maker->makeHeader();
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "curl perform " << (get ? "GET" : "PUT")); // PRQA S 3380
    CURLcode code = curl_easy_perform(_curl);
	if (code == CURLE_HTTP_RETURNED_ERROR)
	{ curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_response_code); }

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Return code  " << int(code));  // PRQA S 3081
    return code;
}


void Curl::logError(CURLcode res, string const& fileToTransfer) const 
{
    std::ostringstream message;
    if (res == CURLE_OK) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,  // PRQA S 3081
                " curl return Ok: " << int(res) << " (" << curl_easy_strerror(res) << ")" << " RemoteFilename: "
                        << fileToTransfer);
    } 
    else {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,   // PRQA S 3081
                " curl return error: " << int(res) << " (" << curl_easy_strerror(res) << ")" << " RemoteFilename: "
                        << fileToTransfer);
        if (res != CURLE_OK) {
            message << "Curl error url: \"" << fileToTransfer << "\" curl returned: " << int(res) << " (" // PRQA S 3081
                    << curl_easy_strerror(res) << ")" << " - ErrMsg = " << _errorMessage.data();
            ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, message.str());
        }
    }
    _lastLoggedErrorMessage = message.str();
}

void Curl::fillInCirculationStatistics(bool source, CirculationStatistics* statistics) const 
{ 
    if (! Application::exists() || !Application::instance()->getDefaultConnectionPool() ) {
        return ;
    }
    dbConnectionWrapper w(*Application::instance()->getDefaultConnectionPool()) ;
    dbConnection &conn = w ; // PRQA S 3050
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, (source ? "Source" : "Destination")); // PRQA S 3380
    if (source) {
        statistics->setSourceSite(_site);
    } 
    else {
        statistics->setDestSite(_site);
    }
    statistics->setNameLookUpTime(_stats.nlt,conn);
    statistics->setConnectionTime(_stats.ct,conn);
    statistics->setAppConnectionTime(_stats.act,conn);
    statistics->setPreTransfTime(_stats.ptt,conn);
    statistics->setStartTransfTime(_stats.stt,conn);
    statistics->setRedirectTime(_stats.rt,conn);
    statistics->setTotalTime(_stats.total_time,conn);
    statistics->setDataSize(static_cast<long long>(max(_stats.sizeUpload, _stats.sizeDownload)),conn); // PRQA S 3081
}

//
//All Default callbacks.
//
int Curl::debugCallback(void *, int infotype, char *dbg, size_t dbgsize, void *dbgdata) // NOSONAR - callback
{
    if (!dbg || !dbgsize) { // no info, just return
        return 0;
    }

    const Curl* thisObj = reinterpret_cast<Curl*>(dbgdata); // NOSONAR
    string currentSite;

    if (thisObj) {
        RemoteSite site = thisObj->getSite();
        currentSite = RemoteSiteFactory::toURL(site, false);
    }
    char str[dbgsize + 1]; // NOSONAR - safe
    memset(str, 0, sizeof(str));
    strncpy(str, dbg, dbgsize); // NOSONAR - safe
    std::map<char, char > tr ; 
    tr['\r'] = ' ' ;
    tr['\n'] = ' ' ;

    string src = StringUtils::transform(str, tr);
    switch (infotype) { // according to info type
    case CURLINFO_TEXT: // text
    case CURLINFO_HEADER_OUT: // header out
    case CURLINFO_HEADER_IN: // header in
        ACS_LOG_DEBUG(exDebugSignatureThread << "[cURL@" << currentSite << "] " << src) ;
        break;
    case CURLINFO_DATA_IN: // no debug for data
    case CURLINFO_DATA_OUT:
        break ;
    default:
        // neither for other types
        ACS_LOG_DEBUG(exDebugSignatureThread << "infotype: " << infotype << " unhandled ***") ;
        break;
    }

    return 0;

}

size_t Curl::listCallback(void *buffer, size_t size, size_t nmemb, void *stream) // NOSONAR - callback
{
    if (stream) {
        auto *out = reinterpret_cast<std::ostream *>(stream); // NOSONAR - is a callback
        if (!out) {
            ACS_LOG_ERROR(SimpleDebugSignature << "cannot write: null stream") ;
            return 0;
        }
        out->write(static_cast<char *>(buffer), size * nmemb); // PRQA S 3000, 3081, 3084
        if (out->fail()) {
            ACS_LOG_ERROR(SimpleDebugSignature << "write failed") ;
            return 0;
        }
    } 
    else {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Warning: the listcallback has been called without a stream for " << size * nmemb << " [B]"); // PRQA S 3084
    }
    return size * nmemb; // PRQA S 3084
}

size_t Curl::probeCallBack(void *, size_t size, size_t nmemb, void *curl) // NOSONAR - callback
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Got " << size*nmemb << " [B] buffer" ) // PRQA S 3084
    ACS_COND_THROW( curl == 0 , exIllegalValueException("the listcallback has been called without a curl.")); // PRQA S 3081
  
    Curl *cUrl = reinterpret_cast<Curl *>(curl); // NOSONAR
    if(cUrl && cUrl->setProbeToken())
    {
        return size * nmemb ; // PRQA S 3084
    }
    return 0; 
}

size_t Curl::getCallback(void *buffer, size_t size, size_t nmemb, void *stream) { // NOSONAR - callback

	// The stream argument is assumed to be a pointer to CurlFile* (in this case, a wrapper to FILE*) -- this is the default

    size_t result = 0;
    CurlFile *out = reinterpret_cast<CurlFile*>(stream); // NOSONAR
    if (!out || !out->getStream()) {
        ACS_LOG_ERROR(SimpleDebugSignature << "cannot write: null stream") ;
    } 
    else {
        ssize_t w = fwrite(buffer, size, nmemb, static_cast<FILE*>(out->getStream())); // PRQA S 3000
        ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Bytes written: " << w);
        if (w >= 0) {
            out->addWrote(off_t(w)); // PRQA S 3081
            ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Total bytes written: " << out->getWrote());
        }
        result = (w > 0) ? size_t(w) : 0; // PRQA S 3081, 3380
    }
    return result;
}


size_t Curl::ostreamGetCallback(void *ptr_buffer_in, size_t size, size_t nmemb, void *ptr_ostream_out) { // NOSONAR - callback

	// The stream argument is assumed to be a pointer to CurlFile* (in this case, a wrapper to ofuntarstream*) -- this is not the default

    size_t result = 0 ;
	const size_t to_write (size * nmemb) ; 
    CurlFile *out = reinterpret_cast<CurlFile*>(ptr_ostream_out); // NOSONAR
    if (!out || !out->getStream()) {
        ACS_LOG_ERROR(SimpleDebugSignature << "cannot write: null stream") ;
    } 
    else 
	{
		// Extract the ostream, which is "embedded" inside the ofuntarstream contained in CurlFile*
		// Note that operator() extracts the ostream contained inside the ofuntarstream object
		std::ostream &os(static_cast<ofuntarstream*>(out->getStream())->operator()()) ; 	
		
		if ( (os.good()) && (to_write > 0) )
		{
			ACS_COND_THROW( (!ptr_buffer_in), exIllegalValueException("buffer_in is a NULL ptr !") ) ;

			// Copy the input buffer to the ostream. Like: ostream << input_buffer but "write" method is used instead.
			os.write(reinterpret_cast <char*>(ptr_buffer_in), to_write) ; // NOSONAR
						
			// ENRCAR:: Please Note:
			// 1. Is _NOT_ possible to know how many bytes have been written by the command above
			//    (only possibility would be the difference among two tellp, before and after, but is implement. dependant and unreliable)
			// 2. Accordingly to the legacy method "getCallback", which is the legacy default, any write error (which would result in -1) is simply ignored, 
			//    generically returning 0 ("nothing to do). This approach might be debatible, but it's at least fully retro-compatible.

 			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Bytes written: " << to_write);
        	out->addWrote(off_t(to_write)); // PRQA S 3081
        	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Total bytes written: " << out->getWrote());
			result = to_write ;
		}
	
	}

    return result;
}


size_t Curl::putCallback(void *ptr, size_t size, size_t nmemb, void *stream) { // NOSONAR - callback
	
	// The stream argument is assumed to be a pointer to FILE*  -- this is the default
   
    size_t retcode = fread(ptr, size, nmemb, static_cast<FILE *>(stream)); // PRQA S 3081
    ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB, "Bytes Sent: " << retcode);
    return retcode;
}

size_t Curl::istreamPutCallback(void *ptr_buffer_out, size_t size, size_t nmemb, void *ptr_istream_in) { // NOSONAR - callback
	
	// The stream argument is assumed to be a pointer to istream*  -- this is not the default

	const size_t to_read (size * nmemb) ; 
	size_t retcode (0) ;
	if (to_read > 0) 
	{
		ACS_COND_THROW( ! ptr_istream_in, exIllegalValueException("Input stream is a NULL ptr !") ) ;
		ACS_COND_THROW( ! ptr_buffer_out, exIllegalValueException("Output buffer is a NULL ptr !") ) ;
	
		(static_cast<std::istream*>(ptr_istream_in))->read(reinterpret_cast <char*>(ptr_buffer_out), to_read); // NOSONAR - is a callback
		retcode = (static_cast<std::istream*>(ptr_istream_in))->gcount() ;
	}

    return retcode;
}

void Curl::setProgressCallback(ProgressCallback progressCallback, void* progressCallbackData) { // NOSONAR - callback
    _progressCallback = progressCallback;
    _progressCallbackData = progressCallbackData;
}

void* Curl::getProgressCallbackData() const { // NOSONAR - callback related types 
    return _progressCallbackData; // PRQA S 4628
}

void Curl::setListCallback(ListCallback listCallback, void* listCallbackData) { // NOSONAR - callback related types 
    _listCallback = listCallback;
    _listCallbackData = listCallbackData;
}

void Curl::setDefaultListCallback() {
    _listCallback = listCallback;
    _listCallbackData = nullptr ;
}

void Curl::deregisterProgressCallback() // PRQA S 4211
{
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, 0);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, true);
    curl_easy_setopt(_curl, CURLOPT_DEBUGFUNCTION, 0);
    curl_easy_setopt(_curl, CURLOPT_READFUNCTION, 0);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, 0);
}

bool Curl::setProbeToken()
{
    ++probeToken_ ;
    return probeToken_ < 2 ; // PRQA S 4400
}

CURLcode Curl::probeElement (bool isFile, string const& element)
{
	CURLcode res = CURLE_OK ;
	string remoteElement = element;
	if (isFile)
	{
        // remove all trailing '/' if it has to test for file
		while (*remoteElement.rbegin() == '/')
		{
			remoteElement.erase ( remoteElement.end()-1);
		}
	}
	else
	{
        // add a trailing '/' to test for directories
	    if ('/' != *remoteElement.rbegin()) 
		{
    	    remoteElement = remoteElement + "/";
	    }
	}

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Probing remote site" << str() << " remote path: " << remoteElement);
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Probing for a " << ( isFile ? "File" : "Directory" ) ); // PRQA S 3380

    // save old value and set a short connection time to avoid hangs during probes
    size_t oldTimeOut_ms = setConnectionTimeout_ms(conf::CurlConfigurerSingleton::instance()->getProbeConnectionTimeout_ms());

	BEGIN_TRANSACTION(TrList, false) // PRQA S 4236
		// reset the probe counter
		probeToken_ = 0;
		// prepare the transfer
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, probeCallBack);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
		try { // NOSONAR - to catch any exception 
			res = performTransfer(true, remoteElement, "", 0, nullptr);
		}
		catch (const std::exception& ex) // NOSONAR - any exception
		{
			// should never happen but in the case
			// notify the exception and go ahead
			ACS_LOG_NOTIFY_EX (ex);
		}
		LOG_TRANSACTION(TrList, remoteElement, res);

        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Checking retvalue (" << int(res) <<  ") \"" <<  curl_easy_strerror(res) << "\" with probeToken " << probeToken_); // PRQA S 3081


		// URLE_WRITE error is expected when emitted by the probeCallBack
		if ( (CURLE_WRITE_ERROR == res) && (probeToken_ > 1) )
		{ 
	        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Remapping expected error (" << int(res) <<  ") \"" <<  curl_easy_strerror(res) << "\" to CURLE_OK"); // PRQA S 3081
			res = CURLE_OK;
		} 

		logError(res, remoteElement);  // log whatever different from CURLE_WRITE_ERROR (even ok) 
	END_TRANSACTION;

	// Restore the connection timeout
    setConnectionTimeout_ms(oldTimeOut_ms);

	CurlStatistics::logResult(res, _site, "probe"); 

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Returning CURL value " << int(res) <<  ": \"" <<  curl_easy_strerror(res) << "\""); // PRQA S 3081

	return res;
}


CURLcode Curl::probeLs(string const &dir) 
{
  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "dir " << _site.getHostAndPort() << "/" << dir);

  CURLcode retval = probeElement (false, dir);

    return retval; 
}



bool Curl::probe(string const &remotePath, bool emitExc, CURLcode *curlCode_p) 
{
    //probe the remote site.

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Probing remote site" << str() << " remote path: " << remotePath);
    bool success = false;
    try {
        //success only if the list method returns ok
		CURLcode res = probeElement (false, remotePath);
        success = ( CURLE_OK == res);
		if (curlCode_p)	{ *curlCode_p = res; }
        
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "success: " << std::boolalpha << success)
    } 
    catch (std::exception const& ) { // NOSONAR - any eception 
		if( emitExc ) {
			throw;
		}
	}
    if (success) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Success probing remote site!");
    } 
    else {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Failing probing remote site!");
    }
    return success;
}

string Curl::str() const {
    return _site.str();
}

//
//End of All Default callbacks.
//

//
//Getters/Setters
//
void Curl::setSite(RemoteSite const& site)  // PRQA S 4121
{
    _site = site;
}

const RemoteSite &Curl::getSite() const noexcept  // PRQA S 4120
{
    return _site;
}

void Curl::setTransaction(CurlTransaction* transaction) noexcept  // PRQA S 4121
{
    _transaction = transaction;
}

void Curl::setCreateMissingDirs(bool createMissingDirs) noexcept  // PRQA S 4121
{
    _createMissingDirs = createMissingDirs;
}

bool Curl::getCreateMissingDirs() const noexcept  // PRQA S 4120
{
    return _createMissingDirs;
}

size_t Curl::getConnectionTimeout_ms() const noexcept  // PRQA S 4120
{
    return _connectionTimeout_ms;
}

size_t Curl::setConnectionTimeout_ms(size_t const ms, bool adjustResponseTimeout) noexcept // PRQA S 4121
{
    return computeTimeouts(ms, adjustResponseTimeout, _connectionTimeout_ms, _connectionTimeout_s, _response_timeout_ms, _response_timeout_s) ;
}

size_t Curl::computeTimeouts(const size_t ms, const bool adjustResponseTimeout, size_t& connectionTimeout_ms, size_t& connectionTimeout_s, size_t& response_timeout_ms, size_t& response_timeout_s) noexcept // PRQA S 4121
{
    size_t old_ms = connectionTimeout_ms ;
    connectionTimeout_ms = ms;
    connectionTimeout_s = max<size_t>(2,connectionTimeout_ms/1000) ; // PRQA S 3084, 4400
	
	if( adjustResponseTimeout ) {
    	// adjust response - just in case 
    	response_timeout_ms = min(connectionTimeout_ms,conf::CurlConfigurerSingleton::instance()->getConnectionResponseTimeout_ms()) ;
    	response_timeout_s = max<size_t>(1, response_timeout_ms/1000) ;  // PRQA S 3084, 4400
	}

    return old_ms ;
}

const string &Curl::getLastErrorMessage() const noexcept // PRQA S 4120
{
    return _lastLoggedErrorMessage;
}


TransactionType Curl::getOperationCode() const noexcept { return _operationCode ; } // PRQA S 4120

void Curl::setOperationCode(TransactionType op) noexcept { _operationCode = op ; } // PRQA S 4121

CurlTransaction* Curl::getTransaction() const noexcept { return _transaction ; } // PRQA S 4120, 4628

void Curl::setCurl(CURL * c) noexcept { _curl = c ; } // NOSONAR - CURL is a type from curl

CURL * Curl::getCurl() const noexcept { return _curl ; } // NOSONAR - CURL is a type from curl

void Curl::checkFtpCommandResponseTimeout() noexcept
{
	if( 0 == _ftp_command_response_timeout_s ) {
		_ftp_command_response_timeout_s = _response_timeout_s;
		ACS_LOG_WARNING("Ftp command response timout is zero: set to " << _ftp_command_response_timeout_s << " s.");
	}
}

std::string Curl::encodeRFC398(const std::string & inString)
{
	// avoid useless operations
	if (inString.empty())
	{
		return "" ;
	}

	string encoded_s ;

	// Call thread safe initialization if needed
	CurlInitializer::instance()->checkSSL() ;

	// Obtain a curl instance
	CURL *curl = curl_easy_init(); // NOSONAR - SSL & TLS are used according to configuration and context
	if(curl)
	{
		// curl_easy_escape uses strlen to detect password length
		char *output = curl_easy_escape(curl, inString.c_str(), 0);
		if(output)
		{
			encoded_s = output;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Encoded string " << encoded_s);
			// free curl resources
			curl_free(output);
		}
		// free curl resources
		curl_easy_cleanup(curl);
	}

	// notify encoding or internal errors
	ACS_COND_THROW(encoded_s.empty(), CurlCodingException() << " Encoding of " << inString << " failed.");

	return encoded_s;
}

std::string Curl::decodeRFC398(const std::string & inString)
{
	// avoid useless things
	if (inString.empty())
	{
		return "" ;
	}

	string decoded_s ;

	// Call thread safe initialization if needed
	CurlInitializer::instance()->checkSSL() ;

	// Obtain a curl instance
	CURL *curl = curl_easy_init(); // NOSONAR - SSL & TLS are used according to configuration and context
	if(curl)
	{
		// curl_easy_escape uses strlen to detect password length
		int outlength;
		char *output = curl_easy_unescape(curl, inString.c_str(), 0, &outlength);
		if(output)
		{
			string res(output, output + outlength);
			decoded_s = res;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Encoded " << decoded_s);
			// free curl resources
			curl_free(output);
		}
		// free curl resources
		curl_easy_cleanup(curl);
	}

	// notify decoding or internal errors
    ACS_COND_THROW(decoded_s.empty(), CurlCodingException() << " Decoding of " << inString << " failed.");

	return decoded_s;
}

_ACS_END_NESTED_NAMESPACE
