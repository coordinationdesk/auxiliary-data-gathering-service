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

	$Prod: A.C.S. Scheduler Abstract Commands Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:57:14  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:31:01  marfav
	Import libSchedCommands
	

*/

#ifndef _scacV_H_
#define _scacV_H_

namespace acs {

//! \brief support class for versioning




class scacV { // PRQA S 2109
public:
	scacV() ;
	~scacV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
    scacV(const scacV &) ; // not implemented
    scacV& operator= (const scacV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _scacV_H_
