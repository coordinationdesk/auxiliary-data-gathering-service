// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. Data Provider Library $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 5.3  2017/05/29 17:53:17  nicvac
 * 	OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
 * 	
 * 	Revision 5.2  2016/12/16 16:55:45  nicvac
 * 	S3MPM-860: PackageExporter resources improvement.
 * 	
 * 	Revision 5.1  2013/11/04 13:56:31  marpas
 * 	coding best practices applied
 * 	robustness improved
 * 	performances improved
 * 	compilation & qa warnings removed
 * 	
 * 	Revision 5.0  2013/06/06 18:51:19  marpas
 * 	adopting libException 5.x standards
 * 	
 * 	Revision 1.26  2013/03/15 10:47:10  nicvac
 * 	S2PDGS-186
 * 	ADC-83
 * 	WaitingAcknowledge Step implemented
 * 	
 * 	Revision 1.25  2013/03/11 17:29:50  nicvac
 * 	Update interface revised.
 * 	
 * 	Revision 1.24  2013/03/05 15:48:49  nicvac
 * 	Support for Retry policy added
 * 	
 * 	Revision 1.23  2013/03/04 17:41:31  nicvac
 * 	update modified to support the new schema (retry handling)
 * 	
 * 	Revision 1.22  2012/11/09 17:10:29  nicvac
 * 	Set of mediatype handling.
 * 	
 * 	Revision 1.21  2012/10/19 15:04:06  micmaz
 * 	updated library
 * 	
 * 	Revision 1.20  2012/10/08 13:59:19  micmaz
 * 	http://jira.acsys.it/browse/S2GPP-611
 * 	
 * 	Revision 1.19  2012/09/19 15:57:12  micmaz
 * 	ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 * 	http://jira.acsys.it/browse/S2GPP-611
 * 	
 * 	Revision 1.18  2012/09/14 16:31:32  micmaz
 * 	S2GPP-611
 * 	Development of Distribution Formatter framework for Export
 * 	
 * 	Revision 1.16  2012/09/11 16:42:31  micmaz
 * 	removed compiling warning
 * 	
 * 	Revision 1.15  2012/02/13 16:59:44  marpas
 * 	refactoring in progress
 * 	
 * 	Revision 1.14  2011/10/13 15:21:28  clanas
 * 	changed return value
 * 	
 * 	Revision 1.13  2011/05/12 13:27:59  crivig
 * 	changed interface in order to fix a bug for suspended packages
 * 	
 * 	Revision 1.12  2011/05/06 10:46:46  crivig
 * 	modified for quality check; all i++ modified into ++i
 * 	
 * 	Revision 1.11  2011/02/25 14:53:05  crivig
 * 	bug on cartId fixed
 * 	
 * 	Revision 1.10  2010/12/17 10:25:28  crivig
 * 	Cache class used; connection pool passed
 * 	
 * 	Revision 1.9  2010/12/03 15:00:49  crivig
 * 	cache cleaning handled with Cache class
 * 	
 * 	Revision 1.8  2010/11/29 11:04:21  crivig
 * 	changed Package to DataPckage; get**Status methods added
 * 	
 * 	Revision 1.7  2010/11/22 11:37:38  crivig
 * 	libDownloadOptimiser eliminated! downloadPackData class used instead
 * 	
 * 	Revision 1.6  2010/10/28 13:48:31  crivig
 * 	testing
 * 	
 * 	Revision 1.5  2010/10/27 10:45:00  crivig
 * 	 new cleanExit method added
 * 	
 * 	Revision 1.4  2010/10/22 13:41:39  crivig
 * 	compiling
 * 	
 * 	Revision 1.3  2010/10/13 14:06:49  crivig
 * 	compiled
 * 	
 * 	Revision 1.2  2010/10/06 13:55:14  crivig
 * 	coding
 * 	
 * 	Revision 1.1.1.1  2010/10/01 13:17:27  crivig
 * 	Imported new library
 * 	
 */

#ifndef _DataProvider_H_
#define _DataProvider_H_

#include <string>

#include <dbConnectionPool.h>
#include <dbPersistent.h>
#include <DownloadPackData.h>  
#include <DataPackage.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
class dbConnectionPool;
class Cache;
_ACS_END_NAMESPACE

_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)
class DataPackage;

class DataProvider { // PRQA S 2109

public:
	DataProvider( std::vector<std::string> const& mediaTypes, Cache& cache, dbConnectionPool& connectionPool);
	virtual ~DataProvider() throw ();

	size_t checkEligiblePackages(std::vector<DataPackage>* failedPackages, std::vector<DataPackage>* suspendedPackages);
	void updateDistrStatus(std::string const& status, std::vector< dbPersistent::IdType > const& packagesId,
			bool incrementRetry, bool updateRelatedItems);
	void checkDownloadThreads(std::vector<DataPackage>* goodPackages, std::vector<DataPackage>* failedPackages);
	void cleanExit();
	/*! class DataProviderException declaration */
	exDECLARE_EXCEPTION(DataProviderException,exException); // PRQA S 2131, 2502
private:
	// declared but not implemented
	DataProvider();
	DataProvider(const DataProvider&);
	DataProvider & operator=(const DataProvider&);

	/** Book a package.
	 * Relies on stored procedures: In case of concurrency, only one db client will be able to set
	 * the order from  ELIGIBLE to PRECONDITIONCHECK.
	 * Who does, get the package.
	 *  */
	bool bookPackage( const dbPersistent::IdType& packageId, dbConnection& conn ) throw();

	/** Check items are available on Short Term Archive
	 * In case One or more items in the package to be distributed needs to be restored from Long Term Archive (LTA)
	 * - a restore request is queued
	 * - register the item to t_distributionWaitingItems
	 * - set the distribution order back to ELIGIBLE
	 * t_distributionWaitingItems will be cleared automatically by stored procedure when an item has been restored from LTA.
	 * */
	bool checkShortTermAvailability( const dbPersistent::IdType& packageId, dbConnection& conn, bool & restoreRequestQueued ) throw();

private:

	/** Configuration: DB connection retry */
	unsigned int _confMaxConnRetry;
	/** Configuration: Max downloading threads */
	unsigned int _confMaxDownloadThreads;
	/** Configuration: support items stored on LTA */
	bool _confSupportLTA;

	std::vector<std::string> _mediaTypes;
	std::string _jobResponsible;

	Cache& _cache;
	std::vector<DownloadPackData*> _downloadThreads;
	dbConnectionPool& _connectionPool;
	bool _stop;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DataProvider) ;
};

_ACS_END_NESTED_NAMESPACE

#endif  // _DataProvider_H_
