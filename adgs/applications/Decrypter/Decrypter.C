/*
 *
 *  Copyright 1995-2011, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Crypto utility$
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 1.3  2012/02/22 13:41:54  micmaz
 *  reworked utilities.
 *
 *  Revision 1.2  2011/06/23 15:07:07  micmaz
 *  work in progress.
 *
 *  Revision 1.1.1.1  2011/06/23 13:43:16  micmaz
 *  Decrypter utility
 *
 *
 *
 */

#include <DecrypterV.h>
#include <RemoteConnections_RSAPrivateKey.h>
#include <RemoteConnections_RSAPublicKey.h>
#include <ACS_SSL.h>

#include <iostream>
#include <iterator>
#include <cstring>

#ifdef USE_DBCONNECTION_EXTERNAL_SECRET
#include <DbConnectionSecret.h>
#endif

using namespace acs;
using namespace std;

static DecrypterV version;


class Decrypter {
public:
	Decrypter() :
		blowfish_(false),
		randomsaltsize_(0),
		saltstring_(),
		blowfishkey_()
	{ }
	
	virtual ~Decrypter() throw() { }
	
	void blowfish(bool v) {
		blowfish_ = v;
	}
	
	void blowfishKey(const std::string& bfkey) {
		blowfishkey_ = bfkey;
	}
	
	void randomSaltSize(size_t v) {
		randomsaltsize_ = v;
	}
	
	void saltString(const std::string& str) {
		saltstring_ = str;
	}
	
	std::string rsaDecrypt(const std::string& password) {
		ACS_SSL::setSalt(randomsaltsize_, saltstring_);
		
		vector<unsigned char> base64_encrypt(password.length()) ;

		size_t base64_decrypt_length = ACS_SSL::ACS_BASE64_decode(password.c_str(), &base64_encrypt[0], password.length()) ;
		base64_encrypt.resize(base64_decrypt_length) ;

		RSA* pubkey = ACS_SSL::ACS_RSA_newkey();
		ACS_COND_THROW(0 == pubkey, exIllegalValueException("Can't create a new key")); // PRQA S 3081
		ACS_SSL::RSAKeyGuard g1(pubkey);

		ACS_SSL::ACS_RSA_fill_PubKey(pubkey, ACS_RemoteConnections_PubKey_n, ACS_RemoteConnections_PubKey_e);

		size_t decrypted_password_size(32*1024) ; // 32 KB -- enough to store a password ...
		unsigned char decrypted_password[ decrypted_password_size ] ;
		memset( decrypted_password, 0x00, sizeof(decrypted_password) ) ; 
		
		// The following check is not exact -- the size of input and output buffer are compared.
		// The point is that the size of the allocated buffer SHOULD BE FAR LARGER (e.g. 10 times) THAN ANY PRACTIAL USE !
		ACS_COND_THROW ( (base64_encrypt.size() > decrypted_password_size), exIllegalValueException ("Internal error, input buffer larger than expected") ) ;

		// The ACS_RSA_decode_password will update decrypted_password_size with the actual value
		const bool success = ACS_SSL::ACS_RSA_decode_password( reinterpret_cast<unsigned char const*>(&base64_encrypt[0]), base64_encrypt.size(), pubkey, decrypted_password, decrypted_password_size); 

		ACS_COND_THROW ( (false == success), exIllegalValueException ("Error while decrypting password " + password) ) ;
		ACS_COND_THROW ( (decrypted_password_size > sizeof(decrypted_password) ), exIllegalValueException ("Internal error, output buffer overwflow") ) ;

		return ( string( reinterpret_cast<char*>(decrypted_password), decrypted_password_size) ) ; 
	}
	
	std::string blowfishDecrypt(const std::string& password) {
		BF_KEY bfkey;
		char decPassword [1024];
		unsigned char salt_buffer[4096];
		vector<void*> vin;
		vector<void*> vout;
		
		memset(decPassword, 0, sizeof(decPassword)); 
		memset(salt_buffer, 0, sizeof(salt_buffer)); 
		
		ACS_SSL::ACS_BLOWFISH_setkey(reinterpret_cast<const unsigned char*>(blowfishkey_.c_str()), blowfishkey_.size(), &bfkey);	/* Get Actual key */
		vin.push_back(&bfkey);

		size_t saltsize = (randomsaltsize_ != 0) ? (randomsaltsize_) : (saltstring_.size());

		size_t salt_buffer_size = ACS_SSL::encode_salt(saltsize); 
		ACS_SSL::encode_salt_buffer(reinterpret_cast<const unsigned char*>(saltstring_.c_str()), saltstring_.length(), saltsize, salt_buffer, salt_buffer_size); 
		vin.push_back(&(salt_buffer[0])) ;
		
		ACS_SSL::EncryptionAlgo algo;
		if (randomsaltsize_ != 0 || !saltstring_.empty()) {
			algo = ACS_SSL::EncryptionAlgo(ACS_SSL::ENC_BLOWFISH | ACS_SSL::ENC_PREPEND_SALT | ACS_SSL::ENC_BASE64);
		}
		else {
			algo = ACS_SSL::EncryptionAlgo(ACS_SSL::ENC_BLOWFISH | ACS_SSL::ENC_BASE64);
		}
		
		size_t s = ACS_SSL::ACS_ALGO_decode(algo, reinterpret_cast<const unsigned char*>(password.c_str()), password.size(),  reinterpret_cast<unsigned char*>(decPassword), vin, vout);
		
		std::string ret(decPassword, s);
		return ret;
	}

	std::string decrypt(const std::string& password) {
		ACS_SSL::setSalt(randomsaltsize_, saltstring_);
		
		std::string decrypted;
		if (!blowfish_) {
			decrypted = rsaDecrypt(password);
		}
		else {
			decrypted = blowfishDecrypt(password);
		}
		
		return decrypted;
	}

private:
	Decrypter(const Decrypter&);
	Decrypter& operator=(const Decrypter&);

private:
	bool blowfish_;
	size_t randomsaltsize_;
	std::string saltstring_;
	std::string blowfishkey_;
};

void usage() {
	std::cout << "./Decrypter [-h|--help] [-b|--blowfish] [-c|--ciphersalt salt] [-s|--randomsize rndsaltsize]\n";
	std::cout << "   -h|--help:       Prints this usage\n";
	std::cout << "   -b|--blowfish:   Use blowfish algorithm\n";
	std::cout << "   -c|--ciphersalt: The salt to be used. It is mutually exclusive with -s|--randomsize\n";
	std::cout << "   -s|--randomsize: The size of the randomic salt. It is mutually exclusive with -c|--ciphersalt\n";
}

int main(int argc, char **argv) {
	std::string pwd; 
	Decrypter en;

	int exitcode = 0;

	if (argc < 2) {
		usage();
		return exitcode;
	}

	std::string salt = "";
	size_t rndsize = 0;
	for (int idx = 1; idx < argc; ++idx) {
		std::string opt = argv[idx];
		if (opt == "-h" || opt == "--help") {
			usage();
			return exitcode;
		}

		if (opt == "-b" || opt == "--blowfish") {
			en.blowfish(true);
		}

		else if (opt == "-c" || opt == "--ciphersalt") {
			salt = argv[++idx];
		}

		else if (opt == "-s" || opt == "--randomsize") {
			rndsize = atol(argv[++idx]);
		}

		else {
			pwd = opt;
		}
	}
	
#ifdef USE_DBCONNECTION_EXTERNAL_SECRET
	DB_CONNECTION_KEY_IN_DEFINITION
#else
	const char key_in[] = { 'm', 'a', 'n', 'u', 'e', 'l' } ;	/* Blowfish passphrase. Provided by RobRic */
#endif
	std::string blowfishkey = std::string(reinterpret_cast<const char*>(key_in), sizeof(key_in));
	
	en.blowfishKey(blowfishkey);
	en.saltString(salt);
	en.randomSaltSize(rndsize);

	if (!salt.empty() && rndsize != 0) {
		cout << "Input argument errors: ciphersalt and randomsize cannot be defined together. Please specify only one of this two options.\n";
		usage();
		exitcode = 255;
	} 
	else {
		if (!pwd.empty()) {
			//Timer::randomDelay(1000, 500);
			try {
				cout << en.decrypt(pwd) << "\n";
			} catch (std::exception& e) {
				cout << "The following error occured during decryption: " << e.what() << "\n";
				exitcode = 255;
			}
			exitcode = 0;
		} else {
			cout << "Input argument errors: No ciphertext provided.\nUse --help for usage.\n";
			exitcode = 255;
		}
	}
	
	return exitcode;
}
