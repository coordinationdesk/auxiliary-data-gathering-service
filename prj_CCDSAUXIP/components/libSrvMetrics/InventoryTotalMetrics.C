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

#include <lsrvmetV.h>
#include <InventoryTotalMetrics.h>
#include <NameIdAssociator.h>
#include <dbApplicationStatus.h>
#include <dbPersistentFactoryChain.h>
#include <Application.h>
#include <XMLIstringstream.h>
#include <XMLOstream.h>
#include <rsResourceSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace db;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InventoryTotalMetrics);

namespace {
	lsrvmetV version;
	
	const char filetypesTbl[] = "t_filetypes";
	const char filetypesId[] = "id_filetype";
	const char filetypesNm[] = "filetype";
	typedef NameIdAssociatorSingleton<filetypesTbl, filetypesId, filetypesNm, dbPersistent::IdType> FiletypesNIASingleton;
}

InventoryTotalMetrics::InventoryTotalMetrics() :
	blockSize_(defaultBlockSize),
	workingDir_("/tmp")
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

std::string InventoryTotalMetrics::getWorkingDir() const {
	return workingDir_;
}

size_t InventoryTotalMetrics::getBlockSize() const { 
	return blockSize_;
}

void InventoryTotalMetrics::setWorkingDir(const std::string& wd) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	workingDir_ = wd;
}

void InventoryTotalMetrics::setBlockSize(size_t bs) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	blockSize_ = bs;
}

void InventoryTotalMetrics::storeLastInventoryId(dbPersistent::IdType invId, const std::string& appname, const std::string& instance, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	const std::string& dbKey = conn.getKey();

	std::string filename = StringUtils::pathJoin(workingDir_, "status.xml");
	{
		std::ofstream fstr(filename);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating file " << filename);
		XMLOstream xost(fstr, "Root");
		xml::Tag lastIdTag(xost, "LastInventoryId" , std::to_string(invId), "", true);
	}
	
	dbApplicationStatus* statusRecord = dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", dbKey));
	ACS_COND_THROW(!statusRecord, exIllegalValueException("Failed dbApplicationStatus dynamic cast"));
	statusRecord->setApplication(appname);
	statusRecord->setInstance(instance);

	bool exists = statusRecord->load(conn);
	if (exists) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Replacing status file in t_applicationstatus associated to key <" << appname << ", " << instance << ">");
		statusRecord->replaceStatusFile(filename, conn);
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inserting status file " << filename << " into t_applicationstatus");
		statusRecord->setStatusUploadFileName(filename);
		statusRecord->save(conn);
	}
}

	
dbPersistent::IdType InventoryTotalMetrics::loadLastInventoryId(const std::string& appname, const std::string& instance, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	const std::string& dbKey = conn.getKey();

	dbApplicationStatus* statusRecord = dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", dbKey));
	ACS_COND_THROW(!statusRecord, exIllegalValueException("Failed dbApplicationStatus dynamic cast"));

	statusRecord->setApplication(appname);
	statusRecord->setInstance(instance);
	
	dbPersistent::IdType lastId = 0;
	if (statusRecord->load(conn)) {
		std::string filename = StringUtils::pathJoin(workingDir_, "status.xml");

		File::unlink(filename);
	
		statusRecord->saveStatusFile(filename, "", conn);
		
		XMLIstream ixst(filename);
		
		rsResourceSet rset;
		ixst >> rset;
		
		if (rset.hasKey("LastInventoryId")) {
			rset.getValue<dbPersistent::IdType>("LastInventoryId", lastId);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Last processed inventory id associated to key <" << appname + "," << instance << "> was " << std::to_string(lastId));
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No inventory id found in t_applicationstatus associated to key <" + appname + "," + instance + ">.");
		}
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No record found in t_applicationstatus associated to key <" + appname + "," + instance + ">.");
	}
	
	return lastId;
}

bool InventoryTotalMetrics::loadTotalMetric(const std::string& prodtype, const std::string& platname, const std::string& platSerial, dbSmartPersistent& sp, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	bool found = false;
	dbSmartQuery sq("t_srv_totalmetrics", dbKey);
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_srv_totalmetrics", "productType", dbParam::EQUAL_TO, dbConv(dbKey, prodtype)));
		pp.addParameter(dbParam(dbKey, "t_srv_totalmetrics", "platformShortName", dbParam::EQUAL_TO, dbConv(dbKey, platname)));
		pp.addParameter(dbParam(dbKey, "t_srv_totalmetrics", "platformSerialIdentifier", dbParam::EQUAL_TO, dbConv(dbKey, platSerial)));
		pp.addParameter(dbParam(dbKey, "t_srv_totalmetrics", "servicealias", dbParam::ISNULL));
		sq.addParameter(pp);
	}
	
	sq.doQuery(conn);
	
	if (sq.size() == 1) {
		found = true;
		sp = *sq.at(0);
	}
	else if (sq.size() > 1) {
		// This should never happen
		ACS_THROW(InventoryTotalMetricsException("Unexpected error. Found more than one record for key <productType, platformShortName, platformSerialIdentifier> = <" + prodtype + ", " + platname + ", " + platSerial + ">."));
	}
	else {
		// no record found
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No record found for key <productType, platformShortName, platformSerialIdentifier> = <" + prodtype + ", " + platname + ", " + platSerial + ">.");
	}
	
	return found;
}


dbSmartQuery* InventoryTotalMetrics::loadNextInventory(dbPersistent::IdType lastInvId, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	dbSmartQuery* sq = new dbSmartQuery("t_srv_inventory", dbKey);
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_srv_inventory", "id", dbParam::MAJOR_OF, dbConv(dbKey, lastInvId)));
		sq->addParameter(pp);
	}
	sq->maxItems(blockSize_);
	sq->addOrderBy("id", SQLString::ASC);
	sq->doQuery(conn);
	
	return sq;
}

std::string InventoryTotalMetrics::platformShortName(const std::string& acronym) {
	std::string ret;
	if (StringUtils::equalsNoCase(acronym, "S1")) {
		ret = "SENTINEL-1";
	}
	else if (StringUtils::equalsNoCase(acronym, "S2")) {
		ret = "SENTINEL-2";
	}
	else if (StringUtils::equalsNoCase(acronym, "S3")) {
		ret = "SENTINEL-3";
	}
	else {
		ACS_THROW(exIllegalValueException("Unknown platform acronym \"" + acronym + "\"")); 
	}
	
	return ret;
}


void InventoryTotalMetrics::upsertTotalMetrics(const dbSmartPersistent& sp, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	bool isNull = false;
	
	// Get the product name
	std::string prodname;
	sp.get<std::string>("name", prodname, isNull, conn);

	// Get the product type
	dbPersistent::IdType ftypeId;
	sp.get<dbPersistent::IdType>("filetype_id", ftypeId, isNull, conn);
	
	// Get filetype and remove first three characters
	std::string filetype = FiletypesNIASingleton::instance(conn.getKey())->name(ftypeId);
	filetype.erase(filetype.begin(), filetype.begin() + 3);
	
	// Get the platform
	std::string platform = platformShortName(StringUtils::substring(prodname, 0, 2));
	std::string platformSerialId = StringUtils::substring(prodname, 2, 3);
	
	// Get size
	size_t contentLength;
	sp.get<size_t>("contentlength", contentLength, isNull, conn);
	
	// Prepare INSERT ON CONFLICT DO UPDATE
	std::ostringstream os;
	os << "INSERT INTO t_srv_totalmetrics("
			"producttype, "
			"platformshortname, "
			"platformserialidentifier, "
			"archivecount, "
			"archivesize) "
		"VALUES ("
			<< "'" << filetype << "', "
			<< "'" << platform << "', "
			<< "'" << platformSerialId << "', "
			<< 1LL << ", "
			<< contentLength << ") "
	<< "ON CONFLICT (producttype, platformshortname, platformserialidentifier, COALESCE(servicealias, 'DUMMY')) DO "
			"UPDATE SET "
				"archivecount = t_srv_totalmetrics.archivecount + 1, "
				"archivesize = t_srv_totalmetrics.archivesize + EXCLUDED.archivesize;";
	

	SQLString sqlString(dbKey, SQLString::COMMAND);
	sqlString.forceSQLString(os.str());
	
	dbQuery query(conn, sqlString, "Upsert Total Metrics");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running query: " << sqlString.getSQL());
	ACS_COND_THROW(!query.execForStatus(), InventoryTotalMetricsException("Cannot execute upsert query."));

}

size_t InventoryTotalMetrics::processNextInventoryBlock(const std::string& appname, const std::string& instance, dbPersistent::IdType& lastInvId, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// Load next inventory records
	std::unique_ptr<dbSmartQuery> sqPtr(loadNextInventory(lastInvId, conn));
	
	size_t loadedEntries = sqPtr->size();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << std::to_string(loadedEntries) << " inventory records from t_srv_inventory.");
	
	if (loadedEntries > 0) {
		// Process each record separately
		size_t count = 0;
		for (size_t idx = 0; idx < loadedEntries; ++idx) {
			const dbSmartPersistent& invSp = *sqPtr->at(idx);
			
			lastInvId = invSp.getId();

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing inventory record having id = " << lastInvId);
			
			try {
				upsertTotalMetrics(invSp, conn);
				++count;
			} catch (std::exception& e) {
				ACS_LOG_NOTIFY_EX(e);
				ACS_LOG_WARNING("An error occurs processing inventory record having id = " << lastInvId << ". Processing will continue.");
			}
		}
		
		// Update applicationstatus with the last processed inventoryId
		storeLastInventoryId(lastInvId, appname, instance, conn);
		
		ACS_LOG_INFO("Inserted/Updated " << std::to_string(count) << " metrics in table t_srv_totalmetrics");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No inventory records to be processed");
	}
	return loadedEntries;
}

_ACS_END_NAMESPACE
