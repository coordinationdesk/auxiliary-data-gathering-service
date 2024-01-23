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

	$Prod: A.C.S. Safe Prodper base library $

	$Id$

	$Author$
	
	$Log$
	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
	First implementation
	
	

*/

#ifndef _l0SafeProdBaseV_H_
#define _l0SafeProdBaseV_H_

namespace acs {

// support class for versioning

class l0SafeProdBaseV { // PRQA S 2109
public:
	l0SafeProdBaseV() ;
	~l0SafeProdBaseV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
    l0SafeProdBaseV(const l0SafeProdBaseV &) ; // not implemented
    l0SafeProdBaseV& operator= (const l0SafeProdBaseV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _l0SafeProdBaseV_H_

