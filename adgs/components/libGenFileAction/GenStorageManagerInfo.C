// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 7.3  2017/11/09 10:19:42  lucio.pulvirenti
	S2PDGS-1843: minor.
	
	Revision 7.2  2017/11/09 10:07:37  lucio.pulvirenti
	S2PDGS-1843: getAvailableStorage: SMInterface getAvailableStorageInfo method called.
	
	Revision 7.1  2017/11/07 16:22:29  lucio.pulvirenti
	S2PDGS-1843: classes added.
	
	

*/


#include <GenStorageManagerInfo.h>

#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <SMInterfaceScopeGuard.h>
#include <dbSmartQuery.h>
#include <dbConnectionPool.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
	const string tableName = "t_storagemanager";
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(GenStorageManagerInfo)

// constructor
GenStorageManagerInfo::GenStorageManagerInfo(dbConnectionPool &pool):
						StorageManagerInfo(pool) 
{
}



GenStorageManagerInfo::~GenStorageManagerInfo() throw()
{
}


void GenStorageManagerInfo::getAvailableStorage( map<string, vector<pair<string, long long int > > > & smMap)
{
	// PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	dbSmartQuery q(tableName, getPool().getKey());
	{
		dbConnectionWrapper pw(getPool()) ;
    	dbConnection &conn = pw ; // PRQA S 3050
		q.doQuery(conn) ;
	}
	size_t qSize = q.size();
	for(size_t i=0 ; i< qSize ; i++) {
		
		dbPersistent::IdType smId = q.at(i)->getId();
		string smType ;
		string smName;
		{
			dbConnectionWrapper pw(getPool()) ;
    		dbConnection &conn = pw ; // PRQA S 3050
			bool isNull = false ;
			q.at(i)->get("type" , smType, isNull , conn ) ;
			q.at(i)->get("name" , smName, isNull , conn ) ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to get available storage of SM \"" << smName << "\" (id=" << smId << ", type=" << smType << ')');
		SMInterface* smInterface = SMInterfaceFactoryMethod::getInterface(smType, smId, 0, getPool()); // PRQA S 3000
		if(smInterface == 0) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No StorageManager plugin defined for type " << smType << " id " << smId );
			continue;
		}
		SMInterfaceScopeGuard guard(smInterface);

		
		try {
			vector< pair<string, long long int> > smInfo ;
			smInterface->getAvailableStorageInfo(smInfo);
			smMap[smName] = smInfo;
		}
		catch(exception &e) {
			ACS_LOG_NOTIFY_EX(e);
			ACS_LOG_ERROR("Cannot retrieve storage information for storage manager \"" << smName << "\" (id=" << smId << ", type=" << smType << ')');
		}
		
	}


}


_ACS_END_NAMESPACE
