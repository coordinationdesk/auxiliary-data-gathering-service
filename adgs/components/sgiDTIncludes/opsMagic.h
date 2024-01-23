/*------------------------------------------------------------------------------------------------------*/
/* Advanced Computer Systems s.r.l.									*/
/* Via Paolo Frisi 7/a 00197 Roma Italia								*/
/*													*/
/*													*/
/* Header file ...: jers1_magic.h									*/
/*													*/
/* Written by B. Versini										*/
/*													*/
/* Version 1.0 created on Febrary 1993									*/
/*													*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/
/*Purpose:	This file defines all the magic number for the files referenced inside the processing	*/
/*		chain.											*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MAGIC_ELMF_FILE			    0x454c4d46
#define				MAGIC_MOEDET_FILE		    0x00002000
#define				MAGIC_MOEPRED_FILE		    0x00002001

#define				MAGIC_TIMECORR_FILE		    0x00002004

#define				MAGIC_RAWSWIR_FILE		    0x00002005
#define				MAGIC_RAWVNIR_FILE		    0x00002006
#define				MAGIC_RAWTLM_FILE		    0x00002007

#define				MAGIC_QLSWIR_FILE		    0x00002010
#define				MAGIC_QLVNIR_FILE		    0x00002011
#define				MAGIC_QLCLOUDCOVER_FILE		    0x00002012
#define				MAGIC_QLSWIR_FILE_JPEG		    0x00002013
#define				MAGIC_QLVNIR_FILE_JPEG		    0x00002014

#define				MAGIC_ARCHIVE_FILE		    0x00002020
#define				MAGIC_QLARCHIVE_FILE		    0x00002021

#define				MAGIC_VNIR_L0_B1_FILE		    0x00001000
#define				MAGIC_VNIR_L0_B2_FILE		    0x00001001
#define				MAGIC_VNIR_L0_B3_FILE		    0x00001002
#define				MAGIC_VNIR_L0_B4_FILE		    0x00001003

#define				MAGIC_VNIR_L1_B1_FILE		    0x00001010
#define				MAGIC_VNIR_L1_B2_FILE		    0x00001011
#define				MAGIC_VNIR_L1_B3_FILE		    0x00001012
#define				MAGIC_VNIR_L1_B4_FILE		    0x00001013

#define				MAGIC_VNIR_L2_B1_FILE		    0x00001020
#define				MAGIC_VNIR_L2_B2_FILE		    0x00001021
#define				MAGIC_VNIR_L2_B3_FILE		    0x00001022
#define				MAGIC_VNIR_L2_B4_FILE		    0x00001023

#define				MAGIC_SWIR_L0_B5_FILE		    0x00001100
#define				MAGIC_SWIR_L0_B6_FILE		    0x00001101
#define				MAGIC_SWIR_L0_B7_FILE		    0x00001102
#define				MAGIC_SWIR_L0_B8_FILE		    0x00001103

#define				MAGIC_SWIR_L1_B5_FILE		    0x00001110
#define				MAGIC_SWIR_L1_B6_FILE		    0x00001111
#define				MAGIC_SWIR_L1_B7_FILE		    0x00001112
#define				MAGIC_SWIR_L1_B8_FILE		    0x00001113

#define				MAGIC_SWIR_L2_B5_FILE		    0x00001120
#define				MAGIC_SWIR_L2_B6_FILE		    0x00001121
#define				MAGIC_SWIR_L2_B7_FILE		    0x00001122
#define				MAGIC_SWIR_L2_B8_FILE		    0x00001123






