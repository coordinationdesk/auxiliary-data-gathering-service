/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2007/02/27 16:52:14  marqua
	version files
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
	Import libDateTime
	

*/

#ifndef _pyepfnV_H_
#define _pyepfnV_H_

namespace acs {

// support class for versioning

class pyepfnV {
public:
	pyepfnV() ;
	~pyepfnV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _pyepfnV_H_



