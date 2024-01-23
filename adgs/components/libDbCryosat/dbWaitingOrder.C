// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 7.0  2017/05/31 17:32:59  nicvac
	OPMAN-260: Need PDS DB schema PDS_12-04.
	
	Revision 6.0  2016/07/07 13:44:05  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.5  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:04  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:32  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.10  2013/04/30 13:31:43  marpas
	dbPersistent interface changed
	
	Revision 2.9  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.8  2013/04/12 12:14:39  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.7  2013/03/29 19:51:31  marpas
	removing html related functions
	
	Revision 2.6  2013/02/22 11:58:56  marpas
	using StringUtils::findNoCase
	
	Revision 2.5  2012/02/14 12:52:51  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.4  2011/03/02 18:47:15  marpas
	serial 8 support
	
	Revision 2.3  2010/03/23 15:35:04  marpas
	new fields added
	
	Revision 2.2  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.1  2008/09/17 09:32:27  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.3  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.2  2005/03/08 10:17:35  marpas
	getTriggerType method interface fixed
	
	Revision 1.1  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
		

*/

#include <dbWaitingOrder.h>
#include <dbQuery.h>
#include <Transaction.h>
#include <dbConv.h>
#include <exStream.h>
#include <dbRow.h>
#include <dbSet.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



//
//
//
// class dbWaitingOrder
//
//
//


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbWaitingOrder)

// constructor
dbWaitingOrder::dbWaitingOrder() :
	dbPersistent(),
	_orderId(),
	_orderIdSet(),
	_fileType(),
	_fileTypeSet(),
	_startTime() ,
	_startTimeSet() ,
	_stopTime() ,
	_stopTimeSet() ,
	_triggerType(),
	_triggerTypeSet(),
	_expDate() ,
	_expDateSet() 

{
}


// copy constructor
dbWaitingOrder::dbWaitingOrder(const dbWaitingOrder &o):
	dbPersistent(o),
	_orderId(o._orderId),
	_orderIdSet(o._orderIdSet),
	_fileType(o._fileType),
	_fileTypeSet(o._fileTypeSet),
	_startTime(o._startTime) ,
	_startTimeSet(o._startTimeSet) ,
	_stopTime(o._stopTime) ,
	_stopTimeSet(o._stopTimeSet) ,
	_triggerType(o._triggerType),
	_triggerTypeSet(o._triggerTypeSet),
	_expDate(o._expDate) ,
	_expDateSet(o._expDateSet) 

{
}


dbWaitingOrder::~dbWaitingOrder() throw() 
{
}




dbPersistent *dbWaitingOrder::clone() const
{
	return new dbWaitingOrder(*this) ;
}



// operator =
dbWaitingOrder &dbWaitingOrder::operator=(const dbWaitingOrder &o)
{
	Lock lock(*this) ; // PRQA S 3050 2
    Lock lo(o) ;
	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_orderId=o._orderId;
		_orderIdSet=o._orderIdSet;
		_fileType=o._fileType;
		_fileTypeSet=o._fileTypeSet;
		_startTime=o._startTime;
		_startTimeSet=o._startTimeSet;
		_stopTime=o._stopTime;
		_stopTimeSet=o._stopTimeSet;
		_triggerType = o._triggerType ;
		_triggerTypeSet = o._triggerTypeSet ; 
		_expDate=o._expDate;
		_expDateSet=o._expDateSet;
	}
	return *this ;
}


// initialize the class
void dbWaitingOrder::reset()
{
	Lock lock(*this) ; // PRQA S 3050 

	// reset dbPersistent
	dbPersistent::reset() ;
	_orderId=0 ;
	_orderIdSet=false ;

	_fileType="";
	_fileTypeSet=false;
	_startTime.reset() ;
	_startTimeSet=false ;
	_stopTime.reset() ;
	_stopTimeSet=false ;
	_triggerType = "" ;
	_triggerTypeSet = false ; 
	_expDate.reset() ;
	_expDateSet=false ;
}



string dbWaitingOrder::className() const
{
	return staticClassName() ;
}


string dbWaitingOrder::staticClassName()
{
	return "dbWaitingOrder" ;
}


// save the data in database
dbPersistent::IdType dbWaitingOrder::save(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050 
		
	if (!isModified()) {
		return 0 ; 
    }
	
	//Check Mandatory fields
	if ((getId() != 0) || isFromDatabase()) 
	{
		ACS_THROW(InvalidUse("Use the method update() instead of save()."))  ; // PRQA S 3081
	}

	if(!_orderIdSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Order Id\" must be set.")) ; // PRQA S 3081
	}
	if(!_startTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Start Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_stopTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Stop Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_triggerTypeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Trigger Type\" must be set.")) ; // PRQA S 3081
	}


    db::Transaction transaction(conn) ;
    transaction.begin_work("dbWaitingOrder_SAVE") ;
    
	conn.addStateWrapper(*this) ;

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO " + getTable() + " (" ;
		queryString += "OrderId" ;
		queryString += ", FileType" ;
		queryString += ", StartTime" ;
		queryString += ", StopTime" ;
		queryString += ", TriggerType" ;
		if(_expDateSet) {
			queryString += ", ExpirationDate" ;
        }
		queryString +=	") VALUES ( " ;
		queryString +=	db::toSQL(dbKey, _orderId) ;
		queryString +=	", " + db::toSQL(dbKey, _fileType) ;
		queryString +=	", " + db::toSQL(dbKey, _startTime,dbConv::YearToFraction) ;
		queryString +=	", " + db::toSQL(dbKey, _stopTime,dbConv::YearToFraction) ;
		queryString +=	", " + db::toSQL(dbKey, _triggerType) ;
		if(_expDateSet) {
			queryString +=	", " + db::toSQL(dbKey, _expDate,dbConv::YearToFraction) ;
        }
		queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString ) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbWaitingOrder::save") ;

	if (!query.execForStatus())
	{
			ACS_THROW(dbQuery::QueryExecuteException("Query Execution error: Command : " // PRQA S 3081
			+ queryString+ " Error : "+ query.errorString())) ;
	}

	// obtain the last serial just inserted
	setId(getLastSerial(conn, getTable())) ;
    transaction.commit() ;      

	// record that the values come from the database
	setFromDatabase() ; 
	
	return getId() ;
}


// load the class from the database. Return true if object exist
bool dbWaitingOrder::load(dbConnection &conn , IdType id) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050 

	IdType Id=id;
	if(Id == 0) {
		Id=getId() ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called with id=" << id << " getId():" << getId() << " Id:" << Id) ;
	if(Id==0) 
	{
		ACS_THROW(IdNotProvided("Cannot obtain \"OrderId\"")) ; // PRQA S 3081
	}

	dbWaitingOrder tempRecord(*this); //store current record.


	reset() ; 

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {

		queryString = "SELECT " ;
		queryString +=	getFields() ;
		queryString += " FROM " + getTable() ;
		queryString += " WHERE WaitOrderId = " ;
		queryString +=	db::toSQL(dbKey, Id) ;
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
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	dbQuery query(conn, sqlString, "dbWaitingOrder::load") ;
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
dbPersistent::IdType dbWaitingOrder::update(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050 

	if (0 == getId())
	{
		ACS_THROW(ObjectNotLoaded("Use the method save() instead.")) ; // PRQA S 3081
	}

	if (!isFromDatabase())
	{
		ACS_THROW(ObjectNotLoaded("Use the method save() instead of update().")) ; // PRQA S 3081
	}

	if (!isModified()) {
		return 0 ; 
    }

	if(!_orderIdSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Order Id\" must be set.")) ; // PRQA S 3081
	}
	if(!_startTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Start Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_stopTimeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Stop Time\" must be set.")) ; // PRQA S 3081
	}
	if(!_triggerTypeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Trigger Type\" must be set.")) ; // PRQA S 3081
	}


	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "UPDATE " + getTable() + " SET " ; 
		queryString += "OrderId= " + db::toSQL(dbKey, _orderId) ;
		queryString += ", FileType= " + db::toSQL(dbKey, _fileType) ;
		queryString += ", StartTime= " + db::toSQL(dbKey, _startTime,dbConv::YearToFraction) ;
		queryString += ", StopTime= " + db::toSQL(dbKey, _stopTime,dbConv::YearToFraction) ;
		queryString += ", TriggerType= " + db::toSQL(dbKey, _triggerType) ;
		if(_expDateSet) {
			queryString += ", ExpirationDate= " + db::toSQL(dbKey, _expDate,dbConv::YearToFraction) ;
        }
		else {
			queryString += ", ExpirationDate=  NULL " ; 
        }

		queryString += " WHERE " ;
		queryString += "WaitOrderId = " + db::toSQL(dbKey, getId()) ;


		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString ) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

    db::Transaction transaction(conn) ;
    transaction.begin_work("dbWaitingOrder_UPDATE") ;

	conn.addStateWrapper(*this) ;
	dbQuery query(conn, sqlString, "dbWaitingOrder::update") ;

	query.execForStatus() ;
    transaction.commit() ;
    
	resetModifiedFlag() ;
	return getId() ;
}


// remove the class from the database
dbPersistent::IdType dbWaitingOrder::remove(dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (0 == getId())
	{
		ACS_THROW(IdNotProvided("Id not set")) ; // PRQA S 3081
	}

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE " ;
		queryString += "WaitOrderId = " + db::toSQL(dbKey, getId()) ;

	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
    db::Transaction transaction(conn) ;
    transaction.begin_work("dbWaitingOrder_REMOVE") ;

	conn.addStateWrapper(*this) ;
	dbQuery query(conn, sqlString, "dbWaitingOrder::remove") ;

	query.execForStatus() ;
    transaction.commit() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbWaitingOrder::validate() const
{
	Lock lock(*this) ; // PRQA S 3050 

	return true;
}


// export the class to stream
void dbWaitingOrder::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050 

	rsPushNSpace nsp(s,"dbWaitingOrder") ;
	
	dbPersistent::exportObj(s);
	s.setValue("_waitOrderId", getId()) ;
	
	if(_orderIdSet)
	{
		s.setValue("_orderId",_orderId) ;
	}
	s.setValue("_orderIdSet",_orderIdSet) ;
	
	if(_fileTypeSet)
	{
		s.setValue("_fileType",_fileType) ;
	}
	s.setValue("_fileTypeSet",_fileTypeSet) ;
	
	if(_startTimeSet)
	{
		rsPushNSpace rpns5(s,"_startTime") ;
		_startTime.exportObj(s) ;
	}
	s.setValue("_startTimeSet",_startTimeSet) ;

	if(_stopTimeSet)
	{
		rsPushNSpace rpns6(s,"_stopTime") ;
		_stopTime.exportObj(s) ;
	}
	s.setValue("_stopTimeSet",_stopTimeSet) ;
	
	if(_triggerTypeSet)
	{
		s.setValue("_triggerType",_triggerType) ;
	}
	s.setValue("_triggerTypeSet",_triggerTypeSet) ;
	if(_expDateSet)
	{
		rsPushNSpace rpns5(s,"_expDate") ;
		_expDate.exportObj(s) ;
	}
	s.setValue("_expDateSet",_expDateSet) ;

}


// import the class from a stream
void dbWaitingOrder::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050 

	rsPushNSpace nsp(s,"dbWaitingOrder") ;

	dbPersistent::importObj(s) ;
	reset() ;
	IdType id = 0 ;
	s.getValue("_waitOrderId", id) ;
	setId(id) ;

	s.getValue("_orderIdSet",_orderIdSet) ;
	if(_orderIdSet)
	{
		s.getValue("_orderId",_orderId) ;
	}
	else {
		_orderId = 0 ;
    }


	s.getValue("_fileTypeSet",_fileTypeSet) ;
	if(_fileTypeSet)
	{
		s.getValue("_fileType",_fileType) ;
	}
	else {
		_fileType = "" ;
    }
		
	s.getValue("_startTimeSet",_startTimeSet) ;
	if(_startTimeSet)
	{
		rsPushNSpace rpns5(s,"_startTime") ;
		_startTime.importObj(s) ;
	}
	else {
		_startTime.reset() ;
    }
	s.getValue("_stopTimeSet",_stopTimeSet) ;
	if(_stopTimeSet)
	{
		rsPushNSpace rpns6(s,"_stopTime") ;
		_stopTime.importObj(s) ;
	}
	else {
		_stopTime.reset() ;
    }
	s.getValue("_triggerTypeSet",_triggerTypeSet) ;
	if(_triggerTypeSet)
	{
		s.getValue("_triggerType",_triggerType) ;
	}
	else {
		_triggerType = "" ;
    }

	s.getValue("_expDateSet",_expDateSet) ;
	if(_expDateSet)
	{
		rsPushNSpace rpns5(s,"_expDate") ;
		_expDate.importObj(s) ;
	}
	else {
		_expDate.reset() ;
    }
}


// export the class to XML stream
void dbWaitingOrder::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ; // PRQA S 3050 

	XMLOstream::Tag txml(xst, "dbWaitingOrder");

	dbPersistent::exportXML(xst);
    string null = "<null>" ;
    
	if(_orderIdSet)
	{
		XMLOstream::Tag txml0(xst, "_orderId", _orderId, "label=\"Order Id\"");
	}
	else
	{
		XMLOstream::Tag txml0(xst, "_orderId", null, "label=\"Order Id\"");
	}
	{
		XMLOstream::Tag txml0Set(xst, "_orderIdSet", (_orderIdSet ? "true" : "false")) ; // PRQA S 3380
	}
	if(_fileTypeSet)
	{
		XMLOstream::Tag txml20(xst, "_fileType", _fileType, "label=\"File Type\"");
	}
	else
	{
		XMLOstream::Tag txml20(xst, "_fileType", null, "label=\"File Type\"");
	}
	{
		XMLOstream::Tag txml20Set(xst, "_fileTypeSet", (_fileTypeSet ? "true" : "false")) ; // PRQA S 3380
	}
	if(_startTimeSet)
	{
		XMLOstream::Tag txml5(xst, "_startTime", "", "label=\"Start Time\"");
		_startTime.exportXML(xst) ;
	}
	else
	{
		XMLOstream::Tag txml5(xst, "_startTime", null, "label=\"Start Time\"");
	}
	{
		XMLOstream::Tag txml5Set(xst, "_startTimeSet", (_startTimeSet ? "true" : "false")) ; // PRQA S 3380
	}
	if(_stopTimeSet)
	{
		XMLOstream::Tag txml6(xst, "_stopTime", "", "label=\"Stop Time\"");
		_stopTime.exportXML(xst) ;
	}
	else
	{
		XMLOstream::Tag txml6(xst, "_stopTime", null, "label=\"Stop Time\"");
	}
	{
		XMLOstream::Tag txml6Set(xst, "_stopTimeSet", (_stopTimeSet ? "true" : "false")) ; // PRQA S 3380
	}
	
	if(_triggerTypeSet)
	{
	XMLOstream::Tag txml1(xst, "_triggerType", _triggerType, "label=\"Trigger Type\"");
	}
	else
	{
	XMLOstream::Tag txml1(xst, "_triggerType", null, "label=\"Trigger Type\"");
	}
	{
	XMLOstream::Tag txml1Set(xst, "_triggerTypeSet", (_triggerTypeSet ? "true" : "false")) ; // PRQA S 3380
	}

	if(_expDateSet)
	{
		XMLOstream::Tag txml5(xst, "_expDate", "", "label=\"Expiration Date\"");
		_expDate.exportXML(xst) ;
	}
	else
	{
		XMLOstream::Tag txml5(xst, "_expDate", null, "label=\"Expiration Date\"");
	}
	{
		XMLOstream::Tag txml5Set(xst, "_expDateSet", (_expDateSet ? "true" : "false")) ; // PRQA S 3380
	}

}


// load the object from a row
int dbWaitingOrder::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ; // PRQA S 3050 

	IdType id = 0 ;

	row.column(offset + 0, id) ; // PRQA S 3000, 3010

	setId(id) ;

	row.columnSet(offset + 1, _orderId, 	_orderIdSet) ; // PRQA S 3000, 3010 6
	row.columnSet(offset + 2, _fileType, 	_fileTypeSet) ; // PRQA S 4400 5
	row.columnSet(offset + 3, _startTime,	_startTimeSet) ;
	row.columnSet(offset + 4, _stopTime,	_stopTimeSet) ;
	row.columnSet(offset + 5, _triggerType,	_triggerTypeSet) ;
	row.columnSet(offset + 6, _expDate,		_expDateSet) ;

	setFromDatabase() ;

	return offset + 7 ; // PRQA S 4400
}



void dbWaitingOrder::setWaitOrderId(dbPersistent::IdType v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (getId() != v ) 
	{
		setId(v) ;
		setModifiedFlag() ;
	}
}


dbPersistent::IdType dbWaitingOrder::getWaitOrderId() const
{
	Lock lock(*this) ; // PRQA S 3050 

	return getId() ;
}

void dbWaitingOrder::setOrderId(dbPersistent::IdType v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_orderIdSet || (_orderId != v ) )
	{
		_orderId = v ;
		_orderIdSet = true ;
		setModifiedFlag() ;
	}
}


dbPersistent::IdType dbWaitingOrder::getOrderId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _orderIdSet ; }
	return _orderId ;
}


void dbWaitingOrder::unsetOrderId()
{
	Lock lock(*this) ; // PRQA S 3050 

	if(_orderIdSet) { setModifiedFlag(); }

	_orderIdSet = false;
}

void dbWaitingOrder::setFileType(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (v.size() > 16) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("Error: dbWaitingOrder::setFileType(). String too long.")) ; // PRQA S 3081
	}

	if (!_fileTypeSet || _fileType != v ) 
	{
		_fileType = v ;
		_fileTypeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbWaitingOrder::getFileType(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _fileTypeSet ; }
	return _fileType ;
}

void dbWaitingOrder::unsetFileType()
{
	Lock lock(*this) ; // PRQA S 3050 

	if(_fileTypeSet) {
		setModifiedFlag();
    }

	_fileTypeSet = false;

}

void dbWaitingOrder::setStartTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_startTimeSet || (_startTime != v ) )
	{
		_startTime = v ;
		_startTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbWaitingOrder::getStartTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _startTimeSet ; }
	return _startTime ;
}


void dbWaitingOrder::unsetStartTime()
{
	Lock lock(*this) ; // PRQA S 3050 
	if(_startTimeSet == true) {
		setModifiedFlag();
    }
	_startTimeSet = false;

}
void dbWaitingOrder::setStopTime(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_stopTimeSet || (_stopTime != v ) )
	{
		_stopTime = v ;
		_stopTimeSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbWaitingOrder::getStopTime(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _stopTimeSet ; }
	return _stopTime ;
}


void dbWaitingOrder::unsetStopTime()
{
	Lock lock(*this) ; // PRQA S 3050 
	if(_stopTimeSet == true) {
		setModifiedFlag();
    }
	_stopTimeSet = false;

}

void dbWaitingOrder::setTriggerType(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (v.size() > 16) // PRQA S 4400
	{
		ACS_THROW(InvalidUse("Error: dbWaitingOrder::setTriggerType(). String too long.")) ; // PRQA S 3081
	}

	if (!_triggerTypeSet || (_triggerType != v ) )
	{
		_triggerType = v ;
		_triggerTypeSet = true ;
		setModifiedFlag() ;
	}
}


string dbWaitingOrder::getTriggerType(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _triggerTypeSet ; }
	return _triggerType ;
}


void dbWaitingOrder::unsetTriggerType()
{
	Lock lock(*this) ; // PRQA S 3050 
	if(_triggerTypeSet == true) {
		setModifiedFlag();
    }
	_triggerTypeSet = false;

}

void dbWaitingOrder::setExpirationDate(const DateTime &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_expDateSet || (_expDate != v ) )
	{
		_expDate = v ;
		_expDateSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbWaitingOrder::getExpirationDate(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _expDateSet ; }
	return _expDate ;
}


void dbWaitingOrder::unsetExpirationDate()
{
	Lock lock(*this) ; // PRQA S 3050 
	if(_expDateSet == true) {
		setModifiedFlag();
    }
	_expDateSet = false;

}


// get the field names
string dbWaitingOrder::getFields()
{
	string s ;
	s += getTable() + "." + "WaitOrderId, ";
	s += getTable() + "." + "OrderId, ";
	s += getTable() + "." + "FileType, ";
	s += getTable() + "." + "StartTime, ";
	s += getTable() + "." + "StopTime, ";
	s += getTable() + "." + "TriggerType,  ";
	s += getTable() + "." + "ExpirationDate  ";
	return s ;
}


// get the table name
string dbWaitingOrder::getTable()
{
	return "T_WaitingOrders" ;
}


// get the order by clause
string dbWaitingOrder::getOrderBy()
{
	return "" ;
}


void dbWaitingOrder::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050
    string null = "<null>" ;
	string tmp ; 

	o
		 << "_waitorderId = "  << getId() << "\n" 
		 << "_orderId = " << (_orderIdSet ? db::toSQL("",_orderId) : null) << "\n"  // PRQA S 3380, 3385
		 << "_fileType = " << (_fileTypeSet ? _fileType : null) << "\n"  // PRQA S 3380 5
		 << "_startTime = " << (_startTimeSet ? _startTime.utcs(tmp) : null) << "\n" 
		 << "_stopTime = " << (_stopTimeSet ? _stopTime.utcs(tmp) : null) << "\n" 
		 << "_triggerType = " << (_triggerTypeSet ? _triggerType : null) << "\n"  
		 << "_expDate = " << (_expDateSet ? _expDate.utcs(tmp) : null) << "\n" ;
}



void dbWaitingOrder::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050
    string null = "<null>" ;
	string tmp ; 

	o
		 << "_waitorderId = "  << getId() << "\n" 
		 << "_orderId = " << (_orderIdSet ? db::toSQL("",_orderId) : null) << "\n"  // PRQA S 3380, 3385
		 << "_fileType = " << (_fileTypeSet ? _fileType : null) << "\n"  // PRQA S 3380 5
		 << "_startTime = " << (_startTimeSet ? _startTime.utcs(tmp) : null) << "\n" 
		 << "_stopTime = " << (_stopTimeSet ? _stopTime.utcs(tmp) : null) << "\n" 
		 << "_triggerType = " << (_triggerTypeSet ? _triggerType : null) << "\n"  
		 << "_expDate = " << (_expDateSet ? _expDate.utcs(tmp) : null) << "\n" ;
}


_ACS_END_NAMESPACE
