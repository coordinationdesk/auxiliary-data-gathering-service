// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2019, Advanced Computer Systems , Inc.
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
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.17  2016/07/26 09:32:25  lucio.pulvirenti
 *  S1PSGS-31765: local transfer mode added to IF to be got by configuration. Set and get methods added.
 *  Conversion methods between local transfer mode as a string and as an enume defined in libFtpCurl added.
 *
 *  Revision 5.16  2015/11/16 16:27:36  lucio.pulvirenti
 *  S2PDGS-1373: logErrorVerbose tag introduced in configuration and managed with proper attribute and get/set methods.
 *
 *  Revision 5.15  2015/09/04 14:58:39  lucio.pulvirenti
 *  S2PDGS-1247: Media circulation records occurrence info period added to IF.
 *
 *  Revision 5.14  2015/03/10 11:35:51  marfav
 *  S2PDGS-1110 Added the function to manage local and remote repositories in different controlling threads
 *
 *  Revision 5.13  2015/02/19 14:56:52  francesco.avanzi
 *  getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
 *
 *  Revision 5.12  2014/09/10 16:01:55  marfav
 *  Default band limit set to unlimited (0 bps)
 *
 *  Revision 5.11  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.10  2014/03/13 11:10:36  francesco.avanzi
 *  PriorityPlainOrder is now taken from configuration file and directily pushed into the BandShaperSingleton
 *
 *  Revision 5.9  2014/02/17 16:14:55  lucio.pulvirenti
 *  Link speed default value now 100 Mbit/s. Data type now int.
 *
 *  Revision 5.8  2014/02/17 13:18:48  lucio.pulvirenti
 *  Link speed [GB/s] added with set and get methods.
 *
 *  Revision 5.7  2014/01/14 11:34:14  marfav
 *  S1PDGS-31350: default max retry read from database
 *
 *  Revision 5.6  2014/01/09 17:25:20  marfav
 *  Support for retry configuration in progress - S1PDGS-31034
 *
 *  Revision 5.5  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/08/05 12:45:43  lucio.pulvirenti
 *  _circulatorPollingPeriod and _useInotify attributes with the proper set and get methods added.
 *  CIRCULATOR_POLLING_PERIOD define added to get _circulatorPollingPeriod.
 *
 *  Revision 5.3  2013/08/02 16:34:20  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.2  2013/07/23 17:14:26  marpas
 *  fixing types
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.11  2013/01/25 13:47:36  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.10  2012/04/03 10:11:06  micmaz
 *  work in progress
 *
 *  Revision 1.9  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.8  2012/03/15 16:49:14  micmaz
 *  work in progress
 *
 *  Revision 1.7  2012/03/15 14:34:29  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.6  2012/03/09 17:43:34  micmaz
 *  added media functionality
 *
 *  Revision 1.5  2012/03/08 13:20:45  micmaz
 *  added media functionality
 *
 *  Revision 1.4  2012/02/28 17:17:16  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.3  2012/02/07 10:25:29  micmaz
 *  removed warnings.
 *
 *  Revision 1.2  2011/11/03 16:43:26  micmaz
 *  IN PROGRESS - issue S1PDGS-2343: CirculationService and DataStorageService hostname problem
 *  http://jira.acsys.it/browse/S1PDGS-2343
 *
 *  Revision 1.1  2011/10/28 13:13:24  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *
 *
 */

#ifndef IMPLICITCIRCULATIONCONFIGURER_H_
#define IMPLICITCIRCULATIONCONFIGURER_H_

#include <acs_c++config.hpp>

#include<Singleton.hpp>
#include<CirculationCurl.h>
#include <rsResourceSet.h>

#include<string>


_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)

class ImplicitCirculationConfigurer{ // PRQA S 2109

public:

    enum RetryAlgo
    {
        LINEAR = 0,
        EXP2LIMITED,
        STEPPED
    };
    
    enum UnlinkFailureAction {
        ON_UNLINK_FAILURE_DO_NOTHING = 0,
        ON_UNLINK_FAILURE_DO_RENAME,
        ON_UNLINK_FAILURE_DO_MOVE
    };
    
    enum UnlinkFailureRenameStrategyMode {
        PREFIX=0,
        SUFFIX
    };

    struct RetryConfiguration // PRQA S 2173, 2175
    {
        RetryConfiguration() : // Default is LINEAR with 60 seconds retry period and 10 retries
            algo (LINEAR),
            retryTimeout (60), // PRQA S 4400 6
            maxRetryTimeout (600),
            step1Timeout (10),
            step2Timeout (60),
            step1Threshold(0),
            maxRetries(10)
        {}

        RetryAlgo algo; // PRQA S 2100 7
        unsigned long retryTimeout;
        unsigned long maxRetryTimeout;
        unsigned long step1Timeout;
        unsigned long step2Timeout;
        unsigned int step1Threshold;
        unsigned int maxRetries;
    };
    
    struct UnlinkFailureRenameStrategy {
        UnlinkFailureRenameStrategy():
            mode(PREFIX),
            value("tmp_")
        {}
        
        UnlinkFailureRenameStrategyMode mode;
        std::string value;
    };
    
    
    static std::string RetryAlgo2String (RetryAlgo);
    static RetryAlgo String2RetryAlgo (const std::string &);
    
    static std::string UnlinkFailureAction2String(UnlinkFailureAction);
    static UnlinkFailureAction String2UnlinkFailureAction(const std::string&);
    
    static std::string UnlinkFailureRenameStrategyMode2String(UnlinkFailureRenameStrategyMode);
    static UnlinkFailureRenameStrategyMode String2UnlinkFailureRenameStrategyMode(const std::string&);

public:
    ImplicitCirculationConfigurer();
    virtual ~ImplicitCirculationConfigurer() throw ();
private:
    ImplicitCirculationConfigurer(ImplicitCirculationConfigurer const& other);
    ImplicitCirculationConfigurer& operator=(ImplicitCirculationConfigurer const& other);
public:
    /**
     * A dump method in order to inspect the configuration parameters read from file.
     */
    virtual void dump() const;
    void dumpRetryConfig (std::ostringstream&, const RetryConfiguration&, const std::string&) const;

    /**
     * Loads the configuration parameters from file.
     */
    virtual void load();

    static void confLoadedCallback(void *) ;
    static bool onceForRun() { return _onceForRun; }
    static void setOnceForRun(bool onceForRun) { _onceForRun = onceForRun; }

    static std::string LocalTransferMode2String( curl::CirculationCurl::LocalTransferMode );
    static curl::CirculationCurl::LocalTransferMode String2LocalTransferMode(const std::string &);
    static void checkLocalTransferModeString(const std::string &);

#ifdef ACS_TEST
public:
#else
private:
#endif
    static bool _onceForRun ;

//
//Getters/Setters
//
public:
    bool getCirculatorRemoveSources() const throw();
    void setCirculatorRemoveSources(bool removeSources) throw();
    bool getCirculatorPriorityPlainOrder()const throw();
    void setCirculatorPriorityPlainOrder(bool priorityPlainOrder) throw();
    size_t getCirculatorNumberOfThreads() const throw();
    void setCirculatorNumberOfThreads(size_t threads) throw();
    size_t getCirculatorNumberOfRemoteThreads() const throw();
    void setCirculatorNumberOfRemoteThreads(size_t threads) throw();

    // Local threads settings are aliases for numberOfThreads settigns
    size_t getCirculatorNumberOfLocalThreads() const throw();
    void setCirculatorNumberOfLocalThreads(size_t threads) throw();


    time_t getCirculatorReprocessInterval() const throw();
    void setCirculatorReprocessInterval(time_t time) throw();
	time_t getCirculatorPollingPeriod() const throw();
    void setCirculatorPollingPeriod(time_t) throw();
    size_t getDelayOnFtpTransfers() const throw();
    void setDelayOnFtpTransfers(size_t delayInMillis) throw();
    size_t getCirculatorCreationTimeWeight() const throw();
    void setCirculatorCreationTimeWeight(size_t factor) throw();
    std::string const& getHostName() const throw();
    void setHostName(std::string const& hostname) throw();
    std::string const& getMediaReportFilename() const throw();
    void setMediaReportFilename(std::string const& mediaReportFilename) throw();
    std::string const& getMediaSignerType() const throw();
    void setMediaSignerType(std::string const& mediaSignerType) throw();
    bool const& useInotify() const throw();
    void setInotify(bool) throw();
    int const& getLinkSpeed() const throw();
    void setLinkSpeed(int) throw();
    const std::string& getAllowedCiphers()const throw();
    void setAllowedCiphers(const std::string &);
    time_t getUpdateDelta()const throw();
    void setUpdateDelta(time_t);
    time_t getMediaCirculationInfoPeriod()const throw();
    void setMediaCirculationInfoPeriod(time_t);
    bool getLogErrorsVerbose() const throw();
    void setLogErrorsVerbose(bool v);
    bool getFilterRecoverableError() const throw();
    UnlinkFailureAction getUnlinkFailureAction() const throw();
    UnlinkFailureRenameStrategy getUnlinkFailureRenameStrategy() const throw();
    std::string getLocalUnlinkFailureTrashPath() const throw();
    
    // S1PDGS-31034 add-ons
    RetryConfiguration getImplicitRetryConfiguration() const throw();
    RetryConfiguration getExplicitRetryConfiguration() const throw();
    RetryConfiguration getDynamicRetryConfiguration() const throw();

    void setImplicitRetryConfiguration(const RetryConfiguration&) throw();
    void setExplicitRetryConfiguration(const RetryConfiguration&) throw();
    void setDynamicRetryConfiguration(const RetryConfiguration&) throw();

    std::string getLocalTransferMode() const ;
    void setLocalTransferMode(const std::string &);
    time_t getLocalSourceFilesRetrieveDTime() const throw() ;
    void setLocalSourceFilesRetrieveDTime(const time_t &);
    size_t getLocalItemsToSkipMaxNum() const throw();
    void setLocalItemsToSkipMaxNum(const size_t &) ;

#ifdef ACS_TEST
public:
#else
private:
#endif
    void retrieveRetryConfiguration (rsResourceSet&, const std::string& baseKey, RetryConfiguration&, bool mandatory) const throw();
    unsigned int getDefaultMaxRetryFromDb () const;

#ifdef ACS_TEST
public:
#else
private:
#endif
    bool _circulatorRemoveSources;
    bool _circulatorPriorityPlainOrder;
    size_t _circulatorNumberOfThreads;
    size_t _circulatorNumberOfRemoteThreads;
    time_t _policiesReprocessInterval;
	time_t _circulatorPollingPeriod;
    size_t _circulatorCreationTimeWeight;
    std::string _hostName;
    std::string _mediaReportFilename;
    std::string _mediaSignerType;
    std::string _allowedCiphers ;
    time_t _updateDelta ;
	bool _useInotify; // if true, use inotify kernel mechanism to find if files are ready to be circulated.
	int _linkSpeed;
	time_t _mediaCirculationInfoPeriod; // period in s to log occurrence of items to be circulated via media.

    // S1PDGS-31034 add-ons
    RetryConfiguration _implicitRetryConf;
    RetryConfiguration _explicitRetryConf;
    RetryConfiguration _dynamicRetryConf;

	bool _logErrorsVerbose;
	bool _filterRecoverableErrors;	// if set to true, recoverable error messages are filtered and not logged as error
	std::string _localTransferMode;
	time_t _localSourceFilesRetrieveDTime; // delta time in s to subtract to modification/creation date of files in local repository to fix latency problems (e.g. for locally mounted remote directories)
	size_t _localItemsToSkipMaxNum ; // maximum nr of local items processed by ImplicitCirculationService::ItemService to be skipped in the following run

    UnlinkFailureAction _unlinkFailureAction;
    UnlinkFailureRenameStrategy _unlinkFailureRenameStrategy;
    std::string _localUnlinkFailureTrashPath;

public:
    // PRQA S 2100 L1
    static const std::string spaceName;
    static const bool DefaultCirculationRemoveSources = true;
    static const int DefaultCirculationThreads = 5 ;
    static const int DefaultCirculationReprocessInterval = 60 ;
	static const int DefaultCirculationPollingPeriod = 60 ;
    static const int DefaultConnPoolSize = 10 ;
    static const int DefaultCreationTimeWeight = 10 ;
    static const std::string DefaultMediaFilename;
    static const std::string DefaultMediaSignerType;
	static const int DefaultLinkSpeed = 0; //this means unlimited bandwithd
    static const std::string DefaultAllowedCiphers ;
    static const time_t DefaultUpdateDelta  = 10 * 60 ;
	static const time_t DefaultMediaCirculationInfoPeriod = 60; // seconds
    static const curl::CirculationCurl::LocalTransferMode DefaultLocalTransferMode;
	static const time_t DefaultLocalSourceFilesRetrieveDTime ; // seconds
	static const size_t DefaultLocalItemsToSkipMaxNum ;
    static const UnlinkFailureAction DefaultUnlinkFailureAction = ON_UNLINK_FAILURE_DO_NOTHING;
    // PRQA L:L1
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ImplicitCirculationConfigurer) ;
};

typedef acs::pattern::Singleton<ImplicitCirculationConfigurer> CirculationConfigurerSingleton;


_ACS_END_NESTED_NAMESPACE
/*
// PRQA S 1020 L2
//Circulator shortcuts for public properties.
#define CIRCULATOR_REMOVE_SOURCES        acs::conf::CirculationConfigurerSingleton::instance()->getCirculatorRemoveSources()
#define CIRCULATOR_THREADS               acs::conf::CirculationConfigurerSingleton::instance()->getCirculatorNumberOfThreads()
#define CIRCULATOR_SECONDS_TO_REPROCESS  acs::conf::CirculationConfigurerSingleton::instance()->getCirculatorReprocessInterval()
#define CIRCULATOR_POLLING_PERIOD  		 acs::conf::CirculationConfigurerSingleton::instance()->getCirculatorPollingPeriod()
#define CIRCULATOR_CREATION_TIME_FACTOR  acs::conf::CirculationConfigurerSingleton::instance()->getCirculatorCreationTimeFactor()
#define HOST_NAME                        acs::conf::CirculationConfigurerSingleton::instance()->getHostName()
#define MEDIA_REPORT_FILENAME            acs::conf::CirculationConfigurerSingleton::instance()->getMediaReportFilename()
#define MEDIA_SIGNER_TYPE                acs::conf::CirculationConfigurerSingleton::instance()->getMediaSignerType()
// PRQA L:L2
*/

#endif /* IMPLICITCIRCULATIONCONFIGURER_H_ */
