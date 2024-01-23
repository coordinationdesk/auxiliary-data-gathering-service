// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.10  2016/07/29 13:48:16  marfav
	Fixed CLOB/BLOB management when loading smart persistent and related objects
	
	Revision 5.9  2016/04/19 13:11:36  marpas
	fieldName method removed
	
	Revision 5.8  2016/04/11 13:21:51  marpas
	removing useless methods
	
	Revision 5.7  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.6  2016/03/14 13:16:53  marpas
	getting rid of deprecvated classes
	
	Revision 5.5  2014/02/11 11:10:04  marpas
	refresh method removed: base class implements it
	
	Revision 5.4  2014/02/07 18:39:00  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2014/01/30 18:03:58  marpas
	interface rationalized
	
	Revision 5.2  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.1  2013/07/15 16:19:38  marpas
	compilation warnings fixed
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.26  2013/05/14 19:00:07  marpas
	dbSchemaMetadataProvider interface now requires db connection key
	
	Revision 2.25  2013/04/30 13:09:35  marpas
	dbPersistent interface changed
	
	Revision 2.24  2013/04/17 17:36:33  marpas
	dbPersistent interface changed
	
	Revision 2.23  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.22  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.21  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.20  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.19  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.16  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.15  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.14  2011/10/20 13:51:10  lucio.pulvirenti
	Statistics improved
	
	Revision 2.13  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.12  2011/03/02 18:16:21  marpas
	serial 8 supported on load too
	
	Revision 2.11  2011/03/02 17:39:39  marpas
	support to serial 8 improved
	
	Revision 2.10  2011/03/02 17:29:14  marpas
	support of serial 8 implemented
	
	Revision 2.9  2009/06/19 18:36:29  marpas
	still working on dbConnection speedup
	
	Revision 2.8  2009/06/19 18:11:35  marpas
	dbConnection used to speed up work and to avoid issuing multiple connections and queries when it is useless
	
	Revision 2.7  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.6  2008/06/03 23:44:25  paoscu
	Added debug
	
	Revision 2.5  2008/05/08 21:38:45  paoscu
	Added options to enable/disable backup of record during the load operation
	Improved debug prints and thread safety
	
	Revision 2.4  2008/04/28 21:48:57  paoscu
	Added the methods: getSelectSQLString and  getColumnTypes
	
	Revision 2.3  2008/04/28 10:54:25  paoscu
	Now dbSmartAggregatePersistent::load is implemente with a single JOINed query.
	
	Revision 2.2  2008/04/23 19:01:35  paoscu
	Added related tables
	
	Revision 2.1  2008/04/23 16:13:23  paoscu
	dbSmartAggregatePersistent added.
	
		

*/




#include <dbSmartAggregatePersistent.h>
#include <dbSmartQuery.h>

#include <dbQuery.h>
#include <dbQueryParameters.h>
#include <dbParam.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbSet.h>
#include <dbRow.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSmartAggregatePersistent)


void dbSmartAggregatePersistent::init(string const & parentTable , 
                                      vector<string> const & childTables, 
                                      dbConnection &conn)
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "parentTable = \""  << parentTable << "\"") ;
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		for(size_t tb=0 ; tb< childTables.size() ; tb++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "childTable["<<tb<<"] = \"" << childTables[tb] << "\".") ;
        }
	ACS_CLASS_END_DEBUG
    _key = conn.getKey() ; // PRQA S 3380, 3385
	
	destroy() ;
	
	try{
		//allocate parent record
		_parent=new dbSmartPersistent(parentTable, conn) ;
		
		for(size_t tb=0 ; tb< childTables.size() ; tb++)
		{
			string childTable = childTables[tb] ;

			std::vector<size_t>	foreignKeys = dbSchemaMetaDataProvider::instance(_key)->getForeignKeys( childTable, conn ) ;

			if(foreignKeys.size() == 0 ) {
				ACS_THROW( ExecutionError("Tables \""+ parentTable + "\" and \"" + childTable + "\" does not seem to be a parent/child couple")) ; // PRQA S 3081
			}

			_childs[childTable]=new dbSmartPersistent(childTable, conn) ;
		
			// //USEFULL ??
			// _parentFields=_parent->getFieldNames() ;
			// _childFields=_child->getFieldNames() ;

            size_t fks = foreignKeys.size() ;
			for( size_t i=0 ; i < fks ; i++ )
			{
				if(  dbSchemaMetaDataProvider::instance(_key)->getForeignTable (  childTable , foreignKeys[i], conn ) == parentTable )
				{
					size_t parentField = dbSchemaMetaDataProvider::instance(_key)->getForeignField ( childTable , foreignKeys[i], conn ) ;
					_relationTable[childTable].push_back( make_pair(parentField ,foreignKeys[i]) ) 	;

					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, getUserIdentifier(conn) <<" : Added relation \"" << childTable << "\".\""
							   << dbSchemaMetaDataProvider::instance(_key)->getFieldName ( childTable , foreignKeys[i], conn) 
							   << "\" -> \""
							   << dbSchemaMetaDataProvider::instance(_key)->getFieldName ( parentTable , parentField, conn)  // PRQA S 3010
							   << "\".") ;


				}
			}	

		}

	}
	catch(std::exception &e){
		//note that exception does not exit destroy method.
		destroy() ;
		throw ;
	}
}

bool dbSmartAggregatePersistent::initialized() const
{
	return ( _parent != 0 ) ;
}


void dbSmartAggregatePersistent::checkInitialized() const
{
	ACS_COND_THROW(!initialized(),ExecutionError( "Not Initialized object" ) ) ; // PRQA S 3081
}


dbSmartAggregatePersistent::dbSmartAggregatePersistent() : 
    dbPersistent(),
    _parent(0),
    _childs(),
    _relationTable(),
    _relatedRecords(),
    _key()
{
}



dbSmartAggregatePersistent::dbSmartAggregatePersistent(
                        string const & parentTable , 
                        vector<string> const & childTables, 
                        dbConnection &conn) : 
    dbPersistent(),
    _parent(0),
    _childs(),
    _relationTable(),
    _relatedRecords(),
    _key(conn.getKey()) // PRQA S 3380, 3385
{
	init(parentTable , childTables, conn) ;

}

			
dbSmartAggregatePersistent::~dbSmartAggregatePersistent() noexcept 
{
    destroy() ; 
}



dbSmartAggregatePersistent::dbSmartAggregatePersistent(const dbSmartAggregatePersistent& rhs) : 
    dbPersistent(),
    _parent(0), 
    _childs(),
    _relationTable(),
    _relatedRecords(),
    _key()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "This id = " << getUserIdentifier() 
		<< " Other id = " << rhs.getUserIdentifier()) ;

	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	ThreadSafe::Lock guard2(rhs); // PRQA S 3050 
	try{
		*this=rhs ;
	}
	catch(std::exception &e)
	{
		destroy() ;
		throw ;
	}
}


dbSmartAggregatePersistent &dbSmartAggregatePersistent::operator=(const dbSmartAggregatePersistent &rhs)
{
    if (this != &rhs) {
        copy(rhs, true) ;
    }
    return *this ;
}


dbSmartAggregatePersistent & dbSmartAggregatePersistent::copy(const dbSmartAggregatePersistent & other, bool copyDbPersistent)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "This id = " << getUserIdentifier() 
		<< " Other id = " << other.getUserIdentifier()) ;

	ThreadSafe::Lock guard(*this) ; // PRQA S 3050 
	ThreadSafe::Lock guard2(other); // PRQA S 3050 
	 
	if(this != &other)
	{
		destroy() ;

		if(copyDbPersistent) {
			dbPersistent::operator=(other) ;
        }
        dbConnectionWrapper cw(db::ConnPools::instance()->getPool(other._key)) ;
        dbConnection &conn = cw ; // PRQA S 3050

		_parent=new dbSmartPersistent( other._parent-> getTableName(), conn) ;
		_parent -> copy (*(other._parent) , copyDbPersistent ) ;
		
		
		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=other._childs.begin() ; 
            it!=other._childs.end() ; 
            ++it)
		{
			string childTable = it->first ;
			_childs[childTable]=new dbSmartPersistent (childTable, conn);
			_childs[childTable] -> copy(*(it->second)  , copyDbPersistent ) ;
		}
		
		size_t rs = other._relatedRecords.size() ;
		for( size_t i=0 ; i < rs ; i++ ) {
			addRelatedRecord( dynamic_cast<dbSmartPersistent *>(other._relatedRecords[i]->clone()) ) ;  // PRQA S 3081
		}
        _key = other._key ;
	}
	return *this ;
}





std::string dbSmartAggregatePersistent::className() const
{
	return "dbSmartAggregatePersistent" ;
}
	

dbPersistent * dbSmartAggregatePersistent::clone() const
{
	ThreadSafe::Lock guard(*this) ; // PRQA S 3050 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier()) ;
	return new dbSmartAggregatePersistent(*this) ;
} 


	
bool operator==(const dbSmartAggregatePersistent &l, const dbSmartAggregatePersistent &r) // PRQA S 4020
{
    if (&l == &r) { return true ; }
	ThreadSafe::Lock guard(l) ; // PRQA S 3050 
	ThreadSafe::Lock guard2(r); // PRQA S 3050 
	if ( *l._parent != *r._parent ) { return false ; }
    if (l._childs.size() != r._childs.size()) { return false ; }
	return equal(l._childs.begin(), l._childs.end(), r._childs.begin()) ;
}



bool operator!=(const dbSmartAggregatePersistent &l, const dbSmartAggregatePersistent &r) 
{
	return not ( l == r ) ;
}

void dbSmartAggregatePersistent::reset()
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	dbPersistent::reset() ;
	
	_parent->reset() ;
	for(std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ;  // PRQA S 4238
        it!=_childs.end() ; 
        ++it)
	{
		it->second->reset() ;
	}
	
	clearRelatedRecords() ;
} 

dbPersistent::IdType dbSmartAggregatePersistent::save( dbConnection &conn )
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) << ": Start") ;

	//Checks like fromDatabase , modified, ecc... are left to _parent and _child
	
	//conn->addStateWrapper(*this) ; useless : use _parent and _child values
	
	try
	{
	    // inside a transaction
	    db::Transaction transaction (conn) ;
	    transaction.begin_work("SAP::save") ;
		setId ( _parent -> save(conn)  ) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) << ": saved parent record.") ;
		
		// Now set child FKs coming from parent (in case they have changed)
		// This is not undo by the memento because plain dbPersistentMemento implementation
		// does not pass all the fields.
		// Moreover the state of the memento is already altered when acquired during the save
		
		// WARNING this should be useless because already done by setId
		updateChildsFKs(conn) ;

		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; // PRQA S 4238
             it!=_childs.end() ; 
             ++it)
		{
			it->second-> save(conn) ;
		}

        size_t rs = _relatedRecords.size() ;
		for(size_t i =0 ; i<rs ; ++i )
		{
			_relatedRecords[i]-> save(conn) ;
		}
		
	    transaction.commit() ;
	} //try
	catch(std::exception &e)
	{		
		ACS_THROW( ExecutionError(e , "Error while saving record " + getUserIdentifier(conn) ) ) ; // PRQA S 3081
	}

	// Finally load all the related records outside the transaction
	size_t rs = _relatedRecords.size() ;
    for(size_t i =0 ; i<rs ; i++ )
	{
		_relatedRecords[i]-> load(conn) ;
	}

	return getId() ;
}


bool dbSmartAggregatePersistent::load(dbConnection &conn , IdType id) // PRQA S 4020
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, getUserIdentifier(conn) << ": Start") ;
	try
	{
        const string &dbKey = conn.getKey() ;
		ThreadSafe::Lock guard(*this); // PRQA S 3050 

		if(! (dbSchemaMetaDataProvider::instance(dbKey) -> hasPrimaryKeys( _parent->getTableName(), conn ) ))
		{
			ACS_THROW(InvalidUse("Load is not possible because there are not primary keys.")) ; // PRQA S 3081
		}

    	SQLString queryString(dbKey, SQLString::SELECT, true) ;
		getSelectSQLString(queryString, conn) ;


    	dbQueryParameters parameters(dbKey) ;
		_parent->getWhereParameters(parameters , conn, id) ; //this also check if Id has been set

    	queryString.addANDWhereParameter(parameters) ;
		string orderBy=_parent->getOrderBy() ;
		if(!orderBy.empty()) { queryString.setOrderByString(orderBy) ; }

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) << ": "
					<<" SQL = \"" << queryString.getSQL() << "\".") ;
		

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			vector<dbSpecificColumnType::Type > types = getColumnTypes(conn);


			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, getUserIdentifier(conn) << ": Types vector is:" );
			size_t ts = types.size() ;
            for( size_t i=0; i<ts ; ++i)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, dbColumnTypeHelper::getTypeName(types[i]));
			}
		ACS_CLASS_END_DEBUG


		dbRow row(dbKey) ;
        dbSet set ;
		
		try
		{
 			dbQuery query(conn, queryString, _parent->getTableName()+ " smart agg. load") ;
	        query.execForSet(set, getColumnTypes(conn)) ;
            ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ; // PRQA S 3081 2
            ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ;
            set.getNextRow(row) ; // first and only 
		}
		catch (dbQuery::QueryNoDataFound &e)
		{
			return false ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) << ": Query successfully executed.") ;

		loadRow(row,0) ;
		
		_parent->setFromDatabase() ;
		_parent->resetAllFielsdModifiedFlag() ;
		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
		{
			it->second->setFromDatabase() ;
			it->second->resetAllFielsdModifiedFlag()  ;
		}

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, getUserIdentifier(conn) << ": Dump : \"" << *this ) ;
	}
	catch(exception &e)
	{
		//delete myCopy ;
		ACS_THROW( ExecutionError(e , "Error while loading record " + getUserIdentifier(conn) ) ) ;
	}

	return true ;
}

dbPersistent::IdType dbSmartAggregatePersistent::update(dbConnection &conn )
{

	ACS_THROW(NotImplemented("update methods not still implemented.")) ; // PRQA S 3081

	ThreadSafe::Lock guard(*this); // PRQA S 3050 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) << ": Start") ;

	//Checks like fromDatabase , modified, ecc... are left to _parent and _child
	
	try
	{
	    // inside a transaction
        db::Transaction transaction(conn) ;
        transaction.begin_work("SAP::update") ;
		_parent -> update(conn) ;
		// Parent PKs cannot have changed, so there is no need to update child FKs coming from parent 
		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
		{
			it->second-> update(conn) ;
		}
		transaction.commit() ;
	
	} //try
	catch(std::exception &ex)
	{		
		ACS_THROW( ExecutionError(ex , "Error while updating record " + getUserIdentifier(conn) ) ) ;
	}
	

	//Finally update _relatedRecords
    size_t rs = _relatedRecords.size() ;
	for(size_t i =0 ; i<rs ; ++i )
	{
		_relatedRecords[i]-> update(conn) ;
	}

	return getId() ;
}



dbPersistent::IdType dbSmartAggregatePersistent::remove(dbConnection &conn )
{
	ThreadSafe::Lock lock(*this) ; // PRQA S 3050 

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    try {
        db::Transaction transaction (conn) ;
        transaction.begin_work("SAP::remove") ;
	    //First remove _relatedRecords
        size_t rs = _relatedRecords.size() ;
	    for(size_t i =0 ; i<rs ; ++i )
	    {
		    _relatedRecords[i]-> remove(conn) ;
	    }

	    for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
	    {
		    it->second-> remove(conn) ;
	    }
	    transaction.commit() ;
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e,  "Error while removing record " + getUserIdentifier(conn) ) ) ;
	}


	return getId() ;
}



bool dbSmartAggregatePersistent::validate() const
{
	return true ;
}


bool dbSmartAggregatePersistent::isModified() const // PRQA S 4020
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	if ( _parent->isModified() ) { return true ; }

	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
	{
		if(it->second-> isModified() ) { return true ; }
	}


	//Finally the _relatedRecords
    size_t rs = _relatedRecords.size() ;
	for(size_t i =0 ; i<rs ; ++i)
	{
		if ( _relatedRecords[i]-> isModified() ) {	return true ; }
	}

	return false ;
}



bool dbSmartAggregatePersistent::isFromDatabase() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent -> isFromDatabase() ;
}



void dbSmartAggregatePersistent::writeTo(std::ostream & os) const  
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent->writeTo(os) ;
	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
	{
		it->second-> writeTo(os) ;
	}
    size_t rs = _relatedRecords.size() ;
	for(size_t i =0 ; i<rs ; ++i)
	{
		_relatedRecords[i]-> writeTo(os) ;
	}
}


void dbSmartAggregatePersistent::writeTo(exostream & os) const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent->writeTo(os) ;
	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
	{
		it->second-> writeTo(os) ;
	}
    size_t rs = _relatedRecords.size() ;
	for(size_t i =0 ; i<rs ; ++i)
	{
		_relatedRecords[i]-> writeTo(os) ;
	}
}


void dbSmartAggregatePersistent::setId(IdType id) 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent->setId(id) ;
	updateChildsFKs() ; //The id is the FK of the child.
}

dbPersistent::IdType dbSmartAggregatePersistent::getId() const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent->getId() ;
}


void dbSmartAggregatePersistent::exportObj(rsResourceSet &) const
{
	// useless ThreadSafe::Lock guard(*this); // PRQA S 3050 
	ACS_THROW(NotImplemented("exportObj methods not still implemented.")) ; // PRQA S 3081
}



void dbSmartAggregatePersistent::importObj(rsResourceSet &) 
{
	// useless ThreadSafe::Lock guard(*this); // PRQA S 3050 
	ACS_THROW(NotImplemented("importObj methods not still implemented.")) ; // PRQA S 3081
}

void dbSmartAggregatePersistent::exportXML(XMLOstream &) const 
{
	// useless ThreadSafe::Lock guard(*this); // PRQA S 3050 
	ACS_THROW(NotImplemented("exportXML methods not still implemented.")) ; // PRQA S 3081
}




std::string dbSmartAggregatePersistent::getParentTableName() const
{
	return _parent->getTableName() ;
}




size_t dbSmartAggregatePersistent::fieldsNumber(dbConnection &conn) const // PRQA S 4020
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	size_t result = parentFieldsNumber() ;
	
	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) { // PRQA S 4238
		return result += childFieldsNumber(it->first, conn) ; // PRQA S 3084
    }
	return result ;
}



void dbSmartAggregatePersistent::saveCopyDuringLoad(bool v)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	
	_parent->saveCopyDuringLoad(v) ;
	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) { // PRQA S 4238
		it->second-> saveCopyDuringLoad(v) ;
    }

}

bool dbSmartAggregatePersistent::saveCopyDuringLoad() const
{
	return  _parent->saveCopyDuringLoad() ;
}


size_t dbSmartAggregatePersistent::parentFieldsNumber() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent->fieldsNumber() ;
}

size_t dbSmartAggregatePersistent::childFieldsNumber(string const & childTable, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	checkChildExists(childTable, conn) ;
	std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.find(childTable) ;
	return it->second->fieldsNumber() ;
}



std::vector<std::string> dbSmartAggregatePersistent::getFieldNames(dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	std::vector<std::string> res=_parent->getFieldNames(conn) ;
	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it)
	{
		std::vector<std::string> tmp=it->second->getFieldNames(conn) ;
		size_t ts = tmp.size() ;
        for(size_t i=0 ; i< ts ; ++i) {
			res.push_back(tmp[i]) ;
        }
	}
	return res ;
}



void dbSmartAggregatePersistent::getSelectSQLString(SQLString &queryString, dbConnection &conn) const // PRQA S 4214
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) <<" : Start \n") ;

	// returns also blobs and clobs as SIGNED INTs
	// Those fields will not be really loaded, but it will be possible to see if they are NULL or not. 

	dbSmartPersistent::setLOBFreeSelectList (_parent->getTableName(), queryString, conn);

 	string fromString(_parent->getTableName()) ;


	for( map<string ,vector< pair<size_t , size_t> > >::const_iterator it=_relationTable.begin() ; it!=_relationTable.end() ; ++it)
	{
		string childTableName = it->first ;
		dbSmartPersistent::setLOBFreeSelectList ( childTableName, queryString, conn);

		fromString += " JOIN " ;
		fromString += childTableName ;
		fromString += " ON (" ;
		for( size_t i=0 ; i < it->second.size() ; i++ )
		{
			fromString +=childTableName ; 
			fromString +="." ; 
			fromString +=dbSchemaMetaDataProvider::instance(_key) ->getFieldName(childTableName ,  it->second[i].second, conn ) ; // PRQA S 3010
			fromString +=" = " ; 
			fromString += _parent->getTableName() ; 
			fromString +="." ; 
			fromString +=dbSchemaMetaDataProvider::instance(_key) ->getFieldName(_parent->getTableName() ,  it->second[i].first, conn ) ;  // PRQA S 3010
		}
		fromString += " ) " ;

	}

	queryString.addTableList( fromString ) ;
}


vector<dbSpecificColumnType::Type > dbSmartAggregatePersistent::getColumnTypes(dbConnection &conn) const // PRQA S 4214
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	vector<dbSpecificColumnType::Type > types ;
	
    size_t fn = dbSchemaMetaDataProvider::instance(_key)->fieldsNumber( _parent->getTableName(), conn ) ;
	for( size_t i=0; i< fn ; ++i)
	{
		types.push_back( dbSchemaMetaDataProvider::instance(_key)->getFieldType( _parent->getTableName() , i, conn) )  ;
	}
	for( map<string ,vector< pair<size_t , size_t> > >::const_iterator it=_relationTable.begin() ; it!=_relationTable.end() ; ++it)
	{
		string childTableName = it->first ;
        size_t fn2 = dbSchemaMetaDataProvider::instance(_key)->fieldsNumber( childTableName, conn ) ;
		for( size_t i=0; i< fn2 ; ++i)
		{
			types.push_back( dbSchemaMetaDataProvider::instance(_key)->getFieldType(childTableName , i, conn) )  ;
		}
	}

	// Remove CLOBS and BLOBS
	dbSmartPersistent::getLOBFreeColumnTypes (types);

	return types ;
}


void dbSmartAggregatePersistent::unsetId()
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> unsetId() ;
	updateChildsFKs() ;
}



void dbSmartAggregatePersistent::addRelatedRecord(dbSmartPersistent *in)
{
	if(in) {
		_relatedRecords.push_back( in ) ;
    }
}


void dbSmartAggregatePersistent::clearRelatedRecords()
{
    size_t rs = _relatedRecords.size() ;
	for(size_t i =0 ; i<rs ; ++i ) {
		delete _relatedRecords[i] ;
	}
	_relatedRecords.clear() ;
}



void dbSmartAggregatePersistent::unsetFromParent(size_t pos, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent->unset(pos, conn) ;
	if (isParentPK(pos, conn)) {
		updateChildsFKs(conn) ;
    }
}

void dbSmartAggregatePersistent::unsetFromParent(std::string const &fieldName, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent->unset(fieldName, conn) ;
	if (isParentPK(fieldName, conn)) {
		updateChildsFKs(conn) ;
    }
}


void dbSmartAggregatePersistent::unsetFromChild(string const & childTable , std::string const &fieldName, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	checkChildExists(childTable, conn) ;
	_childs[childTable]->unset(fieldName, conn) ;
}


void dbSmartAggregatePersistent::unsetFromChild(string const & childTable , size_t pos, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	checkChildExists(childTable, conn) ;
	_childs[childTable]->unset(pos, conn) ;
}



void dbSmartAggregatePersistent::unset(std::string const &fieldName, dbConnection &conn) // PRQA S 4020
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	//Throws exception if the field name is ambiguous
	checkField(fieldName, conn) ;
	
	if ( isInParent(fieldName, conn) )
	{
		unsetFromParent(fieldName, conn) ;
		return ;
	}
	else
	{
		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
		{
			 if ( isInChild(it->first , fieldName, conn) )
			 {
				it->second->unset(fieldName, conn) ;
			 	return ;
			 }
		}
	}
	
	

	std::string errorMessage("There is no field \"");
	errorMessage+=fieldName + "\" in record " ;
	errorMessage+=getUserIdentifier(conn) ;
	ACS_THROW( ExecutionError( errorMessage ) ) ; // PRQA S 3081

}


bool dbSmartAggregatePersistent::isSetInParent(size_t pos, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent->isSet(pos, conn) ;
}


bool dbSmartAggregatePersistent::isSetInParent(std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent->isSet(fieldName, conn) ;
}

bool dbSmartAggregatePersistent::isSetInChild(string const & childTable , size_t pos, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	checkChildExists(childTable, conn) ;
	std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.find(childTable) ;
	return it->second->isSet(pos, conn) ;
}



bool dbSmartAggregatePersistent::isSetInChild(string const & childTable , std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	checkChildExists(childTable, conn) ;
	std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.find(childTable) ;
	return it->second->isSet(fieldName, conn) ;
}





bool dbSmartAggregatePersistent::isSet(std::string const &fieldName, dbConnection &conn) const // PRQA S 4020, 4214
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	
	try{
	
		//Throws exception if the field name is ambiguous
		checkField(fieldName, conn) ;
		

		if ( isInParent(fieldName, conn) )
		{
			return isSetInParent(fieldName, conn) ;
		}	

		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
		{
			if ( isInChild(it->first ,fieldName, conn ) ) {
				return isSetInChild(it->first , fieldName, conn) ;
            }
		}


		std::string errorMessage("There is no field \"");
		errorMessage+=fieldName + "\" in record " ;
		errorMessage+=getUserIdentifier(conn) ;
		ACS_THROW( ExecutionError( errorMessage ) ) ; // PRQA S 3081
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e , "Error in isSet of record " + getUserIdentifier(conn) ) ) ;  // PRQA S 3081
	}
}


void dbSmartAggregatePersistent::setFilePathInParent(size_t pos  , std::string const & filePath, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> setFilePath (pos , filePath, conn) ;
}



void dbSmartAggregatePersistent::setFilePathInParent(std::string const &fieldName  , std::string const & filePath, dbConnection &conn)  // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> setFilePath (fieldName , filePath, conn) ;
}



void dbSmartAggregatePersistent::unsetFilePathFromParent(size_t pos) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> unsetFilePath(pos) ;
}


void dbSmartAggregatePersistent::unsetFilePathFromParent(std::string const &fieldName, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> unsetFilePath(fieldName, conn) ;
}



std::string dbSmartAggregatePersistent::getFilePathFromParent(size_t pos) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent->getFilePath(pos) ;

}


std::string dbSmartAggregatePersistent::getFilePathFromParent(std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	return _parent->getFilePath(fieldName, conn) ;

}



void dbSmartAggregatePersistent::downloadFileFromParent(size_t pos , std::string const & filePath , dbConnection &conn  ) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> downloadFile(pos, filePath , conn ) ;
}

void dbSmartAggregatePersistent::downloadFileFromParent(std::string const &fieldName , std::string const & filePath , dbConnection &conn  ) const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	_parent -> downloadFile(fieldName, filePath , conn ) ;

}


int dbSmartAggregatePersistent::loadRow(const dbRow &row, int offset)
{
	size_t actualOffset = offset ; // PRQA S 3000, 3010
	_parent->loadRow(row, actualOffset ) ; // PRQA S 3000, 3010
	actualOffset += _parent->fieldsNumber() ;  // PRQA S 3084

	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
	{
		it->second->loadRow(row, actualOffset ) ;   // PRQA S 3000, 3010
		actualOffset += it->second->fieldsNumber() ;  // PRQA S 3084
	}
	return actualOffset ; // PRQA S 3000, 3010
}




std::string dbSmartAggregatePersistent::getUserIdentifier() const noexcept //  PRQA S 4020, 4214
{
    return getUserIdentifier(dbConnectionWrapper(db::ConnPools::instance()->getPool(_key))) ; // PRQA S 3050, 3081
}


std::string dbSmartAggregatePersistent::getUserIdentifier(dbConnection &conn) const noexcept // PRQA S 4020
{	
	try
	{
	    ThreadSafe::Lock guard(*this); // PRQA S 3050 
	    ostringstream res ;
		if(_parent)
		{
			res <<_parent->getUserIdentifier(conn) ;
			res << " - " ;
			for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) { // PRQA S 4238
				res << it->second->getUserIdentifier(conn) ;
            }
			
			if(not _relatedRecords.empty()) {
				res <<  " + " <<  _relatedRecords.size() << " attached records " ;
            }
		}
		else  {
			res << "Undefined record" ;
        }
	    return res.str() ;	
	}
	catch(std::exception &e)
	{
		string errorMessage ;
		errorMessage+= "exception is: " ;
		errorMessage+= e.what() ;
		return errorMessage ;
	}
}



bool dbSmartAggregatePersistent::isInParent(const std::string &fieldName, dbConnection &conn) const // PRQA S 4214
{
	return dbSchemaMetaDataProvider::instance(_key)->hasFieldWithName (  _parent->getTableName(), fieldName, conn) ;
}



bool dbSmartAggregatePersistent::isInChild(string const & childTable, const std::string &fieldName, dbConnection &conn) const  // PRQA S 4214
{
	checkChildExists(childTable, conn) ;
	return dbSchemaMetaDataProvider::instance(_key)->hasFieldWithName (  childTable, fieldName, conn) ;
}


bool dbSmartAggregatePersistent::isParentPK(const std::string &fieldName, dbConnection &conn) const // PRQA S 4214
{
	return dbSchemaMetaDataProvider::instance(_key) ->isPrimaryKey(_parent->getTableName() , fieldName, conn) ;

}




bool dbSmartAggregatePersistent::isParentPK(size_t fieldId, dbConnection &conn) const // PRQA S 4214
{
	return dbSchemaMetaDataProvider::instance(_key) ->isPrimaryKey(_parent->getTableName() , fieldId, conn) ; // PRQA S 3010
}	
	



void dbSmartAggregatePersistent::checkField(const std::string &fieldName, dbConnection &conn) const // PRQA S 4214
{
	try{
	
		bool inParent=isInParent(fieldName, conn) ;
		bool alreadyMet = inParent; // says if the field has already been met in a table

		for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
		{
			string childTable =  it->first ;
			if( isInChild(childTable, fieldName, conn) )
			{
				if(alreadyMet)
				{
					bool accepted=false ; // says it is a duplicated field 
					if (inParent)
					{

						size_t fieldId= dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName(childTable, fieldName, conn) ;
						std::vector< std::pair<size_t , size_t> > const & relations(_relationTable.find(childTable)->second) ; 
						for ( size_t i=0 ; i < relations.size() ; i++ )
						{
							if( fieldId == relations[i].second ) //Ok It is a FK in the child
							{
								// I must check that also the field name is the same of the parent.
								if (dbSchemaMetaDataProvider::instance(_key) ->getFieldName(_parent->getTableName(), relations[i].first, conn) == fieldName) { // PRQA S 3010
									accepted = true ;
                                }
								break;
							}
						}

					}

					if(!accepted)
					{
						ACS_THROW( ExecutionError("It is ambigous to reference field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
					}

				}
				alreadyMet=true ;
			}
		}
	
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e , "Error while checking fields of record " + getUserIdentifier(conn) ) ) ;
	}
}



void dbSmartAggregatePersistent::updateChildsFKs()
{
    updateChildsFKs(dbConnectionWrapper(db::ConnPools::instance()->getPool(_key))) ; // PRQA S 3050, 3081
}

void dbSmartAggregatePersistent::updateChildsFKs(dbConnection &conn)
{

	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it) // PRQA S 4238
	{
		string childTable =  it->first ;
		std::vector< std::pair<size_t , size_t> > const & relations(_relationTable.find(childTable)->second) ; 

        size_t rs = relations.size() ;
		for( size_t i=0 ; i < rs ; ++i )
		{
			if( _parent->isSet( relations[i].first, conn) )
			{
				dbColumn col(_key) ;
                bool null = false ;
				_parent->get(relations[i].first , col, null, conn) ;
				it->second->set (relations[i].second , col, conn) ;
			}
			else
			{
				it->second->unset(relations[i].second, conn) ;
			}
		}
	}
	
	string parentTable = _parent->getTableName() ;
	size_t rrs = _relatedRecords.size();
	for( size_t rec=0 ; rec < rrs ; ++rec )
	{
		dbSmartPersistent &record( *_relatedRecords[rec]) ;
		string relatedTable = record.getTableName() ;
		std::vector<size_t>	foreignKeys = dbSchemaMetaDataProvider::instance(_key)->getForeignKeys( relatedTable, conn ) ;

        size_t fks = foreignKeys.size() ;
		for( size_t fk=0 ; fk < fks; ++fk )
		{
			if(  dbSchemaMetaDataProvider::instance(_key)->getForeignTable (  relatedTable , foreignKeys[fk], conn ) == parentTable )
			{
				size_t parentField = dbSchemaMetaDataProvider::instance(_key)->getForeignField ( relatedTable , foreignKeys[fk], conn ) ;

				dbColumn col(_key) ;
                bool null = false ;
				_parent->get(parentField , col, null, conn) ;
				record.set ( foreignKeys[fk] , col, conn) ;
			}
		}
		
	}
	
}


void dbSmartAggregatePersistent::checkChildExists(string const & childTable, dbConnection &conn) const // PRQA S 4020
{
	if( _childs.find(childTable)  != _childs.end() ) {
		return ;
    }

	std::string errorMessage("It is ambigous to reference field \"");
	errorMessage+=childTable + "\" in object " ;
	errorMessage+=getUserIdentifier(conn) ;
	ACS_THROW( ExecutionError( errorMessage ) ) ; // PRQA S 3081
}



void dbSmartAggregatePersistent::destroy() noexcept
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050 
	
	delete _parent ;
    _parent = 0 ;
	
	for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; // PRQA S 4238
         it!=_childs.end() ; 
         ++it)
	{
		delete it->second ;
	}
	_childs.clear() ;
	
	clearRelatedRecords() ;
}


_ACS_END_NAMESPACE

