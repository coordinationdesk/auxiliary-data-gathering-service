// PRQA S 1050 EOF
/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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
	suV() ;
	~suV() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	// declared but not defined to prevent their use
	suV(const suV &) ;
	suV & operator=(const suV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _suV_H_



