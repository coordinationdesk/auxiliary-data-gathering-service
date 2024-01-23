// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2017/09/25 15:45:13  lucio.pulvirenti
	S2PDGS-1843: Passed optional boolean parameter to be passed to objects CTOR
	
	Revision 5.3  2016/04/28 09:20:44  marpas
	useless members removed and interface simplified
	
	Revision 5.2  2013/07/22 18:01:03  marpas
	aligning to new libDbData interface
	
	Revision 5.1  2013/06/11 14:27:21  marpas
	coding best practices applied
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.8  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.7  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.6  2013/02/14 17:57:03  marpas
	messages debug use filterable
	
	Revision 2.5  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.4  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.3  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.2  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.1  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 2.0  2006/02/28 09:09:04  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/11/04 15:34:20  marpas
	check moved away from ctor
	
	Revision 1.6  2005/11/03 13:51:51  marpas
	Operation02::chjeckData method called
	
	Revision 1.5  2005/07/21 10:50:16  marpas
	file actions for dbConfiguration implemented
	
	Revision 1.4  2005/07/21 10:24:43  marpas
	dbConfigurationSpace and its query imported from libDbCryosat
	
	Revision 1.3  2005/03/17 12:42:56  marpas
	headers fixed
	
	Revision 1.2  2003/04/30 14:01:54  marpas
	using namespace std no longer fount into acs includes
	
	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
	libDbConfiguration Import
		

*/


#include <dbResourceSpaceFactory.h>
#include <dbConfigurationFileActionFactory.h>
#include <dbResourceSpace.h>
#include <dbConfigurationSpace.h>
#include <dbConfigurationSpaceQuery.h>
#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>
#include <Operation02.h>
#include <Application.h>
#include <Filterables.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    dbConfigurationFileActionFactory theDbConfigurationFileActionFactory ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbResourceSpaceFactory)

// constructor
dbResourceSpaceFactory::dbResourceSpaceFactory(dbConnectionPool &cp) : 
	ResourceSpaceFactory(),
    _connectionPool(cp) // PRQA S 2528
{
	registerDbPersistentFactoryMethod(dbConfigurationSpace::staticClassName() , &buildDbConfigurationSpace );
	registerDbQueryFactoryMethod(dbConfigurationSpaceQuery::staticClassName() , &buildDbConfigurationSpaceQuery );

}


dbResourceSpaceFactory::~dbResourceSpaceFactory() throw() 
{
// PRQA S 4631 L1
    try {
        unregisterDbPersistentFactoryMethod(dbConfigurationSpace::staticClassName(), &buildDbConfigurationSpace );
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

    try {
        unregisterDbQueryFactoryMethod(dbConfigurationSpaceQuery::staticClassName(), &buildDbConfigurationSpaceQuery );
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
// PRQA L:L1
}


ResourceSpace *dbResourceSpaceFactory::createItem(const string &space, const string &version, bool absPath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	Operation02::checkData(Application::appName()) ;

	return new dbResourceSpace(space,version, _connectionPool, absPath) ;
}

// *************** dbConfigurationSpace ***************************

dbPersistent* dbResourceSpaceFactory::buildDbConfigurationSpace(const string &)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;	
	return new dbConfigurationSpace;
}


dbGeneralQuery* dbResourceSpaceFactory::buildDbConfigurationSpaceQuery(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;	
	return new dbConfigurationSpaceQuery(dbKey);
}

bool dbResourceSpaceFactory::hasOptionalConf() const { return false ; } 
 
_ACS_END_NAMESPACE



