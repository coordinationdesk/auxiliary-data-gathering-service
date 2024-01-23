/*
   
    Copyright 1995-2010, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http:  www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ACScpuid module source $

    $Id$

    $Author$

    $Log$
    Revision 2.5  2012/02/07 17:57:43  marpas
    refactoring in progress

    Revision 2.4  2011/06/07 14:18:56  marpas
    minor changes to compile with the new compiler and new env constraints

    Revision 2.3  2010/11/16 13:44:59  enrcar
    EC:: Fixed compiling error due to -fPIC. 64 bit native architectures handled.

    Revision 2.2  2010/11/12 14:00:46  lucio.pulvirenti
    EC:: Unable to compile when -fPIC set. Forced by disabling a feature (it will be added later)

    Revision 2.1  2010/11/11 16:18:54  enrcar
    Cpuid (Intel architecture) feature handled

  
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ACScpuid.h>


/*
	EC:: In case of modifications:
		- Take into account the -fPIC compatibility, i.e. EBX register must be saved before executing ASM
        - T.i.a. the x86_64 compatibility (so far, code is compiled as 64 bit on 64 bit platforms [!!!] )
        - T.i.a. the multi-endianness (or at least endianess-check)
*/


/*******************************************************************************************
**
**  Get signature and feature
**
********************************************************************************************/
int                                             	/*  0x00 = STATUS_GOOD */
    ACScpuid_sign_feat(	unsigned int* signature, 
						unsigned int* feature)
{
	unsigned int eax, ecx, edx ; 
	unsigned int i ;

	i=1 ;
    __asm__ (   
#ifndef __x86_64__
            "pushl	%%ebx		;"			/* Preserve EBX register (-fPIC compatibility) */
#else
            "pushq	%%rbx		;"			/* (as above, but on 64bit arch.s) */
#endif
            "movl	%3,	%%eax	;"
            "cpuid				;"
#ifndef __x86_64__
            "popl	%%ebx		;"			/* Preserve EBX register (-fPIC compatibility) */
#else
            "popq	%%rbx		;"			/* (as above, but on 64bit arch.s) */
#endif
            :"=a"(eax),"=c"(ecx),"=d"(edx)	/* output */
            :"r"(i)         				/* input */
            /* :         					 clobbered register */
        );       

	if (signature) *signature = eax ;
	if (feature) *feature = edx ;

	return 0 ;
}


/*******************************************************************************************
**
**  Check if PSN is supported
**
********************************************************************************************/
int				                    /*  0x00 = STATUS_GOOD 
				                        0x01 = CPUID Not supported / Disabled
				                        0x02 = PSN Disabled
				                        0x03 = "GenuineIntel" Mismatch */
	ACScpuid_is_supported(unsigned char* vendorIdPtr)	/* IN: Either NULL or a uchar[12] */
{
	int status ;
	unsigned int eax, ebx, ecx, edx ;
    unsigned int feature, signature ;
	unsigned int i ;
    unsigned char ptr[12] ;
	const char 
		gen_intel[] = "GenuineIntel" ;


	/* Get feature */
	if ( (status = ACScpuid_sign_feat(&signature, &feature)) )
	{
		return status ;
	}
	
	/* Bit #18 in feature should be set: */
	status = (feature >> 18) & 0x01  ;

	if (!status) return 0x01 ; 

	i=0 ;
        __asm__ (	
#ifndef __x86_64__
            "pushl	%%ebx		;"			/* Preserve EBX register (-fPIC compatibility) */
#else
            "pushq	%%rbx		;"			/* (as above, but on 64bit arch.s) */
#endif
            "movl	%4,	%%eax	;"
            "cpuid				;"
            "movl	%%ebx, %1	;"			/* Save value of EBX in another register */ 
#ifndef __x86_64__
            "popl	%%ebx		;"			/* Preserve EBX register (-fPIC compatibility) */
#else
            "popq	%%rbx		;"			/* (as above, but on 64bit arch.s) */
#endif
             :"=a"(eax),"=r"(ebx),"=c"(ecx),"=d"(edx)   /* output */
             :"r"(i)         							/* input */
             /*:										 clobbered register */
             );       



    if (eax < 3)
    {
		return 0x02 ;	/* CPUID Command #3 (Processor Serial Number) Disabled */ 
	}


    memcpy(ptr+0, &ebx, sizeof(ebx)) ;
    memcpy(ptr+4, &edx, sizeof(edx)) ;
    memcpy(ptr+8, &ecx, sizeof(ecx)) ;
	if (vendorIdPtr) memcpy(vendorIdPtr, ptr, sizeof(ptr)) ;
    

	if ( memcmp(ptr, gen_intel, sizeof(ptr)) )
	{
		return 0x03 ;	/* "GenuineIntel" Mismatch */ 
	}

	return 0 ;
}


/*******************************************************************************************
**
**  Return PSN (if supported)
**
********************************************************************************************/
int				                                    /*  0x00 = STATUS_GOOD */
	ACScpuid_get_psn(unsigned char psn[12])
{
	int status ;
	unsigned int eax, ecx, edx ;
	unsigned int i ;
	unsigned int feature, signature, psn1, psn2, psn3 ;
	unsigned char *tmp, *ptr ;


	/* Get signature and feature */
    if ( (status = ACScpuid_sign_feat(&signature, &feature)) )
    {
        return status ;
    }

	psn1 = signature ;


    /* Check if PSN is supported */
  	if ( (status = ACScpuid_is_supported(0)) )
    {
        return status ;
    }


    /* Get PSN */
    i=3 ;
    __asm__ (   
#ifndef __x86_64__
            "pushl	%%ebx		;"			/* Preserve EBX register (-fPIC compatibility) */
#else
            "pushq	%%rbx		;"			/* (as above, but on 64bit arch.s) */
#endif
            "movl	%3,	%%eax	;"
            "cpuid				;"
#ifndef __x86_64__
            "popl	%%ebx		;"			/* Preserve EBX register (-fPIC compatibility) */
#else
            "popq	%%rbx		;"			/* (as above, but on 64bit arch.s) */
#endif
            :"=a"(eax),"=c"(ecx),"=d"(edx)	/* output */
            :"r"(i)         				/* input */
            /*:         					-  clobbered register */
        );       

	psn2 = edx ; psn3 = ecx ;

#if ( __BYTE_ORDER == __BIG_ENDIAN )
    #warning "ENRCAR:: Intel Architecture AND BIG_ENDIAN Detected. Maybe mistake?"

    /*  Copy psn1 + psn2 + psn3 in the buffer (no endianness re-alignment) */
    memcpy(psn+0, &psn1, 4) ; memcpy(psn+4, &psn2, 4) ; memcpy(psn+8, &psn3, 4) ;
#else

    /*  Copy psn1 + psn2 + psn3 in the buffer, rotating endianness
        (default for Intel Processors, all LE so far) */
	tmp = (unsigned char*)(&psn1) ;
	ptr = (psn+0); ptr[0] = tmp[3] ; ptr[1] = tmp[2] ; ptr[2] = tmp[1] ; ptr[3] = tmp[0] ;

	tmp = (unsigned char*)(&psn2) ;
	ptr = (psn+4); ptr[0] = tmp[3] ; ptr[1] = tmp[2] ; ptr[2] = tmp[1] ; ptr[3] = tmp[0] ;

	tmp = (unsigned char*)(&psn3) ;
	ptr = (psn+8); ptr[0] = tmp[3] ; ptr[1] = tmp[2] ; ptr[2] = tmp[1] ; ptr[3] = tmp[0] ;

#endif

	return 0 ;
}

