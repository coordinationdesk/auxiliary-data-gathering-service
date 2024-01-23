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
	Revision 1.3  2013/01/24 19:21:29  marpas
	interface rationalization
	control flow simplified for easy maintenance
	coding best practices
	qa rules
	
	Revision 1.2  2012/02/09 15:12:45  marpas
	refactoring
	
	Revision 1.1.1.1  2007/03/15 15:01:03  crivig
	Imported source
	
*/

#ifndef _dgV_H_
#define _dgV_H_
#include <acs_c++config.hpp>


_ACS_BEGIN_NAMESPACE(acs)

// support class for versioning

class dgV { // PRQA S 2109
public:
	dgV() throw() ;
	~dgV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	dgV(const dgV &) throw() ; // not implemented
	dgV & operator= (const dgV &) throw() ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _dgV_H_

