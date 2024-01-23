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

	$Prod: A.C.S. StatBro Library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.1  2012/03/08 13:28:01  marpas
	robustness and messages improved
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/06 14:24:07  marpas
	headers fixed
	
	Revision 1.1.1.1  2003/02/14 10:16:00  marpas
	Import libStatBro
	
	

*/

 
#ifndef _sbV_H_
#define _sbV_H_

namespace acs {

// support class for versioning

class sbV { // PRQA S 2109
public:
	sbV() throw() ;
	~sbV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	sbV(const sbV &) ; // not implemented
	sbV& operator= (const sbV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _sbV_H_


