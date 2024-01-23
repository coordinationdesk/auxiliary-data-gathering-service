// PRQA S 1050 EOF
/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    PRIPRolling Library

*/ 

#ifndef _EvictionTask_h_
#define _EvictionTask_h_

#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <LTAInventoryObject.h>
#include <dbSmartQuery.h>
#include <StopController.h>

_ACS_BEGIN_NAMESPACE(acs)

class EvictionTask {
public:
	exDECLARE_EXCEPTION(EvictionTaskException, exException);

	/** Deleted special functions */
	EvictionTask()                                            = delete;
	EvictionTask(const EvictionTask&)                     = delete;
	EvictionTask& operator=(const EvictionTask&)          = delete;
	EvictionTask(EvictionTask&&) noexcept                 = delete;
	EvictionTask& operator=(EvictionTask&&) noexcept      = delete;
	
	/** Ctor / Dtor */
	EvictionTask(dbConnectionPool& pool, size_t maxItem, bool removeRecord = true);
	virtual ~EvictionTask() noexcept;

	size_t getMaxItem() const;
	
	virtual void execTask(StopController&);
	
#ifndef TEST_LIB
private:
#else
public:
#endif
	db::SmartQuery<acs::db::LTAInventoryObject> buildLoadQuery(dbConnection& conn);
	void loadInventoryObjects(std::vector<acs::db::LTAInventoryObject>& objs, dbConnection& conn);
	void eraseInventoryObject(acs::db::LTAInventoryObject& obj, dbConnection&);
	
private:
	dbConnectionPool& pool_;
	size_t maxItem_;
	bool removeRecord_;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EvictionTask);
};

_ACS_END_NAMESPACE

#endif /* _EvictionTask_h_ */
