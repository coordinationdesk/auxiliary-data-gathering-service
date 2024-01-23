/*
	Copyright 2020, Exprivia SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it


	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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
	Uuid(const Uuid &other);
	Uuid(const uuid_t other_uuid);

	std::string generateInplace();
	static std::string generateUuid();

	// convert uuid into string
	std::string toString();
	std::string getUuid();

	// convert from string to Uuid
	Uuid fromString(char *uuid);
//	bool operator<(const Uuid &other);
	bool operator==(const Uuid &other);
private:
	uuid_t _uuid;

  // ...
};
_ACS_END_NAMESPACE

#endif //_Uuid_H_
