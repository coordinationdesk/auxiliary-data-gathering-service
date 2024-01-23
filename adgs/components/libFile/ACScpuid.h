// PRQA S 1050 EOF
/*
   
   Copyright 1995-2010, Advanced Computer Systems , Inc.
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http:  www.acsys.it

   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc.

   $Prod: A.C.S. ACScpuid module HEADER FILE $

   $Id$

   $Author$

   $Log$
   Revision 2.4  2012/11/29 19:31:29  marpas
   implementing qa rules
   optimizing code and coding best practices
   coverage framework initiated

   Revision 2.3  2011/06/07 14:18:56  marpas
   minor changes to compile with the new compiler and new env constraints

   Revision 2.2  2010/11/16 13:44:59  enrcar
   EC:: Fixed compiling error due to -fPIC. 64 bit native architectures handled.

   Revision 2.1  2010/11/11 16:18:54  enrcar
   Cpuid (Intel architecture) feature handled

  
*/



#ifndef _ACScpuid_H_
#define _ACScpuid_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int                                				 /* 0x00 = STATUS_GOOD */
	ACScpuid_get_psn(unsigned char psn[12]) ;       // PRQA S 4403


int												/*  0x00 = STATUS_GOOD
                                                    0x01 = CPUID Not supported / Disabled
                                                    0x02 = PSN Disabled
                                                    0x03 = "GenuineIntel" Mismatch          */
	ACScpuid_is_supported(unsigned char* vendorIdPtr) ;	/* IN: Either NULL or a uchar[12] */ 


int												/* 0x00 = STATUS_GOOD */
    ACScpuid_sign_feat(	unsigned int* signature, 
    					unsigned int* feature) ;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*  _ACScpuid_H_ */
