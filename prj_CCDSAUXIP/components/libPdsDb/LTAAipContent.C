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

	$Prod: Pds DB Library $

*/

#include <LTAAipContent.h>
#include <dbSmartQuery.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAAipContent);

bool LTAAipContent::loadAipContentByAipIdAndProductUUID(acs::db::LTAAipContent& content, dbPersistent::IdType aipId, const std::string& productUUID, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	db::SmartQuery<LTAAipContent> contentQuery(dbKey);
	
	// WHERE (aip_id == aipId and product_uuid == productUUID)
	{
		dbQueryParameters ppStatus(dbKey);
		ppStatus.addParameter(dbParam(dbKey, LTAAipContent::table(), "aip_id", dbParam::EQUAL_TO, dbConv(dbKey, aipId)));
		ppStatus.addParameter(dbParam(dbKey, LTAAipContent::table(), "product_uuid", dbParam::EQUAL_TO, dbConv(dbKey, productUUID)));
		contentQuery.addParameter(ppStatus);
	}
	
	contentQuery.doQuery(conn);
	
	// Should be exactly 1
	if (contentQuery.size() > 1) {
		std::ostringstream ss;
		ss << "Found more than 1 record associated to pair (aip_id, product_uuid) = (" << aipId << "," << productUUID << ")";
		ACS_THROW(exIllegalValueException(ss.str()));
	}
	
	bool found = (contentQuery.size() == 1);
	if (found) {
		content = *contentQuery[0];
	}
	
	return found;
}

/*
 * Constructor
 */
LTAAipContent::LTAAipContent(dbConnection& conn) :
	dbSmartPersistent(table(), conn)
{ }

/* load by unique keys */
bool LTAAipContent::loadByOrderId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return loadByUniqueKey<dbPersistent::IdType>(conn, "order_id", id);
}

bool LTAAipContent::removeByOrderId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return removeByUniqueKey<dbPersistent::IdType>(conn, "order_id", id);
}

/* Setters */
void LTAAipContent::setAipId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("aip_id", id , conn);
}

void LTAAipContent::unsetAipId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("aip_id", conn);
}

void LTAAipContent::setOrderId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("order_id", id , conn);
}

void LTAAipContent::unsetOrderId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("order_id", conn);
}

void LTAAipContent::setProductUUID(const std::string& uuid, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("product_uuid", uuid , conn);
}

void LTAAipContent::unsetProductUUID(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("product_uuid", conn);
}

void LTAAipContent::setBucketName(const std::string& bn, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("bucket_name", bn , conn);
}

void LTAAipContent::unsetBucketName(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("bucket_name", conn);
}

void LTAAipContent::setStatusId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("status_id", id , conn);
}

void LTAAipContent::unsetStatusId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("status_id", conn);
}

void LTAAipContent::setUploadTime(const DateTime& ut, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("upload_time", ut , conn);
}

void LTAAipContent::unsetUploadTime(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("upload_time", conn);
}

void LTAAipContent::setEvictionTime(const DateTime& et, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("eviction_time", et , conn);
}

void LTAAipContent::unsetEvictionTime(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("eviction_time", conn);
}

void LTAAipContent::setProductSize(off_t ps, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<off_t>("product_size", ps , conn);
}

void LTAAipContent::unsetProductSize(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("product_size", conn);
}

/* Getters */
dbPersistent::IdType LTAAipContent::getAipId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("aip_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"aip_id\" in table " + LTAAipContent::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAAipContent::getOrderId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("order_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"order_id\" in table " + LTAAipContent::table()));
	}
	return val;
}

std::string LTAAipContent::getProductUUID(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("product_uuid", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"product_uuid\" in table " + LTAAipContent::table()));
	}
	return val;
}

std::string LTAAipContent::getBucketName(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("bucket_name", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"bucket_name\" in table " + LTAAipContent::table()));
	}
	return val;
}

dbPersistent::IdType LTAAipContent::getStatusId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("status_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"status_id\" in table " + LTAAipContent::table()));
	}
	return val;
}

DateTime LTAAipContent::getUploadTime(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("upload_time", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"upload_time\" in table " + LTAAipContent::table()));
	}
	return val;
}

DateTime LTAAipContent::getEvictionTime(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("eviction_time", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"eviction_time\" in table " + LTAAipContent::table()));
	}
	return val;
}

off_t LTAAipContent::getProductSize(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	off_t val = 0;
	bool isNull = false;
	get<off_t>("product_size", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"product_size\" in table " + LTAAipContent::table()));
	}
	return val;
}


_ACS_END_NESTED_NAMESPACE

