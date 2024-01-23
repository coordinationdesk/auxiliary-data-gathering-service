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

#ifndef _LTAOrder_H_
#define _LTAOrder_H_

#include <dbSmartPersistent.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

class LTAOrder : public dbSmartPersistent {
public:
	static std::string table() { return "t_srv_ordersqueue"; }
	
public:
	/** Defaulted special functions */
	virtual ~LTAOrder() noexcept          = default;
	LTAOrder(const LTAOrder&)             = default;
	LTAOrder& operator=(const LTAOrder&)  = default;
	LTAOrder(LTAOrder&&)                  = default;
	LTAOrder& operator=(LTAOrder&&)       = default;
	
	/** Deleted special functions */
	LTAOrder()                            = delete;
	
	/** Constructor */
	explicit LTAOrder(dbConnection&);
	
	/** load by unique keys */
	bool loadByUUID(const std::string&, dbConnection&);
	
	/** remove by unique keys */
	bool removeByUUID(const std::string&, dbConnection&);
	
	/** Setters */
	void setUUID(const std::string& uuid, dbConnection&);
	void unsetUUID(dbConnection&);
	void setProductId(dbPersistent::IdType id, dbConnection&);
	void unsetProductId(dbConnection&);
	void setTargetAipId(dbPersistent::IdType id, dbConnection&);
	void unsetTargetAipId(dbConnection&);
	void setLastStatusUpdate(const DateTime& dt, dbConnection&);
	void unsetLastStatusUpdate(dbConnection&);
	void setJobResponsible(const std::string& jobresp, dbConnection&);
	void unsetJobResponsible(dbConnection&);
	void setUploadStatusId(dbPersistent::IdType id, dbConnection&);
	void unsetUploadStatusId(dbConnection&);
	void setRemoteAwsUploaderJobId(dbPersistent::IdType id, dbConnection&);
	void unsetRemoteAwsUploaderJobId(dbConnection&);
	void setRemoteAwsUploaderUrl(const std::string& dcurl, dbConnection&);
	void unsetRemoteAwsUploaderUrl(dbConnection&);
	void setCreationDate(const DateTime& dt, dbConnection&);
	void unsetCreationDate(dbConnection&);
	void setStartDate(const DateTime& dt, dbConnection&);
	void unsetStartDate(dbConnection&);
	void setCompletionDate(const DateTime& dt, dbConnection&);
	void unsetCompletionDate(dbConnection&);
	void setModificationDate(const DateTime& dt, dbConnection&);
	void unsetModificationDate(dbConnection&);
	void setEstimatedDate(const DateTime& dt, dbConnection&);
	void unsetEstimatedDate(dbConnection&);
	void setPriority(int prio, dbConnection&);
	void unsetPriority(dbConnection&);
	void setLinkCount(unsigned long cnt, dbConnection&);
	void unsetLinkCount(dbConnection&);
	void setStatusId(dbPersistent::IdType id, dbConnection&);
	void unsetStatusId(dbConnection&);
	
	/** Getters */
	std::string getUUID(dbConnection&, bool* set = 0) const;
	dbPersistent::IdType getProductId(dbConnection&, bool* set = 0) const;
	dbPersistent::IdType getTargetAipId(dbConnection&, bool* set = 0) const;
	DateTime getLastStatusUpdate(dbConnection&, bool* set = 0) const;
	std::string getJobResponsible(dbConnection&, bool* set = 0) const;
	dbPersistent::IdType getUploadStatusId(dbConnection&, bool* set = 0) const;
	dbPersistent::IdType getRemoteAwsUploaderJobId(dbConnection&, bool* set = 0) const;
	std::string getRemoteAwsUploaderUrl(dbConnection&, bool* set = 0) const;
	DateTime getCreationDate(dbConnection&, bool* set = 0) const;
	DateTime getStartDate(dbConnection&, bool* set = 0) const;
	DateTime getCompletionDate(dbConnection&, bool* set = 0) const;
	DateTime getModificationDate(dbConnection&, bool* set = 0) const;
	DateTime getEstimatedDate(dbConnection&, bool* set = 0) const;
	int getPriority(dbConnection&, bool* set = 0) const;
	unsigned long getLinkCount(dbConnection&, bool* set = 0) const;
	dbPersistent::IdType getStatusId(dbConnection&, bool* set = 0) const;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAOrder);
};

_ACS_END_NESTED_NAMESPACE

#endif /* _LTAOrder_H_ */
