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

#ifndef _LTAAip_H_
#define _LTAAip_H_

#include <dbSmartPersistent.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

class LTAAip : public dbSmartPersistent {
public:
	static std::string table() { return "t_srv_aip"; }
	
public:
	/** Defaulted special functions */
	virtual ~LTAAip() noexcept        = default;
	LTAAip(const LTAAip&)             = default;
	LTAAip& operator=(const LTAAip&)  = default;
	LTAAip(LTAAip&&)                  = default;
	LTAAip& operator=(LTAAip&&)       = default;
	
	/** Deleted special functions */
	LTAAip()                          = delete;
	
	/** Constructor */
	explicit LTAAip(dbConnection&);
	
	/** load by unique keys */
	bool loadByName(const std::string&, dbConnection&);
	
	/** remove by unique keys */
	bool removeByName(const std::string&, dbConnection&);
	
	/** Setters */
	void setAipName(const std::string&, dbConnection&);
	void unsetAipName(dbConnection&);
	void setUrl(const std::string&, dbConnection&);
	void unsetUrl(dbConnection&);
	void setUsername(const std::string&, dbConnection&);
	void unsetUsername(dbConnection&);
	void setPassword(const std::string&, dbConnection&);
	void unsetPassword(dbConnection&);
	void setDiskSpace(off_t, dbConnection&);
	void unsetDiskSpace(dbConnection&);
	void setBaseRedirectUrl(const std::string&, dbConnection&);
	void unsetBaseRedirectUrl(dbConnection&);
	void setParameters(const std::string&, dbConnection&);
	void unsetParameters(dbConnection&);
	void setRetentionTimeSecs(int, dbConnection&);
	void unsetRetentionTimeSecs(dbConnection&);
	
	/** Getters */
	std::string getAipName(dbConnection&, bool* set = 0) const;
	std::string getUrl(dbConnection&, bool* set = 0) const;
	std::string getUsername(dbConnection&, bool* set = 0) const;
	std::string getPassword(dbConnection&, bool* set = 0) const;
	off_t getDiskSpace(dbConnection&, bool* set = 0) const;
	std::string getBaseRedirectUrl(dbConnection&, bool* set = 0) const;
	std::string getParameters(dbConnection&, bool* set = 0) const;
	int getRetentionTimeSecs(dbConnection&, bool* set = 0) const;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAAip);
};

_ACS_END_NESTED_NAMESPACE

#endif /* _LTAAip_H_ */
