/*
 *         Copyright 1995-2001, Advanced Computer Systems , Inc.
 *         Via Della Bufalotta, 378 - 00139 Roma - Italy
 *         http://www.acsys.it
 * 
 *         All Rights Reserved.
 * 
 *         This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *         the contents of this file may not be disclosed to third parties, copied or
 *         duplicated in any form, in whole or in part, without the prior written
 *         permission of Advanced Computer Systems, Inc.
 * 
 *         $Prod: A.C.S. CRC *LUT* Decoder crcmodel.C $
 * 
 *         $Id$
 * 
 *         $Author$
 * 
 *         $Log$
 *         Revision 2.2  2008/07/03 16:39:38  enrcar
 *         EC:: uint32_t is used instead of ulong
 *
 *         Revision 2.1  2006/10/04 15:56:34  enrcar
 *         crcmodel_filedecode method added
 *
 *         Revision 2.0  2006/02/28 08:31:43  marpas
 *         Exception 2.1 I/F adopted
 *
 *         Revision 1.3  2004/12/02 18:53:52  enrcar
 *         header modified
 *
 *         Revision 1.2  2004/02/19 13:26:07  enrcar
 *         Modified for SGI compatibility
 * 
 *         Revision 1.1  2003/07/18 15:00:00  enrcar
 *         renamed - was crcmodel.C
 * 
 *         Revision 1.5  2003/07/18 09:00:51  enrcar
 *         Add versioning
 * 
 *         Revision 1.4  2002/11/12 08:29:46  enrcar
 *         Added a C++ like costructor/destructor
 * 
 *         Revision 1.3  2002/11/11 17:44:31  enrcar
 *         Tags rewritten
 * 
 */


/******************************************************************************/

#include <crcmodel.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#ifndef sgi
    /* version support: */
    #include <crcV.h>

    const char *crcTag() { return crcVbindTag() ; }
    const char *crcName() { return crcVbindName() ; }
#endif

/******************************************************************************/

#define BITMASK(X) (1L << (X))
#define MASK32 0xFFFFFFFFL

/******************************************************************************/


static uint32_t reflect (uint32_t v,int b)
{
	int i;
    uint32_t t = v;
    for (i=0; i<b; i++)
    {
        if (t & 1L)
            v|=  BITMASK((b-1)-i);
        else
            v&= ~BITMASK((b-1)-i);
        t>>=1;
    }
    return v;
}

/******************************************************************************/

static uint32_t widmask (crcmodel* p_cm)
{
    /* Returns a longword whose value is (2^p_cm->cm_width)-1.     */
    /* The trick is to do this portably (e.g. without doing <<32). */
    return (((1L<<(p_cm->cm_width-1))-1L)<<1)|1L;
}


/******************************************************************************/
/* Structure is initialised                                                   */
/******************************************************************************/
void crc_setup(	crcmodel* p_cm,
				int width,	/* Poly WIDTH (in bit, usually 16 or 32) */
                uint32_t poly, /* Poly, usually in the form 0x0000AAAA or 0xBBBBBBBB */
                uint32_t init,	/* INIT pattern, usually in the form 0x0000AAAA or 0xBBBBBBBB */
                bool refin,	/* Reflect INPUT? (TRUE/FALSE) */
                bool refot, /* Reflect OUTPUT? (TRUE/FALSE) */
                uint32_t xorot)/* XOR output pattern, usually in the form 0x0000AAAA or 0xBBBBBBBB */
{
	p_cm->cm_width = width ;
	p_cm->cm_poly  = poly ;
	p_cm->cm_init  = init ;
	p_cm->cm_refin = refin ;
	p_cm->cm_refot = refot ;
	p_cm->cm_xorot = xorot ;

	return ;
}

 
/******************************************************************************/
/* Set the internal CRC register with the INIT pattern                        */
/******************************************************************************/
void cm_ini (crcmodel* p_cm)
{
	p_cm->cm_reg = p_cm->cm_init;
}


/******************************************************************************/
/* CRC is calculated from a single-byte input data.                           */
/* Use cm_crc(.) to extract the CRC value                                     */
/* See also cm_blk(.)                                                         */
/******************************************************************************/
void cm_nxt (crcmodel* p_cm, int ch)
{
 int   i;
 uint32_t uch  = (uint32_t) ch;
 uint32_t topbit = BITMASK(p_cm->cm_width-1);

 if (p_cm->cm_refin) uch = reflect(uch,8);
 p_cm->cm_reg ^= (uch << (p_cm->cm_width-8));
 for (i=0; i<8; i++)
   {
    if (p_cm->cm_reg & topbit)
       p_cm->cm_reg = (p_cm->cm_reg << 1) ^ p_cm->cm_poly;
    else
       p_cm->cm_reg <<= 1;
    p_cm->cm_reg &= widmask(p_cm);
   }
}


/******************************************************************************/
/* CRC is calculated from input data.                                         */
/* Use cm_crc(.) to extract the CRC value                                     */
/* See also cm_nxt(.)                                                         */
/******************************************************************************/
void cm_blk (crcmodel* p_cm,              /* data structure */
			const unsigned char * blk_adr,	/* first input byte address */
            uint32_t  blk_len)		     /* No. of bytes to process */
{
	while (blk_len--) cm_nxt(p_cm,*blk_adr++);
}


/******************************************************************************/
/* Extract CRC value                                                          */
/* YOU MUST RUN cm_nxt(.) *BEFORE* getting this value!!!!!!!!!                */
/******************************************************************************/
uint32_t cm_crc (crcmodel* p_cm)
{
 if (p_cm->cm_refot)
    return p_cm->cm_xorot ^ reflect(p_cm->cm_reg,p_cm->cm_width);
 else
    return p_cm->cm_xorot ^ p_cm->cm_reg;
}


/******************************************************************************/
/* The following function can be used to calculate a CRC lookup table.        */
/******************************************************************************/
uint32_t cm_tab (crcmodel* p_cm, int index)
{
    int   i;
    uint32_t r;
    uint32_t topbit = BITMASK(p_cm->cm_width-1);
    uint32_t inbyte = (uint32_t) index;

    if (p_cm->cm_refin) inbyte = reflect(inbyte,8);
    r = inbyte << (p_cm->cm_width-8);
    for (i=0; i<8; i++)
    if (r & topbit)
       r = (r << 1) ^ p_cm->cm_poly;
    else
       r<<=1;
    if (p_cm->cm_refin) r = reflect(r,p_cm->cm_width);
    return r & widmask(p_cm);
}


/******************************************************************************/
/* Structure constructor                                                      */
/* (Analogy to C++)                                                           */
/******************************************************************************/
crcmodel* crcmodel_construct()
{
    return (crcmodel*)malloc(sizeof(crcmodel));
}


/******************************************************************************/
/* Structure destructor                                                       */
/* (Analogy to C++)                                                           */
/******************************************************************************/
void crcmodel_destruct(crcmodel* p_cm)
{
	free(p_cm);
    return ;
}


/******************************************************************************/
/*                                                                            */
/* CRC is calculated over given filename                                      */
/* Output value: 0 -> CRC evaluated successfully, -1 otherwise                */
/*                                                                            */
/* File size could be checked (if size is passed); use -1 to skip check       */
/*                                                                            */
/* NOTE: use cm_crc AFTER THIS CALL, to GET CRC                           */
/*                                                                            */
/******************************************************************************/
signed int 										/* OUTPUT value:  0:NO ERRORS, -1:ERRORS */
	crcmodel_filedecode(	crcmodel* p_cm,			/* data structure */
                            const char* fileName,	/* File name (full path) */	
                            signed long long int expectedSize)		/* Expected size, or -1 to skip check */
{
	int fd ;
    size_t r ;
    unsigned char buffer[1024] ;
    signed long long int actualSize ;
    
	if ( (!fileName) || (!fileName[0]) )
	{
    	/* Filename is empty! */
        return -5 ;
    }    



	fd = open(fileName, O_RDONLY) ;
    if (fd<0) return -1 ;	/* Bad file */


	/*
    	RESET CRC BUFFER
    */
    cm_ini(p_cm) ;


	/*
    	SCAN FILE
    */
    actualSize = 0 ;
	do
    {
    	r = read(fd, buffer, sizeof(buffer)) ;
        if (r<=0) break ;

        actualSize += (signed long long int)r ;
        cm_blk(p_cm, buffer, (uint32_t)r) ;
    
    } while (r>0) ;

	close(fd) ;
    
    
    if ( (expectedSize>=0) && (expectedSize!=actualSize))
    {
    	/* Wrong file size */
    	return -2 ;
    }
    
    
    return 0 ;
}


/******************************************************************************/
/*                             End of crcmodel.c                              */
/******************************************************************************/
