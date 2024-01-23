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

	$Prod: A.C.S. ThinLayer WS Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:10:54  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:05:07  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/01/31 16:03:27  marfav
	Import libWSConfig
	
	
	

*/

#ifndef _tlwscV_H_
#define _tlwscV_H_

namespace acs {

//! \brief support class for versioning




class tlwscV { // PRQA S 2109
public:
	tlwscV() ;
	~tlwscV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	tlwscV(const tlwscV &) ; // not implemented 
	tlwscV & operator= (const tlwscV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tlwscV_H_
