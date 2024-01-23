/*
 *	This include file contains common structures and tables definition for the Direct Ingestion 
 *	of the following satellites:
 *	    NASA    Landsat Thematic Mapper 		(TM)	Missions:   4, 5
 *	    NASA    Landsat Multispectral Scanner 	(MSS)	Missions:   1, 2, 3, 4, 5
 *	    NASA    Landsat Enhanched Thematic Mapper+ 	(ETM+)	Missions:   7
 */
#ifndef __LAND_STRUCTURES_H__
#define __LAND_STRUCTURES_H__

#include <Ingestion.h>


#ifdef __cplusplus
	extern "C" {
#endif

/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
static char* LANDStructuresId="@(#) Advanced Computer Systems: LAND_Structures.h 1.4 05/07/12";



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   General Purpose defines										! */
/* +----------------------------------------------------------------------------------------------------+ */
/*#define WILMA_ON_DISK_DIRECTORY_NAME_FORMAT "%s/L%1.1d_%s_T%3.3d_S%d_%4.4d%2.2d%2.2d"*/


#define FMT1_ID		    0x00
#define FMT2_ID		    0xFF



/*
 *  Buffer sizes defines
 */
#ifdef INGESTION
#ifdef CIEL_PCI_BOARD
    #define	    Swaths_Per_Buff	    	3
    #define	    Ql_Line_Per_Buff	    8
#endif
 
#ifdef VEXCEL_PCI_BOARD
    #define	    Swaths_Per_Buff	    	6
    #define	    Ql_Line_Per_Buff	    16
#endif

#ifdef CIEL_VME_BOARD
    #define	    Swaths_Per_Buff	    	3
    #define	    Ql_Line_Per_Buff	    8
#endif
#else
    #define	    Ql_Line_Per_Buff	    8
#endif

/*
 *  Track related defines
 */
#define	MaxTrackPointsDes   200			    		/* Max number of pointsdescribing a track		*/
#define	EQUAT_RADIUS	    6378.137L		    	/* Equatorial Radius (a) [Km]					*/
#define	POLAR_RADIUS	    6356.7523L		    	/* Polar      Radius (b) [Km]					*/
#define MEAN_EARTHRADIUS_KM (EQUAT_RADIUS + POLAR_RADIUS) / 2.0L 	/* nominal earth radius kilometers  */     

/*
 *  Defines related to Ingestion
 */
#define INGE_InputBuffers			Buffers*2				/* 2 Mbytes input buffers allocated by 1 formatter */	
#define	INGE_OutputBuffers	    	(int)(Buffers * Swaths_Per_Buff)	/* Swaths allocated from 1 formatter to put QL */
#define INGE_OutputBuffers_Margin   Swaths_Per_Buff			/* margin in QL buffer for synchronisation */


/*
 *  Sensor dependent define
 */
/* Number of swaths in a write block */
#define TM_Fr_Packing_Blocks	    3				/* ~ 2 Mbytes per write */
#define ETM_Fr_Packing_Blocks	    6				/* ~ 4 Mbytes per write */
#define MSS_Fr_Packing_Blocks	    8				/* ~ 1 MBytes per write */
 
/* Number of swaths in the Output buffer */
#define	TM_Fr_Output_Buffers	    (Buffers * TM_Fr_Packing_Blocks)
#define	ETM_Fr_Output_Buffers	    (Buffers * ETM_Fr_Packing_Blocks)
#define	MSS_Fr_Output_Buffers	    (Buffers * MSS_Fr_Packing_Blocks)

/*
 *  DLT format defines
 */
#define LANDSAT_TRANSCRIPTION_FILES 5		/* Thematic Mapper Transcription total number of files	*/


#ifdef __cplusplus
	}
#endif

#endif	/* endif of redefinition */
