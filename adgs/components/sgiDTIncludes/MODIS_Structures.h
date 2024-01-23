/*

	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. MODIS_Structure.h $

	$Id$

	$Author$
	
	$Log$
	Revision 1.9  2008/11/12 13:42:36  enrcar
	EC:: comment added
	

*/


/*
* This include file contains common strucures and table definition for the Direct Ingestion
* of MODIS satellite
*/


#ifndef __MODIS_STRUCTURES_H__
#define __MODIS_STRUCTURES_H__

#ifdef __cplusplus
	  extern "C" {
#endif
 

#define		Modis_DAY_GROUP			0
#define		Modis_NIGHT_GROUP		1
 
#define		Modis_DAY_DLT_Record_Swath_Number       1
#define		Modis_NIGHT_DLT_Record_Swath_Number     3
 
#define 	Modis_N_SAMPLES			830 					/* Samples per MinorFrame during day		*/
#define 	Modis_NIGHT_SAMPLES		170 					/* Samples per MinorFrame during night		*/

#define 	Modis_CALIB_FD			160 			/* Minor Frames of Calibration in a cycle	*/
#define 	Modis_CALIB_PK			(Modis_CALIB_FD*2) 	/* PACKETS of Calibration in a cycle		*/
#define 	Modis_SCIENCE_FD		1354			/* Minor frames of Data in a cycle			*/
#define 	Modis_SCIENCE_DAY_PK		(Modis_SCIENCE_FD*2)	/* PACKETS of DAY Data in a cycle			*/
#define 	Modis_ENG_FD			2			/* Minor Frames of Engeneering in a cycle	*/
#define 	Modis_ENG_PK			(Modis_ENG_FD*2)	/* PACKETS of Engeneering in a cycle		*/
#define 	Modis_LONG_PK_SIZE		642 			/* DAY/CALIB/ENG PACKETS size in bytes		*/
#define 	Modis_SCIENCE_NIGHT_PK		(Modis_SCIENCE_FD)	/* PACKETS of NIGHT Data in a cycle			*/
#define 	Modis_SHORT_PK_SIZE		276 			/* NIGHT PACKETS size in bytes				*/

#define 	Modis_Active_Scan_Time 	451 		/* scan time corresponding to 110 deg around NADIR direction (ms)*/
#define 	Modis_Scan_Time 		1477.17L 	/* scan time corresponding to one mirror cycle (ms)						*/
#define 	Modis_Scan_Time_MAX 	1920.1L 	/* Modis_Scan_Time*1.3 (+30%) */ 
#define 	Modis_Scan_Time_MIN 	1033.9L 	/* Modis_Scan_Time*0.7 (-30%) */ 

#define 	Modis_Scan_Time_JD		1.709687e-5 /* Modis Scan time expressed in days */
#define 	Modis_Scan_Time_JD_MAX	2.222593e-5 /* Modis Scan time expressed in days*1.3 (+30%)  */
#define 	Modis_Scan_Time_JD_MIN	1.196780e-5 /* Modis Scan time expressed in days*0.7 (-30%)  */


/* REAL DAY Cycle size in bytes (= 1946544)*/
#define 	Modis_DAY_CYCLE_SIZE	((Modis_CALIB_PK+Modis_SCIENCE_DAY_PK+Modis_ENG_PK)*Modis_LONG_PK_SIZE)

/* REAL NIGHT Cycle size in bytes  (= 581712)*/
#define 	Modis_NIGHT_CYCLE_SIZE	(((Modis_CALIB_PK+Modis_ENG_PK)*Modis_LONG_PK_SIZE) + (Modis_SCIENCE_NIGHT_PK*Modis_SHORT_PK_SIZE))

/*
 *	Modis Quick Look related defines
 */
#define		Modis_N_bands		    38		/* Total bands number including H an L gain	*/
#define 	Modis_Day_First_Band	0		/* band	1									*/
#define 	Modis_Night_First_Band	21		/* band 20									*/

#define		Modis_SCIENCE_DAY_DATA_SIZE	(Modis_SCIENCE_DAY_PK*Modis_LONG_PK_SIZE) 	/* Day Science data Size in bytes	*/
#define		Modis_SCIENCE_NIGHT_DATA_SIZE	(Modis_SCIENCE_NIGHT_PK*Modis_SHORT_PK_SIZE) 	/* Night Science data Size in bytes	*/

/* Ephemeris data offset from the first ENG PACKET in bytes */
#define		Modis_Relative_Eph_Data_Offset	(2*Modis_LONG_PK_SIZE)		

/* Ephemeris day data offset from cycle start in bytes*/
#define		Modis_DAY_Absolute_Eph_Data_Offset (Modis_SCIENCE_DAY_DATA_SIZE+Modis_Relative_Eph_Data_Offset) 

/* Ephemeris night data offset from cycle start in bytes*/
#define		Modis_NIGHT_Absolute_Eph_Data_Offset (Modis_SCIENCE_NIGHT_DATA_SIZE+Modis_Relative_Eph_Data_Offset) 

#define 	Modis_ENG2_GROUP		4


/*
 *	MODIS Minor Frame Structure: 10 Km x 1 Km ground patch (Nadir value)
 *	Each 12 bits sample is fit in a 16 bits element. 
 *  The structure is 1664 bytes long (64 bits alligned)
 */
//#define Modis_N_SAMPLES 101
typedef struct {
//	u_int16
unsigned short
		raw_data[Modis_N_SAMPLES + 1],
		count,
		dummy;
} Modis_Minor_Frame;


/*
 *  Swath address increments related defines
 */
 

/*
 *	MODIS Swath Header Structure
 */
typedef struct {
	
	unsigned int
		year,
		day,					/* Days of year 							*/
		hour,
		min,
		sec,
		msec,
		fine_time,				/* Number of microsecond of a millisecond 	*/
		scan_type,				/* Day/Night scan				*/
		APID;					/* Application Process ID			*/
#ifndef sgi
	 /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
	 char    __linux_padding[4] ;
#endif
	double
		Satellite_Time; 		/* Milliseconds from the beginning of current year	*/

} Modis_Swath_header; 
 
 

/*
 *	MODIS Modis_FR_Output_Cycle Structure
 */
typedef struct {

	Modis_Swath_header
	head;
	union
	{
		unsigned char
		raw_day_data[Modis_DAY_CYCLE_SIZE];
		unsigned char
		raw_night_data[Modis_NIGHT_CYCLE_SIZE*3];
	}Raw_Cycle;

} Modis_FR_Output_Cycle;



/*
 *	MODIS Major Frame Structure: the swath is 2330 km long	
 */
typedef struct {
	Modis_Swath_header 
			head;
	Modis_Minor_Frame 
			frame[Modis_SCIENCE_FD];
} Modis_Swath;


#ifdef __cplusplus
	}
#endif

#endif  /* endif of the redefinition */
