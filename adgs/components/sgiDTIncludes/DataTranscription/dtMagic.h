/*------------------------------------------------------------------------------------------------------*/
/* Advanced Computer Systems s.r.l.									*/
/* Via Paolo Frisi 7/a 00197 Roma Italia								*/
/*													*/
/*													*/
/* Header file ...: lsMagic.h										*/
/*													*/
/* Written by B. Versini and V.D. Spaventa								*/
/* Modified by A.Vollono													*/
/* Version 2.0 created on Apr 1995									*/
/*													*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/
/*Purpose:	This file defines all the magic number for the files referenced inside the processing	*/
/*		chain.											*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/

#ifndef _ACS_DTMAGIC_H_
#define _ACS_DTMAGIC_H_

#ifdef __cplusplus
	extern "C" {
#endif

/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const * dtMagicId="@(#) Advanced Computer Systems: dtMagic.h               1.7 01/05/23 (include)";

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MAGIC_DTRAWTM_SONY_ID		    0x12121995
#define				MAGIC_DTRAWMSS_SONY_ID		    0x18081994
#define				MAGIC_DTRAWERS_SONY_ID		    0x08031995
#define				MAGIC_DTRAWERS_SONY_ID_PATCH	    0x25041995
#define				MAGIC_DTRAWERS_SONY_DLR_ID_PATCH    0x20000000
#define				MAGIC_DTRAWJERS_SONY_ID		    0x17091996
#define				MAGIC_DTRAWSPOT_DLT_ID		    0x30071996
#define				MAGIC_DTRAWERS_DLT_ID		    0x28111996
#define				MAGIC_DTRAWMSS_DLT_ID		    0x29111996
#define				MAGIC_DTRAWTM_DLT_ID		    0x30111996
#define				MAGIC_DTRAWJERS_DLT_ID		    0x30071998
#define				MAGIC_DTRAWXSAR_DLT_ID		    0x30012000  /* XSAR */
#define				MAGIC_DTRAWEOS_DLT_ID		    0x23062001  /* EOS */

#define				MAGIC_DTRAWMSS_FILE		    0x00006001
#define				MAGIC_DTRAWTM_FILE		    0x00006002
#define				MAGIC_DTRAWETM_FILE		    0x00006003
#define				MAGIC_DTRAWSWIR_FILE		    0x00006004
#define				MAGIC_DTRAWVNIR_FILE		    0x00006005
#define				MAGIC_DTRAWERS_FILE		    0x00006008
#define				MAGIC_DTRAWERS_FILE_PATCH	    0x00006009
#define				MAGIC_DTRAWSPOT_FILE		    0x0000600a
#define				MAGIC_DTRAWJERS_FILE		    0x0000600b
#define				MAGIC_DTRAWXSAR_FILE		    0x0000600c	/* XSAR */
#define				MAGIC_DTRAWEOS_FILE		    0x0000600d	/* EOS */

#define				MAGIC_DT_STATISTICS_FILE_DLT	    0x00006006
#define				MAGIC_DT_STATISTICS_FILE_SONY	    0x00006007


#ifdef __cplusplus
	}
#endif

#endif





