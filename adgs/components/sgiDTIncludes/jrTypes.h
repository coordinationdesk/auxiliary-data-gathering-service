/*------------------------------------------------------------------------------*/
/* Advanced Computer Systems S.p.a.												*/
/* Via Paolo Frisi 7/a 00197 Roma Italia										*/
/*																				*/
/* Source file: VNIRLevel2.h													*/
/*																				*/
/* Written by L.Galli C.Castellani												*/
/*																				*/
/* Version 1.0 created on Febbraio 2000											*/
/*																				*/
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*Purpose:header file for 	jers types	 										*/
/*------------------------------------------------------------------------------*/
#ifndef _jrTypes_h_
#define _jrTypes_h_
#define ODD  0
#define EVEN 1
#define 			JERS1_ORBIT_INCL		97.662	
#define				VNIR_BANDS	    4		/* number of bands for the VNIR sensor		    */
#define				VNIR_LINE_SZ	    4096	/* number of pixel in VNIR line			    */
#define				VNIR_LINE_PER_SWATH 16		/* VNIR number of lines per virtual swath	    */

#define				SWIR_BANDS	    4		/* number of bands for the SWIR sensor		    */
#define				SWIR_LINE_SZ	    4096	/* number of pixel in SWIR line			    */
#define				SWIR_LINE_PER_SWATH 16		/* SWIR number of lines per virtual swath	    */

#define				DWELL_TIME						3.46	// sampling period [msec]		
#define				VNIR_IFOV			   			0.0000322	// VNIR Istantaneous Field of View  
#define				SWIR_IFOV			  			0.0000322	

#define				STEREO_BAND				  		3		// VNIR stereo band
#define				STEREO_ANGLE_DEG		    	15.33	// VNIR stereo band pitch angle [deg]  

#define             FOCAL_LENGHT  310.4				// focal length (mm)

#define				VNIR_PIXEL_SIZE					18		// VNIR output pixel size
#define				ALLOWED_LINE_MISSING			16

#define 			VNIR_RAW_PIXELNUMBER			4096	// VNIR raw number of pixel per line
#define				VNIR_RAW_LINENUMBER				3200    // VNIR raw number of lines per image

#define				VNIR_LEVEL0_PIXELNUMBER			4512   // VNIR level0 number of pixel per line	    
#define				VNIR_LEVEL0_LINENUMBER			3200   // VNIR level0 number of lines per image    

#define				VNIR_LEVEL2_PIXELNUMBER			5512// VNIR level2 number of pixel per line	    
#define				VNIR_LEVEL2_LINENUMBER			4096	// VNIR level2 number of lines per image    

#define 			SWIR_RAW_PIXELNUMBER			4096	// VNIR raw number of pixel per line
#define				SWIR_RAW_LINENUMBER				3200    // VNIR raw number of lines per image

#define				SWIR_LEVEL0_PIXELNUMBER			4512	/* SWIR level0 number of pixel per line	    */
#define				SWIR_LEVEL0_LINENUMBER			3200	/* SWIR level0 number of lines per image    */

#define				SWIR_LEVEL2_PIXELNUMBER			4512	/* SWIR level2 number of pixel per line	    */
#define				SWIR_LEVEL2_LINENUMBER			4096	/* SWIR level2 number of lines per image    */

//   Definition: constants for the image record.			
#define		    FILE_CODE		237
#define		    RECORD_CODE		237
#define		    MISSION_CODE	70
#define		    ORIGIN_CODE		50

#define		    SWIR_LEVEL0_LEFT_FILL_PIXEL		0
#define		    SWIR_LEVEL0_RIGHT_FILL_PIXEL		416
#define		    VNIR_LEVEL0_LEFT_FILL_PIXEL		0
#define		    VNIR_LEVEL0_RIGHT_FILL_PIXEL		416


#define				COUNTERDISTANCE		    16	    /* distance between two Mnf counters    */

/* !   Definitions: image product dimensions.									    ! */
#define				VNIR_LEVEL0_RECLEN		4540
#define				VNIR_LEVEL1_RECLEN		4540
#define				VNIR_LEVEL2_RECLEN		5540

#define				SWIR_LEVEL0_RECLEN		4540
#define				SWIR_LEVEL1_RECLEN		4540
#define				SWIR_LEVEL2_RECLEN		4540

#define				MAGIC_VNIR_L2_B1_FILE		    0x00001020
#define				MAGIC_VNIR_L2_B2_FILE		    0x00001021
#define				MAGIC_VNIR_L2_B3_FILE		    0x00001022
#define				MAGIC_VNIR_L2_B4_FILE		    0x00001023

#define				MAGIC_VNIR_L0_B1_FILE		    0x00001000
#define				MAGIC_VNIR_L0_B2_FILE		    0x00001001
#define				MAGIC_VNIR_L0_B3_FILE		    0x00001002
#define				MAGIC_VNIR_L0_B4_FILE		    0x00001003

#define				MAGIC_SWIR_L0_B5_FILE		    0x00001100
#define				MAGIC_SWIR_L0_B6_FILE		    0x00001101
#define				MAGIC_SWIR_L0_B7_FILE		    0x00001102
#define				MAGIC_SWIR_L0_B8_FILE		    0x00001103

#define				MAGIC_SWIR_L2_B5_FILE		    0x00001120
#define				MAGIC_SWIR_L2_B6_FILE		    0x00001121
#define				MAGIC_SWIR_L2_B7_FILE		    0x00001122
#define				MAGIC_SWIR_L2_B8_FILE		    0x00001123


#define		   		OFFSETLINESFROMNADIR  			389	// offset scan lines from nadir

typedef struct POINT_XY
    {
    double x;
    double y;	
    double lat;			
    double lon;		
} JR_POINT_XY;

typedef struct CORNERS
{
  JR_POINT_XY upleft;    /* X/Y value of the upper left corner  */
  JR_POINT_XY upright;   /* X/Y value of the upper right corner */
  JR_POINT_XY loleft;    /* X/Y value of the lower left corner  */
  JR_POINT_XY loright;   /* X/Y value of the lower right corner */
} JR_CORNERS;

typedef enum 
{
	offNadirBand,
	NadirBands,
	SwirBands
} OpsBandsType;

typedef enum OpsBands__
{
    OpsBand1		= 0,					/* VNIR band 1					    */
    OpsBand2		= 1,					/* VNIR band 2					    */
    OpsBand3		= 2,					/* VNIR band 3					    */
    OpsBand4		= 3,					/* VNIR band 4					    */
    OpsBand5		= 4,					/* SWIR band 5					    */
    OpsBand6		= 5,					/* SWIR band 6					    */
    OpsBand7		= 6,					/* SWIR band 7					    */
    OpsBand8		= 7					/* SWIR band 8					    */
} OpsBands;


/* +------------------------------------------------------------+ */
/* !   Structure: Prefix.										! */
/* +------------------------------------------------------------+ */
typedef struct Prefix__
{
    int				RecSeqNum;			/* record sequence number			    */
    unsigned char		Codes[4];			/* file, record, mission, origin codes		    */
    int				RecLen;				/* record length				    */
    int				ScanLineNum;			/* scan line number				    */
    int				ScanLineTime;			/* scan line time (msec of day)			    */
    int				LeftFillPixels;			/* count of left fill pixel			    */
    int				RightFillPixels;		/* count of right fill pixel			    */
} Prefix;



/* +----------------------------------------------------------------+ */
/* !   Structure: VNIRFullRes.										! */
/* +----------------------------------------------------------------+ */
typedef struct VNIRFullRes__
{
    unsigned char		Jers_VNIR_data[VNIR_BANDS][VNIR_LINE_PER_SWATH][VNIR_LINE_SZ];
    unsigned short		Sync_Flag[4];
    unsigned int		Swath_Time[4];
} VNIRFullRes;


/* +----------------------------------------------------------------+ */
/* !   Structure: SWIRFullRes.										! */
/* +----------------------------------------------------------------+ */
typedef struct SWIRFullRes__
{
    unsigned char		Jers_SWIR_data[SWIR_BANDS][SWIR_LINE_PER_SWATH][SWIR_LINE_SZ];
    unsigned short		Sync_Flag[4];
    unsigned int		Swath_Time[4];
} SWIRFullRes;


/* +------------------------------------------------------------+ */
/* !   Structure: ImgRec2. level 2										! */
/* +------------------------------------------------------------+ */
typedef struct ImgRec2__
{
    Prefix			PrefixData;			/* instance of prefix structure			    */
    unsigned char		ImgData[VNIR_LEVEL2_PIXELNUMBER];			/* file, record, mission, origin codes		    */
} ImgRec2;

/* +------------------------------------------------------------+ */
/* !   Structure: ImgRec0.level 0										! */
/* +------------------------------------------------------------+ */
typedef struct ImgRec0__
{
    Prefix			PrefixData;			/* instance of prefix structure			    */
    unsigned char		ImgData[VNIR_LEVEL0_PIXELNUMBER];			/* file, record, mission, origin codes		    */
} ImgRec0;



#endif

