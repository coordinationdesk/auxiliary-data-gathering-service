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
	Revision 1.6  2013/02/27 12:38:47  marpas
	coding best practices applied
	qa rules
	interface rationalization
	
	Revision 1.5  2012/02/17 14:09:56  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 1.4  2012/02/13 15:27:48  marpas
	refactoring
	
	Revision 1.3  2008/05/22 16:32:26  lucio.pulvirenti
	EC:: Re-commited since DOS EOLs were used
	
	Revision 1.2  2008/05/22 14:07:19  ivafam
	EC:: Modified after importing into Rational Rose
	
	Revision 1.1.1.2  2007/12/04 10:44:23  crivig
	Imported source
	
*/

#ifndef _Postgres8ConnectionParameters_H_
#define _Postgres8ConnectionParameters_H_

#include <acs_c++config.hpp>
#include <dbConnectionParameters.h>


_ACS_BEGIN_NAMESPACE(acs)

// Sets/handles enviroment variable needed by low level client API

class Postgres8ConnectionParametersI : public dbConnectionParametersI // PRQA S 2109
{
public:
	Postgres8ConnectionParametersI(const std::string &key, const std::string &parameters) ;
	virtual ~Postgres8ConnectionParametersI() throw() ;
    
private:
    Postgres8ConnectionParametersI() ;
	Postgres8ConnectionParametersI(const Postgres8ConnectionParametersI &) ;
	Postgres8ConnectionParametersI &operator=(const Postgres8ConnectionParametersI &) ;

public:
	virtual void setOptions(dbSQLConnection &) ;

	// inherited from dbExportable 
    virtual void exportObj(rsResourceSet &s) const ;	// export the class to resourceSet
	virtual void importObj(rsResourceSet &s) ;	// import the class from a resourceSet
	virtual void exportXML(XMLOstream &xst) const ;	// export the class to XML stream

private:
	std::string _key ;
	
} ;

_ACS_END_NAMESPACE

#endif // _Postgres8ConnectionParameters_H_
