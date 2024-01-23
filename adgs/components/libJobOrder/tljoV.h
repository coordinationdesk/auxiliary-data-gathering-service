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

	$Prod: A.C.S. ThinLayer JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2012/02/17 13:40:55  marpas
	refactoring
	
	Revision 2.0  2006/02/28 10:11:29  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:47:54  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/01/31 15:00:48  marfav
	Import libJobOrder
	
	

*/

#ifndef _tljoV_H_
#define _tljoV_H_

namespace acs {

//! \brief support class for versioning




class tljoV { // PRQA S 2109
public:
	tljoV() throw() ;
	~tljoV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	tljoV(const tljoV &) ; // not implemented
	tljoV& operator= (const tljoV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tljoV_H_
