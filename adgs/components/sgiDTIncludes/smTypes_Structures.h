#if 0

	Copyright 2007-2011, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. LANDSAT MDPS library $

	$Id$

	$Author$

	$Log: 
 
#endif

#include <smConstants.h>   		/* usr:  Constants header  */
#include <time.h>			/* sys:  time header */	

#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define _BL_ENDiAN_
#endif	
		
#ifndef __SMTYPES_STRUCTURES_H__
#define __SMTYPES_STRUCTURES_H__


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

static char const *MediaTypeString[] = {
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
/* Commented because not used in TPML */
/*    PublicRelation		= 7, 				 Pub_Rel (level 4 + undersampling)		  */
/*    Quicklook			= 8,				 Quiklook(catalogue quicklook)  		  */
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
    GAIN_LIFE_TIME		= 6,
	
	PREFLIGHT_NOB6 = 7,			/* No Thermal Band Correction */
	PREFLIGHT_ESTB6 = 8,		/* Thermal Band Correction (IC) for both gain and bias */
	PREFLIGHT_CPFB6 = 9,		/* Thermal Band Correction by CPF for both gain and bias */
	
	GAIN_LIFE_TIME_NOB6 = 11,	/* No Thermal Band Correction */
	GAIN_LIFE_TIME_ESTB6 = 12,	/* Thermal Band Correction (IC) for both gain and bias */
	GAIN_LIFE_TIME_CPFB6 = 13,	/* Thermal Band Correction by CPF for both gain and bias */
	
	PREFLIGHT_CPFGAINB6 = 14,	/* Thermal Band Correction CPF for gain and IC for bias */
	PREFLIGHT_CPFBIASB6 = 15,	/* Thermal Band Correction IC for gain and CPF for bias */
	
	GAIN_LIFE_TIME_CPFGAINB6 = 16,	/* Thermal Band Correction CPF for gain and IC for bias */
	GAIN_LIFE_TIME_CPFBIASB6 = 17,	/* Thermal Band Correction IC for gain and CPF for bias */
	
	MSS_CAL_WEDGE = 18,		/* Perform only MSS wedge calibration  */
	MSS_CAL_ABSOLUTE = 19,	/* Perform MSS absolute calibration  */
	
	GAIN_LIFE_TIME_CPFBIAS_NOB6 = 20,		/* Bias from CPF for reflective bands. No Thermal Band Correction */
	GAIN_LIFE_TIME_CPFBIAS_ESTB6 = 21,		/* Bias from CPF for reflective bands. Thermal Band Correction (IC) for both gain and bias */
	GAIN_LIFE_TIME_CPFBIAS_CPFB6 = 22,		/* Bias from CPF for reflective bands. Thermal Band Correction by CPF for both gain and bias */
	GAIN_LIFE_TIME_CPFBIAS_CPFGAINB6 = 23,	/* Bias from CPF for reflective bands. Thermal Band Correction CPF for gain and IC for bias */
	GAIN_LIFE_TIME_CPFBIAS_CPFBIASB6 = 24	/* Bias from CPF for reflective bands. Thermal Band Correction IC for gain and CPF for bias */
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




typedef struct					       
{						       
    int Year;				/* year  */    
    int Month;  			/* month */    
    int Day;				/* day   */    
} DATE; /* ex. Date */			       
    
#ifndef Date    
    typedef DATE Date; /* for backward compatibility */
#endif

typedef struct														     	 
{															     	 
    SensorClassification	SensorId;		   /* sensor identificator		       */		     	 
    Request			RequestedSensor;	   /* requesting sensor flag		       */		     	 
    int 			NumOfBands;		   /* bands number of the sensor	       */		     	 
    Request			BandRequest[BANDS_NUMBER]; /* requested array bands (WARNING: not always completely used) */ 	 
    ProdLevel			ProdLevelData;  	   /* production level correction	       */		     	 
    RadCal			RadCalData;		   /* kind of radiometric calibration	       */		     	 
    RsmplAlg			RsmplAlgData;		   /* kind of applying resampling algorithm    */		     	 
} SENSOR_INFO; /* ex. SensorInfo */											     	 

#ifndef SensorInfo    
    typedef SENSOR_INFO SensorInfo; /* for backward compatibility */
#endif
    
typedef struct  {														      
     int			 MagicNumber;			       /* file magic number				   */	      
     int			 OrderNumber;			       /* order sequence number 			   */	      
     int			 MissionNum;			       /* mission number				   */	      
     SatClassification  	 SatId; 			       /* satellite identificator			   */	      
     int			 Priority;			       /* order priority				   */	      
     StatusClassification	 StatusId;			       /* order status identificator			   */	      
     OrderMode  		 OrderModeFlag; 		       /* flag for (path, row) or (lat, lon)		   */	      
     int			 InputHighDensityMediaUnitNumber;      /* High Density Digital Recorder Number  	   */	      
     int			 InputHighDensityMediaNumber;	       /* High Density Digital Tape Number		   */	      
     DATE		 	 PathDate;			       /* scene path date				   */	      
     int			 PathNumber;			       /* scene path					   */	      
     int			 RowNumber;			       /* scene row					   */	      
     double			 LatDeg __attribute__ ((aligned(8)));  /* latitude (deg)				   */	   				     
     double			 LonDeg __attribute__ ((aligned(8)));  /* longitude (deg)				   */	   				     
     Media			 MediaType;			       /* output media type				   */	      
     FormatType 		 FormatArrangement;		       /* tape format arrangement			   */	      
     SENSOR_INFO		 SensorArray[NUM_OF_SENSORS];	       /* processing info associated to the sat sensors    */	      
     int			 CopyNum;			       /* number of requested copies			   */	      
     TraceMode  		 Trace; 			       /* program trace mode				   */	      
     SceneDim			 RequestedSceneDim;		       /* requested scene dimension (full, quarter, ...)   */	      
     char			 AcqStationName[7];		       /* acquisition station name			   */	      
     char			 AcqStationCode[3];		       /* acquisition station code			   */	      
     char			 ProdStationName[7];		       /* production  station name			   */	      
     char			 ProdStationCode[3];		       /* production  station code			   */	      
     LatSign			 LatSignData;			       /* latitude type 				   */	      
     int			 LatitudeDeg;			       /* latitude degrees value			   */	      
     int			 LatitudeMin;			       /* latitude minutes value			   */	      
     int			 LatitudeSec;			       /* latitude seconds value			   */	      
     LonSign			 LonSignData;			       /* longitude type				   */	      
     int			 LongitudeDeg;  		       /* longitude degrees value			   */	      
     int			 LongitudeMin;  		       /* longitude minutes value			   */	      
     int			 LongitudeSec;  		       /* longitude seconds value			   */	      
     int			 PathDayOfYear; 		       /* scene path day of year			   */	      
     HighDensityMedia		 InputHighDensityMediaType;	       /* input high density media			   */	      
     char			 EsaOrderCode[12];		       /* ESA Order Code				   */	      
     int			 EsaSubOrder;			       /* ESA Sub Order 				   */	      
     int			 FirstAddress;  		       /* Segment Pass Address  			   */	      
     int			 SecondAddress; 		       /* Segment Header Address			   */	      
     char			 Projection[25];		       /* map projection				   */	      
     char			 Ellipsoid[25]; 		       /* ellispoid type				   */	      
     int			 ProjectionZone;		       /* Projection Zone				   */	      
} FRORDER  ; /* ex. FROrder; */ 											      
    
#ifndef FROrder    
    typedef FRORDER FROrder; /* for backward compatibility */
#endif


typedef struct										          
{											          
    int MagicNum;			/* magic number 				    */    
    int OrderNumber;			/* order number 				    */    
    int StartScanLine;  		/* start scan line				    */    
    int StopScanLine;			/* stop  scan line				    */    
    int Shift;  			/* VNIR line corresponding to the 1st SWIR line     */    
} RAW_VIDEO_HDR; /* ex. RawVideoHdr */							          
											          
#ifndef RawVideoHdr    
    typedef RAW_VIDEO_HDR RawVideoHdr; /* for backward compatibility */
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: DummyBuffer.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct DummyBuffer_
{
    unsigned short		    Auxiliary_Buff[10];		/* auxiliary words			*/
    unsigned short		    Sync_Flag[4];		/* synchronization words from the FS	*/ 
} DUMMY_BUFFER;

#ifndef DummyBuffer    
    typedef DUMMY_BUFFER DummyBuffer; /* for backward compatibility */
#endif

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: LineTime.										! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct LineTime__
{
    int				DayOfYear;			/* day of the year DDD				    */
    double			MillisecOfDay __attribute__ ((aligned(8)));			/* milliseconds of the day			    */
} LINETIME;

#ifndef LineTime    
    typedef LINETIME LineTime; /* for backward compatibility */
#endif

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
} UTSW;

#ifndef UT    
    typedef UTSW UT; /* for backward compatibility */
#endif



// defined as the struct tm in sgi environment
/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: OLD_TM.											! */
/* +----------------------------------------------------------------------------------------------------+ */
typedef struct OLD_TM__
{
    int   tm_sec  ;   /* year	 YY    */
    int   tm_min  ;    /* month	 MM    */
    int   tm_hour ;   /* day	 DD    */
    int   tm_mday ;  /* hour	 hh    */
    int   tm_mon  ;   /* min	 mm    */
    int   tm_year ;   /* sec	 ss    */
    int   tm_wday ;  /* msec	 ttt   */
    int   tm_yday ;
    int   tm_isdst;
} OLD_TM;

typedef struct BASE_TM__
{
    int32_t   tm_sec  ;   /* year	 YY    */
    int32_t   tm_min  ;    /* month	 MM    */
    int32_t   tm_hour ;   /* day	 DD    */
    int32_t   tm_mday ;  /* hour	 hh    */
    int32_t   tm_mon  ;   /* min	 mm    */
    int32_t   tm_year ;   /* sec	 ss    */
    int32_t   tm_wday ;  /* msec	 ttt   */
    int32_t   tm_yday ; /* Days in year */
    int32_t   tm_isdst;
    int32_t   spare[2];
} BASE_TM;

/* +------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: OldFdr.									    ! */
/* +------------------------------------------------------------------------------------------------+ */
typedef struct OldFdr__
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
    OLD_TM			ProductionDateTime;		/* production date and time				*/          

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
} OLD_FDR;

/* +------------------------------------------------------------------------------------------------+ */	 
/* !   Structure: Fdr.									    ! */
/* +------------------------------------------------------------------------------------------------+ */
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
    LineTime        FirstLineTime;			/* time of the first line				*/
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
    BASE_TM			ProductionDateTime;		/* production date and time				*/          

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
} FDR;

#ifndef Fdr    
    typedef FDR Fdr; /* for backward compatibility */
#endif


#endif /* !__SMTYPES_STRUCTURES_H__ */
