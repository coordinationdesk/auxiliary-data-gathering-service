// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libPostgres8Dialect $

	$Id$

	$Author$

	$Log$
	Revision 1.12  2013/05/14 18:42:16  marpas
	debug improved
	
	Revision 1.11  2013/04/22 15:27:44  marpas
	role name set in connection if is valorized
	
	Revision 1.10  2013/02/27 12:38:47  marpas
	coding best practices applied
	qa rules
	interface rationalization
	
	Revision 1.9  2012/11/21 10:10:22  marpas
	HEAD realignement
	
	Revision 1.6  2012/02/17 14:09:56  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 1.5  2012/02/13 15:27:47  marpas
	refactoring
	
	Revision 1.4  2010/10/12 13:13:53  clanas
	modified to use pgsql public interfaces
	
	Revision 1.3  2010/09/11 18:15:26  marpas
	libDbExpress 3.2 interface adopted
	
	Revision 1.2  2008/07/08 09:19:45  enrcar
	EC:: ACS_PTRASINT_T is used instead of INT32
	
	Revision 1.1.1.2  2007/12/04 10:44:23  crivig
	Imported source
	
*/

#include <Postgres8ConnectionParameters.h>
#include <SQLDialect.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


Postgres8ConnectionParametersI::Postgres8ConnectionParametersI(const string& key, const string &parameters) : 
    dbConnectionParametersI(parameters), 
    _key(key)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Key: \"" << key << "\"  parameters: \"" << parameters << "\"") ;
}



Postgres8ConnectionParametersI::~Postgres8ConnectionParametersI() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;// PRQA S 4631 
}


Postgres8ConnectionParametersI::Postgres8ConnectionParametersI(const Postgres8ConnectionParametersI &rhs) :
	dbConnectionParametersI(rhs),
	_key(rhs._key) 
{
	// empty
}


Postgres8ConnectionParametersI &Postgres8ConnectionParametersI::operator=(const Postgres8ConnectionParametersI &rhs)
{
	if (this != &rhs) {
		dbConnectionParametersI::operator=(rhs) ;
		_key = rhs._key ; 
	}
	return *this ;
}


void Postgres8ConnectionParametersI::setOptions(dbSQLConnection &c)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	long r = SQLDialect::SQLReturn(_key, c->setOption (eConnHostName, reinterpret_cast<ACS_PTRASINT_T>(server().c_str()))) ; // PRQA S 3081
        if (r!= SQL_SUCCESS) {
		ostringstream os ;
		os << "setOption (eConnHostName,\"" << hostname() << "\" ) returned: " << r ;
		ACS_THROW(ConnParException(os.str())) ; // PRQA S 3081
	}

	r = SQLDialect::SQLReturn(_key, c->setOption(eConnDatabaseName, reinterpret_cast<ACS_PTRASINT_T>(db().c_str()))) ; // PRQA S 3081
		if (r!= SQL_SUCCESS) {
		ostringstream os ;
		os << "setOption (eConnDatabaseName,\"" << db() << "\" ) returned: " << r ;
		ACS_THROW(ConnParException(os.str())) ; // PRQA S 3081
	}

    if ( not roleName().empty() ) {
	    r = SQLDialect::SQLReturn(_key, c->setOption(eConnRoleName, reinterpret_cast<ACS_PTRASINT_T>(roleName().c_str()))) ; // PRQA S 3081 
		    if (r!= SQL_SUCCESS) {
		    ostringstream os ;
		    os << "setOption (eConnRoleName,\"" << roleName() << "\" ) returned: " << r ;
		    ACS_THROW(ConnParException(os.str())) ; // PRQA S 3081
	    }
    }
    if ( 0 != port() ) // PRQA S 3000
    {
	    ostringstream po ;
	    po << port() ;
	    r = SQLDialect::SQLReturn(_key, c->setOption(eConnServerPort, reinterpret_cast<ACS_PTRASINT_T>(po.str().c_str()))) ; // PRQA S 3081 
		    if (r!= SQL_SUCCESS) {
		    ostringstream os ;
		    os << "setOption (eConnServerPort,\"" << po.str() << "\" ) returned: " << r ;
		    ACS_THROW(ConnParException(os.str())) ; // PRQA S 3081
	    }
    }
    else
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Default port will be used") ;
    }
}
		

void Postgres8ConnectionParametersI::exportObj(rsResourceSet &s) const
{
	dbConnectionParametersI::exportObj(s) ;
}

void Postgres8ConnectionParametersI::importObj(rsResourceSet &s) 
{
	dbConnectionParametersI::importObj(s) ;
}

void Postgres8ConnectionParametersI::exportXML(XMLOstream &xst) const
{
	dbConnectionParametersI::exportXML(xst) ;
}

_ACS_END_NAMESPACE
