// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2018/01/11 12:16:41  marpas
	implementing stream inserter operators for dbConnectionParameters
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.11  2013/05/14 18:11:28  marpas
	minor changes and debug improved
	
	Revision 4.10  2013/04/22 15:26:35  marpas
	roleName concept added to connection parameters
	
	Revision 4.9  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.8  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.7  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.6  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.1  2010/09/11 18:05:27  marpas
	libDbExpress 3.2 interface adopted
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.3  2009/06/17 13:36:15  enrcar
	EC:: Encrypted passwords handled
	
	Revision 2.2  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.1  2009/03/02 14:03:30  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/10/25 19:43:40  marpas
	warning about hostname removed
	
	Revision 1.5  2005/08/09 15:22:03  enrcar
	Port parameter handled in db.conf
	
	Revision 1.4  2005/05/05 15:02:59  enrcar
	added DriverLibrary and VendorLibrary
	removed LibraryName
	
	Revision 1.3  2005/05/03 10:16:19  enrcar
	server parameter added:
	hostname is used as machine-name
	server is used for contacting the database
	
	Revision 1.2  2005/04/22 20:29:31  marpas
	work in progress
	
	Revision 1.1  2005/04/21 17:11:15  marpas
	work in progress
	
	

*/

#ifndef _dbConnectionParameters_H_
#define _dbConnectionParameters_H_

#include <acs_c++config.hpp>
#include <DbExpress.h>
#include <rsResourceSet.h>
#include <dbException.h>
#include <ACS_SSL.h>
#include <string>

//////////////////////////////////////////////
//              							//
// class dbConnectionParameters				//
//              							//
// this is the connection info class		//
//////////////////////////////////////////////

/*!
	\brief Used to manage connection


	This abstract class is used to manage the connection with a generic database. <BR>

*/

_ACS_BEGIN_NAMESPACE(acs)


class XMLOstream ;

class dbConnectionParametersI // PRQA S 2109
{
public:
	/*! class ConnParException declaration */
	exDECLARE_EXCEPTION(ConnParException,dbException) ; // PRQA S 2131, 2153, 2502

public:

	/*!
		Constructor with a string formatted as the file in the above example.
	*/
	explicit dbConnectionParametersI(const std::string &parameters) ;

	virtual ~dbConnectionParametersI() throw() ;

protected:

	dbConnectionParametersI(const dbConnectionParametersI &) ; 	

	dbConnectionParametersI &operator=(const dbConnectionParametersI &) ;
private:
    dbConnectionParametersI() ; // not implemented
public:
	void set(const std::string &name, const std::string &value) ;
	std::string get(const std::string &name) const ;
	void get(const std::string &name, std::string &value) const ;

	const std::string & getDriverFunc() const throw() ;
	const std::string & driverLibrary() const throw() ;
	const std::string & vendorLibrary() const throw() ;

	void roleName(const std::string &) ;
	const std::string & roleName() const throw() ;

	const std::string & hostname() const throw() ;
	const std::string & server() const throw() ;
	const std::string & user() const throw() ;
	const std::string & password() const throw() ;	/* Provided "as-is" (i.e. crypted) */
	const ACS_SSL::EncryptionAlgo & pwdEncryptionAlgo() const throw() ;
	const std::string & db() const throw() ;
	unsigned short port() const throw() ;
	const std::string & salt() const throw() ;
	const size_t & saltSize() const throw();

	virtual void setOptions(dbSQLConnection &) = 0 ;
	
	// inherited from dbExportable 
	virtual void exportObj(rsResourceSet &s) const ;    // export the class to resourceSet
	virtual void importObj(rsResourceSet &s) ;  // import the class from a resourceSet
	virtual void exportXML(XMLOstream &xst) const ; // export the class to XML stream
	
private:
    std::string     _getDriverFunc ;
    std::string     _driverLibrary ;
    std::string     _vendorLibrary ;
    rsResourceSet   _params ;
    std::string	    _hostname ;
    std::string	    _server ;
    std::string	    _user ;
    std::string	    _password ;	
    ACS_SSL::EncryptionAlgo _pwdEncryptionAlgo ;
    std::string	    _db ;
    std::string     _roleName ;
    unsigned short   _port ;
	std::string		_salt;
	size_t			_saltSize;
protected:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConnectionParametersI) ; // PRQA S 2101
} ;

// the following pragma has been added to avoid - warning: ... a non-virtual destructor
#pragma GCC diagnostic ignored "-Weffc++"       // PRQA S 1040

typedef DBXSH_PTR_NSPACE::shared_ptr<dbConnectionParametersI> dbConnectionParameters ;
// reactivate -Wnon-virtual-dtor
#pragma GCC diagnostic warning "-Weffc++"       // PRQA S 1040

class dbConnectionParametersBinder { // just to bind this definition
public:
	dbConnectionParametersBinder() throw() {}
	virtual ~dbConnectionParametersBinder() throw() {} // PRQA S 2131
private:
	// declared but not implemented
	dbConnectionParametersBinder(const dbConnectionParametersBinder &) ;
	dbConnectionParametersBinder &operator=(const dbConnectionParametersBinder &) ;

} ;

std::ostream &operator<<(std::ostream &, const dbConnectionParameters &) ;    // PRQA S 2072 2
exostream &operator<<(exostream &, const dbConnectionParameters &) ; 

_ACS_END_NAMESPACE

#endif // _dbConnectionParameters_H_
