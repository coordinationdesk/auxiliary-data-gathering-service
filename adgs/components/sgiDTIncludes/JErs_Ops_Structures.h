/*

	Copyright 1995-2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$
	
	$Log$

*/



/*
 *	This include file contains all the definitions for the processing of the
 *	Japanes Earth Resources Satellite OPTICAL sensors that are on the
 *	National Space Development Agency of Japan (NASDA) JERS mission 1
 *
 *	This include file defines structures used to transcribe JERS OPS data on DLT.
 *	It includes also structures of Sony D1 transcribed data. 
 *	They are used to convert from D1 to DLT 
 */ 

#ifndef _JErs_Ops_Structures_h_
#define _JErs_Ops_Structures_h_
 

#define JersOps_bands	4				/* Number of Jers spectral bands on the VNIR/SWIR sensors	*/
#define JersOps_MinFrameSize	    96  /* Number of pixels of all bands in a Minor Frame		*/   
#define JersOps_HalfMinFrameSize    48  /* Half Number of pixels of all bands in a Minor Frame		*/   

#define Jers_sensors	4096			/* Number of Jers sensor per bands		*/
#define Jers_line_size	4096			/* Jers Ops line length				*/
#define	Jers_tlm_rate	2048			/* Telemetry bits per seconds			*/
#define	Jers_tlm_swath	346				/* Oversampled telemetry bits per scan line	*/
#define Jers_lines	16					/* Jers Ops line per virtual swath (Major Frame)*/
#define JersOps_PackBlock   8			/* Jers Ops Major Frames in a Block on Dlt (2.2 Mbytes)	*/ 
#define JersOps_Fr_Output_Buffers (JersOps_PackBlock*10)	/* Buffers allocated during reading from Dlt */

#define	Jers_Scan_Line	3.46			/* Line Scan Time in Millisecond		*/
#define Jers_Scan_Time	55360			/* Swath Scan Time in Millisecond * 1000	*/
#define MAX_Jers_Scan_Time 83040		/* Swath Scan Time * 1.5 in Millisecond * 1000	*/
#define MIN_Jers_Scan_Time 27680		/* Swath Scan Time * 0.5 in Millisecond * 1000	*/

#define	Jers_ifov_x	.00003220 			/* Istantaneous field of view in x direction	*/
#define	Jers_ifov_y	.00004231 			/* Istantaneous field of view in y direction	*/

#define Swir_FR_lines	232				/* Jers Ops Full Resolution line for Swir	*/
#define Vnir_FR_lines	212				/* Jers Ops Full Resolution line for Vnir	*/
#define Vnir_FR_margin	430				/* Jers Ops Full Resolution margin for Vnir	*/
#define	Ops_FR_lines	3072			/* Raw Lines Number per Full Resolution Frame	*/
#define Msec_for_Corr	4000.			/* Milliseconds of Telemetry for Correlation	*/
#define Jers_DT_swath	1				/* Number of swath  to load to ID1				*/
#define Jers_DT_files	5				/* Number of files  to load to ID1				*/

#define JersOps_MnFperLine  173	    	/* Number of Minor Frames in a Line		*/

#define JersOps_MNFSync	    0x05B8  	/* 11 bits of Minor Frame sync		*/
#define JersOps_MNFSync_inv 0x00ED  	/* 11 bits of Minor Frame sync in reverse	*/

#define JersOps_LINESYNC1   0x3E
#define JersOps_LINESYNC2   0x2F
#define JersOps_LINESYNC3   0x0C
#define JersOps_LINESYNC4   0x34
#define JersOps_LINESYNC5   0x00
#define JersOps_LINESYNC1_inv   0x00
#define JersOps_LINESYNC2_inv   0x0B
#define JersOps_LINESYNC3_inv   0x0C
#define JersOps_LINESYNC4_inv   0x3D
#define JersOps_LINESYNC5_inv   0x1F

#define JersOps_BLANK	    0x2A

#define Jers_QL_size	1024	/* Jers Ops Quick Look line length		*/
#define Swir_QL_lines	256	/* Jers Ops Quick Look line for Swir alignement	*/
#define Vnir_QL_lines	256	/* Jers Ops Quick Look line for Vnir		*/
#define Ops_QL_lines	256	/* Jers Ops Quick Look line for Vnir & Swir	*/
#define Jers_QL_sensors	4	/* Jers Ops Quick Look sensor per swath		*/
#define Jers_QL_sample	4	/* Jers Ops Quick Look sub sampling		*/
#define Jers_QL_lines	4	/* Jers Ops Quick Look lines per swath		*/
#define Vnir_QL_red		1	/* Jers Ops spectral band for red channel	*/
#define Vnir_QL_green	3	/* Jers Ops spectral band for green channel	*/
#define Vnir_QL_blue	2	/* Jers Ops spectral band for blue channel	*/
#define Swir_QL_red		4	/* Jers Ops spectral band for red channel	*/
#define Swir_QL_green	3	/* Jers Ops spectral band for green channel	*/
#define Swir_QL_blue	2	/* Jers Ops spectral band for blue channel	*/
#define	Vnir_B1_Gain	4.	/* Vnir Band 1 Gain for Quick Look		*/
#define	Vnir_B2_Gain	4.	/* Vnir Band 1 Gain for Quick Look		*/
#define	Vnir_B3_Gain	4.	/* Vnir Band 1 Gain for Quick Look		*/

#define JERSVNIR_TRANSCRIPTION_FILES 5

#warning "PARTIALLY PORTED" 
#ifdef ENRICO
/*
 *	RGB pixel basic format structure 
 */
	typedef struct Jers_Rgb_24__
	{
		unsigned char
      				blue_color,
      				green_color,
				red_color;
	} Jers_Rgb_24;

/*
 *	RGB output video line structure 
 */
	typedef struct Jers_Video_Rgb_24__
	{
		Jers_Rgb_24
				video_buffer[Jers_QL_size];
	} Jers_Video_Rgb_24;

#endif


/*
 *	Jers Raw Telemetry Structure
 */
	typedef struct Jers_Telemetry__
	{
		unsigned char
				Telemetry_Buffer[Jers_lines][Jers_tlm_swath];
	} Jers_Telemetry;		


#ifdef ENRICO
/*
 *	Jers Ops Vnir Full resolution structures 
 */
	typedef struct Jers_Vnir_Full_Resolution__
	{
		unsigned char
				Jers_Vnir_data[JersOps_bands][Jers_lines][Jers_line_size];
		Jers_Telemetry
				Telemetry;
		unsigned short
				Sync_Flag[4];
		unsigned int
				Swath_Time[4];
	} Jers_Vnir_Full_Resolution;

#endif


/*
 *	Jers Ops Vnir Full Resolution Structures on Disk
 */
	typedef struct Jers_Vnir_Full_Resolution_on_Disk__
	{
		unsigned char
				Jers_Vnir_data[JersOps_bands][Jers_lines][Jers_line_size];
		unsigned short
				Sync_Flag[4];
		unsigned int
				Swath_Time[4];
	} Jers_Vnir_Full_Resolution_on_Disk;


#ifdef ENRICO
/*
 *	Jers Ops Ops Quick Look structures 
 */
	typedef struct Jers_Ops_Quick_Look__
	{
		unsigned char
				red_channel[Jers_QL_sensors][Jers_QL_size],
      				green_channel[Jers_QL_sensors][Jers_QL_size],
      				blue_channel[Jers_QL_sensors][Jers_QL_size];
		unsigned short
				Sync_Flag[4];
		unsigned short
				Swath_Time[8];
	} Jers_Ops_Quick_Look;

/*
 *	Jers Ops Quick Look Structure for Ops
 */
	typedef struct Jers_Ops_Quick_Look_Line__
	{
		unsigned int
				Satellite_Time[2],
				Delta_Time;
		Jers_Rgb_24
				video[Jers_QL_size];
	} Jers_Ops_Quick_Look_Line;


/*
 *	Structure for the Description of a Track on Maps
 */
	typedef struct	{
	    double
		Lat_0,		    /* Latitude of the west most point of the scan		    */
		Long_0,		    /* Longitude of the west most point of the scan		    */
		Lat_1,		    /* Latitude of the aest most point of the scan		    */
		Long_1, 	    /* Longitude of the west most point of the scan		    */
		Center_Frame_T;	    /* Time of the Center Frame					    */
	} Jers_Track_Descriptor;

#endif

/************************************************************
 *		JERS OPS Structures on DLT		    *
 ************************************************************/
/*
 *  Jers Ops Auxiliary Structure for a Major Frame
 */
	typedef struct Jers_Ops_Auxiliary__
	{
	    u_int
		Day, 
		Hours, 
		Minutes, 
		Seconds, 
		Milliseconds, 
		spare;
		
	    double
		Satellite_Time;
	    
	} Jers_Ops_Auxiliary;
	

/*
 *  Jers Ops Minor Frame Structure
 */
	typedef struct Jers_Ops_Minor_Frame__
	{
	    u_char
			MNFS[2], 
			TLM1, 
			Video_Part1[JersOps_HalfMinFrameSize], 
			MNFS_inv[2], 
			TLM2, 
			Video_Part2[JersOps_HalfMinFrameSize];
		
	} Jers_Ops_Minor_Frame;
	
	
/*
 *  Jers Ops Line Structure
 */
	typedef struct Jers_Ops_Line__
	{
	    Jers_Ops_Minor_Frame
			Minor_Frame[JersOps_MnFperLine];
		
	} Jers_Ops_Line;
	
	
/*
 *  Jers Ops Major Frame Structure
 */
	typedef struct Jers_Ops_Major_Frame__
	{
	    Jers_Ops_Auxiliary
			Auxiliary;
	    
	    Jers_Ops_Line
			Line[Jers_lines];
		
	} Jers_Ops_Major_Frame; 

#endif /* _JErs_Ops_Structures_h_ */
