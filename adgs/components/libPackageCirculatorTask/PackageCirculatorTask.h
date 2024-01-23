/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageCirculatorTask$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2017/02/21 18:09:16  nicvac
	S3EVOL-13: Datapackage serialization.
	
	Revision 1.3  2015/10/09 17:58:21  nicvac
	APF-325: DB mode deprecated. Remote source for Soap mode. New Host_id circulator IF.
	
	Revision 1.2  2015/01/26 16:14:39  nicvac
	S2PDGS-1050
	
	Revision 1.1.1.1  2013/06/18 16:38:21  nicvac
	Import libPackageCirculatorTask
	
        
*/

#ifndef _PackageCirculatorTask_H_
#define _PackageCirculatorTask_H_

#include <ExpProcInfo.h>

#include <exException.h>

#include <DistributionSerializable.h>

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Class implementing Common functions for PackageProcessor Circulation Task running in Task Table.
**/

class PackageCirculatorTask  { 
public:

	/** PackageCirculatorTask Exceptions */
	exDECLARE_EXCEPTION(exPackageCirculatorTaskException, exException); // Base PackageCirculatorTask Exception.
	exDECLARE_EXCEPTION(exPackageCirculatorTaskCriticalException, exPackageCirculatorTaskException); // Critical Exception.


	/** Default Class constructor */
	PackageCirculatorTask();
	/** Destructor */
	virtual ~PackageCirculatorTask();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageCirculatorTask(const PackageCirculatorTask & );
	/** Operator = */
	PackageCirculatorTask &operator=(const PackageCirculatorTask &);
	
protected:

	/** Init from Configuration Space */
	void initFromConf();

	/** Return the Soap service Url */
	std::string getSoapServiceUrl() const;

	/** Compute the "from Url" in compact format.
	 *  by defauld the from url protocol is filesystem (because the prepared Datapackage is on disk).
	 *	If requested by configuration, the Datapackage to be uploaded will be accessed by network protocol (cf. config space). */
	std::string getSourceUrl(std::string const& itemInDataPackagePath) const;
	/** Compute the "to Url" in compact format.
	 * skipUserPwd true:  return like ftp://host/path
	 * skipUserPwd false: return like ftp://usr:pwd@host/path */
	std::string getDestinationUrl(std::string const& itemInDataPackagePath, bool skipUserPwd) const;

	/** Push a circulation request*/ //@@@ To be improved: use class specializzation instead of 'case of...'
	std::string requestCirculation		( std::string const& itemInDataPackagePath ) const;
	std::string requestCirculationDb	( std::string const& itemInDataPackagePath ) const;
	std::string requestCirculationSoap	( std::string const& itemInDataPackagePath ) const;

	/** Get a circulation status */ //@@@ To be improved: use class specializzation instead of 'case of...'
	std::string getCirculationStatus	( std::string const& circulationId ) const;
	std::string getCirculationStatusDb	( std::string const& circulationId ) const;
	std::string getCirculationStatusSoap( std::string const& circulationId ) const;

	/** Monitor a group of Circulations and return the aggregate status  */
	void monitorCirculations( std::vector<std::string> const& circulationIds,
							  bool joinFinalStatus,
							  bool& allAreCompleted, bool& someError, bool& stillWorking ) const;

	/** Abort the Circulations and join them */
	void abortCirculationsAndJoin( std::vector<std::string> const& circulationIds ) const;


protected:
	/** I/O interface with the PackageExporter */
	ExpProcInfo _expProcInfo;

	/** Distribution info */
	DistributionSerializable _distribution;

	/** Datapackage info */
	dataprovider::DataPackage _datapackage;

	/** True when signel received */
	bool _stop;

	/** False: Wait Actual Circulation 
	 *  True:  Exit and notify the Framework that a Check Acknowledge is needed */
	bool _isTransferAsynch;

	/** False: use the Circulation db interface
	 *  True:  use the Circulation soap interface  */
	bool _useSoap;

	/** The label in the db.conf to address the circulation DB */
	std::string _circulationDbName;

	/** For transfers that are not 'filesystem to filesystem', the Datapackage to be uploaded has to be accessed by network protocol. */
	std::string _confFromUrlPrefix;
	std::string _confFromHost;

	/** Soap service configuration */
	std::string _confSoapProtocol;
	std::string _confSoapHost;
	std::string _confSoapPort; 
	std::string _confSoapService;

	/** Retry policy configuration. In case of db/soap exception, how many time to retry and delay between retry. */
	size_t _confRetryTimes; //how many time to retry
	size_t _confRetryDelaySecs; //delay between retries.

	/** NameIdAssociator and related classes are now using Singleton template pattern. Cf. Rev. 1.9 */
	std::string _defaultDbKey;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageCirculatorTask)

// Needed for test purposes
   friend class tPackageCirculatorTask;

};

_ACS_END_NAMESPACE

#endif //_PackageCirculatorTask_H_

