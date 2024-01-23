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

	$Prod: A.C.S. Timer Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/27 12:24:53  marpas
	qa rules
	
	Revision 2.2  2012/02/09 14:14:16  marpas
	refactoring
	
	Revision 2.1  2009/05/15 15:14:34  marpas
	rules fixing
	
	Revision 2.0  2006/02/28 08:28:40  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/05/06 08:39:53  marpas
	headers fixed
	
	Revision 1.2  2003/02/12 13:24:13  marpas
	versioning fixed
	
	Revision 1.1.1.1  2002/10/04 16:26:05  paoscu
	Import libTimer
	

*/

 
#ifndef _tV_H_
#define _tV_H_

namespace acs {

// support class for versioning

class tV {  // PRQA S 2109
public:
	tV() throw() ;
	~tV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not defined to prevent their use
	tV(const tV & ) ;
	tV &operator=(const tV & ) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tV_H_



