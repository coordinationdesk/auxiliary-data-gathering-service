// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
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

#ifndef _InventoryV_H_
#define _InventoryV_H_

namespace acs {

// support class for versioning

class InventoryV { // PRQA S 2109 
public:
	InventoryV() throw() ;
	~InventoryV() throw();
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
    InventoryV(const InventoryV &) ; // not implemented
    InventoryV& operator= (const InventoryV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _InventoryV_H_

