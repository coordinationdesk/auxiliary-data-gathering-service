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


*/

#ifndef _ProdTypeDescrV_H_
#define _ProdTypeDescrV_H_

namespace acs {

// support class for versioning

class ProdTypeDescrV { // PRQA S 2109
public:
	ProdTypeDescrV() ;
	~ProdTypeDescrV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
    ProdTypeDescrV(const ProdTypeDescrV &) ; // not implemented
    ProdTypeDescrV& operator= (const ProdTypeDescrV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ProdTypeDescrV_H_

