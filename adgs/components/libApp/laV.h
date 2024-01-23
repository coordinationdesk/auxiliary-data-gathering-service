// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/06 08:37:36  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.3  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.2  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:59  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:57  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.1  2009/04/03 15:39:25  marpas
	some rules fixed
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/14 15:10:49  paoscu
	headers fixed
	
	Revision 1.1.1.1  2002/11/27 10:23:36  paoscu
	Import libApp
	

*/
 
#ifndef _laV_H_
#define _laV_H_

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class laV { // PRQA S 2109
public:
	laV() ACS_NOEXCEPT ;
	~laV() ACS_NOEXCEPT ;
	static const char* tag() ACS_NOEXCEPT ;
	static const char* name() ACS_NOEXCEPT ;
private:
	//declared but not defined to prevent their use
	laV(const laV &) ;
	laV & operator=(const laV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _laV_H_



