// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2021, Exprivia SpA
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.exprivia.com
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Exprivia SpA
 *
 *  $Prod: A.C.S. FtpCurl Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.26  2017/10/19 15:14:20  marpas
 *  qa warning removed
 *  code best practice implemented
 *
 *  Revision 5.25  2017/10/16 16:51:56  marpas
 *  findProxy signature improved
 *
 *  Revision 5.24  2017/07/26 07:42:16  lucio.pulvirenti
 *  S1PDGS-31936: Optional configuration for timeout to use for remote dir list introduced along with get and set methods.
 *
 *  Revision 5.23  2017/05/05 16:09:53  lucio.pulvirenti
 *  S3PDGS-3137: ftp command response timeout added (default 60 s) along with get and set methods.
 *
 *  Revision 5.22  2017/04/11 16:30:57  marfav
 *  Added fast probe resource configuration flag
 *
 *  Revision 5.21  2016/03/26 21:42:30  marpas
 *  using new db I/F
 *
 *  Revision 5.20  2015/11/27 09:08:23  marpas
 *  findProxy signals outside if the returned proxy was found in the configuration or generate inside the moethod
 *
 *  Revision 5.19  2015/04/23 11:57:50  marpas
 *  qa warning silenced
 *
 *  Revision 5.18  2014/11/13 13:56:15  lucio.pulvirenti
 *  S2PDGS-915: _localTempPath, _httpListStylesheetName and _httpListStylesheetVersion attribute added to IF with get and set methods.
 *
 *  Revision 5.17  2014/10/30 10:32:32  marpas
 *  qa warnings removed
 *
 *  Revision 5.16  2014/10/29 13:13:04  lucio.pulvirenti
 *  S2PDGS-955: List of proxies, if any, got from configuration with the proper new method. getProxy method implemented to return the proxy, if any, corresponding to given host.
 *
 *  Revision 5.15  2014/10/28 16:31:43  lucio.pulvirenti
 *  S2PDGS-955 work in progress.
 *
 *  Revision 5.14  2014/07/03 07:42:17  marpas
 *  measurement unit added to message
 *
 *  Revision 5.13  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.12  2014/02/13 19:15:22  marpas
 *  TemporaryFileService interface changed
 *
 *  Revision 5.11  2014/02/07 18:47:33  marpas
 *  adopting ACS_THROW and ACS_COND_THRO macros
 *
 *  Revision 5.10  2013/12/10 18:49:05  marpas
 *  qa warnings and code simplified
 *
 *  Revision 5.9  2013/11/21 14:45:22  marpas
 *  statistics in case of local files - media too
 *
 *  Revision 5.8  2013/11/04 15:41:51  marpas
 *  simulation mode added to conf
 *
 *  Revision 5.7  2013/11/04 11:37:18  marpas
 *  fixing configuration load
 *
 *  Revision 5.6  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.5  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.4  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/06/18 12:54:14  marpas
 *  debug macros rationalized
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.49  2013/05/02 12:45:12  marpas
 *  dump method uses exFPrivateInfo instead of exFMessage
 *  static definitions for CCC commented out
 *
 *  Revision 2.48  2013/04/17 20:39:35  marpas
 *  robustness improved
 *
 *  Revision 2.47  2013/04/15 09:12:34  marpas
 *  coding best practices applied
 *  interfaces rationalization and simplification
 *
 *  Revision 2.46  2013/04/12 15:43:30  marpas
 *  fixing a bug in default permission
 *
 *  Revision 2.45  2013/04/12 15:34:21  marpas
 *  fixing default permission bug
 *
 *  Revision 2.44  2013/04/12 12:05:48  marpas
 *  statistics enabling logic fixed
 *
 *  Revision 2.43  2013/03/22 13:25:05  marpas
 *  mode is an integral, not a string !
 *   ----------------------------------------------------------------------
 *
 *  Revision 2.42  2013/03/14 15:29:28  marpas
 *  dbConnectionPool use improved
 *
 *  Revision 2.41  2013/01/25 13:06:46  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 2.40  2013/01/24 13:40:14  marpas
 *  uses connection pool from app (if any)
 *
 *  Revision 2.39  2012/09/21 16:41:09  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 2.38  2012/09/21 15:25:02  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 2.37  2012/09/21 13:34:28  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 2.36  2012/09/12 12:45:04  micmaz
 *  for default the low speed limit is disabled.
 *
 *  Revision 2.35  2012/09/06 15:55:24  micmaz
 *  added two new paramters: low speed time and low speed limit in order to stop a low transfer.
 *
 *  Revision 2.34  2012/06/25 12:54:10  lucio.pulvirenti
 *  removed check statistics if disabled and removed use of certificates for
 *  old cURL versions.
 *
 *  Revision 2.33  2012/06/19 13:27:31  micmaz
 *  libFtpCurl now can transfer files without password.
 *
 *  Revision 2.32  2012/06/18 16:51:57  micmaz
 *  removed warning.
 *
 *  Revision 2.31  2012/06/18 13:21:26  micmaz
 *  improved check of statistics
 *
 *  Revision 2.30  2012/06/18 13:18:48  micmaz
 *  improved check of statistics.
 *
 *  Revision 2.29  2012/04/24 14:11:02  micmaz
 *  the configuration now tests if the curl statistics exists.
 *
 *  Revision 2.28  2012/03/29 13:34:05  micmaz
 *  removed conf parameter
 *
 *  Revision 2.27  2012/03/28 15:22:25  micmaz
 *  work in progress
 *
 *  Revision 2.26  2012/03/28 15:03:58  micmaz
 *  work in progress
 *
 *  Revision 2.25  2012/03/28 14:16:02  micmaz
 *  work in progress
 *
 *  Revision 2.24  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.23  2012/03/27 10:52:22  micmaz
 *  work in progress
 *
 *  Revision 2.22  2012/03/15 18:17:18  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.21  2012/03/15 17:44:28  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.20  2012/03/15 16:49:13  micmaz
 *  work in progress
 *
 *  Revision 2.19  2012/03/15 11:45:27  micmaz
 *  work in progress
 *
 *  Revision 2.18  2012/03/15 09:55:22  marpas
 *  no longer use of ConfigurerAdder
 *
 *  Revision 2.17  2012/03/09 17:43:55  micmaz
 *  added media functionality
 *
 *  Revision 2.15  2012/03/08 11:21:39  marpas
 *  const must be defined as variables not DEFINE
 *
 *  Revision 2.14  2012/03/08 10:52:11  marpas
 *  fixing message notification
 *
 *  Revision 2.13  2012/02/28 17:49:21  micmaz
 *  removed wrong headers.
 *
 *  Revision 2.12  2012/02/28 17:19:10  micmaz
 *  refactory Data circulator
 *
 *  Revision 2.11  2012/02/17 16:25:18  micmaz
 *  added compatibility with old curl versions.
 *
 *  Revision 2.10  2012/02/10 17:41:12  micmaz
 *  now the ftps connection will not encrypt data by default.
 *
 *  Revision 2.9  2012/02/09 17:13:04  micmaz
 *  CCC passive as default
 *
 *  Revision 2.8  2012/02/09 17:12:15  micmaz
 *  CCC passive as default
 *
 *  Revision 2.7  2012/02/09 16:48:25  micmaz
 *  added ftps CCC  configuration parameter.
 *
 *  Revision 2.6  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.5  2012/01/31 12:25:03  micmaz
 *  added a new delay method.
 *
 *  Revision 2.4  2012/01/23 16:21:06  micmaz
 *  IN PROGRESS - issue S1PDGS-2735: PolicyGenerator: t_circulationusers population
 *  http://jira.acsys.it/browse/S1PDGS-2735
 *
 *  Revision 2.3  2012/01/20 16:05:14  micmaz
 *  modified logs
 *
 *  Revision 2.2  2011/10/28 15:24:44  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 2.1  2011/10/28 13:13:43  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.23  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.22  2011/09/22 15:08:52  micmaz
 *  updated tests.
 *
 *  Revision 1.21  2011/09/21 15:30:44  micmaz
 *  work in progress
 *
 *  Revision 1.20  2011/09/19 15:45:55  micmaz
 *  error in print messages.
 *
 *  Revision 1.19  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.18  2011/09/13 16:31:39  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.17  2011/09/01 14:25:18  micmaz
 *  work in progress.
 *
 *  Revision 1.16  2011/08/31 15:28:42  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.15  2011/08/26 15:52:04  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.14  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.13  2011/07/05 11:47:27  micmaz
 *  work in progress
 *
 *  Revision 1.12  2011/05/30 15:24:45  micmaz
 *  added localhost as valid remote host
 *
 *  Revision 1.11  2011/05/11 15:47:53  micmaz
 *  rules modifications
 *
 *  Revision 1.10  2011/05/10 10:25:23  micmaz
 *  added a while() loop to slow the reading stream in order to limit the amount of bytes in memory.
 *
 *  Revision 1.9  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.8  2011/04/26 17:33:23  micmaz
 *  work in progress..
 *
 *  Revision 1.7  2011/04/15 18:01:33  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/04/07 13:29:13  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/04/01 14:43:55  micmaz
 *  implemented the Circulations WP
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

#include <CurlConfigurer.h>
#include <CurlErrorBehaviourSingleton.h>

#include <ConfigurationSingleton.h>

#include <TemporaryFileService.h>
#include <StringUtils.h>


#include <exException.h>

#include <rsPushNSpace.h>

#include <Password.h>
#include <DateTime.h>
#include <StringUtils.h>
#include <dbSchemaMetaDataProvider.h>
#include <CirculationStatistics.h>
#include <Application.h>
#include <dbConnectionPool.h>
#include <boost/algorithm/string.hpp> // PRQA S 1013 

#include <curl/curl.h> // PRQA S 1013 
#include <unistd.h>


#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
#warning libcurl >= 7.61.1 configurable buffer support enabled
#else
#warning libcurl < 7.61.1 configurable buffer support DISBALED
#endif


namespace {
    struct ConfProxy { // PRQA S 2171, 2173, 2175
    public:
        ConfProxy() : hostname(), proxy() {}
         ~ConfProxy() throw() {}
        ConfProxy(const ConfProxy &r) : hostname(r.hostname), proxy(r.proxy) {}
        ConfProxy& operator= (const ConfProxy &r) {
            if (this != &r) {
                hostname = r.hostname ; 
                proxy = r.proxy ; 
            }
            return *this ; 
        }

        std::string hostname ; // PRQA S 2100 2
        acs::curl::Proxy proxy ;    
    } ;

}

namespace acs {
    template<>
    inline
    void rsResourceSet::getValue(const std::string &res, ConfProxy &v) const // PRQA S 2133, 2134
    {
        Lock lock(_mutex) ;
 	    getValue(res+".targethost")->get(v.hostname) ;
 	    getValue(res+".proxy", v.proxy) ;
    }

} // end acs namespace 
_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)

using acs::curl::temp::TemporaryFileService;
using namespace std;
using namespace boost;
using namespace acs::curl;
using namespace acs::curl::temp;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CurlConfigurer);

bool CurlConfigurer::_onceForRun = true;
/*
const size_t CurlConfigurer::DEFAULT_MAX_FILES_TO_TRANSFER = 500;
const size_t CurlConfigurer::DEFAULT_CONNECTION_RESPONSE_TIMEOUT = 3000;
const size_t CurlConfigurer::DEFAULT_CONNECTION_TIMEOUT = 10000000;
const size_t CurlConfigurer::DEFAULT_PROBE_CONNECTION_TIMEOUT = 1000;
const bool CurlConfigurer::DEFAULT_FTPS_PROTECT_DATA = false;
const bool CurlConfigurer::DEFAULT_ENABLE_STATISTICS = false ;
*/
const std::string CurlConfigurer::CURL_CONFIGURER_SPACE_NAME = "libFtpCurl.so";
#ifdef ACS_CURL_CCC_SUPPORTED
const std::string CurlConfigurer::FTPS_USE_CCC_NONE = "NONE";
const std::string CurlConfigurer::FTPS_USE_CCC_PASSIVE = "PASSIVE";
const std::string CurlConfigurer::FTPS_USE_CCC_ACTIVE = "ACTIVE";
const std::string CurlConfigurer::DEFAULT_FTPS_USE_CCC = FTPS_USE_CCC_PASSIVE;
#endif
const std::string CurlConfigurer::DEFAULT_REMOTE_TEMP_PATH = "_TEMP_DIR";
const std::string CurlConfigurer::DEFAULT_SSH_PUBLIC_KEY= "$HOME/.ssh/id_rsa.pub";
const std::string CurlConfigurer::DEFAULT_SSH_PRIVATE_KEY= "$HOME/.ssh/id_rsa";
const size_t CurlConfigurer::DEFAULT_LOW_SPEED_LIMIT = 0;
const size_t CurlConfigurer::DEFAULT_LOW_SPEED_TIME = 0;
const mode_t CurlConfigurer::DEFAULT_MODE= 02770 ; // octal
const std::string CurlConfigurer::defaultLocalTempPath = "/tmp";
const std::string CurlConfigurer::defaultHttpListStylesheetName = "HTTP_FILES_LIST_1";
const std::string CurlConfigurer::defaultHttpListStylesheetVersion = "1.0";

void CurlConfigurer::confLoadedCallback(void *cp) // PRQA S 4020
{
    if (!onceForRun() || !cp) {
        return;
    }
    setOnceForRun(false);
    CurlConfigurer* c = reinterpret_cast<CurlConfigurer *>(cp); // PRQA S 3081
    c->load();
    c->dump();
}

CurlConfigurer::CurlConfigurer() :
    _maxFilesToTransfer(DEFAULT_MAX_FILES_TO_TRANSFER), 
    _connectionTimeout_ms(DEFAULT_CONNECTION_TIMEOUT_MS), 
    _httpConnectionTimeout_ms(DEFAULT_HTTP_CONNECTION_TIMEOUT_MS), 
    _probeConnectionTimeout_ms(DEFAULT_PROBE_CONNECTION_TIMEOUT_MS), 
    _lsConnectionTimeout_ms(DEFAULT_LS_CONNECTION_TIMEOUT_MS), 
    _connectionResponseTimeout_ms(DEFAULT_CONNECTION_RESPONSE_TIMEOUT_MS),
    _httpConnectionResponseTimeout_ms(DEFAULT_HTTP_CONNECTION_RESPONSE_TIMEOUT_MS),
	_ftpCommandResponseTimeout_s(DEFAULT_FTP_CMD_RESPONSE_TIMEOUT_S),	
    _remoteTempPath(DEFAULT_REMOTE_TEMP_PATH), 
    _tempNameStrategy(TemporaryFileService::get_DEFAULT_TEMP_NAME_STRATEGY()), 
#ifdef ACS_CURL_CCC_SUPPORTED
    _ftpsCCC(CURLFTPSSL_CCC_PASSIVE), 
#endif
    _ftpsProtectData(DEFAULT_FTPS_PROTECT_DATA), 
    _enableStatistics(DEFAULT_ENABLE_STATISTICS),
    _sshPublicKey(DEFAULT_SSH_PUBLIC_KEY), 
    _sshPrivateKey(DEFAULT_SSH_PRIVATE_KEY),
    _lowSpeedLimit(DEFAULT_LOW_SPEED_LIMIT),
    _lowSpeedTime(DEFAULT_LOW_SPEED_TIME), 
    _permissions(),
    _statisticsCheckGood(false),
    _simulateCirculation(false),
    _proxies(),
	_localTempPath(defaultLocalTempPath),
	_httpListStylesheetName(defaultHttpListStylesheetName),
	_httpListStylesheetVersion(defaultHttpListStylesheetVersion),
	_fastProbeModeEnabled(false)
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
    ,_uploadBufferSize(EXP_UPLOAD_BUFFER_SIZE)
#endif
{
    os::Password::passwd_w pw ;
    try {
        pw = os::Password::getPwUid(getuid()) ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    string homedir = pw.pw_dir;
    replace_first(_sshPublicKey, "$HOME", homedir );
    replace_first(_sshPrivateKey, "$HOME", homedir );
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace(CURL_CONFIGURER_SPACE_NAME)); // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(confLoadedCallback, this);
}

CurlConfigurer::~CurlConfigurer() throw () {
    //empty.
}

#ifdef ACS_CURL_CCC_SUPPORTED
namespace {
    std::string toString(curl_ftpccc m) { // PRQA S 2502, 4020
        switch(m) {
        case CURLFTPSSL_CCC_NONE: { return CurlConfigurer::FTPS_USE_CCC_NONE ; }
        case CURLFTPSSL_CCC_PASSIVE: { return CurlConfigurer::FTPS_USE_CCC_PASSIVE ; }
        case CURLFTPSSL_CCC_ACTIVE: { return CurlConfigurer::FTPS_USE_CCC_ACTIVE ; }
        default: { return "Unrecognized" ; }
        }
    }
}
#endif
    
void CurlConfigurer::dump() const 
{
    //A simple debug function used to print out the most important configuration parameters read.
    ACS_LOG_PRIVINFO("\n"
       << left << setw(50) << "--------------------------------------------  " << "\n"
       << left << setw(50) << "cURL configuration status (times in [ms])     " << "\n"
       << left << setw(50) << "--------------------------------------------  " << "\n"
       << left << setw(50) << "Simulate circulations:" << std::right << setw(30) << boolalpha << getSimulateCirculation() << "\n"
       << left << setw(50) << "Maximum number of files to transfer:" << std::right << setw(30) << getMaxFilesToTransfer() << "\n"
       << left << setw(50) << "Connection response timeout [ms]:" << std::right << setw(30) << getConnectionResponseTimeout_ms() << "\n"
       << left << setw(50) << "Connection timeout [ms]:" << std::right << setw(30) << getConnectionTimeout_ms() << "\n"
       << left << setw(50) << "Http connection response timeout [ms]:" << std::right << setw(30) << getHttpConnectionResponseTimeout_ms() << "\n"
       << left << setw(50) << "Http connection timeout [ms]:" << std::right << setw(30) << getHttpConnectionTimeout_ms() << "\n"
       << left << setw(50) << "Ftp command response timeout [s]:" << std::right << setw(30) << getFtpCommandResponseTimeout_s() << '\n'
       << left << setw(50) << "Probe connection timeout [ms]:" << std::right << setw(30) << getProbeConnectionTimeout_ms() << "\n"
       << left << setw(50) << "ls connection timeout [ms]:" << std::right << setw(30) << getLsConnectionTimeout_ms() << "\n"
       << left << setw(50) << "Remote Temp path:" << std::right << setw(30) << getRemoteTempPath() << "\n"
       << left << setw(50) << "Local Temp path:" << std::right << setw(30) << getLocalTempPath() << "\n"
       << left << setw(50) << "Temporary name strategy:" << std::right << setw(30) << getTempNameStrategy() << "\n"
#ifdef ACS_CURL_CCC_SUPPORTED
       << left << setw(50) << "Ftps Connections CCC Modality:" << std::right << setw(30) << toString(getFtpsCCC()) << "\n"
#endif       
#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
       << left << setw(50) << "Upload buffer size [b]:" << std::right << setw(30) << getUploadBufferSize() << "\n"
#else
       << left << setw(50) << "Upload buffer size [b]:" << std::right << setw(30) << "no configuration support" << "\n"
#endif
       << left << setw(50) << "Ftps Protect data:" << std::right << setw(30) << getIsFtpsProtectData() << "\n"
       << left << setw(50) << "Enable Statistics:" << std::right << setw(30) << getEnableStatistics() << "\n"
       << left << setw(50) << "SSH Public Key:" << std::right << setw(30) << getSshPublicKey()<< "\n"
       << left << setw(50) << "SSH Private Key:" << std::right << setw(30) << getSshPrivateKey() << "\n"
       << left << setw(50) << "Low Speed Limit:" << std::right << setw(30) << getLowSpeedLimit() << "\n"
       << left << setw(50) << "Low Speed Time:" << std::right << setw(30) << getLowSpeedTime() << "\n"
       << left << setw(50) << "Change Owner:" << std::right << setw(30) << _permissions.activatedChangeOwner << "\n"
       << left << setw(50) << "Change Mode:" << std::right << setw(30) << _permissions.activatedChangeMode << "\n"
       << left << setw(50) << "     UserId:" << std::right << setw(30) << _permissions.userId << "\n"
       << left << setw(50) << "    GroupId:" << std::right << setw(30) << _permissions.groupId << "\n"
       << left << setw(50) << "       Mode:" << std::right << setw(30) << std::oct << _permissions.mode << "\n"
       << left << setw(50) << "Http list stylesheet name:" << std::right << setw(30) << getHttpListStylesheetName() << '\n'
       << left << setw(50) << "Http list stylesheet version:" << std::right << setw(30) << getHttpListStylesheetVersion() << '\n'
       << left << setw(50) << "--------------------------------------------") ;
}

namespace {

template <typename T>
bool getValueIfHasKey(rsResourceSet &rs, const std::string &k, T &v) // PRQA S 4020
{
    if (rs.hasKey(k)) {
        rs.getValue(k, v);
        return true ;
    }
    return false ;
}

}

// Due the nature of the method, it will be long and also with a bit of complexity
// So we silent too complex (5500) and too long (5510) qa warnings hereafter
void CurlConfigurer::load() // PRQA S 5500, 5510
{
    rsResourceSet& resultSet = ConfigurationSingleton::instance()->get();
    //Reads the FtpCurl section.
    rsPushNSpace FTPExportDataProviderNS(resultSet, "FtpCurl");

    try {
		const string listOfProxiesNode("List_Of_Proxies");
		if( resultSet.hasKey(listOfProxiesNode) ) {
            vector<ConfProxy> proxies ;
            resultSet.getArray(listOfProxiesNode+".Proxy", proxies) ;
            size_t p_size = proxies.size() ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, p_size << " proxie(s) loaded.");
            for(size_t i=0; i < p_size; ++i) {
                _proxies[proxies[i].hostname] = proxies[i].proxy ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, proxies[i].hostname << " -> " << proxies[i].proxy );
            }
		}
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

    try {
        rsPushNSpace permissionsNS(resultSet, "ChangeLocalPermissions");
        try {
            rsPushNSpace changeOwnNS(resultSet, "ChangeOwn");
            // getValue with 3 parameters allows a default value in case the key does not exists
            resultSet.getValue ("Activate", _permissions.activatedChangeOwner, false);

            if (not getValueIfHasKey(resultSet, "User",_permissions.userId)) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User id not found, using the process owner user");
            }

            if (not getValueIfHasKey(resultSet, "Group",_permissions.groupId)) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Group id not found, using the process owner group");
            }
        } 
        catch (std::exception const& ex) {
            // should not happen.. disable the feature in case of strange issues
            ACS_LOG_NOTIFY_EX (ex);
            ACS_LOG_WARNING ("Got an issue while reading change owner configuration. Disabling it");
            _permissions.activatedChangeOwner = false;
        }
        try {
            rsPushNSpace changeOwnNS(resultSet, "ChangeMod");
            resultSet.getValue("Activate", _permissions.activatedChangeMode, false);

            ostringstream defaultmode; 
            string mode;
            // Compute default mode in octal
            defaultmode << std::oct << DEFAULT_MODE ;
            resultSet.getValue("Mode", mode, defaultmode.str().c_str());
            std::istringstream iss(mode) ;
            iss.exceptions ( std::istringstream::failbit); // generate exception if fail
            iss >> std::oct >> _permissions.mode ;
        }
        catch (std::exception const& ex) {
            // Decodification of the mode failed, disable mode change
            ACS_LOG_NOTIFY_EX (ex);
            ACS_LOG_WARNING ("Got an issue while reading change mode configuration. Disabling it");
            _permissions.activatedChangeMode = false;
        }
    } 
    catch (std::exception const& ex) {
        // the local permissions must not be changed.
        ACS_LOG_NOTIFY_EX (ex);
        ACS_LOG_WARNING ("Got an issue while reading cconfiguration. Disabling change mode and owner features.");
        _permissions.activatedChangeMode = _permissions.activatedChangeOwner= false;
    }
    
    try {
		rsPushNSpace curlErrorTypesNS(resultSet, "CURLErrorBehaviours");
		if (resultSet.hasKey("CurlError")) {
			std::stringstream ss;
			ss << "CurlError";
			size_t arraySize = resultSet.getArraySize(ss.str());
			for (size_t idx = 0; idx < arraySize; ++idx) {
				std::stringstream subtag;
				subtag << ss.str();
				if (arraySize > 1) {
					subtag << "[" << idx << "]";
				}
				
				std::string codestr, behstr;
				try {
					resultSet.getValue<std::string>(subtag.str() + ".Code", codestr);
					resultSet.getValue<std::string>(subtag.str() + ".Behaviour", behstr);
					if (CurlErrorBehaviourSingleton::instance()->exists(codestr)) {
						if (!CurlErrorBehaviourSingleton::instance()->setBehaviour(codestr, CurlErrorBehaviourSingleton::string2CurlErrorBehaviour(behstr))) {
							ACS_LOG_WARNING("Curl error " << codestr << " cannot be configured with behaviour " << behstr << ".");
						} else {
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Behavior for curl error " << codestr << " set to " << behstr);
						}
					} else {
						ACS_LOG_WARNING("Curl error " << codestr << " does not exists and cannot be configured.");
					}
				} catch (std::exception& e) {
					ACS_LOG_NOTIFY_EX(e);
				}
			}
		}
		
	} catch (std::exception const& e) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CURLErrorCodes not found, using default");
	}
    
    
    //Reads the maximum number of files to transfer in a single transaction.
    if (not getValueIfHasKey(resultSet, "maxFilesToTransfer",_maxFilesToTransfer)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "maxFilesToTransfer not found, using default");
    }

    //Simulate circulation.
    if (not getValueIfHasKey(resultSet, "SimulateCirculation",_simulateCirculation)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SimulateCirculation not found, using default");
    }

    //Fast probe mdode
    if (not getValueIfHasKey(resultSet, "FastProbeModeEnabled",_fastProbeModeEnabled)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FastProbeModeEnabled not found, using default");
    }

    //Enable Statistics
    if (not getValueIfHasKey(resultSet, "enableStatistics",_enableStatistics)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "enableStatistics not found, using default");
    }

    //Reads the connection response time out.
    if (not getValueIfHasKey(resultSet, "connectionResponseTimeout",_connectionResponseTimeout_ms)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "connectionResponseTimeout not found, using default");
    }
    //Reads the http connection response time out.
    if (not getValueIfHasKey(resultSet, "httpConnectionResponseTimeout",_httpConnectionResponseTimeout_ms)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "httpConnectionResponseTimeout not found, using default");
    }
    //Reads the connection time out.
    if (not getValueIfHasKey(resultSet, "connectionTimeout",_connectionTimeout_ms)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "connectionTimeout not found, using default");
    }
    //Reads the http connection time out.
    if (not getValueIfHasKey(resultSet, "httpConnectionTimeout",_httpConnectionTimeout_ms)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "httpConnectionTimeout not found, using default");
    }
	//Reads the ftp command response time out.
    if (not getValueIfHasKey(resultSet, "ftpCommandResponseTimeout", _ftpCommandResponseTimeout_s)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftpCommandResponseTimeout not found, using default");
    }

    //Reads the connection time out used to probe the remote sites.
    if (not getValueIfHasKey(resultSet, "probeConnectionTimeout",_probeConnectionTimeout_ms)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "probeConnectionTimeout not found, using default");
    }
	   
	//Reads the connection time out used to ls the remote sites directory.
    if (not getValueIfHasKey(resultSet, "lsConnectionTimeout",_lsConnectionTimeout_ms)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "lsConnectionTimeout not found, using default");
    }

    //Reads the remote temporary path used to store the incomplete files.
    if (not getValueIfHasKey(resultSet, "remoteTempPath",_remoteTempPath)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "remoteTempPath not found, using default");
    }

	//Reads the local temporary path
    if (not getValueIfHasKey(resultSet, "localTempPath",_localTempPath)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "localTempPath not found, using default");
    }

	// Read the name of the stylesheet to convert files list file from http server.
    if (not getValueIfHasKey(resultSet, "httpListStylesheetName",_httpListStylesheetName)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "httpListStylesheetName not found, using default");
    }

	// Read the version of the stylesheet to convert files list file from http server.
    if (not getValueIfHasKey(resultSet, "httpListStylesheetVersion",_httpListStylesheetVersion)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "httpListStylesheetVersion not found, using default");
    }

    //Reads the temp name strategy.
    if (resultSet.hasKey("tempNameStrategy")) {
        resultSet.getValue("tempNameStrategy", _tempNameStrategy);
        if (!StringUtils::equalsNoCase(TemporaryFileService::PREFIX_NAME_STRATEGY, _tempNameStrategy)
                && !StringUtils::equalsNoCase(TemporaryFileService::SUFFIX_NAME_STRATEGY, _tempNameStrategy)
                && !StringUtils::equalsNoCase(TemporaryFileService::TEMP_DIR_NAME_STRATEGY, _tempNameStrategy)
		&& !StringUtils::equalsNoCase(TemporaryFileService::TIMESTAMP_NAME_STRATEGY, _tempNameStrategy)
                && !StringUtils::equalsNoCase(TemporaryFileService::NO_NAME_STRATEGY, _tempNameStrategy)) {
            setTempNameStrategy(TemporaryFileService::get_DEFAULT_TEMP_NAME_STRATEGY());
        }
    } 
    else {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "tempNameStrategy not found, using default");
    }

    if (not getValueIfHasKey(resultSet, "ftpsProtData",_ftpsProtectData)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftpsProtData not found, using default");
    }
#ifdef ACS_CURL_CCC_SUPPORTED
    if (resultSet.hasKey("ftpsCCC")) {
        string cccModality;
        resultSet.getValue("ftpsCCC", cccModality);
        if (StringUtils::equalsNoCase(cccModality, FTPS_USE_CCC_ACTIVE)) {
            _ftpsCCC = CURLFTPSSL_CCC_ACTIVE;
        } 
        else {
            if (StringUtils::equalsNoCase(cccModality, FTPS_USE_CCC_PASSIVE)) {
                _ftpsCCC = CURLFTPSSL_CCC_PASSIVE;
            } 
            else {
                if (StringUtils::equalsNoCase(cccModality, FTPS_USE_CCC_NONE)) {
                    _ftpsCCC = CURLFTPSSL_CCC_NONE;
                } 
                else {
                    ACS_LOG_ERROR("ftps CCC Modality unrecognized! Use default " << DEFAULT_FTPS_USE_CCC);
                }
            }
        }
    } 
    else {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftpsUseCCC not found, using default");
    }
#endif

    if (not getValueIfHasKey(resultSet, "sshPublicKey",_sshPublicKey)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sshPublicKey not found, using default");
    }

    if (not getValueIfHasKey(resultSet, "sshPrivateKey",_sshPrivateKey)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sshPrivateKey not found, using default");
    }

    if (not getValueIfHasKey(resultSet, "lowSpeedLimit",_lowSpeedLimit)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "lowSpeedLimit not found, using default");
    }

    if (not getValueIfHasKey(resultSet, "lowSpeedTime",_lowSpeedTime)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "lowSpeedTime not found, using default");
    }
    os::Password::passwd_w pw ;
    try {
        pw = os::Password::getPwUid(getuid()) ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    string homedir = pw.pw_dir;
    replace_first(_sshPublicKey, "$HOME", homedir);
    replace_first(_sshPrivateKey, "$HOME", homedir);

    { 
        size_t ubs ;

#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
        if (not getValueIfHasKey(resultSet, "uploadBufferSize",ubs)) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "uploadBufferSize not found, using default");
        }
        else { 
            setUploadBufferSize(ubs) ; 
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "uploadBufferSize set to: " << getUploadBufferSize());
        } // range check inside the setter
#else
        if (getValueIfHasKey(resultSet, "uploadBufferSize",ubs)) {
            ACS_LOG_WARNING("uploadBufferSize will not be honored.");
        }
#endif
    }
}

//
//Getters/Setters section
//

const string &CurlConfigurer::getRemoteTempPath() const throw () // PRQA S 4120
{
    return _remoteTempPath;
}

void CurlConfigurer::setRemoteTempPath(const std::string &v)  // PRQA S 4121
{
    _remoteTempPath = v ;
}

size_t CurlConfigurer::getMaxFilesToTransfer() const throw ()  // PRQA S 4120
{
    return _maxFilesToTransfer;
}

size_t CurlConfigurer::getConnectionResponseTimeout_ms() const throw ()   // PRQA S 4120
{
    return _connectionResponseTimeout_ms ;
}

size_t CurlConfigurer::getHttpConnectionResponseTimeout_ms() const throw ()   // PRQA S 4120
{
    return _httpConnectionResponseTimeout_ms ;
}

size_t CurlConfigurer::getConnectionTimeout_ms() const throw ()  // PRQA S 4120
{
    return _connectionTimeout_ms;
}

size_t CurlConfigurer::getHttpConnectionTimeout_ms() const throw ()  // PRQA S 4120
{
    return _httpConnectionTimeout_ms;
}

size_t CurlConfigurer::getProbeConnectionTimeout_ms() const throw () // PRQA S 4120
{
    return _probeConnectionTimeout_ms;
}

size_t CurlConfigurer::getLsConnectionTimeout_ms() const throw () // PRQA S 4120
{
    return _lsConnectionTimeout_ms;
}

void CurlConfigurer::setConnectionResponseTimeout_ms(size_t to) throw ()  // PRQA S 4121
{
    _connectionResponseTimeout_ms = to;
}

void CurlConfigurer::setConnectionTimeout_ms(size_t to) throw ()  // PRQA S 4121
{
    _connectionTimeout_ms = to;
}

void CurlConfigurer::setHttpConnectionTimeout_ms(size_t to) throw ()  // PRQA S 4121
{
    _httpConnectionTimeout_ms = to;
}

size_t CurlConfigurer::getFtpCommandResponseTimeout_s() const throw () // PRQA S 4120
{
	return _ftpCommandResponseTimeout_s;
}
void CurlConfigurer::setFtpCommandResponseTimeout_s(size_t to) throw () // PRQA S 4121
{
	_ftpCommandResponseTimeout_s = to;
}

void CurlConfigurer::setProbeConnectionTimeout_ms(size_t to) throw () // PRQA S 4121
{
    _probeConnectionTimeout_ms = to;
}

void CurlConfigurer::setLsConnectionTimeout_ms(size_t to) throw () // PRQA S 4121
{
    _lsConnectionTimeout_ms = to;
}

void CurlConfigurer::setMaxFilesToTransfer(size_t maxFilesToTransfer) throw () // PRQA S 4121
{
    _maxFilesToTransfer = maxFilesToTransfer;
}

const string &CurlConfigurer::getTempNameStrategy() const throw ()  // PRQA S 4120
{
    return _tempNameStrategy;
}

void CurlConfigurer::setTempNameStrategy(string const& strategy)  // PRQA S 4121
{
    _tempNameStrategy = strategy;
}

#ifdef ACS_CURL_CCC_SUPPORTED
curl_ftpccc CurlConfigurer::getFtpsCCC() const throw ()  // PRQA S 4120
{
    return _ftpsCCC;
}

void CurlConfigurer::setFtpsCCC(curl_ftpccc useCCC) throw ()   // PRQA S 4121
{
    _ftpsCCC = useCCC;
}
#endif

bool CurlConfigurer::getIsFtpsProtectData() const throw ()   // PRQA S 4120
{
    return _ftpsProtectData;
}

void CurlConfigurer::setIsFtpsProtectData(bool protect) throw ()   // PRQA S 4121
{
    _ftpsProtectData = protect;
}

std::string CurlConfigurer::getSshPublicKey() const throw ()   // PRQA S 4120
{
    return _sshPublicKey;
}

void CurlConfigurer::setSshPublicKet(std::string const& key) throw ()   // PRQA S 4121
{
    _sshPublicKey = key;
}

std::string CurlConfigurer::getSshPrivateKey() const throw ()   // PRQA S 4120
{
    return _sshPrivateKey;
}

void CurlConfigurer::setSshPrivateKet(std::string const& key) throw ()   // PRQA S 4121
{
    _sshPrivateKey = key;
}

size_t CurlConfigurer::getLowSpeedLimit() const throw()   // PRQA S 4120
{
	return _lowSpeedLimit;
}

void CurlConfigurer::setLowSpeedLimit(size_t const lowSpeedLimit) throw() // PRQA S 4121
{
	_lowSpeedLimit = lowSpeedLimit;
}

size_t CurlConfigurer::getLowSpeedTime() const throw()   // PRQA S 4120
{
	return _lowSpeedTime;
}

void CurlConfigurer::setLowSpeedTime(size_t const lowSpeedTime) throw()   // PRQA S 4121
{
	_lowSpeedTime = lowSpeedTime;
}

const string &CurlConfigurer::getLocalTempPath() const throw () // PRQA S 4120
{
    return _localTempPath;
}

void CurlConfigurer::setLocalTempPath(const std::string &v)  // PRQA S 4121
{
    _localTempPath = v ;
}


const string &CurlConfigurer::getHttpListStylesheetName() const throw () // PRQA S 4120
{
	return _httpListStylesheetName;
}

void CurlConfigurer::setHttpListStylesheetName(const std::string & v) // PRQA S 4121
{
	_httpListStylesheetName = v;
}

const string &CurlConfigurer::getHttpListStylesheetVersion() const throw ()  // PRQA S 4120
{
	return _httpListStylesheetVersion;
}
    
void CurlConfigurer::setHttpListStylesheetVersion(const std::string & v)  // PRQA S 4121
{
	_httpListStylesheetVersion = v;
}

bool CurlConfigurer::getEnableStatistics() const throw () // PRQA S 4020
{
    if (_enableStatistics && _statisticsCheckGood) { return true ; }
    return checkStatistics();
}

void CurlConfigurer::setEnableStatistics(bool enabled) throw () // PRQA S 4211
{
    _enableStatistics = enabled;
    _statisticsCheckGood = true ; // do not perform the test if wasn't yet done
                                  // explicit set - no way to check for human errors 
}

bool CurlConfigurer::checkStatistics() const throw () 
{
    if (not _enableStatistics) { // statistics are not enabled
        _statisticsCheckGood = true ; // whatever table exists on the db
    }
    
    //verify if the T_Circulationstats exists.
    if (not _statisticsCheckGood) {
        try {
            if (not Application::exists() || (Application::instance()->getDefaultConnectionPool() == 0)) {
	            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No default connection pool found, Application::exists(): " << boolalpha << Application::exists()) ;
                ACS_THROW(exIllegalValueException("No default connection pool found")) ; // PRQA S 3081
            }
            dbConnectionWrapper pw(*Application::instance()->getDefaultConnectionPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050 
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "theConnection.getKey(): \"" << theConnection.getKey() << "\"") ;
            _enableStatistics = dbSchemaMetaDataProvider::existTable(CirculationStatistics::table(),theConnection);
            if (!_enableStatistics) {
                ACS_LOG_WARNING("Warning: Cannot enable Statistics because the statistics table does not exist in this schema.");
            } 
            else {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Circulation statistics enabled.");
            }
            _statisticsCheckGood = true ;
        } 
        catch (std::exception const& ex) {
            _enableStatistics = false;
            ACS_LOG_ERROR("Error in detecting statistics table [" << CirculationStatistics::table() << ']');
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex) ;
        }
    }
    
    return _enableStatistics ;
}

bool CurlConfigurer::changePermissions() const throw() 
{
    return _permissions.isActivated();
}

const CurlConfigurer::ChangeLocalPermissions &  CurlConfigurer::getPermissions() const throw() // PRQA S 4120
{
    return _permissions;
}


void CurlConfigurer::setPermissions(const CurlConfigurer::ChangeLocalPermissions &p) // PRQA S 4121
{
    _permissions = p ;
}

CurlConfigurer::ChangeLocalPermissions::ChangeLocalPermissions() : 
    activatedChangeMode(false),
    activatedChangeOwner(false),
    userId(""), 
    groupId(""), 
    mode(DEFAULT_MODE) 
{
    //empty.
}

CurlConfigurer::ChangeLocalPermissions::ChangeLocalPermissions(ChangeLocalPermissions const& other) :
    activatedChangeMode(other.activatedChangeMode),
    activatedChangeOwner(other.activatedChangeOwner),
    userId(other.userId), 
    groupId(other.groupId), 
    mode(other.mode) 
{
    //empty.
}
CurlConfigurer::ChangeLocalPermissions::~ChangeLocalPermissions() throw () {
    //empty.
}
CurlConfigurer::ChangeLocalPermissions& CurlConfigurer::ChangeLocalPermissions::operator=(ChangeLocalPermissions const& other) {
    if (this != &other) {
        activatedChangeMode = other.activatedChangeMode;
        activatedChangeOwner = other.activatedChangeOwner;
        userId= other.userId;
        groupId= other.groupId;
        mode= other.mode;
    }
    return *this;
}

bool CurlConfigurer::isActivatedChangeMode() const throw() // PRQA S 4120
{
    return _permissions.activatedChangeMode;
}

bool CurlConfigurer::isActivatedChangeOwner() const throw()  // PRQA S 4120
{
    return _permissions.activatedChangeOwner;
}

bool CurlConfigurer::ChangeLocalPermissions::isActivated() const throw()  // PRQA S 4120
{
    return activatedChangeMode || activatedChangeOwner;
}

bool CurlConfigurer::ChangeLocalPermissions::isActivatedChangeMode() const throw()  // PRQA S 4120
{
    return activatedChangeMode;
}

bool CurlConfigurer::ChangeLocalPermissions::isActivatedChangeOwner() const throw()  // PRQA S 4120
{
    return activatedChangeOwner;
}

const string &CurlConfigurer::ChangeLocalPermissions::getUserId() const throw() // PRQA S 4120
{
    return userId;
}

const string &CurlConfigurer::ChangeLocalPermissions::getGroupId() const throw() // PRQA S 4120
{
    return groupId;
}

mode_t CurlConfigurer::ChangeLocalPermissions::getMode() const throw() // PRQA S 4120
{
    return mode;
}

bool CurlConfigurer::getSimulateCirculation() const throw()  // PRQA S 4120
{
    return _simulateCirculation ;
}

void CurlConfigurer::setSimulateCirculation(bool v) throw()  // PRQA S 4121
{
    _simulateCirculation = v ;
}

bool CurlConfigurer::findProxy(const std::string &host, curl::Proxy &proxy) const // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Finding proxy for host: '" << host << "'") ;
    std::map<string, curl::Proxy >::const_iterator p = _proxies.find(host) ;
    if (p !=  _proxies.end()) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, p->second << " proxy " << host << "'") ;
        proxy = p->second ;
        return true ;
    }
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "no proxy for host: '" << host << "'") ;
    proxy = curl::no_proxy() ; // reset
    return false ; 
}


string CurlConfigurer::ChangeLocalPermissions::str() const throw() 
{
    ostringstream os;
    os << "ChangePermissions { "
            << " activatedChangeMode: " << activatedChangeMode
            << " activatedChangeOwner: " << activatedChangeOwner
            << " userid: " << userId
            << " groupid: " << groupId
            << " mode: " << std::oct << mode << " }";
    return os.str();
}

bool CurlConfigurer::getFastProbeModeEnabled() const throw() // PRQA S 4120
{
	return _fastProbeModeEnabled;
}

void CurlConfigurer::setFastProbeModeEnabled(bool b) throw() // PRQA S 4121
{
	_fastProbeModeEnabled = b;
}

#ifdef EXP_UPLOAD_BUFFER_CONFIGURABLE
size_t CurlConfigurer::getUploadBufferSize() const throw () { return _uploadBufferSize ; }
void CurlConfigurer::setUploadBufferSize(size_t const ubs) throw () 
{
    const size_t minUBS = 16 * 1024 ;
    const size_t maxUBS = 2*1024*1024 ;
    if ( minUBS > ubs) {
        ACS_LOG_WARNING("Warning: upload buffer (" << ubs << ") less than " << minUBS << " ignored.");
    }
    else if (maxUBS < ubs) {
        ACS_LOG_WARNING("Warning: upload buffer (" << ubs << ") more than " << maxUBS << " ignored.");
    }
    else { 
        _uploadBufferSize = ubs ;
    }
}
#endif  


namespace { // unnamed
    class CurlConfigurerLoader {
    public:
        CurlConfigurerLoader() {
            CurlConfigurerSingleton::instance();
        }
    } _CurlConfigurerLoader;
}




_ACS_END_NESTED_NAMESPACE
