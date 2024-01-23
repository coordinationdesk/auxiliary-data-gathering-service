/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: CRC Library $

*/
#include <unistd.h>
#include <crypt.h>

#include <PasswordHash.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(PasswordHash);

using std::string ;

string PasswordHash::md5crypt(const string &key, const string &pass)  noexcept
{
	// Initialize data for reentrant version of crypt
	struct crypt_data cdata;
	cdata.initialized = 0;

	if (pass.empty())
	{
		unsigned long seed[2] ; // NOSONAR - fixed

		// generate a (not very) random seed. One could do better than this. 
		// IMHO I think this is enough for majority of user level applications
		seed[0] = ::time(nullptr) ;
		seed[1] = ::getpid() ^ (seed[0] >> 14 & 0x30000);

		// Turn it on printable chars from 'seedchars'.
		const char *const seedchars =
		  "./0123456789ABCDEFGHIJKLMNOPQRST"
		  "UVWXYZabcdefghijklmnopqrstuvwxyz" ;
		char salt[] = "$1$........" ; // NOSONAR - fixed array
		for (int i=0; i < 8; i++) {
			salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f] ;
		}
		return ::crypt_r(key.c_str(), salt, &cdata) ;
	}
	return ::crypt_r(key.c_str(), pass.c_str(), &cdata) ;
}

string PasswordHash::crypt(const string &tbc) noexcept
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "tbc = \"" <<tbc<<"\"") ;

	struct crypt_data cdata;
	cdata.initialized = 0;

	string ac ;
    size_t len=tbc.length()/8+1;
    for(size_t i=0;i<len;i++) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "crypting: \"" << tbc.c_str()[i*8] 
					<< " \" -> \"" << (::crypt(&tbc.c_str()[i*8], "12")+2) << "\"") ;
		ac += (::crypt_r(&tbc.c_str()[i*8], "12", &cdata)+2);
	}
	
	return ac ;
}


_ACS_END_NAMESPACE
