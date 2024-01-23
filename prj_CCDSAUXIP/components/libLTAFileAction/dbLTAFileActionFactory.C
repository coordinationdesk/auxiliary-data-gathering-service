// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
*/


#include <dbLTAFileActionFactory.h>

#include <dbLTAUploadInventoryFile.h>
#include <dbLTADownloadInventoryFile.h>
#include <dbLTAGiveUpInventoryFile.h>
#include <dbLTAEraseInventoryFile.h>
#include <dbLTAReleaseCacheInventoryFile.h>
#include <dbLTAIsOnCacheInventoryFile.h>
#include <dbLTAPutOnCacheInventoryFile.h>

#include <LTAInventoryObject.h>
#include <gfaV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::db;

namespace { // unnamed
    static gfaV version  ;
}

dbLTAFileActionFactory dbLTAFileActionFactory::_theFactory ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbLTAFileActionFactory)

dbLTAFileActionFactory::dbLTAFileActionFactory() : dbFileActionFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

dbFileAction * dbLTAFileActionFactory::newEraseAction(dbConnectionPool &pool, dbPersistent&  invobj, int storageid) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	db::LTAInventoryObject * o = dynamic_cast<db::LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	if(0 != o) {
		return new dbLTAEraseInventoryFile(pool,*o,storageid); // PRQA S 3382, 3385
	}
	return 0;
}



dbFileAction * dbLTAFileActionFactory::newUploadAction(dbConnectionPool &pool, dbPersistent& invobj, const string & path) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
	LTAInventoryObject * o = dynamic_cast<LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		return new dbLTAUploadInventoryFile(pool, *o, path);// PRQA S 3382, 3385
	}
	return  0 ;
}


dbFileAction * dbLTAFileActionFactory::newDownloadAction(dbConnectionPool &pool, dbPersistent& invobj, const string & path, const string& clientName, bool downloadInvalidFile) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	LTAInventoryObject * o = dynamic_cast<LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		return new dbLTADownloadInventoryFile(pool, *o, path, downloadInvalidFile);// PRQA S 3382, 3385
	}

	return 0;
}

dbFileAction * dbLTAFileActionFactory::newGiveUpAction(dbConnectionPool &pool, dbPersistent& invobj, const string& clientName) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	LTAInventoryObject * o = dynamic_cast<LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		return new dbLTAGiveUpInventoryFile(pool, *o);// PRQA S 3382, 3385
	}

	return 0;
}

dbFileAction *dbLTAFileActionFactory::newReleaseCacheAction(dbConnectionPool& pool, dbPersistent& invobj, int storageid) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	LTAInventoryObject * o = dynamic_cast<LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		return new dbLTAReleaseCacheInventoryFile(pool, *o, storageid);// PRQA S 3382, 3385
	}

	return 0;
}


dbFileAction *dbLTAFileActionFactory::newIsOnCacheAction(dbConnectionPool& pool, dbPersistent& invobj, int storageid) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	LTAInventoryObject * o = dynamic_cast<LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		return new dbLTAIsOnCacheInventoryFile(pool, *o, storageid);// PRQA S 3382, 3385
	}

	return 0;
}


dbFileAction *dbLTAFileActionFactory::newPutOnCacheAction(dbConnectionPool& pool, dbPersistent& invobj, int storageid) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	LTAInventoryObject * o = dynamic_cast<LTAInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		return new dbLTAPutOnCacheInventoryFile(pool, *o, storageid);// PRQA S 3382, 3385
	}

	return 0;
}


string dbLTAFileActionFactory::capabilities() const
{
	string ret ;
	ret += string("LTAInventoryObject") + "\n" ;  // PRQA S 3081
	return ret ;
}

string dbLTAFileActionFactory::getInvSmTableName(dbPersistent const &o) const
{
	if (0 != dynamic_cast<LTAInventoryObject const *>(&o)) {
	    return "t_srv_inventorysm" ;
    }
    return "" ;
}

_ACS_END_NAMESPACE
 
