// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2014, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: A.C.S. poV header $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 */

 
#ifndef _poV_H_
#define _poV_H_

namespace acs {

// support class for versioning

class poV { // PRQA S 2109
public:
	poV() ;
	~poV() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	poV(const poV &) ; // not implemented
	poV &operator=(const poV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _poV_H_



