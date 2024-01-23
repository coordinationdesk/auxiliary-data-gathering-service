/*
 *	This include file contains all the structures and tables definition for the Direct Ingestion 
 *	of the following satellites:
 *	    NASA    Landsat Enhanced Thematic Mapper +	(ETM+)	Missions:    7
 *
 *	$Id$
 */

#ifndef __ETM_STRUCTURES_H__
#define __ETM_STRUCTURES_H__

#ifdef __cplusplus
	extern "C" {
#endif
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char const * ETMStructuresId="@(#) Advanced Computer Systems: ETM+_Structures.h 1.4 00/05/05";

/*--------------------------------------------------------------------*/
/*- private include ---------------------------------------------------*/
/*--------------------------------------------------------------------*/

#define	ETM_Missions	    4			    /* Number of Landsat Missions			*/
#define	ETM_MinFrameSize   85			    /* Number of Byte in a Minor Frame			*/
#define	ETM_HalfMinFrameSize	    40		    /* Half usefull Minor Frame, to look for Fill pattern or SLS	*/
#define	ETM_QuarterMinFrameSize	    20		    /* A quarter of usefull Minor Frame, to look for Fill pattern or SLS	*/
#define ETMB6sparesize      5                       /* Number of bytes of band6 data and 1 spare        */
/*#define QlCh1ETM_MinFrameSize 20*/                    /* Size of Ql Minor Frame of Format 1		*/
/*#define QlCh2ETM_MinFrameSize 8 */                    /* Size of Ql Minor Frame of Format 2		*/
#define ETM_MajorFrameSize 7470		            /* Major Frame Size	to skip (in MnF unit)		*/
#define ETM_SafeMajorFrameSize  7650		    /* Redunant Major Frame Size (in MnF unit) 		*/
#define	ETMNominalVideo    6320		            /* Nominal Video Portion of a Major Frame		*/
#define	ETMTimePortion	    6    		    /* Number of Minor Frame Containing the Time	*/
#define	ETMEosPortion	   100			    /* Eos, Fhserr & Shserr Portion of a Major Frame	*/
#define	ETMVideoPortion    6434		            /* Video Portion of a Major Frame (to align at 4 byte boundary			*/
/****Valter proposal ****/
/*#define	ETMVideoPortion    6478*/		            /* Video Portion of a Major Frame (to align at 8 byte boundary)*/
#define	ETMCalibPortion    986			    /* Calibration Portion of a Major Frame		*/
#define ETMFillCodePortion 160			    /* Fill code portion before SLS			*/

#define	ETMPCD_Blocks_Number	750		    /* Safe Number of PCD blocks in a Swath        	*/
#define ETMPCD_Block_Size	10		    /* Number of bytes in a PCD block			*/    
#define	ETMPCD_Fill_Size	300		    /* Safe Number of Pcd bytes extracted from preamble zone	*/

#define ETM_Sync_Pattern	0x1ACFFC1D	    /* Full CADU Sync Pattern (4 bytes)			*/
#define ETM_Sync_1		0x1A		    /* MS CADU Sync Byte				*/
#define ETM_Sync_2		0xCF		    /* Second CADU Sync Byte				*/
#define ETM_Sync_3		0xFC		    /* Third CADU Sync Byte				*/
#define ETM_Sync_4		0x1D		    /* LS CADU Sync Byte				*/
#define ETM_Sync_Pattern_Inv	0xE53003E2	    /* Full CADU Sync Pattern (4 bytes) Inverted	*/

#define	ETM_H0_7	    0x45		    /* First 8 bits of VCDU_Header (Version and Sat_Id)	*/
#define ETM_HCh1	    0x41		    /* Second 8 bits of VCDU_Header (Ch Id)		*/	    			
#define ETM_HCh2	    0x42		    /* Second 8 bits of VCDU_Header (Ch Id)		*/	    			
#define ETM_HPrio	    0x40		    /* Priority Data Option in VCDU Header		*/
#define ETM_HRout	    0x00		    /* Routine Data Option in VCDU Header		*/
#define ETM_HErrCtrl_Ch1Pr  0x6594		    /* BCH Code for ErrCtrl in Header: Priority and CH1 */
#define ETM_HErrCtrl_Ch1Ro  0xbf82		    /* BCH Code for ErrCtrl in Header: Routine and CH1  */
#define ETM_HErrCtrl_Ch2Pr  0x03a5		    /* BCH Code for ErrCtrl in Header: Priority and CH2 */
#define ETM_HErrCtrl_Ch2Ro  0xd9b3		    /* BCH Code for ErrCtrl in Header: Routine and CH2  */

#define BCH31_16_3_Module   31			    /* Size of Galois Field used during Pointer correction  */
#define BCH1023_30_3_Module 1023		    /* Size of Galois Field used during Data correction  */

#define CADU_Size	    1040		    /* Size in bytes of a CADU				*/
#define CADU_Sync_Size	    4			    /* Size of the Sync Mark in bytes			*/
#define VCDU_Size	    1036		    /* Size in bytes of a VCDU (CADU without Sync)	*/
#define VCDU_Header_Size    8			    /* Size of the VCDU Header in bytes			*/
#define VCDU_Video_Size	    982			    /* Size of the Video Part in Mission Data Zone	*/
#define VCDU_PCD_Size	    10			    /* Size of the PCD Part in Mission Data Zone	*/
#define VCDU_BCH_Data_Size  30			    /* Size of the BCH code protecting mission data	*/
#define CADU_Offset_to_Pcd  994			    /* Bytes to skip in CADU for pointing pcd block	*/		 

#define ETM_Data_Rate	    150.0		    /* Data rate in Mbps				*/

#define	ETM_Scan_Time	    71.820		    /* Nominal Scan Time in milliseconds                */
#define	ETM_Scan_Time_MAX   107.73		    /* Maximum accepted Scan Time in milliseconds (3/2) */
#define	ETM_Scan_Time_MIN   35.910		    /* Minimum accepted Scan Time in milliseconds (1/2)	*/

#define	ETM_Scan_Time_JD    8.33125e-7		    /* Nominal Scan Time in days			*/

#define ETM_QL_ScanTime		26.93			/* Delta time between ql lines(ETM_Scan_Time/ETM_Refl_sensors)*ETM_QL_sample*/

#define	ETM_ifov_x	    .0000425		    /* Istantaneous field of view in x direction ********/
#define	ETM_ifov_y	    .0000425		    /* Istantaneous field of view in y direction ********/

#define ETM_sensors	    136			    /* Number of Total ETM sensor (all bands)		*/
#define ETM_B6_sensors	    8			    /* Number of Band 6 ETM sensor			*/
#define	ETM_Refl_sensors    16			    /* Number of Band 1-2-3-4-5-7 sensors		*/
#define ETM_Pan_sensors     32			    /* Number of Panchromatic sensors			*/
#define	ETM_Extra_Pcd	    90			    /* Extra "Pcd Swath at the start/end of video data	*/
#define ETM_FR_swath	    400			    /* Number of swath  to load to disk			*/
#define ETM_FR_Nswath	    375			    /* Number of swath  nominal according to NASA definition */
#define	ETM_Aux_Buf_Size    20			    /* Nominal number of Auxiliary values per swath	*/
#define ETM_line_size	    6400		    /* Thematic Mapper line length			*/
#define ETM_Calib_size	    960			    /* Thematic Mapper calibration length		*/

#define ETM_FR_lines	    5760		    /* Number of lines in a ETM full frame		*/
#define	ETM_QL_Ds_size	    500			    /* ETM Quick Look Line Size During Display		*/
#define	ETM_QL_Fr_size	    1000		    /* ETM Quick Look Line Size During Catalogue	*/
#define	ETM_QL_line_size    6000		    /* Video Portion used for the Quick Look Production	*/
#define	ETM_QL_lines	    3			    /* Max # of Quick Look Lines Extracted per Swath	*/
#define	ETM_lost_pixel	    (160*ETM_MinFrameSize)  /* Number of ETM Pixels not Covered by All Bands	*/
#define	ETM_Ql_lost_pixel   (160*QlETM_MinFrameSize)/* Number of ETM Pixels not Covered by All Bands	*/

#define	ETM_QL_sample	    6		    /* ETM Quick Look Subsample	in Minor Frames		*/

#define ETM_Pan_sample      12			    /* ETM Pan Quick Look Subsample			*/
#define	ETM_Max_Lost_Lines  20			    /* Max Number of Lines Inserted for a Data Loss	*/

#define	ETM_bands	    9			    /* Number of Enhanced Thematic Mapper Spectral Bands (7 bands + Pan + B6 High Gain)	*/

#define	ETM_Band6LG	    6	/* Define a number for the Thermic Band 6 Low Gain (1 relative) */
#define	ETM_Band6HG	    9	/* Define a number for the Thermic Band 6 High Gain (1 relative)*/
#define ETM_Band7	    7	/* Define a number for the Visible band 7 (1 relative) */
#define	ETM_BandPan	    8	/* Define a number for the Panchromatic Band (1 relative) */

#define ETM_PANCHROMATIC    1  /* Panchromatic BW case	*/
#define ETM_REFLECTIVE	    2  /* Visible RGB case	*/
#define ETM_THERMO	    	3  /* Thermic BW case	*/

#define ETM_THERMO_LOW	    31  /* Infrared B6 Low gain case	*/ 
#define ETM_THERMO_HIGH	    32  /* Infrared B6 High gain case	*/


#define	ETM_band1_point	    0			    /* Minor Frame Position of First Band1 Pixel (Ch1)	*/
#define	ETM_band2_point	    1			    /* Minor Frame Position of First Band2 Pixel (Ch1)	*/
#define	ETM_band3_point	    2			    /* Minor Frame Position of First Band3 Pixel (Ch1)	*/
#define	ETM_band4_point	    3			    /* Minor Frame Position of First Band4 Pixel (Ch1)	*/
#define	ETM_band5_point	    4			    /* Minor Frame Position of First Band5 Pixel (Ch1)	*/
#define	ETM_band6_point	    80			    /* Minor Frame Position of First Band6 Pixel (Ch1)	*/
#define	ETM_band7_point	    4			    /* Minor Frame Position of First Band7 Pixel (Ch2)	*/
#define	ETM_bandP_point	    0			    /* Minor Frame Position of First BandP Pixel (Ch2)	*/

#define	ETM_band1_offset	(85 * ETM_MinFrameSize)	    /* Offset from Center for Band1	*/
#define	ETM_band2_offset	(60 * ETM_MinFrameSize)	    /* Offset from Center for Band2	*/
#define	ETM_band3_offset	(35 * ETM_MinFrameSize)	    /* Offset from Center for Band3	*/
#define	ETM_band4_offset	(10 * ETM_MinFrameSize)	    /* Offset from Center for Band4	*/
#define	ETM_band5_offset	(-61 * ETM_MinFrameSize)	/* Offset from Center for Band5	*/
#define	ETM_band6_offset	(-95 * ETM_MinFrameSize)	/* Offset from Center for Band6 (point sensor 8) 	*/
#define	ETM_band7_offset	(-35 * ETM_MinFrameSize)	/* Offset from Center for Band7	*/
#define	ETM_bandP_offset	(110 * ETM_MinFrameSize)	/* Offset from Center for BandP	*/

#define	ETM_Ql_band1_offset  (85 * QlETM_MinFrameSize)   /* Offset from Center of Pan Band for Band 1	*/
#define	ETM_Ql_band2_offset  (60 * QlETM_MinFrameSize)   /* Offset from Center of Pan Band for Band 2	*/
#define	ETM_Ql_band3_offset  (35 * QlETM_MinFrameSize)   /* Offset from Center of Pan Band for Band 3	*/
#define	ETM_Ql_band4_offset  (10 * QlETM_MinFrameSize)   /* Offset from Center of Pan Band for Band 4	*/
#define	ETM_Ql_band5_offset  (-61 * QlETM_MinFrameSize)  /* Offset from Center of Pan Band for Band 5	*/
#define	ETM_Ql_band6_offset  (-95 * QlETM_MinFrameSize)  /* Offset from Center of Pan Band for Band 6	*/
#define	ETM_Ql_band7_offset  (-35 * QlETM_MinFrameSize)  /* Offset from Center of Pan Band for Band 7	*/
#define	ETM_Ql_bandP_offset  (110 * QlETM_MinFrameSize)  /* Offset from Center of Pan Band for Band P	*/

#define	ETM_QL_red	    5			    /* Default Band on Red for Quick Look		*/
#define	ETM_QL_green	    4			    /* Default Band on Green for Quick Look		*/
#define	ETM_QL_blue	    2			    /* Default Band on Blue for Quick Look		*/

#define	ETM_Tc_QL_red	    8			    /* True Color Band on Red for Quick Look		*/
#define	ETM_Tc_QL_green	    8			    /* True Color Band on Green for Quick Look		*/
#define	ETM_Tc_QL_blue	    8			    /* True Color Band on Blue for Quick Look		*/
	
#define	RGB_Size	    3			    /* Number of Bytes for a Color Pixel		*/

#ifndef min
    #define	max(a, b) ((a > b) ? a : b)
    #define	min(a, b) ((a > b) ? b : a)
#endif

/*
 *	Enhanced Thematic Mapper Plus MINOR FRAMES Structures
 */
	typedef struct	{
			 u_char
			    Video_Data[16][5], 
			    Band6[4], 
			    spare; 
	}ETM_Minor_Frame;	

	typedef struct	{
			u_char		
			    Band1_Sensor01, Band2_Sensor01, Band3_Sensor01, Band4_Sensor01, Band5_Sensor01,  
			    Band1_Sensor03, Band2_Sensor03, Band3_Sensor03, Band4_Sensor03, Band5_Sensor03, 
			    Band1_Sensor05, Band2_Sensor05, Band3_Sensor05, Band4_Sensor05, Band5_Sensor05,  		
	    		    Band1_Sensor07, Band2_Sensor07, Band3_Sensor07, Band4_Sensor07, Band5_Sensor07, 		
			    Band1_Sensor09, Band2_Sensor09, Band3_Sensor09, Band4_Sensor09, Band5_Sensor09,  		
			    Band1_Sensor11, Band2_Sensor11, Band3_Sensor11, Band4_Sensor11, Band5_Sensor11, 		
			    Band1_Sensor13, Band2_Sensor13, Band3_Sensor13, Band4_Sensor13, Band5_Sensor13, 
			    Band1_Sensor15, Band2_Sensor15, Band3_Sensor15, Band4_Sensor15, Band5_Sensor15, 
	
			    Band1_Sensor02, Band2_Sensor02, Band3_Sensor02, Band4_Sensor02, Band5_Sensor02,  
			    Band1_Sensor04, Band2_Sensor04, Band3_Sensor04, Band4_Sensor04, Band5_Sensor04, 
			    Band1_Sensor06, Band2_Sensor06, Band3_Sensor06, Band4_Sensor06, Band5_Sensor06, 		
			    Band1_Sensor08, Band2_Sensor08, Band3_Sensor08, Band4_Sensor08, Band5_Sensor08,  		
			    Band1_Sensor10, Band2_Sensor10, Band3_Sensor10, Band4_Sensor10, Band5_Sensor10,  		
			    Band1_Sensor12, Band2_Sensor12, Band3_Sensor12, Band4_Sensor12, Band5_Sensor12,  		
			    Band1_Sensor14, Band2_Sensor14, Band3_Sensor14, Band4_Sensor14, Band5_Sensor14, 
			    Band1_Sensor16, Band2_Sensor16, Band3_Sensor16, Band4_Sensor16, Band5_Sensor16, 
			    
			    Band6_LG1_or_2,    Band6_LG3_or_4,    Band6_LG5_or_6,    Band6_LG7_or_8,    spare; 	
	}ETM_FRCh1_Minor_Frame;

	typedef struct	{
			u_char		
			    Pan_Sensor01_a, Pan_Sensor03_a, Pan_Sensor05_a, Pan_Sensor02_a, Band7_Sensor01,  
			    Pan_Sensor04_a, Pan_Sensor06_a, Pan_Sensor08_a, Pan_Sensor07_a, Band7_Sensor03, 
			    Pan_Sensor09_a, Pan_Sensor11_a, Pan_Sensor13_a, Pan_Sensor10_a, Band7_Sensor05,  		
	    		    Pan_Sensor12_a, Pan_Sensor14_a, Pan_Sensor16_a, Pan_Sensor15_a, Band7_Sensor07, 		
			    Pan_Sensor17_a, Pan_Sensor19_a, Pan_Sensor21_a, Pan_Sensor18_a, Band7_Sensor09,  		
			    Pan_Sensor20_a, Pan_Sensor22_a, Pan_Sensor24_a, Pan_Sensor23_a, Band7_Sensor11, 		
			    Pan_Sensor25_a, Pan_Sensor27_a, Pan_Sensor29_a, Pan_Sensor26_a, Band7_Sensor13, 
			    Pan_Sensor28_a, Pan_Sensor30_a, Pan_Sensor32_a, Pan_Sensor31_a, Band7_Sensor15, 
	
			    Pan_Sensor01_b, Pan_Sensor03_b, Pan_Sensor05_b, Pan_Sensor02_b, Band7_Sensor02,  
			    Pan_Sensor04_b, Pan_Sensor06_b, Pan_Sensor08_b, Pan_Sensor07_b, Band7_Sensor04, 
			    Pan_Sensor09_b, Pan_Sensor11_b, Pan_Sensor13_b, Pan_Sensor10_b, Band7_Sensor06,  		
	    		    Pan_Sensor12_b, Pan_Sensor14_b, Pan_Sensor16_b, Pan_Sensor15_b, Band7_Sensor08, 		
			    Pan_Sensor17_b, Pan_Sensor19_b, Pan_Sensor21_b, Pan_Sensor18_b, Band7_Sensor10,  		
			    Pan_Sensor20_b, Pan_Sensor22_b, Pan_Sensor24_b, Pan_Sensor23_b, Band7_Sensor12, 		
			    Pan_Sensor25_b, Pan_Sensor27_b, Pan_Sensor29_b, Pan_Sensor26_b, Band7_Sensor14, 
			    Pan_Sensor28_b, Pan_Sensor30_b, Pan_Sensor32_b, Pan_Sensor31_b, Band7_Sensor16, 
			    
			    Band6_HG1_or_2,    Band6_HG3_or_4,    Band6_HG5_or_6,    Band6_HG7_or_8,    spare; 	
	}ETM_FRCh2_Minor_Frame;


/*
 *	Thematic Mapper Quick Look Intermediate Minor Frame Structure (1 sensor each 4)
 */
	typedef struct	{
			u_char
    			    Band1_Sensor01, Band2_Sensor01, Band3_Sensor01, Band4_Sensor01, Band5_Sensor01, 
			    Band1_Sensor05, Band2_Sensor05, Band3_Sensor05, Band4_Sensor05, Band5_Sensor05, 		
			    Band1_Sensor09, Band2_Sensor09, Band3_Sensor09, Band4_Sensor09, Band5_Sensor09, 		
			    Band1_Sensor13, Band2_Sensor13, Band3_Sensor13, Band4_Sensor13, Band5_Sensor13;
	}ETM_QLCh1_Minor_Frame;

	typedef struct	{
			u_char
    			    Pan_Sensor01_a, Band7_Sensor01, 
			    Pan_Sensor09_a, Band7_Sensor05, 		
			    Pan_Sensor17_a, Band7_Sensor09, 		
			    Pan_Sensor25_a, Band7_Sensor13;
	}ETM_QLCh2_Minor_Frame;


/*
 *	ETM USER Structures
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
			u_char
			    Format_Id, 
			    spare[3];
			int
			    Fhserr, 
			    Shserr,  
			    Scan_Direction, 
			    Line_Length, 
			    Major_Frame_Length;
			double
			    Satellite_Time;
	}ETM_Satellite_Scan_Data;
	
	
	typedef struct	{
			u_char 
			    PCD_Word[4];
			    
			u_short
			    MF_Counter, 
			    Status_and_Gains;
			
			u_char 
			    spare[2];
	}ETM_PCD_Block;
	

	typedef struct	{
			u_int 
			    CADU_Start_Num, 
			    CADU_Stop_Num,
			    CADU_Offset;
			    
			u_char 
			    Data_Priority, 
			    spare[7];
	}ETM_CADU_Auxiliary_Data;



/*
 *  PCD structure to disk
 */
	typedef struct	{
			ETM_Satellite_Scan_Data
			    Satellite_Data;

			ETM_CADU_Auxiliary_Data
			    CADU_Info;
	
	                u_char
			    PCD_String[ETMPCD_Blocks_Number*ETMPCD_Block_Size + ETMPCD_Fill_Size];	    
#ifndef sgi
        /* LINUX PADDING: required, since size should be multiple by 8 bytes */
        unsigned char __linux_padding[4] ;     
#endif
	}ETM_PCD_Record;

/*
 *  ETM+ Status structure to disk for
 */
	typedef struct	{
			ETM_Satellite_Scan_Data
			    Satellite_Data;

			ETM_CADU_Auxiliary_Data
			    CADU_Info;
	
	                u_char
			    Status_String[(ETMPCD_Blocks_Number*ETMPCD_Block_Size + ETMPCD_Fill_Size) / ETMPCD_Block_Size * 6];	    
#ifndef sgi
        /* LINUX PADDING: required, since size should be multiple by 8 bytes */
        unsigned char __linux_padding[4] ;     
#endif
	}ETM_Status_Record;
	
	
/*
 *	Enhanced Thematic Mapper Plus MAJOR FRAME Structures (on DLT)
 */
	typedef struct	{
			ETM_Satellite_Scan_Data
			    Satellite_Data;

			ETM_Minor_Frame
			    Minor_Frame[ETMVideoPortion + ETMCalibPortion];
			    
			ETM_CADU_Auxiliary_Data
			    CADU_Info;
	
	                u_char
			    PCD_String[ETMPCD_Blocks_Number*ETMPCD_Block_Size + ETMPCD_Fill_Size];
	    
	}ETM_Major_Frame;

	typedef struct	{
			ETM_Satellite_Scan_Data
			    Satellite_Data;

			ETM_FRCh1_Minor_Frame
			    Minor_Frame[ETMVideoPortion + ETMCalibPortion];
	
			ETM_CADU_Auxiliary_Data
			    CADU_Info;
	
	                u_char
			    PCD_String[ETMPCD_Blocks_Number*ETMPCD_Block_Size + ETMPCD_Fill_Size];
	    
	}ETM_Ch1_Major_Frame;

	typedef struct	{
			ETM_Satellite_Scan_Data
			    Satellite_Data;

			ETM_FRCh2_Minor_Frame
			    Minor_Frame[ETMVideoPortion + ETMCalibPortion];
	
			ETM_CADU_Auxiliary_Data
			    CADU_Info;
	
	                u_char
			    PCD_String[ETMPCD_Blocks_Number*ETMPCD_Block_Size + ETMPCD_Fill_Size];
		
	}ETM_Ch2_Major_Frame;


/*
 *  ETM+ Intermediate swath used in Ingestion
 */
	typedef struct	{
			ETM_Minor_Frame
			    ETMTimeMnF[ETMTimePortion];
		
			ETM_Minor_Frame
			    ETMVideoMnF[ETMNominalVideo - ETMTimePortion - 1 - (ETMEosPortion / 2)];
		
			ETM_Minor_Frame
			    ETMEosMnF[ETMEosPortion];
		
			ETM_Satellite_Scan_Data
			    Satellite_Data;
			    
	}Intermediate_ETM_Swath;

/*
 *  Intermediate Swath used during transcription
 */
	typedef struct	{
			double
			    JD50_swath_time;

			ETM_Major_Frame	
			    ETM_Swath;
			    			    
	} Stack_ETM_Swath;
	

/*
 *	Enhanced Thematic Mapper Reduced Resolution Quick Look (for Display Only)
 */
	typedef struct  {
	    double
	       Absol;	       
	    u_int
	       Delta; 
#ifndef sgi
        /* LINUX PADDING: required, since size should be multiple by 8 bytes */
        unsigned char __linux_padding[4] ;     
#endif
	}ETM_Ql_Time_Part;

	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[ETM_QL_Ds_size * RGB_Size],
		Padding[24];
	}Display_Generic_Ql_ETM;
	
/*
 *	E. Thematic Mapper Full Resolution Quick Look (for Archive,  RGB interleaved)
 */
	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[ETM_QL_Fr_size * RGB_Size];
	}Fr_Generic_Ql_ETM;

/*
 *	E. Thematic Mapper Full Resolution Quick Look (for Archive, all bands BW)
 */
	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[ETM_bands][ETM_QL_Fr_size];
	}Fr_Allbands_Ql_ETM;



/*
 *	E. Thematic Mapper Full Resolution Quick Look (for test of size)
 */
	typedef struct	{
	    ETM_Ql_Time_Part
	    	Time;
	    u_char
		Video[ETM_QL_Fr_size * RGB_Size];
	}Fr_Test_Ql_ETM;
	
/*
 *	Thematic Mapper Full Resolution Quick Look (for Reproduction)
 */
	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[ETM_QL_Fr_size * RGB_Size];
	    u_int
		Filler[15];
	}Rp_Generic_Ql_ETM;
	


/*
 *	CADU structures
 */
	typedef struct	{
	    u_int
		CADUSyncPattern;
	    u_int	
#if ( BYTE_ORDER == BIG_ENDIAN )        
		    ID:		    2,
		    SCID:		    8, 
		    VCID:		    6, 
		    VCDUCount_msb:	    16; 
#else
		    VCDUCount_msb:	    16, 
		    VCID:		    6, 
		    SCID:		    8, 
		    ID:		    2 ;
#endif
	    u_int		 
#if ( BYTE_ORDER == BIG_ENDIAN )        
		    VCDUCount_lsb:	8, 
		    Replay:		    1, 
		    Priority:	    1,
		    spares:		    6,  
		    VCDUHeaderErrCtrl:  16; 
#else
		    VCDUHeaderErrCtrl:  16, 
		    spares:		    6,  
		    Priority:	    1,
		    Replay:		    1, 
		    VCDUCount_lsb:	8 ;
#endif
	}CADU_Header;
	
	typedef struct	{
	    CADU_Header
		Header;
		
	    u_char
		MissionDataZone[VCDU_Video_Size], 
		PCDStatus[ETMPCD_Block_Size], 
		BCHDataCorrection[30], 
		Pointer[2], 
		BCHPointerCorrection[2],
		VCDUCorrection[2]; 
	}CADU_Model;


/*
 *	Data quality structure
 *	    - SwathNumber	Progressive swath number (starting from 0)
 *	    - Satellite_Time	Swath Milliseconds from 1 Jan of current year
 *	    - SwathJD50Time	Swath time in JD50
 *	    - MnFLost		Number of Minor Frames lost in current swath
 *	    - PCDStatusLost	Number of PCD and Status lost bytes
 *	    - SwathLostTillNow	Incremental number of lost swath from pass start
 */
	typedef struct	{
	    u_int
		SwathNumber;
#ifndef sgi
        /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
        char    __linux_padding1[4] ;     
#endif	    
		double
		Satellite_Time, 
		SwathJD50Time; 
		
	    u_int
		MnFLost, 
		PCDStatusLost, 
		SwathLostTillNow;
#ifndef sgi
        /* LINUX PADDING: required, since total size should be multiple by 8I */
        char    __linux_padding2[4] ;     
#endif        
	}ETM_Swath_Quality; 
			

/*
 *	Thematic Mapper Full Resolution Structures on Disk
 */
	typedef struct ETM_Full_Resolution_on_Disk
	{
		unsigned char
				ETM_data[ETM_sensors][ETM_line_size + ETM_Calib_size];
		unsigned short
				Sync_Flag[4];
		unsigned short
				Auxiliary[ETM_Aux_Buf_Size / 2];
	} ETM_Full_Resolution_on_Disk;

/*
 *	Thematic Mapper Auxiliary Structures on Disk
 */
	typedef	struct ETM_Auxiliary
	{
		unsigned short
				Auxiliary[ETM_Aux_Buf_Size / 2];
		unsigned short
				Sync_Flag[4];
	} ETM_Auxiliary;

#ifdef __cplusplus
	}
#endif

#endif	/* endif of redefinition */
