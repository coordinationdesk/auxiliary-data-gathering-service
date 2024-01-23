// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2017, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. Data Consumer Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.13  2017/02/06 09:29:58  lucio.pulvirenti
 *  S2PDGS-1656: NOT_STARTED error code added.
 *
 *  Revision 5.12  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.11  2016/03/21 18:26:16  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.10  2016/03/21 10:20:08  francesco.avanzi
 *  rollback to previous state, WORKINPROGRESS
 *
 *  Revision 5.8  2014/04/15 16:18:38  marpas
 *  bad macro epuration in progress
 *
 *  Revision 5.7  2014/04/14 09:27:21  marpas
 *  management of filters fixed and improved. test added
 *
 *  Revision 5.6  2013/10/30 17:50:46  marpas
 *  coding best practices applied
 *  robustness improved
 *  useless classes removed
 *  compilation and qa warnings removed
 *
 *  Revision 5.5  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/09/12 07:43:59  lucio.pulvirenti
 *  getLogInfoPeriod and setLogInfoPeriod new methods for log info on parallel threads.
 *  LOG_INFO_PERIOD define introduced.
 *
 *  Revision 5.3  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.2  2013/07/18 12:29:08  marpas
 *  coding best practices applied
 *  type mismatching fix in progress
 *
 *  Revision 5.1  2013/07/15 16:48:06  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:20  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.12  2013/01/25 13:38:53  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.11  2012/03/27 10:53:29  micmaz
 *  work in progress
 *
 *  Revision 1.10  2012/03/15 16:49:13  micmaz
 *  work in progress
 *
 *  Revision 1.9  2012/03/15 14:36:11  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.8  2012/02/28 17:19:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.7  2012/02/07 10:25:56  micmaz
 *  removed warnings.
 *
 *  Revision 1.6  2011/10/28 13:14:03  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 1.5  2011/09/23 09:14:42  micmaz
 *  removed unused headers.
 *
 *  Revision 1.4  2011/09/22 15:08:23  micmaz
 *  changed dump() method.
 *
 *  Revision 1.3  2011/09/21 12:33:14  micmaz
 *  added the circulator creation time factor.
 *
 *  Revision 1.2  2011/09/19 10:38:36  micmaz
 *  build break in  test files.
 *
 *  Revision 1.1  2011/09/13 16:31:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.17  2011/08/31 15:28:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.16  2011/08/31 15:11:10  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.15  2011/08/29 13:08:34  micmaz
 *  IN PROGRESS - issue S1PDGS-1806: Add to the CirculationAgent configuration file the connection pool size.
 *  http://jira.acsys.it/browse/S1PDGS-1806
 *
 *  Revision 1.14  2011/08/29 12:05:21  micmaz
 *  RESOLVED - issue S1PDGS-1805: the CirculationAgent does not create a new consumer quickly when a single consumer finished.
 *  http://jira.acsys.it/browse/S1PDGS-1805
 *
 *  Revision 1.13  2011/06/28 17:04:20  micmaz
 *  Added the time to reprocess for circulators
 *
 *  Revision 1.12  2011/04/26 14:42:15  micmaz
 *  Updated logs and added removing of sources (if required)
 *
 *  Revision 1.11  2011/04/20 16:41:00  micmaz
 *  the "errored" circulation records must be re-worked only after ELAPSED_TIME_TO_RETRY seconds.
 *
 *  Revision 1.10  2011/03/11 17:56:12  micmaz
 *  reworked timeouts.
 *
 *  Revision 1.9  2011/03/10 16:34:09  micmaz
 *  added new paramters for connection and db  time out.
 *  Improved log in case of failure
 *
 *  Revision 1.8  2011/03/08 18:15:37  micmaz
 *  Added the ovewrite flag to the configurer
 *
 *  Revision 1.7  2011/03/03 14:28:27  micmaz
 *  work in progress
 *
 *  Revision 1.6  2010/12/17 10:29:20  micmaz
 *  Added MCF messages.
 *
 *  Revision 1.5  2010/12/09 16:22:15  micmaz
 *  added Mail Warning.
 *
 *  Revision 1.4  2010/11/30 16:41:49  micmaz
 *  added mail warning.
 *
 *  Revision 1.3  2010/11/22 13:01:15  micmaz
 *  added the product logger, correct some minor bugs.
 *
 *  Revision 1.2  2010/11/11 17:45:03  micmaz
 *  corrected some issues : the consumers terminated with a coredump, the consumer hangs on when stopping with errored items, the data consumer item factory does not build correctly a DataConsumerItem.
 *
 *  Revision 1.1.1.1  2010/11/04 10:18:56  micmaz
 *  create a new library
 *
 *
 *
 */

#ifndef _DataConsumerConfigurer_H_
#define _DataConsumerConfigurer_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <rsResourceSet.h>

#include <string>


_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)

enum ErrorCodes {
    SUCCESS_TRANSFER = 0,
    ERROR_UNKNOWN    = -1,
    ERROR_LOGIN      = -67,
    ERROR_HOST_UNKNOWN = -6, 
    ERROR_TRANSFER_INTERRUPTED = -30,
    ERROR_REMOTE_FILE_SYSTEM_ERROR = -40,
    ERROR_CONNECTION_FAILED_ERROR = -7,
	NOT_STARTED = -10
} ;


class DataConsumerConfigurer  { // PRQA S 2109 

public:
    DataConsumerConfigurer();
private:
    DataConsumerConfigurer(DataConsumerConfigurer const& other);
    DataConsumerConfigurer& operator=(DataConsumerConfigurer const& other);
public:
    virtual ~DataConsumerConfigurer() throw ();

    /**
     * A dump method in order to inspect the configuration parameters read from file.
     */
    virtual void dump() const;
    /**
     * Loads the configuration parameters from file.
     */
    virtual void load();

    static void confLoadedCallback(void *cp);
    static bool onceForRun() { return _onceForRun; }
    static void setOnceForRun(bool onceForRun) { _onceForRun = onceForRun; }
#ifdef ACS_TEST
public:
#else
private:
#endif
    static bool _onceForRun ;

public:
    /**
     * Getters/Setters for all properties.
     */
    std::string const& getFilterList() const throw();
    std::string const& getByEmailExtension() const throw();
    size_t getElapsedTimeToRetry() const throw();
    std::string const& getForceMailWarningAddress() const throw();
    size_t getFtpPort() const throw();
    std::string const& getLogFilename() const throw();
    std::string const& getMailLogin() const throw();
    std::string const& getMailPassword() const throw();
    int getMailPort() const throw();
    std::string const& getMailSender() const throw();
    std::string const& getMailServer() const throw();
    size_t getMailWarningRetries() const throw();
    std::string const& getMailWarningStyle() const throw();
    size_t getMailWarningTimeToRetry() const throw();
    std::string const& getMailWarningXSL() const throw();
    size_t getMaxDbElapsedTime() const throw();
    size_t getMaxDbRetries() const throw();
    size_t getMaxNumberOfRetries() const throw();
    size_t getNumberOfParallelConsumers() const throw();
    bool getOverwriteCompletedFiles() const throw();
    bool getRemoveSources() const throw();
    size_t getSftpPort() const throw();
    int getMaxPriority() const throw();
	size_t getLogInfoPeriod() const throw();
	bool getCheckPreTransfer() const  throw();
    void setByEmailExtension(const std::string &);
    void setElapsedTimeToRetry(size_t ) throw();
    void setForceMailWarningAddress(const std::string &);
    void setFtpPort(size_t) throw();
    void setLogFilename(const std::string &);
    void setMailLogin(const std::string &);
    void setMailPassword(const std::string &);
    void setMailPort(int ) throw();
    void setMailSender(const std::string &);
    void setMailServer(const std::string &);
    void setMailWarningRetries(size_t) throw();
    void setMailWarningStyle(const std::string &);
    void setMailWarningTimeToRetry(size_t) throw();
    void setMailWarningXSL(const std::string &);
    void setMaxDbElapsedTime(size_t) throw();
    void setMaxDbRetries(size_t) throw();
    void setMaxNumberOfRetries(size_t) throw();
    void setNumberOfParalledConsumers(size_t) throw();
    void setOverwriteCompletedFiles(bool) throw();
    void setProductionPath(const std::string &);
    void setRemoveSources(bool) throw();
    void setSftpPort(size_t) throw();
    void setMaxPriority(int) throw();
    size_t getConnectionPoolMaxSize() const throw();
    void setConnectionPoolMaxSize(size_t) throw();
	void setLogInfoPeriod(size_t)  throw();
	void setCheckPreTransfer(bool) throw();
	bool getEarlyFinalRemoval() const  throw() ;

public:
    // PRQA S 2100, 4400 L2
    static const int DefaultSFTPPort = 22 ;
    static const int DefaultFTPPort = 21 ;
    static const int DefaultConsumers = 10 ;
    static const int DefaultRetriesNum = 3 ;
    static const bool DefaultOverwriteFiles = false ;
    static const int DefaultMaxDbRetries = 10 ;
    static const int DefaultDbErrorRetrySec = 10 ;
    static const size_t DefaultErrorRetrySec = 30 ;
    static const int DefaultMaxPriority = 657000 ;
    static const int DefaultConnPoolSize = 10 ;
    static const bool DefaultRemoveSource = false ;
    static const std::string DefaultLogFilename ; // "error.log" ;
    static const std::string spaceName;
	static const int DefaultLogInfoPeriodSec = 15; 

    static const std::string XmlSlipEmailStyle ;    // "XML_SLIP"
    static const std::string XmlEmailStyle ;        // "XML"
    static const std::string HtmlEmailStyle ;        // "HTML"
    static const std::string RsetEmailStyle ;        // "RSET"

    // PRQA L:L2
#ifdef ACS_TEST
public:
#else
private:
#endif
    size_t _sftpPort;
    size_t _ftpPort;
    bool _removeSources;
    bool _overwriteCompletedFiles;
    size_t _numberOfParallelConsumers;
    size_t _maxNumberOfRetries;
    size_t _maxDbRetries;
    /**
     * Elapsed time to retry for errored items (in seconds), if the error occurred during db access .
     */
    size_t _dbErrorRetryTimeSec ;
    /**
     * Elapsed time to retry for errored items (in seconds).
     */
    size_t _retryTimeSec ;
    std::string _byEmailExtension;
    std::string _logFilename;

    std::string _mailWarningStyle;
    std::string _mailWarningXSL;
    std::string _forceMailWarningAddress;
    std::string _mailServer;
    int _mailPort;
    std::string _mailLogin;
    std::string _mailPassword;
    std::string _mailSender;
    size_t _mailWarningRetries;
    size_t _mailWarningTimeToRetry;
    int _maxPriority;
    size_t _connectionPoolMaxSize;
	size_t _logInfoPeriodSec;
    std::string filterList_ ;
    bool earlyFinalRemoval_ ;
    bool checkPreTransfer_;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(DataConsumerConfigurer) ;

};

typedef acs::pattern::Singleton<DataConsumerConfigurer> dcConfigurerSingleton;


_ACS_END_NESTED_NAMESPACE


#endif /* _DataConsumerConfigurer_H_ */
