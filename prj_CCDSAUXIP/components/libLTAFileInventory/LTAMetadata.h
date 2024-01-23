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

#ifndef _LTAMetadata_H_
#define _LTAMetadata_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <exMacros.h>
#include <dbGeoObject.h>
#include <Filterables.h>
#include <dbPersistent.h>
#include <string>
#include <map>
#include <vector>
#include <json.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

class LTAMetadata // PRQA S 2109
{

public:

	/** class LTAMetadataException declaration */
	exDECLARE_EXCEPTION(LTAMetadataException, exException) ; // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(LTAMetadataNotFoundException, LTAMetadataException) ; // PRQA S 2131, 2502


	/** Defaulted special functions */
	LTAMetadata(const LTAMetadata&)            = default;
	virtual ~LTAMetadata() noexcept            = default;
	LTAMetadata &operator=(const LTAMetadata&) = default;
	
	/** Ctors */
	LTAMetadata();
	LTAMetadata(const std::string& prodFile, const std::string& mtdFile, const std::string& attrFile, dbPersistent::IdType ftypeId);
	
	/** Reset all data members */
	void clear();
	
	/** 
	 * Loads the metadata from the mtdfile given as input. If the mtd file is not specified 
	 * it will try to use the member data mtdfile if specified, otherwise throws exception 
	 */
	void loadMetadataObject(const std::string& mtdfile = "");
	
	/** Setter functions */
	void setMetadataJSONObject(const nlohmann::json&);

	void setFileTypeId(dbPersistent::IdType);
	void setProductFile(const std::string&);
	void setMetadataFile(const std::string&);
	void setAttributesFile(const std::string&);
	
	/** If set to true, it tells wether or not the product must be left with validityflag to false */
	void setForceNotValidFlag(bool v);
	
	/** Access functions. If set pointer is 0 and the requested member was not provided, an exception is thrown. */
	std::string getName(bool* set = 0) const;
	std::string getContentType(bool* set = 0) const;
	off_t getContentLength(bool* set = 0) const;
	std::string getChecksum(bool* set = 0) const;
	nlohmann::json getChecksumJSONObject(bool* set = 0) const;
	DateTime getContentDateStart(bool* set = 0) const;
	DateTime getContentDateEnd(bool* set = 0) const;
	DateTime getPublicationDate(bool* set = 0) const;
	std::string getFootprint(bool* set = 0) const;
	dbPersistent::IdType getFileTypeId(bool* set = 0) const;
	std::string getProductFile(bool* set = 0) const;
	std::string getMetadataFile(bool* set = 0) const;
	std::string getAttributesFile(bool* set = 0) const;
	bool getForceNotValidFlag() const;
	short getSatelliteId(bool* set = 0) const;
	short getMission(bool* set = 0) const;
private:
	static const std::string MTD_NAME_KEY;
	static const std::string MTD_CONTENTTYPE_KEY;
	static const std::string MTD_CONTENTLENGTH_KEY;
	static const std::string MTD_CHECKSUM_KEY;
	static const std::string MTD_CONTENTDATE_KEY;
	static const std::string MTD_CONTENTDATE_START_KEY;
	static const std::string MTD_CONTENTDATE_END_KEY;
	static const std::string MTD_PUBLICATIONDATE_KEY;
	static const std::string MTD_FOOTPRINT_KEY;
	static const std::string MTD_SATELLITE_KEY;
	static const std::string MTD_MISSION_KEY;

private:
	Mutex mutex_;
	nlohmann::json metadata_;
	dbPersistent::IdType ftypeId_;
	std::string prodFile_;
	std::string mtdFile_;
	std::string attrFile_;
	bool metadataLoaded_;
	bool ftypeIdSet_;
	bool prodFileSet_;
	bool mtdFileSet_;
	bool attrFileSet_;
	
	bool forceNotValidFlag_;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAMetadata);
};


_ACS_END_NESTED_NAMESPACE

#endif //_LTAMetadata_H_
