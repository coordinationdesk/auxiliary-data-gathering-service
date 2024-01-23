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

	$Prod: LTA PDS Db Library $

*/

#ifndef _LTAInventoryObject_H_
#define _LTAInventoryObject_H_

#include <dbSmartPersistent.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

class LTAInventoryObject : public dbSmartPersistent {
public:
	static std::string table() { return "t_srv_inventory"; }

public:
	/** Defaulted special functions */
	virtual ~LTAInventoryObject() noexcept                    = default;
	LTAInventoryObject(const LTAInventoryObject&)             = default;
	LTAInventoryObject& operator=(const LTAInventoryObject&)  = default;
	LTAInventoryObject(LTAInventoryObject&&)                  = default;
	LTAInventoryObject& operator=(LTAInventoryObject&&)       = default;
	
	/** Deleted special functions */
	LTAInventoryObject()                                      = delete;
	
	/** Constructor */
	explicit LTAInventoryObject(dbConnection&);
	
	/** Load by */
	bool loadByName(const std::string&, dbConnection&);
	bool loadByUUID(const std::string&, dbConnection&);
	
	/** Remove by */
	bool removeByName(const std::string&, dbConnection&);
	bool removeByUUID(const std::string&, dbConnection&);
	
	/** Setters */
	void setUUID(const std::string&, dbConnection&);
	void unsetUUID(dbConnection&);
	void setName(const std::string&, dbConnection&);
	void unsetName(dbConnection&);
	void setContentType(const std::string&, dbConnection&);
	void unsetContentType(dbConnection&);
	void setContentLength(off_t, dbConnection&);
	void unsetContentLength(dbConnection&);
	void setOriginDate(const DateTime&, dbConnection&);
	void unsetOriginDate(dbConnection&);
	void setCreationDate(const DateTime&, dbConnection&);
	void unsetCreationDate(dbConnection&);
	void setModificationDate(const DateTime&, dbConnection&);
	void unsetModificationDate(dbConnection&);
	void setEvictionDate(const DateTime&, dbConnection&);
	void unsetEvictionDate(dbConnection&);
	void setChecksum(const std::string&, dbConnection&);
	void unsetChecksum(dbConnection&);
	void setValidityStart(const DateTime&, dbConnection&);
	void unsetValidityStart(dbConnection&);
	void setValidityStop(const DateTime&, dbConnection&);
	void unsetValidityStop(dbConnection&);
	void setFootprint(const dbGeoObject&, dbConnection&);
	void setFootprint(const std::string&, dbConnection&);
	void unsetFootprint(dbConnection&);
	void setValidityFlag(bool, dbConnection&);
	void unsetValidityFlag(dbConnection&);
	void setFileTypeId(dbPersistent::IdType, dbConnection&);
	void unsetFileTypeId(dbConnection&);
	void setAttributesCLOB(const std::string& filepath, dbConnection&);
	void unsetAttributesCLOB(dbConnection&);
	void setSatelliteId(const short&, dbConnection&);
	void unsetSatelliteId(dbConnection&);
	void setMission(const short&, dbConnection&);
	void unsetMission(dbConnection&);
	
	/** Getters */
	std::string getUUID(dbConnection&, bool* set = 0) const;
	std::string getName(dbConnection&, bool* set = 0) const;
	std::string getContentType(dbConnection&, bool* set = 0) const;
	off_t getContentLength(dbConnection&, bool* set = 0) const;
	DateTime getOriginDate(dbConnection&, bool* set = 0) const;
	DateTime getCreationDate(dbConnection&, bool* set = 0) const;
	DateTime getModificationDate(dbConnection&, bool* set = 0) const;
	DateTime getEvictionDate(dbConnection&, bool* set = 0) const;
	std::string getChecksum(dbConnection&, bool* set = 0) const;
	DateTime getValidityStart(dbConnection&, bool* set = 0) const;
	DateTime getValidityStop(dbConnection&, bool* set = 0) const;
	dbGeoObject* getFootprint(dbConnection&, bool* set = 0) const;
	bool getValidityFlag(dbConnection&, bool* set = 0) const;
	dbPersistent::IdType getFileTypeId(dbConnection&, bool* set = 0) const;
	void getAttributesCLOB(dbConnection&, const std::string& dwnlfile, bool* set = 0) const;
	short getSatelliteId(dbConnection&, bool* set = 0) const;
	short getMission(dbConnection&, bool* set = 0) const;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAInventoryObject);
};

_ACS_END_NESTED_NAMESPACE

#endif /* _LTAInventoryObject_H_ */
