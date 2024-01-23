/**************************************************************************************
 
    Copyright 1995-2007, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ByteSwap.h HEADER $

    $Author$

    $Id$

    $Log$
    Revision 1.1  2007/03/07 14:40:40  enrcar
    Ingestion.h file added



**************************************************************************************/


/*
* This include file contains common strucures and table definition for the Direct Ingestion
* and Transcription of a general satellite
*/


#ifndef __INGESTION_H__
#define __INGESTION_H__

#ifdef INGESTION_PHASE
#ifdef VEXCEL_PCI_BOARD
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include "pcidif_cmd.h"
#endif
#ifdef CIEL_PCI_BOARD
    #include <sys/mman.h>
    #include <sys/ciel_PCI.h>
#endif
#ifdef CIEL_VME_BOARD
    #include <sys/ciel_VME.h>
#endif
#endif

#ifdef MAX_DEBUG_LEVEL
    #define LOW_DEBUG_LEVEL
#endif

/*#include <tiff.h>*/

#ifdef sgi
#include <CCSDS.h>
#endif
#include <QuickLook/Quick_Look.h>
#include <DataTranscription/dtTypes.h>
#include <station_conf.h>
    



#ifdef __cplusplus
	  extern "C" {
#endif

/* +----------------------------------------------------------------------------------------------------+ */
/* !     SCCS identifier                                                             					! */
/* +----------------------------------------------------------------------------------------------------+ */
static char* IngestionId="@(#) Advanced Computer Systems: Ingestion.h             1.5 05/02/01 (include)";


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   General Purpose defines																			! */
/* +----------------------------------------------------------------------------------------------------+ */

#ifndef	FALSE
    #define FALSE			0
#endif

#ifndef	TRUE
    #define TRUE			1
#endif

#ifndef MEGABYTE
    #define MEGABYTE		1048576
#endif

#define	DAY_TO_MSEC			86400000.0L


#define MAX_PROCESS 		20
#define	MAX_ACQUISITION_MIN	20
#define TIME_OUT_LIMIT		200		    /* 2 seconds in 10 milliseconds ticks */
#define MAX_LOSS_MSECS		480000.0	/* Max 8 minutes = 480 secs = 480000 msec can be loss during acquisition */
#define MAX_CADU_LOST		5			/* Max number of consecutive lost CADUs */
#define MAX_PACKETS_LOST	100			/* Max number of consecutive lost CCSDS packets */

#define MAX_bands			40

#define LOW_PRIORITY		5



/*  Directory name for WILMA ON DISK: example Land7_ETM_T182_S1_20010228_081056 */
#define WILMA_ON_DISK_DIRECTORY_GENERAL_NAME_FORMAT "%s/%s%1.1d_%s_T%6.6d_S%d_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d"


/*
 *  Message queue related defines
 */
#define Max_Message_Queue   100         /* Length of the Message Queue FIFO */ 
#define Print_Line          40          /* Number of Byte on a Diag Printing */


/*
 *  Time Stability defines
 */
#define	MAX_STAB_WIND		    10		/* Maximum number of swaths to check for stability	*/ 
#define TimeWindowToStartAcq	5		/* Number of swaths to check at acquisition start (<MAX_STAB_WIND) */
#define TimeWindowDuringAcq	    2		/* Number of swaths to check during acquisition	(<MAX_STAB_WIND) */
#define JD58toJD50_OFFSET		2922	/* Number of days to add to 1958 to have Jd50 */

/*
 *  Buffer sizes defines
 */
#ifdef INGESTION_PHASE
#ifdef CIEL_PCI_BOARD
    #define	    BuffSize		    CIEL_SINGLE_BUFFER_SIZE
    #define	    Buffers		    	CIEL_ALLOCATED_BUFFER
    #define	    Ql_Line_Per_Buff	8
#endif
 
#ifdef VEXCEL_PCI_BOARD
    /* New (11/02/99) 4 Mbytes buffer for VEXCEL PCI board */
    #define	    BuffSize		    4194304
    #define	    Buffers		    	10
#endif

#ifdef CIEL_VME_BOARD
    #define	    BuffSize		    2097152
    #define	    Buffers		    	20
#endif

#else
    #define	    BuffSize		    2097152
    #define	    Buffers		    	10 
    #define	    Ql_Line_Per_Buff	8
#endif

#define BUFFERS_TO_COPY				2

#define MAX_COPY_DATA 				(BUFFERS_TO_COPY*BuffSize)
#define MAX_CONTIGUOUS_SEARCH		MAX_COPY_DATA


#define MAX_InputBuffersNumber		(Buffers*2)							/* 2 Mbytes input buffers allocated by 1 formatter  	*/	

#define	OutputBuffersNumber			(Buffers*2)							/* Swaths allocated from 1 formatter to put Output		*/
#define OutputBuffers_Margin    	2									/* margin in output buffer for synchronisation  		*/



/*
 *  Quick Look related defines
 */
#define	QlDisplay_Line_size	    500					/* Generic Quick Look Line Size for small Display 	    	*/
#define	QlFr_Line_size		    1000				/* Generic Quick Look Line Size for Full Res on Catalogue   */
#define	QL_Dark_Pixels		    4

#define	Ql_Packing_Blocks	    32					/* Block of lines in Display Buffer 		*/
#define	RGB_Size		    	3					/* Number of Bytes for a Color Pixel		*/
#define 	Ch2					1
#define 	Ch1					0


/*
 *  Track related defines
 */
#if ZORRO
#define	MaxTrackPointsDes   200			    		/* Max number of pointsdescribing a track		*/
#define	EQUAT_RADIUS	    6378.137L		    	/* Equatorial Radius (a) [Km]					*/
#define	POLAR_RADIUS	    6356.7523L		    	/* Polar      Radius (b) [Km]					*/
#define MEAN_EARTHRADIUS_KM (EQUAT_RADIUS + POLAR_RADIUS) / 2.0L 	/* nominal earth radius kilometers  */     
#endif

/*
 *  Defines related to Ingestion
 */

#define	DisplayBuffers		    	(((int)(OutputBuffersNumber / Ql_Packing_Blocks) + 4) * Ql_Packing_Blocks)
#define DisplayBuffersMargin	    Ql_Packing_Blocks
#define	DISPLAY_WARNING_LIMIT	    5								/* Number of Ql Blocks before warn operator 		*/	    	    	    
#define	Error_Limit		    		4								/* Accepted Sync Word Erroneous Bits (Tm and ETM, 32 bits sync words) */

#define FF_SEARCH_MODE				3
#define SLOW_SEARCH_MODE			2
#define START_POINT_FOUND			1

/*
 *  Defines related to Transcription
 */
#define	Ql_Output_Buffers	    	(((int)((Buffers * 8)/ Ql_Packing_Blocks) + 4) * Ql_Packing_Blocks)
#define	Ql_Block_Margin		    	Ql_Packing_Blocks			/* margin in Ql buffer for synchronisation */

#define	MAX_Fr_Packing_Blocks	    10									/* MAX swaths in a block  		*/
#define	MIN_Fr_Packing_Blocks	    1									/* MIN swaths in a block  		*/

#define	MAX_Fr_Output_Buffers	    (Buffers * MAX_Fr_Packing_Blocks)	/* MAX swaths in Output buffer  */
#define	MAX_Fr_Input_Buffers	    (Buffers * MAX_Fr_Packing_Blocks)	/* MAX swaths in Input buffer 	*/


/*
 *  DLT format defines
 */
#define Max_Swath_Per_Segment	    12600   
#define Max_Blocks_Per_Segment	    Max_Swath_Per_Segment / MIN_Fr_Packing_Blocks
#define MAX_SEGMENTS		    	10
#define MAX_SWATHS_TO_READ			(Max_Swath_Per_Segment*MAX_SEGMENTS)

#define WRITING						0
#define WAITS_FOR_SLOTS				1
#define WAITS_FOR_READ_LINES		2
#define FILLING						4


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Global Structures																				! */
/* +----------------------------------------------------------------------------------------------------+ */


typedef struct	{
    uint32   Seek_Flag,
	    Device_Descriptor1, 
	    Device_Descriptor2, 
	    Seek_Position;
}Slot_Descriptor;
		
typedef struct	{
    uint32 Data[BuffSize / 4];
}Input_Buffer_Type;

typedef struct	{
    char
	Message[256], 
	*Type;
} Send_Message_Form;
			
/*
 *  Reduced Resolution Quick Look (for Display Only)
 */
typedef struct	{
    uint32
		Line_Time[3];
    uint8
		Video[QlDisplay_Line_size * RGB_Size],
		Padding[24];
} Rt_Generic_Ql;

/*
 *  Full Resolution Quick Look (for Catalogue or Wide Display)
 */
typedef struct	{
    uint32
		Line_Time[3];
    uint8
		Video[QlFr_Line_size * RGB_Size];
} Fr_Generic_Ql;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Ftdcf.																				! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef INCLUDE_FTDCF_FROM_SMTYPES
typedef struct Ftdcf__
{
    int32_t			MagicNumber;			/* file magic number				    			*/
    int32_t			OrderNumber;			/* order number					    				*/
    int32_t			Station;				/* station identification field			    		*/
    int32_t			Satellite;				/* satellite identification field		    		*/
    int32_t			Mission;				/* mission number				    				*/
    int32_t			Orbit;					/* orbit number					    				*/
    int32_t			Track;					/* track number					    				*/
    int32_t			Frame;					/* frame number					    				*/
    int32_t			WrsPath;				/* WRS path number				    				*/
    int32_t			UTM_Zone;				/* UTM zone number				    				*/
    int32_t			Center_Time[7];			/* GMT frame center time			    			*/
    double			datej;					/* Julian date of frame center time		    		*/
    double			GHA;					/* Greenwich Hour Angle				    			*/
    double			Osculating[6];			/* Keplerian Osculanting Elements (Sma / EarthRadius
												   Eccentricity 
												   Inclination  [rad]
												   ArgOfPerigee [rad]
												   AscNodeLon	[rad]
												   MeanAnomaly  [rad])  						*/
    double			Pos[3];					/* Position vector				    				*/
    double			Vel[3];					/* Velocity vector				    				*/
    double			Radius;					/* Satellite Orbit Radius			    			*/
    double			Height;					/* Satellite Height at Center Frame		    		*/
    double			Heightdot;				/* Time derivative of S/C Height at Center Frame    */
    double			Latitude;				/* Frame Center Latitude [rad]			    		*/
    double			Longitude;				/* Frame Center Longitude [rad]			    		*/
    double			LatitudeDeg;			/* Frame Center Latitude [deg]			    		*/
    double			LongitudeDeg;			/* Frame Center Longitude [deg]			    		*/
    double			Ant_Elevation;			/* Antenna Elevation at Center Frame		    	*/
    double			Ant_Azimuth;			/* Antenna Azimuth at Center Frame		    		*/
    double			Sun_Elevation;			/* Sun Elevation at Center Frame		    		*/
    double			Sun_Azimuth;			/* Sun Azimuth at Center Frame			    		*/
    double			Skew_Angle;				/* Skew Angle at Center Frame			    		*/
    double			Tang_Skew;				/* Tangent of Skew Angle			    			*/
    double			Skew_Angle_dot;			/* Time derivative of Skew Angle		    		*/
    double			Heading;				/* Satellite Heading [ Radians ]		    		*/
    double			Velocity;				/* Nadir Satellite Velocity Km/Sec		    		*/
    double			Velocity_Rate;			/* Velocity Change Rate Km/Sec/Sec		    		*/
    double			UTM_Northing;			/* UTM Norting of Center Frame			    		*/
    double			UTM_Easting;			/* UTM Easting of Center Frame			    		*/
    double			theta;					/* Orientation to UTM grid [deg]		    		*/
    int32_t			DescendingFlag;			/* 1 = descending, 0 = ascending		    		*/
} Ftdcf;
#endif


/* +--------------------------------------------------------------------------------+ */	 
/* !  Prototypes																	! */
/* +--------------------------------------------------------------------------------+ */
void	Output_Orbit_File				(void);
void	Send_Message_Task				(void);

void	Output_Display_Quick_Look_1		(void);
void	Output_Display_Quick_Look_2		(void);

void	Output_Catalogue_Quick_Look		(void);
void	Output_Fr_On_DLT				(void);
void	Output_Fr_On_Disk				(void *arg);

void 		PassEnd_WatchDog				(void);
#ifdef CIEL_PCI_BOARD
    #undef CIEL_VME_BOARD
    #undef VEXCEL_PCI_BOARD
    void	Get_Buffer_From_Ciel_PCI	(void);
#endif    
#ifdef CIEL_VME_BOARD
    #undef CIEL_VME_BOARD
    #undef CIEL_PCI_BOARD
    void	Get_Buffer_From_VME		(void);
#endif
#ifdef VEXCEL_PCI_BOARD
    #undef CIEL_PCI_BOARD
    #undef VEXCEL_PCI_BOARD
    void	Get_Buffer_From_Vexel_PCI	(void);
    int		RemoveBuffers			(int, int);
#endif

#ifdef RAID_SYSTEM
#define WriteProcesses		1
void 	Read_From_RAID			(void);
void	Write_to_RAID			(void);
#else
#define WriteProcesses		5
void	Write_to_Disk0			(void);
void	Write_to_Disk1			(void);
void	Write_to_Disk2			(void);
void	Write_to_Disk3			(void);
void	Write_to_Disk4			(void);
void	Read_from_Disk0			(void);
void	Read_from_Disk1			(void);
void	Read_from_Disk2			(void);
void	Read_from_Disk3			(void);
void	Read_from_Disk4			(void);
#endif

void	Read_From_Dlt			(void);

/****************	FUNCTIONS 	******************/
int 	Allocate_Memory 				(void);
void	Prepare_Radiometric_Tables		(int);
int		DecideCielConfiguration			(int);
int		Initialize_Ingestion			(int, int, int, int, char*, int*);
int		InitializeTranscription			(int, int, int, int);
int 	WriteTheWILMAFileOnDisk 		(char*, char*, int);
void 	Prepare_DTHeader				(ID1_Cassette_Descriptor_File*, 
										 struct station_conf*, 
										 Ftdcf*, 
										 Transcription_File_Description_Header*);
void 	Prepare_QLHeader				(char*, char*, Quick_Look_File_Description_Header*);
int 	Select_The_Bands				(char*, char*, int*, int*, int*);
int 	Position_Dlt_On_Pass 			(int, char**);

/* Message Functions */
void	Local_Send_Msg					(char*, char*);
void	Print_Data						(void*, int, char*);
void	Print_Table						(void*, int, char*);

/* Exit functions */
void	Force_Main_Exit_on_Signal		(void);
void	forceGoHomeMainForSignal		(void);
void	forceGoHomeMainForError 		(void);
void	Kill_All_My_Child				(void);
void	ExitNow 						(char*);
void	Abort_Recover_DLT				(void);
void	DoNothing						(void);

#ifdef __cplusplus
	}
#endif

#endif  /* endif of the redefinition */
