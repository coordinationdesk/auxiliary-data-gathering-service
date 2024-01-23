/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  A.C.S. WS Config Merger $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 17:57:53  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2004/03/12 09:16:50  marfav
	Import WS Config Merger
	
	


*/

#ifndef _wscmV_H_
#define _wscmV_H_

namespace acs {

//! \brief support class for versioning




class wscmV {
public:
	wscmV() throw() ;
	~wscmV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _wscmV_H_
