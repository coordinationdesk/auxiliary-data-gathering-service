// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	
        
*/

#ifndef _EOFFilenamePreInventoryV_H_
#define _EOFFilenamePreInventoryV_H_

namespace acs {

//! \brief support class for versioning

class EOFFilenamePreInventoryV { // PRQA S 2109
public:
	EOFFilenamePreInventoryV() ;
	~EOFFilenamePreInventoryV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	EOFFilenamePreInventoryV(const EOFFilenamePreInventoryV &) ; // not implmented
	EOFFilenamePreInventoryV & operator = (const EOFFilenamePreInventoryV &) ; // not implmented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif 


