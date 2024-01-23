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

	$Prod: A.C.S. String Utility Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2009/03/12 10:10:55  marpas
	rules violations fixed and or justified
	
	Revision 1.2  2004/12/03 17:27:54  danalt
	Fixed ACS header
	
	Revision 1.1.1.1  2004/09/20 16:47:24  marfav
	Import libStringUtils
	

*/
 
#ifndef _suV_H_
#define _suV_H_

namespace acs {

// support class for versioning

class suV { // PRQA S 2109
public:
	static const char*  tag() noexcept ;
	static const char*  name() noexcept ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _suV_H_



