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

#include <libprollV.h>
#include <EvictionTask.h>
#include <PRIPRollingConfigurer.h>
#include <ConfigurationSingleton.h>
#include <dbFileAction.h>
#include <dbFileActionScopeGuard.h>
#include <dbFileActionFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace db;

namespace {
	libprollV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(EvictionTask);

/* Ctor / Dtor */
EvictionTask::EvictionTask(dbConnectionPool& pool, size_t maxItem, bool removeRecord) :
	pool_(pool),
	maxItem_(maxItem),
	removeRecord_(removeRecord)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EvictionTask::~EvictionTask() noexcept {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

db::SmartQuery<acs::db::LTAInventoryObject> EvictionTask::buildLoadQuery(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	DateTime now;
	db::SmartQuery<LTAInventoryObject> objQuery(dbKey);
	
	{	// WHERE condition on evictiondate
		std::string nowString;
		const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
		if( dialectName == "Postgres8") {
			nowString = "now()";
		}
		else if (dialectName == "Oracle11") {
			nowString = "LOCALTIMESTAMP";
		}
		else {
			ACS_THROW(EvictionTaskException("Database dialect not supported: " + dialectName));
		}
			
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, LTAInventoryObject::table(), "evictiondate", dbParam::MINOR_OF, nowString));
		objQuery.addParameter(pp);
	}
	
	// WHERE condition on validity flag
	// it allows to avoid looping over the same records in case records removal is disabled
	if (!removeRecord_) {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, LTAInventoryObject::table(), "validity", dbParam::EQUAL_TO, dbConv(dbKey, true)));
		objQuery.addParameter(pp);
	}
	
	if (maxItem_ > 0) {
		objQuery.maxItems(maxItem_);
	}
	
	return objQuery;
}

void EvictionTask::loadInventoryObjects(std::vector<LTAInventoryObject>& objs, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	db::SmartQuery<LTAInventoryObject> objQuery = buildLoadQuery(conn);
	objQuery.doQuery(conn);

	objs.clear();
	size_t objNum = objQuery.size();
	
	if (objNum > 0) {
		std::ostringstream ss;
		ss << "Loaded " << objNum << " products to be removed.";
		ACS_LOG_INFO(ss.str());
	}
	
	for (size_t idx = 0; idx < objNum; ++idx) {
		LTAInventoryObject* obj = objQuery[idx];
		ACS_COND_THROW(0 == obj, EvictionTaskException("Invalid inventory object pointer (null)"));
		objs.push_back(*obj);
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << objs.size() << " inventory objects.");
}

void EvictionTask::eraseInventoryObject(LTAInventoryObject& invobj, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	std::string prodName = invobj.getName(conn);
	dbPersistent::IdType invId = invobj.getId();
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting erase of product " << prodName << " having inventory id " << invId);
	
	// Get dbFileAction object in charge of erasing the inventory object
	dbFileAction* eraser = dbFileActionFactoryChain::instance()->newEraseAction(pool_, invobj);
	dbFileActionScopeGuard guard(eraser);

	ACS_COND_THROW(0 == eraser, exIllegalValueException("Cannot obtain a dbLTAEraseInventoryFile instance")) ; // PRQA S 3081
	
	eraser->start();
	if (removeRecord_) {
		invobj.remove(conn);
	}
    //already logged as INFO in libLTAFileAction
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product " + prodName + " having inventory id " << invId << " successfully removed.");
}

void EvictionTask::execTask(StopController& stopController) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Eviction task started."); 
	
	dbConnectionWrapper cw(pool_);
		
	std::vector<LTAInventoryObject> objs;
	loadInventoryObjects(objs, cw);
	
	size_t objsNum = objs.size();
	for (size_t idx = 0; idx < objsNum; ++idx) {
		if (stopController.haltImmediate() || stopController.requestedCancel()) {
			break;
		}
		
		LTAInventoryObject& invobj = objs[idx];
		try {
			eraseInventoryObject(invobj, cw);
		} catch (std::exception& e) {
			std::ostringstream ss;
			ss << "The inventory object having id " << invobj.getId() << " associated to product " << invobj.getName(cw) << " cannot be evicted. Reason was: " << e.what();
			ACS_LOG_WARNING(ss.str());
		}
	}
}

_ACS_END_NAMESPACE
