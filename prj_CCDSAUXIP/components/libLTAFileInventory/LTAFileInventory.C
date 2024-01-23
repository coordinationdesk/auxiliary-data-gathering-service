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

#include <LTAFileInventory.h>
#include <File.h>
#include <dbLTAUploadInventoryFile.h>
#include <LTAInventoryObject.h>
#include <dbFileActionFactoryChain.h>
#include <SQLString.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <ConfigurationSingleton.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

using namespace db;

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAFileInventory);

/* Ctors / Dtors */
LTAFileInventory::LTAFileInventory(dbConnectionPool& pool) :
	mutex_(),
	pool_(pool),
	simulation_(false),
	abortUpload_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace ("libLTAFileInventory", "", false));
}

void LTAFileInventory::fillInventoryObject(const LTAMetadata& metadata, LTAInventoryObject& invobj, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	bool set = false;
	DateTime dt;
	std::string val;
	dbPersistent::IdType id = 0;
	off_t length = 0; 
	bool attrSet = false;
	
	std::string attrFile = metadata.getAttributesFile(&attrSet);
	
	// Get attribute file to be uploaded
	if (attrSet) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found the following attributes on metadata " << attrFile);
		invobj.setAttributesCLOB(attrFile, conn);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Attributes not found on metadata file.");
	}
	
	// Get name from metadata, throw exception if not found
	val = metadata.getName();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Name: " << val);
	invobj.setName(File::getFileName(val), conn);
	val.clear();
	
	// Get origindate from metadata, throw exception if not found
	dt = metadata.getPublicationDate();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PublicationDate: " << dt.asISO8601(3));
	invobj.setOriginDate(dt, conn);
	set = false;
	
	// Get validitystart from metadata, throw exception if not found
	dt = metadata.getContentDateStart();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ContentDateStart: " << dt.asISO8601(3));   
	invobj.setValidityStart(dt, conn);
	set = false;

	// Get validitystart from metadata, throw exception if not found
	dt = metadata.getContentDateEnd();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ContentDateEnd: " << dt.asISO8601(3));
	invobj.setValidityStop(dt, conn);
	set = false;
	
	// Get the footprint from metadata, not mandatory
	val = metadata.getFootprint(&set);
	if (set) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found the following footprint on metadata " << val);
		
		// Remove prefix that specifies the SRID and transform into uppercase
		val = StringUtils::uppercase(StringUtils::substringAfter(val, ";"));
		// Remove any ' at the end of the string
		val = StringUtils::trimEnd(val, '\'');
		
		invobj.setFootprint(val, conn);
		val.clear();
		set = false;
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Footprint not found on metadata file.");
	}

	// Get filetype id
	id = metadata.getFileTypeId();
	invobj.setFileTypeId(id, conn);
	set = false;
	
	// Get the checksum
	val = metadata.getChecksum();
	invobj.setChecksum(val, conn);
	val.clear();
	set = false;
	
	val = metadata.getContentType();
	invobj.setContentType(val, conn);
	val.clear();
	set = false;

	length = metadata.getContentLength();
	invobj.setContentLength(length, conn);
	length = 0;
	set = false;

	invobj.setValidityFlag(false, conn);

	// Get the satellite from metadata, throw exception if not found
	short valInt = metadata.getSatelliteId();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SatelliteId: " << valInt);
	invobj.setSatelliteId(valInt, conn);

	// Get the mission from metadata, not mandatory
	valInt = metadata.getMission(&set);
	if (set) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found the following mission on metadata " << valInt);

		invobj.setMission(valInt, conn);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission not found on metadata file.");
		bool historicalMode = false;
		
		// Detect --historical from the command line parameter when invoking SRVInventory
		// If set, since no mission has been provided in the metadata structure, force it to -1
		ConfigurationSingleton::instance()->get().getValue ("SRVInventory.historical", historicalMode, false);
		if (historicalMode)
		{
			// -1 is a fixed value to avoid misleading positive values
			ACS_LOG_INFO ("Historical mode detected and no mission set in metadate. Forcing mission to value \'-1\'");
			invobj.setMission(-1, conn);
		}
	}
	
	// useless now.. left here just in case other parameters will be read in the future
	set=false;
}


dbPersistent::IdType LTAFileInventory::inventoryProduct(const LTAMetadata& metadata) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	dbConnectionWrapper cw(pool_);
	dbConnection& conn = cw;

	// Get configuration
	unsigned long uploadPollTime = defaultUploadPollTime;
	bool removeSourceProductFlag = defaultRemoveSourceProductFlag;
	const rsResourceSet& rset = ConfigurationSingleton::instance()->get();
	if (rset.hasKey("LTAFileInventory.UploadPollTime")) {
		rset.getValue<unsigned long>("LTAFileInventory.UploadPollTime", uploadPollTime);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UploadPollTime set to " << uploadPollTime << " sec"); 
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UploadPollTime set to default value " << uploadPollTime << " sec.");
	}
	
	if (rset.hasKey("LTAFileInventory.RemoveUploadedProductFromSource")) {
		rset.getValue<bool>("LTAFileInventory.RemoveUploadedProductFromSource", removeSourceProductFlag);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoveUploadedProductFromSource set to " << std::boolalpha <<  removeSourceProductFlag << " sec"); 
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoveUploadedProductFromSource set to default value " << std::boolalpha << removeSourceProductFlag << " sec.");
	}

	std::string prodFile = metadata.getProductFile();
	std::string metaFile = metadata.getMetadataFile();
	
	ACS_LOG_INFO("Inventory of product " + prodFile + " started.");
	
	LTAInventoryObject invobj(conn);
	fillInventoryObject(metadata, invobj, conn);
	
	// In case of failure throws exception
	dbPersistent::IdType invid = invobj.save(conn);
	
	std::ostringstream ss;
	ss << "Inserted new lta inventory object associated to product " << invobj.getName(conn) << " having id " << invid;
	ACS_LOG_INFO(ss.str());
	
	// Start uploading inventory file
	try {
		dbLTAUploadInventoryFile* uploader = dynamic_cast<dbLTAUploadInventoryFile*>(dbFileActionFactoryChain::instance()->newUploadAction(pool_, invobj, prodFile)); // PRQA S 3081
		ACS_COND_THROW(!uploader, exIllegalValueException("Cannot obtain a dbLTAUploadInventoryFile instance")) ; // PRQA S 3081
		if (metadata.getForceNotValidFlag()) {
			uploader->setDoNotUpdateValidityFlag(true);
			uploader->setInvokeSpOnInventorySuccessFlag(false);
		}
		
		if (!simulation_) {
			ACS_LOG_INFO ("Starting uploading thread for product " + prodFile);
			uploader->start();
			while (uploader->uploading() && !abortUpload_) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Still uploading \"" << prodFile << '\"');
				uploader->waitExitOrTimeout(uploadPollTime * 1000);
			}
		} else {
			ACS_LOG_INFO("Upload simulated!");
		}
		
		if (abortUpload_) {
//			uploader->stop(false);
			ACS_THROW(LTAFileInventoryUploadAborted("Upload aborted as requested."));
		}
		
		ACS_LOG_INFO("Upload completed for file \"" + prodFile + "\"");
	} catch (dbLTAUploadInventoryFile::UpdateValidityException& e) {
		// Do not remove the record, only validityflag update or stored procedure failed.
		// Simply rethrow
		throw;
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
		try {
			invobj.remove(conn);
		} catch (std::exception& e) {
			SQLString rmquery = invobj.removeQuery(conn);
			
			std::ostringstream ss;
			ss << "An error occurs while trying to remove the record on " << invobj.getTableName()
					<< " associated to product " << invobj.getName(conn) << ".\nQuery was: " << rmquery.getSQL(true); 
			ACS_LOG_WARNING(ss.str());
		}
		
		ACS_THROW(LTAFileInventoryException("An error occurs while uploading file " + prodFile));
	}
	
	// Inventory successful, remove data sources
	if (removeSourceProductFlag) {
		bool attrSet = false;
		std::string attrFile = metadata.getAttributesFile(&attrSet);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing metadata file " << metaFile << " of successfully uploaded product " << prodFile << ".");
		File::unlink(prodFile);
		File::unlink(metaFile);
		if (attrSet) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing attributes file " << attrSet << " of successfully uploaded product " << prodFile << ".");
			File::unlink(attrFile);
		}
	}
	
	return invid;
}

_ACS_END_NESTED_NAMESPACE
