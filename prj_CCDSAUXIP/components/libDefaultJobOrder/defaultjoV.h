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

	$Prod: A.C.S. Envisat JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/10/17 13:11:09  chicas
	imported sources
	
*/

#ifndef _defaultjoV_H_
#define _defaultjoV_H_

namespace acs {

//! \brief support class for versioning




class defaultjoV { // PRQA S 2109
public:
	defaultjoV() ;
	~defaultjoV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
    defaultjoV(const defaultjoV &) ; // not implemented
    defaultjoV& operator= (const defaultjoV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _defaultjoV_H_
