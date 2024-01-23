// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.11  2013/07/22 17:51:06  marpas
	aligned to new libDbData interface
	
	Revision 1.10  2013/02/06 16:12:24  marpas
	some quality best practice applied
	some qa rule fixed
	
	Revision 1.9  2012/11/19 14:16:56  marpas
	HEAD REALIGNEMENT
	
	Revision 1.7  2012/02/13 16:50:12  marpas
	refactoring in progress
	
	Revision 1.6  2005/10/20 17:59:22  marpas
	embedded factory declaration removed. Now needs to be explicitely declared
	
	Revision 1.5  2005/06/17 14:09:41  marpas
	std namespace management fixed
	
	Revision 1.4  2005/06/08 16:20:05  marpas
	headers fixed
	
	Revision 1.3  2003/10/21 10:39:32  paoscu
	staticClassName used.
	
	Revision 1.2  2003/07/14 13:47:06  marpas
	useless string ctor removed
	
	Revision 1.1  2002/11/06 13:27:25  danalt
	allineamento con dbCryosat
	
		

*/

#include <dbSpaceFactory.h>

#include <dbSatRelation.h>
#include <dbSatRelationQuery.h>

#include <dbStation.h>
#include <dbStationQuery.h>

#include <dbSensor.h>
#include <dbSensorQuery.h>

#include <dbSatellite.h>
#include <dbSatelliteQuery.h>

#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>


_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSpaceFactory);



dbSpaceFactory::dbSpaceFactory()
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	registerDbPersistentFactoryMethod(dbSatRelation::staticClassName(), &buildDbSatRelation );
	registerDbQueryFactoryMethod(dbSatRelationQuery::staticClassName(), &buildDbSatRelationQuery );

	registerDbPersistentFactoryMethod(dbStation::staticClassName(), &buildDbStation );
	registerDbQueryFactoryMethod(dbStationQuery::staticClassName(), &buildDbStationQuery );

	registerDbPersistentFactoryMethod(dbSensor::staticClassName(), &buildDbSensor );
	registerDbQueryFactoryMethod(dbSensorQuery::staticClassName(), &buildDbSensorQuery );

	registerDbPersistentFactoryMethod(dbSatellite::staticClassName(), &buildDbSatellite );
	registerDbQueryFactoryMethod(dbSatelliteQuery::staticClassName(), &buildDbSatelliteQuery );
}


dbSpaceFactory::~dbSpaceFactory() throw() 
{
    // PRQA S 4631 L_dtor 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    try {
        unregisterDbPersistentFactoryMethod(dbSatRelation::staticClassName(), &buildDbSatRelation );
        unregisterDbQueryFactoryMethod(dbSatRelationQuery::staticClassName(), &buildDbSatRelationQuery );

        unregisterDbPersistentFactoryMethod(dbStation::staticClassName(), &buildDbStation );
        unregisterDbQueryFactoryMethod(dbStationQuery::staticClassName(), &buildDbStationQuery );

        unregisterDbPersistentFactoryMethod(dbSensor::staticClassName(), &buildDbSensor );
        unregisterDbQueryFactoryMethod(dbSensorQuery::staticClassName(), &buildDbSensorQuery );

        unregisterDbPersistentFactoryMethod(dbSatellite::staticClassName(), &buildDbSatellite );
        unregisterDbQueryFactoryMethod(dbSatelliteQuery::staticClassName(), &buildDbSatelliteQuery );
    }
    catch(std::exception &x) {
    	ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L_dtor
}

// *************** dbSatRelation ***************************

dbPersistent* dbSpaceFactory::buildDbSatRelation(const string &)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbSatRelation;
}


dbGeneralQuery* dbSpaceFactory::buildDbSatRelationQuery(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbSatRelationQuery(dbKey);
}

// *************** dbStation ***************************

dbPersistent* dbSpaceFactory::buildDbStation(const string &)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbStation;
}


dbGeneralQuery* dbSpaceFactory::buildDbStationQuery(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbStationQuery(dbKey);
}

// *************** dbSensor ***************************

dbPersistent* dbSpaceFactory::buildDbSensor(const string &)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbSensor;
}


dbGeneralQuery* dbSpaceFactory::buildDbSensorQuery(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbSensorQuery(dbKey);
}

// *************** dbSatellite ***************************

dbPersistent* dbSpaceFactory::buildDbSatellite(const string &)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbSatellite;
}


dbGeneralQuery* dbSpaceFactory::buildDbSatelliteQuery(const string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new dbSatelliteQuery(dbKey) ;
}


_ACS_END_NAMESPACE

