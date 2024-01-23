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
#include <Uuid.h>
#include <exException.h>
#include <iostream>

_ACS_BEGIN_NAMESPACE(acs)

Uuid::Uuid() 
{
	uuid_generate(_uuid);
}

std::string Uuid::getUuid() const
{
	return Uuid(_uuid).toString();
}

Uuid::Uuid(const Uuid &r) 
{
	uuid_copy(_uuid, r._uuid); 
}

Uuid & Uuid::operator=(const Uuid &r) 
{
    if (this != &r) {
	    uuid_copy(_uuid, r._uuid); 
    }
    return *this ;
}

Uuid::Uuid(const uuid_t other_uuid) 
{ 
	uuid_copy(_uuid, other_uuid); 
}

std::string Uuid::generateInplace() 
{ 
	uuid_generate(_uuid);
	return Uuid(_uuid).toString();
}

std::string Uuid::generateUuid() 
{
	Uuid wrapped; 
	uuid_generate(wrapped._uuid); 
	return wrapped.toString(); 
}
std::string Uuid::toString() const
{
	std::array<char, 37> uuid ;
	uuid_unparse(_uuid, uuid.data());
	
	return uuid.data() ;
}


Uuid Uuid::fromString(const char *uuid)
{
	uuid_parse(uuid, _uuid);
	return Uuid(_uuid);
}

bool Uuid::operator==(const Uuid &other) const
{
	return uuid_compare(_uuid, other._uuid) == 0;
}

_ACS_END_NAMESPACE
