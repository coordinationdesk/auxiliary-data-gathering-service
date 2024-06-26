// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Space Library $

	$Id$

	$Author$

	$Log$
	Revision 1.25  2016/03/26 21:44:07  marpas
	using new db I/F
	
	Revision 1.24  2016/03/22 17:14:39  marpas
	getting rid of obsolescent HMI calls
	
	Revision 1.23  2012/11/19 14:16:56  marpas
	HEAD REALIGNEMENT
	
	Revision 1.21  2012/02/13 16:50:12  marpas
	refactoring in progress
	
	Revision 1.20  2008/10/07 13:10:51  crivig
	porting to postgres8
	
	Revision 1.19  2006/01/20 14:41:13  enrcar
	DBEXPRESS compatibility added
	
	Revision 1.18  2005/10/11 07:58:23  enrcar
	getParameters() method added
	
	Revision 1.17  2005/06/17 14:09:41  marpas
	std namespace management fixed
	
	Revision 1.16  2004/08/30 17:49:36  marpas
	doQuery abstract method changed in libDbData
	
	Revision 1.15  2004/04/28 13:09:10  paoscu
	Incremental query support.
	
	Revision 1.14  2004/04/13 16:06:57  ivafam
	Corrected error in operator=
	
	Revision 1.13  2004/04/13 15:05:16  marpas
	ace dependencies removed
	dbPersistent::clone method implemented
	dbGeneralQuery::clone method implemented
	XXXQuery::operator= implemented
	
	Revision 1.12  2004/02/04 10:13:22  paoscu
	dbPersistent interface changed.
		

*/

// This file is automatically generated by dbGenerator tool

#include <dbSensorQuery.h>
#include <dbSensor.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSensorQuery)

std::string dbSensorQuery::staticClassName()
{
	return "dbSensorQuery" ;
}


// constructor
dbSensorQuery::dbSensorQuery(const string &dbkey) :
    dbGeneralQuery(dbkey)
{
}


dbSensorQuery::~dbSensorQuery() throw() {}

dbSensorQuery::dbSensorQuery(const dbSensorQuery &r) :
    dbGeneralQuery(r)
{
}

dbSensorQuery & dbSensorQuery::operator=(const dbSensorQuery &v)
{
	
	if(this != &v)
	{
		Lock lock(*this); // PRQA S 3050 2
		Lock lock1(v);
		dbGeneralQuery::operator=(v);
	}
	
	return *this;
}

dbGeneralQueryIF *dbSensorQuery::clone() const
{
	return new dbSensorQuery(*this) ;
}


unsigned long dbSensorQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}
	
	Lock lock(*this); // PRQA S 3050

	clear() ; // erase previous result set
	if (!enabled) { return 0 ; }
    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT );
    if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
    }

    queryString.addSelectList( dbSensor::getFields() );
    queryString.addTableList( dbSensor::getTable() );

// where parameters and orderby have been copied from _sqlString above 


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Query string is : \"" << queryString.getSQL() << "\"") ;
	dbQuery query(conn, queryString, "dbSensorQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new Sensor") ;
		ACS_SMARTPTR<dbSensor> record(dynamic_cast<dbSensor*>( dbPersistentFactoryChain::instance()->newObject("dbSensor", conn.getKey()))) ; // PRQA S 3081
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}
dbSensor *dbSensorQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbSensor *>(item(i)) ; // PRQA S 3081
}

const dbSensor *dbSensorQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbSensor *>(item(i)) ; // PRQA S 3081
}

_ACS_END_NAMESPACE
