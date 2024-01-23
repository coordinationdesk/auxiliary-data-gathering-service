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

#ifndef _FileType_H_
#define _FileType_H_

#include <dbSmartPersistent.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

class FileType : public dbSmartPersistent {
public:
	static std::string table() { return "t_filetypes"; }
	
public:
	/** Defaulted special functions */
	virtual ~FileType() noexcept          = default;
	FileType(const FileType&)             = default;
	FileType& operator=(const FileType&)  = default;
	FileType(FileType&&)                  = default;
	FileType& operator=(FileType&&)       = default;
	
	/** Deleted special functions */
	FileType()                            = delete;
	
	/** Constructor */
	explicit FileType(dbConnection&);
	
	/** load by unique keys */
	bool loadByFileType(const std::string&, dbConnection&);
	
	/** remove by unique keys */
	bool removeByFileType(const std::string&, dbConnection&);
	
	/** Setters */
	void setFileType(const std::string&, dbConnection&);
	void unsetFileType(dbConnection&);
	void setGroup(const std::string&, dbConnection&);
	void unsetGroup(dbConnection&);
	void setHeaderSeparation(bool, dbConnection&);
	void unsetHeaderSeparation(dbConnection&);
	void setMngSngExt(bool, dbConnection&);
	void unsetMngSngExt(dbConnection&);
	void setNameRegularExpression(const std::string&, dbConnection&);
	void unsetNameRegularExpression(dbConnection&);
	void setDescription(const std::string&, dbConnection&);
	void unsetDescription(dbConnection&);
	void setSngExt(const std::string&, dbConnection&);
	void unsetSngExt(dbConnection&);
	void setHdrExt(const std::string&, dbConnection&);
	void unsetHdrExt(dbConnection&);
	void setDblExt(const std::string&, dbConnection&);
	void unsetDblExt(dbConnection&);
	void setInventoryPeriod(int, dbConnection&);
	void unsetInventoryPeriod(dbConnection&);
	
	/** Getters */
	std::string getFileType(dbConnection&, bool* set = 0) const;
	std::string getGroup(dbConnection&, bool* set = 0) const;
	bool getHeaderSeparation(dbConnection&, bool* set = 0) const;
	bool getMngSngExt(dbConnection&, bool* set = 0) const;
	std::string getNameRegularExpression(dbConnection&, bool* set = 0) const;
	std::string getDescription(dbConnection&, bool* set = 0) const;
	std::string getSngExt(dbConnection&, bool* set = 0) const;
	std::string getHdrExt(dbConnection&, bool* set = 0) const;
	std::string getDblExt(dbConnection&, bool* set = 0) const;
	int getInventoryPeriod(dbConnection&, bool* set = 0) const;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FileType);
};

_ACS_END_NESTED_NAMESPACE

#endif /* _FileType_H_ */
