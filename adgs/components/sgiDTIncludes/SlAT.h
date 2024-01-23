/*

	Copyright 1995-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SlAT.h $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2011/03/01 17:00:54  marpas
	GCC 4.4.x support
	
	Revision 1.2  2007/09/11 09:51:12  aleber
	EC:: ACS_Status_Classification conflict solved
	
	Revision 1.1  2006/08/29 16:55:44  enrcar
	imported source
	
	
*/


#ifndef __SlAT_H__
#define __SlAT_H__
/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const * HeaderId="@(#) Advanced Computer Systems: SlAT.h                  1.6 00/08/30 (include)";

#ifdef sgi
#include <bstring.h>
#endif
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statfs.h>

#define	DEBUG_NONE	-1
#define	DEBUG_LOW	0
#define	DEBUG_MID	1
#define	DEBUG_HIGH	2
#define	DEBUG_LEVEL	DEBUG_NONE	

#include <QuickLook/Quick_Look.h>

#define	ENV_SlotInfo	    "ENV_SlotInfo"
#define	ENV_SlotAllocTable  "ENV_SlotAllocTable"
#define	ENV_SatelliteInfo   "ENV_SatelliteInfo"
#define	ENV_LogStoricPath   "ENV_LogStoricPath"

#ifndef TRUE
#define TRUE	1
#endif

#ifndef	FALSE
#define	FALSE	0
#endif

#if	!defined(BLOCKSIZE) || ((BLOCKSIZE) != 512)
#define	BLOCKSIZE   512
#endif

/* #define	    SLOT_TO_UPDATE		-1024 */
#define	    SLOT_TRANSCRIBED	-3
#define	    SLOT_BOOKED			-2
#define	    SLOT_USED			-1
#define	    SLOT_FREE			 0


#define	    PROCLEN			17
#define	    MAX_PROGRESSIVE_COUNTER	999999
#define	    KILOBYTE			1024.
#define	    MEGABYTE			(1024.*1024.)
#define	    BIT_X_BYTE			8.
#define	    BASIC_SLOT_INFO		6

#define	    STR_SIZEOFDISK		"SizeOfDisk ............ ="
#define	    STR_SIZESLOT		"SizeOfSingleSlot ...... ="
#define	    STR_NUMOFSLOTS		"NumberOfSlots ......... ="
#define	    STR_NUMOFCONTROLLERS	"NumberOfControllers ... ="
#define	    STR_NUMOFDISKS		"NumberOfDisks ......... ="
#define	    STR_NUMOFPARTITIONS		"NumberOfPartitions .... ="
#define	    STR_CONTROLLER		"Controller ............ ="
#define	    STR_DEVICEPATHANDNAME	"Device Path And Name .. ="

#define	    STR_VERSIONTYPE		"Version type .......... ="

/*
 * STR_VERSIONTYPE.
 */
#define	    STR_DISKARRAY		"DiskArray"
#define	    STR_RAID			"Raid"

#define	    VERSION_DISKARRAY		1
#define	    VERSION_RAID		2

extern int	VersionType;

#ifndef sgi
    #define    ACS_SGI_PATH_MAX 1024
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Struct: DeviceInfo.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DeviceInfo__ {
    int 		fd;						/* file descriptor DeviceName TO BE OPENED	*/
    int 		ControllerNumber;		/* controller number						*/
#ifdef sgi
    u_char		DeviceName[PATH_MAX];	/* Device File Path and Name (DISKARRAY)	*/
    u_char		ProcessIdName[PATH_MAX];/* ProcessID File Path and Name	(RAID)		*/
#else
    u_char		DeviceName[ACS_SGI_PATH_MAX];	/* Device File Path and Name (DISKARRAY)	*/
    u_char		ProcessIdName[ACS_SGI_PATH_MAX];/* ProcessID File Path and Name	(RAID)		*/
#endif
} DeviceInfo;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Struct: HeaderSlotAllocTable.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct HeaderSlotAllocTable__ {
    int
	ProgressiveCounter;	    			/* Total Number of ingestion (000001 to 999999)*/
} HeaderSlotAllocTable;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Struct: SlotAllocTable.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct SlotAllocTable__ {
    int 		SlotNumber; 			/* Slot Number								*/
    u_char		ProcessID[PROCLEN];		/* Process Identifier that use this slot	*/
    int 		ProcessSlotIndex;		/* Index Slot of process					*/
#ifndef sgi
    /* LINUX PADDING: required before structure multiple by 8 */
    char        __linux_padding1[4] ;
#endif
    double		SlotDate;				/* Slot Date in format						*/
    int 		SlotStatus;				/* Slot Status								*/
#ifndef sgi
    /* LINUX PADDING: required since structure size must be multiple by 8 */
    char        __linux_padding2[4] ;
#endif

} SlotAllocTable;


#ifndef ACS_StatusClassification_DEFINED
#define ACS_StatusClassification_DEFINED 1
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: StatusClassification.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum StatusClassification__
{
    Aborted			= 1,				/* order aborted				    */
    Done			= 2,				/* order completed				    */
    Eligible		= 3,				/* order eligible				    */
 #ifdef sgi
    Failed			= 4					/* order failed					    */
 #else
    Failed			= 4, 				/* order failed					    */
    Scheduled			= 5				/* order scheduled				    */
 #endif   
} StatusClassification;
#endif


/******************************************************************************/
int InitializeSlAT(void);
int 		slatGetDAFreeSpace(double *mbyteFree);
extern int	ReadSatelliteRateInfo (char [], u_short , u_short , int *);
extern int	OpenProcess (u_short, u_short, u_short, int, u_char *, int *);
extern int	GetNextProcessID(u_short SatelliteID,		/* Satellite Identifier		    */
				    u_short SensorID,					/* Sensor Identifier		    */
				    u_short MissionNr,					/* Mission Number		    */
				    u_char *ProcessID);					/* Output Process Identifier string */

extern int	SlotInit (char SlotConfigFileName[],		/* Slot Configuration File Name	    */
			  char SlotTableFileName[],				/* Slot Table File Name		    */
			  char SatelliteInfoFileName[]);			/* Satellite Info File Name	    */

extern DeviceInfo *GetSlotInfo (int *, int *, int *, int *);

extern int	GetOneSlot (u_char ProcessID[],			/* Process Identifier string	    */
			    int *SlotNr);			/* For DISKARRAY Next available slot*/
								/* for RAID or XLV is unused	    */

extern int	CloseProcess (u_char [], StatusClassification);

extern char	**GetAllProcessIDOnTable(int *);

extern int	*SearchForProcessID(u_char [], int *);

extern int	*SearchForProcessSlotIndex(int, int *);

extern int	*SearchForSlotDate(double, int *);

extern int	GetDateOfProcessAtSlot( u_char ProcessID[],	/* Process Identifier string	    */
					int ProcessSlotIndex,	/* Process slot index		    */
					double *jdDate);	/* Output date (JD50 format)	    */

extern int 	EraseTableForProcess(char [], int *);

extern SlotAllocTable * SlotAllocTableStruct(int *);

extern int	GetSatelliteDataRate(int SatelliteID,		/* Satellite Identifier		    */
				     int InstrumentID, 		/* Sensor Identifier		    */
				     int *SatelliteRate);	/* Satellite data transfer rate	    */

extern int	ReadSlotTable (char FilePathName[]);

extern int	SetFirstSlotTime (double time_set_by_user);

extern int	InitializeSlAT (void );

extern int	GetSlotTime (char *ProcessID,
			     double *TimeOfSlot);

extern int	OpenDevices(u_char ProcessID[],	    		/* Process Identifier string	    */
			    int oflag);				/* Flag for open mode (O_RDONLY O_WRONLY)*/

extern char	**SearchBadIngestions(int *);

extern int	GetDiskSpace(char   *Disk_Path,			/* Input: Disk_Path should name a file which resides on that file system*/
			     double *Disk_Size,			/* Output: disk size (in Bytes)	(error +/- 512 Bytes)*/
			     double *Free_Disk_Space);		/* Output: free disk space (in Bytes) (error +/- 512 Bytes)*/

extern int	GetVersionType (void);
extern char	*GetRaidFileName (void);

extern int slatSetPassAsTranscribed(char *ProcessID);
extern char *slatSlotStatusId2String(int SlotStatus);
extern int slatGetPassStatus(char *ProcessID, int *slotStatus);

#endif /* !__SlAT_H__ */
