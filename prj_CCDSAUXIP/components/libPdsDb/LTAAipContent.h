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

#ifndef _LTAAipContent_H_
#define _LTAAipContent_H_

#include <dbSmartPersistent.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

class LTAAipContent : public dbSmartPersistent {
public:
	static std::string table() { return "t_srv_aip_content"; }
	
	static bool loadAipContentByAipIdAndProductUUID(acs::db::LTAAipContent& content, dbPersistent::IdType aipId, const std::string& productUUID, dbConnection& conn); 
public:
	/** Defaulted special functions */
	virtual ~LTAAipContent() noexcept               = default;
	LTAAipContent(const LTAAipContent&)             = default;
	LTAAipContent& operator=(const LTAAipContent&)  = default;
	LTAAipContent(LTAAipContent&&)                  = default;
	LTAAipContent& operator=(LTAAipContent&&)       = default;
	
	/** Deleted special functions */
	LTAAipContent()                            = delete;
	
	/** Constructor */
	explicit LTAAipContent(dbConnection&);
	
	/** load by unique keys */
	bool loadByOrderId(dbPersistent::IdType, dbConnection&);
	
	/** remove by unique keys */
	bool removeByOrderId(dbPersistent::IdType, dbConnection&);
	
	/** Setters */
	void setAipId(dbPersistent::IdType id, dbConnection&);
	void unsetAipId(dbConnection&);
	void setOrderId(dbPersistent::IdType id, dbConnection&);
	void unsetOrderId(dbConnection&);
	void setProductUUID(const std::string& uuid, dbConnection&);
	void unsetProductUUID(dbConnection&);
	void setBucketName(const std::string& bn, dbConnection&);
	void unsetBucketName(dbConnection&);
	void setStatusId(dbPersistent::IdType id, dbConnection&);
	void unsetStatusId(dbConnection&);
	void setUploadTime(const DateTime& ut, dbConnection&);
	void unsetUploadTime(dbConnection&);
	void setEvictionTime(const DateTime& et, dbConnection&);
	void unsetEvictionTime(dbConnection&);
	void setProductSize(off_t ps, dbConnection&);
	void unsetProductSize(dbConnection&);
	
	/** Getters */
	dbPersistent::IdType getAipId(dbConnection& conn, bool* set = 0) const;
	dbPersistent::IdType getOrderId(dbConnection& conn, bool* set = 0) const;
	std::string getProductUUID(dbConnection& conn, bool* set = 0) const;
	std::string getBucketName(dbConnection& conn, bool* set = 0) const;
	dbPersistent::IdType getStatusId(dbConnection& conn, bool* set = 0) const;
	DateTime getUploadTime(dbConnection& conn, bool* set = 0) const;
	DateTime getEvictionTime(dbConnection& conn, bool* set = 0) const;
	off_t getProductSize(dbConnection& conn, bool* set = 0) const;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAAipContent);
};

_ACS_END_NESTED_NAMESPACE

#endif /* _FileType_H_ */
