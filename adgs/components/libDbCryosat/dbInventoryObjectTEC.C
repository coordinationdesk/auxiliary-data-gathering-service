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
	Revision 6.4  2016/10/07 11:24:01  marpas
	fixing include files
	
	Revision 6.3  2016/09/23 14:32:08  marfav
	APF-391 Fixing baselines management
	
	Revision 6.2  2016/07/29 16:36:43  marfav
	CSGACQ-94 CLOB management fixed for inventoryextra table
	
	Revision 6.1  2016/07/15 10:40:25  nicvac
	CSGACQ-86: fixed getFieldArray call.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.9  2016/07/04 15:15:44  damdec
	CSGACQ-82: new getFields interface adopted.
	
	Revision 5.8  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.7  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.6  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.5  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.4  2015/03/13 17:51:19  marfav
	S2PDGS-1137 Added support to bulk extra parameters and quality value
	
	Revision 5.3  2015/02/26 16:56:32  marfav
	baseline operation conditioned by schema
	
	Revision 5.2  2015/02/26 16:50:00  marfav
	APF-256 baseline associated to TEC objects with external relation table
	
	Revision 5.1  2015/02/20 16:23:53  marfav
	Fixing export object in rs virtual operation signature
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:58  marpas
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
	
	Revision 2.18  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.17  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.16  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.14  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.13  2011/03/30 16:59:32  marpas
	tree management added
	
	Revision 2.12  2011/03/25 14:11:50  marpas
	tree management in progress
	
	Revision 2.11  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.10  2010/04/23 15:24:28  marpas
	reset bug fixed
	export bugs fixed
	
	Revision 2.9  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.8  2009/07/14 13:14:22  marfav
	Added workaround for --replacefile sites management
	to be fixed
	
	Revision 2.7  2009/07/08 10:33:24  marpas
	removeAllSitesLink method fixed
	
	Revision 2.6  2009/06/12 09:27:50  marpas
	removeAllSitesLinks method added
	
	Revision 2.5  2008/10/21 09:54:28  marpas
	wrong query fixed
	
	Revision 2.4  2008/10/20 14:05:46  marpas
	work in progress
	
	Revision 2.3  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.2  2007/03/21 17:18:20  marpas
	Html Page improved with MDS names
	
	Revision 2.1  2007/03/16 14:00:19  marpas
	work in progress
	
	

*/


#include <dbInventoryObjectTEC.h>
#include <dbInventoryMDS.h>
#include <dbInventoryMDSQuery.h>
#include <dbInventorySite.h>
#include <dbInventorySiteQuery.h>
#include <dbInventoryAddParams.h>
#include <dbInventoryAddParamsQuery.h>

#include <dbQueryFactoryChain.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <dbQueryParameters.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConv.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ResourceSpace.h>
#include <File.h>
#include <XMLOstream.h>
#include <ftmpstream.h>

#include <unistd.h>
#include <sstream>


// unnamed namespace
namespace {
    static const std::string baselineXTable ("t_baselines_x_inventories"); 
    static const std::string inventoryExtraTableName ("t_inventoryextra");
};


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


//
//
//
// class dbInventoryObjectTEC
//
//
//

string dbInventoryObjectTEC::className() const
{
	return staticClassName() ;
}


string dbInventoryObjectTEC::staticClassName()
{
	return "dbInventoryObjectTEC" ;
}



dbPersistent *dbInventoryObjectTEC::clone() const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	return new dbInventoryObjectTEC(*this) ;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObjectTEC) 

// constructor
dbInventoryObjectTEC::dbInventoryObjectTEC() :
    dbInventoryObjectWithGapsComments(),
    _MDSsNumSet(false),
    _MDSsNum(0), 
    _inventoryMDSs(0), 
    _MDSsLoaded(false),
    _SitesNumSet(false),
    _SitesNum(0), 
    _inventorySites(0), 
    _SitesLoaded(false),
    _inventoryAddParams(0), 
    _MDSs2Save(),
    _Sites2Save(),
    _addParams2Save(NULL),
    _addParamsSet(false),
    _baselineIds(),
    _baselineModified(false),
    _extraInfo(),
    _extraInfoSet(false)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
#warning MP: using empty string as db key
	_inventoryMDSs=dynamic_cast<dbInventoryMDSQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryMDSQuery", "")); // PRQA S 3081 4
	ACS_COND_THROW(!_inventoryMDSs, exIllegalValueException("dbInventoryMDSQuery creation failed")) ;
	_inventorySites=dynamic_cast<dbInventorySiteQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventorySiteQuery", ""));
	ACS_COND_THROW(!_inventorySites, exIllegalValueException("dbInventorySiteQuery creation failed")) ;
	_inventoryAddParams=dynamic_cast<dbInventoryAddParamsQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryAddParamsQuery", ""));
	ACS_COND_THROW(!_inventoryAddParams, exIllegalValueException("dbInventoryAddParamsQuery creation failed")) ;
	dbInventoryObjectTEC::reset() ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "finished") ;
}


// copy constructor
dbInventoryObjectTEC::dbInventoryObjectTEC(const dbInventoryObjectTEC &o):
	dbInventoryObjectWithGapsComments(o),
   	_MDSsNumSet(o._MDSsNumSet),
   	_MDSsNum(o._MDSsNum), 
	_inventoryMDSs(0), 
	_MDSsLoaded(false),
   	_SitesNumSet(o._SitesNumSet),
   	_SitesNum(o._SitesNum), 
	_inventorySites(0), 
	_SitesLoaded(false),
	_inventoryAddParams(0), 
	_MDSs2Save(o._MDSs2Save),
	_Sites2Save(o._Sites2Save),
	_addParams2Save(0),
	_addParamsSet(o._addParamsSet),
    _baselineIds(o._baselineIds),
    _baselineModified(o._baselineModified),
    _extraInfo(o._extraInfo),
    _extraInfoSet(o._extraInfoSet)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
#warning MP: using empty string as db key
	_inventoryMDSs=dynamic_cast<dbInventoryMDSQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryMDSQuery", "")); // PRQA S 3081 4
	ACS_COND_THROW(!_inventoryMDSs, exIllegalValueException("dbInventoryMDSQuery creation failed")) ;
	_inventorySites=dynamic_cast<dbInventorySiteQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventorySiteQuery", ""));
	ACS_COND_THROW(!_inventorySites, exIllegalValueException("dbInventorySiteQuery creation failed")) ;
	_inventoryAddParams=dynamic_cast<dbInventoryAddParamsQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryAddParamsQuery", ""));
	ACS_COND_THROW(!_inventoryAddParams, exIllegalValueException("dbInventoryAddParamsQuery creation failed")) ;

	if ( (NULL != o._addParams2Save) && (true == o._addParamsSet) )
	{
		_addParams2Save = static_cast<dbInventoryAddParams*>(o._addParams2Save->clone()) ; 
	}
	else
	{
		_addParams2Save = NULL ; _addParamsSet = false ; 
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "finished") ;
}

dbInventoryObjectTEC::~dbInventoryObjectTEC() throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	delete _inventoryMDSs;
    try {
	    deleteMDSs2Save() ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }
	delete _inventorySites;
    try {
	    deleteSites2Save() ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }
    if (NULL != _inventoryAddParams) { delete _inventoryAddParams ; }
    try {
	    deleteAddParams2Save() ; 
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "finished") ;
}


// operator =
dbInventoryObjectTEC &dbInventoryObjectTEC::operator=(const dbInventoryObjectTEC &o)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	Lock lock(*this) ; // PRQA S 3050
	Lock locko(o) ; // PRQA S 3050

	if(this != &o)
	{
		dbInventoryObjectWithGapsComments::operator=(o) ;
		_MDSsNum=o._MDSsNum;
		_MDSsNumSet=o._MDSsNumSet;
		_MDSsLoaded = false ; 
		_SitesNum=o._SitesNum;
		_SitesNumSet=o._SitesNumSet;
		_SitesLoaded = false ; 
	    _MDSs2Save = o._MDSs2Save ;
	    _Sites2Save = o._Sites2Save ;
		if ( (NULL != o._addParams2Save) && (true == o._addParamsSet) )
		{
			_addParams2Save = static_cast<dbInventoryAddParams*>(o._addParams2Save->clone()) ; 
		}
		else
		{
			_addParams2Save = NULL ; _addParamsSet = false ; 
		}
        _baselineIds = o._baselineIds;
        _baselineModified = o._baselineModified;
        _extraInfo = o._extraInfo;
        _extraInfoSet = o._extraInfoSet;
	}
	return *this ;
}



// load the class from the database. Return true if object exist
bool dbInventoryObjectTEC::load(dbConnection &conn, IdType id) // PRQA S 4020
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

	dbInventoryObjectTEC tempRecord(*this); //store current record.

	string oldFileName = getFileName() ;
	string oldFileVersion = getFileVersion() ;

	reset() ; 

    SQLString queryString(dbKey, SQLString::SELECT) ;
	deque<string> fields = dbInventoryObjectTEC::getFieldArray(conn);
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
	dbRow row(dbKey) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

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

	loadRow(conn, row, 0) ;

    if (dbInventoryObject::hasBaseline(conn))
    {
        loadBaselines (conn);
    }

    if (hasExtraInfo(conn))
    {
        loadExtraInfo(conn);
    }

	if (hasAddParams(conn))
	{
		loadAddParams(conn) ; 
	}

	setFromDatabase() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	resetModifiedFlag() ;

	return true ;
}


// initialize the class
void dbInventoryObjectTEC::reset()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	Lock lock(*this) ; // PRQA S 3050

	// reset dbPersistent
	dbInventoryObjectWithGapsComments::reset() ;
	_MDSsNum=0 ;
	_MDSsNumSet=false ;
	// MP we suppose the pointer is never NULL
	_inventoryMDSs->clear() ; 
	_MDSsLoaded = false ; 

	_SitesNum=0 ;
	_SitesNumSet=false ;
	// MP we suppose the pointer is never NULL
	_inventorySites->clear() ; 
	_SitesLoaded = false ; 

	deleteAddParams2Save() ; // Deallocate the AddParams container (pointer and boolean)
	if (NULL != _inventoryAddParams) { _inventoryAddParams->clear() ; }	// Clear the query

    resetBaselines();
}

// update an existing instance in database
dbPersistent ::IdType dbInventoryObjectTEC::update(dbConnection &conn) 
{
	// Thread safety
	Lock lock (*this) ; // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	// Discover if modified
	bool wasModified = this->isModified() ;

	// Start a transaction on the connection
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInventoryObjectTEC_UPDATE") ;

	try
	{
		dbInventoryObjectWithGapsComments::update(conn) ;

		// This is a workaround solution for the --replacefile inventory option
		// it will not work in a real usage of this method
		// The site cleanup is called from outside and the site list
		// is filled with the fromDatabase flag set to false
		// so the saveSites call will work
		// in any other case (real persistent from database) saveSites will do nothing
		// Maybe the MDS should be updated here too... 

		// Update the connected sites
		saveSites(conn) ;
		loadSites(conn) ;


        if (dbInventoryObject::hasBaseline(conn))
        {
            updateBaselines (conn);
        }

        // Update extra info in any case if the object has been modified
        if (hasExtraInfo(conn) && wasModified)
        {
            updateExtraInfo(conn);
        }
       
	    // Update additional parameters
        if (hasAddParams(conn))
        {
            updateAddParams(conn);
        }

		// Work completed!
		transaction.commit() ;
	}
	catch (exception&)
	{
		// Ooops... rollback and throw it away !!
		// Reset original modified flag (base update can reset it!!)
		if (wasModified)
		{
			setModifiedFlag();
		}
		throw ;
	}

	resetModifiedFlag() ;
	return 0 ;
}

// save the data in database
dbPersistent::IdType dbInventoryObjectTEC::save(dbConnection &conn)
{	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	// Start a transaction on the connection
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInventoryObjectTEC_SAVE") ;

	try {
        // preserve baselines and extra infos before executing the save (see comments below).
        vector<IdType> baselines = _baselineIds;
        bool extraInfoSet = _extraInfoSet;
        string extraInfo = _extraInfo;
        bool addParamsSet = _addParamsSet ;
        dbInventoryAddParams* addParams2Save = _addParams2Save ;
        _addParamsSet = false ; _addParams2Save = NULL ; // Disable any Additional Parameter 
		
		// The dbInventoryObjectWithGapsComments::save method below will also perform a dbInventoryObjectTEC::load [sic]. 
		// This explains why local variables are copied and saved: the load method will pollute the values.
		dbInventoryObjectWithGapsComments::save(conn) ;


        // restore old values for attributes
        _baselineIds = baselines;
        _extraInfo = extraInfo;
        _extraInfoSet = extraInfoSet;
        _addParams2Save = addParams2Save ;
        _addParamsSet = addParamsSet ;

        if (dbInventoryObject::hasBaseline(conn))
        {
           saveBaselines(conn);
        }
        if (hasExtraInfo(conn) )
        {
            saveExtraInfo(conn);
        }
		saveMDSs(conn) ;
		saveSites(conn) ;
		// Note that saveAddParams is called anyway, without checking hasAddParams
		// The reason is that if additional parameters ARE PROVIDED they should be saved
		// It the DB does not provide the proper table (so hasAddParams would be false),
		// an exception is expected.
		saveAddParams(conn) ;

		loadMDSs(conn) ;
		loadSites(conn) ;
		if (hasAddParams(conn) )
		{
			loadAddParams(conn) ; 
		}
	
        transaction.commit() ;
	}
	catch(...) {
		resetFromDatabase() ;
		throw ;
	}
	
	return getId() ;
}


// remove the class from the database
dbPersistent::IdType dbInventoryObjectTEC::remove(dbConnection &conn )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	return dbInventoryObjectWithGapsComments::remove (conn) ;
}


// export the class to stream
void dbInventoryObjectTEC::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	rsPushNSpace nsp(s,"dbInventoryObjectTEC") ;
	if(_MDSsNumSet)
	{
		s.setValue("_MDSsNum",_MDSsNum) ;
	}
	s.setValue("_MDSsNumSet",_MDSsNumSet) ;

	if(_SitesNumSet)
	{
		s.setValue("_SitesNum",_SitesNum) ;
	}
	s.setValue("_SitesNumSet",_SitesNumSet) ;
	dbInventoryObjectWithGapsComments::exportObj(s) ; 
}


// import the class from a stream
void dbInventoryObjectTEC::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	rsPushNSpace nsp(s,"dbInventoryObjectTEC") ;
	s.getValue("_MDSsNumSet",_MDSsNumSet) ;
	if(_MDSsNumSet)
	{
		s.getValue("_MDSsNum",_MDSsNum) ;
	}
	else { _MDSsNum = 0 ; }
	s.getValue("_SitesNumSet",_SitesNumSet) ;
	if(_SitesNumSet)
	{
		s.getValue("_SitesNum",_SitesNum) ;
	}
	else { _SitesNum = 0 ; }
	dbInventoryObjectWithGapsComments::importObj(s) ;
}


// export the class to XML stream
void dbInventoryObjectTEC::exportXML(XMLOstream &xst)
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	XMLOstream::Tag txml(xst, "dbInventoryObjectTEC");
    const string K = "" ;
	{ XMLOstream::Tag t(xst, "_MDSsNum", _MDSsNumSet? db::toSQL(K,_MDSsNum) : "0", "label=\"MDSs Num\""); } // PRQA S 3380, 3385 4
	{ XMLOstream::Tag t(xst, "_MDSsNumSet", (_MDSsNumSet ? "true" : "false")) ; }
	{ XMLOstream::Tag t(xst, "_SitesNum", _SitesNumSet? db::toSQL(K,_SitesNum) : "0", "label=\"Sites Num\""); }
	{ XMLOstream::Tag t(xst, "_SitesNumSet", (_SitesNumSet ? "true" : "false")) ; } 
	dbInventoryObjectWithGapsComments::exportXML(xst) ; 
}


// load the object from a row
int dbInventoryObjectTEC::loadRow(dbConnection &conn, const dbRow &row, int offset)
{
    uint16_t loff = offset ; // PRQA S 3000, 3010

	try
	{
		row.column(loff++, _MDSsNum) ;
		_MDSsNumSet = true ;
	}
	catch (dbRow::NullValueException &)
	{
		_MDSsNumSet = false ;
	}

	try
	{
		row.column(loff++, _SitesNum) ;
		_SitesNumSet = true ;
	}
	catch (dbRow::NullValueException &)
	{
		_SitesNumSet = false ;
	}
	return dbInventoryObjectWithGapsComments::loadRow(conn, row, loff) ; // PRQA S 3000
}

// get the field names
deque<string> dbInventoryObjectTEC::getFieldArray(dbConnection &conn)
{
	deque<string> s = dbInventoryObjectWithGapsComments::getFieldArray(conn);
	const string  dbKey = conn.getKey() ;
	SQLString qString ( dbKey, SQLString::SELECT ) ;
	qString.addTableList("t_invsiterelation") ;
    qString.addSelectList( "count(*)" );
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "t_invsiterelation", "filename", dbParam::EQUAL_TO, "t_inventory.filename") ); // PRQA S 3050, 3081
	    pp.addParameter( dbParam(dbKey, "t_invsiterelation", "fileversion", dbParam::EQUAL_TO, "t_inventory.fileversion") ); // PRQA S 3050, 3081
	    qString.addANDWhereParameter(pp);
	}
	s.push_front(qString.getSQL(true)+ "AS Sitesnum");
	
	SQLString sqlString ( dbKey, SQLString::SELECT ) ;
	sqlString.addTableList("t_inventorydataset") ;
    sqlString.addSelectList( "count(*)" );
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "t_inventorydataset", "filename", dbParam::EQUAL_TO, "t_inventory.filename") ); // PRQA S 3050, 3081
	    pp.addParameter( dbParam(dbKey, "t_inventorydataset", "fileversion", dbParam::EQUAL_TO, "t_inventory.fileversion") ); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
	}
	s.push_front(sqlString.getSQL(true)+ "AS MDSsnum");	
	return s ;
}

string dbInventoryObjectTEC::getFields()
{
	return getFields(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
}

string dbInventoryObjectTEC::getFields(dbConnection& conn)
{
	deque <string> stringArray = getFieldArray(conn);
	return dbInventoryObject::convertFieldList (stringArray);
}


void dbInventoryObjectTEC::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_MDSsNum = " << (_MDSsNumSet ? _MDSsNum : 0) << '\n' ;  // PRQA S 3380 2
	o << "_SitesNum = " << (_SitesNumSet ? _SitesNum : 0) << '\n' ; 
	o << "_BaselineNum = " << _baselineIds.size() << '\n';
	dbInventoryObjectWithGapsComments::writeTo(o) ; 
}


void dbInventoryObjectTEC::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_MDSsNum = " << (_MDSsNumSet ? _MDSsNum : 0) << '\n' ;   // PRQA S 3380 2
	o << "_SitesNum = " << (_SitesNumSet ? _SitesNum : 0) << '\n' ; 
	o << "_BaselineNum = " << _baselineIds.size() << '\n';
	dbInventoryObjectWithGapsComments::writeTo(o) ; 
}

unsigned long dbInventoryObjectTEC::loadMDSs(dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	bool fileNameSet = false ;
	string fileName = getFileName(&fileNameSet); 
    bool fileVersionSet = false ;
	string fileVersion= getFileVersion(&fileVersionSet); 
	
	if(!fileNameSet || !fileVersionSet)
	{
		InvalidUse e("fileNameSet and/or fileVersionSet not set") ;
		ACS_THROW(e) ;
	}
		
	_inventoryMDSs->resetParameters() ; 
	_inventoryMDSs->clear() ; 
	
	const string dbKey = conn.getKey() ;
	
	dbQueryParameters par(dbKey) ; 
	
	dbParam fileNamePar(dbKey, "T_Inventorydataset", "FileName", dbParam::EQUAL_TO , db::toSQL(dbKey,fileName)) ; 		
	dbParam fileVersionPar(dbKey, "T_Inventorydataset", "FileVersion", dbParam::EQUAL_TO , db::toSQL(dbKey,fileVersion)) ; 			
	
	par.addParameter(fileNamePar) ; 
	par.addParameter(fileVersionPar) ; 
	
	_inventoryMDSs->addParameter(par) ;
	
	_MDSsLoaded = true ; 
	 
	return _inventoryMDSs->doQuery(conn) ; 
}


unsigned long dbInventoryObjectTEC::loadSites(dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	bool fileNameSet = false ;
	string fileName = getFileName(&fileNameSet); 
    bool fileVersionSet= false ; 
	string fileVersion= getFileVersion(&fileVersionSet); 
	
	if(!fileNameSet || !fileVersionSet)
	{
		InvalidUse e("fileNameSet and/or fileVersionSet not set") ;
		ACS_THROW(e) ;
	}
		
	_inventorySites->resetParameters() ; 
	_inventorySites->clear() ; 
	
	
	const string dbKey = conn.getKey() ;
	dbQueryParameters par(dbKey) ; 
	
	dbParam fileNamePar(dbKey, "T_Invsiterelation", "FileName", dbParam::EQUAL_TO , db::toSQL(dbKey, fileName)) ; 		
	dbParam fileVersionPar(dbKey, "T_Invsiterelation", "FileVersion", dbParam::EQUAL_TO , db::toSQL(dbKey, fileVersion)) ; 			
	
	par.addParameter(fileNamePar) ; 
	par.addParameter(fileVersionPar) ; 
	
	_inventorySites->addParameter(par) ;
	
	_SitesLoaded = true ; 
	 
	return _inventorySites->doQuery(conn) ; 
}


unsigned long dbInventoryObjectTEC::loadAddParams(dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	size_t s (0) ;
	deleteAddParams2Save() ; // Deallocate the AddParams container (pointer and boolean)
	_inventoryAddParams->resetParameters() ; 
	_inventoryAddParams->clear() ; 
	
	
	const string dbKey = conn.getKey() ;
	dbQueryParameters par(dbKey) ; 
	
	dbParam fileNamePar(dbKey, dbInventoryAddParams::table(), "inv_id", dbParam::EQUAL_TO , db::toSQL(dbKey, getId())) ; 		
	
	par.addParameter(fileNamePar) ; 
	
	_inventoryAddParams->addParameter(par) ;
		
	if ( (s = _inventoryAddParams->doQuery(conn)) > 0 )
	{
		// Allocate the AddParams container (pointer and boolean)
		_addParams2Save = static_cast<dbInventoryAddParams*>( ((*_inventoryAddParams)[0])->clone() ) ;
		_addParamsSet = true ; 
	} 

	return s ;
}


void dbInventoryObjectTEC::saveMDSs(dbConnection &conn) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if (_MDSs2Save.empty()) {
		return ;
    }
	// Start a transaction on the connection
	db::Transaction transaction(conn) ;
	transaction.begin_work("MDS_SAVE") ;
    size_t mds_size = _MDSs2Save.size() ;
	for (size_t i=0; i < mds_size ; ++i) {
		_MDSs2Save[i]->save(conn) ;
    }
	deleteMDSs2Save() ;
    transaction.commit() ;
}

void dbInventoryObjectTEC::saveSites(dbConnection &conn) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if (_Sites2Save.empty()) {
		return ;
    }
	// Start a transaction on the connection
	db::Transaction transaction(conn) ;
	transaction.begin_work("Sites_SAVE") ;
    size_t s2s_size = _Sites2Save.size() ;
	for (size_t i=0; i < s2s_size; ++i) {
		_Sites2Save[i]->save(conn) ;
    }
	deleteSites2Save() ;
    transaction.commit() ;
}

void dbInventoryObjectTEC::saveAddParams(dbConnection &conn) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if ((NULL == _addParams2Save) || (false == _addParamsSet)) {
		return ;
    }

	// Start a transaction on the connection
	db::Transaction transaction(conn) ;
	transaction.begin_work("AddParams_SAVE") ;

	_addParams2Save->setInventoryId(getId(), conn) ;
	_addParams2Save->save(conn) ;
	deleteAddParams2Save() ; 
	transaction.commit() ;
}


void dbInventoryObjectTEC::updateAddParams (dbConnection& conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if ( (false == _addParamsSet) || (NULL == _addParams2Save) ) 
	{
		// Do nothing
	}
	else
	{
		_addParams2Save->setInventoryId(getId(), conn) ;

		if (true == _addParams2Save->isFromDatabase())
		{
			_addParams2Save->update(conn) ;	// Update (neither remove or save are supported here)
		}
		else
		{
			clearAddParams(conn) ; 			// Remove from DB
			_addParams2Save->save(conn) ;	// Save
		}
	}

	return ;
}


void dbInventoryObjectTEC::deleteMDSs2Save() throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    size_t mds_size = _MDSs2Save.size() ;
    for (size_t i=0; i < mds_size ; ++i) {
		delete _MDSs2Save[i];
    }
	_MDSs2Save.clear() ;
}

void dbInventoryObjectTEC::deleteSites2Save() throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    size_t s2s_size = _Sites2Save.size() ;
    for (size_t i=0; i < s2s_size; ++i) {
		delete _Sites2Save[i];
    }
	_Sites2Save.clear() ;
}


void dbInventoryObjectTEC::deleteAddParams2Save() throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	
	if ( (true == _addParamsSet) && (NULL != _addParams2Save) )
	{
		// Frees the container
		delete _addParams2Save ;
	}
	
	 _addParams2Save = NULL ; _addParamsSet = false ; 
}


unsigned long dbInventoryObjectTEC::MDSsNum(dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if(!_MDSsLoaded) {
		loadMDSs(conn) ; 
    }
	return _inventoryMDSs->size() ; 
}

unsigned long dbInventoryObjectTEC::SitesNum(dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if(!_SitesLoaded) {
		loadSites(conn) ; 
    }
	return _inventorySites->size() ; 
}

dbInventoryMDS * dbInventoryObjectTEC::MDS(unsigned long ind, dbConnection &conn) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if(!_MDSsLoaded) {
		loadMDSs(conn) ; 
    }
	return  (*_inventoryMDSs)[ind] ; 
}

dbInventorySite * dbInventoryObjectTEC::Site(unsigned long ind, dbConnection &conn) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if(!_SitesLoaded) {
		loadSites(conn) ; 
    }
	return  (*_inventorySites)[ind] ; 
}


void dbInventoryObjectTEC::addMDS(int mdsid, int count, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	dbInventoryMDS *mds = dynamic_cast<dbInventoryMDS*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryMDS", conn.getKey())); // PRQA S 3081, 4412 2
	ACS_COND_THROW(!mds, exIllegalValueException("dbInventoryMDS dynamic cast failed")) ;
	try {
	
		mds->setFileName(getFileName(), conn) ; 
		mds->setFileVersion(getFileVersion(), conn) ; 
		mds->setMeasDataSetId(mdsid, conn) ; 
		mds->setCount(count, conn) ; 
		if (isFromDatabase()) {
			mds->save(conn) ; 
			_MDSsNum++ ;
			loadMDSs(conn) ;
		}
		else {
			_MDSs2Save.push_back(mds) ;
		}
	}
	catch(...) {
		delete mds ;
		throw ;
	}
}

void dbInventoryObjectTEC::addSite(int sid, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	dbInventorySite *s = dynamic_cast<dbInventorySite*>(dbPersistentFactoryChain::instance()->newObject("dbInventorySite", conn.getKey())); // PRQA S 4412, 3081 2
	ACS_COND_THROW(!s, exIllegalValueException("dbInventorySite creation failed")) ;
	try {
	
		s->setFileName(getFileName(), conn) ; 
		s->setFileVersion(getFileVersion(), conn) ; 
		s->setSiteId(sid, conn) ; 
		if (isFromDatabase()) {
			s->save(conn) ; 
			_SitesNum++ ;
			loadSites(conn) ;
		}
		else {
			_Sites2Save.push_back(s) ;
		}
	}
	catch(...) {
		delete s ;
		throw ;
	}
}


void dbInventoryObjectTEC::setAddParams(const map<string, string>& addParams_M, \
										dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	dbInventoryAddParams *s = dynamic_cast<dbInventoryAddParams*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryAddParams", conn.getKey())); // PRQA S 4412, 3081 2
	ACS_COND_THROW(!s, exIllegalValueException("dbInventoryAddParams creation failed")) ;
	_addParamsSet = true ;
	
	try {

		string key = "Session_Id" ; if (addParams_M.end() != addParams_M.find(key))
		{
			const string value_s (addParams_M.at(key)) ; 
			const dbPersistent::IdType value ( getAdditionalParamsTimelinessId(value_s, conn) ) ;
			s->setTimelinessId(value, conn) ;
		}
				
		key = "Granule_Number" ; if (addParams_M.end() != addParams_M.find(key))
		{
			const string value_s (addParams_M.at(key)) ; 
			int value ; stringstream (value_s) >> value;
			s->setGranuleNumber(value, conn) ;
		}
		
		key = "Granule_Position" ; if (addParams_M.end() != addParams_M.find(key))
		{
			const string value_s (addParams_M.at(key)) ; 
			s->setGranulePosition(value_s, conn) ;
		}
		
		key = "Dump_Start" ; if (addParams_M.end() != addParams_M.find(key))
		{
			const string value_s (addParams_M.at(key)) ; 
			s->setDumpStart(DateTime(value_s), conn) ;
		}
		
		key = "Mission_Data_Take_Id" ; if (addParams_M.end() != addParams_M.find(key))
		{
			const string value_s (addParams_M.at(key)) ; 
			unsigned long long int value ; stringstream (value_s) >> value;
			s->setMissionDatatakeId(value, conn) ;
		}
				
		key = "Next_Equator_X_Time" ; if (addParams_M.end() != addParams_M.find(key))
		{
			const string value_s (addParams_M.at(key)) ; 
			s->setNextEquatorXTime(DateTime(value_s), conn) ;
		}

				
		if (isFromDatabase()) {
			s->save(conn) ; 
			loadAddParams(conn) ; 
		}
		else {
			_addParams2Save = s ;
		}
		
	}
	catch(...) {
		delete s ;
		throw ;
	}
}


void dbInventoryObjectTEC::clearAddParams (dbConnection& conn)
{
    Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("AddParams_CLEAR") ;
    string dbKey = conn.getKey();
    SQLString theString(dbKey, SQLString::DELETE);
    theString.addTableList (dbInventoryAddParams::table());
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, dbInventoryAddParams::table(), "inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, getId()))) ;
        theString.addANDWhereParameter(pp) ;
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query [" << theString.getSQL() << "]");

    dbQuery query( conn, theString, "dbInventoryObjectTEC::clearAddParams" );
    query.execForStatus();
    transaction.commit();
}

dbPersistent::IdType dbInventoryObjectTEC::getAdditionalParamsTimelinessId(	const string& sessionKey, \
																dbConnection &conn) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    IdType timelinessId;
    string dbKey = conn.getKey();
    SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addSelectList( "id" ) ;
    queryString.addTableList( "t_downlink_session_tracks" ) ;

    dbQueryParameters pars(dbKey) ;
    pars.addParameter(dbParam(dbKey, "t_downlink_session_tracks","session_id",dbParam::EQUAL_TO,db::toSQL(dbKey, sessionKey))) ;
    queryString.addANDWhereParameter(pars) ;

	dbQuery query(conn, queryString, "getAdditionalParamsTimelinessId") ;
	dbSet set ;
    dbRow row(dbKey) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

    query.execForSet (set);

	ACS_COND_THROW( (set.size() < 1), exIllegalValueException("Unable to resolve SessionKey: \"" + sessionKey + "\"")) ;
	set.getNextRow(row) ; row.column (0, timelinessId);

	return timelinessId ;
}


void dbInventoryObjectTEC::removeAllSitesLink(dbConnection &conn) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    const string &dbKey = conn.getKey() ;
	string queryString ;
	const string dialectName = SQLDialectFactory::dialectName(conn.getKey());
	if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		queryString = "DELETE FROM t_invsiterelation" ;
		queryString += " WHERE FileName = " ;
		queryString +=	db::toSQL(dbKey, getFileName()) ;
		queryString += " AND FileVersion = " + db::toSQL(dbKey, getFileVersion()) ;
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(queryString) ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("SiteLn_REMOVE") ;
	dbQuery query(conn, sqlString, "SiteLinkRemoval") ;
	if (!query.execForStatus())
	{
		dbQuery::QueryExecuteException e("Site Link Removal Execution error: Command : "
		+ queryString
		+ " Error : "
		+ query.errorString()) ;
		ACS_THROW (e) ;
	}
    transaction.commit() ;
}


void dbInventoryObjectTEC::addBaselineId (IdType bid)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    Lock lock (*this);
    if ( find (_baselineIds.begin(), _baselineIds.end(), bid) == _baselineIds.end() )
    {
        // actually a new baseline id
        _baselineIds.push_back (bid);
        _baselineModified = true;
    }
}

void dbInventoryObjectTEC::removeBaselineLinks (dbConnection& conn)
{
    Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("BaselineLn_REMOVE") ;

    string dbKey = conn.getKey();
    SQLString theString(dbKey, SQLString::DELETE);
    theString.addTableList (baselineXTable);
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, baselineXTable,"inventory_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, getId()))) ;
        theString.addANDWhereParameter(pp) ;
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query [" << theString.getSQL() << "]");
    dbQuery query( conn, theString, "dbInventoryObjectTEC::removeBaselineLinks" );
    query.execForStatus();
    transaction.commit() ;
}

const vector<dbPersistent::IdType>& dbInventoryObjectTEC::getBaselineIds()
{
    return _baselineIds;
}

void dbInventoryObjectTEC::resetBaselines()
{
    Lock lock (*this);
    if (!_baselineIds.empty())
    {
        _baselineIds.clear();
        _baselineModified = true;
    }
}
	
void dbInventoryObjectTEC::updateBaselines(dbConnection& conn)
{   
	Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if (_baselineModified)
	{
		db::Transaction transaction(conn) ;
		transaction.begin_work("BaselineLn_UPDATE") ;
		removeBaselineLinks (conn);
		saveBaselines (conn);
		transaction.commit() ;
	}

}

void dbInventoryObjectTEC::saveBaselines(dbConnection& conn)
{

    // If here I assume that the dbInventoryObject is not associated to any baseline
    // otherwise the update must be used
    Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    if (!_baselineIds.empty())
    {
	    db::Transaction transaction(conn) ;
	    transaction.begin_work("BaselineLn_SAVE") ;
        size_t bl_size = _baselineIds.size() ;
        for (size_t idx=0; idx<bl_size; ++idx)
        {
            dbSmartPersistent item (baselineXTable, conn);
            item.set ( "inventory_id", getId(), conn );
            item.set ( "baseline_id", _baselineIds.at(idx), conn );
            item.save (conn);
        }
        transaction.commit() ;
    }
    _baselineModified=false;
}

void dbInventoryObjectTEC::loadBaselines(dbConnection& conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
    string dbKey = conn.getKey();
    SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addSelectList( "baseline_id" ) ;
    queryString.addTableList( baselineXTable ) ;

    dbQueryParameters pars(dbKey) ;
    pars.addParameter(dbParam(dbKey, baselineXTable,"inventory_id",dbParam::EQUAL_TO,db::toSQL(dbKey, getId()))) ;
    queryString.addANDWhereParameter(pars) ;

	dbQuery query(conn, queryString, "TEC baseline load") ;
	dbSet set ;
    dbRow row(dbKey) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

    query.execForSet (set);
    while (set.getNextRow(row))
    {
        IdType blid;
        row.column (0, blid);
        _baselineIds.push_back(blid);
    }
}

// Check if table inventoryAddParams exists
bool dbInventoryObjectTEC::hasAddParams (dbConnection& conn)
{
    return dbSchemaMetaDataProvider::existTable(dbInventoryAddParams::table(), conn) ;
}

// S2PDGS-1137
// Extra info management functions

bool dbInventoryObjectTEC::hasExtraInfo (dbConnection& conn)
{
    return dbSchemaMetaDataProvider::existTable(inventoryExtraTableName, conn) ;
}

void dbInventoryObjectTEC::updateExtraInfo (dbConnection& conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
    clearExtraInfo (conn);
    saveExtraInfo (conn);
}

void dbInventoryObjectTEC::saveExtraInfo (dbConnection& conn)
{

    // If here I assume that the dbInventoryObject is not associated to any extrainfo
    // otherwise the update must be used
    Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
    if (_extraInfoSet)
    {
	    db::Transaction transaction(conn) ;
	    transaction.begin_work("ExtraInfo_SAVE") ;
        dbSmartPersistent item (inventoryExtraTableName, conn);
        item.set ( "inv_id", getId(), conn );
		oftmpstream clobstream ("/tmp/InventoryExtraInfos",".clob") ;
		clobstream() << _extraInfo;
		clobstream.close(); // flush the buffer
		item.setFilePath("extra", clobstream.path(), conn) ;
        //item.set ( "extra", _extraInfo, conn );
        item.save (conn);
        transaction.commit() ;
    }
}

void dbInventoryObjectTEC::loadExtraInfo (dbConnection& conn)
{
    Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

    // Reset the container
    unsetExtraInfo();

    string dbKey = conn.getKey();

	dbSmartPersistent extraInfoSP ( inventoryExtraTableName, conn);

	if (extraInfoSP.load( conn, getId()) && (extraInfoSP.isSet ("extra", conn) ) )
	{
		// Extra info found
		// download the blob data field
		string extrainfo;		

		string dwnName = File::tempname("/tmp/InvExtraInfoLoad", ".clob"); //Temp filename
		FileGuard dwnNameGuard (dwnName);
		extraInfoSP.downloadFile("extra", dwnName, conn) ;
		{
			std::ifstream ifs( dwnName.c_str() );
			std::stringstream buffer;
			buffer << ifs.rdbuf();
			extrainfo = buffer.str();
		}

		setExtraInfo(extrainfo);
	}
}

void dbInventoryObjectTEC::clearExtraInfo (dbConnection& conn)
{
    Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	db::Transaction transaction(conn) ;
	transaction.begin_work("ExtraInfo_CLEAR") ;
    string dbKey = conn.getKey();
    SQLString theString(dbKey, SQLString::DELETE);
    theString.addTableList (inventoryExtraTableName);
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, inventoryExtraTableName, "inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, getId()))) ;
        theString.addANDWhereParameter(pp) ;
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query [" << theString.getSQL() << "]");
    dbQuery query( conn, theString, "dbInventoryObjectTEC::clearExtraInfo" );
    query.execForStatus();
    transaction.commit();
}

void dbInventoryObjectTEC::setExtraInfo(const std::string &v) 
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if (!_extraInfoSet || (_extraInfo != v) ) 
	{
		_extraInfo = v ;
		_extraInfoSet = true ;
		setModifiedFlag() ;
	}
}


const std::string &dbInventoryObjectTEC::getExtraInfo(bool *set) const 
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if (set) {
		*set = _extraInfoSet ;
    }
	return _extraInfo ;
}


void dbInventoryObjectTEC::unsetExtraInfo()
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if(_extraInfoSet) {
		setModifiedFlag();
    }

	_extraInfoSet = false;
}

_ACS_END_NAMESPACE

