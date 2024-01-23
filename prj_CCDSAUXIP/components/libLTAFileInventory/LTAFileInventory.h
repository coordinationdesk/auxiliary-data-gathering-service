// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA File Inventory Library $

*/

#ifndef _LTAFileInventory_H_
#define _LTAFileInventory_H_ 


#include <LTAMetadata.h>
#include <exException.h>
#include <Filterables.h>
#include <Statistics.h>
#include <dbConnectionPool.h>
#include <dbGeoObject.h>
#include <dbPersistent.h>
#include <LTAInventoryObject.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

class LTAFileInventory // PRQA S 2109
{
public:

	/** class LTAFileInventoryException declaration */
	exDECLARE_EXCEPTION(LTAFileInventoryException, exException);  // PRQA S 2131, 2502 3
	exDECLARE_EXCEPTION(LTAFileNotFoundException, LTAFileInventoryException);  // PRQA S 2131, 2502 3
	exDECLARE_EXCEPTION(LTAFileInventoryUploadAborted, LTAFileInventoryException);  // PRQA S 2131, 2502 3
	exDECLARE_EXCEPTION(LTAGeoInventoryException, exException); // PRQA S 2131, 2502 3

	/** Deleted special functions */
	LTAFileInventory() = delete;
	LTAFileInventory(const LTAFileInventory&) = delete;
	LTAFileInventory(LTAFileInventory&&) = delete;
	LTAFileInventory& operator=(const LTAFileInventory&) = delete;
	LTAFileInventory& operator=(LTAFileInventory&&) = delete;
	
	/** Defaulted special functions */
	explicit LTAFileInventory(dbConnectionPool&);
	virtual ~LTAFileInventory() ACS_NOEXCEPT = default;
	
	dbPersistent::IdType inventoryProduct(const LTAMetadata& metadata);

	inline void setAbortUploadFlag(bool v) { Lock l(mutex_); abortUpload_ = v; }
	inline bool getAbortUploadFlag() const { Lock l(mutex_); return abortUpload_; }

private:
	void fillInventoryObject(const LTAMetadata&, acs::db::LTAInventoryObject&, dbConnection&);

private:
	static const unsigned long defaultUploadPollTime = 1;
	static const bool defaultRemoveSourceProductFlag = true;

	Mutex mutex_;
	dbConnectionPool& pool_;
	bool simulation_;
	bool abortUpload_;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAFileInventory);
};

_ACS_END_NESTED_NAMESPACE

#endif //_LTAFileInventory_H_
