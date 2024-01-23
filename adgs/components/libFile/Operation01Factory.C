// PRQA S 1050 EOF
/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/13 10:59:05  marpas
	refactoring in progress
	
	Revision 2.1  2009/05/14 13:54:51  enrcar
	EC:: c++ rules
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/04 12:29:28  marpas
	newObject method name fixed
	
	Revision 1.1  2005/11/03 08:38:35  marpas
	new class added
	
	Revision 1.1  2005/11/02 17:07:12  marpas
	just added
	

*/


#include <Operation01Factory.h>

using namespace std ;

using namespace acs ;

Operation01Factory *Operation01Factory::_theFactory = 0 ;


// constructor
Operation01Factory::Operation01Factory() 
{
	_theFactory = this ;	
}


Operation01Factory::~Operation01Factory() throw() 
{
	if (_theFactory == this) {
		_theFactory = 0 ;
    }
}


Operation01 *Operation01Factory::newObject(const std::string &feature, const std::string &instance) // PRQA S 4020
{
	// W8115_OFF
	// To keep the code legible
	
	if (_theFactory) {
		return _theFactory->createOperation(feature,instance) ;	/* Return new object */
	}
    
	return 0 ;
}
