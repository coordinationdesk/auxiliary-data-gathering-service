// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.10  2013/05/14 18:11:28  marpas
	minor changes and debug improved
	
	Revision 4.9  2013/04/22 15:26:35  marpas
	roleName concept added to connection parameters
	
	Revision 4.8  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.7  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.1  2011/09/12 12:40:14  marpas
	filterables used in nominal notifications
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.2  2009/06/17 13:36:15  enrcar
	EC:: Encrypted passwords handled
	
	Revision 2.1  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/08/09 15:20:23  enrcar
	Port parameter handled in db.conf
	
	Revision 1.3  2005/05/05 15:02:59  enrcar
	added DriverLibrary and VendorLibrary
	removed LibraryName
	
	Revision 1.2  2005/05/03 10:16:19  enrcar
	server parameter added:
	hostname is used as machine-name
	server is used for contacting the database
	
	Revision 1.1  2005/04/21 17:11:15  marpas
	work in progress
	

*/

#include <dbConnectionParameters.h>
#include <dbAppResSet.h>
#include <Application.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using std::string ; 

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConnectionParametersI)


dbConnectionParametersI::dbConnectionParametersI(const std::string &parameters)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

    if (Application::exists()) {
        _roleName = Application::instance()->applicationName() ;
    }

    if (!parameters.empty()) {        
	    std::istringstream is(parameters) ;
	    is >> _params ;
	    dbConnectionParametersI::importObj(_params) ;
    }	
}


// PRQA S 4120, 4121 EOF
const string & dbConnectionParametersI::getDriverFunc() const noexcept { return _getDriverFunc ; }
const string & dbConnectionParametersI::driverLibrary() const noexcept { return _driverLibrary ; }
const string & dbConnectionParametersI::vendorLibrary() const noexcept { return _vendorLibrary ; }

const string & dbConnectionParametersI::hostname() const noexcept { return _hostname ; }

const string & dbConnectionParametersI::server() const noexcept { return _server ; }

const string & dbConnectionParametersI::user() const noexcept { return _user ; }

const string & dbConnectionParametersI::password() const noexcept { return _password ; }

const ACS_SSL::EncryptionAlgo & dbConnectionParametersI::pwdEncryptionAlgo() const noexcept { return _pwdEncryptionAlgo ; }

const string & dbConnectionParametersI::db() const noexcept { return _db ; }

void dbConnectionParametersI::roleName(const std::string &rolename) { _roleName = rolename ; }
const string & dbConnectionParametersI::roleName() const noexcept { return _roleName ; }

unsigned short dbConnectionParametersI::port() const noexcept { return _port ; }

const string & dbConnectionParametersI::salt() const noexcept { return _salt ; }
const size_t & dbConnectionParametersI::saltSize() const noexcept { return _saltSize ; }
bool dbConnectionParametersI::assumeNoPostGIS() const noexcept { return _assumeNoPostGIS ; }


void dbConnectionParametersI::exportObj(rsResourceSet &s) const
{
	// write values in resourceset
	// they are part of dbconf file
	s.setValue("hostname",_hostname) ;
	s.setValue("server",_server) ;
	s.setValue("user",_user) ;
	s.setValue("password",_password) ;
	s.setValue("passwordEncryptionAlgo", int(_pwdEncryptionAlgo)) ; // PRQA S  3081
	s.setValue("db",_db) ;
	s.setValue("port",_port) ;
	s.setValue("DriverLibrary",_driverLibrary) ;
	s.setValue("GetDriverFunc",_getDriverFunc) ;
	s.setValue("VendorLibrary",_vendorLibrary) ;
	if( ! _salt.empty() ) {
		s.setValue("salt_string",_salt) ;
	}
	if( 0 != _saltSize ) {
		s.setValue("salt_random",_saltSize) ;
	}
	s.setValue("assumeNoPostGIS",_assumeNoPostGIS) ;

}

void dbConnectionParametersI::importObj(rsResourceSet &s) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	try {
		// read values from resourceset
		// they are part of dbconf file
		s.getValue("hostname",_hostname) ;
		s.getValue("server",_server) ;
		s.getValue("user",_user) ;
		s.getValue("password",_password) ;	
		s.getValue("db",_db) ;
		s.getValue("port",_port) ;
		s.getValue("DriverLibrary",_driverLibrary) ;
		s.getValue("GetDriverFunc",_getDriverFunc) ;
		s.getValue("VendorLibrary",_vendorLibrary) ;

		// Optional parameters:
		try	{ // NOSONAR - intercept not found
			int i ; // PRQA S 4101
            s.getValue("passwordEncryptionAlgo", i) ; 
			_pwdEncryptionAlgo = ACS_SSL::EncryptionAlgo(i) ; // PRQA S 3013, 3081
		}
		catch (const rsResourceSet::NotFoundException&)
		{
            ACS_LOG_WARNING("passwordEncryptionAlgo missing in db.conf: ASSUMING UNENCRYPTED") ;
			_pwdEncryptionAlgo = ACS_SSL::ENC_NONE ;
		}
		
		try	{ // NOSONAR - intercept not found
            s.getValue("salt_string", _salt) ; 
		}
		catch (const rsResourceSet::NotFoundException& )
		{
			_salt = string();
		}
		
		try	{ // NOSONAR - intercept not found
            s.getValue("salt_random", _saltSize) ; 
		}
		catch (const rsResourceSet::NotFoundException& )
		{
			_saltSize = 0;
		}
		
		try	{ // NOSONAR - intercept not found
            s.getValue("assumeNoPostGIS", _assumeNoPostGIS) ; 
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_assumeNoPostGIS: " << _assumeNoPostGIS) ;
		}
		catch (const rsResourceSet::NotFoundException& )
		{
			_assumeNoPostGIS = false;
		}
		
		
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "hostname: " << _hostname) ;
	
	}
	catch(const std::exception &e) {
		ACS_THROW(ConnParException(e,"Error in connection parameters.")) ; // PRQA S 3081
	}
}

void dbConnectionParametersI::exportXML(XMLOstream &xst) const
{
	// write in xml
	// <hostname>...</hostname>
	// ...
	// <VendorLibrary>...</VendorLibrary>
	{ XMLOstream::Tag t(xst, "hostname", 		_hostname, 		"", true ) ; }
	{ XMLOstream::Tag t(xst, "server", 			_server, 		"", true ) ; }
	{ XMLOstream::Tag t(xst, "user",			_user, 			"", true ) ; }
	{ XMLOstream::Tag t(xst, "password", 		_password, 		"", true ) ; } 
	{ XMLOstream::Tag t(xst, "passwordEncryptionAlgo", 	int(_pwdEncryptionAlgo), "", true ) ; } // PRQA S 3081
	if( ! _salt.empty() ) {
		XMLOstream::Tag t(xst, "salt_string", 	_salt, 			"", true ) ; 
	}
	if( _saltSize ) {
		XMLOstream::Tag t(xst, "salt_random", 	_saltSize, 		"", true ) ; 
	}
	{ XMLOstream::Tag t(xst, "db", 				_db, 			"", true ) ; }
	{ XMLOstream::Tag t(xst, "port", 			int(_port),		"", true ) ; } // PRQA S 3081
	{ XMLOstream::Tag t(xst, "DriverLibrary", 	_driverLibrary,	"", true ) ; }
	{ XMLOstream::Tag t(xst, "GetDriverFunc",	_getDriverFunc,	"", true ) ; }
	{ XMLOstream::Tag t(xst, "VendorLibrary",	_vendorLibrary,	"", true ) ; }
	if (_assumeNoPostGIS) { XMLOstream::Tag t(xst, "assumeNoPostGIS",	_assumeNoPostGIS,	"", true ) ; }
}


std::ostream &operator<<(std::ostream &os, const dbConnectionParameters &p) 
{
    if (!p->hostname().empty()) {
        os << "hostname=" << p->hostname() << ';' ;
    }
    if (!p->server().empty()) {
        os << "server=" << p->server() << ';' ;
    }
    if (p->port() != 0) { // PRQA S 3000
        os << "port=" << p->port() << ';' ;
    }
    if (!p->roleName().empty()) {
        os << "role=" << p->roleName() << ';' ;
    }
    if (!p->db().empty()) {
        os << "db=" << p->db() << ';' ;
    }
    if (!p->salt().empty()) {
        os << "salt_string=" << p->salt() << ';' ;
    }
    if ( p->saltSize() ) {
        os << "salt_random=" << p->saltSize() << ';' ;
    }
    if ( p->assumeNoPostGIS() ) {
	    os << "assumeNoPostGIS: " << std::boolalpha << p->assumeNoPostGIS() << ';' ;
    }
    return os ; // PRQA S 4028
}
  
exostream &operator<<(exostream &os, const dbConnectionParameters &p) // PRQA S 4222
{
    if (!p->hostname().empty()) {
        os << "hostname=" << p->hostname() << ';' ;
    }
    if (!p->server().empty()) {
        os << "server=" << p->server() << ';' ;
    }
    if (p->port() != 0) { // PRQA S 3000
        os << "port=" << p->port() << ';' ;
    }
    if (!p->roleName().empty()) {
        os << "role=" << p->roleName() << ';' ;
    }
    if (!p->db().empty()) {
        os << "db=" << p->db() << ';' ;
    }
    if (!p->salt().empty()) {
        os << "salt_string=" << p->salt() << ';' ;
    }
    if ( p->saltSize() != 0 ) {
        os << "salt_random=" << p->saltSize() << ';' ;
    }
    if ( p->assumeNoPostGIS() ) {
	    os << "assumeNoPostGIS: " << std::boolalpha << p->assumeNoPostGIS() << ';' ;
    }
	
    return os ; // PRQA S 4028
}  


MQTTSubIdHelper toMQTTSubId (const std::string &dbkey) 
{
    MQTTSubIdHelper mqsi ;
    dbConnectionParameters cp = dbAppResSetFactory::instance("",dbkey)->connectionParameters() ;
    mqsi.server = cp->server() ;
    mqsi.db = cp->db() ;
    return mqsi ;
}

_ACS_END_NAMESPACE
