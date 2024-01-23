// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 6.2  2013/10/10 17:42:46  marpas
	dbUploadInventoryFile can be configured to upload blobs possibly conditioned by size
	cond space gives an example of configuration
	PDS_OLD_SCHEMA no longer supported.
	
	Revision 6.1  2013/09/25 16:58:18  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.7  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.6  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.5  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.4  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.3  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.2  2011/04/05 16:52:35  marpas
	Tree management work in progress
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	

*/


#include <dbGenFileActionFactory.h>
#include <dbEraseInventoryFile.h>
#include <dbDownloadInventoryFile.h>
#include <dbGiveUpInventoryFile.h>
#include <dbInventoryObject.h>
#include <dbUploadInventoryFile.h>
#include <dbPersistentFactoryChain.h>
#include <Filterables.h>
#include <gfaV.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

dbGenFileActionFactory dbGenFileActionFactory::_theFactory ;
namespace {
    gfaV version ;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGenFileActionFactory)

dbGenFileActionFactory::dbGenFileActionFactory() : dbFileActionFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	// NB this is done by dbDownloadInventoryFile
	// ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("libGenFileAction", "", false)) ;
}

dbGenFileActionFactory::~dbGenFileActionFactory() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
    // PRQA L:L1
}

dbFileAction * dbGenFileActionFactory::newEraseAction(dbConnectionPool &pool, dbPersistent&  invobj, int storageid) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	dbInventoryObject * o = dynamic_cast<dbInventoryObject*>(&invobj) ; // PRQA S 3081
	return (0 != o) ?  new dbEraseInventoryFile(pool,*o,storageid) : 0 ; // PRQA S 3382, 3385
}



dbFileAction * dbGenFileActionFactory::newUploadAction(dbConnectionPool &pool, dbPersistent& invobj, const string & path) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
	dbInventoryObject * o = dynamic_cast<dbInventoryObject*>(&invobj) ; // PRQA S 3081
	
	return (0 != o) ?  new dbUploadInventoryFile(pool, *o,path) : 0 ;// PRQA S 3382, 3385
}


dbFileAction * dbGenFileActionFactory::newDownloadAction(dbConnectionPool &pool, dbPersistent& invobj, const string & path, const string& clientName, bool downloadInvalidFile) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	dbInventoryObject * o = dynamic_cast<dbInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "(dbInventoryObject)") ;
		return new dbDownloadInventoryFile(pool, *o, path, clientName, downloadInvalidFile);// PRQA S 3382, 3385
	}

	return 0;
}

dbFileAction * dbGenFileActionFactory::newGiveUpAction(dbConnectionPool &pool, dbPersistent& invobj, const string& clientName) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
		
	dbInventoryObject * o = dynamic_cast<dbInventoryObject*>(&invobj) ; // PRQA S 3081
	
	if(0 != o) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "(dbInventoryObject)") ;
		return new dbGiveUpInventoryFile(pool, *o);// PRQA S 3382, 3385
	}

	return 0;
}


string dbGenFileActionFactory::capabilities() const
{
	string ret ;
	ret += string("InventoryObject") + "\n" ;  // PRQA S 3081
	return ret ;
}

string dbGenFileActionFactory::getInvSmTableName(dbPersistent const &o) const
{
	if (0 != dynamic_cast<dbInventoryObject const *>(&o)) {
	    return "t_inventorysm" ;
    }
    return "" ;
}

_ACS_END_NAMESPACE
 
