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
 *  Encrypter utility
 *
 *
 *
 */

#include <EncrypterVersion.h>
#include <RemoteConnections_RSAPrivateKey.h>
#include <RemoteConnections_RSAPublicKey.h>
#include <ACS_SSL.h>
#include <Timer.h>

#include <iostream>
#include <iterator>
#include <cstring>
#include <errno.h>

#ifdef USE_DBCONNECTION_EXTERNAL_SECRET
#include <DbConnectionSecret.h>
#endif

using namespace acs;
using namespace std;

static EncrypterVersion version;


class Encrypter {
public:
	Encrypter() :
		blowfish_(false),
		randomsaltsize_(0),
		saltstring_(),
		blowfishkey_()
	{ }
	
	virtual ~Encrypter() throw() { }
	
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
	
	std::string rsaEncrypt(const std::string& password) {
		ACS_SSL::setSalt(randomsaltsize_, saltstring_);
		
		RSA* privkey = ACS_SSL::ACS_RSA_newkey();
		ACS_SSL::RSAKeyGuard g2(privkey);

		ACS_SSL::ACS_RSA_fill_PrivKey(privkey, ACS_RemoteConnections_PrivKey_n, ACS_RemoteConnections_PrivKey_e,
				ACS_RemoteConnections_PrivKey_d, ACS_RemoteConnections_PrivKey_p, ACS_RemoteConnections_PrivKey_q,
				ACS_RemoteConnections_PrivKey_dmp1, ACS_RemoteConnections_PrivKey_dmq1, ACS_RemoteConnections_PrivKey_iqmp);

		size_t encrypted_password_size(32*1024) ; // 32 KB -- enough to store a password ...
		unsigned char encrypted_password[ encrypted_password_size ] ;
		memset( encrypted_password, 0x00, sizeof(encrypted_password) ) ; 

		// The following check is not exact -- the size of input and output buffer are compared.
		// The point is that the size of the allocated buffer SHOULD BE FAR LARGER (e.g. 10 times) THAN ANY PRACTICAL USE !
		ACS_COND_THROW ( (password.length() >= encrypted_password_size), exIllegalValueException ("Internal error, input buffer larger than expected") ) ;

		// The ACS_RSA_encode_password will update encrypted_password_size with the actual value
		const bool success ( 
			ACS_SSL::ACS_RSA_encode_password( reinterpret_cast<unsigned char const*>(password.c_str()), password.length(), privkey, encrypted_password, encrypted_password_size) \
		) ; 

		ACS_COND_THROW ( (false == success), exIllegalValueException ("Error while encrypting password " + password) ) ;
		ACS_COND_THROW ( (encrypted_password_size > sizeof(encrypted_password) ), exIllegalValueException ("Internal error, output buffer overwflow") ) ;

		vector<char> base64_encrypt ;
		
		size_t base64_encrypt_length = ACS_SSL::ACS_BASE64_encode_buffersize(encrypted_password_size);

		ACS_COND_THROW(base64_encrypt_length == 0, exIllegalValueException("Cannot compute the base64 length.")) ; // PRQA S 3081

		base64_encrypt = vector<char>(base64_encrypt_length) ; // PRQA S 3081
		base64_encrypt_length = ACS_SSL::ACS_BASE64_encode(encrypted_password, &base64_encrypt[0], encrypted_password_size);
		base64_encrypt.resize(base64_encrypt_length);

		stringstream os ;
		ostream_iterator<char> osit (os);
		std::copy (base64_encrypt.begin(), base64_encrypt.end(), osit);

		return os.str();

	}
	
	std::string blowfishEncrypt(const std::string& password) {
		BF_KEY bfkey;
		char encPassword [1024];
		unsigned char salt_buffer[4096];
		vector<void*> vin;
		vector<void*> vout;
		
		memset(encPassword, 0, sizeof(encPassword)); 
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
		
		size_t s = ACS_SSL::ACS_ALGO_encode(algo, reinterpret_cast<const unsigned char*>(password.c_str()), password.size(),  reinterpret_cast<unsigned char*>(encPassword), vin, vout);
		
		std::string ret(encPassword, s);
		return ret;
	}

	std::string encrypt(const std::string& password) {
		ACS_SSL::setSalt(randomsaltsize_, saltstring_);
		
		std::string encrypted;
		if (!blowfish_) {
			encrypted = rsaEncrypt(password);
		}
		else {
			encrypted = blowfishEncrypt(password);
		}
		
		return encrypted;
	}

private:
	Encrypter(const Encrypter&);
	Encrypter& operator=(const Encrypter&);

private:
	bool blowfish_;
	size_t randomsaltsize_;
	std::string saltstring_;
	std::string blowfishkey_;
};

void usage() {
	std::cout << "./Encrypter [-h|--help] [-b|--blowfish] [-c|--ciphersalt salt] [-s|--randomsize rndsaltsize]\n";
	std::cout << "   -h|--help:       Prints this usage\n";
	std::cout << "   -b|--blowfish:   Use blowfish algorithm\n";
	std::cout << "   -c|--ciphersalt: The salt to be used. It is mutually exclusive with -s|--randomsize\n";
	std::cout << "   -s|--randomsize: The size of the randomic salt. It is mutually exclusive with -c|--ciphersalt\n";
}

int main(int argc, char **argv) {
	std::string pwd; 
	Encrypter en;

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
				cout << en.encrypt(pwd) << "\n";
			} catch (std::exception& e) {
				cout << "The following error occured during encryption: " << e.what() << "\n";
				exitcode = 255;
			}
			exitcode = 0;
		} else {
			cout << "Input argument errors: No plaintext provided.\nUse --help for usage.\n";
			exitcode = 255;
		}
	}
	
	return exitcode;
}

