// PRQA S 1050 EOF
///////////////////////////////////////////////////////////////////////////////////////
// 
// 	Copyright 1995-2015, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
// 	$Prod: A.C.S. Operation02 class $
// 
// 	$Author$
// 
// 	$Id$
// 
// 	$Log$
// 	Revision 5.5  2015/05/08 10:21:10  davide.tiriticco
// 	ACS_Operation02_checkData defined in extern C namespace
// 	
// 	Revision 5.4  2014/02/07 18:42:30  marpas
// 	ACS_THROW and ACS_COND_THROW macros adopted
// 	
// 	Revision 5.3  2013/07/04 18:49:27  marpas
// 	finishing adoption of libException 5.x standards
// 	qa warnings removed
// 	compilation warnings removed
// 	coding best practices applied
// 	
// 	Revision 5.2  2013/07/03 16:41:25  marpas
// 	continuing adoption of libException 5.x standards
// 	qa warnings
// 	
// 	Revision 5.1  2013/07/02 17:31:26  marpas
// 	adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
// 	
// 	Revision 5.0  2013/06/20 16:43:53  marpas
// 	adoption of libException 5.x in progress
// 	
// 	Revision 2.13  2012/02/07 17:57:44  marpas
// 	refactoring in progress
// 	
// 	Revision 2.12  2011/03/01 11:40:16  marpas
// 	GCC 4.4.x support added
// 	
// 	Revision 2.11  2010/02/23 10:03:33  enrcar
// 	EC:: S1EGSE application was myspelled. Fixed.
// 	
// 	Revision 2.10  2010/02/22 16:15:03  enrcar
// 	EC:: Operation handled in project S1EGSE
// 	
// 	Revision 2.9  2009/06/19 15:43:33  enrcar
// 	EC:: Prosa project handled
// 	
// 	Revision 2.8  2009/06/04 15:20:31  enrcar
// 	EC:: optional map added to override the expected behaviour when a license is not found
// 	
// 	Revision 2.7  2009/05/14 13:03:46  enrcar
// 	EC:: c++ rules
// 	
// 	Revision 2.6  2009/03/19 15:53:55  enrcar
// 	EC:: GMESMCP Project handled
// 	
// 	Revision 2.5  2009/03/18 15:32:48  enrcar
// 	EC:: GMESMCP project handled
// 	
// 	Revision 2.4  2008/07/01 10:14:37  enrcar
// 	EC:: CipherStyle moved into libACS_SSL
// 	
// 	Revision 2.3  2008/06/30 15:31:02  enrcar
// 	EC:: HW-cypering is now supported (TDI Matrix). A further layer (Operation03) has been added.
// 	
// 	Revision 2.2  2008/06/05 10:23:24  enrcar
// 	EC:: Output improved
// 	
// 	Revision 2.1  2008/06/04 15:15:56  enrcar
// 	EC:: Operation02 supports HW Parameters check
// 	
// 	Revision 2.0  2006/02/28 08:33:12  marpas
// 	Exception 2.1 I/F adopted
// 	
// 	Revision 1.24  2006/02/14 13:04:59  enrcar
// 	Byteswapping macros moved in a separate file
// 	
// 	Revision 1.23  2006/02/14 10:20:16  enrcar
// 	modified to re-use code: byteswapping macro handles output-data requested byteorder
// 	
// 	Revision 1.22  2006/01/10 23:25:36  marpas
// 	oops - testing define was left
// 	
// 	Revision 1.21  2006/01/10 20:00:27  marpas
// 	singleton DB configuration checked trough an automatic declaration of an unused dbConnection instance.
// 	
// 	Revision 1.20  2005/12/14 10:33:55  enrcar
// 	Check on applicationname length added
// 	
// 	Revision 1.19  2005/12/05 17:57:25  giucas
// 	LData handling on CompactStation/L0PreInventory added
// 	
// 	Revision 1.18  2005/11/18 15:29:23  enrcar
// 	buffer residue is evaluated using data pointer and package size
// 	
// 	Revision 1.17  2005/11/18 09:37:39  enrcar
// 	LData header always created as BIGENDIAN (even on LITTLEENDIAN machines)
// 	
// 	Revision 1.16  2005/11/17 18:06:21  enrcar
// 	Now LData are ALWAYS stored as BIGENDIAN, even if handled by LITTLEENDIAN machines
// 	
// 	Revision 1.15  2005/11/17 15:17:59  enrcar
// 	Checks on LData added
// 	
//
///////////////////////////////////////////////////////////////////////////////////////


#include <ACS_ByteSwap.h>
#include <Operation03.h>
#include <Operation02.h>
#include <Operation01Factory.h>
#include <Operation01.h>
#include <HWScan.h>
#include <Filterables.h>
#include <map>
#include <cstring>
#include <string>
#include <algorithm>

// #undef ACS_OPERATION02_CHECKLDATA  DEFAULT IS: UNDEFINED (so, not needed to be undefined) 

#ifdef COSMO
    #define ACS_OPERATION02_CHECKLDATA
    #include <ACS_RSA_PublicKey.h>
    #include <CThread.h>
#endif
#ifdef COMPACTSTATION
    #define ACS_OPERATION02_CHECKLDATA
    #include <ACS_RSA_PublicKey.h>
    // NOT REQUIRED #include <CThread.h>
#endif
#ifdef ASARCANADA
    #define ACS_OPERATION02_CHECKLDATA
    #include <ACS_RSA_PublicKey.h>
    // NOT REQUIRED #include <CThread.h>
#endif
#ifdef GMESMCP
    #define ACS_OPERATION02_CHECKLDATA
    #include <ACS_RSA_PublicKey.h>
    // NOT REQUIRED #include <CThread.h>
#endif
#ifdef PROSA
    #define ACS_OPERATION02_CHECKLDATA
    #include <ACS_RSA_PublicKey.h>
    // NOT REQUIRED #include <CThread.h>
#endif
#ifdef S1EGSE
    #define ACS_OPERATION02_CHECKLDATA
    #include <ACS_RSA_PublicKey.h>
    // NOT REQUIRED #include <CThread.h>
#endif



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
   


ACS_CLASS_DEFINE_DEBUG_LEVEL(Operation02);


Operation02::MemoryGuard::MemoryGuard(void* address) throw() :_address(address) {} 
Operation02::MemoryGuard::~MemoryGuard()  throw() { if (_address) { free(_address); _address=0; } } 

extern "C" {

	//------------------------------------------------------------------------
	//     C-Wrap to Operation02::checkData(const std::string&, bool exceptionType) 
	//------------------------------------------------------------------------
	void ACS_Operation02_checkData(const char* p, int exceptionType_BOOL) // PRQA S 2502
	{
		try 
		{
			Operation02::checkData( p, (exceptionType_BOOL != 0) ) ;
		}
		catch(exception& e)
		{
			ostringstream os ;

			/* The exception won't be notified in order not to disclose the module name / line# */

			if (exceptionType_BOOL != 0) {

				// Emit explicit message
				{
					int i[] = {'N','o',' ','v','a','l','i','d',' ','l','i','c','e','n','s','e',' ','f','o','u','n','d',' ','f','o','r',0} ;
					char s[sizeof(i)/sizeof(i[0])] ; // PRQA S 3084
					for (size_t j=0; j < sizeof(s); j++) {
						s[j] = char(i[j]) ; // PRQA S 3081
                	}
					os << s ;
				}
				os << " " << p ;
				os << " on " << "hostname " << HWScan::hostname() ;  
				os << " please" << " report" << " to" << " technical" << " support." ;
			}
			else {
				// Emit not-explicit message
				os << "Cannot create temporary file \"" << "/tmp/dbOp" << "feature" << ".tmp" << "\"" ;
				os << "on hostname " << HWScan::hostname() ;
				os << " please" << " report" << " to" << " technical" << " support." ;
			}

			exostream::getDefaultOutLogger()<< os.str() << "\n" << "\n" ;
			exit (0x55) ;	/* Check failed */ // PRQA S 4400
		}

		return ;
	}

} // extern "C"
//------------------------------------------------------------------------
//                                 Operation02
//------------------------------------------------------------------------
Operation02::Operation02() throw() 
{	

    #if(!0)
        // IF HERE, DEBUG ALWAYS ALLOWED
        #warning "Operation02:: WARNING: DEBUG FORCED TO BE ALLOWED" ;
        #define ACS_OPERATION02_HAS_DEBUG_ENABLED 1
    #endif


    #if(0)
        // IF HERE, NO DEBUG CAN BE ALLOWED
        #undef ACS_OPERATION02_HAS_DEBUG_ENABLED 
    #endif


    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED        
        #warning "Operation02:: WARNING: DEBUG ALLOWED" ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method Called") ;
    #endif 
}


//------------------------------------------------------------------------
//                                 ~Operation02
//------------------------------------------------------------------------
Operation02::~Operation02() throw()
{
    // PRQA S 4631 5
    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method Called") ;
    #endif 
}


//------------------------------------------------------------------------
//                            ResetAndGuardLDataParam
//------------------------------------------------------------------------
Operation02::ResetAndGuardLDataParam::ResetAndGuardLDataParam(struct LDataParam* a) throw() 
    :_address(a)
{
    if ( _address ) { memset(_address, 0, sizeof(struct LDataParam) ) ; }
} 


//------------------------------------------------------------------------
//                            ~ResetAndGuardLDataParam
//------------------------------------------------------------------------
Operation02::ResetAndGuardLDataParam::~ResetAndGuardLDataParam() throw() // PRQA S 4020
{ 
	// Guard destructor 
    if (!_address) { return ; }

    if (_address->hwInData) { free(_address->hwInData) ; }
    
    if (_address->hwOutData) { free(_address->hwOutData) ; }
    
    free (_address);
    _address = 0;
} 



/****************************************************************************************/
/****************************************************************************************/
/**                        TEMPORARY STATIC FUNCTION                                   **/
/****************************************************************************************/
/****************************************************************************************/
void Operation02::TEMPORARY_fill_LDataParam(struct LDataParam* ldata) 
{
    ldata->hwRescan = 1;
    memset(ldata->appName, 0, sizeof(ldata->appName)) ; strcpy(reinterpret_cast<char*>(ldata->appName), "test"); // PRQA S 3030, 3081
    ldata->creationDate = int(time(0)) ; // PRQA S 3000, 3081

    ldata->type = 0x01 ;
    ldata->expDays = 29 ;   // PRQA S 4400
    memset(ldata->userData, 0, sizeof(ldata->userData) ) ;
    strcpy(reinterpret_cast<char*>(ldata->userData), "COSMO TEST LICENSE $Id$ 2005.11.04 Very preliminary version. No hardware encoded. No private key. Little Endian.") ; // PRQA S 3030, 3081
    ldata->hwParamNum = 0 ;
    struct HINDataParam* hw_invector = reinterpret_cast<struct HINDataParam*>( calloc(ldata->hwParamNum, sizeof(struct HINDataParam))) ; // PRQA S 3081
   
#if(0)
    int tmpcounter = 0;
// Filling hardware element....
    hw_invector[tmpcounter].blocking = 0x99 ;  // PTQA S 4400
    hw_invector[tmpcounter].cmd = 0x11 ;  // PTQA S 4400
    hw_invector[tmpcounter].param = 0x2233 ;  // PTQA S 4400
    
    memset(hw_invector[tmpcounter].buffer, 0, sizeof(hw_invector[tmpcounter].buffer) ) ;
    strcpy(reinterpret_cast<char*>(hw_invector[tmpcounter].buffer), "test") ;
    hw_invector[tmpcounter].buflen = strlen(reinterpret_cast<char*>(hw_invector[tmpcounter].buffer)) ;
    tmpcounter ++ ; // prepare next element  


// Filling hardware element....
    hw_invector[tmpcounter].blocking = 0x99 ;  // PTQA S 4400
    hw_invector[tmpcounter].cmd = 0x11 ;  // PTQA S 4400
    hw_invector[tmpcounter].param = 0x2233 ;  // PTQA S 4400
    
    memset(hw_invector[tmpcounter].buffer, 0, sizeof(hw_invector[tmpcounter].buffer) ) ;
    strcpy(reinterpret_cast<char*>(hw_invector[tmpcounter].buffer), "test") ;
    hw_invector[tmpcounter].buflen = strlen(reinterpret_cast<char*>(hw_invector[tmpcounter].buffer)) ;
    tmpcounter ++ ; // prepare next element  
#endif

// Set pointer to vector
    ldata->hwInData = hw_invector ; // set pointer to vector

    ldata->hwOutData = 0;

}    


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Dump Binary Data for DEBUG
//
///////////////////////////////////////////////////////////////////////////////////////
void 
    Operation02::dumpData_DEBUG(unsigned char const* buffer,    /* output data */ // PRQA S 4020
                                unsigned int l,                 /* input data length */
                                bool carriageReturn)
{
    // In case of problems, generate internal error IE0x0206
    
    try
    {
        printf( "Operation02::dumpData_DEBUG: l=%d CR=%d\n", l, int(carriageReturn) ); // PRQA S 3081

        for (unsigned int i=0; i<l; i++)
        {
            printf("%02x", static_cast<unsigned int>( buffer[i])); // PRQA S 3081
            if (carriageReturn) { printf("\n");  }
        } 
        printf("%s\n", ""); 

        return ; 

    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x0206"));
    }
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Dump LData for DEBUG
//
///////////////////////////////////////////////////////////////////////////////////////
void 
        Operation02::dumpLDataParam_DEBUG(struct LDataParam const* ld) 
{
    // In case of problems, generate internal error IE0x020c

    printf("Operation02::dumpLDataParam_DEBUG. Method called.\n");
    unsigned char* p = 0 ;

    try
    {
        printf("hwRescan: 0x%x\n", int(ld->hwRescan)); // PRQA S 3081
        printf("appName: !%s!\n", ld->appName);
        {
			time_t tmp = ld->creationDate ;	// Get creation date // PRQA S 3000
			string stmp = ctime(&tmp) ;
			printf("creationDate: %d (%s)\n", ld->creationDate, stmp.substr(0,stmp.length()-1).c_str() ); // PRQA S 3084
        }
        printf("type: 0x%x\n", int(ld->type)); // PRQA S 3081
        printf("expDays: %d\n", ld->expDays);
        printf("userData: !%s!\n", ld->userData);
        printf("hwParamNum: %d\n", ld->hwParamNum);

		// Dump all HW parameters
        for (unsigned int i=0; i<ld->hwParamNum; i++)
        {
            printf("\nHINDataParam [%d]\n", i);
            printf("\tblocking: 0x%x\n", int(ld->hwInData[i].blocking)); // PRQA S 3081
            printf("\tcmd: 0x%x\n", ld->hwInData[i].cmd);
            printf("\tparam: 0x%x\n", ld->hwInData[i].param);
            printf("\tbuflen: %d\n", ld->hwInData[i].buflen);
            printf("\tbuffer: !%s! 0x", ld->hwInData[i].buffer);
            p = ld->hwInData[i].buffer ; 
            for (unsigned int j=0; j<sizeof(ld->hwInData[i].buffer); j++) {
                printf(" %02x", static_cast<unsigned int>(p[j])); // PRQA S 3081
            }
            printf("\n") ;

            printf("HOUTDataParam [%d]\n", i);
#if(0)
            printf("\tdata: !%s! 0x", ld->hwOutData[i].data);
#else
            printf("\tdata: 0x");
#endif
            p = ld->hwOutData[i].data ; 
            for (unsigned int j=0; j<sizeof(ld->hwOutData[i].data); j++) {
                printf(" %02x", static_cast<unsigned int>(p[j])); // PRQA S 3081
            }
            printf("\n") ;
           
        }
    }
    catch(exception& e)
    {
		// Return exception
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x020c"));
    }

    return ;	/* Operation02::dumpLDataParam_DEBUG */
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Apply XOR between 2 buffers, and fill a 3rd
//
///////////////////////////////////////////////////////////////////////////////////////
void Operation02::applyXOR( unsigned char* ret,         /* output data */ // PRQA S 4020
                            unsigned char const *s1,    /* input data #1 */ 
                            unsigned char const *s2,    /* input data #2 */
                            unsigned int len)           /* input data length */
{
    //
    // s1, s2, ret MUST BE ALREADY ALLOCATED, size_s1 == size_s2 == size_ret == len
    //
    // In case of problems, generate internal error IE0x0201
    //
    try
    {
        /*
            perform operation:
            ret[i] = s1[i] XOR s2[i], i=0..(len-1)
            as 8-bit digits
        */


        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"len = " << len) ; 
        #endif


        for (unsigned int i=0; i<len; i++)
        { 
            ret[i] = s1[i] ^ s2[i] ; // PRQA S 3000, 3010
            
            #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB,hex << " ret = 0x" << static_cast<unsigned int>(ret[i])  // PRQA S 3081
                    << " s1 = 0x" << static_cast<unsigned int>(s1[i]) << " s2 = 0x" << static_cast<unsigned int>(s2[i])); 
            #endif
        }
        
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Finished") ;
        #endif

        return ; 
        
    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x0201"));
    }
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Generate a sequence (a,b) and Apply XOR
//
///////////////////////////////////////////////////////////////////////////////////////
void Operation02::applyXORtoSequence(   unsigned char* ret,                /* output data */ // PRQA S 4020 
                                        unsigned char const* s,             /* input data */
                                        unsigned char a, unsigned char b,   /* sequence params */
                                        unsigned int l)                     /* input data length */
{
    //
    // s, ret MUST BE ALREADY ALLOCATED, size_s == size_ret == len
    //
    // In case of problems, generate internal error IE0x0203

    try
    {
        /*
            perform operation:
            ret[i] = s1[i] XOR seq(a,b)[i], i=0..(l-1)
            as 8-bit digits
        */


        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB,hex  // PRQA S 3081 
                << " a: 0x" << static_cast<unsigned int>(a) << " b: 0x" << static_cast<unsigned int>(b) << dec << " len: " << l);
        #endif

        // Generate TEMPORARY sequence
        unsigned char* seq = reinterpret_cast<unsigned char*> (malloc(l)) ; // PRQA S 3081 2
		ACS_COND_THROW(!seq,Operation02_InternalError("Operation02::Internal error IE0x0203")) ; 
        
        MemoryGuard memoryGuard(seq) ;  // no free() is required
   
        ACS_SSL::ACS_RC4_pseudorand(seq, a, b, l);
        
        
        // Apply XOR
        applyXOR( ret, s, seq, l); 
            
        
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Finished") ;
        #endif

        return ; 
        
    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x0203")) ;
    }
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Cipher Data
//
///////////////////////////////////////////////////////////////////////////////////////
void 
        Operation02::cipherData(unsigned char* buffer,      /* output data */  // PRQA S 4020
                                unsigned char const* s,     /* input data */
                                unsigned short ab,          /* used to generate sequence */    
                                bool scrambleWithMAC,       /* as above */   
                                unsigned int l)             /* input data length */
{

    // In case of problems, generate internal error IE0x0204
    try
    {
        /*
            perform operation:
            ret[i] = s1[i] XOR s2[i], i=0..(l-1)
            as 8-bit digits
        */


        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, hex // PRQA S 3081  
                << " ab: 0x" << static_cast<unsigned int>(ab) << dec 
                << " scramble with MAC: (bool) " << int(scrambleWithMAC)
                << " len: " << l) ;
        #endif
       
       
        if (scrambleWithMAC)
        {
            #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Getting MAC for first Ethernet card...") ;
            #endif
            
            signed int retval=-1 ; 
            signed int param=0 ;   // 1st ethernet card
                
            // Perform HW Scan. Memory will be deallocated above...
            unsigned char* macAddr = reinterpret_cast<unsigned char*>( HWScan::ScanHW_Scan(&retval, HWScan::GETMAC, param, 0, 0)) ; // PRQA S 3081
            
            if ( !macAddr || (retval<=0) )
            {
                // Cannot get mac address
                ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0205")); // PRQA S 3081
            }
          
            MemoryGuard m(macAddr) ; // no free is required ;

            
            #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MAC Address:") ;
                    dumpData_DEBUG(macAddr, retval) ; // PRQA S 3000
                ACS_CLASS_END_DEBUG
            #endif
        
            unsigned short mac = 0;
            Operation02::fitDataIntoBuffer(reinterpret_cast<unsigned char*>(&mac), sizeof(mac), macAddr, retval) ; // PRQA S 3000, 3030, 3081
            
            // Scramble with mac address
            ab ^= mac ; // Same type // PRQA S 3000
            
            #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New ab parameter: " << ab) ;
                    dumpData_DEBUG(macAddr, retval) ; // PRQA S 3000
                ACS_CLASS_END_DEBUG
            #endif
        
        } // if (scrambleWithMAC)
 

        {        
            unsigned char a = (ab & 0xff00) >> 8; // PRQA S 3000, 3007, 3010, 4400 2
            unsigned char b = (ab & 0x00ff) ;
            applyXORtoSequence(buffer, s, a, b, l);
        }                
 
        
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Finished.") ;
        #endif

        return ; 
        
    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x0204"));
    }
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Copy (Fit) a buffer into another buffer.
//   If source buffer is smaller, is padded with a char
//   If source buffer is larger, is wrapped using (several) XOR passes
//
///////////////////////////////////////////////////////////////////////////////////////
void 
        Operation02::fitDataIntoBuffer( unsigned char* outbuf,      // IN: destination buffer // PRQA S 4020
                                        unsigned int outlen,        // IN: destination buffer len
                                        unsigned char const* inbuf, // IN: source buffer len
                                        unsigned int inlen,         // IN: source buffer len
                                        unsigned char pad)          // IN: Padding char (inlen<outlen) 
{
    // In case of problems, generate internal error IE0x0207
    try
    {
 
        // CAN BE: inlen <, ==, > outlen


        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inlen=" << inlen << " outlen=" << outlen // PRQA S 3081
                << " pad " << hex << int(pad) << dec) ;
        #endif

        memset(outbuf, 0, outlen);


        for (unsigned int j=0; j<inlen; j++) {
            outbuf[ j % outlen ] ^= inbuf[j] ; // PRQA S 3000, 3010, 3084
        }


        if (inlen<outlen) {
            memset( outbuf+inlen, pad, (outlen-inlen) ) ; // PRQA S 3084
        }


        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Finished.") ;
        #endif

        return ;

    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x0207"));
    }
}


void Operation02::checkData(const std::string &feature, bool exType) // PRQA S 4020
{
	// EnrCar
	// To keep the complexity as low as possible
	
    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "feature is: \"" << feature << "\"") ;
    #endif
	vector <string> v ;
    map <string, char> m ;	/* optional overriding map. To override the standard behaviour for a specific task (feature), 
								just insert the feature name into the map, with the parameter: C (core), E (standard error), L (license error).
								The core can be obtained only if Thread.h file included  */
	char override = 0 ;
	
#ifdef ACS_OPERATION02_CHECKLDATA
	#ifdef COSMO
		{
			int i[] = {'I','n','v','e','n','t','o','r','y','A','p','p',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}
		{
			int i[] = {'L','0','P','r','e','P','r','o','c','e','s','s','o','r',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}		
		{
			int i[] = {'L','0','P','_','L','0','F','P','r','o','c','e','s','s','o','r',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
			m.insert(pair<string, char> (string(s), 'L') );	/* Override */
		}
	#endif
	
	#ifdef COMPACTSTATION
		{
			int i[] = {'L','0','P','r','e','P','r','o','c','e','s','s','o','r',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}	
	#endif
    
	#ifdef ASARCANADA
		{
			int i[] = {'G','a','s','p','P','r','o','c','e','s','s','o','r',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}
	#endif

	#ifdef GMESMCP
		{
			int i[] = {'M','c','p',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}

		{
			int i[] = {'m','c','p','C','o','v','e','r','a','g','e',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}
	#endif
	
	#ifdef PROSA
		{
            int i[] = {'I','n','v','e','n','t','o','r','y','A','p','p',0} ;
            char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}	
	#endif
     
	#ifdef S1EGSE
		{
			int i[] = {'L','0','P','r','e','P','r','o','c','e','s','s','o','r',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}	
		{
			int i[] = {'D','i','r','e','c','t','D','a','t','a','C','a','p','t','u','r','e',0} ;
			char s[sizeof(i)/sizeof(i[0])] ;
			for (size_t j=0; j < sizeof(s); j++)
				s[j] = (char)i[j] ;
			v.push_back(string(s)) ;
		}	
	#endif
 
#else

	#ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
		// ACS_OPERATION02_CHECKLDATA undefined. Nothing to do
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Compilation parameter undefined. Nothing to do !" ) ;
	#endif
	
	return ;	/****   Nothing to do!   ****/

#endif /* ifdef ACS_OPERATION02_CHECKLDATA */

// 	{
// 		int i[] = {'H','y','B','r','o',0} ;
// 		char s[sizeof(i)/sizeof(i[0])] ;
// 		for (size_t j=0; j < sizeof(s); j++)
// 			s[j] = (char)i[j] ;
// 		v.push_back(string(s)) ;
//		OPTIONAL: m.insert(pair<string, char> (string(s), 'L') );	/* Override */
// 	}
	if (find(v.begin(), v.end(), feature) == v.end()) {
		return ;
    }

	override = ( m.find( feature) != m.end() ) ? m[feature]: 0 ; // PRQA S 3010, 3380, 3385
	
	if (0 != override)
	{
		exType = ( override == 'L' ) ;	/* Override value */
	}

    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			char ov[128] ; // PRQA S 4403
			if (0 != override) { sprintf(ov, "%c", override); } else { strcpy(ov, "UNDEFINED") ; }
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "feature has been found. Override: !"  << ov << "!  exType:" << boolalpha << exType) ;
        ACS_CLASS_END_DEBUG
    #endif
    Operation01 *theOp1 = Operation01Factory::newObject(feature,"") ; // PRQA S 4412
	
	if (theOp1) {
		unsigned char buffer[32768] ; // PRQA S 4403
		string mod ;
		vector<string> mods ;
		mods.push_back(string("_")+HWScan::hostname()) ; // PRQA S 3081
		mods.push_back("_glob") ; // leave it the last 

        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                size_t mods_size = mods.size() ;
                for ( size_t i=0 ; i< mods_size ; ++i ) {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "mods[" << i <<"] is " << mods[i]);
                }
            ACS_CLASS_END_DEBUG
        #endif
		size_t size = theOp1->get(buffer, sizeof(buffer), mod, mods) ;
		delete theOp1 ;
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " size is " << size) ;
        #endif
		if ((0 == size) || !checkLData(buffer,size,feature+mod) ) { // PRQA S 3230
// DO NOT CHANGE LINE BELOW.
// INSERT THE LIST OF PROJECT(S) TO USE CThread CLASS:
#ifdef COSMO
			if ( !override || (override == 'C'))
			{
				CThread *ct= new CThread(247) ;
				ct->start() ;
				return ; // eh eh
			}
#endif

// IF HERE: NO CThread. An exception will be generated:
			if (exType) {

				ostringstream os ;
				{
					int i[] = {'N','o',' ','v','a','l','i','d',' ','l','i','c','e','n','s','e',' ','f','o','u','n','d',' ','f','o','r',0} ;
					char s[sizeof(i)/sizeof(i[0])] ; // PRQA S 3084
					for (size_t j=0; j < sizeof(s); j++) {
						s[j] = char(i[j]) ; // PRQA S 3081
                    }
					os << s ;
				}
				os << " " << feature ;
				os << " on " << "hostname " << HWScan::hostname() ;  
				os << " please" << " report" << " to" << " technical" << " support." ;
				ACS_THROW(exCriticalException(os.str())) ; // PRQA S 3081
			}
			else {
				ostringstream os ;
				os << "Cannot create temporary file \"" << "/tmp/dbOp" << "feature" << ".tmp" << "\"" ;
				os << "on hostname " << HWScan::hostname() ;
				os << " please" << " report" << " to" << " technical" << " support." ;
				ACS_THROW(exCriticalException(os.str())) ; // PRQA S 3081
			}
		}	
	}
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Process LDataParam, creating LData
//
///////////////////////////////////////////////////////////////////////////////////////
unsigned char*                                                                  /* OUT: LDATA  */
               Operation02::createLData(unsigned int*   outdatalen,             /* OUT_LDATA length  */
                                        char const*     name,                   /* IN: application name (0 term.) */
                                        RSA* privKey,                           /* IN: RSA Private Key (or 0) */
                                        ACS_SSL::CipherStyle cipherRSA,			/* IN: cipher with RSA key */
                                        struct LDataParam const* indataparam)   /* IN: data param  */
{	
	// EnrCar:
	// The method is intentionally not-optimized in order to be aligned w/ all the logical operation performed


    // In case of problems, generate internal error IE0x0208, IE0x0209
    unsigned char*  retdata = 0 ;
    (*outdatalen) = 0 ;
    bool dataIsBEND = true ;    /* LData is BIGENDIAN */
    
    try
    {
        if (strlen(name) >= sizeof(indataparam->appName))
        {
            // Application Name too long to be stored (and later retrived)
			ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0224")); // PRQA S 3081
        }

        strncpy(const_cast<char*>(reinterpret_cast<const char*>(indataparam->appName)), name, sizeof(indataparam->appName)-1); // PRQA S 3030, 3081, 3084
 
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for AppName:" << reinterpret_cast<const char*>(indataparam->appName)) ; // PRQA S 3030, 3081
        #endif

        unsigned int    datapointer = 0 ;
        unsigned char   package[1024] ; // PRQA S 4403

        unsigned char   temp_buffer[1024] ; // PRQA S 4403
        // UNUSED. DECOMMENT IF NEEDED:  unsigned int    temp_i ;
        // UNUSED. DECOMMENT IF NEEDED:  unsigned short  temp_s ;
        unsigned char   temp_b = 0 ;

        unsigned char*  ldata = reinterpret_cast<unsigned char*>(malloc(64*1024)) ; // will be reallocated to correct size // PRQA S 3081
        if (! ldata)
        {
			ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0209")); // PRQA S 3081
        }


        // License params row #1, #2
        for (unsigned int i=0; i<2; i++) // PRQA S 4400
        {
            unsigned int pkgpointer = 0;
            unsigned int dataLen = 64 ; // uncrypted package size

            temp_b = 0x00; 
            if (i > 0) { temp_b = indataparam->hwRescan ; }

            // RESCAN
            ACS_Operation02_AddPackageEND(&temp_b, sizeof(temp_b), dataIsBEND) ; // PRQA S 3081, 3084, 4090
            
            // APPNAME
            ACS_Operation02_AddPackage(indataparam->appName, sizeof(indataparam->appName) ) ; // PRQA S 3084
            
            // CREATIONDATE
            ACS_Operation02_AddPackageEND(&indataparam->creationDate, sizeof(indataparam->creationDate), dataIsBEND ) ; // PRQA S 3081, 3084, 4090
            
            // TYPE
            ACS_Operation02_AddPackageEND(&indataparam->type, sizeof(indataparam->type), dataIsBEND ) ; // PRQA S 3081, 3084, 4090
            
            // EXP.DAYS
            ACS_Operation02_AddPackageEND(&indataparam->expDays, sizeof(indataparam->expDays), dataIsBEND ) ; // PRQA S 3081, 3084, 4090
            
            // RSVD
            memset(temp_buffer, 0, dataLen); ACS_Operation02_AddPackage(temp_buffer, (dataLen-pkgpointer)) ; // PRQA S 3084
		
            
            /* Store [RSA_ciphered] data */
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int outl = 0 ;
                unsigned char* tmp = Operation03::EncodePkg(cipherRSA, &outl, privKey, package, pkgpointer) ; 
                Operation02::MemoryGuard g(tmp);
                ACS_Operation02_AddLData(tmp, outl) ; // PRQA S 3084
            }
            else
                ACS_Operation02_AddLData(package, pkgpointer) ; // PRQA S 3084

        }

        // User Data:
        {
            unsigned int pkgpointer = 0;

            // USERDATA
            ACS_Operation02_AddPackage(indataparam->userData, sizeof(indataparam->userData) ) ; // PRQA S 3084


            /* Store [RSA_ciphered] data */
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int outl = 0 ;
                unsigned char* tmp = Operation03::EncodePkg(cipherRSA, &outl, privKey, package, pkgpointer) ; 
                Operation02::MemoryGuard g(tmp);
                ACS_Operation02_AddLData(tmp, outl) ; // PRQA S 3084
            }
            else
                ACS_Operation02_AddLData(package, pkgpointer) ; // PRQA S 3084
        }

        // HW Params header
        {
            unsigned int pkgpointer = 0;
            unsigned int dataLen = 64 ; // uncrypted package size

            // NUMBER OF HW.PARAMETERS
            ACS_Operation02_AddPackageEND(&indataparam->hwParamNum, sizeof(indataparam->hwParamNum), dataIsBEND ) ; // PRQA S 3081, 3084, 4090
           
            // RSVD
            memset(temp_buffer, 0, dataLen); ACS_Operation02_AddPackage(temp_buffer, (dataLen-pkgpointer)) ; // PRQA S 3084


            /* Store [RSA_ciphered] data */
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int outl = 0 ;
                unsigned char* tmp = Operation03::EncodePkg(cipherRSA, &outl, privKey, package, pkgpointer) ; 
                Operation02::MemoryGuard g(tmp);
                ACS_Operation02_AddLData(tmp, outl) ; // PRQA S 3084
            }
            else
                ACS_Operation02_AddLData(package, pkgpointer) ;// PRQA S 3084
        }


        // HW PARAMS_IN
        for (unsigned int i=0; i<indataparam->hwParamNum; i++)
        {
            unsigned int pkgpointer = 0;

            // Blocking
            ACS_Operation02_AddPackageEND(& indataparam->hwInData[i].blocking, sizeof(indataparam->hwInData[i].blocking), dataIsBEND ) ; // PRQA S 3081, 3084, 4090

            // CMD
            ACS_Operation02_AddPackageEND(& indataparam->hwInData[i].cmd, sizeof(indataparam->hwInData[i].cmd), dataIsBEND ) ; // PRQA S 3081, 3084, 4090

            // PARAM
            ACS_Operation02_AddPackageEND(& indataparam->hwInData[i].param, sizeof(indataparam->hwInData[i].param), dataIsBEND ) ; // PRQA S 3081, 3084, 4090

            // BUFLEN
            ACS_Operation02_AddPackageEND(& indataparam->hwInData[i].buflen, sizeof(indataparam->hwInData[i].buflen), dataIsBEND ) ; // PRQA S 3081, 3084, 4090

            // BUFFER
            ACS_Operation02_AddPackage(indataparam->hwInData[i].buffer, sizeof(indataparam->hwInData[i].buffer) ) ; // PRQA S 3084
            
            // RSVD
            temp_b = 0x00; ACS_Operation02_AddPackage(&temp_b, sizeof(temp_b)) ; // PRQA S 3084
   

            /* Store [RSA_ciphered] data */
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int outl = 0 ;
                unsigned char* tmp = Operation03::EncodePkg(cipherRSA, &outl, privKey, package, pkgpointer) ; 
                Operation02::MemoryGuard g(tmp);
                ACS_Operation02_AddLData(tmp, outl) ; // PRQA S 3084
            }
            else
                ACS_Operation02_AddLData(package, pkgpointer) ; // PRQA S 3084
        }
 

        // HW PARAMS_OUT
        for (unsigned int i=0; i<indataparam->hwParamNum; i++)
        {
	        unsigned int pkgpointer = 0;

          
            // BUFFER
            ACS_Operation02_AddPackage(indataparam->hwOutData[i].data, sizeof(indataparam->hwOutData[i].data) ) ; // PRQA S 3084
            
            // UNUSED
            temp_b = 0x00; ACS_Operation02_AddPackage(&temp_b, sizeof(temp_b)) ;  // PRQA S 3084
   

            /* Store [RSA_ciphered] data */
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int outl = 0 ;
                unsigned char* tmp = Operation03::EncodePkg(cipherRSA, &outl, privKey, package, pkgpointer) ; 
                Operation02::MemoryGuard g(tmp);
                ACS_Operation02_AddLData(tmp, outl) ; // PRQA S 3084
            }
            else
                ACS_Operation02_AddLData(package, pkgpointer) ; // PRQA S 3084
        }
 
        // Realloc to correct size
        retdata = reinterpret_cast<unsigned char*>( realloc(ldata, datapointer) ); // PRQA S 3081
        if (!retdata)
        {
            // Unable to realloc
            if (ldata) { free(ldata) ;  ldata = 0 ; } // PRQA S 3339
			ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x020a")); // PRQA S 3081
        }

        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                ACS_LOG_DEBUG(SimpleDebugSignature << "datapointer: " << datapointer) ;
                dumpData_DEBUG(ldata, datapointer, false) ;   
            ACS_CLASS_END_DEBUG
        #endif

        (*outdatalen) = datapointer ;   // Set correct return size
    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x0208"));
    }
   
    return retdata;
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   Process LData, creating LDataParam
//
///////////////////////////////////////////////////////////////////////////////////////
void 
        Operation02::readLData( struct LDataParam* outdataparam,    /* OUT: struct LDataParam */
                                unsigned char const* ldata,         /* IN: LDATA */
                                RSA* pubKey,                        /* IN: RSA Public Key (or 0) */
                                ACS_SSL::CipherStyle cipherRSA,		/* IN: ciphered with RSA key */
                                unsigned int ldatasize)             /* IN: IN_LDATA_length */
{
	// EnrCar:
	// Due to endianness fast-swapping macros. The code is fully readable

	// The method is intentionally not-optimized in order to be aligned w/ all the logical operation performed
	

    try
    {
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called")  ;
                dumpData_DEBUG(ldata, ldatasize);
            ACS_CLASS_END_DEBUG
        #endif


#ifndef ACS_OPERATION02_CHECKLDATA
		if (cipherRSA != ACS_SSL::NOCIPHER)
		{
			// Decrypting keys are required, but not loaded. 
			// ACS_OPERATION02_CHECKLDATA should be DEFINED (and IT ISN'T)
			ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0230")); // PRQA S 3081
		}
#endif    


        unsigned char   temp_buffer[1024] ; // PRQA S 4403
        // UNUSED. DECOMMENT IF NEEDED:  unsigned int    temp_i ;
        // UNUSED. DECOMMENT IF NEEDED:  unsigned short  temp_s ;
        unsigned char   temp_b = 0;

        unsigned char   package[1024] ; // PRQA S 4403
        unsigned int    datapointer = 0;
        bool dataIsBEND = true ;    /* LData is BIGENDIAN */


        // License params row #1, #2
        for (unsigned int i=0; i<2; i++) // PRQA S 4400
        {
            unsigned int dataLen = 64 ; // uncrypted size
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int packageLen = Operation03::GetCipheredPkgSize(cipherRSA, pubKey, dataLen) ;
                ACS_Operation02_ReadLData(packageLen);  // (temporarily)  fill package with ciphered data // PRQA S 3084
                unsigned int outl = 0;
                unsigned char* tmp = Operation03::DecodePkg(cipherRSA, &outl, pubKey, package, packageLen) ;
                Operation02::MemoryGuard g(tmp); 
                if (outl != dataLen)
                {
                    // size of deciphered package doesn't match to dataLen
 					ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0220")); // PRQA S 3081
                }
                memset(package, 0, sizeof(package)) ;
                memcpy(package, tmp, outl) ;    // fill package with unciphered data
            }
            else
                ACS_Operation02_ReadLData(dataLen); // PRQA S 3084
            
            unsigned int pkgpointer = 0 ;


            // RESCAN
            ACS_Operation02_ReadPackageEND(&temp_b, sizeof(temp_b), dataIsBEND) ; // PRQA S 3081, 3084, 4090
            if (i > 0 ) { outdataparam->hwRescan = temp_b; }

            // APPNAME
            ACS_Operation02_ReadPackage(outdataparam->appName, sizeof(outdataparam->appName)) ; // PRQA S 3084

            // CREATIONDATE
            ACS_Operation02_ReadPackageEND(&outdataparam->creationDate, sizeof(outdataparam->creationDate), dataIsBEND ) ; // PRQA S 3030, 3081, 3084, 4090

            // TYPE
            ACS_Operation02_ReadPackageEND(&outdataparam->type, sizeof(outdataparam->type), dataIsBEND ) ; // PRQA S 3081, 3084, 4090

            // EXP.DAYS
            ACS_Operation02_ReadPackageEND(&outdataparam->expDays, sizeof(outdataparam->expDays), dataIsBEND ) ; // PRQA S 3030, 3081, 3084, 4090

            // RSVD
            ACS_Operation02_ReadPackage(temp_buffer, (dataLen-pkgpointer) ) ; // PRQA S 3084
        }

        // User Data:
        {
            unsigned int dataLen = (64*5) ; // uncrypted size
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int packageLen = Operation03::GetCipheredPkgSize(cipherRSA, pubKey, dataLen) ;
                ACS_Operation02_ReadLData(packageLen);  // (temporarily)  fill package with ciphered data // PRQA S 3084
                unsigned int outl = 0;
                unsigned char* tmp = Operation03::DecodePkg(cipherRSA, &outl, pubKey, package, packageLen) ;
                Operation02::MemoryGuard g(tmp);
                if (outl != dataLen)
                {
                    // size of deciphered package doesn't match to dataLen
 					ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0221")); // PRQA S 3081
                } 
                memset(package, 0, sizeof(package)) ;
                memcpy(package, tmp, outl) ;    // fill package with unciphered data
            }
            else {
                ACS_Operation02_ReadLData(dataLen); // PRQA S 3084
            }
            
            unsigned int pkgpointer = 0 ;


            // USERDATA
            ACS_Operation02_ReadPackage(outdataparam->userData, sizeof(outdataparam->userData) ) ; // PRQA S 3084
        }


        // HW Params header
        {
            unsigned int dataLen = 64 ; // uncrypted size
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int packageLen = Operation03::GetCipheredPkgSize(cipherRSA, pubKey, dataLen) ;
                ACS_Operation02_ReadLData(packageLen);  // (temporarily)  fill package with ciphered data // PRQA S 3084
                unsigned int outl = 0;
                unsigned char* tmp = Operation03::DecodePkg(cipherRSA, &outl, pubKey, package, packageLen) ;
                Operation02::MemoryGuard g(tmp); 
                if (outl != dataLen)
                {
                    // size of deciphered package doesn't match to dataLen
  					ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0222")); // PRQA S 3081
                }
                memset(package, 0, sizeof(package)) ;
                memcpy(package, tmp, outl) ;    // fill package with unciphered data
            }
            else {
                ACS_Operation02_ReadLData(dataLen); // PRQA S 3084
            }
        
            unsigned int pkgpointer = 0;


            // NUMBER OF HW.PARAMETERS
            ACS_Operation02_ReadPackageEND(&outdataparam->hwParamNum, sizeof(outdataparam->hwParamNum), dataIsBEND ) ;  // PRQA S 3030, 3081, 3084, 4090

            // RSVD
            ACS_Operation02_ReadPackage(temp_buffer, (dataLen-pkgpointer)) ; // PRQA S 3084
        }

        // Allocate memory for all HINDataParam structures
        outdataparam->hwInData = reinterpret_cast<HINDataParam*>( calloc(outdataparam->hwParamNum, sizeof(struct HINDataParam))) ; // PRQA S 3081 2
        outdataparam->hwOutData = reinterpret_cast<HOUTDataParam*>( calloc(outdataparam->hwParamNum, sizeof(struct HOUTDataParam))) ;


        // HW PARAMS_IN
        for (unsigned int i=0; i<outdataparam->hwParamNum; i++)
        {
            unsigned int dataLen = (64) ; // uncrypted size
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int packageLen = Operation03::GetCipheredPkgSize(cipherRSA, pubKey, dataLen) ;
                ACS_Operation02_ReadLData(packageLen);  // (temporarily)  fill package with ciphered data // PRQA S 3084
                unsigned int outl = 0;
                unsigned char* tmp = Operation03::DecodePkg(cipherRSA, &outl, pubKey, package, packageLen) ;
                Operation02::MemoryGuard g(tmp); 
                if (outl != dataLen)
                {
                    // size of deciphered package doesn't match to dataLen
  					ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0223")); // PRQA S 3081
                }
                memset(package, 0, sizeof(package)) ;
                memcpy(package, tmp, outl) ;    // fill package with unciphered data
            }
            else {
                ACS_Operation02_ReadLData(dataLen); // PRQA S 3084
            }
        
            unsigned int pkgpointer = 0;


            // Blocking
            ACS_Operation02_ReadPackageEND(& outdataparam->hwInData[i].blocking, sizeof(outdataparam->hwInData[i].blocking), dataIsBEND ) ; // PRQA S 3081, 3084, 4090

            // CMD
            ACS_Operation02_ReadPackageEND(& outdataparam->hwInData[i].cmd, sizeof(outdataparam->hwInData[i].cmd), dataIsBEND ) ;  // PRQA S 3030, 3081, 3084, 4090

            // PARAM
            ACS_Operation02_ReadPackageEND(& outdataparam->hwInData[i].param, sizeof(outdataparam->hwInData[i].param), dataIsBEND ) ; // PRQA S 3030, 3081, 3084, 4090

            // BUFLEN
            ACS_Operation02_ReadPackageEND(& outdataparam->hwInData[i].buflen, sizeof(outdataparam->hwInData[i].buflen), dataIsBEND ) ; // PRQA S 3030, 3081, 3084, 4090

            // BUFFER
            ACS_Operation02_ReadPackage(outdataparam->hwInData[i].buffer, sizeof(outdataparam->hwInData[i].buffer) ) ; // PRQA S 3084

            // RSVD
            ACS_Operation02_ReadPackage(&temp_b, sizeof(temp_b)) ; // PRQA S 3084

        }



        // HW PARAMS_OUT 
        for (unsigned int i=0; i<outdataparam->hwParamNum; i++)
        {
            unsigned int dataLen = (64) ; // uncrypted size
            if (cipherRSA != ACS_SSL::NOCIPHER)
            {
                unsigned int packageLen = Operation03::GetCipheredPkgSize(cipherRSA, pubKey, dataLen) ;
                ACS_Operation02_ReadLData(packageLen);  // (temporarily)  fill package with ciphered data // PRQA S 3084
                unsigned int outl = 0;
                unsigned char* tmp = Operation03::DecodePkg(cipherRSA, &outl, pubKey, package, packageLen) ;
                Operation02::MemoryGuard g(tmp); 
                if (outl != dataLen)
                {
                    // size of deciphered package doesn't match to dataLen
  					ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0225")); // PRQA S 3081
                }
                memset(package, 0, sizeof(package)) ;
                memcpy(package, tmp, outl) ;    // fill package with unciphered data
            }
            else {
                ACS_Operation02_ReadLData(dataLen); // PRQA S 3084
            }
        
            unsigned int pkgpointer = 0;

            // DATA
            ACS_Operation02_ReadPackage(outdataparam->hwOutData[i].data, sizeof(outdataparam->hwOutData[i].data) ) ; // PRQA S 3084

            // RSVD
            ACS_Operation02_ReadPackage(&temp_b, sizeof(temp_b)) ; // PRQA S 3084

        }


    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x020b"));
    }
    
    return ;
}

              
///////////////////////////////////////////////////////////////////////////////////////
// 
//   pack LData to get a valid BLOB (adding also BLOB header)
//
///////////////////////////////////////////////////////////////////////////////////////
unsigned char*                                                      		/* OUT: blobdata */
                Operation02::packLData( unsigned int* outlen,       		/* BLOB size */
                                        const unsigned char* ldata,          /* IN ldata  */
                                        unsigned short ab,          		/* ab parameter */
                                        bool scrambleWithMAC,       	   /* IN: scramble with MAC-Addr. */
                                        ACS_SSL::CipherStyle cipherRSA,	   /* IN: cipher with RSA key */
                                        unsigned int len)           		/* IN: ldata length */ 
{
// EnrCar:
// Due to endianness fast-swapping macros. The code is fully readable

    unsigned char* retBuffer = 0 ;
    
    try
    {
 
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
        #endif

        retBuffer = reinterpret_cast<unsigned char*>( malloc( len+7 ) ) ; // PRQA S 3081, 3084, 4400
		ACS_COND_THROW(!retBuffer,Operation02_InternalError("Operation02::Internal error IE0x020e")); // PRQA S 3081

        // Cipher data
        /////////////////////////////////////////////////////////////
    
        cipherData(retBuffer+7, ldata, ab, scrambleWithMAC, len); // PRQA S 4400
     
     
        // Create BLOB header:
        /////////////////////////////////////////////////////////////
        
        // Add checksum
        ACS_SSL::ACS_CRC16_eval(retBuffer+0, retBuffer+7, 0, len); // PRQA S 4400


        // Add MAC & cipherRSA flags
        unsigned char temp_c = 0x00 ; // reset value
        if (scrambleWithMAC) { temp_c |= 0x01 ; }
        temp_c |= static_cast<unsigned char>(cipherRSA) & 0x30 ;	/* Store cipherRSA */ // PRQA S 3000, 3007, 3010, 3081, 4400
        ACS_ByteSwap_SWAP(&temp_c, sizeof(temp_c), true); // Force BIGENDIAN // PRQA S 3081, 3084, 3260, 4090
        memcpy(retBuffer+2, &temp_c, sizeof(temp_c) ); // PRQA S 4400
        
        // Add data length
        {
            unsigned int temp_i = len ; // Create temp var. for byte swapping
            ACS_ByteSwap_SWAP( &temp_i, sizeof(temp_i), true ); // Force BIGENDIAN// PRQA S 3030, 3081, 3084, 3260, 4090
            memcpy(retBuffer+3, &temp_i, sizeof(temp_i) ); // PRQA S 4400
        }
        
        *outlen = len + 7 ;  // PRQA S 3084, 4400
    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x020d")); // PRQA S 3081
    }
    
    return retBuffer ;
}
     
              
///////////////////////////////////////////////////////////////////////////////////////
// 
//   unpack BLOB (with Header) to get an LData
//
///////////////////////////////////////////////////////////////////////////////////////
unsigned char*																	/* OUT: LData */
                Operation02::unpackLData(   unsigned int* outlen,				/* LData size (should be const)*/
                                            ACS_SSL::CipherStyle* cipheredRSA,	/* OUT: ciphered with RSA */
                                            unsigned char* blobdata,			/* IN blobdata */
                                            unsigned int bloblen,				/* IN blobdata lenght */
                                            unsigned short ab)					/* ab parameter */
{
	// EnrCar:
	// Due to endianness fast-swapping macros. The code is fully readable

    unsigned char* retBuffer = 0 ;
    
    try
    {
 
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called with ab: 0x" << hex << ab << dec 
                    << " bloblen: " << bloblen) ;
        #endif
         
        unsigned int len = 0 ; 
        
        // Get data length
        memcpy(&len, blobdata+3, sizeof(len) ); // PRQA S 4400
        ACS_ByteSwap_SWAP(&len, sizeof(len), true) ; // Force BIGENDIAN // PRQA S 3030, 3081, 3084, 3260, 4090


        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                ACS_LOG_DEBUG(SimpleDebugSignature << "len=" << len) ;
                dumpData_DEBUG(blobdata, len+7) ;   // PRQA S 3084, 4400
            ACS_CLASS_END_DEBUG
        #endif
  
        if (bloblen < (len+7)) // PRQA S 3084, 4400
        {
            // Data buffer (bloblen) smaller than declared content (len+7). Cannot continue
            ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0213")); // PRQA S 3081
        }
  

        retBuffer = reinterpret_cast<unsigned char*>( malloc( len ) ); // PRQA S 3081
		ACS_COND_THROW(!retBuffer,Operation02_InternalError("Operation02::Internal error IE0x0210")); // PRQA S 3081


        // Check BLOB header:
        /////////////////////////////////////////////////////////////
        
        // Evaluate checksum
        unsigned char checkSum[2] ; // PRQA S 4403
        ACS_SSL::ACS_CRC16_eval(checkSum, blobdata+7, 0, len); // PRQA S 4400


        // Compare checksum
        if ( 0 != memcmp(checkSum, blobdata+0, 2) ) // PRQA S 4400
        {
            // BLOB has an invalid checksum
            ACS_THROW(Operation02_InternalError("Operation02::Internal error IE0x0211")); // PRQA S 3081
        }

        // Get MAC flag
        unsigned char temp_c = 0 ;
        memcpy(&temp_c, blobdata+2, sizeof(temp_c) ); // PRQA S 4400
        ACS_ByteSwap_SWAP(&temp_c, sizeof(temp_c), true) ; // Force BIGENDIAN// PRQA S 3081, 3084, 3260, 4090
        bool scrambleWithMAC = (( temp_c & 0x01 ) != 0) ; 
        (*cipheredRSA) = static_cast<ACS_SSL::CipherStyle>( temp_c & 0x30 ); // PRQA S 3013, 3081, 4400


        // Cipher data
        /////////////////////////////////////////////////////////////
    
        cipherData(retBuffer, blobdata+7, ab, scrambleWithMAC, len); // PRQA S 4400
     
     
        *outlen = len ;    
    }
    catch(exception& e)
    {
        ACS_THROW(Operation02_InternalError(e,"Operation02::Internal error IE0x020f")); // PRQA S 3081
    }
    
    return retBuffer ;
}
         
              
///////////////////////////////////////////////////////////////////////////////////////
// 
//   check if BLOB (with BLOB header) is VALID
//
///////////////////////////////////////////////////////////////////////////////////////
bool    // TRUE is good
        Operation02::checkLData(    unsigned char *buffer,          // buffer // PRQA S 4020
                                    size_t bufval,                  // buffer size
                                    const std::string &featmod,     // IN: applicationName
                                    bool dumpData)                  // IN: SET TRUE to dump data (DEBUG ONLY)
{
	// EnrCar ::
	// In order to keep the code legible and aligned to the logical operation performed


	unsigned short ab = ACS_SSL::ACS_CRC16_eval( featmod.c_str(), 0, featmod.length() ) ; // PRQA S 3010
    
#ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called. Feature !" << featmod << "!" 
	    	<< "  length: " << featmod.length() << "  ab: 0x" << hex << ab) ;
#endif

    // Creating blank RSA Key        
    RSA* pubKey = ACS_SSL::ACS_RSA_newkey() ;
    if (!pubKey)
    {
        ACS_THROW(Operation02_InternalError("Cannot create blank RSA key.")); // PRQA S 3081
    }
    ACS_SSL::RSAKeyGuard k(pubKey) ;   // Automatic de-allocating

#ifdef ACS_OPERATION02_CHECKLDATA
    // Filling public key...
    ACS_SSL::ACS_RSA_fill_PubKey(pubKey, ACS_RSA_PublicKey_n, ACS_RSA_PublicKey_e); 
#endif    


    if (bufval<64) { // PRQA S 4400
        return false ;  // size is invalid
    }

    unsigned int ldataoutlen = 0 ;
    ACS_SSL::CipherStyle cipheredRSA ; // PRQA S 4101
	unsigned char* ldataout = unpackLData( &ldataoutlen, &cipheredRSA, buffer, bufval, ab ); // PRQA S 3010
    MemoryGuard g(ldataout) ; // no free is required

    
    bool debug = false ;
    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
            ACS_LOG_DEBUG(SimpleDebugSignature) ;
            dumpData_DEBUG(ldataout, ldataoutlen) ;  	
            debug = true ;
        ACS_CLASS_END_DEBUG
    #endif

    if (dumpData && (!debug)) { dumpData_DEBUG(ldataout, ldataoutlen) ; }
    

    struct LDataParam* lDataParam = reinterpret_cast<LDataParam*>( malloc( sizeof(LDataParam) )) ;  // PRQA S 3081
	ACS_COND_THROW(! lDataParam,Operation02_InternalError("Operation02::Internal error IE0x0212")) ;  // PRQA S 3081

    ResetAndGuardLDataParam rg(lDataParam) ; // free(lDataParam) not required.

    readLData(lDataParam, ldataout, pubKey, cipheredRSA, ldataoutlen );

    debug = false ;
    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
            dumpLDataParam_DEBUG(lDataParam) ; 
            debug = true ;  
        ACS_CLASS_END_DEBUG
    #endif

    if (dumpData && (!debug)) {dumpLDataParam_DEBUG(lDataParam) ; }
 
// W A R N I N G
// Hardware should be re-aligned if rescan == true and dumpData==false

   if (0 != strncmp( reinterpret_cast<char*>(lDataParam->appName), featmod.c_str(), sizeof(lDataParam->appName) )) // PRQA S 3030, 3081
   {
        // Check on appName failed!
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "LData_Appname: " << reinterpret_cast<char*>(lDataParam->appName) // PRQA S 3030, 3081
                    << " doesn't match feature: " << featmod) ;
        #endif
        return false ;
   }

	/*   
    	LICENSE TYPES: (lDataParam->type)
        0x00: Check date and Check HW
        0x01: Check date (HW can change)
        0x10: Check HW (No date expiration)
        0xFF: No check
   	*/

    bool status = true ;
    bool found = false ;

    if (lDataParam->type == 0xff) // PRQA S 4400
    {
        // No check
        found = true ;
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Type 0x" << hex << lDataParam->type << dec
                << " always successfully. GOOD.") ;
        #endif
        status = true ;	/* Always good */
    }

    if ((lDataParam->type == 0x00) || (lDataParam->type == 0x01))
    {
    	/* Check date */
        unsigned int now = time(0) ; // PRQA S 3000, 3010

        found = true ;
        unsigned int toCheck = lDataParam->creationDate + lDataParam->expDays * (24*3600) ;    // PRQA S 3000, 3084, 4400

        bool success = (now <= toCheck) ;
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Type 0x" << hex << lDataParam->type << dec // PRQA S 3380
                << " Checking Date. now: " << now << "\n" 
                << " creationData: " << lDataParam->creationDate << "\n" 
                << " expDays: " << lDataParam->expDays << "\n" 
                << " output: " << (success? "GOOD": "NOT GOOD"));
        #endif

        status &= success ; // PRQA S 3003, 3054
    }
        
        
    if ((lDataParam->type == 0x00) || (lDataParam->type == 0x10)) // PRQA S 4400
    {
        /* Check HW */
#if(0)
        unsigned int toCheck ;
        unsigned int now = time(0) ;

        toCheck = lDataParam->creationDate + lDataParam->expDays * (24*3600) ;   
#endif
        found = true ;
        bool success = checkHWParams(lDataParam) ;
        
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Type 0x" << hex << int(lDataParam->type) << dec // PRQA S 3081, 3380
                << " Checking HW Params. output: " << (success? "GOOD": "NOT GOOD")) ;
        #endif

        status &= success ; // PRQA S 3003, 3054
    }
        
        
    if (!found)   
    {
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Type 0x" << hex << int(lDataParam->type) << dec // PRQA S 3081
                << " UNSUPPORTED. NOT GOOD.") ;
        #endif

		status = false ;	/* Unsupported */
    }

    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning value: " << boolalpha << status) ;
    #endif

	return status ;
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   check HW Params
//
///////////////////////////////////////////////////////////////////////////////////////
bool    // TRUE is good
        Operation02::checkHWParams( struct LDataParam const* ld)
{
	// EnrCar
	// Since it's a check method the indent level is used to logically group parameters
	

	bool status = true ;
	unsigned int nonBlkCounter=0 ;
	
    unsigned int nonBlkThreshold = 2 ;	/* EC:: TODO TO-DO: THIS PARAMETER SHOULD BE CONFIGURABLE */ // PRQA S 4400

    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called") ;
    #endif

	signed int outlen = 0 ;

	for (unsigned int i=0; ( status && (i<ld->hwParamNum) ); i++)
    {    
        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking parameter #" << i << " Blocking? " << int(ld->hwInData[i].blocking) << " Threshold: " << nonBlkThreshold) ; // PRQA S 3081
        #endif


        unsigned char *data = reinterpret_cast<unsigned char*>( HWScan::ScanHW_Scan(&outlen, ld->hwInData[i].cmd, ld->hwInData[i].param, ld->hwInData[i].buffer, ld->hwInData[i].buflen)) ; // PRQA S 3081

		if ( (! data) || (outlen<0) )
        {
            #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << i << "] Invalid return-pointer") ;
            #endif
            
            status = false ;
			continue ;
        }
		else
        {
        	MemoryGuard m(data) ; // no free is required ;
			unsigned char cmpbuffer[sizeof(ld->hwOutData[i].data)]  ;
            memcpy(cmpbuffer, ld->hwOutData[i].data, sizeof(ld->hwOutData[i].data));
			

			#ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
                    printf("\treturned data: 0x");
                    for (int j=0; j<outlen; j++) {
                        printf(" %02x", static_cast<unsigned int>(data[j])); // PRQA S 3081
                    }
                    printf("\n") ;
                    printf("\texpected buffer: 0x");
                    unsigned char *p = ld->hwOutData[i].data ; 
                    for (unsigned int j=0; j<sizeof(ld->hwOutData[i].data); j++) {
                        printf(" %02x", static_cast<unsigned int>(p[j])); // PRQA S 3081
                    }
                    printf("\n") ;
	            ACS_CLASS_END_DEBUG
            #endif

			size_t mn = (size_t(outlen)<sizeof(ld->hwOutData[i].data)) ? outlen : sizeof(ld->hwOutData[i].data) ;	/* Size of common part */ // PRQA S 3000, 3081, 3384
            
            for (size_t j=0; j<sizeof(ld->hwOutData[i].data); j++) 
            {
            	if (j<mn) { cmpbuffer[j] ^= data[j] ; }	/* XOR between expected buffer and HW output */ // PRQA S 3000, 3010
            
            	if (0 != cmpbuffer[j]) 
                {
                	/* Mismatch found */
                
                    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << i << "] Mismatch on byte: " << j) ;
                    #endif
                
                	if ( (0 != ld->hwInData[i].blocking) || (++nonBlkCounter > nonBlkThreshold) ) // PRQA S 3230
                    {
                        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
                            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << i << "] Fatal Error. Blocking? "  // PRQA S 3081
                                << int(ld->hwInData[i].blocking) << " Non-blocking counter: " << nonBlkCounter
                                << " Threshold: " << nonBlkThreshold) ;
                        #endif
              
                	    status = false ;	
					}                
        
                    /* If still here: not blocking and below the threshold */
                    #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
	                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << i << "] Non-blocking counter (so far): " << nonBlkCounter
                            << " Threshold: " << nonBlkThreshold ) ;
                    #endif
        
                    break ;	/* break inner loop */
        
                }
            }	/* for (size_t j=0; j<sizeof(ld->hwOutData[i].data); j++) */
		}	/* else */

        #ifdef ACS_OPERATION02_HAS_DEBUG_ENABLED
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << i << "] Status is: " << boolalpha << status ) ;
        #endif


	}	/* for (unsigned int i=0; i<ld->hwParamNum; i++) */

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Status is: " << boolalpha << status ) ;

	return status ;
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   bufferToBinAscii
//
///////////////////////////////////////////////////////////////////////////////////////

char* 												/* OUT: in case of success: d -- o/w NULL */
		Operation02::bufferToBinAscii(	const unsigned char* s, /* IN: Binary buffer (source) */ // PRQA S 4020
                                        size_t sbuflen, 		/* IN: Buffer size */ 
                                        char* d,				/* OUT: Pointer to a string (already allocated) */ 
                                        size_t dbuflen)			/* IN: Output string size */
{	
	// EnrCar ::
	// In order to keep the complexity low and the code legible

	if (!d) { return 0 ; }
	
	if (!s) { return 0 ; }

	if (sbuflen>(2*dbuflen)) { return 0 ; } // PRQA S 3084, 4400
	
	d[0] = 0 ;

	for (size_t i=0; i<sbuflen; i++)
	{
		char t[3] ; // PRQA S 4403
		sprintf(t, "%02X", int(s[i])) ; // PRQA S 3081
		strcat(d, t) ;
	}


	return d ;
}


///////////////////////////////////////////////////////////////////////////////////////
// 
//   binAsciiToBuffer
//
///////////////////////////////////////////////////////////////////////////////////////
unsigned char* 
				Operation02::binAsciiToBuffer(	const char* s,		/* IN: Pointer to a BinAscii string */ // PRQA S 4020
                                                unsigned char* d, 	/* IN/OUT: Binary buffer (dest.) */
                                                size_t *dbuflen) 	/* IN/OUT:  IN: buffer size
                                                								OUT: how many bytes written into buffer */
{
	// EnrCar ::
	// In order to keep the complexity low and the code legible
	
	
	
	if ((!s) || (!d) || (!dbuflen) ) { return 0 ; }
	
    if ( (strlen(s)/2) > (*dbuflen) ) { return 0 ; } // PRQA S 3084, 4400
    
    if (dbuflen) { *dbuflen = 0; }
    size_t len_s = strlen(s) ;
	for (size_t i=0; i<len_s; i+=2) // PRQA S 3084, 4400
	{
		char t[3]; // PRQA S 4403 

		t[0] = s[i] ; t[1] = s[i+1] ; t[2] = 0 ;	/* A substring for each HEX couple is taken */ // PRQA S 3084, 4400

		if ( (t[0] >= 'a') && (t[0] <= 'f') ) { t[0] -= 0x20 ; } /* from [a-f] to [A-F] */ // PRQA S 4400 2
		if ( (t[1] >= 'a') && (t[1] <= 'f') ) { t[1] -= 0x20 ; } /* from [a-f] to [A-F] */

		int ti = 0;
		sscanf(t, "%02X", &ti) ;	/* From HEX to int decimal (e.g. "4B" to 75, that's 'K') */	
		
		d[i/2] = static_cast<unsigned char>(ti & 0xff);	/* Stored into buffer */ // PRQA S 3000, 3003, 3081, 3084, 4400
		if (dbuflen) { (*dbuflen) ++ ; }
	}
	
	return d ;
}

_ACS_END_NAMESPACE
