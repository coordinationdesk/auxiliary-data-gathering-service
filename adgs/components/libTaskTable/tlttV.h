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

	$Prod: A.C.S. ThinLayer TaskTable Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:24:08  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.1  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 10:07:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 18:03:32  marfav
	Header fixed
	
	Revision 1.2  2003/05/05 09:20:32  marfav
	got rid of extra ^M
	
	Revision 1.1.1.1  2003/01/31 11:05:00  marfav
	Import libTaskTable
	
	

*/

#ifndef _tlttV_H_
#define _tlttV_H_

namespace acs {

// support class for versioning

class tlttV { // PRQA S 2109
public:
	tlttV() throw() ;
	~tlttV() throw() ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
    tlttV(const tlttV &) ; // not implemented 
    tlttV& operator= (const tlttV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tlttV_H_



