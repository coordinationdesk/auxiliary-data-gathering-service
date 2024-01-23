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

#include <FileType.h>
#include <libpdsdbV.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

ACS_CLASS_DEFINE_DEBUG_LEVEL(FileType);

/*
 * Constructor
 */
FileType::FileType(dbConnection& conn) :
	dbSmartPersistent(table(), conn)
{ }

bool FileType::loadByFileType(const std::string& name, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return loadByUniqueKey<std::string>(conn, "filetype", name);
}

bool FileType::removeByFileType(const std::string& name, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return removeByUniqueKey<std::string>(conn, "filetype", name);
}

/*
 * Setters
 */
void FileType::setFileType(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("filetype", val , conn);
}

void FileType::unsetFileType(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("filetype", conn);
}

void FileType::setGroup(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("group", val , conn);
}

void FileType::unsetGroup(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("group", conn);
}

void FileType::setHeaderSeparation(bool val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<bool>("headerseparation", val , conn);
}

void FileType::unsetHeaderSeparation(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("headerseparation", conn);
}

void FileType::setMngSngExt(bool val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<bool>("mngsngext", val , conn);
}

void FileType::unsetMngSngExt(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("mngsngext", conn);
}

void FileType::setNameRegularExpression(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("nameregularexpression", val , conn);
}

void FileType::unsetNameRegularExpression(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("nameregularexpression", conn);
}

void FileType::setDescription(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("description", val , conn);
}

void FileType::unsetDescription(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("description", conn);
}

void FileType::setSngExt(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("sngext", val , conn);
}

void FileType::unsetSngExt(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("sngext", conn);
}

void FileType::setHdrExt(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("hdrext", val , conn);
}

void FileType::unsetHdrExt(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("hdrext", conn);
}

void FileType::setDblExt(const std::string& val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<std::string>("dblext", val , conn);
}

void FileType::unsetDblExt(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("dblext", conn);
}

void FileType::setInventoryPeriod(int val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set<int>("invperiod", val , conn);
}

void FileType::unsetInventoryPeriod(dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unset("invperiod", conn);
}

/*
 * Getters
 */
 
std::string FileType::getFileType(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("filetype", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"filetype\" in table " + FileType::table()));
	}
	
	return val;
}

std::string FileType::getDescription(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("description", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"description\" in table " + FileType::table()));
	}
	
	return val;
}

std::string FileType::getNameRegularExpression(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("nameregularexpression", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"nameregularexpression\" in table " + FileType::table()));
	}
	
	return val;
}

std::string FileType::getGroup(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("group", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"group\" in table " + FileType::table()));
	}
	
	return val;
}

std::string FileType::getSngExt(dbConnection& conn, bool* set) const {
	std::string val;
	bool isNull = false;
	get<std::string>("sngext", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"sngext\" in table " + FileType::table()));
	}
	
	return val;
}

std::string FileType::getHdrExt(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("hdrext", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"hdrext\" in table " + FileType::table()));
	}
	
	return val;
}

std::string FileType::getDblExt(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	bool isNull = false;
	get<std::string>("dblext", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"dblext\" in table " + FileType::table()));
	}
	
	return val;
}

int FileType::getInventoryPeriod(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	int val = 0;
	bool isNull = false;
	get<int>("invperiod", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"invperiod\" in table " + FileType::table()));
	}
	
	return val;
}

bool FileType::getHeaderSeparation(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	bool val = false;
	bool isNull = false;
	get<bool>("headerseparation", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"headerseparation\" in table " + FileType::table()));
	}
	
	return val;
}

bool FileType::getMngSngExt(dbConnection& conn, bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	bool val = false;
	bool isNull = false;
	get<bool>("mngsngext", val, isNull, conn);
	if (0 != set) {
		*set = !isNull;
	} else {
		ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException("Cannot return any value for null field \"mngsngext\" in table " + FileType::table()));
	}
	
	return val;
}

_ACS_END_NESTED_NAMESPACE

