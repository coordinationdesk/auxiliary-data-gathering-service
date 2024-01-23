/*
	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	A.C.S. libFile
*/
#ifndef _Uuid_H_
#define _Uuid_H_
#include <acs_c++config.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

/**
	Wrapper of libuuid
*/
class Uuid {
public:
	Uuid();
    ~Uuid() = default ;
	Uuid(const Uuid &);
	Uuid& operator=(const Uuid &);
	explicit Uuid(const uuid_t );

	std::string generateInplace();
	static std::string generateUuid();

	// convert uuid into string
	std::string toString() const ;
	std::string getUuid() const ;

	// convert from string to Uuid
	Uuid fromString(const char *);
	bool operator==(const Uuid &) const ;
private:
	uuid_t _uuid;

};
_ACS_END_NAMESPACE

#endif //_Uuid_H_
