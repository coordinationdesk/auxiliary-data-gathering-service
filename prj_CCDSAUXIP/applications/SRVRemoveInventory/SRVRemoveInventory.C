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

#include <ProjectAppIncludes>

#include <SRVRemoveInventory.h>
#include <File.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbEraseFile.h>
#include <LTAInventoryObject.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <Statistics.h>
#include <Filterables.h>

#include <sriV.h>


_ACS_BEGIN_NAMESPACE(acs)

using std::vector ;
using std::string ;
using std::exception ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVRemoveInventory)

namespace { // unnamed
    sriV version;

    Application::GenericBuilder<SRVRemoveInventory> theApplicationBuilder("SRVRemoveInventory", "SRV") ;
}



/***************************************************************************************
**
                                SRVRemoveInventory 
**
***************************************************************************************/
SRVRemoveInventory::SRVRemoveInventory(const string &appName, const string &subSystem) :
	PROJECT_APP(appName,subSystem),
    _simulate(false),
    _batchSize(50UL)
{
}

/***************************************************************************************
**
                                     usage 
**
***************************************************************************************/
int SRVRemoveInventory::usage(const string& exeName) const
{
	excerr << "Usage: " << applicationName() << " [ std-opts ] [ specific-opts ] {{--name|--uuid|--id} <prod1> ... | --all}\n" ;
	excerr << "\t--name : prod1 ... are product names\n" ;
	excerr << "\t--uuid : prod1 ... are uuids\n" ;
	excerr << "\t--id   : prod1 ... are product ids\n" ;
	excerr << "\t--all  : all products are removed\n" ;
	excerr << "Specific options:\n" ; 
	excerr << "\t--simul      : do not erase the object\n" ;
	excerr << "\t--enablestat : default stats are disabled, this option allow them\n" ;
	return 1 ;
}

std::string SRVRemoveInventory::getDefaultLogName() const {
	return "./SRVRemoveInventory.log";
}

int SRVRemoveInventory::removeInventory(db::LTAInventoryObject & obj, dbConnection &theConnection) // PRQA S 4020
{    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    if(NoStop != getCurrentStopMode()) {
        return 0; 
    }
          	
    dbEraseFile* eraser = dynamic_cast<dbEraseFile*>(dbFileActionFactoryChain::instance()->newEraseAction(*getDefaultConnectionPool(), obj)); // PRQA S 3081

    ACS_COND_THROW( !eraser, exIllegalValueException("Dynamic_cast failure on dbEraseFile object.")); // PRQA S 3081

	dbFileActionScopeGuard guard(eraser);
	
	try {
        if (not _simulate) {
    	    eraser->start() ;	// erase is synchronous
			if (eraser->getRemainingSyncSM() == 0) {
				try {
					obj.remove(theConnection);
				} catch (std::exception& e) {
					SQLString rmquery = obj.removeQuery(theConnection);

					std::ostringstream ss;
					ss << "An error occurs while trying to remove the record on " << obj.getTableName()
							<< " associated to product " << obj.getName(theConnection) << ".\nQuery was: " << rmquery.getSQL(true); 
					ACS_LOG_WARNING(ss.str());

					throw;
				}
			}
        }
        else {  
            ACS_LOG_INFO("Simulating erasure of " << obj.getName(theConnection) ) ;
        }
	}
	catch( exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
	}   

	return 0 ;
} 

int SRVRemoveInventory::removeInventory(dbPersistent::IdType id, dbConnection &theConnection)
{   
    db::LTAInventoryObject obj(theConnection);

	if (not obj.load(theConnection , id)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << id << " not found");
        return 0 ;
    }

    return removeInventory(obj, theConnection) ;
}



int SRVRemoveInventory::removeSet(vector<dbPersistent::IdType>const & ids) // PRQA S 4020
{    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing " << ids.size() << " inventories");
    
 	for (auto id : ids) {
        if(NoStop != getCurrentStopMode()) {
            return 0; 
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing id: " << id);
  	    
        removeInventory(id, theConnection) ;
	}

	return 0 ;
} 

int SRVRemoveInventory::removeSet(vector<db::LTAInventoryObject>const & objects) // PRQA S 4020
{    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing " << objects.size() << " inventories");
    
 	for (auto obj : objects) {
        if(NoStop != getCurrentStopMode()) {
            return 0; 
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing id: " << obj.getId());
  	    
        removeInventory(obj, theConnection) ;
	}

	return 0 ;
} 

namespace {             ;
bool checkAndTransform(vector<string> const &prods, vector<dbPersistent::IdType> &ids) 
{
    ids.clear() ;
    for (auto&& p : prods) {// access by f-d reference, the type of i is const int&
        char *end = 0 ;
        ids.push_back(strtoll(p.c_str(), &end, 10));
        if (*end != 0) {
            excerr << "\n\"" << p << "\" is not a valid id\n\n" ;
            return false ;
        }
    }
    return true ;
}
}


/***************************************************************************************
**
                                          main
**
***************************************************************************************/
int SRVRemoveInventory::main(int, char const * const *, char const * const *) // PRQA S 5520
{
    math::Statistics::instance()->enableStatistics(false) ;
  
	const vector<string> &parsedArgs = getParsedArgs();
	vector<string> prods ;

    bool argsAreNames = false ;
    bool argsAreUUids = false ;
    bool argsAreIds = false ;
    bool all = false ;
    short exclusiveOpts = 0 ;
	if (parsedArgs.size() > 0)
	{
        
		for (size_t i=0; i < parsedArgs.size(); i++) {
			if(parsedArgs[i] == "--enablestat") {
				math::Statistics::instance()->enableStatistics(true) ;
            }
            else if (parsedArgs[i] == "--name") {
                argsAreNames = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "--uuid") {
                argsAreUUids = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "--id") {
                argsAreIds = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "--all") {
                all = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "--simul") {
                _simulate = true ;
            }
            else {
                prods.push_back(parsedArgs[i]) ;
	            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Adding prod \"" << parsedArgs[i] << "\"");
            }
	    }


    }	// END if (parsedArgs.size() > 0)

    if (exclusiveOpts != 1) {
        excerr << "\none of (--name, --uuid, --id, --all) must be provided \n\n" ;
        internalusage(appName()) ;
        return 1; 
    }
    
    if ((not all) && prods.empty()) {
        excerr << "\nno products given \n\n" ;
        internalusage(appName()) ;
        return 1; 
    }
    
    if (all && (not prods.empty())) {
        excerr << "\n--all requires no products \n\n" ;
        internalusage(appName()) ;
        return 1; 
    }
    
	try
	{
		loadConfiguration() ;
	}
	catch(std::exception &e)
	{
		ACS_THROW(exCriticalException(e,"Configuration load error")) ;
	}

	rotateLog();
	
	int status = 1;
    	
    if (all) {
        status = removeAll() ;
    }
    else {
        if (argsAreNames) {
            status = removeSet(queryNames(prods)) ;
        }
        else if (argsAreUUids) {
            status = removeSet(queryUUIDs(prods)) ;
        }
        else if (argsAreIds) {
            vector<dbPersistent::IdType> ids ;
            if (!checkAndTransform(prods, ids)) {
                internalusage(appName()) ;
                return 1; 
            }
            status = removeSet(ids) ;
        }	
        
 	}	
	
	{
		ACS_LOG_INFO("Execution finished with " << (status? "invalid ":"") << "status code: " << status << "\n") ;
	}
	
	return status ; 
}


int SRVRemoveInventory::removeAll() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

	SQLString sql(dbKey, SQLString::SELECT);
	sql.addSelectList("id");
	sql.addTableList("t_srv_inventory");
	sql.setLimit(_batchSize) ;

    for (;;) {
        if(NoStop != getCurrentStopMode()) {
            return 0; 
        }

	    dbSet set;
	    dbQuery query(theConnection, sql, "getInventoryIdFromProductNames");
	    query.execForSet(set);

        vector<dbPersistent::IdType> ids ;

	    if (!set.empty()) {
		    dbRow row(dbKey);
		    while(set.getNextRow(row)) {
                dbPersistent::IdType invid ;
		        row.column(0, invid);
                ids.push_back(invid) ;
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Adding id \"" << invid << "\"");
            }
	    }
        if(NoStop != getCurrentStopMode()) {
            return 0; 
        }
        
        if (ids.empty()) {
            return 0 ;
        }
        removeSet(ids) ;
        // because during the development or for any other reason some inventory could not be removed
        // in order to avoid neverending loop (if the remaining should be less than 
        // the batch size, we can consider complete 
        if (ids.size() < _batchSize) {
            return 0 ;
        }
    }
}

vector<db::LTAInventoryObject> SRVRemoveInventory::queryNames(vector<string> const &names) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

    vector<db::LTAInventoryObject> objects ;
    for (auto n: names) {
        if(NoStop != getCurrentStopMode()) {
            return vector<db::LTAInventoryObject>() ;  // empty set
        }
        
        db::LTAInventoryObject obj(theConnection);

	    if (obj.loadByUniqueKey(theConnection,"name",n)) {
            objects.push_back(obj) ;
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Name: " << n << " not found");
        }
    }

	return objects ;
}

vector<db::LTAInventoryObject> SRVRemoveInventory::queryUUIDs(vector<string> const &uuids) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

    vector<db::LTAInventoryObject> objects ;
    for (auto u: uuids) {
        if(NoStop != getCurrentStopMode()) {
            return vector<db::LTAInventoryObject>() ;  // empty set
        }
        db::LTAInventoryObject obj(theConnection);

	    if (obj.loadByUniqueKey(theConnection,"uuid",u)) {
            objects.push_back(obj) ;
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UUID: " << u << " not found");
        }
    }

	return objects ;
}


_ACS_END_NAMESPACE
