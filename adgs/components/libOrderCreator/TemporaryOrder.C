// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libOrderCreator $

	$Id$

	$Author$

	$Log$
	Revision 5.12  2017/10/19 17:41:52  marpas
	dtor shall not throw
	
	Revision 5.11  2016/07/25 16:56:11  marfav
	CSGACQ-86
	Plain SQL replaced with SQLString
	Old SQL code still in the sources as reference
	To be deleted after complete test
	
	Revision 5.10  2016/07/20 09:11:22  marfav
	CSGACQ-86 Using dialect for current time retrieval
	
	Revision 5.9  2016/03/26 21:38:53  marpas
	using new db I/F
	
	Revision 5.8  2016/03/10 17:29:33  marpas
	SNAV-39 issue fixed - emitTime and now comparison fixed
	
	Revision 5.7  2016/03/01 17:10:55  nicvac
	OPMAN-155: qualified was missing in getSqlCommandOrderBy.
	
	Revision 5.6  2016/01/11 17:41:28  francesco.avanzi
	 now sorting correctly
	
	Revision 5.5  2014/02/12 14:32:49  francesco.avanzi
	better code practices applied
	
	Revision 5.4  2013/09/03 14:25:25  marfav
	Query syntax fixed
	
	Revision 5.3  2013/07/22 16:32:07  marpas
	compilation warnngs fixed
	
	Revision 5.2  2013/07/22 13:40:18  marpas
	ADC-193 implemented
	coding besta practices enforced
	compilation and qa warnings fixed
	
	Revision 5.1  2013/06/19 18:05:58  marpas
	fixing types
	
	Revision 5.0  2013/06/19 13:54:02  marpas
	satelliteid is now unsigned int - to be coherent with its NameIdAssociator
	OrderCreator has 2 static methods to deal with null satellite.
	libException 5.x standards adopted
	qa rules
	
	Revision 2.21  2013/04/12 12:17:44  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.20  2013/03/07 19:42:40  marpas
	DUP inventory management as in jira ADC-122
	
	Revision 2.19  2013/02/26 18:12:39  marpas
	new dbSet i/F adopted
	
	Revision 2.18  2013/02/12 12:43:50  marpas
	dbConnectionPool enforced
	interface rationalization
	coding best practices applied
	qa rules
	
	Revision 2.17  2013/02/11 12:50:20  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.16  2012/10/10 13:17:50  marfav
	Missing comma in sql statements added
	
	Revision 2.15  2012/09/17 17:07:49  marpas
	new method to get conf values,
	new class TriggeringFile with triggering filename, fileversion and orderid - 0 if was null
	updated XMLOrderParameters class, writes also the generating orderid for the triggering file(s)
	update TemporaryOrder with new queries and a method to get the generating orderid for the triggering file
	
	Revision 2.14  2012/09/17 10:17:23  marpas
	triggering file orderid read from temporary order
	
	Revision 2.13  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	
	Revision 2.12  2010/06/23 14:57:08  marpas
	null satellite management in progress
	
	Revision 2.11  2010/03/08 14:56:26  enrcar
	EC:: Oracle DB Handled
	
	Revision 2.10  2009/09/29 14:08:24  marpas
	missing include added
	
	Revision 2.9  2009/07/01 16:18:31  marpas
	mission management on triggering file added
	
	Revision 2.8  2009/05/15 15:26:38  marpas
	wrong order by column name - fixed
	
	Revision 2.7  2009/05/14 12:14:13  marpas
	temporary order deletion has its own statistic name
	
	Revision 2.6  2009/05/12 10:10:20  marpas
	sites() const method added
	
	Revision 2.5  2009/05/07 09:46:43  marpas
	new method added: isInventoryExpired, isTmpOrderExpired
	older method isExpired deprecated.
	This is in preparation to solve VEN-1198
	
	Revision 2.4  2009/01/29 13:33:36  marpas
	debug message fixed
	
	Revision 2.3  2008/11/25 15:01:51  marpas
	wrong const attribute on integral value removed
	
	Revision 2.2  2008/10/07 12:44:23  crivig
	porting to postgres8
	
	Revision 2.1  2006/04/05 15:07:19  giucas
	debug improved
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2006/02/27 10:08:11  marpas
	queries fixed: last semicolon dropped
	
	Revision 1.3  2005/10/24 17:42:33  marpas
	dbexpress compatibilty
	
	Revision 1.2  2005/10/06 15:18:37  marfav
	Header tags fixed
	

*/
#include <TemporaryOrder.h>
#include <OrderCreator.h>
#include <SatelliteName.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbConv.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <Filterables.h>
#include <sstream>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



TemporaryOrder::UniqueEntry::UniqueEntry() :
    filetype(),
    processorName(),
    processorVersion(),
    processorConfiguration(),
    satelliteId()
{
}

TemporaryOrder::UniqueEntry::UniqueEntry(const UniqueEntry &ue) :
    filetype(ue.filetype),
    processorName(ue.processorName),
    processorVersion(ue.processorVersion),
    processorConfiguration(ue.processorConfiguration),
    satelliteId(ue.satelliteId)

{
	//empty 
}

TemporaryOrder::UniqueEntry::~UniqueEntry() throw() 
{
}

TemporaryOrder::UniqueEntry &TemporaryOrder::UniqueEntry::operator=(const UniqueEntry &ue) 
{
	if (this != &ue) {
		filetype = ue.filetype ;
		processorName = ue.processorName ;
		processorVersion = ue.processorVersion ; 
		processorConfiguration = ue.processorConfiguration ;
		satelliteId = ue.satelliteId ;
	}
	return *this;
}

bool operator==(const TemporaryOrder::UniqueEntry &l, const TemporaryOrder::UniqueEntry &r)  // PRQA S 4020
{
		if (l.filetype != r.filetype) { return false ; }
		if (l.processorName != r.processorName) { return false ; }
		if (l.processorVersion != r.processorVersion) { return false ; }
		if (l.processorConfiguration != r.processorConfiguration) { return false ; }
		if (l.satelliteId != r.satelliteId) { return false ; }
		return true ;
}



void  TemporaryOrder::UniqueEntry::loadRow(const dbRow &row) 
{
	row.column( 0, filetype) ;
	row.column( 1, processorName) ;
	row.column( 2, processorVersion) ;      // PRQA S 4400 4
	row.column( 3, processorConfiguration) ;
	bool satIdNull = false ;
	row.column( 4, satelliteId, &satIdNull) ;
	if (satIdNull) { satelliteId = OrderCreator::getNullSatId() ; }
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}





ACS_CLASS_DEFINE_DEBUG_LEVEL(TemporaryOrder) 

Mutex TemporaryOrder::_tdmutex;
bool TemporaryOrder::_hasTimeDrivenFields = false;
bool TemporaryOrder::_isTimeDrivenInit = false;

void TemporaryOrder::initHasTimeDrivenFields(const std::string& dbKey) {
	if (!_isTimeDrivenInit) {
		bool value = false;
		dbConnectionWrapper connWrapper(db::ConnPools::instance()->getPool(dbKey));
		value = (dbSchemaMetaDataProvider::hasField( "t_temporaryorders", "time_driven_start", connWrapper) && 		// PRQA S 3050, 3081
					( dbSchemaMetaDataProvider::hasField( "t_temporaryorders", "time_driven_stop", connWrapper)));	// PRQA S 3050, 3081
		
		Lock lock(_tdmutex);
		
		_hasTimeDrivenFields = value;
		_isTimeDrivenInit = true;
	}
}

TemporaryOrder::TemporaryOrder() : 
	_id(0),
    _fileName(),
    _fileVersion(),
    _algorithm(),
    _fileType(),
    _orderType(),
    _processorName(),
    _processorVersion(),
    _processorConfiguration(),
	_validityFlag(false),
    _start(),
    _stop(),
    _inventoryDate(),
    _equatorxtime(),
    _priority(),
    _troubleShooting(),
    _processingTimeWindow(),
    _ordersat(),
    _ordersatNull(),
    _mission(),
    _missionNull(),
    _tstamp(),
    _sites(),
    _orderId(),
    _orderIdNull(),
    _qualityInfo(),
    _qualified(),
    _timeDrivenStart(),
    _timeDrivenStartNull(true),
    _timeDrivenStop(),
    _timeDrivenStopNull(true)
{
}


TemporaryOrder::TemporaryOrder(const TemporaryOrder &t) : 
    _id(t._id ),
	_fileName(t._fileName ),
	_fileVersion(t._fileVersion ),
	_algorithm(t._algorithm ),
	_fileType(t._fileType ),
	_orderType(t._orderType ),
	_processorName(t._processorName ),
	_processorVersion(t._processorVersion ),
	_processorConfiguration(t._processorConfiguration ),
	_validityFlag(t._validityFlag ),
	_start(t._start ),
	_stop(t._stop),
	_inventoryDate(t._inventoryDate),
	_equatorxtime(t._equatorxtime),
	_priority(t._priority),
	_troubleShooting(t._troubleShooting),
	_processingTimeWindow(t._processingTimeWindow),
	_ordersat(t._ordersat),
	_ordersatNull(t._ordersatNull),
	_mission(t._mission),
	_missionNull(t._missionNull),
	_tstamp(t._tstamp),
	_sites(t._sites ),
    _orderId(t._orderId),
    _orderIdNull(t._orderIdNull),
    _qualityInfo(t._qualityInfo),
    _qualified(t._qualified),
    _timeDrivenStart(t._timeDrivenStart),
    _timeDrivenStartNull(t._timeDrivenStartNull),
    _timeDrivenStop(t._timeDrivenStop),
    _timeDrivenStopNull(t._timeDrivenStopNull)
{
	// empty 
}

TemporaryOrder::~TemporaryOrder() throw() {	}

TemporaryOrder &TemporaryOrder::operator=(const TemporaryOrder &t)
{
	if(this != &t) {
		
		_id = t._id ;
		_fileName = t._fileName ;
		_fileVersion = t._fileVersion ;
		_algorithm = t._algorithm ;
		_fileType = t._fileType ;
		_orderType = t._orderType ;
		_processorName = t._processorName ;
		_processorVersion = t._processorVersion ;
		_processorConfiguration = t._processorConfiguration ;
		_validityFlag = t._validityFlag ;
		_start = t._start ;
		_stop = t._stop;
		_inventoryDate = t._inventoryDate;
		_equatorxtime = t._equatorxtime;
		_priority = t._priority;
		_troubleShooting = t._troubleShooting;
		_processingTimeWindow = t._processingTimeWindow;
		_ordersat = t._ordersat;
		_ordersatNull = t._ordersatNull;
		_mission = t._mission;
		_missionNull = t._missionNull;
		_tstamp = t._tstamp;
		_sites = t._sites ;
        _orderId = t._orderId ;
        _orderIdNull = t._orderIdNull ;
        _qualityInfo = t._qualityInfo ;
        _qualified = t._qualified ;
		_timeDrivenStart = t._timeDrivenStart ;
		_timeDrivenStartNull = t._timeDrivenStartNull ;
		_timeDrivenStop = t._timeDrivenStop ;
		_timeDrivenStopNull = t._timeDrivenStopNull ;
	}
	
	return *this ;
}


bool TemporaryOrder::isJoinable(const TemporaryOrder & o, double dtime) const // PRQA S 4020
{
	DateTime::JD50  jd_stop = _stop;		// PRQA S 3050
	DateTime::JD50  jd_start = o._start;	// PRQA S 3050
	if (!_timeDrivenStopNull && !o._timeDrivenStartNull) {
		jd_stop  = _timeDrivenStop ; // PRQA S 3050 2
		jd_start = o._timeDrivenStart ;
	}
	
	if((double(jd_start) - double(jd_stop) < dtime) && (_fileType == o._fileType)) { // PRQA S 3050, 3081
		return true ;
    }
	else {
		return false ;
    }
}

size_t TemporaryOrder::orderChain(const vector<TemporaryOrder> &orders, size_t current, double mergeDistance) const // PRQA S 4020, 4214
{
    size_t os = orders.size() ;
	for(size_t i = current ; i < os - 1 ; i++) { // PRQA S 3084
				
		DateTime::JD50  jd_stop1  = orders[i    ].stop() ; // PRQA S 3050 2
		DateTime::JD50  jd_start2 = orders[i + 1].start() ; // PRQA S 3084

		bool isNullStop(true);
		bool isNullStart(true);
		DateTime::JD50 timeDrivenStop  = orders[i    ].timeDrivenStop(&isNullStop);		// PRQA S 3050
		DateTime::JD50 timeDrivenStart = orders[i + 1].timeDrivenStart(&isNullStart);	// PRQA S 3050, 3084
		if (!isNullStop && !isNullStart) {
			jd_stop1 = timeDrivenStop;
			jd_start2 = timeDrivenStart;
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "\n" // PRQA S 3050, 3081, 3084 3
            << "[" << i << "],["<< i+1<< "]: " << double(jd_start2)-double(jd_stop1) << " " << mergeDistance << "\n"
			<< "[" << i << "],["<< i+1<< "]: " << orders[i].fileType() << " " << orders[i + 1].fileType() ) ;
		if(!(
			((double(jd_start2) - double(jd_stop1)) < mergeDistance) && // PRQA S 3050, 3081 2
			(orders[i].fileType() == orders[i + 1].fileType())	)) { // PRQA S 3084
		
			return i ;
		}
	}
	// if here this means that the chain is until the end, so returns orders.size() - 1
	return orders.size() - 1 ; // PRQA S 3084
}

TemporaryOrder TemporaryOrder::join(const TemporaryOrder & o) // PRQA S 4211
{
	TemporaryOrder joint ;
	joint = *this ;
	joint._stop = o._stop ;
	joint._timeDrivenStopNull = o._timeDrivenStopNull;
	joint._timeDrivenStop = o._timeDrivenStop;
	return joint ;
}


string TemporaryOrder::getSqlCommand(const string &db_key, const string &orderType, const string &algo, unsigned int satId) 
{
	return getSqlCommandOrderBy(db_key, orderType, algo, satId, " ORDER BY t.tstamp " ) ; 
}

string TemporaryOrder::getSqlCommandOrderBy(const string &db_key, const string &orderType, const string &algo, unsigned int satId, const string &orderby) 
{
	initHasTimeDrivenFields(db_key);

	bool hasTimeDriven = hasTimeDrivenFields();
							
	// Prepare the SQL statement
	SQLString statement (db_key, SQLString::SELECT) ;

	// Select list
	statement.addSelectList("t.Id");
	statement.addSelectList("t.Filename");
	statement.addSelectList("t.Fileversion");
	statement.addSelectList("t.Algorithm");
	statement.addSelectList("t.Filetype");
	statement.addSelectList("t.Ordertype");
	statement.addSelectList("t.ProcessorName");
	statement.addSelectList("t.ProcessorVersion");
	statement.addSelectList("t.ProcessorConfiguration");
	statement.addSelectList("i.validityflag");
	statement.addSelectList("i.validitystart");
	statement.addSelectList("i.validitystop");
	statement.addSelectList("i.inventorydate");
	statement.addSelectList("i.equatorxtime");
	statement.addSelectList("t.priority");
	statement.addSelectList("t.troubleShooting");
	statement.addSelectList("t.processingTimeWindow");
	statement.addSelectList("t.ordersat");
	statement.addSelectList("t.tstamp");
	if (hasTimeDriven) {
		statement.addSelectList("t.time_driven_start");
		statement.addSelectList("t.time_driven_stop");
	}
	
	statement.addSelectList("i.mission");
	statement.addSelectList("i.orderid");
	statement.addSelectList("i.qualityinfo");
	statement.addSelectList("i.qualified");

	// Table list
	statement.addTableList (" t_temporaryorders", "t");
	statement.addTableList (" t_inventory",       "i");

	dbQueryParameters par1 (db_key) ;
	par1.setParamOperator( dbQueryParameters::AND ) ;

	par1.addParameter (dbParam(db_key, "t", "OrderType",    dbParam::EQUAL_TO, db::toSQL(db_key, orderType) )) ;
	par1.addParameter (dbParam(db_key, "t", "Algorithm",    dbParam::EQUAL_TO, db::toSQL(db_key, algo) )) ;
	par1.addParameter (dbParam(db_key, "i", "filename",     dbParam::EQUAL_TO, "t.filename" )) ;
	par1.addParameter (dbParam(db_key, "i", "fileversion",  dbParam::EQUAL_TO, "t.fileversion" )) ;

	if (satId == OrderCreator::getNullSatId() ) 
	{
		par1.addParameter (dbParam(db_key, "t", "ordersat",  dbParam::ISNULL)) ;
	}
	else
	{
		par1.addParameter (dbParam(db_key, "t", "ordersat",  dbParam::EQUAL_TO, db::toSQL(db_key, satId) )) ;
	}

	statement.addANDWhereParameter (par1); 

    if( dbSchemaMetaDataProvider::hasField( "t_temporaryorders", "emitTime", dbConnectionWrapper(db::ConnPools::instance()->getPool(db_key)))) // PRQA S 3050, 3081
    {
		dbQueryParameters par2 (db_key) ;
		par2.setParamOperator( dbQueryParameters::OR ) ;

		par2.addParameter (dbParam(db_key, "t", "emitTime",    dbParam::ISNULL )) ;
		par2.addParameter (dbParam(db_key, "t", "emitTime",    dbParam::MINOR_OR_EQUAL_OF, SQLDialectFactory::sqlDialect(db_key).currentDateTime() )) ;
		statement.addANDWhereParameter (par2); 
    }

#warning order_by clause is set in plain SQL
	string ret = statement.getSQL();
	ret += orderby;

	return ret;
}

string TemporaryOrder::getSqlCommandForFType(const string &db_key, const string &orderType, const string &algo, const UniqueEntry &ue) 
{
	initHasTimeDrivenFields(db_key);

	bool hasTimeDriven = hasTimeDrivenFields();

	// Prepare the SQL statement
	SQLString statement (db_key, SQLString::SELECT) ;

	// Select list
	statement.addSelectList("t.Id");
	statement.addSelectList("t.Filename");
	statement.addSelectList("t.Fileversion");
	statement.addSelectList("t.Algorithm");
	statement.addSelectList("t.Filetype");
	statement.addSelectList("t.Ordertype");
	statement.addSelectList("t.ProcessorName");
	statement.addSelectList("t.ProcessorVersion");
	statement.addSelectList("t.ProcessorConfiguration");
	statement.addSelectList("i.validityflag");
	statement.addSelectList("i.validitystart");
	statement.addSelectList("i.validitystop");
	statement.addSelectList("i.inventorydate");
	statement.addSelectList("i.equatorxtime");
	statement.addSelectList("t.priority");
	statement.addSelectList("t.troubleShooting");
	statement.addSelectList("t.processingTimeWindow");
	statement.addSelectList("t.ordersat");
	statement.addSelectList("t.tstamp");
	if (hasTimeDriven) {
		statement.addSelectList("t.time_driven_start");
		statement.addSelectList("t.time_driven_stop");
	}
	
	statement.addSelectList("i.mission");
	statement.addSelectList("i.orderid");
	statement.addSelectList("i.qualityinfo");
	statement.addSelectList("i.qualified");

	// Table list
	statement.addTableList (" t_temporaryorders", "t");
	statement.addTableList (" t_inventory",       "i");

	dbQueryParameters par1 (db_key) ;
	par1.setParamOperator( dbQueryParameters::AND ) ;

	par1.addParameter (dbParam(db_key, "t", "OrderType",    dbParam::EQUAL_TO, db::toSQL(db_key, orderType) )) ;
	par1.addParameter (dbParam(db_key, "t", "Algorithm",    dbParam::EQUAL_TO, db::toSQL(db_key, algo) )) ;

	par1.addParameter (dbParam(db_key, "t", "Filetype",               dbParam::EQUAL_TO, db::toSQL(db_key, ue.filetype) )) ;
	par1.addParameter (dbParam(db_key, "t", "ProcessorName",          dbParam::EQUAL_TO, db::toSQL(db_key, ue.processorName) )) ;
	par1.addParameter (dbParam(db_key, "t", "ProcessorVersion",       dbParam::EQUAL_TO, db::toSQL(db_key, ue.processorVersion) )) ;
	par1.addParameter (dbParam(db_key, "t", "processorConfiguration", dbParam::EQUAL_TO, db::toSQL(db_key, ue.processorConfiguration) )) ;

	par1.addParameter (dbParam(db_key, "i", "filename",     dbParam::EQUAL_TO, "t.filename" )) ;
	par1.addParameter (dbParam(db_key, "i", "fileversion",  dbParam::EQUAL_TO, "t.fileversion" )) ;

	if ( ue.satelliteId == OrderCreator::getNullSatId() ) 
	{
		par1.addParameter (dbParam(db_key, "t", "ordersat",  dbParam::ISNULL)) ;
	}
	else
	{
		par1.addParameter (dbParam(db_key, "t", "ordersat",  dbParam::EQUAL_TO, db::toSQL(db_key, ue.satelliteId) )) ;
	}

	statement.addANDWhereParameter (par1); 

    if( dbSchemaMetaDataProvider::hasField( "t_temporaryorders", "emitTime", dbConnectionWrapper(db::ConnPools::instance()->getPool(db_key)))) // PRQA S 3050, 3081
    {
		dbQueryParameters par2 (db_key) ;
		par2.setParamOperator( dbQueryParameters::OR ) ;

		par2.addParameter (dbParam(db_key, "t", "emitTime",    dbParam::ISNULL )) ;
		par2.addParameter (dbParam(db_key, "t", "emitTime",    dbParam::MINOR_OR_EQUAL_OF, SQLDialectFactory::sqlDialect(db_key).currentDateTime() )) ;
		statement.addANDWhereParameter (par2); 
    }
    
	if (hasTimeDriven) {
		statement.addOrderByList("t.time_driven_start");
	}
	statement.addOrderByList("i.validitystart");

	return statement.getSQL();

}

string TemporaryOrder::getUniqueEntriesSqlCommand(const string &db_key, const string &orderType, const string &algo, unsigned int satId) 
{

	// Prepare the SQL statement
	SQLString statement (db_key, SQLString::SELECT) ;

	// Distinct clause
	statement.setDistinct (true);

	// Select list
	statement.addSelectList("Filetype");
	statement.addSelectList("ProcessorName");
	statement.addSelectList("ProcessorVersion");
	statement.addSelectList("ProcessorConfiguration");
	statement.addSelectList("OrderSat");

	statement.addTableList("t_temporaryorders", "t");

	// Where clause parameters
	dbQueryParameters par1 (db_key) ;
	par1.setParamOperator( dbQueryParameters::AND ) ;

	par1.addParameter (dbParam(db_key, "t", "OrderType",    dbParam::EQUAL_TO, db::toSQL(db_key, orderType) )) ;
	par1.addParameter (dbParam(db_key, "t", "Algorithm",    dbParam::EQUAL_TO, db::toSQL(db_key, algo) )) ;

	if ( satId == OrderCreator::getNullSatId() ) 
	{
		par1.addParameter (dbParam(db_key, "t", "ordersat",  dbParam::ISNULL)) ;
	}
	else
	{
		par1.addParameter (dbParam(db_key, "t", "ordersat",  dbParam::EQUAL_TO, db::toSQL(db_key, satId) )) ;
	}

	// Attach where to query
	statement.addANDWhereParameter (par1); 

	return statement.getSQL();

}


string TemporaryOrder::dump(const string& t) const
{
	ostringstream o ; o \
		<< t << "id: " << "\"" <<					  _id << "\"\n" \
		<< t << "fileName: " << "\"" << 			  _fileName << "\"\n" \
		<< t << "fileVersion: " << "\"" <<  		  _fileVersion << "\"\n" \
		<< t << "algorithm: " << "\"" <<			  _algorithm << "\"\n" \
		<< t << "fileType: " << "\"" << 			  _fileType << "\"\n" \
		<< t << "orderType: " << "\"" <<			  _orderType << "\"\n" \
		<< t << "processorName: " << "\"" <<		  _processorName << "\"\n" \
		<< t << "processorVersion: " << "\"" << 	  _processorVersion << "\"\n" \
		<< t << "processorConfiguration: " << "\"" << _processorConfiguration << "\"\n" \
		<< t << "validityFlag: " << 		 		 (_validityFlag?"T":"F") << "\n" \
		<< t << "start: " <<						  _start.asISO8601(6) << "\n" \
		<< t << "stop: "  << 				 		 _stop.asISO8601(6) << "\n" \
		<< t << "inventoryDate: " <<				  _inventoryDate.asISO8601(6) << "\n" \
		<< t << "equatorxtime: " << 				  _equatorxtime.asISO8601(6)  << "\n" \
		<< t << "priority: " << "\"" << 			  _priority << "\"\n" \
		<< t << "troubleShooting: " <<  	 	 	(_troubleShooting?"T":"F") << "\n" \
		<< t << "processingTimeWindow: " << "\"" <<   _processingTimeWindow << "\"\n" \
		<< t << "ordersat: " << "\"" << 			  _ordersat << "\"\n" \
		<< t << "tstamp: " << 						  _tstamp.asISO8601(6) << "\n" ;
	
	if (hasTimeDrivenFields()) {
		o	<< t << "timeDrivenStart: " <<			_timeDrivenStart.asISO8601(6) << "\n" \
			<< t << "timeDrivenStop: "  <<			_timeDrivenStop.asISO8601(6) << "\n" ;
	}
	
	o	<< t << "_mission: " << "\"" << _mission << "\"\n" \
		<< t << "_orderId: " << "\"" << _orderId << "\"\n" \
		<< t << "_qualityInfo: " << "\"" << _qualityInfo << "\"\n" \
 		<< t << "_qualified: " << "\"" << _qualified << "\"" ;
 
 	return o.str() ; 
}


uint16_t TemporaryOrder::loadRow(const dbRow &row) 
{

	bool hasTimeDriven = hasTimeDrivenFields();

    bool isNull = false ;
    uint16_t idx = 0;
    
    // PRQA S 4400 L3
	row.column( idx++, _id) ;
	row.column( idx++, _fileName) ;
	row.column( idx++, _fileVersion) ;
	row.column( idx++, _algorithm) ;
	row.column( idx++, _fileType) ;
	row.column( idx++, _orderType) ;
	row.column( idx++, _processorName) ;
	row.column( idx++, _processorVersion) ;
	row.column( idx++, _processorConfiguration) ;
	row.column( idx++, _validityFlag) ;
	row.column( idx++, _start) ;
	row.column( idx++, _stop) ;
	row.column( idx++, _inventoryDate) ;
	row.column( idx++, _equatorxtime, &isNull) ; if (true == isNull) { _equatorxtime.reset() ; /* set unfilled */ } 
	row.column( idx++, _priority) ;
	row.column( idx++, _troubleShooting) ;
	row.column( idx++, _processingTimeWindow) ;
	row.column( idx++, _ordersat, &_ordersatNull) ;
	row.column( idx++, _tstamp) ;
	if (hasTimeDriven) {
		row.column( idx++, _timeDrivenStart, &_timeDrivenStartNull) ;
		row.column( idx++, _timeDrivenStop, &_timeDrivenStopNull) ;
	}
	
	row.column( idx++, _mission, &_missionNull) ;
	row.column( idx++, _orderId, &_orderIdNull) ;
	row.column( idx++, _qualityInfo, &isNull) ;
 	row.column( idx++, _qualified, &isNull) ;
   // PRQA L:L3
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inv date: " << _inventoryDate.YYYYMMDDThhmmss() 
		    << " tstamp: " << _tstamp.YYYYMMDDThhmmss()) ;
			
	return idx ;
}

bool TemporaryOrder::hasTimeDrivenFields()
{
	Lock lock(_tdmutex);
	return (_isTimeDrivenInit && _hasTimeDrivenFields);
}

bool TemporaryOrder::isExpired(double maxTime) const // PRQA S 4020, 4214
{	
	DateTime::JD50  jd_inv  = _inventoryDate ; // PRQA S 3050 3
	DateTime now ;
	DateTime::JD50  jd_now = now ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inv date: " << _inventoryDate << " now: " << now // PRQA S 3050, 3081 3
        << " diff: " << double(jd_now) - double(jd_inv) << "[d]" 
        << " threshold: " << maxTime << "[sec] " <<  maxTime/86400.0 << "[d]") ;
	if(double(jd_now) - double(jd_inv) > maxTime) {
		return true ;
    }
	
	return false ;
}

bool TemporaryOrder::isInventoryExpired(double maxTime) const // PRQA S 4214
{	
	return isExpired(maxTime) ;
}

bool TemporaryOrder::isTmpOrderExpired(double maxTime) const // PRQA S 4020, 4214
{	
	DateTime::JD50  jd_ord  = _tstamp ; // PRQA S 3050 3
	DateTime now ;
	DateTime::JD50  jd_now = now ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inv date: " << _tstamp << " now: " << now  // PRQA S 3050, 3081 3
        << " diff: " << double(jd_now) - double(jd_ord) << "[d]" 
        << " threshold: " << maxTime << "[sec] " <<  maxTime/86400.0 << "[d]")  ;
    
	if(double(jd_now) - double(jd_ord) > maxTime) { // PRQA S 3050, 3081
		return true ;
    }
	
	return false ;
}

bool TemporaryOrder::isDup() const 
{
    return StringUtils::equalsNoCase(_qualityInfo,"DUP") ;
}


dbPersistent::IdType TemporaryOrder::id() const throw()  // PRQA S 4120
{
	return _id ;
}


const string &TemporaryOrder::fileName() const throw()   // PRQA S 4120
{
	return _fileName ;
}


const string &TemporaryOrder::fileVersion() const throw()  // PRQA S 4120
{
	return _fileVersion ;
}


const string &TemporaryOrder::algorithm() const throw()  // PRQA S 4120
{
	return _algorithm ;
}


const string &TemporaryOrder::fileType() const throw()  // PRQA S 4120
{
	return _fileType ;
}


const string &TemporaryOrder::orderType() const throw()  // PRQA S 4120
{
	return _orderType ;
}


const string &TemporaryOrder::processorName() const throw()  // PRQA S 4120
{
	return _processorName ;
}


const string &TemporaryOrder::processorVersion() const throw() // PRQA S 4120
{
	return _processorVersion ;
}

const string &TemporaryOrder::processorConfiguration() const throw()  // PRQA S 4120
{
	return _processorConfiguration ;
}

const string &TemporaryOrder::qualityInfo() const throw() // PRQA S 4120
{
	return _qualityInfo ;
}

int TemporaryOrder::qualified() const throw() // PRQA S 4120
{
    return _qualified ;
}

bool TemporaryOrder::validityFlag() const throw() // PRQA S 4120
{
	return _validityFlag ;
}


const DateTime &TemporaryOrder::start() const throw() // PRQA S 4120
{
	return _start ;
}


const DateTime &TemporaryOrder::stop() const throw()  // PRQA S 4120
{
	return _stop ;
}

const DateTime &TemporaryOrder::inventoryDate() const throw()  // PRQA S 4120
{
	return _inventoryDate ;
}

const DateTime &TemporaryOrder::equatorxtime() const throw()  // PRQA S 4120
{
	return _equatorxtime ;
}


int TemporaryOrder::priority() const throw()  // PRQA S 4120
{
	return _priority ;
}

bool TemporaryOrder::troubleShooting() const throw()  // PRQA S 4120
{
	return _troubleShooting ;
}

int TemporaryOrder::processingTimeWindow() const throw() // PRQA S 4120
{
	return _processingTimeWindow ;
}

unsigned int TemporaryOrder::ordersat(bool *isNull) const 
{
	if (isNull) { *isNull = _ordersatNull ; }
	return _ordersatNull ? OrderCreator::getNullSatId() : _ordersat ; // PRQA S 3382, 3385
}

int TemporaryOrder::mission(bool *isNull) const 
{
	if (isNull) { *isNull = _missionNull ; }
	return _missionNull ? 0 : _mission ; // PRQA S 3382
}

int TemporaryOrder::trigOrderId(bool *isNull) const 
{
	if (isNull) { *isNull = _orderIdNull ; }
	return _orderIdNull ? 0 : _orderId ; // PRQA S 3382
}

const DateTime& TemporaryOrder::timeDrivenStart(bool *isNull) const {
	if (isNull) { *isNull = _timeDrivenStartNull ; }
	return _timeDrivenStart ;
}

const DateTime& TemporaryOrder::timeDrivenStop(bool *isNull) const {
	if (isNull) { *isNull = _timeDrivenStopNull ; }
	return _timeDrivenStop ;	
}

unsigned int TemporaryOrder::satelliteId(bool *isNull) const 
{
	return ordersat(isNull) ;
}

const DateTime &TemporaryOrder::timestamp() const throw() // PRQA S 4120
{
	return _tstamp ;
}



TriggeringFile::TriggeringFile(const TemporaryOrder &to) :
    filename(to.fileName()),
    fileversion(to.fileVersion()),
    filetype(to.fileType()),
    orderid(to.trigOrderId()) // PRQA S 3000 
{}


void TemporaryOrder::removeFromDb(dbConnection &conn) const
{
	string queryString ;
    const string &db_key = conn.getKey() ;


	// Prepare the SQL statement
	SQLString statement (db_key, SQLString::DELETE) ;
	statement.addTableList("t_temporaryorders", "t");

	// Where clause parameters
	dbQueryParameters par1 (db_key) ;
	par1.addParameter (dbParam(db_key, "t", "id", dbParam::EQUAL_TO, db::toSQL(db_key, _id) )) ;

	// Attach where to query
	statement.addANDWhereParameter (par1); 

	// Execute the query
	dbQuery deleteQuery(conn, statement, "TemporaryOrderDeletion") ;
	deleteQuery.execForStatus() ;

#ifdef OLD_CODE
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM t_temporaryOrders WHERE id=" ;
		queryString += string(db::toSQL(dbKey, _id)) ; // PRQA S 3050, 3081
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: " +dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery deleteQuery(conn,sqlString,"TemporaryOrderDeletion") ;
	deleteQuery.execForStatus() ;
#endif //OLD_CODE
}

bool TemporaryOrder::findById(const std::vector <TemporaryOrder> &o, dbPersistent::IdType id, size_t &pos)  // PRQA S 4020
{
	for (size_t i=0; i < o.size(); i++) {
		if (o[i].id() == id) {
			pos = i ;
			return true ;
		}
	}
	
	return false ;
}


bool TemporaryOrder::sameProcessor(const TemporaryOrder &o) const 
{
	return processorName() == o.processorName() && \
		   processorVersion() == o.processorVersion() && \
		   processorConfiguration() == o.processorConfiguration() ;
	
}

const vector<int> &TemporaryOrder::sites(const string &dbKey) const  // PRQA S 4020, 4214
{
	if (not _sites.empty()) {
		return _sites ;
    }
		
	SQLString siteSqlQuery(dbKey, SQLString::SELECT ) ;
    siteSqlQuery.addSelectList("site");
    siteSqlQuery.addTableList("t_invsiterelation");
    {
        dbQueryParameters qp(dbKey) ;
	    qp.addParameter(dbParam(dbKey, "t_invsiterelation","filename" , dbParam::EQUAL_TO, db::toSQL(dbKey, fileName()) )) ; // PRQA S 3050 2
	    qp.addParameter(dbParam(dbKey, "t_invsiterelation","fileversion" , dbParam::EQUAL_TO, db::toSQL(dbKey, fileVersion()) )) ;
	    siteSqlQuery.addANDWhereParameter(qp) ;
    }
    dbConnectionWrapper pw(db::ConnPools::instance()->getPool(dbKey)) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	dbQuery siteQuery(theConnection,siteSqlQuery,"SiteRetrievalForTmpOrder") ;
	dbSet siteSet ;
	siteQuery.execForSet(siteSet) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "found " << siteSet.size() << " sites.") ;
	
    size_t ssc = siteSet.size() ;
	for (size_t i=0; i < ssc; ++i) {
		dbRow row(dbKey) ;
		siteSet.getNextRow(row) ;
		int siteId = 0 ;
		row.column(0,siteId) ;
		_sites.push_back(siteId) ;
	}
	
	return _sites ;
	
}



_ACS_END_NAMESPACE
