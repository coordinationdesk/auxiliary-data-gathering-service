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
	
	Revision 5.7  2016/07/04 15:10:37  damdec
	CSGACQ-82: new getFields interface adopted.
	
	Revision 5.6  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.5  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.4  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.3  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.2  2016/03/15 15:28:27  marpas
	method to get comments if any
	
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
	
	Revision 2.14  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.13  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.12  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.11  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.10  2010/04/23 15:24:28  marpas
	reset bug fixed
	export bugs fixed
	
	Revision 2.9  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.8  2008/11/17 11:46:28  marpas
	column name changed (text into comment)
	
	Revision 2.7  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.6  2008/07/14 13:17:49  marpas
	getting rid of db_values
	
	Revision 2.5  2007/03/16 14:00:19  marpas
	work in progress
	
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


#include <dbInventoryObjectWithGapsComments.h>
#include <dbInventoryComment.h>
#include <dbInventoryCommentQuery.h>

#include <dbQueryFactoryChain.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryParameters.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
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
// class dbInventoryObjectWithGapsComments
//
//
//

string dbInventoryObjectWithGapsComments::className() const
{
	return staticClassName() ;
}


string dbInventoryObjectWithGapsComments::staticClassName()
{
	return "dbInventoryObjectWithGapsComments" ;
}




dbPersistent *dbInventoryObjectWithGapsComments::clone() const
{
	return new dbInventoryObjectWithGapsComments(*this) ;
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObjectWithGapsComments) 


// constructor
dbInventoryObjectWithGapsComments::dbInventoryObjectWithGapsComments() :
 dbInventoryObjectWithGaps(),
  _commentsNumSet(false),
 _commentsNum(0), 
 _inventoryComments(0), 
 _commentsLoaded(false),
 _comments2Save()

{
#warning MP: using empty string as db key
	_inventoryComments=dynamic_cast<dbInventoryCommentQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryCommentQuery", "")); // PRQA S 3081 2
	ACS_COND_THROW(!_inventoryComments, exIllegalValueException("dbInventoryCommentQuery dynamic cast failed")) ;
	dbInventoryObjectWithGapsComments::reset() ;
}


// copy constructor
dbInventoryObjectWithGapsComments::dbInventoryObjectWithGapsComments(const dbInventoryObjectWithGapsComments &o):
	dbInventoryObjectWithGaps(o),
   	_commentsNumSet(o._commentsNumSet),
   	_commentsNum(o._commentsNum), 
	_inventoryComments(0), 
	_commentsLoaded(false),
    _comments2Save()
{
#warning MP: using empty string as db key
	_inventoryComments=dynamic_cast<dbInventoryCommentQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryCommentQuery", "")); // PRQA S 3081 2
	ACS_COND_THROW(!_inventoryComments, exIllegalValueException("dbInventoryCommentQuery dynamic cast failed")) ;
}


dbInventoryObjectWithGapsComments::~dbInventoryObjectWithGapsComments() throw() 
{
	delete _inventoryComments;
    try {
	    deleteComments2Save() ;
    }
    catch(std::exception &x) {
        excerr.notify(x) ;
    }
}


// operator =
dbInventoryObjectWithGapsComments &dbInventoryObjectWithGapsComments::operator=(const dbInventoryObjectWithGapsComments &o)
{
	Lock lock(*this) ; // PRQA S 3050

	if(this != &o)
	{
		dbInventoryObjectWithGaps::operator=(o) ;
		_commentsNum=o._commentsNum;
		_commentsNumSet=o._commentsNumSet;
		_commentsLoaded = false ; 
        deleteComments2Save();
	}
	return *this ;
}



// load the class from the database. Return true if object exist
bool dbInventoryObjectWithGapsComments::load(dbConnection &conn, IdType id) // PRQA S 4020
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

	dbInventoryObjectWithGapsComments tempRecord(*this); //store current record.

	string oldFileName = getFileName() ;
	string oldFileVersion = getFileVersion() ;

	reset() ; 

    SQLString queryString(dbKey, SQLString::SELECT) ;
	deque<string> fields = dbInventoryObjectWithGaps::getFieldArray(conn);
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

	setFromDatabase() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this) ;

	resetModifiedFlag() ;

	return true ;
}


// initialize the class
void dbInventoryObjectWithGapsComments::reset()
{
	Lock lock(*this) ; // PRQA S 3050

	// reset dbPersistent
	dbInventoryObjectWithGaps::reset() ;
	_commentsNum=0 ;
	_commentsNumSet=false ;
	// MP we suppose the pointer is never NULL
	_inventoryComments->clear() ; 
	_commentsLoaded = false ; 
}

// save the data in database
dbPersistent::IdType dbInventoryObjectWithGapsComments::save(dbConnection &conn)
{
	
	db::Transaction transaction(conn) ;
	transaction.begin_work("dbInventoryObjectWithGapsComments_SAVE") ;

	dbInventoryObjectWithGaps::save(conn) ;
	saveComments(conn) ;
	loadComments(conn) ;
	transaction.commit() ;
	
	return 0 ;
}


// remove the class from the database
dbPersistent::IdType dbInventoryObjectWithGapsComments::remove(dbConnection &conn )
{
	return dbInventoryObjectWithGaps::remove (conn) ;
}


// export the class to stream
void dbInventoryObjectWithGapsComments::exportObj(rsResourceSet &s) const
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryObjectWithGapsComments") ;
	if(_commentsNumSet)
	{
		s.setValue("_commentsNum",_commentsNum) ;
	}
	s.setValue("_commentsNumSet",_commentsNumSet) ;
	dbInventoryObjectWithGaps::exportObj(s) ; 
}


// import the class from a stream
void dbInventoryObjectWithGapsComments::importObj(rsResourceSet &s)
{
	Lock lock(*this) ; // PRQA S 3050

	rsPushNSpace nsp(s,"dbInventoryObjectWithGapsComments") ;
	s.getValue("_commentsNumSet",_commentsNumSet) ;
	if(_commentsNumSet)
	{
		s.getValue("_commentsNum",_commentsNum) ;
	}
	else {
		_commentsNum = 0 ;
    }
	dbInventoryObjectWithGaps::importObj(s) ;
}


// export the class to XML stream
void dbInventoryObjectWithGapsComments::exportXML(XMLOstream &xst)
{
	Lock lock(*this) ; // PRQA S 3050

    // PRQA S 3380, 3385 L1
	XMLOstream::Tag txml(xst, "dbInventoryObjectWithGapsComments");
    const string K = "" ;
	{ XMLOstream::Tag t(xst, "_commentsNum", _commentsNumSet? db::toSQL(K,_commentsNum) : "0", "label=\"Comments Num\""); }
	{ XMLOstream::Tag t(xst, "_commentsNumSet", (_commentsNumSet ? "true" : "false")) ; }

    // PRQA L:L1
	dbInventoryObjectWithGaps::exportXML(xst) ; 
}


// load the object from a row
int dbInventoryObjectWithGapsComments::loadRow(dbConnection &conn, const dbRow &row, int offset)
{
    uint16_t loff = offset ; // PRQA S 3000, 3010
	try
	{
		row.column(loff++, _commentsNum) ;
		_commentsNumSet = true ;
	}
	catch (dbRow::NullValueException &)
	{
		_commentsNumSet = false ;
	}

	return dbInventoryObjectWithGaps::loadRow(conn, row, loff) ; // PRQA S 3000
}
string dbInventoryObjectWithGapsComments::getFields()
{
	return getFields(dbConnectionWrapper(db::ConnPools::instance()->getPool(""))) ; // PRQA S 3050, 3081, 4412
}

string dbInventoryObjectWithGapsComments::getFields(dbConnection& conn)
{
	deque <string> stringArray = getFieldArray(conn);
	return dbInventoryObject::convertFieldList (stringArray);
}


// get the field names
deque<string> dbInventoryObjectWithGapsComments::getFieldArray(dbConnection &conn)
{
	deque<string> s = dbInventoryObjectWithGaps::getFieldArray(conn);
	const string &dbKey = conn.getKey() ;	
	SQLString sqlString ( dbKey, SQLString::SELECT ) ;
	sqlString.addTableList("t_inventorycomments") ;
    sqlString.addSelectList( "count(*)" );
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, "t_inventorycomments", "filename", dbParam::EQUAL_TO, "t_inventory.filename") ); // PRQA S 3050, 3081
	    pp.addParameter( dbParam(dbKey, "t_inventorycomments", "fileversion", dbParam::EQUAL_TO, "t_inventory.fileversion") ); // PRQA S 3050, 3081
	    sqlString.addANDWhereParameter(pp);
	}
	s.push_front(sqlString.getSQL(true)+ "AS commentsnum");

	return s ;
}



void dbInventoryObjectWithGapsComments::writeTo(ostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_commentsNum = " << (_commentsNumSet ? _commentsNum : 0) << '\n' ; // PRQA S 3380
	dbInventoryObjectWithGaps::writeTo(o) ; 
}


void dbInventoryObjectWithGapsComments::writeTo(exostream &o) const
{
	Lock lock(*this) ; // PRQA S 3050

	o << "_commentsNum = " << (_commentsNumSet ? _commentsNum : 0) << '\n' ;  // PRQA S 3380
	dbInventoryObjectWithGaps::writeTo(o) ; 
}

unsigned long dbInventoryObjectWithGapsComments::loadComments(dbConnection &conn)
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
		
	_inventoryComments->resetParameters() ; 
	_inventoryComments->clear() ; 
	
	const string dbKey = conn.getKey() ; 
	dbQueryParameters par(dbKey) ; 
	
	dbParam fileNamePar(dbKey, "T_InventoryComments","FileName", dbParam::EQUAL_TO , db::toSQL(dbKey, fileName)) ; 		
	dbParam fileVersionPar(dbKey, "T_InventoryComments","FileVersion", dbParam::EQUAL_TO , db::toSQL(dbKey, fileVersion)) ; 			
	
	par.addParameter(fileNamePar) ; 
	par.addParameter(fileVersionPar) ; 
	
	_inventoryComments->addParameter(par) ;
	
	_commentsLoaded = true ; 
	 
	return _inventoryComments->doQuery(conn) ; 
}


void dbInventoryObjectWithGapsComments::saveComments(dbConnection &conn) // PRQA S 4020
{
	if (_comments2Save.empty()) {
		return ;
    }
	db::Transaction transaction(conn) ;
	transaction.begin_work("Comments_SAVE") ;
    size_t c2s_size = _comments2Save.size() ;
	for (size_t i=0; i < c2s_size; ++i) {
		_comments2Save[i]->save(conn) ;
    }
	deleteComments2Save() ;
	transaction.commit() ;
	

}

void dbInventoryObjectWithGapsComments::deleteComments2Save() throw() 
{
    size_t c2s_size = _comments2Save.size() ;
	for (size_t i=0; i < c2s_size; ++i) {
		delete _comments2Save[i];
    }
	_comments2Save.clear() ;
}

bool dbInventoryObjectWithGapsComments::hasComments() const throw()
{
	return _commentsNum > 0 ;
}

unsigned long dbInventoryObjectWithGapsComments::commentsNum(dbConnection &conn)
{
	if(!_commentsLoaded) {
		loadComments(conn) ; 
    }
	return _inventoryComments->size() ; 
}

dbInventoryComment * dbInventoryObjectWithGapsComments::comment(unsigned long ind, dbConnection &conn) 
{
	if(!_commentsLoaded) {
		loadComments(conn) ; 
    }
	return  (*_inventoryComments)[ind] ; 
}


void dbInventoryObjectWithGapsComments::addComment(const string &comment, const DateTime &date, const string &author, dbConnection &conn)
{
	dbInventoryComment *comm =dynamic_cast<dbInventoryComment*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryComment", conn.getKey())); // PRQA S 3081, 4412 2 
	ACS_COND_THROW(!comm, exIllegalValueException("dbInventoryComment dynamic cast failed")) ;
	try {
	
		comm->setFileName(getFileName()) ; 
		comm->setFileVersion(getFileVersion()) ; 
		comm->setAuthor(author) ; 
		comm->setComment(comment) ; 
		comm->setDate(date) ; 
		if (isFromDatabase()) {
			comm->save(conn) ; 
			_commentsNum++ ;
			loadComments(conn) ;
		}
		else {
			_comments2Save.push_back(comm) ;
		}
	}
	catch(...) {
		delete comm ;
		throw ;
	}
}

_ACS_END_NAMESPACE
