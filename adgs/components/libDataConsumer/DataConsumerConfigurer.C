// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
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
 *  Revision 5.13  2016/03/21 18:26:16  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.12  2016/03/21 10:20:08  francesco.avanzi
 *  rollback to previous state, WORKINPROGRESS
 *
 *  Revision 5.10  2014/06/05 18:47:34  marpas
 *  getting rid of asserts
 *
 *  Revision 5.9  2014/04/14 09:27:21  marpas
 *  management of filters fixed and improved. test added
 *
 *  Revision 5.8  2013/11/14 14:49:00  marpas
 *  message improved
 *
 *  Revision 5.7  2013/10/30 17:52:50  marpas
 *  versioning fixed
 *
 *  Revision 5.6  2013/10/30 17:52:05  marpas
 *  version fixed
 *
 *  Revision 5.5  2013/10/30 17:50:46  marpas
 *  coding best practices applied
 *  robustness improved
 *  useless classes removed
 *  compilation and qa warnings removed
 *
 *  Revision 5.4  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/09/12 07:46:58  lucio.pulvirenti
 *  getLogInfoPeriod and setLogInfoPeriod methods implemented. logInfoPeriod attribute added to IF to configure the period in seconds
 *  between messages on parallel transfers.
 *
 *  Revision 5.2  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.1  2013/07/18 12:29:08  marpas
 *  coding best practices applied
 *  type mismatching fix in progress
 *
 *  Revision 5.0  2013/06/06 20:55:20  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.23  2013/05/02 08:55:37  danalt
 *  using exFPrivateInfo for some info
 *
 *  Revision 1.22  2013/01/17 16:01:03  micmaz
 *  work in progress
 *
 *  Revision 1.21  2012/12/05 10:33:56  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 1.20  2012/05/22 14:53:17  micmaz
 *  RESOLVED - issue S1PDGS-3412: DC: CirculationService in case of exit on error it has to be reflected in the status file
 *  http://jira.acsys.it/browse/S1PDGS-3412
 *
 *  Revision 1.19  2012/03/27 10:53:29  micmaz
 *  work in progress
 *
 *  Revision 1.18  2012/03/15 18:17:44  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.17  2012/03/15 17:39:39  marpas
 *  forcing ctor at the end of compiilation unit
 *
 *  Revision 1.16  2012/03/15 16:58:18  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.15  2012/03/15 16:49:13  micmaz
 *  work in progress
 *
 *  Revision 1.14  2012/03/15 14:36:11  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.13  2012/02/28 17:49:53  micmaz
 *  removed wrong headers.
 *
 *  Revision 1.12  2012/02/28 17:19:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.11  2012/02/07 10:25:56  micmaz
 *  removed warnings.
 *
 *  Revision 1.10  2012/01/23 16:14:51  micmaz
 *  IN PROGRESS - issue S1PDGS-2735: PolicyGenerator: t_circulationusers population
 *  http://jira.acsys.it/browse/S1PDGS-2735
 *
 *  Revision 1.9  2011/10/28 15:24:52  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 1.8  2011/10/28 13:14:03  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 1.7  2011/10/27 14:29:35  micmaz
 *  IN PROGRESS - issue S1PDGS-2276: CirculationService problema nella gestione di un dato gia' trasferito
 *  http://jira.acsys.it/browse/S1PDGS-2276
 *
 *  Revision 1.6  2011/09/23 09:14:42  micmaz
 *  removed unused headers.
 *
 *  Revision 1.5  2011/09/22 15:08:23  micmaz
 *  changed dump() method.
 *
 *  Revision 1.4  2011/09/21 12:41:53  micmaz
 *  added the circulator creation time factor.
 *
 *  Revision 1.3  2011/09/21 12:33:14  micmaz
 *  added the circulator creation time factor.
 *
 *  Revision 1.2  2011/09/13 16:48:06  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.1  2011/09/13 16:31:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.29  2011/08/31 15:28:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.28  2011/08/31 15:11:10  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.27  2011/08/29 13:08:34  micmaz
 *  IN PROGRESS - issue S1PDGS-1806: Add to the CirculationAgent configuration file the connection pool size.
 *  http://jira.acsys.it/browse/S1PDGS-1806
 *
 *  Revision 1.26  2011/08/29 12:05:21  micmaz
 *  RESOLVED - issue S1PDGS-1805: the CirculationAgent does not create a new consumer quickly when a single consumer finished.
 *  http://jira.acsys.it/browse/S1PDGS-1805
 *
 *  Revision 1.25  2011/07/13 17:18:36  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.24  2011/07/13 17:17:52  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.23  2011/07/13 14:23:33  micmaz
 *  the data provider is now not mandatory (for Circulations)
 *
 *  Revision 1.22  2011/07/13 14:00:07  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.21  2011/07/06 15:49:20  micmaz
 *  IN PROGRESS - issue S1PDGS-1608: Add a configuration file to the CirculationAgent application.
 *  http://jira.acsys.it/browse/S1PDGS-1608
 *
 *  Revision 1.20  2011/06/28 17:04:20  micmaz
 *  Added the time to reprocess for circulators
 *
 *  Revision 1.19  2011/06/14 15:05:06  micmaz
 *  changed default time to retry
 *
 *  Revision 1.18  2011/04/26 17:32:49  micmaz
 *  added logs.
 *
 *  Revision 1.17  2011/04/26 14:42:15  micmaz
 *  Updated logs and added removing of sources (if required)
 *
 *  Revision 1.16  2011/04/21 10:18:03  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.15  2011/04/20 16:41:00  micmaz
 *  the "errored" circulation records must be re-worked only after ELAPSED_TIME_TO_RETRY seconds.
 *
 *  Revision 1.14  2011/04/15 15:33:20  micmaz
 *  work in progress...
 *
 *  Revision 1.13  2011/04/15 10:47:30  micmaz
 *  work in progress
 *
 *  Revision 1.12  2011/04/12 18:03:05  micmaz
 *  work in progress
 *
 *  Revision 1.11  2011/03/11 17:56:12  micmaz
 *  reworked timeouts.
 *
 *  Revision 1.10  2011/03/10 16:34:09  micmaz
 *  added new paramters for connection and db  time out.
 *  Improved log in case of failure
 *
 *  Revision 1.9  2011/03/08 18:15:37  micmaz
 *  Added the ovewrite flag to the DataConsumerConfigurer
 *
 *  Revision 1.8  2011/03/03 16:47:33  micmaz
 *  work in progress...
 *
 *  Revision 1.7  2011/02/08 17:47:08  micmaz
 *  removed comments.
 *
 *  Revision 1.6  2010/12/17 10:29:20  micmaz
 *  Added MCF messages.
 *
 *  Revision 1.5  2010/12/09 16:22:14  micmaz
 *  added Mail Warning.
 *
 *  Revision 1.4  2010/11/30 16:41:49  micmaz
 *  added mail warning.
 *
 *  Revision 1.3  2010/11/22 13:01:14  micmaz
 *  added the product logger, correct some minor bugs.
 *
 *  Revision 1.2  2010/11/11 17:45:02  micmaz
 *  corrected some issues : the consumers terminated with a coredump, the consumer hangs on when stopping with errored items, the data consumer item factory does not build correctly a DataConsumerItem.
 *
 *  Revision 1.1.1.1  2010/11/04 10:18:56  micmaz
 *  create a new library
 *
 *
 *
 */

#include <DataConsumerConfigurer.h>

#include <ConfigurationSingleton.h>
#include <StringUtils.h>

#include <rsPushNSpace.h>

#include <DateTime.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <DataConsumerV.h>



#include <boost/algorithm/string.hpp> // PRQA S 1013


_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)
using namespace std;

namespace {
    using acs::dc::DataConsumerV ;
    DataConsumerV version ;
} 
bool DataConsumerConfigurer::_onceForRun = true ;
const string DataConsumerConfigurer::spaceName="libDataConsumer.so";

void DataConsumerConfigurer::confLoadedCallback(void *cp) // PRQA S 4020
{
    if (!onceForRun() || !cp) { return ; }
    setOnceForRun(false);
    DataConsumerConfigurer* c = reinterpret_cast<DataConsumerConfigurer *>(cp) ; // PRQA S 3081
    c->load() ;
    c->dump() ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(DataConsumerConfigurer);
const string DataConsumerConfigurer::DefaultLogFilename="error.log";
const string DataConsumerConfigurer::XmlSlipEmailStyle = "XML_SLIP" ;
const string DataConsumerConfigurer::XmlEmailStyle = "XML" ;
const string DataConsumerConfigurer::HtmlEmailStyle = "HTML" ;
const string DataConsumerConfigurer::RsetEmailStyle = "RSET" ;

DataConsumerConfigurer::DataConsumerConfigurer() :
    _sftpPort(DefaultSFTPPort), 
    _ftpPort(DefaultFTPPort), 
    _removeSources(DefaultRemoveSource), 
    _overwriteCompletedFiles(DefaultOverwriteFiles), 
    _numberOfParallelConsumers(DefaultConsumers), 
    _maxNumberOfRetries(DefaultRetriesNum), 
    _maxDbRetries(DefaultMaxDbRetries), 
    _dbErrorRetryTimeSec(DefaultDbErrorRetrySec), 
    _retryTimeSec(DefaultErrorRetrySec), 
    _byEmailExtension(),
    _logFilename(DefaultLogFilename), 
    _mailWarningStyle(),
    _mailWarningXSL(),
    _forceMailWarningAddress(),
    _mailServer(),
    _mailPort(),
    _mailLogin(),
    _mailPassword(),
    _mailSender(),
    _mailWarningRetries(),
    _mailWarningTimeToRetry(),
    _maxPriority(DefaultMaxPriority),
    _connectionPoolMaxSize(DefaultConnPoolSize),
	_logInfoPeriodSec(DefaultLogInfoPeriodSec),
    filterList_(),
    earlyFinalRemoval_(true),
    checkPreTransfer_(true)
{
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace(spaceName)) ; // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(confLoadedCallback,this) ;
}

DataConsumerConfigurer::~DataConsumerConfigurer() throw () {
    //empty.
}

void DataConsumerConfigurer::load() {
    rsResourceSet& resultSet= ConfigurationSingleton::instance()->get();
    try {
        rsPushNSpace FTPExportDataProviderNS(resultSet, "DataProvider");
        resultSet.getValue("DeliverySlip.ByMailExtension", _byEmailExtension);
    } catch (exception const& ex) {
        //ignore it.
    }

    if (resultSet.hasKey("maxDbRetries")) {
        resultSet.getValue("maxDbRetries", _maxDbRetries);
        if (_maxDbRetries < 1) {
            _maxDbRetries = DefaultMaxDbRetries;
        }
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"maxDbRetries not found, using default.");
    }
    if (resultSet.hasKey("connectionPoolMaxSize")) {
        resultSet.getValue("connectionPoolMaxSize", _connectionPoolMaxSize);
        if (_connectionPoolMaxSize < 2) { // PRQA S 4400
            _connectionPoolMaxSize = DefaultConnPoolSize;
        }
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"connectionPoolMaxSize not found, using default.");
    }
    if (resultSet.hasKey("dbElapsedTime")) {
        resultSet.getValue("dbElapsedTime", _dbErrorRetryTimeSec);
        if (_dbErrorRetryTimeSec < 1) {
            _dbErrorRetryTimeSec = DefaultDbErrorRetrySec;
        }
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"dbElapsedTime not found, using default.");
    }

    {
        rsPushNSpace FTPExportNS(resultSet, "DataConsumer");
        resultSet.getValue("sftpPort", _sftpPort);
        resultSet.getValue("ftpPort", _ftpPort);
        resultSet.getValue("errorLog", _logFilename);
        if (resultSet.hasKey("checkPreTransfer")) 
        {
            resultSet.getValue("checkPreTransfer", checkPreTransfer_) ;
        }
        else
        {
            checkPreTransfer_ = true ;
        }
        if (resultSet.hasKey("RemoveFinalTargetBeforeTransfer")) 
        {
            resultSet.getValue("RemoveFinalTargetBeforeTransfer", earlyFinalRemoval_) ;
        }
        else
        {
            earlyFinalRemoval_ = true ;
        }
        if (resultSet.hasKey("IgnoreFileList")) {
            resultSet.getValue("IgnoreFileList", filterList_);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"IgnoreFileList not found.");
        }
        
        string mustRemoveSources;
        if (resultSet.hasKey("removeSources")) {
            resultSet.getValue("removeSources", mustRemoveSources);
            _removeSources = (mustRemoveSources == "true");
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"removeSource not found, using default.");
        }

        if (resultSet.hasKey("overwriteFiles")) {
            string mustOverwriteCompletedFiles;
            resultSet.getValue("overwriteFiles", mustOverwriteCompletedFiles);
            _overwriteCompletedFiles = (StringUtils::equalsNoCase(mustOverwriteCompletedFiles, "true"));
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"overwriteFiles not found, using default.");
        }
        if (resultSet.hasKey("maxPriority")) {
            resultSet.getValue("maxPriority", _maxPriority);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"maxPriority not found, using default.");
        }
        if (resultSet.hasKey("numberOfParalledConsumers")) {
            resultSet.getValue("numberOfParalledConsumers", _numberOfParallelConsumers);
        }
        if (resultSet.hasKey("maxNumberOfRetries")) {
            resultSet.getValue("maxNumberOfRetries", _maxNumberOfRetries);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"maxNumberOfRetries not found, using default.");
            _maxNumberOfRetries = DefaultRetriesNum;
        }
        if (resultSet.hasKey("mailWarningRetries")) {
            resultSet.getValue("mailWarningRetries", _mailWarningRetries);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"mailWarningRetries not found, using default.");
            _mailWarningRetries = DefaultRetriesNum;
        }
        if (resultSet.hasKey("elapsedTimeToRetry")) {
            resultSet.getValue("elapsedTimeToRetry", _retryTimeSec);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"elapsedTimeToRetry not found, using default.");
            _retryTimeSec = DefaultErrorRetrySec;
        }
        if (resultSet.hasKey("mailWarningTimeToRetry")) {
            resultSet.getValue("mailWarningTimeToRetry", _mailWarningTimeToRetry);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"mailWarningTimeToRetry not found, using default.");
            _mailWarningTimeToRetry = DefaultErrorRetrySec;
        }
        if (resultSet.hasKey("logInfoPeriod")) {
            resultSet.getValue("logInfoPeriod", _logInfoPeriodSec);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"logInfoPeriod not found, using default.");
			_logInfoPeriodSec = DefaultLogInfoPeriodSec;
        }
		
        if (resultSet.hasKey("MailWarning")) {
            rsPushNSpace EMailWarningNS(resultSet, "MailWarning");

            resultSet.getValue("server", _mailServer);
            string mailPort;
            resultSet.getValue("port", mailPort);
            if (mailPort.length() == 0) {
                _mailPort = -1;
            } else {
                _mailPort = atoi(mailPort.c_str());
            }
            resultSet.getValue("login", _mailLogin);
            resultSet.getValue("password", _mailPassword);
            resultSet.getValue("mailSender", _mailSender);

            string warnStyle;
            _mailWarningStyle = RsetEmailStyle; // default value.
            resultSet.getValue("MailWarningStyle", warnStyle);
            if (StringUtils::equalsNoCase(warnStyle, XmlEmailStyle)
                    || StringUtils::equalsNoCase(warnStyle, XmlSlipEmailStyle)) {
                _mailWarningStyle = warnStyle;
            } else if (StringUtils::equalsNoCase(warnStyle, HtmlEmailStyle)) {
                _mailWarningStyle = warnStyle;
                resultSet.getValue("MailWarningXSL", _mailWarningXSL);
            } else {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,
                        "Warning! The MailWarningStyle configuration value was not correct (" << warnStyle
                                << ") will use " << RsetEmailStyle);
            }
            string setForceMailWarningAddress = "0";
            resultSet.getValue("SetForceMailWarningAddress", setForceMailWarningAddress);
            if (setForceMailWarningAddress != "0") {
                /* Set forced MailWarningAddress and ForceDbReportsMailWarningAddress. Used also as flag. */
                resultSet.getValue("ForceMailWarningAddress", _forceMailWarningAddress);
            } else {
                _forceMailWarningAddress = "";
            }
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"mailWarning not found, using default.");
        }
    }
}

void DataConsumerConfigurer::dump() const {
    ACS_LOG_PRIVINFO( "\n"
       << left << setw(50) << "--------------------------------------------  " << "\n"
       << left << setw(50) << "Data Consumer configuration status            " << "\n"
       << left << setw(50) << "--------------------------------------------  " << "\n"
       << left << setw(50) << "sftp port:" << right << setw(30) << _sftpPort << "\n"
       << left << setw(50) << "ftp port:" << right << setw(30) << _ftpPort << "\n"
       << left << setw(50) << "connection pool max size:" << right << setw(30) << _connectionPoolMaxSize << "\n"
       << left << setw(50) << "remove sources:" << right << setw(30) << _removeSources << "\n"
       << left << setw(50) << "overwrite completed files:" << right << setw(30)
            << _overwriteCompletedFiles << "\n"
       << left << setw(50) << "number of parallel consumers:" << right << setw(30)
            << _numberOfParallelConsumers << "\n"
       << left << setw(50) << "max db retries:" << right << setw(30) << _maxDbRetries << "\n"
       << left << setw(50) << "Elapsed time to retry for db errors [s]:" << right
            << setw(30) << _dbErrorRetryTimeSec << "\n"
       << left << setw(50) << "Elapsed time to retry for erroneous items [s]:" << right
            << setw(30) << _retryTimeSec << "\n"
       << left << setw(50) << "By Email extension:" << right << setw(30) << _byEmailExtension
            << "\n"
       << left << setw(50) << "Log file name:" << right << setw(30) << _logFilename << "\n"
       << left << setw(50) << "Mail warning style:" << right << setw(30) << _mailWarningStyle
            << "\n"
       << left << setw(50) << "Mail warning xsl:" << right << setw(30) << _mailWarningXSL
            << "\n"
       << left << setw(50) << "Force mail warning address:" << right << setw(30)
            << _forceMailWarningAddress << "\n"
       << left << setw(50) << "Mail server:" << right << setw(30) << _mailServer << "\n"
       << left << setw(50) << "Mail port:" << right << setw(30) << _mailPort << "\n"
       << left << setw(50) << "Mail login:" << right << setw(30) << _mailLogin << "\n"
       << left << setw(50) << "Mail password:" << right << setw(30) << _mailPassword << "\n"
       << left << setw(50) << "Mail sender:" << right << setw(30) << _mailSender << "\n"
       << left << setw(50) << "Mail warning retries:" << right << setw(30)
            << _mailWarningRetries << "\n"
       << left << setw(50) << "Mail warning time to retry:" << right << setw(30)
            << _mailWarningTimeToRetry << "\n"
       << left << setw(50) << "Max priority:" << right << setw(30) << _maxPriority << "\n"
       << left << setw(50) << "Log info period [s]:" << right << setw(30) << _logInfoPeriodSec << "\n"
       << left << setw(50) << "FilterList :" << right << setw(30) << filterList_ << "\n"
       << left << setw(50) << "earlyFinalRemoval :" << right << setw(30) << boolalpha <<  earlyFinalRemoval_ << "\n"
       << left << setw(50) << "checkPreTransfer :" << right << setw(30) << boolalpha <<  checkPreTransfer_ << "\n"
       << left << setw(50) << "--------------------------------------------  ") ;
}

//
//Gettes/Setters section.
//

string const& DataConsumerConfigurer::getByEmailExtension() const throw() // PRQA S 4120
{
    return _byEmailExtension;
}
size_t DataConsumerConfigurer::getElapsedTimeToRetry() const throw()  // PRQA S 4120
{
    return _retryTimeSec;
}

string const& DataConsumerConfigurer::getFilterList() const  throw() // PRQA S 4120
{
    return filterList_ ;
}

string const& DataConsumerConfigurer::getForceMailWarningAddress() const  throw() // PRQA S 4120
{
    return _forceMailWarningAddress;
}

size_t DataConsumerConfigurer::getFtpPort() const throw()  // PRQA S 4120
{
    return _ftpPort;
}

string const& DataConsumerConfigurer::getLogFilename() const  throw() // PRQA S 4120
{
    return _logFilename;
}

string const& DataConsumerConfigurer::getMailLogin() const  throw() // PRQA S 4120
{
    return _mailLogin;
}

string const& DataConsumerConfigurer::getMailPassword() const  throw() // PRQA S 4120
{
    return _mailPassword;
}

int DataConsumerConfigurer::getMailPort() const throw()  // PRQA S 4120
{
    return _mailPort;
}

string const& DataConsumerConfigurer::getMailSender() const  throw() // PRQA S 4120
{
    return _mailSender;
}

string const& DataConsumerConfigurer::getMailServer() const  throw() // PRQA S 4120
{
    return _mailServer;
}

size_t DataConsumerConfigurer::getMailWarningRetries() const throw() // PRQA S 4120 
{
    return _mailWarningRetries;
}

string const& DataConsumerConfigurer::getMailWarningStyle() const throw() // PRQA S 4120
{
    return _mailWarningStyle;
}

size_t DataConsumerConfigurer::getMailWarningTimeToRetry() const throw()  // PRQA S 4120
{
    return _mailWarningTimeToRetry;
}

string const& DataConsumerConfigurer::getMailWarningXSL() const throw() // PRQA S 4120
{
    return _mailWarningXSL;
}

size_t DataConsumerConfigurer::getMaxDbElapsedTime() const throw()  // PRQA S 4120
{
    return _dbErrorRetryTimeSec;
}

size_t DataConsumerConfigurer::getMaxDbRetries() const throw()  // PRQA S 4120
{
    return _maxDbRetries;
}

size_t DataConsumerConfigurer::getMaxNumberOfRetries() const throw()  // PRQA S 4120
{
    return _maxNumberOfRetries;
}

size_t DataConsumerConfigurer::getNumberOfParallelConsumers() const throw()  // PRQA S 4120
{
    return _numberOfParallelConsumers;
}

bool DataConsumerConfigurer::getOverwriteCompletedFiles() const throw()  // PRQA S 4120
{
    return _overwriteCompletedFiles;
}

bool DataConsumerConfigurer::getRemoveSources() const throw()  // PRQA S 4120
{
    return _removeSources;
}

size_t DataConsumerConfigurer::getSftpPort() const throw()  // PRQA S 4120
{
    return _sftpPort;
}

size_t DataConsumerConfigurer::getLogInfoPeriod() const throw()  // PRQA S 4120
{
    return _logInfoPeriodSec;
}

void DataConsumerConfigurer::setByEmailExtension(const string &v)  // PRQA S 4121
{
    _byEmailExtension = v;
}

void DataConsumerConfigurer::setElapsedTimeToRetry(size_t v) throw()  // PRQA S 4121
{
    _retryTimeSec = v;
}

void DataConsumerConfigurer::setForceMailWarningAddress(const string &v)  // PRQA S 4121
{
    _forceMailWarningAddress = v;
}

void DataConsumerConfigurer::setFtpPort(size_t v) throw()  // PRQA S 4121
{
    _ftpPort = v;
}

void DataConsumerConfigurer::setLogFilename(const string &v)  // PRQA S 4121
{
    _logFilename = v;
}

void DataConsumerConfigurer::setMailLogin(const string &v)  // PRQA S 4121
{
    _mailLogin = v;
}

void DataConsumerConfigurer::setMailPassword(const string &v)  // PRQA S 4121
{
    _mailPassword = v;
}

void DataConsumerConfigurer::setMailPort(int v) throw()  // PRQA S 4121
{
    _mailPort = v;
}

void DataConsumerConfigurer::setMailSender(const string &v)  // PRQA S 4121
{
   _mailSender = v;
}

void DataConsumerConfigurer::setMailServer(const string &v)  // PRQA S 4121
{
    _mailServer = v;
}

void DataConsumerConfigurer::setMailWarningRetries(size_t v) throw()  // PRQA S 4121
{
    _mailWarningRetries = v;
}

void DataConsumerConfigurer::setMailWarningStyle(const string &v)  // PRQA S 4121
{
    _mailWarningStyle = v;
}

void DataConsumerConfigurer::setMailWarningTimeToRetry(size_t v) throw()  // PRQA S 4121
{
    _mailWarningTimeToRetry = v;
}

void DataConsumerConfigurer::setMailWarningXSL(const string &v)  // PRQA S 4121
{
    _mailWarningXSL = v;
}

void DataConsumerConfigurer::setMaxDbElapsedTime(size_t v) throw()  // PRQA S 4121
{
    _dbErrorRetryTimeSec = v;
}

void DataConsumerConfigurer::setMaxDbRetries(size_t v) throw()  // PRQA S 4121
{
    _maxDbRetries = v;
}

void DataConsumerConfigurer::setMaxNumberOfRetries(size_t v) throw()  // PRQA S 4121
{
    _maxNumberOfRetries = v;
}

void DataConsumerConfigurer::setNumberOfParalledConsumers(size_t v) throw()  // PRQA S 4121
{
    _numberOfParallelConsumers = v;
}

void DataConsumerConfigurer::setOverwriteCompletedFiles(bool v) throw()  // PRQA S 4121
{
    _overwriteCompletedFiles = v;
}

void DataConsumerConfigurer::setRemoveSources(bool v) throw()  // PRQA S 4121
{
    _removeSources = v;
}

void DataConsumerConfigurer::setSftpPort(size_t v) throw()  // PRQA S 4121
{
    _sftpPort = v;
}

void DataConsumerConfigurer::setMaxPriority(int v) throw()  // PRQA S 4121
{
    _maxPriority = v;
}

int DataConsumerConfigurer::getMaxPriority() const throw()   // PRQA S 4120
{
    return _maxPriority;
}

size_t DataConsumerConfigurer::getConnectionPoolMaxSize() const throw() // PRQA S 4120
{
    return _connectionPoolMaxSize;
}

void DataConsumerConfigurer::setConnectionPoolMaxSize(size_t connectionPoolMaxSize) throw()  // PRQA S 4121
{
    _connectionPoolMaxSize = connectionPoolMaxSize;
}

void DataConsumerConfigurer::setLogInfoPeriod(size_t v) throw() // PRQA S 4121
{
    _logInfoPeriodSec = v;
}

void DataConsumerConfigurer::setCheckPreTransfer(bool v) throw() // PRQA S 4121
{
    checkPreTransfer_ = v;
}

bool DataConsumerConfigurer::getEarlyFinalRemoval() const throw()
{ 
    return earlyFinalRemoval_ ;
}
bool DataConsumerConfigurer::getCheckPreTransfer() const throw()
{ 
    return checkPreTransfer_ ;
}
namespace { // unnamed
    class DataConsumerConfigurerLoader {
    public:
        DataConsumerConfigurerLoader(){
            dcConfigurerSingleton::instance();
        }
    } _dataConsumerConfigurerLoader;
}

_ACS_END_NESTED_NAMESPACE
