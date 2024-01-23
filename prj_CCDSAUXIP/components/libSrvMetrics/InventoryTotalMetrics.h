// PRQA S 1050 EOF
/*

	Copyright 2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Metrics Library $


*/

#ifndef _InventoryTotalMetrics_H_
#define _InventoryTotalMetrics_H_

#include <exMacros.h>
#include <exException.h>
#include <Filterables.h>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>
#include <dbConnection.h>

_ACS_BEGIN_NAMESPACE(acs)

class InventoryTotalMetrics {
public:
	exDECLARE_EXCEPTION(InventoryTotalMetricsException, exException);

	InventoryTotalMetrics();
	virtual ~InventoryTotalMetrics() noexcept = default;

	std::string getWorkingDir() const;
	size_t getBlockSize() const;
	void setWorkingDir(const std::string&);
	void setBlockSize(size_t bs);

	dbPersistent::IdType loadLastInventoryId(const std::string& appname, const std::string& instance, dbConnection& conn);
	size_t processNextInventoryBlock(const std::string& appname, const std::string& instance, dbPersistent::IdType& lastInvId, dbConnection& conn);

#ifndef TEST
private:
#else
public:
#endif
	InventoryTotalMetrics(const InventoryTotalMetrics&)                = delete;
	InventoryTotalMetrics(const InventoryTotalMetrics&&)               = delete;
	InventoryTotalMetrics& operator=(const InventoryTotalMetrics&)     = delete;
	InventoryTotalMetrics& operator=(const InventoryTotalMetrics&&)    = delete;
	
	void storeLastInventoryId(dbPersistent::IdType invId, const std::string& appname, const std::string& instance, dbConnection& conn);
	bool loadTotalMetric(const std::string& prodtype, const std::string& platname, const std::string& platSerial, dbSmartPersistent& sp, dbConnection& conn);
	dbSmartQuery* loadNextInventory(dbPersistent::IdType lastInvId, dbConnection& conn);
	std::string platformShortName(const std::string& acronym);
	void upsertTotalMetrics(const dbSmartPersistent& sp, dbConnection& conn);
	
private:
	static const size_t defaultBlockSize = 100;

private:
	size_t blockSize_;
	std::string workingDir_;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InventoryTotalMetrics);
};

_ACS_END_NAMESPACE

#endif /* _InventoryTotalMetrics_H_ */
