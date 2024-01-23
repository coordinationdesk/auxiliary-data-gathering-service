/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageExporter$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/05/29 17:53:43  nicvac
	OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
	
	Revision 1.2  2016/12/16 16:56:15  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.1  2016/12/13 16:39:57  nicvac
	OPMAN-199: Reset intermediate status improved.
	
        
*/

#ifndef _ResetStatus_H_
#define _ResetStatus_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <dbConnection.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Reset Package Exporter Intermediate status
**/

class ResetStatus  { 
public:

	/** ResetStatus Exceptions */
	exDECLARE_EXCEPTION(exResetStatusException, exException); // ResetStatus Exception.

	/** Default Class constructor */
	ResetStatus(std::string const& jobResponsible, dbConnection& conn);
	/** Destructor */
	virtual ~ResetStatus();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	ResetStatus(const ResetStatus & );
	/** Operator = */
	ResetStatus &operator=(const ResetStatus &);
	

private:
	/** Job Responsible I have to look for */
	std::string _jobResponsible;

	/** To access db */
	dbConnection& _conn;

private:
	/** Populate the Temporary Table  with package ids in the provided statuses */
	void fillPackageidTemporaryTable(std::string const& tempTableName, std::vector<std::string> const& statuses  ) const;
	/** Log Packages from temporary table */
	size_t logPackageidFromTemporaryTable(std::string const& tempTableName, std::string const& statusesStr ) const;
	/** Set Packages from temporary table to Error */
	void setErrorPackageidFromTemporaryTable(std::string const& tempTableName ) const;
	/** Set Packages from temporary table to Eligible */
	void setEligiblePackageidFromTemporaryTable(std::string const& tempTableName ) const;

public:
	/** Check for my distribution queue's intermediate status and force them to initial status (if allowed by conf)
	 * Intermediate status can be found if daemon aborted during last run. */
	void resetDistributionQueueStatus() const;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ResetStatus)

};

_ACS_END_NAMESPACE

#endif //_ResetStatus_H_

