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
	
	Revision 5.5  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.4  2016/04/19 13:53:46  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.2  2014/05/26 11:39:44  marpas
	dbConv casted to avoid problem with new libXMLResources templates for tags
	
	Revision 4.1  2014/04/08 16:39:55  marpas
	adopting libDateTime 5.13
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:22  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.23  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.22  2012/11/19 22:51:15  marpas
	compile with PDS_OLD_SCHEMA to use old pds schema.
	dbReceptionRule and related Query class introduced
	
	Revision 2.21  2012/11/16 13:01:13  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.18  2012/03/19 09:52:45  marpas
	virtual ftype mgmnt added
	
	Revision 2.17  2012/03/13 09:38:30  marpas
	fixed tabular representation
	
	Revision 2.16  2012/03/12 13:44:47  marpas
	fixing behaviour in save, some debug added
	
	Revision 2.15  2012/03/12 11:23:45  marpas
	aligning to new schema
	
	Revision 2.14  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.13  2011/03/02 18:47:13  marpas
	serial 8 support
	
	Revision 2.12  2010/06/08 14:49:38  marpas
	dbFileType::getAssociatedSatellites method implemented
	
	Revision 2.11  2009/07/08 13:18:11  manuel.maccaroni
	Since dbFileType::checkPeriodicity() no longer throws an excpetion the periodicty anomaly is now logged on excerr
	
	Revision 2.10  2009/07/08 12:51:50  manuel.maccaroni
	FTypePeriodicityException(s) are no longer being thrown when a product is received with a delay, email containing the related message are no longer sent (it is safe to pass an empty vector). dbFileType::checkPeriodicity() now simply fills t_inventoryanomalies
	
	Revision 2.9  2009/05/18 17:27:35  marpas
	dbFileType::checkPeriodicity has a new parameter to allow an inventory to be performed BEFORE the verification.
	A bug was also removed: no check on the returned count was performed: the error was always reported
	
	Revision 2.8  2009/04/06 12:41:12  marpas
	t_inventoryanomalies INSERT fixed
	
	Revision 2.7  2009/04/06 12:37:17  marpas
	debug improved
	
	Revision 2.6  2009/04/03 17:08:38  marpas
	dbFileType::checkPeriodicity implemented
	
	Revision 2.5  2009/02/12 13:49:11  enrcar
	EC:: typo fixed
	
	Revision 2.4  2009/02/04 17:31:52  marpas
	new file type schema alignement
	
	Revision 2.3  2008/09/23 08:27:38  crivig
	porting to postgres8
	
	Revision 2.2  2006/06/09 12:45:58  giucas
	Fixed syntax error in update query.
	
	Revision 2.1  2006/06/09 11:53:21  giucas
	Update method fixed.
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.40  2005/10/24 16:44:01  marpas
	dbexpress compatibility
	
	Revision 1.39  2005/10/11 08:11:24  enrcar
	added SQLString compatibility
	
	Revision 1.38  2005/06/15 07:44:49  integrator
	multiple storage manager implemented
	
	Revision 1.37  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.36  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.35  2004/10/14 15:14:53  paoscu
	Inventory and PreInventory options added
	
	Revision 1.34  2004/10/04 15:13:52  ivafam
	When set orderBy control if orderBySQLDString contain already clause "ORDER BY"
	
	Revision 1.33  2004/09/17 11:57:51  paoscu
	tabular representation improved
	
	Revision 1.32  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.31  2004/07/07 10:34:32  paoscu
	dbPersistent I/F chnaged
	
	Revision 1.30  2004/05/03 14:55:34  paoscu
	Paramer "id" removed in method "remove"
	
	Revision 1.29  2004/04/16 12:49:34  paoscu
	Bug fixed in Lock usage
	
	Revision 1.28  2004/04/13 16:57:08  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.27  2004/03/04 18:54:33  paoscu
	StoreOntape field added.
		

*/


#include <dbFileType.h>
#include <Application.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConv.h>
#include <Transaction.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <sstream>
#include <XMLOstream.h>
#include <SQLString.h>
#include <exMailStream.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbFileType)

// constructor
dbFileType::dbFileType() :
	dbPersistent(),
	_fileType() ,
	_fileTypeSet() ,
#ifdef PDS_OLD_SCHEMA
	_userFileType() ,
	_userFileTypeSet() ,
#endif
	_description() ,
	_descriptionSet() ,
	_nameRegularExpression() ,
	_nameRegularExpressionSet() ,
#ifdef PDS_OLD_SCHEMA
	_generateHeader() ,
	_generateHeaderSet() ,
#endif
	_headerSeparation() ,
	_headerSeparationSet() ,
#ifdef PDS_OLD_SCHEMA
	_headerGenerator() ,
	_headerGeneratorSet() ,
	_inventoryExecutable() ,
	_inventoryExecutableSet() ,
	_inventoryExecutableOpts() ,
	_inventoryExecutableOptsSet() ,
#endif
	_group() ,
	_groupSet() ,
#ifdef PDS_OLD_SCHEMA
	_package() ,
	_packageSet() ,
	_onlineLatencyTime() ,
	_onlineLatencyTimeSet() ,
	_headerTemplatePrefix() ,
	_headerTemplatePrefixSet() ,
	_storage() ,
	_storageSet() ,
	_overallLifeTime() ,
	_overallLifeTimeSet() ,
	_preInventoryExecutable() ,
	_preInventoryExecutableSet() ,
	_preInventoryExecutableOpts() ,
	_preInventoryExecutableOptsSet() ,
	_storeOnTape() ,
	_storeOnTapeSet(),
#endif
    _inventoryPeriod(), 
    _inventoryPeriodSet(), 
	_mngSngExt() ,
	_mngSngExtSet(),
	_sngExt() ,
	_sngExtSet(),
	_hdrExt() ,
	_hdrExtSet(),
	_dblExt() ,
	_dblExtSet()

{
}


// copy constructor
dbFileType::dbFileType(const dbFileType &o):
	dbPersistent(o),
	_fileType(o._fileType) ,
	_fileTypeSet(o._fileTypeSet) ,
#ifdef PDS_OLD_SCHEMA
	_userFileType(o._userFileType) ,
	_userFileTypeSet(o._userFileTypeSet) ,
#endif
	_description(o._description) ,
	_descriptionSet(o._descriptionSet) ,
	_nameRegularExpression(o._nameRegularExpression) ,
	_nameRegularExpressionSet(o._nameRegularExpressionSet) ,
#ifdef PDS_OLD_SCHEMA
	_generateHeader(o._generateHeader) ,
	_generateHeaderSet(o._generateHeaderSet) ,
#endif
	_headerSeparation(o._headerSeparation) ,
	_headerSeparationSet(o._headerSeparationSet) ,
#ifdef PDS_OLD_SCHEMA
	_headerGenerator(o._headerGenerator) ,
	_headerGeneratorSet(o._headerGeneratorSet) ,
	_inventoryExecutable(o._inventoryExecutable) ,
	_inventoryExecutableSet(o._inventoryExecutableSet) ,
	_inventoryExecutableOpts(o._inventoryExecutableOpts) ,
	_inventoryExecutableOptsSet(o._inventoryExecutableOptsSet) ,
#endif
	_group(o._group) ,
	_groupSet(o._groupSet) ,
#ifdef PDS_OLD_SCHEMA
	_package(o._package) ,
	_packageSet(o._packageSet) ,
	_onlineLatencyTime(o._onlineLatencyTime) ,
	_onlineLatencyTimeSet(o._onlineLatencyTimeSet) ,
	_headerTemplatePrefix(o._headerTemplatePrefix) ,
	_headerTemplatePrefixSet(o._headerTemplatePrefixSet) ,
	_storage(o._storage) ,
	_storageSet(o._storageSet) ,
	_overallLifeTime(o._overallLifeTime) ,
	_overallLifeTimeSet(o._overallLifeTimeSet) ,
	_preInventoryExecutable(o._preInventoryExecutable) ,
	_preInventoryExecutableSet(o._preInventoryExecutableSet) ,
	_preInventoryExecutableOpts(o._preInventoryExecutableOpts) ,
	_preInventoryExecutableOptsSet(o._preInventoryExecutableOptsSet) ,
	_storeOnTape(o._storeOnTape) ,
	_storeOnTapeSet(o._storeOnTapeSet),
#endif
    _inventoryPeriod(o._inventoryPeriod), 
    _inventoryPeriodSet(o._inventoryPeriodSet), 
	_mngSngExt(o._mngSngExt) ,
	_mngSngExtSet(o._mngSngExtSet),
	_sngExt(o._sngExt) ,
	_sngExtSet(o._sngExtSet),
	_hdrExt(o._hdrExt) ,
	_hdrExtSet(o._hdrExtSet),
	_dblExt(o._dblExt) ,
	_dblExtSet(o._dblExtSet)
{
}


dbFileType::~dbFileType() throw()
{
}



dbPersistent *dbFileType::clone() const
{
	return new dbFileType(*this) ;
}


// operator =
dbFileType &dbFileType::operator=(const dbFileType &o)
{
	Lock lock(*this) ;

	if(this != &o)
	{
		dbPersistent::operator=(o) ;
		_fileType=o._fileType;
		_fileTypeSet=o._fileTypeSet;
#ifdef PDS_OLD_SCHEMA
		_userFileType=o._userFileType;
		_userFileTypeSet=o._userFileTypeSet;
#endif
		_description=o._description;
		_descriptionSet=o._descriptionSet;
		_nameRegularExpression=o._nameRegularExpression;
		_nameRegularExpressionSet=o._nameRegularExpressionSet;
#ifdef PDS_OLD_SCHEMA
		_generateHeader=o._generateHeader;
		_generateHeaderSet=o._generateHeaderSet;
#endif
		_headerSeparation=o._headerSeparation;
		_headerSeparationSet=o._headerSeparationSet;
#ifdef PDS_OLD_SCHEMA
		_headerGenerator=o._headerGenerator;
		_headerGeneratorSet=o._headerGeneratorSet;
		_inventoryExecutable=o._inventoryExecutable;
		_inventoryExecutableSet=o._inventoryExecutableSet;
		_inventoryExecutableOpts=o._inventoryExecutableOpts;
		_inventoryExecutableOptsSet=o._inventoryExecutableOptsSet;
#endif
		_group=o._group;
		_groupSet=o._groupSet;
#ifdef PDS_OLD_SCHEMA
		_package=o._package;
		_packageSet=o._packageSet;
		_onlineLatencyTime=o._onlineLatencyTime;
		_onlineLatencyTimeSet=o._onlineLatencyTimeSet;
		_headerTemplatePrefix=o._headerTemplatePrefix;
		_headerTemplatePrefixSet=o._headerTemplatePrefixSet;
		_storage=o._storage;
		_storageSet=o._storageSet;
		_overallLifeTime=o._overallLifeTime;
		_overallLifeTimeSet=o._overallLifeTimeSet;
		_preInventoryExecutable=o._preInventoryExecutable;
		_preInventoryExecutableSet=o._preInventoryExecutableSet;
		_preInventoryExecutableOpts=o._preInventoryExecutableOpts;
		_preInventoryExecutableOptsSet=o._preInventoryExecutableOptsSet;
		_storeOnTape=o._storeOnTape;
		_storeOnTapeSet=o._storeOnTapeSet;
#endif
        _inventoryPeriod = o._inventoryPeriod ;  
        _inventoryPeriodSet = o._inventoryPeriodSet ;
		_mngSngExt=o._mngSngExt; 
		_mngSngExtSet=o._mngSngExtSet;
		_sngExt=o._sngExt; 
		_sngExtSet=o._sngExtSet;
		_hdrExt=o._hdrExt; 
		_hdrExtSet=o._hdrExtSet;
		_dblExt=o._dblExt; 
		_dblExtSet=o._dblExtSet;
	}
	return *this ;
}


string dbFileType::className() const
{
	return staticClassName() ;
}


string dbFileType::staticClassName()
{
	return "dbFileType" ;
}




// initialize the class
void dbFileType::reset()
{
	Lock lock(*this) ;

	// reset dbPersistent
	dbPersistent::reset() ;
	_fileType="" ;
	_fileTypeSet=false ;
#ifdef PDS_OLD_SCHEMA
	_userFileType="" ;
	_userFileTypeSet=false ;
#endif
	_description="" ;
	_descriptionSet=false ;
	_nameRegularExpression="" ;
	_nameRegularExpressionSet=false ;
#ifdef PDS_OLD_SCHEMA
	_generateHeader=false ;
	_generateHeaderSet=false ;
#endif
	_headerSeparation=false ;
	_headerSeparationSet=false ;
#ifdef PDS_OLD_SCHEMA
	_headerGenerator="" ;
	_headerGeneratorSet=false ;
	_inventoryExecutable="" ;
	_inventoryExecutableSet=false ;
	_inventoryExecutableOpts="" ;
	_inventoryExecutableOptsSet=false ;
#endif
	_group="" ;
	_groupSet=false ;
#ifdef PDS_OLD_SCHEMA
	_package=false ;
	_packageSet=false ;
	_onlineLatencyTime=0 ;
	_onlineLatencyTimeSet=false ;
	_headerTemplatePrefix="" ;
	_headerTemplatePrefixSet=false ;
	_storage="" ;
	_storageSet=false ;
	_overallLifeTime=0 ;
	_overallLifeTimeSet=false ;
	_preInventoryExecutable="" ;
	_preInventoryExecutableSet=false ;
	_preInventoryExecutableOpts="" ;
	_preInventoryExecutableOptsSet=false ;
	_storeOnTape=true ;
	_storeOnTapeSet=false ;
#endif
	_inventoryPeriod=0 ;
	_inventoryPeriodSet=false ;
	_mngSngExt = true ;
	_mngSngExtSet = false ;
	_sngExtSet = false ;
	_hdrExtSet = false ;
	_dblExtSet = false ;
}


// save the data in database
dbPersistent::IdType dbFileType::save(dbConnection &conn)
{
	Lock lock(*this) ;

	if (!isModified()) {
		return 0 ; 
    }

	ACS_COND_THROW(isFromDatabase(),InvalidUse("Error: dbFileType::save(). Use the method update() instead of save().")) ;
	ACS_COND_THROW(!_fileTypeSet,InvalidUse("Mandatory field \"File Type\" must be set.")) ;
#ifdef PDS_OLD_SCHEMA
	ACS_COND_THROW(!_userFileTypeSet,InvalidUse("Mandatory field \"User File Type\" must be set.")) ;
	ACS_COND_THROW(!_generateHeaderSet,InvalidUse("Mandatory field \"Generate Header\" must be set.")) ;
#endif
	ACS_COND_THROW(!_headerSeparationSet,InvalidUse("Mandatory field \"Header Separation\" must be set.")) ;
	ACS_COND_THROW(!_groupSet,InvalidUse("Mandatory field \"Group\" must be set.")) ;
#ifdef PDS_OLD_SCHEMA
	ACS_COND_THROW(!_packageSet,InvalidUse("Mandatory field \"Package\" must be set.")) ;
	ACS_COND_THROW(!_onlineLatencyTimeSet,InvalidUse("Mandatory field \"Online Latency Time (Days)\" must be set.")) ;
	ACS_COND_THROW(!_storageSet,InvalidUse("Mandatory field \"Storage\" must be set.")) ;
	ACS_COND_THROW(!_overallLifeTimeSet,InvalidUse("Mandatory field \"Overall Life Time (Days)\" must be set.")) ;
	ACS_COND_THROW(!_storeOnTapeSet,InvalidUse("Mandatory field \"Store on Tape\" must be set.")) ;
#endif
	ACS_COND_THROW(!_mngSngExtSet,InvalidUse("Mandatory field \"Manage Single Ext\" must be set.")) ;

    const string &dbKey = conn.getKey() ;

	// inside a transaction
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbFileType_SAVE") ;
	conn.addStateWrapper(*this) ;

    SQLString queryString(dbKey, SQLString::INSERT) ;
    
	dbQuery query(conn) ;



    queryString.addTableList(getTable()) ;
    queryString.addValuesList("FileType" , db::toSQL(dbKey,_fileType)) ;
#ifdef PDS_OLD_SCHEMA
	queryString.addValuesList("UserFileType" , db::toSQL(dbKey,_userFileType)) ;
#endif
	if(_descriptionSet)
		queryString.addValuesList("Description" , db::toSQL(dbKey,_description)) ;
    if (_nameRegularExpressionSet)
	    queryString.addValuesList("NameRegularExpression" , db::toSQL(dbKey,_nameRegularExpression)) ;
#ifdef PDS_OLD_SCHEMA
	queryString.addValuesList("GenerateHeader" , db::toSQL(dbKey,_generateHeader)) ;
#endif
	queryString.addValuesList("HeaderSeparation" , db::toSQL(dbKey,_headerSeparation)) ;
#ifdef PDS_OLD_SCHEMA
	if(_headerGeneratorSet)
		queryString.addValuesList("HeaderGenerator" , db::toSQL(dbKey,_headerGenerator)) ;
	if(_inventoryExecutableSet)
		queryString.addValuesList("InventoryExecutable" , db::toSQL(dbKey,_inventoryExecutable)) ;
	if(_inventoryExecutableOptsSet)
		queryString.addValuesList("InventoryExecOpts" , db::toSQL(dbKey,_inventoryExecutableOpts)) ;
#endif
	queryString.addValuesList("Group" , db::toSQL(dbKey,_group)) ;
#ifdef PDS_OLD_SCHEMA
	queryString.addValuesList("Package" , db::toSQL(dbKey,_package)) ;
	queryString.addValuesList("OnlineLatencyTime" , db::toSQL(dbKey,_onlineLatencyTime)) ;
	if(_headerTemplatePrefixSet)
		queryString.addValuesList("HeaderTemplatePrefix" , db::toSQL(dbKey,_headerTemplatePrefix)) ;
	queryString.addValuesList("Storage" , db::toSQL(dbKey,_storage)) ;
	queryString.addValuesList("OverallLifeTime" , db::toSQL(dbKey,_overallLifeTime)) ;
	if(_preInventoryExecutableSet)
		queryString.addValuesList("PreInventoryExecutable" , db::toSQL(dbKey,_preInventoryExecutable)) ;
	if(_preInventoryExecutableOptsSet)
		queryString.addValuesList("PreInventoryExecOpts" , db::toSQL(dbKey,_preInventoryExecutableOpts)) ;
#endif
	if(_inventoryPeriodSet)
		queryString.addValuesList("invperiod" , db::toSQL(dbKey,_inventoryPeriod)) ;
#ifdef PDS_OLD_SCHEMA
	queryString.addValuesList("StoreOnTape" , db::toSQL(dbKey,_storeOnTape)) ;
#endif
	queryString.addValuesList("mngsngext" , db::toSQL(dbKey,_mngSngExt)) ;
	if(_sngExtSet)
		queryString.addValuesList("sngext" , db::toSQL(dbKey,_sngExt)) ;
	if(_hdrExtSet)
		queryString.addValuesList("hdrext" , db::toSQL(dbKey,_hdrExt)) ;
	if(_dblExtSet)
		queryString.addValuesList("dblext" , db::toSQL(dbKey,_dblExt)) ;


	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL command : " << queryString.getSQL()) ;

	query.sqlString(queryString, "dbFileType::save") ;

	if (!query.execForStatus())
	{
			ACS_THROW(dbQuery::QueryExecuteException("Query Execution error: Command : " // PRQA S 3081
			          + queryString.getSQL() + " Error : " + query.errorString())) ;
	}

    transaction.commit() ;
	// record that the values come from the database
	setFromDatabase() ; 
	return 0 ;
}


// load the class from the database. Return true if object exist
bool dbFileType::load(dbConnection &conn , IdType id)
{
	Lock lock(*this) ;

	dbFileType tempRecord(*this); //store current record.


	reset() ; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called") ;
    const string &dbKey = conn.getKey() ;

    SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addSelectList(getFields()) ;
    queryString.addTableList(getTable()) ;

    dbQueryParameters pars(dbKey) ;
    if(tempRecord._fileTypeSet) {
        pars.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::EQUAL_TO,db::toSQL(dbKey,tempRecord._fileType))) ;
    }
	else {
        pars.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::ISNULL)) ;
    }
    
    queryString.addANDWhereParameter(pars) ;

    string getOrderByString(getOrderBy()) ;
    if(!getOrderByString.empty() ) {
        queryString.setOrderByString(getOrderByString) ;
    }


	dbQuery query(conn, queryString, "dbFileType::load") ;
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

	loadRow(row, 0) ;

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	return true;
}


// update an existing instance in database
dbPersistent::IdType dbFileType::update(dbConnection &conn)
{
	Lock lock(*this) ;

	ACS_COND_THROW(!isFromDatabase(),ObjectNotLoaded("Error: dbFileType::update(). Use the method save() instead of update().")) ;

	if(!isModified()) {
		return 0 ; 
    }

	ACS_COND_THROW(!_fileTypeSet, InvalidUse("Mandatory field \"File Type\" must be set.")) ;
#ifdef PDS_OLD_SCHEMA
	ACS_COND_THROW(!_userFileTypeSet, InvalidUse("Mandatory field \"User File Type\" must be set.")) ;
	ACS_COND_THROW(!_generateHeaderSet, InvalidUse("Mandatory field \"Generate Header\" must be set.")) ;
#endif
	ACS_COND_THROW(!_headerSeparationSet, InvalidUse("Mandatory field \"Header Separation\" must be set.")) ;
	ACS_COND_THROW(!_groupSet, InvalidUse("Mandatory field \"Group\" must be set.")) ;
#ifdef PDS_OLD_SCHEMA
	ACS_COND_THROW(!_packageSet, InvalidUse("Mandatory field \"Package\" must be set.")) ;
	ACS_COND_THROW(!_onlineLatencyTimeSet, InvalidUse("Mandatory field \"Online Latency Time (Days)\" must be set.")) ;
	ACS_COND_THROW(!_storageSet, InvalidUse("Mandatory field \"Storage\" must be set.")) ;
	ACS_COND_THROW(!_overallLifeTimeSet, InvalidUse("Mandatory field \"Overall Life Time (Days)\" must be set.")) ;
	ACS_COND_THROW(!_storeOnTapeSet, InvalidUse("Mandatory field \"Store on Tape\" must be set.")) ;
#endif
	ACS_COND_THROW(!_mngSngExtSet,InvalidUse("Mandatory field \"Manage Single Ext\" must be set.")) ;

    const string &dbKey = conn.getKey() ;
    SQLString queryString(dbKey, SQLString::UPDATE) ;
    queryString.addTableList( getTable()) ;

#ifdef PDS_OLD_SCHEMA
	queryString.addSetList("UserFileType", db::toSQL(dbKey,_userFileType)) ;
#endif
	if(_descriptionSet) {
		queryString.addSetList("Description", db::toSQL(dbKey,_description)) ;
    }
	else {
		queryString.addSetList("Description","NULL") ; 
    }
        
    if (_nameRegularExpressionSet) {
	    queryString.addSetList("NameRegularExpression", db::toSQL(dbKey,_nameRegularExpression)) ;
    }
	else {
		queryString.addSetList("NameRegularExpression","NULL") ; 
    }
#ifdef PDS_OLD_SCHEMA
	queryString.addSetList("GenerateHeader", db::toSQL(dbKey,_generateHeader)) ;
#endif
	queryString.addSetList("HeaderSeparation", db::toSQL(dbKey,_headerSeparation)) ;

#ifdef PDS_OLD_SCHEMA
	if(_headerGeneratorSet)
		queryString.addSetList("HeaderGenerator", db::toSQL(dbKey,_headerGenerator)) ;
	else
		queryString.addSetList("HeaderGenerator","NULL") ; 
	if(_inventoryExecutableSet)
		queryString.addSetList("InventoryExecutable", db::toSQL(dbKey,_inventoryExecutable)) ;
	else
		queryString.addSetList("InventoryExecutable","NULL") ; 
	if(_inventoryExecutableOptsSet)
		queryString.addSetList("InventoryExecOpts", db::toSQL(dbKey,_inventoryExecutableOpts)) ;
	else
		queryString.addSetList("InventoryExecOpts","NULL") ; 
#endif
	queryString.addSetList("Group", db::toSQL(dbKey,_group)) ;
#ifdef PDS_OLD_SCHEMA
	queryString.addSetList("Package", db::toSQL(dbKey,_package)) ;
	queryString.addSetList("OnlineLatencyTime", db::toSQL(dbKey,_onlineLatencyTime)) ;
	if(_headerTemplatePrefixSet)
		queryString.addSetList("HeaderTemplatePrefix", db::toSQL(dbKey,_headerTemplatePrefix)) ;
	else
		queryString.addSetList("HeaderTemplatePrefix","NULL") ; 
	queryString.addSetList("Storage", db::toSQL(dbKey,_storage)) ;
	queryString.addSetList("OverallLifeTime", db::toSQL(dbKey,_overallLifeTime)) ;
	if(_preInventoryExecutableSet)
		queryString.addSetList("PreInventoryExecutable", db::toSQL(dbKey,_preInventoryExecutable)) ;
	else
		queryString.addSetList("PreInventoryExecutable","NULL") ; 
	if(_preInventoryExecutableOptsSet)
		queryString.addSetList("PreInventoryExecOpts", db::toSQL(dbKey,_preInventoryExecutableOpts)) ;
	else
		queryString.addSetList("PreInventoryExecOpts","NULL") ; 
#endif
	if(_inventoryPeriodSet)
		queryString.addSetList("invperiod", db::toSQL(dbKey,_inventoryPeriod)) ;
	else
		queryString.addSetList("invperiod","NULL") ; 
#ifdef PDS_OLD_SCHEMA
	queryString.addSetList("StoreOnTape", db::toSQL(dbKey,_storeOnTape)) ;
#endif

	queryString.addSetList("mngsngext", db::toSQL(dbKey,_mngSngExtSet)) ;

	queryString.addSetList("sngext", not _sngExtSet ? string("NULL") : string(db::toSQL(dbKey,_sngExt))) ;
	queryString.addSetList("hdrext", not _hdrExtSet ? string("NULL") : string(db::toSQL(dbKey,_hdrExt))) ;
	queryString.addSetList("dblext", not _dblExtSet ? string("NULL") : string(db::toSQL(dbKey,_dblExt))) ;

    dbQueryParameters pars(dbKey) ;

  	if(_fileTypeSet) {
        pars.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::EQUAL_TO,db::toSQL(dbKey,_fileType))) ;
    }
	else {
        pars.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::ISNULL)) ;
    }

    queryString.addANDWhereParameter(pars) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL()) ;

	// inside a transaction
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbFileType::update") ;
 
	conn.addStateWrapper(*this) ;
	dbQuery query(conn, queryString, "dbFileType_UPDATE") ;

    query.execForStatus() ;

	transaction.commit() ;

	resetModifiedFlag() ;
	return 0 ;

}


// remove the class from the database
dbPersistent::IdType dbFileType::remove(dbConnection &conn)
{
	Lock lock(*this) ;

    const string &dbKey = conn.getKey() ;
    SQLString queryString(dbKey, SQLString::DELETE) ;
    queryString.addTableList(getTable()) ;

    dbQueryParameters pars(dbKey) ;
 	
  	if(_fileTypeSet)
        pars.addParameter(dbParam(dbKey,"t_filetypes","filetype", dbParam::EQUAL_TO,db::toSQL(dbKey,_fileType))) ;
	else
        pars.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::ISNULL)) ;

    queryString.addANDWhereParameter(pars) ;    
    
	// inside a transaction
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbFileType::remove") ;
  
  	conn.addStateWrapper(*this) ;
	dbQuery query(conn, queryString, "dbFileType_REMOVE") ;

    query.execForStatus() ;
    transaction.commit() ;


	resetFromDatabase() ;
	setModifiedFlag() ;

	return 0 ;
}


// True if the class have valid parameters, false otherwise
bool dbFileType::validate() const
{
	Lock lock(*this) ;

	return true;
}


// export the class to stream
void dbFileType::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbFileType") ;
	dbPersistent::exportObj(s) ;
	if(_fileTypeSet)
	{
		s.setValue("_fileType",_fileType) ;
	}
	s.setValue("_fileTypeSet",_fileTypeSet) ;

#ifdef PDS_OLD_SCHEMA
	if(_userFileTypeSet)
	{
		s.setValue("_userFileType",_userFileType) ;
	}
	s.setValue("_userFileTypeSet",_userFileTypeSet) ;
#endif
	if(_descriptionSet)
	{
		s.setValue("_description",_description) ;
	}
	s.setValue("_descriptionSet",_descriptionSet) ;

	if(_nameRegularExpressionSet)
	{
		s.setValue("_nameRegularExpression",_nameRegularExpression) ;
	}
	s.setValue("_nameRegularExpressionSet",_nameRegularExpressionSet) ;

#ifdef PDS_OLD_SCHEMA
	if(_generateHeaderSet)
	{
		s.setValue("_generateHeader",_generateHeader) ;
	}
	s.setValue("_generateHeaderSet",_generateHeaderSet) ;
#endif
	if(_headerSeparationSet)
	{
		s.setValue("_headerSeparation",_headerSeparation) ;
	}
	s.setValue("_headerSeparationSet",_headerSeparationSet) ;

#ifdef PDS_OLD_SCHEMA
	if(_headerGeneratorSet)
	{
		s.setValue("_headerGenerator",_headerGenerator) ;
	}
	s.setValue("_headerGeneratorSet",_headerGeneratorSet) ;
	if(_inventoryExecutableSet)
	{
		s.setValue("_inventoryExecutable",_inventoryExecutable) ;
	}
	s.setValue("_inventoryExecutableSet",_inventoryExecutableSet) ;
	if(_inventoryExecutableOptsSet)
	{
		s.setValue("_inventoryExecutableOpts",_inventoryExecutableOpts) ;
	}
	s.setValue("_inventoryExecutableOptsSet",_inventoryExecutableOptsSet) ;
#endif
	if(_groupSet)
	{
		s.setValue("_group",_group) ;
	}
	s.setValue("_groupSet",_groupSet) ;
#ifdef PDS_OLD_SCHEMA
	if(_packageSet)
	{
		s.setValue("_package",_package) ;
	}
	s.setValue("_packageSet",_packageSet) ;
	if(_onlineLatencyTimeSet)
	{
		s.setValue("_onlineLatencyTime",_onlineLatencyTime) ;
	}
	s.setValue("_onlineLatencyTimeSet",_onlineLatencyTimeSet) ;
	if(_headerTemplatePrefixSet)
	{
		s.setValue("_headerTemplatePrefix",_headerTemplatePrefix) ;
	}
	s.setValue("_headerTemplatePrefixSet",_headerTemplatePrefixSet) ;
	if(_storageSet)
	{
		s.setValue("_storage",_storage) ;
	}
	s.setValue("_storageSet",_storageSet) ;
	if(_overallLifeTimeSet)
	{
		s.setValue("_overallLifeTime",_overallLifeTime) ;
	}
	s.setValue("_overallLifeTimeSet",_overallLifeTimeSet) ;
	if(_preInventoryExecutableSet)
	{
		s.setValue("_preInventoryExecutable",_preInventoryExecutable) ;
	}
	s.setValue("_preInventoryExecutableSet",_preInventoryExecutableSet) ;
	if(_preInventoryExecutableOptsSet)
	{
		s.setValue("_preInventoryExecutableOpts",_preInventoryExecutableOpts) ;
	}
	s.setValue("_preInventoryExecutableOptsSet",_preInventoryExecutableOptsSet) ;
	if(_storeOnTapeSet)
	{
		s.setValue("_storeOnTape",_storeOnTape) ;
	}
	s.setValue("_storeOnTapeSet",_storeOnTapeSet) ;

#endif
	if(_inventoryPeriodSet)
	{
		s.setValue("_inventoryPeriod",_inventoryPeriod) ;
	}
	s.setValue("_inventoryPeriodSet",_inventoryPeriodSet) ;

	if(_mngSngExtSet) s.setValue("_mngSngExt",_mngSngExt) ;
	s.setValue("_mngSngExtSet",_mngSngExtSet) ;

	if(_sngExtSet) s.setValue("_sngExt",_sngExt) ;
	s.setValue("_sngExtSet",_sngExtSet) ;

	if(_hdrExtSet) s.setValue("_hdrExt",_hdrExt) ;
	s.setValue("_hdrExtSet",_hdrExtSet) ;

	if(_dblExtSet) s.setValue("_dblExt",_dblExt) ;
	s.setValue("_dblExtSet",_dblExtSet) ;
}


// import the class from a stream
void dbFileType::importObj(rsResourceSet &s)
{
	Lock lock(*this) ;

	rsPushNSpace nsp(s,"dbFileType") ;
	dbPersistent::importObj(s) ;
	reset() ;
	s.getValue("_fileTypeSet",_fileTypeSet) ;
	if(_fileTypeSet)
	{
		s.getValue("_fileType",_fileType) ;
	}
	else
		_fileType = "" ;

#ifdef PDS_OLD_SCHEMA
	s.getValue("_userFileTypeSet",_userFileTypeSet) ;
	if(_userFileTypeSet)
	{
		s.getValue("_userFileType",_userFileType) ;
	}
	else
		_userFileType = "" ;
#endif
	s.getValue("_descriptionSet",_descriptionSet) ;
	if(_descriptionSet)
	{
		s.getValue("_description",_description) ;
	}
	else
		_description = "" ;
	s.getValue("_nameRegularExpressionSet",_nameRegularExpressionSet) ;
	if(_nameRegularExpressionSet)
	{
		s.getValue("_nameRegularExpression",_nameRegularExpression) ;
	}
	else
		_nameRegularExpression = "" ;
#ifdef PDS_OLD_SCHEMA

	s.getValue("_generateHeaderSet",_generateHeaderSet) ;
	if(_generateHeaderSet)
	{
		s.getValue("_generateHeader",_generateHeader) ;
	}
	else
		_generateHeader = false ;

#endif
	s.getValue("_headerSeparationSet",_headerSeparationSet) ;
	if(_headerSeparationSet)
	{
		s.getValue("_headerSeparation",_headerSeparation) ;
	}
	else
		_headerSeparation = false ;

#ifdef PDS_OLD_SCHEMA
	s.getValue("_headerGeneratorSet",_headerGeneratorSet) ;
	if(_headerGeneratorSet)
	{
		s.getValue("_headerGenerator",_headerGenerator) ;
	}
	else
		_headerGenerator = "" ;
	s.getValue("_inventoryExecutableSet",_inventoryExecutableSet) ;
	if(_inventoryExecutableSet)
	{
		s.getValue("_inventoryExecutable",_inventoryExecutable) ;
	}
	else
		_inventoryExecutable = "" ;
	s.getValue("_inventoryExecutableOptsSet",_inventoryExecutableOptsSet) ;
	if(_inventoryExecutableOptsSet)
	{
		s.getValue("_inventoryExecutableOpts",_inventoryExecutableOpts) ;
	}
	else
		_inventoryExecutableOpts = "" ;
#endif

	s.getValue("_groupSet",_groupSet) ;
	if(_groupSet)
	{
		s.getValue("_group",_group) ;
	}
	else
		_group = "" ;

#ifdef PDS_OLD_SCHEMA
	s.getValue("_packageSet",_packageSet) ;
	if(_packageSet)
	{
		s.getValue("_package",_package) ;
	}
	else
		_package = false ;
	s.getValue("_onlineLatencyTimeSet",_onlineLatencyTimeSet) ;
	if(_onlineLatencyTimeSet)
	{
		s.getValue("_onlineLatencyTime",_onlineLatencyTime) ;
	}
	else
		_onlineLatencyTime = 0 ;
	s.getValue("_headerTemplatePrefixSet",_headerTemplatePrefixSet) ;
	if(_headerTemplatePrefixSet)
	{
		s.getValue("_headerTemplatePrefix",_headerTemplatePrefix) ;
	}
	else
		_headerTemplatePrefix = "" ;
	s.getValue("_storageSet",_storageSet) ;
	if(_storageSet)
	{
		s.getValue("_storage",_storage) ;
	}
	else
		_storage = "" ;
	s.getValue("_overallLifeTimeSet",_overallLifeTimeSet) ;
	if(_overallLifeTimeSet)
	{
		s.getValue("_overallLifeTime",_overallLifeTime) ;
	}
	else
		_overallLifeTime = 0 ;
	s.getValue("_preInventoryExecutableSet",_preInventoryExecutableSet) ;
	if(_preInventoryExecutableSet)
	{
		s.getValue("_preInventoryExecutable",_preInventoryExecutable) ;
	}
	else
		_preInventoryExecutable = "" ;
	s.getValue("_preInventoryExecutableOptsSet",_preInventoryExecutableOptsSet) ;
	if(_preInventoryExecutableOptsSet)
	{
		s.getValue("_preInventoryExecutableOpts",_preInventoryExecutableOpts) ;
	}
	else
		_preInventoryExecutableOpts = "" ;

	s.getValue("_storeOnTapeSet",_storeOnTapeSet) ;
	if(_storeOnTapeSet)
	{
		s.getValue("_storeOnTape",_storeOnTape) ;
	}
	else
		_storeOnTape = true ;

#endif
	s.getValue("_inventoryPeriodSet",_inventoryPeriodSet) ;
	if(_inventoryPeriodSet)
	{
		s.getValue("_inventoryPeriod",_inventoryPeriod) ;
	}
	else
		_inventoryPeriod = 0 ;

	s.getValue("_mngSngExtSet",_mngSngExtSet) ;
	if(_mngSngExtSet) 
		s.getValue("_mngSngExt",_mngSngExt) ;
	else
		_mngSngExt = true ;

	s.getValue("_sngExtSet",_sngExtSet) ;
	if(_sngExtSet)	s.getValue("_sngExt",_sngExt) ;
	else _sngExt = "" ;

	s.getValue("_hdrExtSet",_hdrExtSet) ;
	if(_hdrExtSet)	s.getValue("_hdrExt",_hdrExt) ;
	else _hdrExt = "" ;

	s.getValue("_dblExtSet",_dblExtSet) ;
	if(_dblExtSet)	s.getValue("_dblExt",_dblExt) ;
	else _dblExt = "" ;
}


// export the class to XML stream
void dbFileType::exportXML(XMLOstream &xst) const
{
	Lock lock(*this) ;
    const string dbKey = "" ;

	XMLOstream::Tag txml(xst, "dbFileType");
	dbPersistent::exportXML(xst) ;
	if(_fileTypeSet)
	{
	    XMLOstream::Tag txml0(xst, "_fileType", _fileType, "label=\"File Type\"");
	}
	else
	{
	    XMLOstream::Tag txml0(xst, "_fileType", string("<null>"), "label=\"File Type\"");
	}
	{
	    XMLOstream::Tag txml0Set(xst, "_fileTypeSet", (_fileTypeSet ? "true" : "false")) ;
	}
#ifdef PDS_OLD_SCHEMA
	if(_userFileTypeSet)
	{
    	XMLOstream::Tag txml1(xst, "_userFileType", _userFileType, "label=\"User File Type\"");
	}
	else
	{
	    XMLOstream::Tag txml1(xst, "_userFileType", string("<null>"), "label=\"User File Type\"");
	}
	{
	    XMLOstream::Tag txml1Set(xst, "_userFileTypeSet", (_userFileTypeSet ? "true" : "false")) ;
	}
#endif
	if(_descriptionSet)
	{
	    XMLOstream::Tag txml2(xst, "_description", _description, "label=\"Description\"");
	}
	else
	{
	    XMLOstream::Tag txml2(xst, "_description", string("<null>"), "label=\"Description\"");
	}
	{
	    XMLOstream::Tag txml2Set(xst, "_descriptionSet", (_descriptionSet ? "true" : "false")) ;
	}
	if(_nameRegularExpressionSet)
	{
	    XMLOstream::Tag txml3(xst, "_nameRegularExpression", _nameRegularExpression, "label=\"Name Regular Expression\"");
	}
	else
	{
	    XMLOstream::Tag txml3(xst, "_nameRegularExpression", string("<null>"), "label=\"Name Regular Expression\"");
	}
	{
	    XMLOstream::Tag txml3Set(xst, "_nameRegularExpressionSet", (_nameRegularExpressionSet ? "true" : "false")) ;
	}
#ifdef PDS_OLD_SCHEMA
	if(_generateHeaderSet)
	{
	    XMLOstream::Tag txml4(xst, "_generateHeader", (_generateHeader ? "true" : "false"), "label=\"Generate Header\"");
	}
	else
	{
	    XMLOstream::Tag txml4(xst, "_generateHeader", string("<null>"), "label=\"Generate Header\"");
	}
	{
	    XMLOstream::Tag txml4Set(xst, "_generateHeaderSet", (_generateHeaderSet ? "true" : "false")) ;
	}
#endif
	if(_headerSeparationSet)
	{
	    XMLOstream::Tag txml5(xst, "_headerSeparation", (_headerSeparation ? "true" : "false"), "label=\"Header Separation\"");
	}
	else
	{
	    XMLOstream::Tag txml5(xst, "_headerSeparation", string("<null>"), "label=\"Header Separation\"");
	}
	{
	    XMLOstream::Tag txml5Set(xst, "_headerSeparationSet", (_headerSeparationSet ? "true" : "false")) ;
	}
#ifdef PDS_OLD_SCHEMA
	if(_headerGeneratorSet)
	{
	    XMLOstream::Tag txml6(xst, "_headerGenerator", _headerGenerator, "label=\"Header Generator\"");
	}
	else
	{
	    XMLOstream::Tag txml6(xst, "_headerGenerator", string("<null>"), "label=\"Header Generator\"");
	}
	{
	    XMLOstream::Tag txml6Set(xst, "_headerGeneratorSet", (_headerGeneratorSet ? "true" : "false")) ;
	}
	if(_inventoryExecutableSet)
	{
	    XMLOstream::Tag txml7(xst, "_inventoryExecutable", _inventoryExecutable, "label=\"Inventory Executable\"");
	}
	else
	{
	    XMLOstream::Tag txml7(xst, "_inventoryExecutable", string("<null>"), "label=\"Inventory Executable\"");
	}
	{
	    XMLOstream::Tag txml7Set(xst, "_inventoryExecutableSet", (_inventoryExecutableSet ? "true" : "false")) ;
	}


	if(_inventoryExecutableOptsSet)
	{
	    XMLOstream::Tag txml7(xst, "_inventoryExecutableOpts", _inventoryExecutableOpts, "label=\"Inventory Executable Options\"");
	}
	else
	{
	    XMLOstream::Tag txml7(xst, "_inventoryExecutableOpts", string("<null>"), "label=\"Inventory Executable Options\"");
	}
	{
	    XMLOstream::Tag txml7Set(xst, "_inventoryExecutableOptsSet", (_inventoryExecutableOptsSet ? "true" : "false")) ;
	}
#endif
	if(_groupSet)
	{
	    XMLOstream::Tag txml8(xst, "_group", _group, "label=\"Group\"");
	}
	else
	{
	    XMLOstream::Tag txml8(xst, "_group", string("<null>"), "label=\"Group\"");
	}
	{
	    XMLOstream::Tag txml8Set(xst, "_groupSet", (_groupSet ? "true" : "false")) ;
	}

#ifdef PDS_OLD_SCHEMA
	if(_packageSet)
	{
	    XMLOstream::Tag txml9(xst, "_package", (_package ? "true" : "false"), "label=\"Package\"");
	}
	else
	{
	    XMLOstream::Tag txml9(xst, "_package", string("<null>"), "label=\"Package\"");
	}
	{
	    XMLOstream::Tag txml9Set(xst, "_packageSet", (_packageSet ? "true" : "false")) ;
	}
	if(_onlineLatencyTimeSet)
	{
	    XMLOstream::Tag txml10(xst, "_onlineLatencyTime", string(db::toSQL(dbKey,_onlineLatencyTime)), "label=\"Online Latency Time (Days)\"");
	}
	else
	{
	    XMLOstream::Tag txml10(xst, "_onlineLatencyTime", string("<null>"), "label=\"Online Latency Time (Days)\"");
	}
	{
	    XMLOstream::Tag txml10Set(xst, "_onlineLatencyTimeSet", (_onlineLatencyTimeSet ? "true" : "false")) ;
	}
	if(_headerTemplatePrefixSet)
	{
	    XMLOstream::Tag txml11(xst, "_headerTemplatePrefix", _headerTemplatePrefix, "label=\"Header Template Prefix\"");
	}
	else
	{
	    XMLOstream::Tag txml11(xst, "_headerTemplatePrefix", string("<null>"), "label=\"Header Template Prefix\"");
	}
	{
	    XMLOstream::Tag txml11Set(xst, "_headerTemplatePrefixSet", (_headerTemplatePrefixSet ? "true" : "false")) ;
	}
	if(_storageSet)
	{
	    XMLOstream::Tag txml12(xst, "_storage", _storage, "label=\"Storage\"");
	}
	else
	{
	    XMLOstream::Tag txml12(xst, "_storage", string("<null>"), "label=\"Storage\"");
	}
	{
	    XMLOstream::Tag txml12Set(xst, "_storageSet", (_storageSet ? "true" : "false")) ;
	}
	if(_overallLifeTimeSet)
	{
	    XMLOstream::Tag txml13(xst, "_overallLifeTime", string(db::toSQL(dbKey,_overallLifeTime)), "label=\"Overall Life Time (Days)\"");
	}
	else
	{
	    XMLOstream::Tag txml13(xst, "_overallLifeTime", string("<null>"), "label=\"Overall Life Time (Days)\"");
	}
	{
	    XMLOstream::Tag txml13Set(xst, "_overallLifeTimeSet", (_overallLifeTimeSet ? "true" : "false")) ;
	}
	if(_preInventoryExecutableSet)
	{
	    XMLOstream::Tag txml14(xst, "_preInventoryExecutable", _preInventoryExecutable, "label=\"Pre-Inventory Executable\"");
	}
	else
	{
	    XMLOstream::Tag txml14(xst, "_preInventoryExecutable", string("<null>"), "label=\"Pre-Inventory Executable\"");
	}
	{
	    XMLOstream::Tag txml14Set(xst, "_preInventoryExecutableSet", (_preInventoryExecutableSet ? "true" : "false")) ;
	}

	if(_preInventoryExecutableOptsSet)
	{
	    XMLOstream::Tag txml14(xst, "_preInventoryExecutableOpts", _preInventoryExecutableOpts, "label=\"Pre-Inventory Executable Options\"");
	}
	else
	{
	    XMLOstream::Tag txml14(xst, "_preInventoryExecutableOpts", string("<null>"), "label=\"Pre-Inventory Executable Options\"");
	}
	{
	    XMLOstream::Tag txml14Set(xst, "_preInventoryExecutableOptsSet", (_preInventoryExecutableOptsSet ? "true" : "false")) ;
	}


	if(_storeOnTapeSet)
	{
	    XMLOstream::Tag txml15(xst, "_storeOnTape", (_storeOnTape ? "true" : "false"), "label=\"Store on Tape\"");
	}
	else
	{
	    XMLOstream::Tag txml15(xst, "_storeOnTape", string("<null>"), "label=\"Store on Tape\"");
	}
	{
	    XMLOstream::Tag txml15Set(xst, "_storeOnTapeSet", (_storeOnTapeSet ? "true" : "false")) ;
	}
#endif	
	if(_inventoryPeriodSet)
	{
	XMLOstream::Tag txml15(xst, "_inventoryPeriod", string(db::toSQL(dbKey,_inventoryPeriod)), "label=\"Inventory Period\"");
	}
	else
	{
	XMLOstream::Tag txml15(xst, "_inventoryPeriod", string("<null>"), "label=\"Inventory Period\"");
	}
	{
	XMLOstream::Tag txml15Set(xst, "_inventoryPeriodSet", (_inventoryPeriodSet ? "true" : "false")) ;
	}
	{ XMLOstream::Tag t(xst, "_mngSngExt", _mngSngExtSet ? (_mngSngExt ? string("true") : string("false")) : string("<null>"), "label=\"Manage Single Ext\"");	}
	{ XMLOstream::Tag t(xst, "_mngSngExtSet", (_mngSngExtSet ? "true" : "false")) ;	}

	{ XMLOstream::Tag t(xst, "_sngExt", _sngExtSet ? _sngExt : string("<null>"), "label=\"Single Ext\""); }
	{ XMLOstream::Tag t(xst, "_sngExtSet", (_sngExtSet ? "true" : "false")) ;	}

	{ XMLOstream::Tag t(xst, "_hdrExt", _hdrExtSet ? _hdrExt : string("<null>"), "label=\"HDR Ext\""); }
	{ XMLOstream::Tag t(xst, "_hdrExtSet", (_hdrExtSet ? "true" : "false")) ;	}

	{ XMLOstream::Tag t(xst, "_dblExt", _dblExtSet ? _dblExt : string("<null>"), "label=\"DBL Ext\""); }
	{ XMLOstream::Tag t(xst, "_dblExtSet", (_dblExtSet ? "true" : "false")) ;	}

}

// load the object from a row
int dbFileType::loadRow(const dbRow &row, int offset)
{
	Lock lock(*this) ;
    int int_off = 0 ;
	row.columnSet(offset +  int_off++, _fileType, _fileTypeSet) ;
#ifdef PDS_OLD_SCHEMA
	row.columnSet(offset +  int_off++, _userFileType, _userFileTypeSet) ;
#endif    
	row.columnSet(offset +  int_off++, _description, _descriptionSet) ;
	row.columnSet(offset +  int_off++, _nameRegularExpression, _nameRegularExpressionSet) ;
#ifdef PDS_OLD_SCHEMA
	row.columnSet(offset +  int_off++, _generateHeader, _generateHeaderSet) ;
#endif    
	row.columnSet(offset +  int_off++, _headerSeparation, _headerSeparationSet) ;
#ifdef PDS_OLD_SCHEMA
	row.columnSet(offset +  int_off++, _headerGenerator, _headerGeneratorSet) ;
	row.columnSet(offset +  int_off++, _inventoryExecutable, _inventoryExecutableSet) ;
	row.columnSet(offset +  int_off++, _inventoryExecutableOpts, _inventoryExecutableOptsSet) ;
#endif    
	row.columnSet(offset +  int_off++, _group, _groupSet) ;
#ifdef PDS_OLD_SCHEMA
	row.columnSet(offset +  int_off++, _package, _packageSet) ;
	row.columnSet(offset +  int_off++, _onlineLatencyTime, _onlineLatencyTimeSet) ;
	row.columnSet(offset +  int_off++, _headerTemplatePrefix, _headerTemplatePrefixSet) ;
	row.columnSet(offset +  int_off++, _storage, _storageSet) ;
	row.columnSet(offset +  int_off++, _overallLifeTime, _overallLifeTimeSet) ;
	row.columnSet(offset +  int_off++, _preInventoryExecutable, _preInventoryExecutableSet) ;
	row.columnSet(offset +  int_off++, _preInventoryExecutableOpts, _preInventoryExecutableOptsSet) ;
	row.columnSet(offset +  int_off++, _storeOnTape, _storeOnTapeSet) ;
#endif    
	row.columnSet(offset +  int_off++, _inventoryPeriod, _inventoryPeriodSet) ;
	row.columnSet(offset +  int_off++, _mngSngExt, _mngSngExtSet) ;
	row.columnSet(offset +  int_off++, _sngExt, _sngExtSet) ;
	row.columnSet(offset +  int_off++, _hdrExt, _hdrExtSet) ;
	row.columnSet(offset +  int_off++, _dblExt, _dblExtSet) ;


	setFromDatabase() ;

	return offset + int_off ;
}


void dbFileType::setFileType(const string &v)
{
	Lock lock(*this) ;

	if (isFromDatabase())
	{
		InvalidUse e("Error: dbFileType::setFileType(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	if (v.size() > 16)
	{
		InvalidUse e("Error: dbFileType::setFileType(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_fileTypeSet || _fileType != v ) 
	{
		_fileType = v ;
		_fileTypeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getFileType(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _fileTypeSet ;
	return _fileType ;
}


void dbFileType::unsetFileType()
{
	if (isFromDatabase())
	{
		InvalidUse e("Error: dbFileType::unsetFileType(). Primary Key is from Database.") ;
		ACS_THROW(e) ;
	}

	Lock lock(*this) ;
	if(_fileTypeSet == true)
		setModifiedFlag();
	_fileTypeSet = false;

}
#ifdef PDS_OLD_SCHEMA

void dbFileType::setUserFileType(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 16)
	{
		InvalidUse e("Error: dbFileType::setUserFileType(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_userFileTypeSet || _userFileType != v ) 
	{
		_userFileType = v ;
		_userFileTypeSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getUserFileType(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _userFileTypeSet ;
	return _userFileType ;
}


void dbFileType::unsetUserFileType()
{
	Lock lock(*this) ;
	if(_userFileTypeSet == true)
		setModifiedFlag();
	_userFileTypeSet = false;

}
#endif
void dbFileType::setDescription(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 64)
	{
		InvalidUse e("Error: dbFileType::setDescription(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_descriptionSet || _description != v ) 
	{
		_description = v ;
		_descriptionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getDescription(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _descriptionSet ;
	return _description ;
}


void dbFileType::unsetDescription()
{
	Lock lock(*this) ;
	if(_descriptionSet == true)
		setModifiedFlag();
	_descriptionSet = false;

}
void dbFileType::setNameRegularExpression(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 128)
	{
		InvalidUse e("Error: dbFileType::setNameRegularExpression(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_nameRegularExpressionSet || _nameRegularExpression != v ) 
	{
		_nameRegularExpression = v ;
		_nameRegularExpressionSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getNameRegularExpression(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _nameRegularExpressionSet ;
	return _nameRegularExpression ;
}


void dbFileType::unsetNameRegularExpression()
{
	Lock lock(*this) ;
	if(_nameRegularExpressionSet == true)
		setModifiedFlag();
	_nameRegularExpressionSet = false;

}

#ifdef PDS_OLD_SCHEMA

void dbFileType::setGenerateHeader(bool v)
{
	Lock lock(*this) ;

	if (!_generateHeaderSet || _generateHeader != v ) 
	{
		_generateHeader = v ;
		_generateHeaderSet = true ;
		setModifiedFlag() ;
	}
}


bool dbFileType::getGenerateHeader(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _generateHeaderSet ;
	return _generateHeader ;
}


void dbFileType::unsetGenerateHeader()
{
	Lock lock(*this) ;
	if(_generateHeaderSet == true)
		setModifiedFlag();
	_generateHeaderSet = false;

}
#endif
void dbFileType::setHeaderSeparation(bool v)
{
	Lock lock(*this) ;

	if (!_headerSeparationSet || _headerSeparation != v ) 
	{
		_headerSeparation = v ;
		_headerSeparationSet = true ;
		setModifiedFlag() ;
	}
}


bool dbFileType::getHeaderSeparation(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _headerSeparationSet ;
	return _headerSeparation ;
}


void dbFileType::unsetHeaderSeparation()
{
	Lock lock(*this) ;
	if(_headerSeparationSet == true)
		setModifiedFlag();
	_headerSeparationSet = false;

}
#ifdef PDS_OLD_SCHEMA

void dbFileType::setHeaderGenerator(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 128)
	{
		InvalidUse e("Error: dbFileType::setHeaderGenerator(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_headerGeneratorSet || _headerGenerator != v ) 
	{
		_headerGenerator = v ;
		_headerGeneratorSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getHeaderGenerator(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _headerGeneratorSet ;
	return _headerGenerator ;
}


void dbFileType::unsetHeaderGenerator()
{
	Lock lock(*this) ;
	if(_headerGeneratorSet == true)
		setModifiedFlag();
	_headerGeneratorSet = false;

}
void dbFileType::setInventoryExecutable(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 128)
	{
		InvalidUse e("Error: dbFileType::setInventoryExecutable(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_inventoryExecutableSet || _inventoryExecutable != v ) 
	{
		_inventoryExecutable = v ;
		_inventoryExecutableSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getInventoryExecutable(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _inventoryExecutableSet ;
	return _inventoryExecutable ;
}


void dbFileType::unsetInventoryExecutable()
{
	Lock lock(*this) ;
	if(_inventoryExecutableSet == true)
		setModifiedFlag();
	_inventoryExecutableSet = false;

}
void dbFileType::setInventoryExecutableOpts(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 128)
	{
		InvalidUse e("Error: dbFileType::setInventoryExecutableOpts(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_inventoryExecutableOptsSet || _inventoryExecutableOpts != v ) 
	{
		_inventoryExecutableOpts = v ;
		_inventoryExecutableOptsSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getInventoryExecutableOpts(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _inventoryExecutableOptsSet ;
	return _inventoryExecutableOpts ;
}


void dbFileType::unsetInventoryExecutableOpts()
{
	Lock lock(*this) ;
	if(_inventoryExecutableOptsSet == true)
		setModifiedFlag();
	_inventoryExecutableOptsSet = false;

}
#endif
void dbFileType::setGroup(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 32)
	{
		InvalidUse e("Error: dbFileType::setGroup(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_groupSet || _group != v ) 
	{
		_group = v ;
		_groupSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getGroup(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _groupSet ;
	return _group ;
}


void dbFileType::unsetGroup()
{
	Lock lock(*this) ;
	if(_groupSet == true)
		setModifiedFlag();
	_groupSet = false;

}
#ifdef PDS_OLD_SCHEMA

void dbFileType::setPackage(bool v)
{
	Lock lock(*this) ;

	if (!_packageSet || _package != v ) 
	{
		_package = v ;
		_packageSet = true ;
		setModifiedFlag() ;
	}
}


bool dbFileType::getPackage(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _packageSet ;
	return _package ;
}


void dbFileType::unsetPackage()
{
	Lock lock(*this) ;
	if(_packageSet == true)
		setModifiedFlag();
	_packageSet = false;

}
void dbFileType::setOnlineLatencyTime(int v)
{
	Lock lock(*this) ;

	if (!_onlineLatencyTimeSet || _onlineLatencyTime != v ) 
	{
		_onlineLatencyTime = v ;
		_onlineLatencyTimeSet = true ;
		setModifiedFlag() ;
	}
}


int dbFileType::getOnlineLatencyTime(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _onlineLatencyTimeSet ;
	return _onlineLatencyTime ;
}


void dbFileType::unsetOnlineLatencyTime()
{
	Lock lock(*this) ;
	if(_onlineLatencyTimeSet == true)
		setModifiedFlag();
	_onlineLatencyTimeSet = false;

}
void dbFileType::setHeaderTemplatePrefix(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 64)
	{
		InvalidUse e("Error: dbFileType::setHeaderTemplatePrefix(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_headerTemplatePrefixSet || _headerTemplatePrefix != v ) 
	{
		_headerTemplatePrefix = v ;
		_headerTemplatePrefixSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getHeaderTemplatePrefix(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _headerTemplatePrefixSet ;
	return _headerTemplatePrefix ;
}


void dbFileType::unsetHeaderTemplatePrefix()
{
	Lock lock(*this) ;
	if(_headerTemplatePrefixSet == true)
		setModifiedFlag();
	_headerTemplatePrefixSet = false;

}
void dbFileType::setStorage(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 8)
	{
		InvalidUse e("Error: dbFileType::setStorage(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_storageSet || _storage != v ) 
	{
		_storage = v ;
		_storageSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getStorage(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _storageSet ;
	return _storage ;
}


void dbFileType::unsetStorage()
{
	Lock lock(*this) ;
	if(_storageSet == true)
		setModifiedFlag();
	_storageSet = false;

}
void dbFileType::setOverallLifeTime(int v)
{
	Lock lock(*this) ;

	if (!_overallLifeTimeSet || _overallLifeTime != v ) 
	{
		_overallLifeTime = v ;
		_overallLifeTimeSet = true ;
		setModifiedFlag() ;
	}
}


int dbFileType::getOverallLifeTime(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _overallLifeTimeSet ;
	return _overallLifeTime ;
}


void dbFileType::unsetOverallLifeTime()
{
	Lock lock(*this) ;
	if(_overallLifeTimeSet == true)
		setModifiedFlag();
	_overallLifeTimeSet = false;

}


void dbFileType::setPreInventoryExecutable(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 64)
	{
		InvalidUse e("Error: dbFileType::setPreInventoryExecutable(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_preInventoryExecutableSet || _preInventoryExecutable != v ) 
	{
		_preInventoryExecutable = v ;
		_preInventoryExecutableSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getPreInventoryExecutable(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _preInventoryExecutableSet ;
	return _preInventoryExecutable ;
}


void dbFileType::unsetPreInventoryExecutable()
{
	Lock lock(*this) ;
	if(_preInventoryExecutableSet == true)
		setModifiedFlag();
	_preInventoryExecutableSet = false;

}


void dbFileType::setPreInventoryExecutableOpts(const string &v)
{
	Lock lock(*this) ;

	if (v.size() > 64)
	{
		InvalidUse e("Error: dbFileType::setPreInventoryExecutableOpts(). String too long.") ;
		ACS_THROW(e) ;
	}

	if (!_preInventoryExecutableOptsSet || _preInventoryExecutableOpts != v ) 
	{
		_preInventoryExecutableOpts = v ;
		_preInventoryExecutableOptsSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getPreInventoryExecutableOpts(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _preInventoryExecutableOptsSet ;
	return _preInventoryExecutableOpts ;
}


void dbFileType::unsetPreInventoryExecutableOpts()
{
	Lock lock(*this) ;
	if(_preInventoryExecutableOptsSet == true)
		setModifiedFlag();
	_preInventoryExecutableOptsSet = false;

}
void dbFileType::setStoreOnTape(bool v)
{
	Lock lock(*this) ;

	if (!_storeOnTapeSet || _storeOnTape != v ) 
	{
		_storeOnTape = v ;
		_storeOnTapeSet = true ;
		setModifiedFlag() ;
	}
}


bool dbFileType::getStoreOnTape(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _storeOnTapeSet ;
	return _storeOnTape ;
}


void dbFileType::unsetStoreOnTape()
{
	Lock lock(*this) ;
	if(_storeOnTapeSet == true)
		setModifiedFlag();
	_storeOnTapeSet = false;

}
#endif

void dbFileType::setInventoryPeriod(int v) 
{
	Lock lock(*this) ;

	if (!_inventoryPeriodSet || _inventoryPeriod != v ) 
	{
		_inventoryPeriod = v ;
		_inventoryPeriodSet = true ;
		setModifiedFlag() ;
	}
}

bool dbFileType::getInventoryPeriod(bool *set) const 
{
	Lock lock(*this) ;

	if (set)
		*set = _inventoryPeriodSet ;
	return _inventoryPeriod ;
}

void dbFileType::unsetInventoryPeriod() 
{
	Lock lock(*this) ;
	if(_inventoryPeriodSet == true)
		setModifiedFlag();
	_inventoryPeriodSet = false ;
}



void dbFileType::setMngSngExt(bool v)
{
	Lock lock(*this) ;

	if (!_mngSngExtSet || _mngSngExt != v ) 
	{
		_mngSngExt = v ;
		_mngSngExtSet = true ;
		setModifiedFlag() ;
	}
}

bool dbFileType::getMngSngExt(bool *set) const
{
	Lock lock(*this) ;

	if (set)
		*set = _mngSngExtSet ;
	return _mngSngExt ;
}

void dbFileType::unsetMngSngExt()
{
	Lock lock(*this) ;
	if(_mngSngExtSet == true)
		setModifiedFlag();
	_mngSngExtSet = false;

}

void dbFileType::setSngExt(const string &v)
{
	Lock lock(*this) ;

	ACS_COND_THROW(v.size()>8, InvalidUse("Error: dbFileType::setSngExt(). String too long (>8).")) ;

	if (!_sngExtSet || _sngExt != v ) 
	{
		_sngExt = v ;
		_sngExtSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbFileType::getSngExt(bool *set) const 
{
	Lock lock(*this) ;

	if (set)
		*set = _sngExtSet ;
	return _sngExt ;
}

void dbFileType::unsetSngExt()
{
	Lock lock(*this) ;
	if(_sngExtSet == true) {
		setModifiedFlag();
    }
	_sngExtSet = false;

}


void dbFileType::setHdrExt(const string &v)
{
	Lock lock(*this) ;

	ACS_COND_THROW(v.size()>8, InvalidUse("Error: dbFileType::setHdrExt(). String too long (>8).")) ;

	if (!_hdrExtSet || _hdrExt != v ) 
	{
		_hdrExt = v ;
		_hdrExtSet = true ;
		setModifiedFlag() ;
	}
}


const string &dbFileType::getHdrExt(bool *set) const 
{
	Lock lock(*this) ;

	if (set) { *set = _hdrExtSet ; }
	return _hdrExt ;
}

void dbFileType::unsetHdrExt()
{
	Lock lock(*this) ;
	if(_hdrExtSet == true) {
		setModifiedFlag();
    
    }
	_hdrExtSet = false;

}


void dbFileType::setDblExt(const string &v)
{
	Lock lock(*this) ;

	ACS_COND_THROW(v.size()>8, InvalidUse("Error: dbFileType::setDblExt(). String too long (>8).")) ;

	if (!_dblExtSet || _dblExt != v ) 
	{
		_dblExt = v ;
		_dblExtSet = true ;
		setModifiedFlag() ;
	}
}

const string &dbFileType::getDblExt(bool *set) const 
{
	Lock lock(*this) ;

	if (set)
		*set = _dblExtSet ;
	return _dblExt ;
}

void dbFileType::unsetDblExt()
{
	Lock lock(*this) ;
	if(_dblExtSet == true)
		setModifiedFlag();
	_dblExtSet = false;

}




// get the field names
string dbFileType::getFields()
{
	string s ;
	s += getTable() + "." + "FileType, ";
#ifdef PDS_OLD_SCHEMA
	s += getTable() + "." + "UserFileType, ";
#endif
	s += getTable() + "." + "Description, ";
	s += getTable() + "." + "NameRegularExpression, ";
#ifdef PDS_OLD_SCHEMA
	s += getTable() + "." + "GenerateHeader, ";
#endif
	s += getTable() + "." + "HeaderSeparation, ";
#ifdef PDS_OLD_SCHEMA
	s += getTable() + "." + "HeaderGenerator, ";
	s += getTable() + "." + "InventoryExecutable, ";
	s += getTable() + "." + "InventoryExecOpts, ";
#endif
	s += getTable() + "." + "Group, ";
#ifdef PDS_OLD_SCHEMA
	s += getTable() + "." + "Package, ";
	s += getTable() + "." + "OnlineLatencyTime, ";
	s += getTable() + "." + "HeaderTemplatePrefix, ";
	s += getTable() + "." + "Storage, ";
	s += getTable() + "." + "OverallLifeTime, ";
	s += getTable() + "." + "PreInventoryExecutable, ";
	s += getTable() + "." + "PreInventoryExecOpts, ";
	s += getTable() + "." + "StoreOnTape, " ;
#endif
	s += getTable() + "." + "InvPeriod, " ;
	s += getTable() + "." + "mngsngext, " ;
	s += getTable() + "." + "sngext, " ;
	s += getTable() + "." + "hdrext, " ;
	s += getTable() + "." + "dblext" ;
	return s ;
}


// get the table name
string dbFileType::getTable()
{
	return "T_FileTypes" ;
}


// get the order by clause
string dbFileType::getOrderBy()
{
	return "" ;
}


void dbFileType::writeTo(ostream &o) const
{
	Lock lock(*this) ;
    const string dbKey = "" ;
	o
		 << "_fileType = " << (_fileTypeSet ? _fileType : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_userFileType = " << (_userFileTypeSet ? _userFileType : string("<null>")) << "\n" 
#endif
		 << "_description = " << (_descriptionSet ? _description : string("<null>")) << "\n" 
		 << "_nameRegularExpression = " << (_nameRegularExpressionSet ? _nameRegularExpression : string("<null>")) << "\n" 

#ifdef PDS_OLD_SCHEMA
		 << "_generateHeader = " << (_generateHeaderSet ? (_generateHeader ? "True" : "False") : string("<null>")) << "\n" 
#endif
		 << "_headerSeparation = " << (_headerSeparationSet ? (_headerSeparation ? "True" : "False") : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_headerGenerator = " << (_headerGeneratorSet ? _headerGenerator : string("<null>")) << "\n" 
		 << "_inventoryExecutable = " << (_inventoryExecutableSet ? _inventoryExecutable : string("<null>")) << "\n" 
		 << "_inventoryExecutableOpts = " << (_inventoryExecutableOptsSet ? _inventoryExecutableOpts : string("<null>")) << "\n" 
#endif
		 << "_group = " << (_groupSet ? _group : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_package = " << (_packageSet ? (_package ? "True" : "False") : string("<null>")) << "\n" 
		 << "_onlineLatencyTime = " << (_onlineLatencyTimeSet ? string(db::toSQL(dbKey,_onlineLatencyTime)) : string("<null>")) << "\n" 
		 << "_headerTemplatePrefix = " << (_headerTemplatePrefixSet ? _headerTemplatePrefix : string("<null>")) << "\n" 
		 << "_storage = " << (_storageSet ? _storage : string("<null>")) << "\n" 
		 << "_overallLifeTime = " << (_overallLifeTimeSet ? string(db::toSQL(dbKey,_overallLifeTime)) : string("<null>")) << "\n" 
		 << "_preInventoryExecutable = " << (_preInventoryExecutableSet ? _preInventoryExecutable : string("<null>")) << "\n" 
		 << "_preInventoryExecutableOpts = " << (_preInventoryExecutableOptsSet ? _preInventoryExecutableOpts : string("<null>")) << "\n" 
		 << "_storeOnTape = " << (_storeOnTapeSet ? (_storeOnTape ? "True" : "False") : string("<null>")) << "\n"  
#endif
		 << "_inventoryPeriod = " << (_inventoryPeriodSet ? string(db::toSQL(dbKey,_inventoryPeriod)) : string("<null>")) << "\n" 
		 << "_mngSngExtSet = " << (_mngSngExtSet ? (_mngSngExt ? "True" : "False") : string("<null>")) << "\n"  
		 << "_sngExtSet = " << (_sngExtSet ? _sngExt : string("<null>")) << "\n"  
		 << "_hdrExtSet = " << (_hdrExtSet ? _hdrExt : string("<null>")) << "\n"  
		 << "_dblExtSet = " << (_dblExtSet ? _dblExt : string("<null>")) << "\n"  
		 ;
}


void dbFileType::writeTo(exostream &o) const
{
	Lock lock(*this) ;
    const string dbKey = "" ;

	o
		 << "_fileType = " << (_fileTypeSet ? _fileType : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_userFileType = " << (_userFileTypeSet ? _userFileType : string("<null>")) << "\n" 
#endif
		 << "_description = " << (_descriptionSet ? _description : string("<null>")) << "\n" 
		 << "_nameRegularExpression = " << (_nameRegularExpressionSet ? _nameRegularExpression : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_generateHeader = " << (_generateHeaderSet ? (_generateHeader ? "True" : "False") : string("<null>")) << "\n" 
#endif
		 << "_headerSeparation = " << (_headerSeparationSet ? (_headerSeparation ? "True" : "False") : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_headerGenerator = " << (_headerGeneratorSet ? _headerGenerator : string("<null>")) << "\n" 
		 << "_inventoryExecutable = " << (_inventoryExecutableSet ? _inventoryExecutable : string("<null>")) << "\n" 
		 << "_inventoryExecutableOpts = " << (_inventoryExecutableOptsSet ? _inventoryExecutableOpts : string("<null>")) << "\n" 
#endif
		 << "_group = " << (_groupSet ? _group : string("<null>")) << "\n" 
#ifdef PDS_OLD_SCHEMA
		 << "_package = " << (_packageSet ? (_package ? "True" : "False") : string("<null>")) << "\n" 
		 << "_onlineLatencyTime = " << (_onlineLatencyTimeSet ? string(db::toSQL(dbKey,_onlineLatencyTime)) : string("<null>")) << "\n" 
		 << "_headerTemplatePrefix = " << (_headerTemplatePrefixSet ? _headerTemplatePrefix : string("<null>")) << "\n" 
		 << "_storage = " << (_storageSet ? _storage : string("<null>")) << "\n" 
		 << "_overallLifeTime = " << (_overallLifeTimeSet ? string(db::toSQL(dbKey,_overallLifeTime)) : string("<null>")) << "\n" 
		 << "_preInventoryExecutable = " << (_preInventoryExecutableSet ? _preInventoryExecutable : string("<null>")) << "\n" 
		 << "_preInventoryExecutableOpts = " << (_preInventoryExecutableOptsSet ? _preInventoryExecutableOpts : string("<null>")) << "\n" 
		 << "_storeOnTape = " << (_storeOnTapeSet ? (_storeOnTape ? "True" : "False") : string("<null>")) << "\n"  
#endif
		 << "_inventoryPeriod = " << (_inventoryPeriodSet ? string(db::toSQL(dbKey,_inventoryPeriod)) : string("<null>")) << "\n" 
		 << "_mngSngExtSet = " << (_mngSngExtSet ? (_mngSngExt ? "True" : "False") : string("<null>")) << "\n"  
		 << "_sngExtSet = " << (_sngExtSet ? _sngExt : string("<null>")) << "\n"  
		 << "_hdrExtSet = " << (_hdrExtSet ? _hdrExt : string("<null>")) << "\n"  
		 << "_dblExtSet = " << (_dblExtSet ? _dblExt : string("<null>")) << "\n"  
		 ;
}


void dbFileType::checkPeriodicity(dbConnection &conn, 
						          const string &fname, 
								  const string &fversion, 
								  const string &ftype, 
								  const vector<string> &mailRecs,
								  size_t minExpected) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"ftype= " << ftype) ;
    const string &dbKey = conn.getKey() ;
	try {
		// check the period for the given ftype
		SQLString sqlstring(dbKey, SQLString::SELECT ) ;

    	sqlstring.setLimit(1) ;
    	sqlstring.addSelectList("invperiod"); 
    	sqlstring.addTableList("t_filetypes");
        {
            dbQueryParameters pp(dbKey) ;
	        pp.addParameter(dbParam(dbKey, "t_filetypes","filetype" ,dbParam::EQUAL_TO , db::toSQL(dbKey,ftype) )) ;
	        sqlstring.addANDWhereParameter(pp) ;
        }

		dbQuery query(conn, sqlstring, "FTypeInvPeriod") ;

		dbRow row(dbKey) ;

        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
		bool invperset ;
		int invper = 0 ;
		row.columnSet(0, invper, invperset) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"invper= " << invper) ;
		
		if (!invperset || !invper) {
			return ;
        }
		DateTime now ;
		DateTime::JD50 jdnow = now ;
		jdnow -= DateTime::JD50(invper/86400.0L) ;
		DateTime last(jdnow) ;
		
		SQLString invstring(dbKey, SQLString::SELECT ) ;
		invstring.addSelectList("COUNT(*)");
    	invstring.addTableList("t_inventory");
        {
            dbQueryParameters pp(dbKey) ;
	        pp.addParameter(dbParam(dbKey, "t_inventory","filetype" ,dbParam::EQUAL_TO , db::toSQL(dbKey,ftype) )) ;
	        invstring.addANDWhereParameter(pp) ;
        }
        {
            dbQueryParameters pp(dbKey) ;
	        pp.addParameter(dbParam(dbKey, "t_inventory","inventorydate" ,dbParam::MAJOR_OR_EQUAL_OF , db::toSQL(dbKey,last,dbConv::YearToFractionExtended) )) ;
	        invstring.addANDWhereParameter(pp) ;
        }
		dbQuery queryInv(conn, invstring, "InventoryCheckPeriod") ;

		queryInv.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
		int count = 0 ;
		bool vset ;
		row.columnSet(0, count, vset) ;
		
		if (size_t(count) >= minExpected)
			return ;
			
		exMailStream *merr = 0 ;
		
        ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"mailAddress: ") ;
			for (size_t i=0; i < mailRecs.size(); i++) {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,mailRecs[i] << " ") ;
            }
		ACS_CLASS_END_DEBUG
		
		if (not mailRecs.empty()) {
			merr = new exMailStream("noreply") ;
			merr->subjectPrefix(Application::appName().length() ?Application::appName():string("noapp") ) ;
			size_t mr_size = mailRecs.size() ;
            for (size_t i=0; i < mr_size; ++i) { 
				merr->addTo(mailRecs[i]) ;
            }
		}
		// and now ...
		//try {
			ostringstream os ; 
			os << "File " << fname << " version " << fversion << " has been received after " << invper << "[sec] after last " << ftype << " inventoried" ;
        	SQLString tmp( dbKey, SQLString::INSERT ) ;
       		tmp.addTableList("t_inventoryanomalies");


        	// Add Values
        	{
            	tmp.addValuesList("filename", db::toSQL(dbKey,fname) ) ;
            	tmp.addValuesList("fileversion", db::toSQL(dbKey,fversion) ) ;
            	tmp.addValuesList("message", db::toSQL(dbKey,os.str()) ) ;
        	}
	
			dbQuery invAnomalies(conn, tmp, "InventoryAnomalyInsert") ;

			invAnomalies.execForStatus() ;	

			//ACS_THROW(FTypePeriodicityException(os.str())) ;
		//}
		//catch(exception &x) {
			//excerr << x ;
		//}
		
		//notify the anomaly...
		excerr << os.str() << "\n";
		delete merr ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}

}

void dbFileType::getAssociatedSatellites(dbConnection &conn, const std::string &ftype, std::vector<std::string> &sats) 
{
// select s.satellitename from t_filetypes_x_satellites left join t_satellites s on s.satelliteid = t_filetypes_x_satellites.satellite_id where filetype like 'TIM%' ;
    const string &dbKey = conn.getKey() ;
	SQLString ftsel( dbKey, SQLString::SELECT ) ;
	ftsel.addSelectList("t_satellites.satellitename");
    ftsel.addTableList("t_filetypes_x_satellites ");


	// Add Join
	{
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, "t_satellites","satelliteid" , dbParam::EQUAL_TO , "t_filetypes_x_satellites.satellite_id" )) ;
	    ftsel.addJoinList(SQLDialect::LEFT,"t_satellites",pp) ;
	}

    // Build Where clause
    {
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, "t_filetypes_x_satellites","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey,ftype) )) ;
	    ftsel.addANDWhereParameter(pp) ;
    }

	dbQuery queryFt(conn, ftsel, "FtypeGetSatellites") ;
	dbSet ftSelSet ;
	queryFt.execForSet(ftSelSet) ;

    size_t FTSEL_size = ftSelSet.size() ;
	for (size_t i = 0 ; i < FTSEL_size; ++i) {
		dbRow row(dbKey) ;
		ftSelSet.getNextRow(row) ;
		string satellite ; // il filetype
		row.column(0,satellite) ;
		sats.push_back(satellite) ;
	}
}


_ACS_END_NAMESPACE
