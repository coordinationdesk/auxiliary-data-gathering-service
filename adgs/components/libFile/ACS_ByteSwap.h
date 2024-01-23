// PRQA S 1050 EOF
/**************************************************************************************
 
    Copyright 1995-2011, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ByteSwap.h HEADER $

    $Author$

    $Id$

    $Log$
    Revision 2.8  2012/11/30 14:09:44  marpas
    qa rules and optimization

    Revision 2.7  2012/03/13 12:05:33  marpas
    adding need include

    Revision 2.6  2012/02/07 17:57:43  marpas
    refactoring in progress

    Revision 2.5  2011/03/29 16:26:47  enrcar
    EC:: macro was malfunctioning if called with a variable having the same name as one of the local temporary variables (e.g. &tmp, being tmp a macro local variable). Solved using atypical temporary variables, like __tmp

    Revision 2.4  2006/09/26 12:44:00  enrcar
    updated with new macros

    Revision 1.4  2006/08/02 13:09:37  enrcar
    Usage added ; Rotation of any number of bytes handled ; Vector rotation methods renamed

    Revision 1.3  2006/08/01 16:41:28  enrcar
    Updated to libFile version

    Revision 2.2  2006/08/01 16:39:38  enrcar
    EC::ACS_ByteSwapVector_SWAP method added

    Revision 2.1  2006/08/01 15:57:55  enrcar
    EC:: Endianness detection improved


**************************************************************************************/



#ifndef _ACS_ByteSwap_H_
#define _ACS_ByteSwap_H_


#ifdef sgi
    #include <standards.h>    
    #include <sys/endian.h> 
#else
    #ifdef linux
        #include <endian.h>    
    #endif
#endif
#include <unistd.h>    


/*
    Usage:
    ==========
    
    ACS_ByteSwap_SWAP( void* p, size_t size, int bigEndian ) 
    ACS_ByteSwap_VSWAP( int n, void* p, size_t size, int bigEndian ) 

    Rotate "size" bytes starting from posion "p".
    If binary data were generated on a BIG_ENDIAN platform, set bigEndian = 1
    Otherwise:
    set bigEndian = 0
    To rotate a vector, use the second call, passing as argument the number "n" of rows.
    
    Example:
    
    double d = (...) ;    Binary generated on a BIG_ENDIAN Platform 
    unsigned int v[] = { 0x... , 0x... } ;    Binary generated on a LITTLE_ENDIAN Platform 
    
    ACS_ByteSwap_SWAP ( &d, sizeof(double), 1 ) ;
    ACS_ByteSwap_VSWAP ( 2, &v, sizeof(unsigned int), 0 );
    
*/



#ifdef __cplusplus
	extern "C" {
#endif


    /*
        Check endianness macros.
        EC::SHOULD WORKS ON EVERY UN*X  ---  **** PLEASE REPORT PROBLEMS IMMEDIATLY ****
    */
    #ifndef BIG_ENDIAN
        #ifndef _BIG_ENDIAN
            #ifndef __BIG_ENDIAN
                /* Typical: */
                #define LITTLE_ENDIAN  1234   
                #define BIG_ENDIAN     4321   
                #define PDP_ENDIAN     3412    
            #else
                #define LITTLE_ENDIAN __LITTLE_ENDIAN
                #define BIG_ENDIAN __BIG_ENDIAN
                #define PDP_ENDIAN__PDP_ENDIAN
            #endif
        #else
            #define LITTLE_ENDIAN _LITTLE_ENDIAN
            #define BIG_ENDIAN _BIG_ENDIAN
            #define PDP_ENDIAN _PDP_ENDIAN
        #endif        
    #endif


    #ifndef BYTE_ORDER
        #ifdef _BYTE_ORDER
            #define BYTE_ORDER _BYTE_ORDER
        #else
            #ifdef __BYTE_ORDER
                #define BYTE_ORDER __BYTE_ORDER
            #else
                #ifdef sgi
                    #define BYTE_ORDER BIG_ENDIAN   
                #else
                    #define BYTE_ORDER LITTLE_ENDIAN   
                #endif      
            #endif
        #endif
    #endif


    #if ( BYTE_ORDER != BIG_ENDIAN )
        #if ( BYTE_ORDER != LITTLE_ENDIAN )
            #if ( BYTE_ORDER == PDP_ENDIAN )
                #error Unsupported Endianess PDP_ENDIAN
                /* (!!!)  EC::Please call  (!!!) */
            #else
                #error Unsupported Endianess Unknown
                /* (!!!)  EC::Please call  (!!!) */
            #endif
        #endif
    #endif    

// PRQA S 1020, 1023 L1
    /* Endianness macros (internals) */
#ifdef __cplusplus
    #define ACS_ByteSwap_SWAP2Bytes(a1, a2) { \
        unsigned char __t = *reinterpret_cast<unsigned char*>(a1) ; \
        *reinterpret_cast<unsigned char*>(a1)=*reinterpret_cast<unsigned char*>(a2); \
        *reinterpret_cast<unsigned char*>(a2)=__t; }
#else
    #define ACS_ByteSwap_SWAP2Bytes(a1, a2) { unsigned char __t; \
        __t=(*((unsigned char*)a1)); (*((unsigned char*)a1))=(*((unsigned char*)a2)); \
        (*((unsigned char*)a2))=__t; }
#endif


#ifdef __cplusplus
    #define ACS_ByteSwap_SWAP_ANY(val, siz_a) { \
        unsigned char* __p = reinterpret_cast<unsigned char*>(val) ; \
        for (size_t __i=0; __i<size_t((siz_a)/2) ; __i++) \
        { ACS_ByteSwap_SWAP2Bytes(__p + __i, __p+((siz_a)- __i-1)) ; } }
#else
    #define ACS_ByteSwap_SWAP_ANY(val, siz_a) { unsigned char* __p = (unsigned char*) val; \
        unsigned int __i; for (__i=0; __i<((unsigned int)((siz_a)/2)); __i++) \
        { ACS_ByteSwap_SWAP2Bytes(__p + __i, __p+((siz_a)- __i-1)) ; } }
#endif
    
#ifdef __cplusplus
    #define ACS_ByteSwap_SWAP64(val) { \
        unsigned char* __p = reinterpret_cast<unsigned char*>(val); \
        ACS_ByteSwap_SWAP2Bytes(__p+0, __p+7); ACS_ByteSwap_SWAP2Bytes(__p+1, __p+6); \
        ACS_ByteSwap_SWAP2Bytes(__p+2, __p+5); ACS_ByteSwap_SWAP2Bytes(__p+3, __p+4); }
#else
    #define ACS_ByteSwap_SWAP64(val) { unsigned char* __p = (unsigned char*) val; \
        ACS_ByteSwap_SWAP2Bytes(__p+0, __p+7); ACS_ByteSwap_SWAP2Bytes(__p+1, __p+6); \
        ACS_ByteSwap_SWAP2Bytes(__p+2, __p+5); ACS_ByteSwap_SWAP2Bytes(__p+3, __p+4); }
#endif

#ifdef __cplusplus
    #define ACS_ByteSwap_SWAP32(val) { \
        unsigned char* __p = reinterpret_cast<unsigned char*>(val) ; \
        ACS_ByteSwap_SWAP2Bytes(__p+0, __p+3); ACS_ByteSwap_SWAP2Bytes(__p+1, __p+2);  }
#else
    #define ACS_ByteSwap_SWAP32(val) { unsigned char* __p = (unsigned char*) val; \
        ACS_ByteSwap_SWAP2Bytes(__p+0, __p+3); ACS_ByteSwap_SWAP2Bytes(__p+1, __p+2);  }
#endif

#ifdef __cplusplus
    #define ACS_ByteSwap_SWAP16(val) {\
        unsigned char* __p = reinterpret_cast<unsigned char*>(val) ; \
        ACS_ByteSwap_SWAP2Bytes(__p+0, __p+1); }    
#else
    #define ACS_ByteSwap_SWAP16(val) { unsigned char* __p = (unsigned char*) val; \
        ACS_ByteSwap_SWAP2Bytes(__p+0, __p+1); }    
#endif


    /* Define Swapping operation (internal) */
    #define ACS_ByteSwap_ApplySwap(a, siz_a) { if ((siz_a)==8) ACS_ByteSwap_SWAP64(a) \
            else if ((siz_a)==4) ACS_ByteSwap_SWAP32(a) \
            else if ((siz_a)==2) ACS_ByteSwap_SWAP16(a) \
            else ACS_ByteSwap_SWAP_ANY(a, (siz_a)); }

#ifdef __cplusplus
    #define ACS_ByteSwap_ApplySwapVector(n, a, siz_a) { \
        int __i ; for (__i=0; __i<(n); __i++) \
           ACS_ByteSwap_ApplySwap(((__i*(siz_a))+reinterpret_cast<unsigned char*>(a)), (siz_a)) ; }       
#else
    #define ACS_ByteSwap_ApplySwapVector(n, a, siz_a) { \
        int __i ; for (__i=0; __i<(n); __i++) \
           ACS_ByteSwap_ApplySwap(((__i*(siz_a))+(unsigned char*)a), (siz_a)) ; }       
#endif


    /*
        ACS_ByteSwap_SWAP:  E N D I A N N E S S   B Y T E S W A P P I N G    M A C R O
        ACS_ByteSwapVector_SWAP:  V E C T O R S   B Y T E S W A P P I N G    M A C R O
    */
    #if ( BYTE_ORDER == BIG_ENDIAN )

        #ifdef __cplusplus
    
            /* Big endian: swap bytes if LITTLEENDIAN object */
            #define ACS_ByteSwap_SWAP(a, siz_a, isBend) { \
                unsigned char* __tmp = reinterpret_cast<unsigned char*>(a) ; \
                if (isBend) {} else ACS_ByteSwap_ApplySwap((__tmp), (siz_a)) ; }

            #define ACS_ByteSwap_VSWAP(n, a, siz_a, isBend) { \
                unsigned char* __tmp = reinterpret_cast<unsigned char*>(a) ; \
                if (isBend) {} else ACS_ByteSwap_ApplySwapVector(n, (__tmp), (siz_a)) ; }
        #else 
            /* Big endian: swap bytes if LITTLEENDIAN object */
            #define ACS_ByteSwap_SWAP(a, siz_a, isBend) { \
                unsigned char* __tmp ; __tmp = (unsigned char*) a ; \
                if (isBend) {} else ACS_ByteSwap_ApplySwap((__tmp), (siz_a)) ; }

            #define ACS_ByteSwap_VSWAP(n, a, siz_a, isBend) { \
                unsigned char* __tmp ; __tmp = (unsigned char*) a ; \
                if (isBend) {} else ACS_ByteSwap_ApplySwapVector(n, (__tmp), (siz_a)) ; }
        #endif
        
    #else 

        #ifdef __cplusplus
            /* Little endian: swap bytes if BIGENDIAN object */
            #define ACS_ByteSwap_SWAP(a, siz_a, isBend) { \
                unsigned char* __tmp = reinterpret_cast<unsigned char*>(a) ; \
                if (!(isBend)) {} else ACS_ByteSwap_ApplySwap((__tmp), (siz_a)) ; }

            #define ACS_ByteSwap_VSWAP(n, a, siz_a, isBend) { \
                unsigned char* __tmp = reinterpret_cast<unsigned char*>(a) ; \
                if (!(isBend)) {} else ACS_ByteSwap_ApplySwapVector(n, (__tmp), (siz_a)) ; }
        #else 
            /* Little endian: swap bytes if BIGENDIAN object */
            #define ACS_ByteSwap_SWAP(a, siz_a, isBend) { \
                unsigned char* __tmp ; __tmp = (unsigned char*) a ; \
                if (!(isBend)) {} else ACS_ByteSwap_ApplySwap((__tmp), (siz_a)) ; }

            #define ACS_ByteSwap_VSWAP(n, a, siz_a, isBend) { \
                unsigned char* __tmp ; __tmp = (unsigned char*) a ; \
                if (!(isBend)) {} else ACS_ByteSwap_ApplySwapVector(n, (__tmp), (siz_a)) ; }
        #endif

    #endif
// PRQA L:L1

#ifdef __cplusplus
	}
#endif


#endif /* _ACS_ByteSwap_H_ */
