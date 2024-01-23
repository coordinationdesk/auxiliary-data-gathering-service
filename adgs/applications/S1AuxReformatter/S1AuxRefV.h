/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: Sentinel1  Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$

*/

#ifndef _S1AuxRefV_H_
#define _S1AuxRefV_H_

namespace acs {

// support class for versioning

class S1AuxRefV {
public:
	S1AuxRefV() throw() ;
	~S1AuxRefV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _S1AuxRefV_H_

