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
	Revision 1.9  2013/05/14 18:41:33  marpas
	trying to release resources exiting
	
	Revision 1.8  2013/02/27 12:38:47  marpas
	coding best practices applied
	qa rules
	interface rationalization
	
	Revision 1.7  2012/02/22 11:30:39  enrcar
	EC:: Method addNewInstance added to allow the same plugin to contain several instances of the same SQLDialect
	
	Revision 1.6  2012/02/13 15:27:48  marpas
	refactoring
	
	Revision 1.5  2009/03/04 17:56:34  marpas
	CODECHECK parsing improved  - no regressions
	
	Revision 1.4  2009/03/02 16:32:03  marpas
	CODECHECK parsing - no regression
	
	Revision 1.3  2008/05/22 16:32:26  lucio.pulvirenti
	EC:: Re-commited since DOS EOLs were used
	
	Revision 1.2  2008/05/22 14:07:19  ivafam
	EC:: Modified after importing into Rational Rose
	
	Revision 1.1.1.2  2007/12/04 10:44:23  crivig
	Imported source
	
*/

#ifndef _Postgres8SQLDialectFactory_
#define _Postgres8SQLDialectFactory_

#include <acs_c++config.hpp>
#include <SQLDialectFactory.h>
#include <Postgres8SQLDialect.h>
#include <boost/shared_ptr.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

//##ModelId=4835786503B3
class Postgres8SQLDialectFactory : public SQLDialectFactory // PRQA S 2109
{

public:
	virtual ~Postgres8SQLDialectFactory() throw() ; 
	

protected:
	Postgres8SQLDialectFactory() ;
	virtual dbConnectionParameters getConnectionParameters(const std::string &key, const std::string &parameters) ;
	virtual void addNewInstance(const std::string &key) ;

private:
	// declared but not implemented
    Postgres8SQLDialectFactory(const Postgres8SQLDialectFactory &) ;
	Postgres8SQLDialectFactory &operator=(const Postgres8SQLDialectFactory &) ;

public:
	static  char const * const statDialectName ; // PRQA S 2100	

private:
	std::map<std::string, boost::shared_ptr<Postgres8SQLDialect> > _myDialect ;

	static Postgres8SQLDialectFactory _theUniqueInstance ;
	
} ;

_ACS_END_NAMESPACE

#endif
