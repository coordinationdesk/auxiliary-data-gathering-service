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
	Revision 6.1  2016/07/27 08:35:44  damdec
	CSGACS-86: compilation warnings fixed.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.6  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.5  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2014/05/26 11:39:44  marpas
	dbConv casted to avoid problem with new libXMLResources templates for tags
	
	Revision 4.0  2013/09/30 12:30:56  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:24  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.12  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.11  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.10  2013/04/12 12:14:37  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.9  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.8  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.7  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.6  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.5  2010/03/05 15:09:10  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.4  2008/10/10 09:31:13  ivafam
	Added storageid
	
	Revision 2.3  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.2  2008/09/12 17:24:03  ivafam
	Added storageid field
	
	Revision 2.1  2006/05/16 12:53:37  marpas
	useless include removed
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.13  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.12  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.11  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.10  2004/10/04 15:13:53  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.9  2004/09/28 15:08:08  ivafam
	Bug Fixed in update
	
	Revision 1.8  2004/09/28 15:06:30  ivafam
	Bug Fixed
	
	Revision 1.7  2004/09/16 14:42:05  ivafam
	Work in progress
	
	Revision 1.6  2004/09/07 16:58:24  ivafam
	Now import set ID
	
	Revision 1.5  2004/09/07 15:36:04  ivafam
	Work in progess
	
	Revision 1.4  2004/09/07 09:10:16  ivafam
	_expitationDay --> _expirationDate
	
	Revision 1.3  2004/09/07 09:02:17  ivafam
	_fileName    --> _inventoryName
	_fileVersion --> _inventoryVersion
	
	Revision 1.2  2004/09/06 16:13:04  ivafam
	Work in progress
	
	Revision 1.1  2004/09/03 15:29:49  ivafam
	Added new classes
	
	
*/


#include <dbInvSpecialRollingPolicies.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <dbSet.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <RollingPoliciesActions.h>
#include <sstream>
#include <XMLOstream.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInvSpecialRollingPolicies)

// constructor
dbInvSpecialRollingPolicies::dbInvSpecialRollingPolicies():
	dbPersistent(),
	_inventoryName(),
	_inventoryNameSet(false),
	_inventoryVersion(),
	_inventoryVersionSet(false),
	_expirationDate(),
	_expirationDateSet(false), 
	_action(), 
	_actionSet(false),
	_storage(), 
	_storageSet(false)
{
}


// copy constructor
dbInvSpecialRollingPolicies::dbInvSpecialRollingPolicies(const dbInvSpecialRollingPolicies &o):
	dbPersistent(o),
	_inventoryName(o._inventoryName),
	_inventoryNameSet(o._inventoryNameSet),
	_inventoryVersion(o._inventoryVersion),
	_inventoryVersionSet(o._inventoryVersionSet),
	_expirationDate(o._expirationDate),
	_expirationDateSet(o._expirationDateSet), 
	_action(o._action), 
	_actionSet(o._actionSet),
	_storage(o._storage), 
	_storageSet(o._storageSet)
{
}


dbInvSpecialRollingPolicies::~dbInvSpecialRollingPolicies() throw()
{
}



dbPersistent *dbInvSpecialRollingPolicies::clone() const
{
	return new dbInvSpecialRollingPolicies(*this) ;
}

// operator =
dbInvSpecialRollingPolicies &dbInvSpecialRollingPolicies::operator=(const dbInvSpecialRollingPolicies &o)
{
	Lock lock(*this) ;

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_inventoryName = o._inventoryName ;
		_inventoryNameSet = o._inventoryNameSet ; 
		_inventoryVersion = o._inventoryVersion ;
		_inventoryVersionSet = o._inventoryVersionSet ; 
		_expirationDate = o._expirationDate ;
		_expirationDateSet = o._expirationDateSet ; 
		_action = o._action ; 
		_actionSet = o._actionSet ;
		_storage = o._storage ; 
		_storageSet = o._storageSet ; 
		
	}
	return *this ;
}


string dbInvSpecialRollingPolicies::className() const
{
	return staticClassName() ;
}


string dbInvSpecialRollingPolicies::staticClassName()
{
	return "dbInvSpecialRollingPolicies" ;
}


// initialize the class
void dbInvSpecialRollingPolicies::reset()
{
	Lock lock(*this) ;

	// reset dbPersistent
	dbPersistent::reset() ;
	_inventoryName="" ;
	_inventoryNameSet=false ;
	_inventoryVersion="" ;
	_inventoryVersionSet=false ;
	_expirationDate.reset() ;
	_expirationDateSet=false ;
	_action=0 ;
	_actionSet=false ;
	_storage=0 ;
	_storageSet=false ;
}


// save the data in database
dbPersistent::IdType dbInvSpecialRollingPolicies::save(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!isModified())
		return 0 ; 

	if (getId() || isFromDatabase()) 
	{
		InvalidUse e("Error: dbInvSpecialRollingPolicies::save(). Use the method update() instead of save().") ;
		ACS_THROW(e) ;
	}
	
	if(!_inventoryNameSet) 
	{
		InvalidUse e("Mandatory field \"InventoryName\" must be set.") ;
		ACS_THROW(e) ;
	}
	
	if(!_inventoryVersionSet) 
	{
		InvalidUse e("Mandatory field \"InventoryVersion\" must be set.") ;
		ACS_THROW(e) ;
	}
	
	if(!_actionSet) 
	{
		InvalidUse e("Mandatory field \"Action\" must be set.") ;
		ACS_THROW(e) ;
	}
	

    const string &dbKey = conn.getKey() ;
	string queryString ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO " + getTable() + " (" ;
		queryString += "InventoryName" ;
		queryString += ", InventoryVersion" ;
		if(_expirationDateSet)
			queryString += ", ExpirationDate" ;
		queryString += ", Action" ;

		if(_storageSet) 
			queryString += ", StorageId" ;
		queryString +=	") VALUES ( " ;
		queryString +=	 db::toSQL(dbKey, _inventoryName) ;
		queryString +=	", " +  db::toSQL(dbKey, _inventoryVersion);
		if(_expirationDateSet)
			queryString +=	", " +  db::toSQL(dbKey, _expirationDate,dbConv::YearToFraction) ;
		queryString +=	", " +  db::toSQL(dbKey, _action) ;

		if(_storageSet) 
			queryString += ", " +  db::toSQL(dbKey, _storage) ;
		queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbInvSpecialRollingPolicies::save") ;
	if (!query.execForStatus())
	{
		dbQuery::QueryExecuteException e(string("Query Execution error: Command : ")
		+ string(queryString)
		+ string(" Error : ")
		+ string(query.errorString())) ;
		ACS_THROW (e) ;
	}
	
	// obtain the last serial just inserted
	setId(getLastSerial(conn, getTable()) ) ;
	
	// record that the values come from the database
	setFromDatabase() ; 
	return getId() ;
}


// load the class from the database. Return true if object exist
bool dbInvSpecialRollingPolicies::load(dbConnection &conn , IdType id)
{
	Lock lock(*this) ;
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	unsigned long Id=id;
	if(Id == 0)
		Id=getId() ;
	if(Id==0)
	{
		IdNotProvided e("Cannot obtain \"Id\"") ;
		ACS_THROW(e) ;
	}
	
	dbInvSpecialRollingPolicies tempRecord(*this); //store current record.
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "TempRecord: " << tempRecord) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << *this) ;
	reset() ; 
	
	string queryString ;
	const string &dbKey = conn.getKey() ;
	dbRow row(dbKey) ;

	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "SELECT " ;
		queryString +=	getFields() ;
		queryString += " FROM " + getTable() ;
		queryString += " WHERE Id = " ;
		queryString +=	 db::toSQL(dbKey, Id) ;

		string getOrderByString(getOrderBy()) ;
		if(!getOrderByString.empty() )
		{
			string orderby= "ORDER BY";
			if(StringUtils::findNoCase(getOrderByString,orderby) != string::npos)
				queryString += getOrderByString ;
			else
				queryString += " ORDER BY " + getOrderByString ;
		}
	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	dbQuery query(conn, sqlString, "dbInvSpecialRollingPolicies::load") ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;

	dbSet setQ;
	query.execForSet(setQ) ;
    if (setQ.size() > 1) // it should not occur
    {
        ACS_THROW(exIllegalValueException("Unexpected query result dimension: it should return just one row"));
    }
    else
    {
		if ( not setQ.getNextRow(row) )
        {
			*this = tempRecord;  //Nothing found: restore previuos situation.
	    	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			excerr << "dbInvSpecialRollingPolicies::load: Dump : \"" << endl;
			excerr << "TempRecord: " << tempRecord << endl;
			excerr << "this: " << *this<< endl;
	    	ACS_CLASS_END_DEBUG
			return false ;
        }
	}

	loadRow(row, 0) ;

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	return true;
}


// update an existing instance in database
dbPersistent::IdType dbInvSpecialRollingPolicies::update(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!getId())
	{
		ObjectNotLoaded ex("SegmentCommon::update(). Use the method save() instead.") ; 
		ACS_THROW(ex)
	}

	if (!isFromDatabase())
	{
		ObjectNotLoaded ex("Error: dbInvSpecialRollingPolicies::update(). Use the method save() instead of update().") ; 
		ACS_THROW(ex)
	}

	if (!isModified())
		return 0 ; 

	if(!_inventoryNameSet) 
	{
		InvalidUse e("Mandatory field \"InventoryName\" must be set.") ;
		ACS_THROW(e) ;
	}
	
	if(!_inventoryVersionSet) 
	{
		InvalidUse e("Mandatory field \"InventoryVersion\" must be set.") ;
		ACS_THROW(e) ;
	}
	
	if(!_actionSet) 
	{
		InvalidUse e("Mandatory field \"Action\" must be set.") ;
		ACS_THROW(e) ;
	}
	
	
	const string &dbKey = conn.getKey() ;
	string queryString ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = string("UPDATE ") + getTable() + " SET " ; 
		queryString += "InventoryName= " +  db::toSQL(dbKey, _inventoryName) ;
		queryString += ", InventoryVersion= " +  db::toSQL(dbKey, _inventoryVersion) ;
		if(_expirationDateSet)
			queryString += ", ExpirationDate= " +  db::toSQL(dbKey, _expirationDate,dbConv::YearToFraction) ;
		queryString += ", Action= " +  db::toSQL(dbKey, _action) ;

		if(_storageSet) 
			queryString += ", StorageId= " +  db::toSQL(dbKey, _storage) ;
		else
			queryString += ", StorageId= NULL ";
		queryString += " WHERE " ;
		queryString += "Id = " +  db::toSQL(dbKey, getId()) ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	
	dbQuery query(conn, sqlString, "dbInvSpecialRollingPolicies::update") ;

	query.execForStatus() ;
	
	resetModifiedFlag() ;
	return getId() ;

}


// remove the class from the database
dbPersistent::IdType dbInvSpecialRollingPolicies::remove(dbConnection &conn)
{
	Lock lock(*this) ;

    const string &dbKey = conn.getKey() ;
	string queryString ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {

		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE " ;
		queryString += "Id= " ;
		queryString +=	 db::toSQL(dbKey, getId()) ;
	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;


	dbQuery query(conn, sqlString, "dbInvSpecialRollingPolicies::remove") ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbInvSpecialRollingPolicies::validate() const
{
	Lock lock(*this) ;

	return true;
}


// export the class to stream
void dbInvSpecialRollingPolicies::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbInvSpecialRollingPolicies") ;
	dbPersistent::exportObj(s) ;
	
	s.setValue("_id", getId()) ;
	if(_inventoryNameSet)
	{
		s.setValue("_inventoryName",_inventoryName) ;
	}
	s.setValue("_inventoryNameSet",_inventoryNameSet) ;
	
	if(_inventoryVersionSet)
	{
		s.setValue("_inventoryVersion",_inventoryVersion) ;
	}
	s.setValue("_inventoryVersionSet",_inventoryVersionSet) ;

	if(_expirationDateSet)
	{
		rsPushNSpace rpns(s,"_expirationDate") ;
		_expirationDate.exportObj(s) ;
	}
	s.setValue("_expirationDateSet",_expirationDateSet) ;

	if(_actionSet)
	{
		s.setValue("_action",_action) ;
	}
	s.setValue("_actionSet",_actionSet) ;
	
	if(_storageSet)
	{
		s.setValue("_storage",_storage) ;
	}
	s.setValue("_storageSet",_storageSet) ;
}


// import the class from a stream
void dbInvSpecialRollingPolicies::importObj(rsResourceSet &s)
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbInvSpecialRollingPolicies") ;
	dbPersistent::importObj(s) ;
	reset() ;
	
	unsigned long id ;
	s.getValue("_id", id) ;
	setId(id) ;
	
	s.getValue("_inventoryNameSet",_inventoryNameSet) ;
	if(_inventoryNameSet)
	{
		s.getValue("_inventoryName",_inventoryName) ;
	}
	
	s.getValue("_inventoryVersionSet",_inventoryVersionSet) ;
	if(_inventoryVersionSet)
	{
		s.getValue("_inventoryVersion",_inventoryVersion) ;
	}
	
	s.getValue("_expirationDateSet",_expirationDateSet) ;
	if(_expirationDateSet)
	{
		rsPushNSpace rpns(s,"_expirationDate") ;
		_expirationDate.importObj(s) ;
	}
	
	s.getValue("_actionSet",_actionSet) ;
	if(_actionSet)
	{
		s.getValue("_action",_action) ;
	}
	
	s.getValue("_storageSet",_storageSet) ;
	if(_storageSet)
	{
		s.getValue("_storage",_storage) ;
	}
		
}


// export the class to XML stream
void dbInvSpecialRollingPolicies::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ;

	XMLOstream::Tag txml(xst, "dbInvSpecialRollingPolicies");
	dbPersistent::exportXML(xst) ;
	if(getId())
	{
		ostringstream os;
		os << getId();
		XMLOstream::Tag txml0(xst, "_id", os.str(), "label=\"Id\"");
	}
	else
	{
		XMLOstream::Tag txml0(xst, "_id", string("<null>"), "label=\"Id\"");
	}
	
	if(_inventoryNameSet)
	{
		XMLOstream::Tag txml1(xst, "_inventoryName", _inventoryName, "label=\"InventoryName\"");
	}
	else
	{
		XMLOstream::Tag txml1(xst, "_inventoryName", string("<null>"), "label=\"InventoryName\"");
	}
	{
		XMLOstream::Tag txml1Set(xst, "_inventoryNameSet", (_inventoryNameSet ? "true" : "false")) ;
	}
	
	if(_inventoryVersionSet)
	{
		XMLOstream::Tag txml2(xst, "_inventoryVersion", _inventoryVersion, "label=\"InventoryVersion\"");
	}
	else
	{
		XMLOstream::Tag txml2(xst, "_inventoryVersion", string("<null>"), "label=\"InventoryVersion\"");
	}
	{
		XMLOstream::Tag txml2Set(xst, "_inventoryVersionSet", (_inventoryVersionSet ? "true" : "false")) ;
	}
	
	if(_expirationDateSet)
	{
		XMLOstream::Tag txml3(xst, "_expirationDate","", "label=\"Expiration Date\"");
		_expirationDate.exportXML(xst) ;
	}
	else
	{
		XMLOstream::Tag txml3(xst, "_expirationDate", string("<null>"), "label=\"Expiration Date\"");
	}
	{
		XMLOstream::Tag txml3Set(xst, "_expirationDateSet", (_expirationDateSet ? "true" : "false")) ;
	}
	
	if(_actionSet)
	{
		XMLOstream::Tag txml4(xst, "_action",  db::toSQL("", _action), "label=\"Action\"");
	}
	else
	{
		XMLOstream::Tag txml4(xst, "_action", string("<null>"), "label=\"Action\"");
	}
	{
		XMLOstream::Tag txml4Set(xst, "_actionSet", (_actionSet ? "true" : "false")) ;
	}
	
	if(_storageSet)
	{
		XMLOstream::Tag txml5(xst, "_storage",  db::toSQL("", _storage), "label=\"StorageId\"");
	}
	else
	{
		XMLOstream::Tag txml5(xst, "_storage", string("<null>"), "label=\"StorageId\"");
	}
	{
		XMLOstream::Tag txml5Set(xst, "_storageSet", (_storageSet ? "true" : "false")) ;
	}
}



// load the object from a row
int dbInvSpecialRollingPolicies::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ;

	unsigned long id ;

	row.column(offset + 0, id) ;

	setId(id) ;
	
	row.columnSet(offset + 1, _inventoryName, _inventoryNameSet) ;
	row.columnSet(offset + 2, _inventoryVersion, _inventoryVersionSet) ;
	row.columnSet(offset + 3, _expirationDate, _expirationDateSet) ;
	row.columnSet(offset + 4, _action, _actionSet) ;
	row.columnSet(offset + 5, _storage, _storageSet) ;
	

	setFromDatabase() ;

	return offset + 6 ;
}


void dbInvSpecialRollingPolicies::setInventoryName(const string &v)
{
	Lock lock(*this) ;

	if (!_inventoryNameSet || _inventoryName != v ) 
	{
		_inventoryName = v ;
		_inventoryNameSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbInvSpecialRollingPolicies::getInventoryName(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _inventoryNameSet ;
	return _inventoryName ;
}


void dbInvSpecialRollingPolicies::unsetInventoryName()
{
	
	Lock lock(*this) ;
	if(_inventoryNameSet == true)
		setModifiedFlag();
	_inventoryNameSet = false;

}

void dbInvSpecialRollingPolicies::setInventoryVersion(const string &v)
{
	Lock lock(*this) ;

	if (!_inventoryVersionSet || _inventoryVersion != v ) 
	{
		_inventoryVersion = v ;
		_inventoryVersionSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbInvSpecialRollingPolicies::getInventoryVersion(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _inventoryVersionSet ;
	return _inventoryVersion ;
}


void dbInvSpecialRollingPolicies::unsetInventoryVersion()
{
	
	Lock lock(*this) ;
	if(_inventoryVersionSet == true)
		setModifiedFlag();
	_inventoryVersionSet = false;

}

void dbInvSpecialRollingPolicies::setExpirationDate(const DateTime & v)
{
	Lock lock(*this) ;

	if (!_expirationDateSet || _expirationDate != v ) 
	{
		_expirationDate = v ;
		_expirationDateSet = true ;
		setModifiedFlag() ;
	}
}

const DateTime & dbInvSpecialRollingPolicies::getExpirationDate(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _expirationDateSet ;
	return _expirationDate ;
}

void dbInvSpecialRollingPolicies::unsetExpirationDate()
{
	Lock lock(*this) ;
	if(_expirationDateSet == true)
		setModifiedFlag();
	_expirationDateSet = false;

}


void dbInvSpecialRollingPolicies::setAction(unsigned int v)
{
	Lock lock(*this) ;

	if (!_actionSet || _action != v ) 
	{
		_action = v ;
		_actionSet = true ;
		setModifiedFlag() ;
	}
}

unsigned int dbInvSpecialRollingPolicies::getAction(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _actionSet ;
	return _action ;
}

void dbInvSpecialRollingPolicies::unsetAction()
{
	Lock lock(*this) ;
	if(_actionSet == true)
		setModifiedFlag();
	_actionSet = false;

}

void dbInvSpecialRollingPolicies::setStorage(unsigned int v)
{
	Lock lock(*this) ;

	if (!_storageSet || _storage != v ) 
	{
		_storage = v ;
		_storageSet = true ;
		setModifiedFlag() ;
	}
}

unsigned int dbInvSpecialRollingPolicies::getStorage(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _storageSet ;
	return _storage ;
}

void dbInvSpecialRollingPolicies::unsetStorage()
{
	Lock lock(*this) ;
	if(_storageSet == true)
		setModifiedFlag();
	_storageSet = false;

}


// get the field names
string dbInvSpecialRollingPolicies::getFields()
{
	string s ;
	s += getTable() + "." + "Id, ";
	s += getTable() + "." + "InventoryName, ";
	s += getTable() + "." + "InventoryVersion, ";
	s += getTable() + "." + "ExpirationDate," ;
	s += getTable() + "." + "Action, ";
	s += getTable() + "." + "StorageId ";
	
	return s ;
}


// get the table name
string dbInvSpecialRollingPolicies::getTable()
{
	return "T_InvSpecialRollingPolicies" ;
}


// get the order by clause
string dbInvSpecialRollingPolicies::getOrderBy()
{
	return "" ;
}


void dbInvSpecialRollingPolicies::writeTo(ostream &o) const
{
	Lock lock(*this) ;
	string tmp ; 
	RollingPoliciesActions* action = RollingPoliciesActions::instance("");
	
	o
		 << "_id = " << (getId() ?   db::toSQL("", getId()) : string("<null>")) << endl 
		 << "_inventoryName = " << (_inventoryNameSet ? _inventoryName : string("<null>")) << endl 
		 << "_inventoryVersion = " << (_inventoryVersionSet ? _inventoryVersion : string("<null>")) << endl 
		 << "_expirationDate = " << (_expirationDateSet ?  db::toSQL("", _expirationDate.utcs(tmp)) : string("<null>")) << endl 
		 << "_action = " << (_actionSet ? action->name(_action) : string("<null>")) << endl 
		 << "_storage = " << (_storageSet ?  db::toSQL("", _storage) : string("<null>")) << endl ;
		 
}


void dbInvSpecialRollingPolicies::writeTo(exostream &o) const
{
	Lock lock(*this) ;
	string tmp ; 
	RollingPoliciesActions* action = RollingPoliciesActions::instance("");
	
	o
		<< "_id = " << (getId() ?   db::toSQL("", getId()) : string("<null>")) << endl 
		<< "_inventoryName = " << (_inventoryNameSet ? _inventoryName : string("<null>")) << endl 
		<< "_inventoryVersion = " << (_inventoryVersionSet ? _inventoryVersion : string("<null>")) << endl 
		<< "_expirationDate = " << (_expirationDateSet ?  db::toSQL("", _expirationDate.utcs(tmp)) : string("<null>")) << endl 
		<< "_action = " << (_actionSet ? action->name(_action) : string("<null>")) << endl 
		<< "_storage = " << (_storageSet ?  db::toSQL("", _storage) : string("<null>")) << endl ;
		 
}

_ACS_END_NAMESPACE
