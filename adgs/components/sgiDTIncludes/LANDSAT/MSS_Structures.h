#if 0

	Copyright 2007-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. LANDSAT MDPS MSS Processor library $

	$Id$

	$Author$

	$Log: 
 
#endif

#include <smConstants.h>   		/* usr:  Constants header  */
#include <mssConstants.h> 
#include <time.h>			/* sys:  time header */	

#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define _BL_ENDiAN_
#endif	
		
#ifndef __MSS_STRUCTURES_H__
#define __MSS_STRUCTURES_H__


/*
 * local macros
 */

#ifndef __cplusplus
	#ifndef min
	#define min(a, b) ( (a)<(b) ? (a) : (b))
	#endif
	#ifndef max
	#define max(a, b) ( (a)>(b) ? (a) : (b))
	#endif
#endif

#ifndef Mss_bands
#define					Mss_bands	    MSS_NUM_OF_BANDS		/* MSS number of bands		*/
#endif

#ifndef Mss_sensors
#define					Mss_sensors	    MSS_NUM_OF_DET		/* MSS sensor per swath		*/
#endif

#ifndef Mss_line_size
#define					Mss_line_size	MSS_LINE_SIZE	/* line size			*/
#endif

#ifndef Mss_calib_size
#define					Mss_calib_size	MSS_CALIB_SIZE	/* MSS calibration length	*/
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: EOSFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _EOSFlag_
typedef enum EOSFlag_
{
    NOT_DETECTED	    = 0, 				/* end of scan not detected		    */
    DETECTED		    = 1					/* end of scan detected			    */
} EOSFlag;
#define	_EOSFlag_
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: HalfByte.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _HalfByte_
typedef enum HalfByte__
{
    LeastSignificantHalfByte	= 0,
    MostSignificantHalfByte	= 1
} HalfByte;
#define	_HalfByte_
#endif

/* +----------------------------------------------------------------------------------------------------+ */
/* !   Typedef: MJFFlag.                                                                                ! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _MJFFlag_
typedef enum MJFFlag_
{
    NOT_LOSS                = 0,                                /* major frame not loss                     */
    LOSS                    = 1                                 /* major frame loss                         */
} MJFFlag;
#define	_MJFFlag_
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: sctime.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _sctime_
typedef struct sctime__
{
    int32_t			    hundreds_of_days;		/* hundreds of days		4bit	    */
    int32_t			    tens_of_days;		/* tens     of days		4bit	    */
    int32_t			    units_of_days;		/* units    of days		4bit	    */
    int32_t			    tens_of_hours;		/* tens     of hours		4bit	    */
    int32_t			    units_of_hours;		/* units    of hours		4bit	    */
    int32_t			    tens_of_minutes;		/* tens     of minutes		4bit	    */
    int32_t			    units_of_minutes;		/* units    of minutes		4bit	    */
    int32_t			    tens_of_seconds;		/* tens     of seconds		4bit	    */
    int32_t			    units_of_seconds;		/* units    of seconds		4bit	    */
    int32_t			    hundreds_of_milliseconds;	/* hundreds of milliseconds	4bit	    */
    int32_t			    tens_of_milliseconds;	/* tens     of milliseconds	4bit	    */
    int32_t			    units_of_milliseconds;	/* units    of milliseconds	4bit	    */
    int32_t			    fraction_of_milliseconds;	/* fraction of milliseconds (1/16 millisec) */
} SC_TIME;
    
typedef SC_TIME sctime; /* for backward compatibility */

#define	_sctime_
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: EOSType.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct MSS_EOSType_
{
    short int			    MinorFrameNum;		/* minor frame number to be added for finding Last pixel [(MinorFrameNum + 512) * 6 ? ] */ 
    short int			    Sensore;			/* sensor number			    */
    short int			    Band;			/* band number				    */
    short int			    Row;			/* row number				    */
    int				    MnfEOSPosition;		/* minor frame EOS position  Word[5] + 50000 (EOS * 25)	*/
    int				    MnfEOSPosition25;		/* minor frame EOS position  Word[6] * 25 + 50000	*/
} MSS_EOSType;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Auxiliary.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct MSSS_Auxiliary_
{
    short int			    SyncWord[4];		/* synchronization words from the Format Synchronizer	    */
    EOSFlag			    EOSDetected;		/* EOS detected at a particular MNF address		    */
    MJFFlag			    MJFLoss;			/* Major Frame Loss					    */
    sctime			    SpaceCraftTime; 		/* on board time					    */
    short int			    SpaceCraftID;		/* spacecraft identifier				    */
    MSS_EOSType			    EOSAddress;			/* address where End Of Scan was detected		    */
    unsigned char		    SatTimeCode[8];		/* satellite time code at start of scan line		    */
    double			    SCTimeMsecOfDay __attribute__ ((aligned(8)));		/* satellite time millisec of day			    */
    int				    SCTime[8];			/* satellite time yy/mm/dd hh:mm:ss.mmm,m/16		    */
} MSS_Auxiliary;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: ChkAuxiliary.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct MSS_ChkAuxiliary_
{
    int				    LostScanLine;		/* 1 if the scan line is lost 0 otherwise		    */
    int				    CorrespondingScanLineNum;	/* Number of the corresponding scan line in the RawTm file  */
    MSS_Auxiliary		    AuxiliaryValue;		/* instance of the auxiliary class			    */
} MSS_ChkAuxiliary;
    
/* +------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: RawVideo.									    ! */
/* +------------------------------------------------------------------------------------------------+ */

typedef struct MSS_RawVideo__
{
    unsigned char		Mss_data[Mss_bands][Mss_sensors][Mss_line_size+Mss_calib_size];
    unsigned short		Sync_Flag[4];
    unsigned short		Auxiliary[10];
} MSS_RawVideo;


typedef struct MSS_AncillaryDataBlock__
{
    int			scanStartGmtTime;
    unsigned char	zeroFill1[3];		
    unsigned char	syncFlag[5];
    unsigned char	zeroFill2[2];
    unsigned short	minorFrameSyncLossFlag;		
    unsigned char	zeroFill3[52];		 // Total = 68 bytes
    unsigned short	scanLineNum;
    unsigned char	zeroFill4[32];		
    unsigned short	sunAngle;
    unsigned short	dataStartPosition;
    unsigned short	dataStopPosition;	// Total = 108 bytes
    unsigned char	zeroFill5 ;
    unsigned char	uncorrectedLineLength[2];
    unsigned char	satelliteTimeCode[5];	// Total = 116 bytes
    unsigned char	sensorSet ;
    unsigned char	zeroFill6[30] ;		// Total = 147 bytes
    unsigned char	sumDataBand4[3] ;
    unsigned char	sumDataBand5[3] ;
    unsigned char	sumDataBand6[3] ;
    unsigned char	sumDataBand7[3] ;
    unsigned char	sumDataBand8[3] ;
    unsigned char	sumSquareBand4[3] ;
    unsigned char	sumSquareBand5[3] ;
    unsigned char	sumSquareBand6[3] ;
    unsigned char	sumSquareBand7[3] ;
    unsigned char	sumSquareBand8[3] ;
    unsigned char	zeroFill[1];		   		
} MSS_AncillaryDataBlock;


#endif /* !__MSS_STRUCTURES_H__ */
