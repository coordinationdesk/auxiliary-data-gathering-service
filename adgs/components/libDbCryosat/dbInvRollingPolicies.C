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

	$Prod: A.C.S. Db Cosmo Library $

	$Id$

	$Author$

	$Log$
	Revision 6.3  2016/09/01 12:44:03  marfav
	Using plain dialect function to detect not null fields
	
	Revision 6.2  2016/09/01 12:19:41  marfav
	Boolean cast with postgres fixed in case context
	
	Revision 6.1  2016/08/03 13:26:46  marfav
	CSGACQ-94 using clobs in the proper way
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.6  2016/05/05 13:13:25  marpas
	improving maintenance adopting dbPersistent convenience macros
	coding best practice application in progress
	
	Revision 5.5  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/15 10:13:43  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
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
	
	Revision 3.0  2013/09/19 14:57:23  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.15  2013/05/14 19:07:42  marpas
	SQLDialectFactory::sqlDialect() method now requires db connection key
	
	Revision 2.14  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.13  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.12  2013/04/12 12:14:37  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.11  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.10  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.9  2012/04/12 16:43:57  marpas
	update of fileclass to null fixed
	
	Revision 2.8  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.7  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.6  2010/03/05 15:09:10  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.5  2008/10/10 09:31:13  ivafam
	Added storageid
	
	Revision 2.4  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.3  2008/09/12 17:24:03  ivafam
	Added storageid field
	
	Revision 2.2  2007/01/19 15:48:43  ivafam
	";" removed in query
	
	Revision 2.1  2006/05/16 12:53:37  marpas
	useless include removed
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.27  2006/01/20 10:19:55  enrcar
	EC:: define FORCE_DBEXPRESS_QUERY introduced to allow safer tests on dbexpress-ported queries
	
	Revision 1.26  2006/01/13 17:07:16  enrcar
	updated to dbexpress
	
	Revision 1.28  2006/01/11 08:54:36  marfav
	*** empty log message ***
	
	Revision 1.25  2006/01/05 17:19:06  enrcar
	IS NOT NULL replaced with dialect function into getfields()
	
	Revision 1.24  2005/06/15 07:44:50  integrator
	multiple storage manager implemented
	
	Revision 1.23  2005/05/31 14:21:56  integrator
	Html rendering improved (labels)
	
	Revision 1.22  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.21  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.20  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.19  2004/10/25 09:42:49  integrator
	Bug Fixed for Latency Hours header
	
	Revision 1.18  2004/10/18 15:18:51  ivafam
	Bug Fixed in save method
	
	Revision 1.17  2004/10/15 08:36:09  ivafam
	Added dbFileActionFactoryChain dbFileActionFactory dbInventoryFileActionFactory
	and bug fixed in dbInvRollingPolicies::fieldName
	
	Revision 1.16  2004/10/11 11:05:30  ivafam
	FileClass nullable
	
	Revision 1.15  2004/10/04 15:13:53  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.14  2004/10/04 14:04:26  ivafam
	After uploading on Db the file is unliked
	
	Revision 1.13  2004/09/30 09:30:59  ivafam
	unsetParametersFileName method added and bugs fixed
	
	Revision 1.12  2004/09/23 10:43:14  ivafam
	LatencyDays--->LatencyHours
	
	Revision 1.11  2004/09/22 14:58:39  ivafam
	If in _orderByString clause "ORDER BY" is present do not add clause
	
	Revision 1.10  2004/09/22 13:06:02  ivafam
	Added FileClass attribute
	
	Revision 1.9  2004/09/16 14:42:05  ivafam
	Work in progress
	
	Revision 1.8  2004/09/08 16:30:49  ivafam
	I/F changed
	
	Revision 1.7  2004/09/07 16:58:24  ivafam
	Now import set ID
	
	Revision 1.6  2004/09/07 15:37:07  ivafam
	Work in progress
	
	Revision 1.5  2004/09/06 16:13:04  ivafam
	Work in progress
	
	Revision 1.4  2004/09/06 10:09:06  ivafam
	Associator added
	
	Revision 1.3  2004/09/03 15:29:27  ivafam
	Work in progress
	
	Revision 1.2  2004/09/03 13:47:37  ivafam
	Work in progress
	
	
*/


#include <dbInvRollingPolicies.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <Transaction.h>
#include <RollingPoliciesAlgo.h>
#include <RollingPoliciesActions.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <sstream>
#include <XMLOstream.h>
#include <File.h>
#include <SQLDialectFactory.h> 


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInvRollingPolicies)

// constructor
dbInvRollingPolicies::dbInvRollingPolicies() :
	dbPersistent(),
	_fileType(),
	_fileTypeSet(),
	_latencyHours(),
	_latencyHoursSet(), 
	_removeEvenDistribution(), 
	_removeEvenDistributionSet(),
	_removeEvenUsed (),
	_removeEvenUsedSet (),
	_action(), 
	_actionSet(), 
	_algorithm(), 
	_algorithmSet(),
	_priority(),
	_prioritySet(),
	_fileClass(),
	_fileClassSet(),
	_parametersFileName(),
	_parametersFileNameSet(),
	_parameters(),
	_storage(), 
	_storageSet() 

{
}


// copy constructor
dbInvRollingPolicies::dbInvRollingPolicies(const dbInvRollingPolicies &o):
	dbPersistent(o),
	_fileType(o._fileType),
	_fileTypeSet(o._fileTypeSet),
	_latencyHours(o._latencyHours),
	_latencyHoursSet(o._latencyHoursSet), 
	_removeEvenDistribution(o._removeEvenDistribution), 
	_removeEvenDistributionSet(o._removeEvenDistributionSet),
	_removeEvenUsed (o._removeEvenUsed),
	_removeEvenUsedSet (o._removeEvenUsedSet),
	_action(o._action), 
	_actionSet(o._actionSet), 
	_algorithm(o._algorithm), 
	_algorithmSet(o._algorithmSet),
	_priority(o._priority),
	_prioritySet(o._prioritySet),
	_fileClass(o._fileClass),
	_fileClassSet(o._fileClassSet),
	_parametersFileName(o._parametersFileName),
	_parametersFileNameSet(o._parametersFileNameSet),
	_parameters(o._parameters),
	_storage(o._storage), 
	_storageSet(o._storageSet) 
{
}


dbInvRollingPolicies::~dbInvRollingPolicies() throw()
{
}



dbPersistent *dbInvRollingPolicies::clone() const
{
	return new dbInvRollingPolicies(*this) ;
}

// operator =
dbInvRollingPolicies &dbInvRollingPolicies::operator=(const dbInvRollingPolicies &o)
{
	Lock lock(*this) ; // PRQA S 3050

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_fileType = o._fileType ;
		_fileTypeSet = o._fileTypeSet ; 
		_latencyHours = o._latencyHours ;
		_latencyHoursSet = o._latencyHoursSet ; 
		_removeEvenDistribution = o._removeEvenDistribution ; 
		_removeEvenDistributionSet = o._removeEvenDistributionSet ;
		_removeEvenUsed  = o._removeEvenUsed ;
		_removeEvenUsedSet  = o._removeEvenUsedSet ;
		_action = o._action ; 
		_actionSet = o._actionSet ; 
		_algorithm = o._algorithm ;
		_algorithmSet = o._algorithmSet ;
		_priority = o._priority ;
		_prioritySet = o._prioritySet ;
		_parametersFileName = o._parametersFileName ;
		_parametersFileNameSet = o._parametersFileNameSet ;
		_parameters = o._parameters ;
		_fileClass = o._fileClass;
		_fileClassSet = o._fileClassSet;
		_storage = o._storage ; 
		_storageSet = o._storageSet ; 
		
	}
	return *this ;
}


string dbInvRollingPolicies::className() const
{
	return staticClassName() ;
}


string dbInvRollingPolicies::staticClassName()
{
	return "dbInvRollingPolicies" ;
}


// initialize the class
void dbInvRollingPolicies::reset()
{
	Lock lock(*this) ; // PRQA S 3050

	// reset dbPersistent
	dbPersistent::reset() ;
	_fileType="" ;
	_fileTypeSet=false ;
	_latencyHours=0 ;
	_latencyHoursSet=false ;
	_removeEvenDistribution=false;
	_removeEvenDistributionSet=false ;
	_removeEvenUsed=false;
	_removeEvenUsedSet=false ;
	_action=0 ;
	_actionSet=false ;
	_algorithm=0 ;
	_algorithmSet=false ;
	_priority=0 ;
	_prioritySet=false ;
	_parametersFileName = "";
	_parametersFileNameSet = false;
	_parameters = false;
	_fileClass="" ;
	_fileClassSet=false ;
	_storage=0 ;
	_storageSet=false ;
}


// save the data in database
dbPersistent::IdType dbInvRollingPolicies::save(dbConnection &conn) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	if (!isModified()) { return 0 ; }

	if (( 0 != getId()) || isFromDatabase()) 
	{
		ACS_THROW(InvalidUse("Use the method update() instead of save().")) ; // PRQA S 3081
	}
	
	if(!_fileTypeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"FileType\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_latencyHoursSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Latency Hours\" must be set.")) ; // PRQA S 3081
	}
	if(!_removeEvenDistributionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"RemoveEvenDistribution\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_removeEvenUsedSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"RemoveEvenUsed\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_actionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Action\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_algorithmSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Algorithm\" must be set.")) ; // PRQA S 3081
	}
	
	if(!_prioritySet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Priority\" must be set.")) ; // PRQA S 3081
	}
	
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInvRollingPolicies_SAVE") ;
	conn.addStateWrapper(*this) ;

	string queryString ;
	bool hasParam = false ;
	const string &dbKey = conn.getKey();
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "INSERT INTO " + getTable() + " (" ;
		queryString += "FileType" ;
		queryString += ", LatencyHours" ;
		queryString += ", RemoveEvenDistribution" ;
		queryString += ", RemoveEvenUsed" ;
		queryString += ", Action" ;
		queryString += ", Algorithm" ;
		if(_storageSet) {
			queryString += ", StorageId" ;
        }
		queryString += ", Priority" ;
		if(_fileClassSet) {
			queryString += ", FileClass" ;
        }
		queryString +=	") VALUES ( " ;
		queryString +=	db::toSQL(dbKey, _fileType) ;
		queryString +=	", " + db::toSQL(dbKey, _latencyHours) ;
		queryString +=	", " + db::toSQL(dbKey, _removeEvenDistribution) ;
		queryString +=	", " + db::toSQL(dbKey, _removeEvenUsed) ;
		queryString +=	", " + db::toSQL(dbKey, _action) ;
		queryString +=	", " + db::toSQL(dbKey, _algorithm) ;
		if(_storageSet) {
			queryString +=	", " + db::toSQL(dbKey, _storage) ;
        }

		hasParam=false ;
		if(_parametersFileNameSet)
		{
			hasParam = true;
		}

		queryString +=	", " + db::toSQL(dbKey, _priority) ;
		if(_fileClassSet) {
			queryString +=	", " + db::toSQL(dbKey, _fileClass) ;			
        }
		queryString +=	") " ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString) ;

	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;
	dbQuery query(conn, sqlString, "dbInvRollingPolicies::save") ;

	if (!query.execForStatus())
	{
		ACS_THROW(dbQuery::QueryExecuteException("Query Execution error: Command : " // PRQA S 3081
		+ queryString
		+ " Error : "
		+ query.errorString())) ;
	}

	// obtain the last serial just inserted
	setId(getLastSerial(conn, getTable()) ) ;


    if(hasParam)
    {
        SQLString qString(dbKey,  SQLString::LOBJ_UPLOAD ) ;
        qString.setColumnAndFileName( "Parameters", _parametersFileName );
        qString.addTableList( getTable() );
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(),"Id", dbParam::EQUAL_TO, db::toSQL(dbKey,getId())) ) ;
            qString.addANDWhereParameter(p) ;
        }

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << _parametersFileName) ;

	    dbQuery qu(conn, qString, "dbInvRollingPolicies::saveBlob") ;	

        qu.uploadClob();
    }

    transaction.commit() ;

	if(_parametersFileNameSet) {
		File::unlink(_parametersFileName);
    }

	_parameters = hasParam;
	_parametersFileNameSet = false;
	
	
	
	// record that the values come from the database
	setFromDatabase() ; 
	return getId() ;
}


// load the class from the database. Return true if object exist
bool dbInvRollingPolicies::load(dbConnection &conn , IdType id) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Called with Id: " << id) ;

	IdType Id=id;
	if(Id == 0) {
		Id=getId() ;
    }
	if(Id==0)
	{
		ACS_THROW(IdNotProvided("Cannot obtain \"Id\"")) ; // PRQA S 3081
	}
	
	dbInvRollingPolicies tempRecord(*this); //store current record.
	
	reset() ; 
	dbRow row(conn.getKey()) ;
	
	try
	{
		string queryString ;
	    const string &dbKey = conn.getKey();
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

		dbQuery query(conn, sqlString, "dbInvRollingPolicies::load") ;
		
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
		return false ;
	}

	loadRow(row, 0) ;

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	return true;
}


// update an existing instance in database
dbPersistent::IdType dbInvRollingPolicies::update(dbConnection &conn) // PRQA S 4020
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

	if(!_fileTypeSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"FileType\" must be set.")) ;  // PRQA S 3081
	}
	
	if(!_latencyHoursSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Latency Hours\" must be set.")) ;  // PRQA S 3081
	}
	if(!_removeEvenDistributionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"RemoveEvenDistribution\" must be set.")) ;  // PRQA S 3081
	}
	
	if(!_removeEvenUsedSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"RemoveEvenUsed\" must be set.")) ;  // PRQA S 3081
	}
	
	if(!_actionSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Action\" must be set.")) ;  // PRQA S 3081
	}
	
	if(!_algorithmSet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Algorithm\" must be set.")) ;  // PRQA S 3081
	}
	
	if(!_prioritySet) 
	{
		ACS_THROW(InvalidUse("Mandatory field \"Priority\" must be set.")) ;  // PRQA S 3081
	}
	
	bool hasParam = false ;
	string queryString ;
    const string &dbKey = conn.getKey();
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "UPDATE " + getTable() + " SET " ; 
		queryString += "FileType= " + db::toSQL(dbKey, _fileType) ;
		queryString += ", LatencyHours= " + db::toSQL(dbKey, _latencyHours) ;
		queryString += ", RemoveEvenDistribution= " + db::toSQL(dbKey, _removeEvenDistribution) ;
		queryString += ", RemoveEvenUsed= " + db::toSQL(dbKey, _removeEvenUsed) ;
		queryString += ", Action= " + db::toSQL(dbKey, _action) ;
		queryString += ", Algorithm= " + db::toSQL(dbKey, _algorithm) ;
		if(_storageSet) {
			queryString += ", StorageId= " + db::toSQL(dbKey, _storage) ;
        }
		else {
			queryString += ", StorageId = NULL ";
        }
		hasParam=false ;
		if(_parametersFileNameSet ) {
			hasParam=true ;
		}
		else if(!_parameters)
		{
			queryString +=	", Parameters = NULL " ;
			hasParam=false ;
		}
        else { /* nothing */ }

		queryString += ", Priority= " + db::toSQL(dbKey, _priority) ;
		queryString += ", FileClass= " + ( _fileClassSet ? db::toSQL(dbKey, _fileClass) : string("NULL")) ; // PRQA S 3081, 3380, 3385
            
		queryString += " WHERE " ;
		queryString += "Id = " + db::toSQL(dbKey, getId()) ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInvRollingPolicies_UPDATE") ;
	conn.addStateWrapper(*this) ;
	
	dbQuery query(conn, sqlString, "dbInvRollingPolicies::update") ;

	query.execForStatus() ;

    if (hasParam)
    {
        SQLString qString(dbKey,  SQLString::LOBJ_UPLOAD ) ;
        qString.setColumnAndFileName( "Parameters", _parametersFileName );
        qString.addTableList( getTable() );
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, getTable(), "Id", dbParam::EQUAL_TO, db::toSQL(dbKey, getId())))  ;
            qString.addANDWhereParameter(p) ;
        }

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << _parametersFileName) ;

	    dbQuery qu(conn, qString, "dbInvRollingPolicies::updateClob") ;	

        qu.uploadClob();
    }
        
 	transaction.commit() ;

	resetModifiedFlag() ;
	if(_parametersFileNameSet) {
		File::unlink(_parametersFileName);
    }
	
	_parameters=_parameters ? _parameters : hasParam; // PRQA S 3380
	
	return getId() ;

}


// remove the class from the database
dbPersistent::IdType dbInvRollingPolicies::remove(dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050

	string queryString ;

    const string &dbKey = conn.getKey();
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM " + getTable() ;
		queryString += " WHERE " ;
		queryString += "Id= " ;
		queryString +=	db::toSQL(dbKey,getId()) ;
	
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	dbQuery query(conn, sqlString, "dbInvRollingPolicies::remove") ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setModifiedFlag() ;

	return getId() ;
}


// True if the class have valid parameters, false otherwise
bool dbInvRollingPolicies::validate() const
{
	return true;
}

// export the class to stream
void dbInvRollingPolicies::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInvRollingPolicies") ;
	dbPersistent::exportObj(s) ;
	s.setValue("_id", getId()) ;
	
	if(_fileTypeSet)
	{
		s.setValue("_fileType",_fileType) ;
	}
	s.setValue("_fileTypeSet",_fileTypeSet) ;
	if(_latencyHoursSet)
	{
		s.setValue("_latencyHours",_latencyHours) ;
	}
	s.setValue("_latencyHoursSet",_latencyHoursSet) ;
	if(_removeEvenDistributionSet)
	{
		s.setValue("_removeEvenDistribution",_removeEvenDistribution) ;
	}
	s.setValue("_removeEvenDistributionSet",_removeEvenDistributionSet) ;
	if(_removeEvenUsedSet)
	{
		s.setValue("_removeEvenUsed",_removeEvenUsed) ;
	}
	s.setValue("_removeEvenUsedSet",_removeEvenUsedSet) ;
	if(_actionSet)
	{
		s.setValue("_action",_action) ;
	}
	s.setValue("_actionSet",_actionSet) ;
	if(_algorithmSet)
	{
		s.setValue("_algorithm",_algorithm) ;
	}
	s.setValue("_algorithmSet",_algorithmSet) ;
	
	if(_prioritySet)
	{
		s.setValue("_priority",_priority) ;
	}
	s.setValue("_prioritySet",_prioritySet) ;
	if(_fileClassSet)
	{
		s.setValue("_fileClass",_fileClass) ;
	}
	s.setValue("_fileClassSet",_fileClassSet) ;
	
	if(_storageSet)
	{
		s.setValue("_storage",_storage) ;
	}
	s.setValue("_storageSet",_storageSet) ;
	
	if(_parametersFileNameSet)
	{
		s.setValue("_parametersFileName",_parametersFileName) ;
	}
	s.setValue("_parametersFileNameSet",_parametersFileNameSet) ;
	s.setValue("_parameters",_parameters) ;
	
}


// import the class from a stream
void dbInvRollingPolicies::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInvRollingPolicies") ;
	dbPersistent::importObj(s) ;
	reset() ;
	
	IdType id = 0 ;
	s.getValue("_id", id) ;
	setId(id) ;
	
	s.getValue("_fileTypeSet",_fileTypeSet) ;
	if(_fileTypeSet)
	{
		s.getValue("_fileType",_fileType) ;
	}
	
	s.getValue("_latencyHoursSet",_latencyHoursSet) ;
	if(_latencyHoursSet)
	{
		s.getValue("_latencyHours",_latencyHours) ;
	}
	
	s.getValue("_removeEvenDistributionSet",_removeEvenDistributionSet) ;
	if(_removeEvenDistributionSet)
	{
		s.getValue("_removeEvenDistribution",_removeEvenDistribution) ;
	}
	
	s.getValue("_removeEvenUsedSet",_removeEvenUsedSet) ;
	if(_removeEvenUsedSet)
	{
		s.getValue("_removeEvenUsed",_removeEvenUsed) ;
	}
	
	s.getValue("_actionSet",_actionSet) ;
	if(_actionSet)
	{
		s.getValue("_action",_action) ;
	}
	
	s.getValue("_algorithmSet",_algorithmSet) ;
	if(_algorithmSet)
	{
		s.getValue("_algorithm",_algorithm) ;
	}
	
	s.getValue("_prioritySet",_prioritySet) ;
	if(_prioritySet)
	{
		s.getValue("_priority",_priority) ;
	}
	
	s.getValue("_parametersFileNameSet",_parametersFileNameSet) ;
	if(_parametersFileNameSet)
	{
		s.getValue("_parametersFileName",_parametersFileName) ;
	}
	
	s.getValue("_fileClassSet",_fileClassSet) ;
	if(_fileClassSet)
	{
		s.getValue("_fileClass",_fileClass) ;
	}
	
	s.getValue("_storageSet",_storageSet) ;
	if(_storageSet)
	{
		s.getValue("_storage",_storage) ;
	}
	
	s.getValue("_parameters",_parameters) ;
	
}


// export the class to XML stream
void dbInvRollingPolicies::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ; // PRQA S 3050

	XMLOstream::Tag txml(xst, "dbInvRollingPolicies");
	dbPersistent::exportXML(xst) ;

	if(0 != getId())
	{
		ostringstream os;
		os << getId();
		XMLOstream::Tag txml0(xst, "_id", os.str(), "label=\"Id\"");
	}
	else
	{
		XMLOstream::Tag txml0(xst, "_id", string("<null>"), "label=\"Id\""); // PRQA S 3081
	}
    XMLTagM(xst, _fileTypeSet, _fileType, "FileType") ;
    XMLTagM(xst, _latencyHoursSet, _latencyHours, "LatencyHours") ;
    XMLTagM(xst, _removeEvenDistributionSet, _removeEvenDistribution, "RemoveEvenDistribution") ;
    XMLTagM(xst, _removeEvenUsedSet, _removeEvenUsed, "RemoveEvenUsed") ;
    XMLTagM(xst, _actionSet, _actionSet, "Action") ;
    XMLTagM(xst, _algorithmSet, _algorithm, "Algorithm") ;
    XMLTagM(xst, _prioritySet, _priority, "Priority") ;
    XMLTagM(xst, _fileClassSet, _fileClass, "FileClass") ;
    XMLTagM(xst, _storageSet, _storage, "StorageId") ;
    XMLTagM(xst, _parametersFileNameSet, _parametersFileName, "Parameter FileName") ;
	{
		XMLOstream::Tag txml11Set(xst, "_parameters", (_parameters ? "true" : "false")) ; // PRQA S 3380
	}
}

// load the object from a row
int dbInvRollingPolicies::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ; // PRQA S 3050

	IdType id = 0 ;

	row.column(offset + 0, id) ; // PRQA S 3000, 3010

	setId(id) ;
	
	row.columnSet(offset + 1, _fileType, _fileTypeSet) ; // PRQA S 3000, 3010, 4400  L1
	row.columnSet(offset + 2, _latencyHours, _latencyHoursSet) ;
	row.columnSet(offset + 3, _removeEvenDistribution, _removeEvenDistributionSet) ;
	row.columnSet(offset + 4, _removeEvenUsed, _removeEvenUsedSet) ;
	row.columnSet(offset + 5, _action, _actionSet) ;
	row.columnSet(offset + 6, _algorithm, _algorithmSet) ;
	bool dummySet = false ;
	row.columnSet(offset + 7, _parameters, dummySet) ;
	row.columnSet(offset + 8, _priority, _prioritySet) ;
	row.columnSet(offset + 9, _fileClass, _fileClassSet) ;
	row.columnSet(offset + 10, _storage, _storageSet) ;

    // PRQA L:L1
	setFromDatabase() ;

	return offset + 11 ; // PRQA S 4400
}


void dbInvRollingPolicies::replaceParametersFile(const string &file, dbConnection &conn) // PRQA S 4211
{
	Lock lock(*this); // PRQA S 3050

	if (file.empty() ) 
	{
		ACS_THROW(ObjectNotLoaded("Invalid file name.")) ; // PRQA S 3081
	}
	
    const string &dbKey = conn.getKey() ;
    SQLString queryString(dbKey, SQLString::LOBJ_UPLOAD ) ;
    queryString.setColumnAndFileName( "Parameters", file );
    queryString.addTableList( getTable() );
    {
        dbQueryParameters p(dbKey) ;
        p.addParameter( dbParam(dbKey, getTable(), "Id", dbParam::EQUAL_TO, db::toSQL(dbKey, getId())) ) ;
        queryString.addANDWhereParameter(p) ;
    }

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Uploading file: " << file) ;

	dbQuery query(conn, queryString, "dbInvRollingPolicies::replaceParametersFile") ;	

    query.uploadClob();
	
}

// Save the file on disk
void dbInvRollingPolicies::saveParametersFile(const string &filename, dbConnection &conn) const
{
	Lock lock(*this); // PRQA S 3050

	string completePath ;
	if (not filename.empty()) {
		completePath = filename ;
    }
	else {
		completePath = _parametersFileName; 
    }
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "completePath: \"" << completePath  << '\"') ;

	if(isFromDatabase()) {
		
        const string &dbKey = conn.getKey() ;
		dbRow row(dbKey) ;

		string queryString ;
		const string dialectName = SQLDialectFactory::dialectName(dbKey);
		if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
			queryString = "SELECT count(id)" ;
			queryString += " FROM t_InvRollingPolicies" ;
			queryString += " WHERE Id = " ;
			queryString +=	db::toSQL(dbKey, getId()) ;
			queryString += " and parameters is NOT NULL" ;

			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString) ;
		} else {
			ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
		}
		SQLString sqlString(dbKey, SQLString::COMMAND) ;
		sqlString.forceSQLString(queryString) ;
		dbQuery query(conn, sqlString, "dbInvRollingPolicies::saveParametersFile") ;

		int idcounter = 0 ;
		bool counterset = false ;

        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 

		row.columnSet(0, idcounter,counterset) ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "count = " << idcounter) ;

		if(0 != idcounter)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "CLOB from table (t_InvRollingPolicies)") ;


            SQLString qString(dbKey, SQLString::LOBJ_DOWNLOAD ) ;
            qString.setColumnAndFileName( "Parameters", completePath );
            string table = "T_InvRollingPolicies" ;
            qString.addTableList( table );

            {
                dbQueryParameters p(dbKey) ;
                p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, getId())) ) ;
                qString.addANDWhereParameter(p) ;
            }


            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading file: " << completePath) ;
			dbQuery qd(conn, qString, "dbInvRollingPolicies::saveParametersFile") ;	

            try
            {
                qd.downloadClob();
            }
            catch(exception &x)
            {
                ACS_THROW(exIOException(x, "Couldn't download File from Clob.")) ; // PRQA S 3081
            }
			// change permissions allowing other users to override this file later.
			File::chmod(completePath,0664) ; // PRQA S 4400

        }
	}
	else
	{
		ACS_THROW(ObjectNotLoaded("Error: object not loaded")) ; // PRQA S 3081
	}
}


void dbInvRollingPolicies::setFileType(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_fileTypeSet || _fileType != v ) 
	{
		_fileType = v ;
		_fileTypeSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbInvRollingPolicies::getFileType(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _fileTypeSet ; }
	return _fileType ;
}


void dbInvRollingPolicies::unsetFileType()
{
	
	Lock lock(*this) ; // PRQA S 3050
	if(_fileTypeSet == true) { setModifiedFlag(); }
	_fileTypeSet = false;

}

void dbInvRollingPolicies::setLatencyHours(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_latencyHoursSet || (_latencyHours != v ) )
	{
		_latencyHours = v ;
		_latencyHoursSet = true ;
		setModifiedFlag() ;
	}
}

int dbInvRollingPolicies::getLatencyHours(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _latencyHoursSet ; }
	return _latencyHours ;
}

void dbInvRollingPolicies::unsetLatencyHours()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_latencyHoursSet == true) { setModifiedFlag(); }
	_latencyHoursSet = false;

}


void dbInvRollingPolicies::setRemoveEvenDistribution(bool v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_removeEvenDistributionSet || (_removeEvenDistribution != v ) )
	{
		_removeEvenDistribution = v ;
		_removeEvenDistributionSet = true ;
		setModifiedFlag() ;
	}
}

bool dbInvRollingPolicies::getRemoveEvenDistribution(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _removeEvenDistributionSet ; }
	return _removeEvenDistribution ;
}

void dbInvRollingPolicies::unsetRemoveEvenDistribution()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_removeEvenDistributionSet == true) { setModifiedFlag(); }
	_removeEvenDistributionSet = false;

}

void dbInvRollingPolicies::setRemoveEvenUsed(bool v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_removeEvenUsedSet || (_removeEvenUsed != v ) )
	{
		_removeEvenUsed = v ;
		_removeEvenUsedSet = true ;
		setModifiedFlag() ;
	}
}

bool dbInvRollingPolicies::getRemoveEvenUsed(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _removeEvenUsedSet ; }
	return _removeEvenUsed ;
}

void dbInvRollingPolicies::unsetRemoveEvenUsed()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_removeEvenUsedSet == true) { setModifiedFlag(); }
	_removeEvenUsedSet = false;

}

void dbInvRollingPolicies::setAction(unsigned int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_actionSet || (_action != v ) )
	{
		_action = v ;
		_actionSet = true ;
		setModifiedFlag() ;
	}
}

unsigned int dbInvRollingPolicies::getAction(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _actionSet ; }
	return _action ;
}

void dbInvRollingPolicies::unsetAction()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_actionSet == true) { setModifiedFlag(); }
	_actionSet = false;

}

void dbInvRollingPolicies::setAlgorithm(unsigned int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_algorithmSet || (_algorithm != v ) )
	{
		_algorithm = v ;
		_algorithmSet = true ;
		setModifiedFlag() ;
	}
}

unsigned int dbInvRollingPolicies::getAlgorithm(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _algorithmSet ; }
	return _algorithm ;
}

void dbInvRollingPolicies::unsetAlgorithm()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_algorithmSet == true) { setModifiedFlag(); }
	_algorithmSet = false;

}

void dbInvRollingPolicies::setPriority(int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_prioritySet || (_priority != v ) )
	{
		_priority = v ;
		_prioritySet = true ;
		setModifiedFlag() ;
	}
}

int dbInvRollingPolicies::getPriority(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _prioritySet ; }
	return _priority ;
}

void dbInvRollingPolicies::unsetPriority()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_prioritySet == true) { setModifiedFlag(); }
	_prioritySet = false;

}

void dbInvRollingPolicies::setFileClass(const string &v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_fileClassSet || (_fileClass != v ) )
	{
		_fileClass = v ;
		_fileClassSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbInvRollingPolicies::getFileClass(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _fileClassSet ; }
	return _fileClass ;
}


void dbInvRollingPolicies::unsetFileClass()
{
	
	Lock lock(*this) ; // PRQA S 3050
	if(_fileClassSet == true) { setModifiedFlag(); }
	_fileClassSet = false;

}

void dbInvRollingPolicies::setParametersFileName(const string & filename)
{
	Lock lock(*this); // PRQA S 3050

	_parametersFileName = filename ;
	_parametersFileNameSet = true ;
	setModifiedFlag() ;
}

bool dbInvRollingPolicies::hasParameters() const
{
	Lock lock(*this); // PRQA S 3050

	return _parameters;
}

void dbInvRollingPolicies::setParameters(bool param)
{
	Lock lock(*this); // PRQA S 3050

	_parameters =  param;
}

string dbInvRollingPolicies::getParametersFileName() const 
{
	Lock lock(*this); // PRQA S 3050
	
	return _parametersFileName;
}

void dbInvRollingPolicies::unsetParametersFileName()
{
	
	Lock lock(*this) ; // PRQA S 3050
	if(_parametersFileNameSet == true) { setModifiedFlag(); }
	_parametersFileNameSet = false;
	
}

void dbInvRollingPolicies::setStorage(unsigned int v)
{
	Lock lock(*this) ; // PRQA S 3050

	if (!_storageSet || (_storage != v ) )
	{
		_storage = v ;
		_storageSet = true ;
		setModifiedFlag() ;
	}
}

unsigned int dbInvRollingPolicies::getStorage(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) { *set = _storageSet ; }
	return _storage ;
}

void dbInvRollingPolicies::unsetStorage()
{
	Lock lock(*this) ; // PRQA S 3050
	if(_storageSet == true) { setModifiedFlag(); }
	_storageSet = false;

}

// get the field names
string dbInvRollingPolicies::getFields()
{
	string s ;
	s += getTable() + "." + "Id, ";
	s += getTable() + "." + "FileType, ";
	s += getTable() + "." + "LatencyHours, " ;
	s += getTable() + "." + "RemoveEvenDistribution, ";
	s += getTable() + "." + "RemoveEvenUsed, " ;
	s += getTable() + "." + "Action, ";
	s += getTable() + "." + "Algorithm, " ;
    s += SQLDialectFactory::sqlDialect("").function_ISNOTNULL(getTable()+"."+"Parameters") + ", ";
	s += getTable() + "." + "Priority," ;
	s += getTable() + "." + "FileClass," ;
	s += getTable() + "." + "StorageId" ;
	return s ;
}


// get the table name
string dbInvRollingPolicies::getTable()
{
	return "T_InvRollingPolicies" ;
}


// get the order by clause
string dbInvRollingPolicies::getOrderBy()
{
	return "" ;
}


void dbInvRollingPolicies::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050
	o << "_id = " << ((0 != getId()) ?  db::toSQL("",getId()) : string("<null>")) << "\n" ; // PRQA S 3081, 3380, 3385
    writeToM(o, _fileTypeSet, _fileType) ;
    writeToM(o, _latencyHoursSet, _latencyHours) ;
    writeToM(o, _removeEvenDistributionSet, _removeEvenDistribution) ;
    writeToM(o, _removeEvenUsedSet, _removeEvenUsed) ;
	o << "_action = " << (_actionSet ? RollingPoliciesActions::instance("")->name(_action) : string("<null>")) << "\n" ; // PRQA S 3081, 3380, 3385 2
	o << "_algorithm = " << (_algorithmSet ? RollingPoliciesAlgo::instance("")->name(_algorithm) : string("<null>")) << "\n" ;
    o << "_parameters = " << (_parameters ? "*" : " ") << "\n" ;// PRQA S 3380
    writeToM(o, _prioritySet, _prioritySet) ;
    writeToM(o, _fileClassSet, _fileClass) ;
    writeToM(o, _storageSet, _storage) ;
}


void dbInvRollingPolicies::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050
	o << "_id = " << ((0 != getId()) ?  db::toSQL("",getId()) : string("<null>")) << "\n" ; // PRQA S 3081, 3380, 3385
    writeToM(o, _fileTypeSet, _fileType) ;
    writeToM(o, _latencyHoursSet, _latencyHours) ;
    writeToM(o, _removeEvenDistributionSet, _removeEvenDistribution) ;
    writeToM(o, _removeEvenUsedSet, _removeEvenUsed) ;
	o << "_action = " << (_actionSet ? RollingPoliciesActions::instance("")->name(_action) : string("<null>")) << "\n" ; // PRQA S 3081, 3380, 3385 2
	o << "_algorithm = " << (_algorithmSet ? RollingPoliciesAlgo::instance("")->name(_algorithm) : string("<null>")) << "\n" ;
    o << "_parameters = " << (_parameters ? "*" : " ") << "\n" ;// PRQA S 3380
    writeToM(o, _prioritySet, _prioritySet) ;
    writeToM(o, _fileClassSet, _fileClass) ;
    writeToM(o, _storageSet, _storage) ;
}


_ACS_END_NAMESPACE
