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
#include <Uuid.h>
#include <exException.h>
#include <iostream>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

Uuid::Uuid() : _uuid()
{
	uuid_generate(_uuid);
}

std::string Uuid::getUuid()
{
	return Uuid(_uuid).toString();
}

Uuid::Uuid(const Uuid &other) 
{
	uuid_copy(_uuid, other._uuid); 
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
std::string Uuid::toString()
{
	char *uuid = new char[37];
	uuid_unparse(_uuid, uuid);
	
	return string(uuid);
}
Uuid Uuid::fromString(char *uuid)
{
	uuid_parse(uuid, _uuid);
	return Uuid(_uuid);
}

// bool Uuid::operator<(const Uuid &other) 
// { 
// 	return uuid_compare(_uuid, other._uuid) < 0; 
// }

bool Uuid::operator==(const Uuid &other)
{
	return uuid_compare(_uuid, other._uuid) == 0;
}

_ACS_END_NAMESPACE
