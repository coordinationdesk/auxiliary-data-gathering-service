/*
 * Copyright 1998, Advanced Computer Systems, Inc.
 * Via Paolo Frisi, 7a Roma
 *
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Advanced Computer Systems, Inc.
 *
 * Author: B. Versini and V.D. Spaventa 
 *
 * $Id$
 *
 */
/*------------------------------------------------------------------------------------------------------*/
/*Purpose:	This file defines all the magic number for the files referenced inside the processing	*/
/*		chain.											*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _lsMagic_h_
#define _lsMagic_h_

 
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char* lsMagicId="@(#) A.C.S. LANDSAT TM/ETM Library: lsMagic.h 1.1 98/07/27";

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MAGIC_MOEPRED_FILE_LANDSAT	    0x00003001

#define				MAGIC_RAWPCD_FILE		    0x00003005
#define				MAGIC_FTDCF_FILE_LANDSAT	    0x00003006

#define				MAGIC_RAWMSS_FILE		    0x00003007
#define				MAGIC_RAWTM_FILE		    0x00003008
#define				MAGIC_RAWETM_FILE		    0x00003009
#define				MAGIC_RAWAUX_FILE		    0x00003010

#define				MAGIC_L1_MOE_FILE		    0x00003011
#define				MAGIC_L2_MOE_FILE		    0x00003012
#define				MAGIC_L3_MOE_FILE		    0x00003013
#define				MAGIC_L4_MOE_FILE		    0x00003014
#define				MAGIC_L5_MOE_FILE		    0x00003015
#define				MAGIC_L6_MOE_FILE		    0x00003016

#define				MAGIC_QLMSS_FILE		    0x00003020
#define				MAGIC_QLTM_FILE			    0x00003021
#define				MAGIC_QLETM_FILE		    0x00003022
#define				MAGIC_QLMSS_FILE_JPEG		    0x00003025
#define				MAGIC_QLTM_FILE_JPEG		    0x00003026
#define				MAGIC_QLETM_FILE_JPEG		    0x00003027

#define				MAGIC_SYNC_GYRO			    0x00003028
#define				MAGIC_SYNC_ADS			    0x00003029




#define				MAGIC_FS_L0_PAN_FILE		    0x00004000
#define				MAGIC_FS_L0_B1_FILE		    0x00004001
#define				MAGIC_FS_L0_B2_FILE		    0x00004002
#define				MAGIC_FS_L0_B3_FILE		    0x00004003
#define				MAGIC_FS_L0_B4_FILE		    0x00004004
#define				MAGIC_FS_L0_B5_FILE		    0x00004005
#define				MAGIC_FS_L0_B6_FILE		    0x00004006
#define				MAGIC_FS_L0_B7_FILE		    0x00004007

#define				MAGIC_QS_L0_PAN_FILE		    0x00004010
#define				MAGIC_QS_L0_B1_FILE		    0x00004011
#define				MAGIC_QS_L0_B2_FILE		    0x00004012
#define				MAGIC_QS_L0_B3_FILE		    0x00004013
#define				MAGIC_QS_L0_B4_FILE		    0x00004014
#define				MAGIC_QS_L0_B5_FILE		    0x00004015
#define				MAGIC_QS_L0_B6_FILE		    0x00004016
#define				MAGIC_QS_L0_B7_FILE		    0x00004017

#define				MAGIC_MS_L0_PAN_FILE		    0x00004020
#define				MAGIC_MS_L0_B1_FILE		    0x00004021
#define				MAGIC_MS_L0_B2_FILE		    0x00004022
#define				MAGIC_MS_L0_B3_FILE		    0x00004023
#define				MAGIC_MS_L0_B4_FILE		    0x00004024
#define				MAGIC_MS_L0_B5_FILE		    0x00004025
#define				MAGIC_MS_L0_B6_FILE		    0x00004026
#define				MAGIC_MS_L0_B7_FILE		    0x00004027




#define				MAGIC_FS_L1_PAN_FILE		    0x00004100
#define				MAGIC_FS_L1_B1_FILE		    0x00004101
#define				MAGIC_FS_L1_B2_FILE		    0x00004102
#define				MAGIC_FS_L1_B3_FILE		    0x00004103
#define				MAGIC_FS_L1_B4_FILE		    0x00004104
#define				MAGIC_FS_L1_B5_FILE		    0x00004105
#define				MAGIC_FS_L1_B6_FILE		    0x00004106
#define				MAGIC_FS_L1_B7_FILE		    0x00004107

#define				MAGIC_QS_L1_PAN_FILE		    0x00004110
#define				MAGIC_QS_L1_B1_FILE		    0x00004111
#define				MAGIC_QS_L1_B2_FILE		    0x00004112
#define				MAGIC_QS_L1_B3_FILE		    0x00004113
#define				MAGIC_QS_L1_B4_FILE		    0x00004114
#define				MAGIC_QS_L1_B5_FILE		    0x00004115
#define				MAGIC_QS_L1_B6_FILE		    0x00004116
#define				MAGIC_QS_L1_B7_FILE		    0x00004117

#define				MAGIC_MS_L1_PAN_FILE		    0x00004120
#define				MAGIC_MS_L1_B1_FILE		    0x00004121
#define				MAGIC_MS_L1_B2_FILE		    0x00004122
#define				MAGIC_MS_L1_B3_FILE		    0x00004123
#define				MAGIC_MS_L1_B4_FILE		    0x00004124
#define				MAGIC_MS_L1_B5_FILE		    0x00004125
#define				MAGIC_MS_L1_B6_FILE		    0x00004126
#define				MAGIC_MS_L1_B7_FILE		    0x00004127




#define				MAGIC_FS_L5_PAN_FILE		    0x00004500
#define				MAGIC_FS_L5_B1_FILE		    0x00004501
#define				MAGIC_FS_L5_B2_FILE		    0x00004502
#define				MAGIC_FS_L5_B3_FILE		    0x00004503
#define				MAGIC_FS_L5_B4_FILE		    0x00004504
#define				MAGIC_FS_L5_B5_FILE		    0x00004505
#define				MAGIC_FS_L5_B6_FILE		    0x00004506
#define				MAGIC_FS_L5_B7_FILE		    0x00004507

#define				MAGIC_QS_L5_PAN_FILE		    0x00004510
#define				MAGIC_QS_L5_B1_FILE		    0x00004511
#define				MAGIC_QS_L5_B2_FILE		    0x00004512
#define				MAGIC_QS_L5_B3_FILE		    0x00004513
#define				MAGIC_QS_L5_B4_FILE		    0x00004514
#define				MAGIC_QS_L5_B5_FILE		    0x00004515
#define				MAGIC_QS_L5_B6_FILE		    0x00004516
#define				MAGIC_QS_L5_B7_FILE		    0x00004517

#define				MAGIC_MS_L5_PAN_FILE		    0x00004520
#define				MAGIC_MS_L5_B1_FILE		    0x00004521
#define				MAGIC_MS_L5_B2_FILE		    0x00004522
#define				MAGIC_MS_L5_B3_FILE		    0x00004523
#define				MAGIC_MS_L5_B4_FILE		    0x00004524
#define				MAGIC_MS_L5_B5_FILE		    0x00004525
#define				MAGIC_MS_L5_B6_FILE		    0x00004526
#define				MAGIC_MS_L5_B7_FILE		    0x00004527

#endif
