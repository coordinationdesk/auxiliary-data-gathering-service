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

#ifndef _libOAuth2UtV_H_
#define _libOAuth2UtV_H_

#include <acs_c++pers.hpp>

namespace acs
{
// support class for versioning

class libOAuth2UtV
{  // PRQA S 2109
public:
  libOAuth2UtV() ACS_NOEXCEPT;
  ~libOAuth2UtV() ACS_NOEXCEPT;
  static const char* tag();
  static const char* name();

private:
  libOAuth2UtV(const libOAuth2UtV&);
  libOAuth2UtV& operator=(const libOAuth2UtV&);

private:
  static const char* _tag;
  static const char* _name;
};

}  // namespace acs

#endif  // _libOAuth2UtV_H_
