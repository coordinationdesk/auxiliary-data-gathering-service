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

#include <LTAOrder.h>
#include <libpdsdbV.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAOrder);

/*
 * Constructor
 */
LTAOrder::LTAOrder(dbConnection& conn) :
	dbSmartPersistent(table(), conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

bool LTAOrder::loadByUUID(const std::string& uuid, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return loadByUniqueKey<std::string>(conn, "uuid", uuid);
}

bool LTAOrder::removeByUUID(const std::string& uuid, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return removeByUniqueKey<std::string>(conn, "uuid", uuid);
}

/*
 * Setters
 */
void LTAOrder::setUUID(const std::string& uuid, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("uuid", uuid , conn);
}

void LTAOrder::unsetUUID(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("uuid", conn);
}

void LTAOrder::setProductId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("product_id", id , conn);
}

void LTAOrder::unsetProductId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("product_id", conn);
}

void LTAOrder::setTargetAipId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("target_aip_id", id , conn);
}

void LTAOrder::unsetTargetAipId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("target_aip_id", conn);
}

void LTAOrder::setLastStatusUpdate(const DateTime& dt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("last_status_update", dt , conn);
}

void LTAOrder::unsetLastStatusUpdate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("last_status_update", conn);
}

void LTAOrder::setJobResponsible(const std::string& jobresp, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("jobresponsible", jobresp , conn);
}

void LTAOrder::unsetJobResponsible(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("jobresponsible", conn);
}

void LTAOrder::setUploadStatusId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("upload_status_id", id , conn);
}

void LTAOrder::unsetUploadStatusId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("upload_status_id", conn);
}

void LTAOrder::setRemoteAwsUploaderJobId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("remote_aws_uploader_jobid", id , conn);
}

void LTAOrder::unsetRemoteAwsUploaderJobId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("remote_aws_uploader_jobid", conn);
}

void LTAOrder::setRemoteAwsUploaderUrl(const std::string& dcurl, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("remote_aws_uploader_url", dcurl , conn);
}

void LTAOrder::unsetRemoteAwsUploaderUrl(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("remote_aws_uploader_url", conn);
}

void LTAOrder::setCreationDate(const DateTime& dt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("creation_date", dt , conn);
}

void LTAOrder::unsetCreationDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("creation_date", conn);
}

void LTAOrder::setStartDate(const DateTime& dt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("start_date", dt , conn);
}

void LTAOrder::unsetStartDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("start_date", conn);
}

void LTAOrder::setCompletionDate(const DateTime& dt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("completion_date", dt , conn);
}

void LTAOrder::unsetCompletionDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("completion_date", conn);
}

void LTAOrder::setModificationDate(const DateTime& dt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("modification_date", dt , conn);
}

void LTAOrder::unsetModificationDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("modification_date", conn);
}

void LTAOrder::setEstimatedDate(const DateTime& dt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("estimated_date", dt , conn);
}

void LTAOrder::unsetEstimatedDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("estimated_date", conn);
}

void LTAOrder::setPriority(int prio, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<int>("priority", prio, conn);
}

void LTAOrder::unsetPriority(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("priority", conn);
}

void LTAOrder::setLinkCount(unsigned long cnt, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<unsigned long>("links_count", cnt , conn);
}

void LTAOrder::unsetLinkCount(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("links_count", conn);
}

void LTAOrder::setStatusId(dbPersistent::IdType id, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("status_id", id , conn);
}

void LTAOrder::unsetStatusId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("status_id", conn);
}

/*
 * Getters
 */

std::string LTAOrder::getUUID(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("uuid", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"uuid\" in table " + LTAOrder::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAOrder::getProductId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("product_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"product_id\" in table " + LTAOrder::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAOrder::getTargetAipId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("target_aip_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"target_aip_id\" in table " + LTAOrder::table()));
	}
	
	return val;
}

DateTime LTAOrder::getLastStatusUpdate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("last_status_update", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"last_status_update\" in table " + LTAOrder::table()));
	}
	
	return val;
}

std::string LTAOrder::getJobResponsible(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("jobresponsible", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"jobresponsible\" in table " + LTAOrder::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAOrder::getUploadStatusId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("upload_status_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"upload_status_id\" in table " + LTAOrder::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAOrder::getRemoteAwsUploaderJobId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("remote_aws_uploader_jobid", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"remote_aws_uploader_jobid\" in table " + LTAOrder::table()));
	}
	
	return val;
}

std::string LTAOrder::getRemoteAwsUploaderUrl(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("remote_aws_uploader_url", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"remote_aws_uploader_url\" in table " + LTAOrder::table()));
	}
	
	return val;
}

DateTime LTAOrder::getCreationDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("creation_date", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"creation_date\" in table " + LTAOrder::table()));
	}
	
	return val;
}

DateTime LTAOrder::getStartDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("start_date", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"start_date\" in table " + LTAOrder::table()));
	}
	
	return val;
}

DateTime LTAOrder::getCompletionDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("completion_date", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"completion_date\" in table " + LTAOrder::table()));
	}
	
	return val;
}

DateTime LTAOrder::getModificationDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("modification_date", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"modification_date\" in table " + LTAOrder::table()));
	}
	
	return val;
}

DateTime LTAOrder::getEstimatedDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("estimated_date", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"estimated_date\" in table " + LTAOrder::table()));
	}
	
	return val;
}

int LTAOrder::getPriority(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	int val = 0;
	bool isNull = false;
	get<int>("priority", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"priority\" in table " + LTAOrder::table()));
	}
	
	return val;
}

unsigned long LTAOrder::getLinkCount(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unsigned long val = 0;
	bool isNull = false;
	get<unsigned long>("links_count", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"links_count\" in table " + LTAOrder::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAOrder::getStatusId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val = 0;
	bool isNull = false;
	get<dbPersistent::IdType>("status_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"status_id\" in table " + LTAOrder::table()));
	}
	
	return val;
}


_ACS_END_NESTED_NAMESPACE

