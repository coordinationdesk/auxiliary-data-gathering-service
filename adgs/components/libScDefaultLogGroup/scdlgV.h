/*

	Copyright 1995-2004, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Scheduler Default Log Plugin library $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2004/09/14 16:36:52  marfav
	Import libScDefaultLogGroup
	

*/

#ifndef _scdlgV_H_
#define _scdlgV_H_

namespace acs {

// support class for versioning

class scdlgV {
public:
	scdlgV() ;
	~scdlgV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _scdlgV_H_



