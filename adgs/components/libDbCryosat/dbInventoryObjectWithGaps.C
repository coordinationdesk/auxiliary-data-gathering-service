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
	
	Revision 5.6  2016/07/04 15:09:02  damdec
	CSGACQ-82: new getFields interface adopted. deprecated execOneRow replaced.
	
	Revision 5.5  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2015/02/20 16:23:53  marfav
	Fixing export object in rs virtual operation signature
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:59  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.2  2013/09/26 19:32:52  marpas
	qa rules
	coding best practices
	compilation warnings removed
	using libException 5.x interface
	
	Revision 3.1  2013/09/24 17:36:10  marpas
	introducing libException 5.x interface
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.12  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.11  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.10  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.9  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.8  2010/04/23 15:24:28  marpas
	reset bug fixed
	export bugs fixed
	
	Revision 2.7  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.6  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.5  2008/07/14 13:15:30  marpas
	dbvalues not longer used here
	
	Revision 2.4  2007/03/16 12:28:59  marpas
	work in progress
	
	Revision 2.3  2006/10/23 13:33:58  marpas
	::Remove fixed
	
	Revision 2.2  2006/04/28 14:28:36  marpas
	loadComments & loadGaps now have a defaulted connection parameter to allow reloading while transaction is active
	
	Revision 2.1  2006/04/28 14:14:57  marpas
	Gaps & Comments amanagement improved.
	adding those elements to an inventory does not require this has been previously saved into the database.
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/01/10 18:24:21  marpas
	fieldname method improved according with the special query done with their respective queries
	
	Revision 1.2  2005/10/21 18:31:45  marpas
	useless include removed
	
	Revision 1.1  2005/06/20 10:36:11  marpas
	gaps management added
	

*/


#include <dbInventoryObjectWithGaps.h>
#include <dbInventoryGap.h>
#include <dbInventoryGapQuery.h>

#include <dbQueryFactoryChain.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryParameters.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbConv.h>
#include <Transaction.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ResourceSpace.h>
#include <File.h>
#include <XMLOstream.h>

#include <unistd.h>
#include <sstream>
#include <dbConnectionPool.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



//
//
//
// class dbInventoryObjectWithGaps
//
//
//

string dbInventoryObjectWithGaps::className() const
{
	return staticClassName() ;
}


string dbInventoryObjectWithGaps::staticClassName()
{
	return "dbInventoryObjectWithGaps" ;
}



dbPersistent *dbInventoryObjectWithGaps::clone() const
{
	return new dbInventoryObjectWithGaps(*this) ;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObjectWithGaps) 


// constructor
dbInventoryObjectWithGaps::dbInventoryObjectWithGaps() :
    dbInventoryObject(),
    _gapsNumSet(false),
    _gapsNum(0), 
    _gaps(0), 
    _gapsLoaded(false),
    _gaps2Save()
{
#warning MP: using empty string as db key
	_gaps=dynamic_cast<dbInventoryGapQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryGapQuery", "")); // PRQA S 3081 2
	ACS_COND_THROW(!_gaps, exIllegalValueException("dbInventoryGapQuery dynamic cast failed")) ;
	
	dbInventoryObjectWithGaps::reset() ;
}


// copy constructor
dbInventoryObjectWithGaps::dbInventoryObjectWithGaps(const dbInventoryObjectWithGaps &o):
	dbInventoryObject(o),
   	_gapsNumSet(o._gapsNumSet),
   	_gapsNum(o._gapsNum), 
	_gaps(0), 
	_gapsLoaded(false),
    _gaps2Save(o._gaps2Save)
{
#warning MP: using empty string as db key
	_gaps=dynamic_cast<dbInventoryGapQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryGapQuery", "")); // PRQA S 3081 2
	ACS_COND_THROW(!_gaps, exIllegalValueException("dbInventoryGapQuery dynamic cast failed")) ;
}


dbInventoryObjectWithGaps::~dbInventoryObjectWithGaps() throw() 
{
	delete _gaps;
    try {
	    deleteGaps2Save() ;
    }
    catch(std::exception &x) {
        excerr.notify(x) ;
    }
}


// operator =
dbInventoryObjectWithGaps &dbInventoryObjectWithGaps::operator=(const dbInventoryObjectWithGaps &o)
{
	Lock lock(*this) ; // PRQA S 3050

	if(this != &o)
	{
		dbInventoryObject::operator=(o) ;
		_gapsNum=o._gapsNum;
		_gapsNumSet=o._gapsNumSet;
		_gapsLoaded = false ; 
        _gaps2Save = o._gaps2Save ;
	}
	return *this ;
}



// load the class from the database. Return true if object exist
bool dbInventoryObjectWithGaps::load(dbConnection &conn, IdType id) // PRQA S 4020
{
	Lock lock(*this) ; // PRQA S 3050

    const string &dbKey=conn.getKey();

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called") ;

    IdType Id = id ;
    if (hasSerialId(conn)) {
	    if(Id == 0) {
		    Id=getId() ;
        }
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called with id=" << id << " getId():" << getId() << " Id:" << Id) ;
    }

	dbInventoryObjectWithGaps tempRecord(*this); //store current record.

	string oldFileName = getFileName() ;
	string oldFileVersion = getFileVersion() ;

	reset() ; 

    SQLString queryString(dbKey, SQLString::SELECT) ;
	deque<string> fields = getFieldArray(conn);
	for (uint i = 0; i < fields.size(); ++i)
	{
    	queryString.addSelectList( fields[i] );
	}

    queryString.addTableList( getTable() ) ;

    if (hasSerialId(conn) && (0 != Id)) {  
        dbQueryParameters pars(dbKey) ;
        pars.addParameter(dbParam(dbKey, getTable(),getIdColumnName(),dbParam::EQUAL_TO,db::toSQL(dbKey, Id))) ;
        queryString.addANDWhereParameter(pars) ;
    }
    else { 
        dbQueryParameters pars(dbKey) ;
        pars.addParameter(dbParam(dbKey, getTable(),"FileName",dbParam::EQUAL_TO,db::toSQL(dbKey, oldFileName))) ;
 	    if (oldFileVersion != "") {
            pars.addParameter(dbParam(dbKey, getTable(),"FileVersion",dbParam::EQUAL_TO,db::toSQL(dbKey, oldFileVersion))) ;
        }
        queryString.addANDWhereParameter(pars) ;
    }
    queryString.setOrderByString(getOrderBy()) ;

	dbQuery query(conn, queryString, "InventoryLoad") ;
	dbRow row(conn.getKey()) ;
	dbSet set;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

	try
	{
		query.execForSet(set);
        if (set.size() > 1) // it should not occur
        {
        	ACS_THROW(dbInventoryObjectWithGapsException("Unexpected query result dimension: it should return just one row"));
        }
		else
		{		    
			if ( not set.getNextRow(row) )
            {
        		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No t_inventory record found");
				*this = tempRecord;  //Nothing found: restore previous situation.
				return false ;
            }

		}
	}
	catch (exException &e)
	{
        ACS_LOG_ERROR("An error occurred loading record from Db.");
        ACS_LOG_NOTIFY_EX(e);
 		*this = tempRecord;  //Nothing found: restore previuos situation.
 		return false ;
		
	}

	loadRow(conn, row, 0) ;

	setFromDatabase() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	resetModifiedFlag() ;

	return true ;
}


// initialize the class
void dbInventoryObjectWithGaps::reset()
{
	Lock lock(*this) ; // PRQA S 3050

	// reset dbPersistent
	dbInventoryObject::reset() ;
	_gapsNum=0 ;
	_gapsNumSet=false ;
	// MP we suppose the pointer is never NULL
	_gaps->clear() ; 
	_gapsLoaded = false ; 
}

// save the data in database
dbPersistent::IdType dbInventoryObjectWithGaps::save(dbConnection &conn)
{	
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInventoryObjectWithGaps_SAVE") ;

    dbInventoryObject::save(conn) ;
    saveGaps(conn) ;
    loadGaps(conn) ;
    transaction.commit() ;
	
	return 0 ;
}


// remove the class from the database
dbPersistent::IdType dbInventoryObjectWithGaps::remove(dbConnection &conn )
{
	return dbInventoryObject::remove(conn) ;
}


// export the class to stream
void dbInventoryObjectWithGaps::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryObjectWithGaps") ;
	if(_gapsNumSet)
	{
		s.setValue("_gapsNum",_gapsNum) ;
	}
	s.setValue("_gapsNumSet",_gapsNumSet) ;
	dbInventoryObject::exportObj(s) ; 
}


// import the class from a stream
void dbInventoryObjectWithGaps::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryObjectWithGaps") ;
	s.getValue("_gapsNumSet",_gapsNumSet) ;
	if(_gapsNumSet)
	{
		s.getValue("_gapsNum",_gapsNum) ;
	}
	else {
		_gapsNum = 0 ;
    }
	dbInventoryObject::importObj(s) ;
}


// export the class to XML stream
void dbInventoryObjectWithGaps::exportXML(XMLOstream &xst)
{
	Lock lock(*this) ; // PRQA S 3050
    const string K = "" ;
	XMLOstream::Tag txml(xst, "dbInventoryObjectWithGaps");

	{ XMLOstream::Tag t(xst, "_gapsNum", _gapsNumSet? db::toSQL(K, _gapsNum) : "0", "label=\"Gaps Num\""); } // PRQA S 3380, 3385 2
	{ XMLOstream::Tag t(xst, "_gapsNumSet", (_gapsNumSet ? "true" : "false")) ; }

	dbInventoryObject::exportXML(xst) ; 
}



// load the object from a row
int dbInventoryObjectWithGaps::loadRow(dbConnection &conn, const dbRow &row, int offset)
{
    uint16_t loff = offset ; // PRQA S 3000, 3010
	try
	{
		row.column(loff++, _gapsNum) ;
		_gapsNumSet = true ;
	}
	catch (dbRow::NullValueException &)
	{
		_gapsNumSet = false ;
	}

	return dbInventoryObject::loadRow(conn, row, loff) ; // PRQA S 3000
}

// get the field names
deque<string> dbInventoryObjectWithGaps::getFieldArray(dbConnection &conn)
{
	deque<string> s = dbInventoryObject::getFieldArray(conn);
	string dbKey = conn.getKey();
//	string ns = "select count(*) from t_inventorygaps where t_inventory.filename = t_inventorygaps.filename and t_inventory.fileversion = t_inventorygaps.fileversion ) AS gapsnum" ; 
//	s += dbInventoryObject::getFields() ; 
	SQLString sqlString ( dbKey, SQLString::SELECT ) ;
	sqlString.addTableList("t_inventorygaps") ;
    sqlString.addSelectList( "count(*)" );
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "t_inventorygaps", "filename", dbParam::EQUAL_TO, "t_inventory.filename") ); // PRQA S 3050, 3081
	    pp.addParameter( dbParam(dbKey, "t_inventorygaps", "fileversion", dbParam::EQUAL_TO, "t_inventory.fileversion") ); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
	}
	s.push_front(sqlString.getSQL(true)+ "AS gapsnum");
	return s ;
}
string dbInventoryObjectWithGaps::getFields()
{
	return getFields(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
}

string dbInventoryObjectWithGaps::getFields(dbConnection& conn)
{
	deque <string> stringArray = getFieldArray(conn);
	return dbInventoryObject::convertFieldList (stringArray);
}




void dbInventoryObjectWithGaps::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_gapsNum = " << (_gapsNumSet ? _gapsNum : 0) << '\n' ; // PRQA S 3380
	dbInventoryObject::writeTo(o) ; 
}


void dbInventoryObjectWithGaps::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_gapsNum = " << (_gapsNumSet ? _gapsNum : 0) << '\n' ;  // PRQA S 3380
	dbInventoryObject::writeTo(o) ; 
}

unsigned long dbInventoryObjectWithGaps::loadGaps(dbConnection &conn)
{

	bool fileNameSet = false ; 
	string fileName = getFileName(&fileNameSet); 
    bool fileVersionSet = false ;
	string fileVersion= getFileVersion(&fileVersionSet); 
	
	if(!fileNameSet || !fileVersionSet)
	{
		InvalidUse e("fileNameSet and/or fileVersionSet not set") ;
		ACS_THROW(e) ;
	}
		
	_gaps->resetParameters() ; 
	_gaps->clear() ; 
	_gapsLoaded = false ; 
	
	const string dbKey = conn.getKey() ;
	dbQueryParameters par(dbKey) ; 
	
	dbParam fileNamePar(dbKey, "T_InventoryGaps","FileName", dbParam::EQUAL_TO , db::toSQL(dbKey, fileName)) ; 		
	dbParam fileVersionPar(dbKey, "T_InventoryGaps","FileVersion", dbParam::EQUAL_TO , db::toSQL(dbKey, fileVersion)) ; 			
	
	par.addParameter(fileNamePar) ; 
	par.addParameter(fileVersionPar) ; 
	
	_gaps->addParameter(par) ;
	
	unsigned long ret = _gaps->doQuery(conn) ;  
	_gapsLoaded = true ; 
	return ret ;
}


void dbInventoryObjectWithGaps::saveGaps(dbConnection &conn) // PRQA S 4020
{
	if (_gaps2Save.empty()) {
		return ;
    }
	db::Transaction transaction(conn) ;
	transaction.begin_work("Gaps_SAVE") ;
    size_t g2s_size = _gaps2Save.size() ;
	for (size_t i=0; i < g2s_size; ++i) {
		_gaps2Save[i]->save(conn) ;
    }
	deleteGaps2Save() ;
	transaction.commit() ;
}

void dbInventoryObjectWithGaps::deleteGaps2Save() throw() 
{
    size_t g2s_size = _gaps2Save.size() ;
	for (size_t i=0; i < g2s_size; ++i) {
		delete _gaps2Save[i];
    }
	_gaps2Save.clear() ;
}

unsigned long dbInventoryObjectWithGaps::gapsNum(dbConnection &conn)
{
	if(!_gapsLoaded) {
		loadGaps(conn) ; 
    }
	return _gaps->size() ; 
}

dbInventoryGap * dbInventoryObjectWithGaps::gap(unsigned long ind, dbConnection &conn) 
{
	if(!_gapsLoaded) {
		loadGaps(conn) ; 
    }
	return  (*_gaps)[ind] ; 
}


void dbInventoryObjectWithGaps::addGap(const DateTime &begin, const DateTime &end, dbConnection &conn)
{
	dbInventoryGap *comm = dynamic_cast<dbInventoryGap*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryGap", conn.getKey())); // PRQA S 3081, 4412 2
	ACS_COND_THROW(!comm, exIllegalValueException("dbInventoryGap dynamic cast failed")) ;
	try {
		comm->setFileName(getFileName()) ; 
		comm->setFileVersion(getFileVersion()) ; 
		comm->setBegin(begin) ; 
		comm->setEnd(end) ; 
		if (isFromDatabase()) {
			comm->save(conn) ; 
			_gapsNum++ ; 
			loadGaps(conn) ;
		}
		else {
			_gaps2Save.push_back(comm) ;
		}
	}
	catch(...) {
		delete comm ;
		throw ;
	}
}

_ACS_END_NAMESPACE
