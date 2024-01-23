// PRQA S 1050 EOF
/*

    Copyright 2020, Exprivia SpA 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    SRVRemoveInventory  

*/ 

#ifndef _SRVRemoveInventory_H_
#define _SRVRemoveInventory_H_



#if defined (HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#endif

#include <dbPersistent.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

_ACS_BEGIN_NAMESPACE(db)
    class LTAInventoryObject ; // forward
_ACS_END_NAMESPACE


class SRVRemoveInventory: 
	public PROJECT_APP
{

public:
	SRVRemoveInventory(const std::string &appName, const std::string &subSystem);
	virtual ~SRVRemoveInventory() = default ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

private:
	SRVRemoveInventory() = delete ;
	SRVRemoveInventory(const SRVRemoveInventory&) = delete ;
	SRVRemoveInventory & operator=(const SRVRemoveInventory&) = delete ;
    
	virtual std::string getDefaultLogName() const;
	virtual int usage(const std::string &) const ;
	int removeAll() ;
	int removeSet(std::vector<dbPersistent::IdType> const &) ;
	int removeSet(std::vector<db::LTAInventoryObject> const &) ;
    std::vector<db::LTAInventoryObject> queryNames(std::vector<std::string> const &) ;
    std::vector<db::LTAInventoryObject> queryUUIDs(std::vector<std::string> const &) ;
    std::vector<db::LTAInventoryObject> queryIds(std::vector<std::string> const &) ;

    int removeInventory(db::LTAInventoryObject & obj, dbConnection &theConnection) ; 
    int removeInventory(dbPersistent::IdType id, dbConnection &theConnection) ; 
    
private:
    bool _simulate ;
    size_t _batchSize ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVRemoveInventory)
};


_ACS_END_NAMESPACE


#endif /* _SRVRemoveInventory_H_ */
