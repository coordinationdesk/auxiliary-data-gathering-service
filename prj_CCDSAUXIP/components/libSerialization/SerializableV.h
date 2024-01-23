/*
 
    Copyright 2018-2019, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libSerialization -- version file $

    $Log$

*/

#ifndef _SerializableV_H_
#define _SerializableV_H_

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class SerializableV { // PRQA S 2109
public:
	SerializableV() ACS_NOEXCEPT ;
	~SerializableV() ACS_NOEXCEPT ;
	static const char* tag() ACS_NOEXCEPT ;
	static const char* name() ACS_NOEXCEPT ;
private:
	SerializableV(const SerializableV&) ; // not implemented
	SerializableV & operator=(const SerializableV&) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _SerializableV_H_

