/*
	
	Copyright 1995-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libmdps header file $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2006/12/13 17:00:43  davcas
	Added
	
	Revision 1.5  2006/06/12 15:06:25  enrcar
	includes and versioning fixed
	
	Revision 1.4  2006/06/12 15:00:21  davcas
	Wrong define. Fixed.
	
	Revision 1.3  2006/06/12 09:27:55  enrcar
	Library released
	

*/



#ifndef __LIBMDPSC_H__
#define __LIBMDPSC_H__

#ifndef __LIBMDPSC_CH__
static char const* HId="@(#) Advanced Computer Systems: :$Id$ (include)";
#define __LIBMDPSC_CH__
#endif

#ifdef __cplusplus
	extern "C" {
#endif

/* INCLUDE SECTION */
#include <DataTranscription/dtTypes.h>

/* DEFINE SECTION */
#define  DIHWSETUPPAR  "./mdpsHWSetup.par"

/* ENUM SECTION */
/*  */
typedef enum {
	DEVICE_IS_OK=0,
	DEVICE_HAS_ERRORS,
	DEVICE_IS_NOT_OK
} DeviceStatus;

/*  */
typedef enum {
	MDPS_ENV_VARS_MODE=1,
	MDPS_CONF_FILE_MODE,
	MDPS_CONF_FILE_NO_AUTO_MODE
} MdpsMode;

/*  */
typedef enum {
	MDPS_SOURCE_IS_BOARD=1, 
	MDPS_SOURCE_IS_FILE,
	MDPS_SOURCE_IS_TAPE 
} DeviceType;


/* FUNCTIONS USED BY INGESTION MODULES */
#ifdef sgi
	int mdpsIngeGETinputSwitchCh 		(int* InputChannel);
#endif
int mdpsIngeGETdeviceDriver 		(DeviceType* devTypeID, char* devName);
int mdpsIngeGETdeviceDriverAux 		(DeviceType* devTypeID, char* devName);


/* FUNCTIONS USED BY ARCHIVING MODULES */
int mdpsTranGETinputSource 			(char* devType, int* devNum);
int mdpsTranGETCopyInfo 			(int channel, int *copyNum);
int mdpsTranGETdeviceDriver 		(int channel, int copyNum, char* devType, char* devName, int *devId, int *devNum);
int mdpsTranGETCopyInfoTwin 		(int *copyNum);
int mdpsTranGETdeviceDriverTwin     (int copyNum, char* devTypeCh1, char* devNameCh1, int *devIdCh1, int *devNumCh1, 
												  char* devTypeCh2, char* devNameCh2, int *devIdCh2, int *devNumCh2);
/* FUNCTIONS USED BY PROCESSING MODULES */
int mdpsProcGETInpDevDriver 		(char* devName, char *devDriver, int* devNum);
int mdpsProcGETOutDevDriver 		(char* devName, char *devDriver);
int mdpsProcGETOutDevDriverWithInfo (char* devType, char* devDriver, char* label1, char* label2, char* label3, char* label4, char* label5);		


/* GEN PURPOSE MODULES */
void mdpsGEToperationalMode 		 (MdpsMode *mode);
#ifdef sgi
	int  mdpsGETsatSenMiss    			 (int* Satellite, int* Sensor, int* Mission);
#endif
int  mdpsCONFdataSource2SwitchInput  (char* inSource, int *switchInput);
int  mdpsCONFdevTypeAndNum2DevDriver (char* devType, int devNum, char* devDriver);
int  mdpsCONFdevTypeAndNum2DevDriverAux (char* devType, int devNum, char* devDriver);
int  mdpsCONFdevTypeAndNum2DevDriverULD (char* devType, int devNum, char* devDriver);
int  mdpsDevDriver2DevTypeId		 (char *devDriver, DeviceType *devTypeId);
int  mdpsGETdevTypeAndNumFromPar 	 (char* devType, int* devNum);
int  mdpsGETAlwaysRemoveWorkDirs 	 (int *removeFlag);
void mdpsCONFsetConfFilePath 		 (char* filepath);
int mdpsCONFdevTypeAndNum2StackerDevDriver (char* devType, int devNum, char* devDriver);

/* mounted tape info */
int  mdpsGETifTapeIsMounted 		(char *devType, int devNum, int *tapeIsMounted);
int  mdpsGETtapeNumber   			(char *devType, int devNum, int *devId);
int  mdpsGETtapeUsage  				(char *devType, int devNum, double *mbytesUsed);
int  mdpsGETMountedTapeFileName 	(char *devType, int devNum, char *filename);
int  mdpsGETTapeMountedInfo 		(char *devType, int devNum, ID1_Cassette_Descriptor_File *Initialization);
int  mdpsGETStatisticsDirectoryName (char *devType, int devNum, char *dirname);
int  mdpsGETTapeStatistics 		    (char *devType, int devNum, int *numOfRecords, Data_Transcription_Statistic **Statistics);
int  mdpsGETavailableDevs(char *devType, int *devNum);

/* for compatibility with  previous names....wrong names..*/
#define  mdpscGETtapeNumber 		mdpsGETtapeNumber
#define  mdpscGETtapeUsage  		mdpsGETtapeUsage
#define  mdpscGETifTapeIsMounted 	mdpsGETifTapeIsMounted
#ifdef __cplusplus
	}
#endif
#endif
