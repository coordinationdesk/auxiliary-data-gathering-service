/*	This include file contains all the definitions for the description 
 *	of the full resolution video data file on optical tape, relative to the
 *	following satellites:
 *	NASA Landsat MSS and TM sensors,
 *	NASDA Marine Observation Satellite (MOS),
 *	NASDA Japanese Earth Resources Satellite (JERS),
 *	CNES Satellite Pour l'Observation de la Terre (SPOT).
 *	ESA Earth Remote Sensing Satellite (ERS-1,  ERS-2).
 */

#ifndef dtTypes_H
#define dtTypes_H


#ifdef __cplusplus
	extern "C" {
#endif

/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const* dtTypesId="@(#) Advanced Computer Systems: dtTypes.h               1.5 00/12/06 (include)";

#define	MIN_BLOCK_SIZE	    128		/* Minimum Block Size on Tape			    */
#define	MAX_BLOCK_SIZE	    8388608	/* Maximum Block Size on Tape			    */
#define	MAX_PASSES_ON_DLT   200		/* Max number of passes allowed on each DLT tape    */

/*
 *	Define the values for the Fields "Input Hddr Type"
 */
#define HDDR_AVAILABLES 9
#define	AMPEX			1			/* AMPEX recorder						*/
#define	SCHUMBERGER14	2			/* SCHUMBERGER 	recorder 14 tracks		*/
#define	SCHUMBERGER42	3			/* SCHUMBERGER 	recorder 42 tracks		*/
#define	PENNY_GILES		4			/* PENNY_GILES 	recorder 28 tracks		*/
#ifdef sgi
    /* Commented, since a warning is emitted on Linux (due to "-"). DECOMMENT IF NEEDED */
	#define	HD-96			5			/* Honeywell HD-96 recorder 28 tracks	*/
#endif
#define	AMPEX_DCRS		6			/* AMPEX DCRS	recorder				*/
#define	CREO_TAPE		7			/* Creo Tape	recorder				*/
#define	DIRECT_INGEST	8			/* Direct Ingestion						*/
#define	SONYDIR1000		9			/* SONY DIR 1000						*/
	static char const
		*Hddr_Type[] =
		{
			"Ampex_14",
			"Schlum_14",
			"Schlum_42",
			"Penny+Giles", 
			"HD-96", 
			"Ampex_DCRS", 
			"Creo_Tape", 
			"DirectIngest",
			"SONYDIR1000"
		};

/*
 *	Define the values for the Fields "Time Code Reader Type"
 */
#define TCR_AVAILABLES 3
#define	IRIG_A	1		    /* Irig A						*/
#define	IRIG_B	2		    /* Irig B						*/
#define	NASA_36	3		    /* NASA 36						*/

	static char const
		*Tcr_Type[] =
		{
			"IRIG_A",
			"IRIG_B",
			"NASA_36"
		};


/*
 *	Define the values for the Fields "Input/Output Media Type"
 */
#define	Optical_Tape	1		/* CREO Optical Tape				*/
#define	ID1_Cassette	2		/* Sony Dir1000 Cassette			*/
#define	Hddr_Tape		3		/* HDDR Tape						*/
#define	Dlt_Tape		4		/* DLT Tape							*/
#define	Dat_Tape		5		/* DAT Tape							*/
#define	Exa_Tape		6		/* Dlt Tape							*/
#define	Cdrom_Disk		7		/* CDROM Tape						*/
#define	Cct_Tape		8		/* CCT Tape							*/
#define	Film_Recorder	9		/* FILM Tape						*/

	static char const
		*Media_Type[] =
		{
			"Optical Tape",
			"ID1 Cassette",
			"Hddr Tape", 
			"Dlt Tape", 
			"Dat Tape", 
			"Exabyte", 
			"Cdrom", 
			"CCT",
			"Film Record"
		};

	static char const
		*Media_Id[] =
		{
			"CREO",
			"SONY",
			"HDDR", 
			"DLT", 
			"DAT", 
			"EXA", 
			"CDROM", 
			"CCT", 
			"FILM"
		};

/*
 *	Define the values for the Fields "Input Device Type (not for Transcription Types)"  11/2000 by AV
 */
#define INPUT_DEVS_AVAILABLES 4
#define	INDEV_IS_DEMOD		1		/* 			*/
#define	INDEV_IS_SONY		2		/* 			*/
#define	INDEV_IS_DLT		3		/* 			*/
#define	INDEV_IS_HDDR		4		/* 			*/
	static char const
		*Input_Type[] =
		{
			"unknown",
			"DEMOD",
			"SONY", 
			"DLT", 
			"HDDR"
		};


/*
 *	Define the values for the Fields "Output Device Type"
 */
#define	CREO		1		/* CREO Optical Tape Recorder			*/
#define	SONY		2		/* Sony Dir1000 Cassette Recorder		*/
#define	DLT4000		3		/* Digital Linear Cassette Recorder		*/
#define	DLT2000		4		/* Digital Linear Cassette Recorder		*/

	static char const
		*Output_Type[] =
		{
			"Creo OTR 1003",
			"Sony DIR 1000", 
			"Quantum DLT 4000", 
			"Quantum DLT 2000"
		};

/*
 *	Define the values for the Fields "Data_str_ID"
 */
#define	BIL		1		/* Band Interleaved structure			*/
#define	BSQ		2		/* Band Sequential structure			*/

/*
 *	Define the values for the Fields "Record_ID"
 */
#define	MAX_REC_TYP	    10		/* Max number of different record type in the file  */

#define	ADDR		1			/* Video data records addresses identifier	    */
#define	FTDCF		2			/* Centre frames times records identifier (D1-TM)   */
#define	TELEMETRY	3			/* Telemetry records identifier			    */
#define	SEGM_DESCR	4			/* Segment descriptor records identifier	    */
#define	VIDEO_DATA	5			/* Video data records identifier		    */
#define	STATE_VECT	6			/* State Vectors record identifier (ERS)	    */
#define	USER_HEADER	7			/* User header File				    */
#define	STATISTICS_FILE	8		/* Statistics File				    */
#define	PASS_ID  	9			/* Pass Id. File				    */

/*------------------------------------------------*
 *
 *	 _______________________________________
 *	|	        Pass Id.		|	    PASS_ID
 *	|    (Pass Identification Header)	|
 *	----------------------------------------- EOF
 *	|					|
 *	|	       Video Data		|	    VIDEO_DATA
 *	|	     (Sensor Data)		|
 *	|					|
 *	----------------------------------------- EOF
 *	|		User Header		|	    USER_HEADER
 *	----------------------------------------- EOF
 *	|	     Segment Descriptor		|	    SEGM_DESCR
 *	----------------------------------------- EOF
 *	|	        Orbit Data		|	    STATE_VECT ( ** OPTIONAL) -> for TM/D1 FTDCF
 *	----------------------------------------- EOF
 *	|	     Payload Correction		|	    TELEMETRY  ( ** OPTIONAL) .. PCM for JERS
 *	----------------------------------------- EOF
 *	|	     Block Addresses Des	|	    ADDR  
 *	----------------------------------------- EOF
 *
 *	----------------------------------------- 
 *	|	     Statistics			|	    STATISTICS_FILE  
 *	----------------------------------------- EOF
 *------------------------------------------------*/

	static char const
		*Record_ID_Files[] =
		{
			"unknown",			/*  0 */
			"DTBlock.dat",		/*  1 */
			"DTFtdcf",			/*  2 */
			"DTTelemetry.dat",	/*  3 */
			"DTSegment.dat",	/*  4 */
			"DTVideoData.dat",	/*  5 */ 
			"DTOrbitFile.dat",	/*  6 */
			"DTUserHeader.dat",	/*  7 */
			"DTStatisticFile.dat",	/*  8 */
			"DTPassId.dat",		/*  9 */
			"unknown"			/* 10 */
		};
/*
 *	Define the values for the field "Asc_flag"
 */
#define	ASCEND		1		/* Ascending node identifier		 	*/
#define	DESCEND		2		/* Descending node identifier		 	*/


/*
 *	Define the values for the Fields "Frame Synch Types"
 */
#define	MCS_ERS		    1		/* MCS ERS Format Synch				*/
#define	SPACETEC_ERS	2		/* SPACETEC ERS Format Synch			*/
#define	IAI_ERS			3		/* Israeli IAI  ERS Format Synch		*/
#define	LABEN_ERS		4		/* Laben ERS Format Synch			*/
#define	SPACETEC_MIU_ERS 5		/* SPACETEC plus ACS MIU ERS Format Synch 	*/
#define	ENERTEC_MSS		6		/* ENERTEC MSS Format Synch 			*/
#define	ENERTEC_TM		7		/* ENERTEC TM  Format Synch 			*/
#define	ACS_SPOT		8		/* ACS SPOT Software Format Synch 		*/
#define	ACS_ERS			9		/* ACS ERS  Software Format Synch 		*/
#define	ACS_LAND_TM		10		/* ACS TM   Software Format Synch 		*/
#define	ACS_LAND_MSS	11		/* ACS MSS  Software Format Synch 		*/
#define	ACS_NOAA		12		/* ACS NOAA Software Format Synch 		*/
#define ACS_JERS_SAR	13		/* ACS JERS Software Format Synch 		*/
#define ACS_XSAR_SAR	14		/* ACS JERS Software Format Synch 		*/

	static char const
		*FrameSynch_Type[] =
		{
			"MCS ERS",
			"SpaceTec ERS",
			"IAI ERS",
			"Laben ERS", 
			"Spacetec+MIU ERS", 
			"Enertec MSS", 
			"Enertec TM", 
			"ACS_Software", 
			"ACS_Software", 
			"ACS_Software", 
			"ACS_Software", 
			"ACS_Software", 
			"ACS_Software",
			"XSAR Software"
		};

/*
 *	Define the values for the Fields "CopyFlag"
 */
#define THIS_IS_NOT_A_COPY  0
#define COPY_WILMA_TO_WILMA 1
#define COPY_FRED_TO_WILMA  2

/*------------------------------------------------------------------------------------------------------------------*
 *      VOLUME IDENTIFIER
 *------------------------------------------------------------------------------------------------------------------*/
/*
 * 	The Optical_Media_Descriptor_File and the ID1_Cassette_Descriptor_File are containing the
 *	informations needed to identify a given physical volume. In the Optical Tape the Optical_Media_Descriptor_File
 *	is at the beginning of the partition, while in the ID1 cassette the equivalent file
 *	ID1_Cassette_Descriptor_File is written in the Volume ID. In this second case the file contains
 *	also the number of transcriptions on the cassette.
 */
	typedef	struct Optical_Media_Descriptor_File__
	{
		unsigned int File_Key ;		/* This Key is Used to Recognize the DMD File	*/
		unsigned int Tape_Number;	/* Progressive Optical Tape Number		*/

/*
 *	Data Format Identification Fields
 */
		char Tape_In_Date[32] ;	/* Tape Initialisation Date			*/ /* PRQA S 4403 EOF */
		char Tape_ID[32] ;		/* Tape Identification Number (ascii)		*/
	    char Soft_ID[32] ;		/* Software task name				*/
		char Soft_VS[8] ;		/* Software task version			*/

		char Filler[16];		/* Available fields				*/

	} Optical_Media_Descriptor_File;

	typedef	struct ID1_Cassette_Descriptor_File__
	{
		unsigned int File_Key ;		/* This Key is Used to Recognize the DMD File	*/
		unsigned int Tape_Number;	/* Progressive Optical Tape Number		*/

/*
 *	Data Format Identification Fields
 */
		char Tape_In_Date[32] ;	/* Tape Initialisation Date			*/
		char Tape_ID[32] ;		/* Tape Identification Number (ascii)		*/
	    char Soft_ID[32] ;		/* Software task name				*/
		char Soft_VS[8] ;		/* Software task version			*/

		unsigned int Transcription_on_Tape ;	/* The Number of Trascriptions on the Cassette	*/
		unsigned int Write_Address ;            /* Address (in track set) where start to write	*/
        unsigned int Data_On_Tape ;             /* Amount of bytes written in tape (MBytes)	*/
		char Filler[36];                      /* Available fields (Volume ID max size 160)	*/

	} ID1_Cassette_Descriptor_File;


/*
 * 	Type Record Description 
 */
	typedef	struct Record_Descriptor__
	{
		unsigned int Record_ID ;    /* Record identifier						*/
		unsigned int Record_N ;		/* Number of physical Records					*/
		unsigned int Record_L ;		/* Physical Record length					*/
		unsigned int Elem_N ;       /* Number of logical element per record				*/
		unsigned int Elem_L ;       /* Logical element length as bytes				*/
		char Filler[44];           /* Available fields						*/

	} Record_Descriptor;

/*------------------------------------------------------------------------------------------------------------------*
 *      PASS/HEADER RECORD
 *------------------------------------------------------------------------------------------------------------------*/
/*
 *	The "Transcription_File_Description_Header" record contains the acquisition characteristics description and
 *	the logical and physical file structure.
 *	This record is written twice on tape, the first time as "Pass Identifier" also is in some fieds there are
 *	no data jet available, and the second time as "Header Record" pointed by the retival data structure.
 */
	typedef struct Transcription_File_Description_Header__ 
	{
		unsigned int File_Key;		/* This Key is Used to Recognize the DT File	*/

/*
 *	Data Format Identification Fields
 */
		char File_ID[32] ;	    /* Key to recognize the full resolution files	*/
		char Soft_ID[32] ;		/* Software task name				*/
		char Soft_VS[8];		/* Software task version			*/

/*
 *	Instrument and Mission Identification Fields
 */
		unsigned short Satellite_ID ;		/* Satellite Identification Flag:

						1 = NASA Landsat (Landsat)
						2 = NASDA Marine Observation Satellite (MOS)
						3 = NASDA Japanese Earth Resources Satellite (JERS)
						4 = CNES Satellite Pour l' Observation de la Terre (SPOT)
						5 = ESA Earth Resources Satellite (ERS)				*/

		unsigned short Mission_ID ;		/* Mission Identification Number

						1 - 6 -> NASA Landsat
						1 - 2 -> NASDA Marine Observation Satellite (MOS)
						1 -----> NASDA Japanese Earth Resources Satellite (JERS)
						1 - 3 -> CNES Satellite Pour l' Observation de la Terre (SPOT)
						1 -----> ESA Earth Resources Satellite (ERS)			*/

		unsigned short Instrument_Type_ID ;	/* Instrument Type Identification Flag:

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
						11 = ERS Along Track Scanning Radiometer (ATSR)			
						12 = SPOT Hrv
						13 = JERS_SAR							*/

		unsigned short Instrument_Number ;	/* Instrument Number:

						SPOT Case:
						1  = SPOT High Resolution Visible Number 1
						2  = SPOT High Resolution Visible Number 2

						MOS Case:
						1  = Camera 1
						2  = Camera 2							*/

		unsigned short Transmission_Channel ;	/* Transmission Channel :

						SPOT Case:
						1  = Channel 1
						2  = Channel 2
														*/
/*
 *	Ground Station Identification Fields
 */
		unsigned short Station_ID ;		/* Receiving Ground Station Identification Field		*/

		unsigned short Station_DT_ID;		/* Transcription StationIdentification Field			*/


/*
 *	Transcription Identification Fields
 */
		unsigned int Input_Hddr_Type ;	/* Model of the Souce Hddr Used for the Transcription		*/
		unsigned int Format_Sync_Type ;	/* Model of the Format Synchronizer Used for the Transcription	*/
		unsigned int Output_Media_Type ;	/* Type of the Output Support (Optical, Magnetic etc.		*/
		unsigned int Output_Device_Type ;/* Type of the Device Used for the Data Transcription		*/

/*
 *	Orbit and Acquisition Identification Fields 
 */
		int Track_Number ;		/* Track Number (if applicable)					*/
		int Orbit_Number ;		/* Orbit Number (if applicable)					*/
		int Cycle_Number ;		/* Cycle Number (if applicable)					*/
		int Number_of_Frames ;	/* Number of Acquisible Frames for the Present Track		*/
		int First_Frame ;		/* First Frame Really Acquired					*/
		int Minor_Frame_in_Error ;	/* Number of Swath with Minor Frame Lost			*/
		int Eov_in_Error ;		/* Number of Swath with Missing End of Video			*/
		int Calibration_in_Error ;	/* Number of Swath with Calibration Exiding Limits		*/
		int Dark_in_Error ;		/* Number of Swath with Dark Exiding Limits			*/
			
		unsigned int First_OBC ;		/* First On Board Counter					*/
		unsigned int Last_OBC;		/* Last On Board Counter					*/

		unsigned short Acquisition_Date[3] ;	/* Acquisition Date of the Satellite Pass in Year, Monts, Days	*/
		unsigned short Acquisition_Day ;	/* Day in the Year of the Acquisition				*/ 
		unsigned short Acquisition_Start[4] ;	/* Start of Acquisition in Hours, Min, Sec, Millisec		*/
		unsigned short Acquisition_End[4] ;	/* End of Acquisition in Hours, Min, Sec, Millisec		*/
		unsigned short Production_Date[3] ;	/* Production Date of the Satellite Pass in Days, Monts, Year	*/
		unsigned short Production_Start[3] ;	/* Start of Production in Hours, Minute, Seconds		*/
		unsigned short Production_End[3];	/* End of Production in Hours, Minute, Seconds			*/

/*
 *	Pointer to Tape Data Structure 
 */
		unsigned int Number_of_Segments ;	/* Track Divided in N Number of Segments			*/
		unsigned int Loaded_Swath ;		/* Number of Lines Loaded on Tape				*/
		unsigned int Swath_Size ;		/* Swath Record Size						*/
		unsigned int Swath_per_Block ;	/* Number of Swath per Block					*/
		unsigned int Number_of_Blocks;	/* Number of Blocks i.e. Number of Collected Adresses		*/
		daddr_t Header_Address ;		/* Physical Address on Tape of the Header Record		*/
		daddr_t Pass_Address;		/* Physical Address on Tape of the Pass Identifier Record
			
						if == 0 The Record is the "Pass Identifier Record"
						if != 0 The Record is the "Header Record and is pointing to
							the First Video Data Record				*/

		unsigned int Number_of_Files;	/* Number of Files following the present one			*/

		Record_Descriptor Record_List[MAX_REC_TYP]; /* Record Descriptors of the ones following the present	*/ /* PRQA S 1021 */

		char Filler[2];		/* Available fields						*/
		char CopyFlag;		/* Available fields						*/
		char CopyInputMediaType;	/* Available fields						*/
		u_long CopyInputTapeNumber;	/* Available fields						*/

	} Transcription_File_Description_Header;


/*
 *	This Structure contains the description of all segments in which the passage has been divided.
 *	Each segments is completely described by the following fields and contains all information to address
 *	the segment within the video data record.
 *	This structure is repeated as many times as the segment number.
 */
	typedef struct Segment_Description_Record__ 
	{
		unsigned short Acquisition_Date[3] ;	/* Acquisition Date of the Satellite Pass in Year, Monts, Days	*/
		unsigned short Acquisition_Day ;	/* Day in the Year of the Acquisition				*/ 
		unsigned short Segment_Start[4] ;	/* Start of Segment in Hours, Min, Sec, Millisec		*/
		unsigned short Segment_End[4];		/* End of Segment in Hours, Min, Sec, Millisec			*/

/*
 *	Pointer to Tape Data Structure 
 */
		unsigned int Loaded_Swath ; 	/* Number of Lines Loaded on Tape for this Segment		*/
		unsigned int First_Swath ;		/* First Swath of the Segment					*/
		unsigned int Last_Swath ;		/* Last Swath of the Segment					*/
		unsigned int Lost_Swath ;		/* Losts Swath of the Segment					*/
		unsigned int First_Frame ;		/* First Frame of the Segment (if applicable)			*/
		unsigned int Last_Frame ;		/* Last Frame of the Segment (if applicable)			*/
		unsigned int First_OBC ;		/* First On Board Counter (if applicable)	 		*/
		unsigned int Last_OBC;		    /* Last On Board Counter (if applicable)			*/
		daddr_t Pass_Address;		/* Physical Address on Tape of First Video Data Record		*/
		char Filler[68];		/* Filler to Have a Record Size of 128				*/

	} Segment_Description_Record;

/*
 *	This Structure contains the description of all blocks in which the passage has been divided.
 *	Each blocks is completely described by the following fields and contains all information to address
 *	the segment within the video data record.
 *	This structure is repeated as many times as the blocks number.
 */
	typedef struct Block_Description_Record__ 
	{
		daddr_t Video_Starting_Address;	/* Physical Address on Tape of First Swath of the Block		*/

		unsigned int Satellite_Time[2] ;	/* Time in Milliseconds of the first Swath of the Block		*/
		unsigned int Delta_Time ;		/* Millisecond from  Start of Acquisition of First Block Swath	*/
		unsigned int Swath_per_Block;	/* Number of Swat in the Block					*/
		char Filler[12];		/* Available fields						*/

	} Block_Description_Record;

/*------------------------------------------------------------------------------------------------------------------*
 *      USER HEADER
 *------------------------------------------------------------------------------------------------------------------*/
/*
 *	This Structure contains the Data Transcription Statistics that is the input to the Leda.
 */
	typedef struct Data_Transcription_Statistic__ 
	{
		unsigned int File_Key;		/* This Key is Used to Recognize the DT Statistic File		*/

/*
 *	Instrument and Mission Identification Fields
 */
		unsigned short Satellite_ID ;		/* Satellite Identification Flag				*/
		unsigned short Mission_ID ;		/* Mission Identification Number				*/
		unsigned short Instrument_Type_ID ;	/* Instrument Type Identification Flag				*/
		unsigned short Instrument_Number ;	/* Instrument Number						*/
		unsigned short Transmission_Channel ;	/* Transmission Channel						*/
/*
 *	Ground Station Identification Fields
 */
		unsigned short Station_ID ;		/* Receiving Ground Station Identification Field		*/
		unsigned short Station_DT_ID;		/* Transcription StationIdentification Field			*/
/*
 *	Orbit and Acquisition Identification Fields 
 */
		int Track_Number ;		/* Track Number (if applicable)					*/
		int Orbit_Number ;		/* Orbit Number (if applicable)					*/
		int Cycle_Number ;		/* Cycle Number (if applicable)					*/
		int Number_of_Frames ;	/* Number of Acquisible Frames for the Present Track		*/
		int First_Frame ;		/* First Frame Really Acquired					*/
		int Minor_Frame_in_Error ;	/* Number of Swath with Minor Frame Lost			*/
		int Eov_in_Error ;		/* Number of Swath with Missing End of Video			*/
		int Calibration_in_Error ;	/* Number of Swath with Calibration Exiding Limits		*/
		int Dark_in_Error;		/* Number of Swath with Dark Exiding Limits			*/

		unsigned short Acquisition_Date[3] ;	/* Acquisition Date of the Satellite Pass in Year, Monts, Days	*/
		unsigned short Acquisition_Day ;	/* Day in the Year of the Acquisition				*/ 
		unsigned short Acquisition_Start[4] ;	/* Start of Acquisition in Hours, Min, Sec, Millisec		*/
		unsigned short Acquisition_End[4] ;	/* End of Acquisition in Hours, Min, Sec, Millisec		*/
		unsigned short Transcription_Date[3] ;	/* Production Date of the Satellite Pass in Days, Monts, Year	*/
		unsigned short Transcription_Start[3] ;	/* Start of Production in Hours, Minute, Seconds		*/
		unsigned short Transcription_End[3];	/* End of Production in Hours, Minute, Seconds			*/

/*
 *	Transcription Identification Fields
 */
		unsigned int Input_Hddr_Type ;	/* Model of the Souce Hddr Used for the Transcription		*/
		unsigned int Input_Hddr_Number ;	/* Number of the Souce Hddr Used for the Transcription		*/
		unsigned int Output_Media_Type ;	/* Type of the Output Support (Optical, Magnetic etc.		*/
		unsigned int Output_Media_Number ;	/* Catalog Number of the Output Media				*/
		unsigned int Output_Device_Type ;	/* Type of the Device Used for the Data Transcription		*/

/*
 *	Pointer to Tape Data Structure 
 */
		unsigned int Number_of_Segments ;	/* Track Divided in N Number of Segments			*/
		unsigned int Loaded_Swath ;		/* Number of Lines Loaded on Tape				*/
		unsigned int Swath_Size ;		/* Swath Record Size						*/
		unsigned int Swath_per_Block ;	/* Number of Swath per Block					*/
		unsigned int Number_of_Blocks;	/* Number of Blocks i.e. Number of Collected Adresses		*/

		daddr_t Header_Address ;		/* Physical Address on Tape of the Data Transcription Header	*/
		daddr_t Pass_Address;		/* Physical Address on Tape of First Video Data Record		*/

		unsigned int Number_of_Files;		/* Number of Files following the present one			*/

		Record_Descriptor Record_List[MAX_REC_TYP]; /* Record Descriptors of the ones following the present */  /* PRQA S 1021 */

		int  		Data_On_Tape;						/* Data that has been transferred on tape																		*/
		char		Transcription_Mode[2];				/* 'I' = Interleaved, 'S'=sequential. 'I' is set when two channels are written (LANDSAT7)						*/
		char		Data_Take_Id[8];					/* Data Take Id (string, terminated). This is satellite dependent. XSAR is DDD.DDD\0							*/
		char		Filler[38];							/* Available fields																								*/
		char		ThisIsASpotChannelRecovRec;			/* Flag (99            = this is a record which previously beloged to another tape with the wrong channel number
														   Flag (anyothernumber= thisin not a ......																	 */
		char		Copy_Date[3];						/* Copy Date in Days, Monts, Year																				 */
		u_long		CopyInputTapeNumber;				/* input media identificator					*/
		char		CopyFlag;							/* '0' = this is not a  copy					*/
		char		CopyInputMediaType;					/* input media type						*/
		char		dummy;								/* Available fields					*/
		u_char		ValidationFlag;						/* Flag (99		="DO NOT USE IT IN PRODUCTION" anyothernumber	="USE IT IN PRODUCTION  		*/

	} Data_Transcription_Statistic;

#ifdef __cplusplus
	}
#endif

#endif
