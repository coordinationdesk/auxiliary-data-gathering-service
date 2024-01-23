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
	Revision 1.7  2016/12/19 18:32:37  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.6  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.5  2016/05/11 09:17:57  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.4  2015/08/20 16:18:30  nicvac
	ALSAT1B-44: cart handling
	
	Revision 1.3  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.2  2013/03/21 14:44:28  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.1  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
        
*/

#ifndef _PackageProcessorFmtDist_H_
#define _PackageProcessorFmtDist_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <PackageProcessor.h>

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Specialization of PackageProcessor for Format and Distribution
**/

class PackageProcessorFmtDist: public PackageProcessor { 
public:

	/** PackageProcessorFmtDist Exceptions */
	exDECLARE_EXCEPTION(exPackageProcessorFmtDistException, exException) ; // Base PackageProcessorFmtDist Exception.
	exDECLARE_EXCEPTION(exPackageProcessorFmtDistCriticalException, exPackageProcessorFmtDistException) ; // Critical Exception.


	/** Default Class constructor */
	PackageProcessorFmtDist();
	/** Destructor */
	virtual ~PackageProcessorFmtDist() throw();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageProcessorFmtDist(const PackageProcessorFmtDist & );
	/** Operator = */
	PackageProcessorFmtDist &operator=(const PackageProcessorFmtDist &);
	
public:

	/** Init by datapackage to be processed. */
	virtual void init( dbPersistent::IdType const& dataPackageId ) { ACS_THROW(exPackageProcessorFmtDistCriticalException("Cannot be initialized using init(int dataPackageId)")); }
	virtual void init( DataPackageSptr dataPackageSptr );


protected:

	/** Create the Distribution object. Get the task tables needed to process the datapackage and get the destination */
	virtual void setUp();

	/** Prepare the working dir:
	- create the working dir at the same path of the downloaded data package.
	- link (symbolic) the data package directory in the created working dir. */
	virtual void setUpWorkingDir();

	/** Dump in the working dir the dataPackage and the distribution objects as xml files. 
	 * Dumps also the parameters */
	void dumpPackageInfos() const;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageProcessorFmtDist)

};

_ACS_END_NAMESPACE

#endif //_PackageProcessorFmtDist_H_

