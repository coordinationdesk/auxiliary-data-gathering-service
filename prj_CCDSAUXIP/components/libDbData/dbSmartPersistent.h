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



#ifndef _dbSmartPersistent_H_
#define _dbSmartPersistent_H_ 




#include <acs_c++config.hpp>
#include <dbPersistent.h>

#include <dbSchemaMetaDataProvider.h>
#include <dbQuery.h>
#include <SQLString.h>
#include <dbSet.h> // for exception
#include <dbRow.h> // for exception
#include <Transaction.h>
#include <dbColumn.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbQueryParameters ;
class dbRow ;

/**
 * Smart class to manage generic dbPersisten tables
 * 
 */

class dbSmartPersistent : public dbPersistent // PRQA S 2109
{
    friend bool operator==(const dbSmartPersistent &, const dbSmartPersistent &) ; // PRQA S 2107

public:
	//*********************** Exceptions  ****************************** 

	/*! class ExecutionError declaration */
	exDECLARE_EXCEPTION(ExecutionError,exDbPersistent) ; // PRQA S 2131, 2153, 2502

	/*! class NullValueException declaration */
	exDECLARE_EXCEPTION(NullValueException,exDbPersistent) ; // PRQA S 2131, 2153, 2502

	/*! class ErrorInNumericConversion declaration */
	exDECLARE_EXCEPTION(ErrorInNumericConversion,dbException) ; // PRQA S 2131, 2153, 2502

	//*********************** Contructors / Destructors  ****************************** 

	dbSmartPersistent(std::string const & tableName, dbConnection &);				
	dbSmartPersistent(std::string const & tableName, std::map< std::string, dbSpecificColumnType::Type> const & types, dbConnection &);				
	virtual ~dbSmartPersistent() noexcept ;	
	
	dbSmartPersistent(const dbSmartPersistent&) ; 
	dbSmartPersistent &operator=(const dbSmartPersistent &) ;
	dbSmartPersistent & copy(const dbSmartPersistent & , bool copyDbPersistent=true ) ; // PRQA S 2502
    // do not declare any move ctpr or assignment op

	//*********************** From dbPersistent ****************************** 
	virtual dbPersistent * clone() const ; // PRQA S 2502
	
	//* Returns the name of the class
	virtual std::string className() const ;
	const std::string &getKey() const noexcept { return _key ; }

	//* initialize the class
	virtual void reset() ; 
	
	//* returns the query to save (INSERT) - notifies in the given boolean (if any) the presence of BLOBs/CLOBs
	SQLString saveQuery(bool &blobFieldsPresent,dbConnection &conn) const ;
	
	//* save the data in database
	virtual IdType save( dbConnection &)  ;
	

	//* returns the query to load (SELECT) 
	SQLString loadQuery(dbConnection &conn, IdType id=0) const ;

	/* Helper static operation to deal with CLOB/BLOB types replacement when executing queries*/
	static void setLOBFreeSelectList (const std::string& tableName, SQLString& queryString, dbConnection &conn, \
		const std::set <size_t> idxCastClobAsString_S=std::set <size_t>()) ;
	static void getLOBFreeColumnTypes (std::vector < dbSpecificColumnType::Type > & types, 
		const std::set <size_t> idxCastClobAsString_S=std::set <size_t>()) ;
	
	bool canBeLoaded(IdType id, dbConnection &) const ;
    /** 
	 * load the class from the database
	 * for some classes is meaningful to provide object id
	 */
	virtual bool load(dbConnection & , IdType id=0)   ;

    template<typename T>
    bool loadByUniqueKey(dbConnection &conn, size_t pos, T const & value) 
    {
        const std::string &dbKey = conn.getKey() ;
        
 	    if(pos >= fieldsNumber())
	    {
		    std::ostringstream os;
		    os << _tableName << ": field # " << pos << " is out of range. This record has " << fieldsNumber() << " fields.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	    }
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(dbKey) ;
        if(not DBSMP ->isUnique(_tableName , pos, conn) ) { 
		    std::ostringstream os;
		    os << _tableName << "/ field #" << pos << " is not unique.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
        }

	    Lock lock(*this) ; // PRQA S 3050

	    dbPersistent *myCopy(0) ;
        try {

            std::ostringstream os ; 
            os << _tableName << ":#" << pos << ": " << value ;
            std::string dbgId = os.str() ;
		    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId) ;

            SQLString queryString(dbKey, SQLString::SELECT, true) ; //read only
            
            queryString.addTableList( _tableName ) ;

	        dbSmartPersistent::setLOBFreeSelectList (_tableName, queryString, conn, _idxCastClobAsString_S);

            dbQueryParameters parameters(dbKey) ;

            parameters.addParameter(dbParam(dbKey,
                                    _tableName ,
				                    DBSMP -> getFieldName (_tableName , pos, conn) ,
									dbParam::EQUAL_TO,
									dbConv(dbKey, value ))) ;

            queryString.addANDWhereParameter(parameters) ;

		    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId << " SQL = \"" << queryString.getSQL() << "\".") ;

		    //prepares types vector
		    std::vector<dbSpecificColumnType::Type > types ;

            size_t fs = fieldsNumber() ;
		    for( size_t i=0; i<fs ; ++i) 
		    { 
			    types.push_back(_fields[i].getColumnType() ) ;	
		    }

		    // Replace types with a LOB Free version
		    dbSmartPersistent::getLOBFreeColumnTypes (types, _idxCastClobAsString_S);

		    ACS_CLASS_BGN_DEBUG(50)
			    std::ostringstream dbg ;
                dbg << SimpleDebugSignatureThread << dbgId << ": Types are:\n" ;
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
                ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ; // should never occur
                set.getNextRow(row) ; // first and only 
   		    }
		    catch (dbQuery::QueryNoDataFound &e) {
			    return false ;
		    }
		    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId << ": Query successfully executed.") ;

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
		    catch(...) {
			    if(myCopy) { *this = *dynamic_cast<dbSmartPersistent*>(myCopy) ; } // PRQA S 3081
			    throw ;
		    }

		    setFromDatabase() ;
		    resetAllFielsdModifiedFlag() ;

		    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, dbgId << ": Dump: \"\n" << *this ) ;
	    }
	    catch(std::exception &e)
	    {
		    delete myCopy ;
		    ACS_THROW( ExecutionError(e , "Error while loading record " + getUserIdentifier(conn) ) ) ;// PRQA S 3081
	    }

	    delete myCopy ;
	    return true ;
    }
    
	template<typename T>
	bool loadByUniqueKey(dbConnection &conn, std::string const &fieldName, T const & value)
    {
	    return loadByUniqueKey( conn, dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ), value) ;
    }

	//* returns the query to update (UPDATE) 
	SQLString updateQuery(std::vector<size_t> &, dbConnection &conn) const ;
	
	//* update an existing instance in database
	virtual IdType update(dbConnection &)   ;
	virtual IdType update(dbConnection &, const dbQueryParameters& customWhereParameters, bool& updated);

	template<typename T>
	bool updateByUniqueKey(dbConnection &conn, size_t pos, T const & value, const dbQueryParameters &customWhereParameters)
    {
 	    if(pos >= fieldsNumber())
	    {
		    std::ostringstream os;
		    os << _tableName << ": field # " << pos << " is out of range. This record has " << fieldsNumber() << " fields.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	    }
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
        if(not DBSMP ->isUnique(_tableName , pos, conn) ) { 
		    std::ostringstream os;
		    os << _tableName << "/ field #" << pos << " is not unique.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
        }

        std::ostringstream os ; 
        os << _tableName << ":#" << pos << ": " << value ;
        std::string dbgId = os.str() ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId) ;

	    Lock guard(*this); // PRQA S 3050

	    try{

		    std::vector<size_t> updatedFields ;
		    SQLString queryString = updateQuery(updatedFields, conn) ;

		    // here we assume that if the updateQuery has no field to set the query does not contain either the table
		    bool mustOpentransaction = !_filePaths.empty() ;

		    dbQueryParameters parameters(conn.getKey()) ;
		    
            parameters.addParameter(dbParam(conn.getKey(),
                                    _tableName ,
				                    DBSMP -> getFieldName (_tableName , pos, conn) ,
									dbParam::EQUAL_TO,
									dbConv(conn.getKey(), value ))) ;

            queryString.addANDWhereParameter(parameters) ;
			queryString.addANDWhereParameter(customWhereParameters) ; // Add custom parameters 

            db::Transaction transaction(conn) ;
		    if( mustOpentransaction) { transaction.begin_work("SPUPD " + _tableName) ; }

		    if(transaction.inTransaction()) { conn.addStateWrapper(*this) ; }

            bool updateSuccesful = true ;
		    if( ! updatedFields.empty() )
		    {
			    dbQuery query(conn, queryString, "SPUPD " + _tableName) ;
			    query.execForStatus() ;
                updateSuccesful = query.getAffectedRows() > 0 ; // uniqueness should guarantee only one
                ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId << " updating std fields ") ;
		    }
            
		    // Upload all the files in clob/blob
		    typedef std::map<size_t, std::string>::iterator mapIter ;
		    bool uploadSuccesful = true ;
            for(mapIter iter=_filePaths.begin() ; iter != _filePaths.end() ; ++ iter ) // PRQA S 4238
		    {
			    uploadSuccesful = uploadFile (iter->first, iter->second, conn, parameters) && uploadSuccesful ;
                ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId << " uploading pos #" << iter->first) ;
		    }
            
		    transaction.commit() ;

		    // note the file paths can be erased else they will be used again and can be no longer valid
		    _filePaths.clear() ;
		    resetAllFielsdModifiedFlag() ;


		    return updateSuccesful && uploadSuccesful ;
	    } //try
	    catch(std::exception &e)
	    {
		    ACS_THROW( ExecutionError(e ,  "Error while updating record " + getUserIdentifier(conn) ) ) ;// PRQA S 3081
	    }
    }
    
	template<typename T>
    bool updateByUniqueKey(dbConnection &conn, size_t pos, T const & value)
	{
		return updateByUniqueKey(conn, pos, value, dbQueryParameters(conn.getKey()));
	}

	template<typename T>
	bool updateByUniqueKey(dbConnection &conn, std::string const &fieldName, T const & value, const dbQueryParameters &customWhereParameters)
    {
	    return updateByUniqueKey( conn, dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ), value, customWhereParameters) ;
    }
	
	template<typename T>
	bool updateByUniqueKey(dbConnection &conn, std::string const &fieldName, T const & value)
    {
	    return updateByUniqueKey( conn, dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ), value) ;
    }
	
	//* returns the query to remove (DELETE) 
	SQLString removeQuery(dbConnection &) const ;
	
	/** 
	 * remove the class from the database
	 * for some classes is meaningful to provide object id
	 */
	virtual IdType remove(dbConnection &) ; // PRQA S 2502

	template<typename T>
    bool removeByUniqueKey(dbConnection &conn, size_t pos, T const & value) 
    {
 	    if(pos >= fieldsNumber())
	    {
		    std::ostringstream os;
		    os << _tableName << ": field # " << pos << " is out of range. This record has " << fieldsNumber() << " fields.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	    }
        dbSchemaMetaDataProvider * DBSMP = dbSchemaMetaDataProvider::instance(conn.getKey()) ;
        if(not DBSMP ->isUnique(_tableName , pos, conn) ) { 
		    std::ostringstream os;
		    os << _tableName << "/ field #" << pos << " is not unique.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
        }

        std::ostringstream os ; 
        os << _tableName << ":#" << pos << ": " << value ;
        std::string dbgId = os.str() ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, dbgId) ;

	    Lock lock(*this) ; // PRQA S 3050

        dbQueryParameters parameters(conn.getKey()) ;

        parameters.addParameter(dbParam(conn.getKey(),
                                _tableName ,
				                DBSMP -> getFieldName (_tableName , pos, conn) ,
								dbParam::EQUAL_TO,
								dbConv(conn.getKey(), value ))) ;


        SQLString queryString (conn.getKey(), SQLString::DELETE ) ;
	    queryString.addTableList(_tableName) ;
	    queryString.addANDWhereParameter(parameters) ;


	    dbQuery query(conn, queryString , "SPDEL " + _tableName) ;

	    query.execForStatus() ;

        if (query.getAffectedRows() > 0) { // uniqueness - can't be > 1
	        resetFromDatabase() ;
	        setAllFieldModifiedFlags() ;
	        return true ;
        }
        else {
            return false ;
        }
    }

	template<typename T>
	bool removeByUniqueKey(dbConnection &conn, std::string const &fieldName, T const & value)
    {
	    return removeByUniqueKey( conn, dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldFromName( _tableName , fieldName, conn ), value) ;
    }

	
	/** 
	 *	True if the class have valid parameters, false otherwise
	 *  Default implementation always returns TRUE
	 */
	virtual bool validate() const ; 
	
	virtual void writeTo(std::ostream &) const   ;
	virtual void writeTo(exostream &) const   ;

	/** 
	 * Set the unique class id, if any
	 * the id field is asked to the dbSchemaMetaDataProvider
	 */
	virtual void setId(IdType id) ;
	
	/** 
	 * Get the unique class id, if any
	 * the id field is asked to the dbSchemaMetaDataProvider
	 * If the table has not an id this methos returns 0
	 */
	virtual IdType getId() const ; 

	virtual void exportObj(rsResourceSet &) const ;
	virtual void importObj(rsResourceSet &) ;
	virtual void exportXML(XMLOstream &) const ;


	//*********************** Own Methods ****************************** 
	

	/** 
	 * Unset the unique class id, if any
	 * the id field is asked to the dbSchemaMetaDataProvider
	 */
	virtual void unsetId() ;


	
	virtual std::string getTableName() const;
	
	virtual size_t fieldsNumber() const ;
	
	virtual bool fieldIsNull(const size_t& i) const ;

	
	/**
	 * When true a copy of the object is saved before the loadRow phase
	 * it is saved in a temporary object or as a memento in the connection if within a transaction
	 */
	virtual void saveCopyDuringLoad(bool) ;
	virtual bool saveCopyDuringLoad() const ;
	
	/**
	 * Returns a vector with all the field names. 
	 */
	virtual std::vector<std::string> getFieldNames(dbConnection &) const ;
	

	/**
	 * Returns the values of fields. 
	 */
	template<typename T>
	void get(size_t pos, T & value, bool &isNull, dbConnection &conn ) const // PRQA S 4214
    {
	    Lock lock(*this) ; // PRQA S 3050

	    if(pos >= fieldsNumber())
	    {
		    std::ostringstream os;
		    os << getUserIdentifier(conn) << ": error in get: " << pos << " is out of range. This record has " << fieldsNumber() << " fields.\n";
		    ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	    }

	    try{
		    _fields[pos].column(value) ;
		    isNull = false ;
	    }
	    catch(dbRow::NullValueException &env)
	    {
		    isNull = true ; 
	    }
	    catch(dbColumn::NullColumnException &env)
	    {
		    isNull = true ;
        }
	    catch(std::exception &e)
	    {
		    std::ostringstream	os;
		    os << getUserIdentifier(conn) << ": error in get for field n  "<< pos << " \"" << acs::dbSchemaMetaDataProvider::instance(_key) -> getFieldName( _tableName, pos, conn ) << "\"" ;
		    ACS_THROW(ExecutionError (e , os.str() ) ); // PRQA S 3081
	    }

    } 
	/**
	 * Set the values of fields.
	 * Note that it is forbidden to modify a primary key
	 * if record is from database
	 * This method also performs a size check in case of strings 
	 */
	template<typename T>
	void set(size_t pos, T const & value, dbConnection &conn) 
    {
	    Lock lock(*this) ; // PRQA S 3050

	    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) <<": "
		        << "set[" << pos << "] value: \"" << value <<  "\"") ;
	    if(pos >= fieldsNumber())
	    {
		    std::ostringstream	os;
		    os << getUserIdentifier(conn) << ": error in set: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.\n";
		    InvalidUse e(os.str() ) ;
		    ACS_THROW(e) ;
	    }
	    if(dbSchemaMetaDataProvider::instance(_key) ->isPrimaryKey(_tableName , pos, conn) ) {
		    if (isFromDatabase())
		    {
			    std::string errorMesage("Error: It is not possible to set primary key \"") ;
			    errorMesage += dbSchemaMetaDataProvider::instance(_key) -> getFieldLabel( _tableName , pos, conn ) ;
			    errorMesage += "\" in because record has been already been stored in table \"" + _tableName + "\"." ;
			    ACS_THROW(InvalidUse(errorMesage)) ; // PRQA S 3081
		    }
        }

	    try{	
		    _fields[pos].setColumn( value );
		    _fieldModified[pos] = true ;
		    setModifiedFlag() ;
	    }
	    catch(std::exception &e)
	    {
		    std::ostringstream	os;
		    os << getUserIdentifier(conn) << ": error in set for field id  "<< pos ;
		    ACS_THROW(ExecutionError (e , os.str() ) );
	    }

    } 

	/**
	 * Overloaded method. It obtains field position from schema metadata provider.
	 */
	template<typename T>
	void get(std::string const &fieldName, T & value, bool &isNull, dbConnection &conn) const  // PRQA S 4214
    {
	    get( dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName( _tableName , fieldName, conn ), value, isNull, conn ) ;
    }

	/**
	 * Overloaded method. It obtains field position from schema metadata provider.
	 */
	template<typename T>
	void set(std::string const &fieldName, T const & value, dbConnection &conn ) 
    {
	    set( dbSchemaMetaDataProvider::instance(_key) -> getFieldFromName( _tableName , fieldName, conn ) , value, conn ) ;
    }

	/**
	 * \return A new copy of the stored geo object
	 *
	 * \note the pointer is NOT previously deleted.
	 * If the value contained inside the column is not a geo object an exeption is raised.
	 */
	virtual void get(size_t pos, dbGeoObject * & value, dbConnection & ) const ;

	/**
	 * \return A new copy of the stored geo object
	 *
	 * \note the pointer is NOT previously deleted.
	 * If the value contained inside the column is not a geo object an exeption is raised.
	 */
	virtual void get(std::string const &fieldName,dbGeoObject * & value, dbConnection & ) const ;

	
    virtual void unset(size_t pos) ;
	virtual void unset(size_t pos, dbConnection &) ;
    virtual void unset(std::string const &fieldName) ;
	virtual void unset(std::string const &fieldName, dbConnection &) ;
    virtual bool isSet(size_t pos) const ;
	virtual bool isSet(size_t pos, dbConnection &) const ;
    virtual bool isSet(std::string const &fieldName) const ;
	virtual bool isSet(std::string const &fieldName, dbConnection &) const ;
    virtual bool isFieldModified(size_t pos) const ;
	virtual bool isFieldModified(size_t pos, dbConnection &) const ;
    virtual bool isFieldModified(std::string const &fieldName) const ;
	virtual bool isFieldModified(std::string const &fieldName, dbConnection &) const ;
	
	/**
	 * Set file path for upload/download operations for field at position pos
	 * An exception is raised if the field is not Blob or Clob
	 * After the path is set the method setModifiedFlag() is called
	 */
	virtual void setFilePath(size_t pos, std::string const & filePath, dbConnection &) ;


	/**
	 * Set file path for upload/download operations for field with specified name
	 * An exception is raised if the field is not Blob or Clob
	 */
	virtual void setFilePath(std::string const &fieldName  , std::string const & filePath, dbConnection &) ;
	
	
	virtual void unsetFilePath(size_t pos) ;
	virtual void unsetFilePath(std::string const &fieldName, dbConnection &) ;
	
	/**
	 * Returns the path specified for field at position pos.
	 * If the path has not been specified returns an empty string
	 */
	virtual std::string getFilePath(size_t pos) const ;
	
	/**
	 * Returns the path specified for field with specified name.
	 * If the path has not been specified returns an empty string
	 */
	virtual std::string getFilePath(std::string const &fieldName, dbConnection &) const ;


	virtual void downloadFile(size_t pos , std::string const & filePath , dbConnection & ) const ;

	virtual void downloadFile(std::string const &fieldName , std::string const & filePath , dbConnection & ) const ;

	virtual int loadRow(const dbRow &, int offset=0) ;

	

	
	//* Default impementation returns an empty string.
	virtual std::string getOrderBy() const;


	/*
	 * This methos should return a string that identifies the record.
	 * It can be used in all message to the user (exEceptions, debug, ecc)
	 * It does not throw exceptions.
	 * It is possible to specify, as an optional parameter, a pointer to a smart persisten object
	 * If this pointer is not null, the method returns the identifier of the object pointed. This feature is 
	 * used in the load method where the current object is saved into a copy. 
	 */
	virtual std::string getUserIdentifier() const noexcept ;
	virtual std::string getUserIdentifier(dbConnection &) const noexcept ;


	template<typename InputType , typename RequestedType>
	static RequestedType convertToDifferentType(InputType &i, RequestedType& v) 
    {
	    typedef boost::numeric::converter<    RequestedType,
				                              InputType,
										      boost::numeric::conversion_traits<RequestedType , InputType> ,
										      overflowHandler
									     > inputType2RequestedType ;
	    try{
		    v=inputType2RequestedType::convert( i );
	    }
	    catch(std::exception &x)
	    {
		    std::ostringstream errorMessage;
		    errorMessage << "Overflow error: Cannot convert " << i << "  into the requested type: " << dbColumnTypeHelper::getLanguageTypeName(v)  ;
		    ACS_THROW(ErrorInNumericConversion(x,errorMessage.str())) ;
	    }

	    return v ;
    }

	/**
	 * Prepare WHERE parameters used to identify the correct
	 * record for save and load operations.
	 * In the default implementation, the condition is obtained  
	 * applying constraints on all the primary keys used in AND.
	 * If a pk has not been set (provided that the field is NOT NULL) the NULL 
	 * value should be matched.
	 * If the table has an Id (numerical pk) field than it can be specified from ouside:
	 * If the value specified is 0 than the method takes the Id stored into the object.
	 * and throw an exception in case its value too is 0.
	 * \see dbSmartPersistent::load(dbConnection &conn = 0 , IdType)
	 * \throw IdNotProvided No Id specified for an object with an Id field
	 */
	virtual void getWhereParameters(dbQueryParameters &parameters, dbConnection &, IdType requestedId=0) const ;

	virtual void resetAllFielsdModifiedFlag() ;
	virtual void setAllFieldModifiedFlags() ;
	virtual void setSingleFieldModifiedFlag(size_t pos, dbConnection &) ;

	void castClobAsString(const std::string& column_name, dbConnection & conn, const bool to_cast=true) ;
	bool checkCastClobAsString(const size_t& idx) const { return checkCastClobAsString(idx, _idxCastClobAsString_S) ; }
	bool checkCastClobAsString(const std::string& column_name, dbConnection & conn) const \
		{ return checkCastClobAsString(_tableName, column_name, conn, _idxCastClobAsString_S) ; }
	static bool checkCastClobAsString(const size_t& idx, const std::set <size_t>& idxCastClobAsString_S) ;
	static bool checkCastClobAsString(const std::string& table_name, const std::string& column_name, dbConnection & conn, \
										const std::set <size_t>& idxCastClobAsString_S) ; 
	void disableNullCheck (bool) noexcept ;
	bool disableNullCheck () const noexcept ;
	
protected:
	/**
	 * Throw exception if one or more NOT NULL fields are NULL
	 * an exception is the case of serial fields. they are not NULL
	 * but usually they are not specified by user but by database itself.
	 * In theory a similar check should be done for all the fields
	 * that accept auto values.
	 */ 
	virtual void checkNotNullFields(dbConnection &) ;
	
	virtual std::string getFieldSqlString(size_t pos, dbConnection &) const ;

	IdType discoverId(IdType id, dbConnection &) const ;

	virtual bool uploadFile (size_t fieldPosition , std::string const & filePath , dbConnection & ) const ;
	virtual bool uploadFile (std::string const &fieldName , std::string const & filePath , dbConnection & ) const ;

private:
    bool uploadFile (size_t fieldPosition , std::string const & filePath , dbConnection &, dbQueryParameters const & ) const ;
    bool uploadFile (std::string const &fieldName , std::string const & filePath , dbConnection &, dbQueryParameters const & ) const ;

	dbSmartPersistent() ; // declared but not implemented
  	struct overflowHandler
	{
		void operator() ( boost::numeric::range_check_result r )  // PRQA S 4211
        {
	        if ( (r == boost::numeric::cNegOverflow) ||  (r == boost::numeric::cPosOverflow))
	        {
		        ACS_THROW(ErrorInNumericConversion("Overflow error.")) ; // PRQA S 3081
	        }
        }
	} ;

    void savePreconditionCheck( dbConnection &) ;
private:
	std::string _tableName ;
	std::vector<dbColumn> _fields ;
	std::vector<bool> _fieldModified ;
	std::map<size_t, std::string> _filePaths ;
	bool        _saveCopyDuringLoad ;
    long        _idFieldPos ;
    std::string _key ;
	bool _disableNullCheck;
	std::set <size_t> _idxCastClobAsString_S ; 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSmartPersistent) ;


};

	
bool operator==(const dbSmartPersistent &, const dbSmartPersistent &) ;
bool operator!=(const dbSmartPersistent &, const dbSmartPersistent &) ;



//===================  Template methods definitions ====================================

template<> 
inline
void dbSmartPersistent::get<dbColumn>(size_t pos, dbColumn & value, bool &isNull, dbConnection &conn) const // PRQA S 2133, 2134, 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn)) ;
	Lock lock(*this) ; // PRQA S 3050 

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in get: " << pos << " is out of range. This record has  " << fieldsNumber() << " fields.\n";
		ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	}
	
	try{
		value=_fields[pos] ;
		isNull = value.isNull() ; 
	}
	catch(std::exception &e)
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in get for field #"<< pos 
           << " \"" << acs::dbSchemaMetaDataProvider::instance(_key) -> getFieldName( _tableName, pos, conn ) << "\"" ; // PRQA S 3010
		ACS_THROW(ExecutionError (e , os.str() ) );
	}
} 

template<> 
inline
void dbSmartPersistent::set<std::string>(size_t pos, std::string const & value, dbConnection &conn) // PRQA S 2133, 2134
{
	Lock lock(*this) ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn)) ;

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in set: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.\n";
        ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	}
	if(dbSchemaMetaDataProvider::instance(_key) ->isPrimaryKey(_tableName , pos, conn) ) { // PRQA S 3010
		if (isFromDatabase())
		{
			std::string errorMessage("Error: It is not possible to set primary key \"") ;
			errorMessage += dbSchemaMetaDataProvider::instance(_key) -> getFieldLabel( _tableName , pos, conn ) ; // PRQA S 3010
			errorMessage += "\" in because record has been already been stored in table \"" + _tableName + "\"." ;
			ACS_THROW(InvalidUse(errorMessage)) ; // PRQA S 3081
		}
    }
    std::string propType = StringUtils::lowercase(dbSchemaMetaDataProvider::instance(_key) -> getFieldProprietaryType(_tableName , pos, conn)) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getUserIdentifier(conn) <<": "
		       << "propType: \"" << propType << "\"") ;
	
    if ((propType != "text") && (value.size() > dbSchemaMetaDataProvider::instance(_key) -> getFieldSize(_tableName , pos, conn) ) ) // PRQA S 3010
	{
		std::ostringstream errorMessage;
		errorMessage << "String \"" ;
		errorMessage << value << "\" is too long for field \"" << dbSchemaMetaDataProvider::instance(_key) -> getFieldLabel( _tableName , pos, conn ) ; // PRQA S 3010
		errorMessage << "\" of table \""  << _tableName << "\". Max size is " <<dbSchemaMetaDataProvider::instance(_key) -> getFieldSize(_tableName , pos, conn) ; // PRQA S 3010
        ACS_THROW(InvalidUse(errorMessage.str() )) ; // PRQA S 3081
	}


	try{
		_fields[pos].setColumn( value );
		_fieldModified[pos] = true ;
		setModifiedFlag() ;
	}
	catch(std::exception &e)
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in set for field id  "<< pos ;
		ACS_THROW(ExecutionError (e , os.str() ) );
	}

} 


template<> 
inline
void dbSmartPersistent::set<dbColumn>(size_t pos, dbColumn const & value, dbConnection &conn) // PRQA S 2133, 2134
{
	Lock lock(*this) ; // PRQA S 3050 

	if(pos >= fieldsNumber())
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in set: "<< pos << " is out of range. This record has  " << fieldsNumber() << " fields.\n";
		InvalidUse e(os.str() ) ;
		ACS_THROW(e) ;
	}

	if(dbSchemaMetaDataProvider::instance(_key) ->isPrimaryKey(_tableName , pos, conn) ) {  // PRQA S 3010
		if (isFromDatabase())
		{
			std::string errorMessage("Error: It is not possible to set primary key \"") ;
			errorMessage += dbSchemaMetaDataProvider::instance(_key) -> getFieldLabel( _tableName , pos, conn  ) ; // PRQA S 3010
			errorMessage += "\" in because record has been already been stored in table \"" + _tableName + "\"." ;
			ACS_THROW(InvalidUse(errorMessage)) ; // PRQA S 3081
		}
    }

	if ( value.getColumnType() != _fields[pos].getColumnType() )
	{
		std::ostringstream	os;
		os << getUserIdentifier(conn) << ": error in set: field "<< pos << " is of type " << dbColumnTypeHelper::getTypeName( _fields[pos].getColumnType() )
		   << " while input value is of type "<< dbColumnTypeHelper::getTypeName(value.getColumnType()) ;
		ACS_THROW(InvalidUse(os.str() )) ; // PRQA S 3081
	}

	if( value.getColumnType() == dbSpecificColumnType::STRING)
	{
		std::string stringValue;
		value.column(stringValue) ;
		if (stringValue.size() > dbSchemaMetaDataProvider::instance(_key) -> getFieldSize(_tableName , pos, conn)  ) // PRQA S 3010
		{
			std::ostringstream errorMessage;
			errorMessage << "String \"" ;
			errorMessage << stringValue << "\" is too long for field \"" << dbSchemaMetaDataProvider::instance(_key) -> getFieldLabel( _tableName , pos, conn ) ; // PRQA S 3010
			errorMessage << "\" of table \""  << _tableName << "\". Max size is " <<dbSchemaMetaDataProvider::instance(_key) -> getFieldSize(_tableName , pos, conn) ; // PRQA S 3010
			ACS_THROW(InvalidUse(errorMessage.str() )) ; // PRQA S 3081
		}
	}

	_fields[pos] = value ;
	_fieldModified[pos] = true ;
	setModifiedFlag() ;

} 


_ACS_END_NAMESPACE


#endif // _dbSmartPersistent_H_
