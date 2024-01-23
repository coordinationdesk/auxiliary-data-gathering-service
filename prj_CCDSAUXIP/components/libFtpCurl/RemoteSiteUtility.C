// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.16  2017/04/12 13:04:10  marfav
 *  Added configurable support to fast probing
 *  avoiding to "ls" the entire site but consuming two connections per probe
 *
 *  Revision 5.15  2017/04/11 16:31:06  marfav
 *  work in progress
 *
 *  Revision 5.14  2015/03/10 14:57:00  lucio.pulvirenti
 *  S2PDGS-984: probeSite: boolean with default value added to IF.
 *
 *  Revision 5.13  2015/03/06 16:45:56  lucio.pulvirenti
 *  S2PDGS-984: work in progress.
 *
 *  Revision 5.12  2014/10/30 10:32:32  marpas
 *  qa warnings removed
 *
 *  Revision 5.11  2014/10/10 10:49:01  lucio.pulvirenti
 *  S2PDGS-939: composeUrl method added to centralize url normalization w.r.t. body + filename.
 *
 *  Revision 5.10  2014/09/08 15:55:40  marfav
 *  Error messages improved when encode/decode errors
 *  Using ostream_iterators when converting passwords to ostringstreams
 *
 *  Revision 5.9  2014/07/03 10:00:24  marpas
 *  method signature rationalized
 *
 *  Revision 5.8  2014/06/20 14:52:34  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.7  2014/06/05 18:45:58  marpas
 *  getting rid of asserts
 *  interface robustness improved
 *  tests fixed
 *
 *  Revision 5.6  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.5  2014/03/12 11:05:28  marfav
 *  Exception condition in isDir was wrong
 *
 *  Revision 5.4  2014/02/14 13:54:32  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.3  2014/02/13 17:59:28  marpas
 *  Crypto class cleansed - btw a memory leak icrypting/decrypting passwords has been corrected
 *  test added
 *
 *  Revision 5.2  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.1  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2012/04/03 17:46:10  micmaz
 *  work in progress
 *
 *  Revision 2.2  2012/03/30 12:51:02  micmaz
 *  work in progress
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.2  2011/07/14 15:19:32  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.1  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *
 *
 *
 */

#include <RemoteSiteUtility.h>

#include <RemoteConnections_RSAPrivateKey.h>
#include <RemoteConnections_RSAPublicKey.h>
#include <Filterables.h>
#include <File.h>
#include <string>
#include <sstream>

#include <CurlConfigurer.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RemoteSiteUtility);

using std::vector ;
using std::string ;

bool RemoteSiteUtility::isDir(	RemoteSite const& site, long& http_response_code, \
								StopController const& stopController) 
{
	http_response_code = 0 ; 
    if (! site.supportsDirectory()) { // the protocols does not support directories
        return false ; 
    }
    
    BandPriority no_priority ;
    FtpCurl ftp(no_priority, site);
    string error ; 

	bool existsAndIsDir = false;
	if ( conf::CurlConfigurerSingleton::instance()->getFastProbeModeEnabled() )
	{
		bool isDir = false ;
		// Do not emit exception
		existsAndIsDir = ftp.existRemoteResource (isDir, stopController, false);
		if (false  == existsAndIsDir) { http_response_code = ftp.getHttpResponseCode() ; }

	    ACS_COND_THROW( false  == existsAndIsDir, exIOException("The remote resource is not reachable", EAGAIN));
		existsAndIsDir = ( existsAndIsDir && isDir) ;
	}
	else
	{
	    RemoteResourceType response = ftp.probeRemoteResource(error, stopController);
		if (response == 0) { http_response_code = ftp.getHttpResponseCode() ; }

	    ACS_COND_THROW(0 == response, exIOException("The remote resource is not reachable", EAGAIN));
		existsAndIsDir = (response == RRTIsDir);
	}
    return existsAndIsDir;
}

RemoteResourceType RemoteSiteUtility::probeSite(RemoteSite* site, 
                                                StopController const& stopController,
												bool emitExc) 
{
    BandPriority no_priority ;
    FtpCurl curl(no_priority, *site);
    string error ; 
	RemoteResourceType rr = RRTNotExist ;

	if ( conf::CurlConfigurerSingleton::instance()->getFastProbeModeEnabled() )
	{
		// Emit exceptions if needed
		bool isDir = false ;
		bool exist = false ;
		exist = curl.existRemoteResource (isDir, stopController, emitExc);
		if  ( ! exist)
		{
			rr = RRTNotExist;
		}
		else if (isDir)
		{
			rr = RRTIsDir;
		}
		else
		{
			rr = RRTIsFile;
		}
	
	}
	else
	{
		rr = curl.probeRemoteResource(error, stopController, emitExc);
	}

	site->setIsDir(rr== RRTIsDir);
    return rr;
}

string RemoteSiteUtility::encrypt(string const& password) 
{
    try {

    	RSA* privkey = ACS_SSL::ACS_RSA_newkey();
    	ACS_SSL::RSAKeyGuard g2(privkey);

    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Fill in empty keys with RSA private and public key included in the header file.");

    	ACS_SSL::ACS_RSA_fill_PrivKey(privkey, ACS_RemoteConnections_PrivKey_n, ACS_RemoteConnections_PrivKey_e,
            	ACS_RemoteConnections_PrivKey_d, ACS_RemoteConnections_PrivKey_p, ACS_RemoteConnections_PrivKey_q,
            	ACS_RemoteConnections_PrivKey_dmp1, ACS_RemoteConnections_PrivKey_dmq1, ACS_RemoteConnections_PrivKey_iqmp);

		std::array<unsigned char,  32*1024> encrypted_password ; // 32 KB -- enough to store a password ...
        encrypted_password.fill(0) ;
		size_t encrypted_password_size = encrypted_password.size() ; 

		// The following check is not exact -- the size of input and output buffer are compared.
		// The point is that the size of the allocated buffer SHOULD BE FAR LARGER (e.g. 10 times) THAN ANY PRACTICAL USE !
		ACS_COND_THROW ( password.length() >= encrypted_password.size(), CryptException ("Internal error, input buffer larger than expected") ) ;

		// The ACS_RSA_encode_password will update encrypted_password_size with the actual value
		const bool success = 
			ACS_SSL::ACS_RSA_encode_password( reinterpret_cast<unsigned char const*>(password.c_str()), // NOSONAR 
                                              password.length(), 
                                              privkey, 
                                              encrypted_password.data(), 
                                              encrypted_password_size) ; 

		ACS_COND_THROW ( !success, CryptException ("Error while encrypting password *******") ) ;
		ACS_COND_THROW ( encrypted_password_size > encrypted_password.size(), CryptException ("Internal error, output buffer overwflow") ) ;

        vector<char> base64_encrypt ;
        encodeBASE64(encrypted_password.data(), encrypted_password_size, base64_encrypt); // NOSONAR - conversion to smaller type

        std::ostringstream os ;
        std::ostream_iterator<char> osit (os);
        std::copy (base64_encrypt.begin(), base64_encrypt.end(), osit);

        return os.str();
    }
    catch(const std::exception& ex) {
		ACS_THROW (CryptException (ex, "Error while crypting password *******"));
    }
}


string RemoteSiteUtility::decrypt(string const& encrypted_password) 
{
    try {
        vector<unsigned char> base64_encrypt(encrypted_password.length()) ;
        decodeBASE64(encrypted_password.c_str(), encrypted_password.length(), base64_encrypt);

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Create empty public key.");
    	RSA* pubkey = ACS_SSL::ACS_RSA_newkey();
    	ACS_COND_THROW(0 == pubkey, CryptException("Can't create a new key")); // PRQA S 3081
    	ACS_SSL::RSAKeyGuard g1(pubkey);

    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Fill in empty keys with RSA private and public key included in the header file.");
    	ACS_SSL::ACS_RSA_fill_PubKey(pubkey, ACS_RemoteConnections_PubKey_n, ACS_RemoteConnections_PubKey_e);

		std::array<unsigned char, 32*1024> decrypted_password ; // 32 KB -- enough to store a password ...
        decrypted_password.fill(0) ;
		size_t decrypted_password_size = decrypted_password.size() ; 
		
		// The following check is not exact -- the size of input and output buffer are compared.
		// The point is that the size of the allocated buffer SHOULD BE FAR LARGER (e.g. 10 times) THAN ANY PRACTIAL USE !
		ACS_COND_THROW ( base64_encrypt.size() > decrypted_password_size, CryptException ("Internal error, input buffer larger than expected") ) ;

		// The ACS_RSA_decode_password will update decrypted_password_size with the actual value
		const bool success = 
			ACS_SSL::ACS_RSA_decode_password( static_cast<unsigned char const*>(&base64_encrypt[0]), 
                                              base64_encrypt.size(), 
                                              pubkey, 
                                              decrypted_password.data(), 
                                              decrypted_password_size) ; 

		ACS_COND_THROW ( !success, CryptException ("Error while decrypting password *******") ) ;
		ACS_COND_THROW ( decrypted_password_size > decrypted_password.size() , CryptException ("Internal error, output buffer overwflow") ) ;

		return ( string( reinterpret_cast<char*>(decrypted_password.data()), decrypted_password_size) ) ; // NOSONAR - 

    }
    catch(const std::exception& ex) { 
	    ACS_THROW (CryptException (ex, "Error while decrypting password *******"));
    }
}


void RemoteSiteUtility::encodeBASE64(unsigned const char* binary_string, 
                                     unsigned int binary_string_length, 
                                     vector<char> &base64_encrypt) 
{
    size_t base64_encrypt_length = ACS_SSL::ACS_BASE64_encode_buffersize(binary_string_length);
    
    ACS_COND_THROW(base64_encrypt_length == 0, CryptException("Cannot compute the base64 length.")) ; // PRQA S 3081
    
    base64_encrypt = vector<char>(base64_encrypt_length) ; // PRQA S 3081
    base64_encrypt_length = ACS_SSL::ACS_BASE64_encode(binary_string, &base64_encrypt[0], binary_string_length);
    base64_encrypt.resize(base64_encrypt_length) ;
}

void RemoteSiteUtility::decodeBASE64(const char* binary_string, 
                                     size_t binary_string_length, 
                                     vector <unsigned char> &base64_decrypt) 
{

    size_t base64_decrypt_length = ACS_SSL::ACS_BASE64_decode(binary_string, &base64_decrypt[0], binary_string_length) ;
    base64_decrypt.resize(base64_decrypt_length) ;
}


string RemoteSiteUtility::composeUrl(string const & path, string const & filename)
{
	ACS_COND_THROW( path.empty(), exIllegalValueException("Url body is empty") ); // PRQA S 3081
	return ( StringUtils::trimEnd(path, '/') + '/' + StringUtils::trimBegin(filename, '/') );
}

_ACS_END_NESTED_NAMESPACE
