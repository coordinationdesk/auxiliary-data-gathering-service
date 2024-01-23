// PRQA S 1050 EOF
/*

	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

*/

#ifndef _ObjSmV_H_
#define _ObjSmV_H_

namespace acs {

// support class for versioning

class ObjSmV { // PRQA S 2109
public:
	ObjSmV() throw() ;
	~ObjSmV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
    ObjSmV(const ObjSmV &) ; // not implemented
    ObjSmV& operator= (const ObjSmV &) ; // not implemented
    
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ObjSmV_H_

