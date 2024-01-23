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
	Revision 5.8  2016/04/11 13:21:51  marpas
	removing useless methods
	
	Revision 5.7  2016/03/26 21:45:49  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.6  2016/03/14 13:16:53  marpas
	getting rid of deprecvated classes
	
	Revision 5.5  2014/02/11 11:10:04  marpas
	refresh method removed: base class implements it
	
	Revision 5.4  2014/02/07 18:38:59  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2014/01/30 18:04:34  marpas
	interface rationalization
	
	Revision 5.2  2013/07/22 17:46:09  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.1  2013/07/15 16:19:38  marpas
	compilation warnings fixed
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.26  2013/05/14 18:58:31  marpas
	dbSchemaMetadataProvider interface now requires db connection key
	
	Revision 2.25  2013/04/30 13:09:35  marpas
	dbPersistent interface changed
	
	Revision 2.24  2013/04/17 17:36:33  marpas
	dbPersistent interface changed
	
	Revision 2.23  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.22  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.21  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.20  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.19  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.18  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.15  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.14  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.13  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.12  2011/03/02 18:16:21  marpas
	serial 8 supported on load too
	
	Revision 2.11  2011/03/02 17:39:38  marpas
	support to serial 8 improved
	
	Revision 2.10  2011/03/02 17:29:14  marpas
	support of serial 8 implemented
	
	Revision 2.9  2009/06/19 18:36:29  marpas
	still working on dbConnection speedup
	
	Revision 2.8  2009/06/19 18:11:35  marpas
	dbConnection used to speed up work and to avoid issuing multiple connections and queries when it is useless
	
	Revision 2.7  2006/11/13 16:34:11  ivafam
	Bug fixed in isSetInParent.
	
	Revision 2.6  2006/11/03 19:56:03  paoscu
	load method safety improved.
	update and refresh methods implemented
	
	Revision 2.5  2006/11/02 19:00:13  paoscu
	remove method fixed.
	
	Revision 2.4  2006/11/02 15:34:14  paoscu
	Work in progress.
	
	Revision 2.3  2006/10/31 19:17:50  paoscu
	Work in progress.
	
	Revision 2.2  2006/10/27 18:27:13  paoscu
	Work in progress
	
	Revision 2.1  2006/10/26 17:51:49  paoscu
	Class dbDoubleSmartPersistent added.
	
	
		

*/


#include <dbDoubleSmartPersistent.h>
#include <dbSmartQuery.h>

#include <dbQueryParameters.h>
#include <dbParam.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbRow.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDoubleSmartPersistent)


dbDoubleSmartPersistent::dbDoubleSmartPersistent(
            std::string const & parentTable , 
            std::string const & childTable, 
            dbConnection &conn) :
    dbPersistent(),
    _parent(0) , 
    _child(0),
    _relationTable(),
    _parentFields(),
    _childFields(),
    _key(conn.getKey()) // PRQA S 3380, 3385
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "parentTable = \""  << parentTable 
		<< "\" ; childTable = \"" << childTable << "\".") ;

	try{
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(_key) ;

		std::vector<size_t>	foreignKeys = DBSMP->getForeignKeys( childTable, conn ) ;

		if(foreignKeys.size() == 0 ) {
			string errorMessage("Tables \"") ;
			errorMessage+= parentTable + "\" and \"" ;
			errorMessage+= childTable + "\" do not seem to be a parent/child couple" ;
			ACS_THROW( ExecutionError( errorMessage ) ) ; // PRQA S 3081
		}


		_parent=new dbSmartPersistent(parentTable, conn) ;
		_child=new dbSmartPersistent(childTable, conn) ;
		
		//USEFULL ??
		_parentFields=_parent->getFieldNames(conn) ;
		_childFields=_child->getFieldNames(conn) ;

        size_t fks = foreignKeys.size() ;
		for( size_t i=0 ; i < fks ; i++ ){
			if(  DBSMP->getForeignTable (  _child->getTableName() , foreignKeys[i], conn ) == _parent->getTableName() )
			{
				size_t parentField = DBSMP->getForeignField (  _child->getTableName() , foreignKeys[i], conn ) ;
				_relationTable.push_back( make_pair(parentField ,foreignKeys[i]) ) 	;

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) <<" : Added relation \""
						   << DBSMP->getFieldName ( childTable , foreignKeys[i], conn) 
						   << "\" -> \""
						   << DBSMP->getFieldName ( parentTable , parentField, conn)  // PRQA S 3010
						   << "\"." ) ;
				
				
			}

		}

	}
	catch(std::exception &e){
		//exception does not exit the following method.
		destroy() ;
		throw ;
	}
}

			
dbDoubleSmartPersistent::~dbDoubleSmartPersistent() throw() 
{
    destroy() ;
}



dbDoubleSmartPersistent::dbDoubleSmartPersistent(const dbDoubleSmartPersistent& other) :
    dbPersistent(),
    _parent(0) , 
    _child(0),
    _relationTable(),
    _parentFields(),
    _childFields(),
    _key()

{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "This id = " << getUserIdentifier() 
		<< "Other id = " << other.getUserIdentifier() ) ;

	ThreadSafe::Lock guard(*this); // PRQA S 3050 2
	ThreadSafe::Lock guard2(other);
	try{
		*this=other ;
	}
	catch(std::exception &e)
	{
		destroy() ;
		throw ;
	}
}


dbDoubleSmartPersistent &dbDoubleSmartPersistent::operator=(const dbDoubleSmartPersistent &rhs)
{
    if (this != &rhs) {
	    copy(rhs, true) ;
    }
    return *this ;
}


dbDoubleSmartPersistent & dbDoubleSmartPersistent::copy(const dbDoubleSmartPersistent & other, bool copyDbPersistent)
{
	ThreadSafe::Lock guard(*this) ; // PRQA S 3050 2
	ThreadSafe::Lock guard2(other);
	 
	if(this != &other)
	{
		destroy() ;

		if(copyDbPersistent) {
			dbPersistent::operator=(other) ;
        }

        dbConnectionWrapper cw(db::ConnPools::instance()->getPool(other._key)) ;
        dbConnection &conn = cw ; // PRQA S 3050
		_parent=new dbSmartPersistent( other._parent-> getTableName(), conn) ;
		_child=new dbSmartPersistent ( other._child -> getTableName(), conn);
		
		_parent -> copy (*(other._parent) , copyDbPersistent ) ;
		_child  -> copy(*(other._child )  , copyDbPersistent ) ;
        _key = other._key ;
	}
	return *this ;
}





std::string dbDoubleSmartPersistent::className() const
{
	return "dbDoubleSmartPersistent" ;
}
	

dbPersistent * dbDoubleSmartPersistent::clone() const
{
	ThreadSafe::Lock guard(*this) ; // PRQA S 3050
	return new dbDoubleSmartPersistent(*this) ;
} 


bool operator==(const dbDoubleSmartPersistent &l, const dbDoubleSmartPersistent &r) // PRQA S 4020
{
	ThreadSafe::Lock guard(l) ; // PRQA S 3050 2
	ThreadSafe::Lock guard2(r);
    if (&l == &r) { return true ; }
	return (*l._parent == *r._parent) && (*l._child == *r._child) ;
}


bool operator!=(const dbDoubleSmartPersistent &l, const dbDoubleSmartPersistent &r) 
{
	return not (l == r) ;
}


void dbDoubleSmartPersistent::reset()
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	dbPersistent::reset() ;
	
	_parent->reset() ;
	_child->reset() ;

} 

dbPersistent::IdType dbDoubleSmartPersistent::save( dbConnection &conn )
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, getUserIdentifier(conn) << ": Start") ;

	//Checks like fromDatabase , modified, ecc... are left to _parent and _child
	
	try
	{
	    // inside a transaction
        db::Transaction transaction(conn) ;
        transaction.begin_work("DSP::save") ;
		setId ( _parent -> save(conn)  ) ;
		
		// Now set child FKs coming from parent (in case they have changed)
		// This is not undo by the memento because plain dbPersistentMemento implementation
		// does not pass all the fields.
		// Moreover the state of the memento is already altered when acquired during the save
		updateChildFKs(conn) ;
		_child -> save(conn) ;
		
	    transaction.commit() ;

	} //try
	catch(std::exception &e)
	{		
		ACS_THROW( ExecutionError(e , "Error while saving record " + getUserIdentifier(conn) ) ) ;
	}
	
	return getId() ;
}




bool dbDoubleSmartPersistent::load(dbConnection &conn  , IdType id) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, getUserIdentifier(conn) <<": Start") ;

	ThreadSafe::Lock guard(*this); // PRQA S 3050

	// NOTE this is a very bad implementation since it uses two consecutive selects
	// a transaction is needed to ensure teh consistency
	// A better apporach would be to use a single select with a join
	// Latter approach also not requires the transaction.

	try
	{

	    // inside a transaction
        db::Transaction transaction(conn) ;
        transaction.begin_work("DSP::load") ;
		if( ! _parent -> load(conn , id ) )
		{
			transaction.roll_back() ; // There is nothing to undo... It is just to close the transaction
			//Nothing to load. Object has not been altered.
			return false ; 
		}
		
        const string &dbKey = conn.getKey() ;
		// Now try to load 
		dbQueryParameters parameters(dbKey) ;
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(dbKey) ;

        size_t rts = _relationTable.size() ;
		for( size_t i=0 ; i < rts ; i++ )
		{
			dbColumn col(dbKey);
            bool null = false ;
			_parent ->get( _relationTable[i].first , col, null, conn ) ;
			std::string paramFieldName= DBSMP ->getFieldName(_child->getTableName(),  _relationTable[i].second, conn ); // PRQA S 3010
			dbParam par(dbKey, _child->getTableName() , paramFieldName , dbParam::EQUAL_TO , col.getSqlString() ) ;
			parameters.addParameter(par) ;
		}
		
		dbSmartQuery query( _child->getTableName(), dbKey );
		query.maxItems(2) ; //Just to be sure that at maximum a record is returned // PRQA S 4400
		query.addParameter(parameters) ;
		query.doQuery(conn) ;

		if( query.size() == 2) { // PRQA S 4400
			ACS_THROW( ExecutionError( "Multiple child records while loading record " + getUserIdentifier(conn) ) ) ; // PRQA S 3081
		}
		
		if(query.empty())
		{
			ACS_THROW( ExecutionError( "No child record while loading record " + getUserIdentifier(conn) ) ) ; // PRQA S 3081
			
			// Alternative solution is to clear the child part
			// But set it from database to allow further updates
			//_child -> reset() ; 
			//_child -> setFromDatabase() ;
		}
		if( query.size() == 1) {
			(*_child)=*(query.at(0)) ;
        }
	
	    transaction.commit() ;
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e , "Error while loading record " + getUserIdentifier(conn) ) ) ;
	}

	return true ;
}


	
dbPersistent::IdType dbDoubleSmartPersistent::update(dbConnection &conn )
{

	ACS_THROW(NotImplemented("update methods not still implemented.")) ; // PRQA S 3081

	ThreadSafe::Lock guard(*this); // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, getUserIdentifier(conn) << ": Start") ;

	//Checks like fromDatabase , modified, ecc... are left to _parent and _chil
	
	try
	{
	    // inside a transaction
        db::Transaction transaction(conn) ;
        transaction.begin_work("DSP::update") ;
		_parent -> update(conn) ;
		// Parent PKs cannot have changed, so there is no need to update child FKs coming from parent 
		_child -> update(conn) ;
	    transaction.commit() ;
		
	} //try
	catch(std::exception &ex)
	{
		ACS_THROW( ExecutionError(ex , "Error while updating record " + getUserIdentifier(conn) ) ) ;
	}

	return getId() ;
}


dbPersistent::IdType dbDoubleSmartPersistent::remove(dbConnection &conn )
{
	ThreadSafe::Lock lock(*this) ; // PRQA S 3050

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	try{
	    // inside a transaction
        db::Transaction transaction(conn) ;
        transaction.begin_work("DSP::remove") ;
		_child  -> remove(conn) ;
		_parent -> remove(conn) ;
	    transaction.commit() ;
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e,  "Error while removing record " + getUserIdentifier(conn) ) ) ;
	}

	return getId() ;
}



bool dbDoubleSmartPersistent::validate() const
{
	return true ;
}


bool dbDoubleSmartPersistent::isModified() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return (_child->isModified() || _parent->isModified() ) ;
}

bool dbDoubleSmartPersistent::isFromDatabase() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent -> isFromDatabase() ;
}




void dbDoubleSmartPersistent::writeTo(std::ostream & os) const  
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent->writeTo(os) ;
	_child->writeTo(os) ;
	
}

void dbDoubleSmartPersistent::writeTo(exostream & os) const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent->writeTo(os) ;
	_child->writeTo(os) ;
}


void dbDoubleSmartPersistent::setId(IdType id) 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent->setId(id) ;
	updateChildFKs() ; //The id is the FK of the child.
}


dbPersistent::IdType dbDoubleSmartPersistent::getId() const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent->getId() ;
}


void dbDoubleSmartPersistent::exportObj(rsResourceSet &) const
{
	// useless ThreadSafe::Lock guard(*this); // PRQA S 3050
	ACS_THROW(NotImplemented("exportObj methods not still implemented.")) ;  // PRQA S 3081
}



void dbDoubleSmartPersistent::importObj(rsResourceSet &) 
{
	// useless ThreadSafe::Lock guard(*this); // PRQA S 3050
	ACS_THROW(NotImplemented("importObj methods not still implemented.")) ;  // PRQA S 3081
}

void dbDoubleSmartPersistent::exportXML(XMLOstream &) const 
{
	// useless ThreadSafe::Lock guard(*this); // PRQA S 3050
	ACS_THROW(NotImplemented("exportXML methods not still implemented.")) ;  // PRQA S 3081
}




std::string dbDoubleSmartPersistent::getParentTableName() const
{
	return _parent->getTableName() ;
}

std::string dbDoubleSmartPersistent::getChildTableName() const
{
	return _child->getTableName() ;
}




size_t dbDoubleSmartPersistent::fieldsNumber() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return parentFieldsNumber() + childFieldsNumber() ; // PRQA S 3084
}


size_t dbDoubleSmartPersistent::parentFieldsNumber() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent->fieldsNumber() ;
}

size_t dbDoubleSmartPersistent::childFieldsNumber() const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _child->fieldsNumber() ;
}



std::vector<std::string> dbDoubleSmartPersistent::getFieldNames(dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	std::vector<std::string> res=_parent->getFieldNames(conn) ;
	std::vector<std::string> tmp=_child->getFieldNames(conn) ;
    size_t ts = tmp.size() ;
	for(size_t i=0 ; i< ts ; ++i) {
		res.push_back(tmp[i]) ;
	}
	return res ;
}


void dbDoubleSmartPersistent::unsetId()
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> unsetId() ;
	updateChildFKs() ;
}



void dbDoubleSmartPersistent::unsetFromParent(size_t pos, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent->unset(pos, conn) ;
	if (isParentPK(pos)) {
		updateChildFKs(conn) ;
    }
}

void dbDoubleSmartPersistent::unsetFromParent(std::string const &fieldName, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent->unset(fieldName, conn) ;
	if (isParentPK(fieldName, conn)) {
		updateChildFKs(conn) ;
    }
}


void dbDoubleSmartPersistent::unsetFromChild(std::string const &fieldName, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child->unset(fieldName, conn) ;
}


void dbDoubleSmartPersistent::unsetFromChild(size_t pos, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child->unset(pos, conn) ;
}



void dbDoubleSmartPersistent::unset(std::string const &fieldName, dbConnection &conn)
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	//Throws exception if the field name is ambiguous
	checkField(fieldName, conn) ;
	
	if ( isInParent(fieldName, conn) ) {
		unsetFromParent(fieldName, conn) ;
    }
	else if ( isInChild(fieldName, conn) ) {
		unsetFromChild(fieldName, conn) ;
    }
	else
	{
		ACS_THROW( ExecutionError("There is no field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
	}

}


bool dbDoubleSmartPersistent::isSetInParent(size_t pos, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent->isSet(pos, conn) ;
}


bool dbDoubleSmartPersistent::isSetInParent(std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent->isSet(fieldName, conn) ;
}


bool dbDoubleSmartPersistent::isSetInChild(size_t pos, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _child->isSet(pos, conn) ;
}



bool dbDoubleSmartPersistent::isSetInChild(std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _child->isSet(fieldName, conn) ;
}





bool dbDoubleSmartPersistent::isSet(std::string const &fieldName, dbConnection &conn) const // PRQA S 4020, 4214
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	
	try{
	
		//Throws exception if the field name is ambiguous
		checkField(fieldName, conn) ;
		

		if ( isInParent(fieldName, conn) )
		{
			return isSetInParent(fieldName, conn) ;
		}	
			
		else if ( isInChild(fieldName, conn) )
		{
			return isSetInChild(fieldName, conn) ;
		}
		else
		{
		    ACS_THROW( ExecutionError("There is no field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
		}
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e , "Error in isSet of record " + getUserIdentifier(conn) ) ) ;
	}
}



void dbDoubleSmartPersistent::setFilePathInParent(size_t pos  , std::string const & filePath, dbConnection &conn)  // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> setFilePath (pos , filePath, conn) ;
}


void dbDoubleSmartPersistent::setFilePathInParent(std::string const &fieldName  , std::string const & filePath, dbConnection &conn)  // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> setFilePath (fieldName , filePath, conn) ;
}


void dbDoubleSmartPersistent::setFilePathInChild(size_t pos  , std::string const & filePath, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child -> setFilePath (pos , filePath, conn) ;
}


void dbDoubleSmartPersistent::setFilePathInChild(std::string const &fieldName  , std::string const & filePath, dbConnection &conn)  // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child -> setFilePath (fieldName , filePath, conn) ;
}



void dbDoubleSmartPersistent::setFilePath(std::string const &fieldName, std::string const & filePath, dbConnection &conn)  // PRQA S 4020
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	checkField(fieldName, conn) ;
	
	if ( isInParent(fieldName, conn) ) {
		return setFilePathInParent(fieldName, filePath, conn) ;
    }
	else if ( isInChild(fieldName, conn) ) {
		return setFilePathInChild(fieldName, filePath, conn) ;
    }
	else
	{
		ACS_THROW( ExecutionError("There is no field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
	}
}




void dbDoubleSmartPersistent::unsetFilePathFromParent(size_t pos) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> unsetFilePath(pos) ;
}


void dbDoubleSmartPersistent::unsetFilePathFromParent(std::string const &fieldName, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> unsetFilePath(fieldName, conn) ;
}



void dbDoubleSmartPersistent::unsetFilePathFromChild (size_t pos) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child -> unsetFilePath(pos) ;
}


void dbDoubleSmartPersistent::unsetFilePathFromChild (std::string const &fieldName, dbConnection &conn) // PRQA S 4211
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child -> unsetFilePath(fieldName, conn) ;
}


void dbDoubleSmartPersistent::unsetFilePath(std::string const &fieldName, dbConnection &conn) // PRQA S 4020
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	checkField(fieldName, conn) ;
	
	if ( isInParent(fieldName, conn) ) {
		return unsetFilePathFromParent(fieldName, conn) ;
    }
	else if ( isInChild(fieldName, conn) ){
		return unsetFilePathFromChild(fieldName, conn) ;
    }
	else
	{
		ACS_THROW( ExecutionError("There is no field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
	}
}



std::string dbDoubleSmartPersistent::getFilePathFromParent(size_t pos) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent->getFilePath(pos) ;

}


std::string dbDoubleSmartPersistent::getFilePathFromParent(std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _parent->getFilePath(fieldName, conn) ;

}


std::string dbDoubleSmartPersistent::getFilePathFromChild(size_t pos) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _child->getFilePath(pos) ;

}



std::string dbDoubleSmartPersistent::getFilePathFromChild(std::string const &fieldName, dbConnection &conn) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	return _child->getFilePath(fieldName, conn) ;

}


std::string dbDoubleSmartPersistent::getFilePath(std::string const &fieldName, dbConnection &conn) const // PRQA S 4020, 4214
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	checkField(fieldName, conn) ;
	
	if ( isInParent(fieldName, conn) ) {
		return getFilePathFromParent(fieldName, conn) ;
    }
	else if ( isInChild(fieldName, conn) ) {
		return getFilePathFromChild(fieldName, conn) ;
    }
	else
	{
		ACS_THROW( ExecutionError("There is no field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
	}
}



void dbDoubleSmartPersistent::downloadFileFromParent(size_t pos , std::string const & filePath , dbConnection &conn  ) const
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> downloadFile(pos, filePath , conn ) ;
}

void dbDoubleSmartPersistent::downloadFileFromParent(std::string const &fieldName , std::string const & filePath , dbConnection &conn  ) const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_parent -> downloadFile(fieldName, filePath , conn ) ;

}

void dbDoubleSmartPersistent::downloadFileFromChild(size_t pos , std::string const & filePath , dbConnection &conn  ) const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child -> downloadFile(pos, filePath , conn ) ;

}

void dbDoubleSmartPersistent::downloadFileFromChild(std::string const &fieldName , std::string const & filePath , dbConnection &conn  ) const 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	_child ->downloadFile(fieldName, filePath , conn ) ;
}

void dbDoubleSmartPersistent::downloadFile(std::string const &fieldName , std::string const & filePath , dbConnection &conn  ) const  // PRQA S 4020, 4214
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	checkField(fieldName, conn) ;
	
	if ( isInParent(fieldName, conn) ) {
		return downloadFileFromParent(fieldName , filePath , conn ) ;
    }
	else if ( isInChild(fieldName, conn) ) {
		return downloadFileFromChild(fieldName , filePath , conn ) ;
    }
	else
	{
		ACS_THROW( ExecutionError("There is no field \"" + fieldName + "\" in record " + getUserIdentifier(conn))) ; // PRQA S 3081
	}

}



std::string dbDoubleSmartPersistent::getUserIdentifier() const throw() //  PRQA S 4020, 4214
{
    try {
        return getUserIdentifier(dbConnectionWrapper(db::ConnPools::instance()->getPool(_key))) ; // PRQA S 3050, 3081
    }
    catch(exception &e) {
		string errorMessage ;
		errorMessage+= "exception is: " ;
		errorMessage+= e.what() ;
		return errorMessage ;
    }	
}

std::string dbDoubleSmartPersistent::getUserIdentifier(dbConnection &conn) const throw() //  PRQA S 4020
{
	try
	{
	    ThreadSafe::Lock guard(*this); // PRQA S 3050
	    std::string res ;
		if(_parent && _child)
		{
			res=_parent->getUserIdentifier(conn) ;
			res += " - " ;
			res += _child->getUserIdentifier(conn) ;
		}
		else {
			res="Undefined record" ;
        }
	    return res ;
	}
	catch(std::exception &e)
	{
		std::string errorMessage ;
		errorMessage+= "exception is: " ;
		errorMessage+= e.what() ;
		return errorMessage ;
	}
}



bool dbDoubleSmartPersistent::isInParent(const std::string &fieldName, dbConnection &conn) const // PRQA S 4214
{
	return dbSchemaMetaDataProvider::instance(_key)->hasFieldWithName (  _parent->getTableName(), fieldName, conn) ;
}



bool dbDoubleSmartPersistent::isInChild(const std::string &fieldName, dbConnection &conn) const  // PRQA S 4214
{
	return dbSchemaMetaDataProvider::instance(_key)->hasFieldWithName (  _child->getTableName(), fieldName, conn) ;
}


bool dbDoubleSmartPersistent::isParentPK(const std::string &fieldName, dbConnection &conn) const // PRQA S 4214
{
	size_t fieldId= dbSchemaMetaDataProvider::instance(_key) ->getFieldFromName(_parent->getTableName(), fieldName, conn) ;
	return isParentPK(fieldId) ;

}




bool dbDoubleSmartPersistent::isParentPK(size_t fieldId) const // PRQA S 4020
{
    size_t rts = _relationTable.size() ;
	for ( size_t i=0 ; i < rts ; ++i ) {
		if( fieldId == _relationTable[i].first ) {
			return true ;
        }
    }
	return false;
}




void dbDoubleSmartPersistent::checkField(const std::string &fieldName, dbConnection &conn) const // PRQA S 4020, 4214
{
	try{
	
		if( isInParent(fieldName, conn) && isInChild(fieldName, conn) )
		{
            dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(_key) ;

			size_t fieldId= DBSMP ->getFieldFromName(_child->getTableName(), fieldName, conn) ;

            size_t rts = _relationTable.size() ;
	        for ( size_t i=0 ; i < rts ; ++i ) {
				if( fieldId == _relationTable[i].second ) //Ok It is a FK in the child
				{
					// I must check that also the field name is the same of the parent.
					if (DBSMP ->getFieldName(_parent->getTableName(), _relationTable[i].first, conn) == fieldName) { // PRQA S 3010
						return;
                    }
				}
			}

			ACS_THROW( ExecutionError("It is ambigous to reference field \"" + fieldName + "\" in record "  + getUserIdentifier(conn))) ; // PRQA S 3081
		}
	
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e , "Error while checking fields of record " + getUserIdentifier(conn) ) ) ;
	}
}



void dbDoubleSmartPersistent::updateChildFKs()
{
    updateChildFKs(dbConnectionWrapper(db::ConnPools::instance()->getPool(_key))) ; // PRQA S 3050, 3081
}

void dbDoubleSmartPersistent::updateChildFKs(dbConnection &conn)
{
    size_t rts = _relationTable.size() ;
	for ( size_t i=0 ; i < rts ; ++i ) {
		if( _parent->isSet(_relationTable[i].first, conn) )
		{
			dbColumn col(conn.getKey()) ;
            bool null = false ;
			_parent->get(_relationTable[i].first , col, null, conn) ;
			_child->set ( _relationTable[i].second , col, conn) ;
		}
		else
		{
			_child->unset(_relationTable[i].second, conn) ;
		}
	}
}






void dbDoubleSmartPersistent::destroy() throw() 
{
	ThreadSafe::Lock guard(*this); // PRQA S 3050
	
	delete _parent ;
    _parent = 0 ;
	delete _child ;
    _child = 0 ;
}


_ACS_END_NAMESPACE

