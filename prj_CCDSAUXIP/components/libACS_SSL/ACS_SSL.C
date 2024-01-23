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
//    $Prod: A.C.S. SSL Library $
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
//    Revision 2.50  2016/01/11 13:09:41  enrcar
//    EC:: Added method: ACS_RSA_getExpected_buffersize
//
//    Revision 2.49  2015/12/01 17:43:59  marpas
//    coding best practice application in progress
//
//    Revision 2.48  2015/10/22 15:32:42  marpas
//    compilation warning removed
//
//    Revision 2.47  2015/10/22 11:14:51  enrcar
//    EC:: Warnings removed
//
//    Revision 2.46  2015/06/03 08:18:41  enrcar
//    EC:: Buffer size fixed in AES ciphering
//
//    Revision 2.45  2015/05/21 10:07:25  enrcar
//    EC:: Mismatching integer types warnings removed
//
//    Revision 2.44  2015/05/19 14:30:34  enrcar
//    EC:: Interface for AES cipher improved: now finalize is computed automatically (a finalization can be forced sending an empty input buffer)
//
//    Revision 2.43  2015/05/15 15:56:58  enrcar
//    EC:: Added support for AES (128,192,256) ciphering and deciphering. Both with dedicated methods and through the ACS_ALGO
//
//    Revision 2.42  2015/05/08 09:29:04  marpas
//    portability improved: using llabs instead of __gnu_cxx::abs
//    qa warnings removed
//
//    Revision 2.41  2014/08/04 14:27:15  lucio.pulvirenti
//    RSA_encode, RSA_decoder: in case of error, a not-NULL buffer pointer could be returned, even if already freed. This produced a double-free by the caller. Fixed.
//    Bug fixed (typo) in method ACS_BASE64_decode
//
//    Revision 2.40  2013/06/20 15:51:39  marpas
//    insertion operators on CipherStyle added
//
//    Revision 2.39  2013/03/07 13:59:56  marpas
//    coding best practices
//    debug improved
//    qa warning
//    interface rationalization
//
//    Revision 2.38  2013/03/05 19:08:23  marpas
//    coding best practices application in progress
//    qa rules in progress
//
//    Revision 2.37  2012/12/10 16:42:29  enrcar
//    EC:: Method  ACS_SSL::MemoryGuard::set  was added
//
//    Revision 2.36  2012/11/23 17:31:19  marpas
//    qa rules
//
//    Revision 2.35  2012/07/24 09:49:58  enrcar
//    EC:: Compilations warnings removed
//
//    Revision 2.34  2012/05/21 12:12:35  matteo.airoldi
//    Wrong FDGuard instantiation
//
//    Revision 2.33  2012/02/14 13:42:45  marpas
//    removing deprecated macro
//
//    Revision 2.32  2012/02/07 17:56:32  marpas
//    refactoring in progress
//
//    Revision 2.31  2012/02/07 12:46:21  marpas
//    removing compiler warnings
//
//    Revision 2.30  2011/05/27 13:20:26  enrcar
//    ACS_RSA_Decode: zero-length inputs were not correctly handled: fixed.
//
//    Revision 2.29  2011/05/26 14:22:34  enrcar
//    EC:: method ACS_RSA_create_header modifed in order to have a custom identifier in output file
//
//    Revision 2.28  2011/03/01 11:36:21  marpas
//    GCC 4.4.x support
//
//    Revision 2.27  2010/05/18 09:59:39  enrcar
//    EC:: Removed compatibility mode for ACS_SSL_OLD_FLAGS_COMPAT
//    Added new methods :
//    RSA: ACS_RSA_reset_PrivKeyStruct, ACS_RSA_reset_PubKeyStruct, ACS_RSA_dump_key
//    BLOWFISH: ACS_BLOWFISH_randomkey
//
//    Revision 2.26  2010/04/08 09:41:42  enrcar
//    EC:: ACS_RAND_bytes method added
//
//    Revision 2.25  2010/04/01 16:40:14  enrcar
//    EC:: struct ACS_SSL_RSA_Pub, struct ACS_SSL_RSA_Priv and relative methods added
//
//    Revision 2.24  2009/07/13 09:20:16  enrcar
//    EC:: c++rules
//
//    Revision 2.23  2009/07/06 12:32:17  enrcar
//    EC:: enum of ALGOs renumbered -- two bytes are now used
//
//    Revision 2.22  2009/06/30 14:00:13  enrcar
//    EC:: Method ACS_ALGO_encdec_FromFile rewritten
//
//    Revision 2.21  2009/06/25 16:24:10  enrcar
//    EC:: Method ACS_SSL::ACS_ALGO_encdec_File updated to handle long files
//
//    Revision 2.20  2009/06/25 09:55:29  enrcar
//    EC:: Algo Encoding/Decoding is now possible also on files
//
//    Revision 2.19  2009/06/24 10:23:02  enrcar
//    EC:: Added DES and TDES to supported ALGOs
//
//    Revision 2.18  2009/06/19 14:03:37  enrcar
//    EC:: Several changes. ACS_ALGO_encode/ACS_ALGO_decode partially implemented
//
//    Revision 2.17  2009/06/17 13:18:06  enrcar
//    EC:: Generic EncryptionAlgo supported
//
//    Revision 2.16  2009/06/17 09:11:31  enrcar
//    EC:: BLOWFISH encoding/decoding methods now return the output buffer size
//
//    Revision 2.15  2009/06/16 15:53:55  enrcar
//    EC:: BASE64 Encoding/Decoding handled
//
//    Revision 2.14  2009/06/16 14:03:45  enrcar
//    EC:: BLOWFISH ECB Encryption/Decryption added
//
//    Revision 2.13  2009/05/29 14:22:46  danalt
//    EC:: checks added
//
//    Revision 2.12  2009/05/28 17:18:30  danalt
//    EC:: Static methods ACS_DES_cblock_inc, ACS_DES_cblock_sbb, ACS_DES_cblock_sub, ACS_DES_cblock_mod, ACS_DES_cblock_mov, ACS_DES_cblock_nzr added
//
//    Revision 2.11  2009/05/27 16:24:57  danalt
//    EC:: ACS_SSL_StringToHex method added
//
//    Revision 2.10  2009/05/27 12:50:35  danalt
//    EC:: Method ACS_SSL_HexToString added
//
//    Revision 2.9  2009/05/19 16:25:12  enrcar
//    EC:: comment added
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
//    Revision 2.5  2009/03/18 15:35:53  enrcar
//    EC:: Debug improved
//
//    Revision 2.4  2008/07/04 08:03:19  enrcar
//    EC:: ulong replaced with uint
//
//    Revision 2.3  2008/06/27 15:29:03  enrcar
//    EC:: Members made const whenever was possible ;
//    Added methods: ACS_RSA_get_PrivKey, ACS_RSA_get_PubKey
//
//    Revision 2.2  2008/06/19 12:52:33  enrcar
//    EC:: Method ACS_RSA_FillBufferFromHex added
//
//    Revision 2.1  2008/05/28 13:44:58  enrcar
//    EC:: typo fixed
//
//    Revision 2.0  2006/02/28 08:32:29  marpas
//    Exception 2.1 I/F adopted
//
//    Revision 1.7  2005/11/21 13:10:19  enrcar
//    Small changes to interface in order to build on older libssl (COSMO)
//
//    Revision 1.6  2005/11/18 10:45:33  enrcar
//    comment added
//
//    Revision 1.5  2005/11/16 14:13:35  enrcar
//    Added 4 methods for handling cipher-buffer sizes:
//    static int ACS_RSA_size_DLen( RSA const* key )
//    static int ACS_RSA_size_NumB( RSA const* key, unsigned int len)
//    static int ACS_RSA_size_NumP( RSA const* key, unsigned int len)
//    static int ACS_RSA_size_PLen( RSA const* key )
//
//    Revision 1.4  2005/11/04 08:45:39  enrcar
//    Unable to make tag. fixed
//
//    Revision 1.3  2005/11/03 10:23:41  enrcar
//    include added for C compatibility
//
//    Revision 1.2  2005/11/03 10:21:02  enrcar
//    Interface was changed to allow compatibility with older version of system lib. openssl
//
//    Revision 1.1.1.1  2005/11/02 11:20:22  enrcar
//    Import source
//
//    
//////////////////////////////////////////////////////////////////////////////////////


#include <openssl/rsa.h> // PRQA S 1013
#include <openssl/rand.h> // PRQA S 1013
#include <ACS_SSL.h> 
#include <Filterables.h> 

#include <crcmodel.h>

#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <sys/time.h> // PRQA S 1013

#include <sslV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
    sslV version ; // versioning 
}

using namespace std ;

ThreadSafe ACS_SSL::_saltMutex ; 
string ACS_SSL::_saltString ( "" ); 	
size_t ACS_SSL::_saltRandom ( 0 ) ;
bool ACS_SSL::_saltSet( false ) ;	  

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACS_SSL);

ACS_SSL::ACS_SSL() throw() 
{        
#if(!0)
    // IF HERE, DEBUG ALWAYS ALLOWED
    #define ACS_SSL_HAS_DEBUG_ENABLED 1	
#endif


#if(0)
    // IF HERE, NO DEBUG CAN BE ALLOWED
    #undef ACS_SSL_HAS_DEBUG_ENABLED 
#endif


#ifdef ACS_SSL_HAS_DEBUG_ENABLED      
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) 
#endif 
} 

ACS_SSL::~ACS_SSL() throw() {} 	/* Nothing to do */


///////////////////////////////////////
//
//   PERFORM RC4 PERMUTATION
//
///////////////////////////////////////
void
		ACS_SSL::ACS_RC4_scramble(	unsigned char* outbuf,	/* OUTPUT_BUFFER (allocated by user) */
									unsigned char* s,		/* INPUT BUFFER, 256 bytes */
									unsigned int s_len) 	/* OUTPUT BUFFER req'd length */
{

	// input buffer s must be already allocated (256 chars)
	// outbut buffer must be already allocated (s_len chars). s_len can be <,==,> 256  

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"len: " << s_len) ;
#endif 


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		for (unsigned int i=0; i<256; ++i) // PRQA S 4400
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"input buffer: " << hex << "0x" << i  // PRQA S 3081
			    << " value: 0x" << static_cast<unsigned int>(s[i])) ;
		}
    ACS_CLASS_END_DEBUG
#endif 
 
    unsigned char i = 0 ;
    unsigned char j = 0 ;
    unsigned char t = 0 ;
    
	for (unsigned int c=0; c<s_len; ++c)
	{
		i ++ ;
		j += s[i] ; // PRQA S 3000, 3010
		t=s[i]; s[i]=s[j]; s[j]=t;
		outbuf[c] = s[ static_cast<unsigned char>(s[i] + s[j]) ];   // PRQA S 3081
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"output buffer: " << hex << "0x" << c    // PRQA S 3081
			    << " value: 0x" << static_cast<unsigned int>(outbuf[c])) ;
#endif 
	}	

	return ;
}			


///////////////////////////////////////////////
//
//   CREATE RC4 PSEUDO-RANDOM SEQUENCE
//
///////////////////////////////////////////////
void
		ACS_SSL::ACS_RC4_pseudorand(	unsigned char* outbuf,	/* OUTPUT_BUFFER (allocated by user) */
										unsigned char a, unsigned char b,	/* INPUT PARAMS */
										unsigned int len) 					/* BUFFER length */
{

	unsigned char s[256] ; // PRQA S 4403
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    // PRQA S 3081 7
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"a: 0x" << hex << static_cast<unsigned int>(a) 
		    << " b: 0x" << static_cast<unsigned int>(b) << dec << " len: " << len) ;
#endif 
		
	// generate starting-sequence:
    // PRQA S 3000 4
	for (unsigned short i=0; i<256; ++i) {        // PRQA S 4400
		s[i] = (i + a + b);                      // PRQA S 3010
    }

    // PRQA S 4400 4
	// swap 2 bytes: 
	unsigned char t=s[b]; s[b]=s[(0xff)-b]; s[(0xff)-b]=t ;
	if ( (b != a) && (b != (0xff)-a) ) {
		t=s[a]; s[a]=s[(0xff)-a]; s[(0xff)-a]=t ;
    }
	

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    // PRQA S 4400 7
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		for (unsigned int i=0; i<256; ++i) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"i:\t" << i << hex << " (0x" << i // PRQA S 3081
			    <<") \ts[i]=0x" << static_cast<unsigned int>(s[i])) ; 
        }
    ACS_CLASS_END_DEBUG
#endif 
	
	
	return ACS_RC4_scramble(outbuf, s, len) ;
}			


//////////////////////////////////////////////////////
//
//   Get package size (for a given key-lenght)
//
//////////////////////////////////////////////////////
int ACS_SSL::ACS_RSA_size_PLen( const RSA* key )
{
// package size is the sum: (payload_size + header_size) 
// payload_size can be obtained by function ACS_RSA_size_DLen()
// So, if needed, header_size = PLen - DLen  

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"key: " << hex << key) ;
#endif 
	
    return RSA_size(key) ;
}


//////////////////////////////////////////////////////
//
//   Get data size allowed in each package
//
//////////////////////////////////////////////////////
int ACS_SSL::ACS_RSA_size_DLen( const RSA* key )
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"key: " << hex << key) ;
#endif 
	
	/* Check key lenght */
    int t = (ACS_RSA_size_PLen(key) - 12) ;
    
    if (t<1)
    {
        ostringstream m ;
        m << "Invalid package size: " << ACS_RSA_size_PLen(key) ;
        ACS_THROW(ACS_SSL_InternalError(m.str())); // PRQA S 3081
    }
    
    return t ;
}


//////////////////////////////////////////////////////
//
//   Get # of packages req'd for encoding datasize bytes
//
//////////////////////////////////////////////////////
int                                                         // Req'd packages 
            ACS_SSL::ACS_RSA_size_NumP( const RSA* key, 
                                        unsigned int len)   // buffer to cipher
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
        exFDebug dbg ;
		dbg << exDebugSignature << "key: " << hex << key << dec 
			<< "  len: " << len ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	unsigned int payloadLen = ACS_RSA_size_DLen(key); // PRQA S 3000
	unsigned int pkgNumber = ((len + (payloadLen-1)) / payloadLen); //CEIL Function // PRQA S 3084

    return pkgNumber ; // PRQA S 3000
}


//////////////////////////////////////////////////////
//
//   Get bytes req'd for encoding datasize 
//
//////////////////////////////////////////////////////
int                                                         // Req'd packages 
            ACS_SSL::ACS_RSA_size_NumB( const RSA* key, 
                                        unsigned int len)   // buffer to cipher
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
        exFDebug dbg ;
		dbg << exDebugSignature << "key: " << hex << key << dec 
			<< "  len: " << len ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
    return ( ACS_RSA_size_NumP(key, len) * ACS_RSA_size_PLen(key) ) ; 
}


///////////////////////////////////////
//
//   DECODE BUFFER USING PRIVATE KEY
//
///////////////////////////////////////
unsigned char*  													/* OUT_BUFFER (malloc) */
				ACS_SSL::ACS_RSA_priv_dec(	unsigned int* returnLength,	/* Size of OUT_Buffer */
											const RSA* key, 			/* KEY */
											const unsigned char* buffer,/* IN_Buffer */ 
											unsigned int len,			/* Size of IN_Buffer */
											bool emitExcept,			/* IN: true for emit exceptions */ 
											bool* success)				/* OUT: true if successfully */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return  ACS_RSA_decode(buffer, len, key, true, 0, returnLength, emitExcept, success) ;
}


//////////////////////////////////////////////////////
//
//   ENCODE BUFFER USING PRIVATE KEY
//
//////////////////////////////////////////////////////
unsigned char*															/* OUT_BUFFER (malloc) */
				ACS_SSL::ACS_RSA_priv_enc(	unsigned int* returnLength,	/* Size of OUT_Buffer */
											const RSA* key,				/* RSA Key */ 
											const unsigned char* buffer,/* IN_Buffer */ 
											unsigned int len,			/* Size of IN_Buffer */
											bool emitExcept,			/* IN: true for emit exceptions */ 
											bool* success)				/* OUT: true if successfully */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return ACS_RSA_encode (buffer, len, key, true, 0, returnLength, emitExcept, success) ;
}


///////////////////////////////////////
//
//   DECODE BUFFER USING PUBLIC KEY
//
///////////////////////////////////////
unsigned char*  													/* OUT_BUFFER (malloc) */
				ACS_SSL::ACS_RSA_pub_dec(	unsigned int* returnLength,	/* Size of OUT_Buffer */
											const RSA* key, 			/* KEY */
											const unsigned char* buffer,/* IN_Buffer */ 
											unsigned int len,			/* Size of IN_Buffer */
											bool emitExcept,			/* IN: true for emit exceptions */ 
											bool* success)				/* OUT: true if successfully */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return  ACS_RSA_decode(buffer, len, key, false, 0, returnLength, emitExcept, success) ;
}


//////////////////////////////////////////////////////
//
//   ENCODE BUFFER USING PUBLIC KEY
//
//////////////////////////////////////////////////////
unsigned char*														/* OUT_BUFFER (malloc) */
				ACS_SSL::ACS_RSA_pub_enc(	unsigned int* returnLength,	/* Size of OUT_Buffer */
											const RSA* key,				/* Key */ 
											const unsigned char* buffer,/* IN_Buffer */ 
											unsigned int len,			/* Size of IN_Buffer */
											bool emitExcept,			/* IN: true for emit exceptions */ 
											bool* success)				/* OUT: true if successfully */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return ACS_RSA_encode (buffer, len, key, false, 0, returnLength, emitExcept, success) ;
}


//////////////////////////////////////////////////////
//
//   ENCODE BUFFER USING RSA KEY
//
//////////////////////////////////////////////////////
unsigned char*															/* OUT_BUFFER (malloc) */
				ACS_SSL::ACS_RSA_encode(	const unsigned char* buffer,/* IN_Buffer */ 
											unsigned int len,			/* Size of IN_Buffer */
											const RSA* key,				/* RSA Key */ 
											bool isPrivate,				/* TRUE if Key is private*/
                                            unsigned char* outbuffer,	/* Pre-allocated buffer, or NULL to allocate */
                                            unsigned int* returnLength,	/* IN/OUT Size of OUT_Buffer */
											bool emitExcept,			/* IN: true for emit exceptions */ 
											bool* success)				/* OUT: true if successfully */
{
    bool toAllocate = ( outbuffer == 0 ) ;	/* TRUE if memory buffer not provided */

	try
	{
		if (0 != success) { *success = false; }
		
		/* Check key size: */
		ACS_COND_THROW( // PRQA S 3081
			RSA_size(key) < 13,// since 12 is the overhead ...
			ACS_SSL_InternalError("Invalid key size")
		) ; 

		unsigned int pkgLen = ACS_RSA_size_PLen(key) ;          // PRQA S 3000
		unsigned int payloadLen = ACS_RSA_size_DLen(key) ;      // PRQA S 3000
		unsigned int pkgNumber = ACS_RSA_size_NumP(key, len) ;  // PRQA S 3000
		unsigned char tempBuffer[pkgLen] ;
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(20) 
            exFDebug dbg ;
            dbg << exDebugSignature << "pkgLen: " << pkgLen << " payloadLen: " << payloadLen 
                << " pkgNumber: " << pkgNumber << "  isPrivate: " << boolalpha << isPrivate;
            excerr << dbg ;
        ACS_CLASS_END_DEBUG
#endif 
    
        if (toAllocate)
        {
        	outbuffer = reinterpret_cast<unsigned char*>(malloc (ACS_RSA_size_NumB(key, len))) ; // PRQA S 3000, 3081
		    ACS_COND_THROW(!outbuffer,ACS_SSL_InternalError("Out of memory", errno)) ;  /* Check output buffer: */ // PRQA S 3081
		}

		// The guard below is used to automatically release the allocated memory (if any) ONLY IN CASE OF TROUBLE !
		// In case of success (last line of the method is reached) the guard will be detached and the relase will be prevented (see below).
        pattern::CMemoryScopeGuard outbuffer_guard( (true == toAllocate)? outbuffer : NULL ) ;	// Memory free'd in case of problems; guard detached in case of success.

		unsigned int l = payloadLen ;
		for (unsigned int i=0; i<pkgNumber; ++i)
		{
			if (i == pkgNumber-1) { l = len - ((pkgNumber-1) * payloadLen) ; } // PRQA S 3084

			int wrote ; // PRQA S 4101
            
            if (isPrivate) {
            	wrote = RSA_private_encrypt(l, (buffer+(i*payloadLen)), tempBuffer, const_cast<RSA*>(key), RSA_PKCS1_PADDING); // PRQA S 3000, 3081, 3084
			}
            else {
                wrote = RSA_public_encrypt (l, (buffer+(i*payloadLen)), tempBuffer, const_cast<RSA*>(key), RSA_PKCS1_PADDING); // PRQA S 3000, 3081, 3084
            }
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
            bool p = false ;
            ACS_CLASS_BGN_DEBUG(50) 
                exFDebug dbg ;
                dbg << exDebugSignature << "pkg: " << i << "/" << pkgNumber 
                    << " datalen: " << l << " size after encode: " << wrote ;
                excerr << dbg ;
                p = true ;
            ACS_CLASS_END_DEBUG

            ACS_CLASS_BGN_DEBUG(20) 
                if ( (!p && (0 == (i%1024))) || (i == pkgNumber-1) ) { // PRQA S 3084, 4400
			        exFDebug dbg ;
                    dbg << exDebugSignature << "pkg: " << i << "/" << pkgNumber 
                        << " datalen: " << l << " size after encode: " << wrote ;
                    excerr << dbg ;
                }
            ACS_CLASS_END_DEBUG
#endif 


			ACS_COND_THROW( // PRQA S 3081
				wrote != int(pkgLen),
				ACS_SSL_InternalError("RSA encrypt method unable to create valid package")
			) ; 
  
            ACS_COND_THROW( (!toAllocate && (((i+1)*pkgLen)>(*returnLength))), // PRQA S 3081, 3084
                ACS_SSL_InternalError("Output buffer overflow (pre-allocated but too short)") ) ;

			memcpy( (outbuffer+(i*pkgLen)), tempBuffer, pkgLen); // PRQA S 3084
		} // for i

		*returnLength = ACS_RSA_size_NumB(key, len) ; // PRQA S 3000

		if (0 != success) { (*success) = true; }
		if (true == toAllocate) { outbuffer_guard.detach() ; }	// success: about to return -- do not free the locally-allocated memory (it will be returned to the caller)
	} catch(exception &)
	{
		outbuffer = 0;
		if (emitExcept) { throw ; } // Re-Throw exception
	}

	return outbuffer ;
}


///////////////////////////////////////////////////////
//
//  DECODE BUFFER USING RSA KEY
//
//////////////////////////////////////////////////////
unsigned char*  														/* OUT_BUFFER (malloc) */
				ACS_SSL::ACS_RSA_decode(	const unsigned char* buffer,/* IN_Buffer */ 
											unsigned int len,			/* Size of IN_Buffer */
											const RSA* key, 			/* RS KEY */
											bool isPrivate,				/* TRUE if Key is private*/
                                            unsigned char* outbuffer,	/* Pre-allocated buffer, or NULL to allocate */
                                            unsigned int* returnLength,	/* IN/OUT Size of OUT_Buffer */
											bool emitExcept,			/* IN: true for emit exceptions */ 
											bool* success)				/* OUT: true if successfully */
{
	bool toAllocate = ( outbuffer == 0 ) ;	/* TRUE if memory buffer not provided */
    
	unsigned int pkgNumber = 0 ;

        
	try
	{	
		if (0 != success) { (*success) = false; }
	
		ACS_COND_THROW(                              // PRQA S 3081
			RSA_size(key) < 13,// since 12 is the overhead ...
			ACS_SSL_InternalError("Invalid key size")
		) ; 

		unsigned int pkgLen = ACS_RSA_size_PLen(key) ; // PRQA S 3000
        // Number of packages containing ciphered data
		unsigned char tempBuffer[pkgLen] ; // To be sure....
		pkgNumber = (len? ((len + pkgLen-1) / pkgLen): 0); // Do NOT USE: ACS_RSA_size_NumP // PRQA S 3054, 3084, 3380 

		// WARNING: outbuffer will be reallocated into a CORRECT (SMALLER) BUFFER.

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(10) 
            exFDebug dbg ;
            dbg << exDebugSignature << "pkgLen: " << pkgLen << " pkgNumber: " << pkgNumber ;
            excerr << dbg ;
        ACS_CLASS_END_DEBUG
#endif 
	
    	if (toAllocate && pkgNumber) // PRQA S 3054
        {
		    outbuffer = reinterpret_cast<unsigned char*>(malloc (pkgNumber * pkgLen)) ; // To be sure... // PRQA S 3081, 3084
		    ACS_COND_THROW(!outbuffer,ACS_SSL_InternalError("Out of memory", errno)) ; // PRQA S 3081
		}

		// The guard below is used to automatically release the allocated memory (if any) ONLY IN CASE OF TROUBLE !
		// In case of success (last line of the method is reached) the guard will be detached and the relase will be prevented (see below).
        pattern::CMemoryScopeGuard outbuffer_guard( (true == toAllocate)? outbuffer : NULL ) ;	// Memory free'd in case of problems; guard detached in case of success.
				
		unsigned int pos = 0;
		for (unsigned int i=0; i<pkgNumber; ++i)
		{

			int wrote = 0 ;
          
            if (isPrivate) {
                wrote = RSA_private_decrypt(pkgLen, (buffer+(i*pkgLen)), tempBuffer, const_cast<RSA*>(key), RSA_PKCS1_PADDING); // PRQA S 3000, 3081, 3084
            }
            else {
                wrote = RSA_public_decrypt(pkgLen, (buffer+(i*pkgLen)), tempBuffer, const_cast<RSA*>(key), RSA_PKCS1_PADDING); // PRQA S 3000, 3081, 3084
            }
 
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
            bool p = false ;
            ACS_CLASS_BGN_DEBUG(50) 
                exFDebug dbg ; 
		        dbg << exDebugSignature << "pkg: " << i << "/" << pkgNumber << " pos: " 
                    << pos << " pkgLen: " << pkgLen << " size after decode: " << wrote ;
                excerr << dbg ;
                p = true ;
	        ACS_CLASS_END_DEBUG

            ACS_CLASS_BGN_DEBUG(20) 
                if ( (!p && (!(i%1024))) || (i == pkgNumber-1) ) // PRQA S 3054, 3084, 4400
                {
                    exFDebug dbg ;
                    dbg << exDebugSignature << "pkg: " << i << "/" << pkgNumber << " pos: " 
                        << pos << " pkgLen: " << pkgLen << " size after decode: " << wrote ;
                    excerr << dbg ;
                }
            ACS_CLASS_END_DEBUG
#endif 

			ACS_COND_THROW( // PRQA S 3081
				wrote<0,
				ACS_SSL_InternalError("Unable to decode package")
			) ; 

            ACS_COND_THROW( (!toAllocate && ((pos+wrote)>(*returnLength))), // PRQA S 3000, 3081, 3084
                ACS_SSL_InternalError("Output buffer overflow (pre-allocated but too short)") ) ;

			memcpy( (outbuffer+pos), tempBuffer, wrote); // PRQA S 3000, 3084
			pos += wrote ; // PRQA S 3000, 3084

		} // for i

		(*returnLength) = pos ;
        if (toAllocate && (pos != 0))
        {
		    unsigned char* return_buffer = reinterpret_cast<unsigned char*>(realloc(outbuffer, pos)) ; // PRQA S 3081
		    ACS_COND_THROW(  // PRQA S 3081
			    !return_buffer,
			    ACS_SSL_InternalError("Unable to realloc memory", errno)
		    ) ; 

			outbuffer = return_buffer ;	/* Since realloc was successfully */	
		}

		if (success) { (*success) = true; }
		if (true == toAllocate) { outbuffer_guard.detach() ; }	// success: about to return -- do not free the locally-allocated memory (it will be returned to the caller)
	} catch(exception &)
	{
		outbuffer = 0 ; 	
		if (emitExcept) { throw ; } // Re-Throw exception
	}
    
	return outbuffer ;
}


///////////////////////////////////////////////
//
//   GENERATE RSA KEY (PRIVATE+PUBLIC)
//
///////////////////////////////////////////////
RSA*																/* OUTPUT: Key */
		ACS_SSL::ACS_RSA_genkey(int n, unsigned int e)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "n=" << n << " exp = 0x" << hex << e ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
		
	
	unsigned char* buffer = reinterpret_cast<unsigned char*>(malloc(1024)); // Entropy // PRQA S 3081
	ACS_COND_THROW(!buffer, ACS_SSL_InternalError("Out of memory", errno)) ;  // PRQA S 3081

	
	pattern::CMemoryScopeGuard bufferguard(buffer);

	RAND_seed(buffer, 1024); // memory may be dirty: good! // PRQA S 4400
	
	time_t t = time(0) ;
	memcpy(buffer, &t, sizeof(time_t));
	RAND_seed(buffer, 1024);  // PRQA S 4400

	ACS_COND_THROW(RAND_status() == 0,ACS_SSL_InternalError("Random generator unable to work")) ;   // PRQA S 3081
	
	RSA *key ; 

#if OPENSSL_VERSION_NUMBER < 0x10100000L
	key = RSA_generate_key(n, static_cast<unsigned long>(e), 0, 0); // openssl // PRQA S 3081
	ACS_COND_THROW(!key, ACS_SSL_InternalError("Key generation failed")) ;   // PRQA S 3081
#else
	BIGNUM* bn_e = BN_new() ;	// Create a new instance of BIGNUM [e] -- it will be deallocated by the handler of the key
	int ret = BN_set_word(bn_e, e) ; 
	ACS_COND_THROW( (ret<1), ACS_SSL_InternalError("Error setting e")) ;  // PRQA S 3081 
	
	key = RSA_new();
	ret = RSA_generate_key_ex(key, n, bn_e, NULL/* Callback */);
	ACS_COND_THROW( (ret<1), ACS_SSL_InternalError("Error generating the RSA key")) ;  // PRQA S 3081 
#endif

	// no free() required since MemoryGuard used
	
	return key ;
}


///////////////////////////////////////////////
//
//   CREATE PUBLIC KEY FROM "FULL" KEY
//
///////////////////////////////////////////////
RSA* 															/* OUTPUT: Public Key */
			ACS_SSL::ACS_RSA_getPublicKey(	RSA const* key		/* INPUT: "FULL" KEY */
											) 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
	
	ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;  // PRQA S 3081
	
	RSA* tempkey = RSA_new();  
	ACS_COND_THROW(!tempkey, ACS_SSL_InternalError("Unable to create temporary key")) ;   // PRQA S 3081


#if OPENSSL_VERSION_NUMBER < 0x10100000L

    // Copy N to new key
    {
        char* tmp = BN_bn2dec(key->n);  // allocate string. Must be freed with OPENSSL_free
        ACS_COND_THROW(!tmp, ACS_SSL_InternalError("Unable to read N")) ;  // PRQA S 3081

        int r = BN_dec2bn( &(tempkey->n), tmp);
        OPENSSL_free(tmp);
        ACS_COND_THROW(0 == r, ACS_SSL_InternalError("Unable to write N")) ;  // PRQA S 3081
    }

    // Copy E to new key
    {
        char* tmp = BN_bn2dec(key->e);  // allocate string. Must be freed with OPENSSL_free
        ACS_COND_THROW(!tmp, ACS_SSL_InternalError("Unable to read E")) ;  // PRQA S 3081

        int r = BN_dec2bn( &(tempkey->e), tmp);
        OPENSSL_free(tmp);
        ACS_COND_THROW(0 == r, ACS_SSL_InternalError("Unable to write E")) ;  // PRQA S 3081
    }

#else
		const BIGNUM* bn_n	;					// No need to deallocate
		const BIGNUM* bn_e	;					// No need to deallocate
		RSA_get0_key(key, &bn_n, &bn_e, NULL) ; // Extract the parameters

		BIGNUM *tempkey_bn_n (BN_dup(bn_n)) ;	// Create a new instance of BIGNUM [n] -- it will be deallocated by the handler of tempkey
	 	BIGNUM *tempkey_bn_e (BN_dup(bn_e)) ;	// Create a new instance of BIGNUM [e] -- it will be deallocated by the handler of tempkey
		RSA_set0_key(tempkey, tempkey_bn_n, tempkey_bn_e, NULL) ; 

#endif

	return tempkey;
}


///////////////////////////////////////////////
//
//   DEALLOCATE KEY
//
///////////////////////////////////////////////
void ACS_SSL::ACS_RSA_free(RSA* key) throw() 
{
    try {
	    ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;  // PRQA S 3081

	    RSA_free(key);
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}


///////////////////////////////////////////////
//
//   ALLOCATE EMPTY KEY
//
///////////////////////////////////////////////
RSA* ACS_SSL::ACS_RSA_newkey()
{
	RSA* key = RSA_new();  

	ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;  // PRQA S 3081
	
	return key ;
}


///////////////////////////////////////////////
//
//   CREATE HEADER FILE (PUBLIC KEY)
//
///////////////////////////////////////////////
void 
			ACS_SSL::ACS_RSA_create_header_pub( RSA const* key, 
												const char* fileName,
												const char* identifier)
{
	const bool isHeaderPublic = true ;
	return ACS_RSA_create_header(key, fileName, identifier, isHeaderPublic);
}


///////////////////////////////////////////////
//
//   CREATE HEADER FILE (PRIVATE KEY)
//
///////////////////////////////////////////////
void 
			ACS_SSL::ACS_RSA_create_header_priv( 	RSA const* key, 
													const char* fileName, 
													const char* identifier)
{
	const bool isHeaderPublic = false ;
	return ACS_RSA_create_header(key, fileName, identifier, isHeaderPublic);
}


///////////////////////////////////////////////
//
//   CREATE HEADER FILE 
//
///////////////////////////////////////////////
void 
			ACS_SSL::ACS_RSA_create_header( RSA const* key, 
											const char* fileName,
											const char* identifier,
											bool isHeaderPublic) 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "fileName: \"" << fileName << "\"  identifier: " << identifier 
            << "\"  isHeaderPublic: " << boolalpha << isHeaderPublic ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
	
	// Check if Key is defined
	ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;  // PRQA S 3081

	// Check if fileName is defined
	ACS_COND_THROW((!fileName || (0 == strlen(fileName))), ACS_SSL_InternalError("Invalid filename")) ;  // PRQA S 3081
	
	// Check if identifier is defined
	ACS_COND_THROW((!identifier || (0 == strlen(identifier))), ACS_SSL_InternalError("Invalid identifier")) ;  // PRQA S 3081

	// Open header file
	FILE* fd = fopen (fileName, "w") ; // PRQA S 4412
	ACS_COND_THROW(0 == fd, ACS_SSL_InternalError("Can't create file: "+ string(fileName), errno)) ;  // PRQA S 3081
	
	fprintf(fd, "%s", "//\n//  (C) 2005-2011, A.C.S.\n//\n//\n");
	if (isHeaderPublic) {
		fprintf(fd, "//  \x24Prod: %s Header \x24\n//\n", identifier);
    }
	else {
		fprintf(fd, "//  \x24Prod: %s Header \x24\n//\n", identifier);
    }
	{
  		char hn[128]; gethostname(hn, 128); // PRQA S 4400, 4403
		time_t tm = time(0); struct tm *ptr = localtime(&tm);
		char str[128];                      // PRQA S 4400, 4403 2
		strftime(str, 128 , "%F %T",ptr);
		fprintf(fd, "%s %s  host: %s\n//\n", "//  Created on:", str, hn); 
	}
	fprintf(fd, "%s", "//  \x24Id: \x24\n//\n//  Automatic generated file - DO NOT EDIT\n//\n\n\n");
	
	char h_str[1024] ; // PRQA S 4403
	srand(time(0)) ; // PRQA S 3000, 3010
	int r=rand() ;
	fprintf(fd, "#ifndef _%s_%x_HEADER_\n#define _%s_%x_HEADER_\n", identifier, r, identifier, r) ;
	sprintf(h_str, "\tconst char %s_", identifier) ;
	fprintf(fd, "%s", "#ifdef __cplusplus\nextern \x22\x43\x22 {\n#endif\n\n" ) ;
	
	
	// Public Key params (common for both keys)
	{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 		char *tmp = BN_bn2hex(key->n);
#else
		const BIGNUM* bn_n ;			// No need to deallocate
		RSA_get0_key(key, &bn_n , NULL, NULL) ; 
 		char *tmp = BN_bn2hex(bn_n);
#endif
		unsigned short int chk = ACS_CRC16_eval (tmp, 0, sizeof(tmp) ) ;
		fprintf(fd, "//\tKey Checksum: 0x%04x\n\n\n",chk);
		fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "n", tmp);
		OPENSSL_free(tmp);
	}
	{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 		char *tmp = BN_bn2hex(key->e);
#else
		const BIGNUM* bn_e ;			// No need to deallocate
		RSA_get0_key(key, NULL, &bn_e , NULL) ; 
 		char *tmp = BN_bn2hex(bn_e);
#endif
		fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "e", tmp);
		OPENSSL_free(tmp);
	}
	
	
	if (!isHeaderPublic)
	{
		// Private key params:
		fprintf(fd, "\n#warning \x22%s\x22\n\n", "A.C.S. Including Private Key");
		{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 			char *tmp = BN_bn2hex(key->d);	/* param. d */
#else
			const BIGNUM* bn_d ;			// No need to deallocate
			RSA_get0_key(key, NULL, NULL, &bn_d) ; 
 			char *tmp = BN_bn2hex(bn_d);	/* param. d */
#endif
			fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "d", tmp);
			OPENSSL_free(tmp);
		}
		{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 			char *tmp = BN_bn2hex(key->p);	/* factor p */
#else
			const BIGNUM* bn_p ;			// No need to deallocate
			RSA_get0_factors(key, &bn_p, NULL);
 			char *tmp = BN_bn2hex(bn_p);	/* factor p */
#endif
			fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "p", tmp);
			OPENSSL_free(tmp);
		}
		{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 			char *tmp = BN_bn2hex(key->q);	/* factor q */
#else
			const BIGNUM* bn_q ;			// No need to deallocate
			RSA_get0_factors(key, NULL, &bn_q);
 			char *tmp = BN_bn2hex(bn_q);	/* factor q */
#endif
			fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "q", tmp);
			OPENSSL_free(tmp);
		}
		{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 			char *tmp = BN_bn2hex(key->dmp1);	/* param. dmp1 */
#else
			const BIGNUM* bn_dmp1 ;				// No need to deallocate
			RSA_get0_crt_params(key, &bn_dmp1, NULL, NULL) ; 
			char *tmp = BN_bn2hex(bn_dmp1);		/* param. dmp1 */
#endif
			fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "dmp1", tmp);
			OPENSSL_free(tmp);
		}
		{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 			char *tmp = BN_bn2hex(key->dmq1);	/* param. dmq1 */
#else
			const BIGNUM* bn_dmq1 ;				// No need to deallocate
			RSA_get0_crt_params(key, NULL, &bn_dmq1, NULL) ; 
 			char *tmp = BN_bn2hex(bn_dmq1);		/* param. dmq1 */
#endif
			fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "dmq1", tmp);
			OPENSSL_free(tmp);
		}
		{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
 			char *tmp = BN_bn2hex(key->iqmp);	/* param. iqmp */
#else
			const BIGNUM* bn_iqmp ;				// No need to deallocate
			RSA_get0_crt_params(key, NULL, NULL, &bn_iqmp) ; 
 			char *tmp = BN_bn2hex(bn_iqmp);		/* param. iqmp */
#endif
			fprintf(fd, "%s%s[] = \x22%s\x22;\n\n", h_str, "iqmp", tmp);
			OPENSSL_free(tmp);
		}
	} // if (!isHeaderPublic)
	
	fprintf(fd, "%s", "#ifdef __cplusplus\n}\n#endif\n#endif // HEADER\n" ) ;

	fclose(fd);	/* close header file */
	
	return ;
}


///////////////////////////////////////////////
//
//   FILL PUBLIC KEY 
//
///////////////////////////////////////////////
void 
        ACS_SSL::ACS_RSA_fill_PubKey(	RSA* key,						/* Output. Pre-allocated ! */ 
										const char* n, const char* e)	/* Inputs */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
     	exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	/* Check if key is defined */
	ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;   // PRQA S 3081
	
#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ACS_COND_THROW(    // PRQA S 3081
		BN_hex2bn(&(key->n), n) < 1, 
		ACS_SSL_InternalError("Cannot assign N")) ;
 
	ACS_COND_THROW(  // PRQA S 3081
		BN_hex2bn(&(key->e), e) < 1, 
		ACS_SSL_InternalError("Cannot assign E")) ; 
#else
	BIGNUM *bn_n (BN_new()) ;	// Create a new instance of BIGNUM [n] -- it will be deallocated by the handler of the key
	BIGNUM *bn_e (BN_new()) ;	// Create a new instance of BIGNUM [e] -- it will be deallocated by the handler of the key

	try {
		ACS_COND_THROW(    // PRQA S 3081
			BN_hex2bn(&bn_n, n) < 1, 
			ACS_SSL_InternalError("Cannot copy N")) ;
	 
		ACS_COND_THROW(  // PRQA S 3081
			BN_hex2bn(&bn_e, e) < 1, 
			ACS_SSL_InternalError("Cannot copy E")) ; 

		ACS_COND_THROW(  // PRQA S 3081
			RSA_set0_key(key, bn_n, bn_e, NULL) < 1,
			ACS_SSL_InternalError("Cannot assign n / e to the key")) ; 
	} catch(exception& e)
	{
		BN_free(bn_n) ; BN_free(bn_e) ; 
		throw ;
	}

#endif 

	return ;
}


///////////////////////////////////////////////
//
//   RETURN PUBLIC KEY 
//
///////////////////////////////////////////////
void 
        ACS_SSL::ACS_RSA_get_PubKey(	char** n, char** e,	/* Must be freed by caller using OPENSSL_free */
                                        const RSA* key) 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
     	exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	/* Check if key, n and e are defined */
	ACS_COND_THROW(!n, ACS_SSL_InternalError("Invalid input parameter N")) ;  // PRQA S 3081
	ACS_COND_THROW(!e, ACS_SSL_InternalError("Invalid input parameter E")) ;  // PRQA S 3081
	ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid key (NULL)")) ;  // PRQA S 3081
	
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    ACS_COND_THROW( // PRQA S 3081
		!(*n = BN_bn2hex(key->n)),
		ACS_SSL_InternalError("Cannot assign N")) ; 
    ACS_COND_THROW( // PRQA S 3081
		!(*e = BN_bn2hex(key->e)),
		ACS_SSL_InternalError("Cannot assign E")) ; 
#else
	const BIGNUM* bn_n ;					// No need to deallocate
	const BIGNUM* bn_e ;					// No need to deallocate
	RSA_get0_key(key, &bn_n, &bn_e, NULL) ; // Extract the values [no ownership]

    ACS_COND_THROW( // PRQA S 3081
		!(*n = BN_bn2hex(bn_n)),
		ACS_SSL_InternalError("Cannot assign N")) ; 
    ACS_COND_THROW( // PRQA S 3081
		!(*e = BN_bn2hex(bn_e)),
		ACS_SSL_InternalError("Cannot assign E")) ; 
#endif
  
	return ;
}


///////////////////////////////////////////////
//
//   FILL PRIVATE KEY 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_fill_PrivKey(	RSA* key, 
										const char* n, const char* e, 
										const char* d, const char* p, const char* q,  
										const char* dmp1, const char* dmq1, const char* iqmp) 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
     	exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	
	/* Check if key is defined */
	ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;  // PRQA S 3081 

#if OPENSSL_VERSION_NUMBER < 0x10100000L
 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->n), n) < 1,	/* Assign n */ 
		ACS_SSL_InternalError("Cannot assign N")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->e), e) < 1,	/* Assign e */
		ACS_SSL_InternalError("Cannot assign E")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->d), d) < 1,	/* Assign d */ 
		ACS_SSL_InternalError("Cannot assign D")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->p), p) < 1,	/* Assign p */ 
		ACS_SSL_InternalError("Cannot assign P")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->q), q) < 1,	/* Assign q */ 
		ACS_SSL_InternalError("Cannot assign Q")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->dmp1), dmp1) < 1,	/* Assign dmp1 */ 
		ACS_SSL_InternalError("Cannot assign DMP1")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->dmq1), dmq1) < 1,	/* Assign dmq1 */ 
		ACS_SSL_InternalError("Cannot assign DMQ1")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		BN_hex2bn(&(key->iqmp), iqmp) < 1,	/* Assign iqmp */ 
		ACS_SSL_InternalError("Cannot assign IQMP")) ; 
#else
	
	BIGNUM *bn_n	(BN_new()) ;	// Create a new instance of BIGNUM [n] -- it will be deallocated by the handler of the key
	BIGNUM *bn_e	(BN_new()) ;	// Create a new instance of BIGNUM [e] -- it will be deallocated by the handler of the key		
	BIGNUM *bn_d	(BN_new()) ;	// Create a new instance of BIGNUM [d] -- it will be deallocated by the handler of the key		
	BIGNUM *bn_p	(BN_new()) ;	// Create a new instance of BIGNUM [p] -- it will be deallocated by the handler of the key		
	BIGNUM *bn_q	(BN_new()) ;	// Create a new instance of BIGNUM [q] -- it will be deallocated by the handler of the key		
	BIGNUM *bn_dmp1	(BN_new()) ;	// Create a new instance of BIGNUM [dmp1] -- it will be deallocated by the handler of the key		
	BIGNUM *bn_dmq1	(BN_new()) ;	// Create a new instance of BIGNUM [dmq1] -- it will be deallocated by the handler of the key		
	BIGNUM *bn_iqmp	(BN_new()) ;	// Create a new instance of BIGNUM [iqmp] -- it will be deallocated by the handler of the key		

	// Extract all the parameters and store to the pre-allocated BN: 

	try {

		ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_n, n) < 1,	/* extract n */ 
			ACS_SSL_InternalError("Cannot extract N")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_e, e) < 1,	/* extract e */
			ACS_SSL_InternalError("Cannot extract E")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_d, d) < 1,	/* extract d */ 
			ACS_SSL_InternalError("Cannot extract D")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_p, p) < 1,	/* extract p */ 
			ACS_SSL_InternalError("Cannot extract P")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_q, q) < 1,	/* extract q */ 
			ACS_SSL_InternalError("Cannot extract Q")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_dmp1, dmp1) < 1,	/* extract dmp1 */ 
			ACS_SSL_InternalError("Cannot extract DMP1")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_dmq1, dmq1) < 1,	/* extract dmq1 */ 
			ACS_SSL_InternalError("Cannot extract DMQ1")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			BN_hex2bn(&bn_iqmp, iqmp) < 1,	/* extract iqmp */ 
			ACS_SSL_InternalError("Cannot extract IQMP")) ; 

		// Assign the pre-allocated BN to the key: 

	 	ACS_COND_THROW( // PRQA S 3081
			RSA_set0_key(key, bn_n, bn_e, NULL) < 1,	
			ACS_SSL_InternalError("Cannot assign n/e")) ; 

	 	ACS_COND_THROW( // PRQA S 3081
			RSA_set0_factors(key, bn_p, bn_q) < 1,	
			ACS_SSL_InternalError("Cannot assign p/q")) ; 

		 ACS_COND_THROW( // PRQA S 3081
			RSA_set0_crt_params(key, bn_dmp1, bn_dmq1, bn_iqmp) < 1,	
			ACS_SSL_InternalError("Cannot assign dmp1/dmq1/iqmp")) ; 

	} catch (exception& e)
	{
		BN_free(bn_n) ;	BN_free(bn_e) ; BN_free(bn_d) ; BN_free(bn_p) ;	
		BN_free(bn_q) ;	BN_free(bn_dmp1) ; BN_free(bn_dmq1) ; BN_free(bn_iqmp) ;	
		throw ;
	}
	
#endif

	return ;
}


///////////////////////////////////////////////
//
//   GET PRIVATE KEY 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_get_PrivKey(	char** n, char** e,				/* Must be freed by caller using OPENSSL_free */
										char** d, char** p, char** q, 	/* (...) */ 
										char** dmp1, char** dmq1, char** iqmp,	/* (...) */ 
                                        const RSA* key)					/* IN: RSA Key */			
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
     	exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
	
	/* Check parameter pointers */
    ACS_COND_THROW(!n, ACS_SSL_InternalError("Invalid input parameter n")) ;  // PRQA S 3081 
    ACS_COND_THROW(!e, ACS_SSL_InternalError("Invalid input parameter e")) ;  // PRQA S 3081
    ACS_COND_THROW(!p, ACS_SSL_InternalError("Invalid input parameter p")) ;  // PRQA S 3081
    ACS_COND_THROW(!q, ACS_SSL_InternalError("Invalid input parameter q")) ;  // PRQA S 3081
    ACS_COND_THROW(!dmp1, ACS_SSL_InternalError("Invalid input parameter dmp1")) ;  // PRQA S 3081
    ACS_COND_THROW(!dmq1, ACS_SSL_InternalError("Invalid input parameter dmq1")) ;  // PRQA S 3081
    ACS_COND_THROW(!iqmp, ACS_SSL_InternalError("Invalid input parameter iqmp")) ;  // PRQA S 3081
    ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input parameter key")) ;   // PRQA S 3081

	/* Obtain parameters */
#if OPENSSL_VERSION_NUMBER < 0x10100000L

 	ACS_COND_THROW(  // PRQA S 3081
		!(*n = BN_bn2hex(key->n)),	/* Assign n */ 
		ACS_SSL_InternalError("Cannot assign N")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*e = BN_bn2hex(key->e)),	/* Assign e */  
		ACS_SSL_InternalError("Cannot assign E")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*d = BN_bn2hex(key->d)),	/* Assign d */  
		ACS_SSL_InternalError("Cannot assign D")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*p = BN_bn2hex(key->p)),	/* Assign p */  
		ACS_SSL_InternalError("Cannot assign P")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*q = BN_bn2hex(key->q)),	/* Assign q */  
		ACS_SSL_InternalError("Cannot assign Q")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*dmp1 = BN_bn2hex(key->dmp1)),	/* Assign dmp1 */  
		ACS_SSL_InternalError("Cannot assign DMP1")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*dmq1 = BN_bn2hex(key->dmq1)),	/* Assign dmq1 */  
		ACS_SSL_InternalError("Cannot assign DMQ1")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*iqmp = BN_bn2hex(key->iqmp)),	/* Assign iqmp */  
		ACS_SSL_InternalError("Cannot assign IQMP")) ; 

#else

	const BIGNUM* bn_n ;					// No need to deallocate
	const BIGNUM* bn_e ;					// No need to deallocate
	const BIGNUM* bn_d ;					// No need to deallocate
	const BIGNUM* bn_p ;					// No need to deallocate
	const BIGNUM* bn_q ;					// No need to deallocate
	const BIGNUM* bn_dmp1 ;					// No need to deallocate
	const BIGNUM* bn_dmq1 ;					// No need to deallocate
	const BIGNUM* bn_iqmp ;					// No need to deallocate

	// Extract the values [no ownership]
	RSA_get0_key(key, &bn_n, &bn_e, &bn_d) ;				
	RSA_get0_factors(key, &bn_p, &bn_q) ;				
	RSA_get0_crt_params(key, &bn_dmp1, &bn_dmq1, &bn_iqmp) ;
   
	ACS_COND_THROW(  // PRQA S 3081
		!(*n = BN_bn2hex(bn_n)),	/* Assign n */ 
		ACS_SSL_InternalError("Cannot assign N")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*e = BN_bn2hex(bn_e)),	/* Assign e */  
		ACS_SSL_InternalError("Cannot assign E")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*d = BN_bn2hex(bn_d)),	/* Assign d */  
		ACS_SSL_InternalError("Cannot assign D")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*p = BN_bn2hex(bn_p)),	/* Assign p */  
		ACS_SSL_InternalError("Cannot assign P")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*q = BN_bn2hex(bn_q)),	/* Assign q */  
		ACS_SSL_InternalError("Cannot assign Q")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*dmp1 = BN_bn2hex(bn_dmp1)),	/* Assign dmp1 */  
		ACS_SSL_InternalError("Cannot assign DMP1")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*dmq1 = BN_bn2hex(bn_dmq1)),	/* Assign dmq1 */  
		ACS_SSL_InternalError("Cannot assign DMQ1")) ; 

 	ACS_COND_THROW(  // PRQA S 3081
		!(*iqmp = BN_bn2hex(bn_iqmp)),	/* Assign iqmp */  
		ACS_SSL_InternalError("Cannot assign IQMP")) ; 

#endif

	return ;
}


///////////////////////////////////////////////
//
//   DUMP RSA KEY 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_dump_key(	const RSA* key,		/* IN: RSA Key */			
        							bool isPriv)		/* IN: TRUE if Private Key */			
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    exFDebug dbg ;
    dbg << exDebugSignature << "ASSUMING TEST ENVIRONMENT. PLEASE REPORT TO ACS" ;
    excerr << dbg ;
#endif 
	
	/* Obtain parameters */
 	char *n = 0 ;
	char *e = 0 ;

#if OPENSSL_VERSION_NUMBER < 0x10100000L

    ACS_COND_THROW(  // PRQA S 3081
		!(n = BN_bn2hex(key->n)),	/* Assign n */ 
		ACS_SSL_InternalError("Cannot assign N")) ; 
	OPENSSLMemoryGuard nG(n) ;

 	ACS_COND_THROW(  // PRQA S 3081
		!(e = BN_bn2hex(key->e)),	/* Assign e */  
		ACS_SSL_InternalError("Cannot assign E")) ; 
	OPENSSLMemoryGuard eG(e) ;

#else

	const BIGNUM* bn_n ;					// No need to deallocate
	const BIGNUM* bn_e ;					// No need to deallocate
	
	// Extract the values [no ownership]
	RSA_get0_key(key, &bn_n, &bn_e, NULL) ;				
    
	ACS_COND_THROW(  // PRQA S 3081
		!(n = BN_bn2hex(bn_n)),	/* Assign n */ 
		ACS_SSL_InternalError("Cannot assign N")) ; 
	OPENSSLMemoryGuard nG(n) ;

 	ACS_COND_THROW(  // PRQA S 3081
		!(e = BN_bn2hex(bn_e)),	/* Assign e */  
		ACS_SSL_InternalError("Cannot assign E")) ; 
	OPENSSLMemoryGuard eG(e) ;

#endif


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    dbg << exDebugSignature << "     n: " << n ;
    excerr << dbg ;
    dbg << exDebugSignature << "     e: " << e ;
    excerr << dbg ;
#endif 


#if(0)
    #error "ACS_SSL::ACS_RSA_dump_key  NEVER ENABLE" // Decomment for test only.  ENRCAR:: DO NOT COMMIT IF THIS LINE CHANGED

	if ( isPriv )
    {
	    char *d = 0 ;
	    char *p = 0 ;
	    char *q = 0 ;
	    char *dmp1 = 0 ;
	    char *dmq1 = 0 ;
	    char *iqmp = 0 ;

#if OPENSSL_VERSION_NUMBER < 0x10100000L

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(d = BN_bn2hex((BIGNUM*)(key->d))),	/* Assign d */  
		    ACS_SSL_InternalError("Cannot assign D")) ; 
	    OPENSSLMemoryGuard dG(d) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(p = BN_bn2hex((BIGNUM*)(key->p))),	/* Assign p */  
		    ACS_SSL_InternalError("Cannot assign P")) ; 
	    OPENSSLMemoryGuard pG(p) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(q = BN_bn2hex((BIGNUM*)(key->q))),	/* Assign q */  
		    ACS_SSL_InternalError("Cannot assign Q")) ; 
	    OPENSSLMemoryGuard qG(q) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(dmp1 = BN_bn2hex((BIGNUM*)(key->dmp1))),	/* Assign dmp1 */  
		    ACS_SSL_InternalError("Cannot assign DMP1")) ; 
	    OPENSSLMemoryGuard dmp1G(dmp1) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(dmq1 = BN_bn2hex((BIGNUM*)(key->dmq1))),	/* Assign dmq1 */  
		    ACS_SSL_InternalError("Cannot assign DMQ1")) ; 
	    OPENSSLMemoryGuard dmq1G(dmq1) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(iqmp = BN_bn2hex((BIGNUM*)(key->iqmp))),	/* Assign iqmp */  
		    ACS_SSL_InternalError("Cannot assign IQMP")) ; 
	    OPENSSLMemoryGuard iqmpG(iqmp) ;

#else
		const BIGNUM* bn_d ;					// No need to deallocate
		const BIGNUM* bn_p ;					// No need to deallocate
		const BIGNUM* bn_q ;					// No need to deallocate
		const BIGNUM* bn_dmp1 ;					// No need to deallocate
		const BIGNUM* bn_dmq1 ;					// No need to deallocate
		const BIGNUM* bn_iqmp ;					// No need to deallocate

		// Extract the values [no ownership]
		RSA_get0_key(key, NULL, NULL, &bn_d) ;				
		RSA_get0_factors(key, &bn_p, &bn_q) ;				
		RSA_get0_crt_params(key, &bn_dmp1, &bn_dmq1, &bn_iqmp) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(d = BN_bn2hex(bn_d)),	/* Assign d */  
		    ACS_SSL_InternalError("Cannot assign D")) ; 
	    OPENSSLMemoryGuard dG(d) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(p = BN_bn2hex(bn_p)),	/* Assign p */  
		    ACS_SSL_InternalError("Cannot assign P")) ; 
	    OPENSSLMemoryGuard pG(p) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(q = BN_bn2hex(bn_q)),	/* Assign q */  
		    ACS_SSL_InternalError("Cannot assign Q")) ; 
	    OPENSSLMemoryGuard qG(q) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(dmp1 = BN_bn2hex(bn_dmp1)),	/* Assign dmp1 */  
		    ACS_SSL_InternalError("Cannot assign DMP1")) ; 
	    OPENSSLMemoryGuard dmp1G(dmp1) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(dmq1 = BN_bn2hex(bn_dmq1)),	/* Assign dmq1 */  
		    ACS_SSL_InternalError("Cannot assign DMQ1")) ; 
	    OPENSSLMemoryGuard dmq1G(dmq1) ;

 	    ACS_COND_THROW(  // PRQA S 3081
		    !(iqmp = BN_bn2hex(bn_iqmp)),	/* Assign iqmp */  
		    ACS_SSL_InternalError("Cannot assign IQMP")) ; 
	    OPENSSLMemoryGuard iqmpG(iqmp) ;

#endif

    #ifdef ACS_SSL_HAS_DEBUG_ENABLED
        exFDebug dbg ;
        dbg << exDebugSignature << "     d: " << d ;
        excerr << dbg ;
        dbg << exDebugSignature << "     p: " << p ;
        excerr << dbg ;
        dbg << exDebugSignature << "     q: " << q ;
        excerr << dbg ;
        dbg << exDebugSignature << "  dmp1: " << dmp1 ;
        excerr << dbg ;
        dbg << exDebugSignature << "  dmq1: " << dmq1 ;
        excerr << dbg ;
        dbg << exDebugSignature << "  iqmp: " << iqmp ;
        excerr << dbg ;
    #endif 
    }
#endif


	return ;
}


///////////////////////////////////////////////
//
//   ACS_RSA_init_PrivKeyStruct
//
///////////////////////////////////////////////
ACS_SSL::ACS_SSL_RSA_Priv* 
	ACS_SSL::ACS_RSA_init_PrivKeyStruct(ACS_SSL::ACS_SSL_RSA_Priv* key)
{
	if (! key) { 
        key = reinterpret_cast<ACS_SSL_RSA_Priv*>( malloc( sizeof(ACS_SSL_RSA_Priv) )) ; // PRQA S 3081
    }
    
 	ACS_COND_THROW( !key, ACS_SSL_InternalError("Memory Full!", errno) ) ;  // PRQA S 3081

	memset(key, 0, sizeof(ACS_SSL_RSA_Priv)) ;

    unsigned short t = 0x1234 ;	/* Endianness test */
    if ( *(reinterpret_cast<unsigned char*>(&t)) == 0x12 ) { // PRQA S 3030, 3081, 4400
        key->mpi_size_BE = ACS_SSL_mpi_maxsize ;	/* BIGEND */
    }
    else if ( *(reinterpret_cast<unsigned char*>(&t)) == 0x34 ) { // PRQA S 3030, 3081, 4400
        key->mpi_size_BE = ( ((ACS_SSL_mpi_maxsize & 0xff)<<8) | ((ACS_SSL_mpi_maxsize & 0xff00)>>8) ) ;	/* LITTLEEND */ // PRQA S 4400
    }
    else {
        ACS_THROW(ACS_SSL_InternalError("Endianess Error") ) ;  // PRQA S 3081, 4400
    }
     
    key->params = 8 ; // PRQA S 4400
  
  	return key ;
}

    
///////////////////////////////////////////////
//
//   ACS_RSA_init_PubKeyStruct
//
///////////////////////////////////////////////
ACS_SSL::ACS_SSL_RSA_Pub* 
	ACS_SSL::ACS_RSA_init_PubKeyStruct(ACS_SSL::ACS_SSL_RSA_Pub* key)
{
	if (! key) {
        key = reinterpret_cast<ACS_SSL_RSA_Pub*>( malloc( sizeof(ACS_SSL_RSA_Pub) )) ; // PRQA S 3081
    }
    
 	ACS_COND_THROW( !key, ACS_SSL_InternalError("Memory Full!", errno) ) ;  // PRQA S 3081

	memset(key, 0, sizeof(ACS_SSL_RSA_Pub)) ;

    unsigned short t = 0x1234 ;	/* Endianness test */
    if ( *(reinterpret_cast<unsigned char*>(&t)) == 0x12 ) { // PRQA S 3030, 3081, 4400
        key->mpi_size_BE = ACS_SSL_mpi_maxsize ;	/* BIGEND */
    }
    else if ( *( reinterpret_cast<unsigned char*>(&t)) == 0x34 ) { // PRQA S 3030, 3081, 4400
        key->mpi_size_BE = ( ((ACS_SSL_mpi_maxsize & 0xff)<<8) | ((ACS_SSL_mpi_maxsize & 0xff00)>>8) ) ;	/* LITTLEEND */ // PRQA S 4400
    }
    else {
        ACS_THROW(ACS_SSL_InternalError("Endianess Error") ) ;  // PRQA S 3081
    }
    key->params = 2 ; // PRQA S 4400
  
  	return key ;
}


///////////////////////////////////////////////
//
//   ACS_RSA_get_PubKeyStruct
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_get_PubKeyStruct(	ACS_SSL_RSA_Pub* keyStruct,	/* OUTPUT: Filled ACS_SSL_RSA structure */ 
                                            const RSA* key)				/* INPUT: RSA Key */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "mpi_size: 0x" << hex << keyStruct->mpi_size_BE ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
        
    ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;   // PRQA S 3081
    ACS_COND_THROW(!keyStruct, ACS_SSL_InternalError("Invalid output pointer")) ;   // PRQA S 3081

	int ret ;

#if OPENSSL_VERSION_NUMBER < 0x10100000L

	ret = BN_bn2mpi(key->n, keyStruct->n) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting n")) ;   // PRQA S 3081

	ret = BN_bn2mpi(key->e, keyStruct->e) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081
#else
	
	const BIGNUM* bn_n ;					// No need to deallocate
	const BIGNUM* bn_e ;					// No need to deallocate
	
	// Extract the values [no ownership]
	RSA_get0_key(key, &bn_n, &bn_e, NULL) ;				

	ret = BN_bn2mpi(bn_n, keyStruct->n) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting n")) ;   // PRQA S 3081

	ret = BN_bn2mpi(bn_e, keyStruct->e) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081

#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
}


///////////////////////////////////////////////
//
//   ACS_RSA_get_PrivKeyStruct
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_get_PrivKeyStruct(	ACS_SSL_RSA_Priv* keyStruct,		/* OUTPUT: Filled ACS_SSL_RSA structure */ 
                                            const RSA* key)				/* INPUT: RSA Key */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "mpi_size: 0x" << hex << keyStruct->mpi_size_BE ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
        
    ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid input key")) ;   // PRQA S 3081
    ACS_COND_THROW(!keyStruct, ACS_SSL_InternalError("Invalid output pointer")) ;   // PRQA S 3081
	
	int ret ;

#if OPENSSL_VERSION_NUMBER < 0x10100000L

	ret = BN_bn2mpi(key->n, keyStruct->n) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting n")) ;  // PRQA S 3081 

	ret = BN_bn2mpi(key->e, keyStruct->e) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081
   
    ret = BN_bn2mpi(key->d, keyStruct->d) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting d")) ;   // PRQA S 3081

    ret = BN_bn2mpi(key->p, keyStruct->p) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting p")) ;   // PRQA S 3081

    ret = BN_bn2mpi(key->q, keyStruct->q) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting q")) ;   // PRQA S 3081

    ret = BN_bn2mpi(key->dmp1, keyStruct->dmp1) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting dmp1")) ;   // PRQA S 3081

    ret = BN_bn2mpi(key->dmq1, keyStruct->dmq1) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting dmq1")) ;   // PRQA S 3081

    ret = BN_bn2mpi(key->iqmp, keyStruct->iqmp) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting iqmp")) ;   // PRQA S 3081

#else
	const BIGNUM* bn_n ;					// No need to deallocate
	const BIGNUM* bn_e ;					// No need to deallocate
	const BIGNUM* bn_d ;					// No need to deallocate
	const BIGNUM* bn_p ;					// No need to deallocate
	const BIGNUM* bn_q ;					// No need to deallocate
	const BIGNUM* bn_dmp1 ;					// No need to deallocate
	const BIGNUM* bn_dmq1 ;					// No need to deallocate
	const BIGNUM* bn_iqmp ;					// No need to deallocate

	// Extract the values [no ownership]
	RSA_get0_key(key, &bn_n, &bn_e, &bn_d) ;				
	RSA_get0_factors(key, &bn_p, &bn_q) ;				
	RSA_get0_crt_params(key, &bn_dmp1, &bn_dmq1, &bn_iqmp) ;
 
	ret = BN_bn2mpi(bn_n, keyStruct->n) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting n")) ;  // PRQA S 3081 

	ret = BN_bn2mpi(bn_e, keyStruct->e) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081
   
    ret = BN_bn2mpi(bn_d, keyStruct->d) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting d")) ;   // PRQA S 3081

    ret = BN_bn2mpi(bn_p, keyStruct->p) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting p")) ;   // PRQA S 3081

    ret = BN_bn2mpi(bn_q, keyStruct->q) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting q")) ;   // PRQA S 3081

    ret = BN_bn2mpi(bn_dmp1, keyStruct->dmp1) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting dmp1")) ;   // PRQA S 3081

    ret = BN_bn2mpi(bn_dmq1, keyStruct->dmq1) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting dmq1")) ;   // PRQA S 3081

    ret = BN_bn2mpi(bn_iqmp, keyStruct->iqmp) ; 
    ACS_COND_THROW( (ret<1) || (ret>ACS_SSL_mpi_maxsize), ACS_SSL_InternalError("Error converting iqmp")) ;   // PRQA S 3081

#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
}


///////////////////////////////////////////////
//
//   ACS_RSA_set_PubKeyStruct 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_set_PubKeyStruct(	RSA* key,							/* OUTPUT: Pre-allocated RSA Key */ 
                                            const ACS_SSL_RSA_Pub* keyStruct)	/* INPUT: ACS_SSL_RSA structure */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "mpi_size: 0x" << hex << keyStruct->mpi_size_BE ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
        
    ACS_COND_THROW(!keyStruct, ACS_SSL_InternalError("Invalid input keyStruct")) ;   // PRQA S 3081
    ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid output key")) ;   // PRQA S 3081

#if OPENSSL_VERSION_NUMBER < 0x10100000L

    key->n = ACS_SSL_mpi2bn(keyStruct->n, ACS_SSL_mpi_maxsize, key->n) ;
    ACS_COND_THROW(!key->n, ACS_SSL_InternalError("Error converting n")) ;   // PRQA S 3081

    key->e = ACS_SSL_mpi2bn(keyStruct->e, ACS_SSL_mpi_maxsize, key->e) ;
    ACS_COND_THROW(!key->e, ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081

#else

    BIGNUM* bn_n = ACS_SSL_mpi2bn(keyStruct->n, ACS_SSL_mpi_maxsize, NULL) ;	// Generate a new BIGNUM, it will be deallocated by the key handler
    ACS_COND_THROW(!bn_n, ACS_SSL_InternalError("Error converting n")) ;   // PRQA S 3081

    BIGNUM* bn_e = ACS_SSL_mpi2bn(keyStruct->e, ACS_SSL_mpi_maxsize, NULL) ;	// Generate a new BIGNUM, it will be deallocated by the key handler
    ACS_COND_THROW(!bn_e, ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081
	
	ACS_COND_THROW( // PRQA S 3081
		RSA_set0_key(key, bn_n, bn_e, NULL) < 1,	
		ACS_SSL_InternalError("Cannot assign n/e")) ; 

#endif 

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
}


///////////////////////////////////////////////
//
//   ACS_RSA_set_PrivKeyStruct 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_set_PrivKeyStruct(	RSA* key,							/* OUTPUT: Pre-allocated RSA Key */ 
                                            const ACS_SSL_RSA_Priv* keyStruct)	/* INPUT: ACS_SSL_RSA structure */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "mpi_size: 0x" << hex << keyStruct->mpi_size_BE ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
        
    ACS_COND_THROW(!keyStruct, ACS_SSL_InternalError("Invalid input keyStruct")) ;   // PRQA S 3081
    ACS_COND_THROW(!key, ACS_SSL_InternalError("Invalid output key")) ;   // PRQA S 3081

#if OPENSSL_VERSION_NUMBER < 0x10100000L

    key->n = ACS_SSL_mpi2bn(keyStruct->n, ACS_SSL_mpi_maxsize, key->n) ;
    ACS_COND_THROW(!key->n, ACS_SSL_InternalError("Error converting n")) ;   // PRQA S 3081

    key->e = ACS_SSL_mpi2bn(keyStruct->e, ACS_SSL_mpi_maxsize, key->e) ;
    ACS_COND_THROW(!key->e, ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081
     
    key->d = ACS_SSL_mpi2bn(keyStruct->d, ACS_SSL_mpi_maxsize, key->d) ;
    ACS_COND_THROW(!key->d, ACS_SSL_InternalError("Error converting d")) ;   // PRQA S 3081

    key->p = ACS_SSL_mpi2bn(keyStruct->p, ACS_SSL_mpi_maxsize, key->p) ;
    ACS_COND_THROW(!key->p, ACS_SSL_InternalError("Error converting p")) ;   // PRQA S 3081

    key->q = ACS_SSL_mpi2bn(keyStruct->q, ACS_SSL_mpi_maxsize, key->q) ;
    ACS_COND_THROW(!key->q, ACS_SSL_InternalError("Error converting q")) ;   // PRQA S 3081

    key->dmp1 = ACS_SSL_mpi2bn(keyStruct->dmp1, ACS_SSL_mpi_maxsize, key->dmp1) ;
    ACS_COND_THROW(!key->dmp1, ACS_SSL_InternalError("Error converting dmp1")) ;   // PRQA S 3081

    key->dmq1 = ACS_SSL_mpi2bn(keyStruct->dmq1, ACS_SSL_mpi_maxsize, key->dmq1) ;
    ACS_COND_THROW(!key->dmq1, ACS_SSL_InternalError("Error converting dmq1")) ;   // PRQA S 3081

    key->iqmp = ACS_SSL_mpi2bn(keyStruct->iqmp, ACS_SSL_mpi_maxsize, key->iqmp) ;
    ACS_COND_THROW(!key->iqmp, ACS_SSL_InternalError("Error converting iqmp")) ;   // PRQA S 3081

#else

    BIGNUM* bn_n = ACS_SSL_mpi2bn(keyStruct->n, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_n, ACS_SSL_InternalError("Error converting n")) ;   // PRQA S 3081

    BIGNUM* bn_e = ACS_SSL_mpi2bn(keyStruct->e, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_e, ACS_SSL_InternalError("Error converting e")) ;   // PRQA S 3081
     
    BIGNUM* bn_d = ACS_SSL_mpi2bn(keyStruct->d, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_d, ACS_SSL_InternalError("Error converting d")) ;   // PRQA S 3081

    BIGNUM* bn_p = ACS_SSL_mpi2bn(keyStruct->p, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_p, ACS_SSL_InternalError("Error converting p")) ;   // PRQA S 3081

    BIGNUM* bn_q = ACS_SSL_mpi2bn(keyStruct->q, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_q, ACS_SSL_InternalError("Error converting q")) ;   // PRQA S 3081

    BIGNUM* bn_dmp1 = ACS_SSL_mpi2bn(keyStruct->dmp1, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_dmp1, ACS_SSL_InternalError("Error converting dmp1")) ;   // PRQA S 3081

    BIGNUM* bn_dmq1 = ACS_SSL_mpi2bn(keyStruct->dmq1, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_dmq1, ACS_SSL_InternalError("Error converting dmq1")) ;   // PRQA S 3081

    BIGNUM* bn_iqmp = ACS_SSL_mpi2bn(keyStruct->iqmp, ACS_SSL_mpi_maxsize, NULL) ;
    ACS_COND_THROW(!bn_iqmp, ACS_SSL_InternalError("Error converting iqmp")) ;   // PRQA S 3081


 	ACS_COND_THROW( // PRQA S 3081
		RSA_set0_key(key, bn_n, bn_e, NULL) < 1,	
		ACS_SSL_InternalError("Cannot assign n/e")) ; 

 	ACS_COND_THROW( // PRQA S 3081
		RSA_set0_factors(key, bn_p, bn_q) < 1,	
		ACS_SSL_InternalError("Cannot assign p/q")) ; 

	 ACS_COND_THROW( // PRQA S 3081
		RSA_set0_crt_params(key, bn_dmp1, bn_dmq1, bn_iqmp) < 1,	
		ACS_SSL_InternalError("Cannot assign dmp1/dmq1/iqmp")) ; 

#endif


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
}


///////////////////////////////////////////////
//
//   ACS_RSA_reset_PubKeyStruct 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_reset_PubKeyStruct(ACS_SSL_RSA_Pub* keyStruct)	/* INPUT: ACS_SSL_RSA structure */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

    ACS_COND_THROW(!keyStruct, ACS_SSL_InternalError("Invalid input keyStruct")) ;   // PRQA S 3081
   
	memset(keyStruct, 0, sizeof(*keyStruct)) ;	// Erase content from memory

}


///////////////////////////////////////////////
//
//   ACS_RSA_reset_PrivKeyStruct 
//
///////////////////////////////////////////////
void 
		ACS_SSL::ACS_RSA_reset_PrivKeyStruct(ACS_SSL_RSA_Priv* keyStruct)	/* INPUT: ACS_SSL_RSA structure */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
        
    ACS_COND_THROW(!keyStruct, ACS_SSL_InternalError("Invalid input keyStruct")) ;   // PRQA S 3081
   
	memset(keyStruct, 0, sizeof(*keyStruct)) ;	// Erase content from memory

}


///////////////////////////////////////////////
//
//    ACS_RSA_getExpected_buffersize
//
///////////////////////////////////////////////
off_t 																		/* Compute the size AFTER ENCODING. E.G. if n=1024, bytes_in = 5 -> buffer_out = 256  */
				ACS_SSL::ACS_RSA_getExpected_buffersize(const int& n,		// bits: E.G. 2048 
														const off_t& bytes_in,
														const int& padding_type )
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "  n: " << n << "  bytes_in: " << bytes_in ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW((n % 8), ACS_SSL_InternalError("Invalid value for \"n\"")) ;  

	off_t bytes_out ;
	
	if (n >= 1)
	{
		const off_t block_size ( n / 8 ) ;
		off_t max_block (1) ; 

		switch (padding_type)
		{ 
			case RSA_PKCS1_PADDING :
				ACS_COND_THROW((block_size < 11), ACS_SSL_InternalError("n too low for the selected padding !")) ;  
				max_block = block_size - 11 ; 
				break ;
			case RSA_PKCS1_OAEP_PADDING	:
				ACS_COND_THROW((block_size < 41), ACS_SSL_InternalError("n too low for the selected padding !")) ;  
				max_block = block_size - 41 ; 
				break ;
			case RSA_NO_PADDING	:
				max_block = block_size - 0 ;
				break ;
			default:
				ACS_THROW( ACS_SSL_InternalError("Invalid padding !") ) ;  
		}

		const off_t num_blocks ( (bytes_in>=1)? ( 1 + ((bytes_in-1) / max_block) ) : 0) ;

		bytes_out = (num_blocks * block_size) ;
		
	} else {
		// if (n==0) no RSA cipher was chosen -- in this case the same buffer would be returned (assumed that a "void cipher" will be used)
		bytes_out = bytes_in ;
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "  Method finished. Returning: " << bytes_out ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return bytes_out ;
}


///////////////////////////////////////////////
//
//   FillBufferFromHex 
//
///////////////////////////////////////////////
unsigned int 																/* OUT: bytes written into buffer */
				ACS_SSL::ACS_RSA_fillBufferFromHex( const char* s,		    /* IN: Pointer to a BinAscii string */ // PRQA S 4020
                                                    unsigned char* d,  		/* IN/OUT: Binary buffer (dest.) */
                                                    unsigned int buflen ) 	/* IN: (preallocated) buffer size */
{
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

	if ((!s) || (!d) || (0 == buflen) ) { return 0 ; }
	
    memset(d, 0, buflen) ;
    
    size_t mx = ((strlen(s)/2) > buflen)? buflen : (strlen(s)/2)  ;	/* Number of bytes to be transferred in output buffer */ // PRQA S 3084, 3384, 3385

	unsigned int r = 0 ;
    
	for (size_t i=0; i<2*mx; i+=2) // PRQA S 3084, 4400
	{
		char t[3];  // PRQA S 4403
		t[0] = s[i] ; t[1] = s[i+1] ; t[2] = 0 ;	/* A substring for each HEX couple is taken */ // PRQA S 3084, 4400

		if ( (t[0] >= 'a') && (t[0] <= 'f') ) { t[0] -= 0x20 ; } /* from [a-f] to [A-F] */ // PRQA S 4400 2
		if ( (t[1] >= 'a') && (t[1] <= 'f') ) { t[1] -= 0x20 ; } /* from [a-f] to [A-F] */

		int ti = 0 ;

		sscanf(t, "%02X", &ti) ;	/* From HEX to int decimal (e.g. "4B" to 75, that's 'K') */	
		
		d[i/2] = static_cast<unsigned char> (ti & 0xff);	/* Stored into buffer */ // PRQA S 3003, 3081, 3084, 4400
		r ++ ;
	}
	
	return r ;
}


////////////////////////////////////////////////////////////
//
//   checkSaltIsConfigured
//
////////////////////////////////////////////////////////////
bool ACS_SSL::checkSaltIsConfigured(size_t& saltRandom, string& saltString) 
{

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method called." ; 
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	// Default, empty values (meaning: no salt)
	saltString = "" ; 
	saltRandom = 0 ;

	// Step 1 of 2: Acquire salt configuration from "-D" (defines) in common makefile:
	
	// First, check if both macros (which are mutually exclusive) are defined. This is a fatal error.
#if defined CIPHER_SALT_RANDOMSIZE		
	#if CIPHER_SALT_RANDOMSIZE != 0
		#if defined CIPHER_SALT_STRING		
			#if (ACS_SSL_MACRO_EXPAND(CIPHER_SALT_STRING) != 1)
				#error "Compile-time error in libACS_SSL: the mutually-exclusive defines for salt are both defined"
			#endif
		#endif
	#endif
#endif	
	
#if defined CIPHER_SALT_STRING		
	// Look for: -DCIPHER_SALT_STRING="string"
	// Remove the double-quotes (") from the string. Note that the define MUST BE WRITTEN WITH DOUBLE-QUOTES in this way, e.g. -DCIPHER_SALT_STRING="ANY SALT"
	// Also note that the escape (\") SHOULD NOT BE USED: (e.g. -DCIPHER_SALT_STRING=\"THIS IS WRONG\") [ DO NOT USE ESCAPE !!!!!! ]
	saltString = ACS_SSL_MACRO_STRINGIFY( CIPHER_SALT_STRING ) ;
#endif

#if defined CIPHER_SALT_RANDOMSIZE	
	// Look for: -DCIPHER_SALT_RANDOMSIZE=integer
	saltRandom = CIPHER_SALT_RANDOMSIZE ;
#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
    exFDebug dbg ;
    dbg << exDebugSignature << "Salt after parsing compile-time parameters: saltString: \"" << saltString << "\"  saltRandom: " << saltRandom << endl ;
    excerr << dbg ;
ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW( (not saltString.empty() && (saltRandom > 0)), exIllegalValueException("MISCONFIGURATION: both salt string and salt size valorized at compile-time, just one expected!") ) ; // PRQA S 3081

#if(0)		
	const rsResourceSet rs ( ConfigurationSingleton::instance()->get() ) ;

	const string key_salt_string ("SaltCipher.SaltString") ;
	const string key_salt_random ("SaltCipher.SaltRandom") ;

	if( true == rs.hasKey(key_salt_string) ) {
		rs.getValue(key_salt_string, saltString);
	}

	if( true == rs.hasKey(key_salt_random) ) {
		string tmp ; 
		rs.getValue(key_salt_random, tmp);
		saltRandom = ( tmp.empty() ? 0 : atoi(tmp.c_str()) ) ; 
	}
#endif

	if ( true == getSaltSet() )
	{
		bool tmp ; 
		saltString = getSaltString(tmp) ; 
		saltRandom = getSaltRandom(tmp) ; 	
	}


	ACS_COND_THROW( (not saltString.empty() && (saltRandom > 0)), exIllegalValueException("MISCONFIGURATION: both salt string and salt size valorized in configuration, just one expected!") ) ; // PRQA S 3081


	// Check if, eventually, some salt has been set:	
	const bool add_salt ( !saltString.empty() || (saltRandom > 0) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished. Returning: " << (add_salt? "T":"F") << endl ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return add_salt ; 
}


////////////////////////////////////////////////////////////
//
//   ACS_RSA_encode_password
//
////////////////////////////////////////////////////////////
bool ACS_SSL::ACS_RSA_encode_password(	const unsigned char* input_buffer_in,	/* IN: input buffer */ 
										const size_t& input_buffer_size_in,		/* IN: input buffer size */  
										const RSA* key,							/* IN: RSA key */   
										unsigned char* output_buffer,			/* IN: preallocated output buffer */    
										size_t& output_buffer_size) 			/* IN: the size of the total preallocated buffer. OUT: the size of the ciphered buffer */
{
	/**
		Specific method only to encipher ACS passwords. THIS IS NOT A GENERIC MULTI-PURPOSE RSA ENCODER. 
		This also automatically takes into account the salt.
		The Output buffer MUST be preallocated (at a reasonable size, e.g. 4096 bytes are enough for one password...). The actual size will be returned to the caller.
		An exception will be returned if the preallocated output buffer is too short.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method called. input_buffer_size: " << input_buffer_size_in << "  output_buffer_size: " << output_buffer_size ; 
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	bool success (false) ; 
	size_t input_buffer_size (input_buffer_size_in) ; // local copy
	
	ACS_COND_THROW ( ((NULL == input_buffer_in) && (input_buffer_size>0)), exIllegalValueException("Invalid input buffer")) ;
	
	if ( (input_buffer_in != NULL) && (input_buffer_size > 0) )
	{
		ACS_COND_THROW ( (NULL == key), exIllegalValueException("NULL Key pointer")) ;
		ACS_COND_THROW ( (NULL == output_buffer), exIllegalValueException("Invalid output buffer")) ;

		const size_t temp_buffer_size( 32*1024 ) ; // 32 KB -- more than enough to store a password ... ...
		unsigned char temp_buffer [ temp_buffer_size ] ; 

		// Default, empty values (meaning: no salt)
		string saltString ("") ; 
		size_t saltRandom (0) ;

		const bool add_salt ( checkSaltIsConfigured(saltRandom, saltString) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
        	exFDebug dbg ;
        	dbg << exDebugSignature << "saltString: \"" << saltString << "\"  saltRandom: " << saltRandom  << "  Eventually add_salt: " << (add_salt?"T":"F") << endl ;
        	excerr << dbg ;
    	ACS_CLASS_END_DEBUG
#endif 

		if ( true == add_salt )
		{
			ACS_COND_THROW( (not saltString.empty() && (saltRandom > 0)), exIllegalValueException("MISCONFIGURATION: both salt string and salt size valorized, just one expected!") ) ; // PRQA S 3081
			
			size_t saltSize(saltRandom) ;

			if (0 == saltRandom) { saltSize = saltString.length() ; }		
						
			unsigned char coded_salt_buffer [ temp_buffer_size ] ; 
			
			encode_salt_buffer ( reinterpret_cast<const unsigned char*>(saltString.c_str()), saltString.length(), saltSize, coded_salt_buffer, sizeof(coded_salt_buffer)) ; 
			
			/* 
				Compute the new input_buffer_size (has the length and content of (salt+buffer)).
				Note that as salt-padding algorithm ENC_BLOWFISH (and _NOT_ RSA) is used.
				The reason is simple: RSA is not a block cipher, therefore the salt could be not padded. 
				Accordingly to known standards, a 8-bytes padded is enforced (so: 1 -> 8, 11 -> 16 and on), which can be ontained using the ENC_BLOWFISH, indeed.
			*/
			unsigned char *ptr (temp_buffer) ; // Requested, since a pointer of pointer is required by the API, in order to handle automatic memory allocation. 
			const EncryptionAlgo padding_algo ( static_cast<EncryptionAlgo>(ENC_BLOWFISH | ENC_PREPEND_SALT) ) ;
			input_buffer_size = embed_salt_in_buffer (coded_salt_buffer, padding_algo, input_buffer_in, input_buffer_size_in, &ptr, temp_buffer_size) ;		
		}

		const unsigned char* input_buffer ( (true == add_salt) ? temp_buffer : input_buffer_in ) ;  

		const bool is_private (true) ; const bool emitExcept (true) ; 

		unsigned int returnLength ( static_cast<unsigned int>(output_buffer_size) ) ; // make up the interface [before] ...

		ACS_RSA_encode(input_buffer, input_buffer_size, key, is_private, output_buffer, &returnLength, emitExcept, &success) ;
		output_buffer_size = static_cast<size_t>(returnLength) ;	// make up the interface [after] ...

	} else {
	
		// Empty input buffer, nothing to do: virtually, success is reached ...
		success = true ; 
		output_buffer_size = 0 ; 
	
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished. Returning: " << (success?"T":"F") << "  output_buffer_size: " << output_buffer_size ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return success ; 
}


////////////////////////////////////////////////////////////
//
//   ACS_RSA_decode_password
//
////////////////////////////////////////////////////////////
bool ACS_SSL::ACS_RSA_decode_password(	const unsigned char* input_buffer,		/* IN: input buffer */ 
										const size_t& input_buffer_size	,		/* IN: input buffer size */  
										const RSA* key,							/* IN: RSA key */   
										unsigned char* output_buffer,			/* IN: preallocated output buffer */    
										size_t& output_buffer_size) 			/* IN: the size of the total preallocated buffer. OUT: the size of the ciphered buffer */
{
	/**
		Specific method only to decipher ACS passwords. THIS IS NOT A GENERIC MULTI-PURPOSE RSA DECODER. 
		This also automatically takes into account the salt.
		The Output buffer MUST be preallocated (at a reasonable size, e.g. 4096 bytes are enough for one password...). The actual size will be returned to the caller.
		An exception will be returned if the preallocated output buffer is too short.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method called. input_buffer_size: " << input_buffer_size << "  output_buffer_size: " << output_buffer_size ; 
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	bool success (false) ; 

	ACS_COND_THROW ( ((NULL == input_buffer) && (input_buffer_size>0)), exIllegalValueException("Invalid input buffer")) ;
	
	// Default, empty values (meaning: no salt)
	string saltString ("") ; 
	size_t saltRandom (0) ;

	const bool remove_salt ( checkSaltIsConfigured(saltRandom, saltString) ) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
        exFDebug dbg ;
        dbg << exDebugSignature << "saltString: \"" << saltString << "\"  saltRandom: " << saltRandom  << "  Eventually remove_salt: " << (remove_salt?"T":"F") << endl ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	if ( (input_buffer != NULL) && (input_buffer_size > 0) )
	{
		ACS_COND_THROW ( (NULL == key), exIllegalValueException("NULL Key pointer")) ;
		ACS_COND_THROW ( (NULL == output_buffer), exIllegalValueException("Invalid output buffer")) ;

		const bool is_private (false) ; const bool emitExcept (true) ; 

		unsigned int returnLength ( static_cast<unsigned int>(output_buffer_size) ) ; // make up the interface [before] ...
		ACS_RSA_decode(input_buffer, input_buffer_size, key, is_private, output_buffer, &returnLength, emitExcept, &success) ;
		output_buffer_size = static_cast<size_t>(returnLength) ;	// make up the interface [after] ...
		
		if (true == remove_salt) 
		{
			/* 
				Compute the new output_buffer_size (with the salt stripped away).
				Note that as salt-padding algorithm ENC_BLOWFISH (and _NOT_ RSA) is used.
				The reason is simple: RSA is not a block cipher, therefore the salt could be not padded. 
				Accordingly to known standards, a 8-bytes padded is enforced (so: 1 -> 8, 11 -> 16 and on), which can be ontained using the ENC_BLOWFISH, indeed.
			*/			
			
			size_t saltSize(saltRandom) ;
			if (0 == saltRandom) { saltSize = saltString.length() ; }		

			const EncryptionAlgo padding_algo ( static_cast<EncryptionAlgo>(ENC_BLOWFISH | ENC_PREPEND_SALT) ) ;
			output_buffer_size = remove_salt_from_buffer (saltSize, padding_algo, output_buffer, output_buffer_size, output_buffer, output_buffer_size) ;	
		}

	} else {
	
		// Empty input buffer, nothing to do: virtually, success is reached ...
		success = true ; 
		output_buffer_size = 0 ; 
	
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Method finished. Returning: " << (success?"T":"F") << "  output_buffer_size: " << output_buffer_size ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return success ; 
}


////////////////////////////////////////////////////////////
//
//   ACS_SSL_mpi2bn
//
////////////////////////////////////////////////////////////
BIGNUM *
	ACS_SSL::ACS_SSL_mpi2bn (	const unsigned char *s,	/* INPUT BUFFER */ 
    							int buflen,			/* INPUT BUFFER LENGTH (MUST BE >= MPI SIZE) */ 
                                BIGNUM *ret)		/* OUTPUT VALUE. Must be a pre-allocated BIGNUM, or NULL to allocate one */ 
{
    /*
	    Wraps method BN_mpi2bn that is UNABLE to deal with length != mpi_size.
    */

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(200) 
        exFDebug dbg ;
        dbg << exDebugSignature << "buflen: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

    ACS_COND_THROW( (buflen<4), ACS_SSL_InternalError("Invalid buffer size") ) ;  // PRQA S 3081

	int mpi_size = (int(s[0]) << 24) | (int(s[1]) << 16) | (int(s[2]) << 8) | s[3] ; // PRQA S 3003, 3081, 4400


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "mpi_size: 0x" << hex << mpi_size ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
	ACS_CLASS_BGN_DEBUG(200) 
		for (int i=0; i<(mpi_size+4); ++i) // PRQA S 4400
        {
            exFDebug dbg ;
            dbg << exDebugSignature << "DUMP: " << i << ":  0x" << hex << setfill('0') << setw(2) << int(s[i]) ; // PRQA S 3081, 4400
            excerr << dbg ;
        }
	ACS_CLASS_END_DEBUG
#endif 
    
    ACS_COND_THROW( (buflen<(mpi_size+4)), ACS_SSL_InternalError("Buffer size too short") ) ;  // PRQA S 3081

	return BN_mpi2bn (s, (mpi_size+4), ret) ; // PRQA S 4400

}


///////////////////////////////////////////////
//
//   EVALUATE CRC AS BIGENDIAN BUFFER OF CHARS
//
///////////////////////////////////////////////
unsigned short int                                      /* OUT: CRC */
ACS_SSL::ACS_CRC16_eval(unsigned char* crc2bytes,       /* OUT: CRC */
						const unsigned char* buffer,     /* IN_buffer */
						unsigned int offset,            /* IN_offset */
						unsigned int buflen)            /* IN_buffer length */
{
	// crc2bytes must be a uchar vector[2], allocated by user
	// WARNING: CRC copied vector is ALWAYS BIGENDIAN
	// (So the buffer will the same on both BIG/LITTLE ENDIAN archit.)
    unsigned short int retValue = 0;
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "offset=" << offset << " buflen=" << buflen ;        
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
    
    
	retValue = ACS_CRC16_eval(buffer, offset, buflen);

// PRQA S 3000, 3007, 3010, 4400 4
    // Store as BIGENDIAN
	crc2bytes[0] = (retValue & 0xff00) >> 8 ;
	crc2bytes[1] = (retValue & 0x00ff) ;


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Return value as short 0x" << hex << retValue    
            << " Return value B_END as two chars: !" << static_cast<unsigned int>(crc2bytes[0]) << "!"  // PRQA S 3081 2
            << static_cast<unsigned int>(crc2bytes[1]) << dec << "!  (Must be the same value)" ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return retValue ;
}


///////////////////////////////////////////////
//
//   EVALUATE CRC AS NUMBER 
//
///////////////////////////////////////////////
unsigned short int                                  /* OUT: CRC */
ACS_SSL::ACS_CRC16XM_eval(const unsigned char* buffer,		/* IN_buffer */
						unsigned int offset, 		/* IN_offset */
						unsigned int buflen) 		/* IN_buffer length */
{
	unsigned short retVal = 0 ;
	// WARNING: since the function returns a NUMBER (not a data buffer)
    // there are NO BIG/LITTLE ENDIAN issues.
    // (The number will be the same on both archit.)
    // To store the value into a BIGENDIAN BUFFER, use method
    // ACS_SSL::ACS_CRC16XM_eval(unsigned char*, ...)
    
    
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "offset=" << offset << " buflen=" << buflen ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
 	
	
	// Create CRC structure: 
    crcmodel* p_cm = reinterpret_cast<crcmodel*>( calloc( 1, sizeof(crcmodel) )); // PRQA S 3081
 	ACS_COND_THROW( !p_cm, ACS_SSL_InternalError("Memory Full!", errno) ) ;  // PRQA S 3081

	pattern::CMemoryScopeGuard p_cm_guard(p_cm) ; 
	
	// Setup & Init CRC structure:
    crc_setup( p_cm, 16, 0x00001021, 0x00000000, false, false, 0 ); // PRQA S 4400
	cm_ini(p_cm);    

	// Apply CRC to data buffer:
    cm_blk ( p_cm, buffer+offset, buflen ); // PRQA S 3084

	// Get return value:
	retVal = cm_crc(p_cm) ; // PRQA S 3010

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Return value: 0x" << hex << retVal ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

    // No free() required since MemoryGuard is used
    
	return retVal ;
}

///////////////////////////////////////////////
//
//   EVALUATE CRC AS NUMBER 
//
///////////////////////////////////////////////
unsigned short int                                  /* OUT: CRC */
ACS_SSL::ACS_CRC16XM_eval(const char* buffer,        /* IN_buffer */
						unsigned int offset, 		/* IN_offset */
						unsigned int buflen) 		/* IN_buffer length */
{
    return ACS_CRC16XM_eval(reinterpret_cast<const unsigned char *>(buffer), offset, buflen) ; // PRQA S 3030, 3081
}
	
///////////////////////////////////////////////
//
//   EVALUATE CRC AS BIGENDIAN BUFFER OF CHARS
//
///////////////////////////////////////////////
unsigned short int                                      /* OUT: CRC */
ACS_SSL::ACS_CRC16XM_eval(unsigned char* crc2bytes,       /* OUT: CRC */
						const unsigned char* buffer,     /* IN_buffer */
						unsigned int offset,            /* IN_offset */
						unsigned int buflen)            /* IN_buffer length */
{
	// crc2bytes must be a uchar vector[2], allocated by user
	// WARNING: CRC copied vector is ALWAYS BIGENDIAN
	// (So the buffer will the same on both BIG/LITTLE ENDIAN archit.)
    unsigned short int retValue = 0;
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "offset=" << offset << " buflen=" << buflen ;        
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 
    
    
	retValue = ACS_CRC16XM_eval(buffer, offset, buflen);

// PRQA S 3000, 3007, 3010, 4400 4
    // Store as BIGENDIAN
	crc2bytes[0] = (retValue & 0xff00) >> 8 ;
	crc2bytes[1] = (retValue & 0x00ff) ;


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Return value as short 0x" << hex << retValue    
            << " Return value B_END as two chars: !" << static_cast<unsigned int>(crc2bytes[0]) << "!"  // PRQA S 3081 2
            << static_cast<unsigned int>(crc2bytes[1]) << dec << "!  (Must be the same value)" ;
        excerr << dbg ;
    ACS_CLASS_END_DEBUG
#endif 

	return retValue ;
}


////////////////////////////////////////////////////////////
//
//   CONVERT THE KEY FROM A CHAR BUFFER TO A KEY_SCHEDULE
//
////////////////////////////////////////////////////////////
void 
	ACS_SSL::ACS_DES_setkey(
			const_DES_cblock* keyin_ptr,	/* IN: Key (Binary Format, as vector of uchars) */ 
			DES_key_schedule* keyout_ptr )	/* OUT: Key (Architecture-Specific Schedule Format) */ // PRQA S 2010
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

#if OPENSSL_VERSION_NUMBER < 0x10100000L
	des_set_key_unchecked(keyin_ptr, *keyout_ptr) ;
#else
	DES_set_key_unchecked(keyin_ptr, keyout_ptr) ;
#endif

	return ; 
}



////////////////////////////////////////////////////////////
//
//   DES ENCRYPT
//
////////////////////////////////////////////////////////////

size_t
	ACS_SSL::ACS_DES_encrypt1(
					const unsigned char* bufin,	/* IN: Input Buffer (Cleartext) */ 
					size_t buflen,			/* IN: Cleartext size */
					unsigned char* bufout,	/* IN/OUT: Output Buffer, size should be >= buflen AND multiple of 8 */ 
					const DES_key_schedule &key1)	/* IN: KEY */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return ACS_DES_enc_decrypt1(buflen, bufin, bufout, key1, 1) ;
}


////////////////////////////////////////////////////////////
//
//   DES DECRYPT
//
////////////////////////////////////////////////////////////

size_t
	ACS_SSL::ACS_DES_decrypt1(
					const unsigned char* bufin,	/* IN: Input Buffer (Cipheredtext) -- Size SHOULD BE multiple of 8 */ 
					unsigned char* bufout,	/* IN/OUT: Output Buffer (Cleartext) */ 
					size_t buflen,			/* IN: Cleartext size (SHOULD BE <= Cipheredtext size) */
					DES_key_schedule key1)	/* IN: KEY */  // PRQA S 2010
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 


	return ACS_DES_enc_decrypt1(buflen, bufin, bufout, key1, 0) ;
}


////////////////////////////////////////////////////////////
//
//   DES ENCRYPT
//
////////////////////////////////////////////////////////////

size_t
	ACS_SSL::ACS_DES_encrypt3(
					const unsigned char* bufin,	/* IN: Input Buffer (Cleartext) */ 
					size_t buflen,			/* IN: Cleartext size */
					unsigned char* bufout,	/* IN/OUT: Output Buffer, size should be >= buflen AND multiple of 8 */ 
					DES_key_schedule key1,	/* IN: KEYs */  // PRQA S 2010 3
					DES_key_schedule key2,	 
					DES_key_schedule key3)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	return ACS_DES_enc_decrypt3(buflen, bufin, bufout, key1, key2, key3, 1) ;
}


////////////////////////////////////////////////////////////
//
//   DES DECRYPT
//
////////////////////////////////////////////////////////////

size_t
	ACS_SSL::ACS_DES_decrypt3(
					const unsigned char* bufin,	/* IN: Input Buffer (Cipheredtext) -- Size SHOULD BE multiple of 8 */ 
					unsigned char* bufout,	/* IN/OUT: Output Buffer (Cleartext) */ 
					size_t buflen,			/* IN: Cleartext size (SHOULD BE <= Cipheredtext size) */
					DES_key_schedule key1,	/* IN: KEY */  // PRQA S 2010 3
					DES_key_schedule key2,	/* IN: KEY */ 
					DES_key_schedule key3)	/* IN: KEY */ 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return ACS_DES_enc_decrypt3(buflen, bufin, bufout, key1, key2, key3, 0) ;
}


////////////////////////////////////////////////////////////
//
//   TDES TOFB-I ENCRYPT/DECRYPT
//
////////////////////////////////////////////////////////////
size_t 
	ACS_SSL::ACS_DES_TOFB_I_encdec(	const unsigned char* bufin, 
									size_t buflen, 
									unsigned char* bufout, 
									DES_key_schedule key1,  // PRQA S 2010 3
									DES_key_schedule key2, 
									DES_key_schedule key3, 
									const DES_cblock iv1, 
									const DES_cblock iv2, 
									const DES_cblock iv3) 
{
	// EnrCar::
	// The obtained cyclomatic complexity is due to the implementation of 
	// a fast encryption algo.	
	
	DES_cblock  o[10]; // PRQA S 4403
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	size_t bytesTPMax8 = 0 ;	/* Used only in debug */
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 	
	

	size_t bytesToProcess = buflen ;
	size_t i = 0 ; 

	if (bytesToProcess > 0)
	{
		i=0; // O[0] = TDES_ENC(IV1) 	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(110) 
	        exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i 
		        << "  Bytes to process: " << bytesToProcess << "\n"
		        << "\tFeedback input (IV1): 0x" << HexDumper(iv1, DES_KEY_SZ) ;
            excerr << dbg ;
        ACS_CLASS_END_DEBUG
#endif 

#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb3_encrypt (const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(iv1)), &o[i], key1, key2, key3, 1 ) ; // PRQA S 3030, 3081 
#else
		DES_ecb3_encrypt (const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(iv1)), &o[i], &key1, &key2, &key3, 1 ) ; // PRQA S 3030, 3081 
#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(200) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
			    << "\tFeedback Output: O[" << i << "]: 0x" << HexDumper(o[i], DES_KEY_SZ) ; /*!!!*/
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

		// BUFOUT[0] = BUFIN[0] XOR O[0]	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(200) 
	        exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
		        << "\tData input: I[" << i << "]: 0x" << HexDumper(&bufin[i*8], DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
        ACS_CLASS_END_DEBUG
#endif 

		bytesToProcess -= ACS_xor3_lenMax8(&bufin[i*8], o[i], &bufout[i*8], bytesToProcess) ; // PRQA S 3084, 4400
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(100) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
			    << "\t>>>  DATA OUTPUT: C[" << i << "]: 0x" << HexDumper(&bufout[i*8], DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 
	}
	
	
	if (bytesToProcess > 0)
	{
		i=1; // O[1] = TDES_ENC(IV2) 	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(110) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i 
			    << "  Bytes to process: " << bytesToProcess << "\n"
			    << "\tFeedback input (IV2): 0x" << HexDumper(iv2, DES_KEY_SZ) ;
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb3_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(iv2)), &o[i], key1, key2, key3, 1 ) ; // PRQA S 3030, 3081
#else
		DES_ecb3_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(iv2)), &o[i], &key1, &key2, &key3, 1 ) ; // PRQA S 3030, 3081
#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(200) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
			    << "\tFeedback Output: O[" << i << "]: 0x" << HexDumper(o[i], DES_KEY_SZ) ; /*!!!*/
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

		// BUFOUT[0] = BUFIN[0] XOR O[0]	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(200) 
            exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
	            << "\tData input: I[" << i << "]: 0x" << HexDumper(&bufin[i*8], DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
        ACS_CLASS_END_DEBUG
#endif 

		bytesToProcess -= ACS_xor3_lenMax8(&bufin[i*8], o[i], &bufout[i*8], bytesToProcess) ; // PRQA S 3084, 4400
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(100) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
			    << "\t>>>  DATA OUTPUT: C[" << i << "]: 0x" << HexDumper(&bufout[i*8], DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 
	}
	
	
	if (bytesToProcess > 0)
	{
		i=2; // O[2] = TDES_ENC(IV3) 	// PRQA S 4400
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(110) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i 
			    << "  Bytes to process: " << bytesToProcess << "\n"
			    << "\tFeedback input (IV3): 0x" << HexDumper(iv3, DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb3_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(iv3)), &o[i], key1, key2, key3, 1 ) ; // PRQA S 3030, 3081 
#else
		DES_ecb3_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(iv3)), &o[i], &key1, &key2, &key3, 1 ) ; // PRQA S 3030, 3081 
#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(200) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
			    << "\tFeedback Output: O[" << i << "]: 0x" << HexDumper(o[i], DES_KEY_SZ) ; /*!!!*/ // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

		// BUFOUT[0] = BUFIN[0] XOR O[0]	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(200) 
	        exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
		        << "\tData input: I[" << i << "]: 0x" << HexDumper(&bufin[i*8], DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
        ACS_CLASS_END_DEBUG
#endif 

		bytesToProcess -= ACS_xor3_lenMax8(&bufin[i*8], o[i], &bufout[i*8], bytesToProcess) ; // PRQA S 3084, 4400
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(100) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
			    << "\t>>>  DATA OUTPUT: C[" << i << "]: 0x" << HexDumper(&bufout[i*8], DES_KEY_SZ) ; // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

	}


	for (i=3; bytesToProcess > 0; ++i)	/* Loop index and check-condition are based on different variables. SIC. */ // PRQA S 4231, 4235, 4400
	{
	
		size_t j = ACS_SSL_MOD4(i) ; // PRQA S 3000, 3010
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(110) 			
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i 
			    << "  Bytes to process: " << bytesToProcess << "\n"
			    << "\tFeedback Input: O[" << (i-3) << "] = O[" << ACS_SSL_MOD4(i-3) << "]: 0x" // PRQA S 3000, 3010, 3084, 4400
		        << HexDumper(&o[ACS_SSL_MOD4(i-3)], DES_KEY_SZ) ; // PRQA S 3000, 3010, 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

	
		// O[i] = TDES_ENC(O[i-3]) 
		/* IMPLEMENTATION NOTE: Since the feedback is 3 samples back, that is 4 samples including the current, 
		   O{i} = TDES_ENC(O{i-3}) where {} = MOD4 */

#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb3_encrypt ( &o[ACS_SSL_MOD4(i-3)], &o[j], key1, key2, key3, 1 ) ; // PRQA S 3000, 3010, 3084
#else
		DES_ecb3_encrypt ( &o[ACS_SSL_MOD4(i-3)], &o[j], &key1, &key2, &key3, 1 ) ; // PRQA S 3000, 3010, 3084
#endif

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(200) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n" 
			    << "\tFeedback Output: O[" << i << "] = O[" << ACS_SSL_MOD4(i) << "]: 0x"  // PRQA S 3000, 3010
		        << HexDumper(&o[j], DES_KEY_SZ) ; 
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 


		// BUFOUT[i] = BUFIN[i] XOR O[i] 
		// That is: BUFIN[i] XOR O{i} where {i}=(i%4)=j :	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(1) 				
		    bytesTPMax8 = (bytesToProcess <= 8)? bytesToProcess: 8 ; // PRQA S 3380, 4400
	    ACS_CLASS_END_DEBUG

	    ACS_CLASS_BGN_DEBUG(200) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n"
		        << "\tData input: I[" << i << "]: 0x" << HexDumper(&bufin[i*8], bytesTPMax8) ; // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 
		
		bytesToProcess -= ACS_xor3_lenMax8(&bufin[i*8], o[j], &bufout[i*8], bytesToProcess) ; // PRQA S 3084, 4400
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(100) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. "<< i << "\n" 
		        << "\t>>>  DATA OUTPUT: C[" << i << "]: 0x" << HexDumper(&bufout[i*8], bytesTPMax8)  // PRQA S 3084, 4400
		        << "\tBytes to process yet: " << bytesToProcess ;
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

	}	/* for (i=3; bytesToProcess > 0; i++) */


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method Finished. Bytes to process (should be: 0): " << bytesToProcess ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return buflen ;
}


////////////////////////////////////////////////////////////
//
//   DES OFB ENCRYPT/DECRYPT
//
////////////////////////////////////////////////////////////
size_t 
	ACS_SSL::ACS_DES_OFB_encdec(	const unsigned char* bufin, 
									size_t buflen, 
									unsigned char* bufout, 
									const DES_key_schedule &key, 
									const DES_cblock iv1) 
{

	unsigned char algobuf[DES_KEY_SZ] ; /* Decryption buffer */
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
    size_t bytesTPMax8 = 0 ;  /* Used only in debug */
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method called. Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 	
	

	size_t bytesToProcess = buflen ;

	memcpy(algobuf, iv1, DES_KEY_SZ); 	/* Copy IV into algo buffer. The first time IV will be used as beginning  */

	for (size_t i=0; bytesToProcess > 0; ++i)	/* Loop index and check-condition are based on different variables. SIC. */ // PRQA S 4231, 4235
	{
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(110) 			
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i 
			    << "  Bytes to process: " << bytesToProcess << "\n"
			    << "\tAlgo input: 0x" << HexDumper(algobuf, DES_KEY_SZ) ;
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

		/* Decrypt algobuf and save the result into itself -- so it's ready for next iteration */
		ACS_DES_encrypt1( algobuf, DES_KEY_SZ, algobuf, key) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(1) 				
		    bytesTPMax8 = (bytesToProcess <= 8)? bytesToProcess: 8 ; // PRQA S 3380, 4400
	    ACS_CLASS_END_DEBUG

	    ACS_CLASS_BGN_DEBUG(110) 			
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i 
			    << "  Bytes to process: " << bytesToProcess << "\n"
			    << "\tAlgo output: 0x" << HexDumper(algobuf, DES_KEY_SZ) 
		        << "\tData input: 0x" << HexDumper(&bufin[i*8], bytesTPMax8) ; // PRQA S 3084, 4400
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 


		/* The actual output is simply the XOR between algobuf and the input itself */
		bytesToProcess -= ACS_xor3_lenMax8(&bufin[i*DES_KEY_SZ], algobuf, &bufout[i*DES_KEY_SZ], bytesToProcess) ; // PRQA S 3084

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(100) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "Processing Iter. " << i << "\n" 
		        << "\t>>>  DATA OUTPUT: 0x" << HexDumper(&bufout[i*8], bytesTPMax8)  // PRQA S 3084, 4400
		        << "\tBytes to process yet: " << bytesToProcess ;
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 

	}

	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method Finished. Bytes to process (should be: 0): " << bytesToProcess ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return buflen ; 
}	


///////////////////////////////////////////////
//
//   RANDOM NUMBERS Generator
//
///////////////////////////////////////////////
int 												/* OUT: 1 SUCCESS (SIC, 0 means ERROR)*/
		ACS_SSL::ACS_RAND_bytes(unsigned char *buf,	/* IN: Pre-allocated buffer */
        						int bufsize,		/* IN: Buffer size */
                                bool randomSeed)  	/* IN: TRUE: randomize a new seed */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
		dbg << exDebugSignature << "bufsize " << bufsize << "  randomSeed: " << boolalpha << randomSeed ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	  
    ACS_COND_THROW((bufsize<0), ACS_SSL_InternalError("Negative buffer size!") ) ; // PRQA S 3081

	if (randomSeed)
    {
        const size_t rand_size = 128*1024 ;
        unsigned char* p = reinterpret_cast<unsigned char*>( malloc(rand_size) );	// It should be dirty: good ! // PRQA S 3081
        ACS_COND_THROW( !p, ACS_SSL_InternalError("Couldn't allocate memory", errno) ) ;  // PRQA S 3081
        pattern::CMemoryScopeGuard p_guard(p) ;

        gettimeofday(reinterpret_cast<struct timeval*>(p), 0);	// The first part of the buffer is filled with the (current) timeval structure // PRQA S 3030, 3081

        RAND_seed(p, rand_size) ;
    }
    
    return RAND_bytes(buf, bufsize) ;
}


int 												/* OUT: 1 SUCCESS (SIC, 0 means ERROR)*/
		ACS_SSL::ACS_RAND_bytes(unsigned char *buf,	/* IN: Pre-allocated buffer */
        						const int bufsize,		/* IN: Buffer size */
								const pair <unsigned char, unsigned char>& byte_range_in,	/* IN: subrange for the value of each byte */
                                bool randomSeed)  	/* IN: TRUE: randomize a new seed */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
		dbg << exDebugSignature << "bufsize " << bufsize << "  randomSeed: " << boolalpha << randomSeed ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	/**
			Extract random bytes, each one in a subrange.
			For example: 	'0'-'9' (0x30-0x39) would return: '3','1','5','0' ...
							'A'-'Z' (0x41-0x5A) would return: 'K','J','A','D' ...
							and so on.
	*/
	
	const int r ( ACS_RAND_bytes(buf, bufsize, randomSeed) ) ;
	
	const pair <unsigned char, unsigned char> byte_range( 
		pair <unsigned char, unsigned char>(min(byte_range_in.first,byte_range_in.second), max(byte_range_in.first,byte_range_in.second))
	) ; 
	
	const unsigned char offset (byte_range.first) ; 
	const unsigned char modulo (byte_range.second - byte_range.first) ; 

	if ( (r != 0) && (0xff != modulo) )
	{
		// Success obtained, and range different from trivial one (that is, whole range 0-255)

		for (int i=0; i<bufsize; i++) { buf[i] = ( (buf[i] % modulo) + offset ) ; }
	}
	
    return r ;
}


////////////////////////////////////////////////////////////
//
//   GENERATE A BF RANDOM KEY
//
////////////////////////////////////////////////////////////
void 
	ACS_SSL::ACS_BLOWFISH_randomkey(unsigned char* buffer,	/* IN/OUT: Random key (into a pre-allocated ptr) */ // PRQA S 4020
                                    int* bufLen,			/* IN/OUT: IN: Buffer lenght, OUT: key length, useful if default is used */
                                    BF_KEY *key,			/* IN/OUT: Converted Key (into a pre-allocated ptr). NULL to not assing key */
                                    int len_bytes) 			/* IN: Lenght of key to generate [bytes] */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
        dbg << exDebugSignature 
		    << "BF_KEY ptr: 0x" << hex << key << dec << "  Length: " << len_bytes << " bytes" ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW( // PRQA S 3081
		(! buffer),
		ACS_SSL_InternalError("NULL data buffer")
	) ; 

	ACS_COND_THROW( // PRQA S 3081
		( (! bufLen) || (*bufLen < len_bytes)),
		ACS_SSL_InternalError("NULL or invalid buffer lenght")
	) ; 
    
	ACS_COND_THROW( // PRQA S 3081
		(len_bytes<1) || (len_bytes>ACS_SSL_BFkey_maxsize),
		ACS_SSL_InternalError("Key lenght out of range")
	) ; 
    
    (*bufLen) = len_bytes ;
    
    ACS_COND_THROW( // PRQA S 3081
		(ACS_RAND_bytes(buffer, (*bufLen), true) != 1),
		ACS_SSL_InternalError("Unable to generate random key")
	) ; 


	if (key) {
		BF_set_key(key, (*bufLen)/8, buffer) ; // PRQA S 4400
    }
}


////////////////////////////////////////////////////////////
//
//   CONVERT THE KEY FROM A CHAR BUFFER TO A BF_KEY
//
////////////////////////////////////////////////////////////
void 
	ACS_SSL::ACS_BLOWFISH_setkey(
			const unsigned char *datain,	/* IN: Key (Binary Format, as vector of uchars) */
			int len_bytes,					/* IN: Key lenght [bytes] */
			BF_KEY *bfkey)					/* OUT: Converted Key (into a pre-allocated ptr) */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Length: " << len_bytes << " bytes\n" << "\tDump: 0x" << HexDumper(datain, len_bytes) ; // PRQA S 3000
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return BF_set_key(bfkey, len_bytes, datain) ;
}


////////////////////////////////////////////////////////////
//
//   BLOWFISH DECRYPT
//
////////////////////////////////////////////////////////////
size_t										/* OUT: Decrypted buffer size (will be multiple of 8) */
	ACS_SSL::ACS_BLOWFISH_ECB_decrypt(
					const unsigned char* bufin,	/* IN: Input Buffer (will be multiple of 8) */
					unsigned char* bufout,	/* IN/OUT: Output Buffer */ 
					size_t buflen,			/* IN: Input buffer size */
					BF_KEY* bfkey)			/* Key */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return ACS_BLOWFISH_ECB_enc_dec(buflen, bufin, bufout, bfkey, 0) ;
}


////////////////////////////////////////////////////////////
//
//   BLOWFISH ENCRYPT
//
////////////////////////////////////////////////////////////
size_t										/* OUT: Encrypted buffer size (will be multiple of 8) */
	ACS_SSL::ACS_BLOWFISH_ECB_encrypt(
					const unsigned char* bufin,	/* IN: Input Buffer (could be NOT multiple of 8 -- WILL BE PADDED WITH 0x0 ) */
					unsigned char* bufout,	   /* IN/OUT: Output Buffer. It should be pre-allocated using ACS_SSL_CEIL8(size)  */ 
					size_t buflen,			  /* IN: Input buffer size */ 
					BF_KEY* bfkey)			  /* Key */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return ACS_BLOWFISH_ECB_enc_dec(buflen, bufin, bufout, bfkey, 1) ;
}


////////////////////////////////////////////////////////////
//
//   BLOWFISH ENCRYPT plus BASE64
//
////////////////////////////////////////////////////////////
size_t											/* OUT: Encrypted buffer size */
	ACS_SSL::ACS_BLOWFISH_ECB_encrypt_base64(
					const unsigned char* bufin,		/* IN: Input Buffer (could be NOT multiple of 8 -- WILL BE PADDED WITH 0x0 ) */
					char* bufout,				/* IN/OUT: Output Buffer. 
													It should be pre-allocated using ACS_BASE64_encode_buffersize( ACS_SSL_CEIL8(size) )  */
					size_t buflen,				/* IN: Input buffer size.  */ 
					BF_KEY* bfkey)				/* Key */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Input buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	unsigned char* tmp = reinterpret_cast<unsigned char*>(calloc (ACS_SSL_CEIL8(buflen) ,1)) ; // PRQA S 3000, 3010, 3081
    ACS_COND_THROW(!tmp, ACS_SSL_InternalError("Out of memory", errno)) ; // PRQA S 3081
	pattern::CMemoryScopeGuard tmp_guard(tmp) ;	/* no free is required */

	size_t s1 = ACS_BLOWFISH_ECB_enc_dec(buflen, bufin, tmp, bfkey, 1) ;
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
        dbg << exDebugSignature << "ACS_BLOWFISH_ECB_enc_dec returned with output: " << s1 ;
	    excerr << dbg ;
    ACS_CLASS_END_DEBUG	
#endif 
	
	size_t s2 = ACS_BASE64_encode(tmp, bufout, s1) ;
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
        dbg << exDebugSignature << "ACS_BASE64_encode returned with output: " << s2 ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	

	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method finished with output: " << s2 ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	
#endif 

	return s2 ;
	
}


////////////////////////////////////////////////////////////
//
//   BLOWFISH DECRYPT plus BASE64
//
////////////////////////////////////////////////////////////
size_t											/* OUT: Encrypted buffer size (will be multiple of 8) */
	ACS_SSL::ACS_BLOWFISH_ECB_decrypt_base64(
					const char* bufin,				/* IN: Input Buffer */
					unsigned char* bufout,		/* IN/OUT: Output Buffer */ 
					size_t buflen,				/* IN: Input Buffer size */ 
					BF_KEY* bfkey)				/* Key */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Input buffer size: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	unsigned char* tmp = reinterpret_cast<unsigned char*>( calloc (buflen ,1) );	/* To be sure ... */ // PRQA S 3081
    ACS_COND_THROW(!tmp, ACS_SSL_InternalError("Out of memory", errno)) ; // PRQA S 3081
	pattern::CMemoryScopeGuard tmp_guard(tmp) ;	/* no free is required */
	
	
	size_t s1 = ACS_BASE64_decode(bufin, tmp, buflen) ;
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
        dbg << exDebugSignature << "ACS_BASE64_decode returned with output: " << s1 ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	
#endif 


	size_t s2 = ACS_BLOWFISH_ECB_enc_dec(s1, tmp, bufout, bfkey, 0) ;
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
        dbg << exDebugSignature << "ACS_BLOWFISH_ECB_enc_dec returned with output: " << s2 ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	

	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method finished with output: " << s2 ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	
#endif 
	
	
	return s2 ;
	
}


////////////////////////////////////////////////////////////
//
//   PRIVATE: DES ENCRYPT/DECRYPT
//
////////////////////////////////////////////////////////////
size_t
	ACS_SSL::ACS_DES_enc_decrypt1(  // PRQA S 4020
					size_t buflen,			/* IN: Cleartext size */
					const unsigned char* bufin,	/* IN: Input Buffer */ 
					unsigned char* bufout,	/* IN/OUT: Output Buffer */ 
					const DES_key_schedule &key1,	/* IN: KEY */ 
					int enc)				/* IN: enc flag: 0 to decrypt, 1 to encrypt */
{
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG

	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Input Buffer: 0x" << HexDumper(bufin, buflen ) ;	/* Dump input */
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	
	if (buflen<1) { return 0 ; }	/* Nothing to do! */
	

	/* Process all blocks EXECEPT THE LAST ONE */
	for (size_t i=0; i<((buflen-1)/8); ++i) // PRQA S 3084, 4400
	{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb_encrypt( const_cast<des_cblock*>(reinterpret_cast<const des_cblock*>(bufin+(i*8))),  // PRQA S 3030, 3081, 3084
                         reinterpret_cast<des_cblock*>(bufout+(i*8)), 
                         const_cast<DES_key_schedule &>(key1),
                         enc ) ;
#else
		DES_ecb_encrypt( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(bufin+(i*8))),  // PRQA S 3030, 3081, 3084
                         reinterpret_cast<DES_cblock*>(bufout+(i*8)), 
                         const_cast<DES_key_schedule*>(&key1),
                         enc ) ;
#endif
	}

	size_t last_i = (buflen-1)/8 ; // PRQA S 3084
	unsigned char buflen8 = static_cast<unsigned char>(buflen % size_t(8)) ; // PRQA S 3081, 3084

	size_t s_out = 0 ;
	unsigned char tmpbuf[8] ; // PRQA S 4403
	
	/* Process the LAST BLOCK */
	if ( buflen8 != 0 ) 
	{		
		/* Size not multiple of 8: The Cleartext is less than a block */

		if (enc != 0)
		{
			/* ENCODING */
			
			memset (tmpbuf, 0, 8) ; /* Prepare the (shorter) input buffer */ // PRQA S 4400

			memcpy(tmpbuf, (bufin + (8*last_i)), buflen8) ;	/* LEFT-ALIGNED, RIGHT-PADDED */ // PRQA S 3084, 4400
#if OPENSSL_VERSION_NUMBER < 0x10100000L
			des_ecb_encrypt ( reinterpret_cast<des_cblock*>(tmpbuf),   // PRQA S 3030, 3081, 3084
                              reinterpret_cast<des_cblock*>(bufout + (8*last_i)), 
							  const_cast<DES_key_schedule &>(key1),
                              enc ) ;
#else
			DES_ecb_encrypt ( reinterpret_cast<DES_cblock*>(tmpbuf),   // PRQA S 3030, 3081, 3084
                              reinterpret_cast<DES_cblock*>(bufout + (8*last_i)), 
							  const_cast<DES_key_schedule*>(&key1),
                              enc ) ;
#endif

			s_out = ((last_i+1)*8) ;    // PRQA S 3084, 4400
		}
		else
		{
			/* DECODING */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
			des_ecb_encrypt ( const_cast<des_cblock*>(reinterpret_cast<const des_cblock*>(bufin+(last_i*8))),   // PRQA S 3030, 3081, 3084
                              reinterpret_cast<des_cblock*>(tmpbuf), 
                              const_cast<DES_key_schedule &>(key1),
                              enc ) ;
#else
			DES_ecb_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(bufin+(last_i*8))),   // PRQA S 3030, 3081, 3084
                              reinterpret_cast<DES_cblock*>(tmpbuf), 
                              const_cast<DES_key_schedule*>(&key1),
                              enc ) ;
#endif

			memcpy((bufout + (8*last_i)), tmpbuf, buflen8) ; // PRQA S 3084, 4400 2
			s_out = (last_i*8) + buflen8 ;
		}
	}
	else
	{
		/* Size multiple of 8 */

#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb_encrypt ( const_cast<des_cblock*>(reinterpret_cast<const des_cblock*>(bufin+(last_i*8))),    // PRQA S 3030, 3081, 3084
                          reinterpret_cast<des_cblock*>(bufout+(last_i*8)), 
                         const_cast<DES_key_schedule &>(key1),
                          enc ) ;			
#else
		DES_ecb_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(bufin+(last_i*8))),    // PRQA S 3030, 3081, 3084
                          reinterpret_cast<DES_cblock*>(bufout+(last_i*8)), 
                         const_cast<DES_key_schedule*>(&key1),
                          enc ) ;			
#endif

		s_out = ((last_i+1)*8) ;    // PRQA S 3084, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method finished. Output size: " << s_out ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	

	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Output Buffer: 0x" << HexDumper( bufout, s_out ) ;	/* Dump input */
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	
	return s_out;
}


////////////////////////////////////////////////////////////
//
//   PRIVATE: TDES ENCRYPT/DECRYPT
//
////////////////////////////////////////////////////////////
size_t
	ACS_SSL::ACS_DES_enc_decrypt3(  // PRQA S 4020
					size_t buflen,			/* IN: Cleartext size */
					const unsigned char* bufin,	/* IN: Input Buffer */ 
					unsigned char* bufout,	/* IN/OUT: Output Buffer */ 
					const DES_key_schedule &key1,	/* IN: KEYs */ 
					const DES_key_schedule &key2,
					const DES_key_schedule &key3,
					int enc)		/* IN: enc flag: 0 to decrypt, 1 to encrypt */
{
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	
	if (buflen<1) { return 0 ; } /* Nothing to do! */
	

	/* Process all blocks EXECEPT THE LAST ONE */
	for (size_t i=0; i<((buflen-1)/8); ++i) // PRQA S 3084, 4400
	{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb3_encrypt( const_cast<des_cblock*>(reinterpret_cast<const des_cblock*>(bufin+(i*8))), // PRQA S 3030, 3081, 3084
                          reinterpret_cast<des_cblock*>(bufout+(i*8)), 
                          const_cast<DES_key_schedule&>(key1), 
                          const_cast<DES_key_schedule&>(key2), 
                          const_cast<DES_key_schedule&>(key3), 
                          enc ) ;
#else
		DES_ecb3_encrypt( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(bufin+(i*8))), // PRQA S 3030, 3081, 3084
                          reinterpret_cast<DES_cblock*>(bufout+(i*8)), 
                          const_cast<DES_key_schedule*>(&key1), 
                          const_cast<DES_key_schedule*>(&key2), 
                          const_cast<DES_key_schedule*>(&key3), 
                          enc ) ;
#endif
	}

	size_t last_i = (buflen-1)/8 ; // PRQA S 3084
	unsigned char buflen8 = static_cast<unsigned char>((buflen % size_t(8))) ; // PRQA S 3081, 3084

	size_t s_out = 0 ;
	unsigned char tmpbuf[8] ; // PRQA S 4403
	
	
	/* Process the LAST BLOCK */
	if ( 0 != buflen8 ) 
	{		
		/* Size not multiple of 8: The Cleartext is less than a block */

		if (0 != enc)
		{
			/* ENCODING */
			
			memset (tmpbuf, 0, 8) ; /* Prepare the (shorter) input buffer */ // PRQA S 4400

			memcpy(tmpbuf, (bufin + (8*last_i)), buflen8) ;	/* LEFT-ALIGNED, RIGHT-PADDED */ // PRQA S 3084, 4400

#if OPENSSL_VERSION_NUMBER < 0x10100000L
			des_ecb3_encrypt ( reinterpret_cast<des_cblock*>(tmpbuf),  // PRQA S 3030, 3081, 3084
                               reinterpret_cast<des_cblock*>(bufout + (8*last_i)), 
                               const_cast<DES_key_schedule&>(key1), 
                               const_cast<DES_key_schedule&>(key2), 
                               const_cast<DES_key_schedule&>(key3), 
                               enc ) ;
#else
			DES_ecb3_encrypt ( reinterpret_cast<DES_cblock*>(tmpbuf),  // PRQA S 3030, 3081, 3084
                               reinterpret_cast<DES_cblock*>(bufout + (8*last_i)), 
                               const_cast<DES_key_schedule*>(&key1), 
                               const_cast<DES_key_schedule*>(&key2), 
                               const_cast<DES_key_schedule*>(&key3), 
                               enc ) ;
#endif


			s_out = ((last_i+1)*8) ;    // PRQA S 3084, 4400
		}
		else
		{
			/* DECODING */
	
#if OPENSSL_VERSION_NUMBER < 0x10100000L
			des_ecb3_encrypt ( const_cast<des_cblock*>(reinterpret_cast<const des_cblock*>(bufin+(last_i*8))),  // PRQA S 3030, 3081, 3084
                               reinterpret_cast<des_cblock*>(tmpbuf), 
                               const_cast<DES_key_schedule&>(key1), 
                               const_cast<DES_key_schedule&>(key2), 
                               const_cast<DES_key_schedule&>(key3), 
                               enc ) ;
#else
			DES_ecb3_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(bufin+(last_i*8))),  // PRQA S 3030, 3081, 3084
                               reinterpret_cast<DES_cblock*>(tmpbuf), 
                               const_cast<DES_key_schedule*>(&key1), 
                               const_cast<DES_key_schedule*>(&key2), 
                               const_cast<DES_key_schedule*>(&key3), 
                               enc ) ;
#endif
			memcpy((bufout + (8*last_i)), tmpbuf, buflen8) ;     // PRQA S 3084, 4400 2			
			s_out = (last_i*8) + buflen8 ;
		}
	}
	else
	{
		/* Size multiple of 8 */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
		des_ecb3_encrypt ( const_cast<des_cblock*>(reinterpret_cast<const des_cblock*>(bufin+(last_i*8))),  // PRQA S 3030, 3081, 3084
                           reinterpret_cast<des_cblock*>(bufout+(last_i*8)), 
                           const_cast<DES_key_schedule&>(key1), 
                           const_cast<DES_key_schedule&>(key2), 
                           const_cast<DES_key_schedule&>(key3), 
                           enc ) ;
#else
		DES_ecb3_encrypt ( const_cast<DES_cblock*>(reinterpret_cast<const DES_cblock*>(bufin+(last_i*8))),  // PRQA S 3030, 3081, 3084
                           reinterpret_cast<DES_cblock*>(bufout+(last_i*8)), 
                           const_cast<DES_key_schedule*>(&key1), 
                           const_cast<DES_key_schedule*>(&key2), 
                           const_cast<DES_key_schedule*>(&key3), 
                           enc ) ;
#endif
		s_out = ((last_i+1)*8) ;    // PRQA S 3084, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	
	return s_out;
}


////////////////////////////////////////////////////////////
//
//   DES_cblock ADD operation
//
////////////////////////////////////////////////////////////
unsigned char ACS_SSL::ACS_DES_cblock_add (const DES_cblock src, DES_cblock dst)  
{
	/*
		Add source S to destination D
		Store into destination D = (D+S)
		Return MSB carry (0 or 1)
	*/
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ)
		    << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ) ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	return ACS_DES_cblock_adc (src, dst, 0) ;
}	
		

////////////////////////////////////////////////////////////
//
//   DES_cblock ADC operation (Add with Carry)
//
////////////////////////////////////////////////////////////
unsigned char ACS_SSL::ACS_DES_cblock_adc (const DES_cblock src, DES_cblock dst, unsigned char carry) // PRQA S 4020  
{	
	/*
		Add source S to destination D and LSB carry C
		Store into destination D = (D+S+C)
		Return MSB carry (0 or 1)
	*/
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ)
		    << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ)
		    << "\n\tCarry: 0x" << hex << int(carry); // PRQA S 3081
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	
	if (DES_KEY_SZ<1) { return 0 ; } // PRQA S 4090
	
	for (size_t i=1; i<=DES_KEY_SZ; ++i)
	{
		unsigned short dst_sh = static_cast<unsigned short>(dst[DES_KEY_SZ-i]) ; // PRQA S 3081, 3084
		dst_sh += static_cast<unsigned short>(src[DES_KEY_SZ-i]) ; // PRQA S 3000, 3081, 3084
		dst_sh += static_cast<unsigned short>(carry) ; // PRQA S 3000, 3081

		dst[DES_KEY_SZ-i] = (dst_sh & 0xff) ;   // PRQA S 3000, 3010, 3084, 4400
		carry = static_cast<unsigned char> (((dst_sh & 0x100) >> 8) & 0xff) ;// PRQA S 3000, 3007, 3081, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ) 
		    << "\n\tCarry: 0x" << hex << int(carry) ; // PRQA S 3081
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return carry ;
}


////////////////////////////////////////////////////////////
//
//   DES_cblock INC operation (Increment)
//
////////////////////////////////////////////////////////////
unsigned char ACS_SSL::ACS_DES_cblock_inc (DES_cblock dst) // PRQA S 4020
{	
	/*
		Increment the provided value
		Store into destination D = (D+1)
		Return MSB carry (0 or 1)
	*/
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature << "\n\tSRC: 0x" << HexDumper(dst, DES_KEY_SZ) ; 
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	
	if (DES_KEY_SZ<1) { return 0 ; } // PRQA S 4090
	
	unsigned char carry = 0x01 ;
	for (size_t i=1; i<=DES_KEY_SZ; ++i)
	{
		unsigned short dst_sh = static_cast<unsigned short>(dst[DES_KEY_SZ-i]) ; // PRQA S 3081, 3084
		dst_sh += static_cast<unsigned short>( carry ) ; // PRQA S 3000, 3081

		dst[DES_KEY_SZ-i] = (dst_sh & 0xff) ;   // PRQA S 3000, 3010, 3084, 4400 
		carry = static_cast<unsigned char>(((dst_sh & 0x100) >> 8) & 0xff) ;	/* Prepare carry */ // PRQA S 3000, 3007, 3081, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ) 
			<< "\n\tCarry: 0x" << hex << int(carry) ; // PRQA S 3081
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return carry ;
}
	
	
////////////////////////////////////////////////////////////
//
//   DES_cblock SUB operation (Subtract)
//
////////////////////////////////////////////////////////////
unsigned char ACS_SSL::ACS_DES_cblock_sub (const DES_cblock src, DES_cblock dst) 
{	
	/*
		Subtract S from destination D
		Store into destination D = (D-S)
		Return MSB borrow (0 or 1)
	*/
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ)
		    << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ) ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	return 	ACS_DES_cblock_sbb(src, dst, 0) ;
}
	

////////////////////////////////////////////////////////////
//
//   DES_cblock SBB operation (Subtract with Borrow)
//
////////////////////////////////////////////////////////////
unsigned char ACS_SSL::ACS_DES_cblock_sbb (const DES_cblock src, DES_cblock dst, unsigned char carry)  // PRQA S 4020
{	
	/*
		Add carry C and source S, and subtract the result from destination D
		Store into destination D = (D-(S+C))
		Return MSB borrow (0 or 1)
	*/
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ)
		    << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ)
		    << "\n\tCarry: 0x" << hex << int(carry) ;   // PRQA S 3081
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	
	if (DES_KEY_SZ<1) { return 0 ; } // PRQA S 4090
	
	for (size_t i=1; i<=DES_KEY_SZ; ++i)
	{
		unsigned short dst_sh = static_cast<unsigned short>(dst[DES_KEY_SZ-i]) ; // PRQA S 3081, 3084 2
		unsigned short tmp = static_cast<unsigned short>(src[DES_KEY_SZ-i]) ;  
		
		if (0 != carry) { tmp ++ ; carry = 0; } 	/* get borrow from previous run */
		
		// Prepare next borrow
		if (tmp > dst_sh)
		{
			dst_sh |= 0x100 ;	/* apply borrow */ // PRQA S 4400
			carry = 1 ;			/* set borrow for next run */
		}
			
		dst_sh -= (tmp & 0xff); // PRQA S 3000, 3010, 4400
	
		dst[DES_KEY_SZ-i] = (dst_sh & 0xff) ;   // PRQA S 3000, 3010, 3084, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(dst, DES_KEY_SZ) 
		    << "\n\tCarry: 0x" << hex << int(carry) ; // PRQA S 3081
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return carry ;
}

////////////////////////////////////////////////////////////
//
//   DES_cblock MOV operation (Copy Memory)
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_DES_cblock_mov (const DES_cblock src, DES_cblock dst) // PRQA S 4020
{	
	/*
		Copy SRC to DST: D = S
	*/
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ) ; 
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	if (DES_KEY_SZ<1) { return ; }  // PRQA S 4090

	memcpy(dst, src, DES_KEY_SZ) ;
}


////////////////////////////////////////////////////////////
//
//   DES_cblock NZR operation (Check not zero)
//
////////////////////////////////////////////////////////////
unsigned char 														/* 1 if not zero */
				ACS_SSL::ACS_DES_cblock_nzr (const DES_cblock src) // PRQA S 4020
{	
	/*
		Return 1 if SRC not zero
	*/
	
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

	unsigned char status = 0 ;
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ) ; 
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	if (DES_KEY_SZ<1) { return status ; } // PRQA S 4090

	for (size_t i=1; i<=DES_KEY_SZ; ++i)
	{
		if ((src[DES_KEY_SZ-i] % 0xff) != 0) { status = 1; break ; } ;	/* Found a not-zero value */ // PRQA S 3084, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
        exFDebug dbg ;
        dbg << exDebugSignature << "Returning: " << int(status) ; // PRQA S 3081
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return status ;
}


////////////////////////////////////////////////////////////
//
//   DES_cblock MOD operation (Modulus)
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_DES_cblock_mod (const DES_cblock src, DES_cblock dst) // PRQA S 4020
{	
	/*
		Apply the modulus operator D = (D | S)
	*/
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature 
            << "\n\tSRC: 0x" << HexDumper(src, DES_KEY_SZ) 
			<< "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ) ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	/* Check src value */
	ACS_COND_THROW( // PRQA S 3081
		(0 == ACS_DES_cblock_nzr(src)),
		ACS_SSL_InternalError("Cannot divide by zero")
	) ; 
	
	if (DES_KEY_SZ<1) { return ; } // PRQA S 4090
	
	DES_cblock tmp ;
	
	ACS_DES_cblock_mov (dst, tmp) ;
	
	unsigned char carry = 0 ;
	do
	{
		ACS_DES_cblock_mov(tmp, dst) ;
		carry = ACS_DES_cblock_sub(src, tmp) ;	/* unset if tmp >= src */
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	    ACS_CLASS_BGN_DEBUG(250) 
		    exFDebug dbg ;
            dbg << exDebugSignature << "tmp: 0x" << HexDumper(tmp, DES_KEY_SZ) 
			    << "\tCarry: " << int(carry) ; // PRQA S 3081
            excerr << dbg ;
	    ACS_CLASS_END_DEBUG
#endif 
		
	}
	while ( 0 == carry ) ;
	
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(210) 
		exFDebug dbg ;
        dbg << exDebugSignature << "\n\tDST: 0x" << HexDumper(dst, DES_KEY_SZ) ;
        excerr << dbg ; 
	ACS_CLASS_END_DEBUG
#endif 

	return ;
}

	
////////////////////////////////////////////////////////////
//
//   ACS_DES_getIV23
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_DES_getIV23( const DES_cblock iv1, DES_cblock iv2, DES_cblock iv3) 
{
	/*
		Generate IV2 and IV3 from IV1. See
		ANSI X9.52-1998 Paragraph 6.7
	*/
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	for (size_t i=0; i<DES_KEY_SZ; ++i)
	{
		unsigned char* p = iv2 ;
		p[i] = 0x55 ;   // PRQA S 4400
		p = iv3 ;
		p[i] = 0xaa ;   // PRQA S 4400
	}
	
	
	// IV2 = 0x5555555555555555
	// IV2 += IV1 
	
	// IV3 = 0xaaaaaaaaaaaaaaaa
	// IV3 += IV1 
	
	ACS_DES_cblock_add(iv1, iv2) ;	
	ACS_DES_cblock_add(iv1, iv3) ;	


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
        dbg << exDebugSignature
            << "\n\tIV1: 0x" << HexDumper(iv1, DES_KEY_SZ) 
		    << "\n\tIV2: 0x" << HexDumper(iv2, DES_KEY_SZ) 
		    << "\n\tIV3: 0x" << HexDumper(iv3, DES_KEY_SZ) ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	return ;
}


////////////////////////////////////////////////////////////
//
//   ACS_SSL::ACS_AES_handle()
//
////////////////////////////////////////////////////////////
ACS_SSL::ACS_AES_handle::ACS_AES_handle(const ACS_AES_Cipher& cipher, 
										const ACS_AES_Key& key_in, 
										const ACS_AES_IV& iv, 
										const bool enc_oper, 
										const bool paddingSet) :
	_ctx(0), 
	_cipher(cipher),
	_blockLength(0),
	_keyLength(0),
	_encOper(enc_oper),
	_paddingSet(paddingSet),
	_finalizedSet(false)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cipher: " << toString(cipher) // PRQA S 3380
							<< "  Operation: " << (enc_oper?'E':'D') << "  Padding: " << (paddingSet? "SET":"UNSET")) ;
#endif 

	_keyLength = getKeyLength(cipher) ;	// Since CTX already not initialized
	unsigned char key[ _keyLength ] ; key_in.getKey(key, _keyLength) ; // Fill the key


	/* Allocated and initialise the context */
	if(!(_ctx = EVP_CIPHER_CTX_new())) {
		ACS_THROW(ACS_SSL_InternalError("Could not initialize context")); // PRQA S 3081
	}

	CTX_Guard ctx_guard(_ctx) ;	// Automatic guard, to prevent memory leakages in case of c'tor problems. It will be relased as last instruction

	const EVP_CIPHER* (*getTypeFun)(void) = 0 ; // Pointer to a function, specific to any AES length and scheme
	switch( cipher )
	{
		case AES_128_CBC: { getTypeFun = &EVP_aes_128_cbc ; break ; }
		case AES_128_OFB: { getTypeFun = &EVP_aes_128_ofb ; break ; }
		case AES_192_CBC: { getTypeFun = &EVP_aes_192_cbc ; break ; }
		case AES_192_OFB: { getTypeFun = &EVP_aes_192_ofb ; break ; }
		case AES_256_CBC: { getTypeFun = &EVP_aes_256_cbc ; break ; }
		case AES_256_OFB: { getTypeFun = &EVP_aes_256_ofb ; break ; }
		default:
			ACS_THROW(ACS_SSL_InternalError("Cipher unsupported")); // PRQA S 3081
	}

	//Decomment for a temporary low-level debug
	//{
	//  char dump[4096]; 
  	//  excerr << "CIPHER: " << toString(cipher) << endl ;
	//  ACS_SSL::ACS_SSL_HexToString(key, _keyLength, dump, sizeof(dump)) ;
	//  excerr << "KEY DUMP: " << dump << "  size: " << _keyLength << endl ;
	//  ACS_SSL::ACS_SSL_HexToString(iv, getIVLength(cipher), dump, sizeof(dump)) ;
	//  excerr << "IV DUMP: " << dump << "  size: " << getIVLength(cipher) << endl ;
	//}

	if(enc_oper)
	{
		/* Initialise the encryption operation. */
		if(1 != EVP_EncryptInit_ex(_ctx, getTypeFun(), 0, key, iv)) {
			ACS_THROW(ACS_SSL_InternalError("Could not initialize cipher")); // PRQA S 3081
		}
	} else {
		/* Initialise the decryption operation. */
		if(1 != EVP_DecryptInit_ex(_ctx, getTypeFun(), 0, key, iv)) {
			ACS_THROW(ACS_SSL_InternalError("Could not initialize cipher")); // PRQA S 3081
		}
	}

	_blockLength = EVP_CIPHER_CTX_block_size(_ctx) ; // PRQA S 3000
	// Sanity check -- does the block length embedded in the ciphering module HW/SW match with the expected one? (embedded in the static method) 
	ACS_COND_THROW( (_blockLength != getBlockLength(cipher)), ACS_SSL_InternalError("MISMATCHING BLOCK LENGTH")); // PRQA S 3081

	_keyLength = EVP_CIPHER_CTX_key_length(_ctx) ;	// Overwrites the keyLength with the value taken from the ciphering module // PRQA S 3000
	// Sanity check -- does the key length embedded in the ciphering module HW/SW match with the expected one? (embedded in the static method) 
	ACS_COND_THROW( (_keyLength != getKeyLength(cipher)), ACS_SSL_InternalError("MISMATCHING KEY LENGTH")); // PRQA S 3081

	
	/* Set the padding */
	if(1 != EVP_CIPHER_CTX_set_padding(_ctx, paddingSet)) {
		ACS_THROW(ACS_SSL_InternalError(string("Could not set padding to: ") + (paddingSet? "T":"F"))); // PRQA S 3081, 3380
	}
	
	ctx_guard.detach() ; // Detach ("turn off") the automatic guard

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished") ;
#endif 
}

	
////////////////////////////////////////////////////////////
//
//   ACS_SSL::~ACS_AES_handle()
//
////////////////////////////////////////////////////////////
ACS_SSL::ACS_AES_handle::~ACS_AES_handle() throw()
{
    // PRQA S 4631 END_DTOR
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ((_ctx != 0)? " Cleaning-up" : "Nothing to do")) ;  // PRQA S 3380
#endif 

	if (_ctx != 0) { EVP_CIPHER_CTX_cleanup(_ctx) ; }
	_ctx = 0 ; 
    // PRQA L:END_DTOR
}			


////////////////////////////////////////////////////////////
//
//   STATIC: ACS_AES_handle::getKeyLength
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::ACS_AES_handle::getKeyLength(const ACS_AES_Cipher& cipher)
{
	size_t keyLength = 0 ; 
    switch(cipher)
	{
		case AES_128_CBC: 
		case AES_128_OFB: { keyLength = 16 ; break ; } // PRQA S 4400 3
        case AES_192_CBC: 
        case AES_192_OFB: { keyLength = 24 ; break ; }
        case AES_256_CBC: 
        case AES_256_OFB: { keyLength = 32 ; break ; }
		default: { ACS_THROW(ACS_SSL_InternalError("Cipher unsupported")); } // PRQA S 3081
	}
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Cipher: " << toString(cipher) << "  keyLength: " << keyLength) ;
#endif 
	return keyLength ;
}
	

////////////////////////////////////////////////////////////
//
//   STATIC: ACS_AES_handle::getBlockLength
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::ACS_AES_handle::getBlockLength(const ACS_AES_Cipher& cipher)
{
	size_t blockLength = 0 ; 
    switch(cipher)
	{
		case AES_128_CBC: 
        case AES_192_CBC: 
        case AES_256_CBC: { blockLength = 16 ; break ; }	// 16 bytes, i.e. 128 bits: typical AES block cipher
		case AES_128_OFB: 
        case AES_192_OFB:  
        case AES_256_OFB: { blockLength = 1 ; break ; }		// 1 byte: typical stream cipher (regardless it's AES or not)
		default: { ACS_THROW(ACS_SSL_InternalError("Cipher unsupported")); } // PRQA S 3081
	}
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Cipher: " << toString(cipher) << "  blockLength: " << blockLength) ;
#endif 
	return blockLength ;
}


////////////////////////////////////////////////////////////
//
//   STATIC: ACS_AES_handle::getIVLength
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::ACS_AES_handle::getIVLength(const ACS_AES_Cipher& cipher)
{
	size_t ivLength = 0 ; 
    switch(cipher)
	{
		case AES_128_CBC: 
        case AES_192_CBC: 
        case AES_256_CBC: 
		case AES_128_OFB: 
        case AES_192_OFB: 
        case AES_256_OFB: { ivLength = 16 ; break ; }		
		default: { ACS_THROW(ACS_SSL_InternalError("Cipher unsupported")); } // PRQA S 3081
	}
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Cipher: " << toString(cipher) << "  ivLength: " << ivLength) ;
#endif 
	return ivLength ;
}


////////////////////////////////////////////////////////////
//
//   STATIC: ACS_SSL::ACS_AES_handle::getExpected_buffersize
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::ACS_AES_handle::getExpected_buffersize(const size_t& bufin_len, ACS_AES_Cipher cipher, bool enc_oper, bool paddingSet)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "bufin_len: " << bufin_len << "  cipher: " << toString(cipher)  // PRQA S 3380
							<< "  Operation: " << (enc_oper?'E':'D') << "  Padding: " << (paddingSet? "SET":"UNSET")) ;
#endif 

	size_t expected_out_size = 0 ;

	const size_t blk_size ( getBlockLength(cipher) ) ; 

	if (enc_oper)
	{
		if (paddingSet) {	
		
			if (1 == blk_size)
			{
				// Stream cipher. The padding is not enforced, not the blocks have fixed size.
				// Even the single byte can be ciphered. Therefore, the is _no need_ of larger buffers:
				expected_out_size = bufin_len ;
			}
			else
			{
				// Standard block cipher -- the padding will EXTEND to the further block:
				expected_out_size = ( ((bufin_len+blk_size)/blk_size)*blk_size ) ;	// Ceil to the futher block_size  // PRQA S 3084 END_METH
			}

		} else {
			expected_out_size = ( ((bufin_len+(blk_size-1))/blk_size)*blk_size ) ;	// Ceil to the block_size 
		}
	
	} else {
	
		expected_out_size = ( ((bufin_len+(blk_size-1))/blk_size)*blk_size ) ;	// Ceil to the block_size

	}

	return expected_out_size ; // PRQA L:END_METH
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_Key::ACS_AES_Key
//
////////////////////////////////////////////////////////////
ACS_SSL::ACS_AES_Key::ACS_AES_Key(ACS_AES_Cipher cipher, const unsigned char*key, const size_t& s) :
	_key(),
	_aes_cipher(cipher),
	_keyLength(0)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Cipher: " << toString(cipher) << "  size of key input buffer: " << s) ;
#endif 

	_keyLength = ACS_SSL::ACS_AES_handle::getKeyLength(cipher) ;

	resetKey(key, s) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished") ;
#endif	
}


ACS_SSL::ACS_AES_Key::ACS_AES_Key() :
	_key(),
	_aes_cipher(),
	_keyLength(0)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;
#endif	
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_Key::~ACS_AES_Key
//
////////////////////////////////////////////////////////////
ACS_SSL::ACS_AES_Key::~ACS_AES_Key() throw() 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) // PRQA S 4631
#endif 
} 


////////////////////////////////////////////////////////////
//
//   ACS_AES_Key::resetCipher
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_AES_Key::resetCipher(const ACS_AES_Cipher cipher)
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Cipher: " << toString(cipher)) ;
#endif 
	
	_aes_cipher = cipher ;
	_keyLength = ACS_SSL::ACS_AES_handle::getKeyLength(cipher) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished") ;
#endif	
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_Key::getKey
//
////////////////////////////////////////////////////////////
unsigned char* ACS_SSL::ACS_AES_Key::getKey(unsigned char* p, const size_t& s) const // PRQA S 4214
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of return buffer: " << s << " _keyLength: " << _keyLength) ;
#endif 
	ACS_COND_THROW( (s<_keyLength), exIllegalValueException("no room to store key")); // PRQA S 3081
	memset(p, 0, s) ; 
	memcpy(p, _key, _keyLength) ;
	return p ; 
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_Key::resetKey
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_AES_Key::resetKey(const unsigned char* p, const size_t& s) 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of input buffer: " << s << " _keyLength: " << _keyLength) ;
#endif 
	ACS_COND_THROW( (s<_keyLength), exIllegalValueException("input key too short")); // PRQA S 3081
	memset(_key, 0, sizeof(_key)) ; 
	memcpy(_key, p, _keyLength) ;
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_Key::clearKey
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_AES_Key::clearKey() 
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;
#endif 
	memset(_key, 0, sizeof(_key)) ; 
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_encrypt
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::ACS_AES_encrypt( ACS_AES_handle& handle, const unsigned char* bufin, const size_t& bufin_len, unsigned char* bufout, size_t& bufout_len )
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Method called.  bufin_len: " << bufin_len << "  bufout_len (allocated by the user): " << bufout_len // PRQA S 3380
			<< "  isFinalized: " << (handle.isFinalized()? 'T':'F') << "  paddingSet: " << (handle.paddingSet()?'T':'F')) ;
#endif 

	ACS_COND_THROW( !handle.getEncOper(), exIllegalValueException("Unexpected ENC operation !")); // PRQA S 3081

	// Method cannot be executed if a finalization has been done !
	ACS_COND_THROW( handle.isFinalized(), exIllegalValueException("Already finalized ! (Out-of-sequence)")); // PRQA S 3081

	const size_t blk_size( handle.getBlockLength() ) ;
	size_t expected_max_out_size = 0 ;
	size_t ciphertext_len = 0 ;
	int processed = 0 ; 
	const bool finalize = (bufin_len==0) || ((bufin_len % blk_size)>=1) ; // SET FINALIZE if an empty buffer is set (bufin_len==0) OR bufin_len not multiple of blk_size // PRQA S 

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		if (bufin_len==0) { ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Input: END-OF-BUFFER past (bufin_len == 0). Finalize will be called") ;  }
		else if (finalize) { ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Input: END-OF-BUFFER reached (bufin_len shorter than blk_size). Finalize will be called") ; }
		else { ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Input: END-OF-BUFFER not reached (finalize not set)") ; }
	ACS_CLASS_END_DEBUG
#endif 
	
	
	if ( handle.paddingSet() )
	{
		// PADDING SET
		expected_max_out_size = handle.getExpected_buffersize(bufin_len) ;
	} else {

		// PADDING NOT SET. The buffer size should be multiple of the block size
		ACS_COND_THROW ( 0 != ( bufin_len % blk_size ), exIllegalValueException("ACS_SSL::ACS_AES_encrypt : padding unset AND mismatching input buffer size") ); // PRQA S 3081
	
		expected_max_out_size = bufin_len ;	// Since the bufin_len is ALREADY multiple than bufin_len and no more bytes will be added
	}
	

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"expected_max_out_size: " << expected_max_out_size) ;
#endif 

	if (bufout_len < expected_max_out_size)
	{
		ostringstream m ;
		m << "no room to store the ciphered buffer. Buffer size: " << bufout_len << "  Expected max size: " << expected_max_out_size ;
		ACS_THROW( exIllegalValueException( m.str() ));
	}


	if (bufin_len>0)
	{
		/** Start the encryption :
			The method EVP_EncryptUpdate_ex only encrypts the part of the plaintext which is MULTIPLE by the blocksize, returning the number of bytes processed
			E.G. Assuming bs = 16 bytes :
			bufin_len =  15 -> processed = 0	(NO PADDING: remainder=15 ; PADDING: remainder=15)
			bufin_len =  16 -> processed = 16	(NO PADDING: remainder=0  ; PADDING: remainder=16)
			bufin_len =  17 -> processed = 16	(NO PADDING: remainder=1  ; PADDING: remainder=1)
			bufin_len = 195 -> processed = 192	(NO PADDING: remainder=3  ; PADDING: remainder=3)
			bufin_len = 256 -> processed = 256	(NO PADDING: remainder=0  ; PADDING: remainder=16)   
		*/
//{ char dump[4096]; ACS_SSL::ACS_SSL_HexToString(bufin, bufin_len, dump, sizeof(dump)) ;
//  excerr << "DUMP OF INPUT: " << dump << "  bufin_len: " << bufin_len << endl ; }
		processed=0;
		if( 1 != EVP_EncryptUpdate(handle.ctx(), bufout, &processed, bufin, bufin_len) )
		{
			ACS_THROW( exIllegalValueException( "Error running EVP_EncryptUpdate" ));
		}
//{ char dump[4096];  ACS_SSL::ACS_SSL_HexToString(bufout, processed, dump, sizeof(dump)) ;
//  excerr << "DUMP OF OUTPUT: " << dump << "  processed: " << processed << endl ; }

		size_t remainder ( bufin_len - processed ) ;
		if ((remainder == 0) && handle.paddingSet() ) { remainder += blk_size ; } // If a whole block was processed without remainder, a new block will be used for padding // PRQA S 3084

		ciphertext_len += processed;
		bufout += processed	;
		if (bufout_len > static_cast<size_t>(processed)) { bufout_len -= processed ; } else { bufout_len=0 ; } 	

	#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_BGN_DEBUG(100) 
			exFDebug dbg ; dbg << exDebugSignature << "  EVP_EncryptUpdate finished. bufin_len: " << bufin_len << "  processed: " << processed \
				<< "  ciphertext_len: " <<  ciphertext_len << "  remainder to be processed by finalize (guess): " << remainder \
				<< "  unused space in bufout: " << bufout_len ;
    		excerr << dbg ;
		ACS_CLASS_END_DEBUG
	#endif 
	}

	/** Finalise the encryption :
		The method EVP_EncryptFinal_ex only processes the remainder, if any. The remainder is AUTOMATICALLY PADDED to fill the block and then ciphered.
		The remainder to be processed (actually, bufin_len-processed) and the data buffer are found in the context
	*/
	if ( finalize )
	{
		if (bufout_len < expected_max_out_size - processed )
		{
			ostringstream m ;
			m << "no room to store the finalized ciphered buffer. Buffer size: " << bufout_len << "  Expected max size: " << (expected_max_out_size - processed) ;
			ACS_THROW( exIllegalValueException( m.str() ));
		}

		processed=0;
		if( 1 != EVP_EncryptFinal_ex(handle.ctx(), bufout, &processed) )
		{
			ACS_THROW( exIllegalValueException( "Error running EVP_EncryptFinal_ex" ));
		}
//{ char dump[4096];  ACS_SSL::ACS_SSL_HexToString(bufout, processed, dump, sizeof(dump)) ;
//  excerr << "DUMP OF OUTPUT: " << dump << "  processed: " << processed << endl ; }

		ciphertext_len += processed;
		bufout += processed	;
		if (bufout_len > static_cast<size_t>(processed)) { bufout_len -= processed ; } else { bufout_len=0 ; } 	

		handle.finalize() ; // Set the finalized flag

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; dbg << exDebugSignature << "  EVP_EncryptFinal_ex finished.  processed: " << processed \
			<< "  ciphertext_len: " <<  ciphertext_len << "  unused space in bufout: " << bufout_len ;
    	excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	}	// END: if ( finalize )

	bufout_len = ciphertext_len ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; dbg << exDebugSignature << "  Method finished.  bufin_len: " << bufin_len << "  bufout_len (as output): " << bufout_len ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	return bufout_len ;
}


////////////////////////////////////////////////////////////
//
//   ACS_AES_decrypt
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::ACS_AES_decrypt( ACS_AES_handle& handle, const unsigned char* bufin, const size_t& bufin_len, unsigned char* bufout, size_t& bufout_len )
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; dbg << exDebugSignature << "  Method called.  bufin_len: " << bufin_len << "  bufout_len (allocated by the user): " << bufout_len 
			<< "  isFinalized: " << (handle.isFinalized()? "T":"F") << "  paddingSet: " << (handle.paddingSet()?"T":"F") ;
    	excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW( handle.getEncOper(), exIllegalValueException("Unexpected ENC operation !"));

	// Method cannot be executed if a finalization has been done !
	ACS_COND_THROW( handle.isFinalized(), exIllegalValueException("Already finalized ! (Out-of-sequence)"));

	const size_t blk_size( handle.getBlockLength() ) ;
	size_t expected_max_out_size ;
	size_t plaintext_len(0) ;
	int processed ; 
	const bool finalize( bufin_len==0 ) ; // SET FINALIZE if an empty buffer is set (bufin_len==0) 

	// When deciphering, the input buffer MUST MATCH the block size 
	ACS_COND_THROW ( ( bufin_len % blk_size ), exIllegalValueException("ACS_SSL::ACS_AES_decrypt : mismatching input buffer size") );

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; 
		if (bufin_len==0) { dbg << exDebugSignature << "  Input: END-OF-BUFFER past (bufin_len == 0). Finalize will be called" ;  }
		else { dbg << exDebugSignature << "  Input: END-OF-BUFFER not reached (finalize not set)" ; }
    	excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	expected_max_out_size = handle.getExpected_buffersize(bufin_len) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; dbg << exDebugSignature << "  expected_max_out_size: " << expected_max_out_size ;
    	excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	if (bufout_len < expected_max_out_size)
	{
		ostringstream m ;
		m << "no room to store the ciphered buffer. Buffer size: " << bufout_len << "  Expected size: " << expected_max_out_size ;
		ACS_THROW( exIllegalValueException( m.str() ));
	}
	

	if (bufin_len>0)
	{
		/** Start the decryption :
		*/
//{ char dump[4096]; ACS_SSL::ACS_SSL_HexToString(bufin, bufin_len, dump, sizeof(dump)) ;
//  excerr << "DUMP OF INPUT: " << dump << "  bufin_len: " << bufin_len << endl ; }
		processed=0;
	    if( 1 != EVP_DecryptUpdate(handle.ctx(), bufout, &processed, bufin, bufin_len) )
		{
			ACS_THROW( exIllegalValueException( "Error running EVP_EncryptUpdate" ));
		}
//{ char dump[4096];  ACS_SSL::ACS_SSL_HexToString(bufout, processed, dump, sizeof(dump)) ;
//  excerr << "DUMP OF OUTPUT: " << dump << "  processed: " << processed << endl ; }

		size_t remainder ( bufin_len - processed ) ;
		if ((remainder == 0) && handle.paddingSet() ) remainder += blk_size ; // If a whole block was processed without remainder, a new block will be used for padding

		plaintext_len += processed;
		bufout += processed	;
		if (bufout_len > static_cast<size_t>(processed)) { bufout_len -= processed ; } else { bufout_len=0 ; } 	

	#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_BGN_DEBUG(100) 
			exFDebug dbg ; dbg << exDebugSignature << "  EVP_EncryptUpdate finished. bufin_len: " << bufin_len << "  processed: " << processed \
				<< "  plaintext_len: " <<  plaintext_len << "  remainder to be processed by finalize (guess): " << remainder \
				<< "  unused space in bufout: " << bufout_len ;
    		excerr << dbg ;
		ACS_CLASS_END_DEBUG
	#endif 
	}

	/** Finalise the decryption :
		The method EVP_EncryptFinal_ex only processes the remainder, if any. The remainder is AUTOMATICALLY PADDED to fill the block and then plained.
		The remainder to be processed (actually, bufin_len-processed) and the data buffer are found in the context
	*/
	if ( finalize )
	{
		if (bufout_len < expected_max_out_size)
		{
			ostringstream m ;
			m << "ACS_SSL::ACS_AES_decrypt : no room to store the finalized plained buffer. Buffer size: " << bufout_len << "  Expected max size: " << expected_max_out_size ;
			ACS_THROW( exIllegalValueException( m.str() ));
		}

		processed=0;
		if( 1 != EVP_DecryptFinal_ex(handle.ctx(), bufout, &processed) )
		{
			ACS_THROW( exIllegalValueException( "Error running EVP_EncryptFinal_ex" ));
		}
//{ char dump[4096];  ACS_SSL::ACS_SSL_HexToString(bufout, processed, dump, sizeof(dump)) ;
//  excerr << "DUMP OF OUTPUT: " << dump << "  processed: " << processed << endl ; }

		plaintext_len += processed; // PRQA S 3000, 3084
		bufout += processed	;
		if (bufout_len > static_cast<size_t>(processed))  { bufout_len -= processed ; } else { bufout_len=0 ; } // PRQA S 3000, 3081, 3084

		handle.finalize() ; // Set the finalized flag

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; dbg << exDebugSignature << "  EVP_EncryptFinal_ex finished.  processed: " << processed \
			<< "  plaintext_len: " <<  plaintext_len << "  unused space in bufout: " << bufout_len ;
    	excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	}	// END: if ( finalize )

	bufout_len = plaintext_len ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ; dbg << exDebugSignature << "  Method finished.  bufin_len: " << bufin_len << "  bufout_len (as output): " << bufout_len ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	return bufout_len ;
}



////////////////////////////////////////////////////////////
//
//   Hex To String
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_SSL_HexToString(	const unsigned char* buffer,	/* IN: binary buffer (e.g. 0x10ab.. ) */ 
									size_t buflen,					/* IN: binary buffer lenght */
									string& hex_string) 			/* IN: output string (e.g. "10ab"...) */
{
	/* Used to convert a binary buffer to a printable hex string */

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10000) 
		exFDebug dbg ;
        dbg << exDebugSignature << "buflen: " << buflen ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	char outbuffer [ 2*buflen + 1 ] ; // Take into account the "\0" to be added by the method below 
	memset(outbuffer, 0x00, sizeof(outbuffer)) ; 
	
	ACS_SSL_HexToString(buffer, buflen, outbuffer, sizeof(outbuffer)) ;

	hex_string = outbuffer ;
}



////////////////////////////////////////////////////////////
//
//   Hex To String
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_SSL_HexToString(	const unsigned char* buffer,	/* IN: binary buffer (e.g. 0x10ab.. ) */ 
									size_t buflen,					/* IN: binary buffer lenght */
									char* outbuffer,				/* IN/OUT: pre-allocated output string buffer (e.g. "10ab...") */
									size_t outbuflen) 				/* IN: output buffer lenght */
{
	/* Used to convert a binary buffer to a preallocated string buffer */
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10000) 
		exFDebug dbg ;
        dbg << exDebugSignature << "buflen: " << buflen << "  outbuflen: " << outbuflen;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	/* Check buffer pointer */
	ACS_COND_THROW( (buflen != 0) && !buffer, // PRQA S 3081
		ACS_SSL_InternalError("Invalid input buffer pointer")
	) ; 
	
	ACS_COND_THROW( !outbuffer , // PRQA S 3081
		ACS_SSL_InternalError("Invalid output buffer pointer")
	) ; 
	
			
	char e[3] ; // PRQA S 4403
	memset(e, 0, sizeof(e)) ;
	memset(outbuffer, 0, outbuflen) ;

	for (size_t i=0; i<min( ((outbuflen-1)/2), buflen ) ; ++i) // PRQA S 3084, 4400
	{
		unsigned char c = buffer[i] ;
		int d = ((c & 0xf0) >> 4) ; // PRQA S 3007, 3084
		e[0] = (d<=9)? (d+'0') : ((d-0xa)+'a') ; // PRQA S 3010, 3380, 4400
		
		d = (c & 0x0f) ;    // PRQA S 4400
		e[1] = (d<=9)? (d+'0') : ((d-0xa)+'a') ; // PRQA S 3010, 3380, 4400
	
		strcat(outbuffer, e) ;
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10000) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
}


////////////////////////////////////////////////////////////
//
//   String To Hex
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_SSL_StringToHex(	const string &hex_string,	/* IN: input hex string (e.g. "10ab" ...) */
									unsigned char* outbuffer,	/* IN/OUT: pre-allocated output binary buffer (e.g. 0x10ab...) */
									size_t* outbuflen) 			/* IN/OUT: binary output binary buffer lenght. 
																	The actual lenght (number of bytes filled) will be returned */
{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10000) 
		exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	ACS_SSL_StringToHex(hex_string.c_str(), hex_string.length(), outbuffer, outbuflen) ; 
	return ; 
}


////////////////////////////////////////////////////////////
//
//   String To Hex
//
////////////////////////////////////////////////////////////
void ACS_SSL::ACS_SSL_StringToHex(	const char* buffer,			/* IN: string buffer (e.g. "10ab.." ) */ 
									size_t buflen,				/* IN: string buffer lenght */
									unsigned char* outbuffer,	/* IN/OUT: pre-allocated output binary buffer (e.g. 0x10ab...) */
									size_t* outbuflen) 			/* IN/OUT: binary output binary buffer lenght. 
																	The actual lenght (number of bytes filled) will be returned */
{

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10000) 
		exFDebug dbg ;
        dbg << exDebugSignature ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	
	/* Check input buffer pointer */	
	ACS_COND_THROW( ( 0 != buflen ) && !buffer , // PRQA S 3081
		ACS_SSL_InternalError("Invalid input buffer pointer")
	) ; 
	
	/* Check output buffer pointer */	
	ACS_COND_THROW( !outbuffer , // PRQA S 3081
		ACS_SSL_InternalError("Invalid output buffer pointer")
	) ; 


	/* Check output buffer size: */
	ACS_COND_THROW( (!outbuflen) || ((*outbuflen) < (buflen/2)) , // PRQA S 3081, 3084
		ACS_SSL_InternalError("Invalid output buffer size")
	) ; 
			

	*outbuflen = 0 ;
	unsigned char d=0 ;
	unsigned char t=0 ;
    for (size_t i=0; i<buflen; ++i)
	{
		char c = buffer[i] ;

		if ((c>='0') && (c<='9')) {
			t = c-'0' ;
        }
		else if ((c>='a') && (c<='f')) {
			t = c-'a'+0x0a ;    // PRQA S 3000, 3010, 4400
        }
		else if ((c>='A') && (c<='F')) {
			t = c-'A'+0x0a ;   // PRQA S 3000, 3010, 4400
        }
		else
		{
			ACS_THROW(ACS_SSL_InternalError("Input out of HEX range")); // PRQA S 3081
		}

		if (0 != (i & 0x01))
		{
			d |= (t&0x0f) ; // PRQA S 3000, 3007, 3010, 4400
			outbuffer[(*outbuflen)] = (d&0xff) ; // PRQA S 3000, 3010, 4400
			(*outbuflen) = (*outbuflen)+1 ; // PRQA S 3084
		}
		else
		{
			d = (t << 4) & (0xf0) ; // PRQA S 3000, 3007, 3010, 4400
		}


	} // for
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10000) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
} 


////////////////////////////////////////////////////////////
//
//   PRIVATE: BLOWFISH ENCRYPT/DECRYPT
//
////////////////////////////////////////////////////////////
size_t
	ACS_SSL::ACS_BLOWFISH_ECB_enc_dec(  // PRQA S 4020
					size_t buflen,				/* IN: Input buffer size */
					const unsigned char* bufin,	/* IN: Input Buffer */ 
					unsigned char* bufout,		/* IN/OUT: Output Buffer -- THE BUFFER MUST BE PRE-ALLOCATED USING ACS_SSL_CEIL8(size) */
					BF_KEY* bfkey,				/* IN: KEY */ 
					int enc)					/* IN: enc flag: 0 to decrypt, 1 to encrypt */
{


	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Buffer size: " << buflen << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Input Buffer: 0x" << HexDumper(bufin, buflen) ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	
	if (buflen<1) { return 0 ; }/* Nothing to do! */
	

	/* Process all blocks EXECEPT THE LAST ONE */
	for (size_t i=0; i<((buflen-1)/8); ++i) // PRQA S 3084, 4400
	{
		BF_ecb_encrypt( (bufin+(i*8)), (bufout+(i*8)), bfkey, enc ) ; // PRQA S 3084, 4400
	}

	size_t last_i = (buflen-1)/8 ; // PRQA S 3084
	unsigned char buflen8 = static_cast<unsigned char>(buflen % size_t(8)) ; // PRQA S 3081, 3084


	/* Process the LAST BLOCK */
	if ( 0 != buflen8 ) 
	{		
		/* Size not multiple of 8: The Cleartext is less than a block */
	
	    unsigned char tmpbuf[8] ; // PRQA S 4403
		if (0 != enc)
		{
			/* ENCODING */
			
			memset (tmpbuf, 0, 8) ; /* Prepare the (shorter) input buffer */ // PRQA S 4400

			memcpy(tmpbuf, (bufin + (8*last_i)), buflen8) ;	/* LEFT-ALIGNED, RIGHT-PADDED */    // PRQA S 3084, 4400
			BF_ecb_encrypt ( reinterpret_cast<unsigned char*>(tmpbuf), (bufout + (8*last_i)), bfkey, enc ) ; // PRQA S 3081, 3084, 4400
		}
		else
		{
			/* DECODING */
	
			BF_ecb_encrypt( (bufin+(last_i*8)), reinterpret_cast<unsigned char*>(tmpbuf), bfkey, enc ) ; // PRQA S 3081, 3084, 4400
			memcpy((bufout + (8*last_i)), tmpbuf, buflen8) ;     // PRQA S 3084, 4400
		}

	}
	else
	{
		/* Size multiple of 8 */
		BF_ecb_encrypt ( (bufin+(last_i*8)), (bufout+(last_i*8)), bfkey, enc ) ;     // PRQA S 3084, 4400
	}
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Method finished." ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Output Buffer: 0x" << HexDumper( bufout, (8*(1+last_i)) ) ;     // PRQA S 3084, 4400
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 


	return ( 8 * (1+last_i) ) ;     // PRQA S 3084, 4400
}


////////////////////////////////////////////////////////////
//
//   BASE64 ENCODE
//
////////////////////////////////////////////////////////////
size_t															/* OUT: Output (encoded) buffer size */
		ACS_SSL::ACS_BASE64_encode(	const unsigned char *src, 	/* IN: binary buffer */ // PRQA S 4020
									char* out,					/* OUT: Decoded Buffer (NOT ZERO TERMINATED). 
																		IT SHOULD BE PRE-ALLOCATED USING ACS_BASE64_encode_buffersize */
									size_t size)				/* IN: input buffer size */ 
{
	/**
	 * Base64 encode and return size data in 'src'. The caller must allocate the output string
	 * return encoded string size otherwise 0
	 */
 	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Input buffer size: " << size ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Input Buffer: 0x" << HexDumper( src, size ) ;	/* Dump input */
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	if(!src) { return 0; }	/* Empty input */

	size_t s = ACS_ALGO_expected_buffersize(ENC_BASE64, size, 1) ;	/* Get expected size */
	
	char *p = 0 ;
    bool alloc = false ;
    if (llabs(src-reinterpret_cast<unsigned char*>(out)) <= llabs(s)) // PRQA S 3000, 3030, 3081
	{
		/* output buffer [partially] overlaps with source: */
		p = reinterpret_cast<char*>(calloc(s, 1));  // PRQA S 3081 2 
		ACS_COND_THROW( !p, ACS_SSL_InternalError("Couldn't allocate memory", errno) ) ;
	    alloc = true ;
    }
	
	
	/* Do not move following line, MUST BE IN THE UPPER SCOPE */
	pattern::CMemoryScopeGuard p_guard ( p ) ;	/* Either the allocated memory, or 0 (no effect) */
		
	if ( 0 == p ) { p = out ; }	/* set the actual value */
	
	char * p_in = p ; 
    for(size_t i=0; i<size; i+=3) // PRQA S 3084, 4400
	{
      
		unsigned char b1 = src[i];
		unsigned char b2=0; 
        unsigned char b3=0; 
        unsigned char b4=0; 
        unsigned char b5=0; 
        unsigned char b6=0; 
        unsigned char b7=0;

		if(i+1<size) { b2 = src[i+1]; } // PRQA S 3084 2

		if(i+2<size) { b3 = src[i+2]; } // PRQA S 3084, 4400

		b4 = b1>>2; // PRQA S 3000, 3010, 4400
		b5 = ((b1&0x3)<<4)|(b2>>4);// PRQA S 3000, 3007, 3010, 4400 2
		b6 = ((b2&0xf)<<2)|(b3>>6);
		b7 = b3&0x3f; // PRQA S 3000, 3010, 4400

		*(p++)= ACS_BASE64_char_encode(b4);
		*(p++)= ACS_BASE64_char_encode(b5);

		if(i+1<size) // PRQA S 3084 
		{
			*(p++) = ACS_BASE64_char_encode(b6);
		} 
		else 
		{
			*(p++) = '=';
		}

		if(i+2<size)  // PRQA S 3084, 4400
		{
			*(p++)= ACS_BASE64_char_encode(b7);
		} 
		else 
		{
			*(p++)= '=';
		}

	}	/* for */
  
    s = (p-p_in) ; // PRQA S 3000

    if ( alloc ) { memcpy(out, p_in, s) ; }

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Output buffer size: " << s ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Output Buffer: 0x" << HexDumper( out, s )  ;	/* Dump output */
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return s;

}


////////////////////////////////////////////////////////////
//
//  BASE64 DECODE
//
////////////////////////////////////////////////////////////
size_t 														/* OUT: Output (decoded) buffer size */
		ACS_SSL::ACS_BASE64_decode(	const char *src, 		/* IN: ascii coded buffer (NOT ZERO-TERMINATED) */ // PRQA S 4020
									unsigned char *dest, 	/* OUT: Decoded Buffer (Of course, not zero-terminated) */
									size_t s) 				/* IN: input buffer size */
{
	/**
	 * Decode the base64 encoded string 'src' into the memory pointed to by
	 * 'dest'. The dest buffer is <b>not</b> NUL terminated.
	 * @param dest Pointer to memory for holding the decoded string.
	 * Must be large enough to recieve the decoded string.
	 * @param src A base64 encoded string.
	 * @return TRUE (the length of the decoded string) if decode
	 * succeeded otherwise FALSE.
	 */
   	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

   	unsigned char *p = dest;
    size_t sz = 0 ;
    unsigned char *buf = reinterpret_cast<unsigned char*> (calloc(sizeof(unsigned char), s+1));	/* To be sure ... */ // PRQA S 3081, 3084
    ACS_COND_THROW( !buf, ACS_SSL_InternalError("Couldn't allocate memory", errno) ) ;
	pattern::CMemoryScopeGuard buf_guard(buf) ;	/* no free is needed */

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Input buffer size: " << s ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
        dbg << exDebugSignature << "Dumping Input Buffer: 0x" << HexDumper(src, s )  ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

  	if( (0 == src) || (0 == *src ) ) { return 0 ; }
  
  	
	/* Ignore non base64 chars as per the POSIX standard */
    size_t l = 0 ;
    size_t k = 0 ;
    for(k=0, l=0; k<s; ++k) // PRQA S 4230
	{
		if(ACS_BASE64_is_base64(src[k])) 
        {
            buf[l++]= src[k];
        }
    } 
    
	for(k=0; k<l; k+=4) { // PRQA S 3084, 4230, 4400
      
		char c1= buf[k]; 
        char c2='A'; 
        char c3='A'; 
        char c4='A';

		if(k+1<l) { c2= buf[k+1]; } // PRQA S 3084

		if(k+2<l) { c3 = buf[k+2]; } // PRQA S 3084, 4400

		if(k+3<l) { c4 = buf[k+3]; } // PRQA S 3084, 4400

		unsigned char b1 = ACS_BASE64_char_decode(c1);
		unsigned char b2 = ACS_BASE64_char_decode(c2);
		unsigned char b3 = ACS_BASE64_char_decode(c3);
		unsigned char b4 = ACS_BASE64_char_decode(c4);

		*(p++)=((b1<<2)|(b2>>4) ); // PRQA S 3000, 3007, 3010, 4400

		if(c3 != '=') { *(p++)=(((b2&0xf)<<4)|(b3>>2) ); } // PRQA S 3000, 3007, 3010, 4400

		if(c4 != '=') { *(p++)=(((b3&0x3)<<6)|b4 ); } // PRQA S 3000, 3007, 3010, 4400
	
    }	/* for */
 
	sz = size_t(p-dest) ; // PRQA S 3081
 
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(20) 
		exFDebug dbg ; 
		dbg << exDebugSignature << "Method finished. Output buffer size: " << sz  ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG	
	ACS_CLASS_BGN_DEBUG(100)
        exFDebug dbg ; 
		dbg << exDebugSignature << "Dumping Output Buffer: 0x" << HexDumper( dest, sz )  ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

    return sz;

}


////////////////////////////////////////////////////////////
//
//  ALGO ENCDEC
//
////////////////////////////////////////////////////////////
size_t 														/* OUT: Output buffer size */
		ACS_SSL::ACS_ALGO_encdec(	EncryptionAlgo al,				/* ALGO used */ // PRQA S 4020
									const unsigned char *bufin,		/* IN: input buffer */ 
									size_t s_in, 					/* IN: input buffer size (bytes) */
									unsigned char *bufout, 			/* OUT: PRE-ALLOCATED Output buffer */
									const vector <void*> &in_values,	/* IN:  optional input parameters (keys, etc.) */ 
									vector <void*> &out_values, 	/* REF:  optional output parameters */ 
									int enc)						/* IN: encoding flag: 0 = decoding, 1 = encoding */
{		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
        exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al 
			<< " Input buffer size: " << s_in 
            << " Input vector size: " << in_values.size() 
            << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	// EnrCar:: 
	// This method has multiple returns in order to keep its complexity
	// as low as possible

	if (0 != enc) {
		return ACS_ALGO_encode (al, bufin, s_in, bufout, in_values, out_values) ;
    }
	else {
		return ACS_ALGO_decode (al, bufin, s_in, bufout, in_values, out_values) ;
    }
}


////////////////////////////////////////////////////////////
//
//  ALGO ENCDEC_FromFile
//
////////////////////////////////////////////////////////////
size_t 																		/* OUT: Actual output buffer size */
		ACS_SSL::ACS_ALGO_encdec_FromFile(	EncryptionAlgo al,				/* ALGO used */ 
											const string& fileIn,				/* IN: Input File Name */ 
											unsigned char *bufout, 			/* OUT: Output buffer */
											size_t bufout_size,				/* IN: Size of preallocated output buffer */	
											const vector <void*> &in_values,	/* IN: Optional input parameters (keys, etc.) */ 
											vector <void*> &out_values, 	/* REF: optional output parameters */ 
											int enc	)						/* IN: encoding flag: 0 = decoding, 1 = encoding */
{		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Algo: " << al
			<< " Input File: " << fileIn 
            << " Allocated output buffer: " << bufout_size 
			<< " Input vector size: " << in_values.size() << " enc flag: " << enc) ;
#endif 
	
	const size_t toRead = ACS_ALGO_expected_buffersize(al, 1048576, ~enc) ;	/* Evaluate the chunk size*/
	const size_t bufferSize = max(2*ACS_ALGO_expected_buffersize(al, bufout_size, ~enc), toRead) ;	/* Evaluate the input buffer size*/ // PRQA S 3084


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Allocating chunk buffer " << toRead << " bytes" ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	/* Allocate chunk */
	unsigned char *buftmp = reinterpret_cast<unsigned char*>(calloc (toRead ,1)) ; // PRQA S 3081
	
	/* Check input buffer */
	ACS_COND_THROW( !buftmp, ACS_SSL_InternalError("Unable to allocate chunk pointer", errno) ) ;
	pattern::CMemoryScopeGuard buftmp_guard(buftmp) ;	/* Automatically frees the buffer */
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Allocating input buffer " << bufferSize << " bytes" ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	/* Allocate input buffer */
	unsigned char *bufin = reinterpret_cast<unsigned char*>( calloc (bufferSize ,1) ) ; // PRQA S 3081
	
	/* Check input buffer */
	ACS_COND_THROW( !bufin, ACS_SSL_InternalError("Unable to allocate input pointer", errno) ) ;
	pattern::CMemoryScopeGuard bufin_guard(bufin) ;	/* Automatically frees the buffer */
	
	
	
	size_t toAlgo = 0 ;
	{
        int fd = open (fileIn.c_str(), O_RDONLY) ; 
	    ACS_COND_THROW( (fd < 0), ACS_SSL_InternalError("Unable to open input file", errno) ) ;
        pattern::FDGuard fd_guard(fd) ;
	    do	
	    {		
		    ssize_t r = read (fd, buftmp, toRead) ;

		    if (r <= 0) { break ; }	/* Got EOF */

		    if (toAlgo+r <= bufferSize) {           // PRQA S 3000, 3084
			    memcpy(bufin+toAlgo, buftmp, r);    // PRQA S 3000, 3084
            }
		    else { break ; }

		    toAlgo += r ;    // PRQA S 3000, 3084

	    } while (toAlgo < bufferSize) ;	/* Prepare next loop */
    }
	if (toAlgo >= bufferSize)
	{
		/* Didn't get EOF so Input file could be larger. */

        ostringstream m ;
        m << al << " Input buffer overflow" ; 
        ACS_THROW(ACS_SSL_InternalError(m.str())); // PRQA S 3081
    }
	
	ACS_COND_THROW( 0 == toAlgo, ACS_SSL_InternalError("Input file was empty") ) ; // PRQA S 3081
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(30) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Read: " << toAlgo << " bytes from file: " << fileIn ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	/* Checking output buffer */
	size_t tmpSize = ACS_ALGO_expected_buffersize(al, toAlgo, enc) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Required output buffer size: " << tmpSize << " bytes." ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	ACS_COND_THROW( (bufout_size < tmpSize), ACS_SSL_InternalError("Output buffer overflow") ) ; // PRQA S 3081


	size_t s_out = ACS_ALGO_encdec(al, bufin, toAlgo, bufout, in_values, out_values, enc) ;	/* Process data buffer */
		
	ACS_COND_THROW( 0 == s_out, ACS_SSL_InternalError("Unable to process data") ) ; // PRQA S 3081


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(30) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Returning: " << s_out << " bytes" ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return s_out ;
}


////////////////////////////////////////////////////////////
//
//  ALGO ENCDEC_ToFile
//
////////////////////////////////////////////////////////////
size_t 																	/* OUT: Output buffer size */
		ACS_SSL::ACS_ALGO_encdec_ToFile(EncryptionAlgo al,				/* ALGO used */ 
										const unsigned char *bufin,		/* IN: Input buffer */
										size_t s_in,					/* IN: Input buffer size */	
										const string &fileOut, 			/* OUT: Output File Name */
										const vector <void*> &in_values,	/* IN: Optional input parameters (keys, etc.) */ 
										vector <void*> &out_values, 	/* REF: optional output parameters */ 
										int enc	)						/* IN: encoding flag: 0 = decoding, 1 = encoding */
{		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al
			<< " Input buffer size: " << s_in 
            << " Output file: " << fileOut 
            << " Input vector size: " << in_values.size() 
            << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 
	
	/* Allocate output buffer */
	size_t tmpSize = ACS_ALGO_expected_buffersize(al, s_in, enc) ;

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(100) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Allocating " << tmpSize << " bytes." ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	unsigned char *bufout = reinterpret_cast<unsigned char*>( calloc (tmpSize ,1) );	/* Allocate temporary buffer */ // PRQA S 3081

	/* Check output buffer */
	ACS_COND_THROW( !bufout, ACS_SSL_InternalError("Unable to allocate output pointer", errno) ) ;
	pattern::CMemoryScopeGuard bufout_guard(bufout) ;


	size_t s_out = ACS_ALGO_encdec(al, bufin, s_in, bufout, in_values, out_values, enc) ;	/* Process data buffer */
		
	ACS_COND_THROW( 0 == s_out, ACS_SSL_InternalError("Unable to process data") ) ; // PRQA S 3081

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(30) 
		exFDebug dbg ;
		dbg << exDebugSignature << "ACS_ALGO_encdec returned: " << s_out ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	/* Write processed buffer on file */
	int fd = open (fileOut.c_str(), O_RDWR|O_CREAT|O_TRUNC, 0666) ; 
	ACS_COND_THROW( (fd < 0), ACS_SSL_InternalError("Unable to open output file", errno) ) ;
    pattern::FDGuard fd_guard(fd) ;
	ssize_t toWrite = 1024*1024 ; // PRQA S 4400
	ssize_t offset = 0 ;
	ssize_t writeTotal = s_out ; // PRQA S 3000
	do
	{
		if (writeTotal < toWrite) { toWrite = writeTotal ; }
	
		writeTotal -= toWrite ; /* prepare next loop */
	
		ssize_t written = write (fd, bufout+offset, toWrite) ; // PRQA S 3000
		
		if (written != toWrite)
		{
            int eno = errno ;
			ostringstream m ;
			m << "Error writing output file: " << fileOut 
				<< " [" << written << "/" <<  toWrite << "]" ;
			ACS_THROW(ACS_SSL_InternalError(m.str(), eno)); // PRQA S 3081
		}
		
		offset += written ;
		
	} while (writeTotal > 0) ;


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(30) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Wrote: " << s_out << " bytes in file: " << fileOut ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return s_out ;
}
			
		
////////////////////////////////////////////////////////////
//
//  ALGO ENCDEC_File
//
////////////////////////////////////////////////////////////
size_t 																	/* OUT: Output (encoded) buffer size */
		ACS_SSL::ACS_ALGO_encdec_File(	EncryptionAlgo al,				/* ALGO used */ 
										const std::string &fileIn,				/* IN: Input File Name */ 
										const std::string &fileOut, 			/* OUT: Output File Name */
										const vector <void*> &vin,		/* IN: Optional input parameters (keys, etc.) */ 
										vector <void*> &vout, 			/* REF: optional output parameters */ 
										int enc	)						/* IN: encoding flag: 0 = decoding, 1 = encoding */
{		
	unsigned char buffer [4*1024*1024] ; // PRQA S 4403
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al
			<< " Input File: " << fileIn 
            << " Output file: " << fileOut 
            << " Input vector size: " << vin.size() 
            << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 


// OPEN INPUT FILE
    int fdi = open(fileIn.c_str(), O_RDONLY) ; 
	ACS_COND_THROW( fdi < 0 , 
        ACS_SSL_InternalError("Cannot open input file: " + fileIn, errno)) ;

	pattern::FDGuard g1(fdi) ;	/* close on fdi not required */
	
	
	// OPEN OUTPUT FILE		
    int fdo = open(fileOut.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0666) ;	
    try {
	    ACS_COND_THROW( fdo < 0 ,ACS_SSL_InternalError("Cannot open output file: " + fileOut, errno)) ; // PRQA S 3081
	}
    catch(exception &x) {
        excerr << x ;
        exit(1) ;
    }
	
	pattern::FDGuard g2(fdo) ;	/* close on fdo not required */


	size_t toRead = ACS_ALGO_expected_buffersize(al, (1024*1024), enc) ; // PRQA S 4400

	size_t s_out = 0; 
    ssize_t bytesRead = 0 ;
    while ( (bytesRead = read(fdi, buffer, toRead)) > 0 ) // PRQA S 4236
	{	
		ssize_t toWrite = ACS_SSL::ACS_ALGO_encdec(	al, buffer, bytesRead, buffer, vin, vout, enc )  ;  // PRQA S 3000

		ssize_t bytesWritten = write(fdo, buffer, toWrite) ; // PRQA S 3000
        try {
           ACS_COND_THROW(bytesWritten != toWrite, ACS_SSL_InternalError("Cannot write file: " + fileOut, errno)) ; // PRQA S 3081
        }
		catch(exception &x)
		{
			ACS_LOG_NOTIFY_EX(x) ;
			exit(1) ;
		}
		
		s_out += toWrite ; // PRQA S 3000, 3084
	}
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al
			<< " Input File: " << fileIn 
            << " Output file: " << fileOut 
            << " Output vector size: " << vout.size() 
            << " enc flag: " << enc ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return s_out ;
}


////////////////////////////////////////////////////////////
//
//  ALGO DECODE
//
////////////////////////////////////////////////////////////
size_t 																/* OUT: Output (decoded) buffer size */
		ACS_SSL::ACS_ALGO_decode(	EncryptionAlgo al_in,			/* ALGO used */  // PRQA S 4020
									const unsigned char *bufin,		/* IN: input (encoded) buffer */ 
									size_t s_in, 					/* IN: input buffer size (bytes) */
									unsigned char *bufout, 			/* OUT: PRE-ALLOCATED Output buffer */
									const vector <void*>& in_values_in,	/* IN:  optional input parameters (keys, etc.) */ 
									vector <void*> &out_values) 	/* REF:  optional output parameters */ 
{
	// EnrCar:: 
	// This method has multiple returns in order not to increase its complexity

	// EnrCar:: 
	// This method has a high complexity since all the available algorithms are to be checked
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al_in
			<< " Input buffer size: " << s_in 
            << " Input vector size: " << in_values_in.size() ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	if (s_in < 1) { return 0 ; }/* Empty input */

	/* Check input buffer */
	ACS_COND_THROW( !bufin, ACS_SSL_InternalError("NULL input pointer") ) ; // PRQA S 3081
	
	/* Check output buffer */
	ACS_COND_THROW( !bufout, ACS_SSL_InternalError("NULL output pointer") ) ; // PRQA S 3081
	
	// A local copy of the input vector is peformed (only the VALUES of the pointer are copied, no content is duplicated)
	// The rationale is to DROP the salt, passed as last argument in the in_values, in order to detect if enough arguments are passed.
	vector <void*> in_values (in_values_in) ; // Local copy

	out_values = vector <void*>() ; // empty

	/* Default behaviour */

	/* Get the algo */
	EncryptionAlgo al = EncryptionAlgo_Algo(al_in) ;
	
	/* Get the flags */
	EncryptionAlgo flags = EncryptionAlgo_Flags(al_in) ;
	
	const unsigned char* buf_tmp = 0 ;
	size_t s_tmp = 0 ;
	if (0 != (flags & ENC_BASE64)) // PRQA S 3003
	{
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_BGN_DEBUG(20) 
			exFDebug dbg ;
		    dbg << exDebugSignature << "Performing the BASE64 post-encoding. Input buffer size: " << s_in ; 
            excerr << dbg ;
		ACS_CLASS_END_DEBUG
#endif 
		s_tmp = ACS_BASE64_decode(reinterpret_cast<const char*>(bufin), bufout, s_in) ;	/* Decode BASE64  from bufin into bufin */ // PRQA S 3030, 3081
		buf_tmp = bufout ;
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_BGN_DEBUG(20) 
			exFDebug dbg ;
		    dbg << exDebugSignature << "Performing the BASE64 post-encoding. Output buffer size: " << s_tmp ; 
            excerr << dbg ;
		ACS_CLASS_END_DEBUG
#endif 		
	}
	else
	{
		s_tmp = s_in ;
		buf_tmp = bufin ;
	}

    size_t s_out = 0 ;
	switch (al)
	{
		case ENC_NONE :
		{
			if (buf_tmp != bufout) { memcpy(bufout, buf_tmp, s_tmp) ; }
			s_out = s_tmp ;
		}
		break ;
		
		
		case ENC_BLOWFISH :
		{
			ACS_COND_THROW( (in_values.size() < 1), ACS_SSL_InternalError("BLOWFISH. Key not provided") ) ; // PRQA S 3081 2
			BF_KEY *bfkey = reinterpret_cast<BF_KEY *>( in_values[0] ) ;
			s_out = ACS_BLOWFISH_ECB_decrypt (buf_tmp, bufout, s_tmp, bfkey) ;	
			in_values.erase(in_values.begin()) ; // Drop used parameters 
		}
		break ;
		
		
		case ENC_DES :
		{
			// The KEY is stored in in_values[0], as uchar buffer of type: DES_key_schedule (thus the malloc)
			// PLEASE NOTE that the caller of this method MUST FILL in_values[0] with the conversion of the key
			// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
			ACS_COND_THROW( (in_values.size() < 1), ACS_SSL_InternalError("Simple DES. Key not provided") ) ; // PRQA S 3081
			DES_key_schedule deskey; memcpy(reinterpret_cast<unsigned char*>(&deskey), in_values[0], sizeof(deskey));
			s_out = ACS_DES_decrypt1( buf_tmp, bufout, s_tmp, deskey) ;
			in_values.erase(in_values.begin()) ; // Drop used parameters 
		}
		break ;
		
		
		case ENC_DESOFB :
		{
			// The KEY is stored in in_values[0], as uchar buffer of type: DES_key_schedule (thus the malloc)
			// PLEASE NOTE that the caller of this method MUST FILL in_values[0] with the conversion of the key
			// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
			// The IV is stored in in_values[1]
			ACS_COND_THROW( (in_values.size() < 2), ACS_SSL_InternalError("DES OFB. Key or IV not provided") ) ; // PRQA S 3081
			DES_key_schedule deskey; memcpy(reinterpret_cast<unsigned char*>(&deskey), in_values[0], sizeof(deskey));
            DES_cblock iv ; memcpy(static_cast<unsigned char*>(iv), in_values[1], DES_KEY_SZ) ; // PRQA S 3081
			s_out = ACS_DES_OFB_encdec( buf_tmp, s_tmp, bufout, deskey, iv) ;
			in_values.erase(in_values.begin()) ; in_values.erase(in_values.begin()) ; // Drop used parameters 
		}
		break ;
		
		case ENC_TDES :
		{
			// The 3 KEYS are stored in in_values[i] {i=0,1,2}, each one as uchar buffer of type: DES_key_schedule (thus the malloc)
			// PLEASE NOTE that the caller of this method MUST FILL in_values[i] with the conversion of the key
			// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
			ACS_COND_THROW( (in_values.size() < 3), ACS_SSL_InternalError("TDES. Keys not provided") ) ; // PRQA S 3081
			DES_key_schedule deskey[3] ; // PRQA S 4403
			for (size_t i=0; i<3; ++i) { memcpy(reinterpret_cast<unsigned char*>(&(deskey[i])), in_values[i], sizeof(deskey[i])); }	/* Set Keys */  // PRQA S 3000, 3081, 4400

			s_out = ACS_DES_decrypt3( buf_tmp, bufout, s_tmp, deskey[0], deskey[1], deskey[2]) ; // PRQA S 4400
			for (size_t i=0; i<3; ++i) { in_values.erase(in_values.begin()) ; } // Drop used parameters 
		}
		break ;
		
		
		case ENC_TDESOFBI :
		{
			// The 3 KEYS are stored in in_values[i] {i=0,1,2}, each one as uchar buffer of type: DES_key_schedule (thus the malloc)
			// PLEASE NOTE that the caller of this method MUST FILL in_values[i] with the conversion of the key
			// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
			// The 3 IVs are stored in in_values[i] {i=3,4,5}
			ACS_COND_THROW( (in_values.size() < 6), ACS_SSL_InternalError("TDES OFB-I. Key or IV not provided") ) ; // PRQA S 3081
			DES_key_schedule deskey[3] ; // PRQA S 4403
			for (int i=0; i<3; ++i) { memcpy(reinterpret_cast<unsigned char*>(&(deskey[i])), in_values[i], sizeof(deskey[i])); } 	/* Set Keys */  // PRQA S 3000, 3081, 4400
            DES_cblock iv[3] ; // PRQA S 4403
			for (int i=0; i<3; ++i) {memcpy(static_cast<unsigned char*>(iv[i]), in_values[3+i], DES_KEY_SZ) ;}	/* Set IVs */   // PRQA S 3000, 3081, 4400
			
			s_out = ACS_DES_TOFB_I_encdec( buf_tmp, s_tmp, bufout, deskey[0], deskey[1], deskey[2], iv[0], iv[1], iv[2]) ; // PRQA S 4400
			for (size_t i=0; i<6; ++i) { in_values.erase(in_values.begin()) ; } // Drop used parameters 
		}
		break ;
	
	
		case ENC_AES128CBC :
		case ENC_AES128OFB :
		case ENC_AES192CBC :
		case ENC_AES192OFB :
		case ENC_AES256CBC :
		case ENC_AES256OFB :
		{
			// in_values[0]: (unsigned char*) key_buffer (16/24/32 bytes)
			// in_values[1]: (unsigned char*) iv_buffer (16 bytes)

			ACS_COND_THROW( (in_values.size() < 2), ACS_SSL_InternalError("ENC_AES128CBC. Key or IV not provided") ) ; // PRQA S 3081, 4400
			ACS_SSL::ACS_AES_Cipher cipher ; // PRQA S 4101
			switch (al)
			{
				case ENC_AES128CBC: { cipher = ACS_SSL::AES_128_CBC ; break ; }
				case ENC_AES128OFB: { cipher = ACS_SSL::AES_128_OFB ; break ; }
				case ENC_AES192CBC: { cipher = ACS_SSL::AES_192_CBC ; break ; }
				case ENC_AES192OFB: { cipher = ACS_SSL::AES_192_OFB ; break ; }
				case ENC_AES256CBC: { cipher = ACS_SSL::AES_256_CBC ; break ; }
				case ENC_AES256OFB: { cipher = ACS_SSL::AES_256_OFB ; break ; }
				default: { ACS_THROW(ACS_SSL_InternalError("Cipher unsupported")); } // PRQA S 3081
			}
	
			const size_t key_length = ACS_SSL::ACS_AES_handle::getKeyLength(cipher) ;
			ACS_SSL::ACS_AES_Key aes_key(cipher, static_cast<unsigned char*>(in_values[0]), key_length) ; // PRQA S 3081
			const size_t iv_length = ACS_SSL::ACS_AES_handle::getIVLength(cipher) ;
			ACS_SSL::ACS_AES_IV iv_buffer ;
			memcpy(iv_buffer, in_values[1], iv_length) ; 

			const bool enc(false) ; 
			const bool padding(true) ; // This is pratically ignored in case of OFB, since blocklenght=1 byte
			// Create the instance of the ACS_AES_handle
			ACS_SSL::ACS_AES_handle aes_handle( cipher, aes_key, iv_buffer, enc, padding ) ;
		
			size_t bufout_len ( aes_handle.getExpected_buffersize(s_in) ) ;
			s_out = ACS_SSL::ACS_AES_decrypt(aes_handle, buf_tmp, s_tmp, bufout, bufout_len) ; // bufout_len will be returned
			if (! aes_handle.isFinalized())
			{
				// Trigger finalize (process an empty input buffer)
				const size_t bufin_len_finalize (0) ;
				s_out += ACS_SSL::ACS_AES_decrypt(aes_handle, buf_tmp, bufin_len_finalize, (bufout+s_out), bufout_len) ; // PRQA S 3084
			}
			
			in_values.erase(in_values.begin()) ; in_values.erase(in_values.begin()) ; // Drop used parameters 			
		}
		break ;	
	
	
		default :
    	{
        	ostringstream m ;
        	m << static_cast<unsigned int>(al) << " UNIMPLEMENTED" ; // PRQA S 3081 2
        	ACS_THROW(ACS_SSL_InternalError(m.str()));
    	}
	
	}	/* switch */

	
	if (0 != (flags & ENC_PREPEND_SALT)) // PRQA S 3003
	{
		#ifdef ACS_SSL_HAS_DEBUG_ENABLED
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing salt from the end. Output buffer size (before): " << s_out) ;
		#endif 

		ACS_COND_THROW( (in_values.size() < 1), ACS_SSL_InternalError("Salt expected but not provided") ) ; 
		

		// Compute the size of the salt (from the coded salt), not taking into account the padding. Method remove_salt_from_buffer will.
		const size_t salt_size ( decode_salt(static_cast<unsigned char*>(in_values.back())) ) ; 

		if (salt_size > 0)
		{
			// The input and out buffer of the remover match (of course). The output buffer will be shorter than the input
			s_out = ACS_SSL::remove_salt_from_buffer ( salt_size, al_in, bufout, s_out, bufout, s_out ) ;
		
			// WHERE HERE: s_out is the new size of the output buffer, after the removal of the padded salt.
		}
		
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "(Unpadded) salt size: " << salt_size << "  Output buffer size (after removal of the padded salt): " << s_out ) ;
#endif 
		
		in_values.pop_back() ;	// Remove the last argument, since has been processed. This helps finding invalid dimension of the (remaining) arguments.
	} 


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(10) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al
			<< " Output buffer size: " << s_out 
            << " Output vector size: " << out_values.size() ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return s_out ;
}	


////////////////////////////////////////////////////////////
//
//  ALGO ENCODE
//
////////////////////////////////////////////////////////////
size_t 														/* OUT: Output (encoded) buffer size */
		ACS_SSL::ACS_ALGO_encode(	EncryptionAlgo al_in,			/* ALGO used */  // PRQA S 4020
									const unsigned char *buffer_in,		/* IN: input (decoded) buffer */ 
									size_t s_in, 					/* IN: input buffer size (bytes) */
									unsigned char *bufout, 			/* OUT: PRE-ALLOCATED Output buffer */
									const vector <void*> &in_values_in,	/* IN:  optional input parameters (keys, etc.) */ 
									vector <void*> &out_values) 	/* REF:  optional output parameters */ 
{	

	// EnrCar:: 
	// This method has multiple returns in order not to increase its complexity

	// EnrCar:: 
	// This method has a high complexity since all the available algorithms are to be checked
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Algo: " << al_in
			<< " Input buffer size: " << s_in 
            << " Input vector size: " << in_values_in.size()) ;
#endif 
	
	/* Check input buffer */
	ACS_COND_THROW( !buffer_in, ACS_SSL_InternalError("NULL input pointer") ) ; // PRQA S 3081
	
	/* Check output buffer */
	ACS_COND_THROW( !bufout, ACS_SSL_InternalError("NULL output pointer") ) ; // PRQA S 3081
			
	
	// A local copy of the input vector is peformed (only the VALUES of the pointer are copied, no content is duplicated)
	// The rationale is to DROP the salt, passed as last argument in the in_values, in order to detect if enough arguments are passed.
	vector <void*> in_values (in_values_in) ; // Local copy

	out_values = vector <void*>() ;	/* Default behaviour */

	/* Get the algo */
	EncryptionAlgo al = EncryptionAlgo_Algo(al_in) ;
	
	/* Get the flags */
	EncryptionAlgo flags = EncryptionAlgo_Flags(al_in) ;
	
	/* Prepare the temporary input buffer, if needed */
	unsigned char* tmp_bufin = NULL ; 
	bool toAllocate_in (false) ; 
	
	if (0 != (flags & ENC_PREPEND_SALT)) // PRQA S 3003
	{
		#ifdef ACS_SSL_HAS_DEBUG_ENABLED
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Prepending padded salt at the beginning. Input buffer size (before): " << s_in) ;
		#endif 

		ACS_COND_THROW( (in_values.size() < 1), ACS_SSL_InternalError("Salt expected but not provided") ) ; 
		
		// The total size of the padded-salt is computed, so for example "test" will be coded as 8-bytes: "test\x00\x00\x00\x00"
		// This is used to compute the output buffer size (it will be: input_size + padded_salt_size)
		const size_t padded_salt_size ( decode_salt(static_cast<unsigned char*>(in_values.back()), al_in) ) ; // Also take into account the total (padded) size

		ACS_COND_THROW( (padded_salt_size > 1024), ACS_SSL_InternalError("Salt size too big, probably misleading") ) ; 
		
		if (padded_salt_size > 0)
		{
			const size_t out_buffer_size ( ACS_SSL::embed_salt_in_buffer (	static_cast<unsigned char*>(in_values.back()), al_in, buffer_in, s_in, &tmp_bufin, (s_in+padded_salt_size)) ) ;

	 		ACS_COND_THROW( (NULL == tmp_bufin), ACS_SSL_InternalError("Out of memory", errno) ) ;  
			toAllocate_in = true ; // Enable the input buffer guard (tmp_bufin will be fried)
	 	
			ACS_COND_THROW( (out_buffer_size != (s_in+padded_salt_size)), ACS_SSL_InternalError("Internal size error", errno)) ;  
			 							
			// Update the total size of the input buffer
			s_in += padded_salt_size ;

			#ifdef ACS_SSL_HAS_DEBUG_ENABLED
				ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Padded salt size: " << padded_salt_size << "  Input buffer size (after): " << s_in ) ;
			#endif 
		
		} else { toAllocate_in = false ; }
		
		in_values.pop_back() ;	// Remove the last argument, since has been processed. This helps finding invalid dimension of the (remaining) arguments.
		
	} else {

		toAllocate_in = false ; // Disable the input buffer guard (no allocation performed)
	
	}


	// Set the correct value for the input buffer to use:
	const unsigned char* bufin ( (true==toAllocate_in)? tmp_bufin : buffer_in) ; 

	// Set a guard for the temporary input buffer (if needed) ; 
	pattern::CMemoryScopeGuard inbuffer_guard( (true == toAllocate_in)? tmp_bufin : NULL ) ; 

	// Check if, despite a possible extension of the input buffer due to salt, it is still empty ...
	if (s_in < 1) { return 0 ; }	/* Empty input */


    size_t s_out = 0 ;
	switch (al)
	{
		case ENC_NONE :
		    {
			    memcpy(bufout, bufin, s_in) ; 
                s_out = s_in ;
		        break ;
		    }
		
		
		case ENC_BLOWFISH :
		    {
			    ACS_COND_THROW( (in_values.size() < 1), ACS_SSL_InternalError("BLOWFISH. Key not provided") ) ; // PRQA S 3081 2
			    BF_KEY *bfkey = reinterpret_cast<BF_KEY *>( in_values[0] );
			    s_out = ACS_BLOWFISH_ECB_encrypt (bufin, bufout, s_in, bfkey) ;	/* evaluate output size */
		        break ; 
		    }
		
		
		case ENC_DES :
		    {
				// The KEY is stored in in_values[0], as uchar buffer of type: DES_key_schedule (thus the malloc)
				// PLEASE NOTE that the caller of this method MUST FILL in_values[0] with the conversion of the key
				// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
			
			    ACS_COND_THROW( (in_values.size() < 1), ACS_SSL_InternalError("Simple DES. Key not provided") ) ; // PRQA S 3081
				DES_key_schedule deskey ; memcpy(reinterpret_cast<unsigned char*>(&deskey), in_values[0], sizeof(deskey));

			    s_out = ACS_DES_encrypt1( bufin, s_in, bufout, deskey) ;
		        break ;
		    }
		
		
		case ENC_DESOFB :
		    {			
				// The KEY is stored in in_values[0], as uchar buffer of type: DES_key_schedule (thus the malloc)
				// PLEASE NOTE that the caller of this method MUST FILL in_values[0] with the conversion of the key
				// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
				// The IV is stored in in_values[1]

			    ACS_COND_THROW( (in_values.size() < 2), ACS_SSL_InternalError("DES OFB. Key or IV not provided") ) ; // PRQA S 3081
				DES_key_schedule deskey ; memcpy(reinterpret_cast<unsigned char*>(&deskey), in_values[0], sizeof(deskey));
                DES_cblock iv ; memcpy(static_cast<unsigned char*>(iv), in_values[1], DES_KEY_SZ) ; // PRQA S 3081
			    s_out = ACS_DES_OFB_encdec( bufin, s_in, bufout, deskey, iv) ;
		        break ;
		    }
		
			
		case ENC_TDES :
		    {
				// The 3 KEYS are stored in in_values[i] {i=0,1,2}, each one as uchar buffer of type: DES_key_schedule (thus the malloc)
				// PLEASE NOTE that the caller of this method MUST FILL in_values[i] with the conversion of the key
				// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey

			    ACS_COND_THROW( (in_values.size() < 3), ACS_SSL_InternalError("TDES. Key or IV not provided") ) ; // PRQA S 3081, 4400
			    DES_key_schedule deskey[3] ;  // PRQA S 4403

			    for (int i=0; i<3; ++i) { memcpy(reinterpret_cast<unsigned char*>(&(deskey[i])), in_values[i], sizeof(deskey[i])); } // PRQA S 3000, 4400
			    s_out = ACS_DES_encrypt3( bufin, s_in, bufout, deskey[0], deskey[1], deskey[2]) ; // PRQA S 4400
		        break ;	
		    }
		
		
		case ENC_TDESOFBI :
		    {
				// The 3 KEYS are stored in in_values[i] {i=0,1,2}, each one as uchar buffer of type: DES_key_schedule (thus the malloc)
				// PLEASE NOTE that the caller of this method MUST FILL in_values[i] with the conversion of the key
				// from string (e.g. 'P''A''S''S''W'...) to DES_key_schedule, using proper method: ACS_DES_setkey
				// The 3 IVs are stored in in_values[i] {i=3,4,5}
			    
				ACS_COND_THROW( (in_values.size() < 6), ACS_SSL_InternalError("TDES OFB-I. Key or IV not provided") ) ; // PRQA S 3081
			    DES_key_schedule deskey[3] ; // PRQA S 4403

			    for (int i=0; i<3; ++i) { memcpy(reinterpret_cast<unsigned char*>(&(deskey[i])), in_values[i], sizeof(deskey[i])); } // PRQA S 3000, 4400
                DES_cblock iv[3] ; // PRQA S 4403
			    for (int i=0; i<3; ++i) {memcpy(static_cast<unsigned char*>(iv[i]), in_values[3+i], DES_KEY_SZ) ;} // PRQA S 3000, 3081, 4400

			    s_out = ACS_DES_TOFB_I_encdec( bufin, s_in, bufout, deskey[0], deskey[1], deskey[2], iv[0], iv[1], iv[2]) ; // PRQA S 4400
		        break ;
		    }
		
		
		case ENC_AES128CBC :
		case ENC_AES128OFB :
		case ENC_AES192CBC :
		case ENC_AES192OFB :
		case ENC_AES256CBC :
		case ENC_AES256OFB :
		{
			// in_values[0]: (unsigned char*) key_buffer (16/24/32 bytes)
			// in_values[1]: (unsigned char*) iv_buffer (16 bytes)

			ACS_COND_THROW( (in_values.size() < 2), ACS_SSL_InternalError("ENC_AES. Key or IV not provided") ) ; // PRQA S 3081, 4400
			ACS_SSL::ACS_AES_Cipher cipher ; // PRQA S 4101
			switch (al)
			{
				case ENC_AES128CBC: { cipher = ACS_SSL::AES_128_CBC ; break ; }
				case ENC_AES128OFB: { cipher = ACS_SSL::AES_128_OFB ; break ; }
				case ENC_AES192CBC: { cipher = ACS_SSL::AES_192_CBC ; break ; }
				case ENC_AES192OFB: { cipher = ACS_SSL::AES_192_OFB ; break ; }
				case ENC_AES256CBC: { cipher = ACS_SSL::AES_256_CBC ; break ; }
				case ENC_AES256OFB: { cipher = ACS_SSL::AES_256_OFB ; break ; }
				default: { ACS_THROW(ACS_SSL_InternalError("Cipher unsupported")); } // PRQA S 3081
			}
	
			const size_t key_length = ACS_SSL::ACS_AES_handle::getKeyLength(cipher) ;
			ACS_SSL::ACS_AES_Key aes_key(cipher, static_cast<unsigned char*>(in_values[0]), key_length) ; // PRQA S 3081
			const size_t iv_length = ACS_SSL::ACS_AES_handle::getIVLength(cipher) ;
			ACS_SSL::ACS_AES_IV iv_buffer ;
			memcpy(iv_buffer, in_values[1], iv_length) ; 

			const bool enc(true) ; 
			const bool padding(true) ; // This is pratically ignored in case of OFB, since blocklenght=1 byte
			// Create the instance of the ACS_AES_handle
			ACS_SSL::ACS_AES_handle aes_handle( cipher, aes_key, iv_buffer, enc, padding ) ;
		
			size_t bufout_len ( aes_handle.getExpected_buffersize(s_in) ) ;

			s_out = ACS_SSL::ACS_AES_encrypt(aes_handle, bufin, s_in, bufout, bufout_len) ; // bufout_len will be returned
			if (! aes_handle.isFinalized())
			{
				// Trigger finalize (process an empty input buffer)
				const size_t bufin_len_finalize(0) ; 
				s_out += ACS_SSL::ACS_AES_encrypt(aes_handle, bufin, bufin_len_finalize, (bufout+s_out), bufout_len) ; // PRQA S 3084
			}
			
		    break ;	
		}
		
		default :
    	{
        	ostringstream m ;
        	m << static_cast<unsigned int>(al) << " UNIMPLEMENTED" ;  // PRQA S 3081 2
        	ACS_THROW(ACS_SSL_InternalError(m.str())); 
    	}
	
	}	/* switch */


	if (0 != (flags & ENC_BASE64)) // PRQA S 3003
	{
		size_t s_tmp = s_out ;
		
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Performing the BASE64 post-encoding. Input buffer size: " << s_tmp) ; 
#endif 
		s_out = ACS_BASE64_encode(bufout, reinterpret_cast<char*>(bufout), s_tmp) ;	/* evaluate output size */ // PRQA S 3030, 3081
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Performing the BASE64 post-encoding. Output buffer size: " << s_out) ; 
#endif 
	}


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Algo: " << al
			<< " Output buffer size: " << s_out 
            << " Output vector size: " << out_values.size()) ;
#endif 

	return s_out ;

}							


////////////////////////////////////////////////////////////
//
//  ALGO Expected BufferSize
//
////////////////////////////////////////////////////////////
size_t 																	/* OUT: Output expected buffer size */
		ACS_SSL::ACS_ALGO_expected_buffersize(	EncryptionAlgo al_in,	/* IN: ALGO (and FLAGS) used */ 
												size_t s_in, 			/* IN: input buffer size (bytes) */
												int enc)				/* IN: 1 = Encode, 0 = Decode */
{
	/*
		First use:
		  Evaluate the expected buffer size, given a input buffersize, an Algo and an operation (enc. or dec.).
		Use this to pre-allocate the output buffer.
		
		Second use:
		  Evaluate the chunk size, e.g. the one required to encode/decode a large file in chunks. 
		  In this case, s_in is the allocated destination buffer and enc operation must be NEGATED.
		  E.g. to evaluate the encryption reading-chunk given an input buffer of 1 MB, the method must be called with 
		  s_in = 1048576, enc=0
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50)
        exFDebug dbg ;
		dbg << exDebugSignature << "Algo: " << al_in
			<< " input_size: " << s_in 
			<< " Operation: " << ((enc != 0 )? "ENCODE": "DECODE" ) ; // PRQA S 3380
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	size_t s_out = s_in ; /* default value */
	
	/* Get the algo */
	EncryptionAlgo al = EncryptionAlgo_Algo(al_in) ;
	
	/* Get the flags */
	EncryptionAlgo flags = EncryptionAlgo_Flags(al_in) ;
	
	
	/* Check decryption flags: [to be applied BEFORE the Algo itself] */
	if (0 == enc)
	{
		if (0 != (flags & ENC_BASE64)) { // PRQA S 3003
			s_out = ACS_SSL_FLOOR6(s_out) ; // PRQA S 3000, 3010
        }
		/* ... */
	}
	
	
	switch (al)
	{
		case ENC_NONE : { break ; }
		
		case ENC_BLOWFISH :	
		case ENC_DES :
		case ENC_DESOFB :
		case ENC_TDES :
		case ENC_TDESOFBI :
		{
			if ( 0 != enc ) { s_out = ACS_SSL_CEIL8(s_out) ; } // PRQA S 3000, 3010 2
			else { s_out = ACS_SSL_FLOOR8(s_out) ; }
		}
		break ; 
	
		case ENC_AES128CBC :
		{
			s_out = ACS_AES_handle::getExpected_buffersize( s_out, AES_128_CBC, (enc!=0) ) ; // Assuming paddingSet==true, which required larger buffers
			break ;
		}
		case ENC_AES128OFB :
		{
			s_out = ACS_AES_handle::getExpected_buffersize( s_out, AES_128_OFB, (enc!=0) ) ; // Assuming paddingSet==true, which required larger buffers
			break ;
		}
		case ENC_AES192CBC :
		{
			s_out = ACS_AES_handle::getExpected_buffersize( s_out, AES_192_CBC, (enc!=0) ) ; // Assuming paddingSet==true, which required larger buffers
			break ;
		}
		case ENC_AES192OFB :
		{
			s_out = ACS_AES_handle::getExpected_buffersize( s_out, AES_192_OFB, (enc!=0) ) ; // Assuming paddingSet==true, which required larger buffers
			break ;
		}
		case ENC_AES256CBC :
		{
			s_out = ACS_AES_handle::getExpected_buffersize( s_out, AES_256_CBC, (enc!=0) ) ; // Assuming paddingSet==true, which required larger buffers
			break ;
		}
		case ENC_AES256OFB :
		{
			s_out = ACS_AES_handle::getExpected_buffersize( s_out, AES_256_OFB, (enc!=0) ) ; // Assuming paddingSet==true, which required larger buffers
			break ;
		}

		default :
    	{
        	ostringstream m ;
        	m << static_cast<unsigned int>(al) << " UNIMPLEMENTED" ;  // PRQA S 3081 2
        	ACS_THROW(ACS_SSL_InternalError(m.str()));
    	}
	
	}	/* switch */
	
	
	/* Check encryption flags: [to be applied AFTER the Algo itself] */
	if (0 != enc)
	{
		if (0 != (flags & ENC_BASE64)) { // PRQA S 3003
			s_out = s_out * 4/3+4 ; // PRQA S 3084, 4400
		/* ... */
        }
	}
	
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_BGN_DEBUG(50) 
		exFDebug dbg ;
		dbg << exDebugSignature << "Returning: " << s_out ;
        excerr << dbg ;
	ACS_CLASS_END_DEBUG
#endif 

	return s_out ;
}


////////////////////////////////////////////////////////////
//
//  remove_salt_from_buffer
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::remove_salt_from_buffer (	const size_t& salt_size,
											const EncryptionAlgo& algo_in,			
											const unsigned char* input_buffer, 
											const size_t& input_buffer_size, 
											unsigned char* output_buffer,
											const size_t& output_buffer_size)									
{
/** 
	Given the size of the original salt (not the size of the coded salt, nor the coded salt itself, since this knowledge is not required here) 
	remove the strip from the input buffer, copying the output to the output buffer.
	The output buffer could also overlap with the input buffer 
*/
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called. Input size: " << input_buffer_size << " output_buffer_size: " <<   output_buffer_size) ;
#endif 
	
	
	// Note that the pointer of pointer CANNOT BE NULL.
	// Even to request memory allocation, the address container must not be null. E.G. uchar* ptr ; ptr = NULL. (&ptr) WILL BE FILLED WITH THE MALLOC, AND IT CANNOT BE NULL IN ANY CASE !
	ACS_COND_THROW( ((NULL == input_buffer) && (input_buffer_size > 0)), ACS_SSL_InternalError("Invalid input pointer") ) ; 

	size_t copied (0) ; 
	
	if ( (NULL != input_buffer) && (input_buffer_size > 0) ) 
	{
		ACS_COND_THROW( (NULL == output_buffer), ACS_SSL_InternalError("Invalid output pointer") ) ; 
	
		const EncryptionAlgo flags ( EncryptionAlgo_Flags(algo_in) ) ;
		const EncryptionAlgo al ( EncryptionAlgo_Algo(algo_in) ) ;
		if ( 0!= (flags & ENC_PREPEND_SALT) )
		{
			// The zero-padding of the salt is computed, so for example "test" will be coded as 8-bytes: "test\x00\x00\x00\x00"
			// this will allow a simpler removal of the prepended part.
			// Note that the size is computed over the algorithm (al) and not on the algo+flags (al_in)
			const size_t padded_salt_size ( (salt_size > 0)? ACS_ALGO_expected_buffersize( al, salt_size, 1 /* meaning is: size when encoded */ ) : 0) ; 
		
			ACS_COND_THROW( (padded_salt_size > 1024), ACS_SSL_InternalError("Salt size too big, probably misleading") ) ; 
		
			ACS_COND_THROW( (padded_salt_size > input_buffer_size), ACS_SSL_InternalError("Input buffer cannot fit the original salt !") ) ; 

			ACS_COND_THROW( ((input_buffer_size-padded_salt_size) > output_buffer_size), ACS_SSL_InternalError("Output buffer cannot fit the result !") ) ; 

			copied = (input_buffer_size-padded_salt_size) ;
			memmove( output_buffer, (input_buffer+padded_salt_size), copied ) ; 
			
			// Zero-pad the unused bytes in the output buffer
			if (output_buffer_size > copied) { memset ( (output_buffer+copied), 0x00, (output_buffer_size - copied) ) ; }
		}
		else
		{
			ACS_THROW( exIllegalValueException("Flags unhandled: " + toString(flags)) ) ; 
		}
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished. Returning: " << copied) ;
#endif 

	return copied ;
}


////////////////////////////////////////////////////////////
//
//  embed_salt_in_buffer
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::embed_salt_in_buffer (	const unsigned char* coded_salt_buffer,
										const EncryptionAlgo& algo_in,			
										const unsigned char* input_buffer, 
										const size_t& input_buffer_size, 
										unsigned char** output_buffer_ptr,
										const size_t& output_buffer_size)									
{
/** 
	Given a coded salt (generated by method encode_salt_buffer) and an algo, generate a valid salted buffer, ready to be passed to any ciphering algo.
	The salt in the buffer will be already padded (due to the knowledge of the algo).
	The output buffer can be pre-allocated or NOT pre-allocated. In the latter case, a malloc will be performed here and the caller will have to free this memory.
	This is the reason why the pointer of the buffer (and not the buffer itself) is passed here, to allow returning the address of the malloc-ed memory.
	If the output buffer is preallocated, it could also overlap with the input buffer (but output_buffer_size must contain the resized input buffer)
*/
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called. Input size: \"" << input_buffer_size << "\"  output_buffer_size: " <<   output_buffer_size) ;
#endif 
	
	// Note that the pointer of pointer CANNOT BE NULL.
	// Even to request memory allocation, the address container must not be null. E.G. uchar* ptr ; ptr = NULL. (&ptr) WILL BE FILLED WITH THE MALLOC, AND IT CANNOT BE NULL IN ANY CASE !
	ACS_COND_THROW( ((NULL == input_buffer) && (input_buffer_size > 0)), ACS_SSL_InternalError("Invalid input pointer") ) ; 

	size_t copied (0) ; 
	
	if ( (NULL != input_buffer) && (input_buffer_size > 0) ) 
	{
		ACS_COND_THROW( (NULL == output_buffer_ptr), ACS_SSL_InternalError("Invalid output pointer") ) ; 
	
		const EncryptionAlgo flags ( EncryptionAlgo_Flags(algo_in) ) ;
		if ( 0!= (flags & ENC_PREPEND_SALT) )
		{
			if ((NULL == *output_buffer_ptr) && (output_buffer_size>0))
			{
				*output_buffer_ptr = static_cast<unsigned char*>( malloc(output_buffer_size) ) ; 	// This should be freed by the caller !
				ACS_COND_THROW( (NULL == *output_buffer_ptr), ACS_SSL_InternalError("Could not allocate the memory !") ) ; 
			}


			// The zero-padding of the salt is computed, so for example "test" will be coded as 8-bytes: "test\x00\x00\x00\x00"
			// this will allow a simpler removal of the prepended part.
			// Note that the size is computed over the algorithm (al) and not on the algo+flags (al_in)
			const size_t padded_salt_size ( decode_salt(coded_salt_buffer, algo_in) ) ; // Also take into account the total (padded) size

			ACS_COND_THROW( (padded_salt_size > 1024), ACS_SSL_InternalError("Salt size too big, probably misleading") ) ; 

			// Since the salt will be prepended, first the input buffer is copied on the output buffer with offset: padded_salt_size (this will leave space to copy the salt at the beginning)
			ACS_COND_THROW( ((padded_salt_size+input_buffer_size) > output_buffer_size), ACS_SSL_InternalError("Output buffer cannot fit the result !") ) ; 
			copied = input_buffer_size ;
			memmove( ((*output_buffer_ptr)+padded_salt_size), input_buffer, copied ) ; 

			// Now expand the salt at the beggining of the output buffer
			if (padded_salt_size > 0)
			{
				decode_salt(coded_salt_buffer, (*output_buffer_ptr), padded_salt_size, algo_in) ; // Also take into account the total (padded) size
				copied += padded_salt_size ; 
			} 
		}
		else
		{
			ACS_THROW( exIllegalValueException("Flags unhandled: " + toString(flags)) ) ; 
		}
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished. Returning: " << copied) ;
#endif 

	return copied ;
}


////////////////////////////////////////////////////////////
//
//  encode_salt_buffer
//
////////////////////////////////////////////////////////////
size_t ACS_SSL::encode_salt_buffer (const unsigned char* input_buffer, 
									const size_t& input_buffer_size, 
									const size_t& expected_salt_size, 
									unsigned char* output_buffer, 
									const size_t& output_buffer_size) 
{
/** This is the only method which probably should be called from user-level. You must specify the expected_salt_size and an optional input_buffer (e.g. 0x06, 0x22, 0xfe ... ...)
	If (0 == expected_salt_size) the method returns 0, regardless any input buffer.
	If input_buffer is NULL or "" a random salt will be generated instead. If input_buffer is not empty, the output_buffer will be filled up to expected_salt_size.
	An exception will be returned if the output_buffer_size is unable to contain the output buffer.
	On success, the method returns the bytes copied onto the output_buffer.
*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called. Input size: \"" << input_buffer_size << "\"  expected_salt_size: " << expected_salt_size << "  output_buffer_size: " << output_buffer_size) ;
#endif 

	size_t copied (0) ; 

	if (expected_salt_size > 0)
	{
		// Compute the expected output buffer size
		const size_t expected_output_size(encode_salt(expected_salt_size)) ; 
	
		ACS_COND_THROW( (NULL == output_buffer), ACS_SSL_InternalError("Invalid output pointer") ) ; 
		
		// Check the size of the output buffer
		ACS_COND_THROW( (output_buffer_size < expected_output_size), ACS_SSL_InternalError("Output buffer unable to fit the result") ) ; 

		// IF HERE: The output buffer is able to contain the result
		
		if ( (NULL == input_buffer) || (0 == input_buffer_size) )
		{
			// Empty string provided -- a random sequence will be generated
			copied = encode_random_salt (expected_salt_size, output_buffer, output_buffer_size) ;
		}
		else
		{
			copied = encode_salt (input_buffer,	input_buffer_size, expected_salt_size, output_buffer, output_buffer_size ) ;
		}
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. Returning: " << copied ) ; 
#endif 

	return copied ; 
}


////////////////////////////////////////////////////////////
//
//  decode_salt
//
////////////////////////////////////////////////////////////
uint32_t 																	/* OUT: salt size */
			ACS_SSL::decode_salt (	const unsigned char* coded_salt_buffer,	/* IN: input buffer */ 
									const EncryptionAlgo& algo_in)			/* IN: encryption algo (used to compute the padding) */
{
	/**
		Given an input buffer of at least 4 bytes, decode the number of valid bytes it contains (actually, decode the header of the coded salt)
		The coded salt is prefixed with the number of valid bytes it contains.
		Example: input_buffer = { 00,00,00,06,..,..,.. }
		It returns the actual number of bytes following the header. In the example above: 6.
		This method should be used to pre-allocate a valid output buffer, in order to call decode_salt with also the output buffer.
		If the optional Algo is provided, the total size of the buffer (taking into account the padding) is returned
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called" ) ;
#endif 

	uint32_t salt_size(0) ; // The first 4 bytes of the input (since 4 == sizeof(salt_size)) are devoted to the size of the remaining part
	
	if (NULL != coded_salt_buffer) { memcpy(reinterpret_cast<unsigned char*>(&salt_size), coded_salt_buffer, sizeof(salt_size)) ; }

#if __BYTE_ORDER == __LITTLE_ENDIAN
	swap_32( &salt_size ) ;
#endif

	const EncryptionAlgo al(EncryptionAlgo_Algo(algo_in)) ; // Strip the flags
	
	if ( (0 != salt_size) && (ENC_NONE != al) )
	{ 
		// The salt is replaced with its padded version.
		// For example, since BLOWFISH has 8-bytes blocks, the padding of a salt of 3 bytes will be: 8 bytes total. A salt of 11 bytes will become 16 and so on.
		salt_size = ( ACS_ALGO_expected_buffersize( al, salt_size, 1 /* meaning is: size when encoded */ ) ) ; 
	}

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished. Returning: " << salt_size) ;
#endif 

	return salt_size ;	
} 


////////////////////////////////////////////////////////////
//
//  decode_salt
//
////////////////////////////////////////////////////////////
size_t 																		/* OUT: salt size (if the output buffer is not NULL and smaller of the salt, the size of the output is returned) */
		ACS_SSL::decode_salt (	const unsigned char* coded_salt_buffer,	/* IN: coded input buffer (e.g. 00 00 00 02 45 FF) */ 
								unsigned char* output_buffer,				/* IN/OUT: output buffer (or NULL to only decode the salt size) */  
								const size_t& output_buffer_size,			/* IN: size of the output buffer*/ 
								const EncryptionAlgo& algo)					/* IN: encryption algo */
								
{
	/**
		Given a pre-allocated output buffer of size output_buffer_size and an input buffer, decode the coded salt as described in the header
		The coded salt is prefixed with the number of valid bytes it contains.
		Example: input_buffer_size=10 { e.g. 00,00,00,06,ff,12,ea,01,20,a0 }, output_buffer_size=1024 -> output_buffer = { ff, 12, ea, 01, 20, a0 }  [ 6 bytes total ]
		NOTE THAT output_buffer_size should be able to fit the bytes declared in the header. Otherwise: exception.
		On success, returns the actual number of bytes copied in output_buffer, usually: (input_buffer_size-4). In the example above: 6.
		If the optional Algo is provided, the output buffer will be zero-filled up to the padding (so, taking into account the padding).
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called.  output_buffer_size: " << output_buffer_size << "  " << algo) ;
#endif 

	size_t to_copy (0) ; 
	uint32_t salt_size(0) ; // The first 4 bytes of the input (since 4 == sizeof(salt_size)) are devoted to the size of the remaining part

	// Check the input size is valid
	ACS_COND_THROW( (NULL == coded_salt_buffer ), exIllegalValueException("NULL Input buffer")) ;

	memcpy(reinterpret_cast<unsigned char*>(&salt_size), coded_salt_buffer, sizeof(salt_size)) ;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	swap_32( &salt_size ) ;
#endif

	const size_t padded_salt_size (decode_salt(coded_salt_buffer, algo)) ; 

	ACS_COND_THROW( ((NULL == output_buffer) || (output_buffer_size < padded_salt_size)), exIllegalValueException("Output buffer NULL or unable to fit the salt")) ;

	memset(output_buffer, 0x00, padded_salt_size) ; // Also take into account the padding

	to_copy = decode_salt(coded_salt_buffer, (salt_size+sizeof(salt_size)), output_buffer, output_buffer_size) ;

	// The number of bytes copied should also take into account the padding (even if bytes has not be "copied" but "zero-filled" ...)		
	to_copy = max(to_copy, padded_salt_size) ;

	return to_copy ;	/* The size of the buffer copied (which is expected to be the size of the salt, or less if the output buffer cannot fit it)  */ 
} 


////////////////////////////////////////////////////////////
//
//  decode_salt
//
////////////////////////////////////////////////////////////
size_t 																		/* OUT: salt size (if the output buffer is not NULL and smaller of the salt, the size of the output is returned) */
		ACS_SSL::decode_salt (	const unsigned char* coded_salt_buffer,	/* IN: input buffer */ 
								const size_t& input_buffer_size,			/* IN: total size of the input buffer */  
								unsigned char* output_buffer,				/* IN/OUT: output buffer (or NULL to only decode the salt size) */  
								const size_t& output_buffer_size)			/* IN: size of the output buffer*/ 
{
	/**
		Given a pre-allocated output buffer of size output_buffer_size and an input buffer, decode the coded salt of up to random_bytes
		The coded salt is prefixed with the number of valid bytes it contains.
		Example: input_buffer_size=10 { e.g. 00,00,00,06,ff,12,ea,01,20,a0 }, output_buffer_size=1024 -> output_buffer = { ff, 12, ea, 01, 20, a0 }  [ 6 bytes total ]
		NOTE THAT output_buffer_size should be at least (input_buffer_size-4)
		On success, returns the actual number of bytes copied in output_buffer, usually: (input_buffer_size-4). In the example above: 6.
	*/

#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called" ) ;
#endif 

	size_t to_copy (0) ; 
	uint32_t salt_size(0) ; // The first 4 bytes of the input (since 4 == sizeof(salt_size)) are devoted to the size of the remaining part

	if ( (NULL != coded_salt_buffer) && (input_buffer_size >= sizeof(salt_size)) ) /* No check on output buffer, since even with empty output buffer this method can decode the salt size */
	{
		memcpy(reinterpret_cast<unsigned char*>(&salt_size), coded_salt_buffer, sizeof(salt_size)) ;
#if __BYTE_ORDER == __LITTLE_ENDIAN
		swap_32( &salt_size ) ;
#endif

		if (NULL != output_buffer)
		{
			const size_t to_read ( min((input_buffer_size-sizeof(salt_size)), static_cast<size_t>(salt_size)) ) ; 
			to_copy = min( output_buffer_size, to_read ) ;
			if (to_copy > 0) { memcpy(output_buffer, (coded_salt_buffer+sizeof(salt_size)), to_copy) ; }
		} else { 
			to_copy = salt_size ;  // Since no actual buffer has been provided to copy, the nominal salt size is returned. 
		}
	}

	return to_copy ;	/* The size of the buffer copied (which is expected to be the size of the salt, or less if the output buffer cannot fit it)  */ 
} 


////////////////////////////////////////////////////////////
//
//  encode_salt
//
////////////////////////////////////////////////////////////
size_t 																/* OUT: encoded buffer size (including the size itself) */
		ACS_SSL::encode_salt (	const size_t& input_buffer_size)	/* IN: total size of the input buffer */  
{
	/**
		Given an input buffer, return the size of the output buffer that should be pre-allocated by the user, in order to safely call method "encode_salt" 
		passing an input buffer and a pre-allocated output buffer.
		On success, returns the actual number of bytes to copied in output_buffer, usually: (input_buffer_size+4).
	*/

	// NOTE:
	// This method allows writing a safer code, since the size of the output buffer is computed correctly and once (in this method).
	// In case of future changes, just this method will be updated.

	const size_t encoded_salt_size (input_buffer_size + 4) ;

	return encoded_salt_size ;
}


////////////////////////////////////////////////////////////
//
//  encode_salt
//
////////////////////////////////////////////////////////////
size_t 																/* OUT: encoded buffer size (including the size itself) */
		ACS_SSL::encode_salt (	const unsigned char* input_buffer,	/* IN: input buffer */ 
								const size_t& input_buffer_size,	/* IN: total size of the input buffer */  
								const size_t& expected_salt_size,	/* IN: expected salt size */  	
								unsigned char* output_buffer,		/* IN/OUT: output buffer */  
								const size_t& output_buffer_size)	/* IN: size of the output buffer */ 
{
	/**
		Given a pre-allocated output buffer of size output_buffer_size and an input buffer, generate a coded salt of up to input_buffer_size (plus 4-bytes header)
		The coded salt is prefixed with an header containing the number of valid bytes it contains.
		Example: input_buffer_size=8 { e.g. ff,12,ea,01,20,a0,bb,cc }, expected_salt_size=6, output_buffer_size=1024 -> output_buffer = { 00, 00, 00, 06, ff, 12, ea, 01, 20, a0 }  [ 10 bytes total ]
		NOTE THAT output_buffer_size should be at least (random_bytes+4)
		On success, returns the actual number of bytes copied in output_buffer, usually: (input_buffer_size+4). In the example above: (4+6)=10.
	*/


#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called" ) ;
#endif 
	
	uint32_t salt_size(0) ; // The first 4 bytes of the output (since 4 == sizeof(salt_size)) will be devoted to the size of the remaining part

	if ( (NULL != input_buffer) && (output_buffer_size>=sizeof(salt_size)) )
	{
		ACS_COND_THROW( (NULL == output_buffer), ACS_SSL_InternalError("Invalid output pointer") ) ; // The input it not even able to contain the size !
	
		salt_size = min (expected_salt_size, (output_buffer_size-sizeof(salt_size))) ; 
		const size_t to_copy ( min(input_buffer_size, static_cast<size_t>(salt_size)) ) ;
		
		memcpy( output_buffer, reinterpret_cast<unsigned char*>(&salt_size), sizeof(salt_size) ) ;
#if __BYTE_ORDER == __LITTLE_ENDIAN
		swap_32( output_buffer ) ;
#endif
		memcpy( (output_buffer+sizeof(salt_size)), input_buffer, to_copy ) ; 

		if (to_copy < salt_size) { memset( (output_buffer+sizeof(salt_size)+to_copy), 0x00, (salt_size-to_copy)) ; } // zero-pad

		salt_size += sizeof(salt_size) ; // Total size
	}

	return salt_size ;	/* The size of the buffer copied (which is expected to be the size of the salt, or less if the output buffer cannot fit it)  */ 
} 


////////////////////////////////////////////////////////////
//
//  encode_random_salt
//
////////////////////////////////////////////////////////////
size_t 																		/* OUT: encoded buffer size (including the size itself) */
		ACS_SSL::encode_random_salt (	const size_t& random_bytes,			/* IN: total size of the input buffer */  
										unsigned char* output_buffer,		/* IN/OUT: output buffer */  
										const size_t& output_buffer_size)	/* IN: size of the output buffer */
{
	/**
		Given a pre-allocated output buffer of size output_buffer_size and the integer random_bytes, generate a random coded salt of up to random_bytes (plus 4-bytes header)
		The coded salt is prefixed with an header containing the number of random bytes it contains.
		Example: random_bytes=6, output_buffer_size=1024 -> output_buffer = { 00, 00, 00, 06, ff, 12, ea, 01, 20, a0 }  [ 10 bytes total ]
		NOTE THAT output_buffer_size should be at least (random_bytes+4)
		On success, returns the actual number of bytes copied in output_buffer, usually: (random_bytes+4). In the example above: (4+6)=10.
	*/
	
#ifdef ACS_SSL_HAS_DEBUG_ENABLED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called" ) ;
#endif 
	
	uint32_t salt_size(0) ; // The first 4 bytes of the output (since 4 == sizeof(salt_size)) will be devoted to the size of the remaining part

	if (output_buffer_size>=sizeof(salt_size))
	{
		ACS_COND_THROW( (NULL == output_buffer), ACS_SSL_InternalError("Invalid output pointer") ) ; // The input it not even able to contain the size !

		salt_size = min (random_bytes, (output_buffer_size-sizeof(salt_size))) ; 
		
		memcpy( output_buffer, reinterpret_cast<unsigned char*>(&salt_size), sizeof(salt_size) ) ;
#if __BYTE_ORDER == __LITTLE_ENDIAN
		swap_32( output_buffer ) ;
#endif

		if (salt_size > 0)
		{
			const pair <unsigned char, unsigned char> salt_range (33, 126) ; // Typical definition of "printable chars", widely used for random salts 

			ACS_RAND_bytes((output_buffer+sizeof(salt_size)), salt_size, salt_range) ; // Create a random salt
		}

		salt_size += sizeof(salt_size) ; // Total size
	}

	return salt_size ;	/* The size of the buffer copied (which is expected to be the size of the salt, or less if the output buffer cannot fit it)  */ 
} 


////////////////////////////////////////////////////////////
//
//   BASE64 ENCODE_BUFFERSIZE
//
////////////////////////////////////////////////////////////
size_t
			ACS_SSL::ACS_BASE64_encode_buffersize(size_t s) 
{
	return  ACS_ALGO_expected_buffersize (ENC_BASE64, s, 1) ;
}


// PRQA S 4066 L1
string ACS_SSL::toString(EncryptionAlgo al_in) // PRQA S 4020
{
 	string s ; 

	/* Get the algo */
	const EncryptionAlgo al ( EncryptionAlgo_Algo(al_in) ) ;
	
	s = "EncryptionAlgo: " ; 
	switch(al)
	{
		case ENC_NONE: { s +=  "ENC_NONE" ; break ; }	

		/* FLAGS: */
		case ENC_BASE64: { s +=  "ENC_BASE64" ; break ; }	
		case ENC_PREPEND_SALT: { s +=  "ENC_PREPEND_SALT" ; }

		/* UNUSED FLAGS, ALREADY ALLOCATED: */
		case UNUSED0004: { s +=  "UNUSED0004" ; break ; }	
		case UNUSED0008: { s +=  "UNUSED0008" ; break ; }	
		case UNUSED0010: { s +=  "UNUSED0010" ; break ; }	
		case UNUSED0020: { s +=  "UNUSED0020" ; break ; }	
		case UNUSED0040: { s +=  "UNUSED0040" ; break ; }	
		case UNUSED0080: { s +=  "UNUSED0080" ; break ; }	

	    /* ALGOS: */
    	case ENC_BLOWFISH:	{ s +=  "ENC_BLOWFISH" ; break ; }
    	case ENC_DES:		{ s +=  "ENC_DES" ; break ; }
    	case ENC_DESOFB:	{ s +=  "ENC_DESOFB" ; break ; }
    	case ENC_TDES:		{ s +=  "ENC_TDES" ; break ; }
    	case ENC_TDESOFBI:	{ s +=  "ENC_TDESOFBI" ; break ; }
		case ENC_AES128CBC: { s +=  "ENC_AES128CBC" ; break ; }
		case ENC_AES128OFB: { s +=  "ENC_AES128OFB" ; break ; }
		case ENC_AES192CBC: { s +=  "ENC_AES192CBC" ; break ; }
		case ENC_AES192OFB: { s +=  "ENC_AES192OFB" ; break ; }
		case ENC_AES256CBC: { s +=  "ENC_AES256CBC" ; break ; }
		case ENC_AES256OFB: { s +=  "ENC_AES256OFB" ; break ; }

		default: { s +=  "*UNKNOWN*" ; break ; }
    }

	
	/* Get the flags -- note that the flags are NOT mutually exclusive (so all the flags must be checked separately) */
	EncryptionAlgo flags ( EncryptionAlgo_Flags(al_in) ) ;

	s += "  EncryptionAlgo_Flags: " ; 
	size_t flags_count (0) ; 
	if (flags & ENC_BASE64)			{ s += "ENC_BASE64" ; s += "; " ; flags_count ++ ; flags = static_cast<EncryptionAlgo>(flags ^ ENC_BASE64) ; }
	if (flags & ENC_PREPEND_SALT)	{ s += "ENC_PREPEND_SALT" ; s += "; " ; flags_count ++ ; flags = static_cast<EncryptionAlgo>(flags ^ ENC_PREPEND_SALT) ; }
	
	// So far, all the known flags should be disabled (effect of the XOR above)
	if (flags != ENC_NONE)
	{
		s += "*UNKNOWN*" ; s += "; " ; flags_count ++ ; 
	}
	else if (0 == flags_count)
	{
		// No flags found 
		s += "ENC_NONE" ; s += "; " ; flags_count ++ ;  
	}

	return s ; 
}
// PRQA L:L1

string ACS_SSL::toString(CipherStyle c) // PRQA S 4020
{
    switch (c)
    {
    case NOCIPHER: { return "NOCIPHER" ; }
    case CIPHER_RSA: { return "NOCIPHER" ; }
    case CIPHER_HW: { return "NOCIPHER" ; }
    case CIPHER_RSA_HW: { return "NOCIPHER" ; }
    default: 
        {
            ostringstream os ;
            os << "Unknown CipherStyle<" << int(c) << ">" ; // PRQA S 3081
            return os.str() ;
        }
    }
}


string ACS_SSL::toString(ACS_AES_Cipher c) // PRQA S 4020
{
    switch (c)
    {
    case AES_128_CBC: { return "AES_128_CBC" ; }
    case AES_128_OFB: { return "AES_128_OFB" ; }
    case AES_192_CBC: { return "AES_192_CBC" ; }
    case AES_192_OFB: { return "AES_192_OFB" ; }
    case AES_256_CBC: { return "AES_256_CBC" ; }
    case AES_256_OFB: { return "AES_256_OFB" ; }
    default: 
        {
            ostringstream os ;
            os << "Unknown ACS_AES_Cipher<" << int(c) << ">" ; // PRQA S 3081
            return os.str() ;
        }
    }
}


std::ostream & operator << (std::ostream &s, const ACS_SSL::EncryptionAlgo &e) 
{
	s << ACS_SSL::toString(e) ;
	return s ; // PRQA S 4028
}

acs::exostream & operator << (acs::exostream &s, const ACS_SSL::EncryptionAlgo &e) // PRQA S 4222
{
	s << ACS_SSL::toString(e) ;
	return s ; // PRQA S 4028
}


std::ostream & operator << (std::ostream &s, const ACS_SSL::CipherStyle &c) 
{
	s << ACS_SSL::toString(c) ;
	return s ; // PRQA S 4028
}

acs::exostream & operator << (acs::exostream &s, const ACS_SSL::CipherStyle &c) // PRQA S 4222
{
	s << ACS_SSL::toString(c) ;
	return s ; // PRQA S 4028
}


_ACS_END_NAMESPACE
