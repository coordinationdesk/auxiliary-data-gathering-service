// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Order Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:42  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/01/30 15:17:07  danalt
	Import libTlOrder
	

*/

#ifndef _tloV_H_
#define _tloV_H_

namespace acs {

//! \brief support class for versioning


class tloV { // PRQA S 2109
public:
	tloV() ;
	~tloV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	tloV(const tloV &) ; // not implemented
	tloV & operator= (const tloV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tloV_H_
