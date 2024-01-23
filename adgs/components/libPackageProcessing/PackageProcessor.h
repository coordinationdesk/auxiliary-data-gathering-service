/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
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
	Revision 1.22  2017/02/02 10:19:58  davide.tiriticco
	S2PDGS-1655: Library aligned with packageId type update from string to dbPersistent::IdType
	
	Revision 1.21  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.20  2016/05/10 14:44:56  marfav
	APF-376 removing references to the ExpWaitingAckDataPackage structure
	ExpProcInfo is systematically wrote in the database
	
	Revision 1.19  2013/11/06 13:07:02  nicvac
	Simplified Scheduler and processing status handling.
	
	Revision 1.18  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.17  2013/05/20 14:01:46  nicvac
	Threads synchronization improved.
	
	Revision 1.16  2013/03/21 14:44:28  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.15  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.14  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.13  2013/03/05 17:41:05  nicvac
	Retry policy implementation.
	
	Revision 1.12  2013/02/22 17:38:22  nicvac
	S2PDGS-186: mail notification implemented.
	
	Revision 1.11  2013/01/16 17:34:05  nicvac
	PackageProcessingManager waits on PackageProcessor only if this notifies.
	
	Revision 1.10  2013/01/09 14:14:03  nicvac
	Fixed getStatus
	
	Revision 1.9  2012/12/12 16:19:55  nicvac
	ProcInfo, no JobOrder, setUp from Db.
	
	Revision 1.8  2012/12/06 18:35:17  nicvac
	Removed dependency from libDataConsumer.
	
	Revision 1.7  2012/11/30 14:16:32  nicvac
	Implementation.
	
	Revision 1.6  2012/11/27 14:44:17  nicvac
	Implementation.
	
	Revision 1.5  2012/11/22 18:06:09  nicvac
	Implementation.
	
	Revision 1.4  2012/11/21 17:45:47  nicvac
	Implementation.
	
	Revision 1.3  2012/11/19 17:23:50  nicvac
	Implementing.
	
	Revision 1.2  2012/11/16 15:26:27  nicvac
	Implementation
	
	Revision 1.1.1.1  2012/11/12 17:03:12  nicvac
	Import libPackageProcessing
	
        
*/

#ifndef _PackageProcessor_H_
#define _PackageProcessor_H_

#include <ExporterCommon.h>

#include <ExpProcInfo.h>

#include <DataPackage.h>

#include <Thread.h>
#include <ThreadSafe.h>

#include <dbPersistent.h>

#include <exException.h>

#include <boost/shared_ptr.hpp>

namespace acs {


/**
* \brief
* Process the received package.
* 
* For the received package to process:
* - identify the right processing to be performed (task table selection according to the package content)
* - run a Scheduler instance on the identified task table.
* 
* Return to the client the execution status.
**/

class PackageProcessor: public Thread { 
public:

	/** PackageProcessor Exceptions */
	exDECLARE_EXCEPTION(exPackageProcessorException, exException) ; // Base PackageProcessor Exception.
	exDECLARE_EXCEPTION(exPackageProcessorCriticalException, exPackageProcessorException) ; // Critical Exception.


	/** Default Class constructor */
	PackageProcessor();
	/** Destructor */
	virtual ~PackageProcessor() throw();



private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageProcessor(const PackageProcessor & );
	/** Operator = */
	PackageProcessor &operator=(const PackageProcessor &);

public:

	typedef enum {
		Undefined=-1, 	//NOT DEFINED
		Success,		//The Processor (Task table) execution was ok
		Error			//The Processor (Task table) execution was not ok
	} ProcStatus;

	/** Init the object */
	virtual void init( dbPersistent::IdType const& dataPackageId ) = 0;
	virtual void init( DataPackageSptr dataPackageSptr ) = 0;

	/** get the datapackage related to this processing. */
	DataPackageSptr getDataPackage() const;
	
	/** Set the available slot condition attribute. */
	void setAvailableSlotCondition( Condition* availableSlotConditionPtr );
	
	/** Return the status of the data package processing: COMPLETED, NOT COMPLETED. Proc status is also returned (Undefined, Success, ...) */
	bool getStatus(ProcStatus& code);

	/** true if the processing has been performed */
	bool isPerformed() const;

	/** true if this Processor needs a later check for distribution status.
	 * If true returns the parameters that will be used for the later check on distribution status.
	 * To call accordingly with getStatus */
	bool isWaitingAcknoledge() const;

	/** Updates the parameters in the database queue */
	void updateParametersOnDb () const;

protected:
	/** Thread interface implementation */
	virtual void run();
	virtual void halt(StopController::HaltLevel l, bool joinAlso=true);

private:
	// Returns the actual expProcInfo path if available
	virtual std::string getExpProcInfoPath() const;

	// Returns the package id if available
	virtual dbPersistent::IdType getPackageId() const;	


protected:

    /** Instance Setup (e.g. access to DB to init) */
    virtual void setUp() = 0;

	/** Prepare the working dir */
	virtual void setUpWorkingDir() = 0;
	
	/** Remove the working dir */
	void deleteWorkingDir() const;

	/** Send the Notification about the delivery
	 * param success: success or failure notification */
	void handleNotification(bool const& success) const;


protected:

	/** To notify when a slot has been freed. */
	Condition* _availableSlotConditionPtr;

	/** Run called and performed */
	bool _isPerformed;

	/** The datapackage to process */
	DataPackageSptr _dataPackageSptr;

	/** The processing status */
	ProcStatus _status;

	/** Current retry info */
	int _retryCurrent;
	int _retryThreshold;

	/** Processing Parameter related to the distribution rule */
	std::string _parameters;

	/** The working directory in which the package processor runs. */
	std::string _workingDir;
	
	/** List of processing steps to do on each package. To be filled by configuration. */
	std::vector<std::string> _processingSteps;

	/** Map of processing step - task table filename */
	std::map<std::string, std::string> _ttMap;

	/** The object collecting the export processing I/O */
	ExpProcInfo _currentExpProcInfo;

	/** Internal filetypes to refer I/O */
	std::string _fileTypeExpProcInfo;
	std::string _fileTypeExpDatapackage;
	std::string _fileTypeExpDistribution;
	std::string _fileTypeExpParameters;

	/** Processor interface Files: Filetype - Filename */
	std::map<std::string, std::string> _procFileIF;

	/** At the end of processing, if I need a later check for distribution status, I set this field 
	 * E.g.: Distribution left to asynch service. Check later the actual distribution result. */
	bool _isWaitingAcknoledge;

	dbPersistent::IdType _packageId;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageProcessor)

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<PackageProcessor> PackageProcessorSptr;
typedef boost::shared_ptr<const PackageProcessor> PackageProcessorConstSptr;


}

#endif //_PackageProcessor_H_

