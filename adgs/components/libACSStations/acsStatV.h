// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libACSStations Library - HEADER FILE $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2014/02/13 15:04:54  giucas
	New log macros adopted.
	
	Revision 1.2  2009/07/22 10:17:38  marpas
	c++ rules applied
	
	Revision 1.1.1.1  2008/05/22 12:34:01  giucas
	Moved ACSStationsWrapper and python wrapper from libSgiIncludes
	


*/

 
#ifndef _acsStatV_H_
#define _acsStatV_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

// support class for versioning

class acsStatV { // PRQA S 2109
public:
	acsStatV() ;
	~acsStatV() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	//declared but not implemented to prevent their use
	acsStatV(const acsStatV &) ;
	acsStatV& operator=(const acsStatV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _acsStatV_H_



