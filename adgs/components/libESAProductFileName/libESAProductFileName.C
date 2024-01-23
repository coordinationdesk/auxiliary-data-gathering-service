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

    $Prod: A.C.S. ESAProductFileName Library $

    $Id$

    $Author$

    $Log$
    Revision 1.29  2014/06/26 07:44:49  ricfer
    basePath added as optional input to ACS_ESAProductFileName_CheckFileNameIsValid.

    Revision 1.28  2014/03/25 12:48:33  marpas
    qa warnings fixing

    Revision 1.27  2013/11/19 14:02:32  ricfer
    Compiler warnings removed.

    Revision 1.26  2012/03/01 16:23:54  marpas
    using acs::base for default static paths

    Revision 1.25  2009/04/01 12:43:37  enrcar
    EC:: Debug improved

    Revision 1.24  2008/05/26 09:21:04  enrcar
    EC:: Comments heavily misleading -- fixed

    Revision 1.23  2008/02/01 10:39:06  marqua
    added debug information printout

    Revision 1.22  2008/01/14 11:22:23  enrcar
    Interface modified to compute CRC directly from user header file

    Revision 1.21  2007/03/05 17:54:22  enrcar
    Wrong stop-date when acq.day wraps-up. Handled.

    Revision 1.20  2007/02/21 10:30:08  giucas
    Improved messages

    Revision 1.19  2007/02/09 15:53:46  enrcar
    round-up improved

    Revision 1.18  2007/02/08 16:17:07  enrcar
    EC:: Bug in CEIL rounding when getting stop validity --Fixed

    Revision 1.17  2007/01/30 11:06:26  enrcar
    EC:: Offset used to fix a rounding bug

    Revision 1.16  2007/01/29 16:44:51  enrcar
    EC:: bug fixed in .h interface

    Revision 1.15  2007/01/26 12:12:22  aleber
    function ACS_ESAProductFileName_GetKeywordFromFiletype added that converts File Typevalue in File Type key

    Revision 1.14  2007/01/26 10:31:44  davcas
     Corrected bug on the getNameFromStatistic (milliseconds were missing)

    Revision 1.13  2007/01/22 16:36:01  enrcar
    EC:: Some comments were misleading -- fixed

    Revision 1.12  2007/01/19 16:21:31  davcas
    Corrected method getNameFromStatistics to use the Station_DT_ID instead of Station_ID in the product name

    Revision 1.11  2007/01/19 14:37:38  aleber
    method ACS_ESAProductFileName_GetFiletypeFromKeyword() added, that converts a internal key in a file type compliant to ESA standards

    Revision 1.10  2007/01/16 09:26:47  davcas
    Modified getNameFromStatistics to add channel for SPOT

    Revision 1.9  2006/12/01 11:07:32  enrcar
    ACS_ESAProductFileName_JD50ToDate : now handles usec instead of msec

    Revision 1.8  2006/11/14 10:28:18  enrcar
    SGI Compatibility: INT used instead of BOOL

    Revision 1.7  2006/11/13 18:05:29  enrcar
    GetFilenameFromStructure: added parameter to handle both bigendian and littleendian structures

    Revision 1.6  2006/11/13 17:46:33  enrcar
    rounding added when filename is evaluated

    Revision 1.5  2006/10/26 12:04:38  enrcar
    LINUX: Unable to compile under 3.2.x. Fixed.

    Revision 1.4  2006/10/25 09:23:26  enrcar
    SGI:: elDebugLevel Macro definition was missing.. fixed

    Revision 1.3  2006/10/17 13:29:54  enrcar
    linux C compiler still allowed

    Revision 1.2  2006/10/11 12:45:58  enrcar
    linux versioning added

    Revision 1.1  2006/10/11 12:11:04  enrcar
    RENAMED: Extension changed from .c to .C

    Revision 1.20  2006/10/03 15:52:01  enrcar
    Force to use V.1.5 removed -- now version 1.7 will be used
    ACS_ESAProductFileName_GetNameFromStatistics : CRC added to input arguments

    Revision 1.19  2006/09/29 09:19:41  enrcar
    SGI warning removed

    Revision 1.18  2006/09/29 08:57:13  enrcar
    Backward compatibily (ESA 1.5) bug fixed.

    Revision 1.17  2006/09/26 09:57:14  enrcar
    Updated to ESA Product File Name convention Rev. 1.7

    Revision 1.16  2006/09/14 12:17:22  enrcar
    ACS_ESAProductFileName_CheckFileNameIsValid: BUG FIXED, was unable to check misleading filenames

    Revision 1.15  2006/09/13 09:02:02  enrcar
    ACS_ESAProductFileName_GetSatelliteName method added

    Revision 1.14  2006/07/11 14:10:41  enrcar
    ACS_ESAProductFileName_GetSatelliteKey Method added

    Revision 1.13  2006/06/28 11:53:54  enrcar
    check on return value of every sscanf performed

    Revision 1.12  2006/06/27 13:52:41  enrcar
    unable to compile under sgi. fixed.

    Revision 1.11  2006/06/27 13:23:51  enrcar
    ACS_ESAProductFileName_GetParams, ACS_ESAProductFileName_CheckFileNameIsValid, ACS_ESAProductFileName_GetStationNumber, ACS_ESAProductFileNameJD50ToDate, rs_getKeyFromValue, rs_getSatelliteName methods added
    Method dateToJD50 renamed ACS_ESAProductFileName_DateToJD50 and made public

    Revision 1.10  2006/06/22 12:48:44  enrcar
    Product file key built using _AR instead of _WL

    Revision 1.9  2006/06/08 15:19:18  enrcar
    ACS_ESAProductFileName_Endian_DTStatistic_Converter Method added
    ACS_ESAProductFileName_GetNameFromStatistic always use BIGEND structure

    Revision 1.8  2006/06/08 11:56:18  aleber
    getStationId method made public and renamed ACS_ESAProductFileName_GetStationId

    Revision 1.7  2006/06/06 09:14:50  enrcar
    BUG FIXED: If present, CRC should be ALWAYS on 4 digits (left-padded with 0s)

    Revision 1.6  2006/05/24 14:29:15  enrcar
    toUpper method added
    ACS_ESAProductFileName_ToJD50 renamed as dateToJD50 and made static
    jd50_convert renamed as jd50toString
    ACS_ESAProductFileName_GetNameFromStatistics method added

    Revision 1.5  2006/05/19 16:30:49  davcas
    debug improved

    Revision 1.4  2006/05/18 13:12:50  enrcar
    SGI: Warning when compiling a long double. Fixed

    Revision 1.3  2006/05/18 12:50:56  enrcar
    method dateToJD50 added

    Revision 1.2  2006/05/17 15:56:32  aleber
    EC::
    Extension length was 4 (so vector size must be 5). FIXED.
    conf-file path was mistaken if empty string passed (only valid string or NULL pointer were handled)

    Revision 1.1.1.1  2006/05/17 09:22:06  enrcar
    imported source


*/


#include <esapfnV.h>
#include <RmLegacy.h>
#include <ErrorLegacy.h>
#include <libgen.h>
#include <math.h>
#include <crcmodel.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <acsSatellites.h> 
#pragma GCC diagnostic pop
#include <ESAProductFileName.h>
#include <ACS_ByteSwap.h>
#include <acs_c++config.hpp>

static string ACS_ESAProductFileName_DEFAULT_PATH = string(acs::base::defaultConfPath()) + "/local/" ;


#if defined(sgi) || !defined(__cplusplus)
    elDEBUG_LEVEL
#else
namespace {
	/* Linux versioning */
	esapfnV version ;
}
#endif


/* static methods, outside .h */

static double stringToJD50( const char* in_string, double* jd50) ;
static void jd50toString(const double r, char* out_string, int);
static int LRtrim(const char* in_string, char* out_string) ;
static int rs_getFiletype(const char*, const char*, char*) ;
static int rs_getFiletypeName(const char*, const char*, char*) ;
static int rs_getSatelliteID(const char*, const char*, char*) ;
static int rs_getSatelliteName(const char* in_value,const char* pathName, char* out_key);
static int rs_getValueFromKey(const char*, const char*, char*) ;
static char* toUpper(const char* in_str, char* out_str) ;
static int rs_getKeyFromValue(const char* in_value, const char* inResFile, char* out_key);
	

/*
 * DESCRIPTION: This function returns the fileType given the keyword
 * and accessing static tables (configuration files).
 *
 */
int                                                  /* OUT: 0 success */
        ACS_ESAProductFileName_GetFiletypeFromKeyword(
                    const char* in_ftKeyword,        /* IN: fileType Keyword (key in private conf file) */
                    const char* basePath,            /* IN: Optional base path for conf files */
                    char fileType[11])               /* OUT: filetype */ 
{
    char msg[1024] ;
    int status ;

    strcpy(fileType, "");
  
  
    elBGN_DEBUG(10)
	{
		sprintf(msg,"in_ftKeyword !%s!", in_ftKeyword);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
    }
 	elEND_DEBUG
  
    /* Get 10-chars fileType */
    if ( (status = rs_getFiletype(in_ftKeyword, basePath, fileType)) )
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetFiletypeFromKeyword. Error in rs_getfileType (%d) using keyword %s", status, in_ftKeyword);
        elNotifyMessage(msg) ;
        return status ;
    }
 
	return 0 ;
}  

/*
 * DESCRIPTION: This function returns the FileType (i.e. the Keyword in ACS Filetypes conf-file)
 * given the 10-chars (+ null) Filetype taken from a valid ESA Filename
 *
 */
int                                                        /* OUT: 0 success */
        ACS_ESAProductFileName_GetKeywordFromFiletype(
                    const char IN_filetype_key[11],        /* IN: File Type (value in private conf file)*/
                    const char* basePath,                  /* IN: Optional base path for conf files */
                    char* filetypeName )                   /* OUT: File Type name (key in private conf file) */
{
    int status ;

    if ( (status = rs_getFiletypeName( IN_filetype_key, basePath, filetypeName )) )
    {
		/* Error executing method rs_getFiletypeName */
        char msg[1024] ;
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetKeywordFromFiletype. Filetype Value not found: %s", IN_filetype_key);
        elNotifyMessage(msg) ;
    }
    
    return status ;
}
  
/*
 * DESCRIPTION: This function returns the Satellite Keyword given ACS name
 * and accessing static tables (configuration files).
 *
 */
int                                                         /* OUT: 0 success */
        ACS_ESAProductFileName_GetSatelliteKey(
                    const char* in_satelliteName,           /* IN: Satellite ACS name (key in private conf file) */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char satelliteKeyword[3])                /* OUT: file name */ 
{
    char msg[1024] ;
    int status ;


    strcpy(satelliteKeyword, "");
  
  
    elBGN_DEBUG(10)
	{
		sprintf(msg,"in_satelliteName !%s!", in_satelliteName);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
    }
 	elEND_DEBUG
  
    /* Get 2-chars Satellite ID */
    if ( (status = rs_getSatelliteID(in_satelliteName, basePath, satelliteKeyword)) )
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Error in rs_getSatelliteID (%d)", status);
        elNotifyMessage(msg) ;
        return status ;
    }
 
	return 0 ;
}  


/*
 * DESCRIPTION: This function returns the output filename, obtained merging given input-parameters 
 * and accessing static tables (configuration files).
 *
 */
int                                                         /* OUT: 0 success */
        ACS_ESAProductFileName_GetName(
                    const char* in_satelliteName,           /* IN: Satellite ACS name (key in private conf file) */
                    const unsigned char missionNum,         /* IN: Mission number */
                    const char* in_FileType,                /* IN: ACS code for the filetype (key in private conf file) */
                    const double validityStartTime,         /* IN: Validity Start (JD50) */
                    const double validityStopTime,          /* IN: Validity Stop (JD50) */
                    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from ACS_Stations_Table.conf) */
                    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
                    const int safeManifestCrc,              /* IN: Optional: CRC , -1 if not used */
                    const char fileExtension[5],            /* IN: File extension */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char outputFileName[70])                /* OUT: file name */ 
{
    char satelliteName[3], fileType[11], stationID[4], timeStamp[16] ; 
    char msg[1024], tmp[1024] ;
    int status ;
  
	elINIT_DEBUG(ACS_ESAProductFileName_DEBUG) ;

    strcpy(outputFileName, ""); /* So, strcat can be used */
  
  
    elBGN_DEBUG(10)
	{
		sprintf(msg,"in_satelliteName !%s!", in_satelliteName);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"missionNum %d", static_cast<int>(missionNum));
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"in_FileType !%s!", in_FileType);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"validityStartTime %f", validityStartTime);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"validityStopTime %f", validityStopTime);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"originatingFacility %d", static_cast<int>(originatingFacility));
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"absoluteOrbitNum %d", absoluteOrbitNum);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"safeManifestCrc %d", safeManifestCrc);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"fileExtension !%s!", fileExtension);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
		sprintf(msg,"basePath !%s!", basePath);
		elNotifyMessage(msg) ; if (0) printf("%s\n", msg);
    }
 	elEND_DEBUG
  
    /* Get 2-chars Satellite ID */
    if ( (status = rs_getSatelliteID(in_satelliteName, basePath, satelliteName)) )
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Error in rs_getSatelliteID (%d)", status);
        elNotifyMessage(msg) ;
        return status ;
    }
    
    /* Check Satellite ID */
    if (strlen(satelliteName) != 2)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid satelliteName size (%s)", satelliteName );
        elNotifyMessage(msg) ;
        return -1 ;
    }
          
    strcat(outputFileName, satelliteName);             

    
    /* Check Mission Number */
    if (missionNum >= 100)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid missionNum %d", static_cast<int>(missionNum) );
        elNotifyMessage(msg) ;
        return -1 ;
    }
          
    sprintf(tmp, "%02d", static_cast<int>(missionNum) );   
    strcat(outputFileName, tmp);             

    /* Get 10-chars FileType */
    if ( (status = rs_getFiletype(in_FileType, basePath, fileType)) )
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Error in rs_getFiletype (%d)", status);
        elNotifyMessage(msg) ;
        return status ;
    }
              
    
    /* Check FileType */
    if (strlen(fileType) != 10)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid fileType size (%s) key: %s", fileType, in_FileType);
        elNotifyMessage(msg) ;
        return -1 ;
    }
          
    sprintf(tmp, "_%s", fileType );  
    strcat(outputFileName, tmp);             
           
           
    /* Get 10-chars Validity Start */
    jd50toString( validityStartTime,  timeStamp, 0 );	/* FLOOR Rounding */
                 
    /* Check Validity Start */
    if (strlen(timeStamp) != 15)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid validityStartTime size (%s) input: %f", timeStamp, validityStartTime );
        elNotifyMessage(msg) ;
        return -1 ;
    }
          
    sprintf(tmp, "_%s", timeStamp );  
    strcat(outputFileName, tmp);             
      
           
    /* Get 10-chars Validity Stop */
    jd50toString( validityStopTime,  timeStamp, 1 );	/* CEIL Rounding */
                 
    /* Check Validity Stop */
    if (strlen(timeStamp) != 15)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid validityStopTime size (%s) input %f", timeStamp, validityStopTime );
        elNotifyMessage(msg) ;
        return -1 ;
    }
          
    sprintf(tmp, "_%s", timeStamp );  
    strcat(outputFileName, tmp);             
      
           
    /* Get 3-chars Station ID */
    if ( (status = ACS_ESAProductFileName_GetStationID(originatingFacility, basePath, stationID)) )
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Error in rs_getFileName (%d)", status);
        elNotifyMessage(msg) ;
        return status ;
    }
                   
    /* Check Station ID */
    if (strlen(stationID) != 3)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid stationID size (%s) input: %d", stationID, originatingFacility);
        elNotifyMessage(msg) ;
        return -1 ;
    }

    sprintf(tmp, "_%s", stationID );  
    strcat(outputFileName, tmp);             


    /* Check Orbit Number */
    if (absoluteOrbitNum < 0 || absoluteOrbitNum > 999999)
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid absoluteOrbitNum %d", absoluteOrbitNum);
        elNotifyMessage(msg) ;
        return -1 ;
    }
          
    sprintf(tmp, "_%d", absoluteOrbitNum );   
    strcat(outputFileName, tmp);             


    /* Check CRC */
    if ( safeManifestCrc >= 0 && safeManifestCrc <= 0xffff )
        sprintf(tmp, "_%04X", safeManifestCrc );           
    else
        sprintf(tmp, "_%04X", 0 );  /* V. 1.7 or following - If CRC is not used (i.e. == -1), 0000 will be placed anyway */

    strcat(outputFileName, tmp);             

    /* Check File Extension */
/*
    A file with NO EXTENSION is currently not allowed.
    To allow, replace following line:
        if (strlen(fileExtension) < 1 || strlen(fileExtension) > 4 ) 
    with:
        if ( strlen(fileExtension) > 4 ) 
*/

    if (strlen(fileExtension) < 1 || strlen(fileExtension) > 4 ) 
    {
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Invalid fileExtension size (%s)", fileExtension );
        elNotifyMessage(msg) ;
        return -1 ;
    }

    sprintf(tmp, ".%s", fileExtension );   
    strcat(outputFileName, tmp);             


    elBGN_DEBUG(10)
            sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName. Created prodname: !%s!", outputFileName );
            elNotifyMessage(msg) ;
    elEND_DEBUG


    return 0 ;
}  
  
  
/*
 * DESCRIPTION: Same as ACS_ESAProductFileName_GetName, but CRC is evaluated by a given filename
 *
 */
int                                                         /* OUT: 0 success */
        ACS_ESAProductFileName_GetName_ComputeCRC(
                    const char* in_satelliteName,           /* IN: Satellite ACS name (key in private conf file) */
                    const unsigned char missionNum,         /* IN: Mission number */
                    const char* in_FileType,                /* IN: ACS code for the filetype (key in private conf file) */
                    const double validityStartTime,         /* IN: Validity Start (JD50) */
                    const double validityStopTime,          /* IN: Validity Stop (JD50) */
                    const unsigned char originatingFacility,/* IN: Originating Facility ACS code (from ACS_Stations_Table.conf) */
                    const unsigned int absoluteOrbitNum,    /* IN: Absolute Orbit Number */
                    const char* fileToGetCrcFrom,           /* IN: Optional: filename, or NULL to skip CRC */
                    signed long long int expSize,           /* IN: Optional: file size (will be checked), or -1 to skip check */
                    const char fileExtension[5],            /* IN: File extension */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char outputFileName[70])                /* OUT: file name */ 
{
    crcmodel* p_cm ;
    int crc, status ;
    char msg[1024] ;
  
	elINIT_DEBUG(ACS_ESAProductFileName_DEBUG) ;

	crc = -1 ;	/* Default value, means no CRC */
    
    if ( fileToGetCrcFrom && (*fileToGetCrcFrom) )
    {
		/* Since filename is provided, CRC will be evaluated */

        p_cm = crcmodel_construct();	/* Allocate a new CRC Structure */
		
		/* Setup the CRC using the default parameters */
        crc_setup( p_cm, ACS_ESACRC_TB_WIDTH, ACS_ESACRC_TB_POLY, ACS_ESACRC_INIT, ACS_ESACRC_TB_REFIN, ACS_ESACRC_TB_REFOT, ACS_ESACRC_XOROT );

        cm_ini(p_cm); 	/* Setup with starting value */   
        status = crcmodel_filedecode( p_cm, fileToGetCrcFrom, expSize );	/* Decode the file */
        if (status)
        {
			/* Error evaluating CRC */
            sprintf(msg, "ACS_ESAProductFileName_GetName_ComputeCRC  Error evaluating CRC. Filename: %s  Expected Size: %Ld [%d]", fileToGetCrcFrom, expSize, status );
            elNotifyMessage(msg);
            crcmodel_destruct(p_cm);	/* Deallocate CRC structure */
            return -1 ;
        }

        crc = static_cast<int>(cm_crc(p_cm)) ;	/* Get CRC */
        crcmodel_destruct(p_cm);	/* Deallocate CRC structure */

        elBGN_DEBUG(50)
        {
            sprintf(msg,"ACS_ESAProductFileName_GetName_ComputeCRC. CRC is: 0x%x", crc);
            elNotifyMessage(msg) ;
        }
        elEND_DEBUG

	}
    else
    {
		/* No file / invalid file provided */
        elBGN_DEBUG(50)
        {
            sprintf(msg,"ACS_ESAProductFileName_GetName_ComputeCRC. CRC not evaluated.");
            elNotifyMessage(msg) ;
        }
        elEND_DEBUG
    }
    

    status = ACS_ESAProductFileName_GetName(in_satelliteName, 
                                            missionNum,      
                                            in_FileType,      
                                            validityStartTime,    
                                            validityStopTime,     
                                            originatingFacility,
                                            absoluteOrbitNum, 
                                            crc,
                                            fileExtension,
                                            basePath,         
                                            outputFileName ) ;

	if (!status)
	{
        elBGN_DEBUG(10)
                sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName_ComputeCRC. Created prodname: !%s!", outputFileName );
                elNotifyMessage(msg) ;
        elEND_DEBUG
	}
    else
    {
		/* Error running ACS_ESAProductFileName_GetName */
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetName_ComputeCRC. ERROR RUNNING METHOD ACS_ESAProductFileName_GetName [%d]", status );
        elNotifyMessage(msg) ;
    }


    return status ;
}  
  
  
/*
 * DESCRIPTION: This function checks a filename, returning 1 if filename is valid
 *
 */  
int  											/* OUT: 1 VALID, 0 NOT-VALID */
		ACS_ESAProductFileName_CheckFileNameIsValid( 	
			const char* inputFileName,			/* IN: File name */
			const char* basePath				/* The path where are stored the config files */
		)				
{
    char satelliteName[1024], fileType[1024], fileExtension[5] ;
    unsigned char missionNum, originatingFacility ;
    unsigned int absoluteOrbitNum ;
    int status, safeManifestCrc ;
    double validityStartTime, validityStopTime ;
    
    
	status = ACS_ESAProductFileName_GetParams( inputFileName, basePath, satelliteName, &missionNum, fileType, &validityStartTime, &validityStopTime, &originatingFacility, &absoluteOrbitNum, &safeManifestCrc, fileExtension ) ;

	return (!status)? 1 : 0 ;
}

  
/*
 * DESCRIPTION: This function returns the output filename, obtained merging given input-parameters 
 * and accessing static tables (configuration files).
 *
 */
int                                                     /* OUT: 0 success */
        ACS_ESAProductFileName_GetParams(
                  	const char* inputFileName,			/* IN: File name */
                    const char* basePath,               /* IN: Optional base path for conf files */
				    char* satelliteName,           		/* OUT: Satellite ACS name (key in private conf file) */
                    unsigned char* missionNum,         	/* OUT: Mission number */
                    char* fileType,                	    /* OUT: ACS code for the filetype (key in private conf file) */
                    double* validityStartTime,         	/* OUT: Validity Start (JD50) */
                    double* validityStopTime,          	/* OUT: Validity Stop (JD50) */
                    unsigned char* originatingFacility,	/* OUT: Originating Facility ACS code (from ACS_Stations_Table.conf) */
                    unsigned int* absoluteOrbitNum,    	/* OUT: Absolute Orbit Number */
                    int* safeManifestCrc,              	/* OUT: Optional: CRC , -1 if not used */
                    char fileExtension[5])            	/* OUT: File extension */

{
	int fileName_len ;
	char s[128], msg[1024], blank1[1024], blank2[1024];
	unsigned int orbNum, index ;

/* LINUX: precision appears to be 10^-8. A +-(10^-3) offset will be added */
	double roundingTime = 0.0000000001157 ; /* (1ms == 1/1000 s) expressed ad JD50 -> (1/1000)/86400 */


	fileName_len = strlen(inputFileName) ;  
      
	
	/* Get 2-chars Satellite ID */
	if ( fileName_len<2 )
  	{
		/* Invalid filename */
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract Satellite_ID");
		elNotifyMessage(msg) ; return -1 ;
	}
  	else if (satelliteName)
	{
		char sat_key[3] ;
		int status ;
		
		memset(sat_key, 0, sizeof(sat_key));
		if ( sscanf(inputFileName, "%2s%s", sat_key, blank1) != 2)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract Satellite_ID");
			elNotifyMessage(msg) ; return -2 ;
		}
		
		status = rs_getSatelliteName( sat_key, basePath, satelliteName );	/* Fill satelliteName */
        
		if (status)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Satellite_ID Key not found: %s", sat_key);
			elNotifyMessage(msg) ; return status ;
		}
	}

  
	/* Get Mission Number */
	if ( fileName_len<4 )
  	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract Mission Number");
		elNotifyMessage(msg) ; return -1 ;
	}
  	else if (missionNum)
	{
		unsigned int i ;
		
		if ( sscanf( inputFileName, "%2s%02d%s", blank1, &i, blank2 ) != 3)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract Mission Number");
			elNotifyMessage(msg) ; return -2 ;
		}

		*missionNum = static_cast<unsigned char>(i);
	}

  
	/* Get 10-chars FileType */
	if ( fileName_len<15 )
	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract FileType");
		elNotifyMessage(msg) ; return -1 ;
	}
  	else if (fileType)
	{
		memset(fileType, 0, 11) ;
		if ( sscanf(inputFileName, "%5s%10s%s", blank1, fileType, blank2) != 3)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract FileType");
			elNotifyMessage(msg) ; return -2 ;
		}
	}
  
  
	/* Get 15-chars Validity Start */
	if ( fileName_len<31 )
	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract Validity Start");
		elNotifyMessage(msg) ; return -1 ;
	}
  	else if (validityStartTime)
	{
		memset(s, 0, sizeof(s)) ;
		if ( sscanf(inputFileName, "%16s%15s%s", blank1, s, blank2) != 3)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract Validity Start");
			elNotifyMessage(msg) ; return -2 ;
		}
		
        stringToJD50(s, validityStartTime);
        
        (*validityStartTime) += roundingTime ;	/* Since StartTime could be used to perform a FLOOR: If "double" rounding is less than the second (e.g. 1233.9999 instead of 1234.0, a wrong value will result!) */
	}
  
  
 	/* Get 15-chars Validity Stop */
	if ( fileName_len<47 )
	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract Validity Stop");
		elNotifyMessage(msg) ; return -1 ;
	}
  	else if (validityStopTime)
	{
		memset(s, 0, sizeof(s)) ;
		if ( sscanf( inputFileName, "%32s%15s%s", blank1, s, blank2 ) != 3)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract validityStopTime");
			elNotifyMessage(msg) ; return -2 ;
		}
		
        stringToJD50(s, validityStopTime);
        
        (*validityStopTime) -= roundingTime ; /* Since StopTime could be used to perform a CEIL: If "double" rounding is more than the second (e.g. 1234.0001 instead of 1234.0, a wrong value will result!) */
	}


 	/* Get 3-chars StationID */
	if ( fileName_len<51 )
	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract StationID");
		elNotifyMessage(msg) ; return -1 ;
	}
  	else if (originatingFacility)
	{
		char sat_key[4] ;
		int status ;

		memset(sat_key, 0, sizeof(sat_key));
		if ( sscanf( inputFileName, "%48s%3s%s", blank1, sat_key, blank2 ) != 3)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract originatingFacility");
			elNotifyMessage(msg) ; return -2 ;
		}

		status = ACS_ESAProductFileName_GetStationNumber(sat_key, basePath, originatingFacility) ;
		
		if (status)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. originatingFacility Key not value: %s", sat_key);
			elNotifyMessage(msg) ; return status ;
		}
	}


 	/* Get Orbit number */
	if ( fileName_len<53 )
	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract StationID");
		elNotifyMessage(msg) ; return -1 ;
	}
	
	if ( sscanf( inputFileName, "%52s%d%s", blank1, &orbNum, blank2 ) != 3)
	{
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract orbNum");
		elNotifyMessage(msg) ; return -2 ;
	}

	if (absoluteOrbitNum)
	{
		*absoluteOrbitNum = orbNum ;
	}
	
	if (orbNum > 999999)
	{
		/* Invalid value */
		sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. absoluteOrbitNum overflow");
		elNotifyMessage(msg) ; return -1 ;
	}
	
	/* Reset values, could be filled below */
	if (safeManifestCrc) { (*safeManifestCrc) = -1 ; }
	if (fileExtension) memset(fileExtension, 0, 5) ;
	
	/* Check if file has CRC and/or extension */
	index = orbNum? (53 + static_cast<unsigned int>(log10(static_cast<double>(orbNum)))) : 53 ;
	
	if ( static_cast<int>(index) == fileName_len )
	{
		/* No CRC and no extension. Finished */
		return 0 ;
	}

	if ( inputFileName[index] == '_' )
	{
		/* CRC provided */
		
		if (fileName_len < (static_cast<int>(index)+4))
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Cannot extract CRC");
			elNotifyMessage(msg) ; return -1 ;
		}

		if (safeManifestCrc)
        {
			if ( sscanf(inputFileName+index+1, "%04X%s", safeManifestCrc, blank1) != 2)
			{
				sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract CRC");
				elNotifyMessage(msg) ; return -2 ;
			}


#ifndef ACS_LIBESA_PRODUCTFILENAME_FORCE_1_5
            /* Updated to revision 1.7. If 0 is passed as CRC, no CRC has to be evaluated. */
            if ( (*safeManifestCrc) == 0 )
                (*safeManifestCrc) = -1 ;  /* Rev. 1.7 :  No-CRC IS FORCED */
            ;
#endif

        }
		index += 5 ;
	}
	
    
	if ( static_cast<int>(index) == fileName_len || static_cast<int>(index)+1 == fileName_len )
	{
		/* No extension (or just "." - Finished) */
		return 0 ;
	}
	

	if (fileExtension)
    {
        char* cp = (const_cast<char*>(inputFileName))+index ;    /* Pointer to ".EXTN" */
        
        
        if ( (*cp) != '.' )
        {
			/* Invalid extension provided */
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Invalid filename format");
			elNotifyMessage(msg) ; return -2 ;
		}
        
        /* Dot is removed from extension. */
		if ( sscanf(cp+1, "%s", fileExtension) != 1)
		{
			sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetParams. Unable to extract fileExtension");
			elNotifyMessage(msg) ; return -2 ;
		}
    }
  
  	return 0 ;
}
  
  
/*
 * DESCRIPTION: This function returns the Satellite ACS name given the Keyword,
 * accessing static tables (configuration files).
 *
 */
int                                                         /* OUT: 0 success */
        ACS_ESAProductFileName_GetSatelliteName(
                    const char IN_sat_key[3],                  /* IN: Satellite Key */
                    const char* basePath,                   /* IN: Optional base path for conf files */
                    char* satelliteName )                /* OUT: Satellite ACS name (key in private conf file) */
{
    int status ;

    status = rs_getSatelliteName( IN_sat_key, basePath, satelliteName );

    if (status)
    {
		/* Satellite key not found */
        char msg[1024] ;
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetSatelliteName. Satellite_ID Key not found: %s", IN_sat_key);
        elNotifyMessage(msg) ;
    }
    
    return status ;
}
  
  
/*
 *  DESCRIPTION: This function returns the Full WILMA Filename given a valid Statistic structure
 * 
 *  NOTE: Structure can be BOTH LITTLE ENDIAN AND BIGENDIAN
 *	
 *  dirName MUST BE ALLOCATED BEFORE CALLING (i.e. buffer[70])	
 *
 */
int 															/* OUT: return value (0 = GOOD) */
	ACS_ESAProductFileName_GetNameFromStatistics(
                const Data_Transcription_Statistic* statRecordIN, /* IN: Statistics */
                int crc,										  /* IN: CRC, or -1 if not provided */
            	int isBigEnd,									  /* IN: set if given structure is BIGEND */
			    char* dirName								      /* OUT: Wilma filename */
            )
{

    char fileTypeKey[128], tmp[1024] ;
    Data_Transcription_Statistic statRecord ;
	double startDate, stopDate ;


	/* Convert Data_Transcription_Statistic ENDIANESS */
	if ( ACS_ESAProductFileName_Endian_DTStatistic_Converter ( statRecordIN, isBigEnd, &statRecord ) )
	{
		sprintf(tmp, "ESAProductFileName::ACS_ESAProductFileName_GetNameFromStatistics. Error running ACS_ESAProductFileName_Endian_DTStatistic_Converter");
		elNotifyMessage(tmp) ;
		return -1 ;
	}
	
	
	/* Build FileType Key (effective filetype will be taken from conf. file */
	/*
		"SAT_NAME" "_" "INSTRUMENT_NAME" "_AR"

		Both 
		Examples:
			LANDSAT_ETM_AR
			EOSAM_MODIS_AR
			...
		IF SPOT, uses also Transmission_Channel
			SPOT_HRV_CH1_AR, SPOT_HRV_CH2_AR
			SPOT_HRVI_CH1_AR, SPOT_HRVI_CH2_AR	
			
	*/
	strcpy( fileTypeKey, toUpper(Satellite_Name[statRecord.Satellite_ID-1], tmp) );
	strcat( fileTypeKey, "_") ;
	strcat( fileTypeKey, toUpper(Sensor_Name[statRecord.Instrument_Type_ID], tmp) );
	if(statRecord.Satellite_ID == SPOT)
	{
		sprintf(tmp, "_CH%1.1d", statRecord.Transmission_Channel);
		strcat( fileTypeKey,tmp);
	}
	strcat( fileTypeKey, "_AR") ;

    startDate = ACS_ESAProductFileName_DateToJD50(
                    statRecord.Acquisition_Date[0], 
                    statRecord.Acquisition_Date[1], 
                    statRecord.Acquisition_Date[2],
                    statRecord.Acquisition_Start[0],
                    statRecord.Acquisition_Start[1],
                    statRecord.Acquisition_Start[2],
                    statRecord.Acquisition_Start[3]) ;			 

    stopDate = ACS_ESAProductFileName_DateToJD50(
                    statRecord.Acquisition_Date[0], 
                    statRecord.Acquisition_Date[1], 
                    statRecord.Acquisition_Date[2],
                    statRecord.Acquisition_End[0],
                    statRecord.Acquisition_End[1],
                    statRecord.Acquisition_End[2],
                    statRecord.Acquisition_End[3]) ;			                   
    

    if (statRecord.Acquisition_End[0]<statRecord.Acquisition_Start[0])
    {
    	/* 
            Acquisition_End_Hour .LT. Acquisition_Start_Hour -> A Mistake or a midnigth wrap-up: 
        */

		if (statRecord.Acquisition_Start[0]>=23)
        {
        	/* Start time is >= 23 -> Wrap-Up is assumed: */
            stopDate += 1.0 ;
        }
		else
        {
        	/* Mistake */
            sprintf(tmp, "ESAProductFileName::ACS_ESAProductFileName_GetNameFromStatistics. Stop date is invalid");
            elNotifyMessage(tmp) ;
            return -1 ;
        }    
    }


	/* Create filename */
	return ACS_ESAProductFileName_GetName(
                toUpper(Satellite_Name[statRecord.Satellite_ID-1], tmp),	/* Satellite Name */
                static_cast<unsigned char>(statRecord.Mission_ID),         /* IN: Mission number */
                fileTypeKey,            /* IN: ACS code for the filetype (key in private conf file) */
                startDate,				/* IN: Validity Start (JD50) */ 
                stopDate,				/* IN: Validity Stop (JD50) */           
                static_cast<unsigned char>(statRecord.Station_DT_ID),	/* IN: Originating Facility ACS code (from ACS_Stations_Table.conf) */
                static_cast<signed int>(statRecord.Orbit_Number),    	/* IN: Absolute Orbit Number */
                crc,       				/* IN: Optional: CRC , -1 if not used */
                "WILM",            		/* IN: File extension */
                0,                   	/* IN: Optional base path for conf files */
                dirName);				/* OUT: file name */ 

}
 
    
/*
 * DESCRIPTION: Same as ACS_ESAProductFileName_GetNameFromStatistics, but CRC is evaluated by a given filename
*/
    int 															/* OUT: return value (0 = GOOD) */
		ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC(
                const Data_Transcription_Statistic* statRecordIN,	/* IN: Statistics */
                const char* dtHeaderToGetCrcFrom,           		/* IN: Optional: DTHeader file, or NULL to skip CRC */
                int isBigEnd,									    /* IN: set if given structure is BIGEND */
                char* dirName )							        /* OUT: Wilma filename */
{
    crcmodel* p_cm ;
    int crc, status ;
    char msg[1024] ;
  	const signed long long expSize = 876 ;	/* Since the file is ALWAYS DTHeader */
  
	elINIT_DEBUG(ACS_ESAProductFileName_DEBUG) ;

	crc = -1 ;
    
    if ( dtHeaderToGetCrcFrom && (*dtHeaderToGetCrcFrom) )
    {
		/* Since filename is provided, CRC will be evaluated */

        p_cm = crcmodel_construct();	/* Allocate CRC Structure */
        crc_setup( p_cm, ACS_ESACRC_TB_WIDTH, ACS_ESACRC_TB_POLY, ACS_ESACRC_INIT, ACS_ESACRC_TB_REFIN, ACS_ESACRC_TB_REFOT, ACS_ESACRC_XOROT );

        cm_ini(p_cm);	/* Set the startup value */
        if ( (status = crcmodel_filedecode( p_cm, dtHeaderToGetCrcFrom, expSize) ) )
        {
            sprintf(msg, "ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC  Error evaluating CRC. Filename: %s  Expected Size: %Ld", dtHeaderToGetCrcFrom, expSize );
            elNotifyMessage(msg);
            crcmodel_destruct(p_cm);	/* Dellocate CRC Structure */
            return -1 ;
        }

        crc = static_cast<int>(cm_crc(p_cm)) ;	/* Get CRC Value */
        crcmodel_destruct(p_cm);	/* Dellocate CRC Structure */

        elBGN_DEBUG(50)
        {
            sprintf(msg,"ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC. CRC is: 0x%x", crc);
            elNotifyMessage(msg) ;
        }
        elEND_DEBUG

	}
    else
    {
        elBGN_DEBUG(50)
        {
            sprintf(msg,"ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC. CRC not evaluated.");
            elNotifyMessage(msg) ;
        }
        elEND_DEBUG
    }
    

	status = ACS_ESAProductFileName_GetNameFromStatistics (
                    statRecordIN,
                    crc,
                    isBigEnd,
                    dirName);

	if (!status)
	{
        elBGN_DEBUG(10)
                sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC. Created prodname: !%s!", dirName );
                elNotifyMessage(msg) ;
        elEND_DEBUG
	}
    else
    {
		/* Error executing method ACS_ESAProductFileName_GetNameFromStatistics */
        sprintf(msg, "ESAProductFileName::ACS_ESAProductFileName_GetNameFromStatistics_ComputeCRC. ERROR RUNNING METHOD ACS_ESAProductFileName_GetNameFromStatistics [%d]", status );
        elNotifyMessage(msg) ;
    }


    return status ;
}
   
    
/*
 * DESCRIPTION: This function returns the File Type key (10-char) given file type value
 * 
 */
int                                             /* OUT: 0 success, otherwise error */
    rs_getFiletypeName(
                           const char* in_value,  /* IN: file type (10-char value in conf file) */
                           const char* pathName,  /* IN: Optional Path Name */
                           char* out_key          /* OUT: File type key  */
                        )
{
    char resFile[1024] ;

    
    if (!pathName || !pathName[0])
        strcpy(resFile, ACS_ESAProductFileName_DEFAULT_PATH.c_str());	/* Path was not provided */
    else
    	strcpy(resFile, pathName);								/* Path provided */

    strcat(resFile,"/"); strcat(resFile, "ACS_Filetypes_Table.conf");

    return rs_getKeyFromValue(in_value, resFile, out_key) ;
}

/*
 * DESCRIPTION: This function returns the 10-chars FileType keyword, given a Unique ID
 * 
 */
int                                             /* OUT: 0 success, otherwise error */
    rs_getFiletype(
                       const char* in_key,  /* IN: File Type key */
                       const char* pathName,/* IN: Optional Path Name */
                       char out_value[11]   /* OUT: file type (10-char value in conf file) */
                    )
{
    char resFile[1024] ;

    
    if (!pathName || !pathName[0])
        strcpy(resFile, ACS_ESAProductFileName_DEFAULT_PATH.c_str());	/* Path was not provided */
    else
    	strcpy(resFile, pathName);								/* Path provided */

    strcat(resFile,"/"); strcat(resFile, "ACS_Filetypes_Table.conf");

    return rs_getValueFromKey(in_key, resFile, out_value) ;
}


/*
 * DESCRIPTION: This function returns the 2-chars Satellite keyword, given Satellite Name
 * 
 */
int                                             /* OUT: 0 success, otherwise error */
    rs_getSatelliteID(
                           const char* in_key,  /* IN: Satellite Name (conventional) */
                           const char* pathName,/* IN: Optional Path Name */
                           char out_value[3]    /* OUT: 2-chars Satellite keyword */
                        )
{
    char resFile[1024] ;

    
    if (!pathName || !pathName[0])
        strcpy(resFile, ACS_ESAProductFileName_DEFAULT_PATH.c_str());	/* Path was not provided */
    else
    	strcpy(resFile, pathName);								/* Path provided */

    strcat(resFile,"/"); strcat(resFile, "ACS_Satellites_Table.conf");

    return rs_getValueFromKey(in_key, resFile, out_value) ;
}


/*
 * DESCRIPTION: This function returns the Satellite Name given 2-chars Satellite keyword
 * 
 */
int                                             /* OUT: 0 success, otherwise error */
    rs_getSatelliteName(
                           const char* in_value,  /* IN: 2-chars Satellite keyword */
                           const char* pathName,  /* IN: Optional Path Name */
                           char* out_key          /* OUT: Satellite Name (conventional)  */
                        )
{
    char resFile[1024] ;

    
    if (!pathName || !pathName[0])
        strcpy(resFile, ACS_ESAProductFileName_DEFAULT_PATH.c_str());	/* Path was not provided */
    else
    	strcpy(resFile, pathName);								/* Path provided */

    strcat(resFile,"/"); strcat(resFile, "ACS_Satellites_Table.conf");

    return rs_getKeyFromValue(in_value, resFile, out_key) ;
}


/*
 * DESCRIPTION: This function returns the 3-chars StationID keyword, given Station Number (ACS_Stations_Table.conf)
 * 
 */
int                                             /* OUT: 0 success, otherwise error */
    ACS_ESAProductFileName_GetStationID(
                       const unsigned char in_key,  /* IN: ACS Station ID (1-byte unsigned number) */
                       const char* pathName,        /* IN: Optional Path Name */
                       char out_value[4]            /* OUT: 3-chars Station keyword */
                    )
{
    char resFile[1024] ;
    char key[16] ;
 
    
    if (!pathName || !pathName[0])
        strcpy(resFile, ACS_ESAProductFileName_DEFAULT_PATH.c_str());	/* Path was not provided */
    else
    	strcpy(resFile, pathName);								/* Path was provided */


    strcat(resFile,"/"); strcat(resFile, "ACS_Stations_Table.conf");

    sprintf(key, "%d", static_cast<unsigned int>(in_key)); 

    return rs_getValueFromKey(key, resFile, out_value) ;
}


/*
 * DESCRIPTION: This function returns the StationNumber (ACS_Stations_Table.conf) given the 3-chars Station keyword
 * 
 */
int                                                /* OUT: 0 success, otherwise error */
    ACS_ESAProductFileName_GetStationNumber(
					const char in_value[4],        /* IN: 3-chars Station keyword */
					const char* pathName,          /* IN: Optional Path Name */
					unsigned char* st_Num          /* OUT: ACS Station ID (1-byte unsigned number) */
	)
{
    char resFile[1024], out_key[128] ;
 	int status ;
 
    
    if (!pathName || !pathName[0])
        strcpy(resFile, ACS_ESAProductFileName_DEFAULT_PATH.c_str());	/* Path was not provided */
    else
    	strcpy(resFile, pathName);								/* Path was provided */


    strcat(resFile,"/"); strcat(resFile, "ACS_Stations_Table.conf");
	
	if ( ! (status = rs_getKeyFromValue(in_value, resFile, out_key)) )
	{
		/* Successfully obtained out_key */
		int i ;
		if ( sscanf(out_key, "%d", &i) != 1)
			return -2 ;

		*st_Num = static_cast<unsigned char>(i) ;
	}
	
	return status ;	
}


/*
 * DESCRIPTION: This base function read config-file and returns a Value given a Key
 *
 *     Errors:
 *         -1: Generic error
 *         -5: Key not found
 *        -10: Value not found
 */
int                                             /* OUT: 0 success, otherwise error */
    rs_getValueFromKey(
                           const char* in_key,      /* IN: Key */
                           const char* inResFile,   /* IN: Resource file */
                           char* out_value          /* OUT: Value, or NULL */
                        )
{
    int  
            found,
            i,
            error,
            linerr,
            db,
            keynum,
            valuenum ;
    char resFile[128], msg[1100], buffer[1024] ;
    
    strcpy(resFile, inResFile); /* since basename below needs a (not const) char* as input */
    
	
    elBGN_DEBUG(10)
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey   in_key: %s  inResFile: %s", in_key, inResFile);
        elNotifyMessage(msg) ;
	elEND_DEBUG
    
    
    error = rmCreateDb("resdb") ;

    if (error < 0) return error ;	/* Unable to create DB */
        
    db = error ;
    strcpy(msg, basename(resFile));     /* used as temporary buffer */
    strcpy(buffer, dirname(resFile)); strcat(buffer, "/") ; /* used as temporary buffer */
    error = rmRead(&linerr, db, msg, buffer, rmNoEncoding, FALSE, FALSE) ;
 
 
    if (error < 0)
    {
	    sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) rmRead failed (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return error ;
    }

    if ( (error = rmGetValue(db, "KEY[]", rmInt, &keynum)) )
    { 
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) KEY vector not found (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return error ;	/* Internal error */
    }
    
    if ( (error = rmGetValue(db, "VALUE[]", rmInt, &valuenum)) )
    { 
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) VALUE vector not found (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return error ;	/* Internal error */
    }


    if ( keynum != valuenum )
    {
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) File corrupted", inResFile);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return -1 ;	/* INTERNAL ERROR */
    }
    else
    {
        /*  S U C C E S S  */
        elBGN_DEBUG(10)
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Found %d keys", inResFile, keynum );
            elNotifyMessage(msg) ;
        elEND_DEBUG
    }

    
    found = -5 ;    /* Key Not found */
    for (i=0; i<keynum; i++)
    {
        char tmp[1024] ;
        
        sprintf(tmp, "KEY[%03d]", i);
        if ( (error = rmGetValue(db, tmp, rmString, buffer)) )
        {
            elBGN_DEBUG(10)
                sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't access to KEY #%d (%d)", inResFile, i, error);
                elNotifyMessage(msg) ;
            elEND_DEBUG
        }
        else
        {
            elBGN_DEBUG(20)
                sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Processing key: (%d) %s", inResFile, i, buffer );
                elNotifyMessage(msg) ;
            elEND_DEBUG

            if ( (error = LRtrim(buffer, tmp)) )
            {
                sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't perform trim on key %d (%d)", inResFile, i, error);
                elNotifyMessage(msg) ;
                rmDeleteDb(db); return -10 ;    /* INTERNAL ERROR */
            }

            if (!strcmp(tmp, in_key))
            {
                found = i ;
                break ;
            }
        }
    }
    
    if (found<0)
    {
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) key %s NOT FOUND", inResFile, in_key );
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return found ;	/* KEY NOT FOUND */
    }
    else
    {
        char tmp[1024], tmp_out[1024] ;
        sprintf(tmp, "VALUE[%03d]", found);

        if ( (error = rmGetValue(db, tmp, rmString, tmp_out)) )
        {
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't access to VALUE #%d (%d)", inResFile, found, error);
            elNotifyMessage(msg) ;
            rmDeleteDb(db); return -10 ;    /* VALUE NOT FOUND */
        }
        
        
        if ( (error = LRtrim(tmp_out, out_value)) )
        {
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't perform trim on value %d (%d)", inResFile, found, error);
            elNotifyMessage(msg) ;
            rmDeleteDb(db); return -10 ;    /* INTERNAL ERROR */
        }
        
        
        elBGN_DEBUG(20)
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Found value: (%d) %s", inResFile, found, out_value );
            elNotifyMessage(msg) ;
        elEND_DEBUG
    }
        
        
    if ( (error = rmDeleteDb(db)) )
    {
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't erase DN (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        return -1;	/* INTERNAL ERROR */
    }
        
    
    return 0 ;
}


/*
 * DESCRIPTION: This base function read config-file and returns a Key given a Value
 *
 *     Errors:
 *         -1: Generic error
 *         -5: Value not found
 *        -10: Key not found
 */
int                                             /* OUT: 0 success, otherwise error */
    rs_getKeyFromValue(
                           const char* in_value,      /* IN: Key */
                           const char* inResFile,   /* IN: Resource file */
                           char* out_key          /* OUT: Value, or NULL */
                        )
{
    int  
            found,
            i,
            error,
            linerr,
            db,
            keynum,
            valuenum ;
    char resFile[128], msg[1100], buffer[1024] ;
    
    strcpy(resFile, inResFile); /* since basename below needs a (not const) char* as input */
    
    error = rmCreateDb("resdb") ;

    if (error < 0)
	    return error ;

        
    db = error ;
    strcpy(msg, basename(resFile));     /* used as temporary buffer */
    strcpy(buffer, dirname(resFile)); strcat(buffer, "/") ; /* used as temporary buffer */
    error = rmRead(&linerr, db, msg, buffer, rmNoEncoding, FALSE, FALSE) ;
 
 
    if (error < 0)
    {
	    sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) rmRead failed (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return error ;
    }

    if ( (error = rmGetValue(db, "KEY[]", rmInt, &keynum)) )
    { 
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) KEY vector not found (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return keynum ;
    }
    
    if ( (error = rmGetValue(db, "VALUE[]", rmInt, &valuenum)) )
    { 
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) VALUE vector not found (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return valuenum ;
    }

    if ( keynum != valuenum )
    {
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) File corrupted", inResFile);
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return -1 ;
    }
    else
    {
        /*  S U C C E S S  */
        elBGN_DEBUG(10)
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Found %d keys", inResFile, keynum );
            elNotifyMessage(msg) ;
        elEND_DEBUG
    }

    
    found = -5 ;    /* Value Not found */
    for (i=0; i<keynum; i++)
    {
        char tmp[1024] ;
        
        sprintf(tmp, "VALUE[%03d]", i);	/* Create Key Name */
        if ( (error = rmGetValue(db, tmp, rmString, buffer)) )
        {
            elBGN_DEBUG(10)
                sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't access to VALUE #%d (%d)", inResFile, i, error);
                elNotifyMessage(msg) ;
            elEND_DEBUG
        }
        else
        {
            elBGN_DEBUG(20)
                sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Processing value: (%d) %s", inResFile, i, buffer );
                elNotifyMessage(msg) ;
            elEND_DEBUG

            if ( (error = LRtrim(buffer, tmp)) )
            {
                sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't perform trim on value %d (%d)", inResFile, i, error);
                elNotifyMessage(msg) ;
                rmDeleteDb(db); return -10 ;    /* INTERNAL ERROR */
            }

            if (!strcmp(tmp, in_value))
            {
                found = i ;
                break ;
            }
        }
    }
    
    if (found<0)
    {
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) value %s NOT FOUND", inResFile, in_value );
        elNotifyMessage(msg) ;
        rmDeleteDb(db); return found ;	/* Key not found */
    }
    else
    {
        char tmp[1024], tmp_out[1024] ;
        sprintf(tmp, "KEY[%03d]", found);

		/* Get Key */
        if ( (error = rmGetValue(db, tmp, rmString, tmp_out)) )
        {
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't access to KEY #%d (%d)", inResFile, found, error);
            elNotifyMessage(msg) ;
            rmDeleteDb(db); return -10 ;    /* KEY NOT FOUND */
        }
        
        /* Remove blanks from L and R */
        if ( (error = LRtrim(tmp_out, out_key)) )
        {
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't perform trim on key %d (%d)", inResFile, found, error);
            elNotifyMessage(msg) ;
            rmDeleteDb(db); return -10 ;    /* INTERNAL ERROR */
        }
        
        
        elBGN_DEBUG(20)
            sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Found key: (%d) %s", inResFile, found, out_key );
            elNotifyMessage(msg) ;
        elEND_DEBUG
    }
        
        
    if ( (error = rmDeleteDb(db)) )
    {
        sprintf(msg, "ESAProductFileName::rs_getValueFromKey::(%s) Can't erase DN (%d)", inResFile, error);
        elNotifyMessage(msg) ;
        return -1;
    }
        
    
    return 0 ;
}


/*
 * Convert Data_Transcription_Statistic endianess 
*/
int 																/* OUT: return value (0 = GOOD) */
        ACS_ESAProductFileName_Endian_DTStatistic_Converter
        (
            const Data_Transcription_Statistic* statRecord, 	/* IN: Input Statistics */
            int isBigEnd,										/* IN: Set TRUE if Structure is BIGENDIAN */
            Data_Transcription_Statistic* outStatRecord			/* OUT: Converted Statistic */
        )
{
	int i ;
	
	
	if (statRecord != outStatRecord)
		memcpy(outStatRecord, statRecord, sizeof(Data_Transcription_Statistic) ) ;
	;
	
	/* Convert the endianness */
    ACS_ByteSwap_SWAP(&outStatRecord->File_Key, sizeof(outStatRecord->File_Key), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Satellite_ID, sizeof(outStatRecord->Satellite_ID), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Mission_ID, sizeof(outStatRecord->Mission_ID), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Instrument_Type_ID, sizeof(outStatRecord->Instrument_Type_ID), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Instrument_Number, sizeof(outStatRecord->Instrument_Number), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Transmission_Channel, sizeof(outStatRecord->Transmission_Channel), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Station_ID, sizeof(outStatRecord->Station_ID), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Station_DT_ID, sizeof(outStatRecord->Station_DT_ID), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Track_Number, sizeof(outStatRecord->Track_Number), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Orbit_Number, sizeof(outStatRecord->Orbit_Number), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Cycle_Number, sizeof(outStatRecord->Cycle_Number), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Number_of_Frames, sizeof(outStatRecord->Number_of_Frames), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->First_Frame, sizeof(outStatRecord->First_Frame), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Minor_Frame_in_Error, sizeof(outStatRecord->Minor_Frame_in_Error), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Eov_in_Error, sizeof(outStatRecord->Eov_in_Error), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Calibration_in_Error, sizeof(outStatRecord->Calibration_in_Error), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Dark_in_Error, sizeof(outStatRecord->Dark_in_Error), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Acquisition_Day, sizeof(outStatRecord->Acquisition_Day), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Input_Hddr_Type, sizeof(outStatRecord->Input_Hddr_Type), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Input_Hddr_Number, sizeof(outStatRecord->Input_Hddr_Number), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Output_Media_Type, sizeof(outStatRecord->Output_Media_Type), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Output_Media_Number, sizeof(outStatRecord->Output_Media_Number), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Output_Device_Type, sizeof(outStatRecord->Output_Device_Type), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Number_of_Segments, sizeof(outStatRecord->Number_of_Segments), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Loaded_Swath, sizeof(outStatRecord->Loaded_Swath), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Swath_Size, sizeof(outStatRecord->Swath_Size), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Swath_per_Block, sizeof(outStatRecord->Swath_per_Block), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Number_of_Blocks, sizeof(outStatRecord->Number_of_Blocks), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Number_of_Files, sizeof(outStatRecord->Number_of_Files), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Header_Address, sizeof(outStatRecord->Header_Address), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Pass_Address, sizeof(outStatRecord->Pass_Address), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->Data_On_Tape, sizeof(outStatRecord->Data_On_Tape), isBigEnd) ;
    ACS_ByteSwap_SWAP(&outStatRecord->CopyInputTapeNumber, sizeof(outStatRecord->CopyInputTapeNumber), isBigEnd) ;

    for (i=0; i<3; i++)
    {
            ACS_ByteSwap_SWAP(&outStatRecord->Acquisition_Date[i], sizeof(outStatRecord->Acquisition_Date[i]), isBigEnd) ;
            ACS_ByteSwap_SWAP(&outStatRecord->Transcription_Date[i], sizeof(outStatRecord->Transcription_Date[i]), isBigEnd) ;
            ACS_ByteSwap_SWAP(&outStatRecord->Transcription_Start[i], sizeof(outStatRecord->Transcription_Start[i]), isBigEnd) ;
            ACS_ByteSwap_SWAP(&outStatRecord->Transcription_End[i], sizeof(outStatRecord->Transcription_End[i]), isBigEnd) ;
    }

    for (i=0; i<4; i++)
    {
        ACS_ByteSwap_SWAP(&outStatRecord->Acquisition_Start[i], sizeof(outStatRecord->Acquisition_Start[i]), isBigEnd) ;
        ACS_ByteSwap_SWAP(&outStatRecord->Acquisition_End[i], sizeof(outStatRecord->Acquisition_End[i]), isBigEnd) ;
    }


	return 0 ;	/* Successfully */
}




/*
 * DESCRIPTION: Trim a given string (remove space & tabs at beginning and at the end)
 *
 *     WARNING: output string must be allocated by the caller 
 *     For example:
 *         "abcd" -> "abcd" 
 *         "  ab cd " -> "ab cd"
 *         "\tab\tcd\t" -> "ab\tcd"
 */
int                                             /* OUT: 0 = success */ 
        LRtrim(
                    const char* in_string,      /* IN: input string */ 
                    char* out_string)           /* OUT: output string (allocated by the caller) */
{
	signed short int i, i_start, i_stop ;


	i_start = -1 ;
	for (i=0; i<static_cast<signed short int>(strlen(in_string)); i++)
		if (in_string[i] != ' ' && in_string[i] !='\t')
		{
			i_start	= i ;	/* Found the first valid char from the left (if any) */
			break ;
		}

	i_stop = -1 ;	
	for (i=strlen(in_string)-1; i>=0; i--)
		if (in_string[i] != ' ' && in_string[i] !='\t')
		{
			i_stop	= i ;	/* Found the first valid char from the right (if any) */
			break ;
		}


	if (i_start >= 0 && i_stop >= 0)
    {
		strncpy(out_string, (in_string + i_start), (i_stop-i_start+1)) ;
	    out_string[i_stop-i_start+1] = 0 ;    /* terminate with 0 */
    }
    else
		out_string[0] = 0 ;	/* return empty string */


	return 0 ;
}


/*
 * DESCRIPTION: Convert a JD50 date into a string
 *
 *      (if required, method could be splitted in order to handle several output formats)
 *
 *      WARNING: output string must be allocated by the caller 
 *
 */
/* static method */
void 
        jd50toString(   const double jd50,     /* IN: JD50 date (double) */ 
                        char* out_string,            /* OUT: fixed-format time string */
                        int round)				/* TRUE -> CEIL , FALSE -> FLOOR */
{
    struct struct_utcd_usec {
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		int usec;
	} ;

    struct struct_utcd_usec utcd ;


	ACS_ESAProductFileName_JD50ToDate(jd50, &utcd.year, &utcd.month, &utcd.day, &utcd.hour, &utcd.min, &utcd.sec, &utcd.usec ) ;

	if (round && utcd.usec)
    {
    	/* CEIL */
        
        /* 
            Ceil is evaluated by adding + 1 second to initial value, as JD50.
            So, CEIL is the FLOOR of (jd50 + 1 second):
	    In order to avoid that 0.000000s will became 1.0s, a number smaller than 1s is really added
        */
	
	double delta = 0.0000115739 ; /* approx 1.0/(3600*24), minus a small epsilon (tested on SGI) */
    	ACS_ESAProductFileName_JD50ToDate((jd50+delta), &utcd.year, &utcd.month, &utcd.day, &utcd.hour, &utcd.min, &utcd.sec, &utcd.usec ) ;
    }

    /* Format is: YYYYMMDDTHHMMSS */
    sprintf(out_string, "%04d%02d%02dT%02d%02d%02d", utcd.year, utcd.month, utcd.day, utcd.hour, utcd.min, utcd.sec); 

    return ;
}


/*
 * DESCRIPTION: Convert a JD50 date into a string
 *
 *      (if required, method could be splitted in order to handle several output formats)
 *
 *      WARNING: output string must be allocated by the caller 
 *
 */
/* static method */
double 
        stringToJD50(   const char* in_string,	/* IN: fixed-format time string */
						double* jd50     		/* OUT: JD50 date (double) */ 
                    )  
{
    struct struct_utcd {
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		int msec;
	} ;

    struct struct_utcd utcd ;


    /* Format is: YYYYMMDDTHHMMSS */
    sscanf(in_string, "%04d%02d%02dT%02d%02d%02d", &utcd.year, &utcd.month, &utcd.day, &utcd.hour, &utcd.min, &utcd.sec); 


	*jd50 = ACS_ESAProductFileName_DateToJD50(utcd.year, utcd.month, utcd.day, utcd.hour, utcd.min, utcd.sec, 0);


    return (*jd50) ;
}



/*
 * DESCRIPTION: Converts a date(Y,M,D,H,M,S,ms) into a JD50 double
 *
 * (if possible, this method should be removed in order to use common libraries on both SGI/Linux)
 *
 */
double
        ACS_ESAProductFileName_DateToJD50(
            int year,
            int month,
            int day,
            int hour,
            int min,
            int sec,
            int msec
        ) 
{
    /*
     *  From: DateTime.C
     */
    double jsec = sec+msec/1000.0;
    int jj = (14 - month)/12;
    int ll = year - 1900 - jj;
    int jday = day-18234+(1461*ll)/4+(367*(month-2+jj*12))/12;
    double jd50 = jday + ((hour*60 + min)*60.0 + jsec)/86400.0;

    return jd50 ;
}


/*
 * DESCRIPTION: Converts a JD50 double into a date(Y,M,D,H,M,S,usec)
 *
 * (if possible, this method should be removed in order to use common libraries on both SGI/Linux)
 *
 */
void
        ACS_ESAProductFileName_JD50ToDate(const double jd50, int* year, int* month, int* day, int* hour, int* min, int* sec, int* usec ) 
{
	/* Convert a JD50 date to YMD/hmsu*/

    double r = jd50 ;
    double DDAY = r + 0.0000000001 ;
    int JDAY = static_cast<int>(DDAY) ;	/* Get integer part */
    int L = (4000 * (JDAY  + 18204)) / 1461001 ;
    int N = JDAY - (1461 * L) / 4 + 18234 ; 
    int M = (80 * N) / 2447 ;
    int JJ = M / 11 ;
    *day = N - (2447 * M) / 80 ;		/* Get day */
    *month = M + 2 - 12 * JJ ;			/* Get month */
    *year= 1900 + L + JJ ;				/* Get year */
    r = (DDAY - static_cast<double>(JDAY)) * 24.0 ;	/* [ Get time (as a fraction) ] */
    *hour = static_cast<int>(r) ;					/* Get hour */
	r = (r - (*hour)) * 60.;
    *min  = static_cast<int>(r); 					/* Get minute */
	r = (r - (*min)) * 60.;
    *sec  = static_cast<int>(r); 					/* Get second */
	r = (r - (*sec)) * 1.0;
    *usec  = static_cast<int>(r * 1000000.0) ;		/* Get microsec */
	
    return ;
}


/*
 * DESCRIPTION: returns the upper-case of given string
 *
 */
char* toUpper(const char* in_str, char* out_str)
{
	int i ;
    
    if ( !in_str ) 
        out_str[0]=0;
	else
    {
        for (i=0; i<=static_cast<int>(strlen(in_str)); i++)
    	    out_str[i] = ( in_str[i]>='a' && in_str[i]<='z')? (in_str[i] - 0x20): in_str[i] ;
    }    
	
    return out_str ;	/* Return the upper-case string */
}
