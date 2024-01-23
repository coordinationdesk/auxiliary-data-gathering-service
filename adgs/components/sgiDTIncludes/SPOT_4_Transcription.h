/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SPOT_4_Transcription.h $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2012/02/20 13:58:05  marpas
	refactoring (ACS_ByteSwap.h is in libFile
	
	Revision 1.2  2006/08/29 17:09:12  enrcar
	Include added
	
	Revision 1.1  2006/08/25 12:27:54  enrcar
	SPOT_4_Transcription.h: file added
	
	
*/


/*  Based on:  SPOT_4_Transcription.h Date: 2001/07/31 17:02:58 Revision: 1.8  */


#ifndef Spot_4_Transcription__
#define Spot_4_Transcription__

/*Version string*/
static const char SPOT_4_Transcription_h1[]="ACSFILE: $Date$ $Name$," __DATE__ ",REV_$Revision$," __FILE__;
static const char SPOT_4_Transcription_h2[]="ACSSYS : PROJ-SPOT-04, autor G.Trevisiol C. Nasuti," __FILE__;


#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/mtio.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
 
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <SPOT/Spot.h>
#include <SPOT/Spot4.h>
#if(0)
    #include <device.h>
    #include <bstring.h>
    #include <gl/gl.h>
    #include <gl/device.h>
    #include <ulocks.h>
#else
    #warning "missing includes..." 
#endif
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <errno.h>
#include <libgen.h>
#if(0)
    #include <libenv.h>
    #include <libmw.h>
    #include <MovingWindow.h>
#else
    #warning "missing defines..." 
#endif
#include <SlAT.h>
/* #include <SPOT/Spot_Structures.h> */
#include <QuickLook/Quick_Look.h>
#include <station_conf.h>
#include <DirectIngestionOrder.h>
#include <DataTranscription/dtTypes.h>
#include <DataTranscription/dtMagic.h>
#include <Rm.h>
#include <libmdpsc.h>
#include <Spot_4_Structures.h>


extern int errno;

/******** They are not needed anymore. We use an external define
#define	RAID_SYSTEM
#define	NO_DLT
#define	SPOT_FUCINO
*********/
/* #define  Exit_Threshould    (600.0 / 86400) */  
/* Threshould of exit */


#define	BuffSize		(1024*1024*2)
#define	Line_Per_Buff		120
#define	Buffers				12
#define	Ql_Packing_Blocks	128
#define	QlOutputBuffers		((int)((Buffers * Line_Per_Buff) / Ql_Packing_Blocks) * Ql_Packing_Blocks)
#define	Fr_Packing_Blocks	150		/* ~ 2 MBytes per write */
#define	FrOutputBuffers		(((Buffers * Line_Per_Buff) / Fr_Packing_Blocks) * Fr_Packing_Blocks)
#define	FR_Block_Margin		(Fr_Packing_Blocks * 3)
#define	Ql_Block_Margin		(Ql_Packing_Blocks * 7)

#define	Error_Limit		6		/* Accepted Sync Word Erroneous Bits after Sync Status	*/
#define	Max_Search_Attempt	5		/* Attempt to Find Sync Word in a Given Buffer Position	*/
#define	Search_Step_Jump	20		/* Step (In Frame Equivalent) when Sercing Sync Word	*/
#define	Init_Min_Sync_Count	10		/* Consecutive Good Sync Word to Enter in Sync Status	*/ 
#define	Foll_Min_Sync_Count	4		/* Consecutive Good Sync Word to Regain Sync Status	*/ 
#define	Good_Consecutive_Lines	50

#define Max_Lines_Per_Segment	300000   
#define Max_Blocks_Per_Segment	Max_Lines_Per_Segment / Fr_Packing_Blocks
#define MAX_SEGMENTS		10
#define SPOT_TRANSCRIPTION_FILES  6		/* ERS Transcription total number of files			*/
#define	Time_to_Wait		10 

#define	Sample_Smute		5		/* Filter in the Transition Status Pan - Xs		*/

#define	QL_Dark_Pixels		4

#define	Max_Message_Queue	100

#define	Print_Line		40

#define	Max_On_Board_Time_Diff	2000

#define DBFILENAME		"Satellite.conf"
#define SYSTEMATIC_PATH "Systematic_Path"
#define JULIAN_DAY_SHIFT_PAR	    "JulianDateOffset"
#define TWIN_FLAG                   "TwinFlag" 
#define TIME_INTERVAL               "TimeInterval" /*caldulated in seconds*/
#define EXIT_THRESHOLD				"Exit_Threshould" /*caldulated in seconds*/
/**
#define	MAX_DEBUG_LEVEL
#define	LOW_DEBUG_LEVEL
**/

#define LOW_PRIORITY		5

#define DLT_MAX_NUM     10 /* this value is temporary*/

void	forceGoHomeMainForSignal	(void);
int		InitializeIngestion		(int, int, int, int);


int		Get_Minor_Frame_and_Bit_Shift	(void*, int, int*, u_int*);
int		Check_Minor_Frame		(void*);
void	Formatter			(void);
void	Output_Fr_Channel_1		(void);
void	Output_Fr_Channel_2		(void);
void	Output_Fr_Channel_1_clone	(void);
void	Output_Fr_Channel_2_clone	(void);

void	Output_Ql_Channel_1		(void);
void	Output_Ql_Channel_2		(void);
void	Output_Orbit_File		(void);
void	Decode_Auxiliary		(u_short*, u_char*, int, u_char*);
void	Decode_Pan_Lin			(u_short*, u_char*, int, u_char*);
void	Decode_Pan_Dcpm			(u_short*, u_char*, int, u_char*);
void	Decode_Xs				(u_short*, u_char*, int, u_char*);
void	Decode_Ql_Pan			(void*, void*, u_char*);
void	Decode_Ql_Xi 			(void* , void* );
void	Decode_Ql_Xs			(void*, void*, u_char*);
void	Subsample_Ql			(void*, void*);
void	Copy_Ql_Auxiliary		(Generic_Spot4*, Spot_4_Auxiliary*);
void	Diag_Printing			(void*, int, int, void*, char*);
void	Print_Data			(void*, int, char*);
void	Print_Table			(void*, int, char*);
void	Read_from_Disk0			(void);
void	Read_from_Disk1			(void);
void	Read_from_Disk2			(void);
void	Read_from_Disk3			(void);
void	Read_from_Disk4			(void);
void	Send_Message_Task		(void);
void	Local_Send_Msg			(char*, char*);

/*@Updated 4*/
int 											/* 1 if Out is valid 0 if value aren't stable    	*/
Decode_Time_Param(	SpotInfo *OutInfo,	 		/* SpotInfo filled only in record neded for time  	*/
					Spot_4_Auxiliary  *InAux);	/* Spot ancillary decoded from line					*/

int 												/* 1 if Out is valid 0 if value aren't stable    	*/
Calculate_Obt_stability(const unsigned int *Obt); 	/*on board time*/



int
Reformat_Spot123_Line(	Generic_Spot* 	OutBuffer, 	/*Output Buffer in spot 123 mode 	*/
						Generic_Spot4*	InBuffer,	/*Input Buffer in spot 4 mode 		*/
						int 			NumLine);	/*Number of line to reformatted 	*/

#ifdef SOUTH_AFRICA_OLD_AUX
int
Reformat_Spot123_Line_South_Africa(Generic_Spot_South_Africa* 	OutBuffer, 	/*Output Buffer in spot 123 mode 	*/
						Generic_Spot4*	InBuffer,	/*Input Buffer in spot 4 mode 		*/
						int 			NumLine);	/*Number of line to reformatted 	*/

#endif /* SOUTH_AFRICA_OLD_AUX */

void	Decode_Xi_Dcpm (unsigned short*, unsigned char*, unsigned int, unsigned char*);

/**************************************************************************
 *	 Open and Read Configuration file and destroy old database
 **************************************************************************/
 
int                                        /*OUT: 0 if success or -1 if Error occured*/ 
Load_Configuration( char *name,            /*IN: base name of file to read */
		        int *ConfDb);           /*IN,OUT: Pointer to Rm data base */

/*@Updated 4*/

int	SPOTComputeLineTime		(double *, int *,u_long, u_int, SpotInfo *);
int	SatelliteDataIsStable		(u_int, u_int, u_int*);
int	BitDifference			(int, int);
void	Kill_All_My_Child		(void);
void	Abort_Recover_DLT		(void);

	typedef struct	{
	    int	    Seek_Flag,
		    Device_Descriptor1, 
		    Device_Descriptor2;
	    u_int
		    Seek_Position;
	}Slot_Descriptor;
			
	typedef struct	{
	    u_int Data[BuffSize / 4];
	}Ciel_Buffer;
	
	typedef struct	{
	    char
		Message[256], 
		*Type;
	} Send_Message_Form;

#endif /* Spot_4_Transcription__ */
