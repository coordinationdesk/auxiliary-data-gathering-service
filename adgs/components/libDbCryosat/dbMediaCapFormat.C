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
	Revision 6.2  2016/07/26 14:21:50  damdec
	Compilation warnings fixed.
	
	Revision 6.1  2016/07/26 10:10:40  damdec
	Compilation warnings fixed
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
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
	
	Revision 4.1  2014/05/26 11:39:44  marpas
	dbConv casted to avoid problem with new libXMLResources templates for tags
	
	Revision 4.0  2013/09/30 12:31:00  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:28  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.9  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.8  2013/03/29 19:51:30  marpas
	removing html related functions
	
	Revision 2.7  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.6  2012/02/14 12:52:49  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.5  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.4  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.3  2008/09/23 13:16:08  crivig
	porting to postgres8
	
	Revision 2.2  2006/06/09 12:45:58  giucas
	Fixed syntax error in update query.
	
	Revision 2.1  2006/06/09 11:53:21  giucas
	Update method fixed.
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2005/08/31 11:50:28  marpas
	ace no longer used, includes are removed
	
	Revision 1.9  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.8  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.7  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.6  2004/10/04 15:13:53  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.5  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.4  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.3  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.2  2004/04/27 14:13:25  paoscu
	Bug removed.
	
	Revision 1.1  2004/04/19 15:56:40  paoscu
	dbMediaCapFormat added.
		

*/

// This file is automatically generated by dbGenerator tool

#include <dbMediaCapFormat.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbConv.h>
#include <Transaction.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbMediaCapFormat)

// constructor
dbMediaCapFormat::dbMediaCapFormat():
	dbPersistent(),
	_format() ,
	_formatSet(false) ,
	_mediaType() ,
	_mediaTypeSet(false) ,
	_multiplier() ,
	_multiplierSet(false)
{
	//reset() ;
}


// copy constructor
dbMediaCapFormat::dbMediaCapFormat(const dbMediaCapFormat &o):
	dbPersistent(o),
	_format(o._format) ,
	_formatSet(o._formatSet) ,
	_mediaType(o._mediaType) ,
	_mediaTypeSet(o._mediaTypeSet) ,
	_multiplier(o._multiplier) ,
	_multiplierSet(o._multiplierSet)
{
}


dbMediaCapFormat::~dbMediaCapFormat() throw()
{
}


// operator =
dbMediaCapFormat &dbMediaCapFormat::operator=(const dbMediaCapFormat &o)
{
	ThreadSafe::Lock guard(*this);

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_format=o._format;
		_formatSet=o._formatSet;
		_mediaType=o._mediaType;
		_mediaTypeSet=o._mediaTypeSet;
		_multiplier=o._multiplier;
		_multiplierSet=o._multiplierSet;
	}
	return *this ;
}


dbPersistent *dbMediaCapFormat::clone() const
{
	ThreadSafe::Lock guard(*this);

	return new dbMediaCapFormat(*this) ;
}


string dbMediaCapFormat::className() const
{
	return staticClassName() ;
}


string dbMediaCapFormat::staticClassName()
{
	return "dbMediaCapFormat" ;
}


// initialize the class
void dbMediaCapFormat::reset()
{
	ThreadSafe::Lock guard(*this);

	// reset dbPersistent
	dbPersistent::reset() ;
	_format="" ;
	_formatSet=false ;
	_mediaType="" ;
	_mediaTypeSet=false ;
	_multiplier=1.0 ;
	_multiplierSet=false ;
}


// save the data in database
dbPersistent::IdType dbMediaCapFormat::save(dbConnection &conn)
{
	ThreadSafe::Lock guard(*this);

	if (!isModified())
		return 0 ; 

	if (isFromDatabase()) 
	{
		InvalidUse e("Error: dbMediaCapFormat::save(). Use the method update() instead of save().") ;
		ACS_THROW(e) ;
	}

	if(!_formatSet) 
	{
		InvalidUse e("Mandatory field \"Format\" must be set.") ;
		ACS_THROW(e) ;
	}

	if(!_mediaTypeSet) 
	{
		InvalidUse e("Mandatory field \"Media Type\" must be set.") ;
		ACS_THROW(e) ;
	}

	if(!_multiplierSet) 
	{
		InvalidUse e("Mandatory field \"Multiplier\" must be set.") ;
		ACS_THROW(e) ;
	}

	db::Transaction transaction(conn) ;
	transaction.begin_work("dbMediaCapFormat_SAVE") ;
	conn.addStateWrapper(*this) ;


	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO T_MediaCapFormat (" ;
		queryString += "Format" ;
		queryString += ", MediaType" ;
		queryString += ", Multiplier" ;
		queryString +=	") VALUES ( " ;
		queryString +=	db::toSQL(dbKey, _format);
		queryString +=	string(", ") + db::toSQL(dbKey, _mediaType);
		queryString +=	string(", ") + db::toSQL(dbKey, _multiplier);
		queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString ) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbMediaCapFormat::save") ;

	if (!query.execForStatus())
	{
			dbQuery::QueryExecuteException e(string("Query Execution error: Command : ")
			+ string(queryString)
			+ string(" Error : ")
			+ string(query.errorString())) ;
			ACS_THROW (e) ;
	}
	transaction.commit() ;
	// record that the values come from the database
	setFromDatabase() ; 
	return 0 ;
}


// load the class from the database. Return true if object exist
bool dbMediaCapFormat::load(dbConnection &conn , IdType id)
{
	ThreadSafe::Lock guard(*this);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	dbMediaCapFormat tempRecord(*this); //store current record.


	reset() ; 


	string queryString ;
	const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {

		queryString = "SELECT " ;
		queryString +=	privateGetFields() ;
		queryString += " FROM T_MediaCapFormat" ;
		queryString += " WHERE Format " ;
		if(tempRecord._formatSet)
			queryString +=	string("= ") + db::toSQL(dbKey, tempRecord._format);
		else
			queryString +=	" IS NULL" ;
		if(tempRecord._mediaTypeSet)
			queryString += " AND MediaType = " + db::toSQL(dbKey, tempRecord._mediaType);
		else
			queryString += " AND MediaType IS NULL ";
		string getOrderByString(privateGetOrderBy()) ;
		if(!getOrderByString.empty() )
		{
			string orderby= "ORDER BY";
			if(StringUtils::findNoCase(getOrderByString,orderby) != string::npos)
				queryString += getOrderByString ;
			else
				queryString += " ORDER BY " + getOrderByString ;
		}

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString ) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbMediaCapFormat::load") ;

	dbRow row(dbKey) ;
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
			*this = tempRecord;  //Nothing found: restore previuos situation.
			return false ;
        }
	}

	privateLoadRow(row, 0) ;

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	return true;
}


// update an existing instance in database
dbPersistent::IdType dbMediaCapFormat::update(dbConnection &conn)
{
	ThreadSafe::Lock guard(*this);

	if (!isFromDatabase())
	{
		ObjectNotLoaded ex("Error: dbMediaCapFormat::update(). Use the method save() instead of update().") ; 
		ACS_THROW(ex)
	}

	if (!isModified())
		return 0 ; 

	if(!_formatSet) 
	{
		InvalidUse e("Mandatory field \"Format\" must be set.") ;
		ACS_THROW(e) ;
	}
	if(!_mediaTypeSet) 
	{
		InvalidUse e("Mandatory field \"Media Type\" must be set.") ;
		ACS_THROW(e) ;
	}
	if(!_multiplierSet) 
	{
		InvalidUse e("Mandatory field \"Multiplier\" must be set.") ;
		ACS_THROW(e) ;
	}


	db::Transaction transaction(conn) ;
	transaction.begin_work("dbMediaCapFormat_UPDATE") ;

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = string("UPDATE T_MediaCapFormat SET ") ; 
		queryString += "Multiplier= " + db::toSQL(dbKey, _multiplier) ;
		queryString += " WHERE " ;
		queryString += "Format " ;
		queryString +=	string("= ") + db::toSQL(dbKey, _format)  + " AND " ;
		queryString += "MediaType " ;
		queryString +=	string("= ") + db::toSQL(dbKey, _mediaType)   ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString ) ;

	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	conn.addStateWrapper(*this) ;
	dbQuery query(conn, sqlString, "dbMediaCapFormat::update") ;

	query.execForStatus() ;
	transaction.commit() ;

	resetModifiedFlag() ;
	return 0 ;

}


// remove the class from the database
dbPersistent::IdType dbMediaCapFormat::remove(dbConnection &conn)
{
	ThreadSafe::Lock guard(*this);

	db::Transaction transaction(conn) ;
	transaction.begin_work("dbMediaCapFormat_REMOVE") ;

	string queryString ;
    const string &dbKey = conn.getKey() ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {

		queryString = "DELETE FROM T_MediaCapFormat" ;
		queryString += " WHERE " ;
		queryString += "Format " ;
		if(_formatSet)
			queryString +=	string("= ") + db::toSQL(dbKey, _format)  + " AND " ;
		else
			queryString +=	"IS NULL AND" ;
		queryString += "MediaType " ;
		if(_mediaTypeSet)
			queryString +=	string("= ") + db::toSQL(dbKey, _mediaType)   ;
		else
			queryString +=	"IS NULL" ;

	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	conn.addStateWrapper(*this) ;
	dbQuery query(conn, sqlString, "dbMediaCapFormat::remove") ;

	query.execForStatus() ;
    transaction.commit() ;


	resetFromDatabase() ;
	setModifiedFlag() ;

	return 0 ;
}


// True if the class have valid parameters, false otherwise
bool dbMediaCapFormat::validate() const
{
	ThreadSafe::Lock guard(*this);

	return true;
}



// export the class to stream
void dbMediaCapFormat::exportObj(rsResourceSet &s) const
{
	ThreadSafe::Lock guard(*this);

	rsPushNSpace nsp(s,"dbMediaCapFormat") ;
	dbPersistent::exportObj(s) ;

	if(_formatSet)
		s.setValue("_format",_format) ;
	s.setValue("_formatSet",_formatSet) ;

	if(_mediaTypeSet)
		s.setValue("_mediaType",_mediaType) ;
	s.setValue("_mediaTypeSet",_mediaTypeSet) ;

	if(_multiplierSet)
		s.setValue("_multiplier",_multiplier) ;
	s.setValue("_multiplierSet",_multiplierSet) ;

}


// import the class from a stream
void dbMediaCapFormat::importObj(rsResourceSet &s)
{
	ThreadSafe::Lock guard(*this);

	rsPushNSpace nsp(s,"dbMediaCapFormat") ;
	dbPersistent::importObj(s) ;
	reset() ;
	s.getValue("_formatSet",_formatSet) ;
	if(_formatSet)
		s.getValue("_format",_format) ;

	s.getValue("_mediaTypeSet",_mediaTypeSet) ;
	if(_mediaTypeSet)
		s.getValue("_mediaType",_mediaType) ;

	s.getValue("_multiplierSet",_multiplierSet) ;
	if(_multiplierSet)
		s.getValue("_multiplier",_multiplier) ;

}


// export the class to XML stream
void dbMediaCapFormat::exportXML(XMLOstream &xst) const
{
	ThreadSafe::Lock guard(*this);

	XMLOstream::Tag txml(xst, "dbMediaCapFormat");
	dbPersistent::exportXML(xst) ;
	if(_formatSet)
	{
		XMLOstream::Tag txml0(xst, "_format", _format, "label=\"Format\"");
	}
	else
	{
		XMLOstream::Tag txml0(xst, "_format", string("<null>"), "label=\"Format\"");
	}
	{
		XMLOstream::Tag txml0Set(xst, "_formatSet", (_formatSet ? "true" : "false")) ;
	}

	if(_mediaTypeSet)
	{
		XMLOstream::Tag txml1(xst, "_mediaType", _mediaType, "label=\"Media Type\"");
	}
	else
	{
		XMLOstream::Tag txml1(xst, "_mediaType", string("<null>"), "label=\"Media Type\"");
	}
	{
		XMLOstream::Tag txml1Set(xst, "_mediaTypeSet", (_mediaTypeSet ? "true" : "false")) ;
	}

	if(_multiplierSet)
	{
		XMLOstream::Tag txml2(xst, "_multiplier", db::toSQL("", _multiplier), "label=\"Multiplier\"");
	}
	else
	{
		XMLOstream::Tag txml2(xst, "_multiplier", string("<null>"), "label=\"Multiplier\"");
	}
	{
		XMLOstream::Tag txml2Set(xst, "_multiplierSet", (_multiplierSet ? "true" : "false")) ;
	}

}



// load the object from a row
int dbMediaCapFormat::loadRow(const dbRow &row, int offset)
{
	ThreadSafe::Lock guard(*this);

	return privateLoadRow(row, offset) ;

}


// load the object from a row
int dbMediaCapFormat::privateLoadRow(const dbRow &row, int offset)
{
	ThreadSafe::Lock guard(*this);

	row.columnSet(offset + 0, _format, _formatSet) ;
	row.columnSet(offset + 1, _mediaType, _mediaTypeSet) ;
	row.columnSet(offset + 2, _multiplier, _multiplierSet) ;


	setFromDatabase() ;

	return offset + 3 ;
}


string dbMediaCapFormat::getWhere()
{
	string res ;
	return res ;
}


string dbMediaCapFormat::getIdName()
{
	return "" ;
}


void dbMediaCapFormat::setFormat(const string &v)
{
	ThreadSafe::Lock guard(*this);

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbMediaCapFormat::setFormat(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if (v.size() > 8)
	{
		InvalidUse e("Error: dbMediaCapFormat::setFormat(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_formatSet || _format != v ) 
	{
		_format = v ;
		_formatSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbMediaCapFormat::getFormat(bool *set) const
{
	ThreadSafe::Lock guard(*this);

	if (set)
		*set = _formatSet ;
	return _format ;
}


void dbMediaCapFormat::unsetFormat()
{
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbMediaCapFormat::unsetFormat(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	ThreadSafe::Lock guard(*this);
	if(_formatSet == true)
		setModifiedFlag();
	_formatSet = false;

}
void dbMediaCapFormat::setMediaType(const string &v)
{
	ThreadSafe::Lock guard(*this);

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbMediaCapFormat::setMediaType(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if (v.size() > 16)
	{
		InvalidUse e("Error: dbMediaCapFormat::setMediaType(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_mediaTypeSet || _mediaType != v ) 
	{
		_mediaType = v ;
		_mediaTypeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbMediaCapFormat::getMediaType(bool *set) const
{
	ThreadSafe::Lock guard(*this);

	if (set)
		*set = _mediaTypeSet ;
	return _mediaType ;
}


void dbMediaCapFormat::unsetMediaType()
{
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbMediaCapFormat::unsetMediaType(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	ThreadSafe::Lock guard(*this);
	if(_mediaTypeSet == true)
		setModifiedFlag();
	_mediaTypeSet = false;

}


void dbMediaCapFormat::setMultiplier(float v)
{
	ThreadSafe::Lock guard(*this);

	if (!_multiplierSet || _multiplier != v ) //For a float it doesn't make much sense
	{
		_multiplier = v ;
		_multiplierSet = true ;
		setModifiedFlag() ;
	}
}


float dbMediaCapFormat::getMultiplier(bool *set) const
{
	ThreadSafe::Lock guard(*this);

	if (set)
		*set = _multiplierSet ;
	return _multiplier ;
}


void dbMediaCapFormat::unsetMultiplier()
{
	ThreadSafe::Lock guard(*this);
	if(_multiplierSet == true)
		setModifiedFlag();
	_multiplierSet = false;

}
// get the field names
string dbMediaCapFormat::getFields()
{
	string s ;
	s += privateGetFields() ;
	return s ;
}


// get the field names
string dbMediaCapFormat::privateGetFields()
{
	string s ;
	s += "T_MediaCapFormat.Format, ";
	s += "T_MediaCapFormat.MediaType, ";
	s += "T_MediaCapFormat.Multiplier" ;
	return s ;
}


// get the table name
string dbMediaCapFormat::getTable()
{
	return "T_MediaCapFormat" ;
}


// get the order by clause
string dbMediaCapFormat::getOrderBy()
{
	string res ;
	res += privateGetOrderBy() ;
	return res ;
}


// get the order by clause
string dbMediaCapFormat::privateGetOrderBy()
{
	string res ;
	res +="Format , MediaType DESC" ;
	return res ;
}


void dbMediaCapFormat::writeTo(ostream &o) const
{
	ThreadSafe::Lock guard(*this);
    string null = "<null>" ;
	o
		 << "_format = " << (_formatSet ? _format : null) << '\n'  // PRQA S 3380 3
		 << "_mediaType = " << (_mediaTypeSet ? _mediaType : null) << '\n' 
		 << "_multiplier = " << (_multiplierSet ? db::toSQL("", _multiplier) : null) << '\n'  ; // PRQA S 3385
}


void dbMediaCapFormat::writeTo(exostream &o) const
{
	ThreadSafe::Lock guard(*this);
    string null = "<null>" ;
	o
		 << "_format = " << (_formatSet ? _format : null) << '\n'  // PRQA S 3380 3
		 << "_mediaType = " << (_mediaTypeSet ? _mediaType : null) << '\n' 
		 << "_multiplier = " << (_multiplierSet ? db::toSQL("", _multiplier) : null) << '\n'  ; // PRQA S 3385
}


_ACS_END_NAMESPACE
