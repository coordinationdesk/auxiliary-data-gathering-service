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
	Revision 1.3  2016/05/11 09:17:57  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.2  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.1  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
        
*/

#ifndef _PackageProcessorCheck_H_
#define _PackageProcessorCheck_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <PackageProcessor.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Specialization of PackageProcessor for Check on previous Format and Distribution
**/

class PackageProcessorCheck: public PackageProcessor { 
public:

	/** PackageProcessorCheck Exceptions */
	exDECLARE_EXCEPTION(exPackageProcessorCheckException, exException) ; // Base PackageProcessorCheck Exception.
	exDECLARE_EXCEPTION(exPackageProcessorCheckCriticalException, exPackageProcessorCheckException) ; // Critical Exception.


	/** Default Class constructor */
	PackageProcessorCheck();
	/** Destructor */
	virtual ~PackageProcessorCheck() throw();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageProcessorCheck(const PackageProcessorCheck & );
	/** Operator = */
	PackageProcessorCheck &operator=(const PackageProcessorCheck &);
	

public:
	/** Init by datapackage id to be processed. */
	virtual void init( dbPersistent::IdType const& dataPackageId );
	virtual void init( DataPackageSptr dataPackageSptr )  { ACS_THROW(exPackageProcessorCheckCriticalException("Cannot be initialized using init(DataPackageSptr)")); }


protected:
	/** Get the task tables needed to process the datapackage */
	virtual void setUp();

	/** Do nothing. The working dir has been prepared by PackageProcessorFmtDist */
	virtual void setUpWorkingDir();


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageProcessorCheck)

};

_ACS_END_NAMESPACE

#endif //_PackageProcessorCheck_H_

