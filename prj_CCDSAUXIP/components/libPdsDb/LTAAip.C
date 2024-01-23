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

#include <LTAAip.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAAip);

/*
 * Constructor
 */
LTAAip::LTAAip(dbConnection& conn) :
	dbSmartPersistent(table(), conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

bool LTAAip::loadByName(const std::string& name, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return loadByUniqueKey<std::string>(conn, "aip_name", name);
}

bool LTAAip::removeByName(const std::string& name, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return removeByUniqueKey<std::string>(conn, "aip_name", name);
}

/*
 * Setters
 */
void LTAAip::setAipName(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("aip_name", val , conn);
}

void LTAAip::unsetAipName(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("aip_name", conn);
}

void LTAAip::setUrl(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("url", val , conn);
}

void LTAAip::unsetUrl(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("url", conn);
}

void LTAAip::setUsername(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("username", val , conn);
}

void LTAAip::unsetUsername(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("username", conn);
}

void LTAAip::setPassword(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("passwd", val , conn);
}

void LTAAip::unsetPassword(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("passwd", conn);
}

void LTAAip::setDiskSpace(off_t val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<off_t>("disk_space", val , conn);
}

void LTAAip::unsetDiskSpace(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("disk_space", conn);
}

void LTAAip::setBaseRedirectUrl(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("base_redirect_url", val , conn);
}

void LTAAip::unsetBaseRedirectUrl(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("base_redirect_url", conn);
}

void LTAAip::setParameters(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("parameters", val , conn);
}

void LTAAip::unsetParameters(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("parameters", conn);
}

void LTAAip::setRetentionTimeSecs(int val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<int>("retention_time_secs", val , conn);
}

void LTAAip::unsetRetentionTimeSecs(dbConnection& conn) {
	unset("retention_time_secs", conn);
}

/*
 * Getters
 */
std::string LTAAip::getAipName(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("aip_name", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"aip_name\" in table " + LTAAip::table()));
	}
	
	return val;
}

std::string LTAAip::getUrl(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("url", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"url\" in table " + LTAAip::table()));
	}
	
	return val;
}

std::string LTAAip::getUsername(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("username", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"username\" in table " + LTAAip::table()));
	}
	
	return val;
}

std::string LTAAip::getPassword(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("passwd", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"passwd\" in table " + LTAAip::table()));
	}
	
	return val;
}

off_t LTAAip::getDiskSpace(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	off_t val = 0;
	bool isNull = false;
	get<off_t>("disk_space", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"disk_space\" in table " + LTAAip::table()));
	}
	
	return val;
}

std::string LTAAip::getBaseRedirectUrl(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("base_redirect_url", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"base_redirect_url\" in table " + LTAAip::table()));
	}
	
	return val;
}

std::string LTAAip::getParameters(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("parameters", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"parameters\" in table " + LTAAip::table()));
	}
	
	return val;
}

int LTAAip::getRetentionTimeSecs(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	int val = 0;
	bool isNull = false;
	get<int>("retention_time_secs", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"retention_time_secs\" in table " + LTAAip::table()));
	}
	
	return val;
}

_ACS_END_NESTED_NAMESPACE

