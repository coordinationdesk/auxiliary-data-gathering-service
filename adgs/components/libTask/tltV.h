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

	$Prod: A.C.S. ThinLayer Task Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:01:40  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/01/30 18:25:35  marfav
	Import libTask
	
	

*/

#ifndef _tltV_H_
#define _tltV_H_

namespace acs {

//! \brief support class for versioning




class tltV { // PRQA S 2109
public:
	tltV() throw() ;
	~tltV() throw() ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
	tltV(const tltV &) ; // not implemented
	tltV& operator= (const tltV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tltV_H_
