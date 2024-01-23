/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
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
	Revision 1.1.1.1  2012/11/30 12:44:54  chicas
	Imported sources
	
	
        
*/

#ifndef _TTDecompressV_H_
#define _TTDecompressV_H_
#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class TTDecompressV { // PRQA S 2109
public:
	TTDecompressV() throw() ;
	~TTDecompressV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	TTDecompressV(const TTDecompressV &) ; // not implemented
	TTDecompressV & operator= (const TTDecompressV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _TTDecompressV_H_

