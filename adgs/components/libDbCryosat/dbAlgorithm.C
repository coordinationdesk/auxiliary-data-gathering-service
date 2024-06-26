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
	Revision 6.1  2016/07/26 13:54:54  damdec
	Compilation warnings fixed.
	
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/04/19 13:53:46  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:52  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:20  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.9  2013/04/12 12:14:37  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.8  2013/03/29 19:51:28  marpas
	removing html related functions
	
	Revision 2.7  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.6  2012/11/21 10:50:05  marpas
	realignement to HEAD
	
	Revision 2.4  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2011/03/02 18:47:13  marpas
	serial 8 support
	
	Revision 2.2  2010/03/05 15:09:10  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.1  2008/09/23 13:16:08  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.16  2005/06/15 07:44:49  integrator
	multiple storage manager implemented
	
	Revision 1.15  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.14  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.13  2004/10/04 15:13:52  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.12  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.11  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.10  2004/05/03 14:55:33  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.9  2004/04/16 12:49:34  paoscu
	Bug fixed in Lock usage
	
	Revision 1.8  2004/04/13 16:57:08  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.7  2004/02/04 10:12:35  paoscu
	Library generated again. dbPersistent interface changed.
		

*/

// This file is automatically generated by dbGenerator tool

#include <dbAlgorithm.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbConv.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <sstream>
#include <XMLOstream.h>



using namespace acs ;
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbAlgorithm)

// constructor
dbAlgorithm::dbAlgorithm() :
	dbPersistent(),
	_algorithm(),
	_algorithmSet(),
	_description(),
	_descriptionSet()

{
	reset() ;
}


// copy constructor
dbAlgorithm::dbAlgorithm(const dbAlgorithm &o):
	dbPersistent(o),
	_algorithm(o._algorithm),
	_algorithmSet(o._algorithmSet),
	_description(o._description),
	_descriptionSet(o._descriptionSet)
{
}


dbAlgorithm::~dbAlgorithm() throw()
{
}



dbPersistent *dbAlgorithm::clone() const
{
	return new dbAlgorithm(*this) ;
}



// operator =
dbAlgorithm &dbAlgorithm::operator=(const dbAlgorithm &o)
{
	Lock lock(*this) ;

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_algorithm=o._algorithm;
		_algorithmSet=o._algorithmSet;
		_description=o._description;
		_descriptionSet=o._descriptionSet;
	}
	return *this ;
}


string dbAlgorithm::className() const
{
	return staticClassName() ;
}


string dbAlgorithm::staticClassName()
{
	return "dbAlgorithm" ;
}


// initialize the class
void dbAlgorithm::reset()
{
	Lock lock(*this) ;

	// reset dbPersistent
	dbPersistent::reset() ;
	_algorithm="" ;
	_algorithmSet=false ;
	_description="" ;
	_descriptionSet=false ;
}


// save the data in database
dbPersistent::IdType dbAlgorithm::save(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!isModified())
		return 0 ; 

	if (isFromDatabase()) 
	{
		InvalidUse e("Error: dbAlgorithm::save(). Use the method update() instead of save().") ;
		ACS_THROW(e) ;
	}
	if(!_algorithmSet) 
	{
		InvalidUse e("Mandatory field \"Algorithm\" must be set.") ;
		ACS_THROW(e) ;
	}
    const string &dbKey = conn.getKey() ;

	string queryString ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO " + getTable() + " (" ;
		queryString += "Algorithm" ;
		if(_descriptionSet)
			queryString += ", Description" ;
		queryString +=	") VALUES ( " ;
		queryString +=	db::toSQL(dbKey,_algorithm) ;
		if(_descriptionSet)
			queryString +=	", " +  db::toSQL(dbKey, _description) ;
		queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;

	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbAlgorithm::save") ;

	if (!query.execForStatus())
	{
		dbQuery::QueryExecuteException e(string("Query Execution error: Command : ")
		+ string(queryString)
		+ string(" Error : ")
		+ string(query.errorString())) ;
		ACS_THROW (e) ;
	}

	// record that the values come from the database
	setFromDatabase() ; 
	return 0 ;
}


// load the class from the database. Return true if object exist
bool dbAlgorithm::load(dbConnection &conn , IdType id)
{
	Lock lock(*this) ;

	dbAlgorithm tempRecord(*this); //store current record.
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


	reset() ; 

	string queryString ;
    const string &dbKey = conn.getKey() ;

    const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "SELECT " ;
		queryString +=	getFields() ;
		queryString += " FROM " + getTable() ;
		queryString += " WHERE Algorithm " ;
		if(tempRecord._algorithmSet)
			queryString +=	string("= ") +  db::toSQL(dbKey, tempRecord._algorithm) ;
		else
			queryString +=	" IS NULL" ;
		string getOrderByString(getOrderBy()) ;
		if(!getOrderByString.empty() )
		{
			string orderby= "ORDER BY";
			if(StringUtils::findNoCase(getOrderByString,orderby) != string::npos)
				queryString += getOrderByString ;
			else
				queryString += " ORDER BY " + getOrderByString ;
		}

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	dbQuery query(conn, sqlString, "dbAlgorithm::load") ;
	dbRow row(conn.getKey()) ;

	dbSet setQ;
	query.execForSet(setQ) ;
    if (setQ.size() > 1) // it should not occur
    {
        ACS_THROW(InvalidUse("Unexpected query result dimension: it should return just one row"));
    }
    else
    {
		if ( not setQ.getNextRow(row) )
        {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No " << getTable() << " record found ");
			*this = tempRecord;  //Nothing found: restore previuos situation.
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
dbPersistent::IdType dbAlgorithm::update(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!isFromDatabase())
	{
		ObjectNotLoaded ex("Error: dbAlgorithm::update(). Use the method save() instead of update().") ; 
		ACS_THROW(ex)
	}

	if (!isModified())
		return 0 ; 

	string queryString ;
    const string &dbKey = conn.getKey() ;

	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = string("UPDATE ") + getTable() + " SET " ; 
		queryString += "Algorithm= " +  db::toSQL(dbKey, _algorithm) ;
		if(_descriptionSet)
			queryString += ", Description= " +  db::toSQL(dbKey, _description) ;
		else
			queryString += ", Description=  NULL " ; 
		queryString += " WHERE " ;
		queryString += "Algorithm " ;
		if(_algorithmSet)
			queryString +=	string("= ") +  db::toSQL(dbKey, _algorithm) ;
		else
			queryString +=	"IS NULL" ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;

	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	dbQuery query(conn, sqlString, "dbAlgorithm::update") ;

	query.execForStatus() ;


	resetModifiedFlag() ;
	return 0 ;

}


// remove the class from the database
dbPersistent::IdType dbAlgorithm::remove(dbConnection &conn)
{
	Lock lock(*this) ;

	string queryString ;

    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE " ;
		queryString += "Algorithm " ;
		if(_algorithmSet)
			queryString +=	string("= ") +  db::toSQL(dbKey, _algorithm) ;
		else
			queryString +=	"IS NULL" ;

	} else {
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	dbQuery query(conn, sqlString, "dbAlgorithm::remove") ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return 0 ;
}


// True if the class have valid parameters, false otherwise
bool dbAlgorithm::validate() const
{
	Lock lock(*this) ;

	return true;
}


// export the class to stream
void dbAlgorithm::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbAlgorithm") ;
	dbPersistent::exportObj(s) ;
	if(_algorithmSet)
	{
		s.setValue("_algorithm",_algorithm) ;
	}
	s.setValue("_algorithmSet",_algorithmSet) ;
	if(_descriptionSet)
	{
		s.setValue("_description",_description) ;
	}
	s.setValue("_descriptionSet",_descriptionSet) ;
}


// import the class from a stream
void dbAlgorithm::importObj(rsResourceSet &s)
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbAlgorithm") ;
	dbPersistent::importObj(s) ;
	s.getValue("_algorithmSet",_algorithmSet) ;
	if(_algorithmSet)
	{
		s.getValue("_algorithm",_algorithm) ;
	}
	else
		_algorithm = "" ;
	s.getValue("_descriptionSet",_descriptionSet) ;
	if(_descriptionSet)
	{
		s.getValue("_description",_description) ;
	}
	else
		_description = "" ;
}


// export the class to XML stream
void dbAlgorithm::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ;

	XMLOstream::Tag txml(xst, "dbAlgorithm");
	dbPersistent::exportXML(xst) ;
	if(_algorithmSet)
	{
	XMLOstream::Tag txml0(xst, "_algorithm", _algorithm, "label=\"Algorithm\"");
	}
	else
	{
	XMLOstream::Tag txml0(xst, "_algorithm", string("<null>"), "label=\"Algorithm\"");
	}
	{
	XMLOstream::Tag txml0Set(xst, "_algorithmSet", (_algorithmSet ? "true" : "false")) ;
	}
	if(_descriptionSet)
	{
	XMLOstream::Tag txml1(xst, "_description", _description, "label=\"Description\"");
	}
	else
	{
	XMLOstream::Tag txml1(xst, "_description", string("<null>"), "label=\"Description\"");
	}
	{
	XMLOstream::Tag txml1Set(xst, "_descriptionSet", (_descriptionSet ? "true" : "false")) ;
	}
}


// load the object from a row
int dbAlgorithm::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ;

	row.columnSet(offset + 0, _algorithm, _algorithmSet) ;
	row.columnSet(offset + 1, _description, _descriptionSet) ;


	setFromDatabase() ;

	return offset + 2 ;
}


void dbAlgorithm::setAlgorithm(const string &v)
{
	Lock lock(*this) ;

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbAlgorithm::setAlgorithm(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if (v.size() > 32)
	{
		InvalidUse e("Error: dbAlgorithm::setAlgorithm(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_algorithmSet || _algorithm != v ) 
	{
		_algorithm = v ;
		_algorithmSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbAlgorithm::getAlgorithm(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _algorithmSet ;
	return _algorithm ;
}


void dbAlgorithm::unsetAlgorithm()
{
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbAlgorithm::unsetAlgorithm(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	Lock lock(*this) ;
	if(_algorithmSet == true)
		setModifiedFlag();
	_algorithmSet = false;

}
void dbAlgorithm::setDescription(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 64)
	{
		InvalidUse e("Error: dbAlgorithm::setDescription(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_descriptionSet || _description != v ) 
	{
		_description = v ;
		_descriptionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbAlgorithm::getDescription(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _descriptionSet ;
	return _description ;
}


void dbAlgorithm::unsetDescription()
{
	Lock lock(*this) ;
	if(_descriptionSet == true)
		setModifiedFlag();
	_descriptionSet = false;

}
// get the field names
string dbAlgorithm::getFields()
{
	string s ;
	s += getTable() + "." + "Algorithm, ";
	s += getTable() + "." + "Description" ;
	return s ;
}


// get the table name
string dbAlgorithm::getTable()
{
	return "T_Algorithm" ;
}


// get the order by clause
string dbAlgorithm::getOrderBy()
{
	return "" ;
}


void dbAlgorithm::writeTo(ostream &o) const
{
	Lock lock(*this) ;

	o
		 << "_algorithm = " << (_algorithmSet ? _algorithm : string("<null>")) << "\n" 
		 << "_description = " << (_descriptionSet ? _description : string("<null>")) << "\n"  ;
}


void dbAlgorithm::writeTo(exostream &o) const
{
	Lock lock(*this) ;

	o
		 << "_algorithm = " << (_algorithmSet ? _algorithm : string("<null>")) << "\n" 
		 << "_description = " << (_descriptionSet ? _description : string("<null>")) << "\n"  ;
}


