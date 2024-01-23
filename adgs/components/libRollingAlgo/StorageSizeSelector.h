/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libRollingAlgo$
        
*/

#ifndef _StorageSizeSelector_H_
#define _StorageSizeSelector_H_

#include <InvPolicyCommonSelector.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Select inventory items to clean up a storage manager based on a configurable disk occupancy threshold
* A storage manager that has a limited cache space on the disk, and that is not configured
* to migrate its content to Near-Line or Off-Line tapes, can take advantage of this rule to
* keep only the latest files, keeping the size under configured limit.
* Size thresholds are in db table T_StorageRollingThresholds.
*
* The related rolling policy nominally has to be configured with a CLEANSTORAGE action.
**/

class StorageSizeSelector: public InvPolicyCommonSelector {
public:

	/** Default Class constructor */
	explicit StorageSizeSelector( const InvPolicy & );
	/** Destructor */
	virtual ~StorageSizeSelector() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS(StorageSizeSelector);

public:
	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

private:
	/** Build query to roll inventory items according to storage size */
	std::string getQuery (dbConnection &conn, uint64_t thresholdBytes);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageSizeSelector)

};

_ACS_END_NAMESPACE

#endif //_StorageSizeSelector_H_

