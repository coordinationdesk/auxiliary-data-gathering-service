// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPostgres8Dialect $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 09:49:24  marpas
	adoption of libException 5.x standards
	
	Revision 1.10  2013/05/14 18:41:33  marpas
	trying to release resources exiting
	
	Revision 1.9  2013/02/27 12:38:47  marpas
	coding best practices applied
	qa rules
	interface rationalization
	
	Revision 1.8  2012/11/21 10:10:23  marpas
	HEAD realignement
	
	Revision 1.6  2012/02/22 11:30:39  enrcar
	EC:: Method addNewInstance added to allow the same plugin to contain several instances of the same SQLDialect
	
	Revision 1.5  2012/02/17 14:09:56  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 1.4  2012/02/13 15:27:48  marpas
	refactoring
	
	Revision 1.3  2009/03/04 17:56:34  marpas
	CODECHECK parsing improved  - no regressions
	
	Revision 1.2  2009/03/02 16:32:03  marpas
	CODECHECK parsing - no regression
	
	Revision 1.1.1.2  2007/12/04 10:44:23  crivig
	Imported source
	
	
	

*/


#include <Postgres8ConnectionParameters.h>
#include <Postgres8SQLDialectFactory.h>
#include <Psg8DV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
    Psg8DV version ;
}

char const * const Postgres8SQLDialectFactory::statDialectName = "Postgres8" ;

Postgres8SQLDialectFactory Postgres8SQLDialectFactory::_theUniqueInstance ;

Postgres8SQLDialectFactory::~Postgres8SQLDialectFactory() throw() 
{
}

Postgres8SQLDialectFactory::Postgres8SQLDialectFactory():
	SQLDialectFactory(statDialectName),
	_myDialect()
{
	Postgres8SQLDialectFactory::addNewInstance(_curkey) ; 
}


void Postgres8SQLDialectFactory::addNewInstance(const string &key)
{
/* remove _myDialect */
	_myDialect[key].reset(new Postgres8SQLDialect(key)) ;

	registerNewInstance(key,_myDialect[key].get()) ;
}

dbConnectionParameters Postgres8SQLDialectFactory::getConnectionParameters(const string &key, const string &parameters) 
{
    return dbConnectionParameters(new Postgres8ConnectionParametersI(key, parameters)) ; // PRQA S 3081
}

_ACS_END_NAMESPACE
