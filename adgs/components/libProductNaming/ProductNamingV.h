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

	$Prod: A.C.S. libProductNaming$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2010/04/12 10:02:06  marant
	Import libProductNaming
	
        
*/

#ifndef _ProductNamingV_H_
#define _ProductNamingV_H_

namespace acs {

//! \brief support class for versioning

class ProductNamingV { // PRQA S 2109
public:
	ProductNamingV() ;
	~ProductNamingV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	ProductNamingV(const ProductNamingV &) ; // not implemented
	ProductNamingV& operator= (const ProductNamingV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif 


