/*

	Copyright 1995-2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$
	
	$Log$
	Revision 1.3  2008/02/08 09:53:13  serfol
	added the GAIN_LIFE_TIME radiometric correction
	
	Revision 1.2  2007/09/11 12:44:07  giusy.sinatra
	EC:: LINUX conflict solved on StatusClassification enum
	
	Revision 1.1  2007/09/05 14:05:47  giucas
	Moved from libMDPSStationManagement
	
	Revision 1.6  2007/07/27 16:37:07  aleber
	structure Fdr put under ifedf
	
	Revision 1.5  2007/07/26 10:46:19  aleber
	structure Fdr and related are redefined in smTypes_structures.h
	
	Revision 1.4  2007/07/19 09:32:12  aleber
	structures defined in smTypes_Structures (to account for endianess swap) are commented
	
	Revision 1.3  2006/12/05 15:31:02  valspa
	VDS: added in smTypes.h the GrStat structure to be used by LandsatOrb and ModisOrb
	
	Revision 1.2  2006/11/29 18:05:37  lucio.pulvirenti
	VDS: aligned to smFtdcf class modified by LP
	
	Revision 1.1.1.1  2006/10/27 09:21:47  valspa
	VDS: first porting of the MDPS SGI Station Management to new LINUX environment
	

*/

/*------------------------------------------------------------------------------------------------------*/
/*Purpose:	This file contains the main structures and variables used all over the Station		*/
/*		Management library.									*/
/*													*/
/*		List of the enum:									*/
/*		    - BandList;										*/
/*		    - BrowseProductFlag;								*/
/*		    - EphemType;									*/
/*		    - Flag;										*/
/*		    - FormatType;									*/
/*		    - HighDensityMedia;									*/
/*		    - JDtime;										*/
/*		    - LatSign;										*/
/*		    - LonSign;										*/
/*		    - LockOrderArchive;									*/
/*		    - Media;										*/
/*		    - MoeValidation;									*/
/*		    - MsggetMode;									*/
/*		    - OpenMode;										*/
/*		    - OrderMode;									*/
/*		    - ProcMode;										*/
/*		    - ProdClassification;								*/
/*		    - Production;									*/
/*		    - ProductResolution;								*/
/*		    - ProdLevel;									*/
/*		    - RadCal;										*/
/*		    - Request;										*/
/*		    - RsmplAlg;										*/
/*		    - SatClassification;								*/
/*		    - SceneDim;										*/
/*		    - SensorClassification;								*/
/*		    - SortMode;										*/
/*		    - StationClassification;								*/
/*		    - StatusClassification;								*/
/*		    - TraceMode;									*/
/*													*/
/*		List of the structures:									*/
/*		    - Date;										*/
/*		    - DTOrder;										*/
/*		    - EventMsg;										*/
/*		    - Fdr;										*/
/*		    - FROrderAux;									*/
/*		    - FROrder;										*/
/*		    - Ftdcf;										*/
/*		    - FullResSemaphore;									*/
/*		    - GrStat;										*/
/*		    - LineTime;										*/
/*		    - Moe;										*/
/*		    - MoeDbHdr;										*/
/*		    - OrbElem;										*/
/*		    - OrderFileHdr;									*/
/*		    - QLOrder;										*/
/*		    - QueueMsg;										*/
/*		    - RawVideoHdr;									*/
/*		    - SameDateRec;									*/
/*		    - SensorInfo;									*/
/*		    - UT;										*/
/*		    - UTtime.										*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _smTypes_h_
#define _smTypes_h_
 
/*----------------------------------------------------------------------------------*
*	SCCS identifier								    *
*-----------------------------------------------------------------------------------*/ 
/*static char* smTypesId="@(#) A.C.S. SATELLITE Library: smTypes.h %I% %E%";*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Definitions.											! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef NULL
#define				NULL		    0		/* least significant half byte			    */
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Includes C header files.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#include 			<time.h>			/* usr:  time header file			    */	


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Includes C header files.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#include 			<smConstants.h>   		/* usr:  Constants header  */
#include 			<libql.h>
#include 			<smTypes_Structures.h>


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Includes C++ header files.									! */
/* +----------------------------------------------------------------------------------------------------+ */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   MACROS.												! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef __MAX
#define __MAX
#define	MIN(a, b)			((a) < (b) ? (a) : (b))
#define	MAX(a, b)			((a) > (b) ? (a) : (b))
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   TYPEDEFS.											! */
/* +----------------------------------------------------------------------------------------------------+ */
/* +----------------------------------------------------------------------------------------------------+ */

#if ZORRO
// A.B. - 18-07-2007 - This enums are re-defined in ../sgiDTIncludes/smTypes_Structures.h

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: EphemPropagation.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum EphemPropagation__
{
    PropagateByStateVect	= 0,				/* default means BME received from MOC	*/
    PropagateByMoe		= 1, 				/* propagated/predicted from PCDs	*/
    PropagateByMOC		= 2				/* propagated/restituted by MOC		*/
} EphemPropagation;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: EphemType.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum EphemType__
{
    Determined			= 0,				/* determined ephemeris				    */
    Predicted			= 1,				/* predicted  ephemeris				    */
    TimeError			= 2				/* time error					    */
} EphemType;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: FilterFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum Flag__
{
    Reset			= 0,				/* not set					    */
    Set				= 1				/* set						    */
} Flag;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: FormatType.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum FormatType__
{
    CEOS			= 0,				/* CEOS	format type				    */
    FASTFORMAT			= 1,				/* Fast Format type				    */
    MSSFORMAT			= 2, 				/* Mss Format type				    */
    HDFFORMAT			= 3,				/* HDF file format 				    */
    JPEGFORMAT			= 4,				/* JPEG file format 				    */
    TIFFFORMAT			= 5				/* TIFF file format 				    */
} FormatType;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: HighDensityMedia.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum HighDensityMedia__
{
    AllMedia			= 0,				/* no special selection for media		    */
    CreoMedia			= 1,				/* Creo optical tape				    */
    SonyMedia			= 2,				/* Sony DIR-1000				    */
    HddrMedia			= 3, 				/* Hddr						    */
    DLTMedia			= 4, 				/* DLT						    */
    DATMedia			= 5, 				/* Digital Audio Tape				    */
    ExabyteMedia		= 6, 				/* Exabyte					    */
    CdRomMedia			= 7,  				/* CD ROM					    */
    CCTMedia			= 8, 				/* Computer Compatible Tape			    */
    FilmMedia			= 9, 				/* Film Record					    */
    DiskMedia			= 10,  				/* Disk file					    */
    DvdRomMedia			= 11  				/* DVD ROM					    */
} HighDensityMedia;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: JDtime.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef double			JDtime;				/* Julian Date time				    */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: LatType.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum LatSign__
{
    NorthLat			= 0,				/* norther 					    */
    SouthLat			= 1 				/* southern					    */
} LatSign;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: LonType.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum LonSign__
{
    EastLon			= 0,				/* eastern 					    */
    WestLon			= 1 				/* western					    */
} LonSign;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: LockOrderArchive.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum LockOrderArchive__
{
    Green			= 0,				/* file unlocked				    */
    FullResOrderHandling	= 1,				/* file locked by FROrderHandling task		    */
    FullResMonitor		= 2,				/* file locked by FRMonitor task		    */
    Prm				= 3 				/* file locked by PRM task			    */
} LockOrderArchive;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: Media.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum Media__
{
    CCT6250			= 0,				/* output on CCT density 6250 bpi		    */
    DISK			= 1,				/* output on disk				    */
    EXABYTE8200			= 2,				/* output on exabyte (8200 DENSITY)		    */
    EXABYTE8500			= 3,				/* output on exabyte (8500 DENSITY)		    */
    NONE			= 4,				/* no output					    */
    CCT1600			= 5, 				/* output on CCT density 1600 bpi		    */
    DAT				= 6, 				/* output on Digital Audio Tape			    */
    CDROM			= 7, 				/* output on CD-ROM				    */
    DLT				= 8,  				/* output on DLT				    */
    FILM			= 9,  				/* output on Film Record			    */
    ELECTRONIC_TRANSFER		= 10,  				/* output on file to allow electronic transfer	    */
    DVDROM			= 11				/* output on DVD-ROM				    */
} Media;

static char     *MediaTypeString[] = {
"CCT6250",
"DISK",
"EXABYTE",
"EXABYTE",
"NONE",
"CCT1600",
"DAT",
"CDROM",
"DLT",
"FILM",
"ELECTRONIC TRANSFER", 
"DVDROM"
};

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: MoeValidation.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum MoeValidation__
{
    MoeNotChecked		= 0,				/* the Moe data has not been checked		    */
    MoeValid			= 1,				/* the Moe data is valid			    */
    MoeForcedValid		= 2, 				/* the Moe data is forced valid from the operator   */
    MoeNotValid			= 3 				/* the Moe data is not valid			    */
} MoeValidation;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: MsggetMode.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum MsggetMode__
{
    MsgMaster			= 0,				/* msgget with	    IPC_CREAT privilege		    */
    MsgSlave			= 1				/* msgget without   IPC_CREAT privilege		    */
} MsggetMode;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: OpenMode.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum OpenMode__
{
    ReadExistingFile		= 0,				/* an existing file is opened			    */
    CreateNewFile		= 1				/* a new file is created on open		    */
} OpenMode;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: OrderMode.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum OrderMode__
{
    OrderbyLatLon		= 0,				/* order issued by (lat, lon)			    */
    OrderbyPathRow		= 1, 				/* order issued by (path, row)			    */
    OrderbyStartStopTime	= 2				/* order issued by (path, sart+stop time) only if LOW */
} OrderMode;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: ProcMode.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum ProcMode__
{
    Chain			= 0,				/* chain: tasks are activated by the scheduler	    */
    StandAlone			= 1				/* stand alone: each task is activated separatelly  */
} ProcMode;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: ProdClassification.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum ProdClassification__
{
    FullRes			= 1,				/* full resolution production			    */
    QuickLook			= 2,				/* quick look production			    */
    DataTranscription		= 3				/* data transcription production		    */
} ProdClassification;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: ProdLevel.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum ProdLevel__
{
    Level0			= 0,				/* level 0 (uncorrected)			    */
    Level1			= 1,				/* level 1 (radiometric corrected)		    */
    Level2			= 2,				/* level 2 (geometric and radiometric corrected OPS)*/
    Level3			= 3,				/* level 3 (tbd)				    */
    Level4			= 4,				/* level 4 (geometric and radiometric corrected TM) */
    Level5			= 5, 				/* level 5 (geocoded TM)			    */
    Level6			= 6, 				/* level 6 (geocoded TM + GCPs)			    */
    Level7			= 7				/* level 7 (terrain corrected)                      */
/* Not used in TPML project */
/*     PublicRelation		= 7, 				 Pub_Rel (level 4 + undersampling)		    */
/*     Quicklook		= 8,				 Quiklook(catalogue quicklook)		    */
} ProdLevel;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: QLType.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum QLType__
{
    DuringPass			= 0,				/* real time quick look (during the pass)	    */
    PostPass			= 1				/* post pass quick look				    */
} QLType;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: Production.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum Production__
{
    ProdOk			= 0,				/* production is Ok				    */
    ProdError			= 1				/* production halted in error			    */
} Production;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: RadCal.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum RadCal__
{
    NOCAL			= 0,				/* no calibration 				    */
    PREFLIGHT			= 1,				/* preflight calibration			    */
    ONBOARD			= 2,				/* on board calibration				    */
    STATISTICAL			= 3, 				/* statistical calibration			    */
    PREFLIGHT_ATMCOR		= 4,				/* preflight calibration and atmospheric correction applied */
    ONBOARD_ATMCOR		= 5,				/* on board calibration and atmospheric correction applied */
    GAIN_LIFE_TIME		= 6
} RadCal;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: Request.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum Request__
{
    NOTREQUESTED		= 0,				/* not requested				    */
    REQUESTED			= 1,				/* requested					    */
    NOTUSED			= 2				/* not used					    */
} Request;




/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: RsmplAlg.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum RsmplAlg__
{
    NORSMPL			= 0,				/* not requested				    */
    NEARNEIGHBOR		= 1,				/* requested					    */
    LINEARINTERP		= 2,				/* requested					    */
    CUBICINTERP			= 3				/* requested					    */
} RsmplAlg;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: SatClassification.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum SatClassification__
{
    AllSat			= 0,				/* All satellite				    */
    SatLandsat			= 1,				/* Landsat					    */
    SatMos			= 2,				/* Mos						    */
    SatJers			= 3,				/* Jers						    */
    SatSpot			= 4,				/* Spot						    */
    SatErs			= 5				/* Ers						    */
} SatClassification;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Typedef: SceneDim.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum SceneDim__
{
    TOPLEFT		= '1',				/* ascii(49) top left quadrant			    */ 
    TOPRIGHT		= '2',				/* ascii(50) top right quadrant			    */
    BOTLEFT		= '3',				/* ascii(51) bottom left quadrant		    */
    BOTRIGHT		= '4',				/* ascii(52) bottom right quadrant		    */
    FULLSCENE		= 'F',				/* ascii(70) full scene				    */
    MINISCENE		= 'M',				/* ascii(77) mini scene (50 km x 50 km)		    */
    FLOATSCENE		= 'E',				/* ascii(69) floating full scene		    */ 
    QUARTERSCENE	= 'Q', 				/* ascii(81) quarter scene			    */ 
    MICROSCENE		= 'O',				/* ascii(??) mini scene (25 km x 25 km)		    */
    STRIP		= 'S',				/* ascii(??) strip				    */ 
    FULLPASS		= 'P'				/* ascii(??) full pass				    */ 
} SceneDim;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: SensorClassification.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum SensorClassification__
{
    SensorMss			= 1,				/* Mss						    */
    SensorTm			= 2,				/* Tm						    */
    SensorRbv			= 4,				/* Rbv						    */
    SensorMessr			= 5,				/* Messr					    */
    SensorVnir			= 6,				/* Vnir						    */
    SensorSwir			= 7,				/* Swir						    */
    SensorErs_Sar		= 11,				/* Ers_Sar					    */
    SensorAtsr			= 12,				/* Atsr						    */
    SensorEtm			= 3,				/* Etm						    */
    SensorSwirVnir		= 8,				/* Swir and Vnir				    */
    SensorPan			= 9,				/* Swir and Vnir				    */
    SensorXs			= 10				/* Swir and Vnir				    */
} SensorClassification;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: SortMode.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum SortMode__
{
    SortbyOrdNum		= 0,				/* sorting mode by order number			    */
    SortbyPriority		= 1,				/* sorting mode by priority			    */
    SortbyStatus		= 2,				/* sorting mode by status			    */
    SortbyPathDate		= 3,				/* sorting mode by path date			    */
    SortbyPathNumber		= 4				/* sorting mode by path number			    */
} SortMode;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: Station.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum StationClassification__
{
    NoStation			= 0,				/* station not defined				    */
    FucinoStation		= 1,				/* Fucino station				    */
    KirunaStation		= 2,				/* Kiruna station				    */
    MasPalomasStation		= 3,  				/* MasPalomas station				    */
    CotopaxiStation		= 8,  				/* Cotopaxi station				    */
    HartBeesHoekStation		= 19,  				/* Hartbeeshoek station				    */
    CordobaStation		= 103,  			/* Cordoba station				    */
    MalindiStation		= 109 				/* Malindi station				    */
} StationClassification;


#ifndef ACS_StatusClassification_DEFINED
#define ACS_StatusClassification_DEFINED 1
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: StatusClassification.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum StatusClassification__
{
    Aborted			= 1,				/* order aborted				    */
    Done			= 2,				/* order completed				    */
    Eligible			= 3,				/* order eligible				    */
    Failed			= 4, 				/* order failed					    */
    Scheduled			= 5				/* order scheduled				    */
} StatusClassification;
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: TraceMode.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum TraceMode__
{
    NoTrace			= 0,				/* short wave infrared radiometer		    */
    StdoutTrace			= 1,				/* visible near infrared radiometer		    */
    FileTrace			= 2				/* visible near infrared radiometer		    */
} TraceMode;

#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   STRUCTURES.											! */
/* +----------------------------------------------------------------------------------------------------+ */
/* +----------------------------------------------------------------------------------------------------+ */


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: EventMsg.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct EventMsg__
{
    char			MsgDate[24];			/* date of the message				    */
    char			Type[2];			/* type of message				    */
    short			OrderNumber;			/* production order number			    */
    char			TaskName[24];			/* name of the program that send the message	    */
    char			Message[80];			/* message sent to the message queue		    */
    SatClassification		SatId;				/* satellite identification			    */
    int				MissionId;			/* mission identification			    */
    SensorClassification	SensorId;			/* sensor identification			    */
    ProdClassification		ProdId;				/* production identifcation FR or QL		    */
} EventMsg;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Date.											! */
/* +----------------------------------------------------------------------------------------------------+ */

#if ZORRO
// A.B. - 18-07-2007 - This structure is re-defined in ../sgiDTIncludes/smTypes_Structures.h

typedef struct Date__
{
    int				Year;				/* year						    */
    int				Month;				/* month					    */
    int				Day;				/* day						    */
} Date;

#endif


#if ZORRO
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Ftdcf.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef THIS_IS_DEFINED_IN_QL_H
typedef struct Ftdcf__
{
    int32_t			MagicNumber;			/* file magic number				    */
    int32_t			OrderNumber;			/* order number					    */
    int32_t			Station;			/* station identification field			    */
    int32_t			Satellite;			/* satellite identification field		    */
    int32_t			Mission;			/* mission number				    */
    int32_t			Orbit;				/* orbit number					    */
    int32_t			Track;				/* track number					    */
    int32_t			Frame;				/* frame number					    */
    int32_t			WrsPath;			/* WRS path number				    */
    int32_t			UTM_Zone;			/* UTM zone number				    */
    int32_t			Center_Time[7];			/* GMT frame center time			    */
    double			datej;				/* Julian date of frame center time		    */
    double			GHA;				/* Greenwich Hour Angle				    */
    double			Osculating[6];			/* Keplerian Osculanting Elements (Sma / EarthRadius
												   Eccentricity 
												   Inclination  [rad]
												   ArgOfPerigee [rad]
												   AscNodeLon	[rad]
												   MeanAnomaly  [rad]) */
    double			Pos[3];				/* Position vector				    */
    double			Vel[3];				/* Velocity vector				    */
    double			Radius;				/* Satellite Orbit Radius			    */
    double			Height;				/* Satellite Height at Center Frame		    */
    double			Heightdot;			/* Time derivative of S/C Height at Center Frame    */
    double			Latitude;			/* Frame Center Latitude [rad]			    */
    double			Longitude;			/* Frame Center Longitude [rad]			    */
    double			LatitudeDeg;			/* Frame Center Latitude [deg]			    */
    double			LongitudeDeg;			/* Frame Center Longitude [deg]			    */
    double			Ant_Elevation;			/* Antenna Elevation at Center Frame		    */
    double			Ant_Azimuth;			/* Antenna Azimuth at Center Frame		    */
    double			Sun_Elevation;			/* Sun Elevation at Center Frame		    */
    double			Sun_Azimuth;			/* Sun Azimuth at Center Frame			    */
    double			Skew_Angle;			/* Skew Angle at Center Frame			    */
    double			Tang_Skew;			/* Tangent of Skew Angle			    */
    double			Skew_Angle_dot;			/* Time derivative of Skew Angle		    */
    double			Heading;			/* Satellite Heading [ Radians ]		    */
    double			Velocity;			/* Nadir Satellite Velocity Km/Sec		    */
    double			Velocity_Rate;			/* Velocity Change Rate Km/Sec/Sec		    */
    double			UTM_Northing;			/* UTM Norting of Center Frame			    */
    double			UTM_Easting;			/* UTM Easting of Center Frame			    */
    double			theta;				/* Orientation to UTM grid [deg]		    */
    int32_t			DescendingFlag;			/* 1 = descending, 0 = ascending		    */
} Ftdcf;
#endif

#endif

/* +------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: UTtime.									    ! */
/* +------------------------------------------------------------------------------------------------+ */
typedef struct UTtime__
{
    int32_t			year;				/* year	    YY	*/
    int32_t			month;				/* month    MM	*/
    int32_t			day;				/* day	    DD	*/
    int32_t			hour;				/* hour	    hh	*/
    int32_t			min;				/* min	    mm	*/
    int32_t			sec;				/* sec	    ss	*/
    int32_t			msec;				/* msec	    ttt	*/
} UTtime;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Moe.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct Moe__
{
    UTtime			UtTime;				/* time				    (UT)	*/
    JDtime			JdTime;				/* time			    (Julian date)	*/

    EphemPropagation		PropagationFlag;		/* Propagation Flag				*/
    MoeValidation		SetDataValid;			/* validation of data set   			*/

    double			SemiMajorAxis;			/* semimajor axis of orbit	    (Km)	*/
    double			Eccentricity;			/* eccentricity			    (deg)	*/
    double			Inclination;			/* inclination			    (deg)	*/
    double			LongAscNode;			/* longitude of the ascending node  (deg)	*/
    double			RateLongAscNode;		/* longitude of the asc. node rate  (deg/day)	*/
    double			ArgPerigee;			/* argument of perigee		    (deg)	*/
    double			RateArgPerigee;			/* argument of perigee rate	    (deg/day)	*/
    double			MeanAnomaly;			/* mean anomaly			    (deg)	*/
    
    double			Period;				/* actual orbital period	    (min)	*/

    double			PerigeeHeight;			/* height of perigee		    (Km)	*/
    double			ApogeeHeight;			/* height of apogee		    (Km)	*/

    double			PerigeeSpeed;			/* velocity at the perigee	    (Km/sec)	*/
    double			ApogeeSpeed;			/* velocity at the apogee	    (Km/sec)	*/
    
    double			PerigeeLatitude;		/* perigee latitude		    (deg.dec)	*/
    double			EastLongitude;			/* east longitude		    (deg.dec)	*/
    double			GeodeticLat;			/* geodetic latitude		    (deg.dec)	*/
    
    double			HeightSat;			/* height of satellite at ...	    (Km)	*/
    
    double			x;				/* ECITOD x coord		    (Km)	*/
    double			y;				/* ECITOD y coord		    (Km)	*/
    double			z;				/* ECITOD z coord		    (Km)	*/
    
    double			vx;				/* ECITOD vx coord		    (Km/sec)	*/
    double			vy;				/* ECITOD vy coord		    (Km/sec)	*/
    double			vz;				/* ECITOD vz coord		    (Km/sec)	*/
} Moe;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: MoeDbHdr.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct MoeDbHdr__
{
    int				MagicNum;			/* magic number					*/
    int				NumOfRec;			/* number of records inside the file		*/
} MoeDbHdr;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: OrbElem.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct OrbElem__
{
    double			SemiMajorAxis;			/* semimajor axis		    (Earth radius)  */
    double			Eccentricity;			/* eccentricity			    (rad)	    */
    double			Inclination;			/* inclination			    (rad)	    */
    double			ArgPerigee;			/* argument of perigee		    (rad)	    */
    double			LongAscNode;			/* longitude of the ascending node  (rad)	    */
    double			MeanAnomaly;			/* mean anomaly			    (rad)	    */
} OrbElem;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: OrderFileHdr.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct OrderFileHdr__
{
    int				MagicNumber;			/* file magic number				    */
    int				NumOfRec;			/* number of records in the file		    */
    int				LastAssignedOrderNum;		/* last assigned order sequence number		    */
} OrderFileHdr;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: QueueMsg.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct QueueMsg__
{
    int32_t			Type;				/* type of message				    */
    EventMsg			MsgData;			/* message data					    */
} QueueMsg;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: QLOrder.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct QLOrder__   
{
    int				MagicNumber;			/* file magic number				    */
    int				OrderNumber;			/* order sequence number			    */
    SatClassification		SatId;				/* satellite identificator			    */
    int				MissionNum;			/* mission number				    */
    int				Priority;			/* order priority				    */
    StatusClassification	StatusId;			/* order status	identificator			    */
    SensorClassification	SensorId;			/* sensor identificator				    */
    int				PathNumber;			/* scene path number				    */
    Date			PathDate;			/* scene path date				    */
    int				HDDRNumber;			/* High Density Digital Recorder Number		    */
    int				HDDTNumber;			/* High Density Digital Tape Number		    */
    int				HDDTPassStartMeters;		/* Pass Start on the Hddt [m]			    */
    TraceMode			Trace;				/* Program Trace mode				    */
    int				BandSelectionForRed;		/* Band number to be assigned to the red   channel  */
    int				BandSelectionForGreen;		/* Band number to be assigned to the green channel  */
    int				BandSelectionForBlue;		/* Band number to be assigned to the blue  channel  */
    StationClassification	AcquisitionStation;		/* QL Acquisition station			    */	
    StationClassification	ProcessingStation;		/* QL Processing station			    */
    QLType			QLTypeData;			/* QL type (during/post pass)			    */	
} QLOrder;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: SensorInfo.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#if ZORRO
// A.B. - 18-07-2007 - This structure is re-defined in ../sgiDTIncludes/smTypes_Structures.h

typedef struct SensorInfo__   
{
    SensorClassification	SensorId;			/* sensor identificator			    */
    Request			RequestedSensor;		/* requesting sensor flag		    */
    int				NumOfBands;			/* bands number of the sensor		    */
    Request			BandRequest[BANDS_NUMBER];	/* requested array bands (WARNING: not always completely used) */
    ProdLevel			ProdLevelData;			/* production level correction		    */
    RadCal			RadCalData;			/* kind of radiometric calibration	    */
    RsmplAlg			RsmplAlgData;			/* kind of applying resampling algorithm    */ 
} SensorInfo;
#endif


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: BrowseProductFlag.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum BrowseProductFlag__ {
    BANDSELECTION    	= 0,	    /* flag specifying to get user defined band combination */
    CLOUDASSQUICKLOOK	= 1	    /* flag specifying to get the cloud assessment band combination */
} BrowseProductFlag;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: ProductResolution.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum ProductResolution__ {
    FULL_RESOLUTION    	= 0,
    MED_RESOLUTION	= 1,
    LOW_RESOLUTION	= 2
} ProductResolution;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Enum: BandList. 1 relative  !!!! ATTENTION this enum is duplicated in the LandAuxMetadata.h	! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef enum {
    Band1 	= 1,
    Band2 	= 2,
    Band3 	= 3,
    Band4 	= 4,
    Band5 	= 5,
    Band6L 	= 6,
    Band6H 	= 9,
    Band7 	= 7,
    BandPan 	= 8
} BandList;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: FROrderAux.										! */
/* !   This structure contain data added from Landsat 7 mission to contain data used in MDPS system	! */
/* !   This structure is "long" (8 byte) aligned (and int = 4 byte)					! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct FROrderAux__   
{
    int				MagicNumber;			/* file magic number of the Aux file				*/
    int				OrderNumber;			/* order sequence number (link to the Order Database)		*/
    ProductResolution		ProdResolution;			/* Product Resolution (Full Med or Low resolution)		*/
    int				DeliveryUnitId;			/* PROMISE 							*/
    int				ProductOrderItemId;		/* PROMISE 							*/
    char			BandSequence[3];		/* Band sequence in case of TREE_BANDS list 			*/
    char			Spare1[9];			/* spare to array to the next field 				*/
 
    double			AcqStartDateTime;		/* Start time used for Strip products 				*/
    double			AcqEndDateTime;			/* End time used for Strip product 				*/
    BrowseProductFlag 		BrowseFlag;			/* flag that specify if Low product is a combination...
   								   ...of band (0) or the standard Catalogue Quicklook (1)	*/
    char			PromiseFileName[39];		/* File name of the PROMISE order (empy if inserted by other 
    								   sources) BEWARE!!! it is null terminated 			*/
    char			UserOrderId[26];		/* PROMISE							*/
    char			AlignSpare1[3];			/* spare to align to long					*/
} FROrderAux;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: FROrder.										! */
/* +----------------------------------------------------------------------------------------------------+ */
#if ZORRO
// A.B. - 18-07-2007 - This structure is re-defined in ../sgiDTIncludes/smTypes_Structures.h

typedef struct FROrder__   
{
    int				MagicNumber;			/* file magic number				    */
    int				OrderNumber;			/* order sequence number			    */
    int				MissionNum;			/* mission number				    */
    SatClassification		SatId;				/* satellite identificator			    */
    int				Priority;			/* order priority				    */
    StatusClassification	StatusId;			/* order status	identificator			    */
    OrderMode			OrderModeFlag;			/* flag for (path, row) or (lat, lon)		    */

    int				InputHighDensityMediaUnitNumber;/* High Density Digital Recorder Number		    */
    int				InputHighDensityMediaNumber;	/* High Density Digital Tape Number		    */

    Date			PathDate;			/* scene path date				    */
    int				PathNumber;			/* scene path					    */
    int				RowNumber;			/* scene row					    */
    double			LatDeg;				/* latitude (deg)				    */    
    double			LonDeg;				/* longitude (deg)				    */    

    Media			MediaType;			/* output media type				    */
    FormatType			FormatArrangement;		/* tape format arrangement			    */
    SensorInfo			SensorArray[NUM_OF_SENSORS];	/* processing info associated to the sat sensors    */

    int				CopyNum;			/* number of requested copies			    */    
    TraceMode			Trace;				/* program trace mode				    */
    
    SceneDim			RequestedSceneDim;		/* requested scene dimension (full, quarter, ...)   */			
    
    char			AcqStationName[7];		/* acquisition station name			    */
    char			AcqStationCode[3];		/* acquisition station code			    */
    char			ProdStationName[7];		/* production  station name			    */
    char			ProdStationCode[3];		/* production  station code			    */


    LatSign			LatSignData;			/* latitude type				    */
    int				LatitudeDeg;			/* latitude degrees value			    */
    int				LatitudeMin;			/* latitude minutes value			    */
    int				LatitudeSec;			/* latitude seconds value			    */

    LonSign			LonSignData;			/* longitude type				    */
    int				LongitudeDeg;			/* longitude degrees value			    */
    int				LongitudeMin;			/* longitude minutes value			    */
    int				LongitudeSec;			/* longitude seconds value			    */

    int				PathDayOfYear;			/* scene path day of year			    */
    
    HighDensityMedia		InputHighDensityMediaType;	/* input high density media			    */
    char			EsaOrderCode[12];		/* ESA Order Code				    */
    int				EsaSubOrder;			/* ESA Sub Order    				    */
    int				FirstAddress;			/* Segment Pass Address				    */
    int				SecondAddress;			/* Segment Header Address			    */
    char			Projection[25];			/* map projection				    */
    char			Ellipsoid[25];			/* ellispoid type				    */
    int				ProjectionZone;			/* Projection Zone				    */
} FROrder;

#endif
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: FullResSemaphore.									! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct FullResSemaphore__   
{
    int32_t			MagicNumber;			/* file magic number				    */
    LockOrderArchive		LockOrderArchiveValue;		/* lock order value				    */
    char			LockingTask[20];		/* locking task name				    */
} FullResSemaphore;



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: DTOrder.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DTOrder__   
{
    int				MagicNumber;			/* file magic number				    */
    int				OrderNumber;			/* order sequence number			    */
    SatClassification		SatId;				/* satellite identificator			    */
    int				MissionNum;			/* mission number				    */
    int				Priority;			/* order priority				    */
    StatusClassification	StatusId;			/* order status					    */
    SensorClassification	SensorId;			/* sensor identificator				    */
    int				PathNumber;			/* scene path number				    */
    Date			PathDate;			/* scene path date				    */
    int				HDDRNumber;			/* High Density Digital Recorder Number		    */
    int				HDDTNumber;			/* High Density Digital Tape Number		    */
    TraceMode			Trace;				/* program trace mode				    */
    StationClassification	AcquisitionStation;		/* QL Acquisition station			    */	
    StationClassification	ProcessingStation;		/* QL Processing station			    */
    HighDensityMedia		HighDensityMediaType;		/* media output					    */
    int				HighDensityUnitNumberForTrascr;	/* media output number				    */
} DTOrder;


#if ZORRO
// A.B. - 18-07-2007 - This structure is re-defined in ../sgiDTIncludes/smTypes_Structures.h

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: RawVideoHdr.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct RawVideoHdr__
{
    int				MagicNum;			/* magic number					    */
    int				OrderNumber;			/* order number					    */
    int				StartScanLine;			/* start scan line				    */
    int				StopScanLine;			/* stop  scan line				    */
    int				Shift;				/* VNIR line corresponding to the 1st SWIR line	    */
} RawVideoHdr;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: LineTime.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct LineTime__
{
    int				DayOfYear;			/* day of the year DDD				    */
    double			MillisecOfDay;			/* milliseconds of the day			    */
} LineTime;

#endif



/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: SameDateRec.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct SameDateRec_
{
    int				NumberOfRec;			/* number of records				    */
    Moe				*pMoeData;			/* pointer to a Moe structure			    */
} SameDateRec;


#if ZORRO

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: UT.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct UT__
{
    int32_t			year;				/* year	    YY					    */
    int32_t			month;				/* month    MM					    */
    int32_t			day;				/* day	    DD					    */
    int32_t			hour;				/* hour	    hh					    */
    int32_t			min;				/* min	    mm					    */
    int32_t			sec;				/* sec	    ss					    */
    int32_t			msec;				/* msec	    ttt					    */
    int32_t			yday;
} UT;

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Fdr.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct Fdr__
{
    int32_t			MagicNumber;			/* magic number that lets the file to be recognised	*/
    int32_t			RecordLength;			/* record length					*/	
    int				ImgDataOffset;			/* image data offset					*/
    int				ImgDataLength;			/* image data length					*/
    char			FileName[32];			/* file name						*/
    char			SatelliteName[32];		/* satellite name					*/
    int32_t			MissionNumber;			/* satellite mission number				*/

    SensorClassification	SensorType;			/* satellite sensor type				*/

    int32_t			BandNumber;			/* band number						*/
    int32_t			ProcessingLevel;		/* image processing level				*/

    RsmplAlg			ResamplingAlgorithm;		/* image resampling algorithm				*/
    RadCal			RadiometricCalibration;		/* kind of radiometric calibration			*/

    float			HorizontalGeoResolution;	/* horizontal geometric resolution			*/
    float			VerticalGeoResolution;		/* vertical   geometric resolution			*/

    int32_t			PixelNumber;			/* number of pixel per line				*/
    int32_t			LineNumber;			/* number of line per image				*/

    int32_t			PathNumber;			/* orbit path number					*/    
    int32_t			RowNumber;			/* orbit row number 					*/   

    UT				SceneCentreDateTime;		/* scene centre date and time				*/
    LineTime			FirstLineTime;			/* time of the first line				*/
    float			InputSceneCentreLatRad;		/* input scene centre latitude  [rad]			*/
    float			InputSceneCentreLonRad;		/* input scene centre longitude [rad]			*/
    float			InputSceneCentreLatDeg;		/* input scene centre latitude  [deg]			*/
    float			InputSceneCentreLonDeg;		/* input scene centre longitude [deg]			*/
    float			InputSceneCentrePixelNum;	/* input scene centre pixel number			*/
    float			InputSceneCentreLineNum;	/* input scene centre line number			*/
    int32_t			SceneCentreTimeOffset;		/* scene centre time offset from WRS			*/
    float			ProcessedSceneCentreLatRad;	/* processed scene centre latitude  [rad]		*/
    float			ProcessedSceneCentreLonRad;	/* processed scene centre longitude [rad]		*/
    float			ProcessedSceneCentreLatDeg;	/* processed scene centre latitude  [deg]		*/
    float			ProcessedSceneCentreLonDeg;	/* processed scene centre longitude [deg]		*/
    float			ProcessedSceneCentrePixelNum;	/* processed scene centre pixel number			*/
    float			ProcessedSceneCentreLineNum;	/* processed scene centre line number			*/

    char			AcquisitionStationName[32];	/* acquisition station name				*/    
    char			ProcessingStationName[32];	/* processing station name				*/    
    struct tm			ProductionDateTime;		/* production date and time				*/          

    float			NomInterPixelDistance;		/* nominal scale of inter-pixel distance at nadir [m]	*/
    float			NomInterLineDistance;		/* nominal scale of inter-line distance at nadir [m]	*/
    float			ImageSkewSceneCentre;		/* image skew at scene centre				*/

    float			NominalWRSNorthing;		/* nominal WRS northing of centre [m]			*/
    float			NominalWRSEasting;		/* nominal WRS easting of centre [m]			*/

    float			VerticalOffsetToWRSCentre;	/* vertical offset of scene centre to WRS nominal centre */
    float			HorizontalOffsetToWRSCentre;	/* horizontal offset of scene centre to WRS nominal centre */

    int				PixelsPerLineProcImgNum;	/* number of pixels per line of processed image */
    int				LinesPerProcImgNum;		/* number of lines per processed image		*/

    float			ProcessedInterPixelDistance;	/* processed scale of inter-pixel distance [m]		*/
    float			ProcessedInterLineDistance;	/* processed scale of inter-line distance [m]		*/
    float			ProcessedImgSceneCentreOrientation;	/* orientation of processed image centre [deg]	*/

    int				UTMProcessedImgZoneNum;		/* UTM zone number for processed image			*/

    float			NomSatelliteOrbitInclination;	/* nominal satellite orbital inclination		*/	
    float			NomAscendingNodeLon;		/* nominal ascending node longitude at equator [deg]	*/
    float			NomSatelliteAltitude;		/* nominal satellite altitude [m]			*/
    float			NomGroundSpeed;			/* nominal satellite ground speed [m / sec]		*/

    float			CrossTrackFieldOfView;		/* cross track field of view [deg]			*/	
    float			SensorScanRate;			/* sensor scan rate [scans / sec]			*/
    float			SensorSamplingRate;		/* sensor active sampling rate [samples / sec]		*/

    float			TopLeftCornerLatitude;		/* top left corner latitude [deg]			*/	
    float			TopLeftCornerLongitude;		/* top left corner longitude [deg]			*/
    float			TopLeftCornerNorthing;		/* top left corner northing of centre [m]		*/
    float			TopLeftCornerEasting;		/* top left corner easting of centre [m]		*/

    float			TopRightCornerLatitude;		/* top right corner latitude [deg]			*/
    float			TopRightCornerLongitude;	/* top right corner longitude [deg]			*/
    float			TopRightCornerNorthing;		/* top right corner northing of centre [m]		*/
    float			TopRightCornerEasting;		/* top right corner easting of centre [m]		*/

    float			BottomLeftCornerLatitude;	/* bottom left corner latitude [deg]			*/	
    float			BottomLeftCornerLongitude;	/* bottom left corner longitude [deg]			*/
    float			BottomLeftCornerNorthing;	/* bottom left corner northing of centre [m]		*/
    float			BottomLeftCornerEasting;	/* bottom left corner easting of centre [m]		*/

    float			BottomRightCornerLatitude;	/* bottom right corner latitude [deg]			*/
    float			BottomRightCornerLongitude;	/* bottom right corner longitude [deg]			*/
    float			BottomRightCornerNorthing;	/* bottom right corner northing of centre [m]		*/
    float			BottomRightCornerEasting;	/* bottom right corner easting of centre [m]		*/
} Fdr;

#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: GrStat.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef	struct GrStat__ 
{
    double  GrStatLatRad;		    /* ground station geodetic latitude (rad) */
    double  GrStatLonRad;		    /* ground station east longitude    (rad) */
    double  GrStatAlt;			    /* ground station altitude		(km)  */
    int	    GrStatElevAngNum;		    /* number of ground station elevation angles */
    double  *pGrStatElevAng;		    /* pointer to the vector of ground station elevation angles (rad) */ 
} GrStat;


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   End of code redefinition control.								! */
/* +----------------------------------------------------------------------------------------------------+ */
#endif
