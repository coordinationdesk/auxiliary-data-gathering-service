
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
/* 
 *         $Prod: A.C.S. CRC *LUT* Decoder --HEADER FILE: crcmodel.h $
 * 
 *         $Id$
 * 
 *         $Author$
 * 
 *         $Log$
 *         Revision 2.5  2012/11/15 11:53:47  marpas
 *         qa rules
 *
 *         Revision 2.4  2012/02/07 17:54:56  marpas
 *         refactoring
 *
 *         Revision 2.3  2009/03/11 12:55:22  enrcar
 *         EC:: compile-time error fixed
 *
 *         Revision 2.2  2008/07/03 16:39:38  enrcar
 *         EC:: uint32_t is used instead of ulong
 *
 *         Revision 2.1  2006/10/04 15:56:34  enrcar
 *         crcmodel_filedecode method added
 *
 *         Revision 2.0  2006/02/28 08:31:43  marpas
 *         Exception 2.1 I/F adopted
 *
 *         Revision 1.8  2004/12/02 18:53:52  enrcar
 *         header modified
 *
 *         Revision 1.7  2004/02/19 17:22:44  enrcar
 *         Added ifdef to define bool also for sgi
 * 
 *         Revision 1.6  2004/02/19 13:26:07  enrcar
 *         Modified for SGI compatibility
 * 
 *         Revision 1.5  2003/07/18 15:00:49  enrcar
 *         bool revised
 * 
 *         Revision 1.4  2002/11/12 08:29:46  enrcar
 *         Added a C++ like costructor/destructor
 * 
 *         Revision 1.3  2002/11/11 17:44:31  enrcar
 *         Tags rewritten
 * 
 * 
 */


#ifndef _crcmodel_H
#define _crcmodel_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <stdint.h>


#ifdef __cplusplus
    extern "C" {
#endif


#ifndef TRUE
    #define FALSE 0
    #define TRUE  1
#endif

#ifndef __cplusplus
    typedef unsigned char bool;
#ifndef false
    /* Artificial bool type for C */
    #define	false	0
    #define	true	1
#endif
#else
#ifdef sgi
    typedef unsigned char bool;
#endif
#endif

/******************************************************************************/

typedef struct
{
    int   cm_width;   /* Parameter: Width in bits [8,32].       */
    uint32_t cm_poly;    /* Parameter: The algorithm's polynomial. */
    uint32_t cm_init;    /* Parameter: Initial register value.     */
    bool  cm_refin;   /* Parameter: Reflect input bytes?        */
    bool  cm_refot;   /* Parameter: Reflect output CRC?         */
    uint32_t cm_xorot;   /* Parameter: XOR this to output CRC.     */
    uint32_t cm_reg;     /* Context: Context during execution.     */
/* PRQA S 4403 2*/
    uint32_t ptable[256];/* LUT Table */
} crcmodel ;

/******************************************************************************/

/* Functions That Implement The Model */
/* ---------------------------------- */

void crc_setup(crcmodel* p_cm, int width, uint32_t poly, uint32_t init, bool refin, bool refot, uint32_t xorot);
/* Setup CRC parameters */

void cm_ini (crcmodel* p_cm);
/* Initializes the argument CRC model instance.          */
/* All parameter fields must be set before calling this. */

void cm_nxt (crcmodel* p_cm,int ch);
/* Processes a single message byte [0,255]. */

void cm_blk (crcmodel* p_cm,const unsigned char * blk_adr,uint32_t blk_len);
/* Processes a block of message bytes. */

uint32_t cm_crc (const crcmodel* p_cm);
/* Returns the CRC value for the message bytes processed so far. */

crcmodel* crcmodel_construct(void);
/* Structure constructor */

void crcmodel_destruct(crcmodel* p_cm);
/* Structure destructor */

signed int crcmodel_filedecode(crcmodel* p_cm, const char* fileName, signed long long int expectedSize) ;


/******************************************************************************/

/* Functions For Table Calculation */
/* ------------------------------- */
/* The following function can be used to calculate a CRC lookup table.        */

uint32_t cm_tab (const crcmodel* p_cm,int index);

/* Returns the i'th entry for the lookup table for the specified algorithm.   */
/* The function examines the fields cm_width, cm_poly, cm_refin, and the      */
/* argument table index in the range [0,255] and returns the table entry in   */
/* the bottom cm_width bytes of the return value.                             */

/******************************************************************************/

#ifdef __cplusplus
	}
#endif

/* End of the header file */
#endif	/* crcmodel */

/******************************************************************************/
/*                             End of crcmodel.h                              */
/******************************************************************************/
