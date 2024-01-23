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

#include <LTAMetadata.h>
#include <File.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAMetadata)

const std::string LTAMetadata::MTD_NAME_KEY = "Name";
const std::string LTAMetadata::MTD_CONTENTTYPE_KEY = "ContentType";
const std::string LTAMetadata::MTD_CONTENTLENGTH_KEY = "ContentLength";
const std::string LTAMetadata::MTD_CHECKSUM_KEY = "Checksum";
const std::string LTAMetadata::MTD_CONTENTDATE_KEY = "ContentDate";
const std::string LTAMetadata::MTD_CONTENTDATE_START_KEY = "Start";
const std::string LTAMetadata::MTD_CONTENTDATE_END_KEY = "End";
const std::string LTAMetadata::MTD_PUBLICATIONDATE_KEY = "PublicationDate";
const std::string LTAMetadata::MTD_FOOTPRINT_KEY = "Footprint";
const std::string LTAMetadata::MTD_SATELLITE_KEY = "Satellite_Id";
const std::string LTAMetadata::MTD_MISSION_KEY = "Mission";

/* Ctors */
LTAMetadata::LTAMetadata() :
	mutex_(),
	metadata_(),
	ftypeId_(0),
	prodFile_(),
	mtdFile_(),
	attrFile_(),
	metadataLoaded_(false),
	ftypeIdSet_(false),
	prodFileSet_(false),
	mtdFileSet_(false),
	attrFileSet_(false),
	forceNotValidFlag_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

LTAMetadata::LTAMetadata(const std::string& prodFile, const std::string& mtdFile, const std::string& attrFile, dbPersistent::IdType ftypeId) :
	mutex_(),
	metadata_(),
	ftypeId_(ftypeId),
	prodFile_(prodFile),
	mtdFile_(mtdFile),
	attrFile_(attrFile),
	metadataLoaded_(false),
	ftypeIdSet_(true),
	prodFileSet_(true),
	mtdFileSet_(true),
	attrFileSet_(true),
	forceNotValidFlag_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	loadMetadataObject();
}

/* Modifiers */
void LTAMetadata::clear() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	metadata_.clear();
	prodFile_.clear();
	mtdFile_.clear();
	attrFile_.clear();
	ftypeId_ = 0,
	metadataLoaded_ = false;
	ftypeIdSet_ = false;
	prodFileSet_ = false;
	mtdFileSet_ = false;
	attrFileSet_ = false;
	forceNotValidFlag_ = false;
}

void LTAMetadata::loadMetadataObject(const std::string& mtdfile) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if (!mtdfile.empty()) {
		setMetadataFile(mtdfile);
	}

	Lock l(mutex_);

	// throw exception if file does not exists
	ACS_COND_THROW((!File::exists(mtdFile_)), LTAMetadataException("Metadata file " + mtdFile_ + " not found"));
	
	std::ifstream istr(mtdFile_);
	
	metadata_.clear();
	istr >> metadata_;
	metadataLoaded_ = true;
}

void LTAMetadata::setMetadataJSONObject(const nlohmann::json& mtd) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	metadata_ = mtd;
	metadataLoaded_ = true;
}

void LTAMetadata::setFileTypeId(dbPersistent::IdType id) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	ftypeId_ = id;
	ftypeIdSet_ = true;
}

void LTAMetadata::setProductFile(const std::string& prodFile) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	prodFile_ = prodFile;
	prodFileSet_ = true;
}

void LTAMetadata::setMetadataFile(const std::string& mtdFile) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	mtdFile_ = mtdFile;
	mtdFileSet_ = true;
}

void LTAMetadata::setAttributesFile(const std::string& attrFile) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	attrFile_ = attrFile;
	attrFileSet_ = true;
}

void LTAMetadata::setForceNotValidFlag(bool v) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	forceNotValidFlag_ = v;
}

/* Access members */
std::string LTAMetadata::getName(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	std::string name = "";
	nlohmann::json::const_iterator it = metadata_.find(MTD_NAME_KEY);
	bool found = it != metadata_.end();
	if (found) {
		name = it->get<std::string>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_NAME_KEY << "\" -> \"" << name << "\"");
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_NAME_KEY << "\" found into metadata json object");
	}
	
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("Name not found in metadata"));
	}
	
	return name;
}
std::string LTAMetadata::getContentType(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	std::string conttype = "";
	nlohmann::json::const_iterator it = metadata_.find(MTD_CONTENTTYPE_KEY);
	bool found = it != metadata_.end();
	if (found) {
		conttype = it->get<std::string>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_CONTENTTYPE_KEY << "\" -> \"" << conttype << "\"");
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CONTENTTYPE_KEY << "\" found into metadata json object");
	}
	
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("ContentType not found in metadata"));
	}
	
	return conttype;
}

off_t LTAMetadata::getContentLength(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	off_t contlength = 0;
	nlohmann::json::const_iterator it = metadata_.find(MTD_CONTENTLENGTH_KEY);
	bool found = it != metadata_.end();
	if (found) {
		contlength = it->get<off_t>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_CONTENTLENGTH_KEY << "\" -> " << contlength);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CONTENTLENGTH_KEY << "\" found into metadata json object");
	}
	
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("ContentLength not found in metadata"));
	}
	
	return contlength;
}

std::string LTAMetadata::getChecksum(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	std::string checksum;
	nlohmann::json::const_iterator it = metadata_.find(MTD_CHECKSUM_KEY);
	bool found = it != metadata_.end();
	if (found) {
		checksum = it->dump();	// dump sub-json
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_CHECKSUM_KEY << "\" -> " << checksum);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CHECKSUM_KEY << "\" found into metadata json object");
	}
	
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("Checksum not found in metadata"));
	}
	
	return checksum;
}

nlohmann::json LTAMetadata::getChecksumJSONObject(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	bool found = false;
	std::string checksum = getChecksum(&found);
	nlohmann::json js;
	if (found) {
		js = nlohmann::json::parse(checksum);
	}
	
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("Checksum not found in metadata"));
	}
	
	return js;
}

DateTime LTAMetadata::getContentDateStart(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	bool found = false;
	std::string dateStart = "";
	nlohmann::json::const_iterator rootit = metadata_.find(MTD_CONTENTDATE_KEY);
	if (rootit == metadata_.end()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CONTENTDATE_KEY << "\" found into metadata json object");
	} else {
		nlohmann::json::const_iterator it = rootit->find(MTD_CONTENTDATE_START_KEY);
		found = it != rootit->end();
		if (found) {
			dateStart = it->get<std::string>();
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_CONTENTDATE_START_KEY << "\" -> \"" << dateStart << "\".");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CONTENTDATE_START_KEY << "\" found into metadata json.");
		}
	}

	DateTime dt = DateTime::zuluUTCString2DateTime(dateStart);
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("ContentDateStart not found in metadata"));
	}
	
	return dt;
}

DateTime LTAMetadata::getContentDateEnd(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	bool found = false;
	std::string dateEnd = "";
	nlohmann::json::const_iterator rootit = metadata_.find(MTD_CONTENTDATE_KEY);
	if (rootit == metadata_.end()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CONTENTDATE_KEY << "\" found into metadata json object");
	} else {
		nlohmann::json::const_iterator it = rootit->find(MTD_CONTENTDATE_END_KEY);
		found = it != rootit->end();
		if (found) {
			dateEnd = it->get<std::string>();
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_CONTENTDATE_END_KEY << "\" -> \"" << dateEnd << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_CONTENTDATE_END_KEY << "\" found into metadata json object");
		}
	}

	DateTime dt = DateTime::zuluUTCString2DateTime(dateEnd);
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("ContentDateEnd not found in metadata"));
	}
	
	return dt;
}

DateTime LTAMetadata::getPublicationDate(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	std::string pubdate;
	nlohmann::json::const_iterator it = metadata_.find(MTD_PUBLICATIONDATE_KEY);
	bool found = it != metadata_.end();
	if (found) {
		pubdate = it->get<std::string>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_PUBLICATIONDATE_KEY << "\" -> " << pubdate);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_PUBLICATIONDATE_KEY << "\" found into metadata json object");
	}
	
	DateTime dt = DateTime::zuluUTCString2DateTime(pubdate);
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("PublicationDate not found in metadata"));
	}
	
	return dt;
}

std::string LTAMetadata::getFootprint(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));
	
	std::string footprint;
	nlohmann::json::const_iterator it = metadata_.find(MTD_FOOTPRINT_KEY);
	bool found = it != metadata_.end();
	if (found) {
		footprint = it->get<std::string>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_FOOTPRINT_KEY << "\" -> " << footprint);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_FOOTPRINT_KEY << "\" found into metadata json object");
	}
	
	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("Footprint not found in metadata"));
	}
	
	return footprint;
}

short LTAMetadata::getSatelliteId(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));

	short satelliteId(0);
	nlohmann::json::const_iterator it = metadata_.find(MTD_SATELLITE_KEY);
	bool found = it != metadata_.end();
	if (found) {
		satelliteId = it->get<short>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_SATELLITE_KEY << "\" -> " << satelliteId);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_SATELLITE_KEY << "\" found into metadata json object");
	}

	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("Satellite_Id not found in metadata"));
	}

	return satelliteId;
}
short LTAMetadata::getMission(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	ACS_COND_THROW(!metadataLoaded_, LTAMetadataException("Metadata file not loaded yet."));

	short mission(0);
	nlohmann::json::const_iterator it = metadata_.find(MTD_MISSION_KEY);
	bool found = it != metadata_.end();
	if (found) {
		mission = it->get<short>();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found \"" << MTD_MISSION_KEY << "\" -> " << mission);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No key \"" << MTD_MISSION_KEY << "\" found into metadata json object");
	}

	if (0 != set) {
		*set = found;
	} else {
		ACS_COND_THROW(!found, LTAMetadataNotFoundException("Mission not found in metadata"));
	}

	return mission;
}

dbPersistent::IdType LTAMetadata::getFileTypeId(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	
	if (0 != set) {
		*set = ftypeIdSet_;
	} else {
		ACS_COND_THROW(!ftypeIdSet_, LTAMetadataNotFoundException("FileTypeId not found in metadata"));
	}
	
	return ftypeId_;
}

std::string LTAMetadata::getProductFile(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	
	if (0 != set) {
		*set = prodFileSet_;
	} else {
		ACS_COND_THROW(!prodFileSet_, LTAMetadataNotFoundException("Product file not set"));
	}
	
	return prodFile_;
}

std::string LTAMetadata::getMetadataFile(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	
	if (0 != set) {
		*set = mtdFileSet_;
	} else {
		ACS_COND_THROW(!mtdFileSet_, LTAMetadataNotFoundException("Metadata file not set"));
	}
	
	return mtdFile_;
}

std::string LTAMetadata::getAttributesFile(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	
	if (0 != set) {
		*set = attrFileSet_;
	} else {
		ACS_COND_THROW(!attrFileSet_, LTAMetadataNotFoundException("Attributes file not set"));
	}
	
	return attrFile_;
}

bool LTAMetadata::getForceNotValidFlag() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	return forceNotValidFlag_;
}

_ACS_END_NESTED_NAMESPACE // acs
