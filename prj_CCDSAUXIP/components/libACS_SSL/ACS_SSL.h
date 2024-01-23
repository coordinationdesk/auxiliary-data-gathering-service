// PRQA S 1050 EOF
//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 1995-2022, Exprivia S.p.A.  -  DFDA-AS
//    Via Della Bufalotta, 378 - 00139 Roma - Italy
//    http://www.acsys.it
//
//    All Rights Reserved.
//
//    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
//    the contents of this file may not be disclosed to third parties, copied or
//    duplicated in any form, in whole or in part, without the prior written
//    permission of Exprivia S.p.A. 
//
//    $Prod: A.C.S. SSL Library - HEADER FILE $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Release 3.2.2  2019/10/28 15:58:40  enrico.carta@acsys.it
//    EC:: LP:: Add a compile-time check to verify if the salt defines both occur: this would raise a compiling error.
//    
//    Release 3.1.2  2019/10/21 17:12:09  enrico.carta@acsys.it
//    EC:: I/F updated to handle user-provided salt
//    
//    Release 3.1.1  2019/10/18 12:52:29  enrico.carta@acsys.it
//    EC:: Removed cross-reference dependence over libFile
//    
//    Release 3.1.0  2019/10/16 15:24:17  enrico.carta@acsys.it
//    EC:: Added handling of salt and modification of tests
//    EC:: testBLOWFISH_ECB_encode_decode updated to match the latest I/F.
// 
//    Release 3.0.0  2019/04/01 15:45:11  enrico.carta@acsys.it
//    EC:: Method ACS_CRC16_eval made inline to increase performances. 
//    
//    Revision 2.42  2017/02/28 12:04:04  marpas
//    CRC16XM functions family implemented - they compute CRC16 as CRC-CCITT (XModem) -
//    (123456789 string gives 0x31C3)
//
//    Revision 2.41  2016/01/11 13:09:41  enrcar
//    EC:: Added method: ACS_RSA_getExpected_buffersize
//
//    Revision 2.40  2015/12/01 17:43:59  marpas
//    coding best practice application in progress
//
//    Revision 2.39  2015/10/22 11:14:51  enrcar
//    EC:: Warnings removed
//
//    Revision 2.38  2015/05/25 08:56:59  enrcar
//    EC:: Added method ACS_AES_handleGuard
//
//    Revision 2.37  2015/05/21 10:07:25  enrcar
//    EC:: Mismatching integer types warnings removed
//
//    Revision 2.36  2015/05/19 14:30:34  enrcar
//    EC:: Interface for AES cipher improved: now finalize is computed automatically (a finalization can be forced sending an empty input buffer)
//
//    Revision 2.35  2015/05/15 15:56:59  enrcar
//    EC:: Added support for AES (128,192,256) ciphering and deciphering. Both with dedicated methods and through the ACS_ALGO
//
//    Revision 2.34  2015/05/08 09:29:04  marpas
//    portability improved: using llabs instead of __gnu_cxx::abs
//    qa warnings removed
//
//    Revision 2.33  2013/06/20 15:51:39  marpas
//    insertion operators on CipherStyle added
//
//    Revision 2.32  2013/03/07 13:59:56  marpas
//    coding best practices
//    debug improved
//    qa warning
//    interface rationalization
//
//    Revision 2.31  2013/03/05 19:08:23  marpas
//    coding best practices application in progress
//    qa rules in progress
//
//    Revision 2.30  2012/12/10 16:42:29  enrcar
//    EC:: Method  ACS_SSL::MemoryGuard::set  was added
//
//    Revision 2.29  2012/11/23 17:31:19  marpas
//    qa rules
//
//    Revision 2.28  2012/02/07 17:56:32  marpas
//    refactoring in progress
//
//    Revision 2.27  2012/02/07 12:46:21  marpas
//    removing compiler warnings
//
//    Revision 2.26  2011/05/26 14:22:34  enrcar
//    EC:: method ACS_RSA_create_header modifed in order to have a custom identifier in output file
//
//    Revision 2.25  2011/03/01 11:36:21  marpas
//    GCC 4.4.x support
//
//    Revision 2.24  2010/05/18 09:59:39  enrcar
//    EC:: Removed compatibility mode for ACS_SSL_OLD_FLAGS_COMPAT
//    Added new methods :
//    RSA: ACS_RSA_reset_PrivKeyStruct, ACS_RSA_reset_PubKeyStruct, ACS_RSA_dump_key
//    BLOWFISH: ACS_BLOWFISH_randomkey
//
//    Revision 2.23  2010/04/08 09:41:42  enrcar
//    EC:: ACS_RAND_bytes method added
//
//    Revision 2.22  2010/04/02 12:23:00  enrcar
//    EC:: Now is possible to handle key up to 0x8000 bytes, i.e. 262144 bits
//
//    Revision 2.21  2010/04/01 16:40:14  enrcar
//    EC:: struct ACS_SSL_RSA_Pub, struct ACS_SSL_RSA_Priv and relative methods added
//
//    Revision 2.20  2009/07/06 12:32:17  enrcar
//    EC:: enum of ALGOs renumbered -- two bytes are now used
//
//    Revision 2.19  2009/06/30 14:00:13  enrcar
//    EC:: Method ACS_ALGO_encdec_FromFile rewritten
//
//    Revision 2.18  2009/06/25 16:24:10  enrcar
//    EC:: Method ACS_SSL::ACS_ALGO_encdec_File updated to handle long files
//
//    Revision 2.17  2009/06/25 09:55:29  enrcar
//    EC:: Algo Encoding/Decoding is now possible also on files
//
//    Revision 2.16  2009/06/19 14:03:37  enrcar
//    EC:: Several changes. ACS_ALGO_encode/ACS_ALGO_decode partially implemented
//
//    Revision 2.15  2009/06/17 13:18:06  enrcar
//    EC:: Generic EncryptionAlgo supported
//
//    Revision 2.14  2009/06/17 09:11:31  enrcar
//    EC:: BLOWFISH encoding/decoding methods now return the output buffer size
//
//    Revision 2.13  2009/06/16 15:53:55  enrcar
//    EC:: BASE64 Encoding/Decoding handled
//
//    Revision 2.12  2009/06/16 14:03:45  enrcar
//    EC:: BLOWFISH ECB Encryption/Decryption added
//
//    Revision 2.11  2009/05/28 17:18:30  danalt
//    EC:: Static methods ACS_DES_cblock_inc, ACS_DES_cblock_sbb, ACS_DES_cblock_sub, ACS_DES_cblock_mod, ACS_DES_cblock_mov, ACS_DES_cblock_nzr added
//
//    Revision 2.10  2009/05/27 16:24:57  danalt
//    EC:: ACS_SSL_StringToHex method added
//
//    Revision 2.9  2009/05/27 12:50:35  danalt
//    EC:: Method ACS_SSL_HexToString added
//
//    Revision 2.8  2009/05/18 15:27:56  danalt
//    EC:: S-DES OFB handled: method ACS_DES_OFB_encdec added.
//
//    Revision 2.7  2009/05/14 09:40:33  enrcar
//    EC:: c++rules
//
//    Revision 2.6  2009/05/13 12:28:40  enrcar
//    EC:: DES/TDES cyphering added
//
//    Revision 2.5  2008/07/04 08:03:19  enrcar
//    EC:: ulong replaced with uint
//
//    Revision 2.4  2008/07/01 10:06:51  enrcar
//    CipherStyle moved into libACS_SSL
//
//    Revision 2.3  2008/06/27 15:29:03  enrcar
//    EC:: Members made const whenever was possible ;
//    Added methods: ACS_RSA_get_PrivKey, ACS_RSA_get_PubKey
//
//    Revision 2.2  2008/06/19 12:52:33  enrcar
//    EC:: Method ACS_RSA_FillBufferFromHex added
//
//    Revision 2.1  2006/09/19 15:06:54  danalt
//    fixed for 4.1.0
//
//    Revision 2.0  2006/02/28 08:32:29  marpas
//    Exception 2.1 I/F adopted
//
//    Revision 1.5  2005/11/21 13:10:19  enrcar
//    Small changes to interface in order to build on older libssl (COSMO)
//
//    Revision 1.4  2005/11/16 14:13:35  enrcar
//    Added 4 methods for handling cipher-buffer sizes:
//    static int ACS_RSA_size_DLen( RSA const* key )
//    static int ACS_RSA_size_NumB( RSA const* key, unsigned int len)
//    static int ACS_RSA_size_NumP( RSA const* key, unsigned int len)
//    static int ACS_RSA_size_PLen( RSA const* key )
//
//    Revision 1.3  2005/11/04 08:42:53  enrcar
//    ACS_RSA_free() made private
//    RSAKeyGuard added, in order to call method above
//
//    Revision 1.2  2005/11/03 10:21:02  enrcar
//    Interface was changed to allow compatibility with older version of system lib. openssl
//
//    Revision 1.1.1.1  2005/11/02 11:20:22  enrcar
//    Import source
//
//    
//////////////////////////////////////////////////////////////////////////////////////



#ifndef _ACS_SSL_H_
#define _ACS_SSL_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Filterables.h> 
#include <Guard.hpp>
#include <openssl/rsa.h> // PRQA S 1013
#include <openssl/des.h> // PRQA S 1013
#include <openssl/evp.h> // PRQA S 1013
#include <openssl/blowfish.h> // PRQA S 1013
#include <algorithm>

#include <stdint.h>

/* macros used to handle the defines (stripping of double-quoted strings) : */
#define ACS_SSL_MACRO_DOUBLEQUOTE(x) #x
#define ACS_SSL_MACRO_STRINGIFY(arg) ACS_SSL_MACRO_DOUBLEQUOTE(arg) 
#define ACS_SSL_MACRO_DO_EXPAND(VAL)  VAL ## 1
#define ACS_SSL_MACRO_EXPAND(VAL)     ACS_SSL_MACRO_DO_EXPAND(VAL)


_ACS_BEGIN_NAMESPACE(acs)

/* [bytes]  since the maximum lenght for a BF key is 448 bits */
    const int ACS_SSL_BFkey_maxsize = 448/8 ; 
    const int ACS_SSL_mpi_maxsize = 0x8004 ; /* size(INT4) + 32*1024 bytes == sz(I4) + 32768 bits. HEX=0x8004. */

/* CEIL8: Returns the greater-or-equal multiple of 8, given a number (or the number itself) */
/* E.G.:  0 -> 0 ; 1 -> 8 ; 8-> 8 ; 9 -> 16 ; 16 -> 16  ... 1023 -> 1024  ...   */
inline int ACS_SSL_CEIL8(int a) { return (a < 1) ? 0 : (((a-1)/8+1)*8) ; }    // PRQA S 2134, 3382, 4400

/* FLOOR8: Returns the less-or-equal multiple of 8, given a number (or the number itself) */
/* E.G.:  0 -> 0 ; 1 -> 0 ; 8-> 8 ; 9 -> 8 ; 16 -> 16  ... 1025 -> 1024  ...   */
inline int ACS_SSL_FLOOR8(int a) { return ((a)/8)*8 ; }                       // PRQA S 2134, 4400


/* FLOOR6: Returns the less-or-equal multiple of 6, given a number (or the number itself) */
/* E.G.:  0 -> 0 ; 1 -> 0 ; 6-> 6 ; 7 -> 6 ; 12 -> 12  ... 67 -> 66  ...   */
inline int ACS_SSL_FLOOR6(int a) { return ((a)/6)*6 ; }                       // PRQA S 2134, 4400

/* MOD 4, the fastest way to compute %4 */
inline int ACS_SSL_MOD4(int a) { return a & 0x03 ; }                          // PRQA S 2134, 3003, 4400

/* MOD 8, the fastest way to compute %8 */
inline int ACS_SSL_MOD8(int a) { return a & 0x07 ; }                          // PRQA S 2134, 3003, 4400

class ACS_SSL // PRQA S 2109
{
public:

	typedef unsigned char ACS_AES_IV[16] ; // PRQA S 2411, 4403


    enum CipherStyle
    {
        NOCIPHER		= 0x00,
        CIPHER_RSA		= 0x10,
        CIPHER_HW		= 0x20,
        CIPHER_RSA_HW	= 0x30
    } ;
 
 
    enum ACS_AES_Cipher
    {
        AES_128_CBC		= 0x21,
        AES_128_OFB		= 0x22,
        AES_192_CBC		= 0x31,
        AES_192_OFB		= 0x32,
        AES_256_CBC		= 0x41,
        AES_256_OFB		= 0x42
    } ;


	class ACS_AES_Key // PRQA S 2109
	{
	public:
		ACS_AES_Key(const ACS_AES_Cipher, const unsigned char*, const size_t&) ;
		ACS_AES_Key() ; 
		~ACS_AES_Key() ACS_NOEXCEPT ;
		unsigned char* getKey(unsigned char*, const size_t&) const ;
		void resetKey(const unsigned char*, const size_t&) ;
		void clearKey() ;
		size_t getKeyLength() const { return _keyLength ; } // In BYTES, so e.g. 16 for AES-128bit
		void resetCipher(const ACS_AES_Cipher) ;
		ACS_AES_Cipher getCipher() const { return _aes_cipher ; } 
	private:
    	ACS_AES_Key(const ACS_AES_Key &) ; // not implemented
    	ACS_AES_Key& operator=(const ACS_AES_Key &) ; // not implemented

	private:
		unsigned char _key[64] ; // PRQA S 4403
		ACS_AES_Cipher _aes_cipher ;
		size_t _keyLength ;  
	} ;


	class ACS_AES_handle { // PRQA S 2109
	public:
		ACS_AES_handle(const ACS_AES_Cipher&, const ACS_AES_Key&, const ACS_AES_IV&, const bool enc_oper, const bool paddingSet=true) ;
		~ACS_AES_handle() ACS_NOEXCEPT ;
		const EVP_CIPHER_CTX* ctx() const { return _ctx ; }
		EVP_CIPHER_CTX* ctx() { return _ctx ; } // PRQA S 4211
		ACS_AES_Cipher getCipher() const { return _cipher ; } 
		bool getEncOper() const { return _encOper ; } 
		size_t getKeyLength() const { return _keyLength ; } // In BYTES, so e.g. 16 for AES-128bit
		size_t getBlockLength() const { return _blockLength ; } // In BYTES (usually 16 bytes for AES, can be be 1 for configurations like OFB)
		bool paddingSet() const { return _paddingSet ; }
		bool isFinalized() const { return _finalizedSet ; }
		void finalize() { _finalizedSet = true; }
		size_t getExpected_buffersize(const size_t& bufin_len) const { return getExpected_buffersize(bufin_len, _cipher, _encOper, _paddingSet); } // PRQA S 4214
		static size_t getKeyLength(const ACS_AES_Cipher&) ;	  // Static: can be run BEFORE instancing the class, to prepare the input parameters
		static size_t getBlockLength(const ACS_AES_Cipher&) ; // Static: can be run BEFORE instancing the class, to prepare the input parameters
		static size_t getIVLength(const ACS_AES_Cipher&) ; // Static: can be run BEFORE instancing the class, to prepare the input parameters
		static size_t getExpected_buffersize(const size_t& bufin_len, ACS_AES_Cipher cipher, bool enc_oper, bool paddingSet=true) ; // ( as above )
		static void CTX_Release(EVP_CIPHER_CTX* ctx) { if (ctx != 0) { EVP_CIPHER_CTX_cleanup(ctx) ; } }
		typedef pattern::ScopeGuard<EVP_CIPHER_CTX*, &ACS_AES_handle::CTX_Release> CTX_Guard ;
	private:
		ACS_AES_handle() ; // not implemented
		ACS_AES_handle(const ACS_AES_handle &) ;			/* The handle contains an "opaque" pointer, so copy is forbidden */
		ACS_AES_handle &operator=(const ACS_AES_handle &) ;	/* The handle contains an "opaque" pointer, so copy is forbidden */
	private:
		EVP_CIPHER_CTX* _ctx ;		
		ACS_AES_Cipher _cipher ;
		size_t _blockLength ;  
		size_t _keyLength ; 
		bool _encOper ;
		bool _paddingSet ;
		bool _finalizedSet ;
	} ;

	class ACS_AES_handleGuard { // PRQA S 2109
	public:
		explicit ACS_AES_handleGuard(ACS_AES_handle* p) : _p(p) {} ;
		~ACS_AES_handleGuard() { release() ; }
		void replace(ACS_AES_handle* p) { _p = p ; } // PRQA S 2502
		void detach() { _p = 0 ; }
		void release() ACS_NOEXCEPT { if (_p != 0) { delete _p ; } _p=0 ; } 
	private:
		ACS_AES_handleGuard() ;	// NOT IMPLEMENTED
		ACS_AES_handleGuard(const ACS_AES_handleGuard &) ; // NOT IMPLEMENTED
		ACS_AES_handleGuard &operator=(const ACS_AES_handleGuard &) ; // NOT IMPLEMENTED
	private:
		ACS_AES_handle* _p ; 	
	} ;
	 
	enum EncryptionAlgo
	{
		ENC_NONE			= 0x0000,		/*    0:  Unencrypted */

		/* FLAGS: */
		ENC_BASE64			= 0x0001,		/*    1:  (FLAG) Base64 */
		ENC_PREPEND_SALT	= 0x0002,		/*    2:  (FLAG) Prepend a [automatically padded] salt before the input buffer */

		/* UNUSED FLAGS, ALREADY ALLOCATED: */
		UNUSED0004			= 0x0004,
		UNUSED0008			= 0x0008,
		UNUSED0010			= 0x0010,
		UNUSED0020			= 0x0020,
		UNUSED0040			= 0x0040,
		UNUSED0080			= 0x0080,

		/* ALGOS: */
		ENC_BLOWFISH		= 0x0100,		/*  256:  BlowFish ECB */
		ENC_DES				= 0x0200,		/*  512:  Simple DES */
		ENC_DESOFB			= 0x0400,		/* 1024:  DES OFB */
		ENC_AES128CBC		= 0x0500,		/* 1280:  AES_128_CBC */
		ENC_AES192CBC		= 0x0600,		/* 1536:  AES_192_CBC */
		ENC_AES256CBC		= 0x0700,		/* 1792:  AES_256_CBC */
		ENC_TDES			= 0x0800,		/* 2048:  Triple DES */
		ENC_AES128OFB		= 0x0a00,		/* 2560:  AES_128_OFB */
		ENC_AES192OFB		= 0x0b00,		/* 2816:  AES_192_OFB */
		ENC_AES256OFB		= 0x0c00,		/* 3072:  AES_256_OFB */
		ENC_TDESOFBI		= 0x1000		/* 4096:  Triple DES OFB-I */

	} ;


	/*! class ACS_SSL_InternalError declaration. */
	exDECLARE_EXCEPTION(ACS_SSL_InternalError,exException) ;    // PRQA S 2131, 2502
private:   
    static void open_ssl_free(void * a) { OPENSSL_free(a) ; } // because OPENSSL_free is a macro
	static void ACS_RSA_free(RSA* key) ACS_NOEXCEPT ;
public:
   
	ACS_SSL() ACS_NOEXCEPT ;	    /* Nothing to do */
	~ACS_SSL() ACS_NOEXCEPT ;	/* Nothing to do */


/*-----------------------------------------------------------------------------
	RSA Ciphering
-----------------------------------------------------------------------------*/

public:
	/* ACS wrap for struct RSA. Allows easier serializing (no memory ptrs. inside the struct) */
    struct ACS_SSL_RSA_Pub
    {
        unsigned short mpi_size_BE ;/* [2-bytes unsigned] Size of each MPI buffer. Always in BIGENDIAN. */
        unsigned char _resvd_ ;   	/* PAD + struct version */
        unsigned char params ;     	/* Number of params (2) */

        /* Public key */
        unsigned char n[ACS_SSL_mpi_maxsize] ;
        unsigned char e[ACS_SSL_mpi_maxsize] ;

    } ;


    struct ACS_SSL_RSA_Priv
    {
        unsigned short mpi_size_BE ;/* [2-bytes unsigned] Size of each MPI buffer. Always in BIGENDIAN. */
        unsigned char _resvd_ ;    /* PAD + struct version */
        unsigned char params ;      /* Number of params (8) */

        /* Public key */
        unsigned char n[ACS_SSL_mpi_maxsize] ;
        unsigned char e[ACS_SSL_mpi_maxsize] ;
        /* Private key */
        unsigned char d[ACS_SSL_mpi_maxsize] ;
        unsigned char p[ACS_SSL_mpi_maxsize] ;
        unsigned char q[ACS_SSL_mpi_maxsize] ;
        unsigned char dmp1[ACS_SSL_mpi_maxsize] ;
        unsigned char dmq1[ACS_SSL_mpi_maxsize] ;
        unsigned char iqmp[ACS_SSL_mpi_maxsize] ;
    } ;


    static RSA* ACS_RSA_newkey() ;
	static RSA* ACS_RSA_genkey(int n=1024, unsigned int e=0x11) ;
	static RSA* ACS_RSA_getPublicKey(RSA const* key) ;
	static unsigned char* ACS_RSA_priv_dec(
                        unsigned int* returnLength, 
                        const RSA* key,     
                        const unsigned char* buffer, 
                        unsigned int len, 
                        bool emitExcept=true, 
                        bool* success=0) ;

    static unsigned char* ACS_RSA_priv_enc(
                        unsigned int* returnLength, 
                        const RSA* key, 
                        const unsigned char* buffer, 
                        unsigned int len, 
                        bool emitExcept=true, 
                        bool* success=0) ;

    static unsigned char* ACS_RSA_pub_dec(
                        unsigned int* returnLength, 
                        const RSA* key, 
                        const unsigned char* buffer, 
                        unsigned int len, 
                        bool emitExcept=true, 
                        bool* success=0) ;

	static unsigned char* ACS_RSA_pub_enc(
                        unsigned int* returnLength, 
                        const RSA* key, 
                        const unsigned char* buffer, 
                        unsigned int len, 
                        bool emitExcept=true, 
                        bool* success=0) ;

	static unsigned char* ACS_RSA_decode(
                                        const unsigned char*, 
                                        unsigned int, 
                                        const RSA*, 
                                        bool, 
                                        unsigned char*, 
                                        unsigned int*,  
                                        bool emitExcept=true, 
                                        bool* success=0) ;

	static unsigned char* ACS_RSA_encode(
                                        const unsigned char*, 
                                        unsigned int, 
                                        const RSA*, 
                                        bool, 
                                        unsigned char*, 
                                        unsigned int*,  
                                        bool emitExcept=true, 
                                        bool* success=0) ;

	static void ACS_RSA_create_header_pub(RSA const* key, const char* fileName, const char* identifier) ;
	static void ACS_RSA_create_header_priv(RSA const* key, const char* fileName, const char* identifier) ;
	static void ACS_RSA_create_header(RSA const* key, const char* fileName, const char* identifier, bool isHeaderPublic=true) ;
	static void ACS_RSA_fill_PrivKey(RSA* key, const char* n, const char* e, \
		const char* d, const char* p, const char* q, const char* dmp1, \
		const char* dmq1, const char* iqmp) ;
	static void ACS_RSA_fill_PubKey(RSA* key, const char* n, const char* e) ;
	static void ACS_RSA_get_PrivKey(char** n, char** e, char** d, char** p, char** q, char** dmp1, char** dmq1, char** iqmp, \
		const RSA* key) ;
	static void ACS_RSA_get_PubKey(char** n, char** e, const RSA* key) ; 

    static ACS_SSL_RSA_Priv* ACS_RSA_init_PrivKeyStruct(ACS_SSL_RSA_Priv*) ;
    static ACS_SSL_RSA_Pub* ACS_RSA_init_PubKeyStruct(ACS_SSL_RSA_Pub*) ;
    static void ACS_RSA_get_PubKeyStruct(ACS_SSL_RSA_Pub*, const RSA*) ; 
    static void ACS_RSA_set_PubKeyStruct(RSA*, const ACS_SSL_RSA_Pub*) ; 
    static void ACS_RSA_get_PrivKeyStruct(ACS_SSL_RSA_Priv*, const RSA*) ; 
    static void ACS_RSA_set_PrivKeyStruct(RSA*, const ACS_SSL_RSA_Priv*) ; 
    static void ACS_RSA_reset_PrivKeyStruct(ACS_SSL_RSA_Priv*) ;/* Fill content with 0s */
    static void ACS_RSA_reset_PubKeyStruct(ACS_SSL_RSA_Pub*) ;	/* Fill content with 0s */

    static void ACS_RSA_dump_key(const RSA* key, bool isPriv=false) ;	/* Debug only */

    static off_t ACS_RSA_getExpected_buffersize(const int& n, const off_t& bytes_in, const int& padding=RSA_PKCS1_PADDING) ;	/* Compute the size AFTER ENCODING */

	static unsigned int ACS_RSA_fillBufferFromHex(const char* s, unsigned char* d, unsigned int buflen) ;
	static int ACS_RSA_size_DLen( const RSA* key ) ;
	static int ACS_RSA_size_NumB( const RSA* key, unsigned int len) ; 
	static int ACS_RSA_size_NumP( const RSA* key, unsigned int len) ; 
	static int ACS_RSA_size_PLen( const RSA* key ) ;
	static BIGNUM* ACS_SSL_mpi2bn (	const unsigned char*, int, BIGNUM*) ;

	/**
		Specific method only to encipher ACS passwords. THIS IS NOT A GENERIC MULTI-PURPOSE RSA ENCODER. 
		This also automatically takes into account the salt.
		The Output buffer MUST be preallocated (at a reasonable size, e.g. 4096 bytes are enough for one password...). The actual size will be returned to the caller.
		An exception will be returned if the preallocated output buffer is too short.
	*/
	static bool ACS_RSA_encode_password( const unsigned char* input_buffer, const size_t& input_buffer_size, const RSA* key, unsigned char* output_buffer, size_t& output_buffer_size) ;

	/**
		Specific method only to decipher ACS passwords. THIS IS NOT A GENERIC MULTI-PURPOSE RSA DECODER. 
		This also automatically takes into account the salt.
		The Output buffer MUST be preallocated (at a reasonable size, e.g. 4096 bytes are enough for one password...). The actual size will be returned to the caller.
		An exception will be returned if the preallocated output buffer is too short.
	*/
	static bool ACS_RSA_decode_password( const unsigned char* input_buffer, const size_t& input_buffer_size, const RSA* key, unsigned char* output_buffer, size_t& output_buffer_size) ;


/*-----------------------------------------------------------------------------
	DES, TDES Ciphering
-----------------------------------------------------------------------------*/
public:
	static void ACS_DES_setkey( const_DES_cblock* keyin_ptr, DES_key_schedule* keyout_ptr ) ;
	static size_t ACS_DES_decrypt1( const unsigned char* bufin, unsigned char* bufout, size_t, 
		DES_key_schedule key1) ;
	static size_t ACS_DES_encrypt1( const unsigned char* bufin, size_t, unsigned char* bufout, 
		const DES_key_schedule &key1) ;
	static size_t ACS_DES_decrypt3( const unsigned char* bufin, unsigned char* bufout, size_t, 
		DES_key_schedule key1, DES_key_schedule key2, DES_key_schedule key3) ;
	static size_t ACS_DES_encrypt3( const unsigned char* bufin, size_t, unsigned char* bufout, 
		DES_key_schedule key1, DES_key_schedule key2, DES_key_schedule key3) ;

	static size_t ACS_DES_TOFB_I_encdec( const unsigned char* bufin, size_t, unsigned char* bufout, 
		DES_key_schedule key1, DES_key_schedule key2, DES_key_schedule key3, 
		const DES_cblock iv1, const DES_cblock iv2, const DES_cblock iv3) ;

	static size_t ACS_DES_OFB_encdec( const unsigned char* bufin, size_t, unsigned char* bufout, 
		const DES_key_schedule &key, const DES_cblock iv1) ;

	static void ACS_DES_getIV23( const DES_cblock iv1, DES_cblock iv2, DES_cblock iv3) ;


/*-----------------------------------------------------------------------------
	AES Ciphering
-----------------------------------------------------------------------------*/
public:
	static size_t ACS_AES_encrypt( ACS_AES_handle&, const unsigned char* bufin, const size_t&, unsigned char* bufout, size_t& bufout_len ) ; /** Use to cipher a buffer (of a large set) and/or to cipher the last buffer (if detected) */
	static size_t ACS_AES_decrypt( ACS_AES_handle&, const unsigned char* bufin, const size_t&, unsigned char* bufout, size_t& bufout_len ) ; /** Use to cipher a buffer (of a large set) and/or to cipher the last buffer (if detected) */


/*-----------------------------------------------------------------------------
	BLOWFISH Ciphering
-----------------------------------------------------------------------------*/
public:
	static void ACS_BLOWFISH_setkey( const unsigned char*, int len_bytes, BF_KEY*) ;
	static void ACS_BLOWFISH_randomkey( unsigned char*, int* bufLen, BF_KEY *key=0, int len_bytes=56) ;	/* Default lenght is the maximum possible */	
	static size_t ACS_BLOWFISH_ECB_decrypt( const unsigned char* bufin, unsigned char* bufout, size_t, BF_KEY *) ;
	static size_t ACS_BLOWFISH_ECB_encrypt( const unsigned char* bufin, unsigned char* bufout, size_t, BF_KEY *) ;
	static size_t ACS_BLOWFISH_ECB_decrypt_base64( const char* bufin, unsigned char* bufout, size_t, BF_KEY *) ;
	static size_t ACS_BLOWFISH_ECB_encrypt_base64( const unsigned char* bufin, char* bufout, size_t, BF_KEY *) ;


/*-----------------------------------------------------------------------------
	RC4 Ciphering
-----------------------------------------------------------------------------*/
public:
	static void ACS_RC4_pseudorand(unsigned char* outbuf,\
		unsigned char a, unsigned char b, unsigned int len);
	static void ACS_RC4_scramble(unsigned char* outbuf, \
		unsigned char* s, unsigned int s_len);


/*-----------------------------------------------------------------------------
	RANDOM NUMBERS Generator
-----------------------------------------------------------------------------*/
public:
	static int ACS_RAND_bytes(unsigned char *buf, int bufsize, bool randomSeed=true) ;
	static int ACS_RAND_bytes(unsigned char *buf, int bufsize, const std::pair <unsigned char, unsigned char>&, bool randomSeed=true) ;	/* Extract from a subdomain */


/*-----------------------------------------------------------------------------
	CRC-16 Evaluation
-----------------------------------------------------------------------------*/
public:

/*-------------------------------------------------------------------------------------------
	CRC-16 Evaluation CCITT poly=0x1021 init=0xffff refin=refot=xorot=0x0 verification=0x29B1
-------------------------------------------------------------------------------------------*/
	inline static unsigned short int ACS_CRC16_eval(const unsigned char* buffer, \
		unsigned int offset, unsigned int buflen) 
	{ 
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << " offset=" << offset << "  buflen=" << buflen ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
		return ACS_CRC16_eval(buffer+offset, buflen) ; 
	}

	inline static uint16_t ACS_CRC16_eval(const char* buffer, unsigned int offset, unsigned int buflen) 
	{
	    return ACS_CRC16_eval(reinterpret_cast<const unsigned char *>(buffer), offset, buflen) ; // PRQA S 3030, 3081
	}

	inline static uint16_t ACS_CRC16_eval(const unsigned char* data_p, const size_t& length)
	{
		/** Method made inline to increase performances. Also the algorithm is implemented without calling any LUT 
			(The LUT was created to speed-up machines with slow ALU, and also to allow different CRC algorithms.
			In this case, only one algorithm is called)
		*/
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << " buflen=" << length ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

    	uint16_t x, x5, x12 ; 
    	uint16_t crc = 0xFFFF;
    	for (size_t i=0; i<length; i++)
		{
			x = crc >> 8 ^ *data_p++;
			x ^= x>>4; x5 = (x << 5) ; x12 = (x5 << 7) ;  
			crc = ((crc << 8) | x) ^ (x5 ^ x12); 
    	}
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    	ACS_CLASS_BGN_DEBUG(10) 
        	exFDebug dbg ;
        	dbg << exDebugSignature << "Return value: 0x" << std::hex << std::setw(4) << std::setfill('0') << crc << std::dec ;
        	excerr << dbg ;
    	ACS_CLASS_END_DEBUG
#endif 

    	return crc;
	}
					
	static unsigned short int ACS_CRC16_eval(unsigned char* crc2bytes,
		const unsigned char* buffer, unsigned int offset, unsigned int buflen) ;


/*---------------------------------------------------------------------------------------------------
	CRC-16 Evaluation CCITT (XMODEM) poly=0x1021 init=0x0 refin=refot=xorot=0x0 verification=0x31C3
---------------------------------------------------------------------------------------------------*/
	static unsigned short int ACS_CRC16XM_eval(const char* buffer, \
		unsigned int offset, unsigned int buflen) ;
	static unsigned short int ACS_CRC16XM_eval(const unsigned char* buffer, \
		unsigned int offset, unsigned int buflen) ;
	static unsigned short int ACS_CRC16XM_eval(unsigned char* crc2bytes,
		const unsigned char* buffer, unsigned int offset, unsigned int buflen) ;

/*-----------------------------------------------------------------------------
	BASE64 Cyphering
-----------------------------------------------------------------------------*/
public:
	static size_t ACS_BASE64_decode(const char *src, unsigned char *dest, size_t) ;
	static size_t ACS_BASE64_encode(const unsigned char *src, char* out, size_t) ; 
	static size_t ACS_BASE64_encode_buffersize(size_t) ;


/*-----------------------------------------------------------------------------
	HEX Dump and String Conversion
-----------------------------------------------------------------------------*/
public:
	/** Binary buffer to printable string (e.g. 0x42ff -> "42FF" ...) */
	static void ACS_SSL_HexToString(const unsigned char* buffer, size_t buflen, char* outbuffer, size_t outbuflen) ;
	static void ACS_SSL_HexToString(const unsigned char* buffer, size_t buflen, std::string &hex_string) ;
	/** Printable string to binary buffer (e.g. "42FF" -> 0x42ff ...) */
	static void ACS_SSL_StringToHex(const char* buffer, size_t buflen, unsigned char* outbuffer, size_t* outbuflen) ;
	static void ACS_SSL_StringToHex(const std::string &hex_string, unsigned char* outbuffer, size_t* outbuflen) ;


/*-----------------------------------------------------------------------------
	Cblock operations
-----------------------------------------------------------------------------*/
public:
	static unsigned char ACS_DES_cblock_adc (const DES_cblock src, DES_cblock dst, unsigned char carry) ;
	static unsigned char ACS_DES_cblock_add (const DES_cblock src, DES_cblock dst) ;
	static unsigned char ACS_DES_cblock_inc (DES_cblock dst) ;
	static unsigned char ACS_DES_cblock_sbb (const DES_cblock src, DES_cblock dst, unsigned char carry) ;
	static unsigned char ACS_DES_cblock_sub (const DES_cblock src, DES_cblock dst) ;
	static void ACS_DES_cblock_mod (const DES_cblock src, DES_cblock dst) ;
	static void ACS_DES_cblock_mov (const DES_cblock src, DES_cblock dst) ;
	static unsigned char ACS_DES_cblock_nzr (const DES_cblock src) ;


/*-----------------------------------------------------------------------------
	Encryption / Decryption Generic Algos 
-----------------------------------------------------------------------------*/
public:
	static size_t ACS_ALGO_decode(	EncryptionAlgo al_in, const unsigned char *bufin, size_t s_in, unsigned char *bufout,
		const std::vector <void*> &in_values, std::vector <void*> &out_values) ;
	static size_t ACS_ALGO_encode(	EncryptionAlgo al_in, const unsigned char *bufin, size_t s_in, unsigned char *bufout, 
		const std::vector <void*> &in_values, std::vector <void*> &out_values) ;
	static size_t ACS_ALGO_expected_buffersize(EncryptionAlgo al_in, size_t, int enc) ;

	static size_t ACS_ALGO_encdec(	EncryptionAlgo al, const unsigned char *bufin, size_t s_in, unsigned char *bufout,
		const std::vector <void*> &in_values, std::vector <void*> &out_values, int enc) ;

	static size_t ACS_ALGO_encdec_FromFile(	EncryptionAlgo al, const std::string &fileIn, unsigned char *bufout, size_t bufout_size,
		const std::vector <void*> &in_values, std::vector <void*> &out_values, int enc) ;

	static size_t ACS_ALGO_encdec_ToFile(	EncryptionAlgo al, const unsigned char *bufin, size_t s_in, const std::string &fileOut,
		const std::vector <void*> &in_values, std::vector <void*> &out_values, int enc) ;

	static size_t ACS_ALGO_encdec_File(	EncryptionAlgo al, const std::string &fileIn, const std::string &fileOut,
		const std::vector <void*> &vin, std::vector <void*> &vout, int enc) ;

/*-----------------------------------------------------------------------------
	Convenience functions
-----------------------------------------------------------------------------*/
public:

static inline void swap_16 (void* p_in)
{
	unsigned char *p = static_cast<unsigned char*>(p_in) ;
	unsigned char t ; t = p[0] ; p[0] = p[1] ; p[1] = t ; 
}
static inline void swap_32 (void* p_in)
{
	unsigned char *p = static_cast<unsigned char*>(p_in) ;
	unsigned char t ; t = p[0] ; p[0] = p[3] ; p[3] = t ; t = p[1] ; p[1] = p[2] ; p[2] = t ;
}

/* EncryptionAlgo_Flags: Returns the flags set (removing the algo). */
    static inline EncryptionAlgo EncryptionAlgo_Flags(EncryptionAlgo al) 
        { return static_cast<EncryptionAlgo>(al & 0x00ff) ; } // PRQA S 3003, 3013, 3081, 4400

/* EncryptionAlgo_Flags: Returns the algo set (removing the flags). */
    static inline EncryptionAlgo EncryptionAlgo_Algo(EncryptionAlgo al) 
        { return static_cast<EncryptionAlgo>(al & 0xff00) ; } // PRQA S 3003, 3013, 3081, 4400

	static std::string toString(EncryptionAlgo) ; // PRQA S 2502 3
	static std::string toString(CipherStyle) ; 
	static std::string toString(ACS_AES_Cipher) ; 

    typedef pattern::ScopeGuard<void*, &ACS_SSL::open_ssl_free> OPENSSLMemoryGuard ;
    typedef pattern::ScopeGuard<RSA*, &ACS_SSL::ACS_RSA_free> RSAKeyGuard ;

/*-----------------------------------------------------------------------------
	Salt handling
-----------------------------------------------------------------------------*/

/** This is the only method which probably should be called from user-level. You must specify the expected_salt_size and an optional input_buffer (e.g. 0x06, 0x22, 0xfe ... ...)
	If (0 == expected_salt_size) the method returns 0, regardless any input buffer.
	If input_buffer is NULL or "" a random salt will be generated instead. If input_buffer is not empty, the output_buffer will be filled up to expected_salt_size.
	An exception will be returned if the output_buffer_size is unable to contain the output buffer.
	On success, the method returns the bytes copied onto the output_buffer.
*/
	static size_t encode_salt_buffer ( const unsigned char* input_buffer, const size_t& input_buffer_size, const size_t& expected_salt_size, unsigned char* output_buffer, const size_t& output_buffer_size) ; 
	
	static uint32_t decode_salt (const unsigned char* input_buffer, const EncryptionAlgo& algo=ENC_NONE) ;
	static size_t decode_salt (	const unsigned char* input_buffer, const size_t& input_buffer_size,	unsigned char* output_buffer=NULL, const size_t& output_buffer_size=0) ;
	static size_t decode_salt (	const unsigned char* coded_input_buffer, unsigned char* output_buffer, const size_t& output_buffer_size, const EncryptionAlgo& algo) ;
	static size_t encode_salt (	const size_t& input_buffer_size ) ;
	static size_t encode_salt (	const unsigned char* input_buffer, const size_t& input_buffer_size,	 const size_t& expected_salt_size, unsigned char* output_buffer, const size_t& output_buffer_size) ;
	static size_t encode_random_salt (const size_t& random_bytes, unsigned char* output_buffer, const size_t& output_buffer_size) ;
	static size_t embed_salt_in_buffer (const unsigned char* coded_salt_buffer, const EncryptionAlgo& algo_in, const unsigned char* input_buffer, const size_t& input_buffer_size, \
									 	unsigned char** output_buffer_ptr, const size_t& output_buffer_size) ;									
	static size_t remove_salt_from_buffer (	const size_t& salt_size, const EncryptionAlgo& algo_in,	const unsigned char* input_buffer, const size_t& input_buffer_size, \
											unsigned char* output_buffer, const size_t& output_buffer_size)	;							

/*-----------------------------------------------------------------------------
	Check is salt is set (static functions to handle configuration parameters)
-----------------------------------------------------------------------------*/
	static bool checkSaltIsConfigured(size_t& saltRandom, std::string& saltString) ; 
	inline static void setSalt(const size_t& r, const std::string& s=""){ Lock guard(_saltMutex); _saltRandom = r ; _saltString = s ; _saltSet = true ; }
	inline static std::string getSaltString(bool& is_set) 				{ Lock guard(_saltMutex); is_set = _saltSet ; return _saltString ; }
	inline static size_t getSaltRandom(bool& is_set)					{ Lock guard(_saltMutex); is_set = _saltSet ;  return _saltRandom ; }
	inline static bool getSaltSet()										{ Lock guard(_saltMutex); return _saltSet ; }

private:			

	ACS_SSL(const ACS_SSL&) ;             /* Unimplemented */
	ACS_SSL &operator=(const ACS_SSL&) ;	/* Unimplemented */

	static size_t ACS_DES_enc_decrypt1( size_t, const unsigned char* bufin, \
		unsigned char* bufout, const DES_key_schedule &key1, int enc) ;
	static size_t ACS_DES_enc_decrypt3( size_t, const unsigned char* bufin, \
		unsigned char* bufout, const DES_key_schedule &key1, \
		const DES_key_schedule &key2, const DES_key_schedule &key3, int enc) ;
	static size_t ACS_BLOWFISH_ECB_enc_dec( size_t, const unsigned char* bufin, unsigned char* bufout, BF_KEY *, int enc) ;

	inline static void ACS_xor3 (const unsigned char* src1,
						       const unsigned char* src2,
						       unsigned char* dst,
						       size_t length) 
    {
	    // dst = XOR (src1, src2)  where src1 & src2 (and dst of course) 
        // are sequences of "length" bytes
	    for (size_t i=0; i<length; i++) { dst[i] = (src1[i] ^ src2[i]) ; } // PRQA S 3000,3010
    }


	inline static size_t ACS_xor3_lenMax8 (const unsigned char* src1,
									     const unsigned char* src2,
									     unsigned char* dst,
									     size_t length) 
    {
	    //	dst = XOR (src1, src2)  where src1 & src2 (and dst of course) are sequences 
		//  of "length" bytes, where lenght is <= 8 

	    size_t le = std::min(length,size_t(8)) ; // PRQA S 3081
	    for (size_t i=0; i<le; ++i) { dst[i] = (src1[i] ^ src2[i]) ; } // PRQA S 3000,3010
	    return le ;
    }


	inline static bool ACS_BASE64_is_base64(char c) ACS_NOEXCEPT 
    {
      return  ((c >= 'A') && (c <= 'Z')) || 
              ((c >= 'a') && (c <= 'z')) || 
              ((c >= '0') && (c <= '9')) || 
              (c == '+') || 
              (c == '/') || 
              (c == '=') ; 
    }
// PRQA S 3000, 3010, 4400, 5310 L1		
    inline static unsigned char ACS_BASE64_char_decode(char c) ACS_NOEXCEPT // PRQA S 4020
    {
 	    /* Decrypt a single char */

	    if ((c >= 'A') && (c <= 'Z'))      { return (c - 'A'); }
	    else if ((c >= 'a') && (c <= 'z')) { return (c - 'a' + 26); }
	    else if ((c >= '0') && (c <= '9')) { return (c - '0' + 52); }
	    else if (c == '+')                 { return 62 ; }
	    else                               { return 63 ; } // default
    }

	inline static char ACS_BASE64_char_encode(unsigned char u) ACS_NOEXCEPT // PRQA S 4020
    {
	    /* Encrypt a single char */
	    if(u < 26)       { return 'A'+u ; }
	    else if(u < 52)  { return 'a'+(u-26); }
	    else if(u < 62)  { return '0'+(u-52); } //
        else if(u == 62) { return '+';  }       
        else             { return '/'; }	// default value 
    }
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACS_SSL) ;
// PRQA L:L1

private:	
	static acs::ThreadSafe	_saltMutex ; 
	static std::string		_saltString ;     
	static size_t			_saltRandom ;
	static bool				_saltSet ;     

} ;

std::ostream & operator << (std::ostream &, const ACS_SSL::EncryptionAlgo &) ; // PRQA S 2072
exostream & operator << (exostream &, const ACS_SSL::EncryptionAlgo &) ; // PRQA S 2072

std::ostream & operator << (std::ostream &, const ACS_SSL::CipherStyle &) ; // PRQA S 2072
exostream & operator << (exostream &, const ACS_SSL::CipherStyle &) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif	// _ACS_SSL_H_
