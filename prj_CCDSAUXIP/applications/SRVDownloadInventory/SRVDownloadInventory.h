// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SRVDownloadInventory $
	

*/

#include <ProjectAppIncludes>
#include <LTAInventoryObject.h>

_ACS_BEGIN_NAMESPACE(acs)


class SRVDownloadInventory: 
	public PROJECT_APP
{

public:
	SRVDownloadInventory(const std::string &appName, const std::string &subSystem);
	virtual ~SRVDownloadInventory() = default ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

	virtual std::string getDefaultLogName() const;

private:
	SRVDownloadInventory(const SRVDownloadInventory&) = delete ;
	SRVDownloadInventory & operator=(const SRVDownloadInventory&) = delete ;

	virtual int usage(const std::string &) const ;

    std::vector<db::LTAInventoryObject> queryNames(std::vector<std::string> const &) ;
    std::vector<db::LTAInventoryObject> queryUUIDs(std::vector<std::string> const &) ;
    std::vector<db::LTAInventoryObject> queryIds(std::vector<std::string> const &) ;

	int downloadSet(std::vector<dbPersistent::IdType> const &) ;
	int downloadSet(std::vector<db::LTAInventoryObject> const &) ;

    int downloadInventory(db::LTAInventoryObject & obj, dbConnection &theConnection) ; 
    int downloadInventory(dbPersistent::IdType id, dbConnection &theConnection) ; 

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL (SRVDownloadInventory);

	std::string			 _targetDir ;
    off_t                _totalSize ;
    Timer::Delay         _total_msec ;
};

_ACS_END_NAMESPACE
