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
	Revision 5.3  2016/04/11 13:19:54  marpas
	removing useless methods and also deprecated ones
	
	Revision 5.2  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.1  2014/02/07 18:38:59  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.25  2013/04/30 13:09:35  marpas
	dbPersistent interface changed
	
	Revision 2.24  2013/04/17 17:36:33  marpas
	dbPersistent interface changed
	
	Revision 2.23  2013/04/09 07:58:05  marpas
	dbPersistent::getLastSerial signature changed to support statistics
	
	Revision 2.22  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.21  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.20  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.19  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.17  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.16  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.15  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.14  2011/03/02 17:39:39  marpas
	support to serial 8 improved
	
	Revision 2.13  2011/03/02 17:29:14  marpas
	support of serial 8 implemented
	
	Revision 2.12  2009/09/29 15:45:44  marpas
	useless include removed
	
	Revision 2.11  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.10  2008/12/05 17:09:41  marpas
	dbConnection::getLastPersistent used
	
	Revision 2.9  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.8  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.7  2007/10/17 10:38:39  marpas
	getExportFileName i/F fixed again
	
	Revision 2.6  2007/10/17 09:45:11  marpas
	objectExport and getExportFilename methods added
	
	Revision 2.5  2007/06/09 19:49:58  marpas
	getHtmlModifyDescription virtual method added - throws an exception
	
	Revision 2.4  2006/10/27 18:26:25  paoscu
	Better error message in an exception.
	
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
	
	Revision 1.34  2006/01/12 17:11:18  enrcar
	getLastSerial method called
	
	Revision 1.33  2005/10/18 18:13:02  paoscu
	Implementation of alignement tags changed.
	
	Revision 1.32  2005/09/23 17:45:21  marpas
	showAllFieldsInList method added
	
	Revision 1.31  2005/09/06 16:45:42  paoscu
	dbColumn::ColumnType used instead of dbType.
	
	Revision 1.30  2005/08/11 16:55:34  paoscu
	Alignement enum and defaultAlignement method added.
	
	Revision 1.29  2005/08/11 13:09:46  paoscu
	Bug introduced in ver 1.27 fixed: Now setState and getState do not access anymore
		dbPersistent private memebers
	getLastSerial method now takes advantage of SQLString class.
	
	Revision 1.28  2005/08/04 20:53:37  marpas
	regression of the last version fixed

	Revision 1.27  2005/08/04 15:16:52  paoscu
	Now setState and getState do not access anymore dbPersistent private memebers.
	
	Revision 1.26  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.25  2004/09/03 13:26:26  marpas
	toggleMark method added
	bug fixed with marked flag
	
	Revision 1.24  2004/09/01 17:23:39  marpas
	objtype concept introduced
	
	Revision 1.23  2004/08/31 09:54:49  marpas
	no significant changes
	
	Revision 1.22  2004/07/20 09:10:42  marpas
	namespace prepended
	
	Revision 1.21  2004/07/13 14:45:18  marpas
	exDbPersistent is an abstract class
	
	Revision 1.20  2004/05/25 12:28:54  ivafam
	Attribute _marked and related method added
	
	Revision 1.19  2004/04/13 14:10:59  marpas
	ACE dependencies removed
	
	Revision 1.18  2004/04/13 13:36:00  marpas
	changes due to new libThread
	
	Revision 1.17  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.16  2004/03/23 12:59:29  paoscu
	ParentIdProvider interface added.
	 getHtmlTableHeader(HtmlRow &) and getHtmlDescription(HtmlRow &, int index)  added.
	
	Revision 1.15  2004/02/16 13:37:12  paoscu
	ConnectionNotProvided exception added.
	Comments in Doxygen style.
	
	Revision 1.14  2004/02/04 10:11:27  paoscu
	dbPersistent interface chanched.
	
	Revision 1.13  2003/10/29 15:22:52  paoscu
	*** empty log message ***
	
	Revision 1.12  2003/10/15 15:21:34  paoscu
	InvalidLabel exception added.
	Methods className and fieldName added.
	
	Revision 1.11  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.10  2003/07/07 16:55:47  paoscu
	copyFrom removed.
	exportXML added.
	
	Revision 1.9  2003/06/13 17:58:31  paoscu
	exportObj, importObj and copyFrom methods added.
	
	Revision 1.8  2003/04/30 16:00:03  paoscu
	refresh() method added.
	
	Revision 1.7  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.6  2003/02/13 13:53:55  marpas
	*** empty log message ***
	
	Revision 1.5  2003/02/07 17:46:38  marpas
	dbPersistentScopeGuard class added
	
	Revision 1.4  2003/02/03 18:58:17  marpas
	Exception hierarchy tree modified to hilight db exceptions
	
	Revision 1.3  2003/02/03 15:55:19  marpas
	getLastSerial method added
	
	Revision 1.2  2003/01/28 15:23:56  paoscu
	inheritance chain slightly modified
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		
*/
#include <dbPersistent.h>
#include <dbPersistentFactoryChain.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbdV.h>
#include <XMLOstream.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <exStream.h>
    
#include <SQLString.h>
#include <SQLDialectFactory.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
     dbdV version ; // versioning 
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbPersistent)


dbPersistent::dbPersistent() :
	dbStateWrapper(),
	_id(0),
	_modified(false),
	_fromDatabase(false),
    _mutex()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "set dbPersistent_DEBUG greater or equal to 1000 to perform duplicates simulation") ; 
}

dbPersistent::~dbPersistent() noexcept {}

dbPersistent::dbPersistent(const dbPersistent &d) :
	dbStateWrapper(d),
	_id(d._id),
	_modified(d._modified),
	_fromDatabase(d._fromDatabase),
    _mutex()
{
}

dbPersistent &dbPersistent::operator=(const dbPersistent &d)
{
	// note DON'T copy the GUARD neither the mark
	if (this != &d) {
	    dbStateWrapper::operator=(d),
		_id = d._id ;
		_modified = d._modified ;
		_fromDatabase = d._fromDatabase ;
	}
	
	return *this ;
}


string dbPersistent::objType() const 
{
#warning try to optimize this, it should be a class member, not inherited
	return dbPersistentFactoryChain::instance()->getObjType(className()) ;
}



bool operator==(const dbPersistent &lhs, const dbPersistent &rhs) 
{
	return (lhs._id == rhs._id) && 
           (lhs._modified == rhs._modified) ; 
}

bool operator!=(const dbPersistent &lhs, const dbPersistent &rhs) 
{
	return not (lhs == rhs) ;
}

void dbPersistent::reset()
{
	_id = 0 ;
	resetModifiedFlag() ;
	resetFromDatabase() ;
}

//  declarations
void dbPersistent::setId(IdType id) { _id = id ; }
dbPersistent::IdType dbPersistent::getId() const { return _id ; }



// dummy declaration of 'refresh' function
// 
void dbPersistent::refresh(dbConnection &conn) 
{
	Lock lock(_mutex) ;

	if(isFromDatabase()) { load(conn); }
}


void dbPersistent::resetModifiedFlag() 
{
	_modified = false ;
}

void dbPersistent::setModifiedFlag() 
{
	_modified = true ;
}

bool dbPersistent::isModified() const 
{
	return _modified ;
}


void dbPersistent::resetFromDatabase() 
{
	_fromDatabase = false ;
}

void dbPersistent::setFromDatabase() 
{
	_fromDatabase = true ;
}


bool dbPersistent::isFromDatabase() const 
{
	return _fromDatabase ;
}

bool dbPersistent::couldBeDuplicated(const dbPersistent *) // PRQA S 4020
{
	ACS_THROW(InvalidUse("Base class method invocation")) ; // PRQA S 3081
}


dbPersistent::IdType dbPersistent::getLastSerial(dbConnection &conn, const string &table, const std::string &stat ) const 
{
	return conn.getLastSerial(table, stat) ;
}


void dbPersistent::exportObj(rsResourceSet &s) const 
{
	Lock guard(_mutex);
	rsPushNSpace nsp(s,"dbPersistent") ;
	s.setValue("_id",_id) ;
	s.setValue("_modified",_modified) ;
	s.setValue("_fromDatabase",_fromDatabase) ;
}

void dbPersistent::importObj(rsResourceSet &s) 
{
	Lock guard(_mutex);
	rsPushNSpace nsp(s,"dbPersistent") ;
	s.getValue("_id",_id) ;
	s.getValue("_modified",_modified) ;
	s.getValue("_fromDatabase",_fromDatabase) ;
}


void dbPersistent::exportXML(XMLOstream &xst) const
{
	Lock guard(_mutex);
	XMLOstream::Tag txml(xst, "dbPersistent");

	{
		ostringstream os;
		os<<_id;
		XMLOstream::Tag txml(xst, "_id", os.str(), "label=\"Id\""); // PRQA S 2502
	}

	{
		XMLOstream::Tag txml(xst, "_modified", (_modified ? "true" : "false"), "label=\"Modified\""); // PRQA S 2502, 3380
	}

	{
		XMLOstream::Tag txml(xst, "_fromDatabase", (_fromDatabase ? "true" : "false"), "label=\"From Database\""); // PRQA S 2502, 3380
	}
	
}

void dbPersistent::objectExport(const std::string &path, bool aux, bool eraseIfExists) 
{
}

std::string dbPersistent::getExportFilename() 
{
	ACS_THROW(NotImplemented("getExportFileName not implemented for " + className())) ; // PRQA S 3081
}



dbStateWrapper::dbMemento* dbPersistent::getState(dbConnection &conn) const
{
	Lock guard(_mutex);
	
	
	dbPersistentMemento* m=new dbPersistentMemento(isModified(), isFromDatabase(),  getId());

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "m-> _id = " <<  m-> _id ) ;


	return m ;
}


void dbPersistent::setState(dbMemento const* mem)
{
	dbPersistentMemento const* m=dynamic_cast<dbPersistentMemento const*> (mem) ; // PRQA S 3081 2
	ACS_COND_THROW(!m, exIllegalValueException("Error in dynamic cast: expecting a dbPersistentMemento const* ")) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "m-> _id = " <<  m-> _id ) ;

	
	
	setId( m-> _id );
	
	if( m-> _fromDatabase ) {
		setFromDatabase() ;
    }
	else {
		resetFromDatabase() ;
    }
	
	if(m-> _modified) {
		setModifiedFlag() ;
    }
	else {
		resetModifiedFlag() ;
    }
}


// output to stream
ostream &operator<<(ostream &os, const dbPersistent &s) 
{
	s.writeTo(os); 
	return os ; // PRQA S 4028 
}

exostream &operator<<(exostream &os, const dbPersistent &s) // PRQA S 4222
{
	s.writeTo(os); 
	return os ;  // PRQA S 4028 
}


//
// dbPersistentMemento class
//
dbPersistent::dbPersistentMemento::dbPersistentMemento(
        bool modified, 
        bool fromDatabase,
        IdType id) :
    dbStateWrapper::dbMemento(),
    _id(id), 
    _modified(modified), 
    _fromDatabase(fromDatabase)
{}

dbPersistent::dbPersistentMemento::~dbPersistentMemento()
{}


_ACS_END_NAMESPACE
