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
	Revision 5.5  2016/04/11 13:21:51  marpas
	removing useless methods
	
	Revision 5.4  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.3  2014/02/11 11:10:04  marpas
	refresh method removed: base class implements it
	
	Revision 5.2  2014/02/07 18:39:00  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2014/01/30 18:03:58  marpas
	interface rationalized
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.22  2013/05/14 19:00:07  marpas
	dbSchemaMetadataProvider interface now requires db connection key
	
	Revision 2.21  2013/04/30 13:09:35  marpas
	dbPersistent interface changed
	
	Revision 2.20  2013/04/17 17:36:33  marpas
	dbPersistent interface changed
	
	Revision 2.19  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.18  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.17  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.16  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.14  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.13  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.12  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.11  2011/03/02 18:16:21  marpas
	serial 8 supported on load too
	
	Revision 2.10  2011/03/02 17:39:39  marpas
	support to serial 8 improved
	
	Revision 2.9  2011/03/02 17:29:14  marpas
	support of serial 8 implemented
	
	Revision 2.8  2009/06/19 18:36:29  marpas
	still working on dbConnection speedup
	
	Revision 2.7  2009/06/19 18:11:35  marpas
	dbConnection used to speed up work and to avoid issuing multiple connections and queries when it is useless
	
	Revision 2.6  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.5  2008/05/26 19:56:18  paoscu
	isNull parameter added to get methods
	
	Revision 2.4  2008/05/08 21:38:45  paoscu
	Added options to enable/disable backup of record during the load operation
	Improved debug prints and thread safety
	
	Revision 2.3  2008/04/28 21:48:57  paoscu
	Added the methods: getSelectSQLString and  getColumnTypes
	
	Revision 2.2  2008/04/23 19:01:35  paoscu
	Added related tables
	
	Revision 2.1  2008/04/23 16:13:23  paoscu
	dbSmartAggregatePersistent added.
	
	
	
		

*/



#ifndef _dbSmartAggregatePersistent_H_
#define _dbSmartAggregatePersistent_H_ 


#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>

#include <vector>
#include <memory>


_ACS_BEGIN_NAMESPACE(acs)



class dbSmartPersistent ;
class dbRow ;
class dbQueryParameters ;

class dbSmartAggregatePersistent : public dbPersistent // PRQA S 2109
{

friend bool operator==(const dbSmartAggregatePersistent &, const dbSmartAggregatePersistent &) ; // PRQA S 2107
	
public:

	/*! class ExecutionError declaration */
	exDECLARE_EXCEPTION(ExecutionError,exDbPersistent) ; // PRQA S 2131, 2153, 2502



	//*********************** Contructors / Destructors  ****************************** 

	dbSmartAggregatePersistent(std::string const & parentTable , std::vector<std::string> const & childTables, dbConnection &);				
	
	
	virtual ~dbSmartAggregatePersistent() noexcept ;	
	
	dbSmartAggregatePersistent(const dbSmartAggregatePersistent&) ; 
	dbSmartAggregatePersistent &operator=(const dbSmartAggregatePersistent &) ;

	dbSmartAggregatePersistent & copy(const dbSmartAggregatePersistent & , bool copyDbPersistent=true ) ; // PRQA S 2502
	bool initialized() const ;
	
	void checkInitialized() const ;

	//*********************** From dbPersistent ****************************** 
	virtual dbPersistent * clone() const ; // PRQA S 2502
	
	//* Returns the name of the class
	virtual std::string className() const ;
	
    //* initialize the class
	virtual void reset() ; 
	
	//* save the data in database
	virtual IdType save( dbConnection &)  ;
	
	/** 
	 * load the class from the database
	 * for some classes is meaningful to provide object id
	 */
	 	
	virtual bool load(dbConnection & , IdType id=0)   ;
	
	
	//* update an existing instance in database
	virtual IdType update(dbConnection &)   ;
	
	/** 
	 * remove the class from the database
	 * for some classes is meaningful to provide object id
	 */
	virtual IdType remove(dbConnection &) ; // PRQA S 2502


	
	/** 
	 *	True if the class have valid parameters, false otherwise
	 *  Default implementation always returns TRUE
	 */
	virtual bool validate() const ; 

	// Following method has been redefined because the modified flag is the combination
	// of parent and child ones.
	virtual bool isModified() const ;

	// Following method has been redefined because the flag to check is the one inside _parent
	virtual bool isFromDatabase() const ;
	
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
	 * If the table has not an id this methos always returns 0
	 */
	virtual IdType getId() const ; 

	virtual void exportObj(rsResourceSet &) const ;
	virtual void importObj(rsResourceSet &) ;
	virtual void exportXML(XMLOstream &) const ;



	//*********************** Own Methods ****************************** 
	
	
	std::string getParentTableName() const;
	
	//std::string getChildTableName() const;

 	size_t fieldsNumber(dbConnection &conn) const ;
 	size_t parentFieldsNumber() const ;
 	size_t childFieldsNumber(std::string const & childTable, dbConnection &conn) const ;


	/**
	 * When true a copy of the object is saved before the loadRow phase
	 * it is saved in a temporary object or as a memento in the connection if within a transaction
	 */
	void saveCopyDuringLoad(bool) ;
	bool saveCopyDuringLoad() const ;


 	
 	std::vector<std::string> getFieldNames(dbConnection &) const ;


	void getSelectSQLString(SQLString &, dbConnection &) const ;

	std::vector<dbSpecificColumnType::Type > getColumnTypes(dbConnection &) const ;

	/** 
	 * Unset the unique class id, if any
	 * the id field is asked to the dbSchemaMetaDataProvider
	 */
	void unsetId() ;


	/** 
	 * Add a record to be saved together with the persistent
	 * it copy the pointers (shared object).
	 */
	void addRelatedRecord(dbSmartPersistent *) ;


	void clearRelatedRecords() ;

	//size_t relatedRecordsNumber() const ;
	//dbSmartPersistent getRelatedRecord

 	
	/**
	 * Returns the values of fields. 
	 */
    template<typename T>
    inline
    void getFromParent(size_t pos, T & value, bool &isNull,dbConnection &conn ) const
    {
	    ThreadSafe::Lock guard(*this);
	    _parent->get(pos,value,isNull, conn) ;
    }

	/**
	 * Overloaded method. 
	 */
    template<typename T>
    inline
    void getFromParent(std::string const &fieldName, T & value, bool &isNull,dbConnection &conn ) const
    {
	    ThreadSafe::Lock guard(*this);
	    _parent->get(fieldName,value,isNull, conn) ;
    }


	/**
	 * Returns the values of fields. 
	 */
    template<typename T>
    inline
    void getFromChild(std::string const & childTable, size_t pos, T & value, bool &isNull, dbConnection &conn) const
    {
	    ThreadSafe::Lock guard(*this);
	    //std::map<dbSmartPersistent*>::const_iterator it=_childs.find
	    checkChildExists(childTable, conn) ;
	    _childs.find(childTable)->second->get(pos,value,isNull, conn) ;
    }



	/**
	 * Overloaded method. 
	 */
    template<typename T>
    inline
    void getFromChild(std::string const & childTable, std::string const &fieldName, T & value, bool &isNull, dbConnection &conn) const
    {
	    ThreadSafe::Lock guard(*this);
	    checkChildExists(childTable, conn) ;
	    _childs.find(childTable)->second->get(fieldName,value,isNull, conn) ;
    }




	/**
	 * Overloaded method. 
	 * Select which is the table containig the given field (parent or child)
	 * Throws exception if it is not possible to tell.
	 */
    template<typename T>
    inline
    void get(std::string const &fieldName, T & value, bool &isNull, dbConnection &conn) const
    {
    // W8115_OFF
	    ThreadSafe::Lock guard(*this);
	    //Throws exception if the field name is ambiguous
	    checkField(fieldName, conn) ;

	    if ( isInParent(fieldName, conn) )
	    {
		    getFromParent(fieldName, value,isNull, conn) ;
		    return ;
	    }
	    else
	    {
		    for( std::map<std::string , dbSmartPersistent*>::const_iterator it=_childs.begin() ; it!=_childs.end() ; ++it)
		    {
			    if ( isInChild(it->first , fieldName, conn) )
			    {
				    getFromChild(it->first ,fieldName , value,isNull, conn) ;
				    return ;
			    }
		    }
	    }

	    std::string errorMessage("There is no field \"");
	    errorMessage+=fieldName + "\" in record " ;
	    errorMessage+=getUserIdentifier(conn) ;
	    ACS_THROW( ExecutionError( errorMessage ) ) ;
    }




	/**
	 * Set the values of fields.
	 * Note that it is forbidden to modify a primary key
	 * if record is from database
	 * This method also performs a size check in case of strings 
	 */
    template<typename T> 
    inline
    void setInParent(size_t pos, T const & value, dbConnection &conn)
    {
	    ThreadSafe::Lock guard(*this);
	    _parent->set(pos,value, conn) ;
	    if (isParentPK(pos, conn)) {
		    updateChildsFKs(conn) ;
        }
    }


	/**
	 * Set the values of fields.
	 * Note that it is forbidden to modify a primary key
	 * if record is from database
	 * This method also performs a size check in case of strings 
	 */
    template<typename T>
    inline
    void setInParent(std::string const &fieldName, T const & value, dbConnection &conn)
    {
	    ThreadSafe::Lock guard(*this);
	    _parent->set(fieldName,value, conn) ;
	    if (isParentPK(fieldName, conn)) {
		    updateChildsFKs(conn) ;
        }
    }




	/**
	 * Set the values of fields.
	 * Note that it is forbidden to modify a primary key
	 * if record is from database
	 * This method also performs a size check in case of strings 
	 */
    template<typename T> 
    inline
    void setInChild(std::string const & childTable, size_t pos, T const & value, dbConnection &conn)
    {
	    ThreadSafe::Lock guard(*this);
	    checkChildExists(childTable, conn) ;
	    _childs[childTable]->set(pos,value, conn) ;
    }


	/**
	 * Set the values of fields.
	 * Note that it is forbidden to modify a primary key
	 * if record is from database
	 * This method also performs a size check in case of strings 
	 */
    template<typename T>
    inline
    void setInChild(std::string const & childTable, std::string const &fieldName, T const & value, dbConnection &conn)
    {
	    ThreadSafe::Lock guard(*this);
	    checkChildExists(childTable, conn) ;
	    _childs[childTable]->set(fieldName,value, conn) ;
    }



	/**
	 * Overloaded method. It obtains field position from schema metadata provider.
	 * Select which is the table containig the given field (parent or child)
	 * Throws exception if it is not possible to tell.
	 */
    template<typename T>
    inline 
    void set(std::string const &fieldName, T const & value, dbConnection &conn)
    {
    // W8115_OFF
	    ThreadSafe::Lock guard(*this);
	    //Throws exception if the field name is ambiguous
	    checkField(fieldName,conn) ;

	    if ( isInParent(fieldName, conn) )
	    {
		    setInParent(fieldName , value, conn) ;
		    return ;
	    }
	    else
	    {
		    for( std::map<std::string , dbSmartPersistent*>::iterator it=_childs.begin() ; it!=_childs.end() ; ++it)
		    {
			    if ( isInChild(it->first , fieldName, conn) )
			    {
				    setInChild(it->first ,fieldName , value, conn) ;
				    return ;
			    }
		    }
	    }	

	    std::string errorMessage("There is no field \"");
	    errorMessage+=fieldName + "\" in record " ;
	    errorMessage+=getUserIdentifier(conn) ;
	    ACS_THROW( ExecutionError( errorMessage ) ) ;
    }



	
	void unsetFromParent(size_t pos, dbConnection &) ;
	void unsetFromParent(std::string const &fieldName, dbConnection &) ;
	void unsetFromChild(std::string const & childTable, size_t pos, dbConnection &) ;
	void unsetFromChild(std::string const & childTable, std::string const &fieldName, dbConnection &) ;
	void unset(std::string const &fieldName, dbConnection &) ;

	bool isSetInParent(size_t pos, dbConnection &) const ;
	bool isSetInChild(std::string const & childTable, size_t pos, dbConnection &) const ;
	bool isSetInParent(std::string const &fieldName, dbConnection &) const ;
	bool isSetInChild(std::string const & childTable, std::string const &fieldName, dbConnection &) const ;
	bool isSet(std::string const &fieldName, dbConnection &) const;


	
	
	/**
	 * Set file path for upload/download operations for field at position pos
	 * An exception is raised if the field is not Blob or Clob
	 */
	void setFilePathInParent(size_t pos  , std::string const & filePath, dbConnection &) ;


	/**
	 * Set file path for upload/download operations for field with specified name
	 * An exception is raised if the field is not Blob or Clob
	 */
	void setFilePathInParent(std::string const &fieldName  , std::string const & filePath, dbConnection &) ;


	
	
	void unsetFilePathFromParent(size_t pos) ;
	void unsetFilePathFromParent(std::string const &fieldName, dbConnection &conn) ;
    	
	/**
	 * Returns the path specified for field at position pos.
	 * If the path has not been specified returns an empty string
	 */
	std::string getFilePathFromParent(size_t pos) const ;


	/**
	 * Returns the path specified for field with specified name.
	 * If the path has not been specified returns an empty string
	 */
	std::string getFilePathFromParent(std::string const &fieldName, dbConnection &) const ;

	void downloadFileFromParent(size_t pos , std::string const & filePath , dbConnection & ) const ;
	void downloadFileFromParent(std::string const &fieldName , std::string const & filePath , dbConnection & ) const ;
 	
 	int loadRow(const dbRow &, int offset=0) ;

	/*
	 * This methos should return a string that identifies the record.
	 * It can be used in all message to the user (exEceptions, debug, ecc)
	 * It does not throw exceptions - instead the exception will be returned into the string.
	 */
	std::string getUserIdentifier() const noexcept ;
	std::string getUserIdentifier(dbConnection &) const noexcept ;

	
protected:
	dbSmartAggregatePersistent();				
	void init(std::string const & parentTable , std::vector<std::string> const & childTables, dbConnection &);
	/**
	 * Throw exception if one or more NOT NULL fields are NULL
	 * an exception is the case of serial fields. they are not NULL
	 * but usually they are not specified by user but by database itself.
	 * In theory a similar check should be done for all the fields
	 * that accept auto values.
	 */ 
	void checkNotNullFields() ;
	
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
	 * \see dbSmartPersistent::load(dbConnection *conn = 0 , unsigned long id=0)
	 * \throw IdNotProvided No Id specified for an object with an Id field
	 */
	void getWhereParameters(dbQueryParameters &parameters, IdType id=0) const ;

	



protected:
	bool isInParent(const std::string &, dbConnection &) const ;
	bool isInChild(std::string const & childTable , std::string const &, dbConnection &) const ;
	//bool isInParentAndChild(std::string) const ;

	bool isParentPK(const std::string &, dbConnection &) const ;
	bool isParentPK(size_t, dbConnection &) const ;

	void checkField(const std::string &, dbConnection &) const ;
	
	void updateChildsFKs(dbConnection &) ;
	void updateChildsFKs() ;



////// TODO //////////
	void checkChildExists(std::string const & childTable, dbConnection &) const ;



private:
	void destroy() noexcept ;




private:
    // PRQA S 2101 L1
	dbSmartPersistent *_parent ;
	std::map<std::string , dbSmartPersistent *> _childs ;
	std::map<std::string , std::vector< std::pair<size_t , size_t> > > _relationTable ;
	std::vector<dbSmartPersistent *> _relatedRecords ;
    // PRQA L:L1
    std::string _key ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSmartAggregatePersistent) ;

};

	
bool operator==(const dbSmartAggregatePersistent &, const dbSmartAggregatePersistent &) ;
bool operator!=(const dbSmartAggregatePersistent &, const dbSmartAggregatePersistent &) ;
	

_ACS_END_NAMESPACE


#endif // _dbSmartAggregatePersistent_H_
