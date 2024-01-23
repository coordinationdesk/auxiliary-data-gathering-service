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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/07/11 18:31:10  marpas
	work in progress
	
	Revision 1.2  2012/02/09 16:30:19  marpas
	minor changes
	
	Revision 1.1.1.1  2011/05/31 13:51:16  crivig
	imported new library
	
*/
#ifndef _lCSoapV_H_
#define _lCSoapV_H_

namespace acs {

// support class for versioning

class lCSoapV { // PRQA S 2109
public:
	lCSoapV() throw() ;
	~lCSoapV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	lCSoapV(const lCSoapV &) ; // not implemented
	lCSoapV &operator=(const lCSoapV &) ;// not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lCSoapV:_H_


