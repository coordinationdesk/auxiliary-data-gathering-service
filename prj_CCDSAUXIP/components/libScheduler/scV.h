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

	$Prod: A.C.S. Scheduler Interface Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.2  2003/05/05 10:20:43  marfav
	Added Log Field to Headers
	

*/

#ifndef _scV_H_
#define _scV_H_

namespace acs {

//! \brief support class for versioning




class scV { // PRQA S 2109
public:
	scV() ;
	~scV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	scV(const scV &) ; // not implemented
	scV& operator= (const scV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _scV_H_
