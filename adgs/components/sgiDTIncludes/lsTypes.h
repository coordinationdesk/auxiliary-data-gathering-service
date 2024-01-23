/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
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
	Revision 1.12  2018/06/08 12:58:56  ricfer
	QAflags updated.
	
	Revision 1.11  2017/01/03 10:29:20  ricfer
	QAflags added (SLAP CCN3).
	
	Revision 1.10  2013/06/18 10:08:31  ricfer
	Closing IDEAS-PR-13-0568
	
	Revision 1.9  2013/03/18 10:08:57  ricfer
	RawVideoS added.
	
	Revision 1.8  2012/12/04 08:35:24  ricfer
	MissionKind updated.
	
	Revision 1.7  2012/10/07 16:15:07  ricfer
	Time field added to PcdHkTlm.
	
	Revision 1.6  2012/09/20 06:32:28  ricfer
	LandCalibInfo structure (in place of CalibInfoL7) and HKTYPE enum added.
	
	Revision 1.5  2007/09/24 07:32:01  aleber
	definitions overlapping or conflicting with mssTypes.h and MSS_Structures.h put under ifndef directive
	
	Revision 1.4  2007/09/11 12:42:20  giusy.sinatra
	Useless include removed
	
	Revision 1.3  2007/09/06 10:16:45  enrcar
	EC:: Padding added
	
	Revision 1.2  2007/09/04 15:40:54  giucas
	lsError removed
	
	Revision 1.1  2007/09/03 13:49:20  giucas
	Moved from libMDPSLandsatProc
	
	Revision 1.2  2007/07/26 15:25:19  giusy.sinatra
	Update due to endianess problem
	
	Revision 1.1.1.1  2006/10/27 12:17:19  valspa
	VDS: first porting of the MDPS SGI Landsat to new LINUX environment
	

*/

/*------------------------------------------------------------------------------------------------------*/
/*Purpose:	This file contains the main structures and variables used all over the landsat		*/
/*		library.										*/
/*													*/
/*		List of the enum:									*/
/*		    - BERFlag;										*/
/*		    - BitFlag;										*/
/*		    - CalOpMode;									*/
/*		    - DataArrangement;									*/
/*		    - DataValidation;									*/
/*		    - DetectorStatus;									*/
/*		    - EOSFlag;										*/
/*		    - HalfByte;										*/
/*		    - KindofBand;									*/
/*		    - HkConvFactorType;									*/
/*		    - MJFFlag;										*/
/*		    - MissionKind;									*/
/*		    - MnfStatus;									*/
/*		    - OpMode;										*/
/*		    - PcdMnfCounterStatus;								*/
/*		    - PcdUtility;									*/
/*		    - ScanDirection;									*/
/*		    - ScanMirrorEl;									*/
/*		    - TmHKFlag;										*/
/*		    - TlmFlag;										*/
/*													*/
/*													*/
/*		List of the structures:									*/
/*		    - Attitude;										*/
/*		    - Auxiliary;									*/
/*		    - CalibInfo;									*/
/*		    - CeosTime;										*/
/*		    - ChkAuxiliary;									*/
/*		    - Cycle;										*/
/*		    - DegMin;										*/
/*		    - DummyAuxBuffer;									*/
/*		    - DummyBuffer;									*/
/*		    - EOSType;										*/
/*		    - EtmRawVideo;									*/
/*		    - ImgRecFS;										*/
/*		    - ImgRecMS;										*/
/*		    - ImgRecQS;										*/
/*		    - ImgRecMC;										*/
/*		    - ImgRecFSPan;									*/
/*		    - ImgRecMSPan;									*/
/*		    - ImgRecQSPan;									*/
/*		    - ImgRecMCPan;									*/
/*		    - ImgRecInfo;									*/
/*		    - ImgRecPrefix;									*/
/*		    - ImgRecSuffix;									*/
/*		    - IntervalHdrRec;									*/
/*		    - Mjf;										*/
/*		    - MprTapeInfo;									*/
/*		    - PcdAds;										*/
/*		    - PcdAdsTemp;									*/
/*		    - PcdAttitude;									*/
/*		    - PcdEphemeris;									*/
/*		    - PcdGyro;										*/
/*		    - PcdGyroDrift;									*/
/*		    - PcdEtmHkTlm;									*/
/*		    - PcdHkTlm;										*/
/*		    - PcdMnf;										*/
/*		    - PcdMjfQuality;									*/
/*		    - PcdTime;										*/
/*		    - PcdSVClkUpdate;									*/
/*		    - PcdTimeDrift;									*/
/*		    - RawVideo;										*/
/*		    - EtmRawVideo;									*/
/*		    - PanAnd6HRawVideo;									*/
/*		    - sctime;										*/
/*		    - SyncGyroHdr;									*/
/*		    - SyncAdsHdr;									*/
/*		    - TapeInterface;									*/
/*		    - TmHouseKeeping;									*/
/*		    - TmHouseKeepingRec;								*/
/*		    - TmEphemAttRec;									*/
/*		    - TmRawJitterRec;									*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _lsTypes_h_
#define _lsTypes_h_


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions.											! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef NULL
#define NULL 0
#endif
#define					LSBITMASK	    0x0001	/* least significant bit	*/	    		
#define					LSHALFBYTE	    0x000f	/* least significant half byte	*/
#define					MSHALFBYTE	    0x00f0	/* most significant half byte	*/
#define					MSHALFWORD	    0xff00	/* word mask3			*/
#define					WORDMASK	    0x1fff	/* 13 bits mask			*/
#define					WORDMASK1	    0xf000	/* word mask1			*/
#define					WORDMASK2	    0x0fff	/* word mask2			*/	    
#define					WORDMASK3	    0x00ff	/* word mask3			*/
#define					BANDMASK	    0x0007	/* 3 bits mask for band number	*/
#define					SENSMASK	    0x0008	/* sensor mask			*/

#define					SYNCWORD0	    0x12ff	/* first synchro word from MIU	*/
#define					SYNCWORD1	    0x34ff	/* second synchro word from MIU	*/
#define					SYNCWORD2	    0x56ff	/* third synchro word from MIU	*/
#define					SYNCWORD3	    0x78ff	/* fourth synchro word from MIU	*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Landsat 5 Thematic Mapper nominal parameters.							! */
/* +----------------------------------------------------------------------------------------------------+ */
#define					Tm_sensors	    100		/* tm sensor per swath	*/
#define					Tm_line_size	    6400	/* line size		*/
#define					Tm_Calib_size	    960		/* Thematic Mapper calibration length		*/
#define					EOS_Nominal	    6320	/* nominal End Of Scan	*/
#define					Tm_lost_pixelPRO	    160		/* lost pixel per swath	*/

#define					Tm_band1_pointPRO	       4	/* pointer to the first sensor of band 1    */
#define					Tm_band2_pointPRO	      20	/* pointer to the first sensor of band 2    */
#define					Tm_band3_pointPRO	      36	/* pointer to the first sensor of band 3    */
#define					Tm_band4_pointPRO	      52	/* pointer to the first sensor of band 4    */
#define					Tm_band5_pointPRO	      68	/* pointer to the first sensor of band 5    */
#define					Tm_band6_pointPRO	       0	/* pointer to the first sensor of band 6    */
#define					Tm_band7_pointPRO	      84	/* pointer to the first sensor of band 7    */

#define					Tm_band1_offsetPRO	      85	/* offset between band 1 and TM field center	*/
#define					Tm_band2_offsetPRO	      60	/* offset between band 2 and TM field center	*/
#define					Tm_band3_offsetPRO	      35	/* offset between band 3 and TM field center	*/
#define					Tm_band4_offsetPRO	      10	/* offset between band 4 and TM field center	*/
#define					Tm_band5_offsetPRO	     -61	/* offset between band 5 and TM field center	*/
#define					Tm_band6_offsetPRO	     -96	/* offset between band 6 and TM field center	*/
#define					Tm_band7_offsetPRO	     -35	/* offset between band 7 and TM field center	*/

#define					TmL4_band1_acrossoffsetPRO  -0.12 	/* across offset between band 1 and TM field center	*/
#define					TmL4_band2_acrossoffsetPRO  -0.10 	/* across offset between band 2 and TM field center	*/
#define					TmL4_band3_acrossoffsetPRO  -0.03 	/* across offset between band 3 and TM field center	*/
#define					TmL4_band4_acrossoffsetPRO  -0.0 	/* across offset between band 4 and TM field center	*/
#define					TmL4_band5_acrossoffsetPRO  -0.02 	/* across offset between band 5 and TM field center	*/
#define					TmL4_band6_acrossoffsetPRO  +0.06 	/* across offset between band 6 and TM field center	*/
#define					TmL4_band7_acrossoffsetPRO  -0.08 	/* across offset between band 7 and TM field center	*/


#define					TmL5_band1_acrossoffsetPRO  +0.04 	/* across offset between band 1 and TM field center	*/
#define					TmL5_band2_acrossoffsetPRO  +0.04 	/* across offset between band 2 and TM field center	*/
#define					TmL5_band3_acrossoffsetPRO  +0.01	/* across offset between band 3 and TM field center	*/
#define					TmL5_band4_acrossoffsetPRO  +0.0 	/* across offset between band 4 and TM field center	*/
#define					TmL5_band5_acrossoffsetPRO  +0.11 	/* across offset between band 5 and TM field center	*/
#define					TmL5_band6_acrossoffsetPRO  +0.14 	/* across offset between band 6 and TM field center	*/
#define					TmL5_band7_acrossoffsetPRO  +0.12 	/* across offset between band 7 and TM field center	*/



/*#define					Tm_band6_offset	     -95	 offset between band 6 and TM field center	*/

#define NOMINAL_DELAY_30 	1.25	/* Nominal odd detector offset from band center - 30m */
#define NOMINAL_DELAY_15 	1.00	/* Nominal odd detector offset from band center - 15m */
#define NOMINAL_DELAY_60 	2.50	/* Nominal odd detector offset from band center - 60m */
#define NOMINAL_DELAY_120 	5.00	/* Nominal odd detector offset from band center - 120m */

/* Along scan detector offset (NOMINAL + correction delay) */

static double	AlongScanDetDelay[3][7][2][16] = {					/* IFOV 30m */
		{
			{ { 1.075,1.085,1.030,1.090,1.055,1.045,1.005,1.035,1.015,1.045,1.015,1.060,1.065,1.040,1.070,1.050 },/*B1 Reverse*/
				{ 1.460,1.470,1.450,1.365,1.495,1.425,1.455,1.415,1.450,1.425,1.450,1.455,1.495,1.430,1.515,1.455 }}, /*B1 Forward*/
			{ { 1.320,1.390,1.310,1.350,1.345,1.365,1.315,1.335,1.290,1.345,1.280,1.345,1.310,1.380,1.305,1.330 },/*B2 Reverse*/
				{ 1.175,1.235,1.175,1.185,1.215,1.210,1.185,1.185,1.175,1.190,1.155,1.200,1.195,1.240,1.195,1.195 }}, /* B2 Forward*/ 
			{ { 1.320,1.370,1.330,1.360,1.290,1.335,1.335,1.315,1.290,1.345,1.325,1.340,1.375,1.355,1.355,1.350 },/*B3 Reverse*/
				{ 1.205,1.210,1.215,1.210,1.185,1.175,1.220,1.160,1.185,1.220,1.225,1.220,1.270,1.205,1.260,1.205 }}, /*B3 Forward*/
			{ { 1.285,1.290,1.290,1.295,1.265,1.235,1.275,1.260,1.225,1.275,1.235,1.235,1.255,1.255,1.285,1.280 },/* B4 Reverse*/
				{ 1.260,1.245,1.260,1.250,1.250,1.195,1.210,1.225,1.225,1.205,1.225,1.205,1.250,1.230,1.285,1.255 }}, /*B4 Forward*/
			{ { 1.235,1.295,1.255,1.310,1.170,1.385,1.195,1.385,1.235,1.415,1.230,1.355,1.275,1.400,1.175,1.435 },/*B5 Reverse*/
				{ 1.520,1.330,1.485,1.350,1.380,1.420,1.395,1.415,1.425,1.435,1.395,1.350,1.395,1.370,1.355,1.360 }}, /*B5 Forward*/
			{ { 4.9,4.9,4.9,4.9,0,0,0,0,0,0,0,0,0,0,0,0 },/*B6 Reverse*/
				{ 4.9,4.9,4.9,4.9,0,0,0,0,0,0,0,0,0,0,0,0 }}, /*B6 Forward*/
			{ { 1.395,1.535,1.390,1.259,1.440,1.259,1.250,1.575,1.545,1.605,1.475,1.555,1.425,1.590,1.450,1.670 },/*B7 Reverse*/
				{ 1.189,1.055,1.130,1.115,1.150,1.100,1.170,1.125,1.220,1.100,1.180,1.035,1.115,1.040,1.125,1.075 }} /*B7 Forward*/
		},	/*Landsat4*/
		{
			
			{ { 1.075,1.085,1.030,1.090,1.055,1.045,1.005,1.035,1.015,1.045,1.015,1.060,1.065,1.040,1.070,1.050 },/*B1 Reverse*/
				{ 1.460,1.470,1.450,1.365,1.495,1.425,1.455,1.415,1.450,1.425,1.450,1.455,1.495,1.430,1.515,1.455 }}, /*B1 Forward*/
			{ { 1.320,1.390,1.310,1.350,1.345,1.365,1.315,1.335,1.290,1.345,1.280,1.345,1.310,1.380,1.305,1.330 },/*B2 Reverse*/
				{ 1.175,1.235,1.175,1.185,1.215,1.210,1.185,1.185,1.175,1.190,1.155,1.200,1.195,1.240,1.195,1.195 }}, /* B2 Forward*/ 
			{ { 1.320,1.370,1.330,1.360,1.290,1.335,1.335,1.315,1.290,1.345,1.325,1.340,1.375,1.355,1.355,1.350 },/*B3 Reverse*/
				{ 1.205,1.210,1.215,1.210,1.185,1.175,1.220,1.160,1.185,1.220,1.225,1.220,1.270,1.205,1.260,1.205 }}, /*B3 Forward*/
			{ { 1.285,1.290,1.290,1.295,1.265,1.235,1.275,1.260,1.225,1.275,1.235,1.235,1.255,1.255,1.285,1.280 },/* B4 Reverse*/
				{ 1.260,1.245,1.260,1.250,1.250,1.195,1.210,1.225,1.225,1.205,1.225,1.205,1.250,1.230,1.285,1.255 }}, /*B4 Forward*/
			{ { 1.235,1.295,1.255,1.310,1.170,1.385,1.195,1.385,1.235,1.415,1.230,1.355,1.275,1.400,1.175,1.435 },/*B5 Reverse*/
				{ 1.520,1.330,1.485,1.350,1.380,1.420,1.395,1.415,1.425,1.435,1.395,1.350,1.395,1.370,1.355,1.360 }}, /*B5 Forward*/
			{ { 4.9,4.9,4.9,4.9,0,0,0,0,0,0,0,0,0,0,0,0 },/*B6 Reverse*/
				{ 4.9,4.9,4.9,4.9,0,0,0,0,0,0,0,0,0,0,0,0 }}, /*B6 Forward*/
			{ { 1.395,1.535,1.390,1.259,1.440,1.259,1.250,1.575,1.545,1.605,1.475,1.555,1.425,1.590,1.450,1.670 },/*B7 Reverse*/
				{ 1.189,1.055,1.130,1.115,1.150,1.100,1.170,1.125,1.220,1.100,1.180,1.035,1.115,1.040,1.125,1.075 }} /*B7 Forward*/

		}, /*Landsat5*/
		{
		
			{ { 1.075,1.085,1.030,1.090,1.055,1.045,1.005,1.035,1.015,1.045,1.015,1.060,1.065,1.040,1.070,1.050 },/*B1 Reverse*/
				{ 1.460,1.470,1.450,1.365,1.495,1.425,1.455,1.415,1.450,1.425,1.450,1.455,1.495,1.430,1.515,1.455 }}, /*B1 Forward*/
			{ { 1.320,1.390,1.310,1.350,1.345,1.365,1.315,1.335,1.290,1.345,1.280,1.345,1.310,1.380,1.305,1.330 },/*B2 Reverse*/
				{ 1.175,1.235,1.175,1.185,1.215,1.210,1.185,1.185,1.175,1.190,1.155,1.200,1.195,1.240,1.195,1.195 }}, /* B2 Forward */
			{ { 1.320,1.370,1.330,1.360,1.290,1.335,1.335,1.315,1.290,1.345,1.325,1.340,1.375,1.355,1.355,1.350 },/*B3 Reverse*/
				{ 1.205,1.210,1.215,1.210,1.185,1.175,1.220,1.160,1.185,1.220,1.225,1.220,1.270,1.205,1.260,1.205 }}, /*B3 Forward*/
			{ { 1.285,1.290,1.290,1.295,1.265,1.235,1.275,1.260,1.225,1.275,1.235,1.235,1.255,1.255,1.285,1.280 },/* B4 Reverse*/
				{ 1.260,1.245,1.260,1.250,1.250,1.195,1.210,1.225,1.225,1.205,1.225,1.205,1.250,1.230,1.285,1.255 }}, /*B4 Forward*/
			{ { 1.235,1.295,1.255,1.310,1.170,1.385,1.195,1.385,1.235,1.415,1.230,1.355,1.275,1.400,1.175,1.435 },/*B5 Reverse*/
				{ 1.520,1.330,1.485,1.350,1.380,1.420,1.395,1.415,1.425,1.435,1.395,1.350,1.395,1.370,1.355,1.360 }}, /*B5 Forward*/
			{ { 4.9,4.9,4.9,4.9,0,0,0,0,0,0,0,0,0,0,0,0 },/*B6 Reverse*/
				{ 4.9,4.9,4.9,4.9,0,0,0,0,0,0,0,0,0,0,0,0 }}, /*B6 Forward*/
			{ { 1.395,1.535,1.390,1.259,1.440,1.259,1.250,1.575,1.545,1.605,1.475,1.555,1.425,1.590,1.450,1.670 },/*B7 Reverse*/
				{ 1.189,1.055,1.130,1.115,1.150,1.100,1.170,1.125,1.220,1.100,1.180,1.035,1.115,1.040,1.125,1.075 }} /*B7 Forward*/
		}/*Landsat6*/
	};


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Landsat 7 Enhanced Thematic Mapper plus (ETM+) nominal parameters.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define					Etm_sensors	    104		/* ETM+ detectors per swath for the old fashion bands	*/
												/* 8 (band 6 low gain) + 16 times 6 (bands 1,2,3,4,5,7)	*/
									
#define					Pan_sensors	     32		/* ETM+ detectors per swath for the panchromatic band	    */
#define					B6H_sensors	      8		/* ETM+ detectors per swath for the band 6 high gain (FMT2) */
									
									
#define					Etm_line_size	    6434	/* line size		*/
#define					Etm_Calib_size	    986		/* ETM+ calibration length in minor frames		*/
#define					Etm_lost_pixel	    160		/* lost pixel per swath	*/
#define					Etm_EOS_Nominal	    6320	/* nominal End Of Scan	*/

#define					Etm_band1_point	       8	/* pointer to the first sensor of band 1    */
#define					Etm_band2_point	      24	/* pointer to the first sensor of band 2    */
#define					Etm_band3_point	      40	/* pointer to the first sensor of band 3    */
#define					Etm_band4_point	      56	/* pointer to the first sensor of band 4    */
#define					Etm_band5_point	      72	/* pointer to the first sensor of band 5    */
#define					Etm_band6L_point       0	/* pointer to the first sensor of band 6 low gain    */
#define					Etm_band7_point	      88	/* pointer to the first sensor of band 7    */


#define					Etm_band1_offset      85	/* offset between band 1 and ETM+ field center	*/
#define					Etm_band2_offset      60	/* offset between band 2 and ETM+ field center	*/
#define					Etm_band3_offset      35	/* offset between band 3 and ETM+ field center	*/
#define					Etm_band4_offset      10	/* offset between band 4 and ETM+ field center	*/
#define					Etm_band5_offset     -61	/* offset between band 5 and ETM+ field center	*/
#define					Etm_band6_offset     -96	/* offset between band 6 and ETM+ field center	*/
#define					Etm_band7_offset     -35	/* offset between band 7 and ETM+ field center	*/
#define					Etm_band8_offset     110	/* offset between band Panchromatic and ETM+ field center	*/

/* band number define*/
#define					B_1     0
#define					B_2     1
#define					B_3     2
#define					B_4     3
#define					B_5     4
#define					B_6L    5
#define					B_7     6
#define					B_PAN   7
#define					B_6H    8

/* LUT File Names */
#define LOOK_UP_TABLE_L5_FILENAME				"TM_IC_LUT.xml"
#define LOOK_UP_TABLE_L7_FILENAME				"ETM_IC_LUT.xml"
#define LOOK_UP_TABLE_REFLECTIVE_L5_FILENAME	"TM_REFL_Lut"
#define	LOOK_UP_TABLE_EMISSIVE_L5_FILENAME		"TM_B6_Lut"
#define	LOOK_UP_TABLE_REFLECTIVE_L7_FILENAME	"ETM_REFL_Lut"
#define	LOOK_UP_TABLE_EMISSIVE_L7_FILENAME		"ETM_B6LH_Lut"
#define	LOOK_UP_TABLE_PAN_L7_FILENAME			"ETM_PAN_Lut"

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Includes C header files.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#include 			<time.h>			/* sys:  time header */	
/*#include 			<WRS.h>				 To avoid DegMin redefinition */	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Includes C++ header files.									! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifdef sgi
#include 			<lsError.h>		/* USR:  LANDSAT TM/ETM library error codes */	
#endif
#include 			<lsConstants.h>		/* USR:  LANDSAT TM/ETM library constants */	

#ifdef sgi
#include 			<smError.h>		/* USR:  STATION MANAGEMENT library error codes */	
#endif

#include 			<smTypes.h>		/* USR:  STATION MANAGEMENT library types */	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   TYPEDEFS.											! */
/* +----------------------------------------------------------------------------------------------------+ */
/* +----------------------------------------------------------------------------------------------------+ */



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: BERFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum BERFlag_
{
    GOOD		    = 0, 				/* good quality of the Bit Error Rate	    */
    NOT_GOOD		    = 1					/* bad quality of the Bit Error Rate	    */
} BERFlag;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: BitFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum BitFlag_
{
    NOT_ENABLED		    = 0, 				/* not enabled data			    */
    ENABLED		    = 1					/* enabled data				    */
} BitFlag;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: CalOpMode.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum CalOpMode__
{
    TapeInterfaceMode	    = 0,				/* Tape Interface mode			    */
    TMLevel0Mode	    = 1					/* TMLevel0 mode			    */
} CalOpMode;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: DataValidation.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum DataValidation__
{
    DataNotPresent		= 0,				/* the data is not present		    */
    DataPresentButNotReliable	= 1,				/* the data is present but not reliable	    */
    DataPresentandReliable	= 2				/* the data is present and reliable	    */
} DataValidation;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: EOSFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _EOSFlag_
typedef enum EOSFlag_
{
    NOT_DETECTED	    = 0, 				/* end of scan not detected		    */
    DETECTED		    = 1					/* end of scan detected			    */
} EOSFlag;
#define	_EOSFlag_
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Type definition for local variables.								! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum    ElemType__
{
    NOT_FOUND	= 0, 
    FOUND	= 1
} ElemType;




/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: HalfByte.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _HalfByte_
typedef enum HalfByte__
{
    LeastSignificantHalfByte	= 0,
    MostSignificantHalfByte	= 1
} HalfByte;
#define	_HalfByte_
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: HkConvFactorType.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum HkConvFactorType__
{
    BlackBodyTemp	    = 0,				/* black body		    temperature	    */
    SiliconFpaTemp	    = 1,				/* silicon FPA		    temperature	    */
    CalShutterFlagTemp	    = 2,				/* calibration shutter flag temperature	    */
    Unused1		    = 3,				/* unused no. 1				    */
    BaffleTemp		    = 4,				/* baffle		    temperature	    */
    ColdStageFpaTemp	    = 5,				/* cold stage FPA	    temperature	    */
    Unused2		    = 6,				/* unused no. 2				    */
    Unused3		    = 7,				/* unused no. 3				    */
    ScanLineCorr	    = 8,				/* scan line corrector			    */
    CalShutterHubTemp	    = 9,				/* calibration shutter hub  temperature	    */
    Unused4		    = 10,				/* unused no. 4				    */
    Unused5		    = 11,				/* unused no. 5				    */
    RelayOpticsTemp	    = 12,				/* relay optics		    temperature	    */
    Unused6		    = 13,				/* unused no. 6				    */
    Unused7		    = 14,				/* unused no. 7				    */
    Unused8		    = 15,				/* unused no. 8				    */
    SerialWordB		    = 16,				/* serial word B			    */
    Unused9		    = 17,				/* unused no. 9				    */
    SerialWordD		    = 18,				/* serial word D			    */
    SerialWordE		    = 19,				/* serial word E			    */
    SerialWordF		    = 20,				/* serial word F			    */
    SerialWordG		    = 21,				/* serial word G			    */
    Unused10		    = 22,				/* unused no. 10			    */
    SerialWordL		    = 23,				/* serial word L			    */
    PrimaryMirrorTemp	    = 24,				/* primary mirror	    temperature	    */
    Unused11		    = 25,				/* unused no. 11			    */
    SecondaryMirrorTemp	    = 26,				/* secondary mirror	    temperature	    */
    Unused12		    = 27,				/* unused no. 12			    */
    Unused13		    = 28,				/* unused no. 13			    */
    Unused14		    = 29,				/* unused no. 14			    */
    Unused15		    = 30,				/* unused no. 15			    */
    Unused16		    = 31				/* unused no. 16			    */
} HkConvFactorType;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: MJFFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _MJFFlag_
typedef enum MJFFlag_
{
    NOT_LOSS                = 0,                                /* major frame not loss                     */
    LOSS                    = 1                                 /* major frame loss                         */
} MJFFlag;
#define	_MJFFlag_
#endif



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: MissionKind.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum MissionKind__
{
    Landsat4		    = 0,				/* Landsat 4				    */
    Landsat5		    = 1,				/* Landsat 5				    */
    Landsat6		    = 2, 				/* Landsat 6 (present but never used)	    */
    Landsat7		    = 3,				/* Landsat 7				    */
	Landsat1		    = 4,				/* Landsat 1				    */
    Landsat2		    = 5,				/* Landsat 2				    */
    Landsat3		    = 6
} MissionKind;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: MnfStatus.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum MnfStatus__
{
    MnfNotPresent		= 0,				/* the Mnf is not present		    */
    MnfPresentButNotReliable	= 1,				/* the Mnf is present but not reliable	    */
    MnfPresentandReliable	= 2				/* the Mnf is present and reliable	    */
} MnfStatus;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: PcdMnfCounterStatus.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum PcdMnfCounterStatus__
{
    MnfCounterOkay	    = 0,				/* Counter is okay			    */
    MnfCounterRecovered	    = 1,				/* Counter has been recovered		    */
    MnfCounterNotRecovered  = 2					/* Counter not recovered		    */
} PcdMnfCounterStatus;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: PcdUtility.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum PcdUtility__
{
    Unusable	= 'N',						/* Extracted Pcd unusable		    */
    Usable	= 'Y'						/* Extracted Pcd usable			    */
} PcdUtility;




/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: ScanDirection.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum ScanDirection_
{
    REVERSE		    = 0, 				/* backward direction			    */
    FORWARD		    = 1					/* forward direction			    */
} ScanDirection;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: ScanMirrorEl.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum ScanMirrorEl__
{
    Sme1		    = 0,				/* scan mirror electronic 1		    */
    Sme2		    = 1,				/* scan mirror electronic 2		    */
	Sme1_Sam,							/* sme1 sam mode (only Landsat 7)	*/
	Sme2_Sam,							/* sme2 sam mode (only Landsat 7)	*/
	Sme1_Bump,							/* sme1 bumper mode (only Landsat 7)*/
	Sme2_Bump,							/* sme2 bumper mode (only Landsat 7)*/
	maxElectronicType
} ScanMirrorEl;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: TmHKFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum TmHKFlag__
{
    NotUsed		    = 0,
    Used		    = 1
} TmHKFlag;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: TlmFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum TlmFlag__
{
    NotUpdated		    = 0,
    Updated		    = 1
} TlmFlag;




/* +----------------------------------------------------------------------------------------------------+ */	 
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   STRUCTURES.											! */
/* +----------------------------------------------------------------------------------------------------+ */
/* +----------------------------------------------------------------------------------------------------+ */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: sctime.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _sctime_
typedef struct sctime__
{
    int32_t			    hundreds_of_days;		/* hundreds of days		4bit	    */
    int32_t			    tens_of_days;		/* tens     of days		4bit	    */
    int32_t			    units_of_days;		/* units    of days		4bit	    */
    int32_t			    tens_of_hours;		/* tens     of hours		4bit	    */
    int32_t			    units_of_hours;		/* units    of hours		4bit	    */
    int32_t			    tens_of_minutes;		/* tens     of minutes		4bit	    */
    int32_t			    units_of_minutes;		/* units    of minutes		4bit	    */
    int32_t			    tens_of_seconds;		/* tens     of seconds		4bit	    */
    int32_t			    units_of_seconds;		/* units    of seconds		4bit	    */
    int32_t			    hundreds_of_milliseconds;	/* hundreds of milliseconds	4bit	    */
    int32_t			    tens_of_milliseconds;	/* tens     of milliseconds	4bit	    */
    int32_t			    units_of_milliseconds;	/* units    of milliseconds	4bit	    */
    int32_t			    fraction_of_milliseconds;	/* fraction of milliseconds (1/16 millisec) */
} sctime;
#define	_sctime_
#endif
    

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: EOSType.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct EOSType_
{
    short int			    MinorFrameNum;		/* minor frame number			    */ 
    short int			    Sensore;			/* sensor number			    */
    short int			    Band;			/* band number				    */
} EOSType;
    

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Attitude.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct Attitude_
{
    float			    Roll;			/* roll angle or angular rate		    */ 
    float			    Pitch;			/* pitch angle or angular rate		    */ 
    float			    Yaw;			/* yaw angle or angular rate		    */ 
} Attitude;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Auxiliary.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct Auxiliary_
{
    short int			    SyncWord[4];		/* synchronization words from the Format Synchronizer	    */
    ScanDirection		    ScanMode;			/* Mode of scan						    */
    EOSFlag			    EOSDetected;		/* EOS detected at a particular MNF address		    */
    MJFFlag			    MJFLoss;			/* Major Frame Loss					    */
    BERFlag			    BER;			/* Quality of the Bit Error Rate			    */
    sctime			    SpaceCraftTime; 		/* on board time					    */
    short int			    SpaceCraftID;		/* spacecraft identifier				    */
    short int			    SecondHalfScanErr;		/* second half scan error				    */
    short int			    FirstHalfScanErr;		/* first half scan error				    */
    EOSType			    EOSAddress;			/* address where ......					    */
    BitFlag			    MinorFrameEvolFlag;		/* minor frame evolved flag				    */
    short int			    MinorFrameNumEvol;		/* minor frame number where ..... ???			    */
    BitFlag			    MinorFrameDisFlag;		/* minor frame discrepancy flag				    */
    short int			    MinorFrameNumDis;		/* minor frame number where the first discrepancy occurs    */
    unsigned char		    SatTimeCode[8];		/* satellite time code at start of scan line		    */
    double			    SCTimeMsecOfDay;		/* satellite time millisec of day			    */
    int				    SCTime[8];			/* satellite time yy/mm/dd hh:mm:ss.mmm,m/16		    */
} Auxiliary;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ChkAuxiliary.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ChkAuxiliary_
{
    int				    LostScanLine;		/* 1 if the scan line is lost 0 otherwise		    */
    int				    CorrespondingScanLineNum;	/* Number of the corresponding scan line in the RawTm file  */
    Auxiliary			    AuxiliaryValue;		/* instance of the auxiliary class			    */
} ChkAuxiliary;

#ifdef ZORRO
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: DummyBuffer.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DummyBuffer_
{
    unsigned short		    Auxiliary_Buff[10];		/* auxiliary words			*/
    unsigned short		    Sync_Flag[4];		/* synchronization words from the FS	*/ 
} DummyBuffer;
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: DummyAuxBuffer.									! */
/* +----------------------------------------------------------------------------------------------------+ */

#ifndef _DummyAuxBuffer_
typedef struct DummyAuxBuffer_
{
    unsigned short		    Auxiliary_Buff[10];		 /* auxiliary words */			
} DummyAuxBuffer;
#define	_DummyAuxBuffer_
#endif




/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Mjf.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct Mjf__
{
    int				    MnfPosition[128];		/* position (index) of each minor frame	*/
    MnfStatus			    Status[128];		/* Mnf status				*/
} Mjf;	



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Cycle.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct Cycle__
{
    double			    TimeCode;			/* time code as milliseconds of day */
    Mjf				    MjfInfo[4];			/* information on minor frame	    */
} Cycle;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdMnf.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdMnf__
{
    int				    PcdCycleNumber;		    /* number of pcd cycle		*/
    int				    MjfNumber;			    /* Mjf number inside a pcd cycle	*/
    int				    MnfNumber;			    /* Mnf number inside a pcd Mjf	*/
    int				    MnfRealLen;			    /* Mnf real length			*/
    PcdMnfCounterStatus		    CounterStatus;		    /* Mnf counter status		*/
    unsigned char		    pcdbuf[128];		    /* pcd data buffer			*/
} PcdMnf;	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdMjfQuality.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdMjfQuality__
{
    double			    TimeCode;			/* time code as milliseconds of day */
    int				    ClusteredMnFLost;		/* PCD Minor Frames lost clustered  */
    int				    ScatteredMnFLost;		/* PCD Minor Frames lost scattered  */    
} PcdMjfQuality;	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdAds.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdAds__
{
    double			    Time;			    /* time relative to measurement milliseconds of day	*/
    short int			    Raw[3];			    /* Raw [counts] Ads array	    {roll, pitch, yaw}	*/
    short int			    InterpCount[3];		    /* Interpolated [counts] Ads array	{roll, pitch, yaw}	*/
#ifndef sgi
    /* LINUX PADDING: */
    short int    __linux_padding[2] ;
#endif
    double			    Eu[3];			    /* Eu [microradians] Ads array  {roll, pitch, yaw}	*/
    DataValidation		    Validation[3];		    /* Ads data validation array    {roll, pitch, yaw}	*/
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    	__linux_padding1[4] ;
#endif
} PcdAds;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdAdsTemp.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdAdsTemp__
{
    double			    Time[4];			    /* time relative to measurement milliseconds of day	*/
    short int			    Raw[4];			    /* Raw [counts] AdsTemp array		{T1, T2, T3, T4}    */
    double			    Eu[4];			    /* Eu [Celsius] AdsTemp array		{T1, T2, T3, T4}    */
    double			    EuInterp[4];		    /* Eu [Celsius] Interpolated AdsTemp array	{T1, T2, T3, T4}    */
    DataValidation		    Validation[4];		    /* AdsTemp data validation array		{T1, T2, T3, T4}    */
} PcdAdsTemp;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdAttitude.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdAttitude__
{
    double			    Time;			    /* time relative to measurement milliseconds of day	*/
    int32_t			    Raw[4];			    /* Raw Attitude array		{epa 1, epa 2, epa 3, epa 4}	*/
    double			    Eu[4];			    /* Eu  Attitude array		{epa 1, epa 2, epa 3, epa 4}	*/
    double			    EuInterp[4];		    /* Eu  Interpolated Attitude array	{epa 1, epa 2, epa 3, epa 4}	*/
    DataValidation		    Validation[4];		    /* Attitude data validation array   {epa 1, epa 2, epa 3, epa 4}	*/
} PcdAttitude;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdEphemeris.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdEphemeris__
{
    double			    Time;			    /* time relative to measurement milliseconds of day	*/
    int32_t			    Raw[6];			    /* Raw [counts]	Ephemeris array			{x, y, z, xdot, ydot, zdot} */
    double			    Eu[6];			    /* Eu  [m, km/s]	Ephemeris array			{x, y, z, xdot, ydot, zdot} */
    double			    EuInterp[6];		    /* Eu  [m, km/s]	Interpolated Ephemeris array	{x, y, z, xdot, ydot, zdot} */
    DataValidation		    Validation[6];		    /* Ephemeris data validation array			{x, y, z, xdot, ydot, zdot} */
} PcdEphemeris;	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdPolCoeff.																			! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdPolCoeff__
{
    double			    Time;			    /* reference time in milliseconds of day */
    double			    PosVel[6][4];		    /* Position {x, y, z} and Velocity	{xdot, ydot, zdot}
												   for the four coefficients */
} PcdPolCoeff;	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdGyro.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdGyro__
{
    double			    Time;			    /* time relative to measurement milliseconds of day	*/
    int32_t			    Raw[3];			    /* Raw [counts] Gyro array		    {roll, pitch, yaw}	*/	
#ifndef sgi
    /* LINUX PADDING: */
    int32_t    __linux_padding[1] ;
#endif
    double			    Eu[3];			    /* Eu  [arcsec] Gyro array		    {roll, pitch, yaw}	*/
    double			    EuInterp[3];		    /* Eu  [arcsec] Interpolated Gyro array {roll, pitch, yaw}  */
    DataValidation		    Validation[3];		    /* Gyro data validation array	    {roll, pitch, yaw}	*/
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    __linux_padding1[4] ;
#endif
} PcdGyro;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdGyroDrift.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdGyroDrift__
{
    double			    Time;			    /* time relative to measurement milliseconds of day	*/
    int32_t			    Raw[3];			    /* Raw [counts]	    Gyro Drift array		    {roll, pitch, yaw}  */
#ifndef sgi
    /* LINUX PADDING: */
    int32_t    __linux_padding[1] ;
#endif
    double			    Eu[3];			    /* Eu  [rad/(512*msec)] Gyro Drift array		    {roll, pitch, yaw}  */
    double			    EuInterp[3];		    /* Eu  [rad/(512*msec)] Interpolated Gyro Drift data    {roll, pitch, yaw}  */
    DataValidation		    Validation[3];		    /* Ephemeris data validation array			    {roll, pitch, yaw}  */
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    	__linux_padding1[4] ;
#endif
} PcdGyroDrift;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdHkTlm.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdHkTlm__
{
    double					Time;
	unsigned char		    HkTlmData[31];		    /* Pcd telemetry data				    */
} PcdHkTlm;	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdEtmHkTlm.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdEtmHkTlm__
{
    double			    Time;			    /* time relative to measurement milliseconds of day	*/
    unsigned char		    Raw[13];			    /* Raw [counts] mnf 74-80, 81,82,83,84, 116-117	*/	
    unsigned char		    Eu[13];			    /* Eu  [counts] mnf 74-80, 81,82,83,84, 116-117	*/
    unsigned char		    EuInterp[13];		    /* Eu  [counts] mnf 74-80, 81,82,83,84, 116-117	*/
    DataValidation		    Validation[13];		    /* data validation array				*/
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    		__linux_padding1[4] ;
#endif
} PcdEtmHkTlm;	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: CeosTime.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct CeosTime__
{
    int				    DayOfTheYear;			
    int				    MillisecOfDay;			
    int				    ThousandthOfMillisecOfDay;	
} CeosTime;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdTime.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdTime__
{
    CeosTime			    CeosTimeData;		    /* time for Ceos format				*/	
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    		__linux_padding[4] ;
#endif   
    double			    Time;			    /* converted spacecraft time			*/
    int32_t			    Raw[13];			    /* raw spacecraft time				*/
    DataValidation		    Validation[13];		    /* Time data validation array  {time components}	*/
} PcdTime;	    		


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdSVClkUpdate.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdSVClkUpdate__
{
    double			    MjfTime;			    /* major frame [0] time code relative to measurement [milliseconds of day]	*/
    double			    SVClkUpdateTime;		    /* Last clock update from MOC [msec] from midnight of beginning of year	*/
    double			    ClkUpdateMsecOfDay;		    /* Last clock update from MOC [msec] from midnight of beginning of day	*/
    unsigned char		    Raw[6];			    /* Raw [bytes] see DFCB Volume IV - Wideband Data - rev. L			*/
    DataValidation		    Validation[6];		    /* Ephemeris data validation array						*/
} PcdSVClkUpdate;	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: PcdTimeDrift.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct PcdTimeDrift__
{
    double			    MjfTime;			    /* major frame [0] time code relative to measurement [milliseconds of day]	*/
    short int			    Coeff[3];			    /* Time drift coeffs [msec, msec/day, msec/day**2]				*/
    unsigned char		    Raw[6];			    /* Raw [bytes] see DFCB Volume IV - Wideband Data - rev. L			*/
    DataValidation		    Validation[6];		    /* Ephemeris data validation array						*/
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    		__linux_padding[4] ;
#endif   
} PcdTimeDrift;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ImgRecInfo.                                                                           ! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecInfo__
{
    int				RecSeqNum;			/* record sequence number			    */
    unsigned char		Codes[4];			/* file, record, mission, origin codes		    */
    int				RecLen;				/* record length				    */
} ImgRecInfo;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ImgRecPrefix.                                                                         ! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecPrefix__
{
    int				ScanLineNum;			/* scan line number				    */
    int				BandNum;			/* band number			                    */
    u_int32_t		ScanLineTime;			/* scan line time (msec of day)			    */
    int				LeftFillPixels;			/* count of left fill pixel			    */
    int				RightFillPixels;		/* count of right fill pixel			    */
} ImgRecPrefix;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ImgRecSuffix.                                                                         ! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecSuffix__
{
    unsigned char		Codes[8];			/* set of codes					    */
    int				FullScanLineLen;		/* counted full scan line length		    */
    int				EmbeddedLineLen;		/* embedded line length				    */
    short			TimeErrorStartMid;		/* time error from line start to mid scan	    */
    short			TimeErrorMidEnd;		/* time error from mid scan to line end		    */
    int				ScanLineDir;			/* scan line direction				    */
    int				CurrScanLineLen;		/* current scan line length			    */
    unsigned char		SatTimeCode[8];			/* satellite time code at start of scan		    */
    unsigned char		CalCodes[4];			/* codes dealing with calibration		    */
    int				LowLevelCalValue;		/* low level calibration value			    */
    int				HiLevelCalValue;		/* high level calibration value			    */
    int				ComputedGain;			/* calibration lamp computed gain value		    */
    int				ComputedBias;			/* calibration lamp computed bias value		    */
    int				AppliedGain;			/* applied gain value				    */
    int				AppliedBias;			/* applied bias value				    */
    int				LocalUse;			/* local use					    */
} ImgRecSuffix;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecFS.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecFS__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[FULL_SCENE_LINE_LEN];	/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecFS;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecFS.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRec1G__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		*ImgData;			/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRec1G;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecMS.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecMS__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[MINI_SCENE_LINE_LEN];	/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecMS;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ImgRecQS.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecQS__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[QUARTER_SCENE_LINE_LEN];/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecQS;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ImgRecMC.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecMC__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[MICRO_SCENE_LINE_LEN];/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecMC;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecFS_Pan.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecFSPan__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[2*FULL_SCENE_LINE_LEN];	/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecFSPan;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecMS_Pan.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecMS_Pan__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[2*MINI_SCENE_LINE_LEN];	/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecMSPan;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecQS_Pan.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecQS_Pan__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[2*QUARTER_SCENE_LINE_LEN];/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecQSPan;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: ImgRecMC_Pan.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct ImgRecMC_Pan__
{
    ImgRecInfo			Info;				/* instance of ImgRecInfo structure		    */
    ImgRecPrefix		Prefix;				/* instance of ImgRecPrefix structure		    */
    unsigned char		ImgData[2*MICRO_SCENE_LINE_LEN];/* image data file				    */
    ImgRecSuffix		Suffix;				/* instance of ImgRecSuffix structure		    */
} ImgRecMCPan;




/* +------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: RawVideo.									    ! */
/* +------------------------------------------------------------------------------------------------+ */
typedef struct RawVideo__
{
    unsigned char		Tm_data[Tm_sensors][Tm_line_size + Tm_Calib_size];
    unsigned short		Sync_Flag[4];
    unsigned short		Auxiliary_Raw[10];
} RawVideo;

typedef struct RawVideoS__
{
    unsigned short		Tm_data[Tm_sensors][Tm_line_size + Tm_Calib_size];
    unsigned short		Sync_Flag[4];
    unsigned short		Auxiliary_Raw[10];
} RawVideoS;

/* +------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: EtmRawVideo.									    ! */
/* +------------------------------------------------------------------------------------------------+ */
typedef struct EtmRawVideo__
{
    unsigned char		Etm_data[Etm_sensors][Etm_line_size + Etm_Calib_size];
    unsigned short		Sync_Flag[4];
    unsigned short		Auxiliary_Raw[10];
} EtmRawVideo;

/* +------------------------------------------------------------------------------------------------+ */
/* !   Structure: Pan6HRawVideo.								    ! */
/* +------------------------------------------------------------------------------------------------+ */
typedef struct PanAnd6HRawVideo__
{
    unsigned char		B6H_data[B6H_sensors][Etm_line_size + Etm_Calib_size];
    unsigned char		Pan_data[Pan_sensors][2*(Etm_line_size + Etm_Calib_size)];  /* pixel resolution 15m */
    unsigned short		Sync_Flag[4];
    unsigned short		Auxiliary_Raw[10];
} PanAnd6HRawVideo;


/* +----------------------------------------------------------------------------------------------------+ */ 
/* !   Structure: TmHouseKeeping.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct TmHouseKeeping_
{
    TmHKFlag			TmHKFlagData;			/* flag that say if the field is used or not	*/
    int				TimeOffsetMicrosec;		/* time offset (microsec) w.r.t. Mjf	*/
    int				Sample;				/* Tm House Keeping sample value	*/
    float			ACoeff[NUM_OF_CONVCOEFF];	/* Set of six coeff. to convert counts into e.u.    */
    
} TmHouseKeeping;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: IntervalHdrRec.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct IntervalHdrRec_
{
    CeosTime			InputSceneStartTime;    
    CeosTime			InputSceneStopTime;    
    CeosTime			PcdTelemetryStartTime;    
    CeosTime			PcdTelemetryStopTime;
    int				PcdMjfNumber;    
} IntervalHdrRec;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: TmHouseKeepingRec.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct TmHouseKeepingRec_
{
    CeosTime			MjfStartTimeData;    
    TmHouseKeeping		TmHouseKeepingArray[NUM_OF_TMHK];
    char			UnpackedSerWord[NUM_OF_UNPACKSERWORD];
} TmHouseKeepingRec;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: TmEphemAttRec.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct TmEphemAttRec_
{
    TlmFlag			EphemFlag;
    TlmFlag			AttitudeFlag;
    TlmFlag			GyroFlag;
    TlmFlag			GyroDriftFlag;
    CeosTime			MjfStartTimeData;    

	int				EphemTimeOffsetMicrosec;
    double			Ephem_x;
    double			Ephem_y;
    double			Ephem_z;
    double			Ephem_vx;
    double			Ephem_vy;
    double			Ephem_vz;

    int				AttitudeTimeOffsetMicrosec;
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    		__linux_padding1[4] ;
#endif   
    double			Epa1;
    double			Epa2;
    double			Epa3;
    double			Epa4;

    int				GyroTimeOffsetMicrosec;
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    		__linux_padding2[4] ;
#endif   
    double			Gyro_x[NUM_OF_GYROINATTREC];
    double			Gyro_y[NUM_OF_GYROINATTREC];
    double			Gyro_z[NUM_OF_GYROINATTREC];

    int				GyroDriftTimeOffsetMicrosec;
#ifndef sgi
    /* LINUX PADDING: */
    unsigned char    		__linux_padding3[4] ;
#endif   
    double			GyroDrift_x;
    double			GyroDrift_y;
    double			GyroDrift_z;

    
} TmEphemAttRec;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: TmRawJitterRec.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct TmRawJitterRec_
{
    int				SeqNumInMjf;

    CeosTime			MjfStartTimeData;    

    int				AdsTempTimeOffset[4];
    float			AdsTemp[4];
    
    int				TimeOffsetAxis_x;
    int				TimeOffsetAxis_y;
    int				TimeOffsetAxis_z;
    
    short int			Ads_x[NUM_OF_ADSINADSREC];
    short int			Ads_y[NUM_OF_ADSINADSREC];
    short int			Ads_z[NUM_OF_ADSINADSREC];
    
} TmRawJitterRec;


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: MprTapeInfo.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _MprTapeInfo_

typedef struct MprTapeInfo_
{
    float			NomInterPixelDistance;		/* nominal scale of inter-pixel distance at nadir [m]	*/
    float			NomInterLineDistance;		/* nominal scale of inter-line distance at nadir [m]	*/
    float			ImageSkewSceneCentre;		/* image skew at scene centre				*/

    float			NominalWRSNorthing;		/* nominal WRS northing of centre [m]			*/
    float			NominalWRSEasting;		/* nominal WRS easting of centre [m]			*/

    float			VerticalOffsetToWRSCentre;	/* vertical offset of scene centre to WRS nominal centre */
    float			HorizontalOffsetToWRSCentre;	/* horizontal offset of scene centre to WRS nominal centre */

    int				PixelsPerLineProcImgNum;	/* number of pixels per line of processed image */
    int				LinesPerProcImgNum;		/* number of lines per processed image		*/

    float			ProcInterPixelDistance;		/* processed scale of inter-pixel distance [m]	*/
    float			ProcInterLineDistance;		/* processed scale of inter-line distance [m]	*/

    int				UTMProcImgZoneNum;		/* UTM zone number for processed image		*/

    float			ProcImgAtWRSCentreLineNum;	/* line number in processed image at WRS scene centre */	
    float			ProcImgAtWRSCentrePixelNum;	/* pixel number in processed image at WRS scene centre */
    float			ProcImgSceneCentreOrientation;	/* orientation of processed image centre [deg] */

    float			NomSatelliteOrbitInclination;	/* nominal satellite orbital inclination */	
    float			NomAscendingNodeLon;		/* nominal ascending node longitude at equator [deg] */
    float			NomSatelliteAltitude;		/* nominal satellite altitude [m] */
    float			NomGroundSpeed;			/* nominal satellite ground speed [m / sec] */

    float			CrossTrackFieldOfView;		/* cross track field of view [deg] */	
    float			SensorScanRate;			/* sensor scan rate [scans / sec] */
    float			SensorSamplingRate;		/* sensor active sampling rate [samples / sec] */

    float			TopLeftCornerLatitude;		/* top left corner latitude [deg] */	
    float			TopLeftCornerLongitude;		/* top left corner longitude [deg] */

    float			TopRightCornerLatitude;		/* top right corner latitude [deg] */
    float			TopRightCornerLongitude;	/* top right corner longitude [deg] */

    float			BottomLeftCornerLatitude;	/* bottom left corner latitude [deg] */	
    float			BottomLeftCornerLongitude;	/* bottom left corner longitude [deg] */

    float			BottomRightCornerLatitude;	/* bottom right corner latitude [deg] */
    float			BottomRightCornerLongitude;	/* bottom right corner longitude [deg] */

} MprTapeInfo;

#define	_MprTapeInfo_
#endif


/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: SyncAdsHdr.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct SyncAdsHdr_
{
    int				MagicNum;
    int				NumOfScanLine;
}SyncAdsHdr;


/* +----------------------------------------------------------------------------------------------------+ */ 
/* !   Structure: SyncGyroHdr.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct SyncGyroHdr_
{
    int				MagicNum;
    int				NumOfScanLine;
}SyncGyroHdr;

#if(0)
/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: CalibInfo.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct CalibInfo_
{
    unsigned char		LampStatus[SWATHSMAXNUM];	/* lamps state value per scan line	*/
    unsigned char		LampSeqNum[SWATHSMAXNUM];	/* lamps sequence number per scan line	*/
    
    int				DarkCurrentValue[SWATHSMAXNUM][NUM_OF_BANDS - 1][NUM_OF_DET];
								/* dark current value			*/
								
    int				MeanPulseValue[SWATHSMAXNUM][NUM_OF_BANDS - 1][NUM_OF_DET];
								/* mean pulse value			*/
    
    int				ComputedGainValue[NUM_OF_BANDS - 1][NUM_OF_DET];    /* gain values	*/
    int				ComputedBiasValue[NUM_OF_BANDS - 1][NUM_OF_DET];    /* bias values	*/

    unsigned char		LookUpTable[NUM_OF_BANDS - 1][NUM_OF_DET][NUM_OF_LEVELS];
								/* Look Up Table			*/
} CalibInfo;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: CalibInfoL7Pan.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct {
    unsigned char		LampStatus[SWATHSMAXNUM];	/* lamps state value per scan line	*/
    unsigned char		LampSeqNum[SWATHSMAXNUM];	/* lamps sequence number per scan line	*/
    
    int				DarkCurrentValue[SWATHSMAXNUM][Pan_sensors];
								/* dark current value			*/
								
    int				MeanPulseValue[SWATHSMAXNUM][Pan_sensors];
								/* mean pulse value			*/
    
    int				ComputedGainValue[Pan_sensors];    /* gain values	*/
    int				ComputedBiasValue[Pan_sensors];    /* bias values	*/

    unsigned char		LookUpTable[Pan_sensors][NUM_OF_LEVELS];
								/* Look Up Table			*/
} CalibInfoL7Pan;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: CalibInfoL7B6H.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct {
    unsigned char		LampStatus[SWATHSMAXNUM];	/* lamps state value per scan line	*/
    unsigned char		LampSeqNum[SWATHSMAXNUM];	/* lamps sequence number per scan line	*/
    
    int				DarkCurrentValue[SWATHSMAXNUM][B6H_sensors];
								/* dark current value			*/
								
    int				MeanPulseValue[SWATHSMAXNUM][B6H_sensors];
								/* mean pulse value			*/
    
    int				ComputedGainValue[B6H_sensors];    /* gain values	*/
    int				ComputedBiasValue[B6H_sensors];    /* bias values	*/

    unsigned char		LookUpTable[B6H_sensors][NUM_OF_LEVELS];
								/* Look Up Table			*/
} CalibInfoL7B6H;

/* +----------------------------------------------------------------------------------------------------+ */ 
/* !   Structure: CalibInfoL7.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct CalibInfoL7_
{
    char			GainSetting[MAX_BANDNUM];  /* filled in this order B1,B2,B3,B4,B5,B6L,B7,B8 (PAN),B6H */
    CalibInfo			CalibDataL5;
    CalibInfoL7Pan		CalibDataL7Pan;
    CalibInfoL7B6H		CalibDataL7B6H;
} CalibInfoL7;

#endif


/**************************************************************************************************/

typedef struct {
    unsigned char	LampStatus[SWATHSMAXNUM]; /* lamps state value per scan line  */
    unsigned char	LampSeqNum[SWATHSMAXNUM]; /* lamps sequence number per scan line  */
    int				DarkCurrentValue[SWATHSMAXNUM][MAX_BANDNUM][MAX_DETNUM]; /* dark current value */		
    int				MeanPulseValue[SWATHSMAXNUM][MAX_BANDNUM][MAX_DETNUM]; /* mean pulse value */
    int				ComputedGainValue[MAX_BANDNUM][MAX_DETNUM];    /* gain values */
    int				ComputedBiasValue[MAX_BANDNUM][MAX_DETNUM];    /* bias values */
} ICinfo;

typedef struct 
{
    char			GainSetting[BANDS_NUMBER];  /* filled in this order B1,B2,B3,B4,B5,B6L,B7,B8 (PAN),B6H */
    char			GainSettingStop[BANDS_NUMBER];
	int				GainSettingChangeLine[BANDS_NUMBER];
	ICinfo			ICdata;
	unsigned char	LookUpTable[MAX_BANDNUM][SWATHSMAXNUM][MAX_DETNUM][NUM_OF_LEVELS];
} LandCalibInfo;


/**************************************************************************************************/

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: DetectorInfo.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DetectorInfoL5_
{
    u_int32_t		DetectorHistogram[NUM_OF_BANDS - 1][NUM_OF_DET][NUM_OF_LEVELS];
    unsigned char		DetectorLookUpTable[NUM_OF_BANDS - 1][NUM_OF_DET][NUM_OF_LEVELS];
} DetectorInfoL5;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: DetectorInfoPAN.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DetectorInfoPAN_
{
    u_int32_t		DetectorHistogram[Pan_sensors][NUM_OF_LEVELS];
    unsigned char			DetectorLookUpTable[Pan_sensors][NUM_OF_LEVELS];
} DetectorInfoPAN;

/* +----------------------------------------------------------------------------------------------------+ */ 
/* !   Structure: DetectorInfoB6H.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DetectorInfoB6H_
{
    u_int32_t		DetectorHistogram[B6H_sensors][NUM_OF_LEVELS];
    unsigned char			DetectorLookUpTable[B6H_sensors][NUM_OF_LEVELS];
} DetectorInfoB6H;


/* EC:: FIXME:: FOLLOWING STRUCTURE HAS NOT BEEN PADDED */
/* +----------------------------------------------------------------------------------------------------+ */
/* !   Structure: TapeInterface.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct TapeInterface_
{
    FROrder			FROrderData;				    /* full resolution order data		*/
    Ftdcf			FtdcfData;				    /* centre frame time info			*/
    int				NumOfSelectedBand;			    /* number of selected bands			*/
    int				NumOfPhysicalTape;			    /* number of physical tape			*/
    int				NumOfBandsPerPhysicalTape;		    /* number of bands per physical tape	*/
    Fdr				FdrArray[BANDS_NUMBER];		    /* array of Fdr structure			*/
    
    DetectorInfoL5		DetectorInfoL5Data;		    /* detector info for TM bands landsat 5 also valid
								       for correspondent L7 bands (include band 6 low gain */

    int				NumOfAcquiredSwath;			    /* number of acquired swaths		*/
    int				RecRelFirstVideo;			    /* number of acquired swaths		*/
    
    UTtime			AcqStartTime;				    /* acquisition start time			*/
    UTtime			AcqStopTime;				    /* acquisition stop time			*/
    
    IntervalHdrRec		IntervalHdrRecData;			    /* array of Ih record			*/
    
    int				NumOfUsedHkRec;				    /* number of used Hk record			*/
    TmHouseKeepingRec		TmHouseKeepingRecArray[MAXNUMHKREC];	    /* array of Hk record			*/

    int				NumOfUsedEphemAttRec;			    /* number of used EphemAtt record		*/
    TmEphemAttRec		TmEphemAttRecArray[MAXNUMEPHEMATTREC];	    /* array of EphemAtt record			*/
    TmEphemAttRec		TmInterpEphemAttRecArray[MAXNUMEPHEMATTREC];	    /* array of interpolated EphemAtt record	*/

    int				NumOfUsedRawJitterRec;			    /* number of used RawJitter record		*/
    TmRawJitterRec		TmRawJitterRecArray[MAXNUMRAWJITTERREC];    /* array of RawJitter record		*/
    TmRawJitterRec		TmInterpRawJitterRecArray[MAXNUMRAWJITTERREC];	    /* array of interpolated RawJitter record	*/

    MprTapeInfo			MprTapeInfoData;			    /* map projection structure			*/
    
    int				CalLampStatus[SWATHSMAXNUM];    
    int				CalLampSeqNum[SWATHSMAXNUM];    

    int				DaysSinceLaunch;			    /* days since launch			*/    

    PcdUtility			PcdUtilityFlag;				    /* flag to write into the Scene Header	*/    
    double			FirstVideoPixel;			    /* info to write into the Shr bytes [2233, ..]*/    
    int				NumberOfBitsPerPixel;			    /* Added by VDS for 1R product (16 bits): parameter used by HDF, FAST, CEOS producers */    
    DetectorInfoPAN		DetectorInfoPANData;			    /* detector info for Panchromatic band L7	*/
    DetectorInfoB6H		DetectorInfoB6HData;			    /* detector info for thermal high gain L7	*/
    // CalibInfoL7			CalibInfoL7Data;
	LandCalibInfo		LandCalInfo;
} TapeInterface;


# define HKTYPE_NUM 14
typedef enum {
	TBB,
	TSH,
	TSCL,
	TBAFHTR,
	TSECMIR,
	TPRIMIR,
	TSCANMIR,
	TBBISO,
	TBBCTL,
	TCFPCTL,
	TCFPMON,
	TBAFTUB,
	TBAFSUP,
	TTELHOUS
} HKTYPE;

typedef struct HkTemp__
{
    double	Time;
    double	temp;
} HkTemp;

typedef enum QAflags_
{	
	/* No image or missing/bad pixels */
	QA_NOIMAGE			= 0,
	QA_MISSING			= 1,
	QA_CORRUPTED		= 2,
	/* Under/Over saturated pixels */
	QA_UNDERSATURATED	= 20,
	QA_OVERSATURATED	= 21,
	/* Good pixels */
	QA_GOOD				= 127,
	/* SLA1 pixels */
	QA_UNDERSAT_SLA1	= 149,
	QA_SLA1				= 150,
	QA_OVERSAT_SLA1		= 151,
	/* SLA1 AND SLA2 pixels */
	QA_UNDERSAT_SLA12	= 189,
	QA_SLA12			= 190,
	QA_OVERSAT_SLA12	= 191,
	/* SB pixels */
	QA_UNDERSAT_SB		= 253,
	QA_SB				= 254,
	QA_OVERSAT_SB		= 255
} QAflags ;

/* +----------------------------------------------------------------------------------------------------+ */
/* !   End of code redefinition control.								! */
/* +----------------------------------------------------------------------------------------------------+ */
#endif
