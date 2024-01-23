// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.2  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/

#ifndef _tldV_H_
#define _tldV_H_

namespace acs {

//! \brief support class for versioning


class tldV { // PRQA S 2109
public:
	tldV() throw() ;
	~tldV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	tldV(const tldV &) ; // not implemented
	tldV& operator=(const tldV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tldV_H_
