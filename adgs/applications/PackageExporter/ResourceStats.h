/*
	Copyright 1995-2016, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. PackageExporter$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/02/23 16:37:50  nicvac
	S3EVOL-13: Distribution working area is not related anymore to download cache.
	
	Revision 1.2  2017/02/21 17:05:57  nicvac
	S3EVOL-13: Inter process cache lock mode; cache limit check
	
	Revision 1.1  2016/12/20 11:12:56  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
        
*/

#ifndef _ResourceStats_H_
#define _ResourceStats_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <PackageProcessingManager.h>

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Check resources' status
**/

class ResourceStats  { 
public:

	/** ResourceStats Exceptions */
	exDECLARE_EXCEPTION(exResourceStatsException, exException);

	/** Class constructor */
	ResourceStats( 	size_t pendingRequestsLimit, size_t waitingAcknowledgeLimit,
					size_t minSpaceRequiredMb, std::string const& spaceRequiredPath,
					std::string const& jobResponsible,
					PackageProcessingManager* packageProcessingManagerPtr);
	/** Destructor */
	virtual ~ResourceStats();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	ResourceStats(const ResourceStats & );
	/** Operator = */
	ResourceStats &operator=(const ResourceStats &);
	
private:
	/** Cf. PackageExporter _confPendingRequestsLimit */
	size_t _pendingRequestsLimit;
	/** Cf. PackageExporter _confWaitingAcknowledgeLimit */
	size_t _waitingAcknowledgeLimit;

	/** Cf. PackageExporter _confMinSpaceRequiredMb */
	size_t _minSpaceRequiredMb;
	/** Cf. DataProvider.ProductPath */
	std::string _spaceRequiredPath;

	/** JobResponsible */
	std::string _jobResponsible;

	/** Pointer to the package processing manager */
	PackageProcessingManager* _packageProcessingManagerPtr;


public:

	/** Getter and setter */
	size_t 				getMinSpaceRequiredMb() 		const {	return _minSpaceRequiredMb; }
	size_t 				getPendingRequestsLimit() 		const {	return _pendingRequestsLimit; }
	const std::string& 	getSpaceRequiredPath() 			const {	return _spaceRequiredPath; }
	size_t 				getWaitingAcknowledgeLimit() 	const {	return _waitingAcknowledgeLimit; }


	/** Queued Max number of pending requests */
	bool isPendingRequestsLimit() const;

	/** Max number of waiting acknowledge request reached */
	bool isWaitingAcknowledgeLimit() const;

	/** Disk Space Limit reached */
	bool isDiskSpaceLimit() const;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ResourceStats)

};

_ACS_END_NAMESPACE

#endif //_ResourceStats_H_

