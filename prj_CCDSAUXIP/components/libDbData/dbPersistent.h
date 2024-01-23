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
	Revision 5.7  2016/04/21 14:52:35  marpas
	last modification was only partially - completed
	qa warnings fixed
	
	Revision 5.6  2016/04/21 14:45:07  marpas
	convenience templates improved - they are used to import/export/xml and dump
	
	Revision 5.5  2016/04/21 11:47:12  marpas
	parent id provider concept evicted - as for related methods / members
	marking dbPersistent concept evicted - as for related methods / members
	
	Revision 5.4  2016/04/19 13:11:36  marpas
	fieldName method removed
	
	Revision 5.3  2016/04/15 14:37:42  marpas
	getting rid of unused exception definitions
	
	Revision 5.2  2016/04/11 13:19:54  marpas
	removing useless methods and also deprecated ones
	
	Revision 5.1  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.29  2013/06/03 12:40:27  marpas
	IdType is a long long instead of an unsigned long long
	
	Revision 2.28  2013/04/30 13:09:35  marpas
	dbPersistent interface changed
	
	Revision 2.27  2013/04/17 17:36:33  marpas
	dbPersistent interface changed
	
	Revision 2.26  2013/04/09 07:58:05  marpas
	dbPersistent::getLastSerial signature changed to support statistics
	
	Revision 2.25  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.24  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.23  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.22  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.21  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.18  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.17  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.16  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.15  2011/03/02 18:16:21  marpas
	serial 8 supported on load too
	
	Revision 2.14  2011/03/02 17:39:39  marpas
	support to serial 8 improved
	
	Revision 2.13  2011/03/02 17:29:14  marpas
	support of serial 8 implemented
	
	Revision 2.12  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.11  2009/01/29 11:26:18  marpas
	set/rest from database are now public
	
	Revision 2.10  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.9  2007/10/17 10:38:39  marpas
	getExportFileName i/F fixed again
	
	Revision 2.8  2007/10/17 10:33:54  marpas
	getExportFilename was not virtual: fixed
	
	Revision 2.7  2007/10/17 09:45:12  marpas
	objectExport and getExportFilename methods added
	
	Revision 2.6  2007/06/09 19:49:58  marpas
	getHtmlModifyDescription virtual method added - throws an exception
	
	Revision 2.5  2007/06/03 10:49:36  marpas
	import/export convenience template functions added
	
	Revision 2.4  2006/10/31 19:17:26  paoscu
	isModified() and isFromDatabase() are now virtual
	
	Revision 2.3  2006/10/16 10:46:32  marpas
	Drawable concept introduced in libGraphicAttributes, from which this library was already dependent
	dbPersistent is now a Drawable
	dbStObj Is a Drawable too because inheritance from dbPersistent and does not
	longer implements attributes related methods
	Factories dealing with Attributes are now moved to the more appropriate libGraphicAttributes
	
	Revision 2.2  2006/05/11 20:41:33  marpas
	constant strings changed from std::string to const char * to avoid std::string repository problem with shared objects
	
	Revision 2.1  2006/03/09 16:36:53  marpas
	new libDbClient.dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.35  2005/10/21 17:01:32  paoscu
	Operators == and != are now protected.
	
	Revision 1.34  2005/10/18 18:13:02  paoscu
	Implementation of alignement tags changed.
	
	Revision 1.33  2005/10/18 16:28:05  paoscu
	Added static methods to get alignement tags.
	
	Revision 1.32  2005/09/23 17:45:21  marpas
	showAllFieldsInList method added
	
	Revision 1.31  2005/09/06 16:45:42  paoscu
	dbColumn::ColumnType used instead of dbType.
	
	Revision 1.30  2005/08/11 16:55:34  paoscu
	Alignement enum and defaultAlignement method added.
	
	Revision 1.29  2005/06/17 14:14:25  marpas
	std namespace use fixed
	
	Revision 1.28  2004/09/03 13:25:19  marpas
	toggleMark method added
	
	Revision 1.27  2004/09/01 17:23:39  marpas
	objtype concept introduced
	
	Revision 1.26  2004/08/31 09:54:23  marpas
	unused enum removed
	
	Revision 1.25  2004/07/13 14:45:18  marpas
	exDbPersistent is an abstract class
	
	Revision 1.24  2004/07/07 10:27:01  marpas
	dbPersistent I/F changed
	
	Revision 1.23  2004/05/25 12:28:54  ivafam
	Attribute _marked and related method added
	
	Revision 1.22  2004/05/03 14:14:40  marpas
	dbPersistent::remove I/F changed
	
	Revision 1.21  2004/04/13 14:10:59  marpas
	ACE dependencies removed
	
	Revision 1.20  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
		

*/


#ifndef _dbPersistent_H_
#define _dbPersistent_H_ 

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <dbStateWrapper.h>
#include <dbException.h>
#include <dbColumnType.h>
#include <XMLOstream.h>

#include <dbConnectionPool.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;

class exostream ;
 
/**
 *
 * class dbPersistent
 *
 *	abstract interface class for database objects.
 * 	It declare the pure virtual methods:
 *
 * reset() 					: manage the initialization of the private members of the class
 * save(conn) 				: save the data in database
 * load(conn , id) 			: load the class from the database
 * update(conn) 			: update an existing instance in database
 * remove(conn) 			: remove the class from the database
 * validate()  				: True if the class have valid values, false otherwise
 * export(s) 				: export the class to stream
 * import(s)				: import the class from a stream
 *
 * setId(id) 				: set the unique class id
 * getId()					: get the unique class id
 *
 */


class dbPersistent :    // PRQA S 1703, 2109, 2153
	public dbStateWrapper
{
    friend bool operator==(const dbPersistent &, const dbPersistent &)  ; // PRQA S 2107

public:
	typedef long long IdType ;	

	//
	// dbPersistentMemento class
	//
	class dbPersistentMemento : public dbStateWrapper::dbMemento // PRQA S 2109, 2153
	{
	    friend class dbPersistent ; // PRQA S 2107
	public:
		dbPersistentMemento(bool modified, bool fromDatabase, IdType id) ;
		dbPersistentMemento() = delete ;
		dbPersistentMemento(dbPersistentMemento const &) = delete ; ;
		dbPersistentMemento& operator=(dbPersistentMemento const &)  = delete ;;
		dbPersistentMemento(dbPersistentMemento &&) = delete ; ;
		dbPersistentMemento& operator=(dbPersistentMemento &&)  = delete ;;
		virtual ~dbPersistentMemento() ;
  
    private:
		dbPersistentMemento& operator=(dbMemento const &) ; // not implemented

	private:
		IdType  _id ; 
		bool	_modified ;
		bool	_fromDatabase ;	
	};


		
	//	
	// Exceptions
	//

	/*! class exDbPersistent declaration */
	exDECLARE_EXCEPTION(exDbPersistent,dbException) ;   // PRQA S 2131, 2153, 2502

	/*! class NotImplemented declaration */
	exDECLARE_EXCEPTION(NotImplemented,exDbPersistent) ;   // PRQA S 2131, 2153, 2502

	/*! class InvalidUse declaration */
	exDECLARE_EXCEPTION(InvalidUse,exDbPersistent) ;   // PRQA S 2131, 2153, 2502

	//* 'save' and 'load' method sometimes cannot be called withoud providing a unique id
	/*! class IdNotProvided declaration */
	exDECLARE_EXCEPTION(IdNotProvided,exDbPersistent) ;   // PRQA S 2131, 2153, 2502


	//* 'update' method cannot be called prior to retrieve data in database
	/*! class ObjectNotLoaded declaration */
	exDECLARE_EXCEPTION(ObjectNotLoaded,exDbPersistent) ;   // PRQA S 2131, 2153, 2502
	
	/*! class UndefinedDbParentIdProvider declaration */
	exDECLARE_EXCEPTION(UndefinedDbParentIdProvider,exDbPersistent) ;   // PRQA S 2131, 2153, 2502

	


	//	
	// Constructors / Destructor
	//
	dbPersistent();				// constructor
	virtual ~dbPersistent() noexcept ;	
	virtual dbPersistent * clone() const = 0 ; // PRQA S 2502
protected:	
	dbPersistent(const dbPersistent&) ; // copy constructor
	dbPersistent &operator=(const dbPersistent &) ;
    // do not declare any move ctor or assigment op
public:	
	/*! cast operator to Mutex. It allows to lock a resource set from external.
	*/
    operator Mutex &() const noexcept { return _mutex ; } // PRQA S 2181

	//* Returns the name of the class
	virtual std::string className() const = 0 ;
	
	//* Returns the name of the class
	virtual std::string objType() const ;



	
	// ************************  hereafter all the abstract methods to be reimplemented  ************************

	// ************** Query Result Inspection Methods *************	

	virtual void writeTo(std::ostream &) const = 0 ;
	virtual void writeTo(exostream &) const = 0 ;

	//Paolo: Could it have sense to export/inport queries results.
	virtual void exportObj(rsResourceSet &) const ;
	virtual void importObj(rsResourceSet &) ;
	virtual void exportXML(XMLOstream &) const ;

	virtual void objectExport(const std::string &path, bool aux=true, bool eraseIfExists=false) ;
    virtual std::string getExportFilename() ;


	// ************** Specific dbPersistent methods *************	
	
	//* initialize the class
	virtual void reset() ; 
	
	//* save the data in database - returns the id
	virtual IdType save( dbConnection &) = 0 ;
	/** 
	 * load the class from the database
	 * for some classes is meaningful to provide object id
	 */
	virtual bool load( dbConnection &c, IdType id=0) = 0 ;
	
	
	//* update an existing instance in database
	virtual IdType update(dbConnection &) = 0 ; 
	
	/** 
	 * remove the class from the database
	 * for some classes is meaningful to provide object id
	 */
	virtual IdType remove(dbConnection &) = 0 ; // PRQA S 2502
	
	//* True if the class have valid parameters, false otherwise
	virtual bool validate() const = 0 ; 
	
	//* Reload the object if it's from database.
	virtual void refresh(dbConnection &) ; 



	

	//* set the unique class id
	virtual void setId(IdType) ;
	
	//* get the unique class id 
	virtual IdType getId() const ; 

	virtual bool isModified() const ;
	virtual bool isFromDatabase() const ;
	
	//* duplicates management
	virtual bool couldBeDuplicated(const dbPersistent *) ;

	//* Inherited from dbStateWrapper
	virtual dbMemento* getState(dbConnection &) const ;
	virtual void setState(dbMemento const*) ;

protected:

	void resetModifiedFlag() ;
	void setModifiedFlag() ;
public:
	virtual void resetFromDatabase() ;
	virtual void setFromDatabase() ;
protected:
	IdType getLastSerial(dbConnection &, const std::string &table, const std::string &stat="") const ;
	
// PRQA S 1020 EOF
#define XMLTagM(os, s1, s2, l) dbPersistent::XMLTag_(os,# s1,s1,# s2,s2,l)
    template<typename T> 
    inline static void XMLSetTag_(XMLOstream &os, const std::string &s_v, const T & v, const std::string &l)
    {
	    XMLOstream::Tag t(os, s_v, db::toSQL("", v), std::string("label=\"") + l + "\"");  // PRQA S 3081
    }
    
    template<typename T> inline
    static void XMLTag_(XMLOstream &os, 
					  const std::string &s_set, 
					  bool set, 
					  const std::string &s_v, 
					  const T & v, 
					  const std::string &l ) 						 
    {
	    if(set)
	    {
		    XMLSetTag_<T>(os, s_v, v, l);
	    }
	    else
	    {
		    XMLOstream::Tag t(os, s_v, std::string("<null>"), std::string("label=\"") + l + "\""); // PRQA S 3081
	    }
	    {
		    XMLOstream::Tag t(os, s_set, (set ? "true" : "false")) ; // PRQA S 3380
	    }
    }

#define RSGetM(s, s1, s2) dbPersistent::RSGet_(s,# s1,s1,# s2,s2)
    template<typename T> inline
    static void RSGet_(rsResourceSet &s, 
					 const std::string &s_set, 
					 bool & set, 
					 const std::string &s_v,
					 T & v) 
    {
	    s.getValue(s_set,set) ;
	    if(set) {
		    s.getValue(s_v,v) ;
        }
    }


#define RSSetM(s, s1, s2) dbPersistent::RSSet_(s,# s1,s1,# s2,s2)
    template<typename T> inline
    static void RSSet_(rsResourceSet &s, 
					 const std::string &s_set, 
					 bool set, 
					 const std::string &s_v,
					 const T & v) 
    {
	    if(set) {
		    s.setValue(s_v,v) ;
        }
	    s.setValue(s_set,set) ;
    }

#define writeToM(s, s1, s2) dbPersistent::inserter_(s, s1, # s2, s2)
    template<typename S, typename T> inline
    static void inserter_(S &stream, bool set, 
					 const std::string &s_v,
					 const T & v) 
    {
        stream << s_v << " = " << (set ? db::toSQL("", v) : std::string("<null>")) << '\n' ; // PRQA S 3081, 3380, 3385
    }
    
    template<typename S> inline
    static void inserter_(S &stream, bool set, 
					 const std::string &s_v,
					 const std::string & v) 
    {
        stream << s_v << " = " << (set ? v : std::string("<null>")) << '\n' ; // PRQA S 3081, 3380, 3385
    }
    
private:
	IdType _id ; 
	bool	_modified ;
	bool	_fromDatabase ;
	
	// only a flag: not used in comparisons and/or database operations 	
    mutable Mutex   _mutex ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbPersistent) ;
} ; 

template<> 
inline void dbPersistent::XMLSetTag_<std::string>(XMLOstream &os, const std::string &s_v, const std::string & v, const std::string &l) // PRQA S 2133, 2134
{
	XMLOstream::Tag t(os, s_v, v, std::string("label=\"") + l + "\""); // PRQA S 3081
}
    
std::ostream &operator<<(std::ostream &, const dbPersistent&) ;	

exostream &operator<<(exostream &, const dbPersistent&) ;   // PRQA S 2072	

//For base class checks only id and modified flag
bool operator==(const dbPersistent &, const dbPersistent &) ;

//For base class checks only id and modified flag
bool operator!=(const dbPersistent &, const dbPersistent &) ;
	

// Class useful to automatically remove any dbPersistent when exiting from the scope
class dbPersistentAutoRemover
{
public:
    dbPersistentAutoRemover(dbConnectionPool& p, dbPersistent& o) :
        _p(p),
        _o(o),
        _mustdelete (true)
    {}

    ~dbPersistentAutoRemover() noexcept
    {
        if (_mustdelete)
        {
            deleteIt();
        }
    }

    dbPersistentAutoRemover(const dbPersistentAutoRemover&) = delete;
    dbPersistentAutoRemover& operator = (const dbPersistentAutoRemover&) = delete;
    dbPersistentAutoRemover( dbPersistentAutoRemover &&) = delete;
    dbPersistentAutoRemover& operator = ( dbPersistentAutoRemover &&) = delete;

    void release() noexcept
    {
        _mustdelete = false;
    }

private:
    void deleteIt() noexcept
    {
        try
        {
            // do your best to remove it but never throw
            if (_o.isFromDatabase())
            {
                dbConnectionWrapper conn (_p);
                _o.remove(conn);
            }
        }
        catch (std::exception& ex)
        {
            ACS_LOG_ERROR ("Unable to remove a temporary inventory object " << _o.getId() << ": " << ex);
        }
    }

    dbConnectionPool& _p;
    dbPersistent& _o;
    bool _mustdelete;
};

_ACS_END_NAMESPACE


#endif // _dbPersistent_H_
