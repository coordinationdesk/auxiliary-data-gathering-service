///////////////////////////////////////////////////////////////////////////////////////
// 
/* 	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
*/ 
// 	$Prod: A.C.S. Operation02 class HEADER $
// 
// 	$Author$
// 
// 	$Id$
//
// 	$Log$
// 	Revision 5.0  2013/06/20 16:43:53  marpas
// 	adoption of libException 5.x in progress
// 	
// 	Revision 2.7  2012/11/29 19:31:30  marpas
// 	implementing qa rules
// 	optimizing code and coding best practices
// 	coverage framework initiated
// 	
// 	Revision 2.6  2012/02/07 17:57:44  marpas
// 	refactoring in progress
// 	
// 	Revision 2.5  2009/05/14 13:03:46  enrcar
// 	EC:: c++ rules
// 	
// 	Revision 2.4  2008/07/01 10:14:37  enrcar
// 	EC:: CipherStyle moved into libACS_SSL
// 	
// 	Revision 2.3  2008/06/30 15:31:02  enrcar
// 	EC:: HW-cypering is now supported (TDI Matrix). A further layer (Operation03) has been added.
// 	
// 	Revision 2.2  2008/06/04 15:15:56  enrcar
// 	EC:: Operation02 supports HW Parameters check
// 	
// 	Revision 2.1  2006/09/19 15:11:18  danalt
// 	fixed for gcc 4.1.0
// 	
// 	Revision 2.0  2006/02/28 08:33:12  marpas
// 	Exception 2.1 I/F adopted
// 	
// 	Revision 1.19  2006/02/14 13:04:59  enrcar
// 	Byteswapping macros moved in a separate file
// 	
// 	Revision 1.18  2006/02/14 10:20:17  enrcar
// 	modified to re-use code: byteswapping macro handles output-data requested byteorder
// 	
// 	Revision 1.17  2005/11/18 15:30:06  enrcar
// 	Added a temporary section for COSMO LData backward-compatibility
// 	
// 	Revision 1.16  2005/11/18 14:20:24  enrcar
// 	Added temporary section for backward compatibility
// 	
// 	Revision 1.15  2005/11/18 09:37:39  enrcar
// 	LData header always created as BIGENDIAN (even on LITTLEENDIAN machines)
// 	
// 	Revision 1.14  2005/11/17 18:06:21  enrcar
// 	Now LData are ALWAYS stored as BIGENDIAN, even if handled by LITTLEENDIAN machines
// 	
// 	Revision 1.13  2005/11/17 15:17:59  enrcar
// 	Checks on LData added
// 	
// 
///////////////////////////////////////////////////////////////////////////////////////
   

#ifndef _Operation02_H_
#define _Operation02_H_


#ifdef __cplusplus
    #include <acs_c++config.hpp>
	#include <exException.h>
	#include <ACS_SSL.h>
	#include <Operation03.h>

	extern "C" {
#endif
		void ACS_Operation02_checkData(const char* p, int exceptionType_BOOL) ;	/* C Wrap to static void Operation02::checkData(const std::string&, bool exceptionType=true) */
#ifdef __cplusplus
	}


#if(!0)

#else
/* T E M P O R A R Y   O N L Y */
/* Remove this section ASAP */
#warning "ACS::Operation02.h :: FORCED TO USE LITTLE_ENDIAN (COSMO backward compatibility)" ;
#define ACS_ByteSwap_SWAP(a, siz_a, isBend) {} 
#endif

// PRQA S 1020, 1023 L1

/* Access on buffer macros - internals */
#define ACS_Operation02_AddPackage(a, siz_a) { memcpy( (package.data()+pkgpointer), (a), (siz_a) ); pkgpointer += (siz_a); } 
#define ACS_Operation02_AddPackageEND(a, siz_a, isBend) { memcpy( (package.data()+pkgpointer), a, siz_a ); \
    ACS_ByteSwap_SWAP((package.data()+pkgpointer), siz_a, isBend); pkgpointer += (siz_a); }
#define ACS_Operation02_AddLData(a, siz_a) { memcpy( (ldata+datapointer), (a), (siz_a) ); datapointer += (siz_a); } 

#define ACS_Operation02_ReadLData(siz_a) { memcpy(package.data(), (ldata+datapointer), (siz_a)); datapointer += (siz_a); }
#define ACS_Operation02_ReadPackage(a, siz_a) { memcpy((a), (package.data()+pkgpointer), (siz_a)); pkgpointer += (siz_a); }
#define ACS_Operation02_ReadPackageEND(a, siz_a, isBend) { ACS_Operation02_ReadPackage((a), (siz_a)); \
    ACS_ByteSwap_SWAP((a), (siz_a), (isBend)); }

// PRQA L:L1

_ACS_BEGIN_NAMESPACE(acs)

class Operation02 // PRQA S 2109
{
public:  

    struct HINDataParam 
    {
        unsigned char           blocking ;
        unsigned short int      cmd ;
        unsigned char           __pad0000 ;
        signed int              param ;
        unsigned int            buflen ;
        unsigned char           buffer[52] ;        // NOSONAR 
        unsigned char           __unused ;
        unsigned short          __pad0001 ;
    } ;


    struct HOUTDataParam 
    {
        unsigned char           data[63] ;          // NOSONAR 
        unsigned char           __unused ;
    } ;

#if(!0)
    struct LDataParam
    {
        unsigned char           hwRescan ;                    
        unsigned char           appName[48] ;       // NOSONAR 
        unsigned int            creationDate ;
        unsigned char           type ;
        unsigned short          expDays ;            
        unsigned char           userData[320] ;     // NOSONAR 
        unsigned short          hwParamNum ;
        unsigned short          __pad0000 ;
        struct HINDataParam*    hwInData ;
        struct HOUTDataParam*   hwOutData ;
    } ;
#else

/* T E M P O R A R Y   O N L Y */
/* Remove this section ASAP */
#warning "ACS::Operation02.h :: FORCED TO COSMO backward compatibility" ;

    struct LDataParam
    {
        unsigned char           hwRescan ;                    
        unsigned char           appName[24] ;       // NOSONAR 
        unsigned int            creationDate ;
        unsigned char           type ;
        unsigned short          expDays ;            
        unsigned char           userData[320] ;     // NOSONAR 
        unsigned short          hwParamNum ;
        unsigned short          __pad0000 ;
        struct HINDataParam*    hwInData ;
        struct HOUTDataParam*   hwOutData ;
    } ;
#endif
	/*! class ThreadException declaration */
	exDECLARE_EXCEPTION(Operation02_InternalError,exException) ;    // PRQA S 2131, 2502

    class ResetAndGuardLDataParam
    {
    public:
        explicit ResetAndGuardLDataParam(struct LDataParam* a) noexcept ;
        ~ResetAndGuardLDataParam() noexcept ;
    private:
        struct LDataParam* _address ;

		ResetAndGuardLDataParam() ;
		ResetAndGuardLDataParam(const ResetAndGuardLDataParam&) ;
		ResetAndGuardLDataParam &operator=(const ResetAndGuardLDataParam&) ;
    } ;

public: // NOSONAR - for clarity
/* TEMPORARY -- should be removed */        
static void TEMPORARY_fill_LDataParam(struct LDataParam* ldata) ;    


    static void applyXORtoSequence(unsigned char* ret, unsigned char const* s, \
        unsigned char a, unsigned char b, unsigned int l) ;
    static void applyXOR( unsigned char* ret,  unsigned char const *s1, \
        unsigned char const *s2, unsigned int len) ;      
    static void cipherData(unsigned char* buffer, unsigned char const* s, 
        unsigned short ab, bool scrambleWithMAC, unsigned int l) ;  
    static void generateSequence(unsigned char* ret, unsigned short startval, \
        unsigned char a, unsigned char b, unsigned int l) ;   
    static void fitDataIntoBuffer(unsigned char* outbuf, unsigned int outlen, \
        unsigned char const* inbuf, unsigned int inlen, unsigned char pad=0) ;   
    static void dumpData_DEBUG(unsigned char const* buffer, unsigned int l, bool carriageReturn=false) ;   
    static void dumpLDataParam_DEBUG(struct LDataParam const*) ;   

    static char* bufferToBinAscii(const unsigned char* s, size_t sbuflen, char* d, size_t dbuflen) ;  
    static unsigned char* binAsciiToBuffer(const char* s, unsigned char* d, size_t *dbuflen) ;  


    /* exType==TRUE -> generate proper exCriticalException
    exType==FALSE -> generate generic exCriticalException */
    static void checkData(const std::string&, bool exType=true) ;


    static unsigned char* createLData(unsigned int* outdatalen, 
                                      char const* name, 
                                      const RSA* privKey, 
                                      ACS_SSL::CipherStyle cipherRSA, 
                                      struct LDataParam *) ;
                                      
    static void readLData(struct LDataParam*, 
                          unsigned char const*, 
                          const RSA* pubKey, 
                          ACS_SSL::CipherStyle cipherRSA, 
                          unsigned int ldatasize);
    static void dumpLData(struct LDataParam const*);


    static unsigned char* packLData( unsigned int* outlen,
                                  const unsigned char* ldata, 
                                  unsigned short ab, 
                                  bool scrambleWithMAC, 
                                  ACS_SSL::CipherStyle cipherRSA, 
                                  unsigned int len) ;

    static unsigned char* unpackLData(unsigned int* outlen,
                                      ACS_SSL::CipherStyle* cipheredRSA, 
                                      const unsigned char* blobdata, 
                                      unsigned int bloblen, 
                                      unsigned short ab);

    static bool checkLData(const unsigned char *, size_t, const std::string &, bool dumpData=false) ;
    static bool checkHWParams(struct LDataParam const*) ;


private: 

    ACS_CLASS_DECLARE_DEBUG_LEVEL(Operation02) ;

} ;


_ACS_END_NAMESPACE

#endif /* __cplusplus */
 
#endif	/* _Operation02_H_ */

