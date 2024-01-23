/*
    Copyright 1995-2012, Expriva SpA - DADF
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Eprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    $Prod: A.C.S. libPackageExporter$

    $Id$

    $Author$

    $Log$
    Revision 1.22  2017/03/27 13:37:56  nicvac
    APF-273: Dissemination mc messages refactoring.

    Revision 1.21  2017/02/23 16:37:50  nicvac
    S3EVOL-13: Distribution working area is not related anymore to download cache.

    Revision 1.20  2017/02/21 17:05:57  nicvac
    S3EVOL-13: Inter process cache lock mode; cache limit check

    Revision 1.19  2017/02/02 10:31:25  davide.tiriticco
    S2PDGS-1655: Application aligned to packageId type update from string to dbPersistent::IdType

    Revision 1.18  2016/12/20 11:12:56  nicvac
    S3MPM-860: PackageExporter resources improvement.

    Revision 1.17  2016/12/19 18:32:45  nicvac
    S3MPM-860: PackageExporter resources improvement.

    Revision 1.16  2016/12/16 16:56:15  nicvac
    S3MPM-860: PackageExporter resources improvement.

    Revision 1.15  2016/12/13 16:39:57  nicvac
    OPMAN-199: Reset intermediate status improved.

    Revision 1.14  2016/09/23 14:16:03  nicvac
    OPMAN-138: Improved disk space check in the frame of cryo2 evolution.

    Revision 1.13  2016/05/10 14:47:05  marfav
    APF-376 removing references to the ExpWaitingAckDataPackage structure
    the update of delserviceparameters is made by underlying PackageProcessing elements

    Revision 1.12  2014/10/03 12:17:59  nicvac
    S2PDGS-931 implementation.

    Revision 1.11  2014/09/18 16:45:38  nicvac
    S1PDGS-31550 implementation.

    Revision 1.10  2013/11/06 18:05:05  nicvac
    Plugins handling.

    Revision 1.9  2013/10/04 13:32:00  nicvac
    acs sdk warnings processed.

    Revision 1.8  2013/03/20 14:01:27  nicvac
    Improved Application Restart.

    Revision 1.7  2013/03/19 17:52:08  nicvac
    Fixed phoenix case.

    Revision 1.6  2013/03/15 10:36:59  nicvac
    S2PDGS-186
    ADC-83
    WaitingAcknowledge Step implemented

    Revision 1.5  2013/03/11 17:33:27  nicvac
    ExporterBase interface changed.

    Revision 1.4  2013/03/05 17:42:16  nicvac
    Retry policy implementation.

    Revision 1.3  2013/01/11 13:19:25  marfav
    Addedd support to external appname definition

    Revision 1.2  2012/11/12 17:44:25  nicvac
    PackageProcessingManager integrated.

    Revision 1.1.1.1  2012/11/09 13:28:07  nicvac
    Import PackageExporter


*/

#ifndef _PackageExporter_H_
#define _PackageExporter_H_

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#endif


#include <PackageProcessingManager.h>
#include <ResourceStats.h>

#include <DataProvider.h>

#include <dbPersistent.h>

#include <ConfigurablePluginLoader.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Agent in charge of exporting packages (set of items) ready for distribution.
* Handles all types of the "exporting format", such as ftp, media.
* According to the exporting format (mainly) and to other parameters related to the package a proper processing is performed.
* The actual transfer is performed exploiting the DC service.
* The DC service is invoked by a task in the processing chain.
* The processing is executed and controlled exploiting the Scheduler framework.
**/

class PackageExporter: public PROJECT_APP {
public:

    /** PackageExporter Exceptions */
    exDECLARE_EXCEPTION(exPackageExporterException, exException) ; // Base PackageExporter Exception.
    exDECLARE_EXCEPTION(exPackageExporterCriticalException, exPackageExporterException) ; // Critical Exception.


    /** Default Class constructor */
    PackageExporter(const std::string& app, const std::string& subsys);
    /** Destructor */
    virtual ~PackageExporter() throw();

    /** Run entry point */
    virtual int main(int argc, char const * const * argv,
                     char const * const * env);

    /** Signal handlers */
    virtual void customSigIntHandler() ;
    virtual void customSigTermHandler() ;

private:

    /** Core of the main Loop */
    void runExport(bool, bool, dataprovider::DataProvider &, Cache &);

    /** Retrieving failed package handling */
    void handleFailedDwnlPackages(std::vector<dataprovider::DataPackage> const& failedPackages);
    /** Suspended package handling */
    void handleSuspendedPackages(std::vector<dataprovider::DataPackage> const& suspendedPackages);
    /** Downloaded package handling */
    void handleDownloadedPackages(std::vector<dataprovider::DataPackage> const& goodPackages, std::vector<dataprovider::DataPackage>& failedPackages);

    /** Handling Packages that need acknoledge check */
    void handleWaitingAckPackages() throw();
    /** WaitingAcknoledge Degraded case. Cf. comments in implementation. */
    void handleDegradedWaitingAckPackages( std::vector<dbPersistent::IdType> const& failedPackageIds ) const;
    /** Setting status for Packages on Waiting Acknoledge */
    virtual void updateWaitingAckPackages( std::vector<dataprovider::DataPackage> const& waitingAckPackages );

    /** Processed package status retrieving */
    void getProcessedPackages(std::vector<dataprovider::DataPackage>& goodPackages,
                                      std::vector<dataprovider::DataPackage>& failedPackages,
                                      std::vector<dataprovider::DataPackage>& waitingAckPackages );

    /** Return info about the export retry of the package.
     * true: no error retrieving infos. false: error retrieving info. Defaults are returned.
     * \param dataPackage: the input datapackage
     * \param isLastChance: this is the last chance for the datapackage to be delivered (Retry policy implementation)
     * \param retryCurrent, retryTrheshold: the current retry counter and the retry threshold
     * \param elapsedSec: elapsed time from package creation to the last operation on queue.
     * Override this method for the Exporters that implement the retry policy (e.g. PackageExporter implements it) */
    bool getDistributionStats(	dbPersistent::IdType const& dataPackageId,
                                bool& isLastChance, int& retryCurrent, int& retryThreshold,
                                size_t& elapsedSec) throw();

    /** Consumer stop */
    void stopConsumer(bool const& abort);

    /** Mcf messages */
    virtual void sendMcfExitMessages() const;

    /** Load Configuration */
    virtual void loadConf();

    /** Load Configured Plugins */
    void loadPlugins();

    /** Support tools */
    void convertTime(const std::string &, double &);


private:  // copy constructor and operator= defined but not implemented
    /** Copy Constructor */
    PackageExporter(const PackageExporter & );
    /** Operator = */
    PackageExporter &operator=(const PackageExporter &);

private:

    struct timeInterval {
        double timeStart;
        double timeStop;
    };


    std::vector<timeInterval> _timeIntervals;
    std::string _productPath;
    std::string _downloadRepository;
    std::string _errorLog;
    std::string _logFullPath;
    unsigned long int _availableDownloadSpaceSize;
    bool _checkForNewProcessingOnNextMainLoop;

    /** sec to wait if no broadcast */
    size_t _pollingTime;

    /** Return my jobresponsible label*/
    std::string _jobResponsible;

    /** Overridden by configuration - [sec] */
    unsigned long _restartTime;

    /** Delay time before retry the operations after a critical failure. Overridden by configuration - [sec] */
    unsigned long _criticalErrorDelaySec;

    /** Max allowed number of Packages that wait for processing (queue lenght of {distribute, asynch check}) */
    size_t _confPendingRequestsLimit;

    /** Max allowed number of Packages in Waiting Acknlowledge status */
    size_t _confWaitingAcknowledgeLimit;

    /** Minimum disk space required to run PackageExporter */
    size_t _confMinSpaceRequiredMb;

    /** Plugin Loader */
    ConfigurablePluginLoader _pluginLoader;

    /** Pointer to the package processing manager */
    PackageProcessingManager* _packageProcessingManagerPtr;

    /** Pointer to the Resource Stats */
    ResourceStats* _resourceStatsPtr;

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageExporter)

};

_ACS_END_NAMESPACE

#endif //_PackageExporter_H_

