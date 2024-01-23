/*
 * Copyright 1998, Advanced Computer Systems, Inc.
 * Via Paolo Frisi, 7a Roma
 *
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Advanced Computer Systems, Inc.
 *
 * Author: B. Versini and V.D. Spaventa 
 *
 */
/*------------------------------------------------------------------------------------------------------*/
/*Purpose:	Defines the lsPCDProcess class.								*/
/*													*/
/*------------------------------------------------------------------------------------------------------*/

/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _lsCPFTypes_h_
#define _lsCPFTypes_h_

/* +--------------------------------------------------------------------------------------------------------+ */	 
/* !   ERROR DEFINITION																				    ! */
/* +--------------------------------------------------------------------------------------------------------+ */

#define CPF_STATUS_OFFSET          -900
#define CPF_MAX_STATUS             -950

typedef enum {
CPF_OKAY						= 0,
CPF_BADINPUT					= (CPF_STATUS_OFFSET - 1),
CPF_CANNOT_OPEN					= (CPF_STATUS_OFFSET - 2),
CPF_BADSTATUS					= (CPF_STATUS_OFFSET - 3),
CPF_XXX_ERROR					= (CPF_STATUS_OFFSET - 4),
CPF_DATA_FILE_NOT_EXIST			= (CPF_STATUS_OFFSET - 5),
CPF_CANNOT_COPY_CPF_TO_WO		= (CPF_STATUS_OFFSET - 6),
CPF_CPF_DIRECTORY_NOT_EXISTS	= (CPF_STATUS_OFFSET - 7),
CPF_CPF_NOT_EXISTS				= (CPF_STATUS_OFFSET - 7),
CPF_BAD_MALLOC					= (CPF_STATUS_OFFSET - 8),
CPF_CANNOT_CREATE_ISTANCE		= (CPF_STATUS_OFFSET - 9),
CPF_CANNOT_FOUND_CPF			= (CPF_STATUS_OFFSET - 10), 
CPF_CANNOT_GET_ENV_VARIABLE		= (CPF_STATUS_OFFSET - 11), 
CPF_FLAG_GET_WRONG				= (CPF_STATUS_OFFSET - 12), 
CPF_MISSION_NUMBER_WRONG		= (CPF_STATUS_OFFSET - 13) 

} CPF_STATUS_TYPE;

/* +--------------------------------------------------------------------------------------------------------+ */	 
/* !   DEFINE																							    ! */
/* +--------------------------------------------------------------------------------------------------------+ */
#define	NUM_BAND					9
#define	NUM_OF_POLINOMYAL_COEFF		6
#define	SCAN_DIRECTION				2
#define MAX_NUM_DETECTOR		   32
#define L4_MISSION_NUM				4
#define L5_MISSION_NUM	   			5
#define L7_MISSION_NUM	   			7
#define MAX_NUM_ME_FW 30

/* +--------------------------------------------------------------------------------------------------------+ */	 
/* !   STRUCT DEFINITION																				    ! */
/* +--------------------------------------------------------------------------------------------------------+ */
//basic structure for polynomial coefficients
typedef struct {
	int FHSERR_Forward;
	int FHSERR_Reverse;
	int SHSERR_Forward;
	int SHSERR_Reverse;
} BUMPER_ERR;
//basic structure for polynomial coefficients
typedef struct {
	double coeff[NUM_OF_POLINOMYAL_COEFF];
} POLINOMYAL;

//mirror parameters basic struct
typedef struct {
	POLINOMYAL	Along[SCAN_DIRECTION];	//departure from linearity of along scan mirror motion
	POLINOMYAL	Cross[SCAN_DIRECTION];			//departure from linearity of across scan mirror motion
	double		Asm[SCAN_DIRECTION];			//angle from start of scan to mid-scan point (micro rad)
	double		Ame[SCAN_DIRECTION];			//angle from mid-scan to end of scan (micro rad)
}	ANGLES_SME;

//mirror parameters data for each electronic (SME1-SME2) and operational modes(SAM-Bump)
typedef struct {
	ANGLES_SME	Angles[maxElectronicType];	
	double		factor_conversion;	//fh and sh scan mirror error measurement units (sec)
} MIRROR_PARAMETERS;

// SLC rotates the ETM+ line of sight about the sensor Y axis to compensate for the 
// spacecraft 's along track motion during the scanning interval.
typedef struct {
 double Angular_Velocity_1; 			// angular velocity of primary scan corrector (rad/sec)
 double Angular_Velocity_2;				// angular velocity of secondary scan corrector (rad/sec)
 POLINOMYAL Corrector_Motion_1;			// motion of primary   scan line corrector 
 POLINOMYAL Corrector_Motion_2;			// motion of secondary scan line corrector 
} SCAN_LINE_CORRECTOR;

//nominal data for scan time
typedef struct {
	double Scan_time; 					//(microsec)
	double First_Half_Time[SCAN_DIRECTION];		//(microsec)
	double Second_Half_Time[SCAN_DIRECTION];	//(microsec)
} TIME_PARAMETERS;

//scanner parameters
typedef struct {
	unsigned int	Lines_Per_Scan_B123457; // bands 1-5 7	( 30m ->16)
	unsigned int	Lines_Per_Scan_B6; 		// band 6 		(60m ->8 120m ->4)
	unsigned int	Lines_Per_Scan_B8;  	// band 8 		(15m->32)
	int				Scans_Per_Scene;		// 				(375)
	double			Swath_Angle;			// (rad)
	double			Scan_Rate;				// angular scan velocity (rad/sec)
	double			Dwell_Time_B123457; 	// detector sample time; bands 1-5 7  (microsec)
	double			Dwell_Time_B6;		 	// band 6		(microsec)
	double			Dwell_Time_B8;		 	// band 8 		(microsec)
	int				IC_Line_Length_B123457;
	int				IC_Line_Length_B6;
	int				IC_Line_Length_B8;
	int				Scan_Line_Length_B123457;	//  nominal number of detector samples (6330)
	int				Scan_Line_Length_B6;		// 									 (3165)
	int				Scan_Line_Length_B8;	    //  								(12660)
	double			Filter_Frequency_B123457;	//	(kHz)
	double			Filter_Frequency_B6;	//	(kHz)
	double			Filter_Frequency_B8;	//	(kHz)
	double			IFOV_B1234;				//  (microrad)
	double			IFOV_B57_along_scan;	//  (microrad)
	double			IFOV_B57_across_scan;	//  (microrad)
	double			IFOV_B6;				//  (microrad)
	double			IFOV_B8_along_scan;		//  (microrad)
	double			IFOV_B8_across_scan;	//  (microrad)
	double			Scan_Period;			//  forward+reverse (millisec)
	double			Scan_Frequency;			//  scans/seconds   (Hz)
	double			Active_Scan_Time;		//  (microsec)
	double			Turn_Around_Time;		//  (microsec)
} SCANNER_PARAMETERS;

typedef struct {
 	double Along_Scan_Band_Offsets [NUM_BAND];							//(microrad) band offset
    double Across_Scan_Band_Offsets[NUM_BAND];							//(microrad)
    double Focal_Plane_Offsets[SCAN_DIRECTION][NUM_BAND];				//(IFOV)

	double Along_Scan_DO [NUM_BAND][SCAN_DIRECTION][MAX_NUM_DETECTOR];	//(IFOV) detector offset
    double Across_Scan_DO[NUM_BAND][SCAN_DIRECTION][MAX_NUM_DETECTOR];  //(IFOV)

 	double Even_Detector_Shift[SCAN_DIRECTION][NUM_BAND];				//(IFOV) 
    double Odd_Detector_Shift [SCAN_DIRECTION][NUM_BAND];				//(IFOV)
	
} FOCAL_PLANE_PARAMETERS;

typedef struct {
 	double Gyro_To_Attitude_Matrix[9];
  	double ADSA_To_ETM_Matrix[9];
  	double Attitude_To_ETM_Matrix[9];
  	double Spacecraft_Roll_Bias;				// (rad)
  	double Spacecraft_Pitch_Bias;				// (rad)
  	double Spacecraft_Yaw_Bias; 				// (rad)
} ATTITUDE_PARAMETERS;

typedef struct {
	double	Fn;
	double	Zeta;
	double	Tau;
	double	P;
	double	Ak;
} IMU;

typedef struct {
	double	ADS_num[18];
	double	ADS_den[18];
	double	ADS_num_temp[18];
	double	ADS_den_temp[18];
} ADS;

typedef struct {
	double	ADSPre_W[5];
	double	ADSPre_H[5];
	double	ADSPre_T[5];
} PREFIL;

typedef struct {
	IMU		IMUData;
	ADS		ADSData;
	PREFIL	PrefilterData;
} TRANSFER_FUNCTION;

typedef struct {
	int UT1_Year[180];
	char UT1_Month[180][4];
	int UT1_Day[180];
	int UT1_Modified_Julian[180];
	double UT1_X[180];
	double UT1_Y[180];
	double UT1_UTC[180];
} UT1_TIME_PARAMETERS;

typedef struct {
		int	Forward_Bias_Location_30;
		int	Reverse_Bias_Location_30;
		int	Forward_Bias_Location_60;
		int	Reverse_Bias_Location_60;
		int	Forward_Bias_Location_15;
		int	Reverse_Bias_Location_15;
} BIAS_LOCATION;
	
typedef struct {
	double Magnitude[NUM_BAND][MAX_NUM_DETECTOR];
	double Scal_Factor[NUM_BAND][MAX_NUM_DETECTOR];
	double Time_Constant[NUM_BAND][MAX_NUM_DETECTOR];
	int Filter_Widths[MAX_NUM_ME_FW];
	double ME_Weight[NUM_BAND][MAX_NUM_DETECTOR][MAX_NUM_ME_FW];
	double ME_WeightCal[NUM_BAND][MAX_NUM_DETECTOR][MAX_NUM_ME_FW];
} MEMORY_EFFECT;

typedef struct {
	int Reference_Detector[3][3];
	float State_Mask_Parameters[5];
	double Magnitude[NUM_BAND][MAX_NUM_DETECTOR];
} SCAN_CORRELATED_SHIFT;

/* +--------------------------------------------------------------------------------------------------------+ */	 
/* !   End of code redefinition control.								    ! */
/* +--------------------------------------------------------------------------------------------------------+ */

#endif

