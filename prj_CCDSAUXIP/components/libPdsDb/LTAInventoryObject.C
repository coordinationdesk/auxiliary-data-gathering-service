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

#include <LTAInventoryObject.h>
#include <dbGeoPolygon.h>
#include <libpdsdbV.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

namespace {
	libpdsdbV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAInventoryObject);

/*
 * Constructors
 */
LTAInventoryObject::LTAInventoryObject(dbConnection& conn) :
	dbSmartPersistent(table(), conn)
{ }

bool LTAInventoryObject::loadByName(const std::string& name, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return loadByUniqueKey<std::string>(conn, "name", name);
}

bool LTAInventoryObject::loadByUUID(const std::string& uuid, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return loadByUniqueKey<std::string>(conn, "uuid", uuid);
}

bool LTAInventoryObject::removeByName(const std::string& name, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return removeByUniqueKey<std::string>(conn, "name", name);
}

bool LTAInventoryObject::removeByUUID(const std::string& uuid, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return removeByUniqueKey<std::string>(conn, "uuid", uuid);
}

/*
 * Setters
 */
void LTAInventoryObject::setUUID(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("uuid", val, conn);
}

void LTAInventoryObject::unsetUUID(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("uuid", conn);
}

void LTAInventoryObject::setName(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("name", val , conn);
}

void LTAInventoryObject::unsetName(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("name", conn);
}

void LTAInventoryObject::setContentType(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("contenttype", val , conn);
}

void LTAInventoryObject::unsetContentType(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("contenttype", conn);
}

void LTAInventoryObject::setContentLength(off_t val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<off_t>("contentlength", val , conn);
}

void LTAInventoryObject::unsetContentLength(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("contentlength", conn);
}

void LTAInventoryObject::setOriginDate(const DateTime& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("origindate", val , conn);
}

void LTAInventoryObject::unsetOriginDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("origindate", conn);
}

void LTAInventoryObject::setCreationDate(const DateTime& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("creationdate", val , conn);
}

void LTAInventoryObject::unsetCreationDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("creationdate", conn);
}

void LTAInventoryObject::setModificationDate(const DateTime& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("modificationdate", val , conn);
}

void LTAInventoryObject::unsetModificationDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("modificationdate", conn);
}

void LTAInventoryObject::setEvictionDate(const DateTime& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("evictiondate", val , conn);
}

void LTAInventoryObject::unsetEvictionDate(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("evictiondate", conn);
}

void LTAInventoryObject::setChecksum(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("checksum", val , conn);
}

void LTAInventoryObject::unsetChecksum(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("checksum", conn);
}

void LTAInventoryObject::setValidityStart(const DateTime& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("validitystart", val , conn);
}

void LTAInventoryObject::unsetValidityStart(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("validitystart", conn);
}

void LTAInventoryObject::setValidityStop(const DateTime& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<DateTime>("validitystop", val , conn);
}

void LTAInventoryObject::unsetValidityStop(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("validitystop", conn);
}

void LTAInventoryObject::setFootprint(const dbGeoObject& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set("footprint", val, conn);
}

void LTAInventoryObject::setFootprint(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("footprint", val, conn);
}

void LTAInventoryObject::unsetFootprint(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("footprint", conn);
}

void LTAInventoryObject::setValidityFlag(bool val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<bool>("validity", val , conn);
}

void LTAInventoryObject::unsetValidityFlag(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("validity", conn);
}

void LTAInventoryObject::setFileTypeId(dbPersistent::IdType val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<dbPersistent::IdType>("filetype_id", val , conn);
}

void LTAInventoryObject::unsetFileTypeId(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("filetype_id", conn);
}

void LTAInventoryObject::setAttributesCLOB(const std::string& fpath, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	setFilePath("attributes", fpath, conn);
}

void LTAInventoryObject::unsetAttributesCLOB(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unsetFilePath("attributes", conn);
}

void LTAInventoryObject::setSatelliteId(const short& val, dbConnection&conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<short>("satellite_id", val , conn);
}

void LTAInventoryObject::unsetSatelliteId(dbConnection&conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("satellite_id", conn);
}

void LTAInventoryObject::setMission(const short& val, dbConnection&conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<short>("mission", val , conn);
}

void LTAInventoryObject::unsetMission(dbConnection&conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("mission", conn);
}


/*
 * Getters
 */
std::string LTAInventoryObject::getUUID(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("uuid", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"uuid\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

std::string LTAInventoryObject::getName(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("name", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"name\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

std::string LTAInventoryObject::getContentType(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("contenttype", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"contenttype\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

off_t LTAInventoryObject::getContentLength(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	off_t val = 0;
	bool isNull = false;
	get<off_t>("contentlength", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"contentlength\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

DateTime LTAInventoryObject::getOriginDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("origindate", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"origindate\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

DateTime LTAInventoryObject::getCreationDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("creationdate", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"creationdate\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

DateTime LTAInventoryObject::getModificationDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("modificationdate", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"modificationdate\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

DateTime LTAInventoryObject::getEvictionDate(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("evictiondate", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"evictiondate\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

std::string LTAInventoryObject::getChecksum(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("checksum", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"checksum\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

DateTime LTAInventoryObject::getValidityStart(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("validitystart", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"validitystart\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

DateTime LTAInventoryObject::getValidityStop(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	DateTime val;
	bool isNull = false;
	get<DateTime>("validitystop", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"validitystop\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

dbGeoObject* LTAInventoryObject::getFootprint(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	bool isNull = false;
	dbGeoObject* val = 0;
	get("footprint", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"footprint\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

bool LTAInventoryObject::getValidityFlag(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	bool val = false;
	bool isNull = false;
	get<bool>("validity", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"validity\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

dbPersistent::IdType LTAInventoryObject::getFileTypeId(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbPersistent::IdType val;
	bool isNull = false;
	get<dbPersistent::IdType>("filetype_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"filetype_id\" in table " + LTAInventoryObject::table()));
	}
	
	return val;
}

void LTAInventoryObject::getAttributesCLOB(dbConnection& conn, const std::string& dwnlfile, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	bool isset = isSet("attributes");
	if (isset) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Field attributes set, downloading clob.");
		downloadFile("attributes", dwnlfile, conn);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Field attributes not set");
	}
	
	if (0 != set) {
		*set = isset;
	} else {
		ACS_COND_THROW(!isset, dbSmartPersistent::NullValueException("Cannot return any value for null field \"attributes\" in table " + LTAInventoryObject::table()));
	}
}
short LTAInventoryObject::getSatelliteId(dbConnection& conn, bool* set) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	short val(0);
	bool isNull = false;
	get<short>("satellite_id", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"satelliteId\" in table " + LTAInventoryObject::table()));
	}

	return val;
}
short LTAInventoryObject::getMission(dbConnection& conn, bool* set) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	short val(0);
	bool isNull = false;
	get<short>("mission", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"mission\" in table " + LTAInventoryObject::table()));
	}

	return val;
}


_ACS_END_NESTED_NAMESPACE

