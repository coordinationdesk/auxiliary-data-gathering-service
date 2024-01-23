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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.12  2016/07/21 16:05:08  damdec
	CSGACQ-86: SQLString usage introduced in queries.
	
	Revision 5.11  2016/04/21 14:52:35  marpas
	last modification was only partially - completed
	qa warnings fixed
	
	Revision 5.10  2016/04/21 14:45:56  marpas
	convenience templates used
	stat label for download query changed
	test improved
	
	Revision 5.9  2016/04/19 13:11:36  marpas
	fieldName method removed
	
	Revision 5.8  2016/04/11 13:20:17  marpas
	removing useless methods and also deprecated ones
	
	Revision 5.7  2016/03/26 21:45:49  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.6  2016/03/14 13:16:53  marpas
	getting rid of deprecvated classes
	
	Revision 5.5  2014/02/11 11:10:03  marpas
	refresh method removed: base class implements it
	
	Revision 5.4  2014/02/07 18:38:59  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/07/22 17:46:09  marpas
	enforcing use of dbdbKey string - deprecting older methods
	
	Revision 5.2  2013/07/15 16:19:38  marpas
	compilation warnings fixed
	
	Revision 5.1  2013/07/12 09:00:07  marpas
	compilation warning about use of dbConv fixed
	
	Revision 5.0  2013/06/13 14:53:03  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.22  2013/05/14 18:57:48  marpas
	qa warnings
	
	Revision 2.21  2013/04/12 12:12:52  marpas
	SQLDialectFactory::dialectName() needs a db connection dbKey
	
	Revision 2.20  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.19  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.18  2013/01/22 17:37:06  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.17  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.13  2012/06/06 09:31:56  marfav
	code improved to remove compiler warnings
	
	Revision 2.12  2012/02/17 14:09:25  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.11  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.10  2011/10/20 13:51:10  lucio.pulvirenti
	Statistics improved
	
	Revision 2.9  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.8  2011/03/02 18:16:21  marpas
	serial 8 supported on load too
	
	Revision 2.7  2011/03/02 17:39:38  marpas
	support to serial 8 improved
	
	Revision 2.6  2011/03/02 17:29:14  marpas
	support of serial 8 implemented
	
	Revision 2.5  2010/03/08 11:18:53  enrcar
	Oracle compatibility added
	
	Revision 2.4  2009/07/07 16:50:49  marpas
	because double quotes used for postgres, tablename and columname are lowercase
	
	Revision 2.3  2009/07/07 16:37:08  marpas
	column names passed to SQL strings are lowercase
	
	Revision 2.2  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.1  2006/06/09 11:52:03  giucas
	Update method fixed
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.27  2006/01/20 10:07:49  enrcar
	EC:: define FORCE_DBEXPRESS_QUERY introduced to allow safer tests on dbexpress-ported queries
	
	Revision 1.26  2006/01/03 12:28:06  enrcar
	blob handling updated
	
	Revision 1.25  2005/11/04 17:09:38  enrcar
	bug fixed
	
	Revision 1.24  2005/11/02 17:51:19  marpas
	bug fixed in load method when instance is the empty ("") string
	
	Revision 1.23  2005/10/24 16:36:51  marpas
	DBEXPRESS compatibility fixed at file level
	
	Revision 1.22  2005/08/08 11:56:00  enrcar
	SQLString.h was required even if env different from DBEXPRESS -- FIXED
	
	Revision 1.21  2005/08/08 11:47:08  enrcar
	DBEXPRESS handling added
	
	Revision 1.20  2005/06/17 14:14:25  marpas
	std namespace use fixed
	
	Revision 1.19  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.18  2005/03/04 21:19:15  marpas
	refresh method bug fixed
	
	Revision 1.17  2004/07/13 14:45:18  marpas
	exDbPersistent is an abstract class
	
	Revision 1.16  2004/07/07 10:27:01  marpas
	dbPersistent I/F changed
	
	Revision 1.15  2004/05/03 14:14:40  marpas
	dbPersistent::remove I/F changed
	
	Revision 1.14  2004/04/13 14:10:59  marpas
	ACE dependencies removed
	
	Revision 1.13  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.12  2004/02/18 14:04:57  paoscu
	fixed: FILETOCLOB was used instead of FILETOBLOB.
	
	Revision 1.11  2004/02/04 10:11:27  paoscu
	dbPersistent interface chanched.
		

*/




#include <dbApplicationStatus.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <dbConnectionPool.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#include <unistd.h>
#include <sstream>
#include <XMLOstream.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbApplicationStatus)

// constructor
dbApplicationStatus::dbApplicationStatus():
	dbPersistent(),
	_application(),
	_applicationSet(),
	_instance(),
	_instanceSet(),
	_statusUploadFileName()

{
	dbApplicationStatus::reset() ;
}


// copy constructor
dbApplicationStatus::dbApplicationStatus(const dbApplicationStatus &o):
	dbPersistent(o),
	_application(o._application),
	_applicationSet(o._applicationSet),
	_instance(o._instance),
	_instanceSet(o._instanceSet),
	_statusUploadFileName(o._statusUploadFileName)
{
}


dbApplicationStatus::~dbApplicationStatus() throw() {}


// operator =
dbApplicationStatus &dbApplicationStatus::operator=(const dbApplicationStatus &o)
{
	Lock lock(*this); // PRQA S 3050

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_application=o._application;
		_applicationSet=o._applicationSet;
		_instance=o._instance;
		_instanceSet=o._instanceSet;
		_statusUploadFileName=o._statusUploadFileName;
	}
	return *this ;
}


dbPersistent *dbApplicationStatus::clone() const
{
	Lock guard(*this); // PRQA S 3050

	return new dbApplicationStatus(*this) ;
}




std::string dbApplicationStatus::className() const
{
	return staticClassName() ;
}


std::string dbApplicationStatus::staticClassName()
{
	return "dbApplicationStatus" ;
}


// initialize the class
void dbApplicationStatus::reset()
{
	Lock lock(*this); // PRQA S 3050

	// reset dbPersistent
	dbPersistent::reset() ;
	_application="" ;
	_applicationSet=false ;
	_instance="" ;
	_instanceSet=false ;
	_statusUploadFileName="" ;
}


// save the data in database
dbPersistent::IdType dbApplicationStatus::save(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this); // PRQA S 3050

	if (!isModified()) { return 0 ; }
    
	ACS_COND_THROW(isFromDatabase(),InvalidUse("Error: dbApplicationStatus::save(). Use the method update() instead of save().")) ; // PRQA S 3081
	ACS_COND_THROW(!_applicationSet,InvalidUse("Mandatory field \"Application\" must be set.")) ; // PRQA S 3081
	ACS_COND_THROW(!_instanceSet, InvalidUse("Mandatory field \"Instance\" must be set.")) ; // PRQA S 3081

	std::string uploadfile = _statusUploadFileName == "" ? getStatusFileName() : _statusUploadFileName ; // PRQA S 3384
    const string &dbKey = conn.getKey() ; 

	SQLString queryString( dbKey, SQLString::INSERT ) ;
	queryString.addTableList ( getTable() );

	queryString.addValuesList("Application", db::toSQL(dbKey, _application) ) ;
	queryString.addValuesList("Instance", db::toSQL(dbKey, _instance) ) ;

	dbQuery query(conn, queryString, "INS " + getTable()) ;	

	if (!query.execForStatus())
	{
		dbQuery::QueryExecuteException e("Query Execution error: Command : "
	                                     + queryString.getSQL()
	                                     + " Error : "
	                                     + query.errorString()) ;
		ACS_THROW(e) ;
	}

    {
        SQLString queryString(dbKey, SQLString::LOBJ_UPLOAD ) ; // PRQA S 2502
        queryString.setColumnAndFileName( "status", uploadfile );
        queryString.addTableList( getTable() );
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(), "application" , dbParam::EQUAL_TO, db::toSQL(dbKey, _application)) ) ; // PRQA S 3050, 3081
            queryString.addANDWhereParameter(p) ;
        }
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(), "instance" , dbParam::EQUAL_TO, db::toSQL(dbKey, _instance)) ) ; // PRQA S 3050, 3081
            queryString.addANDWhereParameter(p) ;
        }
        

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << uploadfile) ;
    
        dbQuery query(conn, queryString, getTable() + " lob upld" ) ;	   // PRQA S 2502

        query.uploadBlob();
    }    

	// record that the values come from the database
	setFromDatabase() ; 
	return 0 ;
}


// load the class from the database. Return true if object exist
bool dbApplicationStatus::load(dbConnection &conn , IdType id) // PRQA S 4020
{
	Lock lock(*this); // PRQA S 3050

	dbApplicationStatus tempRecord(*this); //store current record.


	reset() ; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called") ;

	std::string queryString ;
    
    const string &dbKey = conn.getKey() ;

	SQLString sqlString ( dbKey, SQLString::SELECT, true ) ; // readOnly set to true
	sqlString.addTableList(getTable()) ;
    sqlString.addSelectList( getFields() );

	//add where
    if (tempRecord._applicationSet)
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, getTable(), "Application", dbParam::EQUAL_TO, db::toSQL(dbKey, tempRecord._application))); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
    }  
	else
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, getTable(), "Application", dbParam::ISNULL)); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
	
	}
	if(tempRecord._instanceSet) {
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, getTable(), "Instance", dbParam::EQUAL_TO, db::toSQL(dbKey, tempRecord._instance))); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
	}
    else {
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, getTable(), "Instance", dbParam::ISNULL)); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
    }
	
	sqlString.setOrderByString(getOrderBy());

	dbQuery query(conn, sqlString, getTable() + " load") ;
	dbRow row(dbKey) ;
	dbSet setQ ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;

	query.execForSet(setQ) ;
    if (setQ.size() > 1) // it should not occur
    {
        ACS_THROW(InvalidUse("Unexpected query result dimension: it should return just one row")); // PRQA S 3081
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
dbPersistent::IdType dbApplicationStatus::update(dbConnection &conn)
{
	// useless Lock lock(*this); // PRQA S 3050

	ACS_THROW(InvalidUse("No update foreseen on this record type")) ; // PRQA S 3081

}


// remove the class from the database
dbPersistent::IdType dbApplicationStatus::remove(dbConnection &conn)
{
	Lock lock(*this); // PRQA S 3050

	std::string queryString ;
    const string &dbKey = conn.getKey() ;
// 	const string dialectName = SQLDialectFactory::dialectName(dbKey);
// 	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
// 		queryString = "DELETE FROM " + getTable() ;
// 		queryString += " WHERE " ;
// 		queryString += "Application " ;
// 		if(_applicationSet) {
// 			queryString +=	"= " + db::toSQL(dbKey, _application) + " AND " ; // PRQA S 3050, 3081
// 		}
//         else {
// 			queryString +=	"IS NULL AND" ;
//         }
// 		queryString += "Instance " ;
// 		if(_instanceSet) {
// 			queryString +=	"= " + db::toSQL(dbKey, _instance) ; // PRQA S 3050, 3081
//         }
// 		else {
// 			queryString +=	"IS NULL" ;
//         }
// 
// 	} else {
// 		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
// 	}
// 	SQLString sqlString(dbKey, SQLString::COMMAND) ;
// 	sqlString.forceSQLString(queryString) ;

	SQLString sqlString ( dbKey, SQLString::DELETE ) ;
	sqlString.addTableList(getTable()) ;
	// Add where
    {
		dbQueryParameters pp(dbKey) ;
		if (_applicationSet)
		{
       		pp.addParameter(dbParam(dbKey, getTable(), "Application" , dbParam::EQUAL_TO , db::toSQL(dbKey, _application ))) ;
		}
		else
		{
       		pp.addParameter(dbParam(dbKey, getTable(), "Application" , dbParam::ISNULL)) ;
			
		}
		if(_instanceSet)
		{
       		pp.addParameter(dbParam(dbKey, getTable(), "Instance" , dbParam::EQUAL_TO , db::toSQL(dbKey, _instance ))) ;			
		}
		else
		{
       		pp.addParameter(dbParam(dbKey, getTable(), "Instance" , dbParam::ISNULL)) ;
		}
		
		sqlString.addANDWhereParameter(pp) ;
	}


	dbQuery query(conn, sqlString, getTable() + " remove") ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return 0 ;
}


// True if the class have valid parameters, false otherwise
bool dbApplicationStatus::validate() const
{
	// useless Lock lock(*this); // PRQA S 3050

	return true;
}

// export the class to stream
void dbApplicationStatus::exportObj(rsResourceSet &s) const
{
	Lock lock(*this); // PRQA S 3050

	rsPushNSpace nsp(s,"dbApplicationStatus") ;
	dbPersistent::exportObj(s) ;
    RSSetM(s, _applicationSet, _application) ;
    RSSetM(s, _instanceSet, _instance) ;
}


// import the class from a stream
void dbApplicationStatus::importObj(rsResourceSet &s)
{
	Lock lock(*this); // PRQA S 3050

	rsPushNSpace nsp(s,"dbApplicationStatus") ;
	dbPersistent::importObj(s) ;
    RSGetM(s, _applicationSet, _application) ;
    RSGetM(s, _instanceSet, _instance) ;
}


// export the class to XML stream
void dbApplicationStatus::exportXML(XMLOstream &xst) const
{
	Lock lock(*this); // PRQA S 3050

	XMLOstream::Tag txml(xst, "dbApplicationStatus");
	dbPersistent::exportXML(xst) ;
    XMLTagM(xst, _applicationSet, _application, "Application") ;
    XMLTagM(xst, _instanceSet, _instance, "Instance") ;
}


// load the object from a row
int dbApplicationStatus::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this); // PRQA S 3050

	row.columnSet(offset + 0, _application, _applicationSet) ; // PRQA S 3000, 3010 2
	row.columnSet(offset + 1, _instance, _instanceSet) ;


	setFromDatabase() ;

	return offset + 2 ; // PRQA S 4400
}



void dbApplicationStatus::setApplication(const string &v)
{
	Lock lock(*this); // PRQA S 3050

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbApplicationStatus::setApplication(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if (v.size() > 64) // PRQA S 4400
	{
		InvalidUse e("Error: dbApplicationStatus::setApplication(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_applicationSet || _application != v ) 
	{
		_application = v ;
		_applicationSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbApplicationStatus::getApplication(bool *set) const
{
	Lock lock(*this); // PRQA S 3050

	if (set) {
		*set = _applicationSet ;
    }
	return _application ;
}


void dbApplicationStatus::unsetApplication()
{
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbApplicationStatus::unsetApplication(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	Lock lock(*this); // PRQA S 3050
	if(_applicationSet == true) {
		setModifiedFlag();
    }
	_applicationSet = false;

}
void dbApplicationStatus::setInstance(const string &v)
{
	Lock lock(*this); // PRQA S 3050

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbApplicationStatus::setInstance(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if (v.size() > 8) // PRQA S 4400
	{
		InvalidUse e("Error: dbApplicationStatus::setInstance(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_instanceSet || _instance != v ) 
	{
		_instance = v ;
		_instanceSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbApplicationStatus::getInstance(bool *set) const
{
	Lock lock(*this); // PRQA S 3050

	if (set) {
		*set = _instanceSet ;
    }
	return _instance ;
}


void dbApplicationStatus::unsetInstance()
{
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbApplicationStatus::unsetInstance(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	Lock lock(*this); // PRQA S 3050
	if(_instanceSet == true) {
		setModifiedFlag();
    }
	_instanceSet = false;

}



void dbApplicationStatus::replaceStatusFile(const std::string &path, dbConnection &conn)  // PRQA S 4211 // PRQA S 4211
{
	Lock lock(*this); // PRQA S 3050

	if (!_applicationSet || !_instanceSet)
	{
		ObjectNotLoaded ex("Namespace or Version are not filled.") ;
		ACS_THROW(ex)
	}
	std::string uploadfile = path ;

	if (uploadfile.empty()) {
		uploadfile = _statusUploadFileName == "" ? getStatusFileName() : _statusUploadFileName ; // PRQA S 3380
    }

    const string dbKey = conn.getKey() ; 
    
    SQLString queryString(dbKey, SQLString::LOBJ_UPLOAD ) ;
    queryString.setColumnAndFileName( "status", uploadfile );
    queryString.addTableList( getTable() );
    {
        dbQueryParameters p(dbKey) ;
        p.addParameter( dbParam(dbKey, getTable(), "application" , dbParam::EQUAL_TO, db::toSQL(dbKey, _application)) ) ; // PRQA S 3050, 3081
        queryString.addANDWhereParameter(p) ;
    }
    {
        dbQueryParameters p(dbKey) ;
        p.addParameter( dbParam(dbKey, getTable(), "instance" , dbParam::EQUAL_TO, db::toSQL(dbKey, _instance)) ) ; // PRQA S 3050, 3081
        queryString.addANDWhereParameter(p) ;
    }   

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << uploadfile) ;

	dbQuery query(conn, queryString, getTable() + " lob upld") ;	

    query.uploadBlob();

}


// Save the file on disk
void dbApplicationStatus::saveStatusFile(const std::string &path, const std::string &filename, dbConnection &conn)  // PRQA S 4211
{
	Lock lock(*this); // PRQA S 3050

	std::string completePath ;
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
		completePath = getStatusFileName() ;
    }

	if(isFromDatabase()) {

        const string &dbKey = conn.getKey() ;
        
        SQLString queryString(dbKey, SQLString::LOBJ_DOWNLOAD ) ;
        queryString.setColumnAndFileName( "status", completePath );
        queryString.addTableList( getTable() ) ;

        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(), "application" , dbParam::EQUAL_TO, db::toSQL(dbKey, _application)) ) ; // PRQA S 3050, 3081
            queryString.addANDWhereParameter(p) ;
        }
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(), "instance" , dbParam::EQUAL_TO, db::toSQL(dbKey, _instance)) ) ; // PRQA S 3050, 3081
            queryString.addANDWhereParameter(p) ;
        }

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Download file = " << completePath) ;

		dbQuery query(conn, queryString, getTable() + " lob dwnl") ;	

        try {
            query.downloadBlob();
        }
        catch(exception &x) {
            ACS_THROW(exIOException(x,"Couldn't download File from Blob.")) ;
        }
        
    
    	// change permissions allowing other users to override this file later.
		File::chmod(completePath,0664) ; // PRQA S 4400

	}
	else
	{
		ObjectNotLoaded ex("Error: object not loaded") ;
		ACS_THROW(ex) ;
	}
}

// by definition: <namespace>_<version>.<ext>
std::string dbApplicationStatus::getStatusFileName() const 
{
	return _application + "_"+ _instance + ".conf" ;
}

void dbApplicationStatus::setStatusUploadFileName(const std::string &uf)
{
	Lock lock(*this); // PRQA S 3050

	_statusUploadFileName = uf ;
}

std::string dbApplicationStatus::getStatusUploadFileName() const
{
	Lock lock(*this); // PRQA S 3050

	return _statusUploadFileName ;
}


// get the field names
std::string dbApplicationStatus::getFields()
{
	std::string s ;
	s += getTable() + "." + "application, ";
	s += getTable() + "." + "instance" ;
	return s ;
}


// get the table name
std::string dbApplicationStatus::getTable()
{
	return "t_applicationstatus" ;
}


// get the order by clause
std::string dbApplicationStatus::getOrderBy()
{
	return "Application, Instance DESC" ;
}


void dbApplicationStatus::writeTo(ostream &o) const
{
	Lock lock(*this); // PRQA S 3050
    writeToM(o, _applicationSet, _application) ;
    writeToM(o, _instanceSet, _instance) ;
}


void dbApplicationStatus::writeTo(exostream &o) const
{
	Lock lock(*this); // PRQA S 3050
    writeToM(o, _applicationSet, _application) ;
    writeToM(o, _instanceSet, _instance) ;
}



_ACS_END_NAMESPACE
