// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: OAuth2 Utils library $


*/

#include <auto_version_file>
#include <libOAuth2UtV.h>

using namespace acs;

const char* libOAuth2UtV::_name = auto_version_file::name;
const char* libOAuth2UtV::_tag = auto_version_file::tag;

libOAuth2UtV::libOAuth2UtV() ACS_NOEXCEPT {}
libOAuth2UtV::~libOAuth2UtV() ACS_NOEXCEPT {}

const char* libOAuth2UtV::tag()
{
  return _tag;
}
const char* libOAuth2UtV::name()
{
  return _name;
}
