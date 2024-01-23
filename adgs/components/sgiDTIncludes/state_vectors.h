/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. state_vectors.h $

	$Id$

	$Author$

	$Log$
	
*/



/*
 *	Global variables and definitions of the record of the state vectorsparameter file 
 *	Version 1.2 March 1993 - Bangkok project
 */
#include<sys/types.h>


#ifndef _STATE_VECTORS_H_
#define _STATE_VECTORS_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
/*1*/	double		State_Vector_X,			/* position at ascending node in E-2 m 	*/
/*2*/			State_Vector_Y,
/*3*/			State_Vector_Z,
/*4*/			State_Vector_X_Velocity,	/* velocity at ascending node in E-5 m/s*/
/*5*/			State_Vector_Y_Velocity,
/*6*/			State_Vector_Z_Velocity;		
/*7*/	char		AscNodeUTC[25];			/* Asc Node UTC	  			*/
/*8*/	char		ReferenceUTC[25];		/* UTC reference time 	  		*/
/*9*/	u_int32_t	SatelliteBinaryTime;		/* Satellite Reference binary time	*/
/*10*/	u_int32_t	ClockStepLength;		/* Step Length of clock (ns) 	  	*/
#ifndef sgi
        /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
        char    __linux_padding[4] ;     
#endif
/*11*/	double		MinDistance;			/* closest approach distance	  	*/
/*12*/	double		CompensatedSpeed;		/* compensated speed	  		*/
/*13*/	double		PulseRepetitionFreq;		/* prf	  				*/
}state_vectors;

typedef struct
{
/*1*/	double		State_Vector_X,			/* position at ascending node in E-2 m 	*/
/*2*/			State_Vector_Y,
/*3*/			State_Vector_Z,
/*4*/			State_Vector_X_Velocity,	/* velocity at ascending node in E-5 m/s*/
/*5*/			State_Vector_Y_Velocity,
/*6*/			State_Vector_Z_Velocity;		
/*7*/	char		AscNodeUTC[25];			/* Asc Node UTC	  			*/
/*8*/	char		ReferenceUTC[25];		/* UTC reference time 	  		*/
/*9*/	u_int32_t	SatelliteBinaryTime;		/* Satellite Reference binary time	*/
/*10*/	u_int32_t	ClockStepLength;		/* Step Length of clock (ns) 	  	*/
/*11*/	int		DBType;				/* '0'=predicted '1'=Restituted	  	*/
}DT_state_vectors;

int WriteSV(char *ParFileName, state_vectors *sv);
int GetSV(char *ParFileName, state_vectors *sv);
int WriteDtSV(char *ParFileName, DT_state_vectors *dtsv);
int GetDtSV(char *ParFileName, DT_state_vectors *dtsv);
int ENVDumpSVPar(state_vectors *sv);
int ENVDumpDtSVPar(DT_state_vectors *dtsv);

int ERSSatBinaryTime2Track(ulong AuxSBC, int AuxSWST, int AuxPRI, DT_state_vectors *sv, double *NRLong, double *NRLat, double *FRLong, double *FRLat);


#ifdef __cplusplus
}
#endif

#endif
