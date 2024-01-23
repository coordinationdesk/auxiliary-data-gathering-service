/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Spot_4_Structures.h $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2006/08/23 12:55:07  enrcar
	Versioning added
	
	
*/


/*
    Based on original SGI file:
        Spot_4_Structures.h,v 1.2.4.3 2000/11/28 09:31:19 sardev Exp
        Size: 16724
*/


#ifndef Spot_4_Structures__
#define Spot_4_Structures__

/*Version string*/
static const char Spot_4_Structures_h1[]="ACSFILE: $Id$ ," __DATE__;
static const char Spot_4_Structures_h2[]="ACSSYS : PROJ-SPOT-04, autor G.Trevisiol C. Nasuti";


#define	Spot_Missions	    4			    /* Number of Spot Missions				*/
#define	CCD_Array_Number    4			    /* Number of CCD Array per Line (Pan & Xs)		*/
#define	Xs_Spectral_Bands   3			    /* Number of Spectral Bands (Xs)			*/

/*@Updated 4*/
#define Spot_Xi_Line			3000
#define Spot_Xi_Num_Package		252 		/* Number of package are 252  */	 


#define Spot_Xi_Dim_Package		18 		/* espressed in byte */
#define Spot_Xi_Size_Buffer 	4536 	/* Size of half coded image line */

#define Spot_Xi_RGBIR_Size		4
#define Spot_Xi_Num_Byte		24
#define	Spot_Ql_Xi_Sampl    	3			    /* Spot Sampling Factor for the Xi Ql		*/
#define Spot_Time_Param_Threshold 400			/* Threshold For stability of time paramiter */
#define Spot_Time_Obt_Threshold   200           /* Threshold For stability of obt parameter */ 
#define Spot_Time_Fc_Threshold    10           /* Threshold For stability of Fc parameter */ 


/*@Updated 4*/ 

#define	XI_Spectral_Bands   4			    /* Number of Spectral Bands (XI Spot 4)		*/

#define	SpotMF_Sync1	    0x04314f47		    /* Bits  0 - 31 of Spot Sync Word			*/
#define	SpotMF_Sync2	    0x25bb357e		    /* Bits 32 - 63 of Spot Sync Word			*/

#define	Spot_Line_Period    3.008		    /* SpotScan Time in Milliseconds			*/

#define	RGB_Size	    3			    /* Number of Bytes for a Color Pixel		*/
#define RGBIR_Size      4               /* Number of Bytes for a Color Pixel with infrared band		*/
#define	BitPerMinFrame	    74256		    /* Number of Bits per Minor Frame Spot		*/
#define	MinFrameSize	    (BitPerMinFrame / 8)    /* Number of Bytes per Minor Frame Spot		*/
#define	SyncWordSize	    8			    /* Number of Bytes of the Sync Word			*/
#define	AuxiliarySize	    126			    /* Number of Original Spot Auxiliary Bytes		*/


#define	TopAuxiliary	    128			    /* Number of Auxiliary Bytes in ACS Transcription	*/
#define Spot_VideoDataSize	12000 			/* Size of video data */

#ifdef SOUTH_AFRICA_OLD_AUX
static char* SAId="@(#) Spot_Structes.h : ** set SOUTH AFRICA auxiliary length";
#endif
#define	TopAuxiliarySouthAfrica	    104			    /* Number of Auxiliary Bytes in ACS Transcription	*/



#define	Spot_Pancro_Line    6000		    /* Spot Fr Panchromatic Line Size			*/
#define	Spot_Multi_Line	    3000		    /* Spot Fr Multispectral Line Size			*/

#define	Offset_Pan_Line_1   126			    /* Offset in Byte of First Pixel of Pan Line 1	*/
#define	Offset_Pan_Line_2   4767		    /* Offset in Byte of First Pixel of Pan Line 2	*/
#define	Offset_XS_Line	    126			    /* Offset in Byte of First Pixel of Xs Line		*/

#define	Spot_Ql_Pan_Sampl   6			    /* Spot Sampling Factor for the Pan Ql		*/
#define	Spot_Ql_Xs_Sampl    3			    /* Spot Sampling Factor for the Xs Ql		*/
#define	Spot_Ql_Fr_Sampl    3			    /* Spot Sampling During Full Resolution		*/
#define	Spot_Ql_Rt_Sampl    6			    /* Spot SamplingDuring Real Time Acq		*/

#define	Spot_QL_Rt_size	    500			    /* Spot Quick Look Line Size During Phase 1		*/
#define	Spot_QL_size	    1000		    /* Spot Quick Look Line Size During Phase 2		*/
#define	Spot_QL_Rt_Record   ((((Spot_QL_Rt_size * RGB_Size) + 511) / 512) * 512) /* Ql Record Size 	*/
#define	Spot_QL_Record	    ((((Spot_QL_size * RGB_Size) + 511) / 512) * 512) /* Ql Record Size		*/
#define	Ql_Dark_Pixels	    12			    /* Number of Dark Pixels at Image Left		*/
#define	Ch1		    0			    /* Spot Direct Bit Stream Channel			*/
#define	Ch2		    1			    /* Spot Inverted Bit Stream Channel			*/

#define	DataToCheckForStab  5			    /* Number of Parameter to be check for Stability	*/

#define	FormatCounterID_Ch1 0			    /* Format Counter Id in the Filtering Table		*/
#define	FormatCounterID_Ch2 1			    /* Format Counter Id in the Filtering Table		*/
#define	On_Board_Time_ID    2			    /* On Board Time Id in the Filtering Table		*/

#define	MaxSpotAcquisition  12			    /* Max Spot Acquisition in Minutes			*/

#define	MaxTrackPointsDes   500			    /* Max number of pointsdescribing a track		*/
		
	typedef struct Spot_4_Auxiliary__
	{
	    u_int
		Sync_Word[2];		/* W1234: Sync pattern						*/
	   
        u_int  
#if ( BYTE_ORDER == BIG_ENDIAN )
		Form_Equip_Num:	    4,	/* W5: Format Equipment Number					*/
		Form_Equip_Num_R:   4,	/* W5: Format Equipment Number Ridundant			*/
		reserved_1:	    3,	/* W5: Reserved ( code allarm )					*/
		REFT:		    1,	/* W5: Time tagging for channel switch				*/
		Frame_Number:	   20; 	/* W56: Frame Number (0 - 19)					*/
#else
		Frame_Number:	   20, 	/* W56: Frame Number (0 - 19)					*/
		REFT:		    1,	/* W5: Time tagging for channel switch				*/
		reserved_1:	    3,	/* W5: Reserved ( code allarm )					*/
		Form_Equip_Num_R:   4,	/* W5: Format Equipment Number Ridundant			*/
		Form_Equip_Num:	    4;	/* W5: Format Equipment Number					*/

#endif	    
  
        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )
		Mode_Channel_1:	    8,	/* W7: Working Mode of Channel 1				*/
		Mode_Channel_2:	    8, 	/* W7: Working Mode of Channel 2				*/
		Recorder_Mode:	    4,	/* W8: Reserved							*/
		Payload_Mode:	    4,	/* W8: Reserved							*/
		HRVIR1_SWIR_Gain:   4, 	/* W8: MIR gain for instrument 1				*/
		HRVIR2_SWIR_Gain:   4; 	/* W8: MIR gain for instrument 2				*/
#else
		HRVIR2_SWIR_Gain:   4, 	/* W8: MIR gain for instrument 2				*/
		HRVIR1_SWIR_Gain:   4, 	/* W8: MIR gain for instrument 1				*/
		Payload_Mode:	    4,	/* W8: Reserved							*/
		Recorder_Mode:	    4,	/* W8: Reserved							*/
		Mode_Channel_2:	    8, 	/* W7: Working Mode of Channel 2				*/
	    Mode_Channel_1:	    8;	/* W7: Working Mode of Channel 1				*/

#endif	  


        u_int	
#if ( BYTE_ORDER == BIG_ENDIAN )
	
		Mode_HRVIR1_Sun_Cal:	1,	/* W9: Working Mode of HRVIR1 Sun Calibration		    */
		Mode_HRVIR1_Mirror:	1,	/* W9: Working Mode of HRVIR1 Mirror Calibration Position   */
		Mode_HRVIR1_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR1 Calibration Lamp		    */
		Mode_HRVIR1_DPCM:	1,	/* W9: Working Mode of HRVIR1 DPCM Coding		    */
		Mode_HRVIR1_MultiSp:	1,	/* W9: Working Mode of HRVIR1 Multi Spectral Mode On	    */
		Mode_HRVIR1_Pancro:	1,	/* W9: Working Mode of HRVIR1 Panchromatic Mode On	    */
		Mode_HRVIR1_Tel_type:	1,	/* W9: Working Mode of HRVIR1 Not Used Bit		    */
		Mode_HRVIR1_Mirror_V:	1,	/* W9: Working Mode of HRVIR1 Front Mirror Voltage	    */

		Mode_HRVIR2_Sun_Cal:	1,	/* W9: Working Mode of HRVIR2 Sun Calibration		    */
		Mode_HRVIR2_Mirror:	1,	/* W9: Working Mode of HRVIR2 Mirror Calibration Position   */
		Mode_HRVIR2_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR2 Calibration Lamp		    */
		Mode_HRVIR2_DPCM:	1,	/* W9: Working Mode of HRVIR2 DPCM Coding	    	    */
		Mode_HRVIR2_MultiSp:	1,	/* W9: Working Mode of HRVIR2 Multi Spectral Mode On	    */
		Mode_HRVIR2_Pancro:	1,	/* W9: Working Mode of HRVIR2 Panchromatic Mode On	    */
		Mode_HRVIR2_Tel_type:	1,	/* W9: Working Mode of HRVIR2 Not Used Bit		    */
		Mode_HRVIR2_Mirror_V:	1, 	/* W9: Working Mode of HRVIR2 Front Mirror Voltage  	    */
		
		HRVIR1_Gain_M:	    4,	/* W10: HRVIR1 Gain of Panchromatic Channel			*/
		HRVIR1_Gain_XS1:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 1		        */
		HRVIR1_Gain_XS2:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 2		        */
		HRVIR1_Gain_XS3:    4, 	/* W10: HRVIR1 Gain of Multispectral Channel 3		        */
#else
		HRVIR1_Gain_XS3:    4, 	/* W10: HRVIR1 Gain of Multispectral Channel 3		        */
		HRVIR1_Gain_XS2:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 2		        */
		HRVIR1_Gain_XS1:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 1		        */
		HRVIR1_Gain_M:	    4,	/* W10: HRVIR1 Gain of Panchromatic Channel			*/
        
		Mode_HRVIR2_Mirror_V:	1, 	/* W9: Working Mode of HRVIR2 Front Mirror Voltage  	    */
		Mode_HRVIR2_Tel_type:	1,	/* W9: Working Mode of HRVIR2 Not Used Bit		    */
		Mode_HRVIR2_Pancro:	1,	/* W9: Working Mode of HRVIR2 Panchromatic Mode On	    */
		Mode_HRVIR2_MultiSp:	1,	/* W9: Working Mode of HRVIR2 Multi Spectral Mode On	    */
		Mode_HRVIR2_DPCM:	1,	/* W9: Working Mode of HRVIR2 DPCM Coding	    	    */
		Mode_HRVIR2_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR2 Calibration Lamp		    */
		Mode_HRVIR2_Mirror:	1,	/* W9: Working Mode of HRVIR2 Mirror Calibration Position   */
		Mode_HRVIR2_Sun_Cal:	1,	/* W9: Working Mode of HRVIR2 Sun Calibration		    */
        
		Mode_HRVIR1_Mirror_V:	1,	/* W9: Working Mode of HRVIR1 Front Mirror Voltage	    */
		Mode_HRVIR1_Tel_type:	1,	/* W9: Working Mode of HRVIR1 Not Used Bit		    */
		Mode_HRVIR1_Pancro:	1,	/* W9: Working Mode of HRVIR1 Panchromatic Mode On	    */
		Mode_HRVIR1_MultiSp:	1,	/* W9: Working Mode of HRVIR1 Multi Spectral Mode On	    */
		Mode_HRVIR1_DPCM:	1,	/* W9: Working Mode of HRVIR1 DPCM Coding		    */
		Mode_HRVIR1_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR1 Calibration Lamp		    */
		Mode_HRVIR1_Mirror:	1,	/* W9: Working Mode of HRVIR1 Mirror Calibration Position   */
		Mode_HRVIR1_Sun_Cal:	1,	/* W9: Working Mode of HRVIR1 Sun Calibration		    */
        
#endif	   

#if ( BYTE_ORDER == BIG_ENDIAN )
		HRVIR2_Gain_M:	    4,	/* W11: HRVIR2 Gain of Panchromatic Channel			*/
		HRVIR2_Gain_XS1:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 1			*/
		HRVIR2_Gain_XS2:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 2			*/
		HRVIR2_Gain_XS3:    4, 	/* W11: HRVIR2 Gain of Multispectral Channel 3			*/
		
		Satellite_num:	    4,  /* W12: Reserved						*/ 
		HRVIR1_conf:	    2,  /* W12: Reserved						*/ 
		HRVIR2_conf:	    2,  /* W12: Reserved						*/ 
		HRVIR1_conf_Mir:    4,	/* W12: SWIR working mode					*/
		HRVIR2_conf_Mir:    4;	/* W12: SWIR working mode					*/				
#else
		HRVIR2_conf_Mir:    4,	/* W12: SWIR working mode					*/				
		HRVIR1_conf_Mir:    4,	/* W12: SWIR working mode					*/
		HRVIR2_conf:	    2,  /* W12: Reserved						*/ 
		HRVIR1_conf:	    2,  /* W12: Reserved						*/ 
		Satellite_num:	    4,  /* W12: Reserved						*/ 

		HRVIR2_Gain_XS3:    4, 	/* W11: HRVIR2 Gain of Multispectral Channel 3			*/
		HRVIR2_Gain_XS2:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 2			*/
		HRVIR2_Gain_XS1:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 1			*/
		HRVIR2_Gain_M:	    4;	/* W11: HRVIR2 Gain of Panchromatic Channel			*/
#endif	   
 
        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )
		Attitude_Status:    8,	/* W13: Attitude Control System Status				*/
		Yaw_Velocity_lsb:   8, 	/* W13: Yaw Velocity (8 lsb)					*/
		Roll_Velocity_lsb:  8, 	/* W14: Roll Velocity (8 lsb)					*/
		Pitch_Velocity_lsb: 8;	/* W14: Pitch Velocity	(8 lsb)					*/
#else
		Pitch_Velocity_lsb: 8,	/* W14: Pitch Velocity	(8 lsb)					*/
		Attitude_Status:    8,	/* W13: Attitude Control System Status				*/
		Yaw_Velocity_lsb:   8, 	/* W13: Yaw Velocity (8 lsb)					*/
		Roll_Velocity_lsb:  8; 	/* W14: Roll Velocity (8 lsb)					*/
#endif	   
 
        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )
		reserved_2:	    16, /* W15: not applicable						*/
		reserved_3:	    10, /* W16: not applicable						*/
		Yaw_Velocity_msb:   2, 	/* W16: Yaw Velocity (2 msb)					*/
		Roll_Velocity_msb:  2, 	/* W16: Roll Velocity (2 msb)					*/
		Pitch_Velocity_msb: 2;	/* W16: Pitch Velocity	(2 msb)					*/
#else
		Pitch_Velocity_msb: 2,	/* W16: Pitch Velocity	(2 msb)					*/
		Roll_Velocity_msb:  2, 	/* W16: Roll Velocity (2 msb)					*/
		Yaw_Velocity_msb:   2, 	/* W16: Yaw Velocity (2 msb)					*/
		reserved_3:	    10, /* W16: not applicable						*/
		reserved_2:	    16; /* W15: not applicable						*/
#endif

	    u_int
		Reserved_for_Decoder[5];/* W17->26 Used for decoder initialisation			*/		
	  
        u_int
		Orbital_elements[11];	/* W27->48 Orbital elements and DIODE Ephemeris			*/

	    u_int
#if ( BYTE_ORDER == BIG_ENDIAN )        
		MCV_ST1:	    8,	/* W49: MCV_ST1							*/
		MCV_POS1:	    8, 	/* W49: MCV_POS1						*/
		MCV_ST2:	    8,	/* W50: MCV_ST2							*/
		MCV_POS2:	    8;	/* W50: MCV_POS2						*/
#else
		MCV_POS2:	    8,	/* W50: MCV_POS2						*/
		MCV_ST2:	    8,	/* W50: MCV_ST2							*/
		MCV_POS1:	    8, 	/* W49: MCV_POS1						*/
		MCV_ST1:	    8;	/* W49: MCV_ST1							*/
#endif	   
        u_int
		On_Board_Time;		/* W51,W52 Spot On Board Time					*/
	    
        u_int
		Decoder_validation[2];  /* W53->56  Decoder validation sequence				*/	    
	    
        u_int
		reserved_4;		/* W57,W58  Reserved						*/
	    
        u_int
		Time_tagging_coeff[3];  /* W59->63 Time tagging coefficients + 1 word (W64) padding	*/		
	} Spot_4_Auxiliary;
		
	typedef struct SPOT_4_Ql_Only_Auxiliary__
	{
	    u_int  
#if ( BYTE_ORDER == BIG_ENDIAN )        
		Form_Equip_Num:	    4,	/* W5: Format Equipment Number					*/
		Form_Equip_Num_R:   4,	/* W5: Format Equipment Number Ridundant			*/
		reserved_1:	    3,	/* W5: Reserved ( code allarm )					*/
		REFT:		    1,	/* W5: Time tagging for channel switch				*/
		Frame_Number:	   20; 	/* W56: Frame Number (0 - 19)					*/
#else
		Frame_Number:	   20, 	/* W56: Frame Number (0 - 19)					*/
		REFT:		    1,	/* W5: Time tagging for channel switch				*/
		reserved_1:	    3,	/* W5: Reserved ( code allarm )					*/
		Form_Equip_Num_R:   4,	/* W5: Format Equipment Number Ridundant			*/
		Form_Equip_Num:	    4;	/* W5: Format Equipment Number					*/

#endif	  

        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )        
		Mode_Channel_1:	    8,	/* W7: Working Mode of Channel 1				*/
		Mode_Channel_2:	    8, 	/* W7: Working Mode of Channel 2				*/
		Recorder_Mode:	    4,	/* W8: Reserved							*/
		Payload_Mode:	    4,	/* W8: Reserved							*/
		HRVIR1_SWIR_Gain:   4, 	/* W8: MIR gain for instrument 1				*/
		HRVIR2_SWIR_Gain:   4; 	/* W8: MIR gain for instrument 2				*/
#else
        HRVIR2_SWIR_Gain:   4, 	/* W8: MIR gain for instrument 2				*/
        HRVIR1_SWIR_Gain:   4, 	/* W8: MIR gain for instrument 1				*/
        Payload_Mode:	    4,	/* W8: Reserved							*/
        Recorder_Mode:	    4,	/* W8: Reserved							*/
        Mode_Channel_2:	    8, 	/* W7: Working Mode of Channel 2				*/
        Mode_Channel_1:	    8;	/* W7: Working Mode of Channel 1				*/
#endif
	   
        u_int		
#if ( BYTE_ORDER == BIG_ENDIAN )        
		Mode_HRVIR1_Sun_Cal:	1,	/* W9: Working Mode of HRVIR1 Sun Calibration		    */
		Mode_HRVIR1_Mirror:	1,	/* W9: Working Mode of HRVIR1 Mirror Calibration Position   */
		Mode_HRVIR1_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR1 Calibration Lamp		    */
		Mode_HRVIR1_DPCM:	1,	/* W9: Working Mode of HRVIR1 DPCM Coding		    */
		Mode_HRVIR1_MultiSp:	1,	/* W9: Working Mode of HRVIR1 Multi Spectral Mode On	    */
		Mode_HRVIR1_Pancro:	1,	/* W9: Working Mode of HRVIR1 Panchromatic Mode On	    */
		Mode_HRVIR1_Not_Used:	1,	/* W9: Working Mode of HRVIR1 Not Used Bit		    */
		Mode_HRVIR1_Mirror_V:	1,	/* W9: Working Mode of HRVIR1 Front Mirror Voltage	    */

		Mode_HRVIR2_Sun_Cal:	1,	/* W9: Working Mode of HRVIR2 Sun Calibration		    */
		Mode_HRVIR2_Mirror:	1,	/* W9: Working Mode of HRVIR2 Mirror Calibration Position   */
		Mode_HRVIR2_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR2 Calibration Lamp		    */
		Mode_HRVIR2_DPCM:	1,	/* W9: Working Mode of HRVIR2 DPCM Coding	    	    */
		Mode_HRVIR2_MultiSp:	1,	/* W9: Working Mode of HRVIR2 Multi Spectral Mode On	    */
		Mode_HRVIR2_Pancro:	1,	/* W9: Working Mode of HRVIR2 Panchromatic Mode On	    */
		Mode_HRVIR2_Tel_Type:	1,	/* W9: Working Mode of HRVIR2 Not Used Bit		    */
		Mode_HRVIR2_Mirror_V:	1, 	/* W9: Working Mode of HRVIR2 Front Mirror Voltage  	    */
		
		HRVIR1_Gain_M:	    4,	/* W10: HRVIR1 Gain of Panchromatic Channel			*/
		HRVIR1_Gain_XS1:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 1		        */
		HRVIR1_Gain_XS2:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 2		        */
		HRVIR1_Gain_XS3:    4, 	/* W10: HRVIR1 Gain of Multispectral Channel 3		        */
#else 
		HRVIR1_Gain_XS3:    4, 	/* W10: HRVIR1 Gain of Multispectral Channel 3		        */
		HRVIR1_Gain_XS2:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 2		        */
		HRVIR1_Gain_XS1:    4,	/* W10: HRVIR1 Gain of Multispectral Channel 1		        */
		HRVIR1_Gain_M:	    4,	/* W10: HRVIR1 Gain of Panchromatic Channel			*/

		Mode_HRVIR2_Mirror_V:	1, 	/* W9: Working Mode of HRVIR2 Front Mirror Voltage  	    */
		Mode_HRVIR2_Tel_Type:	1,	/* W9: Working Mode of HRVIR2 Not Used Bit		    */
		Mode_HRVIR2_Pancro:	1,	/* W9: Working Mode of HRVIR2 Panchromatic Mode On	    */
		Mode_HRVIR2_MultiSp:	1,	/* W9: Working Mode of HRVIR2 Multi Spectral Mode On	    */
		Mode_HRVIR2_DPCM:	1,	/* W9: Working Mode of HRVIR2 DPCM Coding	    	    */
		Mode_HRVIR2_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR2 Calibration Lamp		    */
		Mode_HRVIR2_Mirror:	1,	/* W9: Working Mode of HRVIR2 Mirror Calibration Position   */
		Mode_HRVIR2_Sun_Cal:	1,	/* W9: Working Mode of HRVIR2 Sun Calibration		    */

		Mode_HRVIR1_Mirror_V:	1,	/* W9: Working Mode of HRVIR1 Front Mirror Voltage	    */
		Mode_HRVIR1_Not_Used:	1,	/* W9: Working Mode of HRVIR1 Not Used Bit		    */
		Mode_HRVIR1_Pancro:	1,	/* W9: Working Mode of HRVIR1 Panchromatic Mode On	    */
		Mode_HRVIR1_MultiSp:	1,	/* W9: Working Mode of HRVIR1 Multi Spectral Mode On	    */
		Mode_HRVIR1_DPCM:	1,	/* W9: Working Mode of HRVIR1 DPCM Coding		    */
		Mode_HRVIR1_Cal_Lamp:	1,	/* W9: Working Mode of HRVIR1 Calibration Lamp		    */
		Mode_HRVIR1_Mirror:	1,	/* W9: Working Mode of HRVIR1 Mirror Calibration Position   */
		Mode_HRVIR1_Sun_Cal:	1,	/* W9: Working Mode of HRVIR1 Sun Calibration		    */
#endif

#if ( BYTE_ORDER == BIG_ENDIAN )        
		HRVIR2_Gain_M:	    4,	/* W11: HRVIR2 Gain of Panchromatic Channel			*/
		HRVIR2_Gain_XS1:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 1			*/
		HRVIR2_Gain_XS2:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 2			*/
		HRVIR2_Gain_XS3:    4, 	/* W11: HRVIR2 Gain of Multispectral Channel 3			*/
		
		Satellite_num:	    4,  /* W12: Reserved						*/ 
		HRVIR1_conf:	    2,  /* W12: Reserved						*/ 
		HRVIR2_conf:	    2,  /* W12: Reserved						*/ 
		HRVIR1_conf_Mir:    4,	/* W12: SWIR working mode					*/
		HRVIR2_conf_Mir:    4;	/* W12: SWIR working mode					*/				
#else
        HRVIR2_conf_Mir:    4,	/* W12: SWIR working mode					*/				
        HRVIR1_conf_Mir:    4,	/* W12: SWIR working mode					*/
        HRVIR2_conf:	    2,  /* W12: Reserved						*/ 
        HRVIR1_conf:	    2,  /* W12: Reserved						*/ 
        Satellite_num:	    4,  /* W12: Reserved						*/ 

        HRVIR2_Gain_XS3:    4, 	/* W11: HRVIR2 Gain of Multispectral Channel 3			*/
        HRVIR2_Gain_XS2:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 2			*/
        HRVIR2_Gain_XS1:    4,	/* W11: HRVIR2 Gain of Multispectral Channel 1			*/
        HRVIR2_Gain_M:	    4;	/* W11: HRVIR2 Gain of Panchromatic Channel			*/
#endif

	    
        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )        
		Attitude_Status:    8,	/* W13: Attitude Control System Status				*/
		Yaw_Velocity_lsb:   8, 	/* W13: Yaw Velocity (8 lsb)					*/
		Roll_Velocity_lsb:  8, 	/* W14: Roll Velocity (8 lsb)					*/
		Pitch_Velocity_lsb: 8;	/* W14: Pitch Velocity	(8 lsb)					*/
#else
        Pitch_Velocity_lsb: 8,	/* W14: Pitch Velocity	(8 lsb)					*/
        Roll_Velocity_lsb:  8, 	/* W14: Roll Velocity (8 lsb)					*/
        Yaw_Velocity_lsb:   8, 	/* W13: Yaw Velocity (8 lsb)					*/
        Attitude_Status:    8;	/* W13: Attitude Control System Status				*/
#endif

        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )        
		reserved_2:	    16, /* W15: not applicable						*/
		reserved_3:	    10, /* W16: not applicable						*/
		Yaw_Velocity_msb:   2, 	/* W16: Yaw Velocity (2 msb)					*/
		Roll_Velocity_msb:  2, 	/* W16: Roll Velocity (2 msb)					*/
		Pitch_Velocity_msb: 2;	/* W16: Pitch Velocity	(2 msb)					*/
#else
        Pitch_Velocity_msb: 2,	/* W16: Pitch Velocity	(2 msb)					*/
        Roll_Velocity_msb:  2, 	/* W16: Roll Velocity (2 msb)					*/
        Yaw_Velocity_msb:   2, 	/* W16: Yaw Velocity (2 msb)					*/
        reserved_3:	    10, /* W16: not applicable						*/
        reserved_2:	    16; /* W15: not applicable						*/
#endif
	   
        u_int
#if ( BYTE_ORDER == BIG_ENDIAN )        
		MCV_ST1:	    8,	/* W49: MCV_ST1							*/
		MCV_POS1:	    8, 	/* W49: MCV_POS1						*/
		MCV_ST2:	    8,	/* W50: MCV_ST2							*/
		MCV_POS2:	    8;	/* W50: MCV_POS2						*/
#else
		MCV_POS2:	    8,	/* W50: MCV_POS2						*/
		MCV_ST2:	    8,	/* W50: MCV_ST2							*/
		MCV_POS1:	    8, 	/* W49: MCV_POS1						*/
		MCV_ST1:	    8;	/* W49: MCV_ST1							*/
#endif	    
        u_int
		On_Board_Time;		/* W51,W52 Spot On Board Time					*/
	}SPOT_4_Ql_Auxiliary;
	
	typedef struct	{
	    Spot_4_Auxiliary
		Auxiliary[Spot_Ql_Fr_Sampl];
	} Ql_Spot_Auxiliary;
		
/*
 *  Spot lines used in 123 mode and 4 mode
 */
	typedef struct	{
	    u_short
		Auxiliary[63];		/* Spot Auxiliary Data Structure				*/
	    u_char
		Pan_Lin_1[4500];	/* First Linear Panchromatic Line				*/
	    u_char
		Non_Significant1[141];
	    u_char
		Pan_Lin_2[4500];	/* First Linear Panchromatic Line				*/
	    u_char
		Non_Significant2[15];
	} Spot_Panchromatic_Lin;
		
	typedef struct	{
	    u_short
		Auxiliary[63];		/* Spot Auxiliary Data Structure				*/
	    u_char
		Pan_Dpcm_1[4509];	/* First DPCM Panchromatic Line					*/
	    u_char
		Non_Significant1[132];
	    u_char
		Pan_Dpcm_2[4509];	/* Second DPCM Panchromatic Line				*/
	    u_char
		Non_Significant2[6];
	} Spot_Panchromatic_Dpcm;
		
	typedef struct	{
	    u_short
		Auxiliary[63];		/* Spot Auxiliary Data Structure				*/
	    u_char
		Pan_XS[9000];		/* Multispectral Line Line					*/
	    u_char
		Non_Significant1[156];
	} Spot_Multispectral;
	
	typedef struct	{
	    u_short
		Auxiliary[63];		/* Spot Auxiliary Data Structure				*/
	    u_char
		Multi_XI[4536*2];	/* Multispectral Line XI (Spot 4 mode)				*/
	    u_char
		Non_Significant[84];
	} Spot_4_Multispectral;


/*
 *  Generic Spot 123 full resolution line
 */
	typedef struct Generic_Spot__	
	{
	    Spot_4_Auxiliary
						Auxiliary;		    /* Spot Auxiliary Data Structure		*/
	    
	    union 
	    {
		u_char
		    Spot_Xs[Spot_Multi_Line * RGB_Size];    /* Multispectral Spot Line			*/
		u_char
		    Spot_Pan[2][Spot_Pancro_Line];	    /* Panchromatic Spot Line #1 and #2		*/
	    } Spot_Lines;

	}Generic_Spot;




/*
 *  Generic Spot 123 full resolution line for South africa
 */
	typedef struct Generic_Spot_South_Africa__	
	{
	   u_int
		   Auxiliary[TopAuxiliarySouthAfrica / 4]; 			   /* Spot Auxiliary Data Structure            */
	    
	    union 
	    {
		u_char
		    Spot_Xs[Spot_Multi_Line * RGB_Size];    /* Multispectral Spot Line			*/
		u_char
		    Spot_Pan[2][Spot_Pancro_Line];	    /* Panchromatic Spot Line #1 and #2		*/
	    } Spot_Lines;

	}Generic_Spot_South_Africa;


	
/*
 *  Generic Spot 4 full resolution line
 */
	typedef struct Generic_Spot4__	
	{
	    Spot_4_Auxiliary
						Auxiliary;		    /* Spot Auxiliary Data Structure		*/
	    
	    union 
	    {
		u_char
		    Spot_Xs[Spot_Multi_Line * RGB_Size ];    /* Multispectral Spot Line */
		u_char
		    Spot_Pan[2][Spot_Pancro_Line];	    /* Panchromatic Spot Line #1 and #2		*/
		u_char
		    Spot_Xi[Spot_Multi_Line * XI_Spectral_Bands ];    /* Multispectral Spot Line		*/
	    } Spot_Lines;
		
	    u_char
		Spot_Tail[48]; 				    /* Spot 4 ancillary data for inter array pixel value */

	}Generic_Spot4;

/* 
*Spot structure for frame counter and on board counter
*/	

typedef struct 
		{
	unsigned int
				fc, 	/* frame counter */
				obt; 	/* on board counter */
	unsigned char
				no_valid; 	/* Flag of validity line */
		} Set_Fc_Obt;


typedef struct	{
	    u_int
		Auxiliary[3];				    /* Spot Auxiliary Data Structure		*/
	    u_char
		Video[Spot_QL_size * RGBIR_Size]; 	    /* Video Line				*/
	} Fr_Generic_Ql_Spot;
	
	typedef struct	{
	    u_int
		Auxiliary[3];				    /* Spot Auxiliary Data Structure		*/
	    u_char
		Video[Spot_QL_Rt_size * RGB_Size], 	    /* Video Line				*/
		Padding[24];				    /* Padding for Sector Allignement		*/
	} Rt_Generic_Ql_Spot;

	typedef struct	{
	    double
		HRV1_Pan[4],	/* Pan Radiometric Correction Coefficient	*/ 
		HRV1_Xs1[4], 	/* Xs1 Radiometric Correction Coefficient	*/
		HRV1_Xs2[4], 	/* Xs2 Radiometric Correction Coefficient	*/
		HRV1_Xs3[4], 	/* Xs3 Radiometric Correction Coefficient	*/

		HRV2_Pan[4],	/* Pan Radiometric Correction Coefficient	*/ 
		HRV2_Xs1[4], 	/* Xs1 Radiometric Correction Coefficient	*/
		HRV2_Xs2[4], 	/* Xs2 Radiometric Correction Coefficient	*/
		HRV2_Xs3[4]; 	/* Xs3 Radiometric Correction Coefficient	*/
	
	}Radiometric_Coefficients;
	
	typedef struct	{
	    int
		Seconds_of_Day;	/* Used to fill segments on Moving Window	*/
#ifndef sgi
        /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
        char    __linux_padding[4] ;     
#endif
        double
		Lat_0,		/* Latitude of the west most point of the scan	*/
		Long_0,		/* Longitude of the west most point of the scan	*/
		Lat_1,		/* Latitude of the aest most point of the scan	*/
		Long_1;		/* Longitude of the west most point of the scan	*/
	} Track_Spot4_Descriptor;


#endif /* Spot_4_Structures__ */
