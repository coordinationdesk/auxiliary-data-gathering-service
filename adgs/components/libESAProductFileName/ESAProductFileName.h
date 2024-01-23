// PRQA S 1050 EOF
/* 
    Copyright 1995-2014, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ESAProductFileName Library - HEADER FILE $

    $Id$

    $Author$
    
    $Log$
    Revision 1.34  2014/03/25 12:48:32  marpas
    qa warnings fixing

    Revision 1.33  2012/03/01 16:25:35  marpas
    using acs::base for default static paths

    Revision 1.32  2012/03/01 16:24:52  marpas
    using acs::base for default static paths

    Revision 1.31  2008/05/26 09:21:04  enrcar
    EC:: Comments heavily misleading -- fixed

    Revision 1.30  2008/01/14 11:22:23  enrcar
    Interface modified to compute CRC directly from user header file

    Revision 1.29  2007/07/13 14:48:38  enrcar
    EC:: Deprecated method removed

    Revision 1.28  2007/06/22 15:27:19  enrcar
    EC:: Deprecated methods removed

    Revision 1.27  2007/02/08 16:17:07  enrcar
    EC:: Bug in CEIL rounding when getting stop validity --Fixed

    Revision 1.26  2007/01/31 18:05:56  enrcar
    ACS_ESAProductFileName_GetFiletypeFromKeyword, ACS_ESAProductFileName_GetKeywordFromFiletype methods added

    Revision 1.25  2007/01/29 16:44:51  enrcar
    EC:: bug fixed in .h interface

    Revision 1.24  2007/01/26 12:12:44  aleber
    function ACS_ESAProductFileName_GetFiletypeName added that converts File Type value in File Type key

    Revision 1.23  2007/01/26 10:32:14  davcas
    Suppressed warning message in compile

    Revision 1.22  2007/01/22 16:36:01  enrcar
    EC:: Some comments were misleading -- fixed

    Revision 1.21  2007/01/19 14:37:37  aleber
    method ACS_ESAProductFileName_GetFiletypeKey() added, that converts a internal key in a file type compliant to ESA standards

    Revision 1.20  2006/12/05 10:50:28  enrcar
    Added CRC Parameters

    Revision 1.19  2006/12/01 11:07:32  enrcar
    ACS_ESAProductFileName_JD50ToDate : now handles usec instead of msec

    Revision 1.18  2006/11/14 10:28:18  enrcar
    SGI Compatibility: INT used instead of BOOL

    Revision 1.17  2006/11/13 18:05:29  enrcar
    GetFilenameFromStructure: added parameter to handle both bigendian and littleendian structures

    Revision 1.16  2006/10/03 15:52:01  enrcar
    Force to use V.1.5 removed -- now version 1.7 will be used
    ACS_ESAProductFileName_GetNameFromStatistics : CRC added to input arguments

    Revision 1.15  2006/09/29 08:55:50  enrcar
    During a temporary transition period, ESA Naming Convention PRE-1.7 (i.e. 1.5) will be forced. To enable the new feature, just remove the FIXME section.

    Revision 1.14  2006/09/26 09:57:43  enrcar
    Updated to ESA Product File Name convention Rev. 1.7

    Revision 1.13  2006/09/25 10:23:19  enrcar
    ESA versioning added

    Revision 1.12  2006/09/13 09:02:02  enrcar
    ACS_ESAProductFileName_GetSatelliteName method added

    Revision 1.11  2006/07/11 14:10:41  enrcar
    ACS_ESAProductFileName_GetSatelliteKey Method added

    Revision 1.10  2006/06/27 13:52:41  enrcar
    unable to compile under sgi. fixed.

    Revision 1.9  2006/06/27 13:17:04  enrcar
    ACS_ESAProductFileNameJD50ToDate mathod added
    dateToJD50 renamed ACS_ESAProductFileName_DateToJD50 and made public

    Revision 1.8  2006/06/27 12:57:28  enrcar
    ACS_ESAProductFileName_GetParams, ACS_ESAProductFileName_CheckFileNameIsValid, ACS_ESAProductFileName_GetStationID methods added

    Revision 1.7  2006/06/08 15:19:18  enrcar
    ACS_ESAProductFileName_Endian_DTStatistic_Converter Method added
    ACS_ESAProductFileName_GetNameFromStatistic always use BIGEND structure

    Revision 1.6  2006/06/08 11:56:18  aleber
    getStationId method made public and renamed ACS_ESAProductFileName_GetStationId

    Revision 1.5  2006/05/24 14:26:37  enrcar
    Required includes added

    Revision 1.4  2006/05/24 13:54:29  enrcar
    ACS_ESAProductFileName_GetNameFromStatistics : method added
    ACS_ESAProductFileName_ToJD50 : method removed and made static
  
    Revision 1.3  2006/05/18 13:28:49  davcas
    EC:: Extension length was 4 (so vector size must be 5). FIXED.

    Revision 1.2  2006/05/18 12:50:56  enrcar
    method ACS_ESAProductFileName_ToJD50 added

    Revision 1.1.1.1  2006/05/17 09:22:06  enrcar
    imported source

    
*/ 


#ifndef __LIBESAPRODFN_H__
#define __LIBESAPRODFN_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <DataTranscription/dtTypes.h> // PRQA S 1013 2
#include <DataTranscription/dtMagic.h>
#pragma GCC diagnostic pop

/*
	ESA CRC Defines
*/
/* Name: "CRC-16/CITT" -- Check: 0x29B1 */
#define ACS_ESACRC_TB_WIDTH		16
#define ACS_ESACRC_TB_POLY		0x00001021
#define ACS_ESACRC_INIT			0x0000FFFF
#define ACS_ESACRC_TB_REFIN		FALSE   /* PRQA S 1021 2 */
#define ACS_ESACRC_TB_REFOT		FALSE
#define ACS_ESACRC_XOROT		0
#define ACS_ESACRC_CRC_CHECK	0x29B1



/*
 * DESCRIPTION: This function returns the output filename, obtained merging given input-parameters 
 * and accessing static tables (configuration files).
 *
 */
int                                         		/* OUT: 0 success */
        ACS_ESAProductFileName_GetName
        (
            const char* satelliteName,              /* IN: Satellite ACS name (key in private conf file) */
            const unsigned char missionNum,         /* IN: Mission number */
            const char* filetype,                   /* IN: ACS code for the filetype (key in private conf file) */
            const double validityStartTime,         /* IN: Validity Start (JD50) */
            const double validityStopTime,          /* IN: Validity Stop (JD50) */
            const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from ACS_Stations_Table.conf) */
            const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
            const int safeManifestCrc,              /* IN: Optional: CRC , -1 if not used */
            const char fileExtension[5],            /* IN: File extension */ // PRQA S 4403 EOF
            const char* basePath,                   /* IN: Optional base path for conf files. Otherwise default is base::defaultConfPath()/local/ */
            char outputFileName[70]                 /* OUT: file name */ 
        ) ;            



/*
 * DESCRIPTION: Same as ACS_ESAProductFileName_GetName, but CRC is evaluated by a given filename
 *
 */
int                                         			/* OUT: 0 success */
        ACS_ESAProductFileName_GetName_ComputeCRC
        (
            const char* satelliteName,              	/* IN: Satellite ACS name (key in private conf file) */
            const unsigned char missionNum,         	/* IN: Mission number */
            const char* filetype,                   	/* IN: ACS code for the filetype (key in private conf file) */
            const double validityStartTime,         	/* IN: Validity Start (JD50) */
            const double validityStopTime,          	/* IN: Validity Stop (JD50) */
            const unsigned char originatingFacility,	/* IN: Originating Facility ACS code (from ACS_Stations_Table.conf) */
            const unsigned int absoluteOrbitNum,    	/* IN: Absolute Orbit Number */
            const char* fileToGetCrcFrom,           	/* IN: Optional: filename, or NULL to skip CRC */
            signed long long int expSize,           	/* IN: Optional: file size (will be checked), or -1 to skip check */
            const char fileExtension[5],            	/* IN: File extension */
            const char* basePath,                   	/* IN: Optional base path for conf files. Otherwise default is base::defaultConfPath()/local/ */
            char outputFileName[70]                 	/* OUT: file name */ 
        ) ;            


/*
 * DESCRIPTION: This function disassembles the output filename, obtaining originating parameters
 * (i.e. the corresponding keys of configuration files).
 *
 */
int                                                     /* OUT: 0 success */
        ACS_ESAProductFileName_GetParams(
                  	const char* inputFileName,			/* IN: File name */
                    const char* basePath,               /* IN: Optional base path for conf files. Otherwise default is base::defaultConfPath()/local/ */
				    char* satelliteName,           		/* OUT: Satellite ACS name (key in private conf file) */
                    unsigned char* missionNum,         	/* OUT: Mission number */
                    char* fileType,                	    /* OUT: ACS code for the filetype (key in private conf file) */
                    double* validityStartTime,         	/* OUT: Validity Start (JD50) */
                    double* validityStopTime,          	/* OUT: Validity Stop (JD50) */
                    unsigned char* originatingFacility,	/* OUT: Originating Facility ACS code (from ACS_Stations_Table.conf) */
                    unsigned int* absoluteOrbitNum,    	/* OUT: Absolute Orbit Number */
                    int* safeManifestCrc,              	/* OUT: Optional: CRC , -1 if not used */
                    char fileExtension[5]				/* OUT: File extension */
		) ;       


/*
 *  DESCRIPTION: This function returns the 10-chars (+ null) FILETYPE given the
 *    KEYWORD in ACS Filetypes conf-file
 *  EXAMPLE: INPUT(KEYWORD): ORBVIEW_SEAWIFS_AR  -> OUTPUT(FILETYPE): SWF_L1A_1P
 */
int ACS_ESAProductFileName_GetFiletypeFromKeyword(
                    const char* in_ftKeyword,        /* IN: fileType Keyword (key in private conf file) */
                    const char* basePath,            /* IN: Optional base path for conf files */
                    char fileType[11]) ;             /* OUT: filetype */ 


/*
 *  DESCRIPTION: This function returns the KEYWORD in ACS Filetypes conf-file
 *  given the 10-chars (+ null) FILETYPE taken from a valid ESA Filename
 *  EXAMPLE: INPUT(FILETYPE): SWF_L1A_1P  -> OUTPUT(KEYWORD): ORBVIEW_SEAWIFS_AR
 */
int ACS_ESAProductFileName_GetKeywordFromFiletype(
                    const char in_filetypeKeyword[11],      /* IN: FileType */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char* filetypeName ) ;                  /* OUT: File type name (key in private conf file) */



/*
 * DESCRIPTION: This function returns the Satellite Keyword given ACS name
 * and accessing static tables (configuration files).
 *
 */
int                                                         /* OUT: 0 success */
        ACS_ESAProductFileName_GetSatelliteKey(
                    const char* in_satelliteName,           /* IN: Satellite ACS name (key in private conf file) */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char satelliteKeyword[3]) ;             /* OUT: Satellite Key */


/*
 * DESCRIPTION: This function returns the Satellite ACS name given the Keyword,
 * accessing static tables (configuration files).
 *
 */
int                                                         /* OUT: 0 success */
        ACS_ESAProductFileName_GetSatelliteName(
                    const char in_satelliteKeyword[3],      /* IN: Satellite Key */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char* satelliteName ) ;                 /* OUT: Satellite ACS name (key in private conf file) */


/*
 * DESCRIPTION: This function checks a filename, returning 1 if filename is valid
 *
 */  
int  											/* OUT: 1 VALID, 0 NOT-VALID */
		ACS_ESAProductFileName_CheckFileNameIsValid( 	
			const char* inputFileName,			/* IN: File name */
			const char* basePath = NULL
		) ;				


/*
 * Return Product Dir. Name using Data_Transcription_Statistic as input
*/
int 																/* OUT: return value (0 = GOOD) */
        ACS_ESAProductFileName_GetNameFromStatistics
        (
            const Data_Transcription_Statistic* statRecord, 	/* IN: Statistics */
            int crc,											/* IN: CRC (or -1 if not available) */
            int structureIsBIGEND,								/* IN: SET TRUE if structure is BIGENDIAN */
            char* dirName								    	/* OUT: Wilma filename */
        );


/*
 * DESCRIPTION: Same as ACS_ESAProductFileName_GetNameFromStatistics, but CRC is evaluated by a given filename
*/
int 																/* OUT: return value (0 = GOOD) */
        ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC
        (
            const Data_Transcription_Statistic* statRecord, 	/* IN: Statistics */
            const char* DTHeaderToGetCrcFrom,           		/* IN: Optional: DTHeader file, or NULL to skip CRC */
            int structureIsBIGEND,								/* IN: SET TRUE if structure is BIGENDIAN */
            char* dirName								    	/* OUT: Wilma filename */
        );


/*
 * DESCRIPTION: This function returns the 3-chars StationID keyword, given Station Number (ACS_Stations_Table.conf)
 * 
 */
int                                             	/* OUT: 0 success, otherwise error */
    	ACS_ESAProductFileName_GetStationID(
                       const unsigned char in_key,  /* IN: ACS Station ID (1-byte unsigned number) */
                       const char* pathName,        /* IN: Optional Path Name */
                       char out_value[4]            /* OUT: 3-chars Station keyword */
		) ;


/*
 * DESCRIPTION: This function returns the StationNumber (ACS_Stations_Table.conf) given the 3-chars Station keyword
 * 
 */
int                                                /* OUT: 0 success, otherwise error */
		ACS_ESAProductFileName_GetStationNumber(
					const char in_value[4],              /* IN: 3-chars Station keyword */
					const char* pathName,          /* IN: Optional Path Name */
					unsigned char* out_key   /* OUT: ACS Station ID (1-byte unsigned number) */
		) ;


/*
 * Convert Data_Transcription_Statistic endianess 
*/
int 																/* OUT: return value (0 = GOOD) */
        ACS_ESAProductFileName_Endian_DTStatistic_Converter
        (
            const Data_Transcription_Statistic* statRecord, 	/* IN: Input Statistics */
            int structureIsBigendian,							/* IN: Set TRUE if given structure is Bigendian */
            Data_Transcription_Statistic* outStatRecord			/* OUT: Converted Statistic */
        );


double
        ACS_ESAProductFileName_DateToJD50(int year, int month, int day, int hour, int min, int sec, int msec ) ;
		 
void
        ACS_ESAProductFileName_JD50ToDate(const double, int* year, int* month, int* day, int* hour, int* min, int* sec, int* usec ) ;





#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !__LIBESAPRODFN_H__ */
