/*

	Copyright 1995-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DirectIngestionOrder.h $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2006/08/29 13:27:38  enrcar
	DirectIngestionOrder.h file added
	
		
*/


#ifndef __DIRINGORD_H__
#define __DIRINGORD_H__

#ifndef DEBUG_LOW
#define DEBUG_LOW 0
#define DEBUG_MID 1
#define DEBUG_HIGH 2
#define DEBUG_ALL 3
#endif

#ifndef Ext
#ifdef THIS_IS_MAIN
#define Ext 
#else
#define Ext extern
#endif
#endif 

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const* DirectIngestionOrderhdt="@(#) Advanced Computer Systems: DirectIngestionOrder.h  1.7 02/06/18 (include)";

typedef struct
{
/*1*/	char	Mode[50];				/* Processing Mode (Archiving or Ingestion)	*/
/*2*/	int		SatelliteId;			/* Satellite Identificator		 			*/
/*3*/	int		Mission;				/* Mission  Number							*/ 
/*4*/	int		SensorId;				/* Sensor identificator		     			*/
/*5*/	double	AcquisitionJdtTime;		/* Acquisition Jdt Time		     			*/
/*6*/	int		OrbitNumber;			/* Orbit Number	    		     			*/
/*7*/	int		PassEstimatedDuration;	/* Pass len in minutes 						*/
/*8*/	int		MessageQueueId	;		/* MessaQueue Identificator					*/ 
/*9*/	char	ProcessingId[20];		/* Processing Identificator					*/
/*10*/	int		SystematicProductionFlag;/* SystematicProductionFlag				*/
/*11*/	int		InputDeviceType;		/* InputDeviceType							*/
/*12*/	int		WilmaOnDiskFlag;		/* WilmaOnDisk								*/
/*13*/	char	OutputPath[512];		/* Output Path	(to force output saving not in work dir */
#ifndef sgi
    /* LINUX PADDING: required since structure size must be multiple by 8 */
        char    __linux_padding[4] ;
#endif
}direct_ingestion_order;


int DIReadOrderFile (char *ParFileName, direct_ingestion_order *dipar);
int DIWriteOrderFile(char *ParFileName, direct_ingestion_order *dipar);
int ENVDIDumpOrderFile(direct_ingestion_order *dipar);

#ifdef __cplusplus
}
#endif
#endif
