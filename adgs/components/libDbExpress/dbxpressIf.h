// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. db Express Client Library $

	$Id$

	$Author$

	$Log$
	Revision 3.12  2016/09/16 12:57:22  ricfer
	ctring include really added.
	
	Revision 3.11  2016/09/16 09:14:02  ricfer
	cstring include added.
	
	Revision 3.10  2016/03/14 15:19:35  marpas
	qa warings fixed
	
	Revision 3.9  2016/01/22 17:03:20  enrcar
	EC:: c++ strings should not be 0-terminated ...
	
	Revision 3.8  2013/07/09 08:23:10  marpas
	fixed error string that was previously initialized
	
	Revision 3.7  2013/05/31 09:24:37  marpas
	qa warning removed
	
	Revision 3.6  2013/05/30 11:55:29  marpas
	dbSQLDriver will never invoke Release on SQLDriver
	
	Revision 3.5  2012/12/11 09:55:50  marpas
	qa rules
	
	Revision 3.4  2012/12/10 16:34:44  marpas
	types strong redefinition
	
	Revision 3.3  2012/12/10 12:02:39  marpas
	qa rules
	
	Revision 3.2  2011/12/14 09:57:50  lucio.pulvirenti
	EC:: a fixed-lenght string, with trailing zeros was returned. FIXED: a cast from char* is now returned
	
	Revision 3.1  2010/09/11 17:28:26  marpas
	dbSQLGetError template function added
	
	Revision 3.0  2010/09/10 16:12:00  marpas
	new file dbxpressIf.h with template code to support DbExpress types and I/F
	new documentation
	other improvements in existing files
	
	
	
*/


#ifndef _dbxpressIf_H_
#define _dbxpressIf_H_

#if __cplusplus > 199711L
  #include <memory>
  #define DBXSH_PTR_NSPACE std
#else
  #include <boost/shared_ptr.hpp> // PRQA S 1013
  #define DBXSH_PTR_NSPACE boost
#endif
#include <dbxpress.hpp>
#include <string>
#include <cstring>
#include <algorithm>

/*!
 * \file dbxpressIf.h
 * \brief This file describes convenience types, templates and functions to interface the dbxpress framework (see also \ref index )
 */


/*! 
 * An external function declaration to be linked with 
 */
extern SQLResult __stdcall (*getSQLDriver)(char * VendorLib, char * SResourceFile, ppSQLDriver); // PRQA S 2300

namespace acs {
/*! 
 * \namespace acs 
 * \brief This namespace describes convenience types, templates and functions to interface the dbxpress framework
	(see also \ref index )
*/


/*! 
 * the prototype of the hook to be called in order to get an appropriate driver (SQLDriver)
 */
typedef SQLResult __stdcall (*getSQLDriverPrototype)(char * , char * , ppSQLDriver);


/*!
 * \brief dbSQLDeleter class 
 *
 * This class is to avoid shared_ptr to call dtors on its pointers
 * if the pointer is not null the method Release() will be called.
 *
 */
template <class T>
class dbSQLDeleter {
public:
	/*! 
	 * the operator() to be invoked when a shared_ptr shall dispose its pointer
	 * \param p : The pointer to be disposed (it is checked for null avoiding segmentation faults
	 */
	void operator()(T *p) { if (p) p->Release() ; }
} ;


/*!
 * \brief dbSQLDeleter class (specialization for SQLDriver
 *
 * This class is to avoid shared_ptr to call dtors on its pointers
 * SQLDriver will be never deleted - it is strictly linked to plugins (driver so)
 *
 */
template <>
class dbSQLDeleter<SQLDriver> {
public:
	/*! 
	 * the operator() to be invoked when a shared_ptr shall dispose its pointer
	 * \param p : The pointer to be disposed (it is checked for null avoiding segmentation faults
	 */
	void operator()(SQLDriver *) { } // PRQA S 4211
} ;


/*! 
 * A template function to create a shared_ptr with an appropriate acs::dbSQLDeleter class. 
 * \param p : The pointer to be owned by the created shared_ptr
 */
template<class T>
inline
DBXSH_PTR_NSPACE::shared_ptr<T> dbSQLCreator(T *p) // PRQA S 2134
{
	return DBXSH_PTR_NSPACE::shared_ptr<T>(p, dbSQLDeleter<T>()) ;
}



/*! 
 * A type corresponding to a shared_ptr over an SQLDriver abstract class object pointer 
 */
typedef DBXSH_PTR_NSPACE::shared_ptr<SQLDriver> dbSQLDriver ;		
/*! 
 * A type corresponding to a shared_ptr over an SQLConnection abstract class object pointer 
 */
typedef DBXSH_PTR_NSPACE::shared_ptr<SQLConnection> dbSQLConnection ;
/*! 
 * A type corresponding to a shared_ptr over an SQLCommand abstract class object pointer 
 */
typedef DBXSH_PTR_NSPACE::shared_ptr<SQLCommand> dbSQLCommand ;
/*! 
 * A type corresponding to a shared_ptr over an SQLMetaData abstract class object pointer 
 */
typedef DBXSH_PTR_NSPACE::shared_ptr<SQLMetaData> dbSQLMetaData ;
/*! 
 * A type corresponding to a shared_ptr over an SQLCursor abstract class object pointer 
 */
typedef DBXSH_PTR_NSPACE::shared_ptr<SQLCursor> dbSQLCursor ;

/*!
 * \brief an error getter conveninece function 
 *
 * creates and returns a std:.string with the error gor from the passed SQLConnection, SQLCommand, SQLCursor or SQLMetaData
 * 
 * \param t the object on which the getErrorMessage will be invoked.
 * \return the string with the error 
 */
template <class T>
inline
std::string dbSQLGetError(T &t) // PRQA S 2134, 4020
{
	if (!t) {
		return "" ;
    }
    UINT16 elen ; // PRQA S 4101
	t->getErrorMessageLen(&elen) ; 
	unsigned char s[ std::max( (128*1024), elen+1) ] ; // PRQA S 3000, 4403
	memset(s, 0, sizeof(s)) ; 
	t->getErrorMessage(s) ; // PRQA S 3030, 3081
	const std::string es ( reinterpret_cast<const char*>(s) ) ;  // PRQA S 3030, 3081
 
    return es ;
}


/*!
 * \brief acs::dbSQLDriver creator function 
 *
 * creates and returns a acs::dbSQLDriver pointing to the correct instance of SQLDriver.
 * 
 * \param f  the function to be invoked (the hook in the shared object)
 * \param vlib vendor lib to be passed to f
 * \param resf resource file to be passed to f 
 * \param d reference to the driver (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline
long dbSQLDriverCreator(getSQLDriverPrototype f, // PRQA S 2134
						const char *vlib, 
						const char *resf, 
						dbSQLDriver &d) 

{
	SQLDriver *p = 0 ;
	int status = f(const_cast<char *>(vlib), const_cast<char *>(resf), &p) ; // PRQA S 3081
	d = dbSQLCreator(p);
	return status ;
}

/*!
 * \brief acs::dbSQLConnection creator function 
 *
 * creates and returns a acs::dbSQLConnection pointing to the correct instance of SQLConnection.
 *
 * \param d  the instance to acs::dbSQLDriver (returned from dbSQLDriverCreator() )
 * \param c reference to the connection (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline 
long dbSQLConnectionCreator(const dbSQLDriver &d, dbSQLConnection &c) // PRQA S 2134
{
	SQLConnection *p = 0 ;
	int status = d->getSQLConnection(&p) ;
	c = dbSQLCreator(p) ;
	return status ;
}

/*!
 * \brief acs::dbSQLMetaData creator function 
 *
 * creates and returns a acs::dbSQLMetaData pointing to the correct instance of SQLMetaData.
 *
 * \param c  the instance to acs::dbSQLConnection (returned from dbSQLConnectionCreator() )
 * \param m reference to the metadata (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline 
long dbSQLMetaDataCreator(const dbSQLConnection &c, dbSQLMetaData &m) // PRQA S 2134
{
	SQLMetaData *p = 0 ;
	int status = c->getSQLMetaData(&p) ;
	m = dbSQLCreator(p) ;
	return status ;
}

/*!
 * \brief acs::dbSQLCommand creator function 
 *
 * creates and returns a acs::dbSQLCommand pointing to the correct instance of SQLCommand.
 *
 * \param c  the instance to acs::dbSQLConnection (returned from dbSQLConnectionCreator() )
 * \param cmd reference to the command (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline 
long dbSQLCommandCreator(const dbSQLConnection &c, dbSQLCommand &cmd)   // PRQA S 2134
{
	SQLCommand *p = 0 ;
	int status = c->getSQLCommand(&p) ;
	cmd = dbSQLCreator(p) ;
	return status ;
}

/*!
 * \brief acs::dbSQLCursor creator function (executing prepared SQL statements) 
 *
 * creates and returns a acs::dbSQLCursor pointing to the correct instance of SQLCursor.
 *
 * \param comm  the instance to acs::dbSQLCommand (returned from dbSQLCommandCreator() )
 * \param cur reference to the cursor (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline long dbSQLCommandExecute(const dbSQLCommand &comm, dbSQLCursor &cur) // PRQA S 2134
{
	SQLCursor *p = 0 ;
	int status = comm->execute(&p) ;
	cur = dbSQLCreator(p) ;
	return status ;
}

/*!
 * \brief acs::dbSQLCursor creator function (executing immediate SQL statements) 
 *
 * creates and returns a acs::dbSQLCursor pointing to the correct instance of SQLCursor.
 *
 * \param comm  the instance to acs::dbSQLCommand (returned from dbSQLCommandCreator() )
 * \param sql the SQL statements to be executed
 * \param cur reference to the cursor (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline long dbSQLCommandExecuteImmediate(const dbSQLCommand &comm, const char *sql, dbSQLCursor &cur) // PRQA S 2134
{
	SQLCursor *p = 0 ;
	int status = comm->executeImmediate(const_cast<char *>(sql),&p) ;   // PRQA S 3081
	cur = dbSQLCreator(p) ;
	return status ;
}

/*!
 * \brief acs::dbSQLCursor creator function (executing metadata get tables) 
 *
 * creates and returns an acs::dbSQLCursor pointing to the correct instance of SQLCursor.
 *
 * \param m  the instance to acs::dbSQLMetaData (returned from dbSQLMetaDataCreator() )
 * \param pszTableName the table name (string with zero terminator)
 * \param uTableType the type of the table 
 * \param cur reference to the cursor (will be filled if succesful) 
 * \return the completion code (SQL_SUCCESS means success) 
 */
inline long dbSQLMetaDataGetTables(const dbSQLMetaData &m, const char *pszTableName, UINT32 uTableType, dbSQLCursor &cur)  // PRQA S 2134
{
	SQLCursor *p = 0 ;
	int status = m->getTables ( const_cast<char *>(pszTableName),uTableType,&p) ;   // PRQA S 3081
	cur = dbSQLCreator(p) ;
	return status ;
}

} // namespace
#endif
