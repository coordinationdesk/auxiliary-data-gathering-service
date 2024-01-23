/*
 *	This include file contains all the structures and tables definition for the Direct Ingestion 
 *	of the following satellites:
 *	    NASA    Landsat Thematic Mapper 	(TM)	Missions:    4, 5
 *
 *	$Id$
 *
 */

#ifndef __TM_STRUCTURES_H__
#define __TM_STRUCTURES_H__

#ifdef __cplusplus
	extern "C" {
#endif
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char const * TMStructuresId="@(#) Advanced Computer Systems: TM_Structures.h 1.5 04/01/30";

#define	Tm_Missions	    	3			    /* Number of Landsat Tm Missions			*/
#define	TmMinFrameSize	    102			    /* Number of Byte in a Minor Frame			*/
#define	TmQlMinFrameSize    30			    /* Number of Byte in a Quick LookMinor Frame	*/
#define	TmSyncWordSize	    4			    /* Number of Byte with Sync Word		    	*/
#define	TmSincPcdB6Size	    6			    /* Number of Byte with Sync Word, Pcd and Band 6	*/
#define TmMajorFrameSize    7500		    /* Nominal Major Frame Size				*/
#define TmSafeMajorFrameSize	7600		    /* Nominal Major Frame Size				*/
#define	TmNominalVideo	    6340		    /* Nominal Video Portion of a Major Frame (max)	*/
#define	TmSatelliteTime	    7			    /* Number of Minor Frame Containing the Time	*/
#define	TmVideoPortion	    6400		    /* Video Portion of a Major Frame			*/
#define	TmCalibPortion	    960			    /* Calibration Portion of a Major Frame		*/
#define	TmEosPortion	    100			    /* Eos, Fhserr & Shserr Portion of a Major Frame	*/
#define	TmQlSkipPortion	    850			    /* Quick Look Portion of a Major Frame to Skip	*/
#define	TmPreamblePortion   5			    /* Number of Minor Frame with Preamble Patern	*/
#define	TmMfSearchWindow    8			    /* Minor Frame Search Windows			*/
#define	TmMjSearchWindow    4			    /* Major Frame Search Windows			*/
#define	TmMjMaxErrors	    5			    /* Major Frame Max Number Of Bit Errors	*/

#define	Tm_Auxiliary_Size   10			    /* Thematic Mapper Auxiliary Size (Enerter FS Like)	*/
#define	Tm_Pcd_Requested_Swath 2000		    /* Number of Swath Covering Two Complete Pcd Cycle	*/
#define	Tm_Pcd_Preamble_Size   300		    /* Number of Swath Covering Two Complete Pcd Cycle	*/

#define	Tm_FirstScanHalf    161164		    /* Nominal # Clocks for the First Half Scan		*/
#define	Tm_SecondScanHalf   161165		    /* Nominal # Clocks for the Second Half Scan	*/
#define	Tm_Scan_Time	    71.462		    /* Nominal Scan Time				*/
#define	MAX_Tm_Scan_Time    107.193		    /* Maximum accepted Scan Time in milliseconds (3/2) */
#define	MIN_Tm_Scan_Time    35.731		    /* Minimum accepted Scan Time in milliseconds (1/2)	*/

#define L5_BUMPER_MODE_STARTJD50 19083.0	/* Start L5 Bumper mode JD50		*/
#define L5_BUMPER_MODE_OFFSET	(73 * TmMinFrameSize)

#define Tm_Scan_Line	    4.4663		    /* Line Scan Time in Millisecond			*/

#define	Tm_ifov_x	    .0000425		    /* Istantaneous field of view in x direction ********/
#define	Tm_ifov_y	    .0000425		    /* Istantaneous field of view in y direction ********/

#define	Tm_Pcd_Buf_Size		7680			/* Nominal number of PCD samples per swath on SONY	*/

/******* Added by V.D. Spaventa *******/
#define Tm_sensors	100			    /* Number of Total Tm sensor (all bands)		*/
#define Tm_B6_sensors	4			    /* Number of Band 6 Tm sensor		*/
#define	Tm_Extra_Pcd	90			    /* Extra "Pcd Swath at the start/end of video data	*/
#define Tm_FR_swath	400			    /* Number of swath  to load to disk			*/
#define	Tm_Aux_Buf_Size	20			    /* Nominal number of Auxiliary values per swath	*/
#define Tm_line_size	6400			    /* Thematic Mapper line length			*/
#define Tm_Calib_size	960			    /* Thematic Mapper calibration length		*/
/**************************************/

#define	TmMF_Sync	    0x023716D1		    /* Bits  0 - 31 of Tm Sync Word			*/
#define	TmMF_Sync_1	    0x0237		    /* Bits  0 - 31 of Tm Sync Word			*/
#define	TmMF_Sync_2	    0x16D1		    /* Bits  0 - 31 of Tm Sync Word			*/
#define	TmMF_Byte_1	    0x02		    /* Bits  0 - 31 of Tm Sync Word			*/
#define	TmMF_Byte_2	    0x37		    /* Bits  0 - 31 of Tm Sync Word			*/
#define	TmMF_Byte_3	    0x16		    /* Bits  0 - 31 of Tm Sync Word			*/
#define	TmMF_Byte_4	    0xD1		    /* Bits  0 - 31 of Tm Sync Word			*/
#define Tm_FR_lines	    5760		    /* Number of lines in a Tm full frame	 ********/
#define	Tm_QL_Rt_size	    500			    /* Tm Quick Look Line Size During Phase 1		*/
#define	Tm_QL_size	    1000		    /* Tm Quick Look Line Size During Phase 2		*/
#define	Tm_QL_line_size	    6000		    /* Video Portion used for the Quick Look Production	*/
#define	Tm_QL_lines	    3			    /* Max # of Quick Look Lines Extracted per Swath	*/
#define	Tm_bands	    7			    /* Number of Thematic Mapper Spectral Bands		*/
#define	Tm_QL_sensors	    16			    /* Number of Thematic Mapper Sensors per Band	*/
#define	Tm_lost_pixel	    (160 * TmMinFrameSize)  /* Number of Tm Pixels not Covered by All Bands	*/
#define	Tm_Ql_lost_pixel    (160 * TmQlMinFrameSize)/* Number of Tm Pixels not Covered by All Bands	*/
#define	Tm_QL_sample	    6			    /* Tm Quick Look Subsample				*/
#define	Tm_Max_Lost_Lines   20			    /* Max Number of Lines Inserted for a Data Loss	*/

#define	Tm_band1_point	    6			    /* Minor Frame Position of First Band 1 Pixel	*/
#define	Tm_band2_point	    7			    /* Minor Frame Position of First Band 2 Pixel	*/
#define	Tm_band3_point	    8			    /* Minor Frame Position of First Band 3 Pixel	*/
#define	Tm_band4_point	    9			    /* Minor Frame Position of First Band 4 Pixel	*/
#define	Tm_band5_point	    10			    /* Minor Frame Position of First Band 5 Pixel	*/
#define	Tm_band6_point	    4			    /* Minor Frame Position of First Band 6 Pixel	*/
#define	Tm_band7_point	    11			    /* Minor Frame Position of First Band 7 Pixel	*/

#define	Tm_band1_offset	    (85 * TmMinFrameSize)   /* Offset from Center of Scanning for Band 1	*/
#define	Tm_band2_offset	    (60 * TmMinFrameSize)   /* Offset from Center of Scanning for Band 2	*/
#define	Tm_band3_offset	    (35 * TmMinFrameSize)   /* Offset from Center of Scanning for Band 3	*/
#define	Tm_band4_offset	    (10 * TmMinFrameSize)   /* Offset from Center of Scanning for Band 4	*/
#define	Tm_band5_offset	    (-61 * TmMinFrameSize)  /* Offset from Center of Scanning for Band 5	*/
#define	Tm_band6_offset	    (-95 * TmMinFrameSize)  /* Offset from Center of Scanning for Band 6	*/
#define	Tm_band7_offset	    (-35 * TmMinFrameSize)  /* Offset from Center of Scanning for Band 7	*/

#define	Tm_Ql_band1_offset  (85 * TmQlMinFrameSize) /* Offset from Center of Scanning for Band 1	*/
#define	Tm_Ql_band2_offset  (60 * TmQlMinFrameSize) /* Offset from Center of Scanning for Band 2	*/
#define	Tm_Ql_band3_offset  (35 * TmQlMinFrameSize) /* Offset from Center of Scanning for Band 3	*/
#define	Tm_Ql_band4_offset  (10 * TmQlMinFrameSize) /* Offset from Center of Scanning for Band 4	*/
#define	Tm_Ql_band5_offset  (-61 * TmQlMinFrameSize)/* Offset from Center of Scanning for Band 5	*/
#define	Tm_Ql_band6_offset  (-95 * TmQlMinFrameSize)/* Offset from Center of Scanning for Band 6	*/
#define	Tm_Ql_band7_offset  (-35 * TmQlMinFrameSize)/* Offset from Center of Scanning for Band 7	*/

/** SONY ID1 pointers (from landsat_tm.h) **/
#define Tm_ID1_band1_point	4	/* Pointer to the first sensor of band 1	*/
#define Tm_ID1_band2_point	20	/* Pointer to the first sensor of band 2	*/
#define Tm_ID1_band3_point	36	/* Pointer to the first sensor of band 3	*/
#define Tm_ID1_band4_point	52	/* Pointer to the first sensor of band 4	*/
#define Tm_ID1_band5_point	68	/* Pointer to the first sensor of band 5	*/
#define Tm_ID1_band6_point	0	/* Pointer to the first sensor of band 6	*/
#define Tm_ID1_band7_point	84	/* Pointer to the first sensor of band 7	*/

#define Tm_ID1_band1_offset	85	/* Offset from band 1 and the Tm field center	*/
#define Tm_ID1_band2_offset	60	/* Offset from band 2 and the Tm field center	*/
#define Tm_ID1_band3_offset	35	/* Offset from band 3 and the Tm field center	*/
#define Tm_ID1_band4_offset	10	/* Offset from band 4 and the Tm field center	*/
#define Tm_ID1_band5_offset	-61	/* Offset from band 5 and the Tm field center	*/
#define Tm_ID1_band6_offset	-95	/* Offset from band 6 and the Tm field center	*/
#define Tm_ID1_band7_offset	-35	/* Offset from band 7 and the Tm field center	*/

#define	Tm_ID1_lost_pixel	160 /* Number of Tm Pixels not Covered by All Bands	*/

#define Tm_Thermal_QL_conf 5			/* Configuration for thermal QL */

#define	Tm_QL_red	    7			    /* Default Band on Red for Quick Look		*/
#define	Tm_QL_green	    5			    /* Default Band on Green for Quick Look		*/
#define	Tm_QL_blue	    3			    /* Default Band on Blue for Quick Look		*/

#define	Tm_Tc_QL_red	    3			    /* True Color Band on Red for Quick Look		*/
#define	Tm_Tc_QL_green	    2			    /* True Color Band on Green for Quick Look		*/
#define	Tm_Tc_QL_blue	    1			    /* True Color Band on Blue for Quick Look		*/
	

/*
 *	Thematic Mapper Minor Frame Structure
 */
	typedef struct	{
	    u_char
		Sync_Word[4], 
		Band6, 
		Pcd, 
		Band1_Sensor01, Band2_Sensor01, Band3_Sensor01, Band4_Sensor01, Band5_Sensor01, Band7_Sensor01, 
		Band1_Sensor03, Band2_Sensor03, Band3_Sensor03, Band4_Sensor03, Band5_Sensor03, Band7_Sensor03,
		Band1_Sensor05, Band2_Sensor05, Band3_Sensor05, Band4_Sensor05, Band5_Sensor05, Band7_Sensor05, 		
		Band1_Sensor07, Band2_Sensor07, Band3_Sensor07, Band4_Sensor07, Band5_Sensor07, Band7_Sensor07, 		
		Band1_Sensor09, Band2_Sensor09, Band3_Sensor09, Band4_Sensor09, Band5_Sensor09, Band7_Sensor09, 		
		Band1_Sensor11, Band2_Sensor11, Band3_Sensor11, Band4_Sensor11, Band5_Sensor11, Band7_Sensor11, 		
		Band1_Sensor13, Band2_Sensor13, Band3_Sensor13, Band4_Sensor13, Band5_Sensor13, Band7_Sensor13,
		Band1_Sensor15, Band2_Sensor15, Band3_Sensor15, Band4_Sensor15, Band5_Sensor15, Band7_Sensor15, 
	
		Band1_Sensor02, Band2_Sensor02, Band3_Sensor02, Band4_Sensor02, Band5_Sensor02, Band7_Sensor02, 
		Band1_Sensor04, Band2_Sensor04, Band3_Sensor04, Band4_Sensor04, Band5_Sensor04, Band7_Sensor04,
		Band1_Sensor06, Band2_Sensor06, Band3_Sensor06, Band4_Sensor06, Band5_Sensor06, Band7_Sensor06, 		
		Band1_Sensor08, Band2_Sensor08, Band3_Sensor08, Band4_Sensor08, Band5_Sensor08, Band7_Sensor08, 		
		Band1_Sensor10, Band2_Sensor10, Band3_Sensor10, Band4_Sensor10, Band5_Sensor10, Band7_Sensor10, 		
		Band1_Sensor12, Band2_Sensor12, Band3_Sensor12, Band4_Sensor12, Band5_Sensor12, Band7_Sensor12, 		
		Band1_Sensor14, Band2_Sensor14, Band3_Sensor14, Band4_Sensor14, Band5_Sensor14, Band7_Sensor14,
		Band1_Sensor16, Band2_Sensor16, Band3_Sensor16, Band4_Sensor16, Band5_Sensor16, Band7_Sensor16; 	
	}Tm_FR_Minor_Frame;

/*
 *	Thematic Mapper Quick Look Intermediate Minor Frame Structure
 */
	typedef struct	{
	    u_int
		Sync_Word;
		
	    u_char
		Band6, 
		Pcd, 
		Band1_Sensor01, Band2_Sensor01, Band3_Sensor01, Band4_Sensor01, Band5_Sensor01, Band7_Sensor01, 
		Band1_Sensor05, Band2_Sensor05, Band3_Sensor05, Band4_Sensor05, Band5_Sensor05, Band7_Sensor05, 		
		Band1_Sensor09, Band2_Sensor09, Band3_Sensor09, Band4_Sensor09, Band5_Sensor09, Band7_Sensor09, 		
		Band1_Sensor13, Band2_Sensor13, Band3_Sensor13, Band4_Sensor13, Band5_Sensor13, Band7_Sensor13;
	}Tm_QL_Minor_Frame;
	
/*
 *	Thematic Mapper Satellite Time Structure
 */
	typedef struct	{
	    int
		Day,
		Hours, 
		Minutes, 
		Seconds, 
		Milliseconds;
	u_short
		Millisec_Frac, 
		Satellite_Id;
	    int
		Fhserr, 
		Shserr,  
		Scan_Direction, 
		Line_Length, 
		Major_Frame_Length;
#ifndef sgi
		unsigned char __padding [4] ;	/* required to align double below */
#endif
	    double
		Satellite_Time;
	}Tm_Satellite_Scan_Data;
	
	typedef struct	{
	    u_int
		Sync_Word;
	    u_char
		Band6, 
		Pcd, 
		Video_Data[16][6];
	}Tm_Minor_Frame;
	
/*
 *	Thematic Mapper Major Frame Structure
 */
	typedef struct	{

	    Tm_Satellite_Scan_Data
		Satellite_Data;

	    Tm_FR_Minor_Frame
		Minor_Frame[TmVideoPortion + TmCalibPortion];
	
	    u_char
		Preamble_Pcd_Buffer[Tm_Pcd_Preamble_Size];
#ifndef sgi
		unsigned char __padding [4] ;	/* required to force size multiple by 8 */
#endif
	}Tm_Major_Frame;
	
/*
 *	ECL Output Thematic Mapper Major Frame Structure
 */
	typedef struct	{

	    Tm_FR_Minor_Frame
		Minor_Frame[TmSafeMajorFrameSize];
	    
	}Ecl_Tm_Major_Frame;
	
/*
 *	Intermediate Thematic Mapper Major Frame Structure
 */
	typedef struct	{
	    Tm_FR_Minor_Frame
		Satellite_Time[TmSatelliteTime];
		
	    Tm_QL_Minor_Frame
		Video_Data[TmNominalVideo - (TmEosPortion / 2)];
		
	    Tm_FR_Minor_Frame
		Eos_Data[TmEosPortion];
		
	    Tm_Satellite_Scan_Data
		Satellite_Data;
	}Intermediate_Tm_Swath;
		
	
/*
 *	Thematic Mapper Full Resolution Quick Look (for Reproduction)
 */
	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[Tm_QL_size * RGB_Size];
	    u_int
		Filler[15];
	}Rp_Generic_Ql_Tm;
	
/*
 *	Tables for the Description of a Track on Maps
 */
	typedef struct	{
	    double
		Lat_0,		/* Latitude of the west most point of the scan	*/
		Long_0,		/* Longitude of the west most point of the scan	*/
		Lat_1,		/* Latitude of the aest most point of the scan	*/
		Long_1, 	/* Longitude of the west most point of the scan	*/
		Center_Frame_T;	/* Time of the Center Frame			*/
	} Tm_Track_Descriptor;
	

/* 
 *  Structures used to convert from Sony D1 format to DLT WILMA
 */
 
/*
 *	Thematic Mapper Full Resolution Structures on Disk
 */
	typedef struct Tm_Full_Resolution_on_Disk
	{
		unsigned char
				Tm_data[Tm_sensors][Tm_line_size + Tm_Calib_size];
		unsigned short
				Sync_Flag[4];
		unsigned short
				Auxiliary[Tm_Aux_Buf_Size / 2];
	} Tm_Full_Resolution_on_Disk;

/*
 *	Thematic Mapper Auxiliary Structures on Disk
 */
	typedef	struct Tm_Auxiliary
	{
		unsigned short
				Auxiliary[Tm_Aux_Buf_Size / 2];
		unsigned short
				Sync_Flag[4];
	} Tm_Auxiliary;

/*
 *	Thematic Mapper Pay Load Data (PCD) structure on SONY
 */
	typedef struct Pcd
	{
		unsigned char
			pcd_data[Tm_Pcd_Buf_Size];
	} Pcd;

/*
 *	Thematic Mapper Pay Load Data (PCD) structure on SONY
 */
	typedef struct Pcd_Swath
	{
		double
			Time;
		Pcd
			Data;
	} Pcd_Swath;

/*
 *	RGB pixel basic format structure 
 */
	typedef struct Tm_Rgb_24
	{
		unsigned char
      				blue_color,
      				green_color,
				red_color;
	} Tm_Rgb_24;

/*
 *	RGB output video line structure 
 */
	typedef struct Tm_Video_Rgb_24
	{
		Tm_Rgb_24
				video_buffer[Tm_QL_lines][Tm_QL_size];
	} Tm_Video_Rgb_24;

#ifdef __cplusplus
	}
#endif

#endif	/* endif of redefinition */
