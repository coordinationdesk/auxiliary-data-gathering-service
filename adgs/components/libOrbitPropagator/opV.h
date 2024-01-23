// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

    $Log$
*/

#ifndef _opV_H_
#define _opV_H_

#include <acs_c++config.hpp>
_ACS_BEGIN_NAMESPACE(acs)

class opV { // PRQA S 2109
public:
	opV() ;
	~opV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	opV(const opV &) ; // not implemented
	opV& operator= (const opV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _opV_H_

