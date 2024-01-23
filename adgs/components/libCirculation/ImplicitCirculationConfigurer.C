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
 *  Revision 7.1  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.24  2016/07/26 09:32:25  lucio.pulvirenti
 *  S1PSGS-31765: local transfer mode added to IF to be got by configuration. Set and get methods added.
 *  Conversion methods between local transfer mode as a string and as an enume defined in libFtpCurl added.
 *
 *  Revision 5.23  2015/11/16 16:27:36  lucio.pulvirenti
 *  S2PDGS-1373: logErrorVerbose tag introduced in configuration and managed with proper attribute and get/set methods.
 *
 *  Revision 5.22  2015/09/04 14:58:39  lucio.pulvirenti
 *  S2PDGS-1247: Media circulation records occurrence info period added to IF.
 *
 *  Revision 5.21  2015/03/10 11:35:51  marfav
 *  S2PDGS-1110 Added the function to manage local and remote repositories in different controlling threads
 *
 *  Revision 5.20  2015/02/19 14:56:52  francesco.avanzi
 *  getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
 *
 *  Revision 5.19  2014/09/10 16:01:55  marfav
 *  Default band limit set to unlimited (0 bps)
 *
 *  Revision 5.18  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.17  2014/06/05 18:48:09  marpas
 *  getting rid of asserts
 *
 *  Revision 5.16  2014/03/13 14:33:02  marfav
 *  getCirculatorPriorityPlainOrder implemented
 *
 *  Revision 5.15  2014/03/13 13:07:53  francesco.avanzi
 *  better code practice applied
 *
 *  Revision 5.14  2014/03/13 11:10:36  francesco.avanzi
 *  PriorityPlainOrder is now taken from configuration file and directily pushed into the BandShaperSingleton
 *
 *  Revision 5.13  2014/02/17 16:14:55  lucio.pulvirenti
 *  Link speed default value now 100 Mbit/s. Data type now int.
 *
 *  Revision 5.12  2014/02/17 13:18:48  lucio.pulvirenti
 *  Link speed [GB/s] added with set and get methods.
 *
 *  Revision 5.11  2014/02/05 15:12:33  lucio.pulvirenti
 *  If HostName tag not occurring/valorized in CirculationService node, set _hostname by using HWScan class.
 *
 *  Revision 5.10  2014/01/14 11:34:14  marfav
 *  S1PDGS-31350: default max retry read from database
 *
 *  Revision 5.9  2014/01/10 16:04:28  marfav
 *  Configuration management implementation completed
 *
 *  Revision 5.8  2014/01/09 17:25:20  marfav
 *  Support for retry configuration in progress - S1PDGS-31034
 *
 *  Revision 5.7  2013/11/04 15:28:14  marpas
 *  log messages improved
 *
 *  Revision 5.6  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.5  2013/08/05 12:48:55  lucio.pulvirenti
 *  _circulatorPollingPeriod valorized by configuration pollingPeriodInSeconds tag value.
 *   _useInotify valorized by configuration useInotify tag value.
 *  Get and set methods of _circulatorPollingPeriod and _useInotify attributes implemented.
 *
 *  Revision 5.4  2013/08/02 16:34:20  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.3  2013/07/23 17:14:26  marpas
 *  fixing types
 *
 *  Revision 5.2  2013/07/18 12:29:51  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.22  2013/05/17 15:20:35  marpas
 *  namespace added
 *
 *  Revision 1.21  2013/05/02 16:44:13  danalt
 *  using exFPrivateInfo for some info
 *
 *  Revision 1.20  2013/01/25 13:47:36  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.19  2012/12/04 17:43:34  marpas
 *  getting rid of FilenameUtils class
 *
 *  Revision 1.18  2012/04/03 10:11:06  micmaz
 *  work in progress
 *
 *  Revision 1.17  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.16  2012/03/15 18:17:06  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.15  2012/03/15 17:44:28  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.14  2012/03/15 16:58:07  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.13  2012/03/15 16:49:14  micmaz
 *  work in progress
 *
 *  Revision 1.12  2012/03/15 14:34:29  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.11  2012/03/09 17:43:34  micmaz
 *  added media functionality
 *
 *  Revision 1.10  2012/03/08 13:20:45  micmaz
 *  added media functionality
 *
 *  Revision 1.9  2012/02/28 17:50:21  micmaz
 *  removed wrong headers.
 *
 *  Revision 1.8  2012/02/28 17:17:16  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.7  2012/02/20 15:11:10  marpas
 *  refactoring
 *
 *  Revision 1.6  2012/01/23 16:20:52  micmaz
 *  IN PROGRESS - issue S1PDGS-2735: PolicyGenerator: t_circulationusers population
 *  http://jira.acsys.it/browse/S1PDGS-2735
 *
 *  Revision 1.5  2011/12/12 13:44:31  micmaz
 *  IN PROGRESS - issue S1PDGS-2411: Circulation Agent hostname problem
 *  http://jira.acsys.it/browse/S1PDGS-2411
 *
 *  Revision 1.4  2011/11/04 10:25:14  micmaz
 *  IN PROGRESS - issue S1PDGS-2343: CirculationService and DataStorageService hostname problem
 *  http://jira.acsys.it/browse/S1PDGS-2343
 *
 *
 *  Revision 1.2  2011/10/28 15:24:59  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 1.1  2011/10/28 13:13:24  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *
 *
 */

#include <ImplicitCirculationConfigurer.h>

#include <ConfigurationSingleton.h>

#include <BandPriority.h>
#include <StringUtils.h>

#include <rsPushNSpace.h>
#include <dbSmartQuery.h>

#include <DateTime.h>
#include <StringUtils.h>

#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>

#include <Application.h>
#include <HWScan.h>


#include <boost/algorithm/string.hpp> // PRQA S 1013


_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)

using namespace std;


bool ImplicitCirculationConfigurer::_onceForRun = true ;
const string ImplicitCirculationConfigurer::spaceName = "libCirculation.so";
/*
const bool ImplicitCirculationConfigurer::DefaultCirculationRemoveSources=true;
const int ImplicitCirculationConfigurer::DefaultCirculationThreads=5;
const int ImplicitCirculationConfigurer::DefaultCirculationReprocessInterval=60;
const int ImplicitCirculationConfigurer::DefaultCirculationPollingPeriod=60; // seconds
const int ImplicitCirculationConfigurer::DefaultConnPoolSize=10;
const int ImplicitCirculationConfigurer::DefaultCreationTimeWeight=10;
*/
const string ImplicitCirculationConfigurer::DefaultMediaFilename = "${DIR_NAME}/${FILE_NAME}.MF";
const string ImplicitCirculationConfigurer::DefaultMediaSignerType = "MD5";
const string ImplicitCirculationConfigurer::DefaultAllowedCiphers="";
const curl::CirculationCurl::LocalTransferMode ImplicitCirculationConfigurer::DefaultLocalTransferMode = curl::CirculationCurl::COPY;
const time_t ImplicitCirculationConfigurer::DefaultLocalSourceFilesRetrieveDTime = 5;
const size_t ImplicitCirculationConfigurer::DefaultLocalItemsToSkipMaxNum = 1000;


void ImplicitCirculationConfigurer::confLoadedCallback(void *cp) // PRQA S 4020
{
    if (!onceForRun() || !cp) { return ; }
    setOnceForRun(false);
    ImplicitCirculationConfigurer* c = reinterpret_cast<ImplicitCirculationConfigurer *>(cp) ; // PRQA S 3081
    c->load() ;
    c->dump() ;

}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ImplicitCirculationConfigurer);

ImplicitCirculationConfigurer::ImplicitCirculationConfigurer() :
        _circulatorRemoveSources(DefaultCirculationRemoveSources),
        _circulatorPriorityPlainOrder(true),
        _circulatorNumberOfThreads(DefaultCirculationThreads),
        _circulatorNumberOfRemoteThreads(DefaultCirculationThreads),
        _policiesReprocessInterval(DefaultCirculationReprocessInterval),
		_circulatorPollingPeriod(DefaultCirculationPollingPeriod),
        _circulatorCreationTimeWeight(DefaultCreationTimeWeight),
        _hostName(""),
        _mediaReportFilename(DefaultMediaFilename),
        _mediaSignerType(DefaultMediaSignerType),
        _allowedCiphers(DefaultAllowedCiphers),
        _updateDelta(DefaultUpdateDelta),
		_useInotify(true),
		_linkSpeed(DefaultLinkSpeed),
		_mediaCirculationInfoPeriod(DefaultMediaCirculationInfoPeriod),
        _implicitRetryConf(),
        _explicitRetryConf(),
        _dynamicRetryConf(),
		_logErrorsVerbose(true),
		_filterRecoverableErrors(false),
		_localTransferMode(LocalTransferMode2String(DefaultLocalTransferMode)),
		_localSourceFilesRetrieveDTime(DefaultLocalSourceFilesRetrieveDTime),
		_localItemsToSkipMaxNum(DefaultLocalItemsToSkipMaxNum),
        _unlinkFailureAction(DefaultUnlinkFailureAction),
        _unlinkFailureRenameStrategy(),
        _localUnlinkFailureTrashPath()
{
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace(spaceName)) ; // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(confLoadedCallback,this) ;
}

ImplicitCirculationConfigurer::~ImplicitCirculationConfigurer() throw () {
    // empty
}

void ImplicitCirculationConfigurer::load() {
    rsResourceSet& resultSet = ConfigurationSingleton::instance()->get();
    try {
        rsPushNSpace FTPExportNS(resultSet, "ImplicitCirculator");
        if (resultSet.hasKey("removeSources")) {
            string removeSources;
            resultSet.getValue("removeSources", removeSources);
            _circulatorRemoveSources = (boost::algorithm::iequals(removeSources, "true")
                    || boost::algorithm::iequals(removeSources, "t"));
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "removeSources not found, using default.");
        }
        if (resultSet.hasKey("priorityPlainOrder")) {
            resultSet.getValue("priorityPlainOrder", _circulatorPriorityPlainOrder);
            curl::BandShaperSingleton::instance()->setIsDirect(_circulatorPriorityPlainOrder);
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "priorityPlainOrder not found, using default.");
        }
        
        if (resultSet.hasKey("numberOfThreads") || resultSet.hasKey ("numberOfLocalThreads")) {
            resultSet.getValue("numberOfThreads", _circulatorNumberOfThreads, false);
            resultSet.getValue("numberOfLocalThreads", _circulatorNumberOfThreads, false);
            if (_circulatorNumberOfThreads < 1) {
                ACS_LOG_ERROR("Invalid circulatorNumberOfThreads property: it must be > 0. Setting to the default value.");
                _circulatorNumberOfThreads = DefaultCirculationThreads;
            }
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "numberOfThreads not found, using default.");
        }

        if (resultSet.hasKey("numberOfRemoteThreads")) {
            resultSet.getValue("numberOfRemoteThreads", _circulatorNumberOfRemoteThreads);
            if (_circulatorNumberOfRemoteThreads < 1) {
                ACS_LOG_ERROR("Invalid circulatorNumberOfThreads property: it must be > 0. Setting to the default value.");
                _circulatorNumberOfRemoteThreads = DefaultCirculationThreads;
            }
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "numberOfRemoteThreads not found, using default.");
        }

        



        if (resultSet.hasKey("secondsToReprocessPolicies")) {
            resultSet.getValue("secondsToReprocessPolicies", _policiesReprocessInterval);
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "secondsToReprocessPolicies not found, using default.");
        }
        if (resultSet.hasKey("pollingPeriodInSeconds")) {
            resultSet.getValue("pollingPeriodInSeconds", _circulatorPollingPeriod);
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "pollingPeriodInSeconds not found, using default.");
        }
        if (resultSet.hasKey("allowedCiphers")) {
            resultSet.getValue("allowedCiphers", _allowedCiphers);
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "allowedCiphers not found, using default.");
        }
        if (resultSet.hasKey("updateDelta")) {
            resultSet.getValue("updateDelta", _updateDelta);
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "updateDelta not found, using default.");
        }

        if (resultSet.hasKey("creationTimeWeight")) {
            resultSet.getValue("creationTimeWeight", _circulatorCreationTimeWeight);
            if (_circulatorCreationTimeWeight < 1) {
                ACS_LOG_ERROR("Invalid creation time weight: it must be > 0. Setting to the default value.");
                _circulatorCreationTimeWeight = DefaultCreationTimeWeight;
            }
        } 
        else {
            if (resultSet.hasKey("creationTimeFactor")) {
                ACS_LOG_WARNING("Found obsolete creationTimeFactor, should be creationTimeWeight instead") ;
                resultSet.getValue("creationTimeWeight", _circulatorCreationTimeWeight);
                if (_circulatorCreationTimeWeight < 1) {
                    ACS_LOG_ERROR("Invalid creation time weight: it must be > 0. Setting to the default value.");
                    _circulatorCreationTimeWeight = DefaultCreationTimeWeight;
                }
            } 
            else {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "creationTimeWeight not found, using default.");
            }
        }



        if (resultSet.hasKey("useInotify")) {
        	resultSet.getValue("useInotify", _useInotify);
    	} 
        else {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "useInotify not found: use inotify");
    	}
		
        if (resultSet.hasKey("linkSpeed")) {
        	resultSet.getValue("linkSpeed", _linkSpeed);
    	} 
        else {
			ACS_LOG_WARNING("Configuration key ImplicitCirculator.linkSpeed not found. Using default value " << _linkSpeed << " Mbit/s");
    	}
		curl::BandShaperSingleton::instance()->setLinkSpeed( _linkSpeed ) ;
		
        if (resultSet.hasKey("MediaCirculationInfoPeriod")) {
        	resultSet.getValue("MediaCirculationInfoPeriod", _mediaCirculationInfoPeriod);
    	} 
        else {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "MediaCirculationInfoPeriod not found, using default: " << _mediaCirculationInfoPeriod << " s");
    	}
		
	if (resultSet.hasKey("logErrorsVerbose")) {
        	resultSet.getValue("logErrorsVerbose", _logErrorsVerbose);
    	} 
        else {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "logErrorsVerbose not found: set true");
    	}
		if (resultSet.hasKey("filterRecoverableError")) {
			resultSet.getValue("filterRecoverableError", _filterRecoverableErrors);
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "filterRecoverableError not found: set to false");
		}
        
        if (resultSet.hasKey("UnlinkFailureAction")) {
            std::string val;
            resultSet.getValue("UnlinkFailureAction", val);
            _unlinkFailureAction = ImplicitCirculationConfigurer::String2UnlinkFailureAction(val);
            if (!resultSet.hasKey("LocalUnlinkFailuresTrashPath") && _unlinkFailureAction == ON_UNLINK_FAILURE_DO_MOVE) {
                ACS_LOG_WARNING("UnlinkFailureAction is set to " + ImplicitCirculationConfigurer::UnlinkFailureAction2String(_unlinkFailureAction) + 
                                    " but no LocalUnlinkFailuresTrashPath was specified. UnlinkFailureAction set to " + 
                                        ImplicitCirculationConfigurer::UnlinkFailureAction2String(ON_UNLINK_FAILURE_DO_NOTHING) + ".");
                _unlinkFailureAction = ON_UNLINK_FAILURE_DO_NOTHING;
            }
            else {
                resultSet.getValue("LocalUnlinkFailuresTrashPath", _localUnlinkFailureTrashPath);
            }
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "UnlinkFailureAction not found: set to " << ImplicitCirculationConfigurer::UnlinkFailureAction2String(_unlinkFailureAction));
        }
        
        if (resultSet.hasKey("UnlinkFailureRenameStrategy.Mode") && resultSet.hasKey("UnlinkFailureRenameStrategy.Value")) {
            std::string val;
            resultSet.getValue("UnlinkFailureRenameStrategy.Mode", val);
            resultSet.getValue("UnlinkFailureRenameStrategy.Value", _unlinkFailureRenameStrategy.value);

            _unlinkFailureRenameStrategy.mode = ImplicitCirculationConfigurer::String2UnlinkFailureRenameStrategyMode(val);

        }

		if (resultSet.hasKey("localTransferMode")) {
        	resultSet.getValue("localTransferMode", _localTransferMode);
    	} 
        else {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "localTransferMode not found, using default: " << _localTransferMode );
    	}

		if (resultSet.hasKey("localSourceFilesRetrieveDTime")) {
        	resultSet.getValue("localSourceFilesRetrieveDTime", _localSourceFilesRetrieveDTime);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "localSourceFilesRetrieveDTime: " << _localSourceFilesRetrieveDTime << " s" );
    	} 
        else {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "localSourceFilesRetrieveDTime not found, using default: " << _localSourceFilesRetrieveDTime );
    	}

		if (resultSet.hasKey("localItemsToSkipMaxNum")) {
        	resultSet.getValue("localItemsToSkipMaxNum", _localItemsToSkipMaxNum);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "localItemsToSkipMaxNum: " << _localItemsToSkipMaxNum );
    	} 
        else {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "localItemsToSkipMaxNum not found, using default: " << _localItemsToSkipMaxNum );
    	}

		
    } catch (exception const& ex) {
        //ignore it.
    }

    try {

        rsPushNSpace rconfpusher (resultSet, "RetriesConfigurationPolicies");

        // Retry policies configuration 
        // Create the default policy and copy the old value of reprocess timeout
        RetryConfiguration defaultRetryConf;
        defaultRetryConf.retryTimeout = _policiesReprocessInterval; // PRQA S 3000
        try
        {
            defaultRetryConf.maxRetries= getDefaultMaxRetryFromDb();
        }
        catch (exception& ex)
        {
            // No problem.. unable to get from database.. use the super-master-hardcoded-default       
            ACS_LOG_WARNING ("Unable to get default max retries from the db. Will use the hard-coded default if not provided in configuration");
            ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, ex.what());
        }

        // Try to retrieve the default retry configuration
        // This call makes changes directly in defaultRetryConf object
        // furthermore this will never throw but will raise warning if necessary
        retrieveRetryConfiguration (resultSet, "Default", defaultRetryConf, true); // this is mandatory

        // Copy the default in Implicit, Explicit, Dynamic configurations
        // and then override with set values (if any)
        RetryConfiguration implicitRetryConf = defaultRetryConf;
        retrieveRetryConfiguration (resultSet, "ImplicitMode", implicitRetryConf, false); // this is not mandatory

        RetryConfiguration explicitRetryConf = defaultRetryConf;
        retrieveRetryConfiguration (resultSet, "ExplicitMode", explicitRetryConf, false); // this is not mandatory

        RetryConfiguration  dynamicRetryConf = defaultRetryConf;
        retrieveRetryConfiguration (resultSet, "DynamicMode", dynamicRetryConf, false); // this is not mandatory

        // safe them permanently
        setImplicitRetryConfiguration (implicitRetryConf);
        setExplicitRetryConfiguration (explicitRetryConf);
        setDynamicRetryConfiguration  (dynamicRetryConf);

    } catch (exception const& ex) {
        ACS_LOG_WARNING ("Unable to read retry configuration settings from configuration space. Defaults will apply.");
    }

    try {
        rsPushNSpace cServ(resultSet, "CirculationService");
        resultSet.getValue("HostName", _hostName);
    } catch (exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Host Name not found, using hostname unix call");
    }
	if (StringUtils::isBlank(_hostName)) {
        _hostName = HWScan::hostname();
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Using local host name from HWScan class: " << _hostName);
    }

    try {
        rsPushNSpace cServ(resultSet, "CirculationService");
        if (resultSet.hasKey("mediaReport")) {
            string mediaReport;
            resultSet.getValue("mediaReport", mediaReport);
            _mediaReportFilename = mediaReport;
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "mediaReport not found, using default.");
        }
        if (resultSet.hasKey("mediaSignerType")) {
            string mediaSignerType;
            resultSet.getValue("mediaSignerType", mediaSignerType);
            _mediaSignerType = mediaSignerType;
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "mediaSignerType not found, using default.");
        }
    } catch (exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Host Name not found, using hostname unix call");
    }
	
}

unsigned int ImplicitCirculationConfigurer::getDefaultMaxRetryFromDb () const
{
    // Use the default connection pool 
    dbConnectionPool* pool = Application::instance()->getDefaultConnectionPool();
    dbConnectionWrapper cw(*pool) ;
    dbConnection &conn = cw ; // PRQA S 3050
    string dbKey = conn.getKey();

    unsigned int retVal = 0;

    SQLString selectSQL( dbKey, SQLString::SELECT ) ;
    selectSQL.addTableList("t_dbparameters");
    selectSQL.addSelectList("value_int");

    {
        dbQueryParameters pp (dbKey);
        pp.addParameter(dbParam(dbKey, "t_dbparameters", "parameter" , dbParam::EQUAL_TO , db::toSQL(dbKey, "circ_def_max_ret")));
        selectSQL.addORWhereParameter(pp) ;
    }

    {
        dbQueryParameters pp (dbKey);
        pp.addParameter(dbParam(dbKey, "t_dbparameters", "parameter" , dbParam::EQUAL_TO , db::toSQL(dbKey, "circ_max_ret")));
        selectSQL.addORWhereParameter(pp) ;
    }


    dbQuery query(conn, selectSQL, "SEL def max ret") ;

    dbSet dbset ;  
    query.execForSet(dbset) ;
	dbRow myRow(dbKey) ;  
	if( dbset.getNextRow(myRow) ) {
        // Got 
        myRow.column(0,retVal);
	}
    myRow.column(0,retVal);

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Default circ retry counter is " << retVal);

    return retVal;
}

void ImplicitCirculationConfigurer::retrieveRetryConfiguration (rsResourceSet& rs, const string& baseKey, RetryConfiguration& conf, bool mandatory) const throw()
{
    // Check if the section exists and if mandatory warn if does not exists
    if (!rs.hasKey (baseKey))
    {
        if (mandatory)
        {
            ACS_LOG_WARNING ("Mandatory circulation retry configuration section <" << baseKey << "> does not exists. Defaults will apply. See details hereafter in the log.");
        }
    }
    else
    {
        rsPushNSpace pusher (rs, baseKey);

        // Constants
        static const string retryAlgoKey ("RetryAlgo");
        static const string maxRetriesKey ("MaxRetries");
        static const string retryTimeoutKey ("RetryTimeout");
        static const string maxRetryTimeoutKey ("MaxRetryTimeout");
        static const string step1TimeoutKey ("Step1Timeout");
        static const string step2TimeoutKey ("Step2Timeout");
        static const string step1ThresholdKey ("Step1Threshold");

        // parking variables
        string aString;
        unsigned long aLong = 0 ;
        unsigned int aInt = 0 ;

        // Retry Algo
        if (rs.hasKey (retryAlgoKey) )
        {
            rs.getValue (retryAlgoKey, aString);
            conf.algo = String2RetryAlgo (aString);
        }
        else
        {
            // Warn about it
            ACS_LOG_WARNING ("Configuration key " << baseKey << "." << retryAlgoKey << " not found. Using default value " << RetryAlgo2String (conf.algo));
        }

        // Max Retries
        if (rs.hasKey (maxRetriesKey) )
        {
            rs.getValue (maxRetriesKey, aInt);
            conf.maxRetries = aInt;
        }
        else
        {
            // Warn about it
            ACS_LOG_WARNING ("Configuration key " << baseKey << "." << maxRetriesKey << " not found. Using default value " << conf.maxRetries );
        }

        // Retry Timeout applies only to LINEAR and EXP2LIMITED
        if (conf.algo != STEPPED)
        {
            if (rs.hasKey (retryTimeoutKey) )
            {
                rs.getValue (retryTimeoutKey, aLong);
                conf.retryTimeout = aLong;
            }
            else
            {
                // Warn about it
                ACS_LOG_WARNING ("Configuration key " << baseKey << "." << retryTimeoutKey << " not found. Using default value " << conf.retryTimeout );
            }
        }        

        // Max Retry Timeout applies only to EXP2LIMITED
        if (EXP2LIMITED == conf.algo)
        {
            if (rs.hasKey (maxRetryTimeoutKey) )
            {
                rs.getValue (maxRetryTimeoutKey, aLong);
                conf.maxRetryTimeout = aLong;
            }
            else
            {
                // Warn about it
                ACS_LOG_WARNING ("Configuration key " << baseKey << "." << maxRetryTimeoutKey << " not found. Using default value " << conf.maxRetryTimeout );
            }
        }        

        // Step1, Step2 and StepThreshold apply only to STEPPED
        if (STEPPED == conf.algo)
        {
            // Step1
            if (rs.hasKey (step1TimeoutKey) )
            {
                rs.getValue (step1TimeoutKey, aLong);
                conf.step1Timeout = aLong;
            }
            else
            {
                // Warn about it
                ACS_LOG_WARNING ("Configuration key " << baseKey << "." << step1TimeoutKey << " not found. Using default value " << conf.step1Timeout );
            }
            // Step2
            if (rs.hasKey (step2TimeoutKey) )
            {
                rs.getValue (step2TimeoutKey, aLong);
                conf.step2Timeout = aLong;
            }
            else
            {
                // Warn about it
                ACS_LOG_WARNING ("Configuration key " << baseKey << "." << step2TimeoutKey << " not found. Using default value " << conf.step2Timeout );
            }
            // Step1Threshold
            if (rs.hasKey (step1ThresholdKey) )
            {
                rs.getValue (step1ThresholdKey, aInt);
                conf.step1Threshold = aInt;
            }
            else
            {
                // Warn about it
                ACS_LOG_WARNING ("Configuration key " << baseKey << "." << step1ThresholdKey << " not found. Using default value " << conf.step1Threshold );
            }
        }        
    }
}

string ImplicitCirculationConfigurer::RetryAlgo2String (RetryAlgo algo) // PRQA S 4020
{
    switch (algo)
    {
    case LINEAR: { return "LINEAR"; }
    case EXP2LIMITED: { return "EXP2LIMITED"; }
    case STEPPED:{ return "STEPPED"; }
    default:{ return "UNKNOWN"; }
    }
}

ImplicitCirculationConfigurer::RetryAlgo ImplicitCirculationConfigurer::String2RetryAlgo (const string & s) // PRQA S 4020
{
    // This will warn and return LINEAR in case of unknown strings
    if ("LINEAR" == s)
    {
        return LINEAR;
    }
    else if ("EXP2LIMITED" == s)
    {
        return EXP2LIMITED;
    }
    else if ("STEPPED" == s)
    {
        return STEPPED;
    }
    else
    {
        ACS_LOG_WARNING ("Unable to understand the retry algo " << s << ". LINEAR will be used instead");
        return LINEAR ;
    }
}

std::string ImplicitCirculationConfigurer::UnlinkFailureAction2String(ImplicitCirculationConfigurer::UnlinkFailureAction action) {
    switch (action) {
    case ON_UNLINK_FAILURE_DO_NOTHING: { return "ON_UNLINK_FAILURE_DO_NOTHING"; }
    case ON_UNLINK_FAILURE_DO_RENAME: { return "ON_UNLINK_FAILURE_DO_RENAME"; }
    case ON_UNLINK_FAILURE_DO_MOVE: { return "ON_UNLINK_FAILURE_DO_MOVE"; }
    default: { return "UNKNOWN"; }
    }
}

ImplicitCirculationConfigurer::UnlinkFailureAction ImplicitCirculationConfigurer::String2UnlinkFailureAction(const std::string& s) {
    if (StringUtils::equalsNoCase(s, "ON_UNLINK_FAILURE_DO_NOTHING")) {
        return ImplicitCirculationConfigurer::ON_UNLINK_FAILURE_DO_NOTHING;
    }
    else if (StringUtils::equalsNoCase(s, "ON_UNLINK_FAILURE_DO_RENAME")) {
        return ImplicitCirculationConfigurer::ON_UNLINK_FAILURE_DO_RENAME;
    }
    else if (StringUtils::equalsNoCase(s, "ON_UNLINK_FAILURE_DO_MOVE")) {
        return ImplicitCirculationConfigurer::ON_UNLINK_FAILURE_DO_MOVE;
    }
    else {
        ACS_LOG_WARNING ("Unable to understand the on failure action " << s << ". ON_UNLINK_FAILURE_DO_NOTHING will be used instead");
        return ImplicitCirculationConfigurer::ON_UNLINK_FAILURE_DO_NOTHING ;
    }
}

std::string ImplicitCirculationConfigurer::UnlinkFailureRenameStrategyMode2String(ImplicitCirculationConfigurer::UnlinkFailureRenameStrategyMode mode) {
    switch (mode) {
    case ImplicitCirculationConfigurer::PREFIX: { return "PREFIX"; }
    case ImplicitCirculationConfigurer::SUFFIX: { return "SUFFIX"; }
    default: { return "UNKNOWN"; }
    }
}

ImplicitCirculationConfigurer::UnlinkFailureRenameStrategyMode ImplicitCirculationConfigurer::String2UnlinkFailureRenameStrategyMode(const std::string& s) {
    if (StringUtils::equalsNoCase(s, "PREFIX")) {
        return ImplicitCirculationConfigurer::PREFIX;
    }
    else if (StringUtils::equalsNoCase(s, "SUFFIX")) {
        return ImplicitCirculationConfigurer::SUFFIX;
    }
    else {
        ACS_LOG_WARNING ("Unable to understand the on failure rename strategy mode " << s << ". PREFIX  will be used instead");
        return ImplicitCirculationConfigurer::PREFIX ;
    }
}

string ImplicitCirculationConfigurer::LocalTransferMode2String (curl::CirculationCurl::LocalTransferMode ltmode) // PRQA S 4020
{
	string s;
   	switch (ltmode)
    {
    	case curl::CirculationCurl::COPY: 
		{ 
			s="COPY"; 
			break;
		}
    	case curl::CirculationCurl::HARDLINK: 
		{	
			s="HARDLINK"; 
			break;
		}
    	case curl::CirculationCurl::SYMLINK:
		{ 
			s="SYMLINK"; 
			break;
		}
    	default:
		{ 
			ostringstream exMsg;
			exMsg << "Unrecognized local transfer mode " << static_cast<unsigned int>(ltmode); // PRQA S 3081 2
			ACS_THROW( exIllegalValueException ( exMsg.str() ) ); 
		}
    }
	return s;

}

curl::CirculationCurl::LocalTransferMode ImplicitCirculationConfigurer::String2LocalTransferMode (const string & s) // PRQA S 4020
{
    if ( StringUtils::equalsNoCase("COPY", s) )
    {
        return curl::CirculationCurl::COPY;
    }
    else if ( StringUtils::equalsNoCase("HARDLINK", s) )
    {
        return curl::CirculationCurl::HARDLINK;
    }
    else if ( StringUtils::equalsNoCase("SYMLINK", s) )
    {
        return curl::CirculationCurl::SYMLINK;
    }
    else
    {
        ACS_THROW( exIllegalValueException("Unrecognized local transfer mode: \"" + s + '\"') ); // PRQA S 3081
    }
}

void ImplicitCirculationConfigurer::checkLocalTransferModeString(const string & s)
{
	curl::CirculationCurl::LocalTransferMode ltmode = String2LocalTransferMode(s);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Local transfer mode \"" << s << "\" -> " << static_cast<unsigned int>(ltmode)); // PRQA S 3081
}


void ImplicitCirculationConfigurer::dump() const {

    // PRQA S 4400 L1
    ostringstream os;
    os << "\n";
    os << left << setw(50) << "--------------------------------------------      \n";
    os << left << setw(50) << "Implicit Circulation configuration                \n";
    os << left << setw(50) << "--------------------------------------------      \n";
    os << left << setw(50) << "Local Host Name (both for Service and Agent):" << right << setw(30) << _hostName << "\n";
    os << left << setw(50) << "Circulator remove sources:" << right << setw(30) << _circulatorRemoveSources << "\n";
    os << left << setw(50) << "Circulator number of (local) threads:" << right << setw(30) << _circulatorNumberOfThreads << "\n";
    os << left << setw(50) << "Circulator number of remote threads:" << right << setw(30) << _circulatorNumberOfRemoteThreads << "\n";
    os << left << setw(50) << "Circulator Reprocess Interval:" << right << setw(30) << _policiesReprocessInterval << " [s]\n";
    os << left << setw(50) << "Circulator Polling period:" << right << setw(30) << _circulatorPollingPeriod << " [s]\n";
    os << left << setw(50) << "Circulator Creation Time Factor:" << right << setw(30) << _circulatorCreationTimeWeight << "\n";
    os << left << setw(50) << "Use inotify:" << right << setw(30) << boolalpha << _useInotify << "\n";
    ostringstream lspeedstr;
    if (0 ==_linkSpeed)
    {
       lspeedstr << "unlimited";
    }
    else
    {
       lspeedstr << _linkSpeed;
    }
    os << left << setw(50) << "Link speed: " << right << setw(30) << lspeedstr.str() << " [Mbit/s]\n";
    os << left << setw(50) << "--------------------------------------------        \n";
    os << left << setw(50) << "Retry Configuration Settings                        \n";
    os << left << setw(50) << "(Algo, RetryTimeout, MaxRetryTimeout, Step1Timeout, Step2Timeout, Step1Threshold, MaxRetries)\n";
    os << left << setw(50) << "--------------------------------------------         \n";
    dumpRetryConfig (os, getImplicitRetryConfiguration(), "Implicit");
    dumpRetryConfig (os, getExplicitRetryConfiguration(), "Explicit");
    dumpRetryConfig (os, getDynamicRetryConfiguration(), "Dynamic");
    // PRQA L:L1
    ACS_LOG_PRIVINFO(os.str()) ;
}

void ImplicitCirculationConfigurer::dumpRetryConfig (ostringstream& os, const RetryConfiguration& conf, const string& mode) const
{
    os << "Mode " << left << setw (9) << mode << " -> ("; // PRQA S 4400 8
    os << right << setw (12) << RetryAlgo2String (conf.algo) << ", ";
    os << right << setw (10) << conf.retryTimeout << ", ";
    os << right << setw (10) << conf.maxRetryTimeout << ", ";
    os << right << setw (10) << conf.step1Timeout << ", ";
    os << right << setw (10) << conf.step2Timeout << ", ";
    os << right << setw (5) << conf.step1Threshold << ", ";
    os << right << setw (5) << conf.maxRetries << ") \n";
}

// PRQA S 4120 L2
//
//Gettes/Setters section.
//
bool ImplicitCirculationConfigurer::getCirculatorRemoveSources() const throw(){
    return _circulatorRemoveSources;
}
size_t ImplicitCirculationConfigurer::getCirculatorNumberOfThreads() const throw(){
    return _circulatorNumberOfThreads;
}
size_t ImplicitCirculationConfigurer::getCirculatorNumberOfLocalThreads() const throw(){
    return getCirculatorNumberOfThreads();
}
size_t ImplicitCirculationConfigurer::getCirculatorNumberOfRemoteThreads() const throw(){
    return _circulatorNumberOfRemoteThreads;
}

time_t ImplicitCirculationConfigurer::getCirculatorReprocessInterval() const throw(){
    return _policiesReprocessInterval;
}
time_t ImplicitCirculationConfigurer::getCirculatorPollingPeriod() const throw(){
    return _circulatorPollingPeriod;
}
size_t ImplicitCirculationConfigurer::getCirculatorCreationTimeWeight() const throw(){
    return _circulatorCreationTimeWeight;
}
string const& ImplicitCirculationConfigurer::getHostName() const throw(){
    return _hostName;
}
string const& ImplicitCirculationConfigurer::getMediaReportFilename() const throw(){
    return _mediaReportFilename;
}
string const& ImplicitCirculationConfigurer::getMediaSignerType() const throw(){
    return _mediaSignerType;
}
bool const& ImplicitCirculationConfigurer::useInotify() const throw() {
	return _useInotify;
}
int const& ImplicitCirculationConfigurer::getLinkSpeed() const throw() {
	return _linkSpeed;
}
const string& ImplicitCirculationConfigurer::getAllowedCiphers()const throw(){return _allowedCiphers;}

time_t ImplicitCirculationConfigurer::getUpdateDelta()const throw(){return _updateDelta ;}

time_t ImplicitCirculationConfigurer::getMediaCirculationInfoPeriod()const throw(){
	return _mediaCirculationInfoPeriod ;
}

bool ImplicitCirculationConfigurer::getLogErrorsVerbose() const throw() {
	return _logErrorsVerbose;
}
bool ImplicitCirculationConfigurer::getFilterRecoverableError() const throw() {
	return _filterRecoverableErrors;
}
ImplicitCirculationConfigurer::UnlinkFailureAction ImplicitCirculationConfigurer::getUnlinkFailureAction() const throw() {
    return _unlinkFailureAction;
}

ImplicitCirculationConfigurer::UnlinkFailureRenameStrategy ImplicitCirculationConfigurer::getUnlinkFailureRenameStrategy() const throw() {
    return _unlinkFailureRenameStrategy;
}
std::string ImplicitCirculationConfigurer::getLocalUnlinkFailureTrashPath() const throw() {
	return _localUnlinkFailureTrashPath;
}

// PRQA L:L2

// PRQA S 4121 L3
void ImplicitCirculationConfigurer::setCirculatorRemoveSources(bool removeSources) throw(){
    _circulatorRemoveSources = removeSources;
}
void ImplicitCirculationConfigurer::setCirculatorPriorityPlainOrder(bool priorityPlainOrder) throw(){
    _circulatorPriorityPlainOrder = priorityPlainOrder;
}

bool ImplicitCirculationConfigurer::getCirculatorPriorityPlainOrder() const throw() // PRQA S 4120
{
    return _circulatorPriorityPlainOrder;
}

void ImplicitCirculationConfigurer::setCirculatorNumberOfThreads(size_t threads) throw(){
    _circulatorNumberOfThreads = threads;
}
void ImplicitCirculationConfigurer::setCirculatorNumberOfLocalThreads(size_t threads) throw(){
    setCirculatorNumberOfThreads (threads);
}
void ImplicitCirculationConfigurer::setCirculatorNumberOfRemoteThreads(size_t threads) throw(){
    _circulatorNumberOfRemoteThreads = threads;
}

void ImplicitCirculationConfigurer::setCirculatorReprocessInterval(time_t time) throw(){
    _policiesReprocessInterval = time;
}
void ImplicitCirculationConfigurer::setCirculatorPollingPeriod(time_t time) throw() {
    _circulatorPollingPeriod = time;
}
void ImplicitCirculationConfigurer::setCirculatorCreationTimeWeight(size_t factor) throw(){
    _circulatorCreationTimeWeight = factor;
}
void ImplicitCirculationConfigurer::setHostName(string const& hostname) throw(){
    _hostName = hostname;
}
void ImplicitCirculationConfigurer::setMediaReportFilename(string const& mediaReportFilename) throw(){
    _mediaReportFilename = mediaReportFilename;
}
void ImplicitCirculationConfigurer::setMediaSignerType(string const& mediaSignerType) throw(){
    _mediaSignerType = mediaSignerType;
}
void ImplicitCirculationConfigurer::setInotify(bool v) throw() {
	_useInotify = v;
}
void ImplicitCirculationConfigurer::setLinkSpeed(int sp) throw() {
	_linkSpeed = sp;
}

ImplicitCirculationConfigurer::RetryConfiguration ImplicitCirculationConfigurer::getImplicitRetryConfiguration() const throw()  // PRQA S 4120
{
    return _implicitRetryConf;
}

ImplicitCirculationConfigurer::RetryConfiguration ImplicitCirculationConfigurer::getExplicitRetryConfiguration() const throw() // PRQA S 4120
{
    return _explicitRetryConf;
}

ImplicitCirculationConfigurer::RetryConfiguration ImplicitCirculationConfigurer::getDynamicRetryConfiguration() const throw() // PRQA S 4120
{
    return _dynamicRetryConf;
}

void ImplicitCirculationConfigurer::setImplicitRetryConfiguration(const ImplicitCirculationConfigurer::RetryConfiguration& c) throw()
{
    _implicitRetryConf = c;
}

void ImplicitCirculationConfigurer::setExplicitRetryConfiguration(const ImplicitCirculationConfigurer::RetryConfiguration& c) throw()
{
    _explicitRetryConf = c;
}

void ImplicitCirculationConfigurer::setDynamicRetryConfiguration(const ImplicitCirculationConfigurer::RetryConfiguration& c) throw()
{
    _dynamicRetryConf = c;
}
void ImplicitCirculationConfigurer::setAllowedCiphers(const std::string &ac){_allowedCiphers = ac;}

void ImplicitCirculationConfigurer::setUpdateDelta(time_t ud){_updateDelta = ud ; }

void ImplicitCirculationConfigurer::setMediaCirculationInfoPeriod(time_t v)
{
	_mediaCirculationInfoPeriod = v ; 
}


void ImplicitCirculationConfigurer::setLogErrorsVerbose(bool v)
{
	_logErrorsVerbose = v;
}


string ImplicitCirculationConfigurer::getLocalTransferMode() const // PRQA S 4120
{
	return _localTransferMode;
}

void ImplicitCirculationConfigurer::setLocalTransferMode(const string &s)
{
	_localTransferMode = s;
}

time_t ImplicitCirculationConfigurer::getLocalSourceFilesRetrieveDTime() const throw()
{
	return _localSourceFilesRetrieveDTime;
}

void ImplicitCirculationConfigurer::setLocalSourceFilesRetrieveDTime( const time_t & t )
{
	_localSourceFilesRetrieveDTime = t;
}

size_t ImplicitCirculationConfigurer::getLocalItemsToSkipMaxNum() const throw()
{
	return _localItemsToSkipMaxNum;
}

void ImplicitCirculationConfigurer::setLocalItemsToSkipMaxNum(const size_t & v) 
{
	_localItemsToSkipMaxNum = v;
}


// PRQA L:L3

namespace { // unnamed
    class CirculationConfigurerLoader {
    public:
        CirculationConfigurerLoader(){
            CirculationConfigurerSingleton::instance();
        }
    } _CirculationConfigurerLoader;

}

_ACS_END_NESTED_NAMESPACE
