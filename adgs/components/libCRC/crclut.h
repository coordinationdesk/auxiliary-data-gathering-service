/*
 * 
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
 *         $Prod: A.C.S. CRC *LUT* Decoder --HEADER FILE: crclut.h $
 * 
 *         $Id$
 * 
 *         $Author$
 * 
 *         $Log$
 *         Revision 2.1  2006/10/04 15:54:55  enrcar
 *         global path used for includes
 *
 *         Revision 2.0  2006/02/28 08:31:43  marpas
 *         Exception 2.1 I/F adopted
 *
 *         Revision 1.7  2004/12/02 18:53:52  enrcar
 *         header modified
 *
 *         Revision 1.6  2002/12/03 14:42:34  enrcar
 *         changed crc_lutcheck and crc_lutdecode arguments in order to avoid a typedef
 * 
 *         Revision 1.5  2002/12/03 14:25:33  enrcar
 *         Added a const in argument of crc_lutcheck() and crc_lutdecode()
 * 
 *         Revision 1.4  2002/11/12 08:29:46  enrcar
 *         Added a C++ like costructor/destructor
 * 
 *         Revision 1.3  2002/11/11 17:44:31  enrcar
 *         Tags rewritten
 * 
 */


#ifndef _CRCLUT_H
#define _CRCLUT_H

#ifdef __cplusplus
	  extern "C" {
#endif

#include <crcmodel.h>


void crc_lutbyte (crcmodel* p_cm, int ch) ;
uint32_t crc_lutget(crcmodel* p_cm);
void crc_lutgentable(crcmodel* p_cm);
void crc_lutdecode(crcmodel* p_cm, const unsigned char* blk_adr, uint32_t  blk_len);
int crc_lutcheck(crcmodel* p_cm, const unsigned char* blk_adr, uint32_t  blk_len);

#ifdef __cplusplus
	}
#endif


/* End Header */
#endif /* CRCLUT_H */
