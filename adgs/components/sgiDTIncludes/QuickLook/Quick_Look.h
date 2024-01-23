/*

	Copyright 1995-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Quick_Look.h $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2006/11/16 14:17:06  lucio.pulvirenti
	Structures moved to libql.h
	
	Revision 1.1  2006/09/27 12:03:48  enrcar
	Directory added
	
	Revision 1.1  2006/08/29 16:55:44  enrcar
	imported source
	
	
*/


/*
 *	This include file contains all the definitions for the description
 *	of the quick look video data file for the following satellite:
 *	    ERS     Earth Resource Satellite		(ERS)	Missions:    1 and 2
 *	    NASA    Landsat Multi Spectral Scanner	(MSS)	Missions:    1, 2, 3, 4, and 5
 *	    NASA    Landsat Thematic Mapper		(TM)	Missions:    4, 5, and 6
 *	    NASDA   Marine Observation Satellite	(MOS)	Missions:    1 and 2
 *	    NASDA   Japanese Earth Resources Satellite	(JERS)	Mission	:    1
 *	    CNES    Satellite Pour l' Observation de la Terre (SPOT) Missions 1, 2 and 3
 *  	CONAE  Satelite de Aplicaciones Cientificas C (SAC-C) Mission: 1
 */

/*----------------------------------------------------------------------------------*
*										    *
*	define section								    *
*										    *
*-----------------------------------------------------------------------------------*/ 
/*----------------------------------------------------------------------------------*
*	general defines								    *
*-----------------------------------------------------------------------------------*/ 
#ifndef QuickLook_H
#define QuickLook_H

/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const* QuickLookhId="@(#) Advanced Computer Systems: Quick_Look.h            1.31 01/04/02 (include)";

#ifdef __cplusplus
	extern "C" {
#endif

#define Upper_Position	995										/* Upper Window Position					*/
#define Lower_Position	10										/* Lower Window Position					*/
#define Display_V_Size_QL 981									/* Number of lines on screen				*/
#define Display_V_Size	(Upper_Position - Lower_Position) 		/* Number of lines on screen				*/

#define	QL_Dark_Pixels	4										/* Pixels Containing Time Information 		*/

#define	Magic_Word	17111949									/* Magic Word for Quick Look RGB File		*/

/*----------------------------------------------------------------------------------*
*	Define the Values for the Transcription mode flag "Transcriptionflag"	    	*
*	in QL_CONF.$STC of the MDPS station					    						*
*-----------------------------------------------------------------------------------*/
#define ONLY_QUICKLOOK		    	0
#define QUICKLOOK_AND_TRANSCRIPTION 1
#define ONLY_TRANSCRIPTION	    	2
#define ONLY_SCREENING		    	3

/* for compatibility loads includes */
#include <acsSatellites.h>		
#include <acsStations.h>
#include <libql.h>		
		
/*----------------------------------------------------------------------------------*
*	Define the Values for the Field "Video_Format"				    *
*-----------------------------------------------------------------------------------*/ 
#define	BW	1			/* Quick Look Video Format is Black & White				*/
#define	RGB	3			/* Quick Look Video Format is Red Green Blue (RGB)			*/


/*#ifdef NOT_IN_QL_LIBRARY*/
/*----------------------------------------------------------------------------------*
*										    *
*	structures definition section						    *
*										    *
*-----------------------------------------------------------------------------------*/ 
/****************************************************************************************************************
	Quick_Look_File_Description_Header
****************************************************************************************************************/

// 	typedef struct Quick_Look_File_Description_Header__
// 	{
// 		unsigned int
// 			File_Key;		/* This Key is Used to Recognize the Quick Look RGB Files	*/
// 
// 		unsigned short
// 
// /*
//  *	Instrument and Mission Identification Fields
//  */
// 
// 			Satellite_ID,		/* Satellite Identification Flag:
// 
// 						1 = NASA Landsat (Landsat)
// 						2 = NASDA Marine Observation Satellite (MOS)
// 						3 = NASDA Japanese Earth Resources Satellite (JERS)
// 						4 = CNES Satellite Pour l' Observation de la Terre (SPOT)
// 						5 = ESA Earth Resources Satellite (ERS)				
// 						6 = ESA Earth Resources Satellite (ERS)				*/
// 
// 			Mission_ID,		/* Mission Identification Number
// 
// 						1 - 6 -> NASA Landsat
// 						1 - 2 -> NASDA Marine Observation Satellite (MOS)
// 						1 -----> NASDA Japanese Earth Resources Satellite (JERS)
// 						1 - 3 -> CNES Satellite Pour l' Observation de la Terre (SPOT)
// 						1 -----> ESA Earth Resources Satellite (ERS)			*/
// 
// 			Instrument_Type_ID,	/* Instrument Type Identification Flag:
// 
// 						1  = Landsat Multi Spectral Scanner (MSS)
// 						2  = Landsat Thematic Mapper (TM)
// 						3  = Landsat Enhanced Thematic Mapper (ETM)
// 						4  = Landsat Return Beam Vidicon (RBV)
// 
// 						5  = MOS Multispectral Electronic Self Scanning Radiometer (MESSR)
// 
// 						6  = JERS Visible and Near Infrared Radiometer (VNIR)
// 						7  = JERS Short Wave Infrared Radiometer (SWIR)
// 
// 						8  = SPOT Panchromatic (Pan)
// 						9  = SPOT Multispectral Scanner (XS)
// 
// 						10 = ERS Synthetic Aperture Radar (SAR)
// 						11 = ERS Along Track Scanning Radiometer (ATSR)			*/
// 
// 			Instrument_Number,	/* Instrument Number:
// 
// 						SPOT Case:
// 						1  = SPOT High Resolution Visible Number 1
// 						2  = SPOT High Resolution Visible Number 2
// 
// 						MOS Case:
// 						1  = Camera 1
// 						2  = Camera 2							*/
// 
// /*
//  *	Ground Station Identification Fields
//  */
// 			Station_ID,		/* Receiving Ground Station Identification Field		*/
// 
// 			Hddt_Number,		/* High Density Digital Tape Number				*/
// 
// /*
//  *	Quick Look Video Data Format Identification Fields
//  */
// 
// 			Video_Format,		/* Format of the Video Data:
// 
// 						1 = Black and White (BW) Format
// 						3 = Red, Green, Blue (RGB) Format				*/
// 
// 			Lines_per_Jpeg_Block,	/* Number of Compressed Lines per Jpeg Block			*/
// 
// 			Line_Size,		/* Length of the Quick Look Video Line for the Given Sensor	*/
// 
// 			Frame_Size,		/* Length of the Quick Look Frame fof the Given Sensor		*/
// 
// 			Spectral_Bands[3],	/* Selected Spectral Bands for Red, Green, Blue			*/
// 
// 			Compression_Flag;	/* Flag for Video Data Compression:
// 
// 						FALSE = No Compression Applied
// 						TRUE  = JPEG Compression Applied				*/
// 
// /*
//  *	Orbit and Acquisition Identification Fields
//  */
// 		int
// 
// 			Track_Number,		/* Track Number (if applicable)					*/
// 
// 			Orbit_Number,		/* Orbit Number (if applicable)					*/
// 
// 			Number_of_Frames,	/* Number of Acquisible Frames for the Present Track		*/
// 
// 			First_Frame;		/* First Frame Realy Acquired					*/
// 
// 		double
// 
// 			QL_Line_Time,		/* Fly Time for Acquiring a Single Quick Look Line * 10000	*/
// 
// 			QL_Ifov_X,		/* Pixel Size in the X direction in radiants			*/
// 
// 			QL_Ifov_Y;		/* Pixel Size in the Y direction in radiants			*/
// 
// 		unsigned short
// 
// 			QL_X_Offset,		/* Pointing offset in X direction of Quick Look			*/
// 
// 			Acquisition_Date[3],	/* Acquisition Date of the Satellite Pass in Year, Monts, Days	*/
// 
// 			Acquisition_Day,	/* Day in the Year of the Acquisition				*/ 
// 
// 			Acquisition_Start[4],	/* Start of Acquisition in Hours, Min, Sec, Millisec		*/
// 
// 			Acquisition_End[4],	/* End of Acquisition in Hours, Min, Sec, Millisec		*/
// 
// 			Production_Date[3],	/* Production Date of the Satellite Pass in Days, Monts, Year	*/
// 
// 			Production_Start[3],	/* Start of Production in Hours, Minute, Seconds		*/
// 
// 			Production_End[3];	/* End of Production in Hours, Minute, Seconds			*/
// 
// 		unsigned int
// 
// 			Loaded_Lines;		/* Number of Lines Loaded on Disk				*/
// 
// 		unsigned int
// 
// 			Mission_Dependent[100];	/* Fields for mission dependent data and contingenty		*/
// 
// 	} Quick_Look_File_Description_Header;


/****************************************************************************************************************
*****************************************************************************************************************
	QL_Jpeg_Block_Descriptor
*****************************************************************************************************************
****************************************************************************************************************/

// 	typedef struct	QL_Jpeg_Block_Descriptor__
// 	{
// 		double
// 				Satellite_Time;
// 
// 		unsigned int
// 				Delta_Time,
// 				Jpeg_Block_Size;
// 
// 	} QL_Jpeg_Block_Descriptor;
/*#endif*/

/****************************************************************************************************************
*****************************************************************************************************************
	QL_Jpeg_Block_Descriptor_Int
*****************************************************************************************************************
****************************************************************************************************************/

	typedef struct	QL_Jpeg_Block_Descriptor_Int__
	{
		unsigned int
			Satellite_Time[2],
			Delta_Time,
			Jpeg_Block_Size;

	} QL_Jpeg_Block_Descriptor_Int;


/****************************************************************************************************************
*****************************************************************************************************************
	Cloud_Cover
*****************************************************************************************************************
****************************************************************************************************************/

	typedef struct Cloud_Cover__
	{
		short
			Frame_Number,		/* Number of the Frame						*/
			WrsPath,		/* world reference ssystem path number				*/
			DescendingFlag,		/* 1: descending, 0: ascending					*/
			Years,			/* Years of Center Frame from first of Genuary			*/
			Months,			/* Months of Center Frame from first of Genuary			*/
			Days,			/* Days of Center Frame from first of Genuary			*/
			Hours,			/* Hours of Center Frame					*/
			Minutes,		/* Minutes of Center Frame					*/
			Seconds,		/* Seconds of Center Frame					*/
			Milliseconds,		/* Milliseconds of Center Frame					*/

			CF_Lat,			/* Frame Center Latitude in Hundred of Degrees (-18000, 18000)	*/
			CF_Lon,			/* Frame Center Longitude in Hundred of Degrees (-9000, 9000)	*/

			NW_Lat,			/* North West Pixel Latitude in Hundred of Degrees		*/
			NW_Lon,			/* North West Pixel Longitude in Hundred of Degrees		*/

			NE_Lat,			/* North East Pixel Latitude in Hundred of Degrees		*/
			NE_Lon,			/* North East Pixel Longitude in Hundred of Degrees		*/

			SW_Lat,			/* South West Pixel Latitude in Hundred of Degrees		*/
			SW_Lon,			/* South West Pixel Longitude in Hundred of Degrees		*/

			SE_Lat,			/* South East Pixel Latitude in Hundred of Degrees		*/
			SE_Lon,			/* South East Pixel Longitude in Hundred of Degrees		*/

			Sun_Elevation,		/* Sun Elevation at Center Frame				*/
			Sun_Azimuth;		/* Sun Azimuth at Center Frame					*/

		char
			NorthWest,		/* Cloud Coverage Vote:	Values from '0' to '9'			*/
			NorthEast,
			SouthWest,
			SouthEast,

			Acq_Vote, 		/* Acquisition Vote:
						    '0' no data loss, '3' = 3 or More Lines Lost, '-' Not Available	*/
			Satur_Vote;		/* added by A. D'elia e Bencivenni */
	} Cloud_Cover;


/****************************************************************************************************************
*****************************************************************************************************************
	Frame_List
*****************************************************************************************************************
****************************************************************************************************************/

	typedef struct	Requested_Frame_List__
	{
		unsigned int
			File_Key;		/* This Key is Used to Recognize the Quick Look RGB Files	*/

		unsigned short

/*
 *	Instrument and Mission Identification Fields
 */

			Satellite_ID,		/* Satellite Identification Flag:

						1 = NASA Landsat (Landsat)
						2 = NASDA Marine Observation Satellite (MOS)
						3 = NASDA Japanese Earth Resources Satellite (JERS)
						4 = CNES Satellite Pour l' Observation de la Terre (SPOT)
						5 = ESA Earth Resources Satellite (ERS)				*/

			Mission_ID,		/* Mission Identification Number

						1 - 6 -> NASA Landsat
						1 - 2 -> NASDA Marine Observation Satellite (MOS)
						1 -----> NASDA Japanese Earth Resources Satellite (JERS)
						1 - 3 -> CNES Satellite Pour l' Observation de la Terre (SPOT)
						1 -----> ESA Earth Resources Satellite (ERS)			*/

			Instrument_Type_ID,	/* Instrument Type Identification Flag:

						1  = Landsat Multi Spectral Scanner (MSS)
						2  = Landsat Thematic Mapper (TM)
						3  = Landsat Enhanced Thematic Mapper (ETM)
						4  = Landsat Return Beam Vidicon (RBV)

						5  = MOS Multispectral Electronic Self Scanning Radiometer (MESSR)

						6  = JERS Visible and Near Infrared Radiometer (VNIR)
						7  = JERS Short Wave Infrared Radiometer (SWIR)

						8  = SPOT Panchromatic (Pan)
						9  = SPOT Multispectral Scanner (XS)

						10 = ERS Synthetic Aperture Radar (SAR)
						11 = ERS Along Track Scanning Radiometer (ATSR)			*/

			Instrument_Number,	/* Instrument Number:

						SPOT Case:
						1  = SPOT High Resolution Visible Number 1
						2  = SPOT High Resolution Visible Number 2

						MOS Case:
						1  = Camera 1
						2  = Camera 2							*/

/*
 *	Ground Station Identification Fields
 */
			Station_ID,		/* Receiving Ground Station Identification Field		*/

			Compression_Flag;	/* Flag for Video Data Compression:

						FALSE = No Compression Applied
						TRUE  = JPEG Compression Applied				*/

/*
 *	Orbit and Acquisition Identification Fields
 */
		int

			Track_Number,		/* Track Number							*/

			Frame_Number;		/* Requested Frame 						*/

		unsigned short

			Starting_Date[3],	/* Start of the Period of Interest in Year, Monts, Days		*/

			Ending_Date[3];		/* End of the Period of Interest in Year, Monts, Days		*/
	} Requested_Frame_List;

#ifdef __cplusplus
	}
#endif

#endif
