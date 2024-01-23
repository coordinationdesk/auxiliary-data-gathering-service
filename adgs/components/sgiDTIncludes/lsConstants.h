/*

	Copyright 1995-2011, Advanced Computer Systems , Inc.
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
	Revision 1.5  2013/03/18 10:08:20  ricfer
	QUANTVALUE and QUANTLEVELS added.
	
	Revision 1.4  2012/12/10 14:01:47  lucgal
	MAX_LOST_SCAN_LINES increased from 10 to 40
	
	Revision 1.3  2012/09/20 06:29:50  ricfer
	MAX_BANDNUM and MAX_DETNUM added.
	
	Revision 1.2  2011/03/01 17:00:54  marpas
	GCC 4.4.x support
	
	Revision 1.1  2007/09/03 13:26:27  giucas
	Moved from libMDPSLandsatProc
	
	Revision 1.1.1.1  2006/10/27 12:17:19  valspa
	VDS: first porting of the MDPS SGI Landsat to new LINUX environment
	


*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _lsConstants_h_
#define _lsConstants_h_
 
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char const * lsConstantsId="@(#) A.C.S. LANDSAT TM/ETM Library: lsConstants.h 1.3 02/02/15";


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions.											! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef NULL
#define NULL 0
#endif




/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the world reference systems.						! */
/* +----------------------------------------------------------------------------------------------------+ */
#define		    N_OF_ROWS		248		/* Landsat 1-2-3-4-5-6					*/	

#define		    N_OF_ORBITS_1	251		/* Landsat 1						*/
#define		    CYCLE_1		18		/* Landsat 1						*/

#define		    N_OF_ORBITS_2	251		/* Landsat 2						*/		
#define		    CYCLE_2		18		/* Landsat 2						*/	

#define		    N_OF_ORBITS_3	251		/* Landsat 3						*/		
#define		    CYCLE_3		18		/* Landsat 3						*/	

#define		    N_OF_ORBITS_4	233		/* Landsat 4						*/		
#define		    CYCLE_4		16		/* Landsat 4						*/	

#define		    N_OF_ORBITS_5	233		/* Landsat 5						*/		
#define		    CYCLE_5		16		/* Landsat 5						*/	

#define		    N_OF_ORBITS_6	233		/* Landsat 6						*/		
#define		    CYCLE_6		16		/* Landsat 6						*/	

#define		    N_OF_ORBITS_7	233		/* Landsat 7						*/		
#define		    CYCLE_7		16		/* Landsat 7						*/	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Nominal orbital period for the different missions.						! */
/* +----------------------------------------------------------------------------------------------------+ */
#define		    T1nominalMins	103.266		/* Landsat 1 nominal period [min]			*/
#define		    T2nominalMins	103.2665	/* Landsat 2 nominal period [min]			*/
#define		    T3nominalMins	103.266		/* Landsat 3 nominal period [min]			*/
#define		    T4nominalMins	98.88412	/* Landsat 4 nominal period [min]			*/
#define		    T5nominalMins	98.88412	/* Landsat 5 nominal period [min]			*/
#define		    T6nominalMins	98.88412	/* Landsat 6 nominal period [min]			*/
#define		    T7nominalMins	98.88412	/* Landsat 7 nominal period [min]			*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions comes from the Geodetic Reference System GRS80.			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				EARTH_ELLIPSOID		"GRS80"		    /* Earth Ellipsoid used 		*/
#define				ISSUED_YEAR		 1984		    /* Issued Year			*/
#define				EQUAT_RADIUS		 6378.137L	    /* Equatorial Radius (a) [Km]	*/
#define				POLAR_RADIUS		 6356.7523L	    /* Polar      Radius (b) [Km]	*/
#define				FLATTENING		1.0L / 298.257L	    /* Flattening [1/f = a / (a - b)]   */
#define				EARTH_ROT_PERIOD	 1440.0L	    /* Earth rotation period [min]	*/
#define				EARTH_ROT_PERIOD_SEC	86400.0L	    /* Earth rotation period [sec]	*/

#define				GRAVCONST		6.670e-11	    /* gravitational constants [m**3 / (kg * sec**2)]   */
#define				EARTHMASS		5.976e+24	    /* earth mass [kg]					*/



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the World Reference System (LANDSAT 1).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND1_PATH_MIN_NUM	  1		    /* path number minimum  */
#define				LAND1_PATH_MAX_NUM	251		    /* path number maximum  */
#define				LAND1_ROW_MIN_NUM	  1		    /* row  number minimum  */
#define				LAND1_ROW_MAX_NUM	248		    /* row  number maximum  */

#define				LAND1_ASCNODE_ROWNUM	184		    /* ascending node row number */

#define				LAND1_LAUNCH_YEAR      1972		    /* launch year	    */
#define				LAND1_LAUNCH_MONTH	  7		    /* launch month	    */
#define				LAND1_LAUNCH_DAY	 22		    /* launch day	    */
#define				LAND1_LAUNCH_HOURS	 10		    /* launch hours	    */
#define				LAND1_LAUNCH_MINUTES	  0		    /* launch minutes	    */
#define				LAND1_LAUNCH_SECONDS	  0		    /* launch seconds	    */
#define				LAND1_LAUNCH_MILSECS	  0		    /* launch milliseconds  */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 1).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND1_ORBIT_INCL	99.110L		    /* orbital inclination in degrees		    */
#define				LAND1_SAT_REV_PERIOD   103.151L		    /* nominal satellite revolution period [min]    */

#define				LAND1_DESCNODE_LON   -65.483L		    /* descending node longitude [degrees.dec] at 09:45 local time */



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the World Reference System (LANDSAT 2).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND2_PATH_MIN_NUM	  1		    /* path number minimum  */
#define				LAND2_PATH_MAX_NUM	251		    /* path number maximum  */
#define				LAND2_ROW_MIN_NUM	  1		    /* row  number minimum  */
#define				LAND2_ROW_MAX_NUM	248		    /* row  number maximum  */

#define				LAND2_ASCNODE_ROWNUM	184		    /* ascending node row number */

#define				LAND2_LAUNCH_YEAR      1975		    /* launch year	    */
#define				LAND2_LAUNCH_MONTH	  1		    /* launch month	    */
#define				LAND2_LAUNCH_DAY	 22		    /* launch day	    */
#define				LAND2_LAUNCH_HOURS	 10		    /* launch hours	    */
#define				LAND2_LAUNCH_MINUTES	  0		    /* launch minutes	    */
#define				LAND2_LAUNCH_SECONDS	  0		    /* launch seconds	    */
#define				LAND2_LAUNCH_MILSECS	  0		    /* launch milliseconds  */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 2).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND2_ORBIT_INCL	99.110L		    /* orbital inclination in degrees		    */
#define				LAND2_SAT_REV_PERIOD   103.151L		    /* nominal satellite revolution period [min]    */

#define				LAND2_DESCNODE_LON   -65.483L		    /* descending node longitude [degrees.dec] at 09:45 local time */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the World Reference System (LANDSAT 3).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND3_PATH_MIN_NUM	  1		    /* path number minimum  */
#define				LAND3_PATH_MAX_NUM	251		    /* path number maximum  */
#define				LAND3_ROW_MIN_NUM	  1		    /* row  number minimum  */
#define				LAND3_ROW_MAX_NUM	248		    /* row  number maximum  */

#define				LAND3_ASCNODE_ROWNUM	184		    /* ascending node row number */

#define				LAND3_LAUNCH_YEAR      1978		    /* launch year	    */
#define				LAND3_LAUNCH_MONTH	  3		    /* launch month	    */
#define				LAND3_LAUNCH_DAY	  5		    /* launch day	    */
#define				LAND3_LAUNCH_HOURS	 10		    /* launch hours	    */
#define				LAND3_LAUNCH_MINUTES	  0		    /* launch minutes	    */
#define				LAND3_LAUNCH_SECONDS	  0		    /* launch seconds	    */
#define				LAND3_LAUNCH_MILSECS	  0		    /* launch milliseconds  */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 3).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND3_ORBIT_INCL	99.110L		    /* orbital inclination in degrees		    */
#define				LAND3_SAT_REV_PERIOD   103.151L		    /* nominal satellite revolution period [min]    */

#define				LAND3_DESCNODE_LON   -65.483L		    /* descending node longitude [degrees.dec] at 09:45 local time */



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the World Reference System (LANDSAT 4).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND4_PATH_MIN_NUM	  1		    /* path number minimum  */
#define				LAND4_PATH_MAX_NUM	233		    /* path number maximum  */
#define				LAND4_ROW_MIN_NUM	  1		    /* row  number minimum  */
#define				LAND4_ROW_MAX_NUM	248		    /* row  number maximum  */

#define				LAND4_ASCNODE_ROWNUM	184		    /* ascending node row number */

#define				LAND4_LAUNCH_YEAR      1982		    /* launch year	    */
#define				LAND4_LAUNCH_MONTH	  7		    /* launch month	    */
#define				LAND4_LAUNCH_DAY	 16		    /* launch day	    */
#define				LAND4_LAUNCH_HOURS	 10		    /* launch hours	    */
#define				LAND4_LAUNCH_MINUTES	  0		    /* launch minutes	    */
#define				LAND4_LAUNCH_SECONDS	  0		    /* launch seconds	    */
#define				LAND4_LAUNCH_MILSECS	  0		    /* launch milliseconds  */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 4).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND4_ORBIT_INCL	98.200L		    /* orbital inclination in degrees		    */
#define				LAND4_SAT_REV_PERIOD	98.900L		    /* nominal satellite revolution period [min]    */

#define				LAND4_DESCNODE_LON   -64.600L		    /* descending node longitude [degrees.dec] at 09:45 local time */



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the World Reference System (LANDSAT 5).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND5_PATH_MIN_NUM	  1		    /* path number minimum  */
#define				LAND5_PATH_MAX_NUM	233		    /* path number maximum  */
#define				LAND5_ROW_MIN_NUM	  1		    /* row  number minimum  */
#define				LAND5_ROW_MAX_NUM	248		    /* row  number maximum  */

#define				LAND5_ASCNODE_ROWNUM	184		    /* ascending node row number */

#define				LAND5_LAUNCH_YEAR      1984		    /* launch year	    */
#define				LAND5_LAUNCH_MONTH	  3		    /* launch month	    */
#define				LAND5_LAUNCH_DAY	  1		    /* launch day	    */
#define				LAND5_LAUNCH_HOURS	 10		    /* launch hours	    */
#define				LAND5_LAUNCH_MINUTES	 22		    /* launch minutes	    */
#define				LAND5_LAUNCH_SECONDS	 44		    /* launch seconds	    */
#define				LAND5_LAUNCH_MILSECS	  0		    /* launch milliseconds  */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 5).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND5_ORBIT_INCL		98.200L		/* orbital inclination in degrees				*/
#define				LAND5_SAT_REV_PERIOD		98.900L		/* nominal satellite revolution period [min]			*/

#define				LAND5_DESCNODE_LON		-64.600L	/* descending node longitude [degrees.dec] at 09:45 local time */



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the World Reference System (LANDSAT 6).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND6_PATH_MIN_NUM		1		    /* path number minimum  */
#define				LAND6_PATH_MAX_NUM		233		    /* path number maximum  */
#define				LAND6_ROW_MIN_NUM		1		    /* row  number minimum  */
#define				LAND6_ROW_MAX_NUM		248		    /* row  number maximum  */

#define				LAND6_ASCNODE_ROWNUM		184		    /* ascending node row number */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 6).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND6_ORBIT_INCL		98.200L		/* orbital inclination in degrees				*/
#define				LAND6_SAT_REV_PERIOD		98.900L		/* nominal satellite revolution period [min]			*/

#define				LAND6_DESCNODE_LON		-64.600L	/* descending node longitude [degrees.dec] at 09:45 local time */

#define				POLYNCOE				4

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 7).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LAND7_ORBIT_INCL		98.2098L		/* orbital inclination in degrees				*/

#define				LAND7_LAUNCH_YEAR      1999		    /* launch year	    */
#define				LAND7_LAUNCH_MONTH	  4		    /* launch month	    */
#define				LAND7_LAUNCH_DAY	 15		    /* launch day	    */
#define				LAND7_LAUNCH_HOURS	 18		    /* launch hours	    */
#define				LAND7_LAUNCH_MINUTES	 32		    /* launch minutes	    */
#define				LAND7_LAUNCH_SECONDS	  0		    /* launch seconds	    */
#define				LAND7_LAUNCH_MILSECS	  0		    /* launch milliseconds  */
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the satellite parameters (LANDSAT 4, 5, 6).			! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L4_OVER_ANGLE			419430.4	/* Landsat 4 overflow angle of the gyro data [arcsec]	    */
#define				L5_OVER_ANGLE			419430.4	/* Landsat 5 overflow angle of the gyro data [arcsec]	    */
#define				L6_OVER_ANGLE			511705.1	/* Landsat 6 overflow angle of the gyro data [arcsec]	    */


#define				L4_GYRO_RESET			8388608.0	/*  Landsat 4 gyro reset counts */
#define				L5_GYRO_RESET			8388608.0	/*  Landsat 5 gyro reset counts */
#define				L6_GYRO_RESET			10234102.0	/*  Landsat 6 gyro reset counts */
#define				L7_GYRO_RESET			8388608.0	/*  Landsat 7 gyro reset counts */

#define				L4_GYRO_OVER			4194304.0	/*  Landsat 4 gyro over counts */
#define				L5_GYRO_OVER			4194304.0	/*  Landsat 5 gyro over counts */
#define				L6_GYRO_OVER			5117051.0	/*  Landsat 6 gyro over counts */
#define				L7_GYRO_OVER			4194304.0	/*  Landsat 7 gyro over counts */

#define				L4_GYRO_EU_CONV			0.050		/*   Landsat 4 gyro ARCSEC per count */
#define				L5_GYRO_EU_CONV			0.050		/*   Landsat 5 gyro ARCSEC per count */
#define				L6_GYRO_EU_CONV			0.061		/*   Landsat 6 gyro ARCSEC per count */
#define				L7_GYRO_EU_CONV			0.061		/*   Landsat 7 gyro ARCSEC per count */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definition is necessary for the array ReqBand (mask).				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				NUM_OF_BANDS			8		/* 7 bands plus panchromatic band			    */	
#define				NUM_OF_DET			16		/* number of detectors for the 7 bands			    */
#define				MAX_BANDNUM				9
#define				MAX_DETNUM				32
#define				SWATHSMAXNUM			400		/* maximum number of swaths in a raw data file on disk	    */
#define				NUM_OF_LINES_PER_SWATH		16		/* number of lines per scan line			    */

#define				NUM_OF_LEVELS			256		/* number of coulored levels				    */
#define				NUM_OF_CONVCOEFF		6		/* number of conversion coeff.				    */

#define				QUANTVALUE		8
#define				QUANTLEVELS		2048

#define				NUM_OF_TMHK			32		/* number of house keeping data				    */
#define				NUM_OF_UNPACKSERWORD		48		/* number of unpacked serial word			    */
#define				NUM_OF_GYROINATTREC		64		/* number of gyro data in the EphemAtt record		    */
#define				NUM_OF_ADSINADSREC		1024		/* number of ads data in the Ads record			    */

#define				MAXNUMHKREC			9		/* number of ads data in the Ads record			    */
#define				MAXNUMEPHEMATTREC		15		/* number of ads data in the Ads record			    */
#define				MAXNUMRAWJITTERREC		30		/* number of ads data in the Ads record			    */

#define				MAX_LOST_SCAN_LINES		40		/* maximum number of lost scan lines accepted		    */

#define				Q_CAL_MAX			255
#define				Q_CAL_MIN			1
#define				LMIN				0
#define				LMAX				1

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Kalman Filter and Smoothing.																		! */
/* +----------------------------------------------------------------------------------------------------+ */
/* defines to show the size of matricies being used */
#define		M_SIZE		3
#define		N_SIZE		3

/* Define for worst case fyro data sizing */
#define MAX_GYRO	(NUM_GYRO_PER_MF*MAXMJF)


/* ******** DEFINES FOR NUMBER OF GYRO PER DRIFT AND EPA ****** */

/* number of gyro per quaternion */
#define NUM_GYRO_PER_EPA NUM_GYRO_PER_MF /* one quaternion per every 64 gyro = 
					   one quaternion per mf */

/* number of gyro per drift */
#define NUM_GYRO_PER_DRIFT (NUM_GYRO_PER_MF*4) /* one drift per every 256 gyro =
					     one drift/4 mf = one drift/cycle */

/* ******** DEFINFES FOR GYRO, DRIFT AND EPA OFFSETS ****** */

/* offset in gyro data for time syncing */
#define GYRO_OFFSET 0	/* start using the gyro data at the first sample */

/* time offset of drift relative to gyro points */
#define DRIFT_MF_START	2
			/* the first drift is 2 mf's (8.192 secs) behind the 
			   first gyro point (so the 3rd mf's drift matches
			   with the first gyro point). Also, drift values
			   are only valid in Major Frame Type 0. */

/* time offset of quaternion relative to gyro points */
#define EPA_MF_START  2
			/* the first EPA is 2 mf's (8.192 secs) behind the
			   first gyro point (so the 3rd mf's epa matches with
			   the first gyro point) */

/* Defines for filter noise parameters (could be put in system table) */
#define PNOISE_ATTRATE_SIGMA	1.0e-2	/* Attitude rate process noise in
					   radians per second per second  */
#define PNOISE_DRIFT_SIGMA	1.0e-5	/* Gyro drift rate process noise in
					   radians per second per second  */
#define	ONOISE_EPA_SIGMA	4.2	/* Quaternion observation noise in
					   arcsec  */
#define	ONOISE_GYRO_SIGMA	0.061/(GYRO_SAMP_RATE * 2.4494897428)
					/* Gyro observation noise (based on one
					    gyro count) in arcsec per second */
#define	ONOISE_DRIFT_SIGMA	5.0	/* Gyro drift observation noise in
					   arcsec per second		*/
#define	SNOISE_ATT_SIGMA	45.0	/* Initial attitude state standard
					   deviation in arcsec */
#define	SNOISE_ATTRATE_SIGMA	45.0	/* Initial attitude rate state standard
					   deviation in arcsec per second */
#define	SNOISE_DRIFT_SIGMA	5.0	/* Initial drift state standard
					   deviation in arcsec per second  */

#define				GYRO_SAMP_RATE	0.064	/* sec */

#define				ARC_SEC_TO_RAD			M_PI/(180.0*3600.0)		/* arc seconds to radians	*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the optical sensor parameters ( not for Landsat 7).				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				TM_IFOV_RAD			42.50e-6	/* TM istantaneous field of view in radians		    */	
#define				PAN_IFOV_RAD			21.25e-6	/* PAN istantaneous field of view in radians		    */	

#define				OFFSET_SCENE			100015.0	/* offset [m]						    */	
#define				PIXEL_SIZE				30.0		/* pixel size [m]					    */	
#define				CENTRE_PIXEL			3050		/* centre pixel index				    */	

#define				DWELL_TIME			9.6110000e-6	/* sampling time				[sec]	    */	
#define				DATA_RATE			84.9030000e+6	/* data rate 				[bits/sec]	    */
#define				SCAN_FREQ			5.3064375e+6	/* scan sampling frequency 1:16 data rate	[Hz]	   */

#define				FIRSTHALF_REF			161164		/* nominal first half scan time in counts   (FORWARD)  */	
#define				SECONDHALF_REF			161165		/* nominal second half scan time in counts  (FORWARD)  */
#define				ACTIVE_SCAN_TIME		60743.013e-6    /* nominal active scan time			[sec]	       */
#define				NOM_SCAN_TIME_MSEC		71.462		/* nominal total scan time 			[msec]	  		   */

#define				ADS_SAMPLE_TIME_MSEC		2.000		/* ADS sampling time 			[msec]		    */
#define				GYRO_SAMPLE_TIME_MSEC		64.000		/* Gyro sampling time 			[msec]		    */

#define				SWATH_ANGLE			0.26860617	/* nominal swath angle during the active scan time [rad]    */
										/* temporary: comes from the nominal value of 15.390 [deg]  */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the optical sensor parameters (  Landsat 7).				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				DWELL_TIME_L7			9.6109603e-6	/* sampling time				[sec]	    */	
#define				ETM_NOM_SCAN_TIME_MSEC		71.820		/* nominal total scan time 			[msec]	  		   */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the telemetry synchronization to the scan line.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				NUM_OF_ADS_FOR_SCAN_LINE	32

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the TM fixed or floating full scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				TMFS_LEVEL0_IMGRECLEN			6100    /* Thematic Mapper Full    Scene Level0    */	
#define				TMFS_LEVEL1_IMGRECLEN			6100    /* Thematic Mapper Full    Scene Level1    */	
#define				TMFS_LEVEL4_IMGRECLEN			6920    /* Thematic Mapper Full    Scene Level5    */	
#define				TMFS_LEVEL4_EFFECTIVEIMGRECLEN	6100    /* Thematic Mapper Full    Scene Level5    */	
#define				TMFS_LEVEL5_IMGRECLEN			6920    /* Thematic Mapper Full    Scene Level5    */	
#define				TMFS_LEVEL5_EFFECTIVEIMGRECLEN	6100    /* Thematic Mapper Full    Scene Level5    */	

#define				TMFS_CEOS_RECLEN				7020	

#define				TMFS_LEVEL0_NUMOFLINES		5760	
#define				TMFS_LEVEL1_NUMOFLINES		5760	
#define				TMFS_LEVEL4_NUMOFLINES		5760	
#define				TMFS_LEVEL5_NUMOFLINES		5760	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the TM fixed or floating quarter scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				TMQS_LEVEL0_IMGRECLEN			3150    /* Thematic Mapper Quarter Scene Level0    */	
#define				TMQS_LEVEL1_IMGRECLEN			3150    /* Thematic Mapper Quarter Scene Level1    */	

#define				TMQS_LEVEL4_IMGRECLEN			3500    /* Thematic Mapper Quarter Scene Level5    */	
#define				TMQS_LEVEL4_EFFECTIVEIMGRECLEN	3150    /* Thematic Mapper Full    Scene Level5    */	
#define				TMQS_LEVEL5_IMGRECLEN			3500    /* Thematic Mapper Quarter Scene Level5    */	
#define				TMQS_LEVEL5_EFFECTIVEIMGRECLEN	3150    /* Thematic Mapper Full    Scene Level5    */	

#define				TMQS_CEOS_RECLEN				3600	

#define				TMQS_LEVEL0_NUMOFLINES		2944	
#define				TMQS_LEVEL1_NUMOFLINES		2944	
#define				TMQS_LEVEL4_NUMOFLINES		2944	
#define				TMQS_LEVEL5_NUMOFLINES		2944	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the TM fixed or floating mini scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				TMMS_LEVEL0_IMGRECLEN			1834    /* Thematic Mapper Mini    Scene Level0   ???????? */
#define				TMMS_LEVEL1_IMGRECLEN			1834    /* Thematic Mapper Mini    Scene Level1   ???????? */
#define				TMMS_LEVEL4_IMGRECLEN			2184    /* Thematic Mapper Mini    Scene Level5    */	
#define				TMMS_LEVEL4_EFFECTIVEIMGRECLEN	1834    /* Thematic Mapper Full    Scene Level5    */	
#define				TMMS_LEVEL4_IMGRECLEN			2184    /* Thematic Mapper Mini    Scene Level5    */	
#define				TMMS_LEVEL4_EFFECTIVEIMGRECLEN	1834    /* Thematic Mapper Full    Scene Level5    */	
#define				TMMS_LEVEL5_IMGRECLEN			2184    /* Thematic Mapper Mini    Scene Level5    */	
#define				TMMS_LEVEL5_EFFECTIVEIMGRECLEN	1834    /* Thematic Mapper Full    Scene Level5    */	
#define				TMMS_LEVEL5_IMGRECLEN			2184    /* Thematic Mapper Mini    Scene Level5    */	
#define				TMMS_LEVEL5_EFFECTIVEIMGRECLEN	1834    /* Thematic Mapper Full    Scene Level5    */	

#define				TMMS_CEOS_RECLEN				2284	

#define				TMMS_LEVEL0_NUMOFLINES		1836	
#define				TMMS_LEVEL1_NUMOFLINES		1836	
#define				TMMS_LEVEL4_NUMOFLINES		1836	
#define				TMMS_LEVEL5_NUMOFLINES		1836	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the TM fixed or floating micro scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				TMMC_LEVEL4_IMGRECLEN			1188    /* Thematic Mapper Full    Scene Level5    */	
#define				TMMC_LEVEL4_EFFECTIVEIMGRECLEN	1000    /* Thematic Mapper Full    Scene Level5    */	
#define				TMMC_LEVEL5_IMGRECLEN			1188    /* Thematic Mapper Full    Scene Level5    */	
#define				TMMC_LEVEL5_EFFECTIVEIMGRECLEN	1000    /* Thematic Mapper Full    Scene Level5    */	

#define				TMMC_CEOS_RECLEN				1288	

#define				TMMC_LEVEL0_NUMOFLINES		836	
#define				TMMC_LEVEL1_NUMOFLINES		836	
#define				TMMC_LEVEL4_NUMOFLINES		836	
#define				TMMC_LEVEL5_NUMOFLINES		836	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the ETM fixed or floating full scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				ETMFS_LEVEL0_IMGRECLEN		12640   /* Panchromatic Thematic Mapper Full    Scene Level0    */	
#define				ETMFS_LEVEL1_IMGRECLEN		12640   /* Panchromatic Thematic Mapper Full    Scene Level1    */	
#define				ETMFS_LEVEL4_IMGRECLEN		13400   /* Panchromatic Thematic Mapper Full    Scene Level5    */	
#define				ETMFS_LEVEL5_IMGRECLEN		13400   /* Panchromatic Thematic Mapper Full    Scene Level5    */	

#define				ETMFS_LEVEL0_NUMOFLINES		11520	
#define				ETMFS_LEVEL1_NUMOFLINES		11520	
#define				ETMFS_LEVEL4_NUMOFLINES		11520	
#define				ETMFS_LEVEL5_NUMOFLINES		11520	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the ETM fixed or floating quarter scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				ETMQS_LEVEL0_IMGRECLEN		6320    /* Panchromatic Thematic Mapper Quarter Scene Level0    */	
#define				ETMQS_LEVEL1_IMGRECLEN		6320    /* Panchromatic Thematic Mapper Quarter Scene Level1    */	
#define				ETMQS_LEVEL4_IMGRECLEN		6920    /* Panchromatic Thematic Mapper Quarter Scene Level5    */	
#define				ETMQS_LEVEL5_IMGRECLEN		6920    /* Panchromatic Thematic Mapper Quarter Scene Level5    */	

#define				ETMQS_LEVEL0_NUMOFLINES		5888	
#define				ETMQS_LEVEL1_NUMOFLINES		5888	
#define				ETMQS_LEVEL4_NUMOFLINES		5888	
#define				ETMQS_LEVEL5_NUMOFLINES		5888	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the ETM fixed or floating mini scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				ETMMS_LEVEL0_IMGRECLEN		3334    /* Panchromatic Thematic Mapper Mini    Scene Level0    */	
#define				ETMMS_LEVEL1_IMGRECLEN		3334    /* Panchromatic Thematic Mapper Mini    Scene Level1    */	
#define				ETMMS_LEVEL4_IMGRECLEN		4000    /* Panchromatic Thematic Mapper Mini    Scene Level5    */	
#define				ETMMS_LEVEL5_IMGRECLEN		4000    /* Panchromatic Thematic Mapper Mini    Scene Level5    */	

#define				ETMMS_LEVEL0_NUMOFLINES		3670	
#define				ETMMS_LEVEL1_NUMOFLINES		3670	
#define				ETMMS_LEVEL4_NUMOFLINES		3670	
#define				ETMMS_LEVEL5_NUMOFLINES		3670	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions: dealing with array dimension (backward sw compatibility).				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				FULL_SCENE_LINE_LEN		TMFS_LEVEL5_IMGRECLEN	
#define				QUARTER_SCENE_LINE_LEN	TMQS_LEVEL5_IMGRECLEN	
#define				MINI_SCENE_LINE_LEN		TMMS_LEVEL5_IMGRECLEN	
#define				MICRO_SCENE_LINE_LEN	TMMC_LEVEL5_IMGRECLEN	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions: resampling step.									! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				LEVEL4_LINE_RESAMPLING_STEP		30.0
#define				LEVEL4_PIXEL_RESAMPLING_STEP	30.0
#define				LEVEL5_LINE_RESAMPLING_STEP		30.0
#define				LEVEL5_PIXEL_RESAMPLING_STEP	30.0

#define				NOMINAL_ADS_TEMP 	15.0		/* degrees C */
#define 			LOW_TRAN			2.0 		/* Low bound of transition area */
#define 			HIGH_TRAN			7.8125		/* High bound of transition area */
#define				SAMP_RATE			0.002		/* Sampling rate (seconds)		     */
#define 			F_MAX				100.0 		/* transition frequency (hertz) */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   End of code redefinition control.								! */
/* +----------------------------------------------------------------------------------------------------+ */
#endif
