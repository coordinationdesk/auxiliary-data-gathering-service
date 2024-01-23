//////////////////////////////////////////////////////////////////////////////////////
//
/*
    Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
*/
//    $Prod: A.C.S. Operation03 class $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 5.2  2013/07/04 18:49:28  marpas
//    finishing adoption of libException 5.x standards
//    qa warnings removed
//    compilation warnings removed
//    coding best practices applied
//
//    Revision 5.1  2013/07/02 17:31:26  marpas
//    adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
//
//    Revision 5.0  2013/06/20 16:43:54  marpas
//    adoption of libException 5.x in progress
//
//    Revision 1.5  2012/02/13 10:59:05  marpas
//    refactoring in progress
//
//    Revision 1.4  2012/02/07 17:57:44  marpas
//    refactoring in progress
//
//    Revision 1.3  2011/03/01 11:40:16  marpas
//    GCC 4.4.x support added
//
//    Revision 1.2  2009/05/14 13:19:29  enrcar
//    EC:: c++ rules
//
//    Revision 1.1  2008/07/01 13:14:05  enrcar
//    EC::Imported source
//
//    
//////////////////////////////////////////////////////////////////////////////////////


#include <Operation03.h> 
#include <Filterables.h> 

#ifdef ACS_TDIM_DEFINED
	#include <ACS_TDIM.h> 
#endif
#include <cstring>

_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(Operation03);

//////////////////////////////////////////////////////
//
//   ENCODE BUFFER 
//
//////////////////////////////////////////////////////
unsigned char*															/* OUT_BUFFER (malloc) */
				Operation03::EncodePkg(	ACS_SSL::CipherStyle cipher,	/* IN: Cipher Style */
                						unsigned int* returnLength,		/* Size of OUT_Buffer */
										const RSA* key,					/* Key */ 
										const unsigned char* buffer,      	/* IN_Buffer */ 
										unsigned int len,				/* Size of IN_Buffer */
										bool emitExcept,				/* IN: true for emit exceptions */ 
										bool* success)					/* OUT: true if successfully */
{
    #ifdef Operation03_HAS_DEBUG_ENABLED      
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Method called with CipherStyle: " << cipher) ;
    #endif 

	unsigned char* retPointer = nullptr ;	/* Prepare return value */
    
    *returnLength = 0 ;
    if (success) { *success = false ; }

    switch (cipher)
    {
    case ACS_SSL::NOCIPHER :
		/* Just copy the buffer */
        retPointer = reinterpret_cast<unsigned char*>(malloc (len)); // NOSONAR - memory to be allocated here

        if (emitExcept) { ACS_COND_THROW(!retPointer,Operation03_InternalError("Operation03::EncodePkg. Out of memory")) ; } // PRQA S 3081

        if (retPointer)
        {
            memcpy(retPointer, buffer, len) ;	/* Copy the buffer */
            *returnLength = len ;
            if (success) { *success = true ; }
		}
        break ;
        
    case ACS_SSL::CIPHER_RSA:
        retPointer = ACS_SSL::ACS_RSA_priv_enc(returnLength, key,  buffer, len, emitExcept, success) ;
        break ;
    case ACS_SSL::CIPHER_HW:
#ifdef ACS_TDIM_DEFINED
        ACS_TDIM tdim ;				/* Handler is created */
        tdim.ACS_TDIM_Init(key) ;	/* Encryption engine is initialised using the public key */

        retPointer = tdim.ACS_TDIM_EncodePkg(cipher, returnLength, key,  buffer, len, emitExcept, success) ;
        break ;
#else
		ACS_THROW(Operation03_InternalError("Operation03. SEVERE RUNTIME ERROR. HW CRYPTING UNDEFINED")); // PRQA S 3081
#endif
//    case ACS_SSL::CIPHER_RSA_HW: // NOSONAR - unused case falls into default
    default:            
		/* Unsupported */
#ifdef Operation03_HAS_DEBUG_ENABLED      
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Unsupported CipherStyle: " << cipher) ;
#endif 

        if (emitExcept) {   
            ACS_THROW(Operation03_InternalError() << "Operation03::EncodePkg. Unsupported CipherStyle: " << cipher); // PRQA S 3081
        }

        
    
    }	/* switch */


#ifdef Operation03_HAS_DEBUG_ENABLED      
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        std::ostringstream s ;
		if (!success) {
            s << "UNDEFINED SINCE NULL PTR" ;
        }
		else {
            s << ( (*success)? "T": "F") ; // PRQA S 3380
        }

	    ACS_LOG_DEBUG(SimpleDebugSignature << "success: " << s.str() // PRQA S 3380
            << " retPointer: " << ( (retPointer)? "NOT NULL": "NULL" ) << " ptr.") ;
    ACS_CLASS_END_DEBUG
#endif 

	return retPointer ;

}



///////////////////////////////////////
//
//   DECODE BUFFER 
//
///////////////////////////////////////
unsigned char*  														/* OUT_BUFFER (malloc) */
				Operation03::DecodePkg(	ACS_SSL::CipherStyle cipher,	/* IN: Cipher Style */
                						unsigned int* returnLength,		/* Size of OUT_Buffer */
										const RSA* key, 				/* KEY */
										const unsigned char* buffer,			/* IN_Buffer */ 
										unsigned int len,				/* Size of IN_Buffer */
										bool emitExcept,				/* IN: true for emit exceptions */ 
										bool* success)					/* OUT: true if successfully */
{
	#ifdef Operation03_HAS_DEBUG_ENABLED      
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called with CipherStyle: " << cipher);
    #endif 

	unsigned char* retPointer = nullptr ;	/* Prepare return pointer */
    
    *returnLength = 0 ;
    if (success) { *success = false ; }

    switch (cipher)
    {
    case ACS_SSL::NOCIPHER:
		/* Just copy the buffer */
        retPointer = reinterpret_cast<unsigned char*>(malloc (len)); // NOSONAR - memory to be allocated

        if (emitExcept) { ACS_COND_THROW(!retPointer,Operation03_InternalError("Operation03::DecodePkg. Out of memory")) ; } // PRQA S 3081

        if (retPointer)
        {
            memcpy(retPointer, buffer, len) ;	/* Copy the buffer */
            *returnLength = len ;
            if (success) { *success = true ; }
		}
        break ;
        
    case ACS_SSL::CIPHER_RSA:
        retPointer = ACS_SSL::ACS_RSA_pub_dec(returnLength, key, buffer, len, emitExcept, success) ;
        break ;
        
    case ACS_SSL::CIPHER_HW:
#ifdef ACS_TDIM_DEFINED
        ACS_TDIM tdim ;				/* Handler is created */
        tdim.ACS_TDIM_Init(key) ;	/* Decryption engine is initialised using the public key */

        retPointer = tdim.ACS_TDIM_DecodePkg(cipher, returnLength, key, buffer, len, emitExcept, success) ;
        break ;
#else      
		ACS_THROW(Operation03_InternalError("Operation03. SEVERE RUNTIME ERROR. HW CRYPTING UNDEFINED")); // PRQA S 3081
#endif
//    case ACS_SSL::CIPHER_RSA_HW: unused case: falls into default
    default:            
		/* Unsupported CipherStyle */
#ifdef Operation03_HAS_DEBUG_ENABLED      
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Unsupported CipherStyle: " << int(cipher)) ; // PRQA S 3081
#endif 
        if (emitExcept) {
            ACS_THROW(Operation03_InternalError()<< "Unsupported CipherStyle: " << int(cipher));  // PRQA S 3081
        }
        
    
    }	/* switch */


#ifdef Operation03_HAS_DEBUG_ENABLED      
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        std::ostringstream s ;
	    if (!success) {
            s << "UNDEFINED SINCE NULL PTR" ;
        }
	    else {
            s << ( (*success)? "T": "F") ; // PRQA S 3380
        }

	    ACS_LOG_DEBUG(SimpleDebugSignature << "success: " << s.str()  // PRQA S 3380
            << " retPointer: " << ( (retPointer)? "NOT NULL": "NULL" ) << " ptr.") ;
    ACS_CLASS_END_DEBUG
#endif 

	return retPointer ;

}


///////////////////////////////////////
//
//   GetCipheredPkgSize 
//
///////////////////////////////////////
unsigned int  										
				Operation03::GetCipheredPkgSize(	ACS_SSL::CipherStyle cipher,	/* IN: Cipher Style */
                                                    const RSA* key, 				/* KEY */
                                                    unsigned int len ) 				/* Pre-ciphering package lenght */
{
	/* 
        Given the original package size ("len"), before encryption, the ciphered-data size will be evaluated.
        This could be useful:
        1) To predict the final size of the outbuffer (to reserve memory, etc.)
        2) To evaluate the size of an input buffer (ciphered), if is known the original, unciphered, datasize
    */


#ifdef Operation03_HAS_DEBUG_ENABLED      
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called with CipherStyle: " << cipher) ;
#endif 

	unsigned int retValue = 0 ;
    

    switch (cipher)
    {
    case ACS_SSL::NOCIPHER:
        retValue = len ;
        break ;
    case ACS_SSL::CIPHER_RSA:
        retValue = ACS_SSL::ACS_RSA_size_NumB( key, len ) ; // PRQA S 3000
        break ;
    case ACS_SSL::CIPHER_HW:
#ifdef ACS_TDIM_DEFINED
        retValue = ACS_TDIM::ACS_TDIM_GetCipheredPkgSize(cipher, key, len) ;
        break ;
#else      
        ACS_THROW(Operation03_InternalError("Operation03. SEVERE RUNTIME ERROR. HW CRYPTING UNDEFINED")); // PRQA S 3081
#endif
//    case ACS_SSL::CIPHER_RSA_HW: any case falls into default
    default:            

#ifdef Operation03_HAS_DEBUG_ENABLED      
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Unsupported CipherStyle: " << int(cipher)) ; // PRQA S 3081
#endif 

        ACS_THROW(Operation03_InternalError() << "Operation03::GetCipheredPkgSize. Unsupported CipherStyle: " << int(cipher)); // PRQA S 3081

        
    
    }	/* switch */


	return retValue ;

}

_ACS_END_NAMESPACE
