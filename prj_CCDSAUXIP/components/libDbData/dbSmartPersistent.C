// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia S.p.A. - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia S.p.A.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$

*/


#include <dbSmartPersistent.h>

#include <dbSchemaMetaDataProvider.h>
#include <SQLString.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbConv.h>
#include <dbSet.h>
#include <dbRow.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <exStream.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSmartPersistent)



dbSmartPersistent::dbSmartPersistent(std::string const & tableName, dbConnection &conn) :
	dbPersistent() ,
	_tableName(tableName) ,
    _fields(),
    _fieldModified(),
    _filePaths(),
	_saveCopyDuringLoad(false),
    _idFieldPos(-1),
    _key(conn.getKey()), // PRQA S 3380, 3385
	_disableNullCheck(false),
	_idxCastClobAsString_S()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_tableName: " << _tableName) ;

    dbSchemaMetaDataProvider *DBSMP = dbSchemaMetaDataProvider::instance(_key) ;
    size_t fields = DBSMP->fieldsNumber(_tableName, conn) ;


	for( size_t i=0; i< fields ; ++i)
	{
		{
			_fields.push_back(dbColumn(_key, DBSMP->getFieldType(_tableName,i, conn))) ; // PRQA S 3081
		}
		_fieldModified.push_back(false) ;
	}

	if(DBSMP -> hasIdField( _tableName, conn ) )
	{
		_idFieldPos = DBSMP -> getIdField( _tableName, conn ) ; // PRQA S 3000
    }
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_idFieldPos: " << _idFieldPos) ;
}



dbSmartPersistent::dbSmartPersistent(std::string const & tableName, 
                                     map< string, dbSpecificColumnType::Type> const &  types, 
                                     dbConnection &conn) :
	dbPersistent() ,
	_tableName(tableName) ,
    _fields(),
    _fieldModified(),
    _filePaths(),
	_saveCopyDuringLoad(false),
    _idFieldPos(-1),
    _key(conn.getKey()), // PRQA S 3380, 3385
    _disableNullCheck(false),
    _idxCastClobAsString_S()
    
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_tableName: " << _tableName) ;
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(_key) ;
    size_t fields = DBSMP->fieldsNumber(_tableName, conn) ;
	for( size_t i=0; i< fields ; ++i)
	{
		string fieldName = DBSMP->getFieldName(_tableName,i, conn) ;
		map< string, dbSpecificColumnType::Type>::const_iterator t= types.find(fieldName) ;
		if( t == types.end() )
		{
			std::ostringstream	os;
			os << "Error in constructor dbSmartPersistent: missing type of filed \"" << fieldName
			<< "\" in types map.";
			ACS_THROW(ExecutionError(os.str() )) ; // PRQA S 3081
		} 


		_fields.push_back(dbColumn(_key, t->second)) ; // PRQA S 3081
		_fieldModified.push_back(false) ;
	}
	if(DBSMP -> hasIdField( _tableName, conn ) )
	{
		_idFieldPos = DBSMP -> getIdField( _tableName, conn ) ; // PRQA S 3000
    }
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_idFieldPos: " << _idFieldPos ) ;
}
				



dbSmartPersistent::~dbSmartPersistent() noexcept 
{
}

	
dbSmartPersistent::dbSmartPersistent(const dbSmartPersistent& r) :
	dbPersistent(r) ,
	_tableName(r._tableName),
    _fields(r._fields),
    _fieldModified(r._fieldModified),
    _filePaths(r._filePaths),
	_saveCopyDuringLoad(false),
    _idFieldPos(r._idFieldPos),
    _key(r._key),
	_disableNullCheck (r._disableNullCheck),
	_idxCastClobAsString_S (r._idxCastClobAsString_S)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_tableName: " << _tableName) ;
}


dbSmartPersistent &dbSmartPersistent::operator=(const dbSmartPersistent & rhs)
{
    if (this != &rhs) {
	    copy(rhs, true) ;
    }
    return *this ;
}


dbSmartPersistent & dbSmartPersistent::copy(const dbSmartPersistent & other, bool copyDbPersistent)
{
	Lock guard(*this) ; // PRQA S 3050 2
	Lock guard2(other);

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "This id = " << getUserIdentifier() 
		    << " Other id = " << other.getUserIdentifier()) ;
	 
	if(this != &other)
	{
		if(copyDbPersistent) {
			dbPersistent::operator=(other) ;
        }
		
		_tableName = other._tableName ;
		_saveCopyDuringLoad = other._saveCopyDuringLoad ;
 		_fields = other._fields ;
		_fieldModified = other._fieldModified ;
 		_filePaths = other._filePaths ;
        _idFieldPos = other._idFieldPos ;
        _key = other._key ;
		_disableNullCheck = other._disableNullCheck;
		_idxCastClobAsString_S = other._idxCastClobAsString_S;
		 
	}
	return *this ;
}




std::string dbSmartPersistent::className() const
{
	return "dbSmartPersistent" ;
}

dbPersistent * dbSmartPersistent::clone() const
{
	Lock guard(*this) ; // PRQA S 3050 2
	dbSmartPersistent *cloned = new dbSmartPersistent(*this) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "cloned: " << *cloned ) ;
    return cloned ;
} 


bool operator==(const dbSmartPersistent &lhs, const dbSmartPersistent &rhs) // PRQA S 4020
{
	Lock guard(lhs); // PRQA S 3050 2
	Lock guardO(rhs);
	
	if(&lhs == &rhs) { return true ; }
		
    const dbPersistent &basel = lhs ;
    const dbPersistent &baser = rhs ;
	if(basel != baser) { return false; }
	
	if(lhs._tableName != rhs._tableName) { return false; } 
	if(lhs._filePaths != rhs._filePaths) { return false ; }
	if(lhs._fields.size() != rhs._fields.size() ) {	return false; }    

    return equal(lhs._fields.begin(), lhs._fields.end(), rhs._fields.begin()) ; 
}


bool operator!=(const dbSmartPersistent &lhs, const dbSmartPersistent &rhs) 
{
	return not (lhs == rhs) ;
}


void dbSmartPersistent::reset()
{
	Lock guard(*this); // PRQA S 3050 
	dbPersistent::reset() ;
	_filePaths.clear() ;
	
    size_t fs = fieldsNumber() ;
	for( size_t i=0; i<fs ; i++)
	{
		_fields[i].reset() ;
	}
	resetAllFielsdModifiedFlag() ;
} 


SQLString dbSmartPersistent::saveQuery(bool &blobFieldsPresent,dbConnection &conn) const // PRQA S 4214
{
	//Prepare INSERT query for normal fields
	// nofifies that there are Blobs and clobs ...see later
	SQLString queryString(conn.getKey(), SQLString::INSERT) ;
    queryString.addTableList(_tableName) ;
	blobFieldsPresent = false ;	
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
	for(size_t i=0; i < fieldsNumber() ; i++)
	{
		bool isClobOrJsonb = (DBSMP -> isClob (_tableName , i, conn) || DBSMP -> isJsonb (_tableName , i, conn));
		// do not set values for CLOBs / BLOBs and SERIALs (the latter should be an autoincrement sequence)
		bool isBlob = DBSMP -> isBlob (_tableName , i, conn) ;
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
            if (isClobOrJsonb) {
			    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "clob presence detected for column " << i) ;
            }
		ACS_CLASS_END_DEBUG
        bool hasLob = false ;
        if (isClobOrJsonb && fieldIsNull(i)) {
			// Meaning that the corresponding field has _NOT_ been manually set, so the only way
			// to obtain the value is by reading a file -> set hasLob = T ;
			// Vice-versa, if the corresponding field has been manually set, it will be placed in the "INSERT query"
			// so -> set hasLob = F
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "clob presence detected and corresponding column(" << i << ") not set.") ;
            hasLob = true ;
            blobFieldsPresent = true ;
        }
		else if (isBlob) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "lob presence detected for column " << i ) ;
			hasLob = true ;
            blobFieldsPresent = true ;
		}
        else { /* nothing */  }
		
		if( not hasLob && not ( DBSMP -> isSerial (_tableName , i, conn) ) && not fieldIsNull(i) )
		{ 
			// Here for filled objects (also including CLOB/JSON), but not for BLOBs
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Executing: addValuesList. TableName:" << _tableName << " Field: " << i) ;
			queryString.addValuesList( DBSMP -> getFieldName( _tableName , i, conn ) ,
				                       getFieldSqlString(i, conn) );

		}
	}
	
	return queryString ;
}





//*****************************************   save  *****************************************
void dbSmartPersistent::savePreconditionCheck( dbConnection &conn ) 
{
	if(! (dbSchemaMetaDataProvider::instance(conn.getKey()) -> hasPrimaryKeys( _tableName, conn ) ))
	{
		ACS_THROW(InvalidUse("Save is not possible because there are not primary keys.")) ; // PRQA S 3081 
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "PKs check passed.") ;

	ACS_COND_THROW( isFromDatabase(),InvalidUse("Record has already been been saved: use the method update()."))  ; // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "FromDatabase check passed.") ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "record is modified: really save.") ;
    
	// NOTE: there could be the possibility to use some keys obtained from connection resources.
	//       For the moment this feature is not used. 

	checkNotNullFields(conn) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "checkNotNullFields passed." ) ;

}

namespace  {
    class CondTransaction { // manage a transaction if the ctor flag is true
    public:
        CondTransaction(dbConnection &conn, bool doTrans) : connection_(conn), transaction_()
        {
            if (doTrans) { 
                transaction_ = ACS_SMARTPTR<db::Transaction>(new db::Transaction(conn)) ; 
            }
        }
         
        ~CondTransaction() {}

        void begin_work(const std::string &name) 
        {
            if (0 != transaction_.get()) {
                transaction_->begin_work(name) ; 
            }
        }
        
        void commit() 
        {
            if (0 != transaction_.get()) {
                transaction_->commit() ; 
            }
        }
        
        void roll_back() 
        {
            if (0 != transaction_.get()) {
                transaction_->roll_back() ; 
            }
        }
         
        bool inTransaction() const
        {
            return (0 != transaction_.get() && transaction_->inTransaction() ) ;
        }
        
    private:    
        CondTransaction() ;
        CondTransaction(const CondTransaction &) ;
        CondTransaction& operator=(const CondTransaction &) ;
        
    private:
        dbConnection &connection_ ;
        ACS_SMARTPTR<db::Transaction> transaction_ ;
        
    } ;
}

dbPersistent::IdType dbSmartPersistent::save( dbConnection &conn ) // PRQA S 4020
{
	Lock guard(*this); // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Conn ptr: " << hex << &conn) ;

	try
	{

	    if (!isModified())
	    {
		    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "object does not result modified, so it will not be saved." ) ;
		    return getId() ; 
	    }
        savePreconditionCheck(conn) ;


		//Prepare INSERT query for normal fields
		//Blobs and clobs are uploaded with ad-hoc queries...see later
		bool hasBlobOrClobs = false ;
		
		SQLString queryString = saveQuery(hasBlobOrClobs, conn) ;

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "SQL command is \"" << queryString.getSQL() << "\"" ) ;
		
		// Start transaction only in case of blob or clob fields
		bool mustOpentransaction = hasBlobOrClobs ;
		try
		{
            CondTransaction transaction(conn, mustOpentransaction) ;

			// note that  is possible that the connection was in transaction
			// before calling the save method 


			if(mustOpentransaction) {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "mustOpentransaction: " << boolalpha << mustOpentransaction << " begin transaction") ;
                transaction.begin_work("SPINS " + _tableName) ; // NB is conditioned 
			}
			
			// If in transaction memento to connection	
			if(transaction.inTransaction()) { conn.addStateWrapper(*this) ; }

			dbQuery query(conn,queryString , "SPINS " + _tableName) ;
			if (!query.execForStatus())
			{
				string ErrorString ;
				ErrorString += "Query Execution error SQL command is \"" ;
				ErrorString += queryString.getSQL() ;
				ErrorString += "\". Error is \"" + query.errorString() + "\".";

				ACS_THROW(dbQuery::QueryExecuteException(ErrorString)) ; // PRQA S 3081
			}
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "new record inserted.") ;

            dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
			// If the table has a serial field obtain the last serial just inserted
			if(DBSMP-> hasSerialField( _tableName, conn ) )
			{
				IdType value=getLastSerial(conn, _tableName, "SP ")  ; // PRQA S 4412
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "new id is "<< value) ;
				setId(value) ;
			}

			// Upload all the files in clob/blob
			typedef std::map<size_t, std::string>::iterator mapIter ;
			for(mapIter iter=_filePaths.begin() ; iter != _filePaths.end() ; ++ iter ) // PRQA S 4238
			{
				string fieldName = DBSMP -> getFieldName( _tableName , iter->first, conn ) ; 
				uploadFile (fieldName, iter->second, conn);
			}
            transaction.commit() ;
		}
		catch(exception &)
		{
			unsetId() ; 
			throw ;
		}

		setFromDatabase() ; // record that the values come from the database
		resetAllFielsdModifiedFlag() ;

		// note the file paths can be erased else they will be used again and can be no longer valid
		_filePaths.clear() ;

		return getId() ;

	} //try
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e , "Error while saving record " + getUserIdentifier(conn) ) ) ; // PRQA S 3081
	}
	
} 

void dbSmartPersistent::setLOBFreeSelectList (	const string& tableName, SQLString& queryString, dbConnection &conn, \
												const std::set <size_t> idxCastClobAsString_S) 
{
	// When building a SELECT list, CLOBS and BLOBS have to be identified.
	// If _NOT_ included in the CastClobAsString set (i.e. they are "standard" binary object, handled via filesystem)
	// their corresponding column name is replaced with a INTEGER (driven by a CASE) which just allows to identify in the object is null or not-null
	// So the name of this method: Build a SELECT LIST without the names of the LOBs.
	
	const string& dbKey = conn.getKey();
	dbSchemaMetaDataProvider* DSMP = dbSchemaMetaDataProvider::instance(dbKey);

	vector<string> fieldList = DSMP -> getFieldNames( tableName, conn );
    size_t fl_size = fieldList.size();
	for (size_t idx=0; idx < fl_size; ++idx)
	{
		if ( DSMP -> isClobOrBlob( tableName , idx, conn) )
		{
			if (false == checkCastClobAsString(idx, idxCastClobAsString_S))
			{
				SQLString::SQLCase sql_case ;
				dbQueryParameters cond1(dbKey) ;	
				cond1.addParameter(dbParam(dbKey, tableName, fieldList[idx] , dbParam::ISNULL )) ;
				sql_case.addWhen(cond1, dbConv(dbKey, 1) ) ; // PRQA S 3050 2
				sql_case.setElse (dbConv(dbKey, 0));

				queryString.addSelectList (sql_case);
			} else {
						
				queryString.addSelectList ((tableName + "." + fieldList[idx]), SQLDialect::SQLCast(SQLDialect::SQL_VARCHAR));
			}
		}
		else
		{
            queryString.addSelectList (
                SQLDialect::convertColumnToString(dbKey, 
                                                  tableName + "." + fieldList[idx], 
                                                  DSMP->getFieldType (tableName, fieldList[idx], conn) ) 
                                       ); 
		}
	}
}


SQLString dbSmartPersistent::loadQuery(dbConnection &conn, IdType id) const // PRQA S 4214
{
    dbQueryParameters parameters(conn.getKey()) ;
	getWhereParameters(parameters , conn, id) ; //this also check if Id has been set

    SQLString queryString(conn.getKey(), SQLString::SELECT, true) ; //read only
    queryString.addTableList( _tableName ) ;

	// Build a list of columns, removing the LOBS but taking into account CastClobAsString
	dbSmartPersistent::setLOBFreeSelectList (_tableName, queryString, conn, _idxCastClobAsString_S) ;

    queryString.addANDWhereParameter(parameters) ;
	string orderBy=getOrderBy() ;
	if(!orderBy.empty()) {
		queryString.setOrderByString(orderBy) ;
    }
	
	return queryString ;
}

void dbSmartPersistent::getLOBFreeColumnTypes (	vector < dbSpecificColumnType::Type > & types, \
												const std::set <size_t> idxCastClobAsString_S) 
{
	// When building a SELECT list, CLOBS and BLOBS have to be identified.
	// If _NOT_ included in the CastClobAsString set (i.e. they are "standard" binary object, handled via filesystem)
	// their corresponding type is replaced with a INTEGER (driven by a CASE) which just allows to identify in the object is null or not-null
	// So the name of this method: Build a LIST OF TYPES without the type of the LOBs.


	size_t fs = types.size() ;
	for( size_t i=0; i<fs ; ++i) 
	{ 
		// CLOB and BLOB are set as int the others as native types, taking into account CastClobAsString
		if ( ( dbSpecificColumnType::BLOB == types[i] ) || ( dbSpecificColumnType::CLOB == types[i] ) )
		{
			if (false == checkCastClobAsString(i, idxCastClobAsString_S))
			{ types[i] = dbSpecificColumnType::SIGNED_INT; }
			else
			{ types[i] = dbSpecificColumnType::STRING; }
		}
	}
}

//*****************************************   load  *****************************************

bool dbSmartPersistent::load(dbConnection &conn , dbPersistent::IdType id) // PRQA S 4020
{
	dbPersistent *myCopy(0) ;
    
	try
	{
		Lock guard(*this); // PRQA S 3050

		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, getUserIdentifier(conn) << " id: " << id) ;
        
        const string &dbKey=conn.getKey() ;
        
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(dbKey) ;
		if(! (DBSMP -> hasPrimaryKeys( _tableName, conn ) ))
		{
			ACS_THROW(InvalidUse("Load is not possible because there are not primary keys.")); // PRQA S 3081
		}

		// Query returns integer for CLOB and BLOB fields, taking into account CastClobAsString
		SQLString queryString = loadQuery(conn, id) ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, getUserIdentifier(conn) << " SQL = \"" << queryString.getSQL() << "\".") ;
		
		//prepares types vector
		vector<dbSpecificColumnType::Type > types ;

        size_t fs = fieldsNumber() ;
		for( size_t i=0; i<fs ; ++i) 
		{ 
			types.push_back(_fields[i].getColumnType() ) ;	
		}

		// Replace types with a LOB Free version, taking into account CastClobAsString
		getLOBFreeColumnTypes (types, _idxCastClobAsString_S);

		ACS_CLASS_BGN_DEBUG(50)
			ostringstream dbg ;
            dbg << SimpleDebugSignatureThread << getUserIdentifier(conn) << ": Types are:\n" ;
            size_t ts = types.size() ;
			for( size_t i=0; i<ts ; ++i)
			{
				dbg << DBSMP -> getFieldName( _tableName , i, conn ) <<" : "
                    << dbColumnTypeHelper::getTypeName(types[i]) << "\n" ;
			}
            ACS_LOG_DEBUG(dbg.str()) ; 
		ACS_CLASS_END_DEBUG

		dbRow row(dbKey) ;
		
		try	{
			dbQuery query(conn, queryString, "SPSEL " + _tableName) ;
            dbSet set ; 
	        query.execForSet(set, types) ;

            ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
            ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
            set.getNextRow(row) ; // first and only 
   		}
		catch (dbQuery::QueryNoDataFound &e) {
			return false ;
		}
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, getUserIdentifier(conn) << ": Query successfully executed.") ;

		try {
			if(_saveCopyDuringLoad)	{
				// If in transaction, then send memento to connection. Otherwise save locally	
				bool transactionStarted = false ;
				transactionStarted = (conn.transactionState() == dbConnection::Begin) ; 

				if(transactionStarted) { conn.addStateWrapper(*this) ; }
				else { myCopy=clone() ; }
			}
			reset() ;
	
			// LoadRow will remap the hidden LOB values reporting only if the relevant fields are null or not
			loadRow(row, 0) ;
		}
		catch(std::exception &) {
			if(myCopy) { *this = *dynamic_cast<dbSmartPersistent*>(myCopy) ; } // PRQA S 3081
			throw ;
		}
		
		setFromDatabase() ;
		resetAllFielsdModifiedFlag() ;

		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, getUserIdentifier(conn) << ": Dump: \"\n" << *this ) ;

	}
	catch(exception &e)
	{
		delete myCopy ;
		ACS_THROW( ExecutionError(e , "Error while loading record " + getUserIdentifier(conn) ) ) ;// PRQA S 3081
	}

	delete myCopy ;
	return true ;
}



SQLString dbSmartPersistent::updateQuery(vector<size_t> &updatedFields, dbConnection &conn) const  // PRQA S 4020, 4214
{
    const string &dbKey=conn.getKey() ;
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(dbKey) ;
	{
		std::vector<size_t>	updatableFields = DBSMP ->getUpdatableFields(_tableName, conn) ;
		size_t us = updatableFields.size() ;
        for(size_t i=0; i< us ; ++i)
		{
			if ( isFieldModified(updatableFields[i], conn ) )
			{
				if( (!DBSMP -> isUploadableObject (_tableName , updatableFields[i], conn)) || checkCastClobAsString(updatableFields[i]) ) 
				{
					if( DBSMP -> notNull( _tableName , updatableFields[i], conn )
					    && fieldIsNull(updatableFields[i]) )
					{
						if (_disableNullCheck)
						{
							ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Skipping NULL checks and relying on the DBMS integrity rules");
						}
						else
						{
							ACS_THROW(NullValueException("Cannot set to NULL field " + DBSMP -> getFieldName( _tableName,  updatableFields[i], conn )) ) ; // PRQA S 3081
						}
					}
				    // allowable update fields are just the ones not LOBs
				    updatedFields.push_back(updatableFields[i] ) ;
				}
                else if ( fieldIsNull(updatableFields[i]) ) {
				    // allowable update fields are just the ones not LOBs
				    updatedFields.push_back(updatableFields[i] ) ;
                }
			}
		}
	}

	SQLString queryString (dbKey, SQLString::UPDATE ) ;
	
	//Is there something to do??
	if( updatedFields.empty() && _filePaths.empty() )
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, getUserIdentifier(conn)
                << ": object has not updatable fields nor LO to upload, so it will not be updated.") ;
		return queryString ;
	}
	
	if( ! updatedFields.empty() )
	{
		queryString.addTableList(_tableName) ;
        size_t us = updatedFields.size() ;
		for(size_t i=0; i< us ; i++)
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier(conn) << ": adding to query field n. " << updatedFields[i]) ;
			queryString.addSetList( DBSMP -> getFieldName( _tableName , updatedFields[i], conn ) ,
			                        getFieldSqlString( updatedFields[i], conn)  );
		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "SQL = \"" << queryString.getSQL() << "\".") ;
	}

	return queryString ;
}		


//*****************************************   update  *****************************************

dbPersistent::IdType dbSmartPersistent::update(dbConnection &conn, const dbQueryParameters &customWhereParams, bool& updated)
{
	Lock guard(*this); // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, getUserIdentifier(conn) << ": Start") ;

	try{

        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
		if(! (DBSMP -> hasPrimaryKeys( _tableName, conn ) ))
		{
			ACS_THROW(InvalidUse("Update is not possible because there are not primary keys.")) ; // PRQA S 3081
		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier(conn) << ": PKs check passed. ") ;

		
		if(_idFieldPos != -1 ) // has id field
		{
			if(0 == getId()) 
			{
				ObjectNotLoaded e("The object has an id=0 : Use the method save() instead of update().") ;
				ACS_THROW(e) ;
			}
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier(conn) << ": ID check passed.") ;
		}
		vector<size_t> updatedFields ;
		SQLString queryString = updateQuery(updatedFields, conn) ;
		
		// here we assume that if the updateQuery has no field to set the query does not contain either the table
		bool mustOpentransaction = !_filePaths.empty() ;
		dbQueryParameters parameters(conn.getKey()) ;
		getWhereParameters(parameters , conn, 0) ; //this also check if Id has been set 
		queryString.addANDWhereParameter(parameters) ;
		queryString.addANDWhereParameter(customWhereParams); // Add user-defined query parameters

        CondTransaction transaction(conn, mustOpentransaction) ;
		if( mustOpentransaction) { transaction.begin_work("SPUPD " + _tableName) ; }

		if(transaction.inTransaction()) { conn.addStateWrapper(*this) ; }

		if( ! updatedFields.empty() )
		{
			dbQuery query(conn, queryString, "SPUPD " + _tableName) ;
			query.execForStatus() ;
			updated = query.getAffectedRows() > 0;
		}
		// Upload all the files in clob/blob
		typedef std::map<size_t, std::string>::iterator mapIter ;
		for(mapIter iter=_filePaths.begin() ; iter != _filePaths.end() ; ++ iter ) // PRQA S 4238
		{
			uploadFile (iter->first, iter->second, conn);
		}
		transaction.commit() ;

		// note the file paths can be erased else they will be used again and can be no longer valid
		_filePaths.clear() ;
		resetAllFielsdModifiedFlag() ;


		return getId() ;
	} //try
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e ,  "Error while updating record " + getUserIdentifier(conn) ) ) ;// PRQA S 3081
	}
}

dbPersistent::IdType dbSmartPersistent::update(dbConnection &conn)
{
	bool updated = false; // Not used - Without any additional parameters on WHERE clause, the affectedRows will always be 1 if the record exists
	return update(conn, dbQueryParameters(conn.getKey()), updated);
}


SQLString dbSmartPersistent::removeQuery(dbConnection &conn) const  // PRQA S 4214
{
	dbQueryParameters parameters(conn.getKey()) ;
	getWhereParameters(parameters ,conn, 0) ; //this also check if Id has been set 

    SQLString queryString (conn.getKey(), SQLString::DELETE ) ;
	queryString.addTableList(_tableName) ;
	queryString.addANDWhereParameter(parameters) ;

	return queryString ;
}


//*****************************************   remove  *****************************************

dbPersistent::IdType dbSmartPersistent::remove(dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, getUserIdentifier(conn)) ;

	if(! (dbSchemaMetaDataProvider::instance(conn.getKey()) -> hasPrimaryKeys( _tableName, conn ) ))
	{
		ACS_THROW(InvalidUse("Remove on table \"" + _tableName + "\" is not possible because there are not primary keys.")) ; // PRQA S 3081
	}

	SQLString queryString = removeQuery(conn) ;

	dbQuery query(conn, queryString , "SPDEL " + _tableName) ;

	query.execForStatus() ;

	resetFromDatabase() ;
	setAllFieldModifiedFlags() ;

	return getId() ;

}







bool dbSmartPersistent::validate() const
{
	return true ;
} 


void dbSmartPersistent::writeTo(std::ostream &os) const
{
	Lock guard(*this); // PRQA S 3050
    size_t fs = fieldsNumber() ;
    dbSchemaMetaDataProvider *DBSMP = dbSchemaMetaDataProvider::instance(_key) ;
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S 3050
	for(size_t i=0; i< fs ; ++i)
	{
		os << DBSMP -> getFieldName( _tableName , i, conn ) <<" : " 
		   << dbColumnTypeHelper::getTypeName( _fields[i].getColumnType() ) <<" : " ;
			os << "\"" <<_fields[i] <<	"\"\n" ;
	}
}


void dbSmartPersistent::writeTo(exostream &os) const
{
	Lock guard(*this); // PRQA S 3050

    size_t fs = fieldsNumber() ;
    dbSchemaMetaDataProvider *DBSMP = dbSchemaMetaDataProvider::instance(_key) ;
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S 3050
	for(size_t i=0; i< fs ; ++i)
	{
		os << DBSMP -> getFieldName( _tableName , i, conn ) <<" : " 
		   << dbColumnTypeHelper::getTypeName( _fields[i].getColumnType() ) <<" : " ;
			os << "\"" <<_fields[i] <<	"\"\n" ;
	}
}


#warning Import/export methods still not implemented
void dbSmartPersistent::exportObj(rsResourceSet &set) const
{
	ACS_THROW(NotImplemented("Import/export methods not still implemented.")) ; // PRQA S 3081
}

void dbSmartPersistent::importObj(rsResourceSet &)
{
	ACS_THROW(NotImplemented("Import/export methods not still implemented.")) ; // PRQA S 3081
	// useless Lock guard(*this); // PRQA S 3050
}

void dbSmartPersistent::exportXML(XMLOstream &) const
{
	ACS_THROW(NotImplemented("Import/export methods not still implemented.")) ; // PRQA S 3081
	// useless Lock guard(*this); // PRQA S 3050
}






void dbSmartPersistent::setId(IdType id)
{
	Lock guard(*this); // PRQA S 3050
    dbPersistent::setId(id) ;
	try{
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "id: " << id) ;
		if(_idFieldPos != -1)
		{
			size_t idPos = _idFieldPos ; // PRQA S 3000
			switch(  _fields[idPos].getColumnType() ) {
            case dbSpecificColumnType::SIGNED_INT:
			    {
				    signed int newId = 0;
				    convertToDifferentType(id, newId) ;
				    _fields[idPos].setColumn (newId) ;
                    break ;
			    }
			case dbSpecificColumnType::SIGNED_LONG_LONG_INT:
			    {
				    signed long long int newId = 0 ;
				    convertToDifferentType(id, newId) ;
				    _fields[idPos].setColumn (newId) ;
                    break ;
			    }
			case dbSpecificColumnType::SIGNED_SHORT_INT:
			    {
				    signed short int newId = 0 ;
				    convertToDifferentType(id, newId) ;
				    _fields[idPos].setColumn (newId) ;
                    break ;
			    }
			default:
			    {
				    ostringstream err ;
				    err << "Cannot set id when id field is of type " << dbColumnTypeHelper::getTypeName( _fields[idPos].getColumnType() ) ;
				    ACS_THROW(ErrorInNumericConversion(err.str()) ) ; // PRQA S 3081
			    }
            }
		}
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e, "Error in setId. Record id is "+ getUserIdentifier() )) ;// PRQA S 3081
	}

}


dbPersistent::IdType dbSmartPersistent::getId() const // PRQA S 4020
{
	Lock guard(*this); // PRQA S 3050
	if(_idFieldPos != -1 )
	{
		size_t pos = _idFieldPos ; // PRQA S 3000
		
		if( fieldsNumber() < pos ) {
			return -1 ; // PRQA S 2890
        }
		
		if( fieldIsNull(pos) ) {
			return 0 ;
        }
		else {
			IdType id = 0;
			return _fields[ pos ].column(id) ;
		}
	}
	else {
		return 0 ;
    }
}

void dbSmartPersistent::get(size_t pos, dbGeoObject * & value, dbConnection &conn ) const 
{
	Lock lock(*this) ;  // PRQA S 3050

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in get: " << pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	}

	_fields[pos].column(value) ;
} 



void dbSmartPersistent::get(std::string const &fieldName,dbGeoObject * & value, dbConnection &conn ) const 
{
	get( dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ) , value, conn ) ;
}



void dbSmartPersistent::unset(size_t pos)
{
	Lock lock(*this) ; // PRQA S 3050
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S 3050
	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in unset: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
	
	if ( dbSchemaMetaDataProvider::instance(_key) -> isClobOrBlob( _tableName , pos, conn ) && !checkCastClobAsString(pos) ) 
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in unset: it is not possible to unset field at position "<< pos << " which is BLOB or CLOB.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
	
	unsetFilePath(pos) ; // to revert any possible setFilePath on pos
	_fields[pos].reset() ;
	setSingleFieldModifiedFlag(pos, conn) ;
}


void dbSmartPersistent::unset(size_t pos, dbConnection &conn)
{
	Lock lock(*this) ; // PRQA S 3050

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in unset: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
	
	if( dbSchemaMetaDataProvider::instance(conn.getKey()) -> isClobOrBlob( _tableName , pos, conn ) && !checkCastClobAsString(pos) )
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in unset: it is not possible to unset field at position "<< pos << " which is BLOB or CLOB.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
	
	_fields[pos].reset() ;
	setSingleFieldModifiedFlag(pos, conn) ;
}


std::string dbSmartPersistent::getTableName() const
{
	Lock guard(*this); // PRQA S 3050
	return _tableName;
}



size_t dbSmartPersistent::fieldsNumber() const
{
	return (_fields.size()) ; 
}

bool dbSmartPersistent::fieldIsNull(const size_t& i) const
{
	 return _fields[i].isNull() ; 
}


void dbSmartPersistent::saveCopyDuringLoad(bool v)
{
	Lock guard(*this); // PRQA S 3050
	_saveCopyDuringLoad = v ;
}

bool dbSmartPersistent::saveCopyDuringLoad() const
{
	return _saveCopyDuringLoad ;
}


std::vector<std::string> dbSmartPersistent::getFieldNames(dbConnection &conn) const
{
	return dbSchemaMetaDataProvider::instance(conn.getKey())->getFieldNames(_tableName, conn) ;
}


void dbSmartPersistent::unset(std::string const &fieldName)
{
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S  3050
	unset(dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName( _tableName , fieldName, conn ), conn ) ;
}


void dbSmartPersistent::unset(std::string const &fieldName, dbConnection &conn)
{
	unset(dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ), conn ) ;
}


bool dbSmartPersistent::isSet(size_t pos) const // PRQA S 4020
{
    return isSet(pos, dbConnectionWrapper(db::ConnPools::instance()->getPool(_key))) ; // PRQA S 3050, 3081
}

bool dbSmartPersistent::isSet(size_t pos, dbConnection &conn) const // PRQA S 4020
{
	Lock guard(*this); // PRQA S 3050
	try{
        const string& dbKey=conn.getKey() ;
		if(pos >= fieldsNumber())
		{
			std::ostringstream	os;
			os << getUserIdentifier(conn) << ": error in isSet: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		    ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
		}
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(dbKey) ;
		if( DBSMP -> isUploadableObject( _tableName , pos, conn ) && (!checkCastClobAsString(pos)) ) 
		{
			string fieldName = DBSMP->getFieldName(_tableName , pos, conn) ; 

			SQLString queryStr(dbKey, SQLString::LOBJ_ISNULL);

			queryStr.addTableList(_tableName);
			queryStr.setColumnName( fieldName );
			dbQueryParameters parameters(dbKey) ;
			getWhereParameters(parameters , conn, getId() ) ;
			queryStr.addANDWhereParameter(parameters) ;
			
			dbQuery checkQuery( conn , queryStr, "SPLOB_CK " + _tableName );

			if( DBSMP ->isClob(_tableName , pos, conn) || DBSMP ->isJsonb(_tableName , pos, conn) ) { 
				return !(checkQuery.clobIsNull());
            }
			else if( DBSMP ->isBlob(_tableName , pos, conn) ) { 
				return !(checkQuery.blobIsNull());
            }
            else {
                // nothing 
            }
		}

		return (! fieldIsNull(pos) ) ;
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e, "Error in isSet. Record id is "+ getUserIdentifier(conn) )) ;// PRQA S 3081
	}
}


bool dbSmartPersistent::isSet(std::string const &fieldName) const 
{
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S 3050
	return isSet( dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName( _tableName , fieldName, conn ), conn ) ;
}


bool dbSmartPersistent::isSet(std::string const &fieldName, dbConnection &conn) const 
{
	return isSet( dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName( _tableName , fieldName, conn ), conn ) ;
}



bool dbSmartPersistent::isFieldModified(size_t pos) const
{
	Lock guard(*this); // PRQA S 3050
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S 3050

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in isModified: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}

	return (_fieldModified[pos]) ;
}



bool dbSmartPersistent::isFieldModified(size_t pos, dbConnection &conn) const
{
	Lock guard(*this); // PRQA S 3050

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in isModified: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}

	return (_fieldModified[pos]) ;
}



bool dbSmartPersistent::isFieldModified(std::string const &fieldName) const 
{
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;
    dbConnection &conn = cw ; // PRQA S 3050 
	return isFieldModified( dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName( _tableName , fieldName, conn ), conn ) ;
}





bool dbSmartPersistent::isFieldModified(std::string const &fieldName, dbConnection &conn) const 
{
	return isFieldModified( dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ), conn ) ;
}



void dbSmartPersistent::setFilePath(size_t pos, string const & filePath, dbConnection &conn)
{
	Lock guard(*this); // PRQA S 3050
    const string &dbKey = conn.getKey() ;
	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in setFilePath: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
		
	if( !dbSchemaMetaDataProvider::instance(dbKey) -> isUploadableObject( _tableName , pos, conn ) || checkCastClobAsString(pos) ) 
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in setFilePath: field at position "<< pos << " is not BLOB nor CLOB.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
	
	_filePaths[pos]=filePath ;
    _fieldModified[pos] = false ; // reverts any "unset" on pos

	setSingleFieldModifiedFlag(pos, conn) ;
}


void dbSmartPersistent::setFilePath(std::string const &fieldName, string const & filePath, dbConnection &conn) 
{
	Lock guard(*this); // PRQA S 3050

    // Perform the check here to have an exception containing the fiel name instead of the position
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;

	if(!DBSMP -> isUploadableObject( _tableName , fieldName, conn ) || checkCastClobAsString(fieldName, conn) ) 
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in setFilePath: field \""<< fieldName << "\" is not BLOB nor CLOB.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}
	setFilePath( DBSMP -> getFieldFromName( _tableName , fieldName, conn ) , filePath, conn) ;
}


void dbSmartPersistent::unsetFilePath(size_t pos)
{
	Lock guard(*this); // PRQA S 3050

	_filePaths.erase(pos) ;
}


void dbSmartPersistent::unsetFilePath(std::string const &fieldName, dbConnection &conn)
{
	unsetFilePath(dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ) ) ;
}


std::string dbSmartPersistent::getFilePath(size_t pos) const // PRQA S 4020
{
	if( _filePaths.find(pos) != _filePaths.end() ) {
		return _filePaths.find(pos)->second ;
    }
	else {
		return "" ;
    }
}

std::string dbSmartPersistent::getFilePath(std::string const &fieldName, dbConnection &conn) const 
{
	return getFilePath ( dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ) ) ;
}


int dbSmartPersistent::loadRow(const dbRow & row, int offset)
{
	Lock guard(*this); // PRQA S 3050

	// Load ROW will be used with a row containint INT instead of CLOBs and BLOBs
	// a cross check is needed here to map into the correct types and values

	size_t i=0 ;
    size_t fs = fieldsNumber() ;
	for(; i< fs ; ++i) // PRQA S 4231
	{

		dbSpecificColumnType::Type columnType = _fields[i].getColumnType ();
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Column " << i << " is " << dbColumnTypeHelper::getTypeName( columnType )) ;

		if ( (( dbSpecificColumnType::BLOB == columnType ) || ( dbSpecificColumnType::CLOB == columnType )) && !checkCastClobAsString(i) )
		{
			// Select will report 1 in case the clob / blob is null in the row
			int nullclobSelect = 0 ;
			row.column(uint16_t(offset+i)).column(nullclobSelect); // PRQA S 3000, 3081, 3084

			// Just for sake of simplicity
			bool nullclob = (1 == nullclobSelect);
			_fields[i].init (columnType, nullclob);
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Column " << i << " is CLOB/BLOB and is " << (nullclob ? "" : "NOT") << " NULL") ; // PRQA S 3380

		}
		else
		{
			_fields[i] = row.column(  offset + i) ; // PRQA S 3000, 3010, 3084
		}

        if (long(i) == _idFieldPos) { // PRQA S 3081
            if( not fieldIsNull(i)) {
			    IdType id = 0;
			    setId(_fields[ i ].column(id)) ;
		    }
        }	
    }
	setFromDatabase() ;

	return offset + i ; // PRQA S 3000, 3010, 3084


}


bool dbSmartPersistent::uploadFile (size_t fieldPosition, 
                                    const string& filePath, 
                                    dbConnection& conn) const
{
	Lock guard(*this); // PRQA S 3050

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
	string fieldName = DBSMP -> getFieldName( _tableName , fieldPosition, conn ) ; 
	return uploadFile (fieldName, filePath, conn);
}

bool dbSmartPersistent::uploadFile (size_t fieldPosition, 
                                    std::string const & filePath, 
                                    dbConnection & conn, 
                                    dbQueryParameters const & parameters) const 
{
	Lock guard(*this); // PRQA S 3050

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
	string fieldName = DBSMP -> getFieldName( _tableName , fieldPosition, conn ) ; 
	return uploadFile (fieldName, filePath, conn, parameters);
}


bool dbSmartPersistent::uploadFile (const string& fieldName, 
                                    const string& filePath, 
                                    dbConnection& conn) const
{
	Lock guard(*this); // PRQA S 3050

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
	dbQueryParameters parameters(conn.getKey()) ;
	getWhereParameters(parameters , conn, getId() ) ;
    return uploadFile(fieldName, filePath, conn, parameters) ;
}

bool dbSmartPersistent::uploadFile (const string& fieldName, 
                                    const string& filePath, 
                                    dbConnection& conn, 
                                    dbQueryParameters const &parameters) const
{
	Lock guard(*this); // PRQA S 3050

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;

	dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
	size_t fieldPos = DBSMP -> getFieldFromName( _tableName , fieldName, conn ) ;

	SQLString uploadQueryString(conn.getKey(),  SQLString::LOBJ_UPLOAD ) ;
	uploadQueryString.setColumnAndFileName( fieldName , filePath );
	uploadQueryString.addTableList(_tableName);
	uploadQueryString.addANDWhereParameter(parameters) ;

   	dbQuery uploadQuery( conn , uploadQueryString , "SPUPL " + _tableName );
	if( DBSMP ->isClob(_tableName , fieldPos, conn) || DBSMP ->isJsonb(_tableName , fieldPos, conn) ) { 
		uploadQuery.uploadClob();
    }
	else if( DBSMP ->isBlob(_tableName , fieldPos, conn) ) {
		uploadQuery.uploadBlob();
    }
    else {
        // nothing 
    }
    
    return uploadQuery.getAffectedRows() > 0 ; // hoping on unique where param
}

void dbSmartPersistent::downloadFile(size_t pos , std::string  const & filePath , dbConnection &conn) const
{
	Lock guard(*this); // PRQA S 3050

    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
	string fieldName = DBSMP ->getFieldName(_tableName , pos, conn ) ; 
	
	if( ! ( DBSMP -> isUploadableObject (_tableName , pos, conn ) ) ) 
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": Cannot download the file \""<< filePath << "\" because field n. "
		   << pos << " named \"" <<  fieldName << "\" is not blob nor clob.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}

	if(!isFromDatabase())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": Cannot download the file \""<< filePath << "\" because record is not from db.";
		ACS_THROW(InvalidUse(os.str() ))  ; // PRQA S 3081
	}

    SQLString donwloadQueryString(conn.getKey(), SQLString::LOBJ_DOWNLOAD ) ;

    donwloadQueryString.setColumnAndFileName( fieldName , filePath );
    donwloadQueryString.addTableList(_tableName);
	dbQueryParameters parameters(conn.getKey()) ;
	getWhereParameters(parameters , conn, getId() ) ;
	donwloadQueryString.addANDWhereParameter(parameters) ;

	dbQuery downloadQuery( conn , donwloadQueryString, "SPDWL " + _tableName );

	if( DBSMP ->isClob(_tableName , pos, conn) || DBSMP ->isJsonb(_tableName , pos, conn) ) { 
		downloadQuery.downloadClob();
    }
	else if( DBSMP ->isBlob(_tableName , pos, conn) ) {
		downloadQuery.downloadBlob();
    }
    else {
        // nothing 
    }
}



void dbSmartPersistent::downloadFile(std::string const &fieldName , std::string const & filePath , dbConnection &conn) const
{
	downloadFile( dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn )  ,filePath , conn) ;
}


string dbSmartPersistent::getOrderBy() const
{
	return "" ;
}


std::string dbSmartPersistent::getUserIdentifier() const noexcept //  PRQA S 4020
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

std::string dbSmartPersistent::getUserIdentifier(dbConnection &conn) const noexcept // PRQA S 4020
{
 	try
	{
	    Lock guard(*this);  // PRQA S 3050
	    ostringstream result ;
	    if( _tableName.empty() ) {
		    return "UNSET" ;
        }
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
		result << "\"" <<_tableName  ;
		if(DBSMP -> hasIdField( _tableName, conn ) )
		{
			IdType id=getId() ;
			if(id!=0) {
				result << "/" << id ;
            }
		}
		else
		{
			std::vector<size_t> primaryKeys=DBSMP -> getPrimaryKeys (_tableName, conn) ;
			size_t pks = primaryKeys.size() ;
            for(size_t i=0 ; i< pks ; ++i )
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "analyzing primary key n " << primaryKeys[i]) ;
				result << "/" <<DBSMP -> getFieldName (_tableName , primaryKeys[i], conn) <<"=" ;
				result << _fields[ primaryKeys[i] ] ;
			}
		}
	    result <<"\"" ; 
	    return result.str() ;
	}
	catch(exception &e)
	{
		string errorMessage ;
		errorMessage+= "exception is: " ;
		errorMessage+= e.what() ;
		return errorMessage ;
	}
}


bool dbSmartPersistent::checkCastClobAsString(const size_t& idx, const std::set <size_t>& idxCastClobAsString_S) 
{
	return (idxCastClobAsString_S.find(idx) != idxCastClobAsString_S.end()) ; 
}

bool dbSmartPersistent::checkCastClobAsString(const string& tableName, 
												const string& column_name, 
												dbConnection& conn, 
												const std::set <size_t>& idxCastClobAsString_S) 
{
 	const string& dbKey = conn.getKey();
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(dbKey) ;
    size_t fields = DBSMP->fieldsNumber(tableName, conn) ;
	ssize_t idx (-1) ; 
	for( size_t i=0; i< fields ; ++i)
	{
		const string fieldName ( DBSMP->getFieldName(tableName, i, conn) ) ;
		if (column_name == fieldName) { idx = i ; break ; }
	}
	
	if (idx < 0)
	{
		std::ostringstream	os;
		os << "Table: \"" << tableName << "\" does not seem to contain column \"" << column_name << "\" provided in method checkCastClobAsString" ; 
		ACS_THROW(ExecutionError(os.str() )) ; 	
	}

	return checkCastClobAsString(idx, idxCastClobAsString_S) ; 
}


void dbSmartPersistent::castClobAsString(const string& column_name, dbConnection& conn, const bool to_cast) 
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_tableName: " << _tableName << " column_name: \"" << column_name << "\"" ) ;
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(_key) ;
    size_t fields = DBSMP->fieldsNumber(_tableName, conn) ;
	ssize_t idx (-1) ; 
	for( size_t i=0; i< fields ; ++i)
	{
		const string fieldName ( DBSMP->getFieldName(_tableName, i, conn) ) ;
		if (column_name == fieldName) { idx = i ; break ; }
	}
	
	if (idx < 0)
	{
		std::ostringstream	os;
		os << "Table: \"" << _tableName << "\" does not seem to contain column \"" << column_name << "\" provided in method castClobAsString" ; 
		ACS_THROW(ExecutionError(os.str() )) ; 	
	}
	
	if (to_cast)	
	{
		 _idxCastClobAsString_S.insert(idx) ; 
		 if (static_cast<size_t>(idx) < fieldsNumber())
		 {
			_fields[idx] = dbColumn(_key, dbSpecificColumnType::STRING) ; 
		 }
	}
	else			
	{	
		_idxCastClobAsString_S.erase(idx) ; 
		if (static_cast<size_t>(idx) < fieldsNumber())
		{
			_fields[idx] = dbColumn(_key, DBSMP->getFieldType(_tableName,idx, conn)) ; 
		}
	}	
	
		
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_tableName: " << _tableName << " column_name: \"" << column_name << "\"" \
		<< (to_cast?"Added":"Removed") << " index: " << idx ) ;
}


void dbSmartPersistent::disableNullCheck (bool b) noexcept // PRQA S 4121
{
	_disableNullCheck = b;
}

bool dbSmartPersistent::disableNullCheck () const noexcept // PRQA S 4120
{
	return _disableNullCheck;
}


void dbSmartPersistent::checkNotNullFields(dbConnection &conn) // PRQA S 4020
{
	// NOP in case the null check has been disabled
	if (_disableNullCheck) 
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Skipping NULL checks and relying on the DBMS integrity rules");
		return;
	}

	Lock guard(*this); // PRQA S 3050

	try{
	    size_t fs =fieldsNumber() ;
		for(size_t i=0; i< fs ; ++i)
		{
			try{
			
				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, getUserIdentifier(conn) << ": checking field n." <<i) ;
                dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;

				if( DBSMP -> notNull( _tableName , i, conn ) && 
					! DBSMP -> hasDefault( _tableName , i, conn ) ) 
				{
					// Note that following instruction fails (with exception) in case of Blob / Clob
					// Which means that LO fields cannot be mandatory in the schema.
					if( fieldIsNull(i)  )
					{
						if(! DBSMP -> isSerial( _tableName , i, conn ) ) 
						{
							ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier(conn) << ": field n." <<i << " must be set.") ;
							ACS_THROW(InvalidUse("Mandatory field \"" + DBSMP -> getFieldLabel( _tableName , i, conn ) + "\" must be set.")) ; // PRQA S 3081
						}

					}
				}

			}
			catch(dbColumn::CannotCheckNull &e)
			{
				std::ostringstream	os;
				os << " error in checkNotNullFields: it is not possible to have field "<< i << " not nullable. Please modify your schema." ;
				ACS_THROW(InvalidUse(e, os.str() )) ;
			}
			
		}
	}
	catch(exception &e)
	{
		ACS_THROW(InvalidUse(e, "Error in dbSmartPersistent::checkNotNullFields")) ;
	}

}

bool dbSmartPersistent::canBeLoaded(IdType id, dbConnection &conn) const // PRQA S 4020, 4214
{
	if(dbSchemaMetaDataProvider::instance(conn.getKey()) -> hasIdField( _tableName, conn ) )
	{
        IdType d_id = discoverId(id, conn) ;
        return (d_id != 0) && (d_id != -1)  ; // note it is unsigned
    }
    else {
        return true ;
    }
}

dbPersistent::IdType dbSmartPersistent::discoverId(IdType id, dbConnection &conn) const // PRQA S 4214
{
    IdType usedId=id;
	if(usedId == 0) {
		usedId = getId() ;
    }
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier() << ": Table has ID field called with id= " << id << " getId()= " << getId() << " --> Used id= " << usedId) ;
	return usedId ;
}

void dbSmartPersistent::getWhereParameters(
    dbQueryParameters &parameters, 
    dbConnection &conn, 
    IdType requestedId) const
{
	Lock guard(*this); // PRQA S 3050
    dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier(conn) << ": called with id= " << requestedId) ;
	try
	{
	
		if(DBSMP -> hasIdField( _tableName, conn ) )
		{
			//First see if requestedId is valid (!= 0). If not use local id (using method getId() )
			IdType usedId=discoverId(requestedId, conn) ;
			if(0 == usedId) 
			{
				ACS_THROW(IdNotProvided("Id has not been set for record " + getUserIdentifier(conn))) ; // PRQA S 3081
			}

			parameters.addParameter(dbParam( conn.getKey(), 
                                             _tableName ,
		                                	 DBSMP ->getIdFieldName(_tableName, conn) ,
		                                	 dbParam::EQUAL_TO,
											 dbConv(conn.getKey(), usedId ) )) ; // PRQA S 3050
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, getUserIdentifier(conn) << ": Table has not ID field.") 
			std::vector<size_t> primaryKeys=DBSMP -> getPrimaryKeys (_tableName, conn) ;
			if(primaryKeys.empty()) 
			{
				string errorMessage("No primary keys for record ") ;
				errorMessage += getUserIdentifier(conn) ;
				ACS_THROW(IdNotProvided(errorMessage)) ; // PRQA S 3081
			}
            size_t pks =  primaryKeys.size() ;
			for(size_t i=0 ; i<pks ; ++i )
			{
				if( fieldIsNull(primaryKeys[i]) ) {
					parameters.addParameter(dbParam(conn.getKey(),
                                                    _tableName ,
				                                    DBSMP -> getFieldName (_tableName , primaryKeys[i], conn) ,
													dbParam::ISNULL )) ;
				}
                else {
					parameters.addParameter(dbParam(conn.getKey(),
                                                    _tableName ,
				                                    DBSMP -> getFieldName (_tableName , primaryKeys[i], conn) ,
													dbParam::EQUAL_TO,
													getFieldSqlString(primaryKeys[i], conn ) )) ;
			    }
            }
		}
	}
	catch(exception &e)
	{
		ACS_THROW( ExecutionError(e, "Error in getWhereParameters. Record id is "+ getUserIdentifier(conn) )) ;// PRQA S 3081
	}
}


void dbSmartPersistent::resetAllFielsdModifiedFlag()
{
	Lock guard(*this); // PRQA S 3050

    size_t fms = _fieldModified.size() ;
	for( size_t i = 0 ; i < fms ; ++i) {
		_fieldModified[i] = false;
    }
		
	dbPersistent::resetModifiedFlag() ;
}

void dbSmartPersistent::setAllFieldModifiedFlags()
{
	Lock guard(*this); // PRQA S 3050
    size_t fms = _fieldModified.size() ;
	for( size_t i = 0 ; i < fms ; ++i) {
		_fieldModified[i] = true;
    }
		
	dbPersistent::setModifiedFlag() ;
}

void dbSmartPersistent::setSingleFieldModifiedFlag(size_t pos, dbConnection &conn)
{
	Lock guard(*this); // PRQA S 3050

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in setSingleFieldModifiedFlag: " << pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	}

	_fieldModified[pos] = true;
	dbPersistent::setModifiedFlag() ;
}

std::string dbSmartPersistent::getFieldSqlString(size_t pos, dbConnection &conn) const // PRQA S 4020
{
	Lock guard(*this); // PRQA S 3050

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in getFieldSqlString: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.";
		ACS_THROW(ExecutionError(os.str() )) ; // PRQA S 3081
	}

	
	// For DATETIME case it shall be provided the exact number of strings.
	if( _fields[pos].getColumnType () == dbSpecificColumnType::DATETIME)
	{
		if( fieldIsNull(pos) ) {
			return _fields[pos].getSqlString() ;  //Just returns null string.
		}
		DateTime tmp;
		_fields[pos].column(tmp);
		dbConv::DateTimeMode dbctm = dbColumnTypeHelper::columnToConvDateTimeMode( dbSchemaMetaDataProvider::instance(conn.getKey()) ->  getDateTimeMode( _tableName ,pos, conn ) ) ; 
		
		return dbConv( conn.getKey(), tmp , dbctm ) ; // PRQA S 3050
	}
	
	return _fields[pos].getSqlString() ;

}



void dbSmartPersistent::unsetId()
{
	Lock guard(*this); // PRQA S 3050
	if(_idFieldPos != -1)
	{
        unset (_idFieldPos ) ; // PRQA S 3000
    }
}


_ACS_END_NAMESPACE
