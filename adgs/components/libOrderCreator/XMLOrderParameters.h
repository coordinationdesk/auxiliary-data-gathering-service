// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libOrderCreator $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/02/11 12:50:20  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.4  2012/10/26 10:21:20  marpas
	TriggeringUserDistribution added to TriggeringFile
	
	Revision 2.3  2012/09/17 17:07:49  marpas
	new method to get conf values,
	new class TriggeringFile with triggering filename, fileversion and orderid - 0 if was null
	updated XMLOrderParameters class, writes also the generating orderid for the triggering file(s)
	update TemporaryOrder with new queries and a method to get the generating orderid for the triggering file
	
	Revision 2.2  2012/03/07 13:32:18  marpas
	parameters file management improved and i/f changed
	
	Revision 2.1  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	
*/



#ifndef _XMLOrderParameters_H_
#define _XMLOrderParameters_H_ 

#include <acs_c++config.hpp>
#include <TemporaryOrder.h>
#include <XMLOstream.h>
#include <sstream>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)


class XMLOrderParameters { // PRQA S 2109
public :

	XMLOrderParameters() ;
	virtual ~XMLOrderParameters() throw() ;
    XMLOstream &xml() ;
    void setTriggeringFiles(const std::vector<TriggeringFile > &, const std::string &user="") ;
    void write(std::ostream &) ; // PRQA S 2502
    
private:
    XMLOrderParameters(const XMLOrderParameters &) ;
    XMLOrderParameters & operator= (const XMLOrderParameters &) ;
    
#ifdef TEST_XMLOrderParameters
public:
#else
private :
#endif
    std::ostringstream xmlos_ ;
	XMLOstream xml_ ;
} ;

_ACS_END_NAMESPACE


#endif /* _XMLOrderParameters_H_ */
