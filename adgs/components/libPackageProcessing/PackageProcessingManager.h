// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageProcessing$

	$Id$

	$Author$

	$Log$
	Revision 1.17  2017/05/31 16:36:23  nicvac
	OPMAN-260: Fixed check for existing processing.
	
	Revision 1.16  2017/05/29 17:53:24  nicvac
	OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
	
	Revision 1.15  2017/03/03 09:43:41  marfav
	S3MPM-874
	Aligned to libException Rev_5_93 Condition Interface
	
	Revision 1.14  2017/02/02 10:19:58  davide.tiriticco
	S2PDGS-1655: Library aligned with packageId type update from string to dbPersistent::IdType
	
	Revision 1.13  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.12  2016/05/10 14:44:56  marfav
	APF-376 removing references to the ExpWaitingAckDataPackage structure
	ExpProcInfo is systematically wrote in the database
	
	Revision 1.11  2014/09/18 16:44:27  nicvac
	S1PDGS-31550 implementation.
	
	Revision 1.10  2014/04/18 15:41:03  nicvac
	S2PDGS-603: Scheduler log filtering.
	
	Revision 1.9  2013/11/04 10:00:33  marpas
	avoiding useless macros
	
	Revision 1.8  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.7  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.6  2013/05/20 14:01:46  nicvac
	Threads synchronization improved.
	
	Revision 1.5  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.4  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.3  2012/11/16 15:26:27  nicvac
	Implementation
	
	Revision 1.2  2012/11/12 17:43:56  nicvac
	Interface updated.
	
	Revision 1.1.1.1  2012/11/12 17:03:12  nicvac
	Import libPackageProcessing
	
        
*/

#ifndef _PackageProcessingManager_H_
#define _PackageProcessingManager_H_

#include <PackageProcessor.h>
#include <ExporterCommon.h>
#include <DataPackage.h>

#include <Thread.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Process the received packages.
* 
* For each received package to process:
* - start a processor on the package
* - query the processor's status
* 
* Return to the client the distributed and failed packages (successfully and error processing).
**/

class PackageProcessingManager: public Thread  { 
public:

	/** PackageProcessingManager Exceptions */
	exDECLARE_EXCEPTION(exPackageProcessingManagerException, exException) ; // Base PackageProcessingManager Exception.
	exDECLARE_EXCEPTION(exPackageProcessingManagerCriticalException, exPackageProcessingManagerException) ; // Critical Exception.


	/** Default Class constructor */
	PackageProcessingManager();
	/** Destructor */
	virtual ~PackageProcessingManager() throw();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageProcessingManager(const PackageProcessingManager & );
	/** Operator = */
	PackageProcessingManager &operator=(const PackageProcessingManager &);
	

public:
	/** Add Packages to process to this consumer.
	 * - failedPackages: list of packages on which cannot initialize processing */
	void addPackagesToProcess( std::vector<DataPackageSptr> const& packages, std::vector<DataPackageSptr>& failedPackages);

	/** Add Packages to check to this consumer.
	 * - failedPackagesIds: list of package ids on which cannot initialize processing */
	void addPackagesToCheck( std::vector<dbPersistent::IdType> const& packagesIds, std::vector<dbPersistent::IdType>& failedPackagesIds);
	
	/** Retrieves the success packages and errored packages. */
	void getPackages( std::vector<DataPackageSptr>& distributedPackages, 
					  std::vector<DataPackageSptr>& failedPackages,
					  std::vector<DataPackageSptr>& waitingAckPackages );

	/** Return the number of pending processing */
	size_t getNumOfPendingRequests() const;

protected:
	/** Thread interface implementation */
	virtual void run();
	virtual void halt(StopController::HaltLevel l, bool joinAlso=true);

private:

	/** Support for public methods addPackagesTo* */
	template <class PackageProcessorT, class PackageProcessorInitT>
	void addPackagesToRun( std::vector<PackageProcessorInitT> const& packages, std::vector<PackageProcessorInitT>& failedPackages);

	/** The package is already in the current processing list */
	bool isInCurrentProcessingList( dbPersistent::IdType const& packageId ) const;
	bool isInCurrentProcessingList( DataPackageSptr const& package ) const;


	/** Sort the Data Packages according to priority. */
	void sortPackageProcessors();
	
	/** Query the  package processors to update:
	- the emptyProcessingSlots
	- the goodPackages
	- the failedPackages 
	- the waitingAckPackages */
	void updateFromPackageProcessorsStatus();

	/** Check the type of the cancel request Vs pending work.
	 * True: end my work as soon as possible */
	bool endMyJobNow();

private:

	/** Condition for the standby.
	 * Note: here Thread::waitExitOrTimeout is not enought because this condition can be also notified from other Threads (e.g. for package addition request)
	 *  */
	Condition _standbyCondition;

	/** Wake up after this time interval. Even if nobody notifies you on the standby condition.  [msec] 
	 * (initialized by configuration)
	 * The processing manager is woken up by event (Processor ends, request for a new package...).
	 * But, anyway wake up after this time interval to prevent hanging. */
	unsigned long _pollingTimerMsec;

	/** Current number of available slots for package processing (initialized by configuration). */
	int _emptyProcessingSlots;

	/** Running package processors. */
	std::deque<PackageProcessorSptr> _runningPackageProcessors;

	/** Set of data package to process. */
	std::deque<PackageProcessorSptr> _queuedPackageProcessors;
	
	/** DataPackages successfully processed. */
	std::deque<DataPackageSptr> _distributedPackages;
	
	/** DataPackages processing in error or aborted. */
	std::deque<DataPackageSptr> _failedPackages;

	/** Chek later for these DataPackages */
	std::deque<DataPackageSptr> _waitingAckPackages;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageProcessingManager)

};


//Template implementations
template <class PackageProcessorT, class PackageProcessorInitT>
void PackageProcessingManager::addPackagesToRun( std::vector<PackageProcessorInitT> const& packages, std::vector<PackageProcessorInitT>& failedPackages) {

	if ( ! this->requestedCancel() ) {

		{
			Lock lock( _standbyCondition );

			size_t countNewProcessing(0);
			size_t countErrorInit(0);
			size_t countAlreadyRunning(0);
			for ( typename std::vector<PackageProcessorInitT>::const_iterator it = packages.begin(); it!=packages.end(); ++it ) {

				if ( ! this->isInCurrentProcessingList( *it ) ) {

					//Set up the package processing and queue it
					PackageProcessorSptr packageProcessorSptr(
						dynamic_cast<PackageProcessor*> (new PackageProcessorT) );

					try {
		                packageProcessorSptr->init( *it );
						packageProcessorSptr->setAvailableSlotCondition( &_standbyCondition );

						_queuedPackageProcessors.push_back( packageProcessorSptr );
						++countNewProcessing;

					} catch (std::exception& ex) {

						failedPackages.push_back( *it );
						++countErrorInit;

						ACS_LOG_NOTIFY_EX(ex);
					}

				} else {
					++countAlreadyRunning;
				}

			}

			//Sort current. Note: even if input is sorted by client, here some new request can be promoted in the queue
			this->sortPackageProcessors();

			ACS_LOG_INFO("PackageProcessingManager::addPackagesToRun: received "<<countNewProcessing<<" new packages to be processed.");
			if (countErrorInit > 0) {
				ACS_LOG_INFO("PackageProcessingManager::addPackagesToRun: failed initialization of "<<countErrorInit<<" new packages to be processed.");
			}
			if (countAlreadyRunning > 0) {
				ACS_LOG_INFO("PackageProcessingManager::addPackagesToRun: ignored "<<countAlreadyRunning<<" packages because already in current processing list.");
			}

		}

		{//Notify something to be processed
			ThreadSafe::Lock conditionLock(_standbyCondition);
			_standbyCondition.notify_all();
		}

	} else {
		ACS_LOG_WARNING("PackageProcessingManager::addPackagesToRun: Request to export packages rejected. Stop received on Package Exporter.");
	}
}

_ACS_END_NAMESPACE

#endif //_PackageProcessingManager_H_

