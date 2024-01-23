
/*

	Copyright 2013-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/04/11 09:13:22  ricfer
	MSS_CALIB_SIZE and MSS_CALIB_GAP re-defined. MSS_EOS_POSMIN added.
	
	Revision 1.2  2013/02/26 17:03:57  ricfer
	Added new MSS defines. NUM_OF_LEVELS renamed in NUM_OF_MSS_LEVELS.
	
	Revision 1.1  2013/01/16 15:07:18  ricfer
	Moved here from libMDPSMssProc.
	
	Revision 1.1  2013/01/16 16:04:41  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _mssConstants_h_
#define _mssConstants_h_


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


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Nominal orbital period for the different missions.						! */
/* +----------------------------------------------------------------------------------------------------+ */
#define		    T1nominalMins	103.266		/* Landsat 1 nominal period [min]			*/
#define		    T2nominalMins	103.2665	/* Landsat 2 nominal period [min]			*/
#define		    T3nominalMins	103.266		/* Landsat 3 nominal period [min]			*/
#define		    T4nominalMins	98.88412	/* Landsat 4 nominal period [min]			*/
#define		    T5nominalMins	98.88412	/* Landsat 5 nominal period [min]			*/
#define		    T6nominalMins	98.88412	/* Landsat 6 nominal period [min]			*/


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


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definition is necessary for the array ReqBand (mask).				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSS_NUM_OF_BANDS			4		/* number of bands for the MSS sensor			    */	
#define				MSS_NUM_OF_DET				6		/* number of detectors per each band			    */
#define				MSS_NUM_OF_LINES_PER_SWATH	6		/* number of lines per MSS swath			    */
#define				MSS_NUM_OF_CAL_WEDGE		6
#define				MSS_NUM_OF_CAL_LINES		200

#define				MSS_LINE_SIZE			3240	/* line size			*/
#define				MSS_CALIB_SIZE			1560	/* 1300 MSS calibration length	*/
#define				MSS_CALIB_GAP			800  /* 860 */
#define				MSS_Q_COMPR_MAX			63
#define				MSS_Q_UNCOMPR_MAX		127

#define				MSS_EOS_POSMIN			(3218 - 80)

#define				MSS_Q_CAL_MAX			255
#define				MSS_Q_CAL_MIN			1
#define				MSS_DESTR_BINS			1024 // 1024

#define				MSS_DATA_RATE			15.0626304e+6	/* data rate 				[bits/sec]	    */

#define				SWATHSMAXNUM			400		/* maximum number of swaths in a raw data file on disk	    */
#define				NUM_OF_MSS_LEVELS		256		/* number of coulored levels				    */
#define				NUM_OF_CONVCOEFF		6		/* number of conversion coeff.				    */
#define				MAX_MSS_DN_COUNT		NUM_OF_MSS_LEVELS

#define				MSS_MAX_LOST_SCAN_LINES	40		/* maximum number of lost scan lines accepted		    */

#define				NUM_OF_GRID_POINTS		16		/* number of grid points (atmospheric corrections)	    */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the MSS optical sensor parameters Landsat 1.		! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L1_MSS_X_IFOV_RAD		62.183e-6   /*86.184e-6*/   /* MSS istantaneous field of view in radians (interpixel)   */	
#define				L1_MSS_Y_IFOV_RAD	        86.184e-6   /*117.255e-6*/  /* MSS istantaneous field of view in radians (interline)    */	

#define				L1_MSS_X_PIXEL_SIZE		57.0	/**/		/* pixel size [m] (interpixel)	*/	
#define				L1_MSS_Y_PIXEL_SIZE	        79.0	/**/		/* pixel size [m] (interline)	*/	

#define				L1_MSS_SWATH_ANGLE		0.20176 /*0.2603*/	/* nominal swath angle during the active scan time [rad]    */
											/* comes from the nominal value of 14.914 [deg]		    */

#define				L1_NOM_ACTIVE_SCAN_TIME		32.300		/* nominal active scan time			[msec]	    */
#define				L1_NOM_SWEEP_SCAN_TIME		73.420		/* nominal time between consecutive sweeps	[msec]	    */

#define				L1_NOM_EOS_IN_PIXELS		  3220		/* nominal end of scan position in pixels		    */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the pre-launch mirror scan profile.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L1_MSS_HARM_AMPLITUDE_RAD	 0.233391	/* harmonic amplitude of the actual mirror scan profile [rad]	*/
#define				L1_MSS_BETA_DAMPING		 0.00739	/* damping constant [1 / sec]					*/
#define				L1_MSS_MIRROR_FREQUENCY		17.4987		/* mirror frequency [rad / sec]					*/
#define				L1_MSS_CENTRE_TIME_OFFSET      -16.148		/* start time for scan  [msec]					*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the MSS optical sensor parameters Landsat 2.		! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L2_MSS_X_IFOV_RAD		62.183e-6   /*86.184e-6*/	/* MSS istantaneous field of view in radians (interpixel)   */	
#define				L2_MSS_Y_IFOV_RAD		86.184e-6  /*117.255e-6*/	/* MSS istantaneous field of view in radians (interline)    */	

#define				L2_MSS_X_PIXEL_SIZE		57.0 /*57.0*/	/* pixel size [m] (interpixel)				    */	
#define				L2_MSS_Y_PIXEL_SIZE		79.0 /*82.7*/	/* pixel size [m] (interline)				    */	

#define				L2_MSS_SWATH_ANGLE		0.20176 /*0.2603*/		/* nominal swath angle during the active scan time [rad]    */
										/* comes from the nominal value of 14.914 [deg]		    */

#define				L2_NOM_ACTIVE_SCAN_TIME		32.300		/* nominal active scan time			[msec]	    */
#define				L2_NOM_SWEEP_SCAN_TIME		73.420		/* nominal time between consecutive sweeps	[msec]	    */

#define				L2_NOM_EOS_IN_PIXELS		  3220		/* nominal end of scan position in pixels		    */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the pre-launch mirror scan profile.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L2_MSS_HARM_AMPLITUDE_RAD	 0.233391	/* harmonic amplitude of the actual mirror scan profile [rad]	*/
#define				L2_MSS_BETA_DAMPING		 0.00739	/* damping constant [1 / sec]					*/
#define				L2_MSS_MIRROR_FREQUENCY		17.4987		/* mirror frequency [rad / sec]					*/
#define				L2_MSS_CENTRE_TIME_OFFSET      -16.148		/* start time for scan  [msec]					*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the MSS optical sensor parameters Landsat 3.		! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L3_MSS_X_IFOV_RAD		62.183e-6 /*80.817e-6*/	/* MSS istantaneous field of view in radians (interpixel)   */	
#define				L3_MSS_Y_IFOV_RAD		86.184e-6  /*117.255e-6*/	/* MSS istantaneous field of view in radians (interline)    */	

#define				L3_MSS_X_PIXEL_SIZE		57.0 /*57.0*/	/* pixel size [m] (interpixel)				    */	
#define				L3_MSS_Y_PIXEL_SIZE		79.0 /*82.7*/	/* pixel size [m] (interline)				    */	

#define				L3_MSS_SWATH_ANGLE		0.20176 /*0.2603*/		/* nominal swath angle during the active scan time [rad]    */
										/* comes from the nominal value of 14.914 [deg]		    */

#define				L3_NOM_ACTIVE_SCAN_TIME		32.300		/* nominal active scan time			[msec]	    */
#define				L3_NOM_SWEEP_SCAN_TIME		73.420		/* nominal time between consecutive sweeps	[msec]	    */

#define				L3_NOM_EOS_IN_PIXELS		  3220		/* nominal end of scan position in pixels		    */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the pre-launch mirror scan profile.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L3_MSS_HARM_AMPLITUDE_RAD	 0.233391	/* harmonic amplitude of the actual mirror scan profile [rad]	*/
#define				L3_MSS_BETA_DAMPING		 0.00739	/* damping constant [1 / sec]					*/
#define				L3_MSS_MIRROR_FREQUENCY		17.4987		/* mirror frequency [rad / sec]					*/
#define				L3_MSS_CENTRE_TIME_OFFSET      -16.148		/* start time for scan  [msec]					*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the MSS optical sensor parameters Landsat 4.		! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L4_MSS_X_IFOV_RAD		 80.817e-6	/* MSS istantaneous field of view in radians (interpixel)   */	
#define				L4_MSS_Y_IFOV_RAD		117.255e-6	/* MSS istantaneous field of view in radians (interline)    */	

#define				L4_MSS_X_PIXEL_SIZE		57.0		/* pixel size [m] (interpixel)				    */	
#define				L4_MSS_Y_PIXEL_SIZE		82.7		/* pixel size [m] (interline)				    */	

#define				L4_MSS_SWATH_ANGLE		0.2603		/* nominal swath angle during the active scan time [rad]    */
										/* comes from the nominal value of 14.914 [deg]		    */

#define				L4_NOM_ACTIVE_SCAN_TIME		32.300		/* nominal active scan time			[msec]	    */
#define				L4_NOM_SWEEP_SCAN_TIME		73.420		/* nominal time between consecutive sweeps	[msec]	    */

#define				L4_NOM_EOS_IN_PIXELS		  3240		/* nominal end of scan position in pixels		    */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the pre-launch mirror scan profile.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L4_MSS_HARM_AMPLITUDE_RAD	 0.233391	/* harmonic amplitude of the actual mirror scan profile [rad]	*/
#define				L4_MSS_BETA_DAMPING		 0.00739	/* damping constant [1 / sec]					*/
#define				L4_MSS_MIRROR_FREQUENCY		17.4987		/* mirror frequency [rad / sec]					*/
#define				L4_MSS_CENTRE_TIME_OFFSET      -16.148		/* start time for scan  [msec]					*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   The following definitions deals with the MSS optical sensor parameters Landsat 5.		! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L5_MSS_X_IFOV_RAD		 80.817e-6	/* MSS istantaneous field of view in radians (interpixel)   */	
#define				L5_MSS_Y_IFOV_RAD		117.255e-6	/* MSS istantaneous field of view in radians (interline)    */	

#define				L5_MSS_X_PIXEL_SIZE		57.0		/* pixel size [m] (interpixel)				    */	
#define				L5_MSS_Y_PIXEL_SIZE		82.7		/* pixel size [m] (interline)				    */	

#define				L5_MSS_SWATH_ANGLE		0.2605		/* nominal swath angle during the active scan time [rad]    */
										/* comes from the nominal value of 14.9255 [deg]	    */

#define				L5_NOM_ACTIVE_SCAN_TIME		32.200		/* nominal active scan time			[msec]	    */
#define				L5_NOM_SWEEP_SCAN_TIME		73.420		/* nominal time between consecutive sweeps	[msec]	    */

#define				L5_NOM_EOS_IN_PIXELS		  3233		/* nominal end of scan position in pixels		    */

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the pre-launch mirror scan profile.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				L5_MSS_HARM_AMPLITUDE_RAD	 0.2379662	/* harmonic amplitude of the actual mirror scan profile [rad]	*/
#define				L5_MSS_BETA_DAMPING		 0.00695558	/* damping constant [1 / sec]					*/
#define				L5_MSS_MIRROR_FREQUENCY		17.22		/* mirror frequency [rad / sec]					*/
#define				L5_MSS_CENTRE_TIME_OFFSET      -16.098		/* start time for scan  [msec]					*/



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the MSS fixed or floating full scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSS_FS_LEVEL0_IMGRECLEN		    3200    /* MultiSpectral Scanner Full    Scene Level0    */	
#define				MSS_FS_LEVEL1_IMGRECLEN		    3200    /* MultiSpectral Scanner Full    Scene Level1    */	
#define				MSS_FS_LEVEL4_IMGRECLEN		    3800    /* MultiSpectral Scanner Full    Scene Level4    */	
#define				MSS_FS_LEVEL5_IMGRECLEN		    3800    /* MultiSpectral Scanner Full    Scene Level5    */	
#define				MSS_FS_LEVEL5_EFFECTIVEIMGRECLEN    3200    /* MultiSpectral Scanner Full    effective reclen    */	

#define				MSS_FS_CEOS_RECLEN		    3900	

#define				MSS_FS_LEVEL0_NUMOFLINES	    2288	
#define				MSS_FS_LEVEL1_NUMOFLINES	    2288	
#define				MSS_FS_LEVEL4_NUMOFLINES	    2288    /*3200*/	
#define				MSS_FS_LEVEL5_NUMOFLINES	    3500    /*3200*/	

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the MSS fixed or floating quarter scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSS_QS_LEVEL0_IMGRECLEN		    1680    /* MultiSpectral Scanner Quarter Scene Level0    */	
#define				MSS_QS_LEVEL1_IMGRECLEN		    1680    /* MultiSpectral Scanner Quarter Scene Level1    */	
#define				MSS_QS_LEVEL4_IMGRECLEN		    1900    /* MultiSpectral Scanner Quarter Scene Level5    */	
#define				MSS_QS_LEVEL5_IMGRECLEN		    1900    /* MultiSpectral Scanner Quarter Scene Level5    */	
#define				MSS_QS_LEVEL5_EFFECTIVEIMGRECLEN    1680    /* MultiSpectral Scanner Quarter Scene Level5    */	

#define				MSS_QS_CEOS_RECLEN		    2000	

#define				MSS_QS_LEVEL0_NUMOFLINES	    1208	
#define				MSS_QS_LEVEL1_NUMOFLINES	    1208	
#define				MSS_QS_LEVEL4_NUMOFLINES	    1208    /*1680*/	
#define				MSS_QS_LEVEL5_NUMOFLINES	    1208    /*1680*/	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions dealing with the MSS fixed or floating mini scene size.				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSS_MS_LEVEL0_IMGRECLEN		     964    /* MultiSpectral Scanner Mini    Scene Level0    */
#define				MSS_MS_LEVEL1_IMGRECLEN		     964    /* MultiSpectral Scanner Mini    Scene Level1    */
#define				MSS_MS_LEVEL4_IMGRECLEN		    1100    /* MultiSpectral Scanner Mini    Scene Level5    */	
#define				MSS_MS_LEVEL5_IMGRECLEN		    1100    /* MultiSpectral Scanner Mini    Scene Level5    */	
#define				MSS_MS_LEVEL5_EFFECTIVEIMGRECLEN     964    /* MultiSpectral Scanner Mini    Scene Level5    */	

#define				MSS_MS_CEOS_RECLEN		    1200	

#define				MSS_MS_LEVEL0_NUMOFLINES	     664	
#define				MSS_MS_LEVEL1_NUMOFLINES	     664	
#define				MSS_MS_LEVEL4_NUMOFLINES	     664    /*1000*/
#define				MSS_MS_LEVEL5_NUMOFLINES	     664    /*1000*/	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions: dealing with array dimension (backward sw compatibility).				! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSS_FULL_SCENE_LINE_LEN		    MSS_FS_LEVEL5_IMGRECLEN	
#define				MSS_QUARTER_SCENE_LINE_LEN	    MSS_QS_LEVEL5_IMGRECLEN	
#define				MSS_MINI_SCENE_LINE_LEN		    MSS_MS_LEVEL5_IMGRECLEN	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions: resampling step.									! */
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSS_LEVEL5_LINE_RESAMPLING_STEP	    82.7	/*57.0*/
#define				MSS_LEVEL5_PIXEL_RESAMPLING_STEP    57.0

#define				MSS_NUM_OF_OVERLAPPED_LINES	54	/* number of overlapped lines (7 scans x 6) for the fixed quarters */



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   End of code redefinition control.								! */
/* +----------------------------------------------------------------------------------------------------+ */


/*+----------------------------------------------------------------------------------------------------+ */
/* Resampling parameters
/* +----------------------------------------------------------------------------------------------------+ */
#define				MSSGRID_SAMPLE_OFFSET		1550 // 3000
#define             MSSGRID_LINE_OFFSET			1150 // 1150 
#define 			MSSGRID_NUM_SCAN_INBUF5		395
#define 			MSSGRID_SCAN_OVERLAP_5		390
#define 			MSS_NUM_DIRECTIONS			1
#define 			MSSGRID_KERNEL_SIZE			4	  //Resampling kernel size
#define 			MSSGRID_LINES_IN_KERNEL		4
#define 			MSSGRID_SAMPLES_IN_KERNEL	6
#define 			MSSGRID_SUBPIXEL_STEPS 		32
#define 			MSSGRID_MAG_GAP				30 	  // 6
#define 			MSSGRID_ALFA				-0.5
#define 			MSSGRID_ROUND				.0156250  // 1/32*0.5 roundoff error
#define				MSSGRID_NN_MAX_GAP			2
#define				MSSGRID_CC_MAX_GAP			2
#define 			MSSGRID_LINES_IN_EXTEND_BUF	5
#define				MSSGRID_MAX_ITER			100 
/* defines the maximum number of iterations to try before failing */
#define 			MSS_HISTORY_TO_KEEP			5

#endif
