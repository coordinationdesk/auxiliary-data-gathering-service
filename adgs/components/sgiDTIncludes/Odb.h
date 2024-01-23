/*

    Copyright 1995-2006, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. Odb.h HEADER $

    $Author$

    $Id$

    $Log$
    Revision 1.1  2006/08/24 09:39:56  enrcar
    Imported source


*/


/* Based on: A.C.S. Odb Library - Odb.h 1.11 97/10/21 (include) */

#ifndef __LIBODB_H__
#define __LIBODB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ODB_ERROR_OFFSET	-13200
#define ODB_MAX_ERROR		-13299

#ifndef JD50
typedef double JD50 ;
#endif

typedef enum {
	ODB_NOT_ENOUGH_MEMORY		= (ODB_ERROR_OFFSET -  0),
	ODB_EOF_REACHED			= (ODB_ERROR_OFFSET -  1),
	ODB_CANNOT_READ_REC		= (ODB_ERROR_OFFSET -  2),
	ODB_CANNOT_WRITE_REC		= (ODB_ERROR_OFFSET -  3),
	ODB_INVALID_FORMAT		= (ODB_ERROR_OFFSET -  4),
	ODB_CANNOT_READ_LINE		= (ODB_ERROR_OFFSET -  5),
	ODB_CANNOT_ADD_REC		= (ODB_ERROR_OFFSET -  6),
	ODB_CANNOT_DEL_REC		= (ODB_ERROR_OFFSET -  7),
	ODB_INVALID_INPUT		= (ODB_ERROR_OFFSET -  8),
	ODB_CANNOT_OPEN_DB		= (ODB_ERROR_OFFSET -  9),
	ODB_CANNOT_SKIP_HEADER		= (ODB_ERROR_OFFSET - 10),
	ODB_CANNOT_FIND_WIDGET		= (ODB_ERROR_OFFSET - 11),
	ODB_INVALID_SATELLITE		= (ODB_ERROR_OFFSET - 12),
	ODB_INVALID_MISSION		= (ODB_ERROR_OFFSET - 13),
	ODB_INVALID_DBTYPE		= (ODB_ERROR_OFFSET - 14),
	ODB_ENVIRONMENT_ERROR		= (ODB_ERROR_OFFSET - 15),
	ODB_RECORD_NOT_FOUND		= (ODB_ERROR_OFFSET - 16),
	ODB_INVALID_FILTER_TYPE		= (ODB_ERROR_OFFSET - 17),
	ODB_INVALID_CARD		= (ODB_ERROR_OFFSET - 18),
	ODB_CANNOT_RENAME_DB		= (ODB_ERROR_OFFSET - 19),
	ODB_INVALID_RECORD		= (ODB_ERROR_OFFSET - 20),
	ODB_INVALID_YEAR		= (ODB_ERROR_OFFSET - 21),
	ODB_NOT_IMPLEMENTED		= (ODB_ERROR_OFFSET - 99)
} ODB_ERROR_TYPE ;

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olComeFrom {
#endif
	ComeFromManual,
	ComeFromFile,
	ComeFromDb
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olComeFrom ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olFilterType {
#endif
	NoFilter,
	OrbitFilter,
	DateFilter
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olFilterType ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olFormat {
#endif
	Ascii,
	AsciiPrcl,
	Binary,
	Binary2
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olFormat ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olPropagation {
#endif
	PropBySv, /* orbit propagation is performed from State Vect */
	PropByMoe /* orbit propagation is performed from Moe */
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olPropagation ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olValidation {
#endif
	NotChecked,  /* the Moe data has not been checked */
	Valid,       /* the Moe data is valid */
	ForcedValid, /* the Moe data is forced valid from the operator */
	NotValid     /* the Moe data is not valid */
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olValidation ;
#else
} ;
#endif


#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olRespToReservDiagnostic {
#endif
	Refused = 0,
	AcceptedInPart = 10,
	AcceptedInFull = 20
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olRespToReservDiagnostic ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olRespToProgDiagnostic {
#endif
	ProgReqAccepted	= 10,
	CenterPtInaccessible	= 20,
	TooManySegments		= 30,
	SpecularReflection	= 40,
	CenterPtOutside		= 50,
	SegmentTooShort		= 60,
	WrongPlayloadConfig	= 70
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olRespToProgDiagnostic ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olIspDiagnostic {
#endif
	StationProgramming = 10,
	DefaultProgramming = 20,
	NoProgramming	   = 30
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olIspDiagnostic ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olRequestType {
#endif
	ReservDefaultProg = 1,
	ReservNodefaultProg
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olRequestType ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olProgrammingType {
#endif
	Hrv = 1,
	CoupledHrvs,
	IndepHrvs
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olProgrammingType ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olSpotGainType {
#endif
	StandardGain,
	HighGain,
	LowGain
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olSpotGainType ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olSpectralMode {
#endif
	PaMode = 1,
	PaDpcmMode,
	XsMode,
	PaXsMode,	/* available only with HRV programming type */
	PaDpcmXsMode	/* available only with HRV programming type */
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olSpectralMode ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
typedef enum {
#else
enum olAntennaPtDataType {
#endif
	OsculatingElems,
	EphemerisData
#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )
} olAntennaPtDataType ;
#else
} ;
#endif

#if !defined(__cplusplus) || ( !defined(sgi) && !defined(ACSTEST) )

	typedef struct olErsSvList  olErsSvList ;
	typedef struct olErsTcList  olErsTcList ;
	typedef struct olJLSvList   olJLSvList ;
	typedef struct olJersTcList olJersTcList ;
	typedef struct olSpotList   olSpotList ;
	typedef struct olSpotInfo   olSpotInfo ;
	typedef struct olJLSvInfo   olJLSvInfo ;
	typedef struct olJersTcInfo  olJersTcInfo ;
	typedef struct olErsSvInfo  olErsSvInfo ;
	typedef struct olErsTcInfo  olErsTcInfo ;

#ifdef __STDC__

	olErsSvList *olErsSvListCreate(void) ;
	olErsTcList *olErsTcListCreate(void) ;
	olJLSvList *olJLSvListCreate(void) ;
	olJersTcList *olJersTcListCreate(void) ;
	olSpotList *olSpotListCreate(void) ;

	int olErsSvListDestroy(olErsSvList *) ;
	int olErsTcListDestroy(olErsTcList *) ;
	int olJLSvListDestroy(olJLSvList *) ;
	int olJersTcListDestroy(olJersTcList *) ;
	int olSpotListDestroy(olSpotList *) ;

	int olErsSvGetLoadFlag(olErsSvList *) ;
	int olErsTcGetLoadFlag(olErsTcList *) ;
	int olJLSvGetLoadFlag(olJLSvList *) ;
	int olJersTcGetLoadFlag(olJersTcList *) ;
	int olSpotGetLoadFlag(olSpotList *) ;

	int olErsSvSetLoadFlag(olErsSvList *, unsigned char) ;
	int olErsTcSetLoadFlag(olErsTcList *, unsigned char) ;
	int olJLSvSetLoadFlag(olJLSvList *, unsigned char) ;
	int olJersTcSetLoadFlag(olJersTcList *, unsigned char) ;
	int olSpotSetLoadFlag(olSpotList *, unsigned char) ;

	int olErsSvSetSat(olErsSvList *, int) ;
	int olErsTcSetSat(olErsTcList *, int) ;
	int olJLSvSetSat(olJLSvList *, int) ;
	int olJersTcSetSat(olJersTcList *, int) ;
	int olSpotSetSat(olSpotList *, int) ;

	int olErsSvGetSat(olErsSvList *) ;
	int olErsTcGetSat(olErsTcList *) ;
	int olJLSvGetSat(olJLSvList *) ;
	int olJersTcGetSat(olJersTcList *) ;
	int olSpotGetSat(olSpotList *) ;

	int olErsSvSetMis(olErsSvList *, int) ;
	int olErsTcSetMis(olErsTcList *, int) ;
	int olJLSvSetMis(olJLSvList *, int) ;
	int olJersTcSetMis(olJersTcList *, int) ;
	int olSpotSetMis(olSpotList *, int) ;

	int olErsSvGetMis(olErsSvList *) ;
	int olErsTcGetMis(olErsTcList *) ;
	int olJLSvGetMis(olJLSvList *) ;
	int olJersTcGetMis(olJersTcList *) ;
	int olSpotGetMis(olSpotList *) ;

	int olErsSvSetDbType(olErsSvList *, int) ;
	int olErsTcSetDbType(olErsTcList *, int) ;
	int olJLSvSetDbType(olJLSvList *, int) ;
	int olJersTcSetDbType(olJersTcList *, int) ;
	int olSpotSetDbType(olSpotList *, int) ;

	int olErsSvGetDbType(olErsSvList *) ;
	int olErsTcGetDbType(olErsTcList *) ;
	int olJLSvGetDbType(olJLSvList *) ;
	int olJersTcGetDbType(olJersTcList *) ;
	int olSpotGetDbType(olSpotList *) ;

	int olErsSvSetName(olErsSvList *, char *) ;
	int olErsTcSetName(olErsTcList *, char *) ;
	int olJLSvSetName(olJLSvList *, char *) ;
	int olJersTcSetName(olJersTcList *, char *) ;
	int olSpotSetName(olSpotList *, char *) ;

	int olErsSvSetDateName(olErsSvList *, JD50) ;
	int olErsTcSetDateName(olErsTcList *, JD50) ;
	int olJLSvSetDateName(olJLSvList *, JD50) ;
	int olJersTcSetDateName(olJersTcList *, JD50) ;
	int olSpotSetDateName(olSpotList *, JD50) ;

	char *olErsSvGetName(olErsSvList *) ;
	char *olErsTcGetName(olErsTcList *) ;
	char *olJLSvGetName(olJLSvList *) ;
	char *olJersTcGetName(olJersTcList *) ;
	char *olSpotGetName(olSpotList *) ;

	int olErsSvGetRecNb(olErsSvList *) ;
	int olErsTcGetRecNb(olErsTcList *) ;
	int olJLSvGetRecNb(olJLSvList *) ;
	int olJersTcGetRecNb(olJersTcList *) ;
	int olSpotGetRecNb(olSpotList *) ;

	int olErsSvLoadDb(olErsSvList *, olComeFrom) ;
	int olErsTcLoadDb(olErsTcList *, olComeFrom) ;
	int olJLSvLoadDb(olJLSvList *, olComeFrom) ;
	int olJersTcLoadDb(olJersTcList *, olComeFrom) ;
	int olSpotLoadDb(olSpotList *, olComeFrom) ;

	int olErsSvSaveDb(olErsSvList *) ;
	int olErsTcSaveDb(olErsTcList *) ;
	int olJLSvSaveDb(olJLSvList *) ;
	int olJersTcSaveDb(olJersTcList *) ;
	int olSpotSaveDb(olSpotList *) ;

	int olGetSpotRecord(olSpotList *, int, unsigned, olSpotInfo **) ;
	int olGetSpotRecByDate(olSpotList *, int, unsigned **, olSpotInfo **) ;
	int olGetErsSvRecByDate(olErsSvList *, JD50, olErsSvInfo **) ;
	int olGetErsTcRecByDate(olErsTcList *, JD50, olErsTcInfo **) ;
	void olInitLib(void) ;

#else /* __STDC__ */

	olErsSvList *olErsSvListCreate() ;
	olErsTcList *olErsTcListCreate() ;
	olJLSvList *olJLSvListCreate() ;
	olJersTcList *olJersTcListCreate() ;
	olSpotList *olSpotListCreate() ;

	int olErsSvListDestroy() ;
	int olErsTcListDestroy() ;
	int olJLSvListDestroy() ;
	int olJersTcListDestroy() ;
	int olSpotListDestroy() ;

	int olErsSvGetLoadFlag() ;
	int olErsTcGetLoadFlag() ;
	int olJLSvGetLoadFlag() ;
	int olJersTcGetLoadFlag() ;
	int olSpotGetLoadFlag() ;

	int olErsSvSetLoadFlag() ;
	int olErsTcSetLoadFlag() ;
	int olJLSvSetLoadFlag() ;
	int olJersTcSetLoadFlag() ;
	int olSpotSetLoadFlag() ;

	int olErsSvSetSat() ;
	int olErsTcSetSat() ;
	int olJLSvSetSat() ;
	int olJersTcSetSat() ;
	int olSpotSetSat() ;

	int olErsSvGetSat() ;
	int olErsTcGetSat() ;
	int olJLSvGetSat() ;
	int olJersTcGetSat() ;
	int olSpotGetSat() ;

	int olErsSvSetMis() ;
	int olErsTcSetMis() ;
	int olJLSvSetMis() ;
	int olJersTcSetMis() ;
	int olSpotSetMis() ;

	int olErsSvGetMis() ;
	int olErsTcGetMis() ;
	int olJLSvGetMis() ;
	int olJersTcGetMis() ;
	int olSpotGetMis() ;

	int olErsSvSetDbType() ;
	int olErsTcSetDbType() ;
	int olJLSvSetDbType() ;
	int olJersTcSetDbType() ;
	int olSpotSetDbType() ;

	int olErsSvGetDbType() ;
	int olErsTcGetDbType() ;
	int olJLSvGetDbType() ;
	int olJersTcGetDbType() ;
	int olSpotGetDbType() ;

	int olErsSvSetName() ;
	int olErsTcSetName() ;
	int olJLSvSetName() ;
	int olJersTcSetName() ;
	int olSpotSetName() ;

	int olErsSvSetDateName() ;
	int olErsTcSetDateName() ;
	int olJLSvSetDateName() ;
	int olJersTcSetDateName() ;
	int olSpotSetDateName() ;

	char *olErsSvGetName() ;
	char *olErsTcGetName() ;
	char *olJLSvGetName() ;
	char *olJersTcGetName() ;
	char *olSpotGetName() ;

	int olErsSvGetRecNb() ;
	int olErsTcGetRecNb() ;
	int olJLSvGetRecNb() ;
	int olJersTcGetRecNb() ;
	int olSpotGetRecNb() ;

	int olErsSvLoadDb() ;
	int olErsTcLoadDb() ;
	int olJLSvLoadDb() ;
	int olJersTcLoadDb() ;
	int olSpotLoadDb() ;

	int olErsSvSaveDb() ;
	int olErsTcSaveDb() ;
	int olJLSvSaveDb() ;
	int olJersTcSaveDb() ;
	int olSpotSaveDb() ;

	int olGetSpotRecord() ;
	int olGetSpotRecByDate() ;
	int olGetErsSvRecByDate() ;
	int olGetErsTcRecByDate() ;
	void olInitLib() ;

#endif /* __STDC__ */
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LIBODB_H__*/
