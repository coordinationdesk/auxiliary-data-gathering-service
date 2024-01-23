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
 *  $Prod: A.C.S. libFtpCurl
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.13  2017/07/26 07:42:16  lucio.pulvirenti
 *  S1PDGS-31936: Optional configuration for timeout to use for remote dir list introduced along with get and set methods.
 *
 *  Revision 5.12  2017/05/05 16:09:53  lucio.pulvirenti
 *  S3PDGS-3137: ftp command response timeout added (default 60 s) along with get and set methods.
 *
 *  Revision 5.11  2017/04/11 16:30:57  marfav
 *  Added fast probe resource configuration flag
 *
 *  Revision 5.10  2015/11/27 09:08:23  marpas
 *  findProxy signals outside if the returned proxy was found in the configuration or generate inside the moethod
 *
 *  Revision 5.9  2014/11/13 13:56:15  lucio.pulvirenti
 *  S2PDGS-915: _localTempPath, _httpListStylesheetName and _httpListStylesheetVersion attribute added to IF with get and set methods.
 *
 *  Revision 5.8  2014/10/29 13:13:04  lucio.pulvirenti
 *  S2PDGS-955: List of proxies, if any, got from configuration with the proper new method. getProxy method implemented to return the proxy, if any, corresponding to given host.
 *
 *  Revision 5.7  2014/10/28 16:31:43  lucio.pulvirenti
 *  S2PDGS-955 work in progress.
 *
 *  Revision 5.6  2013/11/04 15:41:51  marpas
 *  simulation mode added to conf
 *
 *  Revision 5.5  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.4  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.3  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/15 16:30:26  marpas
 *  qa warnings fixed
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.29  2013/05/14 18:45:18  marpas
 *  Singleton release at exit behaviour adopted
 *
 *  Revision 2.28  2013/05/02 12:44:22  marpas
 *  static definition for CCC commented out
 *
 *  Revision 2.27  2013/04/12 12:05:48  marpas
 *  statistics enabling logic fixed
 *
 *  Revision 2.26  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.25  2013/03/22 13:25:05  marpas
 *  mode is an integral, not a string !
 *   ----------------------------------------------------------------------
 *
 *  Revision 2.24  2013/02/13 15:09:58  micmaz
 *  updated to new Singleton
 *
 *  Revision 2.23  2013/01/25 13:06:46  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 2.22  2012/09/21 16:41:09  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 2.21  2012/09/21 13:34:28  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 2.20  2012/09/06 15:55:24  micmaz
 *  added two new paramters: low speed time and low speed limit in order to stop a low transfer.
 *
 *  Revision 2.19  2012/06/19 13:27:31  micmaz
 *  libFtpCurl now can transfer files without password.
 *
 *  Revision 2.18  2012/06/18 13:18:48  micmaz
 *  improved check of statistics.
 *
 *  Revision 2.17  2012/04/24 14:11:02  micmaz
 *  the configuration now tests if the curl statistics exists.
 *
 *  Revision 2.16  2012/03/29 13:34:05  micmaz
 *  removed conf parameter
 *
 *  Revision 2.15  2012/03/28 15:03:57  micmaz
 *  work in progress
 *
 *  Revision 2.14  2012/03/28 14:16:02  micmaz
 *  work in progress
 *
 *  Revision 2.13  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.12  2012/03/15 16:49:13  micmaz
 *  work in progress
 *
 *  Revision 2.11  2012/03/15 11:45:26  micmaz
 *  work in progress
 *
 *  Revision 2.10  2012/03/15 09:55:22  marpas
 *  no longer use of ConfigurerAdder
 *
 *  Revision 2.9  2012/03/09 17:43:55  micmaz
 *  added media functionality
 *
 *  Revision 2.7  2012/03/08 11:21:39  marpas
 *  const must be defined as variables not DEFINE
 *
 *  Revision 2.6  2012/02/28 17:19:10  micmaz
 *  refactory Data circulator
 *
 *  Revision 2.5  2012/02/10 17:41:12  micmaz
 *  now the ftps connection will not encrypt data by default.
 *
 *  Revision 2.4  2012/02/09 16:48:25  micmaz
 *  added ftps CCC  configuration parameter.
 *
 *  Revision 2.3  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/01/31 12:25:03  micmaz
 *  added a new delay method.
 *
 *  Revision 2.1  2011/10/28 13:13:43  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.14  2011/09/22 15:08:52  micmaz
 *  updated tests.
 *
 *  Revision 1.13  2011/09/13 16:31:39  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.12  2011/09/01 14:25:18  micmaz
 *  work in progress.
 *
 *  Revision 1.11  2011/08/31 15:28:42  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.10  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.9  2011/05/30 15:24:45  micmaz
 *  added localhost as valid remote host
 *
 *  Revision 1.8  2011/05/11 15:47:53  micmaz
 *  rules modifications
 *
 *  Revision 1.7  2011/05/10 10:25:23  micmaz
 *  added a while() loop to slow the reading stream in order to limit the amount of bytes in memory.
 *
 *  Revision 1.6  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.5  2011/04/07 13:29:13  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/03/23 10:04:03  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/03/11 17:55:46  micmaz
 *  reworked time outs.
 *
 *  Revision 1.2  2011/03/10 16:34:38  micmaz
 *  added new time out parameters. improved logs on failures.
 *
 *  Revision 1.1  2011/02/08 17:45:20  micmaz
 *  work in  progress...
 *
 *
 *
 *
 */

#ifndef _CurlConfigurer_H_
#define _CurlConfigurer_H_

#include <TemporaryFileService.h>
#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <RemoteSite.h>
#include <rsResourceSet.h>
#include <curl/curl.h> // PRQA S 1013
#include <string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, conf);

#if ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 16)) || (LIBCURL_VERSION_MAJOR > 7) 
#define ACS_CURL_CCC_SUPPORTED
#endif 


#if ( (LIBCURL_VERSION_MAJOR > 7) || ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 61)) )
#define EXP_UPLOAD_BUFFER_CONFIGURABLE
#else
#undef EXP_UPLOAD_BUFFER_CONFIGURABLE
#endif

/**
 * The Configurer reads from XML configuration file the following data: <p>
 * <<b>FtpCurl</b>><p>
 *    <<b>maxFilesToTransfer</b>>500<<b>/sftpPort</b>><p>
 * <<b>/FtpCurl</b>><p>
 * <p>
 * <b>maxFilesToTransfer</b>: maximum number of files to transfer in the same connection.<p>
 * <p>
 */
class CurlConfigurer { // PRQA S 2109

public:	
    CurlConfigurer();
    CurlConfigurer(CurlConfigurer const& other) = delete ;
    CurlConfigurer& operator=(CurlConfigurer const& other) = delete ;
    virtual ~CurlConfigurer() = default ;

    /**
     * A dump method in order to inspect the configuration parameters read from file.
     */
    virtual void dump() const;
    /**
     * Loads the configuration parameters from file.
     */
    virtual void load();

    static void confLoadedCallback(void *);
    static bool onceForRun() {
        return _onceForRun;
    }
    
    static void setOnceForRun(bool onceForRun) noexcept { _onceForRun = onceForRun; }
    
    // ChangeLocalPermissions class 
    class ChangeLocalPermissions { // PRQA S 2109
        friend class CurlConfigurer; // PRQA S 2107
    public:
        bool isActivated() const noexcept;
        bool isActivatedChangeOwner() const noexcept;
        bool isActivatedChangeMode() const noexcept;
        const std::string &getUserId() const noexcept;
        const std::string &getGroupId() const noexcept;
        mode_t getMode() const noexcept;
        std::string str() const noexcept;
#ifdef ACS_TEST
    public:
#else
    private:
#endif
        bool activatedChangeMode = false ;
        bool activatedChangeOwner = false ;
        std::string userId = "" ;
        std::string groupId = "" ;
        mode_t mode = DEFAULT_MODE ;
    };

#ifdef ACS_TEST
public:
#else
private:
#endif
    bool checkStatistics() const noexcept;
    static bool _onceForRun;

public:
    //Getters/Setters
    size_t getMaxFilesToTransfer() const noexcept;
    void setMaxFilesToTransfer(size_t) noexcept;
    size_t getConnectionResponseTimeout_ms() const noexcept;
    size_t getConnectionTimeout_ms() const noexcept;
    size_t getHttpConnectionResponseTimeout_ms() const noexcept;
    size_t getHttpConnectionTimeout_ms() const noexcept;
    void setConnectionResponseTimeout_ms(size_t) noexcept;
    void setConnectionTimeout_ms(size_t) noexcept;
    void setHttpConnectionResponseTimeout_ms(size_t) noexcept;
    void setHttpConnectionTimeout_ms(size_t) noexcept;
    size_t getFtpCommandResponseTimeout_s() const noexcept;
    void setFtpCommandResponseTimeout_s(size_t) noexcept;
    void setProbeConnectionTimeout_ms(size_t) noexcept;
    size_t getProbeConnectionTimeout_ms() const noexcept;
    void setLsConnectionTimeout_ms(size_t) noexcept;
    size_t getLsConnectionTimeout_ms() const noexcept;
    const std::string &getRemoteTempPath() const noexcept;
    void setRemoteTempPath(const std::string &);
    void setIsWrapDirTransfer(bool) noexcept;
    const std::string &getTempNameStrategy() const noexcept;
    void setTempNameStrategy(std::string const&);
#ifdef ACS_CURL_CCC_SUPPORTED
    curl_ftpccc getFtpsCCC() const noexcept;
    void setFtpsCCC(curl_ftpccc) noexcept;
#endif
    bool getIsFtpsProtectData() const noexcept;
    void setIsFtpsProtectData(bool) noexcept;
    bool getEnableStatistics() const noexcept;
    void setEnableStatistics(bool) noexcept; // MP: no table verification done
    std::string getSshPublicKey() const noexcept;
    void setSshPublicKet(std::string const& ) noexcept;
    std::string getSshPrivateKey() const noexcept;
    void setSshPrivateKet(std::string const& ) noexcept;
    size_t getLowSpeedLimit() const noexcept;
    void setLowSpeedLimit(size_t const ) noexcept;
    size_t getLowSpeedTime() const noexcept;
    void setLowSpeedTime(size_t const ) noexcept;
    bool changePermissions() const noexcept;
    bool isActivatedChangeMode() const noexcept;
    bool isActivatedChangeOwner() const noexcept;
    std::string getUserId() const noexcept;
    std::string getGroupId() const noexcept;
    std::string getMode() const noexcept;
    const CurlConfigurer::ChangeLocalPermissions& getPermissions() const noexcept;
    void setPermissions(const CurlConfigurer::ChangeLocalPermissions&) ;
    bool getSimulateCirculation() const noexcept ;
    void setSimulateCirculation(bool) noexcept ;
    const std::string &getLocalTempPath() const noexcept;
    void setLocalTempPath(const std::string &);
    const std::string &getHttpListStylesheetName() const noexcept;
    void setHttpListStylesheetName(const std::string &);
    const std::string &getHttpListStylesheetVersion() const noexcept;
    void setHttpListStylesheetVersion(const std::string &);
  
    bool findProxy(const std::string &host, curl::Proxy &) const ;

	// Enable the Fast-Probe mode
	bool getFastProbeModeEnabled() const noexcept;
	void setFastProbeModeEnabled(bool) noexcept;
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
    size_t getUploadBufferSize() const noexcept;
    void setUploadBufferSize(size_t const ) noexcept;
#endif  
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CurlConfigurer) ;
public:

    // PRQA S 2100 L1
    static const size_t DEFAULT_MAX_FILES_TO_TRANSFER = 500 ;
    static const size_t DEFAULT_HTTP_CONNECTION_RESPONSE_TIMEOUT_MS = 3000;
    static const size_t DEFAULT_CONNECTION_RESPONSE_TIMEOUT_MS = 3000;
    static const size_t DEFAULT_HTTP_CONNECTION_TIMEOUT_MS = 10000000;
    static const size_t DEFAULT_CONNECTION_TIMEOUT_MS = 10000000;
    static const size_t DEFAULT_FTP_CMD_RESPONSE_TIMEOUT_S = 60;
    static const size_t DEFAULT_PROBE_CONNECTION_TIMEOUT_MS = 1000 ;
    static const size_t DEFAULT_LS_CONNECTION_TIMEOUT_MS = 15000 ;
    static const bool DEFAULT_FTPS_PROTECT_DATA = false ;
    static const bool DEFAULT_ENABLE_STATISTICS = false ;
    static const std::string DEFAULT_REMOTE_TEMP_PATH;
#ifdef ACS_CURL_CCC_SUPPORTED
    static const std::string DEFAULT_FTPS_USE_CCC;
    static const std::string FTPS_USE_CCC_ACTIVE;
    static const std::string FTPS_USE_CCC_PASSIVE;
    static const std::string FTPS_USE_CCC_NONE;
#endif
    static const std::string CURL_CONFIGURER_SPACE_NAME;
    static const std::string DEFAULT_SSH_PUBLIC_KEY;
    static const std::string DEFAULT_SSH_PRIVATE_KEY;
    static const size_t DEFAULT_LOW_SPEED_LIMIT = 0 ;
    static const size_t DEFAULT_LOW_SPEED_TIME = 0 ;
    static const mode_t DEFAULT_MODE = 02770 ; // octal
    static const std::string defaultLocalTempPath;
    static const std::string defaultHttpListStylesheetName;
    static const std::string defaultHttpListStylesheetVersion;
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
    static const size_t EXP_UPLOAD_BUFFER_SIZE = (64*1024) ; // 64K
#endif
    // PRQA L:L1
private:
    size_t _maxFilesToTransfer              = DEFAULT_MAX_FILES_TO_TRANSFER ;
    size_t _connectionTimeout_ms            = DEFAULT_CONNECTION_TIMEOUT_MS ;
    size_t _httpConnectionTimeout_ms        = DEFAULT_HTTP_CONNECTION_TIMEOUT_MS ;
    size_t _probeConnectionTimeout_ms       = DEFAULT_PROBE_CONNECTION_TIMEOUT_MS ;
    size_t _lsConnectionTimeout_ms          = DEFAULT_LS_CONNECTION_TIMEOUT_MS ;
    size_t _connectionResponseTimeout_ms    = DEFAULT_CONNECTION_RESPONSE_TIMEOUT_MS ;
    size_t _httpConnectionResponseTimeout_ms= DEFAULT_HTTP_CONNECTION_RESPONSE_TIMEOUT_MS ;
	size_t _ftpCommandResponseTimeout_s     = DEFAULT_FTP_CMD_RESPONSE_TIMEOUT_S ;
    std::string _remoteTempPath             = DEFAULT_REMOTE_TEMP_PATH ;
    std::string _tempNameStrategy           = curl::temp::TemporaryFileService::get_DEFAULT_TEMP_NAME_STRATEGY() ;
#ifdef ACS_CURL_CCC_SUPPORTED
    curl_ftpccc _ftpsCCC                    = CURLFTPSSL_CCC_PASSIVE ; // Clear Command Channel
#endif
    bool _ftpsProtectData                   = DEFAULT_FTPS_PROTECT_DATA ;
    mutable bool _enableStatistics          = DEFAULT_ENABLE_STATISTICS ;
    std::string _sshPublicKey               = DEFAULT_SSH_PUBLIC_KEY ;
    std::string _sshPrivateKey              = DEFAULT_SSH_PRIVATE_KEY ;
    size_t _lowSpeedLimit                   = DEFAULT_LOW_SPEED_LIMIT ;
    size_t _lowSpeedTime                    = DEFAULT_LOW_SPEED_TIME ;
    ChangeLocalPermissions _permissions     = {} ;
    mutable bool _statisticsCheckGood       = false  ;
    bool _simulateCirculation               = false ;
    std::map<std::string, curl::Proxy, std::less<> > _proxies = {} ;
	std::string _localTempPath              = defaultLocalTempPath ;
	std::string _httpListStylesheetName     = defaultHttpListStylesheetName ;
	std::string _httpListStylesheetVersion  = defaultHttpListStylesheetVersion ;
	bool _fastProbeModeEnabled              = false ;
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
    size_t _uploadBufferSize                = EXP_UPLOAD_BUFFER_SIZE ; // range is 16K - 2M 
#endif
};

using CurlConfigurerSingleton = pattern::Singleton<CurlConfigurer> ;

_ACS_END_NESTED_NAMESPACE

#endif /* _CurlConfigurer_H_ */
