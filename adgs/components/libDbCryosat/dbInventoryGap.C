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
	Revision 6.2  2016/07/26 15:59:44  damdec
	compilation warnings fixed
	
	Revision 6.1  2016/07/26 13:57:33  damdec
	CSGACQ-86: porting to SQLString.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:57  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:25  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.14  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.13  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.12  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.11  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.10  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.9  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.7  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.6  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.5  2010/03/05 15:09:10  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.4  2008/11/14 18:21:10  ivafam
	Bug Fixed: now lower case for postgres
	
	Revision 2.3  2008/09/22 09:26:20  crivig
	begin, end fields handled for postgres8
	
	Revision 2.2  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.1  2006/04/28 14:14:57  marpas
	Gaps & Comments amanagement improved.
	adding those elements to an inventory does not require this has been previously saved into the database.
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/20 08:16:07  marpas
	id management added
	
	Revision 1.1  2005/06/17 16:37:49  marpas
	Inventory gaps concepts introduced
	
		

*/

#include <dbInventoryGap.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <dbSet.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <sstream>
#include <XMLOstream.h>



using namespace acs ;
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryGap)

// constructor
dbInventoryGap::dbInventoryGap() :
	dbPersistent(),
	_begin(),
	_beginSet(false),
	_end(),
	_endSet(false),
	_fileName(),
	_fileNameSet(false),
	_fileVersion(),
	_fileVersionSet(false)
{
	//reset() ;
}


// copy constructor
dbInventoryGap::dbInventoryGap(const dbInventoryGap &o):
	dbPersistent(o),
	_begin(o._begin),
	_beginSet(o._beginSet),
	_end(o._end),
	_endSet(o._endSet),
	_fileName(o._fileName),
	_fileNameSet(o._fileNameSet),
	_fileVersion(o._fileVersion),
	_fileVersionSet(o._fileVersionSet)
{
}


dbInventoryGap::~dbInventoryGap() throw()
{
}



dbPersistent *dbInventoryGap::clone() const
{
	return new dbInventoryGap(*this) ;
}


// operator =
dbInventoryGap &dbInventoryGap::operator=(const dbInventoryGap &o)
{
	Lock lock(*this) ;

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_begin=o._begin;
		_beginSet=o._beginSet;
		_end=o._end;
		_endSet=o._endSet;
		_fileName=o._fileName;
		_fileNameSet=o._fileNameSet;
		_fileVersion=o._fileVersion;
		_fileVersionSet=o._fileVersionSet;
	}
	return *this ;
}


string dbInventoryGap::className() const
{
	return staticClassName() ;
}


string dbInventoryGap::staticClassName()
{
	return "dbInventoryGap" ;
}


// initialize the class
void dbInventoryGap::reset()
{
	Lock lock(*this) ;

	// reset dbPersistent
	dbPersistent::reset() ;
	_begin.reset() ;
	_beginSet=false ;
	_end.reset() ;
	_endSet=false ;
	_fileName="" ;
	_fileNameSet=false ;
	_fileVersion="" ;
	_fileVersionSet=false ;
}


// save the data in database
dbPersistent::IdType dbInventoryGap::save(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!isModified())
		return getId() ; 

	if (isFromDatabase()) 
	{
		InvalidUse e("Error: dbInventoryGap::save(). Use the method update() instead of save().") ;
		ACS_THROW(e) ;
	}
	if(!_beginSet) 
	{
		InvalidUse e("Mandatory field \"Begin\" must be set.") ;
		ACS_THROW(e) ;
	}
	if(!_endSet) 
	{
		InvalidUse e("Mandatory field \"End\" must be set.") ;
		ACS_THROW(e) ;
	}
	if(!_fileNameSet) 
	{
		InvalidUse e("Mandatory field \"File Name\" must be set.") ;
		ACS_THROW(e) ;
	}
	if(!_fileVersionSet) 
	{
		InvalidUse e("Mandatory field \"File Version\" must be set.") ;
		ACS_THROW(e) ;
	}

    const string &dbKey = conn.getKey() ;

	SQLString queryString(dbKey, SQLString::INSERT) ;

    queryString.addTableList(getTable()) ;
	queryString.addValuesList(getBeginField(), db::toSQL(dbKey, _begin,dbConv::YearToFraction)) ;
	queryString.addValuesList(getEndField(), db::toSQL(dbKey, _end,dbConv::YearToFraction)) ;
	queryString.addValuesList("FileName", db::toSQL(dbKey, _fileName)) ;
	queryString.addValuesList("FileVersion", db::toSQL(dbKey, _fileVersion)) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString.getSQL() ) ;
	dbQuery query(conn, queryString, "Insert") ;

	if (!query.execForStatus())
	{
			dbQuery::QueryExecuteException e(string("Query Execution error: Command : ")
			+ string(queryString.getSQL())
			+ string(" Error : ")
			+ string(query.errorString())) ;
			ACS_THROW (e) ;
	}

	// obtain the last serial just inserted
	setId(getLastSerial(conn, getTable())) ;

	// record that the values come from the database
	setFromDatabase() ; 
	return getId()  ;
}


// load the class from the database. Return true if object exist
bool dbInventoryGap::load(dbConnection &conn , IdType id)
{
	Lock lock(*this) ;

	unsigned long Id=id;
	if(Id == 0)
	{
		Id=getId() ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Called with id=" << id << " getId():" << getId() << " Id:" << Id) ;
	if(Id==0) 
	{
		IdNotProvided e("Cannot obtain \"Inventorygapid\"") ;
		ACS_THROW(e) ;
	}
	dbInventoryGap tempRecord(*this); //store current record.


	reset() ; 

    const string &dbKey = conn.getKey() ;
	SQLString sqlString ( dbKey, SQLString::SELECT ) ;
	sqlString.addTableList(getTable()) ;
    sqlString.addSelectList(getFields());
    {
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, getTable(), "Inventorygapid", dbParam::EQUAL_TO, db::toSQL(dbKey, Id))); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
    }  
	sqlString.setOrderByString(getOrderBy());

	dbQuery query(conn, sqlString, "Select_dbInventoryGap") ;
	dbRow row(conn.getKey()) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << sqlString.getSQL()) ;

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
dbPersistent::IdType dbInventoryGap::update(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!isFromDatabase())
	{
		ObjectNotLoaded ex("Error: dbInventoryGap::update(). Use the method save() instead of update().") ; 
		ACS_THROW(ex)
	}

	if (!isModified())
		return 0 ; 

	const string &dbKey = conn.getKey() ;
   	SQLString queryString(dbKey, SQLString::UPDATE) ;

   	queryString.addTableList( getTable()) ;
	queryString.addSetList(getBeginField(),  db::toSQL(dbKey, _begin,dbConv::YearToFraction)) ;
	queryString.addSetList(getEndField(),  db::toSQL(dbKey, _end,dbConv::YearToFraction)) ;
	queryString.addSetList("FileName",  db::toSQL(dbKey, _fileName)) ;
	queryString.addSetList("FileVersion",  db::toSQL(dbKey, _fileVersion)) ;
		
	{
		dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, getTable(), "InventoryGapId" , dbParam::EQUAL_TO , db::toSQL(dbKey, getId()) )) ;
		queryString.addANDWhereParameter(pp) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

	dbQuery query(conn, queryString, "Update_dbInventoryGap") ;

	query.execForStatus() ;

	resetModifiedFlag() ;
	return getId() ;

}


// remove the class from the database
dbPersistent::IdType dbInventoryGap::remove(dbConnection &conn)
{
	Lock lock(*this) ;

	const string &dbKey = conn.getKey() ;
	SQLString sqlString ( dbKey, SQLString::DELETE ) ;
	sqlString.addTableList(getTable()) ;
	{
		dbQueryParameters pp(dbKey) ;
   		pp.addParameter(dbParam(dbKey, getTable(), "InventoryGapId" , dbParam::EQUAL_TO , db::toSQL(dbKey, getId()) )) ;
		sqlString.addANDWhereParameter(pp) ;
	}

	dbQuery query(conn, sqlString, "Delete_dbInventoryGap") ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbInventoryGap::validate() const
{
	Lock lock(*this) ;

	return true;
}

// export the class to stream
void dbInventoryGap::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbInventoryGap") ;
	dbPersistent::exportObj(s) ;
	if(_beginSet)
	{
		{
			rsPushNSpace rpns1(s,"_begin") ;
			_begin.exportObj(s) ;
		}
	}
	s.setValue("_beginSet",_beginSet) ;
	if(_endSet)
	{
		{
			rsPushNSpace rpns1(s,"_end") ;
			_end.exportObj(s) ;
		}
	}
	s.setValue("_endSet",_endSet) ;
	if(_fileNameSet)
	{
		s.setValue("_fileName",_fileName) ;
	}
	s.setValue("_fileNameSet",_fileNameSet) ;
	if(_fileVersionSet)
	{
		s.setValue("_fileVersion",_fileVersion) ;
	}
	s.setValue("_fileVersionSet",_fileVersionSet) ;
}


// import the class from a stream
void dbInventoryGap::importObj(rsResourceSet &s)
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbInventoryGap") ;
	dbPersistent::importObj(s) ;
	s.getValue("_beginSet",_beginSet) ;
	if(_beginSet)
	{
		{
			rsPushNSpace rpns1(s,"_begin") ;
			_begin.importObj(s) ;
		}
	}
	else
		_begin.reset() ;
		
	s.getValue("_endSet",_endSet) ;
	if(_endSet)
	{
		{
			rsPushNSpace rpns1(s,"_end") ;
			_end.importObj(s) ;
		}
	}
	else
		_end.reset() ;
		
	s.getValue("_fileNameSet",_fileNameSet) ;
	if(_fileNameSet)
	{
		s.getValue("_fileName",_fileName) ;
	}
	else
		_fileName = "" ;
	s.getValue("_fileVersionSet",_fileVersionSet) ;
	if(_fileVersionSet)
	{
		s.getValue("_fileVersion",_fileVersion) ;
	}
	else
		_fileVersion = "" ;
}


// export the class to XML stream
void dbInventoryGap::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ;

	XMLOstream::Tag txml(xst, "dbInventoryGap");
	dbPersistent::exportXML(xst) ;
	if(_beginSet)
	{
		XMLOstream::Tag txml1(xst, "_begin", "", "label=\"Begin\"");
		_begin.exportXML(xst) ;
	}
	else
	{
		XMLOstream::Tag txml1(xst, "_begin", string("<null>"), "label=\"Begin\"");
	}
	{
		XMLOstream::Tag txml1Set(xst, "_beginSet", (_beginSet ? "true" : "false")) ;
	}
	
	if(_endSet)
	{
		XMLOstream::Tag txml1(xst, "_end", "", "label=\"Begin\"");
		_end.exportXML(xst) ;
	}
	else
	{
		XMLOstream::Tag txml1(xst, "_end", string("<null>"), "label=\"Begin\"");
	}
	{
		XMLOstream::Tag txml1Set(xst, "_endSet", (_endSet ? "true" : "false")) ;
	}
	
	if(_fileNameSet)
	{
		XMLOstream::Tag txml3(xst, "_fileName", _fileName, "label=\"File Name\"");
	}
	else
	{
		XMLOstream::Tag txml3(xst, "_fileName", string("<null>"), "label=\"File Name\"");
	}
	{
		XMLOstream::Tag txml3Set(xst, "_fileNameSet", (_fileNameSet ? "true" : "false")) ;
	}
	
	if(_fileVersionSet)
	{
		XMLOstream::Tag txml4(xst, "_fileVersion", _fileVersion, "label=\"File Version\"");
	}
	else
	{
		XMLOstream::Tag txml4(xst, "_fileVersion", string("<null>"), "label=\"File Version\"");
	}
	{
		XMLOstream::Tag txml4Set(xst, "_fileVersionSet", (_fileVersionSet ? "true" : "false")) ;
	}
}

// load the object from a row
int dbInventoryGap::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ;


	unsigned long id ;

	row.column(offset + 0, id) ;

	setId(id) ;

	row.columnSet(offset + 1, _begin, _beginSet) ;
	row.columnSet(offset + 2, _end, _endSet) ;
	row.columnSet(offset + 3, _fileName, _fileNameSet) ;
	row.columnSet(offset + 4, _fileVersion, _fileVersionSet) ;


	setFromDatabase() ;

	return offset + 4 ;
}



void dbInventoryGap::setGapId(dbPersistent::IdType v)
{
	Lock lock(*this) ;

	if (getId() != v ) 
	{
		setId(v) ;
		setModifiedFlag() ;
	}
}


dbPersistent::IdType dbInventoryGap::getGapId() const
{
	Lock lock(*this) ;

	return getId() ;
}




void dbInventoryGap::setBegin(const DateTime &v)
{
	Lock lock(*this) ;

	if (!_beginSet || _begin != v ) 
	{
		_begin = v ;
		_beginSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryGap::getBegin(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _beginSet ;
	return _begin ;
}


void dbInventoryGap::unsetBegin()
{
	Lock lock(*this) ;
	if(_beginSet == true)
		setModifiedFlag();
	_beginSet = false;

}



void dbInventoryGap::setEnd(const DateTime &v)
{
	Lock lock(*this) ;

	if (!_endSet || _end != v ) 
	{
		_end = v ;
		_endSet = true ;
		setModifiedFlag() ;
	}
}


const DateTime &dbInventoryGap::getEnd(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _endSet ;
	return _end ;
}


void dbInventoryGap::unsetEnd()
{
	Lock lock(*this) ;
	if(_endSet == true)
		setModifiedFlag();
	_endSet = false;

}



void dbInventoryGap::setFileName(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 128)
	{
		InvalidUse e("Error: dbInventoryGap::setFileName(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_fileNameSet || _fileName != v ) 
	{
		_fileName = v ;
		_fileNameSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryGap::getFileName(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _fileNameSet ;
	return _fileName ;
}


void dbInventoryGap::unsetFileName()
{
	Lock lock(*this) ;
	if(_fileNameSet == true)
		setModifiedFlag();
	_fileNameSet = false;

}
void dbInventoryGap::setFileVersion(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 8)
	{
		InvalidUse e("Error: dbInventoryGap::setFileVersion(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_fileVersionSet || _fileVersion != v ) 
	{
		_fileVersion = v ;
		_fileVersionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbInventoryGap::getFileVersion(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _fileVersionSet ;
	return _fileVersion ;
}


void dbInventoryGap::unsetFileVersion()
{
	Lock lock(*this) ;
	if(_fileVersionSet == true)
		setModifiedFlag();
	_fileVersionSet = false;

}

string dbInventoryGap::getBeginField()
{
#warning MP connection is neede for dialectName()
	const string dialectName = SQLDialectFactory::dialectName("");
	if( dialectName == "Postgres8" )
	{
		return "\"begin\"";
	} else if( dialectName == "Informix9" || dialectName == "Oracle11" ) 
	{
		return "Begin";
	} else 
	{
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
}

string dbInventoryGap::getEndField()
{
#warning MP connection is neede for dialectName()
	const string dialectName = SQLDialectFactory::dialectName("");
	if( dialectName == "Postgres8" )
	{
		return "\"end\"";
	} else if( dialectName == "Informix9" || dialectName == "Oracle11" ) 
	{
		return "End";
	} else 
	{
		ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialectName ));
	}
}


// get the field names
string dbInventoryGap::getFields()
{
	string s ;
	s += getTable() + "." + "Inventorygapid, ";
	s += getTable() + "." + getBeginField() + ", "; 
	s += getTable() + "." + getEndField() + ", "; 
	s += getTable() + "." + "FileName, ";
	s += getTable() + "." + "FileVersion" ;
	return s ;
}


// get the table name
string dbInventoryGap::getTable()
{
	return "T_InventoryGaps" ;
}


// get the order by clause
string dbInventoryGap::getOrderBy()
{
	return "Begin DESC" ;
}


void dbInventoryGap::writeTo(ostream &o) const
{
	Lock lock(*this) ;

	string tmp ; 

	o
		 << "_begin = " << (_beginSet ? _begin.utcs(tmp) : string("<null>")) << endl 
		 << "_end = " << (_endSet ? _end.utcs(tmp) : string("<null>")) << endl 
		 << "_fileName = " << (_fileNameSet ? _fileName : string("<null>")) << endl 
		 << "_fileVersion = " << (_fileVersionSet ? _fileVersion : string("<null>")) << endl  ;
}


void dbInventoryGap::writeTo(exostream &o) const
{
	Lock lock(*this) ;

	string tmp ; 

	o
		 << "_begin = " << (_beginSet ? _begin.utcs(tmp) : string("<null>")) << endl 
		 << "_end = " << (_endSet ? _end.utcs(tmp) : string("<null>")) << endl 
		 << "_fileName = " << (_fileNameSet ? _fileName : string("<null>")) << endl 
		 << "_fileVersion = " << (_fileVersionSet ? _fileVersion : string("<null>")) << endl  ;
}


