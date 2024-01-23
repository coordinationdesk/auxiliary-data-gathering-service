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

	$Prod: A.C.S. TL Scheduler Order Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:56:12  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/15 08:12:07  marfav
	Import libScOrder
	
	

*/

#ifndef _scoV_H_
#define _scoV_H_

namespace acs {

//! \brief support class for versioning




class scoV { // PRQA S 2109
public:
	scoV() ;
	~scoV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	scoV(const scoV &) ; // not implemented
	scoV& operator= (const scoV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _scoV_H_
