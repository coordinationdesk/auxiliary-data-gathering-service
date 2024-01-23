// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Exprivia SPA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	$Prod: CRC Library $
	* 
*/

#ifndef _PasswordHash_H_
#define _PasswordHash_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <Filterables.h>
#include <cstring>

/*
 * All static helper class to manage
 * password hash computations
 */

_ACS_BEGIN_NAMESPACE(acs)

class PasswordHash
{
	public:
		static std::string md5crypt(const std::string &key, const std::string &pass=std::string("")) ACS_NOEXCEPT ;
		static std::string crypt(const std::string &) ACS_NOEXCEPT ;
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PasswordHash);

};

_ACS_END_NAMESPACE

#endif //_PasswordHash_H_
