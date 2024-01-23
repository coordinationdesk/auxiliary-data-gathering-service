// PRQA S 1050 EOF
/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/29 19:31:30  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/13 10:59:05  marpas
	refactoring in progress
	
	Revision 2.1  2009/03/13 15:26:45  marpas
	some rule violations fixed
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/30 22:06:41  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/10/04 16:58:31  paoscu
	Import libFile
	
		

*/
 
#ifndef _lfV_H_
#define _lfV_H_

namespace acs {

// support class for versioning

class lfV { // PRQA S 2109
public:
	lfV() throw() ;
	~lfV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	lfV(const lfV &) ;
	lfV &operator=(const lfV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lfV_H_



