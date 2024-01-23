#ifndef __MSS_STRUCTURES_H__
#define __MSS_STRUCTURES_H__

#include <mssConstants.h> 

#ifdef __cplusplus
	extern "C" {
#endif
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char const * MSSStructuresId="@(#) Advanced Computer Systems: MSS_Structures.h 1.5 00/12/08";

#define	Mss_Missions	    	5			    /* Number of Landsat Missions with MSS		*/
#define	MssMinFrameSize	    	225			    /* Number of Byte for two Minor Frame		*/
#define	MssMinFrameValue    	150			    /* Number of Values (6 bits) in two Minor Frames	*/
#define	MinFrameMssValue    	150			    /* Number of Values in a Minor Frame		*/
#define	MssSampleFrameSize  	25			    /* Number of Byte In a Mss block			*/
#define	MssSyncWordSize	    	4			    /* Number of Byte with Sync Word		    	*/
#define	MssNominalVideo	    	3288		    /* Nominal Video Portion of a Major Frame (max)	*/
#define	MssNominalVideoToEOS	3218		    /* Nominal Video Portion of a Major Frame before EOS	*/
#define	MssNominalEos	    	192			    /* Nominal Number of Black and White EOS Pixel	*/
#define	MssEosDetectionMargin   20			    /* Margin on Detecting the Eos Pattern		*/
#define MssNominalEosPosition 	3216			/* Position in Mnf of the BEGINNING of the EOS	*/
#define	MssVideoPortion	    	3300		    /* Video Portion of a Major Frame			*/
#define	MssCalibPortion	    	2300		    /* Calibration Portion of a Major Frame		*/
#define	MssEosPortion	    	100			    /* Eos of a Major Frame				*/

/*#define	FrMssSkipPortion    3000*/		    /* Major Frame Portion to Skip During Full Res in bytes	*/
#define	FrMssSkipPortion    25500
#define	QlMssSkipPortion    60000		    /* Major Frame Portion to Skip During Quick Look	*/
#define	MssMfSearchWindow   140000		    /* Major Frame Search Windows in bytes		*/
/*#define	MssPreampleWindow   70000*/		/* Major Frame Search Preamble Windows in bytes	(Original value)*/
#define	MssPreampleWindow   35000 			/* Major Frame Search Preamble Windows in bytes	(value verified in transcription) */
#define	MssL3PreampleWindow   100000		/* Major Frame Search Preamble Windows in bytes	(value tested for L3)*/

#define	Mss_Auxiliary_Size	    10		    /* Multi Spectral Scanner Auxiliary Size (Enerter FS Like)	*/

#define	Mss_Max_Search_Attempt	    5		    /* Attempt to Find Sync Word in a Given Buffer Position	*/
#define	Mss_Search_Step_Jump	    4		    /* Step (In Frame Equivalent) when Sercing Sync Word	*/
#define Mss_MjF_Error_Limit 		2			/* Max bits in error during SLS search					*/
#define Mss_MnF_Error_Limit 		1			/* Max bits in error during Minor Frame check			*/

#define	Mss_Scan_Time	    73.42		    /* Nominal Scan Time				*/
#define MAX_Mss_Scan_Time   110.13		    /* Mss_Scan_Time * 1.5				*/
#define MIN_Mss_Scan_Time   36.71		    /* Mss_Scan_Time * 0.5				*/
#define MAX_SatelliteTimeErrors 3			/* Accepted Mss time errors */

#define Mss_Scan_Time_JD    8.4977e-7		    /* Mss_Scan_Time in days				*/

#define MSS_Data_Rate	    15.0		    /* Data Rate in Mbits per second			*/
#define	MSS_Buffer_Time	    (double)(((double)(BuffSize/MEGABYTE)/(MSS_Data_Rate/8.0))*1000.0)	/* Buffer duration in Milliseconds */	

#define Mss_Scan_Line	    12.2367		    /* Line Scan Time in Millisecond			*/

#define	Mss_ifov_x	    .000080817		    /* MODIFIED by Valter on 21 July 97 Istantaneous field of view in x direction ********/
#define	Mss_ifov_y	    .000117255		    /* MODIFIED by Valter on 21 July 97 Istantaneous field of view in y direction ********/

#define Mss_sensors			(MSS_NUM_OF_BANDS*MSS_NUM_OF_DET)	/* Number of Total Msssensor (all bands)		*/
#define Mss_FR_swath	    400			    /* Number of swath  to load to disk			*/
#define	Mss_Aux_Buf_Size    20			    /* Nominal number of Auxiliary values per swath	*/
#define Mss_line_size	    MSS_LINE_SIZE	/* Multi Spectral Scanner line length		*/
#define Mss_Calib_size	    MSS_CALIB_SIZE	/* Multi Spectral Scanner calibration length	*/
#define Mss_Calib_gap	    MSS_CALIB_GAP	/* Multi Spectral Scanner portion inserted between line and calibration	*/

#define	MssMF_Sync	    0xC71C7E0B		    /* Bits  0 - 31 of Mss Sync Word			*/
#define	MssMF_Sync_1	    0xC71C		    /* Bits  0 - 31 of Mss Sync Word			*/
#define	MssMF_Sync_2	    0x7E0B		    /* Bits  0 - 31 of Mss Sync Word			*/
#define MssMnF_Sync 		0x07			/* Minor Frame sync byte					*/
#define Mss_FR_lines	    2100		    /* Number of lines in a Mss full frame	 ********/
#define	Mss_QL_Rt_size	    508			    /* Mss Quick Look Line Size During Phase 1		*/
#define	Mss_QL_size	    	1020		    /* Mss Quick Look Line Size During Phase 2		*/
#define	Mss_QL_line_size    6000		    /* Video Portion used for the Quick Look Production	*/
#define	Mss_QL_lines	    3			    /* Max # of Quick Look Lines Extracted per Swath	*/
#define	Mss_bands	    	MSS_NUM_OF_BANDS			    /* Multi Spectral Scanner Spectral Bands		*/
#define	Mss_QL_sensors	    MSS_NUM_OF_DET			    /* Multi Spectral Scanner Sensors per Band		*/
#define	Mss_QL_sample	    3			    /* Mss Quick Look Subsample				*/

#ifdef INGESTION
    #define Mss_Line_Sample 4			    /* Sampling in lines				*/  
#else
    #define Mss_Line_Sample 2	    
#endif

#define	Mss_Max_Lost_Lines  20			    /* Max Number of Lines Inserted for a Data Loss	*/

#define	Mss_band1_point	    1			    /* Minor Frame Position of First Band 1 Pixel	*/
#define	Mss_band2_point	    2			    /* Minor Frame Position of First Band 2 Pixel	*/
#define	Mss_band3_point	    13			    /* Minor Frame Position of First Band 3 Pixel	*/
#define	Mss_band4_point	    14			    /* Minor Frame Position of First Band 4 Pixel	*/

#define	Mss_QL_red	    4			    /* Default Band on Red for Quick Look		*/
#define	Mss_QL_green	    3			    /* Default Band on Green for Quick Look		*/
#define	Mss_QL_blue	    2			    /* Default Band on Blue for Quick Look		*/

#define	Mss_Tc_QL_red	    3			    /* True Color Band on Red for Quick Look		*/
#define	Mss_Tc_QL_green	    2			    /* True Color Band on Green for Quick Look		*/
#define	Mss_Tc_QL_blue	    1			    /* True Color Band on Blue for Quick Look		*/
	
/*
 *	Multi Spectral Scanner Minor Frame Structure
 */
	typedef struct	{
	    u_char
		Sync_Byte, 
		Band1_SensorA_1, Band2_SensorA_1, Band3_SensorA_1, Band4_SensorA_1, 
		Band1_SensorB_1, Band2_SensorB_1, Band3_SensorB_1, Band4_SensorB_1, 
		Band1_SensorC_1, Band2_SensorC_1, Band3_SensorC_1, Band4_SensorC_1, 
		Band1_SensorD_1, Band2_SensorD_1, Band3_SensorD_1, Band4_SensorD_1, 
		Band1_SensorE_1, Band2_SensorE_1, Band3_SensorE_1, Band4_SensorE_1, 
		Band1_SensorF_1, Band2_SensorF_1, Band3_SensorF_1, Band4_SensorF_1; 
	}Mss_FR_Minor_Frame;
	
/*
 *	Multi Spectral Scanner Satellite Time Structure
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
		Line_Length, 
		Major_Frame_Length;
	    double
		Satellite_Time;
	}Mss_Satellite_Scan_Data;
	
	typedef struct	{
	    u_char
		Sync_Id, 
		Video_Data[4][6];
	}Mss_Minor_Frame;
	
/*
 *	Multi Spectral Scanner Major Frame Structure
 */
	typedef struct	{

	    Mss_Satellite_Scan_Data
		Satellite_Data;

	    Mss_FR_Minor_Frame
		Minor_Frame[MssVideoPortion + MssCalibPortion];
	}Mss_Major_Frame;


#warning "PARTIALLY PORTED" ;
#ifdef ENRICO	
/*
 *	Multi Spectral Scanner Reduced Resolution Quick Look (for Display Only)
 */
	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[Mss_QL_Rt_size * RGB_Size];
	}Rt_Generic_Ql_Mss;
	
/*
 *	Multi Spectral Scanner Full Resolution Quick Look (for Archive)
 */
	typedef struct	{
	    u_int
		Line_Time[3];
	    u_char
		Video[Mss_QL_size * RGB_Size];
	}Fr_Generic_Ql_Mss;
#endif

/*
 *	Multi Spectral Scanner Auxiliary Structures on Disk
 */
	typedef	struct Mss_Auxiliary
	{
		unsigned short
				Auxiliary[Mss_Aux_Buf_Size / 2];
		unsigned short
				Sync_Flag[4];
	} Mss_Auxiliary;


/*
 *	Multi Spectral Scanner Full Resolution structure of data on Disk
 */
	typedef struct Mss_Full_Resolution_on_Disk__
	{
		unsigned char
      			Video_data[Mss_sensors][Mss_line_size + Mss_Calib_size];
		unsigned short
				Sync_Flag[4];
		unsigned short
				Auxiliary[10];
	} Mss_Full_Resolution_on_Disk;
	

#ifdef ENRICO
/*
 *	Multi Spectral Scanner Full Resolution structure of data on SONY
 */
	typedef struct Mss_Full_Resolution_on_SONY__
	{
		unsigned char
      			Video_data[Mss_bands][Mss_QL_sensors][Mss_line_size + Mss_Calib_size];
		unsigned short
				Sync_Flag[4];
		unsigned short
				Auxiliary[10];
	} Mss_Full_Resolution_on_SONY;


/*
 *	Tables for the Description of a Track on Maps
 */
	typedef struct	{
	    double
		Lat_0,		    /* Latitude of the west most point of the scan		    */
		Long_0,		    /* Longitude of the west most point of the scan		    */
		Lat_1,		    /* Latitude of the aest most point of the scan		    */
		Long_1, 	    /* Longitude of the west most point of the scan		    */
		Center_Frame_T;	    /* Time of the Center Frame					    */
	} Mss_Track_Descriptor;
	

/*
 *	RGB pixel basic format structure (for display from SONY)
 */
	typedef struct Mss_Rgb_24
	{
		unsigned char
      			blue_color,
      			green_color,
				red_color;
	} Mss_Rgb_24;

/*
 *	RGB output video line structure (for display from SONY)
 */
	typedef struct Mss_Video_Rgb_24
	{
		Mss_Rgb_24
				video_buffer[Mss_QL_lines][Mss_QL_size];
	} Mss_Video_Rgb_24;

#endif

#ifdef __cplusplus
	}
#endif

#endif	/* endif of redefinition */
