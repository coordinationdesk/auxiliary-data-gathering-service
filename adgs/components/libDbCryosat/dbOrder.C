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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.6  2016/05/05 13:13:25  marpas
	improving maintenance adopting dbPersistent convenience macros
	coding best practice application in progress
	
	Revision 5.5  2016/05/02 17:27:08  marpas
	adopting new convenience macros from dbPersistent
	code rationalization
	coding best practices applied
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2014/05/26 11:39:45  marpas
	dbConv casted to avoid problem with new libXMLResources templates for tags
	
	Revision 4.0  2013/09/30 12:31:01  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:29  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.22  2013/06/19 18:04:17  marpas
	orderid is a serial so IdType has to be used
	adopting libException 5.x standards
	
	Revision 2.21  2013/05/14 19:04:51  marpas
	SQLDialectFactory::sqlDialect() mthods now requires db connection key
	
	Revision 2.20  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.19  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.18  2013/04/12 12:14:39  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.17  2013/03/29 19:51:30  marpas
	removing html related functions
	
	Revision 2.16  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.15  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.12  2012/02/14 12:52:49  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.11  2011/09/28 13:16:12  danalt
	JobResponsible size is now 64
	
	Revision 2.10  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.9  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.8  2009/10/08 13:11:53  marpas
	statistics improved
	
	Revision 2.7  2009/09/29 15:34:32  marpas
	useless include removed
	
	Revision 2.6  2009/06/04 11:56:16  marpas
	bug fixed in method save: subreq field was not enumerated in value names
	
	Revision 2.5  2009/06/03 16:14:51  marpas
	subrequest field added: for CNM
	
	Revision 2.4  2009/02/13 12:28:05  marpas
	constness added when downloading parameter file
	
	Revision 2.3  2008/09/16 16:01:49  crivig
	porting to postgres8
	
	Revision 2.2  2006/10/24 16:01:40  marfav
	Now can export parameter files using streams
	
	Revision 2.1  2006/05/16 12:54:04  marpas
	new check for save and update method: start time <= stop time
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.49  2006/01/20 10:19:55  enrcar
	EC:: define FORCE_DBEXPRESS_QUERY introduced to allow safer tests on dbexpress-ported queries
	
	Revision 1.48  2006/01/13 13:48:22  enrcar
	updated to dbexprss
	

	Revision 1.47  2005/11/03 09:56:39  marpas
	jobresponsible enlarged to 24 chars
	
	Revision 1.46  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.45  2005/06/06 17:48:04  giucas
	XML export fixed when processorconfiguration is the empty string
	
	Revision 1.44  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.43  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.42  2005/01/12 10:15:10  paoscu
	import-export obj now use OrderQueueHasMission feature to deal with mission.
	In case no feature is enabled, mission is always considered as not set )null=.
	
	Revision 1.41  2004/10/14 15:15:39  paoscu
	mission added, conditioned to feature OrderHasStandByState
	
	Revision 1.40  2004/10/04 15:13:53  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.39  2004/09/08 16:30:06  ivafam
	Bug Fixed in Update method
	
	Revision 1.38  2004/09/03 13:33:03  paoscu
	dbOrder::hasParametersFile removed
	dbOrder::getParameters changed into dbOrder::hasParameters
	bug fixed in updating internal dbOrder::_parameters member
	dbOrder::update can save parameters file too
	
	Revision 1.37  2004/09/02 18:27:27  paoscu
	getParameters method added
	hasParameterFile method I/F changed and reimplemented to call getPameters
	hasParameterFile method is obsolete and it is deprecated
	
	Revision 1.36  2004/08/26 18:25:23  paoscu
	detail will show presence of parameter file
	
	Revision 1.35  2004/08/26 14:40:01  paoscu
	dbOrder::hasParameterFile(...) method added
	
	Revision 1.34  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.33  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.32  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.31  2004/04/16 12:49:34  paoscu
	Bug fixed in Lock usage
	
	Revision 1.30  2004/04/13 16:57:09  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.29  2004/03/23 12:06:25  paoscu
	Modified flag behavior changed for setStatus method.
	
	Revision 1.28  2004/03/09 18:08:31  paoscu
	load fixed
	
	Revision 1.27  2004/03/04 18:55:06  paoscu
	SatelliteId and Parameters fields added.
		

*/

// This file is automatically generated by dbGenerator tool

#include <dbOrder.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbConv.h>
#include <Transaction.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#include <FileGuard.h>
#include <unistd.h>
#include <sstream>
#include <XMLOstream.h>
#include <SQLDialectFactory.h> 


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbOrder)

// constructor
dbOrder::dbOrder() :
	dbPersistent(),
	_orderType() ,
	_orderTypeSet() ,
	_status() ,
	_statusSet() ,
	_processorName() ,
	_processorNameSet() ,
	_satelliteId() ,
	_satelliteIdSet() ,
	_mission() ,
	_missionSet() ,
	_startTime() ,
	_startTimeSet() ,
	_stopTime() ,
	_stopTimeSet() ,
	_jobResponsible() ,
	_jobResponsibleSet() ,
	_productionStartTime() ,
	_productionStartTimeSet() ,
	_productionStopTime() ,
	_productionStopTimeSet() ,
	_externalOrderId() ,
	_externalOrderIdSet() ,
	_creationDate() ,
	_creationDateSet() ,
	_priority() ,
	_prioritySet() ,
	_processorVersion() ,
	_processorVersionSet() ,
	_processorConfiguration() ,
	_processorConfigurationSet() ,
	_parametersUploadFileName() ,
	_parametersUploadFileNameSet() ,
	_troubleshooting() ,
	_troubleshootingSet() ,
	_processingTimeWindow() ,
	_processingTimeWindowSet() ,
	_processorId() ,
	_processorIdSet(),
	_subreq() ,
	_subreqSet(),
	_parameters()
{
}


// copy constructor
dbOrder::dbOrder(const dbOrder &o):
	dbPersistent(o),
	_orderType(o._orderType) ,
	_orderTypeSet(o._orderTypeSet) ,
	_status(o._status) ,
	_statusSet(o._statusSet) ,
	_processorName(o._processorName) ,
	_processorNameSet(o._processorNameSet) ,
	_satelliteId(o._satelliteId) ,
	_satelliteIdSet(o._satelliteIdSet) ,
	_mission(o._mission) ,
	_missionSet(o._missionSet) ,
	_startTime(o._startTime) ,
	_startTimeSet(o._startTimeSet) ,
	_stopTime(o._stopTime) ,
	_stopTimeSet(o._stopTimeSet) ,
	_jobResponsible(o._jobResponsible) ,
	_jobResponsibleSet(o._jobResponsibleSet) ,
	_productionStartTime(o._productionStartTime) ,
	_productionStartTimeSet(o._productionStartTimeSet) ,
	_productionStopTime(o._productionStopTime) ,
	_productionStopTimeSet(o._productionStopTimeSet) ,
	_externalOrderId(o._externalOrderId) ,
	_externalOrderIdSet(o._externalOrderIdSet) ,
	_creationDate(o._creationDate) ,
	_creationDateSet(o._creationDateSet) ,
	_priority(o._priority) ,
	_prioritySet(o._prioritySet) ,
	_processorVersion(o._processorVersion) ,
	_processorVersionSet(o._processorVersionSet) ,
	_processorConfiguration(o._processorConfiguration) ,
	_processorConfigurationSet(o._processorConfigurationSet) ,
	_parametersUploadFileName(o._parametersUploadFileName) ,
	_parametersUploadFileNameSet(o._parametersUploadFileNameSet) ,
	_troubleshooting(o._troubleshooting) ,
	_troubleshootingSet(o._troubleshootingSet) ,
	_processingTimeWindow(o._processingTimeWindow) ,
	_processingTimeWindowSet(o._processingTimeWindowSet) ,
	_processorId(o._processorId) ,
	_processorIdSet(o._processorIdSet),
	_subreq(o._subreq) ,
	_subreqSet(o._subreqSet),
	_parameters(o._parameters)
{
}


dbOrder::~dbOrder() throw()
{
}


dbPersistent *dbOrder::clone() const
{
	return new dbOrder(*this) ;
}



// operator =
dbOrder &dbOrder::operator=(const dbOrder &o)
{
	Lock lock(*this) ; // PRQA S 3050 // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_orderType=o._orderType;
		_orderTypeSet=o._orderTypeSet;
		_status=o._status;
		_statusSet=o._statusSet;
		_processorName=o._processorName;
		_processorNameSet=o._processorNameSet;
		_satelliteId=o._satelliteId;
		_satelliteIdSet=o._satelliteIdSet;
		_mission=o._mission;
		_missionSet=o._missionSet;
		_startTime=o._startTime;
		_startTimeSet=o._startTimeSet;
		_stopTime=o._stopTime;
		_stopTimeSet=o._stopTimeSet;
		_jobResponsible=o._jobResponsible;
		_jobResponsibleSet=o._jobResponsibleSet;
		_productionStartTime=o._productionStartTime;
		_productionStartTimeSet=o._productionStartTimeSet;
		_productionStopTime=o._productionStopTime;
		_productionStopTimeSet=o._productionStopTimeSet;
		_externalOrderId=o._externalOrderId;
		_externalOrderIdSet=o._externalOrderIdSet;
		_creationDate=o._creationDate;
		_creationDateSet=o._creationDateSet;
		_priority=o._priority;
		_prioritySet=o._prioritySet;
		_processorVersion=o._processorVersion;
		_processorVersionSet=o._processorVersionSet;
		_processorConfiguration=o._processorConfiguration;
		_processorConfigurationSet=o._processorConfigurationSet;
		_parametersUploadFileName=o._parametersUploadFileName;
		_parametersUploadFileNameSet=o._parametersUploadFileNameSet;
		_troubleshooting=o._troubleshooting;
		_troubleshootingSet=o._troubleshootingSet;
		_processingTimeWindow=o._processingTimeWindow;
		_processingTimeWindowSet=o._processingTimeWindowSet;
		_processorId=o._processorId;
		_processorIdSet=o._processorIdSet;
		_subreq=o._subreq;
		_subreqSet=o._subreqSet;
		_parameters=o._parameters;
	}
	return *this ;
}


string dbOrder::className() const
{
	return staticClassName() ;
}


string dbOrder::staticClassName()
{
	return "dbOrder" ;
}



// initialize the class
void dbOrder::reset()
{
	Lock lock(*this) ; // PRQA S 3050

	// reset dbPersistent
	dbPersistent::reset() ;
	_orderType="" ;
	_orderTypeSet=false ;
	_status="" ;
	_statusSet=false ;
	_processorName="" ;
	_processorNameSet=false ;
	_satelliteId=0 ;
	_satelliteIdSet=false ;
	_mission=0 ;
	_missionSet=false ;
	_startTime.reset() ;
	_startTimeSet=false ;
	_stopTime.reset() ;
	_stopTimeSet=false ;
	_jobResponsible="" ;
	_jobResponsibleSet=false ;
	_productionStartTime.reset() ;
	_productionStartTimeSet=false ;
	_productionStopTime.reset() ;
	_productionStopTimeSet=false ;
	_externalOrderId="" ;
	_externalOrderIdSet=false ;
	_creationDate.reset() ;
	_creationDateSet=false ;
	_priority=0 ;
	_prioritySet=false ;
	_processorVersion="" ;
	_processorVersionSet=false ;
	_processorConfiguration="" ;
	_processorConfigurationSet=false ;
	_parametersUploadFileName="" ;
	_parametersUploadFileNameSet=false ;
	_troubleshooting=false ;
	_troubleshootingSet=false ;
	_processingTimeWindow=0 ;
	_processingTimeWindowSet=false ;
	_processorId="" ;
	_processorIdSet=false ;
	_subreq=0 ;
	_subreqSet=false ;
	_parameters=false ;
}


// save the data in database
dbPersistent::IdType dbOrder::save(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	if (!isModified()) { return getId() ; }

	if ((getId() != 0) || isFromDatabase()) 
	{
		ACS_THROW(InvalidUse("Use the method update() instead of save().")) ; // PRQA S 3081
	}
	if(!_orderTypeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Order Type\" must be set.")) ; // PRQA S 3081
	}
	if(!_statusSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Status\" must be set.")) ; // PRQA S 3081
	}
	if(!_processorNameSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Processor Name\" must be set.")) ; // PRQA S 3081
	}
	if(!_startTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Start Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_stopTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Stop Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_creationDateSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Creation Date\" must be set.")) ; // PRQA S 3081
	}
	if(!_prioritySet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Priority\" must be set.")) ; // PRQA S 3081
	}
	if(!_processorVersionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Processor Version\" must be set.")) ; // PRQA S 3081
	}
	if(!_troubleshootingSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Troubleshooting\" must be set.")) ; // PRQA S 3081
	}
	if(!_processingTimeWindowSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Processing Time Window (Days)\" must be set.")) ; // PRQA S 3081
	}
	
	if(_startTime > _stopTime) {
		ACS_THROW(InvalidUse("Start-Time greater than Stop-Time: incongruent.")) ; // PRQA S 3081
	}

	// inside a transaction
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbOrder_SAVE") ;
   	conn.addStateWrapper(*this) ;

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO " + getTable() + " (" ;
		queryString += "OrderType" ;
		queryString += ", Status" ;
		queryString += ", ProcessorName" ;
		if(_satelliteIdSet) {
			queryString += ", SatelliteId" ;
        }
		if(_missionSet) {
			queryString += ", Mission" ;
        }
		queryString += ", StartTime" ;
		queryString += ", StopTime" ;
		if(_jobResponsibleSet) {
			queryString += ", JobResponsible" ;
        }
		if(_productionStartTimeSet) {
			queryString += ", ProductionStartTime" ;
        }
		if(_productionStopTimeSet) {
			queryString += ", ProductionStopTime" ;
        }
		if(_externalOrderIdSet) {
			queryString += ", ExternalOrderId" ;
        }
		queryString += ", CreationDate" ;
		queryString += ", Priority" ;
		queryString += ", ProcessorVersion" ;
		if(_processorConfigurationSet) {
			queryString += ", ProcessorConfiguration" ;
        }
		queryString += ", Troubleshooting" ;
		queryString += ", ProcessingTimeWindow" ;
		if(_processorIdSet) {
			queryString += ", ProcessorId" ;
        }
		if(_subreqSet) {
			queryString += ", Subreq" ;
        }
		queryString +=	") VALUES ( " ;
		queryString +=	db::toSQL(dbKey,_orderType) ;
		queryString +=	", " + db::toSQL(dbKey,_status) ;
		queryString +=	", " + db::toSQL(dbKey,_processorName) ;
		if(_satelliteIdSet) {
			queryString +=	", " + db::toSQL(dbKey,_satelliteId) ;
		}
		if(_missionSet) {
			queryString +=	", " + db::toSQL(dbKey,_mission) ;
		}
		queryString +=	", " + db::toSQL(dbKey,_startTime,dbConv::YearToFraction) ;
		queryString +=	", " + db::toSQL(dbKey,_stopTime,dbConv::YearToFraction) ;
		if(_jobResponsibleSet) {
			queryString +=	", " + db::toSQL(dbKey,_jobResponsible) ;
		}
		if(_productionStartTimeSet) {
			queryString +=	", " + db::toSQL(dbKey,_productionStartTime,dbConv::YearToFraction) ;
		}
		if(_productionStopTimeSet) {
			queryString +=	", " + db::toSQL(dbKey,_productionStopTime,dbConv::YearToFraction) ;
		}
		if(_externalOrderIdSet) {
			queryString +=	", " + db::toSQL(dbKey,_externalOrderId) ;
		}
		queryString +=	", " + db::toSQL(dbKey,_creationDate,dbConv::YearToFraction) ;
		queryString +=	", " + db::toSQL(dbKey,_priority) ;
		queryString +=	", " + db::toSQL(dbKey,_processorVersion) ;
		if(_processorConfigurationSet) {
			queryString +=	", " + db::toSQL(dbKey,_processorConfiguration) ;
		}
		queryString +=	", " + db::toSQL(dbKey,_troubleshooting) ;
		queryString +=	", " + db::toSQL(dbKey,_processingTimeWindow) ;
		if(_processorIdSet) {
			queryString +=	", " + db::toSQL(dbKey,_processorId) ;
		}
		if(_subreqSet) {
			queryString +=	", " + db::toSQL(dbKey,_subreq) ;
		}
        queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"SQL command : " << queryString ) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, getTable()+"_Insert") ;

	if (!query.execForStatus())
	{
			ACS_THROW(dbQuery::QueryExecuteException("Query Execution error: Command : " // PRQA S 3081
			            + queryString
			            + " Error : "
			            + query.errorString())) ;
	}

	// obtain the last serial just inserted
	setId( getLastSerial(conn, getTable()) ) ;

    if(_parametersUploadFileNameSet)
    {
        SQLString qString(dbKey, SQLString::LOBJ_UPLOAD ) ;
        qString.setColumnAndFileName( "Parameters", _parametersUploadFileName );
        qString.addTableList( getTable() );
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter(dbParam(dbKey, getTable(), "OrderId", dbParam::EQUAL_TO, db::toSQL(dbKey,getId())) ) ;
            qString.addANDWhereParameter(p) ;
        }

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Uploading file: " << _parametersUploadFileName );

	    dbQuery uplquery(conn, qString, getTable()+".Parameters"+"_Insert") ;	

        uplquery.uploadClob();
    }  
    transaction.commit() ;
	// record that the values come from the database
	setFromDatabase() ; 
	_parameters=_parametersUploadFileNameSet ;
	
	return getId() ;
}


// load the class from the database. Return true if object exist
bool dbOrder::load(dbConnection &conn , IdType id) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	IdType Id=id;

	if(Id == 0) {
		Id=getId() ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"called with id=" << id << " getId():" << getId() << " Id:" << Id );
	if(Id==0) 
	{
		ACS_THROW(IdNotProvided("Cannot obtain \"OrderId\"")) ; // PRQA S 3081
	}

	dbOrder tempRecord(*this); //store current record.


	reset() ; 


	string queryString ;
    const string &dbKey = conn.getKey(); 
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "SELECT " ;
		queryString +=	getFields() ;
		queryString += " FROM " + getTable() ;
		queryString += " WHERE OrderId = " ;
		queryString +=	db::toSQL(dbKey,Id) ;
		string getOrderByString(getOrderBy()) ;
		if(!getOrderByString.empty() )
		{
			string orderby= "ORDER BY";
			if(StringUtils::findNoCase(getOrderByString,orderby) != string::npos) {
				queryString += getOrderByString ;
            }
			else {
				queryString += " ORDER BY " + getOrderByString ;
            }
		}
	} else {
		ACS_THROW(exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}

	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, getTable()+"_Load") ;
	dbRow row(conn.getKey()) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;

	try
	{
        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
	}
	catch (dbQuery::QueryNoDataFound &e)
	{
		*this = tempRecord;  //Nothing found: restore previuos situation.
		return false ;
	}

	loadRow(row, 0) ;

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	return true;
}


// update an existing instance in database
dbPersistent::IdType dbOrder::update(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	if ((0 == getId()) || !isFromDatabase())
	{
		ACS_THROW(ObjectNotLoaded("Use the method save() instead of update().")) ; // PRQA S 3081
	}

	if (!isModified()) { return 0 ; }

	if(!_orderTypeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Order Type\" must be set.")) ; // PRQA S 3081
	}
	if(!_statusSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Status\" must be set.")) ; // PRQA S 3081
	}
	if(!_processorNameSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Processor Name\" must be set.")) ; // PRQA S 3081
	}
	if(!_startTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Start Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_stopTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Stop Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_creationDateSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Creation Date\" must be set.")) ; // PRQA S 3081
	}
	if(!_prioritySet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Priority\" must be set.")) ; // PRQA S 3081
	}
	if(!_processorVersionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Processor Version\" must be set.")) ; // PRQA S 3081
	}
	if(!_troubleshootingSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Troubleshooting\" must be set.")) ; // PRQA S 3081
	}
	if(!_processingTimeWindowSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Processing Time Window (Days)\" must be set.")) ; // PRQA S 3081
	}

	if(_startTime > _stopTime) {
		ACS_THROW(InvalidUse("Start-Time greater than Stop-Time: incongruent.")) ; // PRQA S 3081
	}

    const string &dbKey = conn.getKey(); 
	string  queryString;
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "UPDATE " + getTable() + " SET " ;
		queryString += "OrderType= " + db::toSQL(dbKey,_orderType) ;
		queryString += ", Status= " + db::toSQL(dbKey,_status) ;
		queryString += ", ProcessorName= " + db::toSQL(dbKey,_processorName) ;
		if(_satelliteIdSet) {
			queryString += ", SatelliteId= " + db::toSQL(dbKey,_satelliteId) ;
		}
        else {
			queryString += ", SatelliteId=  NULL " ; 
        }
		if(_missionSet) {
			queryString += ", Mission= " + db::toSQL(dbKey,_mission) ;
		}
        else {
			queryString += ", Mission=  NULL " ; 
        }
		queryString += ", StartTime= " + db::toSQL(dbKey,_startTime,dbConv::YearToFraction) ;
		queryString += ", StopTime= " + db::toSQL(dbKey,_stopTime,dbConv::YearToFraction) ;
		if(_jobResponsibleSet) {
			queryString += ", JobResponsible= " + db::toSQL(dbKey,_jobResponsible) ;
		}
        else {
			queryString += ", JobResponsible=  NULL " ; 
        }
		if(_productionStartTimeSet) {
			queryString += ", ProductionStartTime= " + db::toSQL(dbKey,_productionStartTime,dbConv::YearToFraction) ;
		}
        else {
			queryString += ", ProductionStartTime=  NULL " ; 
        }
		if(_productionStopTimeSet) {
			queryString += ", ProductionStopTime= " + db::toSQL(dbKey,_productionStopTime,dbConv::YearToFraction) ;
		}
        else {
			queryString += ", ProductionStopTime=  NULL " ; 
        }
		if(_externalOrderIdSet) {
			queryString += ", ExternalOrderId= " + db::toSQL(dbKey,_externalOrderId) ;
		}
        else {
			queryString += ", ExternalOrderId=  NULL " ; 
        }
		queryString += ", CreationDate= " + db::toSQL(dbKey,_creationDate,dbConv::YearToFraction) ;
		queryString += ", Priority= " + db::toSQL(dbKey,_priority) ;
		queryString += ", ProcessorVersion= " + db::toSQL(dbKey,_processorVersion) ;
		if(_processorConfigurationSet) {
			queryString += ", ProcessorConfiguration= " + db::toSQL(dbKey,_processorConfiguration) ;
		}
        else {
			queryString += ", ProcessorConfiguration=  NULL " ; 
        }
		queryString += ", Troubleshooting= " + db::toSQL(dbKey,_troubleshooting) ;
		queryString += ", ProcessingTimeWindow= " + db::toSQL(dbKey,_processingTimeWindow) ;
		if(_processorIdSet) {
			queryString += ", ProcessorId= " + db::toSQL(dbKey,_processorId) ;
		}
        else {
			queryString += ", ProcessorId=  NULL " ; 
		}
        if(_subreqSet) {
			queryString += ", Subreq= " + db::toSQL(dbKey,_subreq) ;
		}
        else {
			queryString += ", Subreq=  NULL " ; 
        }
		queryString += " WHERE " ;
		queryString += "OrderId = " + db::toSQL(dbKey,getId()) ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"SQL = " << queryString) ;
	}
    else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	// inside a transaction
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbOrder_UPDATE") ;
  
	conn.addStateWrapper(*this) ;
	dbQuery query(conn, sqlString, getTable()+"_Update") ;

	query.execForStatus() ;

    if(_parametersUploadFileNameSet)
    {
        SQLString qString(dbKey, SQLString::LOBJ_UPLOAD ) ;
        qString.setColumnAndFileName( "Parameters", _parametersUploadFileName );
        qString.addTableList( getTable() );
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(), "OrderId", dbParam::EQUAL_TO, db::toSQL(dbKey,getId())) ) ;
            qString.addANDWhereParameter(p) ;
        }

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << _parametersUploadFileName) ;

	    dbQuery uplquery(conn, qString, getTable()+".Parameters"+"_Update") ;	

        uplquery.uploadClob();
    }
    transaction.commit() ;

	resetModifiedFlag() ;
	_parameters=_parameters ? _parameters : _parametersUploadFileNameSet; // PRQA S 3380
	return getId() ;

}


// remove the class from the database
dbPersistent::IdType dbOrder::remove(dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050

	if (0 == getId())
	{
		ACS_THROW(IdNotProvided("Id not set")) ; // PRQA S 3081
	}

    const string &dbKey = conn.getKey(); 
	string queryString ;
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {

		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE " ;
		queryString += "OrderId = " + db::toSQL(dbKey,getId()) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	// inside a transaction
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbOrder_REMOVE") ;
  
	conn.addStateWrapper(*this) ;
	dbQuery query(conn, sqlString,getTable()+"_Delete") ;

	query.execForStatus() ;
	transaction.commit() ;


	resetFromDatabase() ;
	setModifiedFlag() ;

	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbOrder::validate() const
{
	Lock lock(*this) ; // PRQA S 3050

	return true;
}


// export the class to stream
void dbOrder::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbOrder") ;
	dbPersistent::exportObj(s) ;
	s.setValue("_orderId", getId()) ;
    RSSetM(s, _orderTypeSet, _orderType) ;
    RSSetM(s, _statusSet, _status) ;
    RSSetM(s, _processorNameSet, _processorName) ;
    RSSetM(s, _satelliteIdSet, _satelliteId) ;
    RSSetM(s, _missionSet, _mission) ;
    RSSetM(s, _startTimeSet, _startTime) ;
    RSSetM(s, _stopTimeSet, _stopTime) ;
    RSSetM(s, _jobResponsibleSet, _jobResponsible) ;
    RSSetM(s, _productionStartTimeSet, _productionStartTime) ;
    RSSetM(s, _productionStopTimeSet, _productionStopTime) ;
    RSSetM(s, _externalOrderIdSet, _externalOrderId) ;
    RSSetM(s, _creationDateSet, _creationDate) ;
    RSSetM(s, _prioritySet, _priority) ;
    RSSetM(s, _processorVersionSet, _processorVersion) ;
    RSSetM(s, _processorConfigurationSet, _processorConfiguration) ;
    RSSetM(s, _troubleshootingSet, _troubleshooting) ;
    RSSetM(s, _processingTimeWindowSet, _processingTimeWindow) ;
    RSSetM(s, _processorIdSet, _processorId) ;
    RSSetM(s, _subreqSet, _subreq) ;

	s.setValue("_parameters",_parameters) ;
}


// import the class from a stream
void dbOrder::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbOrder") ;
	dbPersistent::importObj(s) ;
	reset() ;
	IdType id = 0 ;
	s.getValue("_orderId", id) ;
	setId(id) ;
    RSGetM(s, _orderTypeSet, _orderType) ;
    RSGetM(s, _statusSet, _status) ;
    RSGetM(s, _processorNameSet, _processorName) ;
    RSGetM(s, _satelliteIdSet, _satelliteId) ;
    RSGetM(s, _missionSet, _mission) ;
    RSGetM(s, _startTimeSet, _startTime) ;
    RSGetM(s, _stopTimeSet, _stopTime) ;
    RSGetM(s, _jobResponsibleSet, _jobResponsible) ;
    RSGetM(s, _productionStartTimeSet, _productionStartTime) ;
    RSGetM(s, _productionStopTimeSet, _productionStopTime) ;
    RSGetM(s, _externalOrderIdSet, _externalOrderId) ;
    RSGetM(s, _creationDateSet, _creationDate) ;
    RSGetM(s, _prioritySet, _priority) ;
    RSGetM(s, _processorVersionSet, _processorVersion) ;
    RSGetM(s, _processorConfigurationSet, _processorConfiguration) ;
    RSGetM(s, _troubleshootingSet, _troubleshooting) ;
    RSGetM(s, _processingTimeWindowSet, _processingTimeWindow) ;
    RSGetM(s, _processorIdSet, _processorId) ;
    RSGetM(s, _subreqSet, _subreq) ;
	s.getValue("_parameters",_parameters) ;
}


// export the class to XML stream
void dbOrder::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ; // PRQA S 3050
    const string dbKey = "" ;
	XMLOstream::Tag txml(xst, "dbOrder");
	dbPersistent::exportXML(xst) ;
	{
		ostringstream os;
		os << getId();
		XMLOstream::Tag txml0(xst, "_orderId", os.str(), "label=\"Order Id\"");
	}

    XMLTagM(xst, _orderTypeSet, _orderType, "Order Type") ;
    XMLTagM(xst, _statusSet, _status, "Status") ;
    XMLTagM(xst, _processorNameSet, _processorName, "Processor Name") ;
    XMLTagM(xst, _satelliteIdSet, _satelliteId, "Satellite Id") ;
    XMLTagM(xst, _missionSet, _mission, "Mission") ;
    XMLTagM(xst, _startTimeSet, _startTime, "Start Time") ;
    XMLTagM(xst, _stopTimeSet, _stopTime, "Stop Time") ;
    XMLTagM(xst, _jobResponsibleSet, _jobResponsible, "Job Responsible") ;
    XMLTagM(xst, _productionStartTimeSet, _productionStartTime, "Production Start Time") ;
    XMLTagM(xst, _productionStopTimeSet, _productionStopTime, "Production Stop Time") ;
    XMLTagM(xst, _externalOrderIdSet, _externalOrderId, "External Order Id") ;
    XMLTagM(xst, _creationDateSet, _creationDate, "Creation Date") ;
    XMLTagM(xst, _prioritySet, _priority, "Priority") ;
    XMLTagM(xst, _processorVersionSet, _processorVersion, "Processor Version") ;
    XMLTagM(xst, _processorConfigurationSet, _processorConfiguration, "Processor Configuration") ;
    XMLTagM(xst, _troubleshootingSet, _troubleshooting, "Troubleshooting") ;
    XMLTagM(xst, _processingTimeWindowSet, _processingTimeWindow, "Processing Time Window (Days)") ;
    XMLTagM(xst, _processorIdSet, _processorId, "Processor Id") ;
    XMLTagM(xst, _subreqSet, _subreq, "Subreq") ;

	{
		XMLOstream::Tag txml18Set(xst, "_parameters", (_parameters ? "true" : "false")) ; // PRQA S 3380
	}
}



// load the object from a row
int dbOrder::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ; // PRQA S 3050

	IdType id = 0 ;

	row.column(offset + 0, id) ; // PRQA S 3000, 3010, 4400 L1

	setId(id) ;

	row.columnSet(offset + 1, _orderType, _orderTypeSet) ;
	row.columnSet(offset + 2, _status, _statusSet) ;
	row.columnSet(offset + 3, _processorName, _processorNameSet) ;
	row.columnSet(offset + 4, _satelliteId, _satelliteIdSet) ;
	row.columnSet(offset + 5, _mission, _missionSet) ;
	row.columnSet(offset + 6, _startTime, _startTimeSet) ;
	row.columnSet(offset + 7, _stopTime, _stopTimeSet) ;
	row.columnSet(offset + 8, _jobResponsible, _jobResponsibleSet) ;
	row.columnSet(offset + 9, _productionStartTime, _productionStartTimeSet) ;
	row.columnSet(offset + 10, _productionStopTime, _productionStopTimeSet) ;
	row.columnSet(offset + 11, _externalOrderId, _externalOrderIdSet) ;
	row.columnSet(offset + 12, _creationDate, _creationDateSet) ;
	row.columnSet(offset + 13, _priority, _prioritySet) ;
	row.columnSet(offset + 14, _processorVersion, _processorVersionSet) ;
	row.columnSet(offset + 15, _processorConfiguration, _processorConfigurationSet) ;
	row.columnSet(offset + 16, _troubleshooting, _troubleshootingSet) ;
	row.columnSet(offset + 17, _processingTimeWindow, _processingTimeWindowSet) ;
	row.columnSet(offset + 18, _processorId, _processorIdSet) ;
	row.columnSet(offset + 19, _subreq, _subreqSet) ;
	bool dummySet = false ;
	row.columnSet(offset + 20, _parameters, dummySet) ;

    // PRQA L:L1
	setFromDatabase() ;

	return offset + 21 ; // PRQA S 4400
}



void dbOrder::setOrderId(dbPersistent::IdType v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (getId() != v ) 
	{
		setId(v) ;
		setModifiedFlag() ;
	}
}


dbPersistent::IdType dbOrder::getOrderId() const
{
	Lock lock(*this) ; // PRQA S 3050

	return getId() ;
}


void dbOrder::setOrderType(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 8) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_orderTypeSet || (_orderType != v ) )
	{
		_orderType = v ;
		_orderTypeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getOrderType(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _orderTypeSet ; }
	return _orderType ;
}


void dbOrder::unsetOrderType()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_orderTypeSet == true) { setModifiedFlag(); }
	_orderTypeSet = false;

}
void dbOrder::setStatus(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 32) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	_status = v ;
	_statusSet = true ;
	setModifiedFlag() ;
}


const string &dbOrder::getStatus(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _statusSet ; }
	return _status ;
}


void dbOrder::unsetStatus()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_statusSet == true) { setModifiedFlag(); }
	_statusSet = false;

}
void dbOrder::setProcessorName(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 32) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_processorNameSet || (_processorName != v )  )
	{
		_processorName = v ;
		_processorNameSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getProcessorName(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _processorNameSet ; }
	return _processorName ;
}


void dbOrder::unsetProcessorName()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_processorNameSet == true) { setModifiedFlag(); }
	_processorNameSet = false;

}
void dbOrder::setSatelliteId(unsigned int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_satelliteIdSet || (_satelliteId != v ) )
	{
		_satelliteId = v ;
		_satelliteIdSet = true ;
		setModifiedFlag() ;
	}
}


unsigned int dbOrder::getSatelliteId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _satelliteIdSet ; }
	return _satelliteId ;
}


void dbOrder::unsetSatelliteId()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_satelliteIdSet == true) { setModifiedFlag(); }
	_satelliteIdSet = false;

}

void dbOrder::setMission(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_missionSet || (_mission != v ) )
	{
		_mission = v ;
		_missionSet = true ;
		setModifiedFlag() ;
	}
}


int dbOrder::getMission(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _missionSet ; }
	return _mission ;
}


void dbOrder::unsetMission()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_missionSet == true) { setModifiedFlag(); }
	_missionSet = false;

}

void dbOrder::setStartTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_startTimeSet || (_startTime != v ) )
	{
		_startTime = v ;
		_startTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbOrder::getStartTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _startTimeSet ; }
	return _startTime ;
}


void dbOrder::unsetStartTime()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_startTimeSet == true) { setModifiedFlag(); }
	_startTimeSet = false;

}
void dbOrder::setStopTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_stopTimeSet || (_stopTime != v ) )
	{
		_stopTime = v ;
		_stopTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbOrder::getStopTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _stopTimeSet ; }
	return _stopTime ;
}


void dbOrder::unsetStopTime()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_stopTimeSet == true) { setModifiedFlag(); }
	_stopTimeSet = false;

}
void dbOrder::setJobResponsible(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 64) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_jobResponsibleSet || (_jobResponsible != v ) )
	{
		_jobResponsible = v ;
		_jobResponsibleSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getJobResponsible(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _jobResponsibleSet ; }
	return _jobResponsible ;
}


void dbOrder::unsetJobResponsible()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_jobResponsibleSet == true) { setModifiedFlag(); }
	_jobResponsibleSet = false;

}
void dbOrder::setProductionStartTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_productionStartTimeSet || (_productionStartTime != v ) )
	{
		_productionStartTime = v ;
		_productionStartTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbOrder::getProductionStartTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _productionStartTimeSet ; }
	return _productionStartTime ;
}


void dbOrder::unsetProductionStartTime()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_productionStartTimeSet == true) { setModifiedFlag(); }
	_productionStartTimeSet = false;

}
void dbOrder::setProductionStopTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_productionStopTimeSet || (_productionStopTime != v ) )
	{
		_productionStopTime = v ;
		_productionStopTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbOrder::getProductionStopTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _productionStopTimeSet ; }
	return _productionStopTime ;
}


void dbOrder::unsetProductionStopTime()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_productionStopTimeSet == true) { setModifiedFlag(); }
	_productionStopTimeSet = false;

}
void dbOrder::setExternalOrderId(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 32) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_externalOrderIdSet || (_externalOrderId != v ) )
	{
		_externalOrderId = v ;
		_externalOrderIdSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getExternalOrderId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _externalOrderIdSet ; }
	return _externalOrderId ;
}


void dbOrder::unsetExternalOrderId()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_externalOrderIdSet == true) { setModifiedFlag(); }
	_externalOrderIdSet = false;

}
void dbOrder::setCreationDate(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_creationDateSet || (_creationDate != v ) )
	{
		_creationDate = v ;
		_creationDateSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbOrder::getCreationDate(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _creationDateSet ; }
	return _creationDate ;
}


void dbOrder::unsetCreationDate()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_creationDateSet == true) { setModifiedFlag(); }
	_creationDateSet = false;

}
void dbOrder::setPriority(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_prioritySet || (_priority != v ) )
	{
		_priority = v ;
		_prioritySet = true ;
		setModifiedFlag() ;
	}
}


int dbOrder::getPriority(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _prioritySet ; }
	return _priority ;
}


void dbOrder::unsetPriority()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_prioritySet == true) { setModifiedFlag(); }
	_prioritySet = false;

}
void dbOrder::setProcessorVersion(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 8) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_processorVersionSet || (_processorVersion != v ) )
	{
		_processorVersion = v ;
		_processorVersionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getProcessorVersion(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _processorVersionSet ; }
	return _processorVersion ;
}


void dbOrder::unsetProcessorVersion()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_processorVersionSet == true) { setModifiedFlag(); }
	_processorVersionSet = false;

}
void dbOrder::setProcessorConfiguration(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 8) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_processorConfigurationSet || (_processorConfiguration != v ) )
	{
		_processorConfiguration = v ;
		_processorConfigurationSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getProcessorConfiguration(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _processorConfigurationSet ; }
	return _processorConfiguration ;
}


void dbOrder::unsetProcessorConfiguration()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_processorConfigurationSet == true) { setModifiedFlag(); }
	_processorConfigurationSet = false;

}

void dbOrder::replaceParametersFile(const string &path, dbConnection &conn) 
{
	Lock lock(*this) ; // PRQA S 3050

	if (_orderType.empty())
	{
		ACS_THROW(ObjectNotLoaded("_orderType is an empty string.")) ; // PRQA S 3081
	}
	if (_status.empty())
	{
		ACS_THROW(ObjectNotLoaded("_status is an empty string.")) ; // PRQA S 3081
	}
	if (_processorName.empty())
	{
		ACS_THROW(ObjectNotLoaded("_processorName is an empty string.")) ; // PRQA S 3081
	}
	if (_processorVersion.empty())
	{
		ACS_THROW(ObjectNotLoaded("_processorVersion is an empty string.")) ; // PRQA S 3081
	}
	string uploadfile = path ;

	if (uploadfile.empty() ) 
	{
		ACS_THROW(ObjectNotLoaded("Path is empty.")) ; // PRQA S 3081
	}
    
    const string &dbKey= conn.getKey() ;
    SQLString queryString(dbKey, SQLString::LOBJ_UPLOAD ) ;
    queryString.setColumnAndFileName( "Parameters", uploadfile );
    queryString.addTableList( getTable() );
    {
        dbQueryParameters p(dbKey) ;
        p.addParameter( dbParam(dbKey, getTable(), "OrderId" , dbParam::EQUAL_TO, db::toSQL(dbKey,getId())) ) ;
        queryString.addANDWhereParameter(p) ;
    }

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << uploadfile) ;
    
	dbQuery query(conn, queryString, "dbOrder::replaceParametersFile") ;	

    query.uploadClob();

	_parameters=true ;
}



// Save the file on disk
void dbOrder::saveParametersFile(const string &path, const string &filename, dbConnection &conn) const
{
	Lock lock(*this) ; // PRQA S 3050

	string completePath ;
	if (path != "" && filename != "") {
		completePath = path + "/" + filename ;
    }
	else if (filename != "") {
		completePath = filename ;
    }
	else if (path != "") {
		completePath = path ;
    }
	else {
		completePath = getParametersFileName() ;
    }

	if(isFromDatabase()) {
 
        const string &dbKey= conn.getKey() ;
        SQLString queryString(dbKey, SQLString::LOBJ_DOWNLOAD ) ;
        queryString.setColumnAndFileName( "Parameters", completePath );
        queryString.addTableList( getTable() );
        
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam( dbKey, getTable(), "OrderId" , dbParam::EQUAL_TO, db::toSQL(dbKey,getId())) ) ;
            queryString.addANDWhereParameter(p) ;
        }
            
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading file: " << completePath) ;

		dbQuery query(conn, queryString, "dbOrder::saveParametersFile") ;	

       
        try
        {
            query.downloadClob();
        }
        catch(exception &x)
        {
            ACS_THROW(exIOException(x, "Cannot download Clob to " + completePath)) ; // PRQA S 3081
        }
                
		// change permissions allowing other users to override this file later.
		File::chmod(completePath,0664) ; // PRQA S 4400

	}
	else
	{
		ACS_THROW(ObjectNotLoaded("Error: object not loaded")) ; // PRQA S 3081
	}
}


void dbOrder::saveParametersFile(ostream &theStream, dbConnection& conn) const
{
	Lock lock(*this) ; // PRQA S 3050

	string tempFileName = File::tempname("/tmp/dbOrderParamFileTemp"); // PRQA S 3050

	// The temporary file will be deleted by the guard when exiting this method
	FileGuard theGuard (tempFileName);

	this->saveParametersFile ("/tmp", File::getFileName(tempFileName), conn);

	// Now the parameter file is downlaoded in tmp repository
	// transfer it into the stream

	ifstream theFile (tempFileName.c_str());
	string tmpString;

	// Assuming here that the parameter file is not an HUGE file!!
	off_t theFileSize = File::size(tempFileName);

	char buffer[theFileSize];
	theFile.read(buffer, theFileSize);
	theStream.write(buffer, theFileSize);

	theFile.close();

	// work complete !!
}



string dbOrder::getParametersFileName() const 
{
	ACS_THROW(InvalidUse("A valid path must be provided.")) ; // PRQA S 3081
    return "not-reachable" ;
}

void dbOrder::setParametersUploadFileName(const string &uf)
{
	Lock lock(*this) ; // PRQA S 3050

	_parametersUploadFileName = uf ;
	_parametersUploadFileNameSet = true ;
}

string dbOrder::getParametersUploadFileName(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _parametersUploadFileNameSet ; }
	return _parametersUploadFileName ;
}


void dbOrder::setTroubleshooting(bool v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_troubleshootingSet || (_troubleshooting != v ) )
	{
		_troubleshooting = v ;
		_troubleshootingSet = true ;
		setModifiedFlag() ;
	}
}


bool dbOrder::getTroubleshooting(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _troubleshootingSet ; }
	return _troubleshooting ;
}


void dbOrder::unsetTroubleshooting()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_troubleshootingSet == true) { setModifiedFlag(); }
	_troubleshootingSet = false;

}
void dbOrder::setProcessingTimeWindow(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_processingTimeWindowSet || (_processingTimeWindow != v ) )
	{
		_processingTimeWindow = v ;
		_processingTimeWindowSet = true ;
		setModifiedFlag() ;
	}
}


int dbOrder::getProcessingTimeWindow(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _processingTimeWindowSet ; }
	return _processingTimeWindow ;
}


void dbOrder::unsetProcessingTimeWindow()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_processingTimeWindowSet == true) { setModifiedFlag(); }
	_processingTimeWindowSet = false;

}
void dbOrder::setProcessorId(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (v.size() > 16) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("String too long.")) ; // PRQA S 3081
	}

	if (!_processorIdSet || (_processorId != v ) )
	{
		_processorId = v ;
		_processorIdSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbOrder::getProcessorId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _processorIdSet ; }
	return _processorId ;
}


void dbOrder::unsetProcessorId()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_processorIdSet == true) { setModifiedFlag(); }
	_processorIdSet = false;

}


void dbOrder::setSubReq(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_subreqSet || (_subreq != v ) )
	{
		_subreq = v ;
		_subreqSet = true ;
		setModifiedFlag() ;
	}
}


int dbOrder::getSubReq(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _subreqSet ; }
	return _subreq ;
}


void dbOrder::unsetSubReq()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_subreqSet == true) { setModifiedFlag(); }
	_subreqSet = false;

}


bool dbOrder::hasParameters() const 
{
	Lock lock(*this) ; // PRQA S 3050

	return _parameters ;
}


// get the field names
string dbOrder::getFields()
{
	string s ;
	s += getTable() + "." + "OrderId, ";
	s += getTable() + "." + "OrderType, ";
	s += getTable() + "." + "Status, ";
	s += getTable() + "." + "ProcessorName, ";
	s += getTable() + "." + "SatelliteId, ";
	s += getTable() + "." + "Mission, ";
	s += getTable() + "." + "StartTime, ";
	s += getTable() + "." + "StopTime, ";
	s += getTable() + "." + "JobResponsible, ";
	s += getTable() + "." + "ProductionStartTime, ";
	s += getTable() + "." + "ProductionStopTime, ";
	s += getTable() + "." + "ExternalOrderId, ";
	s += getTable() + "." + "CreationDate, ";
	s += getTable() + "." + "Priority, ";
	s += getTable() + "." + "ProcessorVersion, ";
	s += getTable() + "." + "ProcessorConfiguration, ";
	s += getTable() + "." + "Troubleshooting, ";
	s += getTable() + "." + "ProcessingTimeWindow, ";
	s += getTable() + "." + "ProcessorId, " ;
	s += getTable() + "." + "SubReq, " ;
    string t = getTable() + "." + "Parameters" ;
    s += SQLDialectFactory::sqlDialect("").function_ISNOTNULL(t)  ;
    
	return s ;
}


// get the table name
string dbOrder::getTable()
{
	return "T_OrdersQueue" ;
}


// get the order by clause
string dbOrder::getOrderBy()
{
	return "" ;
}


void dbOrder::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_orderId = " << getId() << "\n" ; 
    writeToM(o, _orderTypeSet, _orderType) ;
    writeToM(o, _statusSet, _status) ;
    writeToM(o, _processorNameSet, _processorName) ;
    writeToM(o, _satelliteIdSet, _satelliteId) ;
    writeToM(o, _missionSet, _mission) ;
    writeToM(o, _startTimeSet, _startTime) ;
    writeToM(o, _stopTimeSet, _stopTime) ;
    writeToM(o, _jobResponsibleSet, _jobResponsible) ;
    writeToM(o, _productionStartTimeSet, _productionStartTime) ;
    writeToM(o, _productionStopTimeSet, _productionStopTime) ;
    writeToM(o, _externalOrderIdSet, _externalOrderId) ;
    writeToM(o, _creationDateSet, _creationDate) ;
    writeToM(o, _prioritySet, _priority) ;
    writeToM(o, _processorVersionSet, _processorVersion) ;
    writeToM(o, _processorConfigurationSet, _processorConfiguration) ;
    writeToM(o, _troubleshootingSet, _troubleshooting) ;
    writeToM(o, _processingTimeWindowSet, _processingTimeWindow) ;
    writeToM(o, _processorIdSet, _processorId) ;
    writeToM(o, _subreqSet, _subreq) ;
    o << "_parameters = " << (_parameters ? "*" : " ") << "\n" ; // PRQA S 3380
}


void dbOrder::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_orderId = " << getId() << "\n" ;
    writeToM(o, _orderTypeSet, _orderType) ;
    writeToM(o, _statusSet, _status) ;
    writeToM(o, _processorNameSet, _processorName) ;
    writeToM(o, _satelliteIdSet, _satelliteId) ;
    writeToM(o, _missionSet, _mission) ;
    writeToM(o, _startTimeSet, _startTime) ;
    writeToM(o, _stopTimeSet, _stopTime) ;
    writeToM(o, _jobResponsibleSet, _jobResponsible) ;
    writeToM(o, _productionStartTimeSet, _productionStartTime) ;
    writeToM(o, _productionStopTimeSet, _productionStopTime) ;
    writeToM(o, _externalOrderIdSet, _externalOrderId) ;
    writeToM(o, _creationDateSet, _creationDate) ;
    writeToM(o, _prioritySet, _priority) ;
    writeToM(o, _processorVersionSet, _processorVersion) ;
    writeToM(o, _processorConfigurationSet, _processorConfiguration) ;
    writeToM(o, _troubleshootingSet, _troubleshooting) ;
    writeToM(o, _processingTimeWindowSet, _processingTimeWindow) ;
    writeToM(o, _processorIdSet, _processorId) ;
    writeToM(o, _subreqSet, _subreq) ;
    o << "_parameters = " << (_parameters ? "*" : " ") << "\n" ;// PRQA S 3380
}


_ACS_END_NAMESPACE
