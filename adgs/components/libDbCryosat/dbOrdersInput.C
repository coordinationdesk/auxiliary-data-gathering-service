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
	
	Revision 5.5  2016/05/16 15:03:57  marpas
	fixing potential bug when orderid will be very large
	coding best practice application in progress
	
	Revision 5.4  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2014/05/26 11:39:45  marpas
	dbConv casted to avoid problem with new libXMLResources templates for tags
	
	Revision 4.0  2013/09/30 12:31:02  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:30  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.10  2013/04/30 13:31:43  marpas
	dbPersistent interface changed
	
	Revision 2.9  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.8  2013/04/12 12:14:39  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.7  2013/03/29 19:51:30  marpas
	removing html related functions
	
	Revision 2.6  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.5  2012/02/14 12:52:50  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.4  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.3  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.2  2008/09/17 09:32:27  crivig
	porting to postgres8
	
	Revision 2.1  2006/05/16 12:53:37  marpas
	useless include removed
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.4  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.3  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.2  2004/10/04 15:13:53  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.1  2004/09/24 12:21:14  ivafam
	Added dbOrdersInput class
	
		
*/


#include <dbOrdersInput.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbConv.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <sstream>
#include <XMLOstream.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbOrdersInput)

// constructor
dbOrdersInput::dbOrdersInput() :
    dbPersistent(),
    _fileName(),
    _fileNameSet(),
	_fileVersion(),
	_fileVersionSet(),	
	_orderId(),
	_orderIdSet()
{
}


// copy constructor
dbOrdersInput::dbOrdersInput(const dbOrdersInput &o):
	dbPersistent(o),
	_fileName(o._fileName),
	_fileNameSet(o._fileNameSet),
	_fileVersion(o._fileVersion),
	_fileVersionSet(o._fileVersionSet),
	_orderId(o._orderId), 
	_orderIdSet(o._orderIdSet)
{
}


dbOrdersInput::~dbOrdersInput() throw()
{
}



dbPersistent *dbOrdersInput::clone() const
{
	return new dbOrdersInput(*this) ;
}

// operator =
dbOrdersInput &dbOrdersInput::operator=(const dbOrdersInput &o)
{
	Lock lock(*this) ; // PRQA S 3050  // PRQA S 3050

	if(this != &o)
	{
	    Lock lock1(o) ; // PRQA S 3050
		dbPersistent::operator=(o) ;
		_fileName = o._fileName ;
		_fileNameSet = o._fileNameSet ; 
		_fileVersion = o._fileVersion ;
		_fileVersionSet = o._fileVersionSet ; 
		_orderId = o._orderId ; 
		_orderIdSet = o._orderIdSet ; 
		
	}
	return *this ;
}


string dbOrdersInput::className() const
{
	return staticClassName() ;
}


string dbOrdersInput::staticClassName()
{
	return "dbOrdersInput" ;
}


// initialize the class
void dbOrdersInput::reset()
{
	Lock lock(*this) ; // PRQA S 3050 

	// reset dbPersistent
	dbPersistent::reset() ;
	_fileName="" ;
	_fileNameSet=false ;
	_fileVersion="" ;
	_fileVersionSet=false ;
	_orderId=0 ;
	_orderIdSet=false ;
}


// save the data in database
dbPersistent::IdType dbOrdersInput::save(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!isModified()) {
		return 0 ; 
    }

	if ((0 != getId()) || isFromDatabase()) 
	{
		ACS_THROW(InvalidUse("Use the method update() instead of save().")) ; // PRQA S 3081
	}
	
	if(!_fileNameSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"FileName\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_fileVersionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"FileVersion\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_orderIdSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"OrderId\" must be set.")) ; // PRQA S 3081
	}
	

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO " + getTable() + " (" ;
		queryString += "FileName" ;
		queryString += ", FileVersion" ;
		queryString += ", OrderId" ;
		queryString +=	") VALUES ( " ;
		queryString +=	db::toSQL(dbKey, _fileName) ;
		queryString +=	", " + db::toSQL(dbKey, _fileVersion);
		queryString +=	", " + db::toSQL(dbKey, _orderId) ;
		queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString ) ;

	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbOrdersInput::save") ;

	if (!query.execForStatus())
	{
		ACS_THROW(dbQuery::QueryExecuteException("Query Execution error: Command : " + queryString // PRQA S 3081
		                                        + " Error : " + query.errorString())) ;
	}
	
	// obtain the last serial just inserted
	setId(getLastSerial(conn, getTable()) ) ;
	
	// record that the values come from the database
	setFromDatabase() ; 
	return getId() ;
}


// load the class from the database. Return true if object exist
bool dbOrdersInput::load(dbConnection &conn , IdType id) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050 

	IdType Id=id;
	if(Id == 0) {
		Id=getId() ;
    }
	if(Id==0)
	{
		ACS_THROW(IdNotProvided("Cannot obtain \"Id\"")) ; // PRQA S 3081
	}
	
	dbOrdersInput tempRecord(*this); //store current record.
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "TempRecord: " << tempRecord) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << *this) ;
	reset() ; 
    const string &dbKey = conn.getKey() ;
	dbRow row(dbKey) ;
	
	try
	{
		string queryString ;
		const string dialectName = SQLDialectFactory::dialectName(dbKey);
		if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {

			queryString = "SELECT " ;
			queryString +=	getFields() ;
			queryString += " FROM " + getTable() ;
			queryString += " WHERE Id = " ;
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
		dbQuery query(conn, sqlString, "dbOrdersInput::load") ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;

        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
	}
	catch (dbQuery::QueryNoDataFound &e)
	{
		*this = tempRecord;  //Nothing found: restore previuos situation.
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump: \"\n" <<
		    "TempRecord: " << tempRecord << 
		    "this: " << *this ) ;
		return false ;
	}

	loadRow(row, 0) ;

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this ) ;

	return true;
}


// update an existing instance in database
dbPersistent::IdType dbOrdersInput::update(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050 

	if (0 == getId())
	{
		ACS_THROW(ObjectNotLoaded("Use the method save() instead.")) ;  // PRQA S 3081
	}

	if (!isFromDatabase())
	{
		ACS_THROW(ObjectNotLoaded("Use the method save() instead of update().")) ;  // PRQA S 3081
	}

	if (!isModified()) {
		return 0 ; 
    }

	if(!_fileNameSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"FileName\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_fileVersionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"FileVersion\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_orderIdSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"OrderId\" must be set.")) ; // PRQA S 3081
	}
	
	const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	string queryString;
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "UPDATE " + getTable() + " SET " ; 
		queryString += "FileName= " + db::toSQL(dbKey, _fileName) ;
		queryString += ", FileVersion= " + db::toSQL(dbKey, _fileVersion) ;
		queryString += ", OrderId= " + db::toSQL(dbKey, _orderId) ;
		queryString += " WHERE " ;
		queryString += "Id = " + db::toSQL(dbKey, getId()) ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbOrdersInput::update") ;

	query.execForStatus() ;
	
	resetModifiedFlag() ;
	return getId() ;

}


// remove the class from the database
dbPersistent::IdType dbOrdersInput::remove(dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050 

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE " ;
		queryString += "Id= " ;
		queryString +=	db::toSQL(dbKey, getId()) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	

	dbQuery query(conn, sqlString, "dbOrdersInput::remove") ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbOrdersInput::validate() const
{
	Lock lock(*this) ; // PRQA S 3050 

	return true;
}

// export the class to stream
void dbOrdersInput::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050 

	rsPushNSpace nsp(s,"dbOrdersInput") ;
	dbPersistent::exportObj(s) ;
	
	s.setValue("_id", getId()) ;
    RSSetM(s, _fileNameSet, _fileName) ;
    RSSetM(s, _fileVersionSet, _fileVersion) ;
    RSSetM(s, _orderIdSet, _orderId) ;
}


// import the class from a stream
void dbOrdersInput::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050 

	rsPushNSpace nsp(s,"dbOrdersInput") ;
	dbPersistent::importObj(s) ;
	reset() ;
	IdType id = 0 ;
	s.getValue("_id", id) ;
	setId(id) ;
    RSGetM(s, _fileNameSet, _fileName) ;
    RSGetM(s, _fileVersionSet, _fileVersion) ;
    RSGetM(s, _orderIdSet, _orderId) ;
}


// export the class to XML stream
void dbOrdersInput::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ; // PRQA S 3050 

	XMLOstream::Tag txml(xst, "dbOrdersInput");
	dbPersistent::exportXML(xst) ;
	{
		ostringstream os;
		os << getId();
		XMLOstream::Tag txml0(xst, "_id", os.str(), "label=\"Id\"");
	}

    XMLTagM(xst, _fileNameSet, _fileName, "FileName") ;
    XMLTagM(xst, _fileVersionSet, _fileVersion, "FileVersion") ;
    XMLTagM(xst, _orderIdSet, _orderId, "OrderId") ;
}


// load the object from a row
int dbOrdersInput::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ; // PRQA S 3050 

	IdType id = 0 ;

	row.column(offset + 0, id) ; // PRQA S 3000, 3010

	setId(id) ;
	
	row.columnSet(offset + 1, _fileName, _fileNameSet) ; // PRQA S 3000, 3010, 4400 3
	row.columnSet(offset + 2, _fileVersion, _fileVersionSet) ;
	row.columnSet(offset + 3, _orderId, _orderIdSet) ;
	

	setFromDatabase() ;

	return offset + 4 ; // PRQA S 4400
}



void dbOrdersInput::setFileName(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_fileNameSet || (_fileName != v ) )
	{
		_fileName = v ;
		_fileNameSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbOrdersInput::getFileName(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _fileNameSet ; }
	return _fileName ;
}


void dbOrdersInput::unsetFileName()
{
	
	Lock lock(*this) ; // PRQA S 3050 
	if(_fileNameSet == true) { setModifiedFlag(); }
	_fileNameSet = false;

}

void dbOrdersInput::setFileVersion(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_fileVersionSet || _fileVersion != v ) 
	{
		_fileVersion = v ;
		_fileVersionSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbOrdersInput::getFileVersion(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _fileVersionSet ; }
	return _fileVersion ;
}


void dbOrdersInput::unsetFileVersion()
{
	
	Lock lock(*this) ; // PRQA S 3050 
	if(_fileVersionSet == true) { setModifiedFlag(); }
	_fileVersionSet = false;

}

void dbOrdersInput::setOrderId(IdType v)
{
	Lock lock(*this) ; // PRQA S 3050 

	if (!_orderIdSet || (_orderId != v ) )
	{
		_orderId = v ;
		_orderIdSet = true ;
		setModifiedFlag() ;
	}
}

dbPersistent::IdType dbOrdersInput::getOrderId(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050 

	if (set) { *set = _orderIdSet ; }
	return _orderId ;
}

void dbOrdersInput::unsetOrderId()
{
	Lock lock(*this) ; // PRQA S 3050 
	if(_orderIdSet == true) { setModifiedFlag(); }
	_orderIdSet = false;

}


// get the field names
string dbOrdersInput::getFields()
{
	string s ;
	s += getTable() + "." + "Id, ";
	s += getTable() + "." + "FileName, ";
	s += getTable() + "." + "FileVersion, ";
	s += getTable() + "." + "OrderId ";
	
	return s ;
}


// get the table name
string dbOrdersInput::getTable()
{
	return "T_OrdersInput" ;
}


// get the order by clause
string dbOrdersInput::getOrderBy()
{
	return "" ;
}


void dbOrdersInput::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050  
	o << "_id = " << getId() << "\n" ;
    writeToM(o, _fileNameSet, _fileName) ;
    writeToM(o, _fileVersionSet, _fileVersion) ;
    writeToM(o, _orderIdSet, _orderId) ;
}


void dbOrdersInput::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050  
	o << "_id = " << getId() << "\n" ;
    writeToM(o, _fileNameSet, _fileName) ;
    writeToM(o, _fileVersionSet, _fileVersion) ;
    writeToM(o, _orderIdSet, _orderId) ;
}

_ACS_END_NAMESPACE
